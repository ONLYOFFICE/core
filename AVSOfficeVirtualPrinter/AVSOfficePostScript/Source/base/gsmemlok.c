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

/* $Id: gsmemlok.c 8721 2008-05-09 02:18:14Z ray $ */
/* Monitor-locked heap memory allocator */

/* Initial version 2/1/98 by John Desrosiers (soho@crl.com) */
/* Revised 8/6/98 by L. Peter Deutsch (ghost@aladdin.com) for changes */
/*   in memory manager API */
/* Edited 3/23/1999 by L. Peter Deutsch to remove compiler warnings. */

#include "gx.h"
#include "gsmemlok.h"
#include "gserrors.h"

/* Raw memory procedures */
static gs_memory_proc_alloc_bytes(gs_locked_alloc_bytes_immovable);
static gs_memory_proc_resize_object(gs_locked_resize_object);
static gs_memory_proc_free_object(gs_locked_free_object);
static gs_memory_proc_stable(gs_locked_stable);
static gs_memory_proc_status(gs_locked_status);
static gs_memory_proc_free_all(gs_locked_free_all);
static gs_memory_proc_consolidate_free(gs_locked_consolidate_free);

/* Object memory procedures */
static gs_memory_proc_alloc_bytes(gs_locked_alloc_bytes);
static gs_memory_proc_alloc_struct(gs_locked_alloc_struct);
static gs_memory_proc_alloc_struct(gs_locked_alloc_struct_immovable);
static gs_memory_proc_alloc_byte_array(gs_locked_alloc_byte_array);
static gs_memory_proc_alloc_byte_array(gs_locked_alloc_byte_array_immovable);
static gs_memory_proc_alloc_struct_array(gs_locked_alloc_struct_array);
static gs_memory_proc_alloc_struct_array(gs_locked_alloc_struct_array_immovable);
static gs_memory_proc_object_size(gs_locked_object_size);
static gs_memory_proc_object_type(gs_locked_object_type);
static gs_memory_proc_alloc_string(gs_locked_alloc_string);
static gs_memory_proc_alloc_string(gs_locked_alloc_string_immovable);
static gs_memory_proc_resize_string(gs_locked_resize_string);
static gs_memory_proc_free_string(gs_locked_free_string);
static gs_memory_proc_register_root(gs_locked_register_root);
static gs_memory_proc_unregister_root(gs_locked_unregister_root);
static gs_memory_proc_enable_free(gs_locked_enable_free);
static const gs_memory_procs_t locked_procs =
{
    /* Raw memory procedures */
    gs_locked_alloc_bytes_immovable,
    gs_locked_resize_object,
    gs_locked_free_object,
    gs_locked_stable,
    gs_locked_status,
    gs_locked_free_all,
    gs_locked_consolidate_free,
    /* Object memory procedures */
    gs_locked_alloc_bytes,
    gs_locked_alloc_struct,
    gs_locked_alloc_struct_immovable,
    gs_locked_alloc_byte_array,
    gs_locked_alloc_byte_array_immovable,
    gs_locked_alloc_struct_array,
    gs_locked_alloc_struct_array_immovable,
    gs_locked_object_size,
    gs_locked_object_type,
    gs_locked_alloc_string,
    gs_locked_alloc_string_immovable,
    gs_locked_resize_string,
    gs_locked_free_string,
    gs_locked_register_root,
    gs_locked_unregister_root,
    gs_locked_enable_free
};

/* ---------- Public constructors/destructors ---------- */

/* Initialize a gs_memory_locked_t */
int				/* -ve error code or 0 */
gs_memory_locked_init(
		      gs_memory_locked_t * lmem,	/* allocator to init */
		      gs_memory_t * target	/* allocator to monitor lock */
)
{
    lmem->stable_memory = 0;
    lmem->procs = locked_procs;
    lmem->target = target;
    lmem->gs_lib_ctx = target->gs_lib_ctx;
    lmem->non_gc_memory = (gs_memory_t *)lmem;

    /* Allocate a monitor to serialize access to structures within */
    lmem->monitor = gx_monitor_alloc(target);
    return (lmem->monitor ? 0 : gs_note_error(gs_error_VMerror));
}

/* Release a locked memory manager. */
/* Note that this has no effect on the target. */
void
gs_memory_locked_release(gs_memory_locked_t *lmem)
{
    gs_memory_free_all((gs_memory_t *)lmem, FREE_ALL_STRUCTURES,
		       "gs_memory_locked_release");
    gx_monitor_free(lmem->monitor);
}

/* ---------- Accessors ------------- */

/* Retrieve this allocator's target */
gs_memory_t *
gs_memory_locked_target(const gs_memory_locked_t *lmem)
{
    return lmem->target;
}

/* -------- Private members just wrap a monitor around a gs_memory_heap --- */

/*
 * Contrary to our usual practice, we don't use BEGIN/END here, because
 * that causes some compilers to give bogus error messages.
 */

#define DO_MONITORED(call_target)\
	gs_memory_locked_t * const lmem = (gs_memory_locked_t *)mem;\
\
	gx_monitor_enter(lmem->monitor);\
	call_target;\
	gx_monitor_leave(lmem->monitor)

#define RETURN_MONITORED(result_type, call_target)\
	gs_memory_locked_t * const lmem = (gs_memory_locked_t *)mem;\
	result_type temp;\
\
	gx_monitor_enter(lmem->monitor);\
	temp = call_target;\
	gx_monitor_leave(lmem->monitor);\
	return temp

/* Procedures */
static void
gs_locked_free_all(gs_memory_t * mem, uint free_mask, client_name_t cname)
{
    gs_memory_locked_t * const lmem = (gs_memory_locked_t *)mem;
    gs_memory_t * const target = lmem->target;

    /* Only free the structures and the allocator itself. */
    if (mem->stable_memory) {
	if (mem->stable_memory != mem)
	    gs_memory_free_all(mem->stable_memory, free_mask, cname);
	if (free_mask & FREE_ALL_ALLOCATOR)
	    mem->stable_memory = 0;
    }
    if (free_mask & FREE_ALL_STRUCTURES) {
	/*
	 * Check for monitor == 0, in case this is called after a
	 * failure during initialization.
	 */
	if (lmem->monitor)
	    gx_monitor_free(lmem->monitor);
	lmem->monitor = 0;
	lmem->target = 0;
    }
    if (free_mask & FREE_ALL_ALLOCATOR)
	gs_free_object(target, lmem, cname);
}
static void
gs_locked_consolidate_free(gs_memory_t * mem)
{
    DO_MONITORED(
		 (*lmem->target->procs.consolidate_free)(lmem->target)
		 );
}
static byte *
gs_locked_alloc_bytes(gs_memory_t * mem, uint size, client_name_t cname)
{
    RETURN_MONITORED(
		     byte *,
		     (*lmem->target->procs.alloc_bytes)
		       (lmem->target, size, cname)
		     );
}
static byte *
gs_locked_alloc_bytes_immovable(gs_memory_t * mem, uint size,
				client_name_t cname)
{
    RETURN_MONITORED(
		     byte *,
		     (*lmem->target->procs.alloc_bytes_immovable)
		       (lmem->target, size, cname)
		     );
}
static void *
gs_locked_alloc_struct(gs_memory_t * mem, gs_memory_type_ptr_t pstype,
		       client_name_t cname)
{
    RETURN_MONITORED(
		     void *,
		     (*lmem->target->procs.alloc_struct)
		       (lmem->target, pstype, cname)
		     );
}
static void *
gs_locked_alloc_struct_immovable(gs_memory_t * mem,
			   gs_memory_type_ptr_t pstype, client_name_t cname)
{
    RETURN_MONITORED(
		     void *,
		     (*lmem->target->procs.alloc_struct_immovable)
		       (lmem->target, pstype, cname)
		     );
}
static byte *
gs_locked_alloc_byte_array(gs_memory_t * mem, uint num_elements, uint elt_size,
			   client_name_t cname)
{
    RETURN_MONITORED(
		     byte *,
		     (*lmem->target->procs.alloc_byte_array)
		       (lmem->target, num_elements, elt_size, cname)
		     );
}
static byte *
gs_locked_alloc_byte_array_immovable(gs_memory_t * mem, uint num_elements,
				     uint elt_size, client_name_t cname)
{
    RETURN_MONITORED(
		     byte *,
		     (*lmem->target->procs.alloc_byte_array_immovable)
		       (lmem->target, num_elements, elt_size, cname)
		     );
}
static void *
gs_locked_alloc_struct_array(gs_memory_t * mem, uint num_elements,
			   gs_memory_type_ptr_t pstype, client_name_t cname)
{
    RETURN_MONITORED(
		     void *,
		     (*lmem->target->procs.alloc_struct_array)
		       (lmem->target, num_elements, pstype, cname)
		     );
}
static void *
gs_locked_alloc_struct_array_immovable(gs_memory_t * mem, uint num_elements,
			   gs_memory_type_ptr_t pstype, client_name_t cname)
{
    RETURN_MONITORED(
		     void *,
		     (*lmem->target->procs.alloc_struct_array_immovable)
		       (lmem->target, num_elements, pstype, cname)
		     );
}
static void *
gs_locked_resize_object(gs_memory_t * mem, void *obj, uint new_num_elements,
			client_name_t cname)
{
    RETURN_MONITORED(
		     void *,
		     (*lmem->target->procs.resize_object)
		       (lmem->target, obj, new_num_elements, cname)
		     );
}
static uint
gs_locked_object_size(gs_memory_t * mem, const void *ptr)
{
    RETURN_MONITORED(
		     uint,
		     (*lmem->target->procs.object_size)
		       (lmem->target, ptr)
		     );
}
static gs_memory_type_ptr_t
gs_locked_object_type(const gs_memory_t * mem, const void *ptr)
{
    RETURN_MONITORED(
		     gs_memory_type_ptr_t,
		     (*lmem->target->procs.object_type)
		       (lmem->target, ptr)
		     );
}
static void
gs_locked_free_object(gs_memory_t * mem, void *ptr, client_name_t cname)
{
    DO_MONITORED(
		 (*lmem->target->procs.free_object)
	           (lmem->target, ptr, cname)
		 );
}
static byte *
gs_locked_alloc_string(gs_memory_t * mem, uint nbytes, client_name_t cname)
{
    RETURN_MONITORED(
		     byte *,
		     (*lmem->target->procs.alloc_string)
		       (lmem->target, nbytes, cname)
		     );
}
static byte *
gs_locked_alloc_string_immovable(gs_memory_t * mem, uint nbytes,
				 client_name_t cname)
{
    RETURN_MONITORED(
		     byte *,
		     (*lmem->target->procs.alloc_string_immovable)
		       (lmem->target, nbytes, cname)
		     );
}
static byte *
gs_locked_resize_string(gs_memory_t * mem, byte * data, uint old_num,
			uint new_num,
			client_name_t cname)
{
    RETURN_MONITORED(
		     byte *,
		     (*lmem->target->procs.resize_string)
		       (lmem->target, data, old_num, new_num, cname)
		     );
}
static void
gs_locked_free_string(gs_memory_t * mem, byte * data, uint nbytes,
		      client_name_t cname)
{
    DO_MONITORED(
		 (*lmem->target->procs.free_string)
		   (lmem->target, data, nbytes, cname)
		 );
}
static int
gs_locked_register_root(gs_memory_t * mem, gs_gc_root_t * rp,
			gs_ptr_type_t ptype, void **up, client_name_t cname)
{
    RETURN_MONITORED(
		     int,
		     (*lmem->target->procs.register_root)
		       (lmem->target, rp, ptype, up, cname)
		     );
}
static void
gs_locked_unregister_root(gs_memory_t * mem, gs_gc_root_t * rp,
			  client_name_t cname)
{
    DO_MONITORED(
		 (*lmem->target->procs.unregister_root)
		   (lmem->target, rp, cname)
		 );
}
static gs_memory_t *
gs_locked_stable(gs_memory_t * mem)
{
    if (!mem->stable_memory) {
	gs_memory_locked_t * const lmem = (gs_memory_locked_t *)mem;
	gs_memory_t *stable;

	gx_monitor_enter(lmem->monitor);
	stable = gs_memory_stable(lmem->target);
	if (stable == lmem->target)
	    mem->stable_memory = mem;
	else {
	    gs_memory_locked_t *locked_stable = (gs_memory_locked_t *)
		gs_alloc_bytes(stable, sizeof(*lmem), "gs_locked_stable");

	    if (locked_stable) {
		int code = gs_memory_locked_init(locked_stable, stable);

		if (code < 0)
		    gs_free_object(stable, locked_stable, "gs_locked_stable");
		else
		    mem->stable_memory = (gs_memory_t *)locked_stable;
	    }
	}
	gx_monitor_leave(lmem->monitor);
    }
    return mem->stable_memory;
}
static void
gs_locked_status(gs_memory_t * mem, gs_memory_status_t * pstat)
{
    DO_MONITORED(
		 (*lmem->target->procs.status)(lmem->target, pstat)
		 );
}
static void
gs_locked_enable_free(gs_memory_t * mem, bool enable)
{
    DO_MONITORED(
		 (*lmem->target->procs.enable_free)(lmem->target, enable)
		 );
}
