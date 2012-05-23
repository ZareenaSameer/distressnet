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
#include <boost/date_time/posix_time/posix_time.hpp>

#include "../DistressNet.h"

using boost::asio::ip::udp;

class udp_server
{
public:
	udp_server( boost::asio::io_service& io_service, int port ) :
		v4socket_( io_service, udp::endpoint( udp::v4(), port ) )
	{

		std::cout << "Initialized" << std::endl;
		start_v4receive();

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

	void handle_receive_v4( const boost::system::error_code& error, std::size_t bytez )
	{
		std::cout << "recvd " << bytez << " on v4 side" << std::endl;
		if ( !error || error == boost::asio::error::message_size )
		{
			std::cout << "recvd " << bytez << " on v4 side" << std::endl;


			start_v4receive();
		}

	}



	udp::socket v4socket_;
	udp::endpoint v4remote_endpoint_;
	boost::array<unsigned char, 1024> v4recv_buffer_;
};





int main( int argc, char** argv )
{
	std::cout << "starting server" << std::endl;
	std::cout << "listening on port " << atoi( argv[1] ) << std::endl;
	boost::asio::io_service io_service;
	udp_server myserver( io_service, atoi( argv[1] ) );
	io_service.run();





	return 0;
}
