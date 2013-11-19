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

/* $Id: szlibx.h 8022 2007-06-05 22:23:38Z giles $ */
/* zlib filter state definition */

#ifndef szlibx_INCLUDED
#  define szlibx_INCLUDED

/* Define an opaque type for the dynamic part of the state. */
typedef struct zlib_dynamic_state_s zlib_dynamic_state_t;

/* Define the stream state structure. */
typedef struct stream_zlib_state_s {
    stream_state_common;
    /* Parameters - compression and decompression */
    int windowBits;
    bool no_wrapper;		/* omit wrapper and checksum */
    /* Parameters - compression only */
    int level;			/* effort level */
    int method;
    int memLevel;
    int strategy;
    /* Dynamic state */
    zlib_dynamic_state_t *dynamic;
} stream_zlib_state;

/*
 * The state descriptor is public only to allow us to split up
 * the encoding and decoding filters.
 */
extern_st(st_zlib_state);
#define public_st_zlib_state()	/* in szlibc.c */\
  gs_public_st_ptrs1(st_zlib_state, stream_zlib_state,\
    "zlibEncode/Decode state", zlib_state_enum_ptrs, zlib_state_reloc_ptrs,\
    dynamic)
extern const stream_template s_zlibD_template;
extern const stream_template s_zlibE_template;

/* Shared procedures */
stream_proc_set_defaults(s_zlib_set_defaults);

#endif /* szlibx_INCLUDED */
