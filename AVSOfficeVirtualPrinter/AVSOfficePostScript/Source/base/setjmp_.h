/* Copyright (C) 2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: setjmp_.h 6651 2006-03-13 16:18:19Z raph $ */
/* Macros for making sure setjmp buffers are correctly aligned. */

#ifndef setjmp_INCLUDED
#  define setjmp_INCLUDED

#include <setjmp.h>

typedef struct {
    char c;
    jmp_buf j;
} gsfix_jmp_buf_test;

#define gsfix_jmp_buf_align ((size_t)&((gsfix_jmp_buf_test*)0)->j)

typedef struct {
    unsigned char stuff[sizeof(jmp_buf) + gsfix_jmp_buf_align];
} gsfix_jmp_buf;


/* This could be moved into a function, but it's fairly harmless as a macro. */
#define find_jmp_buf(gsfjb) 				\
  (*(jmp_buf *)(					\
   ((size_t)(gsfjb).stuff + gsfix_jmp_buf_align) 	\
   & ~(size_t)(gsfix_jmp_buf_align-1)			\
  ))

#endif /* setjmp_INCLUDED */

