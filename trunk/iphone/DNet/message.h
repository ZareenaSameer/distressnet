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
/* Copyright (c) 2006 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
/* Authors:  David Gay  <dgay@intel-research.net>
 *           Intel Research Berkeley Lab
 */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** The type of message buffers */
typedef struct tmsg tmsg_t;

/** Invoke the function set by tmsg_set_fail.
 *  tmsg_fail is called by the tmsg_read and tmsg_write functions when an
 *  out-of-buffer access is attempted.
*/
void tmsg_fail(void);

/** Set the function that tmsg_fail should call, and return the previous
 *  function. If the function is NULL, tmsg_fail does nothing.
*/
void (*tmsg_set_fail(void (*fn)(void)))(void);

/**
 * Create a message buffer from array 'packet' of 'len' bytes
 */
tmsg_t *new_tmsg(void *packet, size_t len);

/**
 * Free a message buffer. This does NOT free the underlying array.
 */
void free_tmsg(tmsg_t *msg);

/** 
 * Reuse an existing tmsg
 */
void reset_tmsg(tmsg_t *msg, void *packet, size_t len) ;

/**
 * Return underlying array of a message buffer
 */
void *tmsg_data(tmsg_t *msg);

/**
 * Return length of a message buffer
 */
size_t tmsg_length(tmsg_t *msg);

/**
 * Read an unsigned little-endian integer of 'bit_length' bits from bit offset
 * 'bit_offset'
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and 0 is returned.
 */
uint64_t tmsg_read_ule(tmsg_t *msg, size_t bit_offset, size_t bit_length);

/**
 * Read a signed little-endian integer of 'bit_length' bits from bit offset
 * 'bit_offset'
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and 0 is returned.
 */
int64_t tmsg_read_le(tmsg_t *msg, size_t bit_offset, size_t bit_length);

/**
 * Write an unsigned little-endian integer of 'bit_length' bits to bit offset
 * 'bit_offset'.
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and no write occurs.
 */
void tmsg_write_ule(tmsg_t *msg, size_t bit_offset, size_t bit_length, uint64_t value);

/**
 * Write a signed little-endian integer of 'bit_length' bits to bit offset
 * 'bit_offset'.
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and no write occurs.
 */
void tmsg_write_le(tmsg_t *msg, size_t bit_offset, size_t bit_length, int64_t value);

/**
 * Read an unsigned big-endian integer of 'bit_length' bits from bit offset
 * 'bit_offset'
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and 0 is returned.
 */
uint64_t tmsg_read_ube(tmsg_t *msg, size_t bit_offset, size_t bit_length);

/**
 * Read a signed big-endian integer of 'bit_length' bits from bit offset
 * 'bit_offset'
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and 0 is returned.
 */
int64_t tmsg_read_be(tmsg_t *msg, size_t bit_offset, size_t bit_length);

/**
 * Write an unsigned big-endian integer of 'bit_length' bits to bit offset
 * 'bit_offset'.
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and no write occurs.
 */
void tmsg_write_ube(tmsg_t *msg, size_t bit_offset, size_t bit_length, uint64_t value);

/**
 * Write a signed big-endian integer of 'bit_length' bits to bit offset
 * 'bit_offset'.
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and no write occurs.
 */
void tmsg_write_be(tmsg_t *msg, size_t bit_offset, size_t bit_length, int64_t value);

/**
 * Read a 32-bit IEEE float stored in little-endian format (bit 31: sign, 
 * bits 30-23: exponent, bits 22-0: mantissa) from bit offset 'bit_offset'
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and 0 is returned.
 */
float tmsg_read_float_le(tmsg_t *msg, size_t offset);

/**
 * Write a 32-bit IEEE float in little-endian format (bit 31: sign, 
 * bits 30-23: exponent, bits 22-0: mantissa) to bit offset 'bit_offset'
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and no write occurs.
 */
void tmsg_write_float_le(tmsg_t *msg, size_t offset, float x);

/**
 * Read a 32-bit IEEE float stored in big-endian format (bit 31: sign, 
 * bits 30-23: exponent, bits 22-0: mantissa) from bit offset 'bit_offset'
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and 0 is returned.
 */
float tmsg_read_float_be(tmsg_t *msg, size_t offset);

/**
 * Write a 32-bit IEEE float in big-endian format (bit 31: sign, 
 * bits 30-23: exponent, bits 22-0: mantissa) to bit offset 'bit_offset'
 * If the specified field is out of range for the buffer, tmsg_fail is called
 * and no write occurs.
 */
void tmsg_write_float_be(tmsg_t *msg, size_t offset, float x);

#ifdef __cplusplus
}
#endif

#endif
