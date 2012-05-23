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
#include <iostream>
#include <boost/asio.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;



class SimpleSerial
{
public:
	/**
	 * Constructor.
	 * \param port device name, example "/dev/ttyUSB0" or "COM4"
	 * \param baud_rate communication speed, example 9600 or 115200
	 * \throws boost::system::system_error if cannot open the
	 * serial device
	 */
	SimpleSerial( std::string port, unsigned int baud_rate )
		: io(), serial( io,port )
	{
		serial.set_option( boost::asio::serial_port_base::baud_rate( baud_rate ) );
	}

	/**
	 * Write a string to the serial device.
	 * \param s string to write
	 * \throws boost::system::system_error on failure
	 */
	void writeString( std::string s )
	{
		boost::asio::write( serial,boost::asio::buffer( s.c_str(),s.size() ) );
	}

	/**
	 * Blocks until a line is received from the serial device.
	 * Eventual '\n' or '\r\n' characters at the end of the string are removed.
	 * \return a string containing the received line
	 * \throws boost::system::system_error on failure
	 */
	std::string readLine()
	{
		//Reading data char by char, code is optimized for simplicity, not speed
		using namespace boost;
		char c;
		std::string result;
		for( ;; )
		{
			asio::read( serial,asio::buffer( &c,1 ) );
			switch( c )
			{
			case '\r':
				break;
			case '\n':
				return result;
			default:
				result+=c;
			}
		}
	}

private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
};


int main( int argc, char* argv[] )
{
	try {

		SimpleSerial serial( argv[1],115200 );

		while ( 1 )
		{

			string text = serial.readLine();
			cout << text << endl;

			/*			char_separator<char> sep(",");
						tokenizer< char_separator<char> > tokens(text, sep);
						BOOST_FOREACH(string t, tokens)
						{
							char_separator<char> sep1(": ");
							tokenizer< char_separator<char> > tok(t, sep1);
							vector< string > vec;
							vec.assign(tok.begin(),tok.end());
							cout << vec.at(0) << " ";

							if (vec.at(0) == "scr" || vec.at(0) == "snkscr" || vec.at(0) == "volt")
							{
								uint32_t temp = atoi(vec.at(1).c_str());
								float flt = *((float*) &temp);
								cout << flt << " ";
							} else {
								cout << vec.at(1) << " ";
							}

						}
						cout << endl;*/


		}

	} catch( boost::system::system_error& e )
	{
		cout<<"Error: "<<e.what()<<endl;
		return 1;
	}
}