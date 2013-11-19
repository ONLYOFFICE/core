/* Copyright (C) 2001-2007 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: smd5.h 8032 2007-06-08 18:02:05Z giles $ */
/* Definitions for MD5Encode filter */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef smd5_INCLUDED
#  define smd5_INCLUDED

#include "md5.h"

/*
 * The MD5Encode filter accepts an arbitrary amount of input data, and then,
 * when closed, emits the 16-byte MD5 digest.
 */
typedef struct stream_MD5E_state_s {
    stream_state_common;
    gs_md5_state_t md5;
} stream_MD5E_state;

#define private_st_MD5E_state()	/* in smd5.c */\
  gs_private_st_simple(st_MD5E_state, stream_MD5E_state,\
    "MD5Encode state")
extern const stream_template s_MD5E_template;

stream *s_MD5E_make_stream(gs_memory_t *mem, byte *digest, int digest_size);

/*
 * The MD5Counter filter accepts an arbitrary amount of input data, 
 * and copies them to output. It also computes an MD5 digest of the data,
 * which may be gathered at any time by s_MD5C_get_digest;
 */
stream *s_MD5C_make_stream(gs_memory_t *mem, stream *strm);
/* Get a digest. Apply 'flush' before calling. */
int s_MD5C_get_digest(stream *s, byte *buf, int buf_length);

#endif /* smd5_INCLUDED */
