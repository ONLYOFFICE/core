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

/* $Id: iscanbin.h 9043 2008-08-28 22:48:19Z giles $ */
/* Interface to binary token scanner */

#ifndef iscanbin_INCLUDED
#  define iscanbin_INCLUDED

/*
 * Scan a binary token.  The main scanner calls this iff recognize_btokens()
 * is true.  Return e_unregistered if Level 2 features are not included.
 * Return 0 or scan_BOS on success, <0 on failure.
 *
 * This header file exists only because there are two implementations of
 * this procedure: a dummy one for Level 1 systems, and the real one.
 * The interface is entirely internal to the scanner.
 */
int scan_binary_token(i_ctx_t *i_ctx_p, ref *pref, scanner_state *pstate);

#endif /* iscanbin_INCLUDED */
