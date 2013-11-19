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

/* $Id: slzwx.h 8022 2007-06-05 22:23:38Z giles $ */
/* Definitions for LZW filters */
/* Requires strimpl.h */

#ifndef slzwx_INCLUDED
#  define slzwx_INCLUDED

typedef struct lzw_decode_s lzw_decode;
typedef struct lzw_encode_table_s lzw_encode_table;
typedef struct stream_LZW_state_s {
    stream_state_common;
    /* The following are set before initialization. */
    int InitialCodeLength;	/* decoding only */
    /*
     * Adobe calls FirstBitLowOrder LowBitFirst.  Either one will work
     * in PostScript code.
     */
    bool FirstBitLowOrder;	/* decoding only */
    bool BlockData;		/* decoding only */
    int EarlyChange;		/* decoding only */
    /* The following are updated dynamically. */
    uint bits;			/* buffer for input bits */
    int bits_left;		/* Decode: # of valid bits left, [0..7] */
				/* (low-order bits if !FirstBitLowOrder, */
				/* high-order bits if FirstBitLowOrder) */
    int bytes_left;		/* # of bytes left in current block */
				/* (arbitrary large # if not GIF) */
    union _lzt {
	lzw_decode *decode;
	lzw_encode_table *encode;
    } table;
    uint next_code;		/* next code to be assigned */
    int code_size;		/* current # of bits per code */
    int prev_code;		/* previous code recognized or assigned */
    uint prev_len;		/* length of prev_code */
    int copy_code;		/* code whose string is being */
				/* copied, -1 if none */
    uint copy_len;		/* length of copy_code */
    int copy_left;		/* amount of string left to copy */
    bool first;			/* true if no output yet */
} stream_LZW_state;

extern_st(st_LZW_state);
#define public_st_LZW_state()	/* in slzwc.c */\
  gs_public_st_ptrs1(st_LZW_state, stream_LZW_state,\
    "LZWDecode state", lzwd_enum_ptrs, lzwd_reloc_ptrs, table.decode)
#define s_LZW_set_defaults_inline(ss)\
  ((ss)->InitialCodeLength = 8,\
   (ss)->FirstBitLowOrder = false,\
   (ss)->BlockData = false,\
   (ss)->EarlyChange = 1,\
   /* Clear pointers */\
   (ss)->table.decode /*=encode*/ = 0)
extern const stream_template s_LZWD_template;
extern const stream_template s_LZWE_template;

/* Shared procedures */
void s_LZW_set_defaults(stream_state *);
void s_LZW_release(stream_state *);

#endif /* slzwx_INCLUDED */
