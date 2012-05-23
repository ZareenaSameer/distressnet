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
//
// async_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class server
{
public:
	server( boost::asio::io_service& io_service, short port )
		: io_service_( io_service ),
		  socket_( io_service, udp::endpoint( udp::v4(), port ) )
	{
		socket_.async_receive_from(
		    boost::asio::buffer( data_, max_length ), sender_endpoint_,
		    boost::bind( &server::handle_receive_from, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );
	}

	void handle_receive_from( const boost::system::error_code& error,
	                          size_t bytes_recvd )
	{
		if ( !error && bytes_recvd > 0 )
		{
			socket_.async_send_to(
			    boost::asio::buffer( data_, bytes_recvd ), sender_endpoint_,
			    boost::bind( &server::handle_send_to, this,
			                 boost::asio::placeholders::error,
			                 boost::asio::placeholders::bytes_transferred ) );
		}
		else
		{
			socket_.async_receive_from(
			    boost::asio::buffer( data_, max_length ), sender_endpoint_,
			    boost::bind( &server::handle_receive_from, this,
			                 boost::asio::placeholders::error,
			                 boost::asio::placeholders::bytes_transferred ) );
		}
	}

	void handle_send_to( const boost::system::error_code& error, size_t bytes_sent )
	{
		socket_.async_receive_from(
		    boost::asio::buffer( data_, max_length ), sender_endpoint_,
		    boost::bind( &server::handle_receive_from, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );
	}

private:
	boost::asio::io_service& io_service_;
	udp::socket socket_;
	udp::endpoint sender_endpoint_;
	enum { max_length = 1024 };
	char data_[max_length];
};

int main( int argc, char* argv[] )
{
	try
	{
		if ( argc != 2 )
		{
			std::cerr << "Usage: async_udp_echo_server <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		using namespace std; // For atoi.
		server s( io_service, atoi( argv[1] ) );

		io_service.run();
	}
	catch ( std::exception& e )
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}