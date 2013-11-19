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

/* $Id: gxdcconv.h 8022 2007-06-05 22:23:38Z giles $ */
/* Internal device color conversion interfaces */

#ifndef gxdcconv_INCLUDED
#  define gxdcconv_INCLUDED

#include "gxfrac.h"

/* Color space conversion routines */
frac color_rgb_to_gray(frac r, frac g, frac b,
		       const gs_imager_state * pis);
void color_rgb_to_cmyk(frac r, frac g, frac b,
		       const gs_imager_state * pis, frac cmyk[4]);
frac color_cmyk_to_gray(frac c, frac m, frac y, frac k,
			const gs_imager_state * pis);
void color_cmyk_to_rgb(frac c, frac m, frac y, frac k,
		       const gs_imager_state * pis, frac rgb[3]);

#endif /* gxdcconv_INCLUDED */
