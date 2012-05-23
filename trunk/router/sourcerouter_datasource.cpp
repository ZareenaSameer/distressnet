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
	std::cout << "this is for node 7 only: data source for source routing" << std::endl;

	signal( SIGINT, term );
	signal( SIGTERM, term );

	string file_source = "sourceroutesender";
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

	//	timed data generation:
	//	first 15 mins: generate for no waypoints
	//	next: waypoint1, waypoint2, both waypoints


	//	create 300kb bundles every 30 seconds (else can run out of memory), use sourcerouting
	//	source route format: <numhops><hop1><hop2><hop3>... should be the first few ascii chars in payload

	int tot_size = 300000;

	//	no waypoints: generate 20 bundles (10 mins)
	//	flow 1->2 and flow 1->3
	//	routing string: 1-v3-2=8-7 and 1-v2-v1-3 = 3-2-1
	for ( int i = 0; i < 20; i++ )
	{

		ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
		stringstream data;
		::gettimeofday( &now, 0 );
		data << "17 | ";
		data << "c:" << ( now.tv_sec - 946684800 );
		data << " | at:" << hostname;
		data << " | seq:" << setw ( 3 ) << i << " " << 20;
		data << " | size:" << tot_size;
		data << " | sroute:8-7";
		data << " | with:security ";
		data << " | wpconfig:0";

		string datas = data.str();
		cout << "payload: " << datas << endl;

		datas.resize( tot_size, '-' );
		( *ref.iostream() ) << datas;

		string file_destination = "dtn://router8.dtn/sourcerouter";
		cout << "desti: " << file_destination << endl;

		unsigned int lifetime = 1000;
		int priority = 2;
		dtn::api::BLOBBundle b( file_destination, ref );
		b.setLifetime( lifetime );
		b.requestSigned();
		b.requestEncryption();
		*_client << b;
		_client->flush();
//  		cout << datas << endl;
		cout.flush();


		ibrcommon::BLOB::Reference ref1 = ibrcommon::BLOB::create();
		stringstream data1;
		::gettimeofday( &now, 0 );
		data1 << "221 | ";
		data1 << "c:" << ( now.tv_sec - 946684800 );
		data1 << " | at:" << hostname;
		data1 << " | seq:" << setw ( 3 ) << i << " " << 20;
		data1 << " | size:" << tot_size;
		data1 << " | sroute:8-2-1";
		data1 << " | with:security ";
		data1 << " | wpconfig:0";

		string datas1 = data1.str();
		cout << "payload: " << datas1 << endl;


		datas1.resize( tot_size, '-' );
		( *ref1.iostream() ) << datas1;

		file_destination = "dtn://router3.dtn/sourcerouter";
		cout << "desti: " << file_destination << endl;
		dtn::api::BLOBBundle b1( file_destination, ref1 );
		b1.setLifetime( lifetime );
		b1.requestSigned();
		b1.requestEncryption();
		*_client << b1;
		_client->flush();
//  		cout << datas << endl;
		cout.flush();

		sleep( 30 );
	}

	//	waypoint 4 only : generate 20 bundles (10 mins)
	//	routing string: 1-v3-4-v1-3 = 8-4-2-1
	for ( int i = 0; i < 20; i++ )
	{

		ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
		stringstream data;

		::gettimeofday( &now, 0 );

		data << "3421 | ";

		data << "c:" << ( now.tv_sec - 946684800 );
		data << " | at:" << hostname;
		data << " | seq:" << setw ( 3 ) << i << " " << 20;
		data << " | size:" << tot_size;
		data << " | sroute:8-4-2-1";
		data << " | with:security ";
		data << " | wpconfig:1";

		string datas = data.str();
		cout << "payload: " << datas << endl;
		datas.resize( tot_size, '-' );
		( *ref.iostream() ) << datas;

		string file_destination = "dtn://router8.dtn/sourcerouter";
		cout << "desti: " << file_destination << endl;
		unsigned int lifetime = 1000;
		int priority = 2;
		dtn::api::BLOBBundle b( file_destination, ref );
		b.setLifetime( lifetime );
		b.requestSigned();
		b.requestEncryption();
		*_client << b;
		_client->flush();
//  		cout << datas << endl;
		cout.flush();


		ibrcommon::BLOB::Reference ref1 = ibrcommon::BLOB::create();
		stringstream data1;
		::gettimeofday( &now, 0 );
		data1 << "17 | ";
		data1 << "c:" << ( now.tv_sec - 946684800 );
		data1 << " | at:" << hostname;
		data1 << " | seq:" << setw ( 3 ) << i << " " << 20;
		data1 << " | size:" << tot_size;
		data1 << " | sroute:8-7";
		data1 << " | with:security ";
		data1 << " | wpconfig:1";

		string datas1 = data1.str();
		cout << "payload: " << datas1 << endl;
		datas1.resize( tot_size, '-' );
		( *ref1.iostream() ) << datas1;

		file_destination = "dtn://router8.dtn/sourcerouter";
		cout << "desti: " << file_destination << endl;
		dtn::api::BLOBBundle b1( file_destination, ref1 );
		b1.setLifetime( lifetime );
		b1.requestSigned();
		b1.requestEncryption();
		*_client << b1;
		_client->flush();
//  		cout << datas << endl;
		cout.flush();

		sleep( 30 );
	}

	//	waypoint 5 only : generate 20 bundles (10 mins)
	//	routing string: 1-v2-5-v1-3 = 6(me)-3-5-2-1
	for ( int i = 0; i < 20; i++ )
	{

		ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
		stringstream data;

		::gettimeofday( &now, 0 );

		data << "3521 | ";

		data << "c:" << ( now.tv_sec - 946684800 );
		data << " | at:" << hostname;
		data << " | seq:" << setw ( 3 ) << i << " " << 20;
		data << " | size:" << tot_size;
		data << " | sroute:3-5-2-1";
		data << " | with:security ";
		data << " | wpconfig:2";

		string datas = data.str();
		cout << "payload: " << datas << endl;
		datas.resize( tot_size, '-' );
		( *ref.iostream() ) << datas;

		string file_destination = "dtn://router3.dtn/sourcerouter";
		cout << "desti: " << file_destination << endl;
		unsigned int lifetime = 1000;
		int priority = 2;
		dtn::api::BLOBBundle b( file_destination, ref );
		b.setLifetime( lifetime );
		b.requestSigned();
		b.requestEncryption();
		*_client << b;
		_client->flush();
//  		cout << datas << endl;
		cout.flush();


		ibrcommon::BLOB::Reference ref1 = ibrcommon::BLOB::create();
		stringstream data1;
		::gettimeofday( &now, 0 );
		data1 << "17 | ";
		data1 << "c:" << ( now.tv_sec - 946684800 );
		data1 << " | at:" << hostname;
		data1 << " | seq:" << setw ( 3 ) << i << " " << 20;
		data1 << " | size:" << tot_size;
		data1 << " | sroute:8-7";
		data1 << " | with:security ";
		data1 << " | wpconfig:2";


		string datas1 = data1.str();
		cout << "payload: " << datas1 << endl;
		datas1.resize( tot_size, '-' );
		( *ref1.iostream() ) << datas1;

		file_destination = "dtn://router8.dtn/sourcerouter";
		cout << "desti: " << file_destination << endl;
		dtn::api::BLOBBundle b1( file_destination, ref1 );
		b1.setLifetime( lifetime );
		b1.requestSigned();
		b1.requestEncryption();
		*_client << b1;
		_client->flush();
//  		cout << datas << endl;
		cout.flush();


		sleep( 30 );
	}

	//	2 waypoints: generate 20 bundles (10 mins)
	//	routing string: 1-v2-5-v1-3 = 6(me)-3-5-2-1
	for ( int i = 0; i < 20; i++ )
	{

		ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();
		stringstream data;

		::gettimeofday( &now, 0 );

		data << "3521 | ";
		data << "c:" << ( now.tv_sec - 946684800 );
		data << " | at:" << hostname;
		data << " | seq:" << setw ( 3 ) << i << " " << 20;
		data << " | size:" << tot_size;
		data << " | sroute:3-5-2-1";
		data << " | with:security ";
		data << " | wpconfig:3";

		string datas = data.str();
		cout << "payload: " << datas << endl;
		datas.resize( tot_size, '-' );
		( *ref.iostream() ) << datas;

		string file_destination = "dtn://router3.dtn/sourcerouter";
		cout << "desti: " << file_destination << endl;
		unsigned int lifetime = 1000;
		int priority = 2;
		dtn::api::BLOBBundle b( file_destination, ref );
		b.setLifetime( lifetime );
		b.requestSigned();
		b.requestEncryption();
		*_client << b;
		_client->flush();
//  		cout << datas << endl;
		cout.flush();

		ibrcommon::BLOB::Reference ref1 = ibrcommon::BLOB::create();
		stringstream data1;
		::gettimeofday( &now, 0 );
		data1 << "17 | ";
		data1 << "c:" << ( now.tv_sec - 946684800 );
		data1 << " | at:" << hostname;
		data1 << " | seq:" << setw ( 3 ) << i << " " << 20;
		data1 << " | size:" << tot_size;
		data1 << " | sroute:8-7";
		data1 << " | with:security ";
		data1 << " | wpconfig:3";

		string datas1 = data1.str();
		cout << "payload: " << datas1 << endl;
		datas1.resize( tot_size, '-' );
		( *ref1.iostream() ) << datas1;

		file_destination = "dtn://router8.dtn/sourcerouter";
		cout << "desti: " << file_destination << endl;
		dtn::api::BLOBBundle b1( file_destination, ref1 );
		b1.setLifetime( lifetime );
		b1.requestSigned();
		b1.requestEncryption();
		*_client << b1;
		_client->flush();
//  		cout << datas << endl;
		cout.flush();

		sleep( 30 );
	}

	cout << endl;
//	_client->flush();
	_client->close();
	_conn->close();
	return 0;
}
