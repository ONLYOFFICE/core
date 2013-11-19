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

/* $Id: gdevjbig2.c 6300 2005-12-28 19:56:24Z giles $ */
/* JBIG2 encode filter test device */

#include "gdevprn.h"
#include "stream.h"
#include "strimpl.h"
#include "sjbig2_luratech.h"


/* Structure for the JBIG2-writing device. */
typedef struct gx_device_jbig2_s {
    gx_device_common;
    gx_prn_device_common;
} gx_device_jbig2;

/* The device descriptor */
static dev_proc_print_page(jbig2_print_page);

/* ------ The device descriptors ------ */

/* Default X and Y resolution. */
#ifndef X_DPI
#  define X_DPI 72
#endif
#ifndef Y_DPI
#  define Y_DPI 72
#endif

static dev_proc_print_page(jbig2_print_page);

/* Monochrome only */

const gx_device_printer gs_gdevjbig2_device =
prn_device(prn_std_procs, "jbig2",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	X_DPI, Y_DPI,	/* resolution */
	0, 0, 0, 0,	/* margins */
	1, jbig2_print_page);


/* Send the page to the file. */
static int
jbig2_print_page(gx_device_printer * pdev, FILE * prn_stream)
{
    gx_device_jbig2 *jdev = (gx_device_jbig2 *) pdev;
    gs_memory_t *mem = jdev->memory;
    int line_size = gdev_mem_bytes_per_scan_line((gx_device *) pdev);
    byte *in = gs_alloc_bytes(mem, line_size, "jbig2_print_page(in)");
    byte *fbuf = 0;
    uint fbuf_size;
    byte *jbuf = 0;
    uint jbuf_size;
    int lnum;
    int code = 0;
    stream_jbig2encode_state state;
    stream fstrm, cstrm;

    if (in == 0) {
	code = gs_note_error(gs_error_VMerror);
	goto fail;
    }
    /* Create the jbig2encode state. */
    s_init_state((stream_state *)&state, &s_jbig2encode_template, 0);
    if (state.template->set_defaults)
	(*state.template->set_defaults) ((stream_state *) & state);
    state.width = jdev->width;
    state.height = jdev->height;
    /* Set up the streams. */
    fbuf_size = max(512 /* arbitrary */ , state.template->min_out_size);
    jbuf_size = state.template->min_in_size;
    if ((fbuf = gs_alloc_bytes(mem, fbuf_size, "jbig2_print_page(fbuf)")) == 0 ||
	(jbuf = gs_alloc_bytes(mem, jbuf_size, "jbig2_print_page(jbuf)")) == 0
	) {
	code = gs_note_error(gs_error_VMerror);
	goto done;
    }
    s_init(&fstrm, mem);
    swrite_file(&fstrm, prn_stream, fbuf, fbuf_size);
    s_init(&cstrm, mem);
    s_std_init(&cstrm, jbuf, jbuf_size, &s_filter_write_procs,
	       s_mode_write);
    cstrm.state = (stream_state *) & state;
    cstrm.procs.process = state.template->process;
    cstrm.strm = &fstrm;
    if (state.template->init)
	(*state.template->init) (cstrm.state);

    /* Copy the data to the output. */
    for (lnum = 0; lnum < jdev->height; ++lnum) {
	byte *data;
	uint ignore_used;

        if (cstrm.end_status) {
	    code = gs_note_error(gs_error_ioerror);
            goto done;
        }
	gdev_prn_get_bits(pdev, lnum, in, &data);
	sputs(&cstrm, data, state.stride, &ignore_used);
    }

    /* Wrap up. */
    sclose(&cstrm);
    sflush(&fstrm);
  done:
    gs_free_object(mem, jbuf, "jbig2_print_page(jbuf)");
    gs_free_object(mem, fbuf, "jbig2_print_page(fbuf)");
    gs_free_object(mem, in, "jbig2_print_page(in)");
    return code;
  fail:
    gs_free_object(mem, in, "jbig2_print_page(in)");
    return code;
}
