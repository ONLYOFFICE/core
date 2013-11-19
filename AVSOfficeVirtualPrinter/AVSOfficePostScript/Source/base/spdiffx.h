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

/* $Id: spdiffx.h 8022 2007-06-05 22:23:38Z giles $ */
/* Definitions for PixelDifference filters */
/* Requires strimpl.h */

#ifndef spdiffx_INCLUDED
#  define spdiffx_INCLUDED

/*
 * Define the maximum value for Colors.  This must be at least 4, but can
 * be arbitrarily large: the only cost is a larger stream state structure.
 * Some PS3 CET files require this to be at least 57.
 */
#define s_PDiff_max_Colors 60

/* PixelDifferenceDecode / PixelDifferenceEncode */
typedef struct stream_PDiff_state_s {
    stream_state_common;
    /* The client sets the following before initialization. */
    int Colors;			/* # of colors, 1..s_PDiff_max_Colors */
    int BitsPerComponent;	/* 1, 2, 4, 8 */
    int Columns;
    /* The init procedure computes the following. */
    uint row_count;		/* # of bytes per row */
    byte end_mask;		/* mask for left-over bits in last byte */
    int case_index;		/* switch index for case dispatch */
    /* The following are updated dynamically. */
    uint row_left;		/* # of bytes left in row */
    uint prev[s_PDiff_max_Colors];	/* previous sample */
} stream_PDiff_state;

#define private_st_PDiff_state()	/* in spdiff.c */\
  gs_private_st_simple(st_PDiff_state, stream_PDiff_state,\
    "PixelDifferenceEncode/Decode state")
#define s_PDiff_set_defaults_inline(ss)\
  ((ss)->Colors = 1, (ss)->BitsPerComponent = 8, (ss)->Columns = 1)
extern const stream_template s_PDiffD_template;
extern const stream_template s_PDiffE_template;

#endif /* spdiffx_INCLUDED */
