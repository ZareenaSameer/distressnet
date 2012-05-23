/*
 * Copyright (c) 2007-2020 Texas A&M University System
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holders nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <boost/asio.hpp>
#include <string>

#include <ibrdtn/api/Client.h>
#include <ibrdtn/api/FileBundle.h>
#include <ibrdtn/api/BLOBBundle.h>
#include <ibrcommon/net/tcpclient.h>
#include <ibrcommon/thread/Mutex.h>
#include <ibrcommon/thread/MutexLock.h>
#include <ibrcommon/data/BLOB.h>
#include <iostream>


dtn::api::Client* _client = NULL;
ibrcommon::tcpclient* _conn = NULL;
bool run = true;

void term( int signal )
{
	std::cout << "caught signal" << std::endl;
	if ( signal >= 1 )
	{
		if ( _client != NULL )
		{
			std::cout << "exiting..." << std::endl;
			run = false;
		}
	}
}




class SimpleSerial
{
public:
	/**
	 * Constructor.
	 * \param port device name, example "/dev/ttyUSB0" or "COM4"
	 * \param baud_rate communication speed, example 9600 or 115200
	 * \throws boost::system::system_error if cannot open the
	 * serial device
	 */
	SimpleSerial( std::string port, unsigned int baud_rate )
		: io(), serial( io,port )
	{
		serial.set_option( boost::asio::serial_port_base::baud_rate( baud_rate ) );
	}

	/**
	 * Write a string to the serial device.
	 * \param s string to write
	 * \throws boost::system::system_error on failure
	 */
	void writeString( std::string s )
	{
		boost::asio::write( serial,boost::asio::buffer( s.c_str(),s.size() ) );
	}

	/**
	 * Blocks until a line is received from the serial device.
	 * Eventual '\n' or '\r\n' characters at the end of the string are removed.
	 * \return a string containing the received line
	 * \throws boost::system::system_error on failure
	 */
	std::string readLine()
	{
		//Reading data char by char, code is optimized for simplicity, not speed
		using namespace boost;
		char c;
		std::string result;
		for( ;; )
		{
			asio::read( serial,asio::buffer( &c,1 ) );
			switch( c )
			{
			case '\r':
				break;
			case '\n':
				return result;
			default:
				result+=c;
			}
		}
	}

private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
};


int main( int argc, char* argv[] )
{

	int curr_waypoint_config = 0;
	int num_pkts_sent = 0;

	string file_source = "motesourceroutesender";
	_conn = new ibrcommon::tcpclient;
	_conn->open( "127.0.0.1", 4550 );
	_client = new dtn::api::Client( file_source, *_conn, dtn::api::Client::MODE_SENDONLY );
	std::cout << "Created dtn client" << std::endl;
	_client->connect();

	struct timeval now;

	char hostname[1024];
	hostname[1023] = '\0';
	gethostname( hostname, 1023 );

	int tot_size = 200000;

	string sourceroutes[4];
	sourceroutes[0] = "221"; // no waypoints
	sourceroutes[1] = "3421";
	sourceroutes[2] = "3521"; //1-v2-5-v1-3
	sourceroutes[3] = "3421";


	try {

		SimpleSerial serial( argv[1],115200 );

		while ( 1 )
		{

			string text = serial.readLine();
			size_t found = text.find( "recv data" );
			if ( found == string::npos )
				continue;

			cout << text << endl;

			//now send this stuff over dtn
			ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
			stringstream data;
			::gettimeofday( &now, 0 );
			data << sourceroutes[curr_waypoint_config];
			data << " c:" << ( now.tv_sec - 946684800 );
			data << " | at:" << hostname;
			data << " | seq:" << num_pkts_sent << " " << 20;
			data << " | size:" << 90;
			data << " | sroute-with-lth:8" << sourceroutes[curr_waypoint_config];
			data << " | with:security";
			data << " | wpconfig:" << curr_waypoint_config;
			data << " | motepld:" << text;

			string datas = data.str();
			cout << "payload: " << datas << endl;

			datas.resize( tot_size, '-' );
			( *ref.iostream() ) << datas;

			string file_destination = "dtn://router8.dtn/sourcerouter";
			cout << "desti: " << file_destination << endl;


			unsigned int lifetime = 1000;
			int priority = 2;
			dtn::api::BLOBBundle b( file_destination, ref );
			b.setLifetime( lifetime );
			b.requestSigned();
			b.requestEncryption();
			*_client << b;
			_client->flush();

			num_pkts_sent++;
			if ( num_pkts_sent > 19 )
			{
				num_pkts_sent = 0;
				curr_waypoint_config = ( curr_waypoint_config + 1 ) % 4;
			}

			sleep( 1 );
		}

	} catch( boost::system::system_error& e )
	{
		cout<<"Error: "<<e.what()<<endl;
		return 1;
	}
}