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

#include "../DistressNet.h"

using boost::asio::ip::udp;

class udp_server
{
public:
	udp_server( boost::asio::io_service& io_service ) :
		socket_( io_service, udp::endpoint( udp::v6(), 5676 ) )
	{
		//	DTN stuff
		string file_source = "motegw";

		dtnapiconn_.open( "127.0.0.1", 4550 );
		dtnclient_ = new dtn::api::Client( file_source, dtnapiconn_, dtn::api::Client::MODE_SENDONLY );
		std::cout << "Created dtn client" << std::endl;
		dtnclient_->connect();

		start_receive();
	}

	~udp_server()
	{
		cout<< "Signal caught..." << endl;
	}


private:

	void start_receive()
	{
		socket_.async_receive_from(
		    boost::asio::buffer( recv_buffer_, 1024 ),
		    remote_endpoint_,
		    boost::bind( &udp_server::handle_receive, this,
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

		std::cout << "recvd " << bytez << " bytes" << endl;

		if ( !error || error == boost::asio::error::message_size )
		{

// 			for (int i = 0; i < bytez; i++)
// 				printf("%x ", recv_buffer_.at(i));
// 			std::cout << std::endl;


			//	now send the data over dtn also
			//	-----------------------------------------------
			ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
			std::ostringstream ss;
			ss.write( recv_buffer_, bytez );

			std::cout << "recvd this: " << ss.str().c_str() << endl;

			( *ref.iostream() ) << ss.str().c_str();

			string file_destination = "dtn://router1.dtn/basestation";
			unsigned int lifetime = 500;
			dtn::api::BLOBBundle b( file_destination, ref );
			b.setLifetime( lifetime );

			// send the bundle
			*dtnclient_ << b;
			dtnclient_->flush();

			start_receive();
		}
	}


	udp::socket socket_;	//	to listen on ipv6 for mote
	udp::endpoint remote_endpoint_;
	udp::endpoint moteside_endpoint_;
	char recv_buffer_[1024];

	dtn::api::Client* dtnclient_;
	ibrcommon::tcpclient dtnapiconn_;

};

int main()
{
	std::cout << "starting server" << std::endl;

	boost::asio::io_service io_service;
	udp_server myserver( io_service );
	io_service.run();

	return 0;
}
