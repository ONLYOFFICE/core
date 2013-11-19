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

/* $Id: istkparm.h 9043 2008-08-28 22:48:19Z giles $ */
/* Parameter structure for expandable stacks of refs */

#ifndef istkparm_INCLUDED
#  define istkparm_INCLUDED

/*
 * Define the structure for stack parameters set at initialization.
 */
/*typedef struct ref_stack_params_s ref_stack_params_t;*/ /* in istack.h */
struct ref_stack_params_s {
    uint bot_guard;		/* # of guard elements below bot */
    uint top_guard;		/* # of guard elements above top */
    uint block_size;		/* size of each block */
    uint data_size;		/* # of data slots in each block */
    ref guard_value;		/* t__invalid or t_operator, */
				/* bottom guard value */
    int underflow_error;	/* error code for underflow */
    int overflow_error;		/* error code for overflow */
    bool allow_expansion;	/* if false, don't expand */
};
#define private_st_ref_stack_params() /* in istack.c */\
  gs_private_st_simple(st_ref_stack_params, ref_stack_params_t,\
    "ref_stack_params_t")

#endif /* istkparm_INCLUDED */
