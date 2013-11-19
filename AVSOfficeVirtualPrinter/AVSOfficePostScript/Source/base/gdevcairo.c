/* Copyright (C) 2007-2008 Artifex Software, Inc.
   All Rights Reserved.

   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: gdevcairo.c 8771 2008-05-23 00:13:50Z giles $ */
/* cairo output device - contributed by Behdad Esfahbod */

#include <cairo.h>

#include "gx.h"
#include "gserrors.h"
#include "gdevvec.h"
#include "gsutil.h"
#include "memory_.h"
#include "stream.h"
#include "string_.h"
#include "stdlib.h"

/* default resolution. */
#ifndef X_DPI
#  define X_DPI 300
#endif
#ifndef Y_DPI
#  define Y_DPI 300
#endif

/* ---------------- Device definition ---------------- */

#define OUTPUT_PARAM_NAME  "CairoOptions"
#define CONTEXT_PARAM_NAME "CairoContext"

typedef struct gx_device_cairo_s {
    /* superclass state */
    gx_device_vector_common;
    /* local state */
    cairo_t *cr;
    cairo_pattern_t *stroke_pattern;
    cairo_pattern_t *fill_pattern;

    char *output_param;
    char *context_param;

    cairo_bool_t should_close_file;
    cairo_bool_t should_write_png;
} gx_device_cairo;

#define devcairo_device_body(dname, depth)\
  std_device_dci_type_body(gx_device_cairo, 0, dname, &st_device_cairo, \
			   DEFAULT_WIDTH_10THS * X_DPI / 10, \
			   DEFAULT_HEIGHT_10THS * Y_DPI / 10, \
			   X_DPI, Y_DPI, \
			   (depth > 8 ? 3 : 1), depth, \
			   (depth > 1 ? 255 : 1), (depth > 8 ? 255 : 0), \
			   (depth > 1 ? 256 : 2), (depth > 8 ? 256 : 1))

static dev_proc_open_device(devcairo_open_device);
static dev_proc_sync_output(devcairo_sync_output);
static dev_proc_output_page(devcairo_output_page);
static dev_proc_close_device(devcairo_close_device);

static dev_proc_get_params(devcairo_get_params);
static dev_proc_put_params(devcairo_put_params);

#define devcairo_device_procs \
{ \
	devcairo_open_device, \
        NULL,                   /* get_initial_matrix */\
        devcairo_sync_output,\
        devcairo_output_page,\
        devcairo_close_device,\
        gx_default_rgb_map_rgb_color,\
        gx_default_rgb_map_color_rgb,\
        gdev_vector_fill_rectangle,\
        NULL,                   /* tile_rectangle */\
        NULL,			/* copy_mono */\
        NULL,			/* copy_color */\
        NULL,                   /* draw_line */\
        NULL,                   /* get_bits */\
        devcairo_get_params,\
        devcairo_put_params,\
        NULL,                   /* map_cmyk_color */\
        NULL,                   /* get_xfont_procs */\
        NULL,                   /* get_xfont_device */\
        NULL,                   /* map_rgb_alpha_color */\
        gx_page_device_get_page_device,\
        NULL,                   /* get_alpha_bits */\
        NULL,                   /* copy_alpha */\
        NULL,                   /* get_band */\
        NULL,                   /* copy_rop */\
        gdev_vector_fill_path,\
        gdev_vector_stroke_path,\
        NULL,			/* fill_mask */\
        gdev_vector_fill_trapezoid,\
        gdev_vector_fill_parallelogram,\
        gdev_vector_fill_triangle,\
        NULL,			/* draw_thin_line */\
        NULL,			/* begin_image */\
        NULL,                   /* image_data */\
        NULL,                   /* end_image */\
        NULL,                   /* strip_tile_rectangle */\
        NULL			/* strip_copy_rop */\
}

gs_public_st_suffix_add0_final(st_device_cairo, gx_device_cairo,
                               "gx_device_cairo",
                               device_cairo_enum_ptrs, 
			       device_cairo_reloc_ptrs,
                               gx_device_finalize,
			       st_device_vector);

const gx_device_cairo gs_cairo_device = {
    devcairo_device_body("cairo", 24),
    devcairo_device_procs
};

/* Vector device procedures */

static int
devcairo_beginpage(gx_device_vector *vdev);
static int
devcairo_setlinewidth(gx_device_vector *vdev, floatp width);
static int
devcairo_setlinecap(gx_device_vector *vdev, gs_line_cap cap);
static int
devcairo_setlinejoin(gx_device_vector *vdev, gs_line_join join);
static int
devcairo_setmiterlimit(gx_device_vector *vdev, floatp limit);
static int
devcairo_setdash(gx_device_vector *vdev, const float *pattern,
	    uint count, floatp offset);
static int
devcairo_setlogop(gx_device_vector *vdev, gs_logical_operation_t lop,
	     gs_logical_operation_t diff);

static int
devcairo_can_handle_hl_color(gx_device_vector *vdev, const gs_imager_state *pis,
			const gx_drawing_color * pdc);
static int
devcairo_setfillcolor(gx_device_vector *vdev, const gs_imager_state *pis,
                 const gx_drawing_color *pdc);
static int
devcairo_setstrokecolor(gx_device_vector *vdev, const gs_imager_state *pis,
		   const gx_drawing_color *pdc);

static int
devcairo_beginpath(gx_device_vector *vdev, gx_path_type_t type);

static int
devcairo_moveto(gx_device_vector *vdev, floatp x0, floatp y0,
	   floatp x, floatp y, gx_path_type_t type);
static int
devcairo_lineto(gx_device_vector *vdev, floatp x0, floatp y0,
	   floatp x, floatp y, gx_path_type_t type);
static int
devcairo_curveto(gx_device_vector *vdev, floatp x0, floatp y0,
	    floatp x1, floatp y1, floatp x2, floatp y2,
	    floatp x3, floatp y3, gx_path_type_t type);
static int
devcairo_closepath(gx_device_vector *vdev, floatp x, floatp y,
	      floatp x_start, floatp y_start, gx_path_type_t type);
static int
devcairo_endpath(gx_device_vector *vdev, gx_path_type_t type);

/* Vector device function table */

static const gx_device_vector_procs devcairo_vector_procs = {
        /* Page management */
    devcairo_beginpage,
        /* Imager state */
    devcairo_setlinewidth,
    devcairo_setlinecap,
    devcairo_setlinejoin,
    devcairo_setmiterlimit,
    devcairo_setdash,
    gdev_vector_setflat,
    devcairo_setlogop,
        /* Other state */
    devcairo_can_handle_hl_color,
    devcairo_setfillcolor,
    devcairo_setstrokecolor,
        /* Paths */
    gdev_vector_dopath,
    gdev_vector_dorect,
    devcairo_beginpath,
    devcairo_moveto,
    devcairo_lineto,
    devcairo_curveto,
    devcairo_closepath,
    devcairo_endpath
};

/* local utility prototypes */

/* Driver procedure implementation */

cairo_status_t
devcairo_write_func (void                *closure,
		     const unsigned char *data,
		     unsigned int         length)
{
    gx_device_cairo *const devcairo = (gx_device_cairo*)closure;
    gx_device_vector *const vdev = (gx_device_vector*)devcairo;

    stream *s = gdev_vector_stream(vdev);
    uint used;

    sputs(s, (const byte *)data, length, &used);

    return (length == used) ? CAIRO_STATUS_SUCCESS : CAIRO_STATUS_WRITE_ERROR;
}

#ifdef CAIRO_HAS_SVG_SURFACE
#    include <cairo-svg.h>
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
#    include <cairo-pdf.h>
#endif
#ifdef CAIRO_HAS_PS_SURFACE
#    include <cairo-ps.h>
#  if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
#    define HAS_EPS 1
#  else
#    undef HAS_EPS
#  endif
#endif

/* Open the device */
static int
devcairo_open_device(gx_device *dev)
{
    gx_device_vector *const vdev = (gx_device_vector*)dev;
    gx_device_cairo *const devcairo = (gx_device_cairo*)dev;
    int code = 0;

    cairo_t *cr = NULL;

    cairo_bool_t scale_to_points = false;

    vdev->v_memory = dev->memory;
    vdev->vec_procs = &devcairo_vector_procs;
    gdev_vector_init(vdev);

    if (!!vdev->fname[0] == !!devcairo->context_param) {
        return gs_throw(gs_error_undefinedfilename, "Either "CONTEXT_PARAM_NAME" or OutputFile should be set, and not both.\nTo render to a file, set OutputFile, and if needed "OUTPUT_PARAM_NAME".\nTo render to a cairo_t, set "CONTEXT_PARAM_NAME" to a hex printout of the pointer, prefixed by '0x'.");
    }

    devcairo->should_close_file = false;
    devcairo->should_write_png = false;

    if (vdev->fname[0]) {
	const char *extension;
	cairo_surface_t *surface;

	code = gdev_vector_open_file_options(vdev, 512, VECTOR_OPEN_FILE_SEQUENTIAL);
	if (code < 0) return code;

	devcairo->should_close_file = true;

	if (devcairo->output_param)
	    extension = devcairo->output_param;
	else {
	    extension = strrchr (vdev->fname, '.');
	    if (extension)
		extension++; /* skip the dot */
	}

	if (0)
	  ;
	#ifdef CAIRO_HAS_PNG_FUNCTIONS
	    else if (extension && 0 == strcasecmp (extension, "png")) {
	        surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, dev->width, dev->height);
		devcairo->should_write_png = true;
	    }
	#endif
	#ifdef CAIRO_HAS_SVG_SURFACE
	    else if (extension && 0 == strcasecmp (extension, "svg")) {
		surface = cairo_svg_surface_create_for_stream (devcairo_write_func, dev, dev->MediaSize[0], dev->MediaSize[1]);
		scale_to_points = true;
	    }
	#endif
	#ifdef CAIRO_HAS_PDF_SURFACE
	    else if (extension && 0 == strcasecmp (extension, "pdf")) {
		surface = cairo_pdf_surface_create_for_stream (devcairo_write_func, dev, dev->MediaSize[0], dev->MediaSize[1]);
		scale_to_points = true;
	    }
	#endif
	#ifdef CAIRO_HAS_PS_SURFACE
	    else if (extension && 0 == strcasecmp (extension, "ps")) {
		surface = cairo_ps_surface_create_for_stream (devcairo_write_func, dev, dev->MediaSize[0], dev->MediaSize[1]);
		scale_to_points = true;
	    }
	  #ifdef HAS_EPS
	    else if (extension && 0 == strcasecmp (extension, "eps")) {
		surface = cairo_ps_surface_create_for_stream (devcairo_write_func, dev, dev->MediaSize[0], dev->MediaSize[1]);
		cairo_ps_surface_set_eps (surface, true);
		scale_to_points = true;
	    }
	  #endif
	#endif
	else {
	    const char *s1, *s2;
	    s1 = devcairo->output_param ?
		 OUTPUT_PARAM_NAME " value '%s' is not a recognized output format.\n%s" :
		 "OutputFile has %s extension.\nThat is not a recognized output format.\nSet "OUTPUT_PARAM_NAME" to select output format.\n%s";
	    s2 = "It should be one of"
	    #ifdef CAIRO_HAS_PNG_FUNCTIONS
	        " png"
	    #endif
	    #ifdef CAIRO_HAS_SVG_SURFACE
		", svg"
	    #endif
	    #ifdef CAIRO_HAS_PDF_SURFACE
		", pdf"
	    #endif
	    #ifdef CAIRO_HAS_PS_SURFACE
		", ps"
	      #ifdef HAS_EPS
		", eps"
	      #endif
	    #endif
		".";

	    return gs_throw2(gs_error_invalidfileaccess, s1, extension ? extension : "no", s2);
	}

	cairo_surface_set_fallback_resolution (surface, dev->HWResolution[0], dev->HWResolution[1]);

	cr = cairo_create (surface);
	cairo_surface_destroy (surface);

    } else {
	int len = -1;

        if (sscanf (devcairo->context_param, "0x%p%n", &cr, &len) < 1 || len != strlen (devcairo->context_param) || !cr)
	 {
	    return gs_throw1(gs_error_undefined, "Failed to understand "CONTEXT_PARAM_NAME" value '%s'.\nSet "CONTEXT_PARAM_NAME" to a hex printout of the cairo_t pointer, prefixed by '0x'.", devcairo->context_param);
	 }

	if (cairo_status (cr) != CAIRO_STATUS_SUCCESS)
	    return gs_throw1(gs_error_unknownerror, "The cairo_t at '%s' passed as "CONTEXT_PARAM_NAME" is already in error status.", devcairo->context_param);

	cairo_reference (cr);
    }

    devcairo->cr = cr;

    cairo_save (cr);
    cairo_set_source_rgb (cr, 0., 0., 0.);
    devcairo->fill_pattern = cairo_pattern_reference (cairo_get_source (cr));
    devcairo->stroke_pattern = cairo_pattern_reference (cairo_get_source (cr));

    if (scale_to_points)
	cairo_scale (devcairo->cr,
		     72. / dev->HWResolution[0],
		     72. / dev->HWResolution[1]);

    return code;
}

int
devcairo_check_status (gx_device_cairo *devcairo)
{
    /* little trick to check both surface and context status */
    switch (cairo_surface_status (cairo_get_target (devcairo->cr))) {
    case CAIRO_STATUS_SUCCESS:		return 0;
    case CAIRO_STATUS_WRITE_ERROR:	return gs_error_ioerror;
    case CAIRO_STATUS_NO_MEMORY:	return gs_error_VMerror;
    case CAIRO_STATUS_NO_CURRENT_POINT:	return gs_error_nocurrentpoint;
    default:				return gs_error_unknownerror;
    }
}

int
devcairo_sync_output(gx_device * dev)
{
    gx_device_cairo *const devcairo = (gx_device_cairo*)dev;
    int code = 0;

    cairo_surface_flush (cairo_get_target (devcairo->cr));

    code = devcairo_check_status (devcairo);
    if (code < 0) return_error (code);

    return 0;
}

/* Complete a page */
static int
devcairo_output_page(gx_device *dev, int num_copies, int flush)
{
    gx_device_cairo *const devcairo = (gx_device_cairo*)dev;
    int code = 0;
    int i;

    for (i = num_copies - 1; i > 0; i++)
	cairo_copy_page (devcairo->cr);

    if (flush)
	cairo_show_page (devcairo->cr);
    else
	cairo_copy_page (devcairo->cr);

    code = devcairo_check_status (devcairo);
    if (code < 0) return_error (code);

    return gx_finish_output_page(dev, num_copies, flush);
}

/* Close the device */
static int
devcairo_close_device(gx_device *dev)
{
    gx_device_cairo *const devcairo = (gx_device_cairo*)dev;
    int code = 0, ecode;

    if (devcairo->should_close_file) {
	cairo_surface_t *surface = cairo_get_target (devcairo->cr);

#ifdef CAIRO_HAS_PNG_FUNCTIONS
	if (devcairo->should_write_png) {
	    cairo_surface_write_to_png_stream (surface, devcairo_write_func, devcairo);
	    devcairo->should_write_png = false;
	}
#endif

	cairo_surface_finish (surface);

	code = gdev_vector_close_file((gx_device_vector*)dev);
	devcairo->should_close_file = false;
    }

    ecode = devcairo_check_status (devcairo);
    if (ecode < 0) {
	gs_note_error (ecode);
	if (code >= 0) code = ecode;
    }

    cairo_restore (devcairo->cr);
    cairo_destroy (devcairo->cr);

    return code;
}

/* Respond to a device parameter query from the client */
static int
devcairo_get_params(gx_device *dev, gs_param_list *plist)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)dev;
    int code = gdev_vector_get_params(dev, plist);

    if (code < 0) return code;

#define write_string_param(param, ParamName) \
    do {	\
	gs_param_string ofns;	\
	ofns.data = (const byte *)(param ? param : "");	\
	ofns.size = param ? strlen(param) : 0;	\
	ofns.persistent = false;	\
	if ((code = param_write_string(plist, ParamName, &ofns)) < 0)	\
	    return code;	\
    } while (0)

    write_string_param (devcairo->output_param, OUTPUT_PARAM_NAME);
    write_string_param (devcairo->context_param, CONTEXT_PARAM_NAME);

    return code;
}

/* Read the device parameters passed to us by the client */
static int
devcairo_put_params(gx_device *dev, gs_param_list *plist)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)dev;
    int code = 0;

    if (code < 0) return code;

#define read_string_param(param,ParamName) \
    do {	\
	gs_param_name param_name;	\
	gs_param_string ofns;	\
	switch (code = param_read_string(plist, (param_name = ParamName), &ofns)) {	\
	    case 0:	\
		if (param && !bytes_compare(ofns.data, ofns.size,	\
					   (const byte *)param,	\
					    strlen(param))	\
		   )	\
		    /* The new param is the same as the old param.  Do nothing. */	\
		    ofns.data = 0;	\
		break;	\
	    default:	\
		param_signal_error(plist, param_name, code);	\
		break;	\
	    case 1:	\
		ofns.data = 0;	\
		code = 0;	\
		break;	\
	}	\
	if (code < 0) return code;	\
	    \
	if (ofns.data != 0) {	\
	    if (param) {	\
		free (param);	\
	        param = NULL;	\
	    }	\
	    if (ofns.data[0]) {	\
		param = malloc (ofns.size + 1);	\
		memcpy(param, ofns.data, ofns.size);	\
		param[ofns.size] = 0;	\
	    }	\
	} \
    } while (0)

    read_string_param (devcairo->output_param, OUTPUT_PARAM_NAME);
    read_string_param (devcairo->context_param, CONTEXT_PARAM_NAME);

    code = gdev_vector_put_params(dev, plist);

    return code;
}

static cairo_pattern_t *
devcairo_make_pattern(gx_device_cairo *devcairo, gx_drawing_color *pdc)
{
    cairo_pattern_t *pattern;

    if (gx_dc_is_pure(pdc)) {
      gx_color_index color = gx_dc_pure_color(pdc);
      pattern = cairo_pattern_create_rgb (((color >> 16) & 0xff) / 255.,
					  ((color >>  8) & 0xff) / 255.,
					  ((color      ) & 0xff) / 255.);
    } else if (gx_dc_is_null(pdc)) {
      pattern = cairo_pattern_create_rgb (1., 1., 1.); /* just use black */
    } else {
      pattern = cairo_pattern_create_rgb (1., 0., 0.); /* XXX red... */
      gs_note_error(gs_error_rangecheck);
      return pattern;
    }

    return pattern;
}

/* vector device implementation */

        /* Page management */
static int
devcairo_beginpage(gx_device_vector *vdev)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;

    return 0;
}

/* Imager state */
static int
devcairo_setlinewidth(gx_device_vector *vdev, floatp width)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;

    cairo_set_line_width (devcairo->cr, width);

    return 0;
}
static int
devcairo_setlinecap(gx_device_vector *vdev, gs_line_cap cap)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;
    cairo_line_cap_t cairo_cap;

    switch (cap) {
    case gs_cap_butt:   cairo_cap = CAIRO_LINE_CAP_BUTT;   break;
    case gs_cap_square: cairo_cap = CAIRO_LINE_CAP_SQUARE; break;
    case gs_cap_round:  cairo_cap = CAIRO_LINE_CAP_ROUND;  break;
    default:
	gs_note_error(gs_error_rangecheck);
	return 1;
    }

    cairo_set_line_cap (devcairo->cr, cairo_cap);

    return 0;
}
static int
devcairo_setlinejoin(gx_device_vector *vdev, gs_line_join join)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;
    cairo_line_join_t cairo_join;

    switch (join) {
    case gs_join_miter: cairo_join = CAIRO_LINE_JOIN_MITER; break;
    case gs_join_round: cairo_join = CAIRO_LINE_JOIN_ROUND; break;
    case gs_join_bevel: cairo_join = CAIRO_LINE_JOIN_BEVEL; break;
    default:
	gs_note_error(gs_error_rangecheck);
	return 1;
    }

    cairo_set_line_join (devcairo->cr, cairo_join);

    return 0;
}
static int
devcairo_setmiterlimit(gx_device_vector *vdev, floatp limit)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;

    cairo_set_miter_limit (devcairo->cr, limit);

    return 0;
}
static int
devcairo_setdash(gx_device_vector *vdev, const float *pattern,
	    uint count, floatp offset)
{
    dprintf("XXX devcairo_setdash\n");
    return 0;
}
static int
devcairo_setlogop(gx_device_vector *vdev, gs_logical_operation_t lop,
	     gs_logical_operation_t diff)
{
/*    dprintf("XXX devcairo_setlogop\n"); */
    return 0;
}

        /* Other state */

static int
devcairo_can_handle_hl_color(gx_device_vector *vdev, const gs_imager_state *pis,
			  const gx_drawing_color * pdc)
{
/*    dprintf("XXX devcairo_can_handle_hl_color\n"); */
    return 0;
}

static int
devcairo_setfillcolor(gx_device_vector *vdev, const gs_imager_state *pis,
		 const gx_drawing_color *pdc)
{
    gx_device_cairo *devcairo = (gx_device_cairo*)vdev;

    cairo_pattern_destroy (devcairo->fill_pattern);
    devcairo->fill_pattern = devcairo_make_pattern (devcairo, pdc);

    return 0;
}

static int
devcairo_setstrokecolor(gx_device_vector *vdev, const gs_imager_state *pis,
		   const gx_drawing_color *pdc)
{
    gx_device_cairo *devcairo = (gx_device_cairo*)vdev;

    cairo_pattern_destroy (devcairo->stroke_pattern);
    devcairo->stroke_pattern = devcairo_make_pattern (devcairo, pdc);

    return 0;
}

	/* Paths */
/*    gdev_vector_dopath */

static int
devcairo_beginpath(gx_device_vector *vdev, gx_path_type_t type)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;

    cairo_new_path (devcairo->cr);

    return 0;
}

static int
devcairo_moveto(gx_device_vector *vdev, floatp x0, floatp y0,
	   floatp x, floatp y, gx_path_type_t type)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;

    cairo_move_to (devcairo->cr, x,y);

    return 0;
}

static int
devcairo_lineto(gx_device_vector *vdev, floatp x0, floatp y0,
	   floatp x, floatp y, gx_path_type_t type)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;

    cairo_line_to (devcairo->cr, x,y);

    return 0;
}

static int
devcairo_curveto(gx_device_vector *vdev, floatp x0, floatp y0,
	    floatp x1, floatp y1, floatp x2, floatp y2,
	    floatp x3, floatp y3, gx_path_type_t type)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;

    cairo_curve_to (devcairo->cr, x1,y1, x2,y2, x3,y3);

    return 0;
}

static int
devcairo_closepath(gx_device_vector *vdev, floatp x, floatp y,
	      floatp x_start, floatp y_start, gx_path_type_t type)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;

    cairo_close_path (devcairo->cr);

    return 0;
}

static int
devcairo_endpath(gx_device_vector *vdev, gx_path_type_t type)
{
    gx_device_cairo *devcairo = (gx_device_cairo *)vdev;
    cairo_t *cr = devcairo->cr;

    cairo_set_fill_rule (cr,
			 (type & gx_path_type_even_odd) ? 
			 CAIRO_FILL_RULE_EVEN_ODD : CAIRO_FILL_RULE_WINDING);

    if (type & gx_path_type_fill) {
	cairo_set_source (cr, devcairo->fill_pattern);
        cairo_fill_preserve (cr);
    }

    if (type & gx_path_type_stroke) {
	cairo_set_source (cr, devcairo->stroke_pattern);
        cairo_stroke_preserve (cr);
    }

    if (type & gx_path_type_clip)
        cairo_clip_preserve (cr);

    cairo_new_path (devcairo->cr);

    return 0;
}
