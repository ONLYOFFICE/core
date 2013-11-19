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

/* $Id: zchar42.h 9222 2008-11-16 14:47:49Z leonardo $ */

#ifndef zchar42_INCLUDED
#  define zchar42_INCLUDED

/* Get a Type 42 character metrics and set the cache device. */
int zchar42_set_cache(i_ctx_t *i_ctx_p, gs_font_base *pbfont, ref *cnref, 
	    uint glyph_index, op_proc_t cont, op_proc_t *exec_cont);

#endif /* zchar42_INCLUDED */
