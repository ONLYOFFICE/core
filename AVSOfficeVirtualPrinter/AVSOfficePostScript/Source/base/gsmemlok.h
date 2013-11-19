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

/* $Id: gsmemlok.h 8022 2007-06-05 22:23:38Z giles $ */
/* Interface to monitor-locked heap memory allocator */

/* Initial version 2/1/98 by John Desrosiers (soho@crl.com) */

#if !defined(gsmemlok_INCLUDED)
#  define gsmemlok_INCLUDED

#include "gsmemory.h"
#include "gxsync.h"

/*
 * This allocator encapsulates another allocator with a mutex.
 * Note that it does not keep track of memory that it acquires:
 * thus free_all with FREE_ALL_DATA is a no-op.
 */

typedef struct gs_memory_locked_s {
    gs_memory_common;		/* interface outside world sees */
    gs_memory_t *target;	/* allocator to front */
    gx_monitor_t *monitor;	/* monitor to serialize access to functions */
} gs_memory_locked_t;

/* ---------- Public constructors/destructors ---------- */

/* Initialize a locked memory manager. */
int gs_memory_locked_init(
			  gs_memory_locked_t * lmem,	/* allocator to init */
			  gs_memory_t * target	/* allocator to monitor lock */
			  );

/* Release a locked memory manager. */
/* Note that this has no effect on the target. */
void gs_memory_locked_release(gs_memory_locked_t *lmem);

/* Get the target of a locked memory manager. */
gs_memory_t * gs_memory_locked_target(const gs_memory_locked_t *lmem);

#endif /*!defined(gsmemlok_INCLUDED) */
