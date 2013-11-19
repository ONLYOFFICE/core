/* Copyright (C) 2001-2006 artofcode LLC.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/
/*$Id: gdevtxtw.c 7795 2007-03-23 13:56:11Z tim $ */
/* Device for ASCII or Unicode text extraction */
/* FIXME: not all .h files are listed as dependencies */
#include "memory_.h"
#include "gp.h"			/* for gp_file_name_sizeof */
#include "gx.h"
#include "gserrors.h"
#include "gsparam.h"
#include "gsutil.h"
#include "gxdevice.h"
#include "gsdevice.h"		/* requires gsmatrix.h */

typedef struct gx_device_txtwrite_s {
  gx_device_common;
  void *example_data;
  char fname[gp_file_name_sizeof];	/* OutputFile */
  FILE *file;
} gx_device_txtwrite_t;

/* GC descriptor */
gs_private_st_suffix_add1_final(st_device_txtwrite, gx_device_txtwrite_t,
   "gx_device_txtwrite", device_txtwrite_enum_ptrs, device_txtwrite_reloc_ptrs,
    gx_device_finalize, st_device_forward, example_data);

/* Device procedures */
static dev_proc_open_device(txtwrite_open_device);
static dev_proc_close_device(txtwrite_close_device);
static dev_proc_output_page(txtwrite_output_page);
static dev_proc_fill_rectangle(txtwrite_fill_rectangle);
static dev_proc_get_params(txtwrite_get_params);
static dev_proc_put_params(txtwrite_put_params);
static dev_proc_copy_alpha(txtwrite_copy_alpha);
static dev_proc_copy_mono(txtwrite_copy_mono);
static dev_proc_copy_color(txtwrite_copy_color);
static dev_proc_fill_path(txtwrite_fill_path);
static dev_proc_stroke_path(txtwrite_stroke_path);
static dev_proc_fill_mask(txtwrite_fill_mask);
static dev_proc_fill_trapezoid(txtwrite_fill_trapezoid);
static dev_proc_fill_parallelogram(txtwrite_fill_parallelogram);
static dev_proc_fill_triangle(txtwrite_fill_triangle);
static dev_proc_draw_thin_line(txtwrite_draw_thin_line);
static dev_proc_strip_tile_rectangle(txtwrite_strip_tile_rectangle);
static dev_proc_strip_copy_rop(txtwrite_strip_copy_rop);
static dev_proc_begin_typed_image(txtwrite_begin_typed_image);
static dev_proc_text_begin(txtwrite_text_begin);

/* The device prototype */
#define X_DPI 720
#define Y_DPI 720

const gx_device_txtwrite_t gs_txtwrite_device =
{
    /* Define the device as 8-bit gray scale to avoid computing halftones. */
    std_device_dci_body(gx_device_txtwrite_t, 0, "txtwrite",
			DEFAULT_WIDTH_10THS * X_DPI / 10,
			DEFAULT_HEIGHT_10THS * Y_DPI / 10,
			X_DPI, Y_DPI,
			1, 8, 255, 0, 256, 1),
    {txtwrite_open_device,
     gx_upright_get_initial_matrix,
     gx_default_sync_output,
     txtwrite_output_page,
     gx_default_close_device,
     gx_default_gray_map_rgb_color,
     gx_default_gray_map_color_rgb,
     txtwrite_fill_rectangle,
     gx_default_tile_rectangle,
     txtwrite_copy_mono,
     txtwrite_copy_color,
     gx_default_draw_line,
     gx_default_get_bits,
     txtwrite_get_params,
     txtwrite_put_params,
     gx_default_map_cmyk_color,
     gx_default_get_xfont_procs,
     gx_default_get_xfont_device,
     gx_default_map_rgb_alpha_color,
     gx_page_device_get_page_device,
     NULL,			/* get_alpha_bits */
     txtwrite_copy_alpha,
     NULL,			/* get_band */
     NULL,			/* copy_rop */
     txtwrite_fill_path,
     txtwrite_stroke_path,
     txtwrite_fill_mask,
     txtwrite_fill_trapezoid,
     txtwrite_fill_parallelogram,
     txtwrite_fill_triangle,
     txtwrite_draw_thin_line,
     gx_default_begin_image,
     NULL,			/* image_data */
     NULL,			/* end_image */
     txtwrite_strip_tile_rectangle,
     txtwrite_strip_copy_rop,
     NULL,			/* get_clipping_box */
     txtwrite_begin_typed_image,
     NULL,			/* get_bits_rectangle */
     gx_default_map_color_rgb_alpha,
     gx_null_create_compositor,
     NULL,			/* get_hardware_params */
     txtwrite_text_begin,
     NULL,			/* finish_copydevice */
     NULL,			/* begin_transparency_group */
     NULL,			/* end_transparency_group */
     NULL,			/* begin_transparency_mask */
     NULL,			/* end_transparency_mask */
     NULL,			/* discard_transparency_layer */
     NULL,			/* get_color_mapping_procs */
     NULL,			/* get_color_comp_index */
     NULL,			/* encode_color */
     NULL			/* decode_color */
    },
    0,				/* example_data */
    { 0 },                      /* OutputFile */
    0                           /* FILE *file */
};

     /* ---------------- Open/close/page ---------------- */

static int
txtwrite_open_device(gx_device * dev)
{
    int code;
    gx_device_txtwrite_t *const tdev = (gx_device_txtwrite_t *) dev;

    gx_device_fill_in_procs(dev);
    if (tdev->fname[0] == 0)
        return_error(gs_error_undefinedfilename);
    code = gx_device_open_output_file(dev, tdev->fname,
		true, false, &tdev->file); /* binary, sequential */
    if (code < 0)
        return code;
    return 0;
}

static int
txtwrite_close_device(gx_device * dev)
{
    return 0;

}

static int
txtwrite_output_page(gx_device * dev, int num_copies, int flush)
{
    gx_device_txtwrite_t *const tdev = (gx_device_txtwrite_t *) dev;

    fprintf(tdev->file, "Hello world\n");
    return gx_default_output_page(dev, num_copies, flush);
}

/* ---------------- Low-level drawing ---------------- */

static int
txtwrite_fill_rectangle(gx_device * dev, int x, int y, int w, int h,
		    gx_color_index color)
{
    return 0;
}

static int
txtwrite_copy_alpha(gx_device * dev, const byte * data, int data_x,
		int raster, gx_bitmap_id id, int x, int y, int w, int h,
		gx_color_index color, int depth)
{
    return 0;
}

static int
txtwrite_copy_mono(gx_device * dev, const byte * data, int dx, int raster,
	       gx_bitmap_id id, int x, int y, int w, int h,
	       gx_color_index zero, gx_color_index one)
{
    return 0;
}
static int
txtwrite_copy_color(gx_device * dev, const byte * data,
		int data_x, int raster, gx_bitmap_id id,
		int x, int y, int width, int height)
{
    return 0;
}

static int
txtwrite_strip_tile_rectangle(gx_device * dev, const gx_strip_bitmap * tiles,
   int x, int y, int w, int h, gx_color_index color0, gx_color_index color1,
			  int px, int py)
{
    return 0;
}

static int
txtwrite_strip_copy_rop(gx_device * dev,
		    const byte * sdata, int sourcex, uint sraster,
		    gx_bitmap_id id,
		    const gx_color_index * scolors,
		    const gx_strip_bitmap * textures,
		    const gx_color_index * tcolors,
		    int x, int y, int w, int h,
		    int phase_x, int phase_y, gs_logical_operation_t lop)
{
    return 0;
}

/* ---------------- Parameters ---------------- */

static int
txtwrite_get_params(gx_device * dev, gs_param_list * plist)
{
    int code;
    gs_param_string ofns;
    gx_device_txtwrite_t *const tdev = (gx_device_txtwrite_t *) dev;

    code = gx_default_get_params(dev, plist);
    if (code < 0)
        return code;

    ofns.data = (const byte *)tdev->fname,
    ofns.size = strlen(tdev->fname),
    ofns.persistent = false;
    code = param_write_string(plist, "OutputFile", &ofns);

    return code;
}

/* We implement put_params to ensure that we keep the important */
/* device parameters up to date, and to prevent an /undefined error */
static int
txtwrite_put_params(gx_device * dev, gs_param_list * plist)
{
    gx_device_txtwrite_t *tdev = (gx_device_txtwrite_t *) dev;
    int ecode = 0;
    int code;
    const char *param_name;
    gs_param_string ofs;

    switch (code = param_read_string(plist, (param_name = "OutputFile"), &ofs)) {
	case 0:
	    if (dev->LockSafetyParams &&
		    bytes_compare(ofs.data, ofs.size,
			(const byte *)tdev->fname, strlen(tdev->fname))) {
	        ecode = gs_note_error(gs_error_invalidaccess);
		goto ofe;
	    }
	    if (ofs.size >= gp_file_name_sizeof)
		ecode = gs_error_limitcheck;
	    else
		break;
	    goto ofe;
	default:
	    ecode = code;
	  ofe:param_signal_error(plist, param_name, ecode);
	case 1:
	    ofs.data = 0;
	    break;
    }

    if (ecode < 0)
	return ecode;
    code = gx_default_put_params(dev, plist);
    if (code < 0)
	return code;

    if (ofs.data != 0) {	/* Close the file if it's open. */
	if (tdev->file != 0) {
	    fclose(tdev->file);
	    tdev->file = 0;
	}
	memcpy(tdev->fname, ofs.data, ofs.size);
	tdev->fname[ofs.size] = 0;
	tdev->file = fopen(tdev->fname, "wb");
	if (tdev->file == 0)
	    return_error(gs_error_ioerror);
    }
    return 0;
}

/* ---------------- Polygon drawing ---------------- */

static int
txtwrite_fill_trapezoid(gx_device * dev,
		    const gs_fixed_edge * left, const gs_fixed_edge * right,
		    fixed ybot, fixed ytop, bool swap_axes,
		    const gx_device_color * pdevc, gs_logical_operation_t lop)
{
    return 0;
}

static int
txtwrite_fill_parallelogram(gx_device * dev,
			fixed px, fixed py, fixed ax, fixed ay,
			fixed bx, fixed by, const gx_device_color * pdevc,
			gs_logical_operation_t lop)
{
    return 0;
}

static int
txtwrite_fill_triangle(gx_device * dev,
		   fixed px, fixed py, fixed ax, fixed ay, fixed bx, fixed by,
		   const gx_device_color * pdevc, gs_logical_operation_t lop)
{
    return 0;
}

static int
txtwrite_draw_thin_line(gx_device * dev,
		    fixed fx0, fixed fy0, fixed fx1, fixed fy1,
		    const gx_device_color * pdevc, gs_logical_operation_t lop)
{
    return 0;
}

/* ---------------- High-level drawing ---------------- */

static int
txtwrite_fill_path(gx_device * dev, const gs_imager_state * pis, gx_path * ppath,
	       const gx_fill_params * params, const gx_device_color * pdevc,
	       const gx_clip_path * pcpath)
{
	return 0;
}

static int
txtwrite_stroke_path(gx_device * dev, const gs_imager_state * pis, gx_path * ppath,
		 const gx_stroke_params * params,
		 const gx_drawing_color * pdevc, const gx_clip_path * pcpath)
{
    return 0;
}

static int
txtwrite_fill_mask(gx_device * dev,
	       const byte * data, int dx, int raster, gx_bitmap_id id,
	       int x, int y, int w, int h,
	       const gx_drawing_color * pdcolor, int depth,
	       gs_logical_operation_t lop, const gx_clip_path * pcpath)
{
    return 0;
}

static int
txtwrite_begin_typed_image(gx_device * dev,
		       const gs_imager_state * pis, const gs_matrix * pmat,
		   const gs_image_common_t * pic, const gs_int_rect * prect,
		       const gx_drawing_color * pdcolor,
		       const gx_clip_path * pcpath,
		       gs_memory_t * memory, gx_image_enum_common_t ** pinfo)
{
    return 0;
}

/* ------ Text imaging ------ */

static int
txtwrite_text_begin(gx_device * dev, gs_imager_state * pis,
		const gs_text_params_t * text, gs_font * font,
		gx_path * path, const gx_device_color * pdcolor,
		const gx_clip_path * pcpath,
		gs_memory_t * memory, gs_text_enum_t ** ppenum)
{
    int code = gx_default_text_begin(dev, pis, text, font, path, pdcolor,
				     pcpath, memory, ppenum);

    return code;
}

