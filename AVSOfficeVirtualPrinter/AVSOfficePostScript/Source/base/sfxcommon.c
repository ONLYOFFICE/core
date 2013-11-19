/* Copyright (C) 2008 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: sfxcommon.c 8250 2007-09-25 13:31:24Z giles $ */
/* Common routines for stdio and fd file stream implementations. */
#include "stdio_.h"		/* includes std.h */
#include "memory_.h"
#include "unistd_.h"
#include "gsmemory.h"
#include "gp.h"
#include "gserror.h"
#include "gserrors.h"
#include "stream.h"

#define DEFAULT_BUFFER_SIZE 2048
const uint file_default_buffer_size = DEFAULT_BUFFER_SIZE;

/* Allocate and return a file stream. */
/* Return 0 if the allocation failed. */
/* The stream is initialized to an invalid state, so the caller need not */
/* worry about cleaning up if a later step in opening the stream fails. */
stream *
file_alloc_stream(gs_memory_t * mem, client_name_t cname)
{
    stream *s;
    s = s_alloc(mem, cname);
    if (s == 0)
	return 0;
    s_init_ids(s);
    s->is_temp = 0;		/* not a temp stream */
    s->foreign = 0;
    /*
     * Disable the stream now (in case we can't open the file,
     * or a filter init procedure fails) so that `restore' won't
     * crash when it tries to close open files.
     */
    s_disable(s);
    s->prev = 0;
    s->next = 0;
    return s;
}

/* Open a file stream, optionally on an OS file. */
/* Return 0 if successful, error code if not. */
/* On a successful return, the C file name is in the stream buffer. */
/* If fname==0, set up the file entry, stream, and buffer, */
/* but don't open an OS file or initialize the stream. */
int
file_open_stream(const char *fname, uint len, const char *file_access,
		 uint buffer_size, stream ** ps, gx_io_device *iodev,
		 iodev_proc_fopen_t fopen_proc, gs_memory_t *mem)
{
    int code;
    FILE *file;
    char fmode[4];  /* r/w/a, [+], [b], null */

    if (!iodev)
	iodev = iodev_default;
    code = file_prepare_stream(fname, len, file_access, buffer_size, ps, fmode, mem);
    if (code < 0)
	return code;
    if (fname == 0)
	return 0;
    if (fname[0] == 0)		/* fopen_proc gets NUL terminated string, not len */
	return 0;		/* so this is the same as len == 0, so return NULL */
    code = (*fopen_proc)(iodev, (char *)(*ps)->cbuf, fmode, &file,
			 (char *)(*ps)->cbuf, (*ps)->bsize);
    if (code < 0)
	return code;
    file_init_stream(*ps, file, fmode, (*ps)->cbuf, (*ps)->bsize);
    return 0;
}

/* Close a file stream.  This replaces the close procedure in the stream */
/* for normal (OS) files and for filters. */
int
file_close_file(stream * s)
{
    stream *stemp = s->strm;
    gs_memory_t *mem;
    int code = file_close_disable(s);

    if (code)
	return code;
    /*
     * Check for temporary streams created for filters.
     * There may be more than one in the case of a procedure-based filter,
     * or if we created an intermediate stream to ensure
     * a large enough buffer.  Note that these streams may have been
     * allocated by file_alloc_stream, so we mustn't free them.
     */
    while (stemp != 0 && stemp->is_temp != 0) {
	stream *snext = stemp->strm;

	mem = stemp->memory;
	if (stemp->is_temp > 1)
	    gs_free_object(mem, stemp->cbuf,
			   "file_close(temp stream buffer)");
	s_disable(stemp);
	stemp = snext;
    }
    mem = s->memory;
    gs_free_object(mem, s->cbuf, "file_close(buffer)");
    if (s->close_strm && stemp != 0)
	return sclose(stemp);
    return 0;
}

/*
 * Set up a file stream on an OS file.  The caller has allocated the
 * stream and buffer.
 */
void
file_init_stream(stream *s, FILE *file, const char *fmode, byte *buffer,
		 uint buffer_size)
{
    switch (fmode[0]) {
    case 'a':
	sappend_file(s, file, buffer, buffer_size);
	break;
    case 'r':
	/* Defeat buffering for terminals. */
	{
	    struct stat rstat;

	    fstat(fileno(file), &rstat);
	    sread_file(s, file, buffer,
		       (S_ISCHR(rstat.st_mode) ? 1 : buffer_size));
	}
	break;
    case 'w':
	swrite_file(s, file, buffer, buffer_size);
    }
    if (fmode[1] == '+')
	s->file_modes |= s_mode_read | s_mode_write;
    s->save_close = s->procs.close;
    s->procs.close = file_close_file;
}

/* Prepare a stream with a file name. */
/* Return 0 if successful, error code if not. */
/* On a successful return, the C file name is in the stream buffer. */
/* If fname==0, set up stream, and buffer. */
int
file_prepare_stream(const char *fname, uint len, const char *file_access, 
		 uint buffer_size, stream ** ps, char fmode[4], gs_memory_t *mem)
{
    byte *buffer;
    register stream *s;

    /* Open the file, always in binary mode. */
    strcpy(fmode, file_access);
    strcat(fmode, gp_fmode_binary_suffix);
    if (buffer_size == 0)
	buffer_size = file_default_buffer_size;
    if (len >= buffer_size)    /* we copy the file name into the buffer */
	return_error(gs_error_limitcheck);
    /* Allocate the stream first, since it persists */
    /* even after the file has been closed. */
    s = file_alloc_stream(mem, "file_prepare_stream");
    if (s == 0)
	return_error(gs_error_VMerror);
    /* Allocate the buffer. */
    buffer = gs_alloc_bytes(mem, buffer_size, "file_prepare_stream(buffer)");
    if (buffer == 0)
	return_error(gs_error_VMerror);
    if (fname != 0) {
	memcpy(buffer, fname, len);
	buffer[len] = 0;	/* terminate string */
    } else
	buffer[0] = 0;	/* safety */
    s->cbuf = buffer;
    s->bsize = s->cbsize = buffer_size;
    *ps = s;
    return 0;
}
