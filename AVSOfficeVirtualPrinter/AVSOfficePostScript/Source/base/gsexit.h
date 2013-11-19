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

/* $Id: gsexit.h 8022 2007-06-05 22:23:38Z giles $ */
/* Declarations for exits */

#ifndef gsexit_INCLUDED
#  define gsexit_INCLUDED


/** The client must provide this.
 *  normally they do exit cleanup and error messaging
 *  without calling system exit() returning to the caller.
 */
int gs_to_exit(const gs_memory_t *mem, int exit_status);

/** some clients prefer this to return the postscript error code
 * to the caller otherwise the same as gs_to_exit()
 */
int gs_to_exit_with_code(const gs_memory_t *mem, int exit_status, int code);

/** The client must provide this.  
 * After possible cleanup it may call gp_do_exit() which calls exit() in a platform
 * independent way.  This is a fatal error so returning is not a good idea.
 */
void gs_abort(const gs_memory_t *mem);

#endif /* gsexit_INCLUDED */
