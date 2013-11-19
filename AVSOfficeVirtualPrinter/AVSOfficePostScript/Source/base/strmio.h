/* Copyright (C) 2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: strmio.h 8022 2007-06-05 22:23:38Z giles $ */
/* Interface for streams that mimic stdio functions (fopen, fread, fseek, ftell) */
/* Requires stream.h */

#ifndef strmio_INCLUDED
#  define strmio_INCLUDED

#ifndef stream_DEFINED
#  define stream_DEFINED
typedef struct stream_s stream;
#endif /* stream_DEFINED */

#ifndef gs_memory_DEFINED
#  define gs_memory_DEFINED
typedef struct gs_memory_s gs_memory_t;
#endif /* gs_memory_DEFINED */

/*
 * Open a stream using a filename that can include a PS style IODevice prefix
 * If iodev_default is the '%os' device, then the file will be on the host
 * file system transparently to the caller. The "%os%" prefix can be used
 * to explicilty access the host file system.
 */
stream * sfopen(const char *path, const char *mode, gs_memory_t *mem);

/*
 * Read a number of bytes from a stream, returning number read. Return count
 * will be less than count if EOF or error. Return count is number of elements.
 */
int sfread(void *ptr, size_t size, size_t count, stream *s);	

/*
 * Read a byte from a stream
 */
int sfgetc(stream *s);

/*
 * Seek to a position in the stream. Returns the 0, or -1 if error
 */
int sfseek(stream *s, long offset, int whence);

/*
 * Seek to beginning of the file
 */
int srewind(stream *s);;

/*
 * Return the current position in the stream or -1 if error.
 */
long sftell(stream *s);

/*
 * Return the EOF status, or 0 if not at EOF.
 */
int sfeof(stream *s);

/*
 * Return the error status, or 0 if no error
 */
int sferror(stream *s);

/*
 * close and free the stream. Any further access (including another call to sfclose())
 * to  the  stream results in undefined behaviour (reference to freed memory);
 */
int sfclose(stream *s);

#endif /* strmio_INCLUDED */
