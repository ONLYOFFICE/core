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

/* $Id: idosave.h 9043 2008-08-28 22:48:19Z giles $ */
/* Supporting procedures for 'save' recording. */

#ifndef idosave_INCLUDED
#  define idosave_INCLUDED

/*
 * Save a change that must be undone by restore.  We have to pass the
 * pointer to the containing object to alloc_save_change for two reasons:
 *
 *      - We need to know which VM the containing object is in, so we can
 * know on which chain of saved changes to put the new change.
 *
 *      - We need to know whether the object is an array of refs (which
 * includes dictionaries) or a struct, so we can properly trace and
 * relocate the pointer to it from the change record during garbage
 * collection.
 */
int alloc_save_change(gs_dual_memory_t *dmem, const ref *pcont,
		      ref_packed *ptr, client_name_t cname);
int alloc_save_change_in(gs_ref_memory_t *mem, const ref *pcont,
			 ref_packed *ptr, client_name_t cname);
/* Remove an AC_OFFSET_ALLOCATED element. */
void alloc_save_remove(gs_ref_memory_t *mem, ref_packed *obj, client_name_t cname);
/* Allocate a structure for recording an allocation event. */
int alloc_save_change_alloc(gs_ref_memory_t *mem, client_name_t cname, ref_packed ***ppr);

#endif /* idosave_INCLUDED */
