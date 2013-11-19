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

/* $Id: gp_stdin.c 8022 2007-06-05 22:23:38Z giles $ */
/* Read stdin on platforms that do not support unbuffered read.
 * This is the most portable implementation, but it is very slow
 * when reading stdin because it will read one byte at a time.
 * Platforms that support unbuffered read should use gp_stdia.c
 * or provide their own implementation
 */

#include "stdio_.h"
#include "gx.h"
#include "gp.h"

/* Read bytes from stdin, using unbuffered if possible.
 * This implementation doesn't do unbuffered, so if 
 * interactive read one byte at a time.
 */
int gp_stdin_read(char *buf, int len, int interactive, FILE *f)
{
    return fread(buf, 1, interactive ? 1 : len, f);
}

