/* Copyright (C) 1999 Aladdin Enterprises.  All rights reserved.
  
  This software is provided AS-IS with no warranty, either express or
  implied.
  
  This software is distributed under license and may not be copied,
  modified or distributed except as expressly authorized under the terms
  of the license contained in the file LICENSE in this distribution.
  
  For more information about licensing, please refer to
  http://www.ghostscript.com/licensing/. For information on
  commercial licensing, go to http://www.artifex.com/licensing/ or
  contact Artifex Software, Inc., 101 Lucas Valley Road #110,
  San Rafael, CA  94903, U.S.A., +1(415)492-9861.
*/

/* $Id: gdevimdi.c 8250 2007-09-25 13:31:24Z giles $ */
/* IMDI Device.
 *
 * This is an RGB contone device, that outputs the raster
 * converted to CMYK using ICCLib / IMDI
 */

/* TODO: this should be configurable */
#define LINK_ICC_NAME	"link.icc"

#include "errno_.h"
#include "string_.h"

#include "gserror.h"
#include "gdevprn.h"
#include "gxfrac.h"

#include "icc.h"
#include "imdi.h"

/*
 * Set up the device structures and function tables.
 */

#ifndef X_DPI
#  define X_DPI 72
#endif
#ifndef Y_DPI
#  define Y_DPI 72
#endif

typedef struct gx_device_imdi_s gx_device_imdi;

static dev_proc_open_device(imdi_open_device);
static dev_proc_close_device(imdi_close_device);
static dev_proc_print_page(imdi_print_page);

struct gx_device_imdi_s
{
    gx_device_common;
    gx_prn_device_common;

    icmFile *fp;
    icc *icco;
    icmLuBase *luo;
    imdi *mdo;
};

static const gx_device_procs imdi_procs =
{
    imdi_open_device, NULL, NULL, gdev_prn_output_page, imdi_close_device,
    gx_default_rgb_map_rgb_color, gx_default_rgb_map_color_rgb,
    NULL, NULL, NULL, NULL, NULL, NULL,
    gdev_prn_get_params, gdev_prn_put_params,
    NULL, NULL, NULL, NULL, gx_page_device_get_page_device
};

const gx_device_imdi gs_imdi_device =
{
    prn_device_body(gx_device_imdi, imdi_procs, "imdi",
	    DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	    X_DPI, Y_DPI,
	    0, 0, 0, 0,	/* Margins */
	    3, 24, 255, 255, 256, 256, imdi_print_page)
};

static double incurve(void *ctx, int ch, double val)
{
    return val;
}

static double outcurve(void *ctx, int ch, double val)
{
    return val;
}

static void mdtable(void *ctx, double *outvals, double *invals)
{
    icmLuBase *luo = ctx;
    luo->lookup(luo, outvals, invals);
}

/*
 * Open IMDI device.
 * Load ICC device link profile (to map sRGB to FOGRA CMYK).
 */

static int
imdi_open_device(gx_device *dev)
{
    gx_device_imdi *idev = (gx_device_imdi*)dev;
    int code;

    icColorSpaceSignature ins, outs;
    int inn, outn;
    icmLuAlgType alg;

    icmFile *fp;
    icc *icco;
    icmLuBase *luo;
    imdi *mdo;

    /* Open and read profile */

    fp = new_icmFileStd_name(LINK_ICC_NAME, "rb");
    if (!fp)
	return gs_throw1(-1, "could not open file '%s'", LINK_ICC_NAME);

    icco = new_icc();
    if (!icco)
	return gs_throw(-1, "could not create ICC object");

    code = icco->read(icco, fp, 0);
    if (code != 0)
	return gs_throw1(-1, "could not read ICC profile: %s", icco->err);

    /* Get conversion object */

    luo = icco->get_luobj(icco, icmFwd, icPerceptual, icmSigDefaultData, icmLuOrdNorm);
    if (!luo)
	return gs_throw1(-1, "could not create ICC conversion object: %s", icco->err);
    
    luo->spaces(luo, &ins, &inn, &outs, &outn, &alg, NULL, NULL, NULL);

#ifdef DEBUG
    dprintf3("%s -> %s [%s]\n",
	    icm2str(icmColorSpaceSignature, ins),
	    icm2str(icmColorSpaceSignature, outs),
	    icm2str(icmLuAlg, alg));
#endif

    if (inn != 3)
	return gs_throw1(-1, "profile must have 3 input channels. got %d.", inn);
    if (outn != 4)
	return gs_throw1(-1, "profile must have 4 output channels. got %d.", outn);

    /* Create IMDI optimized lookup object */

    mdo = new_imdi(inn, outn, pixint8, 0, pixint8, 0,
			 33, incurve, mdtable, outcurve, luo);
    if (!mdo)
	return gs_throw(-1, "new_imdi failed");

    idev->fp = fp;
    idev->icco = icco;
    idev->luo = luo;
    idev->mdo = mdo;

    return gdev_prn_open(dev);
}


/*
 * Close device and clean up ICC structures.
 */

static int
imdi_close_device(gx_device *dev)
{
    gx_device_imdi *idev = (gx_device_imdi*)dev;

    idev->mdo->done(idev->mdo);
    idev->luo->del(idev->luo);
    idev->icco->del(idev->icco);
    idev->fp->del(idev->fp);

    return gdev_prn_close(dev);
}


/*
 * Output the page raster.
 */

static int
imdi_print_page(gx_device_printer *pdev, FILE *prn_stream)
{
    gx_device_imdi *idev = (gx_device_imdi*)pdev;

    byte *srcbuffer = 0;
    byte *dstbuffer = 0;
    FILE *fp[4] = {0};

    int srcstride;
    int dststride;
    int srcplanes;
    int dstplanes;

    double srcpixel[GX_DEVICE_COLOR_MAX_COMPONENTS];
    double dstpixel[GX_DEVICE_COLOR_MAX_COMPONENTS];
    byte *srcdata;

    int code = 0;
    int x, y, k;

    int nsame = 0, lsame = 0;
    int ndiff = 0, ldiff = 0;

    /*
     * Open auxiliary CMYK files.
     */

    fprintf(prn_stream, "P6\n%d %d\n255\n", pdev->width, pdev->height);

    for (k = 0; k < 4; k++)
    {
	char name[256];

	sprintf(name, "%s.%c.pgm", pdev->fname, "cmyk"[k]);

	dprintf1("output file: %s\n", name);

	fp[k] = fopen(name, "wb");
	if (!fp[k])
	{
	    code = gs_throw2(-1, "could not open file: %s (%s)", name, strerror(errno));
	    goto cleanup;
	}

	fprintf(fp[k], "P5\n%d %d\n255\n", pdev->width, pdev->height);
    }

    /*
     * Allocate scratch buffers.
     */

    srcplanes = pdev->color_info.num_components;
    srcstride = gdev_mem_bytes_per_scan_line((gx_device*)pdev);
    srcbuffer = gs_malloc(pdev->memory, srcstride, 1, "imdi_print_page(srcbuffer)");
    if (!srcbuffer)
    {
	code = gs_throw1(-1, "outofmem: src buffer %d", srcstride);
	goto cleanup;
    }

    dstplanes = 4;
    dststride = dstplanes * pdev->width;
    dstbuffer = gs_malloc(pdev->memory, dststride, 1, "imdi_print_page(dstbuffer)");
    if (!dstbuffer)
    {
	code = gs_throw1(-1, "outofmem: dst buffer %d", dststride);
	goto cleanup;
    }

    /*
     * Extract pixels, convert colors and write data to files.
     */

    for (y = 0; y < pdev->height; y++)
    {
	gdev_prn_get_bits(pdev, y, srcbuffer, &srcdata);

	/* write rgb to original output file */
	fwrite(srcdata, 1, srcstride, prn_stream);

#if 1 /* Collect runlengths */

	{
	void *inp[1];
	void *outp[1];
	int sx, ex;
	int w = pdev->width;

	sx = ex = 0;

	while (sx < w)
	{
	    inp[0] = srcdata + sx * 3;
	    outp[0] = dstbuffer + sx * 4;

	    while (ex < w && 
		srcdata[ex * 3 + 0] == srcdata[sx * 3 + 0] &&
		srcdata[ex * 3 + 1] == srcdata[sx * 3 + 1] &&
		srcdata[ex * 3 + 2] == srcdata[sx * 3 + 2])
	    {
		ex ++;
	    }

	    /* same-run */
	    if (ex - sx > 1)
	    {
		nsame ++; lsame += ex - sx;

		idev->mdo->interp(idev->mdo, outp, inp, 1);
		for (x = sx + 1; x < ex; x++)
		{
		    dstbuffer[x * 4 + 0] = dstbuffer[sx * 4 + 0];
		    dstbuffer[x * 4 + 1] = dstbuffer[sx * 4 + 1];
		    dstbuffer[x * 4 + 2] = dstbuffer[sx * 4 + 2];
		    dstbuffer[x * 4 + 3] = dstbuffer[sx * 4 + 3];
		}
	    }

	    /* diff-run */
	    else
	    {
		ndiff ++;

		while (ex < w && 
			srcdata[ex * 3 + 0] != srcdata[ex * 3 - 3] &&
			srcdata[ex * 3 + 1] != srcdata[ex * 3 - 2] &&
			srcdata[ex * 3 + 2] != srcdata[ex * 3 - 1])
		{
		    ex ++;
		}

		ldiff += ex - sx;

		idev->mdo->interp(idev->mdo, outp, inp, ex - sx);
	    }

	    sx = ex;
	}
	}

#endif

#if 0 /* Call IMDI for entire scanline */
	void *inp[1];
	void *outp[1];

	inp[0] = srcdata;
	outp[0] = dstbuffer;

	idev->mdo->interp(idev->mdo, outp, inp, pdev->width);

#if 0
	/* output planar data to auxiliary output files */
	for (x = 0; x < pdev->width; x++)
	    for (k = 0; k < 4; k++)
		putc(dstbuffer[x * 4 + k], fp[k]);
#endif
#endif

#if 0 /* Call IMDI for every pixel */
	for (x = 0; x < pdev->width; x++)
	{
	    void *inp[1];
	    void *outp[1];

	    inp[0] = srcdata + x * 3;
	    outp[0] = dstbuffer + x * 4;

	    idev->mdo->interp(idev->mdo, outp, inp, 1);

	    /* output planar data to auxiliary output files */
	    for (k = 0; k < 4; k++)
		putc(dstbuffer[x * 4 + k], fp[k]);
	}
#endif

#if 0 /* Slow but accurate every pixel */

	for (x = 0; x < pdev->width; x++)
	{
	    srcpixel[0] = srcdata[x * 3 + 0] / 255.0;
	    srcpixel[1] = srcdata[x * 3 + 1] / 255.0;
	    srcpixel[2] = srcdata[x * 3 + 2] / 255.0;

	    code = idev->luo->lookup(idev->luo, dstpixel, srcpixel);
	    if (code > 1)
	    {
		code = gs_throw1(-1, "icc lookup failed: %s", idev->icco->err);
		goto cleanup;
	    }

	    dstbuffer[x * 4 + 0] = dstpixel[0] * 255 + 0.5;
	    dstbuffer[x * 4 + 1] = dstpixel[1] * 255 + 0.5;
	    dstbuffer[x * 4 + 2] = dstpixel[2] * 255 + 0.5;
	    dstbuffer[x * 4 + 3] = dstpixel[3] * 255 + 0.5;

	    /* output planar data to auxiliary output files */
	    for (k = 0; k < 4; k++)
		putc(dstbuffer[x * 4 + k], fp[k]);
	}
#endif
    }

    dprintf4("same=%d/%d diff=%d/%d\n", lsame, nsame, ldiff, ndiff);


    /*
     * Cleanup memory and files.
     */

cleanup:

    for (k = 0; k < 4; k++)
	if (fp[k])
	    fclose(fp[k]);

    if (dstbuffer)
	gs_free(pdev->memory, dstbuffer, dststride, 1, "imdi_print_page(dstbuffer)");

    if (srcbuffer)
	gs_free(pdev->memory, srcbuffer, srcstride, 1, "imdi_print_page(srcbuffer)");

    return code;
}

