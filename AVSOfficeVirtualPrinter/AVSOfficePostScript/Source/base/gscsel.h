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

/* $Id: gscsel.h 8022 2007-06-05 22:23:38Z giles $ */
/* Definitions for color operand selection */

#ifndef gscsel_INCLUDED
#  define gscsel_INCLUDED

/*
 * Define whether we are mapping a "source" or a "texture" color for
 * RasterOp.  Right the source and texture only have separate halftone
 * phases in the graphics state, but someday they might have more.
 */
typedef enum {
    gs_color_select_all = -1,	/* for setting only, not for reading */
    gs_color_select_texture = 0,	/* 0 is the one is used for currenthtphase */
    gs_color_select_source = 1
} gs_color_select_t;

#define gs_color_select_count 2

#endif /* gscsel_INCLUDED */
