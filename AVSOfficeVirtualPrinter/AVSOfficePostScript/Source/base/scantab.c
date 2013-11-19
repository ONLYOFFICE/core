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

/* $Id: scantab.c 8022 2007-06-05 22:23:38Z giles $ */
/* Scanner table for PostScript/PDF tokens */
#include "stdpre.h"
#include "scommon.h"
#include "scanchar.h"		/* defines interface */

/* Define the character scanning table (see scanchar.h). */
const byte scan_char_array[max_stream_exception + 256] =
{stream_exception_repeat(ctype_exception),
		/* Control characters 0-31. */
 ctype_space,			/* NULL - standard only in Level 2 */
 ctype_name, ctype_name, ctype_name,
 ctype_other,                 /* EOT == ctrl-d <04> */
 ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_space,			/* TAB (\t) */
 ctype_space,			/* LF (\n) */
 ctype_name,
 ctype_space,			/* FF (\f) */
 ctype_space,			/* CR (\r) */
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name,
		/* Printable characters 32-63 */
 ctype_space,			/* space (\s) */
 ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_other,			/* % */
 ctype_name, ctype_name,
 ctype_other,			/* ( */
 ctype_other,			/* ) */
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_other,			/* / */
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,	/* digits 0-9 */
 ctype_name, ctype_name,
 ctype_other,			/* < */
 ctype_name,
 ctype_other,			/* > */
 ctype_name,
		/* Printable characters 64-95 */
 ctype_name,
 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
 30, 31, 32, 33, 34, 35,
 ctype_other,			/* [ */
 ctype_name,
 ctype_other,			/* ] */
 ctype_name, ctype_name,
		/* Printable characters 96-126 and DEL */
 ctype_name,
 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
 30, 31, 32, 33, 34, 35,
 ctype_other,			/* { */
 ctype_name,
 ctype_other,			/* } */
 ctype_name, ctype_name,
		/* Characters 128-159, binary tokens */
 ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken,
 ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken,
 ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken,
 ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken,
 ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken,
 ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken, ctype_btoken,
 ctype_btoken, ctype_btoken,
		/* Characters 160-191, not defined */
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name,
		/* Characters 192-223, not defined */
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name,
		/* Characters 224-255, not defined */
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name, ctype_name, ctype_name, ctype_name,
 ctype_name, ctype_name
};
