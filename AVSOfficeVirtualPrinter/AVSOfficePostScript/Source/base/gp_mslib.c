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

/* $Id: gp_mslib.c 8022 2007-06-05 22:23:38Z giles $ */
/*
 * Microsoft Windows platform support for Graphics Library
 *
 * This file implements functions that differ between the graphics
 * library and the interpreter.
 */

/* ------ Process message loop ------ */
/* 
 * Check messages and interrupts; return true if interrupted.
 * This is called frequently - it must be quick!
 */
#ifdef CHECK_INTERRUPTS

#include "gx.h"

int
gp_check_interrupts(const gs_memory_t *mem)
{
    return 0;
}
#endif
