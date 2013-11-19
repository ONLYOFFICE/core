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

/* $Id: ifont2.h 9043 2008-08-28 22:48:19Z giles $ */
/* Type 2 font utilities 2 */

#ifndef ifont2_INCLUDED
#  define ifont2_INCLUDED

/* Default value of lenIV */
#define DEFAULT_LENIV_2 (-1)

/*
 * Get the additional parameters for a Type 2 font (or FontType 2 FDArray
 * entry in a CIDFontType 0 font), beyond those common to Type 1 and Type 2
 * fonts.
 */
int type2_font_params(const_os_ptr op, charstring_font_refs_t *pfr,
		      gs_type1_data *pdata1);

#endif /* ifont2_INCLUDED */
