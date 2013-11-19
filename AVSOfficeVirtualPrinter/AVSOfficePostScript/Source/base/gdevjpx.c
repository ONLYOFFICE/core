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

/* $Id: gdevjpx.c 6300 2005-12-28 19:56:24Z giles $ */
/* JPX encode filter test device */

#include "gdevprn.h"
#include "stream.h"
#include "strimpl.h"
#include "sjpx_luratech.h"


/* Structure for the JPX-writing device. */
typedef struct gx_device_jpx_s {
    gx_device_common;
    gx_prn_device_common;
} gx_device_jpx;

/* The device descriptor */
static dev_proc_print_page(jpx_print_page);

/* ------ The device descriptors ------ */

/* Default X and Y resolution. */
#ifndef X_DPI
#  define X_DPI 72
#endif
#ifndef Y_DPI
#  define Y_DPI 72
#endif

static dev_proc_print_page(jpx_print_page);

/* 24 bit RGB default */
static const gx_device_procs jpxrgb_procs =
prn_color_procs(gdev_prn_open, gdev_prn_output_page, gdev_prn_close,
		       gx_default_rgb_map_rgb_color,
		       gx_default_rgb_map_color_rgb);
const gx_device_printer gs_jpxrgb_device = {
    prn_device_std_body(gx_device_jpx, jpxrgb_procs, "jpx",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	X_DPI, Y_DPI,	/* resolution */
	0, 0, 0, 0,	/* margins */
	24,		/* bits per pixel */
	jpx_print_page)
};

/* 8 bit Grayscale */
static const gx_device_procs jpxgray_procs =
prn_color_procs(gdev_prn_open, gdev_prn_output_page, gdev_prn_close,
		       gx_default_gray_map_rgb_color,
		       gx_default_gray_map_color_rgb);
const gx_device_printer gs_jpxgray_device = {
    prn_device_body(gx_device_jpx, jpxgray_procs, "jpxgray",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	X_DPI, Y_DPI,	/* resolution */
	0, 0, 0, 0,	/* margins */
	1, 8, 255, 0, 256, 0, /* components, depth and min/max values */
	jpx_print_page)
};

/* 32 bit CMKY */
static dev_proc_map_color_rgb(jpx_cmyk_map_color_rgb);
static dev_proc_map_cmyk_color(jpx_cmyk_map_cmyk_color);
static const gx_device_procs jpxcmyk_procs = 
{       gdev_prn_open,
        gx_default_get_initial_matrix,
        NULL,   /* sync_output */
        gdev_prn_output_page,
        gdev_prn_close,
        NULL,
        jpx_cmyk_map_color_rgb,
        NULL,   /* fill_rectangle */
        NULL,   /* tile_rectangle */
        NULL,   /* copy_mono */
        NULL,   /* copy_color */
        NULL,   /* draw_line */
        NULL,   /* get_bits */
	gdev_prn_get_params,
        gdev_prn_put_params,
        jpx_cmyk_map_cmyk_color,
        NULL,   /* get_xfont_procs */
        NULL,   /* get_xfont_device */
        NULL,   /* map_rgb_alpha_color */
        gx_page_device_get_page_device  /* get_page_device */
};
const gx_device_printer gs_jpxcmyk_device = {
    prn_device_std_body(gx_device_jpx, jpxcmyk_procs, "jpxcmyk",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	X_DPI, Y_DPI,	/* resolution */
	0, 0, 0, 0,	/* margins */
	32,		/* bits per pixel */
	jpx_print_page)
};

/* private color conversion routines; 
   we don't seem to have defaults for cmyk. */
static int
jpx_cmyk_map_color_rgb(gx_device * dev, gx_color_index color,
                        gx_color_value prgb[3])
{
    int not_k = color & 0xff,
        r = not_k - ~(color >> 24),
        g = not_k - ~((color >> 16) & 0xff),
        b = not_k - ~((color >> 8) & 0xff);

    prgb[0] = (r < 0 ? 0 : gx_color_value_from_byte(r));
    prgb[1] = (g < 0 ? 0 : gx_color_value_from_byte(g));
    prgb[2] = (b < 0 ? 0 : gx_color_value_from_byte(b));
    return 0;
}

static gx_color_index
jpx_cmyk_map_cmyk_color(gx_device * dev, const gx_color_value cv[])
{
    gx_color_index color = ~(
        gx_color_value_to_byte(cv[3]) +
        ((uint)gx_color_value_to_byte(cv[2]) << 8) +
        ((uint)gx_color_value_to_byte(cv[1]) << 16) +
        ((uint)gx_color_value_to_byte(cv[0]) << 24));

    return (color == gx_no_color_index ? color ^ 1 : color);
}


/* Send the page to the file. */
static int
jpx_print_page(gx_device_printer * pdev, FILE * prn_stream)
{
    gx_device_jpx *jdev = (gx_device_jpx *) pdev;
    gs_memory_t *mem = jdev->memory;
    int line_size = gdev_mem_bytes_per_scan_line((gx_device *) pdev);
    byte *in = gs_alloc_bytes(mem, line_size, "jpx_print_page(in)");
    byte *fbuf = 0;
    uint fbuf_size;
    byte *jbuf = 0;
    uint jbuf_size;
    int lnum;
    int code = 0;
    stream_jpxe_state state;
    stream fstrm, cstrm;

    if (in == 0) {
	code = gs_note_error(gs_error_VMerror);
	goto fail;
    }
    /* Create the jpx encoder state. */
    s_init_state((stream_state *)&state, &s_jpxe_template, 0);
    if (state.template->set_defaults)
	(*state.template->set_defaults) ((stream_state *) & state);
    state.width = jdev->width;
    state.height = jdev->height;
    switch (jdev->color_info.depth) {
	case 32: state.colorspace = gs_jpx_cs_cmyk; break;
	case 24: state.colorspace = gs_jpx_cs_rgb; break;
	case  8: state.colorspace = gs_jpx_cs_gray; break;
	default:
	    state.colorspace = gs_jpx_cs_gray; /* safest option */
	    dlprintf1("unexpected color_info depth %d\n",
					jdev->color_info.depth);
    }
    state.bpc = 8; /* currently only 8 bits per component is supported */

    /* ask for lossless encoding */
    /* state.lossless = 1; */
    /* or, set the quality level different from the default */
    /* state.quality = 35; */

    /* Set up the streams. */
    fbuf_size = max(512 /* arbitrary */ , state.template->min_out_size);
    jbuf_size = state.template->min_in_size;
    if ((fbuf = gs_alloc_bytes(mem, fbuf_size, "jpx_print_page(fbuf)")) == 0 ||
	(jbuf = gs_alloc_bytes(mem, jbuf_size, "jpx_print_page(jbuf)")) == 0
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
    gs_free_object(mem, jbuf, "jpx_print_page(jbuf)");
    gs_free_object(mem, fbuf, "jpx_print_page(fbuf)");
    gs_free_object(mem, in, "jpx_print_page(in)");
    return code;
  fail:
    gs_free_object(mem, in, "jpx_print_page(in)");
    return code;
}
