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

/* $Id: gxlum.h 8022 2007-06-05 22:23:38Z giles $ */
/* Luminance computation parameters for Ghostscript */

#ifndef gxlum_INCLUDED
#  define gxlum_INCLUDED

/* Color weights used for computing luminance. */
#define lum_red_weight	30
#define lum_green_weight	59
#define lum_blue_weight	11
#define lum_all_weights	(lum_red_weight + lum_green_weight + lum_blue_weight)

#endif /* gxlum_INCLUDED */
