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

/* $Id: gxclthrd.h 8750 2008-05-20 20:34:52Z ray $ */
/* Command list multiple rendering threads */
/* Requires gxsync.h */

#ifndef gxclthrd_INCLUDED
#  define gxclthrd_INCLUDED

#include "gxsync.h"

#define RENDER_THREAD_IDLE 0
#define RENDER_THREAD_DONE 1
#define RENDER_THREAD_BUSY 2

#ifndef clist_render_thread_control_t_DEFINED
#  define clist_render_thread_control_t_DEFINED
typedef struct clist_render_thread_control_s clist_render_thread_control_t;
#endif

struct clist_render_thread_control_s {
    int status;	/* 0: not started, 1: done, 2: busy, < 0: error */ 
		/* values allow waiting until status < 2 */
    gs_memory_t *memory;	/* thread's 'chunk' memory allocator */
    gx_semaphore_t *sema_this;
    gx_semaphore_t *sema_group;
    gx_device *cdev;	/* clist device copy */
    gx_device *bdev;	/* this thread's buffer device */
    int band;
#ifdef DEBUG
    ulong cputime;
#endif
};

#endif /* gxclthrd_INCLUDED */
