/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: sarc4.c 8250 2007-09-25 13:31:24Z giles $ */

/* Arcfour cipher and filter implementation */

#include "memory_.h"
#include "gserrors.h"
#include "gserror.h"
#include "strimpl.h"
#include "sarc4.h"

/* This is an independent implementation of the symmetric block
 * cipher commonly known as 'arcfour' based on Bruce Schneier's
 * description of the algorithm in _Applied Cryptography_. Arcfour
 * is believed to be functionally equivalent to the RC4(tm) cipher
 * mentioned in the PDF specification. (RC4 is a registered 
 * trademark of RSA Data Security, Inc.)
 */

/* stream implementation */

private_st_arcfour_state();	/* creates a gc object for our state, defined in sarc4.h */

/* initialize the S box using the given key */
int
s_arcfour_set_key(stream_arcfour_state * state, const unsigned char *key,
		  int keylength)
{
    unsigned int x, y;
    unsigned char s, *S = state->S;

    if (keylength < 1)
	return_error(gs_error_rangecheck);

    /* initialize to eponymous values */
    for (x = 0; x < 256; x++)
	S[x] = x;

    /* scramble based on the key */
    y = 0;
    for (x = 0; x < 256; x++) {
	y = (y + S[x] + key[x % keylength]) & 0xFF;
	s = S[x];
	S[x] = S[y];
	S[y] = s;
    }

    /* initialize the indicies */
    state->x = 0;
    state->y = 0;

    /* return successfully */
    return 0;
}

/* (de)crypt a section of text--the procedure is the same
 * in each direction. see strimpl.h for return codes.
 */
static int
s_arcfour_process(stream_state * ss, stream_cursor_read * pr,
		  stream_cursor_write * pw, bool last)
{
    stream_arcfour_state *const state = (stream_arcfour_state *) ss;
    unsigned int x = state->x;
    unsigned int y = state->y;
    unsigned char s, *S = state->S;
    unsigned char z;
   const unsigned char *limit;
    int status;

    /* figure out if we're going to run out of space */
    if ((pr->limit - pr->ptr) > (pw->limit - pw->ptr)) {
	limit = pr->ptr + (pw->limit - pw->ptr);
	status = 1;
    } else {
	limit = pr->limit;
	status = last ? EOFC : 0;
    }
    /* generate a pseudorandom byte stream and xor it with the input */
    while (pr->ptr < limit) {
	x = (x + 1) & 0xFF;
	y = (y + S[x]) & 0xFF;
	s = S[x];
	S[x] = S[y];
	S[y] = s;
	z = S[(S[x] + S[y]) & 0xFF];

	*++pw->ptr = (*++pr->ptr) ^ z;
    }
    /* save state */
    state->x = x;
    state->y = y;

    return status;
}

/* stream template */
const stream_template s_arcfour_template = {
    &st_arcfour_state, NULL, s_arcfour_process, 1, 1
};

/* (de)crypt a section of text in a buffer -- the procedure is the same
 * in each direction. see strimpl.h for return codes.
 */
int
s_arcfour_process_buffer(stream_arcfour_state *ss, byte *buf, int buf_size)
{
    stream_cursor_read r;
    stream_cursor_write w;
    const bool unused = false;

    r.ptr = w.ptr = buf - 1;
    r.limit = w.limit = buf - 1 + buf_size;
    return s_arcfour_process((stream_state *)ss, &r, &w, unused);
}

