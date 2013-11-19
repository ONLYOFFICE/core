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

/* $Id: strmio.c 8022 2007-06-05 22:23:38Z giles $ */
/* Interface for streams that mimic stdio functions (fopen, fread, fseek, ftell) */

#include "malloc_.h"
#include "memory_.h"
#include "gxiodev.h"
#include "gdebug.h"
#include "gsfname.h"
#include "gslibctx.h"
#include "gsmemret.h" /* for gs_memory_type_ptr_t */
#include "gsmalloc.h"
#include "gsstype.h"
#include "stream.h"
#include "strmio.h"

/*
 * Open a stream using a filename that can include a PS style IODevice prefix
 * If iodev_default is the '%os' device, then the file will be on the host
 * file system transparently to the caller. The "%os%" prefix can be used
 * to explicilty access the host file system.
 */
stream *
sfopen(const char *path, const char *mode, gs_memory_t *memory)
{
    gs_parsed_file_name_t pfn;
    stream *s;
    iodev_proc_open_file((*open_file));
    gs_memory_t *mem = (memory == NULL) ? gs_lib_ctx_get_non_gc_memory_t() : memory;

    int code = gs_parse_file_name(&pfn, path, strlen(path));
    if (code < 0) {
#	define EMSG	"sfopen: gs_parse_file_name failed.\n"
	errwrite(EMSG, strlen(EMSG));
#	undef EMSG
	return NULL;
    }
    if (pfn.fname == NULL) {	/* just a device */
#	define EMSG	"sfopen: not allowed with %device only.\n"
	errwrite(EMSG, strlen(EMSG));
#	undef EMSG
	return NULL;
    }
    if (pfn.iodev == NULL)
	pfn.iodev = iodev_default;
    open_file = pfn.iodev->procs.open_file;
    if (open_file == 0)
	code = file_open_stream(pfn.fname, pfn.len, mode, 2048, &s,
				pfn.iodev, pfn.iodev->procs.fopen, mem);
    else
	code = open_file(pfn.iodev, pfn.fname, pfn.len, mode, &s, mem);
    if (code < 0)
	return NULL;
    s->position = 0;
    code = ssetfilename(s, (const byte *)path, strlen(path));
    if (code < 0) {
	/* Only error is e_VMerror */
	sclose(s);
	gs_free_object(s->memory, s, "sfopen: allocation error");
#	define EMSG	"sfopen: allocation error setting path name into stream.\n"
	errwrite(EMSG, strlen(EMSG));
#	undef EMSG
	return NULL;
    }
    return s;
}

/*
 * Read a number of bytes from a stream, returning number read. Return count
 * will be less than count if EOF or error. Return count is number of elements.
 */
int
sfread(void *ptr, size_t size, size_t count, stream *s)
{
    int code;
    uint nread;

    code = sgets(s, ptr, size*count, &nread);
    return nread*size;
}

/*
 * Read a byte from a stream
 */
int
sfgetc(stream *s)
{
    int code = sgetc(s);

    return code >= 0 ? code : EOF;
}

/*
 * Seek to a position in the stream. Returns the 0, or -1 if error
 */
int
sfseek(stream *s, long offset, int whence)
{
    long newpos = offset;

    if (whence == SEEK_CUR)
	newpos += stell(s);
    if (whence == SEEK_END) {
	long endpos;

	if (savailable(s, &endpos) < 0)
	    return -1;
	newpos = endpos - offset;
    }
    if (s_can_seek(s) || newpos == stell(s)) {
	return sseek(s, newpos);
    }
    return -1;		/* fail */
}

/*
 * Position to the beginning of the file
 */
int
srewind(stream *s)
{
    return sfseek(s, 0, SEEK_SET);
}

/*
 * Return the current position in the stream or -1 if error.
 */
long
sftell(stream *s)
{
    return stell(s);
}

/*
 * Return the EOF status, or 0 if not at EOF.
 */
int
sfeof(stream *s)
{
    return (s->end_status == EOFC) ? -1 : 0;
}

/*
 * Return the error status, or 0 if no error
 */
int
sferror(stream *s)
{
    return (s->end_status == ERRC) ? -1 : 0;
}

int
sfclose(stream *s)
{
    /* no need to flush since these are 'read' only */
    gs_memory_t *mem = s->memory;
    sclose(s);
    gs_free_object(mem, s, "sfclose(stream)");
    return 0;
}
