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

#include <ibrdtn/api/Client.h>
#include <ibrdtn/api/FileBundle.h>
#include <ibrdtn/api/BLOBBundle.h>
#include <ibrcommon/net/tcpclient.h>
#include <ibrcommon/thread/Mutex.h>
#include <ibrcommon/thread/MutexLock.h>
#include <ibrcommon/data/BLOB.h>
#include <ibrcommon/Logger.h>

#include "../DistressNet.h"

using boost::asio::ip::udp;

class udp_server
{
public:
	udp_server( boost::asio::io_service& io_service ) :
		socket_( io_service, udp::endpoint( udp::v6(), PORT_BLDG_TAG_SEND ) ),
		listenerSocket_( io_service, udp::endpoint( udp::v4(), PORT_BLDG_TAG_LISTEN ) )
	{
		multicast_endpoint_ = udp::endpoint( boost::asio::ip::address::from_string( "225.0.11.5" ), 7010 );

		//	DTN stuff
		string file_source = "motegw";

		dtnapiconn_.open( "127.0.0.1", 4550 );
		dtnclient_ = new dtn::api::Client( file_source, dtnapiconn_, dtn::api::Client::MODE_SENDONLY );
		std::cout << "Created dtn client" << std::endl;
		dtnclient_->connect();

		boost::asio::ip::address multicast_address = boost::asio::ip::address::from_string( "225.0.11.5" );
		boost::asio::ip::multicast::join_group option( multicast_address );
		listenerSocket_.set_option( option );
		std::cout << "Joined multicast group" << std::endl;

		std::cout << "Initialized" << std::endl;


		start_receive();
		start_receive_moteside();
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
	void start_receive_moteside()
	{
		listenerSocket_.async_receive_from(
		    boost::asio::buffer( recv_buffer_moteside_ ),
		    moteside_endpoint_,
		    boost::bind( &udp_server::handle_receive_moteside, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );

	}

	void handle_receive( const boost::system::error_code& error,
	                     std::size_t bytez )
	{
		/*
		 * 	ANYTHING RECEIVED ON FEC0::64 IS NOT COMPLIANT WITH IP
		 * 	IT IS LITTLE ENDIAN, BUT SHOULD BE BIG ENDIAN
		 *
		 *
		 */
		struct timeval now;
		::gettimeofday( &now, 0 );
		char hostname[1024];
		hostname[1023] = '\0';
		gethostname( hostname, 1023 );

		std::cout << "recvd " << bytez << " stuff" << endl;

		if ( !error || error == boost::asio::error::message_size )
		{

// 			for (int i = 0; i < bytez; i++)
// 				printf("%x ", recv_buffer_.at(i));
// 			std::cout << std::endl;

// 			struct simp_tag t1;
			btag_t btag;

			memcpy( &btag, &recv_buffer_, sizeof( btag_t ) );

			//DANGER: the below code is not needed, since i shall use htons on the mote
			//DANGER: always convert tinyos bytes to network convention
// 			t1.mote_id = SwapTwoBytes(t1.mote_id);
// 			t1.loc = swapf(t1.loc);
// 			printf("making tinyos bytes big endian\n");
			//DANGER ENDS

			btag.mote_id = ntohs( btag.mote_id );
			btag.living = ntohs( btag.living );
			btag.dead = ntohs( btag.dead );

			//	now send the data over dtn also
			//	-----------------------------------------------
			ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
			stringstream data;

			data << "btag: ";
			data << "recvd:" << ( now.tv_sec - 946684800 ) << " ";
			data << "at:" << hostname << " ";
			data << "data:";
			data << btag.mote_id << "---";
			data << btag.address_line_1 << "---";
			data << btag.address_line_2 << "---";
			data << btag.task_force << "---";
			data << btag.living << "---";
			data << btag.dead << "---";
			data << ( int ) btag.search_complete << "---";

			std::cout << "recvd this: " << data.str() << endl;

			( *ref.iostream() ) << data.str();

			string file_destination = "dtn://router1.dtn/basestation";
			unsigned int lifetime = 9999;
			int priority = 1;


			dtn::api::BLOBBundle b( file_destination, ref );
			b.setLifetime( lifetime );
			b.setPriority( dtn::api::Bundle::BUNDLE_PRIORITY( priority ) );

			// send the bundle
			*dtnclient_ << b;
			dtnclient_->flush();


			//	also to storage nodes
			string file_destination2 = "dtn://storagenode1.dtn/storageclient";
			dtn::api::BLOBBundle b1( file_destination2, ref );
			b1.setLifetime( lifetime );
			b1.setPriority( dtn::api::Bundle::BUNDLE_PRIORITY( priority ) );
			*dtnclient_ << b1;
			dtnclient_->flush();

			string file_destination3 = "dtn://storagenode2.dtn/storageclient";
			dtn::api::BLOBBundle b2( file_destination3, ref );
			b2.setLifetime( lifetime );
			b2.setPriority( dtn::api::Bundle::BUNDLE_PRIORITY( priority ) );
			*dtnclient_ << b2;
			dtnclient_->flush();

			//	-----------------------------------------------


//  			printf("memcpying %ld bytes\n", sizeof(btag_t));
// 			printf("id  : %d\n", btag.mote_id);
// 			printf("add1: %s\n", btag.address_line_1);
// 			printf("add2: %s\n", btag.address_line_2);
// 			printf("tfor: %s\n", btag.task_force);
// 			printf("livi: %d\n", btag.living);
// 			printf("dead: %d\n", btag.dead);
// 			printf("scom: %d\n", btag.search_complete);


			//	ALWAYS CONVERT TO NETWORK BYTE ORDER
			btag.mote_id = htons( btag.mote_id );
			btag.living = htons( btag.living );
			btag.dead = htons( btag.dead );


			/*			printf("AFTER NtoHS stuff\n");
						printf("uint16_t data: 0x%x, size is %ld\n", t1.mote_id, sizeof(uint16_t));
						printf("float    data: %f, size is %ld\n", t1.loc, sizeof(float));
						printf("string   data: %s, size is %ld\n", t1.street, sizeof(char));*/

			listenerSocket_.async_send_to( boost::asio::buffer( &btag, sizeof( btag_t ) ),
			                               multicast_endpoint_,
			                               boost::bind( &udp_server::handle_send,
			                                       this,
			                                       boost::asio::placeholders::error,
			                                       boost::asio::placeholders::bytes_transferred )
			                             );

// 			std::cout << std::endl;
			start_receive();
		}
	}

	void handle_send( const boost::system::error_code& /*error*/, std::size_t bytez )
	{
		printf( "sent %ld stuff\n", bytez );
	}

	void handle_send_moteside( const boost::system::error_code& /*error*/, std::size_t bytez )
	{
		printf( "sent %ld stuff moteside\n", bytez );
	}

	void handle_receive_moteside( const boost::system::error_code& error, std::size_t bytez )
	{
		printf( "recvd %ld moteside\n", bytez );
		if ( !error || error == boost::asio::error::message_size )
		{

			btag_t btag;
			memcpy( &btag, &recv_buffer_moteside_, sizeof( btag_t ) );

			stringstream temp;
			temp << "fec0::" << hex << ntohs( btag.mote_id );
			std::cout << "sending to " << temp.str() << std::endl;

			socket_.async_send_to( boost::asio::buffer( &btag, sizeof( btag_t ) ),
			                       udp::endpoint( boost::asio::ip::address_v6::from_string( temp.str() ), PORT_BLDG_TAG_LISTEN ),
			                       boost::bind( &udp_server::handle_send_moteside,
			                                    this,
			                                    boost::asio::placeholders::error,
			                                    boost::asio::placeholders::bytes_transferred )
			                     );

			printf( "sending moteside\n" );
			start_receive_moteside();
		}

	}

	udp::socket socket_;
	udp::endpoint remote_endpoint_, multicast_endpoint_, moteside_endpoint_;
	boost::array<unsigned char, 1024> recv_buffer_;
	boost::array<unsigned char, 1024> recv_buffer_moteside_;
	boost::array<unsigned char, 1024> send_buffer_;
	dtn::api::Client* dtnclient_;
	ibrcommon::tcpclient dtnapiconn_;

	//	multicast listener socket
	udp::socket listenerSocket_;
};

int main()
{
	std::cout << "starting server" << std::endl;

	boost::asio::io_service io_service;
	udp_server myserver( io_service );
	io_service.run();

// 	// Wait for signal indicating time to shut down.
// 	sigset_t set;
// 	int sig;
// 	int *sigptr = &sig;
// 	int ret_val;
// 	sigemptyset(&set);
// 	sigaddset(&set, SIGINT);
// 	sigprocmask( SIG_BLOCK, &set, NULL );
//
// 	printf("Waiting for a SIGINT signal\n");
//
// 	ret_val = sigwait(&set,sigptr);
// 	if(ret_val == -1)
// 		perror("sigwait failed\n");
// 	else {
// 		if(*sigptr == 2)
// 		{
// 			printf("SIGINT was received\n");
// 			std::cout << "stopping server" << std::endl;
// 			io_service.stop();
// 		}
// 		else
// 			printf("sigwait returned with sig: %d\n", *sigptr);
// 	}



	return 0;
}
