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
#include "ibrdtn/api/Client.h"
#include "ibrdtn/api/FileBundle.h"
#include "ibrcommon/net/tcpclient.h"
#include "ibrcommon/thread/Mutex.h"
#include "ibrcommon/thread/MutexLock.h"
#include <ibrcommon/Logger.h>

#include <csignal>
#include <sys/types.h>
#include <sys/time.h>
#include <iostream>

void print_help()
{
	cout << "-- dtnrecv (IBR-DTN) --" << endl;
	cout << "Syntax: dtnrecv [options]"  << endl;
	cout << "* optional parameters *" << endl;
	cout << " -h|--help            display this text" << endl;
	cout << " --file <filename>    write the incoming data to the a file instead of the standard output" << endl;
	cout << " --name <name>        set the application name (e.g. filetransfer)" << endl;
	cout << " --timeout <seconds>  receive timeout in seconds" << endl;
	cout << " --count <number>     receive that many bundles" << endl;
	cout << " -U <socket>     use UNIX domain sockets" << endl;
}

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
	string name = "basestation";
	int timeout = 0;
	int count   = 1;

//	ibrcommon::Logger::setVerbosity(99);
//	ibrcommon::Logger::addStream(std::cout, ibrcommon::Logger::LOGGER_ALL, ibrcommon::Logger::LOG_DATETIME | ibrcommon::Logger::LOG_LEVEL);

	try {
		// Create a stream to the server using TCP.
		ibrcommon::tcpclient conn;

		// connect to the standard local api port
		conn.open( "127.0.0.1", 4550 );

		// enable nodelay option
		conn.enableNoDelay();

		// Initiate a client for synchronous receiving
		dtn::api::Client client( name, conn );

		// export objects for the signal handler
		_conn = &conn;
		_client = &client;

		// Connect to the server. Actually, this function initiate the
		// stream protocol by starting the thread and sending the contact header.
		client.connect();

// 		std::fstream file;

// 		if (!_stdout)
// 		{
// 			std::cout << "Wait for incoming bundle... " << std::endl;
// 			file.open(filename.c_str(), ios::in|ios::out|ios::binary|ios::trunc);
// 			file.exceptions(std::ios::badbit | std::ios::eofbit);
// 		}

		while( run == true )
		{
			// receive the bundle
			dtn::api::Bundle b = client.getBundle( timeout );

			// get the reference to the blob
			ibrcommon::BLOB::Reference ref = b.getData();
			struct timeval now;
			::gettimeofday( &now, 0 );

			char c[256];
			ref.iostream()->get( c,256 );

// 			cout << (now.tv_sec - 946684800) << ": " << ref.iostream()->rdbuf() << endl;
			cout << ( now.tv_sec - 946684800 ) << " : " << c << endl;

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
