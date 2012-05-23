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
#include <stdlib.h>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <ibrdtn/api/Client.h>
#include <ibrdtn/api/FileBundle.h>
#include <ibrdtn/api/BLOBBundle.h>
#include <ibrcommon/net/tcpclient.h>
#include <ibrcommon/thread/Mutex.h>
#include <ibrcommon/thread/MutexLock.h>
#include <ibrcommon/data/BLOB.h>
#include <ibrcommon/Logger.h>

#include "../DistressNet.h"
#include "dn-utils.h"

#define NUM_USERS 4
#define NUM_ROUTERS 10

using namespace boost::filesystem;

dtn::api::Client *_client = NULL;
ibrcommon::tcpclient *_conn = NULL;



class fogbox_server
{
public:
	fogbox_server( boost::asio::io_service& io_service ) :
		t( io_service, boost::posix_time::seconds( 5 ) )
	{

		DN_DEBUG_MACRO << "Initialized" << std::endl;

		string file_source = "fogbox";
		dtnapiconn_.open( "127.0.0.1", 4550 );
		dtnapiconn_.enableNoDelay();	//	for recving shit

		dtnclient_ = new dtn::api::Client( file_source, dtnapiconn_ );
		DN_DEBUG_MACRO << "Created dtn client" << std::endl;
		dtnclient_->connect();

		_conn = &dtnapiconn_;
		_client = dtnclient_;
		t.async_wait( boost::bind( &fogbox_server::fogsync, this ) );
	}

	void fogsync()
	{
		DN_DEBUG_MACRO << "timer fired" << std::endl;

		//	check for any recvd bundles

		DN_DEBUG_MACRO << "started recv bundles" << std::endl;

		dtn::api::Bundle recvdb;

label:
		bool recvd = true;
		try
		{
			recvdb = dtnclient_->getBundle( 5 );
		}
		catch ( const dtn::api::ConnectionTimeoutException& )
		{
			DN_DEBUG_MACRO << "Timeout, didn't recv any bundles" << std::endl;
			recvd = false;
		}

		if ( recvd )
		{
			DN_DEBUG_MACRO << "got a bundle!" << std::endl;
			ibrcommon::BLOB::Reference ref = recvdb.getData();

			fstream tmpfile;
			tmpfile.open( "/root/rsync/media/tmpfile", ios::in|ios::out|ios::binary|ios::trunc );
			tmpfile.exceptions( std::ios::badbit | std::ios::eofbit );

			tmpfile << ref.iostream()->rdbuf();
			DN_DEBUG_MACRO << "wrote to temp file" << endl;

			tmpfile.seekg ( 0, ios::beg );

			char c[256];
			tmpfile.get( c,256 );
			ref.iostream()->get( c,256 );

			stringstream filename;

			int counter = 0;
			while ( c[counter] != '|' )
				filename << c[counter++];

			DN_DEBUG_MACRO << "filename is " << filename.str() << endl;

			if ( boost::filesystem::exists( filename.str() ) )
			{
				DN_DEBUG_MACRO << "file exists, not overwriting" << endl;
				goto here;
			}

			fstream actualfile;
			actualfile.open( filename.str().c_str(), ios::in|ios::out|ios::binary|ios::trunc );
			actualfile.exceptions( std::ios::badbit | std::ios::eofbit );

			tmpfile.seekg ( 0, ios::beg );
			tmpfile.seekg ( counter+3 );
			actualfile << tmpfile.rdbuf();

			DN_DEBUG_MACRO << "wrote to real file " << endl;

			tmpfile.close();
			actualfile.close();


			//	update lmt
			std::time_t t = boost::filesystem::last_write_time( filename.str() );
			lmt_map[filename.str()] = t;

			DN_DEBUG_MACRO << "size is " << boost::filesystem::file_size( filename.str() ) << " updated lmt is " << std::ctime( &t );

			goto label;	// see if any moar bundles
		}

here:
		//	send stuff now
		string file_destination;

		unsigned int lifetime = 600;
		int priority = 1;

		for ( int user_id = 1; user_id <= NUM_USERS; user_id++ )
		{
			std::stringstream dirpath;
			dirpath << "/root/rsync/media/USER" << user_id << "/";
			DN_DEBUG_MACRO << "directory is " << dirpath.str() << std::endl;

			path dir_path( dirpath.str() );

			directory_iterator end_itr; // default construction yields past-the-end
			for ( directory_iterator itr( dir_path ); itr != end_itr; ++itr )
			{
				//	ignore directories
				if ( is_directory( itr->status() ) )
				{
					continue;
				}

				std::string fqfilename = itr->string();

				std::time_t t = boost::filesystem::last_write_time( fqfilename ) ;

				DN_DEBUG_MACRO << "file " << fqfilename << " size " << boost::filesystem::file_size( fqfilename ) << " lmt " << std::ctime( &t );

				DN_DEBUG_MACRO << "extension is " << itr->path().extension() << endl;

				if ( ! ( boost::algorithm::ends_with( itr->leaf(), ".jpg" ) || boost::algorithm::ends_with( itr->leaf(), ".JPG" ) ) )
				{
					DN_DEBUG_MACRO << "ignoring based on extension" << endl;
					continue;
				}

				ibrcommon::BLOB::Reference ref = ibrcommon::BLOB::create();

				if ( lmt_map.find( fqfilename ) != lmt_map.end() )
				{
					DN_DEBUG_MACRO << "lmt in database is " << std::ctime( &lmt_map[fqfilename] );

					//	this is an old file
					if ( t <= lmt_map[fqfilename] )
					{
						DN_DEBUG_MACRO << "not sending since old file " << endl;
						continue;
					}

				}

				//	file came from rsync, so it is new
				lmt_map[fqfilename] = t;

				DN_DEBUG_MACRO << "sending file " << fqfilename << " to ";
				std::ifstream filer( fqfilename.c_str() );
				( *ref.iostream() ) << fqfilename << "|||" << filer.rdbuf() ;
				( *ref.iostream() ).flush();

				filer.close();


				file_destination = "dtn://router";
				dtn::api::BLOBBundle b( file_destination, ref );
				b.setPriority( dtn::api::Bundle::BUNDLE_PRIORITY( priority ) );
				b.setLifetime( lifetime );

				for ( int k = 1; k <= NUM_ROUTERS; k++ )
				{
					if ( k == dn_utils::gethostnumber() )
						continue;

					cout << k << " ";

					stringstream temper;
					temper << file_destination << k << ".dtn/fogbox";

					b.setDestination( temper.str() );

					// send the bundle
					*dtnclient_ << b;
					dtnclient_->flush();
					sleep( 1 );
				}// each destination
				cout << endl;
			}// each file
		}// each user

		DN_DEBUG_MACRO << "restarting timer..." << endl;

		t.expires_at( t.expires_at() + boost::posix_time::seconds( 30 ) );
		t.async_wait( boost::bind( &fogbox_server::fogsync, this ) );
	}

	~fogbox_server()
	{

	}


private:
	boost::asio::deadline_timer t;
	dtn::api::Client* dtnclient_;
	ibrcommon::tcpclient dtnapiconn_;
	std::map<std::string, std::time_t> lmt_map;


};


void term( int signal )
{
	DN_DEBUG_MACRO << "caught signal, will stfu now" << endl;

	if ( signal >= 1 )
	{
		if ( _client != NULL )
		{
			_client->close();
			_conn->close();
		}
	}
}

int main()
{
	DN_DEBUG_MACRO << "starting fogbox server" << std::endl;

	system( "mkdir -p /root/rsync/media/USER1/" );
	system( "mkdir -p /root/rsync/media/USER2/" );
	system( "mkdir -p /root/rsync/media/USER3/" );
	system( "mkdir -p /root/rsync/media/USER4/" );

	// catch process signals
	signal( SIGINT, term );
	signal( SIGTERM, term );


	boost::asio::io_service io_service;
	fogbox_server myserver( io_service );
	io_service.run();


	return 0;
}









