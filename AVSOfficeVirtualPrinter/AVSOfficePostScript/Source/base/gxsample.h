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

/* $Id: gxsample.h 8022 2007-06-05 22:23:38Z giles $ */
/* Sample lookup and expansion */

#ifndef gxsample_INCLUDED
#  define gxsample_INCLUDED

/*
 * The following union implements the expansion of sample
 * values from N bits to 8, and a possible linear transformation.
 */
typedef union sample_lookup_s {
    bits32 lookup4x1to32[16];	/* 1 bit/sample, not spreading */
    bits16 lookup2x2to16[16];	/* 2 bits/sample, not spreading */
    byte lookup8[256];		/* 1 bit/sample, spreading [2] */
    /* 2 bits/sample, spreading [4] */
    /* 4 bits/sample [16] */
    /* 8 bits/sample [256] */
} sample_lookup_t;

/*
 * Define identity and inverted expansion lookups for 1-bit input values.
 * These can be cast to a const sample_lookup_t.
 */
extern const bits32 lookup4x1to32_identity[16];
extern const bits32 lookup4x1to32_inverted[16];

#ifndef sample_map_DEFINED
#define sample_map_DEFINED
typedef struct sample_map_s sample_map;
#endif

/*
 * Define procedures to unpack and shuffle image data samples.  The Unix C
 * compiler can't handle typedefs for procedure (as opposed to
 * pointer-to-procedure) types, so we have to do it with macros instead.
 *
 * The original data start at sample data_x relative to data.
 * bptr points to the buffer normally used to deliver the unpacked data.
 * The unpacked data are at sample *pdata_x relative to the return value.
 *
 * Note that this procedure may return either a pointer to the buffer, or
 * a pointer to the original data.
 */
#define SAMPLE_UNPACK_PROC(proc)\
  const byte *proc(byte *bptr, int *pdata_x, const byte * data, int data_x,\
		   uint dsize, const sample_map *smap, int spread,\
		   int num_components_per_plane)
typedef SAMPLE_UNPACK_PROC((*sample_unpack_proc_t));

/*
 * Declare the 1-for-1 unpacking procedure.
 */
SAMPLE_UNPACK_PROC(sample_unpack_copy);
/*
 * Declare unpacking procedures for 1, 2, 4, and 8 bits per pixel,
 * with optional spreading of the result.
 */
SAMPLE_UNPACK_PROC(sample_unpack_1);
SAMPLE_UNPACK_PROC(sample_unpack_2);
SAMPLE_UNPACK_PROC(sample_unpack_4);
SAMPLE_UNPACK_PROC(sample_unpack_8);

SAMPLE_UNPACK_PROC(sample_unpack_1_interleaved);
SAMPLE_UNPACK_PROC(sample_unpack_2_interleaved);
SAMPLE_UNPACK_PROC(sample_unpack_4_interleaved);
SAMPLE_UNPACK_PROC(sample_unpack_8_interleaved);

#endif /* gxsample_INCLUDED */
