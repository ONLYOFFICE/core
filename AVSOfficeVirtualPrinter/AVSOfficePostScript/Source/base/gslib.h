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

/* $Id: gslib.h 8022 2007-06-05 22:23:38Z giles $ */
/* Library initialization and finalization interface */
/* Requires stdio.h, gsmemory.h */

#ifndef gslib_INCLUDED
#  define gslib_INCLUDED

/*
 * Initialize the library.  gs_lib_init does all of the initialization,
 * using the C heap for initial allocation; if a client wants the library to
 * use a different default allocator during initialization, it should call
 * gs_lib_init0 and then gs_lib_init1.
 */
int gs_lib_init(FILE * debug_out);
gs_memory_t *gs_lib_init0(FILE * debug_out);
int gs_lib_init1(gs_memory_t *);

/* Clean up after execution. */
void gs_lib_finit(int exit_status, int code, gs_memory_t *);

#endif /* gslib_INCLUDED */
