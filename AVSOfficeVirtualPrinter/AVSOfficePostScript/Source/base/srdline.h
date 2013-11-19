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

/* $Id: srdline.h 8022 2007-06-05 22:23:38Z giles $ */
/* Interface for readline */
/* Requires gsmemory.h, gstypes.h */

#ifndef srdline_INCLUDED
#  define srdline_INCLUDED

/*
 * Read a line from s_in, starting at index *pcount in buf.  Start by
 * printing prompt on s_out.  If the string is longer than size - 1 (we need
 * 1 extra byte at the end for a null or an EOL), use bufmem to reallocate
 * buf; if bufmem is NULL, just return 1.  In any case, store in *pcount the
 * first unused index in buf.  *pin_eol is normally false; it should be set
 * to true (and true should be recognized) to indicate that the last
 * character read was a ^M, which should cause a following ^J to be
 * discarded.  is_stdin(s) returns true iff s is stdin: this is needed for
 * an obscure condition in the default implementation.
 */
#ifndef stream_DEFINED
#  define stream_DEFINED
typedef struct stream_s stream;
#endif
#define sreadline_proc(proc)\
  int proc(stream *s_in, stream *s_out, void *readline_data,\
	   gs_const_string *prompt, gs_string *buf,\
	   gs_memory_t *bufmem, uint *pcount, bool *pin_eol,\
	   bool (*is_stdin)(const stream *))

/* Declare the default implementation. */
extern sreadline_proc(sreadline);

#endif /* srdline_INCLUDED */
