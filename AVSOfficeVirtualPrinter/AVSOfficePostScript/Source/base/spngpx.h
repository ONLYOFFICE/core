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

/* $Id: spngpx.h 8823 2008-07-07 08:56:59Z ken $ */
/* Definitions for PNGPredictor filters */
/* Requires strimpl.h */

#ifndef spngpx_INCLUDED
#  define spngpx_INCLUDED

/*
 * Define the maximum value for Colors.  The PNG specification probably
 * defines this as 16, but some PS3 CET files require it to be as large as
 * 53.  The only cost of larger values is a larger stream state structure.
 * In fact some CET files (09-34.ps) require 250 inks, so bump this to the 
 * current maximum of 256.
 */
#define s_PNG_max_Colors 256

/* PNGPredictorDecode / PNGPredictorEncode */
typedef struct stream_PNGP_state_s {
    stream_state_common;
    /* The client sets the following before initialization. */
    int Colors;			/* # of colors, 1..s_PNG_max_Colors */
    int BitsPerComponent;	/* 1, 2, 4, 8, 16 */
    uint Columns;		/* >0 */
    int Predictor;		/* 10-15, only relevant for Encode */
    /* The init procedure computes the following. */
    uint row_count;		/* # of bytes per row */
    byte end_mask;		/* mask for left-over bits in last byte */
    int bpp;			/* bytes per pixel */
    byte *prev_row;		/* previous row */
    int case_index;		/* switch index for case dispatch, */
				/* set dynamically when decoding */
    /* The following are updated dynamically. */
    long row_left;		/* # of bytes left in row */
    byte prev[2 * s_PNG_max_Colors]; /* previous samples */
} stream_PNGP_state;

#define private_st_PNGP_state()	/* in sPNGP.c */\
  gs_private_st_ptrs1(st_PNGP_state, stream_PNGP_state,\
    "PNGPredictorEncode/Decode state", pngp_enum_ptrs, pngp_reloc_ptrs,\
    prev_row)
#define s_PNGP_set_defaults_inline(ss)\
  ((ss)->Colors = 1, (ss)->BitsPerComponent = 8, (ss)->Columns = 1,\
   (ss)->Predictor = 15,\
		/* Clear pointers */\
   (ss)->prev_row = 0)
extern const stream_template s_PNGPD_template;
extern const stream_template s_PNGPE_template;

#endif /* spngpx_INCLUDED */
