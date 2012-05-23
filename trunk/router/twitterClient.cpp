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
#include "twitterClient.h"

//	266263893-P9g5kdxLrpkazhrRHug2ks16XvypSr4LjXY7mYKe key
//	KWiGEqGldGUbLBNZ72zZavCSNZK0iXgF0f7gwA3Ms	secret

void printUsage()
{
	printf( "\nUsage:\ntwitterClient -u username -p password\n" );
}

int main( int argc, char* argv[] )
{
	std::string user( "lenss_lab" );
	std::string pass( "Lenss123" );

	twitCurl twitterObj;
	std::string tmpStr;
	std::string replyMsg;
	char tmpBuf[1024];
	int tmpVar = 0;

	/* Set twitter username and password */
	twitterObj.setTwitterUsername( user );
	twitterObj.setTwitterPassword( pass );

	/* OAuth flow begins */
	/* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
	twitterObj.getOAuth().setConsumerKey( std::string( "PYfkFFlcdqJEyiarhOtug" ) );
	twitterObj.getOAuth().setConsumerSecret( std::string( "mM7WWBAbSXfqOlXrVqKjt3Gna7SrG7X8u0EAhEwi8" ) );


	/* Step 1: Check if we alredy have OAuth access token from a previous run */
	std::string myOAuthAccessTokenKey( "266263893-P9g5kdxLrpkazhrRHug2ks16XvypSr4LjXY7mYKe" );
	std::string myOAuthAccessTokenSecret( "KWiGEqGldGUbLBNZ72zZavCSNZK0iXgF0f7gwA3Ms" );
	std::ifstream oAuthTokenKeyIn;
	std::ifstream oAuthTokenSecretIn;

	/*    oAuthTokenKeyIn.open( "twitterClient_token_key.txt" );
	    oAuthTokenSecretIn.open( "twitterClient_token_secret.txt" );

	    memset( tmpBuf, 0, 1024 );
	    oAuthTokenKeyIn >> tmpBuf;
	    myOAuthAccessTokenKey = tmpBuf;

	    memset( tmpBuf, 0, 1024 );
	    oAuthTokenSecretIn >> tmpBuf;
	    myOAuthAccessTokenSecret = tmpBuf;

	    oAuthTokenKeyIn.close();
	    oAuthTokenSecretIn.close();*/

	if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
	{
		/* If we already have these keys, then no need to go through auth again */
		printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

		twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
		twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
	}
	else
	{
		/* Step 2: Get request token key and secret */
		twitterObj.oAuthRequestToken( tmpStr );

		/* Step 3: Get PIN  */
		memset( tmpBuf, 0, 1024 );
		printf( "\nDo you want to visit twitter.com for PIN (0 for no; 1 for yes): " );
		gets( tmpBuf );
		tmpVar = atoi( tmpBuf );

		if( tmpVar > 0 )
		{
			/* Ask user to visit twitter.com page and get PIN */
			memset( tmpBuf, 0, 1024 );
			printf( "\nPlease visit this link in web browser and authorize this application:\n%s", tmpStr.c_str() );
			printf( "\nEnter the PIN provided by twitter: " );
			gets( tmpBuf );
			tmpStr = tmpBuf;
			twitterObj.getOAuth().setOAuthPin( tmpStr );
		}
		else
		{
			/* Else, get it via twitcurl PIN handling */
			twitterObj.oAuthHandlePIN( tmpStr );
		}

		/* Step 4: Exchange request token with access token */
		twitterObj.oAuthAccessToken();

		/* Step 5: Now, save this access token key and secret for future use without PIN */
		twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
		twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

		/* Step 6: Save these keys in a file or wherever */
		std::ofstream oAuthTokenKeyOut;
		std::ofstream oAuthTokenSecretOut;

		oAuthTokenKeyOut.open( "twitterClient_token_key.txt" );
		oAuthTokenSecretOut.open( "twitterClient_token_secret.txt" );

		oAuthTokenKeyOut.clear();
		oAuthTokenSecretOut.clear();

		oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
		oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

		oAuthTokenKeyOut.close();
		oAuthTokenSecretOut.close();
	}
	/* OAuth flow ends */





	/* Post a new status message */
	char statusMsg[1024];
	memset( statusMsg, 0, 1024 );
	printf( "\nEnter a new status message: " );
	gets( statusMsg );
	tmpStr = statusMsg;
	replyMsg = "";
	if( twitterObj.statusUpdate( tmpStr ) )
	{
		twitterObj.getLastWebResponse( replyMsg );
		printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
	}
	else
	{
		twitterObj.getLastCurlError( replyMsg );
		printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
	}


	return 0;
}
