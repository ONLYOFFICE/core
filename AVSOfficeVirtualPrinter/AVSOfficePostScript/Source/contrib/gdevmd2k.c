/* Copyright (C) 1991, 1995-1999 Aladdin Enterprises.  All rights reserved.
  
  This file is part of Aladdin Ghostscript.
  
  Aladdin Ghostscript is distributed with NO WARRANTY OF ANY KIND.  No author
  or distributor accepts any responsibility for the consequences of using it,
  or for whether it serves any particular purpose or works at all, unless he
  or she says so in writing.  Refer to the Aladdin Ghostscript Free Public
  License (the "License") for full details.
  
  Every copy of Aladdin Ghostscript must include a copy of the License,
  normally in a plain ASCII text file named PUBLIC.  The License grants you
  the right to copy, modify and redistribute Aladdin Ghostscript, but only
  under certain conditions described in the License.  Among other things, the
  License requires that the copyright notice and this notice be preserved on
  all copies.
*/

/*$Id: gdevmd2k.c,v 1.4 2002/07/12 20:46:29 tillkamppeter Exp $*/
/* ALPS MD series printer driver for Ghostscript 5.xx */
/*
 *  1999-04-18  Ver. 0.1   Masakazu Higaki <higamasa@dd.mbn.or.jp>
 *  1999-05-02  Ver. 0.2a
 */

#include "gdevprn.h"
#include "gsparam.h"
#include <stdlib.h>
#include <limits.h>

#define MM_PER_INCH 25.4
#define TOP_MARGIN    12. / MM_PER_INCH
#define BOTTOM_MARGIN 15. / MM_PER_INCH
#define LEFT_MARGIN   3.4 / MM_PER_INCH
#define RIGHT_MARGIN  3.4 / MM_PER_INCH

/* The device descriptor */
static dev_proc_open_device(alps_open);
static dev_proc_get_params(alps_get_params);
static dev_proc_put_params(alps_put_params);
static dev_proc_print_page(md2k_print_page);
static dev_proc_print_page(md5k_print_page);
static dev_proc_map_cmyk_color(alps_map_cmyk_color);
static dev_proc_map_cmyk_color(alps_map_cmy_color);
static dev_proc_map_rgb_color(alps_map_rgb_color);
static dev_proc_map_color_rgb(alps_map_color_rgb);


struct gx_device_alps_s {
    gx_device_common;
    gx_prn_device_common;
    bool color;			/* grayscale / color */
    bool dither;
    bool manualFeed;
    bool reverseSide;
    bool ecoBlack;
    int mediaType;
    int cyan;			/* weight for cyan */
    int magenta;		/* weight for magenta */
    int yellow;			/* weight for yellow */
    int black;			/* weight for black */
};

typedef struct gx_device_alps_s gx_device_alps;

#define dev_alps ((gx_device_alps *) pdev)

static gx_device_procs alps_procs = {
    alps_open,
    gx_default_get_initial_matrix,
    NULL,			/* sync_output */
    gdev_prn_output_page,
    gdev_prn_close,
    NULL,			/* map_rgb_color,*/
    alps_map_color_rgb,
    NULL,			/* fill_rectangle */
    NULL,			/* tile_rectangle */
    NULL,			/* copy_mono */
    NULL,			/* copy_color */
    NULL,			/* draw_line */
    NULL,			/* get_bits */
    alps_get_params,
    alps_put_params,
    alps_map_cmyk_color,
    NULL,			/* get_xfont_procs */
    NULL,			/* get_xfont_device */
    NULL,			/* map_rgb_alpha_color */
    gx_page_device_get_page_device /* get_page_device */
};

#define alps_device(dname, print_page) \
{\
    prn_device_body(gx_device_alps, alps_procs, dname,\
		    DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,\
		    600, 600,\
		    0, 0, 0, 0, /* margin */\
		    4, 0, 0, 0, 0, 0, print_page),\
    true, false, false, false, false, 0, 1024, 1024, 1024, 1024\
}

gx_device_alps far_data gs_md2k_device = alps_device("md2k", md2k_print_page);
gx_device_alps far_data gs_md5k_device = alps_device("md5k", md5k_print_page);

/* Printer types */
typedef enum {
    MD2000,
    MD1000,
    MD5000
} alps_printer_type;


static int
alps_open(gx_device *pdev)
{
    int xdpi = pdev->x_pixels_per_inch;
    int ydpi = pdev->y_pixels_per_inch;
    const float margins[4] = {
	LEFT_MARGIN,
	BOTTOM_MARGIN,
	RIGHT_MARGIN,
	TOP_MARGIN
    };
    float density;

    gx_device_set_margins(pdev, margins, true);

    /* resolution check */
    if ((ydpi != 300 || xdpi != 300)
     && (ydpi != 600 || xdpi != 600)
     && (ydpi != 600 || xdpi != 1200))
	return_error(gs_error_rangecheck);

    density = (xdpi == 300 ? 0.75 : xdpi == 600 ? 0.44 : 0.4);
    dev_alps->cyan    *= density;
    dev_alps->magenta *= density;
    dev_alps->yellow  *= density;
    dev_alps->black   *= density;

    return gdev_prn_open(pdev);
}


static int
alps_get_params(gx_device *pdev, gs_param_list *plist)
{
    gs_param_string mediaType = { "", 1, false };
    int code = gdev_prn_get_params(pdev, plist);
    if (code < 0 ||
	(code = param_write_bool(plist, "Color",   &dev_alps->color))   < 0 ||
	(code = param_write_bool(plist, "Dither",  &dev_alps->dither))  < 0 ||
	(code = param_write_bool(plist, "ManualFeed",
				        &dev_alps->manualFeed))         < 0 ||
	(code = param_write_bool(plist, "ReverseSide",
				        &dev_alps->reverseSide))        < 0 ||
	(code = param_write_bool(plist, "EcoBlack",
				        &dev_alps->ecoBlack))           < 0 ||
	(code = param_write_int (plist, "Cyan",	   &dev_alps->cyan))    < 0 ||
	(code = param_write_int (plist, "Magenta", &dev_alps->magenta)) < 0 ||
	(code = param_write_int (plist, "Yellow",  &dev_alps->yellow))  < 0 ||
	(code = param_write_int (plist, "Black",   &dev_alps->black))   < 0 ||
	(code = param_write_string(plist, "MediaType", &mediaType))     < 0)
	return code;

    return code;
}

static int
alps_put_param_bool(gs_param_list *plist, gs_param_name pname, int *pvalue,
		   int ecode)
{
    int code, value;
    switch (code = param_read_bool(plist, pname, &value)) {
    default:
	return code;
    case 1:
	return ecode;
    case 0:
	*pvalue = value;
	return (ecode < 0 ? ecode : 1);
    }
}

static int
alps_put_param_int(gs_param_list *plist, gs_param_name pname, int *pvalue,
		   int minval, int maxval, int ecode)
{
    int code, value;
    switch (code = param_read_int(plist, pname, &value)) {
    default:
	return code;
    case 1:
	return ecode;
    case 0:
	if (value < minval || value > maxval)
	    param_signal_error(plist, pname, gs_error_rangecheck);
	*pvalue = value;
	return (ecode < 0 ? ecode : 1);
    }
}

/* Put properties. */
static int
alps_put_params(gx_device *pdev, gs_param_list *plist)
{
    int code = 0;
    int bpp = dev_alps->color_info.depth;
    bool color       = dev_alps->color;
    bool dither      = dev_alps->dither;
    bool manualFeed  = dev_alps->manualFeed;
    bool reverseSide = dev_alps->reverseSide;
    bool ecoBlack    = dev_alps->ecoBlack;
    int cyan         = dev_alps->cyan;
    int magenta      = dev_alps->magenta;
    int yellow       = dev_alps->yellow;
    int black        = dev_alps->black;
    gs_param_string    mediaType;

    code = alps_put_param_bool(plist, "Color",       &color,        code);
    code = alps_put_param_bool(plist, "Dither",      &dither,       code);
    code = alps_put_param_bool(plist, "ManualFeed",  &manualFeed,   code);
    code = alps_put_param_bool(plist, "ReverseSide", &reverseSide,  code);
    code = alps_put_param_bool(plist, "EcoBlack",    &ecoBlack,     code);
    code = alps_put_param_int (plist, "BitsPerPixel", &bpp, 1, 32,  code);
    code = alps_put_param_int (plist, "Cyan",    &cyan,    0, 2048, code);
    code = alps_put_param_int (plist, "Magenta", &magenta, 0, 2048, code);
    code = alps_put_param_int (plist, "Yellow",  &yellow,  0, 2048, code);
    code = alps_put_param_int (plist, "Black",   &black,   0, 2048, code);

#define mediaTypeCmp(mname) strncmp(mediaType.data, mname, mediaType.size)
    if (param_read_string(plist, "MediaType", &mediaType) == 0) {
	dev_alps->mediaType
	    = (! mediaTypeCmp("PlainPaper"      ) ? 0
	     : ! mediaTypeCmp("OHP_MD2000"      ) ? 1
	     : ! mediaTypeCmp("IronSeal"        ) ? 2
	     : ! mediaTypeCmp("RebecaFree"      ) ? 3
	     : ! mediaTypeCmp("CardBoard"       ) ? 5
	     : ! mediaTypeCmp("PostCard"        ) ? 6
	     : ! mediaTypeCmp("FinePaper"       ) ? 7
	     : ! mediaTypeCmp("CoatedFilm"      ) ? 9
	     : ! mediaTypeCmp("GlossyPaper"     ) ? 15
	     : ! mediaTypeCmp("TransparencyFilm") ? 0x108
	     : ! mediaTypeCmp("OHP"             ) ? 0x108 : -1);
	switch (dev_alps->mediaType) {
	case -1:
	    param_signal_error(plist, "MediaType", code = gs_error_rangecheck);
	    break;
	case 1:			/* MD-2000 transparency mode */
	    dev_proc(pdev, map_cmyk_color) = alps_map_cmy_color;
	    break;
	case 6:			/* hagaki */
	    pdev->MediaSize[0] = 284;
	    pdev->MediaSize[1] = 419;
	default:
	    break;
	}
    }
    if (code < 0)
	return code;

    dev_alps->color       = color;
    dev_alps->dither      = dither;
    dev_alps->manualFeed  = manualFeed;
    dev_alps->reverseSide = reverseSide;
    dev_alps->ecoBlack    = ecoBlack;
    dev_alps->cyan     	  = cyan;
    dev_alps->magenta  	  = magenta;
    dev_alps->yellow   	  = yellow;
    dev_alps->black    	  = black;

    if (bpp)
	/* Only valid bits-per-pixel are 1, 4, 8, 32 */
	bpp = (bpp < 4 ? 1 : bpp < 8 ? 4 : bpp > 8 ? 32 : 8);
    else
	bpp = (color ? 4 : 1) * (dither ? 8 : 1);
    
    if (ecoBlack && ! strcmp(pdev->dname, "md5k"))
	bpp = 1;

    if (bpp == 1 || bpp == 8)
	dev_proc(pdev, map_rgb_color) = alps_map_rgb_color;

    pdev->color_info.depth = bpp;
    pdev->color_info.num_components = (bpp == 1 || bpp == 8 ? 1 : 4);
    pdev->color_info.max_gray = (bpp >= 8 ? 255 : 1);
    pdev->color_info.max_color = (bpp >= 8 ? 255 : bpp > 1 ? 1 : 0);
    pdev->color_info.dither_grays = (bpp >= 8 ? 5 : 2);
    pdev->color_info.dither_colors = (bpp >= 8 ? 5 : bpp > 1 ? 2 : 0);

    gdev_prn_put_params(pdev, plist);
    return 0;
}


/*
 * these F/S dithering macros are based on gdevcdj.c
 */
/* Floyd-Steinberg dithering. Often results in a dramatic improvement in
 * subjective image quality, but can also produce dramatic increases in
 * amount of printer data generated and actual printing time!! Mode 9 2D
 * compression is still useful for fairly flat color or blank areas but its
 * compression is much less effective in areas where the dithering has
 * effectively randomised the dot distribution. */

#define I sizeof(int)
#define SHIFT ((I * 8) - 13)
#define RSHIFT ((I * 8) - 16)
#define RANDOM() (((rand() << RSHIFT) % (MAXVALUE / 2))  - MAXVALUE / 4);
#define MINVALUE  0
#define MAXVALUE  (255 << SHIFT)
#define THRESHOLD (128 << SHIFT)
#define C 8

#define FSdither(inP, out, errP, Err, Bit, Offset)\
    oldErr = Err;\
    Err = (*errP + ((Err * 7 + C) >> 4) + (*inP << SHIFT));\
    inP += Offset;\
    if (Err > THRESHOLD) {\
	out |= Bit;\
	Err -= MAXVALUE;\
    }\
    errP[-Offset] += ((Err * 3 + C) >> 4);\
    *errP++ = ((Err * 5 + oldErr + C) >> 4);

#define FSDline(i, j, len, outP, n)\
{\
    byte c, bitmask;\
    int Err = 0, oldErr;\
    for (i = 0; i < len; i++) {\
        bitmask = 0x80;\
        for (c = j = 0; j < 8; j++) {\
            FSdither(dp, c, ep, Err, bitmask, n);\
            bitmask >>= 1;\
	}\
	*outP++ = c;\
    }\
}
/* END MACROS FOR DITHERING */

/*
 * get a component of CMYK from raster data
 */
static int
cmyk_to_bit(byte *out, byte *in, int length, int c_comp)
{
    byte *p_in = in, *p_out = out, *p_end;
    byte in_data, out_data;
    byte mask1, mask2;

    p_end = in + length;
    mask1 = 0x80 >> c_comp;
    mask2 = mask1 >> 4;

    while(p_in < p_end) {
	int i; 

	out_data = 0;
	for(i = 0; i < 4; i ++) {
	    in_data = (p_in < p_end ? *p_in++ : 0);
	    out_data <<= 2;
	    out_data |= (in_data & mask1) >> (6-c_comp)
		| (in_data & mask2) >> (3-c_comp);
	}
	*p_out ++ = out_data;
    }

    return p_out - out;
}


/*
 * run-length compression
 */
static int
runlength(byte *out, byte *in, int length)
{
    byte *p_in = in, *p_out = out, *p_end;
    int count, climit, contflag;

    p_end = p_in + length;

    while(p_in < p_end) {
	/* loop until 3 same data found */
	count = 0;
	climit = min(128, p_end - p_in);
	for( ; count < climit; count++, p_in ++) {
	    contflag = (*p_in == *(p_in+1) && *p_in == *(p_in+2));
	    if (contflag)
		break;
	}
	if (count > 0) {
	    *p_out ++ = count - 1;
	    memcpy(p_out, p_in - count, count);
	    p_out += count;
	}

	if (contflag) {
	    climit = min(129, p_end - p_in);
	    p_in += 3;
	    count = 3;
	    for( ; count < climit && *(p_in-1) == *p_in; count++, p_in++);

	    *p_out ++ = (byte) (0x101 - count);
	    *p_out ++ = *(p_in-1);
	}
    }

    return p_out - out;
}


#define write_short(data, stream) { \
    fputc((unsigned char) (data), stream); \
    fputc((unsigned short) (data) >> 8, stream); \
}

#define alps_cmd(cmd1, data, cmd2, stream) { \
    fwrite(cmd1, 1, 3, stream); \
    write_short(data, stream); \
    fputc(cmd2, stream); \
}

static void
alps_init(gx_device_printer *pdev, FILE *prn_stream, alps_printer_type ptype)
{
    short height;		/* page height (unit: dots) */
    
    fwrite  ("\033\145"
	     "\033\045\200\101"
	     "\033\032\0\0\114", 1, 11, prn_stream);
    /* paper feed (auto=1, manual=2) */
    alps_cmd("\033\046\154", (dev_alps->manualFeed ? 2 : 1), 0110, prn_stream);
    /* media type */
    alps_cmd("\033\046\154", dev_alps->mediaType, 0115, prn_stream);
    
    /* paper size */
    alps_cmd("\033\046\154", (pdev->MediaSize[0] == 612) ? 2 :    /* letter */
	                     (pdev->MediaSize[0] >= 595 &&
	                      pdev->MediaSize[0] <= 598) ? 4 :    /* A4 */
	                     (pdev->MediaSize[0] == 516 ||
			      pdev->MediaSize[0] == 501) ? 5 :    /* B5 */
	                     (pdev->MediaSize[0] == 284) ? 6 : 0, /* hagaki */
	     0101, prn_stream);

    /* monocrome=0, eco black=1, CMYK=4, CMYK(MD-5000)=8 */
    fwrite("\033\052\162", 1, 3, prn_stream);
    fputc((dev_alps->mediaType == 1 ? 4 :
	   pdev->color_info.num_components == 1 ? dev_alps->ecoBlack ? 1 : 0
	                                        : ptype == MD5000    ? 8 : 4),
	  prn_stream);
    fputc(0125, prn_stream);
   
    /* set resolution (300dpi = 2, 600dpi = 3, 1200x600dpi = 4) */
    fwrite("\033\052\164", 1, 3, prn_stream);
    fputc((pdev->x_pixels_per_inch == 300 ? 2
         : pdev->x_pixels_per_inch == 600 ? 3 : 4), prn_stream);
    fputc(0122, prn_stream);

    height = (pdev->MediaSize[1] - pdev->HWMargins[1] - pdev->HWMargins[3])
	    * pdev->y_pixels_per_inch / 72.;
    alps_cmd("\033\046\154", height, 0120, prn_stream);

    /* if -dReverseSide ... */
    fwrite("\033\032", 1, 2, prn_stream);
    fputc (dev_alps->reverseSide, prn_stream);
    fwrite("\0\101",   1, 2, prn_stream);

    if (ptype == MD5000) {
	if (dev_alps->ecoBlack) {
	    fwrite("\033\032\001\0\103",         1, 5, prn_stream);
	    fwrite("\033\046\154\001\0\103\027", 1, 7, prn_stream);
	} else if (pdev->color_info.num_components == 1)
	    fwrite("\033\046\154\001\0\103\0",   1, 7, prn_stream);
	else
	    fwrite("\033\046\154\004\0\103\003\002\001\0", 1, 10, prn_stream);
	fwrite("\033\032\0\0\125",     1, 5, prn_stream);
	fwrite("\033\052\162\1\101",   1, 5, prn_stream);
	fwrite("\033\052\142\0\0\115", 1, 6, prn_stream);
    } else {
	fwrite("\033\052\162\0\101",   1, 5, prn_stream);
	fwrite("\033\052\142\2\0\115", 1, 6, prn_stream);
    }
}

/* Send the page to the printer. */
static int
alps_print_page(gx_device_printer *pdev, FILE *prn_stream,
		alps_printer_type ptype)
{
    int line_size = gdev_mem_bytes_per_scan_line((gx_device *)pdev);
    byte *in, *out, *work, *dp, *outP;
    int y, y_height = pdev->height;
    int c_comp, num_comp = pdev->color_info.num_components;
    int n_comp = (dev_alps->mediaType == 1 ? 3 : num_comp);
    int *error, *ep;
    int i, j;

    /* allocate memory */
    work = (byte *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), 3+sizeof(int), line_size,
			     "alps_print_page(work)");
    if (work == 0)
	return_error(gs_error_VMerror);
    in = work + line_size;
    out = in + line_size;
    error = (int *)(out + line_size);

    /* initialize printer */
    alps_init(pdev, prn_stream, ptype);
    
    for(c_comp = 0; c_comp < n_comp; c_comp ++) {
	int yskip = 0;
	int color_weight;

	for(i = 0; i < line_size; i ++)
	    error[i] = RANDOM();

	if (n_comp == 1)
	    color_weight = dev_alps->black;
	else
	    color_weight = (c_comp < 2
		? c_comp == 0 ? dev_alps->cyan   : dev_alps->magenta
		: c_comp == 2 ? dev_alps->yellow : dev_alps->black);

	/* select color (black=0, cyan=1, magenta=2, yellow=3,
	                 gold=4, metalic red=5, metalic blue=6, silver=7,
			 rebeca black=8, rebeca red=9, rebeca blue=10,
			 white=11, glossy=14) */
	fwrite("\033\032", 1, 2, prn_stream);
	fputc((n_comp == 1 ? dev_alps->ecoBlack ? 026 : 0 : (c_comp+1) % 4),
	      prn_stream);
	fputc((c_comp == n_comp-1 && ptype == MD5000 ? 0200 : 0),
	      prn_stream);
	fputc(0162, prn_stream);

	for(y = 0; y < y_height; y ++) {
	    uint len = line_size;

	    gdev_prn_get_bits(pdev, y, in, &dp);

	    switch (pdev->color_info.depth) {
	    case 4:
		/* get a component of CMYK from raster data */
		len = cmyk_to_bit(work, dp, len, c_comp);
		dp = work;
	    case 1:
		/* remove trailing 0s */
		for( ; len > 0 && dp[len-1] == 0; len --);
		break;
	    case 32:
		dp += c_comp;
	    case 8:
		outP = work;
		ep = error;

		/* remove trailing 0s */
		for( ; len > 0 && dp[len-num_comp] == 0; len -= num_comp);
		
		for(i = 0; i < len; i += num_comp)
		    dp[i] = (dp[i] * color_weight) >> 10;
		len = (len / num_comp + 7) >> 3;

		FSDline(i, j, len, outP, num_comp);
		dp = work;
		break;
	    }

	    if (len == 0)
		yskip ++;
	    else {
		if (yskip) {
		    alps_cmd("\033\052\142", yskip, 0131, prn_stream);
		    yskip = 0;
		}
		if (ptype == MD5000) {
		    int xskip = 0;

		    /* Count pre print skip octets */
		    for( ; len > 0 && *dp == 0; len --, dp ++, xskip ++);

		    alps_cmd("\033\052\142", len, 0124, prn_stream);
		    write_short(xskip, prn_stream);
		    fwrite(dp, 1, len, prn_stream);
		} else {
		    len = runlength(out, dp, len);
		    alps_cmd("\033\052\142", len, 0127, prn_stream);
		    fwrite(out, 1, len, prn_stream);
		}
	    }
	}

	/* rewind */
	if (c_comp + 1 < n_comp)
	    fwrite("\033\032\0\0\014", 1, 5, prn_stream);
    }
    
    /* end of print */
    fwrite("\014"
	   "\033\052\162\103"
	   "\033\045\0\130", 1, 9, prn_stream);

    gs_free(gs_lib_ctx_get_non_gc_memory_t(), (char *)work, 3+sizeof(int), line_size, "alps_print_page(work)");
    return 0;
}

static int
md2k_print_page(gx_device_printer *pdev, FILE *prn_stream)
{
    return alps_print_page(pdev, prn_stream, MD2000);
}

static int
md5k_print_page(gx_device_printer *pdev, FILE *prn_stream)
{
    return alps_print_page(pdev, prn_stream, MD5000);
}


/* functions below are mainly based on gdevcdj.c */
/*
 * Map a CMYK color to a color index. We just use depth / 4 bits per color
 * to produce the color index.
 */

#define gx_color_value_to_bits(cv, b) \
    ((cv) >> (gx_color_value_bits - (b)))
#define gx_bits_to_color_value(cv, b) \
    ((cv) << (gx_color_value_bits - (b)))

#define gx_cmyk_value_bits(c, m, y, k, b) \
    ((gx_color_value_to_bits((c), (b)) << (3 * (b))) | \
     (gx_color_value_to_bits((m), (b)) << (2 * (b))) | \
     (gx_color_value_to_bits((y), (b)) << (b)) | \
     (gx_color_value_to_bits((k), (b))))

#define gx_value_cmyk_bits(v, c, m, y, k, b) \
    (c) = gx_bits_to_color_value(((v) >> (3 * (b))) & ((1 << (b)) - 1), (b)), \
    (m) = gx_bits_to_color_value(((v) >> (2 * (b))) & ((1 << (b)) - 1), (b)), \
    (y) = gx_bits_to_color_value(((v) >> (b)) & ((1 << (b)) - 1), (b)), \
    (k) = gx_bits_to_color_value((v) & ((1 << (b)) - 1), (b))

static gx_color_index
alps_map_cmyk_color(gx_device* pdev,
		    const gx_color_value cv[])
{
    gx_color_index color;
    gx_color_value c, m, y, k;

    c = cv[0]; m = cv[1]; y = cv[2]; k = cv[3];
    switch (pdev->color_info.depth) {
    case 1:
	color = ((c | m | y | k) > gx_max_color_value / 2 ?
		 (gx_color_index) 1 : (gx_color_index) 0);
	break;

    default:
    {
	int nbits = pdev->color_info.depth;
	ulong weight;

	ulong black = (c <= y ? c <= m ? c : m : m <= y ? m : y);
	weight = (black < gx_max_color_value
		  ? (gx_max_color_value << 10) / (gx_max_color_value - black)
		  : 0);
	c = ((c - black) * weight) >> 10;
	m = ((m - black) * weight) >> 10;
	y = ((y - black) * weight) >> 10;

	black += k;
	k = (gx_color_index) (black <= gx_max_color_value
			      ? black : gx_max_color_value);

	color = gx_cmyk_value_bits(c, m, y, k, nbits >> 2);
    }
    }

   return color;
}

static gx_color_index
alps_map_cmy_color(gx_device* pdev,
		   const gx_color_value cv[])
{
    int nbits = pdev->color_info.depth;
    gx_color_value c = cv[0], m = cv[1], y = cv[2], k = cv[3];
    ulong weight = ((gx_max_color_value - k) << 10) / gx_max_color_value;
    c = ((c * weight) >> 10) + k;
    m = ((m * weight) >> 10) + k;
    y = ((y * weight) >> 10) + k;
    if (c > gx_max_color_value) c = gx_max_color_value;
    if (m > gx_max_color_value) m = gx_max_color_value;
    if (y > gx_max_color_value) y = gx_max_color_value;

    return gx_cmyk_value_bits(c, m, y, 0, nbits >> 2);
}

/* Mapping of RGB colors to gray values. */

static gx_color_index
alps_map_rgb_color(gx_device *pdev,
		   const gx_color_value cv[])
{
  gx_color_value r, g, b;

  r = cv[0]; g = cv[1]; b = cv[2];
  if (gx_color_value_to_byte(r & g & b) == 0xff) {
      return (gx_color_index) 0;	/* White */
  } else {
      gx_color_value c = gx_max_color_value - r;
      gx_color_value m = gx_max_color_value - g;
      gx_color_value y = gx_max_color_value - b;

      switch (pdev->color_info.depth) {
      case 1:
	  return ((c | m | y) > gx_max_color_value / 2 ?
		  (gx_color_index) 1 : (gx_color_index) 0);
	  break;

      case 8:
#define red_weight 306
#define green_weight 601
#define blue_weight 117
	  return ((ulong)c * red_weight
		+ (ulong)m * green_weight
		+ (ulong)y * blue_weight)
		    >> (gx_color_value_bits + 2);
	  break;
      }
  }

   return (gx_color_index) 0;	/* This should never happen. */
}

/* Mapping of CMYK colors. */

static int
alps_map_color_rgb(gx_device *pdev,
		   gx_color_index color, gx_color_value prgb[3])
{
    switch (pdev->color_info.depth) {
    case 1:
	prgb[0] = prgb[1] = prgb[2] = gx_max_color_value * (1 - color);
	break;

    case 8:
	if (pdev->color_info.num_components == 1) {
	    gx_color_value value = (gx_color_value) color ^ 0xff;

	    prgb[0] = prgb[1] = prgb[2] = (value << 8) + value;

	    break;
	}

    default:
    {
	unsigned long bcyan, bmagenta, byellow, black;
	int nbits = pdev->color_info.depth;

	gx_value_cmyk_bits(color, bcyan, bmagenta, byellow, black,
			   nbits >> 2);

#ifdef USE_ADOBE_CMYK_RGB

	/* R = 1.0 - min(1.0, C + K), etc. */

	bcyan += black, bmagenta += black, byellow += black;
	prgb[0] = (bcyan > gx_max_color_value ? (gx_color_value) 0 :
		   gx_max_color_value - bcyan);
	prgb[1] = (bmagenta > gx_max_color_value ? (gx_color_value) 0 :
		   gx_max_color_value - bmagenta);
	prgb[2] = (byellow > gx_max_color_value ? (gx_color_value) 0 :
		   gx_max_color_value - byellow);

#else

	    /* R = (1.0 - C) * (1.0 - K), etc. */

	prgb[0] = (gx_color_value)
	    ((ulong)(gx_max_color_value - bcyan) *
	     (gx_max_color_value - black) / gx_max_color_value);
	prgb[1] = (gx_color_value)
	    ((ulong)(gx_max_color_value - bmagenta) *
	     (gx_max_color_value - black) / gx_max_color_value);
	prgb[2] = (gx_color_value)
	    ((ulong)(gx_max_color_value - byellow) *
	     (gx_max_color_value - black) / gx_max_color_value);
	
#endif

    }
    }

    return 0;
}
