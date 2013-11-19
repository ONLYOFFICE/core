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

/* $Id: gsmchunk.h 8750 2008-05-20 20:34:52Z ray $ */
/* chunk consolidating wrapper on a base memory allocator */

#ifndef gsmchunk_INCLUDED
#  define gsmchunk_INCLUDED

#define CHUNK_SIZE		65536

/* ---------- Public constructors/destructors ---------- */

/* Initialize a gs_memory_chunk_t */
	/* -ve error code or 0 */
int gs_memory_chunk_wrap(gs_memory_t **wrapped,	/* chunk allocator init */
		      gs_memory_t * target );	/* base allocator */

/* Release a chunk memory manager and all of the memory it held */
void gs_memory_chunk_release(gs_memory_t *cmem);

/* ---------- Accessors ------------- */

/* Retrieve this allocator's target */
gs_memory_t *gs_memory_chunk_target(const gs_memory_t *cmem);

#ifdef DEBUG
    void gs_memory_chunk_dump_memory(const gs_memory_t *mem);

    int chunk_allocator_unit_test(gs_memory_t *mem);
#endif /* DEBUG */

#endif /* gsmchunk_INCLUDED */
