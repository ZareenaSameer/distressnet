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
#include "urlencode.h"

std::string char2hex( char dec )
{
	char dig1 = ( dec&0xF0 )>>4;
	char dig2 = ( dec&0x0F );
	if ( 0<= dig1 && dig1<= 9 ) dig1+=48;   //0,48 in ascii
	if ( 10<= dig1 && dig1<=15 ) dig1+=65-10; //A,65 in ascii
	if ( 0<= dig2 && dig2<= 9 ) dig2+=48;
	if ( 10<= dig2 && dig2<=15 ) dig2+=65-10;

	std::string r;
	r.append( &dig1, 1 );
	r.append( &dig2, 1 );
	return r;
}

std::string urlencode( const std::string &c )
{

	std::string escaped;
	int max = c.length();
	for( int i=0; i<max; i++ )
	{
		if ( ( 48 <= c[i] && c[i] <= 57 ) || //0-9
		        ( 65 <= c[i] && c[i] <= 90 ) || //ABC...XYZ
		        ( 97 <= c[i] && c[i] <= 122 ) || //abc...xyz
		        ( c[i]=='~' || c[i]=='-' || c[i]=='_' || c[i]=='.' )
		   )
		{
			escaped.append( &c[i], 1 );
		}
		else
		{
			escaped.append( "%" );
			escaped.append( char2hex( c[i] ) ); //converts char 255 to string "FF"
		}
	}
	return escaped;
}