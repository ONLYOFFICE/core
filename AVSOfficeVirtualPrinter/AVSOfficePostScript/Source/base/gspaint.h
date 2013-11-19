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

/* $Id: gspaint.h 8022 2007-06-05 22:23:38Z giles $ */
/* Erase/fill/stroke procedures */
/* Requires gsstate.h */

#ifndef gspaint_INCLUDED
#  define gspaint_INCLUDED

/* Painting */
int gs_erasepage(gs_state *),
    gs_fillpage(gs_state *),
    gs_fill(gs_state *),
    gs_eofill(gs_state *),
    gs_stroke(gs_state *);

/* Image tracing */
int gs_imagepath(gs_state *, int, int, const byte *);

#endif /* gspaint_INCLUDED */
