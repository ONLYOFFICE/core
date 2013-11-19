/* Copyright (C) 2001-2008 Artifex Software, Inc.
   All Rights Reserved.

   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id$ */

/* AES filter implementation */

#include "memory_.h"
#include "gserrors.h"
#include "gserror.h"
#include "strimpl.h"
#include "saes.h"

/* stream implementation */

private_st_aes_state();	/* creates a gc object for our state,
			   defined in saes.h */

/* Store a key in our crypt context */
int
s_aes_set_key(stream_aes_state * state, const unsigned char *key,
		  int keylength)
{
    int code = 0;

    if ( (keylength < 1) || (keylength > SAES_MAX_KEYLENGTH) )
	return_error(gs_error_rangecheck);
    if (key == NULL)
	return_error(gs_error_invalidaccess);

    /* we can't set the key here because the interpreter's
       filter implementation wants to duplicate our state
       after the zfaes.c binding calls us. So stash it now
       and handle it in our process method. */
    memcpy(state->key, key, keylength);
    state->keylength = keylength;

    if (code) {
      return gs_throw(gs_error_rangecheck, "could not set AES key");
    }

    /* return successfully */
    return 0;
}

/* initialize our state object. */
static int
s_aes_init(stream_state *ss)
{
    stream_aes_state *const state = (stream_aes_state *) ss;

    /* clear the flags so we know we're at the start of a stream */
    state->initialized = 0;
    state->ctx = NULL;

    return 0;
}

/* release our private storage */
static void
s_aes_release(stream_state *ss)
{
    stream_aes_state *const state = (stream_aes_state *) ss;

    if (state->ctx != NULL)
        gs_free_object(state->memory, state->ctx, "aes context structure");
}

/* (de)crypt a section of text--the procedure is the same
 * in each direction. see strimpl.h for return codes.
 */
static int
s_aes_process(stream_state * ss, stream_cursor_read * pr,
		  stream_cursor_write * pw, bool last)
{
    stream_aes_state *const state = (stream_aes_state *) ss;
    const unsigned char *limit;
    const long in_size = pr->limit - pr->ptr;
    const long out_size = pw->limit - pw->ptr;
    unsigned char temp[16];
    int status = 0;

    /* figure out if we're going to run out of space */
    if (in_size > out_size) {
	limit = pr->ptr + out_size;
	status = 1; /* need more output space */
    } else {
	limit = pr->limit;
	status = last ? EOFC : 0; /* need more input */
    }

    /* set up state and context */
    if (state->ctx == NULL) {
      /* allocate the aes context. this is a public struct but it
         contains internal pointers, so we need to store it separately
         in immovable memory like any opaque structure. */
      state->ctx = (aes_context *)gs_alloc_bytes_immovable(state->memory,
		sizeof(aes_context), "aes context structure");
      if (state->ctx == NULL) {
	gs_throw(gs_error_VMerror, "could not allocate aes context");
	return ERRC;
      }
      if (state->keylength < 1 || state->keylength > SAES_MAX_KEYLENGTH) {
	gs_throw1(gs_error_rangecheck, "invalid aes key length (%d bytes)",
		state->keylength);
	return ERRC;
      }
      aes_setkey_dec(state->ctx, state->key, state->keylength * 8);
    }
    if (!state->initialized) {
	/* read the initialization vector from the first 16 bytes */
	if (in_size < 16) return 0; /* get more data */
	memcpy(state->iv, pr->ptr + 1, 16);
	state->initialized = 1;
	pr->ptr += 16;
    }

    /* decrypt available blocks */
    while (pr->ptr + 16 <= limit) {
      aes_crypt_cbc(state->ctx, AES_DECRYPT, 16, state->iv,
				pr->ptr + 1, temp);
      pr->ptr += 16;
      if (last && pr->ptr == pr->limit) {
        /* we're on the last block; unpad */
        int pad = temp[15];
        if (pad < 1 || pad > 16) return ERRC;
        memcpy(pw->ptr + 1, temp, 16 - pad);
        pw->ptr +=  16 - pad;
        return EOFC;
      }
      memcpy(pw->ptr + 1, temp, 16);
      pw->ptr += 16;
    }

    return status;
}

/* stream template */
const stream_template s_aes_template = {
    &st_aes_state, s_aes_init,
    s_aes_process, 16, 16,
    s_aes_release
};
