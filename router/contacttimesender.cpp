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

int main( int argc, char** argv )
{
// 	std::cout << "btag faker server" << std::endl;

	signal( SIGINT, term );
	signal( SIGTERM, term );

	if ( argc < 5 )
	{
		cout << "usage: <#bundles> <size> <veh speed> <run #> opt<security>" << endl;
		return 0;
	}

	string file_source = "contacttimesender";
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

	int numcopies;
	int tot_size;
	int speed;
	int runnum;

	numcopies = atoi( argv[1] );
	tot_size = atoi( argv[2] );
	speed = atoi( argv[3] );
	runnum = atoi( argv[4] );

	bool enable_security = false;
	if ( argc > 5 )
	{
		std::string arg = argv[5];
		if ( arg == "s" )
		{
			enable_security = true;
			cout << "enabling security..." << endl;
		}
	}

	cout << "creating " << numcopies << " copies of " << tot_size << " B each" << endl;

	for ( int i = 0; i < numcopies; i++ )
	{

		ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
		stringstream data;

		::gettimeofday( &now, 0 );
		data << "c:" << ( now.tv_sec - 946684800 );
		data << " | seq:" << setw ( 3 ) << i << " " << numcopies;
		data << " | size:" << tot_size;
		data << " | speed:" << speed;
		data << " | run:" << runnum;

		if ( enable_security )
		{
			data << " | with:security ";
		}

		string datas = data.str();
		datas.resize( tot_size, '-' );
		( *ref.iostream() ) << datas;

		string file_destination = "dtn://router1.dtn/contacttimereceiver";
		unsigned int lifetime = 1000;
		int priority = 2;

		dtn::api::BLOBBundle b( file_destination, ref );
		b.setLifetime( lifetime );

		if ( enable_security )
		{
			b.requestSigned();
			b.requestEncryption();
		}

		*_client << b;
		_client->flush();
		cout << ".";
		cout.flush();

		sleep( 0.1 );
	}
	cout << endl;

//	_client->flush();
	_client->close();
	_conn->close();
	return 0;
}
