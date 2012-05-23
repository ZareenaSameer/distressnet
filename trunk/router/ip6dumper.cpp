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

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


//#include "../DistressNet.h"

using boost::asio::ip::udp;
using namespace std;

typedef struct
{
	uint16_t mote_id;           //  2  Mote id
//	char address_line_1[30];    // 30  This array size needs to be optimized for the packet size
//	char address_line_2[30];    // 30  This array size needs to be optimized for the packet size
//	char task_force[10];        // 10  ID of group that searched building
// 	char date_entered[7];       //  7  Date format ddmmyy
// 	char time_entered[5];       //  5  Date format hhmm
// 	char date_exited[7];        //  7  Date format ddmmyy
// 	char time_exited[5];        //  5  Date format hhmm
// 	char hazards[30];           // 30  This array size needs to be optimized for the packet size
//	uint16_t living;            //  2  Number of people alive in the house
//	uint16_t dead;              //  4  Number of people dead in the house
//	uint8_t  search_complete;   //  1  Was the search completed?
// 	char comment[100];          //100  Freeform comments, really added because we have to write 256b anyway

	uint16_t curr_sink;
	uint16_t myscore;
	uint16_t sink_score;
	uint16_t qsize;
	uint16_t iter;
	uint16_t bcnt;
	uint16_t ccnt;
	uint16_t volt;




} __attribute__ ( ( packed ) ) btag_t;


class udp_server
{
public:
	udp_server( boost::asio::io_service& io_service, int port ) :
		listenerSocket_( io_service, udp::endpoint( udp::v6(), port ) )
	{
		std::cout << "Initialized on port " << port << std::endl;

		start_receive_moteside();

	}

	~udp_server()
	{
		cout<< "Signal caught..." << endl;
	}


private:

	void start_receive_moteside()
	{
		listenerSocket_.async_receive_from(
		    boost::asio::buffer( recv_buffer_moteside_ ),
		    moteside_endpoint_,
		    boost::bind( &udp_server::handle_receive_moteside, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );

	}

	void handle_receive_moteside( const boost::system::error_code& error, std::size_t bytez )
	{
		printf( "recvd %ld moteside\n", bytez );
		if ( !error || error == boost::asio::error::message_size )
		{
// 			btag_t temper;
// 			memcpy(&temper, &recv_buffer_moteside_, sizeof(btag_t));

// 			cout << "frm:" << temper.mote_id << endl;

			for ( int i = 0; i < bytez; i+=2 )
			{
				uint16_t temp = 0;
				temp = recv_buffer_moteside_[i+1];
				temp |= temp << 8 ;
				temp |= recv_buffer_moteside_[i];

				printf( "%02d ", temp );
			}
			cout << endl;

			for ( int i = 0; i < ( bytez/sizeof( btag_t ) ); i++ )
			{
				btag_t temper;
				memcpy( &temper, &recv_buffer_moteside_[i*sizeof( btag_t )], sizeof( btag_t ) );

				cout <<
				     "frm " << temper.mote_id << " " <<
				     "snk " << temper.curr_sink << " " <<
				     "sns " << temper.sink_score << " " <<

				     "itr " << temper.iter << " " <<
				     "scr " << temper.myscore << " " <<

				     "bcn " << temper.bcnt << " " <<
				     "cat " << temper.ccnt << " " <<
				     "bat " << temper.volt << " " <<

				     endl;

			}

			start_receive_moteside();
		}

	}

	udp::endpoint moteside_endpoint_;
	boost::array<unsigned char, 1024> recv_buffer_moteside_;

	//	multicast listener socket
	udp::socket listenerSocket_;
};

int main( int argc, char** argv )
{
	std::cout << "starting dumper" << std::endl;

	boost::asio::io_service io_service;
	int port;

	stringstream temp;
	temp << argv[1];
	temp >> port;

	udp_server myserver( io_service, port );
	io_service.run();

	return 0;
}
