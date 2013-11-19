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

/* $Id: string_.h 8022 2007-06-05 22:23:38Z giles $ */
/* Generic substitute for Unix string.h */

#ifndef string__INCLUDED
#  define string__INCLUDED

/* We must include std.h before any file that includes sys/types.h. */
#include "std.h"

#ifdef BSD4_2
#  include <strings.h>
#  define strchr index
#else
#  ifdef MEMORY__NEED_MEMMOVE
#    undef memmove		/* This is disgusting, but so is GCC */
#  endif
#  include <string.h>
#  if defined(THINK_C)
	/* Patch strlen to return a uint rather than a size_t. */
#    define strlen (uint)strlen
#  endif
#  ifdef MEMORY__NEED_MEMMOVE
#    define memmove(dest,src,len) gs_memmove(dest,src,len)
#  endif
#endif

#endif /* string__INCLUDED */
