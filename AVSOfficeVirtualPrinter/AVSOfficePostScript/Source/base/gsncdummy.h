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
/* $Id: gsncdummy.h 8044 2007-06-12 02:41:44Z giles $ */
/* Global definitions for the example custom color callback handling. */

#ifndef gsncdummy_INCLUDED
#define gsncdummy_INCLUDED

#if ENABLE_CUSTOM_COLOR_CALLBACK		/* Defined in src/gsnamecl.h */

/*
 * This s a list of PANTONE color names and a set of equivalent CMYK values,
 */
typedef struct pantone_list_s {
    const char *name;		/* Name of the PANTONE color */
    double c, m, y, k;		/* Equivalent CMYK values */
} pantone_list_t;


#endif		/* if ENABLE_CUSTOM_COLOR_CALLBACK */

#endif		/* if gsncdummy_INCLUDED */
