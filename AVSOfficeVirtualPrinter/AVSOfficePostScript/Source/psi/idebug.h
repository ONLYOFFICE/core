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

/* $Id: idebug.h 9043 2008-08-28 22:48:19Z giles $ */
/* Prototypes for debugging procedures in idebug.c */

#ifndef idebug_INCLUDED
#  define idebug_INCLUDED

/* Print individual values. */
void debug_print_name(const gs_memory_t *mem, const ref *);
void debug_print_name_index(const gs_memory_t *mem, uint /*name_index_t*/);
void debug_print_ref(const gs_memory_t *mem, const ref *);
void debug_print_ref_packed(const gs_memory_t *mem, const ref_packed *);

/* Dump regions of memory. */
void debug_dump_one_ref(const gs_memory_t *mem, const ref *);
void debug_dump_refs(const gs_memory_t *mem, 
		     const ref * from, uint size, const char *msg);
void debug_dump_array(const gs_memory_t *mem, const ref * array);

/* Dump a stack.  Using this requires istack.h. */
#ifndef ref_stack_DEFINED
typedef struct ref_stack_s ref_stack_t;	/* also defined in isdata.h */
#  define ref_stack_DEFINED
#endif
void debug_dump_stack(const gs_memory_t *mem, 
		      const ref_stack_t * pstack, const char *msg);

#endif /* idebug_INCLUDED */
