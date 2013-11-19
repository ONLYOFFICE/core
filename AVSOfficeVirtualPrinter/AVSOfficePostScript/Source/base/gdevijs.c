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
/* $Id: gdevijs.c 8563 2008-02-29 20:49:04Z giles $ */
/*
 * IJS device for Ghostscript.
 * Intended to work with any IJS compliant inkjet driver, including
 * hpijs 1.0 and later, an IJS-enhanced gimp-print driver, and
 * the IJS Windows GDI server (ijsmswin.exe).
 * 
 * DRAFT
 *
 * WARNING: The ijs server can be selected on the gs command line
 * which is a security risk, since any program can be run.
 * You should use -dSAFER which sets .LockSafetyParams to true 
 * before opening this device.
 *
 * 11/26/03 David Suffield (gdevijs-krgb-1.0.patch)
 * (c) 2003-2004 Copyright Hewlett-Packard Development Company, LP
 *
 * 1. Removed hpijs 1.0-1.0.2 workarounds, use hpijs 1.0.3 or higher.
 * 2. Added krgb support.
 *
 * 02/21/05 David Suffield (gdevijs-krgb-1.1.patch)
 * 1. Fixed segfault issue with 1-bit color space.
 * 2. Fixed z-order issue with colored text on black rectangle.
 *
 * 02/22/06 David Suffield (gdevijs-krgb-1.2.patch)
 * 1. Fixed krgb buffer overflow issue with out-of-band data in fill_rectangle and copy_mono. 
 *    This buffer overflow condition occurred with fullbleed print jobs that had k-band images.
 * 2. Added Dan Coby (artifex) fix for gsijs_read_string_malloc gs_free *str memory leak.
 *
 * 06/02/06 David Suffield (gdevijs-krgb-1.3.patch)
 * 1. Revisited the krgb buffer overflow issue with out-of-band data in fill_rectangle and 
 *    copy_mono. Changed the fill_rectangle and copy_mono to an inner loop buffer check 
 *    instead of a outer loop x/y extent check.
 * 2. As requested by Ralph Giles, added K 1-bit and 8-bit support for krgb, but only 1-bit is 
 *    implemented for now.
 *
 *    KRGB definition:
 *    1. K=1-bit or 8-bit black plane, RGB=24 bit color raster.
 *    2. K-plane will only contain objects that are black text and black line drawings.
 *    3. RGB raster will not contain K-plane objects.
 *    4. K resolution and RGB resolution will be equal.
 *    5. K-plane will be byte aligned.
 *    6. K-plane 1-bit definition; 1=black, 0=nothing (KRGB).
 *    7. K-plane 8-bit definition; 255=black, 0=nothing (KxRGB).
 *
 * 1/15/08 David Suffield (gdevijs-krgb-1.5.patch)
 * 1. Added checks for null forward device in the graphic procedures.
 * 2. Corrected the "force banding" code in gsijs_open. Needed for small images (IE: hagaki in landscape).
 *
 */

#include "unistd_.h"	/* for dup() */
#include <stdlib.h>
#include <fcntl.h>
#include "gdevprn.h"
#include "gp.h"
#include "ijs.h"
#include "ijs_client.h"

#if 0
#define KRGB_DEBUG
#endif

/* This should go into gdevprn.h, or, better yet, gdevprn should
   acquire an API for changing resolution. */
int gdev_prn_maybe_realloc_memory(gx_device_printer *pdev,
				  gdev_prn_space_params *old_space,
			          int old_width, int old_height,
			          bool old_page_uses_transparency);

/* Device procedures */

/* See gxdevice.h for the definitions of the procedures. */
static dev_proc_open_device(gsijs_open);
static dev_proc_close_device(gsijs_close);
static dev_proc_output_page(gsijs_output_page);
static dev_proc_get_params(gsijs_get_params);
static dev_proc_put_params(gsijs_put_params);
static dev_proc_finish_copydevice(gsijs_finish_copydevice);

/* Following definitions are for krgb support. */
static dev_proc_create_buf_device(gsijs_create_buf_device);
static dev_proc_fill_rectangle(gsijs_fill_rectangle);
static dev_proc_copy_mono(gsijs_copy_mono);
static dev_proc_fill_mask(gsijs_fill_mask);
static dev_proc_fill_path(gsijs_fill_path);
static dev_proc_stroke_path(gsijs_stroke_path);

static const gx_device_procs gsijs_procs = {
	gsijs_open,
	NULL,	/* get_initial_matrix */
	NULL,	/* sync_output */
	gsijs_output_page,
	gsijs_close,
	gx_default_rgb_map_rgb_color,
	gx_default_rgb_map_color_rgb,
	NULL,	/* fill_rectangle */
	NULL,	/* tile_rectangle */
	NULL,	/* copy_mono */
	NULL,	/* copy_color */
	NULL,	/* draw_line */
	NULL,	/* get_bits */
	gsijs_get_params,
	gsijs_put_params,
	NULL,	/* map_cmyk_color */
	NULL,	/* get_xfont_procs */
	NULL,	/* get_xfont_device */
	NULL,	/* map_rgb_alpha_color */
	gx_page_device_get_page_device,
	NULL,	/* get_alpha_bits */
	NULL,	/* copy_alpha */
	NULL,	/* get_band */
	NULL,	/* copy_rop */
	NULL,	/* fill_path */
	NULL,	/* stroke_path */
	NULL,	/* fill_mask */
	NULL,	/* fill_trapezoid */
	NULL,	/* fill_parallelogram */
	NULL,	/* fill_triangle */
	NULL,	/* draw_thin_line */
	NULL,	/* begin_image */
	NULL,	/* image_data */
	NULL,	/* end_image */
	NULL,	/* strip_tile_rectangle */
	NULL,	/* strip_copy_rop, */
	NULL,	/* get_clipping_box */
	NULL,	/* begin_typed_image */
	NULL,	/* get_bits_rectangle */
	NULL,	/* map_color_rgb_alpha */
	NULL,	/* create_compositor */
	NULL,	/* get_hardware_params */
	NULL,	/* text_begin */
	gsijs_finish_copydevice
};

typedef struct gx_device_ijs_s gx_device_ijs;

/* The device descriptor */
struct gx_device_ijs_s {
    gx_device_common;
    gx_prn_device_common;
    bool IjsUseOutputFD;
    char IjsServer[gp_file_name_sizeof]; /* name of executable ijs server */
    char *ColorSpace;
    int ColorSpace_size;
    int BitsPerSample;
    char *DeviceManufacturer;
    int DeviceManufacturer_size;
    char *DeviceModel;
    int DeviceModel_size;
    char *IjsParams;
    int IjsParams_size;

    /* Common setpagedevice parameters supported by ijs but not
       currently parsed by gx_prn_device. We prefix these with Ijs to
       avoid namespace collision if they do get added to gx_prn_device.
    */
    bool IjsTumble;
    bool IjsTumble_set;

    IjsClientCtx *ctx;
    int ijs_version;

    /* Additional parameters for krgb support. */
    int krgb_mode;     /* 0=false, 1=true */
    int k_bits;        /* number of bits in k plane, 1 or 8 */
    int k_path;        /* k plane path, 0=false, 1=true */
    int k_width;       /* k plane width in pixels */
    int k_band_size;   /* k plane buffer size in bytes, byte aligned */
    unsigned char *k_band;  /* k plane buffer */
    gx_device_procs prn_procs;  /* banding playback procedures */
};

#define DEFAULT_DPI 74   /* See gsijs_set_resolution() below. */

gx_device_ijs gs_ijs_device =
{
    prn_device_std_body(gx_device_ijs, gsijs_procs, "ijs",
			DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
			DEFAULT_DPI, DEFAULT_DPI,
			0, 0, 0, 0,
			24 /* depth */, NULL /* print page */),
    FALSE,	/* IjsUseOutputFD */
    "",		/* IjsServer */
    NULL,	/* ColorSpace */
    0,		/* ColorSpace_size */
    8,		/* BitsPerSample */
    NULL,	/* DeviceManufacturer */
    0,		/* DeviceManufacturer_size */
    NULL,	/* DeviceModel */
    0,		/* DeviceModel_size */
    NULL,	/* IjsParams */
    0,		/* IjsParams_size */

    FALSE,	/* Tumble */
    FALSE,	/* Tumble_set */

    NULL,	/* IjsClient *ctx */
    0,		/* ijs_version */
    0,          /* krgb_mode */
    0,          /* k_bits */
    0,          /* k_path */
    0,          /* k_width */
    0,          /* k_band_size */
    NULL        /* k_band buffer */
};


static int gsijs_client_set_param(gx_device_ijs *ijsdev, const char *key,
    const char *value);
static int gsijs_set_color_format(gx_device_ijs *ijsdev);
static int gsijs_read_int(gs_param_list *plist, gs_param_name pname, 
   int *pval, int min_value, int max_value, bool only_when_closed);
static int gsijs_read_bool(gs_param_list *plist, gs_param_name pname, 
   bool *pval, bool only_when_closed);
static int gsijs_read_string(gs_param_list * plist, gs_param_name pname, 
   char * str, uint size, bool safety, bool only_when_closed);

/**************************************************************************/

/* ---------------- Low-level graphic procedures ---------------- */

static unsigned char xmask[] =
{
   0x80,    /* x=0 */
   0x40,    /* 1 */
   0x20,    /* 2 */
   0x10,    /* 3 */
   0x08,    /* 4 */
   0x04,    /* 5 */
   0x02,    /* 6 */
   0x01     /* 7 */
};

static int gsijs_fill_rectangle(gx_device * dev, int x, int y, int w, int h,
           gx_color_index color)
{
   gx_device_ijs *ijsdev = (gx_device_ijs *)((gx_device_forward *)dev)->target;

   if (!ijsdev)
      return 0;  /* no forward device, bug?? */

   if (ijsdev->krgb_mode && ijsdev->k_path && y >= 0 && x >= 0) 
   {
      int raster = (ijsdev->k_width+7) >> 3;
      register unsigned char *dest;
      int dest_start_bit;
      int band_height = ijsdev->k_band_size/raster;
      int i,j;
      unsigned char *beg = ijsdev->k_band;
      unsigned char *end = ijsdev->k_band+ijsdev->k_band_size;
      unsigned char *p;

      if (h <= 0 || w <= 0)
         return 0;

      /* Check for out-of-band graphic. */
      if (x >= ijsdev->k_width || y >= band_height)
         return 0;  /* out-of-band */

      dest_start_bit = x & 7;
      dest=ijsdev->k_band+(raster*y)+(x >> 3);

      /* Note x,y orgin 0,0 is stored first byte 0 left to right. */

      if (color==0x0)
      { 
         /* Color is black, store in k plane band instead of regular band. */
         for (j=0; j<h; j++)
         {
            for (i=0; i<w; i++)
            {
               p = &dest[(dest_start_bit+i)>>3];
               if (p >= beg && p <= end)
                  *p |= xmask[(dest_start_bit+i)&7];
            }
            dest+=raster;    
         }
         return 0;
      }
      else
      {
         /* Color is not black, remove any k plane bits for z-order dependencies, store in regular band. */
         for (j=0; j<h; j++)
         {
            for (i=0; i<w; i++)
            {
               p = &dest[(dest_start_bit+i)>>3];
               if (p >= beg && p <= end)
                  *p &= ~xmask[(dest_start_bit+i)&7];
            }
            dest+=raster;    
         }
      }
   }

   return (*ijsdev->prn_procs.fill_rectangle)(dev, x, y, w, h, color);
}

static int gsijs_copy_mono(gx_device * dev, const byte * data,
      int dx, int draster, gx_bitmap_id id,
      int x, int y, int w, int height, gx_color_index zero, gx_color_index one)
{
   gx_device_ijs *ijsdev = (gx_device_ijs *)((gx_device_forward *)dev)->target;

   if (!ijsdev)
      return 0;  /* no forward device, bug?? */

   /*   if (ijsdev->krgb_mode && ijsdev->k_path && one==0x0) */
   if (ijsdev->krgb_mode && ijsdev->k_path) 
   {
      /* Store in k plane band instead of regular band. */
      int raster = (ijsdev->k_width+7) >> 3;       /* raster width in bytes, byte aligned */
      register unsigned char *dest;
      register const unsigned char *scan;
      int dest_start_bit;
      int scan_start_bit;
      int band_height = ijsdev->k_band_size/raster;
      int i,h=height;
      unsigned char *beg = ijsdev->k_band;
      unsigned char *end = ijsdev->k_band+ijsdev->k_band_size;
      unsigned char *p;
      
      if (h <= 0 || w <= 0)
         return 0;

      /* Check for out-of-band graphic. */
      if (x >= ijsdev->k_width || y >= band_height)
         return 0;  /* out-of-band */

      scan=data+(dx >> 3);
      dest_start_bit = x & 7;
      scan_start_bit = dx & 7;
      dest=ijsdev->k_band+(raster*y)+(x >> 3);

      if (one==0x0)
      {
         /* Color is black, store in k plane band instead of regular band. */
         while (h-- > 0)
         {
            for (i=0; i<w; i++)
            {
               if (scan[(scan_start_bit+i)>>3] & xmask[(scan_start_bit+i)&7])
               {
                  p = &dest[(dest_start_bit+i)>>3];
                  if (p >= beg && p <= end)
                     *p |= xmask[(dest_start_bit+i)&7];
               }
            }
            scan+=draster;
            dest+=raster;
         }
         return 0;
      }
      else
      {
         /* Color is not black, remove any k plane bits for z-order dependencies, store in regular band. */
         while (h-- > 0)
         {
            for (i=0; i<w; i++)
            {
               if (scan[(scan_start_bit+i)>>3] & xmask[(scan_start_bit+i)&7])
               {
                  p = &dest[(dest_start_bit+i)>>3];
                  if (p >= beg && p <= end)
                     *p &= ~xmask[(dest_start_bit+i)&7];
               }
            }
            scan+=draster;
            dest+=raster;
         }
      }   
   }

   return (*ijsdev->prn_procs.copy_mono)(dev, data, dx, draster, id, x, y, w, height, zero, one);
}

/* ---------------- High-level graphic procedures ---------------- */

static int gsijs_fill_mask(gx_device * dev,
      const byte * data, int dx, int raster, gx_bitmap_id id,
      int x, int y, int w, int h,
      const gx_drawing_color * pdcolor, int depth,
      gs_logical_operation_t lop, const gx_clip_path * pcpath)
{
   gx_device_ijs *ijsdev = (gx_device_ijs *)((gx_device_forward *)dev)->target;
   int code;

   if (!ijsdev)
      return 0;  /* no forward device, bug?? */

   ijsdev->k_path = 1;

   code = (*ijsdev->prn_procs.fill_mask)(dev, data, dx, raster, id, x, y, w, h, pdcolor, depth, lop, pcpath);

   ijsdev->k_path = 0;

   return code;
}

static int gsijs_fill_path(gx_device * dev, const gs_imager_state * pis,
      gx_path * ppath, const gx_fill_params * params,
      const gx_drawing_color * pdcolor,
      const gx_clip_path * pcpath)
{
   gx_device_ijs *ijsdev = (gx_device_ijs *)((gx_device_forward *)dev)->target;
   int code;

   if (!ijsdev)
      return 0;  /* no forward device, bug?? */

   ijsdev->k_path = 1;

   code = (*ijsdev->prn_procs.fill_path)(dev, pis, ppath, params, pdcolor, pcpath);

   ijsdev->k_path = 0;

   return 0;
}

static int gsijs_stroke_path(gx_device * dev, const gs_imager_state * pis,
        gx_path * ppath, const gx_stroke_params * params,
        const gx_drawing_color * pdcolor,
        const gx_clip_path * pcpath)
{
   gx_device_ijs *ijsdev = (gx_device_ijs *)((gx_device_forward *)dev)->target;
   int code;

   if (!ijsdev)
      return 0;  /* no forward device, bug?? */

   ijsdev->k_path = 1;

   code = (*ijsdev->prn_procs.stroke_path)(dev, pis, ppath, params, pdcolor, pcpath);

   ijsdev->k_path = 0;

   return code;
}

/* ---------------- krgb banding playback procedures ---------------- */

static int gsijs_get_bits(gx_device_printer * pdev, int y, byte * str, byte ** actual_data)
{
   gx_device_ijs *ijsdev = (gx_device_ijs *)pdev;
   gx_device_clist_common *cdev = (gx_device_clist_common *)pdev;
   int band_height = cdev->page_info.band_params.BandHeight;
   int band_number = y/band_height;
   int raster = (ijsdev->k_width+7) >> 3;       /* raster width in bytes, byte aligned */
   int y1=raster*(y-(band_height*band_number));
 
   if (y1 == 0)
   {
      /* First raster for band, clear k_band. Banding playback occurs on first raster. */
      memset(ijsdev->k_band, 0, ijsdev->k_band_size); 
   }

   return gdev_prn_get_bits(pdev, y, str, actual_data);  /* get raster from regular band */
}

static int gsijs_k_get_bits(gx_device_printer * pdev, int y, byte ** actual_data)
{
   gx_device_ijs *ijsdev = (gx_device_ijs *)pdev;
   gx_device_clist_common *cdev = (gx_device_clist_common *)pdev;
   int band_height = cdev->page_info.band_params.BandHeight;
   int band_number = y/band_height;
   int raster = (ijsdev->k_width+7) >> 3;       /* raster width in bytes, byte aligned */
   int y1=raster*(y-(band_height*band_number));
 
   *actual_data = ijsdev->k_band+y1;

   return 0;
}

static int gsijs_create_buf_device(gx_device **pbdev, gx_device *target, int y,
            const gx_render_plane_t *render_plane, gs_memory_t *mem, gx_band_complexity_t *for_band)
{
   gx_device_ijs *ijsdev = (gx_device_ijs *)target;
   int n_chan = ijsdev->color_info.num_components;
   int code = gx_default_create_buf_device(pbdev, target, y, render_plane, mem, for_band);
   if (code < 0 || n_chan != 3)
      return code;

   /* Save buffer (vector) procedures so that we can hook them during banding playback. */
   ijsdev->prn_procs = (*pbdev)->procs;

   /* Replace buffer procedures with krgb procedures. */
   set_dev_proc(*pbdev, fill_rectangle, gsijs_fill_rectangle);
   set_dev_proc(*pbdev, copy_mono, gsijs_copy_mono);
   set_dev_proc(*pbdev, fill_mask, gsijs_fill_mask);
   set_dev_proc(*pbdev, fill_path, gsijs_fill_path);
   set_dev_proc(*pbdev, stroke_path, gsijs_stroke_path);

   return code;
}

/* See if IJS server supports krgb. */
static int
gsijs_set_krgb_mode(gx_device_ijs *ijsdev)
{
    char buf[256];
    int n_chan = ijsdev->color_info.num_components;
    int code;

    ijsdev->krgb_mode = 0;  /* default is no krgb */

    if (n_chan != 3)
        return 0;    /* no krgb support, not RGB colorspace */

    buf[0] = 0;
    code = ijs_client_enum_param(ijsdev->ctx, 0, "ColorSpace", buf, sizeof(buf)-1);
    if (code >= 0)
        buf[code] = 0;
    if (strstr(buf, "KRGB") != NULL)
    {
        ijsdev->krgb_mode = 1;     /* yes KRGB is supported */
        ijsdev->k_bits = 1;        /* KRGB = 1x8x8x8 */ 
    }
    else if (strstr(buf, "KxRGB") != NULL)
    {
        ijsdev->krgb_mode = 1;    /* yes KRGB is supported */
        ijsdev->k_bits = 8;       /* KRGB = 8x8x8x8 */
    }

    return 0; 
}

/* ------ Private definitions ------ */

static int
gsijs_parse_wxh (const char *val, int size, double *pw, double *ph)
{
    char buf[256];
    char *tail;
    int i;

    for (i = 0; i < size; i++)
	if (val[i] == 'x')
	    break;

    if (i + 1 >= size)
	return IJS_ESYNTAX;

    if (i >= sizeof(buf))
	return IJS_EBUF;

    memcpy (buf, val, i);
    buf[i] = 0;
    *pw = strtod (buf, &tail);
    if (tail == buf)
	return IJS_ESYNTAX;

    if (size - i > sizeof(buf))
	return IJS_EBUF;

    memcpy (buf, val + i + 1, size - i - 1);
    buf[size - i - 1] = 0;
    *ph = strtod (buf, &tail);
    if (tail == buf)
	return IJS_ESYNTAX;

    return 0;
}

/**
 * gsijs_set_generic_params: Set generic IJS parameters.
 **/
static int
gsijs_set_generic_params(gx_device_ijs *ijsdev)
{
    char buf[256];
    int code = 0;
    int i, j;
    char *value;

    /* Split IjsParams into separate parameters and send to ijs server */
    value = NULL;
    for (i=0, j=0; (j < ijsdev->IjsParams_size) && (i < sizeof(buf)-1); j++) {
	char ch = ijsdev->IjsParams[j];
	if (ch == '\\') {
	    j++;
	    buf[i++] = ijsdev->IjsParams[j];
	}
	else {
	    if (ch == '=') {
		buf[i++] = '\0';
		value = &buf[i];
	    }
	    else
		buf[i++] = ch;
	    if (ch == ',') {
		buf[i-1] = '\0';
		if (value)
		    gsijs_client_set_param(ijsdev, buf, value);
		i = 0;
		value = NULL;
	    }
	}
    }
    if (value)
	code = gsijs_client_set_param(ijsdev, buf, value);

    if (code == 0 && ijsdev->Duplex_set) {
	code = gsijs_client_set_param(ijsdev, "PS:Duplex",
				      ijsdev->Duplex ? "true" : "false");
    }
    if (code == 0 && ijsdev->IjsTumble_set) {
	code = gsijs_client_set_param(ijsdev, "PS:Tumble",
				      ijsdev->IjsTumble ? "true" :
				      "false");
    }
    return code;
}

/**
 * gsijs_set_margin_params: Do margin negotiation with IJS server.
 **/
static int
gsijs_set_margin_params(gx_device_ijs *ijsdev)
{
    char buf[256];
    int code = 0;
    int i, j;
    char *value;

    /* Split IjsParams into separate parameters and send to ijs server */
    value = NULL;
    for (i=0, j=0; (j < ijsdev->IjsParams_size) && (i < sizeof(buf)-1); j++) {
	char ch = ijsdev->IjsParams[j];
	if (ch == '\\') {
	    j++;
	    buf[i++] = ijsdev->IjsParams[j];
	}
	else {
	    if (ch == '=') {
		buf[i++] = '\0';
		value = &buf[i];
	    }
	    else
		buf[i++] = ch;
	    if (ch == ',') {
		buf[i-1] = '\0';
		if (value)
		    gsijs_client_set_param(ijsdev, buf, value);
		i = 0;
		value = NULL;
	    }
	}
    }
    if (value)
	code = gsijs_client_set_param(ijsdev, buf, value);

    if (code == 0 && ijsdev->Duplex_set) {
	code = gsijs_client_set_param(ijsdev, "Duplex",
				      ijsdev->Duplex ? "true" : "false");
    }
    if (code == 0 && ijsdev->IjsTumble_set) {
	code = gsijs_client_set_param(ijsdev, "Tumble",
				      ijsdev->IjsTumble ? "true" :
				      "false");
    }

    if (code == 0) {
	sprintf (buf, "%gx%g", ijsdev->MediaSize[0] * (1.0 / 72),
		 ijsdev->MediaSize[1] * (1.0 / 72));
	code = ijs_client_set_param(ijsdev->ctx, 0, "PaperSize",
				    buf, strlen(buf));
    }

    if (code == 0) {
	double printable_width, printable_height;
	double printable_left, printable_top;
	float m[4];

	code = ijs_client_get_param(ijsdev->ctx, 0, "PrintableArea",
				   buf, sizeof(buf));
	if (code == IJS_EUNKPARAM)
	    /* IJS server doesn't support margin negotiations.
	       That's ok. */
	    return 0;
	else if (code >= 0) {
	    code = gsijs_parse_wxh (buf, code,
				    &printable_width, &printable_height);
	}

	if (code == 0) {
	    code = ijs_client_get_param(ijsdev->ctx, 0, "PrintableTopLeft",
					buf, sizeof(buf));
	    if (code == IJS_EUNKPARAM)
		return 0;
	    else if (code >= 0) {
		code = gsijs_parse_wxh(buf, code,
					&printable_left, &printable_top);
	    }
	}

	if (code == 0) {
	    m[0] = printable_left;
	    m[3] = printable_top;
	    m[2] = ijsdev->MediaSize[0] * (1.0 / 72) -
		printable_left - printable_width;
	    m[1] = ijsdev->MediaSize[1] * (1.0 / 72) -
		printable_top - printable_height;
	    gx_device_set_margins((gx_device *)ijsdev, m, true);
	    sprintf (buf, "%gx%g", printable_left, printable_top);
	    code = ijs_client_set_param(ijsdev->ctx, 0, "TopLeft",
					buf, strlen(buf));
	}
    }

    return code;
}

/**
 * gsijs_set_resolution: Set resolution.
 *
 * The priority order, highest first, is: commandline -r switch,
 * if specified; IJS get_param of Dpi; 72 dpi default.
 *
 * Because Ghostscript doesn't have a really good way to detect
 * whether resolution was set on the command line, we set a
 * low-probability resolution (DEFAULT_DPI) in the static
 * initialization of the device, then detect whether it has been
 * changed from that.  This causes a minor infelicity: if DEFAULT_DPI
 * is set on the command line, it is changed to the default here.
 **/
static int
gsijs_set_resolution(gx_device_ijs *ijsdev)
{
    char buf[256];
    int code;
    floatp x_dpi, y_dpi;
    int width = ijsdev->width;
    int height = ijsdev->height;
    bool save_is_open = ijsdev->is_open;

    if (ijsdev->HWResolution[0] != DEFAULT_DPI ||
	ijsdev->HWResolution[1] != DEFAULT_DPI) {
	/* Resolution has been set on command line. */
	return 0;
    }
    code = ijs_client_get_param(ijsdev->ctx, 0, "Dpi",
				buf, sizeof(buf));
    if (code >= 0) {
	int i;

	for (i = 0; i < code; i++)
	    if (buf[i] == 'x')
		break;
	if (i == code) {
	    char *tail;

	    if (i == sizeof(buf))
		code = IJS_EBUF;
	    buf[i] = 0;
	    x_dpi = y_dpi = strtod (buf, &tail);
	    if (tail == buf)
		code = IJS_ESYNTAX;
	} else {
	    double x, y;

	    code = gsijs_parse_wxh(buf, code, &x, &y);
	    x_dpi = x;
	    y_dpi = y;
	}
    }

    if (code < 0) {
	x_dpi = 72.0;
	y_dpi = 72.0;
    }

    gx_device_set_resolution((gx_device *)ijsdev, x_dpi, y_dpi);

    ijsdev->is_open = true;
    code = gdev_prn_maybe_realloc_memory((gx_device_printer *)ijsdev,
					 &ijsdev->space_params, width, height,
					 ijsdev->page_uses_transparency);
    ijsdev->is_open = save_is_open;
    return code;
}

/* Open the gsijs driver */
static int
gsijs_open(gx_device *dev)
{
    gx_device_ijs *ijsdev = (gx_device_ijs *)dev;
    int code;
    char buf[256];
    bool use_outputfd;
    int fd = -1;

    if (strlen(ijsdev->IjsServer) == 0) {
	eprintf("ijs server not specified\n");
	return gs_note_error(gs_error_ioerror);
    }

    ijsdev->space_params.banding_type = BandingAlways;	/* always force banding */

    /* Set create_buf_device in printer device, so that we can hook the banding playback procedures. */
    ijsdev->printer_procs.buf_procs.create_buf_device = gsijs_create_buf_device;

    /* Decide whether to use OutputFile or OutputFD. Note: how to
       determine this is a tricky question, so we just allow the
       user to set it.
    */
    use_outputfd = ijsdev->IjsUseOutputFD;

    /* If using OutputFilename, we don't want to open the output file.
       Leave that to the ijs server. */
    ijsdev->OpenOutputFile = use_outputfd;

    code = gdev_prn_open(dev);
    if (code < 0)
	return code;

    if (use_outputfd) {
	/* Note: dup() may not be portable to all interesting IJS
	   platforms. In that case, this branch should be #ifdef'ed out.
	*/
	fd = dup(fileno(ijsdev->file));
    }

    /* WARNING: Ghostscript should be run with -dSAFER to stop
     * someone changing the ijs server (e.g. running a shell).
     */
    ijsdev->ctx = ijs_invoke_server(ijsdev->IjsServer);
    if (ijsdev->ctx == (IjsClientCtx *)NULL) {
	eprintf1("Can't start ijs server \042%s\042\n", ijsdev->IjsServer);
	return gs_note_error(gs_error_ioerror);
    }

    ijsdev->ijs_version = ijs_client_get_version (ijsdev->ctx);

    if (ijs_client_open(ijsdev->ctx) < 0) {
	eprintf("Can't open ijs\n");
	return gs_note_error(gs_error_ioerror);
    }
    if (ijs_client_begin_job(ijsdev->ctx, 0) < 0) {
	eprintf("Can't begin ijs job 0\n");
	ijs_client_close(ijsdev->ctx);
	return gs_note_error(gs_error_ioerror);
    }

    if (use_outputfd) {
	/* Note: dup() may not be portable to all interesting IJS
	   platforms. In that case, this branch should be #ifdef'ed out.
	*/
	sprintf(buf, "%d", fd);
	ijs_client_set_param(ijsdev->ctx, 0, "OutputFD", buf, strlen(buf)); 
	close(fd);
    } else {
	ijs_client_set_param(ijsdev->ctx, 0, "OutputFile", 
			     ijsdev->fname, strlen(ijsdev->fname));
    }

    if (code >= 0 && ijsdev->DeviceManufacturer)
	code = ijs_client_set_param(ijsdev->ctx, 0, "DeviceManufacturer",
			     ijsdev->DeviceManufacturer,
			     strlen(ijsdev->DeviceManufacturer));

    if (code >= 0 && ijsdev->DeviceModel)
	code = ijs_client_set_param(ijsdev->ctx, 0, "DeviceModel",
			     ijsdev->DeviceModel,
			     strlen(ijsdev->DeviceModel));

    if (code >= 0)
	code = gsijs_set_generic_params(ijsdev);

    if (code >= 0)
	code = gsijs_set_resolution(ijsdev);

    if (code >= 0)
	code = gsijs_set_margin_params(ijsdev);

    if (code >= 0)
        code = gsijs_set_krgb_mode(ijsdev);

    return code;
}

/* Finish device initialization. */
static int
gsijs_finish_copydevice(gx_device *dev, const gx_device *from_dev)
{
    int code;
    static const char rgb[] = "DeviceRGB";
    gx_device_ijs *ijsdev = (gx_device_ijs *)dev;

    code = gx_default_finish_copydevice(dev, from_dev);
    if(code < 0)
        return code;
 
    if (!ijsdev->ColorSpace) {
	ijsdev->ColorSpace = gs_malloc(ijsdev->memory, sizeof(rgb), 1, 
		"gsijs_finish_copydevice");
        if (!ijsdev->ColorSpace)
 	    return gs_note_error(gs_error_VMerror);
        ijsdev->ColorSpace_size = sizeof(rgb);
        memcpy(ijsdev->ColorSpace, rgb, sizeof(rgb));
    }
    return code;
}

/* Close the gsijs driver */
static int
gsijs_close(gx_device *dev)
{
    gx_device_ijs *ijsdev = (gx_device_ijs *)dev;
    int code;

    /* ignore ijs errors on close */
    ijs_client_end_job(ijsdev->ctx, 0);
    ijs_client_close(ijsdev->ctx);
    ijs_client_begin_cmd(ijsdev->ctx, IJS_CMD_EXIT);
    ijs_client_send_cmd_wait(ijsdev->ctx);

    code = gdev_prn_close(dev);
    if (ijsdev->IjsParams)
	gs_free(dev->memory, ijsdev->IjsParams,
		ijsdev->IjsParams_size, 1, "gsijs_read_string_malloc");
    if (ijsdev->ColorSpace)
	gs_free(dev->memory, ijsdev->ColorSpace,
		ijsdev->ColorSpace_size, 1, "gsijs_read_string_malloc");
    if (ijsdev->DeviceManufacturer)
	gs_free(dev->memory, ijsdev->DeviceManufacturer,
		ijsdev->DeviceManufacturer_size, 1, "gsijs_read_string_malloc");
    if (ijsdev->DeviceModel)
	gs_free(dev->memory, ijsdev->DeviceModel,
		ijsdev->DeviceModel_size, 1, "gsijs_read_string_malloc");
    ijsdev->IjsParams = NULL;
    ijsdev->IjsParams_size = 0;
    ijsdev->DeviceManufacturer = NULL;
    ijsdev->DeviceManufacturer_size = 0;
    ijsdev->DeviceModel = NULL;
    ijsdev->DeviceModel_size = 0;
    return code;
}

/* This routine is entirely analagous to gdev_prn_print_scan_lines(),
   but computes width instead of height. It is not specific to IJS,
   and a strong case could be made for moving it into gdevprn.c. */
static int
gsijs_raster_width(gx_device *pdev)
{
    int width = pdev->width;
    gs_matrix imat;
    float xscale;
    int right, offset, end;

    (*dev_proc(pdev, get_initial_matrix)) (pdev, &imat);
    xscale = imat.xx * 72.0;
    right = (int)(dev_r_margin(pdev) * xscale);
    offset = (int)(dev_x_offset(pdev) * xscale);
    end = offset + width - right;
    return min(width, end);
}

/* Print a page.  Don't use normal printer gdev_prn_output_page 
 * because it opens the output file.
 */
static int
gsijs_output_page(gx_device *dev, int num_copies, int flush)
{
    gx_device_ijs *ijsdev = (gx_device_ijs *)dev;
    gx_device_printer *pdev = (gx_device_printer *)dev;
    int raster = gdev_prn_raster(pdev);
    int ijs_width, ijs_height;
    int row_bytes, k_row_bytes=0;
    int n_chan = pdev->color_info.num_components;
    int krgb_mode = ijsdev->krgb_mode;
    int k_bits = ijsdev->k_bits;
    unsigned char *data;
    char buf[256];
    double xres = pdev->HWResolution[0];
    double yres = pdev->HWResolution[1];
    int code = 0;
    int endcode = 0;
    int status = 0;
    int i, y;

    if ((data = gs_alloc_bytes(pdev->memory, raster, "gsijs_output_page"))
	== (unsigned char *)NULL)
        return gs_note_error(gs_error_VMerror);

    /* Determine bitmap width and height */
    ijs_height = gdev_prn_print_scan_lines(dev);
	ijs_width = gsijs_raster_width(dev);

    row_bytes = (ijs_width * pdev->color_info.depth + 7) >> 3;

    if (krgb_mode)
    {
        gx_device_clist_common *cdev = (gx_device_clist_common *)dev;
        int band_height = cdev->page_info.band_params.BandHeight;
        k_row_bytes = (ijs_width + 7) >> 3;

        /* Create banding buffer for k plane. */
        ijsdev->k_width = ijs_width;
        ijsdev->k_band_size = band_height * k_row_bytes;   
        if ((ijsdev->k_band = gs_malloc(pdev->memory, ijsdev->k_band_size, 1, "gsijs_output_page")) == (unsigned char *)NULL)
           return gs_note_error(gs_error_VMerror);
    }

    /* Required page parameters */
    sprintf(buf, "%d", n_chan);
    gsijs_client_set_param(ijsdev, "NumChan", buf);
    sprintf(buf, "%d", ijsdev->BitsPerSample);
    gsijs_client_set_param(ijsdev, "BitsPerSample", buf);

    /* This needs to become more sophisticated for DeviceN. */
    strcpy(buf, (n_chan == 4) ? "DeviceCMYK" : 
	((n_chan == 3) ? (krgb_mode ? ((k_bits == 1) ? "KRGB" : "KxRGB") : "DeviceRGB") : "DeviceGray"));
    gsijs_client_set_param(ijsdev, "ColorSpace", buf);

    sprintf(buf, "%d", ijs_width);
    gsijs_client_set_param(ijsdev, "Width", buf);
    sprintf(buf, "%d", ijs_height);
    gsijs_client_set_param(ijsdev, "Height", buf);

    sprintf(buf, "%gx%g", xres, yres);
    gsijs_client_set_param(ijsdev, "Dpi", buf);

#ifdef KRGB_DEBUG
    int kfd, rgbfd;
    char sz[128];
    kfd = open("/tmp/k.pbm", O_CREAT | O_TRUNC | O_RDWR, 0644);
    rgbfd = open("/tmp/rgb.ppm", O_CREAT | O_TRUNC | O_RDWR, 0644);
    snprintf(sz, sizeof(sz), "P4\n#gdevijs test\n%d\n%d\n", ijs_width, ijs_height);
    write(kfd, sz, strlen(sz));
    snprintf(sz, sizeof(sz), "P6\n#gdevijs test\n%d\n%d\n255\n", ijs_width, ijs_height);
    write(rgbfd, sz, strlen(sz));
#endif

    for (i=0; i<num_copies; i++) {
 	unsigned char *actual_data;
	ijs_client_begin_cmd (ijsdev->ctx, IJS_CMD_BEGIN_PAGE);
	status = ijs_client_send_cmd_wait(ijsdev->ctx);

	for (y = 0; y < ijs_height; y++) {
            if (krgb_mode)
                code = gsijs_get_bits(pdev, y, data, &actual_data);
            else
                code = gdev_prn_get_bits(pdev, y, data, &actual_data);
            if (code < 0)
                break;
#ifdef KRGB_DEBUG
            write(rgbfd, actual_data, row_bytes);
#endif
            status = ijs_client_send_data_wait(ijsdev->ctx, 0, (char *)actual_data, row_bytes);
            if (status)
                break;

            if (krgb_mode) {
                code = gsijs_k_get_bits(pdev, y, &actual_data);
                if (code < 0)
                    break;
#ifdef KRGB_DEBUG
                write(kfd, actual_data, k_row_bytes);
#endif
                status = ijs_client_send_data_wait(ijsdev->ctx, 0, (char *)actual_data, k_row_bytes);
                if (status)
                    break;
	    }
	}
	ijs_client_begin_cmd(ijsdev->ctx, IJS_CMD_END_PAGE);
	status = ijs_client_send_cmd_wait(ijsdev->ctx);
    }

#ifdef KRGB_DEBUG
    close(kfd);
    close(rgbfd);
#endif

    if(krgb_mode)
        gs_free(pdev->memory, ijsdev->k_band, ijsdev->k_band_size, 1, "gsijs_output_page");

    gs_free_object(pdev->memory, data, "gsijs_output_page");

    endcode = (pdev->buffer_space && !pdev->is_async_renderer ?
	       clist_finish_page(dev, flush) : 0);


    if (endcode < 0)
	return endcode;

    if (code < 0)
	return endcode;

    if (status < 0)
	return gs_note_error(gs_error_ioerror);

    code = gx_finish_output_page(dev, num_copies, flush);
    return code;
}


/**************************************************************************/

/* Get device parameters */
static int
gsijs_get_params(gx_device *dev, gs_param_list *plist)
{
    gx_device_ijs *ijsdev = (gx_device_ijs *)dev;
    gs_param_string gps;
    int code = gdev_prn_get_params(dev, plist);

    if (code >= 0) {
	param_string_from_transient_string(gps, ijsdev->IjsServer);
	code = param_write_string(plist, "IjsServer", &gps);
    }

    if (code >= 0) {
	if (ijsdev->DeviceManufacturer) {
	    param_string_from_transient_string(gps,
					       ijsdev->DeviceManufacturer);
	    code = param_write_string(plist, "DeviceManufacturer", &gps);
	} else {
	    code = param_write_null(plist, "DeviceManufacturer");
	}
    }

    if (code >= 0) {
	if (ijsdev->DeviceModel) {
	    param_string_from_transient_string(gps, ijsdev->DeviceModel);
	    code = param_write_string(plist, "DeviceModel", &gps);
	} else {
	    code = param_write_null(plist, "DeviceModel");
	}
    }

    if (code >= 0) {
	if (ijsdev->IjsParams) {
	    param_string_from_transient_string(gps, ijsdev->IjsParams);
	    code = param_write_string(plist, "IjsParams", &gps);
	} else {
	    code = param_write_null(plist, "IjsParams");
	}
    }

    if (code >= 0)
 	code = param_write_int(plist, "BitsPerSample", &ijsdev->BitsPerSample);

    if (code >= 0)
 	code = param_write_bool(plist, "IjsUseOutputFD",
				&ijsdev->IjsUseOutputFD);

    if (code >= 0) {
	if (ijsdev->IjsTumble_set) {
	    code = param_write_bool(plist, "Tumble", &ijsdev->IjsTumble);
	} else {
	    code = param_write_null(plist, "Tumble");
	}
    }

    return code;
}

static int
gsijs_read_int(gs_param_list *plist, gs_param_name pname, int *pval,
     int min_value, int max_value, bool only_when_closed)
{
    int code = 0;
    int new_value;

    switch (code = param_read_int(plist, pname, &new_value)) {
	case 0:
	    if (only_when_closed && (new_value != *pval)) {
		code = gs_error_rangecheck;
		goto e;
	    }
	    if ((new_value >= min_value) && (new_value <= max_value)) {
		*pval = new_value;
		break;
	    }
	    code = gs_note_error(gs_error_rangecheck);
	    goto e;
	default:
	    if (param_read_null(plist, pname) == 0)
		return 1;
	    e:param_signal_error(plist, pname, code);
	case 1:
	    ;
    }
    return code;
}

static int
gsijs_read_bool(gs_param_list *plist, gs_param_name pname, bool *pval,
		bool only_when_closed)
{
    int code = 0;
    bool new_value;

    switch (code = param_read_bool(plist, pname, &new_value)) {
	case 0:
	    if (only_when_closed && (new_value != *pval)) {
		code = gs_error_rangecheck;
		goto e;
	    }
	    *pval = new_value;
	    break;
	default:
	    if (param_read_null(plist, pname) == 0) {
		return 1;
	    }
	    e:param_signal_error(plist, pname, code);
	case 1:
	    ;
    }
    return code;
}

static int
gsijs_read_string(gs_param_list *plist, gs_param_name pname, char *str,
    uint size, bool safety, bool only_when_closed)
{
    int code;
    gs_param_string new_value;
    int differs;

    switch (code = param_read_string(plist, pname, &new_value)) {
        case 0:
	    differs = bytes_compare(new_value.data, new_value.size,
			(const byte *)str, strlen(str));
	    if (safety && differs) {
		code = gs_error_invalidaccess;
		goto e;
	    }
	    if (only_when_closed && differs) {
		code = gs_error_rangecheck;
		goto e;
	    }
            if (new_value.size < size) {
		strncpy(str, (const char *)new_value.data, new_value.size);
		str[new_value.size+1] = '\0';
                break;
	    }
            code = gs_note_error(gs_error_rangecheck);
            goto e;
        default:
            if (param_read_null(plist, pname) == 0)
                return 1;
          e:param_signal_error(plist, pname, code);
        case 1:
            ;
    }
    return code;
}

static int
gsijs_read_string_malloc(gs_param_list *plist, gs_param_name pname, char **str,
    int *size, bool only_when_closed)
{
    int code;
    gs_param_string new_value;
    int differs;

    switch (code = param_read_string(plist, pname, &new_value)) {
        case 0:
	    differs = bytes_compare(new_value.data, new_value.size,
			(const byte *)(*str ? *str : ""), 
		  	*str ? strlen(*str) : 0);
	    if (only_when_closed && differs) {
		code = gs_error_rangecheck;
		goto e;
	    }
	    if (new_value.size + 1 != *size) {
	        if (*str)
		    gs_free(plist->memory, *str, *size, 1,
					"gsijs_read_string_malloc");
		*str = NULL;
		*size = 0;
	    }
	    if (*str == NULL)
	        *str = gs_malloc(plist->memory, new_value.size + 1, 1, 
					"gsijs_read_string_malloc");
	    if (*str == NULL) {
                code = gs_note_error(gs_error_VMerror);
                goto e;
	    }
	    *size = new_value.size + 1;
	    strncpy(*str, (const char *)new_value.data, new_value.size);
	    (*str)[new_value.size] = '\0';
            break;
        default:
            if (param_read_null(plist, pname) == 0)
                return 1;
          e:param_signal_error(plist, pname, code);
        case 1:
            ;
    }
    return code;
}


static int
gsijs_put_params(gx_device *dev, gs_param_list *plist)
{
    gx_device_ijs *ijsdev = (gx_device_ijs *)dev;
    int code = 0;
    bool is_open = dev->is_open;

    /* We allow duplex to be set in all cases. At some point, it may
       be worthwhile to query the device to see if it supports
       duplex. Note also that this code will get called even before
       the device has been opened, which is when the -DDuplex
       command line is processed. */
    if (ijsdev->Duplex_set < 0) {
	ijsdev->Duplex = 1;
	ijsdev->Duplex_set = 0;
    }

    /* If a parameter must not be changed after the device is open,
     * the last parameter of gsijs_read_xxx() is is_open.
     * If a parameter may be changed at any time, it is false.
     */
    if (code >= 0)
	code = gsijs_read_string(plist, "IjsServer", 
	    ijsdev->IjsServer, sizeof(ijsdev->IjsServer), 
	    dev->LockSafetyParams, is_open);

    if (code >= 0)
	code = gsijs_read_string_malloc(plist, "DeviceManufacturer", 
	    &ijsdev->DeviceManufacturer, &ijsdev->DeviceManufacturer_size, 
	    is_open);

    if (code >= 0)
	code = gsijs_read_string_malloc(plist, "DeviceModel", 
	    &ijsdev->DeviceModel, &ijsdev->DeviceModel_size, 
	    is_open);

    if (code >= 0)
	code = gsijs_read_string_malloc(plist, "IjsParams", 
	    &(ijsdev->IjsParams), &(ijsdev->IjsParams_size), is_open);

    if (code >= 0)
	code = gsijs_read_int(plist, "BitsPerSample", &ijsdev->BitsPerSample, 
		1, 16, is_open);

    if (code >= 0)
 	code = gsijs_read_bool(plist, "IjsUseOutputFD",
			       &ijsdev->IjsUseOutputFD, is_open);

    if (code >= 0) {
	code = gsijs_read_string_malloc(plist, "ProcessColorModel",
	    &ijsdev->ColorSpace, &ijsdev->ColorSpace_size, is_open);
    }

    if (code >= 0) {
 	code = gsijs_read_bool(plist, "Tumble", &ijsdev->IjsTumble, false);
	if (code == 0)
	    ijsdev->IjsTumble_set = true;
    }

    if (code >= 0)
	code = gsijs_set_color_format(ijsdev);

    if (code >= 0)
	code = gdev_prn_put_params(dev, plist);

    if (code >= 0 && is_open) {
	code = gsijs_set_generic_params(ijsdev);
	if (code >= 0)
	  code = gsijs_set_margin_params(ijsdev);
	if (code < 0)
	    return gs_note_error(gs_error_ioerror);
    }
    
    return code;
}

static int
gsijs_client_set_param(gx_device_ijs *ijsdev, const char *key,
    const char *value)
{
    int code = ijs_client_set_param(ijsdev->ctx, 0 /* job id */, 
	key, value, strlen(value));
    if (code < 0)
	dprintf2("ijs: Can't set parameter %s=%s\n", key, value);
    return code;
}

static int
gsijs_set_color_format(gx_device_ijs *ijsdev)
{
    gx_device_color_info dci = ijsdev->color_info;
    int components;	/* 1=gray, 3=RGB, 4=CMYK */
    int bpc = ijsdev->BitsPerSample;		/* bits per component */
    int maxvalue;
    const char *ColorSpace = ijsdev->ColorSpace;

    if (ColorSpace == NULL)
	ColorSpace = "DeviceRGB";

    if (!strcmp (ColorSpace, "DeviceGray")) {
	components = 1;
	if (bpc == 1) {
	    ijsdev->procs.map_rgb_color = gx_default_w_b_map_rgb_color;
	    ijsdev->procs.map_color_rgb = gx_default_w_b_map_color_rgb;
	} else {
	    ijsdev->procs.map_rgb_color = gx_default_gray_map_rgb_color;
	    ijsdev->procs.map_color_rgb = gx_default_gray_map_color_rgb;
	}
	ijsdev->procs.encode_color = gx_default_gray_fast_encode;
	ijsdev->procs.decode_color = gx_default_decode_color;
	dci.polarity = GX_CINFO_POLARITY_ADDITIVE;
	dci.gray_index = 0;
    } else if (!strcmp (ColorSpace, "DeviceRGB")) {
	components = 3;
	ijsdev->procs.map_rgb_color = gx_default_rgb_map_rgb_color;
	ijsdev->procs.map_color_rgb = gx_default_rgb_map_color_rgb;
	ijsdev->procs.encode_color = gx_default_rgb_map_rgb_color;
	ijsdev->procs.decode_color = gx_default_rgb_map_color_rgb;
	dci.polarity = GX_CINFO_POLARITY_ADDITIVE;
	dci.gray_index = GX_CINFO_COMP_NO_INDEX;
    } else if (!strcmp (ColorSpace, "DeviceCMYK")) {
	components = 4;
	ijsdev->procs.map_cmyk_color = cmyk_8bit_map_cmyk_color;
	ijsdev->procs.map_color_rgb = cmyk_8bit_map_color_rgb;
	ijsdev->procs.encode_color = cmyk_8bit_map_cmyk_color;
	ijsdev->procs.decode_color = gx_default_decode_color;
	dci.polarity = GX_CINFO_POLARITY_SUBTRACTIVE;
	dci.gray_index = 3;
    } else {
	return -1;
    }

    maxvalue = (1 << bpc) - 1;
    dci.max_components = components;
    dci.num_components = components;
    dci.depth = bpc * components;
    dci.max_gray = maxvalue;
    dci.max_color = components > 1 ? maxvalue : 0;
    dci.dither_grays = maxvalue+1;
    dci.dither_colors = components > 1 ? maxvalue+1 : 0;

    dci.separable_and_linear = GX_CINFO_SEP_LIN;
    dci.cm_name = ColorSpace;
    
    ijsdev->color_info = dci;

    set_linear_color_bits_mask_shift((gx_device *)ijsdev);

    return 0;
}
