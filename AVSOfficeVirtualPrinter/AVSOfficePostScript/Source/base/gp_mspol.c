/* Copyright (C) 2001-2007 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: gp_mspol.c 8114 2007-07-10 23:00:51Z giles $ */
/*
 * Microsoft Windows platform polling support for Ghostscript.
 *
 */

#include "gx.h"
#include "gp.h"
#include "gpcheck.h"

/* ------ Process message loop ------ */
/* 
 * Check messages and interrupts; return true if interrupted.
 * This is called frequently - it must be quick!
 */
#ifdef CHECK_INTERRUPTS
int
gp_check_interrupts(const gs_memory_t *mem)
{
    if(mem == NULL) {
	/* MAJOR HACK will NOT work in multithreaded environment */
	mem = gs_lib_ctx_get_non_gc_memory_t();
    }
    if (mem && mem->gs_lib_ctx && mem->gs_lib_ctx->poll_fn)
	return (*mem->gs_lib_ctx->poll_fn)(mem->gs_lib_ctx->caller_handle);
    return 0;
}
#endif
