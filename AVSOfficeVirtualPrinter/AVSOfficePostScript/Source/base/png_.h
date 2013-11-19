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

/* $Id: png_.h 8022 2007-06-05 22:23:38Z giles $ */
/* Wrapper for png.h */

#ifndef png__INCLUDED
#  define png__INCLUDED

#if SHARE_LIBPNG
#include <png.h>
#else
#include "png.h"
#endif

#endif /* png__INCLUDED */
