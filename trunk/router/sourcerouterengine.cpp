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
#include <ibrcommon/Logger.h>
#include <ibrdtn/api/Client.h>
#include <ibrdtn/api/FileBundle.h>
#include <ibrdtn/api/BLOBBundle.h>
#include <ibrcommon/net/tcpclient.h>
#include <ibrcommon/thread/Mutex.h>
#include <ibrcommon/thread/MutexLock.h>
#include <ibrcommon/data/BLOB.h>
#include <ibrcommon/Logger.h>

#include <csignal>
#include <sys/types.h>
#include <sys/time.h>
#include <iostream>

dtn::api::Client *_client = NULL;
ibrcommon::tcpclient *_conn = NULL;

int h = 0;
bool _stdout = true;
bool run = true;

void term( int signal )
{
	run = false;
	if ( !_stdout )
	{
		std::cout << h << " bundles received." << std::endl;
	}

	if ( signal >= 1 )
	{
		if ( _client != NULL )
		{
			_client->close();
			_conn->close();
			exit( 0 );
		}
	}
}

int main( int argc, char *argv[] )
{
	// catch process signals
	signal( SIGINT, term );
	signal( SIGTERM, term );

	int ret = EXIT_SUCCESS;
	string filename = "";
	string name = "sourcerouter";
	int timeout = 0;
	int count   = 1;

	char hostname[1024];
	hostname[1023] = '\0';
	gethostname( hostname, 1023 );


	try {
		ibrcommon::tcpclient conn;
		conn.open( "127.0.0.1", 4550 );
		conn.enableNoDelay();
		dtn::api::Client client( name, conn );
		_conn = &conn;
		_client = &client;
		client.connect();

		while( run == true )
		{
			// receive the bundle
			dtn::api::Bundle b = client.getBundle( timeout );

			// get the reference to the blob
			ibrcommon::BLOB::Reference ref = b.getData();
			struct timeval now;
			::gettimeofday( &now, 0 );

			stringstream tss;
			tss << ref.iostream()->rdbuf();

			string payloadstr = tss.str();
			cout << ( now.tv_sec - 946684800 ) << " payload is " << payloadstr.substr( 0,256 ) << endl;
			int numhops = payloadstr.at( 0 ) - '0';
			cout << "numhops is " << numhops << " full stamp is " << payloadstr.substr( 0, numhops+1 ) << endl;

			int nexthop = 999;
			for ( int i=0; i < numhops; i++ )
			{
				if ( payloadstr.at( i+1 ) == 'x' )
					continue;

				nexthop = payloadstr.at( i+1 ) - '0';
				payloadstr.at( i+1 ) = 'x';
				break;
			}

			if ( nexthop == 999 )
				nexthop = hostname[6]-'0';

			cout << "nexthop is " << nexthop << " new stamp is " << payloadstr.substr( 0, numhops+1 ) << endl;
			cout << "new payload is " << payloadstr.substr( 0,256 ) << endl;

			// now send it to the damn next hop
			ibrcommon::BLOB::Reference ref1 = ibrcommon::BLOB::create();
			stringstream data;
			::gettimeofday( &now, 0 );
			data << payloadstr;
			string datas = data.str();
			( *ref1.iostream() ) << datas;

			stringstream tempss;
			if ( hostname[6]-'0' == nexthop )
				tempss << "dtn://router" << nexthop << ".dtn/basestation";
			else
				tempss << "dtn://router" << nexthop << ".dtn/sourcerouter";

			string file_destination = tempss.str();
			cout << "new desti is " << file_destination << endl;

			unsigned int lifetime = 1000;
			int priority = 2;

			dtn::api::BLOBBundle b1( file_destination, ref1 );
			b1.setLifetime( lifetime );
			b1.requestSigned();
			b1.requestEncryption();
			client << b1;
			client.flush();



// 			cout << (now.tv_sec - 946684800) << ": " << ref.iostream()->rdbuf() << endl;
// 			cout << (now.tv_sec - 946684800) << " : " << c << endl;

		}

		// Shutdown the client connection.
		client.close();

		// close the tcp connection
		conn.close();
	} catch ( const dtn::api::ConnectionTimeoutException& ) {
		std::cerr << "Timeout." << std::endl;
		ret = EXIT_FAILURE;
	} catch ( const dtn::api::ConnectionAbortedException& ) {
		std::cerr << "Aborted." << std::endl;
		ret = EXIT_FAILURE;
	} catch ( const dtn::api::ConnectionException& ) {
	} catch ( const std::exception &ex ) {
		std::cerr << "Error: " << ex.what() << std::endl;
		ret = EXIT_FAILURE;
	}

	return ret;
}
