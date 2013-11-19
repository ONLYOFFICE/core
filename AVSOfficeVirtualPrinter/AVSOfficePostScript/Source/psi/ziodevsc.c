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

/* $Id: ziodevsc.c 9043 2008-08-28 22:48:19Z giles $ */
/* %stdxxx IODevice implementation using callouts for PostScript interpreter */
#include "stdio_.h"
#include "ghost.h"
#include "gpcheck.h"
#include "gp.h"
#include "oper.h"
#include "stream.h"
#include "gxiodev.h"		/* must come after stream.h */
				/* and before files.h */
#include "istream.h"
#include "files.h"
#include "ifilter.h"
#include "store.h"

/* Define the special devices. */
const char iodev_dtype_stdio[] = "Special";
#define iodev_special(dname, init, open) {\
    dname, iodev_dtype_stdio,\
	{ init, open, iodev_no_open_file, iodev_no_fopen, iodev_no_fclose,\
	  iodev_no_delete_file, iodev_no_rename_file, iodev_no_file_status,\
	  iodev_no_enumerate_files, NULL, NULL,\
	  iodev_no_get_params, iodev_no_put_params\
	}\
}

/*
 * We need the current context pointer for accessing / opening the %std
 * IODevices.  However, this is not available to the open routine.
 * Therefore, we use the hack of storing this pointer in the IODevice state
 * pointer just before calling the open routines.  We clear the pointer
 * immediately afterwards so as not to wind up with dangling references.
 */

#define STDIN_BUF_SIZE 1024
static iodev_proc_init(stdin_init);
static iodev_proc_open_device(stdin_open);
const gx_io_device gs_iodev_stdin =
    iodev_special("%stdin%", stdin_init, stdin_open);

#define STDOUT_BUF_SIZE 128
static iodev_proc_open_device(stdout_open);
const gx_io_device gs_iodev_stdout =
    iodev_special("%stdout%", iodev_no_init, stdout_open);

#define STDERR_BUF_SIZE 128
static iodev_proc_open_device(stderr_open);
const gx_io_device gs_iodev_stderr =
    iodev_special("%stderr%", iodev_no_init, stderr_open);

/* ------- %stdin, %stdout, and %stderr ------ */

/*
 * According to Adobe, it is legal to close the %std... files and then
 * re-open them later.  However, the re-opened file object is not 'eq' to
 * the original file object (in our implementation, it has a different
 * read_id or write_id). This is performed in 'file_close_file' by the
 * call to file_close_disable.
 */

static int
    s_stdin_read_process(stream_state *, stream_cursor_read *,
			 stream_cursor_write *, bool);

static int
stdin_init(gx_io_device * iodev, gs_memory_t * mem)
{
    mem->gs_lib_ctx->stdin_is_interactive = true;
    return 0;
}

/* Read from stdin into the buffer. */
/* If interactive, only read one character. */
static int
s_stdin_read_process(stream_state * st, stream_cursor_read * ignore_pr,
		     stream_cursor_write * pw, bool last)
{
    int wcount = (int)(pw->limit - pw->ptr);
    int count;
    gs_memory_t *mem = st->memory;

    if (wcount <= 0)
	return 0;

    /* do the callout */
    if (mem->gs_lib_ctx->stdin_fn)
	count = (*mem->gs_lib_ctx->stdin_fn)
	    (mem->gs_lib_ctx->caller_handle, (char *)pw->ptr + 1,
	     mem->gs_lib_ctx->stdin_is_interactive ? 1 : wcount);
    else
	count = gp_stdin_read((char *)pw->ptr + 1, wcount,
		      mem->gs_lib_ctx->stdin_is_interactive,
		      mem->gs_lib_ctx->fstdin);

    pw->ptr += (count < 0) ? 0 : count;
    return ((count < 0) ? ERRC : (count == 0) ? EOFC : count);
}

static int
stdin_open(gx_io_device * iodev, const char *access, stream ** ps,
	   gs_memory_t * mem)
{
    i_ctx_t *i_ctx_p = (i_ctx_t *)iodev->state;	/* see above */
    stream *s;

    if (!streq1(access, 'r'))
	return_error(e_invalidfileaccess);
    if (file_is_invalid(s, &ref_stdin)) {
	/****** stdin SHOULD NOT LINE-BUFFER ******/
	gs_memory_t *mem = imemory_system;
	byte *buf;
	static const stream_procs p = {
	    s_std_noavailable, s_std_noseek, s_std_read_reset,
	    s_std_read_flush, file_close_file, s_stdin_read_process
	};

	s = file_alloc_stream(mem, "stdin_open(stream)");

	/* We want stdin to read only one character at a time, */
	/* but it must have a substantial buffer, in case it is used */
	/* by a stream that requires more than one input byte */
	/* to make progress. */
	buf = gs_alloc_bytes(mem, STDIN_BUF_SIZE, "stdin_open(buffer)");
	if (s == 0 || buf == 0)
	    return_error(e_VMerror);

	s_std_init(s, buf, STDIN_BUF_SIZE, &p, s_mode_read);
	s->file = 0;
	s->file_modes = s->modes;
	s->file_offset = 0;
	s->file_limit = max_long;
	s->save_close = s_std_null;
	make_file(&ref_stdin, a_readonly | avm_system, s->read_id, s);
	*ps = s;
	return 1;
    }
    *ps = s;
    return 0;
}
/* This is the public routine for getting the stdin stream. */
int
zget_stdin(i_ctx_t *i_ctx_p, stream ** ps)
{
    stream *s;
    gx_io_device *iodev;
    int code;

    if (file_is_valid(s, &ref_stdin)) {
	*ps = s;
	return 0;
    }
    iodev = gs_findiodevice((const byte *)"%stdin", 6);
    iodev->state = i_ctx_p;
    code = (*iodev->procs.open_device)(iodev, "r", ps, imemory_system);
    iodev->state = NULL;
    return min(code, 0);
}

/* Test whether a stream is stdin. */
bool
zis_stdin(const stream *s)
{
    return (s_is_valid(s) && s->procs.process == s_stdin_read_process);
}

/* Write a buffer to stdout, potentially writing to callback */
static int
s_stdout_write_process(stream_state * st, stream_cursor_read *pr,
		     stream_cursor_write *ignore_pw, bool last)
{
    uint count = pr->limit - pr->ptr;
    int written;

    if (count == 0) 
	return 0;
    written = outwrite(st->memory, (const char *)pr->ptr + 1, count);
    if (written < count)
	return ERRC;
    pr->ptr += written;
    return 0;
}

static int
stdout_open(gx_io_device * iodev, const char *access, stream ** ps,
	    gs_memory_t * mem)
{
    i_ctx_t *i_ctx_p = (i_ctx_t *)iodev->state;	/* see above */
    stream *s;

    if (!streq1(access, 'w'))
	return_error(e_invalidfileaccess);
    if (file_is_invalid(s, &ref_stdout)) {
	gs_memory_t *mem = imemory_system;
	byte *buf;
	static const stream_procs p = {
	    s_std_noavailable, s_std_noseek, s_std_write_reset,
	    s_std_write_flush, file_close_file, s_stdout_write_process
	};

	s = file_alloc_stream(mem, "stdout_open(stream)");
	buf = gs_alloc_bytes(mem, STDOUT_BUF_SIZE, "stdout_open(buffer)");
	if (s == 0 || buf == 0)
	    return_error(e_VMerror);
	s_std_init(s, buf, STDOUT_BUF_SIZE, &p, s_mode_write);
	s->file = 0;
	s->file_modes = s->modes;
	s->file_offset = 0;		/* in case we switch to reading later */
	s->file_limit = max_long;	/* ibid. */
	s->save_close = s->procs.flush;
	make_file(&ref_stdout, a_write | avm_system, s->write_id, s);
	*ps = s;
	return 1;
    }
    *ps = s;
    return 0;
}

/* This is the public routine for getting the stdout stream. */
int
zget_stdout(i_ctx_t *i_ctx_p, stream ** ps)
{
    stream *s;
    gx_io_device *iodev;
    int code;

    if (file_is_valid(s, &ref_stdout)) {
	*ps = s;
	return 0;
    }
    iodev = gs_findiodevice((const byte *)"%stdout", 7);
    iodev->state = i_ctx_p;
    code = (*iodev->procs.open_device)(iodev, "w", ps, imemory_system);
    iodev->state = NULL;
    return min(code, 0);
}

/* Write a buffer to stderr, potentially writing to callback */
static int
s_stderr_write_process(stream_state * st, stream_cursor_read *pr,
		     stream_cursor_write *ignore_pw, bool last)
{
    uint count = pr->limit - pr->ptr;
    int written;

    if (count == 0) 
	return 0;
    written = errwrite((const char *)(pr->ptr + 1), count);
    if (written < count) 
	return ERRC;
    pr->ptr += written;
    return 0;
}

static int
stderr_open(gx_io_device * iodev, const char *access, stream ** ps,
	    gs_memory_t * mem)
{
    i_ctx_t *i_ctx_p = (i_ctx_t *)iodev->state;	/* see above */
    stream *s;

    if (!streq1(access, 'w'))
	return_error(e_invalidfileaccess);
    if (file_is_invalid(s, &ref_stderr)) {
	gs_memory_t *mem = imemory_system;
	byte *buf;
	static const stream_procs p = {
	    s_std_noavailable, s_std_noseek, s_std_write_reset,
	    s_std_write_flush, file_close_file, s_stderr_write_process
	};

	s = file_alloc_stream(mem, "stderr_open(stream)");
	buf = gs_alloc_bytes(mem, STDERR_BUF_SIZE, "stderr_open(buffer)");
	if (s == 0 || buf == 0)
	    return_error(e_VMerror);
	s_std_init(s, buf, STDERR_BUF_SIZE, &p, s_mode_write);
	s->file = 0;
	s->file_modes = s->modes;
	s->file_offset = 0;		/* in case we switch to reading later */
	s->file_limit = max_long;	/* ibid. */
	s->save_close = s->procs.flush;
	make_file(&ref_stderr, a_write | avm_system, s->write_id, s);
	*ps = s;
	return 1;
    }
    *ps = s;
    return 0;
}

/* This is the public routine for getting the stderr stream. */
int
zget_stderr(i_ctx_t *i_ctx_p, stream ** ps)
{
    stream *s;
    gx_io_device *iodev;
    int code;

    if (file_is_valid(s, &ref_stderr)) {
	*ps = s;
	return 0;
    }
    iodev = gs_findiodevice((const byte *)"%stderr", 7);
    iodev->state = i_ctx_p;
    code = (*iodev->procs.open_device)(iodev, "w", ps, imemory_system);
    iodev->state = NULL;
    return min(code, 0);
}
