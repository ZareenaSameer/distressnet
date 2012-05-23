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


dtn::api::Client* _client = NULL;
ibrcommon::tcpclient* _conn = NULL;
bool run = true;

void term( int signal )
{
	std::cout << "caught signal" << std::endl;
	if ( signal >= 1 )
	{
		if ( _client != NULL )
		{
			std::cout << "exiting..." << std::endl;
			run = false;
		}
	}
}

int main()
{
	std::cout << "btag faker server" << std::endl;

	signal( SIGINT, term );
	signal( SIGTERM, term );

	string file_source = "btagfaker";
	_conn = new ibrcommon::tcpclient;
	_conn->open( "127.0.0.1", 4550 );
	_client = new dtn::api::Client( file_source, *_conn, dtn::api::Client::MODE_SENDONLY );
	std::cout << "Created dtn client" << std::endl;
	_client->connect();

	std::cout << "Initialized" << std::endl;

	struct timeval now;

	char hostname[1024];
	hostname[1023] = '\0';
	gethostname( hostname, 1023 );

	int size = 1000000; //20kb
	int sleepu = 20;
// 	char filler[size];
// 	for (int i=0; i < size; i++)
// 		filler[i] = 'r';

	cout << "sending " << size << "B bundles every " << sleepu << " secs " << endl;
	while ( run == true )
	{
		ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
		stringstream data;

		btag_t btag;

		btag.mote_id = 99;
		strcpy( btag.address, "fake1" );
		strcpy( btag.task_force, "fake2" );

		btag.living = 99;
		btag.dead = 99;

		::gettimeofday( &now, 0 );
// 		data << "btag: ";
		data << "created " << ( now.tv_sec - 946684800 ) << " ";
		data << "at " << hostname << " ";
		data << "uniqid " << hostname << " ,,, " << ( now.tv_sec - 946684800 ) << ",,," << "btagfaker" << " ";
		data << "data: ";
		std::cout << "sending this: " << data.str() << endl;

		string datas = data.str();
		datas.resize( size, 'r' );
		( *ref.iostream() ) << datas;


// 		data << btag.mote_id << "---";
// 		data << btag.address_line_1 << "---";
// 		data << btag.address_line_2 << "---";
// 		data << btag.task_force << "---";
// 		data << btag.living << "---";
// 		data << btag.dead << "---";
// 		data << (int) btag.search_complete << "---";

// 		data << filler;



// 		(*ref.iostream()) << data.str();

		string file_destination = "dtn://router1.dtn/basestation";
		unsigned int lifetime = 1000;
		int priority = 2;
		dtn::api::BLOBBundle b( file_destination, ref );
		b.setLifetime( lifetime );
		*_client << b;
		std::cout << "sending to router1 " << endl;
		_client->flush();

// 		string file_destination1 = "dtn://router7.dtn/basestation";
// 		unsigned int lifetime1 = 1000;
// 		int priority1 = 2;
// 		dtn::api::BLOBBundle b1(file_destination1, ref);
// 		b1.setLifetime(lifetime1);
// 		*_client << b1;
// 		std::cout << "sending to router7 " << endl;
// 		_client->flush();

		sleep( sleepu );
	}

	_client->close();
	_conn->close();
	return 0;
}
