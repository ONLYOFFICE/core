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

/* $Id: iparray.h 9043 2008-08-28 22:48:19Z giles $ */
/* Packed array constructor for Ghostscript */
/* Requires ipacked.h, istack.h */

#ifndef iparray_INCLUDED
#  define iparray_INCLUDED

/*
 * The only reason to put this in a separate header is that it requires
 * both ipacked.h and istack.h; putting it in either one would make it
 * depend on the other one.  There must be a better way....
 */

/* Procedures implemented in zpacked.c */

/* Make a packed array from the top N elements of a stack. */
int make_packed_array(ref *, ref_stack_t *, uint, gs_dual_memory_t *,
		      client_name_t);

#endif /* iparray_INCLUDED */
