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
#include <ctime>
#include <iostream>
#include <string>
#include <signal.h>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include <ibrdtn/api/Client.h>
#include <ibrdtn/api/FileBundle.h>
#include <ibrdtn/api/BLOBBundle.h>
#include <ibrcommon/net/tcpclient.h>
#include <ibrcommon/thread/Mutex.h>
#include <ibrcommon/thread/MutexLock.h>
#include <ibrcommon/data/BLOB.h>
#include <ibrcommon/Logger.h>


// #include "../DistressNet.h"
typedef struct
{
	uint8_t to_id;
	char tweet[100];
} __attribute ( ( packed ) ) tweet_t;

using boost::asio::ip::udp;

class udp_server
{
public:
	udp_server( boost::asio::io_service& io_service ) :
		socket_( io_service, udp::endpoint( udp::v4(), 7010 ) )
	{

		//	DTN stuff
		string file_source = "dtntwittergw";

		dtnapiconn_.open( "127.0.0.1", 4550 );
		dtnclient_ = new dtn::api::Client( file_source, dtnapiconn_, dtn::api::Client::MODE_SENDONLY );
		std::cout << "Created dtn client" << std::endl;
		dtnclient_->connect();
		std::cout << "Connected dtn client" << std::endl;



		// Join the multicast group.
		boost::asio::ip::address multicast_address = boost::asio::ip::address::from_string( "225.0.11.5" );
		boost::asio::ip::multicast::join_group option( multicast_address );
		socket_.set_option( option );

		start_receive();
	}

	~udp_server()
	{
		cout<< "Signal caught..." << endl;
// 		dtnclient_->close();
// 		dtnapiconn_.close();
	}


private:

	void start_receive()
	{
		socket_.async_receive_from(
		    boost::asio::buffer( recv_buffer_ ),
		    remote_endpoint_,
		    boost::bind( &udp_server::handle_receive, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );
	}


	void handle_receive( const boost::system::error_code& error,
	                     std::size_t bytez )
	{
		struct timeval now;
		::gettimeofday( &now, 0 );
		char hostname[1024];
		hostname[1023] = '\0';
		gethostname( hostname, 1023 );


		printf( "recvd %ld stuff\n", bytez );
		if ( !error || error == boost::asio::error::message_size )
		{

			for ( int i = 0; i < bytez; i++ )
				printf( "%x ", recv_buffer_.at( i ) );
			std::cout << std::endl;

			tweet_t twet;

			memcpy( &twet, &recv_buffer_, sizeof( tweet_t ) );

			//	now send the data over dtn also
			//	-----------------------------------------------
			ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
			stringstream data;

			data << twet.tweet;
			printf( "sending tweet %s\n", data.str().c_str() );
			( *ref.iostream() ) << data.str().c_str();

			string file_destination = "dtn://router1.dtn/twitterproxy";
			unsigned int lifetime = 100;
			dtn::api::BLOBBundle b( file_destination, ref );
			b.setLifetime( lifetime );

			// send the bundle
			*dtnclient_ << b;
			dtnclient_->flush();

// 			std::cout << std::endl;
			start_receive();
		}
	}

	void handle_send( const boost::system::error_code& /*error*/, std::size_t bytez )
	{
		printf( "sent %ld stuff\n", bytez );
	}


	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	boost::array<unsigned char, 1024> recv_buffer_;
	dtn::api::Client* dtnclient_;
	ibrcommon::tcpclient dtnapiconn_;
};

int main()
{
	std::cout << "starting dtntwittergw server" << std::endl;

	boost::asio::io_service io_service;
	udp_server myserver( io_service );
	io_service.run();


	return 0;
}
