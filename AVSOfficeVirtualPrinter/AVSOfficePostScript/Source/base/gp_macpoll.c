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

/* $Id: gp_macpoll.c 8022 2007-06-05 22:23:38Z giles $ */
/*
 * Macintosh platform polling support for Ghostscript.
 *
 */

#ifndef __CARBON__
#include <Timer.h>
#else
#include <Carbon.h>
#endif

#include "gx.h"
#include "gp.h"
#include "gsdll.h"
#include "gpcheck.h"
#include "iapi.h"
#include "iref.h"
#include "iminst.h"
#include "imain.h"

#ifdef CHECK_INTERRUPTS

extern HWND hwndtext;

/* ------ Process message loop ------ */
/* 
 * Check messages and interrupts; return true if interrupted.
 * This is called frequently - it must be quick!
 */
int gp_check_interrupts(const gs_memory_t *mem)
{
	/* static variables need to go away for thread safety */
	static unsigned long	lastYieldTicks = 0;
	int iRetVal = 0;
	
	if ((TickCount() - lastYieldTicks) > 2) {
	    lastYieldTicks = TickCount();
	    if (pgsdll_callback) {
		/* WARNING: The use of the old gsdll interface is deprecated. 
		 * The caller should use the newer gsapi_set_poll.
		 * If the caller needs access to "hwndtext", it should do 
		 * this via caller_handle which is passed to poll_fn.
		 */
		/* the hwnd parameter which is submitted in gsdll_init 
		 * to the DLL is returned in every gsdll_poll message 
		 * in the count parameter
		 */
		iRetVal = (*pgsdll_callback)(GSDLL_POLL, 0, (long) hwndtext);
	    } else {
	    	if (mem == NULL) {
	    		/* this is not thread safe */
	    		mem = gs_lib_ctx_get_non_gc_memory_t();
	    	}
		if (mem && mem->gs_lib_ctx && mem->gs_lib_ctx->poll_fn)
		    iRetVal = (*mem->gs_lib_ctx->poll_fn)(mem->gs_lib_ctx->caller_handle);
	    }
	}
	return iRetVal;
}
#endif
