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
//******************************************************************************
//* HMAC_SHA1.cpp : Implementation of HMAC SHA1 algorithm
//*                 Comfort to RFC 2104
//*
//******************************************************************************
#include "HMAC_SHA1.h"
#include <iostream>
#include <memory>


void CHMAC_SHA1::HMAC_SHA1( BYTE *text, int text_len, BYTE *key, int key_len, BYTE *digest )
{
	memset( SHA1_Key, 0, SHA1_BLOCK_SIZE );

	/* repeated 64 times for values in ipad and opad */
	memset( m_ipad, 0x36, sizeof( m_ipad ) );
	memset( m_opad, 0x5c, sizeof( m_opad ) );

	/* STEP 1 */
	if ( key_len > SHA1_BLOCK_SIZE )
	{
		CSHA1::Reset();
		CSHA1::Update( ( UINT_8 * )key, key_len );
		CSHA1::Final();

		CSHA1::GetHash( ( UINT_8 * )SHA1_Key );
	}
	else
		memcpy( SHA1_Key, key, key_len );

	/* STEP 2 */
	for ( int i=0; i<sizeof( m_ipad ); i++ )
	{
		m_ipad[i] ^= SHA1_Key[i];
	}

	/* STEP 3 */
	memcpy( AppendBuf1, m_ipad, sizeof( m_ipad ) );
	memcpy( AppendBuf1 + sizeof( m_ipad ), text, text_len );

	/* STEP 4 */
	CSHA1::Reset();
	CSHA1::Update( ( UINT_8 * )AppendBuf1, sizeof( m_ipad ) + text_len );
	CSHA1::Final();

	CSHA1::GetHash( ( UINT_8 * )szReport );

	/* STEP 5 */
	for ( int j=0; j<sizeof( m_opad ); j++ )
	{
		m_opad[j] ^= SHA1_Key[j];
	}

	/* STEP 6 */
	memcpy( AppendBuf2, m_opad, sizeof( m_opad ) );
	memcpy( AppendBuf2 + sizeof( m_opad ), szReport, SHA1_DIGEST_LENGTH );

	/*STEP 7 */
	CSHA1::Reset();
	CSHA1::Update( ( UINT_8 * )AppendBuf2, sizeof( m_opad ) + SHA1_DIGEST_LENGTH );
	CSHA1::Final();

	CSHA1::GetHash( ( UINT_8 * )digest );
}
