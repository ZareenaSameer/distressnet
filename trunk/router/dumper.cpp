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
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
	using namespace std;
	// For time_t, time and ctime;
	time_t now = time( 0 );
	return ctime( &now );
}

class udp_server
{
public:
	udp_server( boost::asio::io_service& io_service ) :
		socket_( io_service, udp::endpoint( udp::v4(), 4561 ) )
	{
		boost::asio::ip::address multicast_address = boost::asio::ip::address::from_string(
		            "224.0.12.5" );
		boost::asio::ip::multicast::join_group option( multicast_address );
		socket_.set_option( option );
		start_receive();
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
		printf( "recvd %ld stuff\n", bytez );
		if ( !error || error == boost::asio::error::message_size )
		{

			for ( int i = 0; i < bytez; i++ )
				printf( "%x ", recv_buffer_.at( i ) );

			std::cout << std::endl;
			//       socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
			//           boost::bind(&udp_server::handle_send, this, message,
			//             boost::asio::placeholders::error,
			//             boost::asio::placeholders::bytes_transferred));

			start_receive();
		}
	}

	void handle_send( boost::shared_ptr<std::string> /*message*/,
	                  const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/ )
	{
	}

	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	boost::array<char, 1024> recv_buffer_;
};

int main()
{
	std::cout << "startin" << std::endl;
	try
	{
		boost::asio::io_service io_service;
		udp_server server( io_service );
		io_service.run();
	} catch ( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
