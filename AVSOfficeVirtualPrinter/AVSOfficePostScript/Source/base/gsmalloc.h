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

/* $Id: gsmalloc.h 8721 2008-05-09 02:18:14Z ray $ */
/* Client interface to default (C heap) allocator */
/* Requires gsmemory.h */

#ifndef gsmalloc_INCLUDED
#  define gsmalloc_INCLUDED

#include "gxsync.h"

/* Define a memory manager that allocates directly from the C heap. */
typedef struct gs_malloc_block_s gs_malloc_block_t;
typedef struct gs_malloc_memory_s {
    gs_memory_common;
    gs_malloc_block_t *allocated;
    long limit;
    long used;
    long max_used;
    gx_monitor_t *monitor;	/* monitor to serialize access to functions */
} gs_malloc_memory_t;

/* Allocate and initialize a malloc memory manager. */
gs_malloc_memory_t *gs_malloc_memory_init(void);

/* Release all the allocated blocks, and free the memory manager. */
/* The cast is unfortunate, but unavoidable. */
#define gs_malloc_memory_release(mem)\
  gs_memory_free_all((gs_memory_t *)mem, FREE_ALL_EVERYTHING,\
		     "gs_malloc_memory_release")

gs_memory_t * gs_malloc_init(const gs_memory_t *parent);
void gs_malloc_release(gs_memory_t *mem);

#define gs_malloc(mem, nelts, esize, cname)\
  (void *)gs_alloc_byte_array(mem->non_gc_memory, nelts, esize, cname)
#define gs_free(mem, data, nelts, esize, cname)\
  gs_free_object(mem->non_gc_memory, data, cname)

/* ---------------- Locking ---------------- */

/* Create a locked wrapper for a heap allocator. */
int gs_malloc_wrap(gs_memory_t **wrapped, gs_malloc_memory_t *contents);

/* Get the wrapped contents. */
gs_malloc_memory_t *gs_malloc_wrapped_contents(gs_memory_t *wrapped);

/* Free the wrapper, and return the wrapped contents. */
gs_malloc_memory_t *gs_malloc_unwrap(gs_memory_t *wrapped);

#endif /* gsmalloc_INCLUDED */
