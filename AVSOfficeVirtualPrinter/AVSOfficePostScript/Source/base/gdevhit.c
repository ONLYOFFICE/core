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
/* $Id: gdevhit.c 8250 2007-09-25 13:31:24Z giles $ */
/* Hit detection device */
#include "std.h"
#include "gserror.h"
#include "gserrors.h"
#include "gstypes.h"
#include "gsmemory.h"
#include "gxdevice.h"

/* Define the value returned for a detected hit. */
const int gs_hit_detected = gs_error_hit_detected;

/*
 * Define a minimal device for insideness testing.
 * It returns e_hit whenever it is asked to actually paint any pixels.
 */
static dev_proc_fill_rectangle(hit_fill_rectangle);
const gx_device gs_hit_device = {
 std_device_std_body(gx_device, 0, "hit detector",
		     0, 0, 1, 1),
 {NULL,				/* open_device */
  NULL,				/* get_initial_matrix */
  NULL,				/* sync_output */
  NULL,				/* output_page */
  NULL,				/* close_device */
  gx_default_map_rgb_color,
  gx_default_map_color_rgb,
  hit_fill_rectangle,
  NULL,				/* tile_rectangle */
  NULL,				/* copy_mono */
  NULL,				/* copy_color */
  gx_default_draw_line,
  NULL,				/* get_bits */
  NULL,				/* get_params */
  NULL,				/* put_params */
  gx_default_map_cmyk_color,
  NULL,				/* get_xfont_procs */
  NULL,				/* get_xfont_device */
  gx_default_map_rgb_alpha_color,
  gx_default_get_page_device,
  gx_default_get_alpha_bits,
  NULL,				/* copy_alpha */
  gx_default_get_band,
  NULL,				/* copy_rop */
  gx_default_fill_path,
  NULL,				/* stroke_path */
  NULL,				/* fill_mask */
  gx_default_fill_trapezoid,
  gx_default_fill_parallelogram,
  gx_default_fill_triangle,
  gx_default_draw_thin_line,
  gx_default_begin_image,
  gx_default_image_data,
  gx_default_end_image,
  gx_default_strip_tile_rectangle,
  gx_default_strip_copy_rop,
  gx_get_largest_clipping_box,
  gx_default_begin_typed_image,
  NULL,				/* get_bits_rectangle */
  gx_default_map_color_rgb_alpha,
  gx_non_imaging_create_compositor,
  NULL				/* get_hardware_params */
 }
};

/* Test for a hit when filling a rectangle. */
static int
hit_fill_rectangle(gx_device * dev, int x, int y, int w, int h,
		   gx_color_index color)
{
    if (w > 0 && h > 0)
	return_error(gs_error_hit_detected);
    return 0;
}
