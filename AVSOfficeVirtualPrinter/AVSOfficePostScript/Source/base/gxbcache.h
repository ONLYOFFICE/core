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

/* $Id: gxbcache.h 8022 2007-06-05 22:23:38Z giles $ */
/* Bitmap cache structures */

#ifndef gxbcache_INCLUDED
#  define gxbcache_INCLUDED

#include "gxbitmap.h"

/*
 * These structures are superclasses for a cache in which the 'value' is
 * a bitmap.  The structures defined here don't take any position about
 * the nature of the 'key'.
 */

/* ---------------- Bitmap cache entry ---------------- */

/*
 * The cache may contain both used and free blocks.
 * All blocks have a common header; free blocks have ONLY the header.
 */
typedef struct gx_cached_bits_head_s {
    uint size;			/* total block size in bytes */
    uint depth;			/* bits per pixel, free block if 0 */
} gx_cached_bits_head;

#define cb_head_is_free(cbh) ((cbh)->depth == 0)
#define cb_head_set_free(cbh) ((cbh)->depth = 0)
#define gx_cached_bits_common\
	gx_cached_bits_head head;	/* must be first */\
		/* The rest of the entry is an abbreviation of */\
		/* gx_strip_bitmap, sans data. */\
	ushort width, height, shift;\
	ushort raster;\
	gx_bitmap_id id
/* Define aliases for head members. */
#define cb_depth head.depth
/* Define aliases for common members formerly in the head. */
#define cb_raster raster
typedef struct gx_cached_bits_s {
    gx_cached_bits_common;
} gx_cached_bits;

#define cb_is_free(cb) cb_head_is_free(&(cb)->head)
/*
 * Define the alignment of the gx_cached_bits structure.  We must ensure
 * that an immediately following bitmap will be properly aligned.
 */
#define align_cached_bits_mod\
  (max(align_bitmap_mod, max(ARCH_ALIGN_PTR_MOD, ARCH_ALIGN_LONG_MOD)))

/*
 * We may allocate a bitmap cache in chunks, so as not to tie up memory
 * prematurely if it isn't needed (or something else needs it more).
 * Thus there is a structure for managing an entire cache, and another
 * structure for managing each chunk.
 */
typedef struct gx_bits_cache_chunk_s gx_bits_cache_chunk;
struct gx_bits_cache_chunk_s {
    gx_bits_cache_chunk *next;
    byte *data;			/* gx_cached_bits_head * */
    uint size;
    uint allocated;		/* amount of allocated data */
};

/* ---------------- Bitmap cache ---------------- */

#define gx_bits_cache_common\
	gx_bits_cache_chunk *chunks;	/* current chunk in circular list */\
	uint cnext;			/* rover for allocating entries */\
					/* in current chunk */\
	uint bsize;			/* total # of bytes for all entries */\
	uint csize		/* # of entries */
typedef struct gx_bits_cache_s {
    gx_bits_cache_common;
} gx_bits_cache;

/* ---------------- Procedural interface ---------------- */

/* ------ Entire cache ------ */

/* Initialize a cache.  The caller must allocate and initialize */
/* the first chunk. */
void gx_bits_cache_init(gx_bits_cache *, gx_bits_cache_chunk *);

/* ------ Chunks ------ */

/* Initialize a chunk.  The caller must allocate it and its data. */
void gx_bits_cache_chunk_init(gx_bits_cache_chunk *, byte *, uint);

/* ------ Individual entries ------ */

/* Attempt to allocate an entry.  If successful, set *pcbh and return 0. */
/* If there isn't enough room, set *pcbh to an entry requiring freeing, */
/* or to 0 if we are at the end of the chunk, and return -1. */
int gx_bits_cache_alloc(gx_bits_cache *, ulong, gx_cached_bits_head **);

/* Shorten an entry by a given amount. */
void gx_bits_cache_shorten(gx_bits_cache *, gx_cached_bits_head *,
			   uint, gx_bits_cache_chunk *);

/* Free an entry.  The caller is responsible for removing the entry */
/* from any other structures (like a hash table). */
void gx_bits_cache_free(gx_bits_cache *, gx_cached_bits_head *,
			gx_bits_cache_chunk *);

#endif /* gxbcache_INCLUDED */
