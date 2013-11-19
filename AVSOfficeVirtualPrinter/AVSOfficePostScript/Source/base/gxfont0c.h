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

/* $Id: gxfont0c.h 8022 2007-06-05 22:23:38Z giles $ */
/* Interface to gsfont0c.c */

#ifndef gxfont0c_INCLUDED
#  define gxfont0c_INCLUDED

#include "gxfont0.h"
#include "gxfcid.h"

/* Exported by gsfont0c.c */

/*
 * Create a Type 0 font wrapper for a CIDFont.
 * psmat == NULL means use the identity matrix.
 */
int gs_font_type0_from_cidfont(gs_font_type0 **ppfont0, gs_font *font,
			       int wmode, const gs_matrix *psmat,
			       gs_memory_t *mem);

/*
 * Create a Type 0 font wrapper for a Type 42 font (converted to a Type 2
 * CIDFont), optionally using the TrueType cmap as the CMap.
 * See gs_cmap_from_type42_cmap below for details.
 */
int gs_font_type0_from_type42(gs_font_type0 **ppfont0, gs_font_type42 *pfont42,
			      int wmode, bool use_cmap, gs_memory_t *mem);

/* Exported by gsfcid2.c */

/*
 * Create a Type 2 CIDFont from a Type 42 font.
 */
int gs_font_cid2_from_type42(gs_font_cid2 **ppfcid, gs_font_type42 *pfont42,
			     int wmode, gs_memory_t *mem);

/*
 * Create a CMap from a TrueType cmap (Platform 3, Encoding 1, Format 4 only).
 * The resulting CMap is marked as being Unicode-based.
 */
int gs_cmap_from_type42_cmap(gs_cmap_t **ppcmap, gs_font_type42 *pfont42,
			     int wmode, gs_memory_t *mem);

#endif /* gxfont0c_INCLUDED */
