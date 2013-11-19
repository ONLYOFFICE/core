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

/* $Id: gsmchunk.c 8928 2008-08-03 12:26:41Z leonardo $ */
/* chunk consolidating wrapper on a base memory allocator */

#include "memory_.h"
#include "gx.h"
#include "gsstype.h"
#include "gserrors.h"
#include "gsmchunk.h"

/* Raw memory procedures */
static gs_memory_proc_alloc_bytes(chunk_alloc_bytes_immovable);
static gs_memory_proc_resize_object(chunk_resize_object);
static gs_memory_proc_free_object(chunk_free_object);
static gs_memory_proc_stable(chunk_stable);
static gs_memory_proc_status(chunk_status);
static gs_memory_proc_free_all(chunk_free_all);
static gs_memory_proc_consolidate_free(chunk_consolidate_free);

/* Object memory procedures */
static gs_memory_proc_alloc_bytes(chunk_alloc_bytes);
static gs_memory_proc_alloc_struct(chunk_alloc_struct);
static gs_memory_proc_alloc_struct(chunk_alloc_struct_immovable);
static gs_memory_proc_alloc_byte_array(chunk_alloc_byte_array);
static gs_memory_proc_alloc_byte_array(chunk_alloc_byte_array_immovable);
static gs_memory_proc_alloc_struct_array(chunk_alloc_struct_array);
static gs_memory_proc_alloc_struct_array(chunk_alloc_struct_array_immovable);
static gs_memory_proc_object_size(chunk_object_size);
static gs_memory_proc_object_type(chunk_object_type);
static gs_memory_proc_alloc_string(chunk_alloc_string);
static gs_memory_proc_alloc_string(chunk_alloc_string_immovable);
static gs_memory_proc_resize_string(chunk_resize_string);
static gs_memory_proc_free_string(chunk_free_string);
static gs_memory_proc_register_root(chunk_register_root);
static gs_memory_proc_unregister_root(chunk_unregister_root);
static gs_memory_proc_enable_free(chunk_enable_free);
static const gs_memory_procs_t chunk_procs =
{
    /* Raw memory procedures */
    chunk_alloc_bytes_immovable,
    chunk_resize_object,
    chunk_free_object,
    chunk_stable,
    chunk_status,
    chunk_free_all,
    chunk_consolidate_free,
    /* Object memory procedures */
    chunk_alloc_bytes,
    chunk_alloc_struct,
    chunk_alloc_struct_immovable,
    chunk_alloc_byte_array,
    chunk_alloc_byte_array_immovable,
    chunk_alloc_struct_array,
    chunk_alloc_struct_array_immovable,
    chunk_object_size,
    chunk_object_type,
    chunk_alloc_string,
    chunk_alloc_string_immovable,
    chunk_resize_string,
    chunk_free_string,
    chunk_register_root,
    chunk_unregister_root,
    chunk_enable_free
};

typedef struct chunk_obj_node_s {
    struct chunk_obj_node_s *next;
    uint size;			/* objlist: client size */
    	/* if freelist: size of block (obj header and client area must fit in block) */
    gs_memory_type_ptr_t type;
#ifdef DEBUG
    unsigned long sequence;
#endif
} chunk_obj_node_t;

/*
 * Note: All objects within a chunk are 'aligned' since we round_up_to_align
 * the free list pointer when removing part of a free area.
 */
typedef struct chunk_mem_node_s {
    uint size;
    uint largest_free;		/* quick check when allocating */
    struct chunk_mem_node_s *next;
    chunk_obj_node_t *objlist;	/* head of objects in this chunk (no order) */
    chunk_obj_node_t *freelist;		/* free list (ordered) */
    /* chunk data follows immediately */
} chunk_mem_node_t;

typedef struct gs_memory_chunk_s {
    gs_memory_common;		/* interface outside world sees */
    gs_memory_t *target;	/* base allocator */
    chunk_mem_node_t *head_chunk;
#ifdef DEBUG
    unsigned long sequence_counter;
#endif
} gs_memory_chunk_t;

/* ---------- Public constructors/destructors ---------- */

/* Initialize a gs_memory_chunk_t */
int				/* -ve error code or 0 */
gs_memory_chunk_wrap( gs_memory_t **wrapped,	/* chunk allocator init */
		      gs_memory_t * target )	/* base allocator */
{
    /* Use the non-GC allocator of the target. The chunk allocator is NOT GC safe */
    gs_memory_t *non_gc_target = target->non_gc_memory;
    gs_memory_chunk_t *cmem = NULL;

    *wrapped = NULL;		/* don't leave garbage in case we fail */
    if (non_gc_target) 
	    cmem = (gs_memory_chunk_t *) gs_alloc_bytes_immovable(non_gc_target,
			sizeof(gs_memory_chunk_t), "gs_malloc_wrap(chunk)");
    if (cmem == 0)
        return_error(gs_error_VMerror);
    cmem->stable_memory = (gs_memory_t *)cmem;	/* we are stable */
    cmem->procs = chunk_procs;
    cmem->gs_lib_ctx = non_gc_target->gs_lib_ctx;
    cmem->non_gc_memory = (gs_memory_t *)cmem;	/* and are not subject to GC */
    cmem->target = non_gc_target;
    cmem->head_chunk = NULL;
#ifdef DEBUG
    cmem->sequence_counter = 0;
#endif

    /* Init the chunk management values */

    *wrapped = (gs_memory_t *)cmem;
    return 0;
}

/* Release a chunk memory manager. */
/* Note that this has no effect on the target. */
void
gs_memory_chunk_release(gs_memory_t *mem)
{
    gs_memory_free_all((gs_memory_t *)mem, FREE_ALL_EVERYTHING,
		       "gs_memory_chunk_release");
}

/* ---------- Accessors ------------- */

/* Retrieve this allocator's target */
gs_memory_t *
gs_memory_chunk_target(const gs_memory_t *mem)
{
    gs_memory_chunk_t *cmem = (gs_memory_chunk_t *)mem;
    return cmem->target;
}

#ifdef DEBUG
void
gs_memory_chunk_dump_memory(const gs_memory_t *mem)
{
    gs_memory_chunk_t *cmem = (gs_memory_chunk_t *)mem;
    chunk_mem_node_t *head = cmem->head_chunk;
    chunk_mem_node_t *current;
    chunk_mem_node_t *next;

    current = head;
    while ( current != NULL ) { 
	if (current->objlist != NULL) {
	    chunk_obj_node_t *obj;

	    for (obj= current->objlist; obj != NULL; obj=obj->next) 
		dprintf4("chunk_mem leak, obj=0x%lx, size=%d, type=%s, sequence#=%ld\n",
			(ulong)obj, obj->size, obj->type->sname, obj->sequence);
	}
	next = current->next; 
	current = next;
    }
}
#endif

/* -------- Private members --------- */

/* Note that all of the data is 'immovable' and is opaque to the base allocator */
/* thus even if it is a GC type of allocator, no GC functions will be applied   */
/* All allocations are done in the non_gc_memory of the base */

/* Procedures */

static void
chunk_mem_node_free_all_remaining(gs_memory_chunk_t *cmem)
{
    chunk_mem_node_t *head = cmem->head_chunk;
    gs_memory_t * const target = cmem->target;
    chunk_mem_node_t *current;
    chunk_mem_node_t *next;

    current = head;
    while ( current != NULL ) { 
	next = current->next; 
	gs_free_object(target, current, "chunk_mem_node_remove");
	current = next;
    }
    cmem->head_chunk = NULL;
}

static void
chunk_free_all(gs_memory_t * mem, uint free_mask, client_name_t cname)
{
    gs_memory_chunk_t * const cmem = (gs_memory_chunk_t *)mem;
    gs_memory_t * const target = cmem->target;

    /* Only free the structures and the allocator itself. */
    if (mem->stable_memory) {
	if (mem->stable_memory != mem)
	    gs_memory_free_all(mem->stable_memory, free_mask, cname);
	if (free_mask & FREE_ALL_ALLOCATOR)
	    mem->stable_memory = 0;
    }
    if (free_mask & FREE_ALL_DATA) {
	chunk_mem_node_free_all_remaining(cmem);
    }
    if (free_mask & FREE_ALL_STRUCTURES) {
	cmem->target = 0;
    }
    if (free_mask & FREE_ALL_ALLOCATOR)
	gs_free_object(target, cmem, cname);
}

extern const gs_memory_struct_type_t st_bytes;

/* round up objects to make sure we have room for a header left */
inline static uint
round_up_to_align(uint size)
{
    uint num_node_headers = (size + sizeof(chunk_obj_node_t) - 1) / sizeof(chunk_obj_node_t);

    return num_node_headers * sizeof(chunk_obj_node_t);
}

#define IS_SINGLE_OBJ_SIZE(chunk_size) \
    (chunk_size > (CHUNK_SIZE>>1))
#define MULTIPLE_OBJ_CHUNK_SIZE \
    (sizeof(chunk_mem_node_t) + round_up_to_align(CHUNK_SIZE))


/* return -1 on error, 0 on success */
static int 
chunk_mem_node_add(gs_memory_chunk_t *cmem, uint size_needed, chunk_mem_node_t **newchunk)
{
    chunk_mem_node_t *node, *prev_node;
    gs_memory_t *target = cmem->target;
    /* Allocate enough for the chunk header, and the size_needed */
    /* The size needed already includes the object header from caller */
    /* and is already rounded up to the obj_node_t sized elements */
    uint chunk_size = size_needed + sizeof(chunk_mem_node_t);
    bool is_multiple_object_node = false;

    /* Objects > half the default chunk size get their own chunk */
    if ( ! IS_SINGLE_OBJ_SIZE(chunk_size)) {
	chunk_size = MULTIPLE_OBJ_CHUNK_SIZE;	/* the size for collections of objects */
	is_multiple_object_node = true;
    }

    *newchunk = NULL;
    node = (chunk_mem_node_t *)gs_alloc_bytes_immovable(target, chunk_size, "chunk_mem_node_add");
    if ( node == NULL )
        return -1;
    node->size = chunk_size;	/* how much we allocated */
    node->largest_free = chunk_size - sizeof(chunk_mem_node_t);
    node->objlist = NULL;
    node->freelist = (chunk_obj_node_t *)((byte *)(node) + sizeof(chunk_mem_node_t));
    node->freelist->next = NULL;
    node->freelist->size = node->largest_free;

    prev_node = NULL;
    if (!is_multiple_object_node) {
        chunk_mem_node_t *scan_node;

	/* Scan past chunks that are collections of smaller chunks */
	/* This allows the most frequently accessed chunks to be near the head */
	for (scan_node = cmem->head_chunk; scan_node != NULL; scan_node = scan_node->next) {
	    if (scan_node->size != MULTIPLE_OBJ_CHUNK_SIZE)
		break;
	    prev_node = scan_node;
	}
    }
    if (prev_node == NULL) {
	if (cmem->head_chunk == NULL) {
	    cmem->head_chunk = node;
	    node->next = NULL;
	} else {
	    node->next = cmem->head_chunk;
	    cmem->head_chunk = node;
	}
    } else {
	node->next = prev_node->next;
	prev_node->next = node;
    }

    *newchunk = node;	    /* return the chunk we just allocated */
    return 0;
}

static int        
chunk_mem_node_remove(gs_memory_chunk_t *cmem, chunk_mem_node_t *addr)
{
    chunk_mem_node_t *head = cmem->head_chunk;
    gs_memory_t * const target = cmem->target;

    /* check the head first */
    if (head == NULL) {
        dprintf("FAIL - no nodes to be removed\n" );
        return -1;
    }
    if (head == addr) {
        cmem->head_chunk = head->next;
	gs_free_object(target, head, "chunk_mem_node_remove");
    } else {
	chunk_mem_node_t *current;
        bool found = false;

        /* scan the list, stopping in front of element */
        for (current = head; current != NULL; current = current->next) {
            if ( current->next && (current->next == addr) ) {
                current->next = current->next->next;	/* de-link it */
		gs_free_object(target, addr, "chunk_mem_node_remove");
                found = true;
                break;
            }
        }
        if ( !found ) {
            dprintf1("FAIL freeing wild pointer freed address 0x%lx not found\n", (ulong)addr );
	    return -1;
	}
    }
    return 0;
}
    
/* all of the allocation routines reduce to the this function */
static byte *
chunk_obj_alloc(gs_memory_t *mem, uint size, gs_memory_type_ptr_t type, client_name_t cname)
{
    gs_memory_chunk_t *cmem = (gs_memory_chunk_t *)mem;
    chunk_mem_node_t *head = cmem->head_chunk;
    uint newsize, free_size;
    chunk_obj_node_t *newobj = NULL;
    chunk_obj_node_t *free_obj, *prev_free, *new_free;
    chunk_mem_node_t *current;
    bool rescan_free_list;

    newsize = round_up_to_align(size + sizeof(chunk_obj_node_t));	/* space we will need */
    
    /* Search the chunks for one with a large enough free area */
    for (current = head; current != NULL; current = current->next) {
	if ( current->largest_free >= newsize) 
	    break;
    }
    if (current == NULL) {
	/* No chunks with enough space, allocate one */
	if (chunk_mem_node_add(cmem, newsize, &current) < 0)
	    return NULL;
    }
    /* Find the first free area in the current chunk that is big enough */
    /* LATER: might be better to find the 'best fit' */
    prev_free = NULL;		/* NULL means chunk */
    for (free_obj = current->freelist; free_obj != NULL; free_obj=free_obj->next) {
	if (free_obj->size >= newsize) 
	    break;
	prev_free = free_obj;	/* keep track so we can update link */
    }

    if (free_obj == NULL) {
	dprintf2("largest_free value = %d is too large, cannot find room for size = %d\n",
	    current->largest_free, newsize);
	return NULL;
    }

    /* If this free object's size == largest_free, we'll have to re-scan */
    rescan_free_list = free_obj->size == current->largest_free;

    /* Make an object in the free_obj we found above, reducing it's size */
    /* and adjusting the free list preserving alignment	*/
    newobj = free_obj;
    free_size = free_obj->size - newsize;	/* amount remaining */
    new_free = (chunk_obj_node_t *)((byte *)(free_obj) + newsize);	/* start of remaining free area */
    if (free_size >= sizeof(chunk_obj_node_t)) {
	if (prev_free != NULL)
	    prev_free->next = new_free;
	else
	    current->freelist = new_free;
	new_free->next = free_obj->next;
	new_free->size = free_size;
    } else {
       /* Not enough space remaining, just skip around it */
	if (prev_free != NULL)
	    prev_free->next = free_obj->next;
	else
	    current->freelist = free_obj->next;
    }

#ifdef DEBUG
    memset((byte *)(newobj) + sizeof(chunk_obj_node_t), 0xa1, newsize - sizeof(chunk_obj_node_t));
    memset((byte *)(newobj) + sizeof(chunk_obj_node_t), 0xac, size);
    newobj->sequence = cmem->sequence_counter++;
#endif

    newobj->next = current->objlist;	/* link to start of list */
    current->objlist = newobj;
    newobj->size = size;		/* client requested size */
    newobj->type = type;		/* and client desired type */

    /* If we flagged for re-scan to find the new largest_free, do it now */
    if (rescan_free_list) {
	current->largest_free = 0;
	for (free_obj = current->freelist; free_obj != NULL; free_obj=free_obj->next)
	    if (free_obj->size > current->largest_free) 
		current->largest_free = free_obj->size;
    }

    /* return the client area of the object we allocated */
    return (byte *)(newobj) + sizeof(chunk_obj_node_t);
}

static byte *
chunk_alloc_bytes_immovable(gs_memory_t * mem, uint size, client_name_t cname)
{
    return chunk_obj_alloc(mem, size, &st_bytes, cname);
}

static byte *
chunk_alloc_bytes(gs_memory_t * mem, uint size, client_name_t cname)
{
    return chunk_obj_alloc(mem, size, &st_bytes, cname);
}

static void *
chunk_alloc_struct_immovable(gs_memory_t * mem, gs_memory_type_ptr_t pstype,
			 client_name_t cname)
{
    return chunk_obj_alloc(mem, pstype->ssize, pstype, cname);
}

static void *
chunk_alloc_struct(gs_memory_t * mem, gs_memory_type_ptr_t pstype,
	       client_name_t cname)
{
    return chunk_obj_alloc(mem, pstype->ssize, pstype, cname);
}

static byte *
chunk_alloc_byte_array_immovable(gs_memory_t * mem, uint num_elements,
			     uint elt_size, client_name_t cname)
{
    return chunk_alloc_bytes(mem, num_elements * elt_size, cname);
}

static byte *
chunk_alloc_byte_array(gs_memory_t * mem, uint num_elements, uint elt_size,
		   client_name_t cname)
{
    return chunk_alloc_bytes(mem, num_elements * elt_size, cname);
}

static void *
chunk_alloc_struct_array_immovable(gs_memory_t * mem, uint num_elements,
			   gs_memory_type_ptr_t pstype, client_name_t cname)
{
    return chunk_obj_alloc(mem, num_elements * pstype->ssize, pstype, cname);
}

static void *
chunk_alloc_struct_array(gs_memory_t * mem, uint num_elements,
		     gs_memory_type_ptr_t pstype, client_name_t cname)
{
    return chunk_obj_alloc(mem, num_elements * pstype->ssize, pstype, cname);
}


static void *
chunk_resize_object(gs_memory_t * mem, void *ptr, uint new_num_elements, client_name_t cname)
{
    /* get the type from the old object */
    chunk_obj_node_t *obj = ((chunk_obj_node_t *)ptr) - 1;
    uint new_size = (obj->type->ssize * new_num_elements);

    /* This isn't particularly efficient, but it is rarely used */
    chunk_free_object(mem, ptr, cname);
    return chunk_obj_alloc(mem, new_size, obj->type, cname);
}
	
static void
chunk_free_object(gs_memory_t * mem, void *ptr, client_name_t cname)
{
    if (ptr == NULL ) 
	return;
    {
	/* back up to obj header */
	chunk_obj_node_t *obj = ((chunk_obj_node_t *)ptr) - 1;
	void (*finalize)(void *ptr) = obj->type->finalize;
	gs_memory_chunk_t * const cmem = (gs_memory_chunk_t *)mem;
	chunk_mem_node_t *current;
	chunk_obj_node_t *free_obj, *prev_free;
	chunk_obj_node_t *scan_obj, *prev_obj;
	/* space we will free */
	uint freed_size = round_up_to_align(obj->size + sizeof(chunk_obj_node_t));

	if ( finalize != NULL )
	    finalize(ptr);

	/* Find the chunk containing this object */
	for (current = cmem->head_chunk; current != NULL; current = current->next) {
	    if (((byte *)obj > (byte *)current) && ((byte *)obj < (byte *)(current) + current->size))
		break;
	}
	if (current == NULL) {
	    /* Object not found in any chunk */
	    dprintf1("chunk_free_obj failed, object 0x%lx not in any chunk\n", ((ulong)obj));
	    return;
	}

	/* Scan obj list to find this element */
	prev_obj = NULL;	/* object is head, linked to mem node */
	for (scan_obj = current->objlist; scan_obj != NULL; scan_obj = scan_obj->next) {
	    if (scan_obj == obj)
	        break;
	    prev_obj = scan_obj;
	}
	if (scan_obj == NULL) {
	    /* Object not found in expected chunk */
	    dprintf3("chunk_free_obj failed, object 0x%lx not in chunk at 0x%lx, size = %d\n",
			    ((ulong)obj), ((ulong)current), current->size);
	    return;
	}
	/* link around the object being freed */
	if (prev_obj == NULL)
	    current->objlist = obj->next;
	else 
	    prev_obj->next = obj->next;

	/* Add this object's space (including the header) to the free list */

	/* Scan free list to find where this element goes */
	obj->size = freed_size;	    /* adjust size to include chunk_obj_node and pad */

	prev_free = NULL;
	for (free_obj = current->freelist; free_obj != NULL; free_obj = free_obj->next) {
	    if (obj < free_obj)
		break;
	    prev_free = free_obj;
	}
	if (prev_free == NULL) {
	    /* this object is before any other free objects */
	    obj->next = current->freelist;
	    current->freelist = obj;
	} else {
	    obj->next = free_obj;
	    prev_free->next = obj;
	}
	/* If the end of this object is adjacent to the next free space,
	 * merge the two. Next we'll merge with predecessor (prev_free)
	 */
	if (free_obj != NULL) {
	    byte *after_obj = (byte*)(obj) + freed_size;
	    
	    if (free_obj <= (chunk_obj_node_t *)after_obj) {
		/* Object is adjacent to following free space block -- merge it */
		obj->next = free_obj->next;	/* link around the one being absorbed */
		obj->size = (byte *)(free_obj) - (byte *)(obj) + free_obj->size;
	    }
	}
	/* the prev_free object precedes this object that is now free,
	 * it _may_ be adjacent
	 */
	if (prev_free != NULL) {
	    byte *after_free = (byte*)(prev_free) + prev_free->size;
	    
	    if (obj <= (chunk_obj_node_t *)after_free) {
		/* Object is adjacent to prior free space block -- merge it */
		/* NB: this is the common case with LIFO alloc-free patterns */
		/* (LIFO: Last-allocated, first freed) */
		prev_free->size = (byte *)(obj) - (byte *)(prev_free) + obj->size;
		prev_free->next = obj->next;		/* link around 'obj' area */
		obj = prev_free;
	    }
	}
#ifdef DEBUG
memset((byte *)(obj) + sizeof(chunk_obj_node_t), 0xf1, obj->size - sizeof(chunk_obj_node_t));
#endif
	if (current->largest_free < obj->size)
	    current->largest_free = obj->size;

	/* If this chunk is now totally empty, free it */
	if (current->objlist == NULL) {
	    if (current->size != current->freelist->size + sizeof(chunk_mem_node_t))
		dprintf2("chunk freelist size not correct, is: %d, should be: %d\n",
		    round_up_to_align(current->freelist->size + sizeof(chunk_mem_node_t)), current->size);
	    chunk_mem_node_remove(cmem, current);
	}
    }
}

static byte *
chunk_alloc_string_immovable(gs_memory_t * mem, uint nbytes, client_name_t cname)
{
    /* we just alloc bytes here */
    return chunk_alloc_bytes(mem, nbytes, cname);
}

static byte *
chunk_alloc_string(gs_memory_t * mem, uint nbytes, client_name_t cname)
{
    /* we just alloc bytes here */
    return chunk_alloc_bytes(mem, nbytes, cname);
}

static byte *
chunk_resize_string(gs_memory_t * mem, byte * data, uint old_num, uint new_num,
		client_name_t cname)
{
    /* just resize object - ignores old_num */
    return chunk_resize_object(mem, data, new_num, cname);
}

static void
chunk_free_string(gs_memory_t * mem, byte * data, uint nbytes,
	      client_name_t cname)
{
    chunk_free_object(mem, data, cname);
}

static void
chunk_status(gs_memory_t * mem, gs_memory_status_t * pstat)
{
}

static gs_memory_t *
chunk_stable(gs_memory_t * mem)
{
    return mem;
}

static void
chunk_enable_free(gs_memory_t * mem, bool enable)
{
}

static void
chunk_consolidate_free(gs_memory_t *mem)
{
}

/* aceesors to get size and type given the pointer returned to the client */
static uint
chunk_object_size(gs_memory_t * mem, const void *ptr)
{
    chunk_obj_node_t *obj = ((chunk_obj_node_t *)ptr) - 1;

    return obj->size;
}

static gs_memory_type_ptr_t
chunk_object_type(const gs_memory_t * mem, const void *ptr)
{
    chunk_obj_node_t *obj = ((chunk_obj_node_t *)ptr) - 1;
    return obj->type;
}

static int
chunk_register_root(gs_memory_t * mem, gs_gc_root_t * rp, gs_ptr_type_t ptype,
		 void **up, client_name_t cname)
{
    return 0;
}

static void
chunk_unregister_root(gs_memory_t * mem, gs_gc_root_t * rp, client_name_t cname)
{
}

#ifdef DEBUG

#define A(obj, size) \
    if ((obj = gs_alloc_bytes(cmem, size, "chunk_alloc_unit_test")) == NULL) { \
	dprintf("chunk alloc failed\n"); \
        return_error(gs_error_VMerror); \
    }

#define F(obj) \
    gs_free_object(cmem, obj, "chunk_alloc_unit_test");

int
chunk_allocator_unit_test(gs_memory_t *mem)
{
    int code;
    gs_memory_t *cmem;
    byte *obj1, *obj2, *obj3, *obj4, *obj5, *obj6, *obj7; 

    if ((code = gs_memory_chunk_wrap(&cmem, mem )) < 0) {
	dprintf1("chunk_wrap returned error code: %d\n", code);
	return code;
    }

    /* Allocate a large object */
    A(obj1, 80000);
    F(obj1);
    A(obj1, 80000);

    A(obj2, 3);
    A(obj3, 7);
    A(obj4, 15);
    A(obj5, 16);
    A(obj6, 16);
    A(obj7, 16);

    F(obj2);
    F(obj1);
    F(obj5);
    F(obj4);
    F(obj6);
    F(obj7);
    F(obj3);

    /* cleanup */
    gs_memory_chunk_release(cmem);
    return 0;
}

#endif /* DEBUG */
