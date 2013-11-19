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

/* $Id: iosdata.h 9043 2008-08-28 22:48:19Z giles $ */
/* Generic operand stack API */

#ifndef iosdata_INCLUDED
#  define iosdata_INCLUDED

#include "isdata.h"

/* Define the operand stack structure. */
/* Currently this is just a generic ref stack. */
typedef struct op_stack_s {

    ref_stack_t stack;		/* the actual operand stack */

} op_stack_t;

#define public_st_op_stack()	/* in interp.c */\
  gs_public_st_suffix_add0(st_op_stack, op_stack_t, "op_stack_t",\
    op_stack_enum_ptrs, op_stack_reloc_ptrs, st_ref_stack)
#define st_op_stack_num_ptrs st_ref_stack_num_ptrs

#endif /* iosdata_INCLUDED */
