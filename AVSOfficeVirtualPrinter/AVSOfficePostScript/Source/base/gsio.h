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

/* $Id: gsio.h 8022 2007-06-05 22:23:38Z giles $ */
/* stdio redirection */

#ifndef gsio_INCLUDED
#  define gsio_INCLUDED

/*
 * The library and interpreter must never use stdin/out/err directly.
 * Make references to them illegal.
 */
#undef stdin
#define stdin stdin_not_available
#undef stdout
#define stdout stdout_not_available
#undef stderr
#define stderr stderr_not_available

/*
 * Redefine all the relevant stdio functions to reference stdin/out/err
 * explicitly, or to be illegal.
 */
#undef fgetchar
#define fgetchar() Function._fgetchar_.unavailable
#undef fputchar
#define fputchar(c) Function._fputchar_.unavailable
#undef getchar
#define getchar() Function._getchar_.unavailable
#undef gets
#define gets Function._gets_.unavailable
/* We should do something about perror, but since many Unix systems */
/* don't provide the strerror function, we can't.  (No Aladdin-maintained */
/* code uses perror.) */
#undef printf
#define printf Function._printf_.unavailable
#undef putchar
#define putchar(c) Function._putchar_.unavailable
#undef puts
#define puts(s) Function._putchar_.unavailable
#undef scanf
#define scanf Function._scanf_.unavailable
#undef vprintf
#define vprintf Function._vprintf_.unavailable
#undef vscanf
#define vscanf Function._vscanf_.unavailable

#endif /* gsio_INCLUDED */
