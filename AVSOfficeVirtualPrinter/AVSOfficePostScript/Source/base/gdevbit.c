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

/* $Id: gdevbit.c 9007 2008-08-20 23:58:43Z giles $ */
/* "Plain bits" devices to measure rendering time. */

#include "gdevprn.h"
#include "gsparam.h"
#include "gscrd.h"
#include "gscrdp.h"
#include "gxlum.h"
#include "gxdcconv.h"
#include "gdevdcrd.h"
#include "gsutil.h" /* for bittags hack */


/* Define the device parameters. */
#ifndef X_DPI
#  define X_DPI 72
#endif
#ifndef Y_DPI
#  define Y_DPI 72
#endif

/* The device descriptor */
static dev_proc_get_color_mapping_procs(bittag_get_color_mapping_procs);
static dev_proc_map_rgb_color(bittag_rgb_map_rgb_color);
static dev_proc_map_color_rgb(bittag_map_color_rgb);
static dev_proc_put_params(bittag_put_params);
static dev_proc_map_rgb_color(bit_mono_map_color);
#if 0 /* unused */
static dev_proc_map_rgb_color(bit_forcemono_map_rgb_color);
#endif
static dev_proc_map_color_rgb(bit_map_color_rgb);
static dev_proc_map_cmyk_color(bit_map_cmyk_color);
static dev_proc_get_params(bit_get_params);
static dev_proc_put_params(bit_put_params);
static dev_proc_print_page(bit_print_page);

#define bit_procs(encode_color)\
{	gdev_prn_open,\
	gx_default_get_initial_matrix,\
	NULL,	/* sync_output */\
	gdev_prn_output_page,\
	gdev_prn_close,\
	encode_color,	/* map_rgb_color */\
	bit_map_color_rgb,	/* map_color_rgb */\
	NULL,	/* fill_rectangle */\
	NULL,	/* tile_rectangle */\
	NULL,	/* copy_mono */\
	NULL,	/* copy_color */\
	NULL,	/* draw_line */\
	NULL,	/* get_bits */\
	bit_get_params,\
	bit_put_params,\
	encode_color,	/* map_cmyk_color */\
	NULL,	/* get_xfont_procs */\
	NULL,	/* get_xfont_device */\
	NULL,	/* map_rgb_alpha_color */\
	gx_page_device_get_page_device,	/* get_page_device */\
	NULL,	/* get_alpha_bits */\
	NULL,	/* copy_alpha */\
	NULL,	/* get_band */\
	NULL,	/* copy_rop */\
	NULL,	/* fill_path */\
	NULL,	/* stroke_path */\
	NULL,	/* fill_mask */\
	NULL,	/* fill_trapezoid */\
	NULL,	/* fill_parallelogram */\
	NULL,	/* fill_triangle */\
	NULL,	/* draw_thin_line */\
	NULL,	/* begin_image */\
	NULL,	/* image_data */\
	NULL,	/* end_image */\
	NULL,	/* strip_tile_rectangle */\
	NULL,	/* strip_copy_rop */\
	NULL,	/* get_clipping_box */\
	NULL,	/* begin_typed_image */\
	NULL,	/* get_bits_rectangle */\
	NULL,	/* map_color_rgb_alpha */\
	NULL,	/* create_compositor */\
	NULL,	/* get_hardware_params */\
	NULL,	/* text_begin */\
	NULL,	/* finish_copydevice */\
	NULL,	/* begin_transparency_group */\
	NULL,	/* end_transparency_group */\
	NULL,	/* begin_transparency_mask */\
	NULL,	/* end_transparency_mask */\
	NULL,	/* discard_transparency_layer */\
	NULL,	/* get_color_mapping_procs */\
	NULL,	/* get_color_comp_index */\
	encode_color,		/* encode_color */\
	bit_map_color_rgb	/* decode_color */\
}

/*
 * The following macro is used in get_params and put_params to determine the
 * num_components for the current device. It works using the device name
 * character after "bit" which is either '\0', 'r', or 'c'. Any new devices
 * that are added to this module must modify this macro to return the
 * correct num_components. This is needed to support the ForceMono
 * parameter, which alters dev->num_components.
 */
#define REAL_NUM_COMPONENTS(dev) (dev->dname[3] == 'c' ? 4 : \
				  dev->dname[3] == 'r' ? 3 : 1)
struct gx_device_bit_s {
    gx_device_common;
    gx_prn_device_common;
    int  FirstLine, LastLine;	/* to allow multi-threaded rendering testing */
};
typedef struct gx_device_bit_s gx_device_bit;

static const gx_device_procs bitmono_procs =
bit_procs(bit_mono_map_color);
const gx_device_bit gs_bit_device =
{prn_device_body(gx_device_bit, bitmono_procs, "bit",
		 DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
		 X_DPI, Y_DPI,
		 0, 0, 0, 0,    /* margins */
		 1, 1, 1, 0, 2, 1, bit_print_page)
};

static const gx_device_procs bitrgb_procs =
bit_procs(gx_default_rgb_map_rgb_color);
const gx_device_bit gs_bitrgb_device =
{prn_device_body(gx_device_bit, bitrgb_procs, "bitrgb",
		 DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
		 X_DPI, Y_DPI,
		 0, 0, 0, 0,	/* margins */
		 3, 4, 1, 1, 2, 2, bit_print_page)
};

static const gx_device_procs bitcmyk_procs =
bit_procs(bit_map_cmyk_color);
const gx_device_bit gs_bitcmyk_device =
{prn_device_body(gx_device_bit, bitcmyk_procs, "bitcmyk",
		 DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
		 X_DPI, Y_DPI,
		 0, 0, 0, 0,	/* margins */
		 4, 4, 1, 1, 2, 2, bit_print_page)
};

static const gx_device_procs bitrgbtags_procs =
    { 
        gdev_prn_open,                        /* open_device */
        gx_default_get_initial_matrix,        /* initial_matrix */
        ((void *)0),                        /* sync_output */
        gdev_prn_output_page,                 /* output page */
        gdev_prn_close,                       /* close_device */
        bittag_rgb_map_rgb_color,             /* map rgb color */
        bittag_map_color_rgb,                 /* map color rgb */
        ((void *)0),                        /* fill_rectangle */
        ((void *)0),                        /* tile rectangle */
        ((void *)0),                        /* copy mono */
        ((void *)0),                        /* copy color */
        ((void *)0),                        /* obsolete draw line */
        ((void *)0),                        /* get_bits */
        gdev_prn_get_params,                  /* get params */
        bittag_put_params,                    /* put params */
        bittag_rgb_map_rgb_color,             /* map_cmyk_color */
        ((void *)0),                        /* get_xfonts */
        ((void *)0),                        /* get_xfont_device */
        ((void *)0),                        /* map_rgb_alpha_color */
        gx_page_device_get_page_device,       /* get_page_device */
        ((void *)0),                        /* get_alpha_bits */
        ((void *)0),                        /* copy_alpha */
        ((void *)0),                        /* get_band */
        ((void *)0),                        /* copy_rop */
        ((void *)0),                       /* fill_path */
        ((void *)0),                       /* stroke_path */
        ((void *)0),                       /* fill_mask */
        ((void *)0),                        /* fill_trapezoid */
        ((void *)0),                        /* fill_parallelogram */
        ((void *)0),                        /* fill_triangle */
        ((void *)0),                        /* draw_thin_line */
        ((void *)0),                        /* begin_image */
        ((void *)0),                        /* image_data */
        ((void *)0),                        /* end_image */
        ((void *)0),                        /* strip_tile_rectangle */
        ((void *)0),                        /* strip_copy_rop */
        ((void *)0),                        /* get_clipping_box */
        ((void *)0),                        /* begin_typed_image */
        ((void *)0),                        /* get_bits_rectangle */
        ((void *)0),                        /* map_color_rgb_alpha */
        ((void *)0),                       /* create_compositor */
        ((void *)0),                       /* get_hardware_params */
        ((void *)0),                       /* text_begin */
        ((void *)0),                       /* finish_copydevice */
        ((void *)0),                       /* begin_transparency_group */
        ((void *)0),                       /* end_transparency_group */
        ((void *)0),                       /* begin_transparency_mask */
        ((void *)0),                       /* end_transparency_mask */
        ((void *)0),                       /* discard_transparency_layer */
        bittag_get_color_mapping_procs,      /* get_color_mapping_procs */
        ((void *)0),                       /* get_color_comp_index */
        bittag_rgb_map_rgb_color,            /* encode_color */
        bittag_map_color_rgb                 /* decode_color */
    };

const gx_device_bit gs_bitrgbtags_device =
    {
        sizeof(gx_device_bit),
        &bitrgbtags_procs,
        "bitrgbtags",
        0 ,                             /* memory */
        &st_device_printer,
        0 ,                             /* stype_is_dynamic */
        0 ,                             /* finalize */
        { 0 } ,                         /* rc header */
        0 ,                             /* retained */
        0 ,                             /* is open */
        0,                              /* max_fill_band */
        {                               /* color infor */
            4,                          /* max_components */
            4,                          /* num_components */
            GX_CINFO_POLARITY_ADDITIVE, /* polarity */
            32,                         /* depth */                        
            GX_CINFO_COMP_NO_INDEX,     /* gray index */
            255 ,                         /* max_gray */
            255 ,                         /* max_colors */
            256 ,                         /* dither grays */
            256 ,                         /* dither colors */
            { 1, 1 } ,                  /* antialiasing */
            GX_CINFO_UNKNOWN_SEP_LIN,   /* sep and linear */
            { 0 } ,                     /* comp shift */
            { 0 } ,                     /* comp bits */
            { 0 } ,                     /* comp mask */
            ( "DeviceRGB" ),            /* color model name */
            GX_CINFO_OPMODE_UNKNOWN ,   /* overprint mode */
            0                           /* process comps */
        },
        { 
            ((gx_color_index)(~0)),
            ((gx_color_index)(~0)) 
        },
        (int)((float)(85) * (X_DPI) / 10 + 0.5),
        (int)((float)(110) * (Y_DPI) / 10 + 0.5),
        0,
        { 
            (float)(((((int)((float)(85) * (X_DPI) / 10 + 0.5)) * 72.0 + 0.5) - 0.5) / (X_DPI)) ,
            (float)(((((int)((float)(110) * (Y_DPI) / 10 + 0.5)) * 72.0 + 0.5) - 0.5) / (Y_DPI)) },
        {
            0,
            0,
            0,
            0 
        } ,
        0 ,
        { X_DPI, Y_DPI } ,
        { X_DPI, Y_DPI },
        {(float)(-(0) * (X_DPI)),
         (float)(-(0) * (Y_DPI))},
        {(float)((0) * 72.0),
         (float)((0) * 72.0),
         (float)((0) * 72.0),
         (float)((0) * 72.0)},
        0 ,
        0 ,
        1 ,
        0 ,
        0 ,
        0 ,
        0 ,
	0,
	0,
	{false},
        { 
            gx_default_install,
            gx_default_begin_page,
            gx_default_end_page
        },
        { 0 },
        { 0 },
        { bit_print_page,
          gx_default_print_page_copies,
          { gx_default_create_buf_device,
            gx_default_size_buf_device,
            gx_default_setup_buf_device,
            gx_default_destroy_buf_device },
          gx_default_get_space_params,
          gx_default_start_render_thread,
          gx_default_open_render_device,
          gx_default_close_render_device,
          gx_default_buffer_page },
        { 
            PRN_MAX_BITMAP,
            PRN_BUFFER_SPACE,
            { 0, 0, 0 },
            0 ,
            BandingAlways },
        { 0 },
        0 ,
        0 ,
        0 ,
        -1,
        0 ,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0 ,
        0,
        0,
        0
    };

static void
cmyk_cs_to_rgb_cm(gx_device * dev, frac c, frac m, frac y, frac k, frac out[])
{
    color_cmyk_to_rgb(c, m, y, k, NULL, out);
};

static void
private_rgb_cs_to_rgb_cm(gx_device * dev, const gs_imager_state *pis,
				  frac r, frac g, frac b, frac out[])
{
    out[0] = r;
    out[1] = g;
    out[2] = b;
}

static void
gray_cs_to_rgb_cm(gx_device * dev, frac gray, frac out[])
{
    out[0] = out[1] = out[2] = gray;
}


static const gx_cm_color_map_procs bittag_DeviceRGB_procs = {
    gray_cs_to_rgb_cm, private_rgb_cs_to_rgb_cm, cmyk_cs_to_rgb_cm
};

static const gx_cm_color_map_procs *
bittag_get_color_mapping_procs(const gx_device *dev)
{
    return &bittag_DeviceRGB_procs;
}

static gx_color_index
bittag_rgb_map_rgb_color(gx_device * dev, const gx_color_value cv[])
{
    return
        ((cv[2]) >> ((sizeof(gx_color_value) * 8) - 8)) +
        ((uint) ((cv[1]) >> ((sizeof(gx_color_value) * 8) - 8)) << 8) +
        ((ulong) ((cv[0]) >> ((sizeof(gx_color_value) * 8) - 8)) << 16) +
        ((ulong)gs_current_object_tag() << 24);
}

static int
bittag_map_color_rgb(gx_device * dev, gx_color_index color, gx_color_value cv[4])
{
    int depth = 24;
    int ncomp = 3;
    int bpc = depth / ncomp;
    uint mask = (1 << bpc) - 1;

#define cvalue(c) ((gx_color_value)((ulong)(c) * gx_max_color_value / mask))

    gx_color_index cshift = color;
    cv[2] = cvalue(cshift & mask);
    cshift >>= bpc;
    cv[1] = cvalue(cshift & mask);
    cshift >>= bpc;
    cv[0] = cvalue(cshift & mask);
    return 0;
#undef cvalue
}

/* Map gray to color. */
/* Note that 1-bit monochrome is a special case. */
static gx_color_index
bit_mono_map_color(gx_device * dev, const gx_color_value cv[])
{
    int bpc = dev->color_info.depth;
    int drop = sizeof(gx_color_value) * 8 - bpc;
    gx_color_value gray = cv[0];

    return (bpc == 1 ? gx_max_color_value - gray : gray) >> drop;
}

#if 0 /* unused */
/* Map RGB to gray shade. */
/* Only used in CMYK mode when put_params has set ForceMono=1 */
static gx_color_index
bit_forcemono_map_rgb_color(gx_device * dev, const gx_color_value cv[])
{
    gx_color_value color;
    int bpc = dev->color_info.depth / 4;	/* This function is used in CMYK mode */
    int drop = sizeof(gx_color_value) * 8 - bpc;
    gx_color_value gray, red, green, blue;
    red = cv[0]; green = cv[1]; blue = cv[2];
    gray = red;
    if ((red != green) || (green != blue))
	gray = (red * (unsigned long)lum_red_weight +
	     green * (unsigned long)lum_green_weight +
	     blue * (unsigned long)lum_blue_weight +
	     (lum_all_weights / 2))
		/ lum_all_weights;

    color = (gx_max_color_value - gray) >> drop;	/* color is in K channel */
    return color;
}
#endif

/* Map color to RGB.  This has 3 separate cases, but since it is rarely */
/* used, we do a case test rather than providing 3 separate routines. */
static int
bit_map_color_rgb(gx_device * dev, gx_color_index color, gx_color_value cv[4])
{
    int depth = dev->color_info.depth;
    int ncomp = REAL_NUM_COMPONENTS(dev);
    int bpc = depth / ncomp;
    uint mask = (1 << bpc) - 1;

#define cvalue(c) ((gx_color_value)((ulong)(c) * gx_max_color_value / mask))

    switch (ncomp) {
	case 1:		/* gray */
	    cv[0] =
		(depth == 1 ? (color ? 0 : gx_max_color_value) :
		 cvalue(color));
	    break;
	case 3:		/* RGB */
	    {
		gx_color_index cshift = color;

		cv[2] = cvalue(cshift & mask);
		cshift >>= bpc;
		cv[1] = cvalue(cshift & mask);
		cv[0] = cvalue(cshift >> bpc);
	    }
	    break;
	case 4:		/* CMYK */
	    /* Map CMYK back to RGB. */
	    {
		gx_color_index cshift = color;
		uint c, m, y, k;

		k = cshift & mask;
		cshift >>= bpc;
		y = cshift & mask;
		cshift >>= bpc;
		m = cshift & mask;
		c = cshift >> bpc;
		/* We use our improved conversion rule.... */
		cv[0] = cvalue((mask - c) * (mask - k) / mask);
		cv[1] = cvalue((mask - m) * (mask - k) / mask);
		cv[2] = cvalue((mask - y) * (mask - k) / mask);
	    }
	    break;
    }
    return 0;
#undef cvalue
}

/* Map CMYK to color. */
static gx_color_index
bit_map_cmyk_color(gx_device * dev, const gx_color_value cv[])
{
    int bpc = dev->color_info.depth / 4;
    int drop = sizeof(gx_color_value) * 8 - bpc;
    gx_color_index color =
    (((((((gx_color_index) cv[0] >> drop) << bpc) +
	(cv[1] >> drop)) << bpc) +
      (cv[2] >> drop)) << bpc) +
    (cv[3] >> drop);

    return (color == gx_no_color_index ? color ^ 1 : color);
}

static int
bittag_put_params(gx_device * pdev, gs_param_list * plist)
{
    gs_enable_object_tagging();
    return gdev_prn_put_params(pdev, plist);
}
/* Get parameters.  We provide a default CRD. */
static int
bit_get_params(gx_device * pdev, gs_param_list * plist)
{
    int code, ecode;
    /*
     * The following is a hack to get the original num_components.
     * See comment above.
     */
    int real_ncomps = REAL_NUM_COMPONENTS(pdev);
    int ncomps = pdev->color_info.num_components;
    int forcemono = (ncomps == real_ncomps ? 0 : 1);

    /*
     * Temporarily set num_components back to the "real" value to avoid
     * confusing those that rely on it.
     */
    pdev->color_info.num_components = real_ncomps;

    ecode = gdev_prn_get_params(pdev, plist);
    code = sample_device_crd_get_params(pdev, plist, "CRDDefault");
    if (code < 0)
	    ecode = code;
    if ((code = param_write_int(plist, "ForceMono", &forcemono)) < 0) {
	ecode = code;
    }
    if ((code = param_write_int(plist, "FirstLine", &((gx_device_bit *)pdev)->FirstLine)) < 0) {
	ecode = code;
    }
    if ((code = param_write_int(plist, "LastLine", &((gx_device_bit *)pdev)->LastLine)) < 0) {
	ecode = code;
    }

    /* Restore the working num_components */
    pdev->color_info.num_components = ncomps;

    return ecode;
}

/* Set parameters.  We allow setting the number of bits per component. */
/* Also, ForceMono=1 forces monochrome output from RGB/CMYK devices. */
static int
bit_put_params(gx_device * pdev, gs_param_list * plist)
{
    gx_device_color_info save_info;
    int ncomps = pdev->color_info.num_components;
    int real_ncomps = REAL_NUM_COMPONENTS(pdev);
    int bpc = pdev->color_info.depth / real_ncomps;
    int v;
    int ecode = 0;
    int code;
    static const byte depths[4][16] = {
	{1, 2, 0, 4, 8, 0, 0, 8, 0, 0, 0, 16, 0, 0, 0, 16},
	{0},
	{4, 8, 0, 16, 16, 0, 0, 24, 0, 0, 0, 40, 0, 0, 0, 48},
	{4, 8, 0, 16, 32, 0, 0, 32, 0, 0, 0, 48, 0, 0, 0, 64}
    };
    const char *vname;
    int FirstLine = ((gx_device_bit *)pdev)->FirstLine;
    int LastLine = ((gx_device_bit *)pdev)->LastLine;

    /*
     * Temporarily set num_components back to the "real" value to avoid
     * confusing those that rely on it.
     */
    pdev->color_info.num_components = real_ncomps;

    if ((code = param_read_int(plist, (vname = "GrayValues"), &v)) != 1 ||
	(code = param_read_int(plist, (vname = "RedValues"), &v)) != 1 ||
	(code = param_read_int(plist, (vname = "GreenValues"), &v)) != 1 ||
	(code = param_read_int(plist, (vname = "BlueValues"), &v)) != 1
	) {
	if (code < 0)
	    ecode = code;
	else
	    switch (v) {
		case   2: bpc = 1; break;
		case   4: bpc = 2; break;
		case  16: bpc = 4; break;
		case  32: bpc = 5; break;
		case 256: bpc = 8; break;
		case 4096: bpc = 12; break;
		case 65536: bpc = 16; break;
		default:
		    param_signal_error(plist, vname,
				       ecode = gs_error_rangecheck);
	    }
    }

    switch (code = param_read_int(plist, (vname = "ForceMono"), &v)) {
    case 0:
	if (v == 1) {
	    ncomps = 1;
	    break;
	}
	else if (v == 0) {
	    ncomps = real_ncomps;
	    break;
	}
	code = gs_error_rangecheck;
    default:
	ecode = code;
	param_signal_error(plist, vname, ecode);
    case 1:
	break;
    }
    if (ecode < 0)
	return ecode;
    switch (code = param_read_int(plist, (vname = "FirstLine"), &v)) {
    case 0:
        FirstLine = v;
	break;
    default:
	ecode = code;
	param_signal_error(plist, vname, ecode);
    case 1:
	break;
    }
    if (ecode < 0)
	return ecode;

    switch (code = param_read_int(plist, (vname = "LastLine"), &v)) {
    case 0:
        LastLine = v;
	break;
    default:
	ecode = code;
	param_signal_error(plist, vname, ecode);
    case 1:
	break;
    }
    if (ecode < 0)
	return ecode;

    /*
     * Save the color_info in case gdev_prn_put_params fails, and for
     * comparison.  Note that depth is computed from real_ncomps.
     */
    save_info = pdev->color_info;
    pdev->color_info.depth = depths[real_ncomps - 1][bpc - 1];
    pdev->color_info.max_gray = pdev->color_info.max_color =
	(pdev->color_info.dither_grays =
	 pdev->color_info.dither_colors =
	 (1 << bpc)) - 1;
    ecode = gdev_prn_put_params(pdev, plist);
    if (ecode < 0) {
	pdev->color_info = save_info;
	return ecode;
    }
    /* Now restore/change num_components. This is done after other	*/
    /* processing since it is used in gx_default_put_params		*/
    pdev->color_info.num_components = ncomps;
    if (pdev->color_info.depth != save_info.depth ||
	pdev->color_info.num_components != save_info.num_components
	) {
	gs_closedevice(pdev);
    }
    /* Reset the map_cmyk_color procedure if appropriate. */
    if (dev_proc(pdev, map_cmyk_color) == cmyk_1bit_map_cmyk_color ||
	dev_proc(pdev, map_cmyk_color) == cmyk_8bit_map_cmyk_color ||
	dev_proc(pdev, map_cmyk_color) == bit_map_cmyk_color) {
	set_dev_proc(pdev, map_cmyk_color,
		     pdev->color_info.depth == 4 ? cmyk_1bit_map_cmyk_color :
		     pdev->color_info.depth == 32 ? cmyk_8bit_map_cmyk_color :
		     bit_map_cmyk_color);
    }
    /* Reset the sparable and linear shift, masks, bits. */
    set_linear_color_bits_mask_shift(pdev);
    pdev->color_info.separable_and_linear = GX_CINFO_SEP_LIN;
    ((gx_device_bit *)pdev)->FirstLine = FirstLine;
    ((gx_device_bit *)pdev)->LastLine = LastLine;

    return 0;
}

/* Send the page to the printer. */
static int
bit_print_page(gx_device_printer * pdev, FILE * prn_stream)
{				/* Just dump the bits on the file. */
    /* If the file is 'nul', don't even do the writes. */
    int line_size = gdev_mem_bytes_per_scan_line((gx_device *) pdev);
    byte *in = gs_alloc_bytes(pdev->memory, line_size, "bit_print_page(in)");
    byte *data;
    int nul = !strcmp(pdev->fname, "nul") || !strcmp(pdev->fname, "/dev/null");
    int lnum = ((gx_device_bit *)pdev)->FirstLine;
    int bottom = ((gx_device_bit *)pdev)->LastLine;
    int line_count = any_abs(bottom - lnum);
    int i, step = lnum > bottom ? -1 : 1;

    if (in == 0)
	return_error(gs_error_VMerror);
    if ((lnum == 0) && (bottom == 0))
	bottom = pdev->height - 1;
    for (i = 0; i < line_count; i++, lnum += step) {
	gdev_prn_get_bits(pdev, lnum, in, &data);
	if (!nul)
	    fwrite(data, 1, line_size, prn_stream);
    }
    gs_free_object(pdev->memory, in, "bit_print_page(in)");
    return 0;
}
