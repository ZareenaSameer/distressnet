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
// receiver.cpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "boost/bind.hpp"

const short multicast_port = 7010;

class receiver
{
public:
	receiver( boost::asio::io_service& io_service,
	          const boost::asio::ip::address& listen_address,
	          const boost::asio::ip::address& multicast_address )
		: socket_( io_service )
	{
		// Create the socket so that multiple may be bound to the same address.
		boost::asio::ip::udp::endpoint listen_endpoint(
		    listen_address, multicast_port );
		socket_.open( listen_endpoint.protocol() );
		socket_.set_option( boost::asio::ip::udp::socket::reuse_address( true ) );
		socket_.bind( listen_endpoint );

		// Join the multicast group.
		socket_.set_option(
		    boost::asio::ip::multicast::join_group( multicast_address ) );

		socket_.async_receive_from(
		    boost::asio::buffer( data_, max_length ), sender_endpoint_,
		    boost::bind( &receiver::handle_receive_from, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );
	}

	void handle_receive_from( const boost::system::error_code& error,
	                          size_t bytes_recvd )
	{
		if ( !error )
		{
			std::cout.write( data_, bytes_recvd );
			std::cout << std::endl;

			socket_.async_receive_from(
			    boost::asio::buffer( data_, max_length ), sender_endpoint_,
			    boost::bind( &receiver::handle_receive_from, this,
			                 boost::asio::placeholders::error,
			                 boost::asio::placeholders::bytes_transferred ) );
		}
	}

private:
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
	enum { max_length = 1024 };
	char data_[max_length];
};

int main( int argc, char* argv[] )
{
	try
	{
		if ( argc != 3 )
		{
			std::cerr << "Usage: receiver <listen_address> <multicast_address>\n";
			std::cerr << "  For IPv4, try:\n";
			std::cerr << "    receiver 0.0.0.0 239.255.0.1\n";
			std::cerr << "  For IPv6, try:\n";
			std::cerr << "    receiver 0::0 ff31::8000:1234\n";
			return 1;
		}

		boost::asio::io_service io_service;
		receiver r( io_service,
		            boost::asio::ip::address::from_string( argv[1] ),
		            boost::asio::ip::address::from_string( argv[2] ) );
		io_service.run();
	}
	catch ( std::exception& e )
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}