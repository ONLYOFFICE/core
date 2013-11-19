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

/* $Id: malloc_.h 8022 2007-06-05 22:23:38Z giles $ */
/* Generic substitute for Unix malloc.h */

#ifndef malloc__INCLUDED
#  define malloc__INCLUDED

/* We must include std.h before any file that includes sys/types.h. */
#include "std.h"

#ifdef __TURBOC__
#  include <alloc.h>
#else
#  if defined(BSD4_2) || defined(apollo) || defined(vax) || defined(sequent) || defined(UTEK)
#    if defined(_POSIX_SOURCE) || (defined(__STDC__) && (!defined(sun) || defined(__svr4__)))	/* >>> */
#      include <stdlib.h>
#    else			/* Ancient breakage */
extern char *malloc();
extern void free();

#    endif
#  else
#    if defined(_HPUX_SOURCE) || defined(__CONVEX__) || defined(__convex__) || defined(__OSF__) || defined(__386BSD__) || defined(_POSIX_SOURCE) || defined(__STDC__) || defined(VMS)
#      include <stdlib.h>
#    else
#      include <malloc.h>
#    endif			/* !_HPUX_SOURCE, ... */
#  endif			/* !BSD4_2, ... */
#endif /* !__TURBOC__ */

/* (At least some versions of) Linux don't have a working realloc.... */
#ifdef linux
#  define malloc__need_realloc
void *gs_realloc(void *, size_t, size_t);

#else
#  define gs_realloc(ptr, old_size, new_size) realloc(ptr, new_size)
#endif

#endif /* malloc__INCLUDED */
