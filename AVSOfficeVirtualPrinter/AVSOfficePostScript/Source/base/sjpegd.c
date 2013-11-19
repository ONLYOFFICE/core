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

/* $Id: sjpegd.c 8022 2007-06-05 22:23:38Z giles $ */
/* Interface routines for IJG decoding code. */
#include "stdio_.h"
#include "string_.h"
#include "jpeglib_.h"
#include "jerror_.h"
#include "gx.h"
#include "gserrors.h"
#include "strimpl.h"
#include "sdct.h"
#include "sjpeg.h"

/*
 * Interface routines.  This layer of routines exists solely to limit
 * side-effects from using setjmp.
 */

int
gs_jpeg_create_decompress(stream_DCT_state * st)
{				/* Initialize error handling */
    gs_jpeg_error_setup(st);
    /* Establish the setjmp return context for gs_jpeg_error_exit to use. */
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));

    jpeg_stream_data_common_init(st->data.decompress);
    jpeg_create_decompress(&st->data.decompress->dinfo);
    return 0;
}

int
gs_jpeg_read_header(stream_DCT_state * st,
		    boolean require_image)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    return jpeg_read_header(&st->data.decompress->dinfo, require_image);
}

int
gs_jpeg_start_decompress(stream_DCT_state * st)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
#if JPEG_LIB_VERSION > 55
    return (int)jpeg_start_decompress(&st->data.decompress->dinfo);
#else
    /* in IJG version 5, jpeg_start_decompress had no return value */
    jpeg_start_decompress(&st->data.decompress->dinfo);
    return 1;
#endif
}

int
gs_jpeg_read_scanlines(stream_DCT_state * st,
		       JSAMPARRAY scanlines,
		       int max_lines)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    return (int)jpeg_read_scanlines(&st->data.decompress->dinfo,
				    scanlines, (JDIMENSION) max_lines);
}

int
gs_jpeg_finish_decompress(stream_DCT_state * st)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    return (int)jpeg_finish_decompress(&st->data.decompress->dinfo);
}
