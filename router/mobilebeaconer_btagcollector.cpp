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
#include "dn-utils.h"

using boost::asio::ip::udp;

class udp_server
{
public:
	udp_server( boost::asio::io_service& io_service ) :
		wlan1sock( io_service, udp::endpoint( boost::asio::ip::address_v4::from_string( dn_utils::getwlan1ip().c_str() ), 7010 ) ),
		ppp0sock( io_service, udp::endpoint( udp::v6(), 5679 ) ),
		t( io_service, boost::posix_time::seconds( 2 ) )
	{

		DN_DEBUG_MACRO << "Initialized" << std::endl;

		start_ppp0();
		start_wlan1();

	}

	~udp_server()
	{

	}



private:



	void
	start_wlan1()
	{
		wlan1sock.async_receive_from(
		    boost::asio::buffer( wlan1sock_buffer ),
		    wlan1sock_remote_endpoint,
		    boost::bind( &udp_server::recvd_wlan1, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );

	}



	void
	start_ppp0()
	{
		ppp0sock.async_receive_from(
		    boost::asio::buffer( ppp0_recv_buf ),
		    v6remote_endpoint_,
		    boost::bind( &udp_server::recvd_ppp0, this,
		                 boost::asio::placeholders::error,
		                 boost::asio::placeholders::bytes_transferred ) );

	}



	void
	recvd_ppp0( const boost::system::error_code& error, std::size_t bytez )
	{
		DN_DEBUG_MACRO << "recvd " << bytez << " on ppp0" << std::endl;
		dn_utils::print_raw_bytes( ppp0_recv_buf, bytez );

		if ( !error || error == boost::asio::error::message_size )
		{
			//	incoming data is btagloc

			btagloc_t btagloc;
			memcpy( &btagloc, &ppp0_recv_buf, bytez );

			DN_DEBUG_MACRO << "junkloc btagloc is ";
			dn_utils::print_btagloc( btagloc );
			printf( "\r\n" );


			for ( int i = 1; i <= 10; i++ )
			{
				std::stringstream temper;
				temper << "192.168.50." << i;

				wlan1sock.async_send_to(
				    boost::asio::buffer( &btagloc, sizeof( btagloc_t ) ),
				    udp::endpoint( boost::asio::ip::address_v4::from_string( temper.str() ), 7011 ),
				    boost::bind( &udp_server::sent_wlan1, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
				);
			}

			start_ppp0();
		}

	}

	void sent_wlan1( const boost::system::error_code& ec, std::size_t bytez )
	{
		DN_DEBUG_MACRO << "sent " << bytez << " on wlan1 error code " << ec.value() << " " << ec.message() << std::endl;
	}

	void
	recvd_wlan1( const boost::system::error_code& error, std::size_t bytez )
	{
		DN_DEBUG_MACRO << "recvd " << bytez << " on wlan1" << std::endl;
		dn_utils::print_raw_bytes( wlan1sock_buffer, bytez );

		if ( !error || error == boost::asio::error::message_size )
		{
			//	incoming data is btagloc, send as btagloc
			btagloc_t btagloc;
			memcpy( &btagloc, &wlan1sock_buffer, bytez );

			DN_DEBUG_MACRO << "btagloc is ";
			dn_utils::print_btagloc( btagloc );
			printf( "\r\n" );


			ppp0sock.async_send_to(
			    boost::asio::buffer( &btagloc, sizeof( btagloc_t ) ),
			    udp::endpoint( boost::asio::ip::address_v6::from_string( dn_utils::getpppmoteip() ), PORT_BTAG_PROGRAMMER ),
			    boost::bind( &udp_server::handle_send_ppp0, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
			);

			start_wlan1();
		}

	}

	void handle_send_ppp0( const boost::system::error_code& ec, std::size_t bytez )
	{
		DN_DEBUG_MACRO << "sent " << bytez << " on ppp0 error code " << ec.value() << " " << ec.message() << std::endl;
	}

	udp::socket ppp0sock, wlan1sock;
	udp::endpoint v6remote_endpoint_, wlan1sock_remote_endpoint;
	boost::array<unsigned char, 1024> ppp0_recv_buf, wlan1sock_buffer;
	boost::asio::io_service io_service;
	boost::asio::deadline_timer t;
};





int main()
{
	DN_DEBUG_MACRO << "starting server" << std::endl;
	DN_DEBUG_MACRO << "mote other end is " << dn_utils::getpppmoteip() << std::endl;

	boost::asio::io_service io_service;
	udp_server myserver( io_service );
	io_service.run();





	return 0;
}
