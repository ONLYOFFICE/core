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
/* $Id: gdev8bcm.h 8022 2007-06-05 22:23:38Z giles $ */
/* 8-bit color map support */
/* Requires gxdevice.h (for gx_color_value) */

#ifndef gdev8bcm_INCLUDED
#  define gdev8bcm_INCLUDED

/*
 * The MS-DOS, MS Windows, and X Windows drivers all use (at least on
 * some platforms) an 8-bit color map in which some fraction is reserved
 * for a pre-allocated cube and some or all of the remainder is
 * allocated dynamically.  Since looking up colors in this map can be
 * a major performance bottleneck, we provide an efficient implementation
 * that can be shared among drivers.
 *
 * As a performance compromise, we only look up the top 5 bits of the
 * RGB value in the color map.  This compromises color quality very little,
 * and allows substantial optimizations.
 */

#define gx_8bit_map_size 323
#define gx_8bit_map_spreader 123	/* approx. 323 - (1.618 * 323) */
typedef struct gx_8bit_map_entry_s {
    ushort rgb;			/* key = 0rrrrrgggggbbbbb */
#define gx_8bit_no_rgb ((ushort)0xffff)
#define gx_8bit_rgb_key(r, g, b)\
  (((r >> (gx_color_value_bits - 5)) << 10) +\
   ((g >> (gx_color_value_bits - 5)) << 5) +\
   (b >> (gx_color_value_bits - 5)))
    short index;		/* value */
} gx_8bit_map_entry;
typedef struct gx_8bit_color_map_s {
    int count;			/* # of occupied entries */
    int max_count;		/* max # of occupied entries */
    gx_8bit_map_entry map[gx_8bit_map_size + 1];
} gx_8bit_color_map;

/* Initialize an 8-bit color map. */
void gx_8bit_map_init(gx_8bit_color_map *, int);

/* Look up a color in an 8-bit color map. */
/* Return -1 if not found. */
int gx_8bit_map_rgb_color(const gx_8bit_color_map *, gx_color_value,
			  gx_color_value, gx_color_value);

/* Test whether an 8-bit color map has room for more entries. */
#define gx_8bit_map_is_full(pcm)\
  ((pcm)->count == (pcm)->max_count)

/* Add a color to an 8-bit color map. */
/* Return -1 if the map is full. */
int gx_8bit_add_rgb_color(gx_8bit_color_map *, gx_color_value,
			  gx_color_value, gx_color_value);

#endif /* gdev8bcm_INCLUDED */
