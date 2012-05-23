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


#include "../DistressNet.h"

using boost::asio::ip::udp;

class udp_server
{
public:
	udp_server( boost::asio::io_service& io_service ) :
		v6socket_( io_service, udp::endpoint( udp::v6(), 9999 ) ),
		v4socket_( io_service, udp::endpoint( udp::v4(), 7010 ) )
	{

		boost::asio::ip::address multicast_address = boost::asio::ip::address::from_string( "225.0.11.5" );
		boost::asio::ip::multicast::join_group option( multicast_address );
		v4socket_.set_option( option );

		std::cout << "Joined multicast group" << std::endl;
		std::cout << "Initialized" << std::endl;

		start_v4receive();
		start_v6receive();
	}

	~udp_server()
	{

	}


private:

	void start_v4receive()
	{
		v4socket_.async_receive_from(
		    boost::asio::buffer( v4recv_buffer_ ),
		    v4remote_endpoint_,
		    boost::bind( &udp_server::handle_receive_v4, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );
	}
	void start_v6receive()
	{
		v6socket_.async_receive_from(
		    boost::asio::buffer( v6recv_buffer_ ),
		    v6remote_endpoint_,
		    boost::bind( &udp_server::handle_receive_v6, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );

	}

	void handle_receive_v4( const boost::system::error_code& error,
	                        std::size_t bytez )
	{

		if ( !error || error == boost::asio::error::message_size )
		{
			std::cout << "recvd " << bytez << " on v4 side" << std::endl;
			uint16_t moteid;
			memcpy( &moteid, &v4recv_buffer_, sizeof( uint16_t ) );
			std::string temper;
			std::stringstream ss;
			ss << "fec0::" << ntohs( moteid );

			v6socket_.async_send_to( boost::asio::buffer( v4recv_buffer_, bytez ),
			                         udp::endpoint( boost::asio::ip::address_v6::from_string( ss.str().c_str() ), 7001 ),
			                         boost::bind( &udp_server::handle_send_v6,
			                                      this,
			                                      boost::asio::placeholders::error,
			                                      boost::asio::placeholders::bytes_transferred )
			                       );


			start_v4receive();
		}
	}

	void handle_receive_v6( const boost::system::error_code& error, std::size_t bytez )
	{
		if ( !error || error == boost::asio::error::message_size )
		{
			std::cout << "recvd " << bytez << " on v6 side" << std::endl;
			v4socket_.async_send_to( boost::asio::buffer( v6recv_buffer_, bytez ),
			                         udp::endpoint( boost::asio::ip::address_v4::from_string( "225.0.11.5" ), 7004 ),
			                         boost::bind( &udp_server::handle_send_v4,
			                                      this,
			                                      boost::asio::placeholders::error,
			                                      boost::asio::placeholders::bytes_transferred )
			                       );

			start_v6receive();
		}

	}


	void handle_send_v6( const boost::system::error_code& /*error*/, std::size_t bytez )
	{
		std::cout << "sent " << bytez << " to v6 side" << std::endl;
	}

	void handle_send_v4( const boost::system::error_code& /*error*/, std::size_t bytez )
	{
		std::cout << "sent " << bytez << " to v4 side" << std::endl;
	}

	udp::socket v4socket_, v6socket_;
	udp::endpoint v4remote_endpoint_, v6remote_endpoint_;
	boost::array<unsigned char, 1024> v4recv_buffer_;
	boost::array<unsigned char, 1024> v6recv_buffer_;
};

int main()
{
	std::cout << "starting server" << std::endl;

	boost::asio::io_service io_service;
	udp_server myserver( io_service );
	io_service.run();

	return 0;
}
