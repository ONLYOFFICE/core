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

/* $Id: gsccolor.h 8864 2008-07-22 18:32:34Z ray $ */
/* Client color structure definition */

#ifndef gsccolor_INCLUDED
#  define gsccolor_INCLUDED

#include "gsstype.h"		/* for extern_st */

/* Pattern instance, usable in color. */
#ifndef gs_pattern_instance_DEFINED
#  define gs_pattern_instance_DEFINED
typedef struct gs_pattern_instance_s gs_pattern_instance_t;
#endif

/*
 * Define the maximum number of components in a client color.
 * This must be at least 4, and should be at least 6 to accommodate
 * hexachrome DeviceN color spaces.  It is currently set to the same
 * value as Adobe CPSI can handle 
 */
#ifndef GS_CLIENT_COLOR_MAX_COMPONENTS		/* Allow override with XCFLAGS */
#  define GS_CLIENT_COLOR_MAX_COMPONENTS (8)
#endif

/* Paint (non-Pattern) colors */
typedef struct gs_paint_color_s {
    float values[GS_CLIENT_COLOR_MAX_COMPONENTS];
    /* CAUTION: The shading decomposition algorithm may allocate 
       a smaller space when a small number of color components is in use.
    */
} gs_paint_color;

/* General colors */
#ifndef gs_client_color_DEFINED
#  define gs_client_color_DEFINED
typedef struct gs_client_color_s gs_client_color;

#endif
struct gs_client_color_s {
    gs_pattern_instance_t *pattern;
    gs_paint_color paint;	/* also color for uncolored pattern */
    /* CAUTION: gs_paint_color structure must be the last field in 
       gs_client_color_s to allow allocating a smaller space when 
       a small number of color components is in use.
    */
};

extern_st(st_client_color);
#define public_st_client_color() /* in gscolor.c */\
  gs_public_st_ptrs1(st_client_color, gs_client_color, "gs_client_color",\
    client_color_enum_ptrs, client_color_reloc_ptrs, pattern)
#define st_client_color_max_ptrs 1

#endif /* gsccolor_INCLUDED */
