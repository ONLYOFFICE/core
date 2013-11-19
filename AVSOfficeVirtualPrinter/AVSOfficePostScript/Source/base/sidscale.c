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

/* $Id: sidscale.c 8784 2008-05-28 18:16:26Z giles $ */
/* Special Image downsample scaling filters for dithered devices */
#include "math_.h"
#include "memory_.h"
#include "stdio_.h"
#include "gdebug.h"
#include "gxfixed.h"		/* for gxdda.h */
#include "gxdda.h"
#include "gxfrac.h"
#include "strimpl.h"
#include "sidscale.h"

/* Temporary intermediate values */
typedef byte PixelTmp;

#define minPixelTmp 0
#define maxPixelTmp 255
#define unitPixelTmp 255

/* Max of all pixel sizes */
#define maxSizeofPixel 2

/* Auxiliary structures. */

/* ImageSpecialDownScaleEncode / ImageSpecialDownScaleDecode */
typedef struct stream_ISpecialDownScale_state_s {
    /* The client sets the params values before initialization. */
    stream_image_scale_state_common;  /* = state_common + params */
    /* The init procedure sets the following. */
    int sizeofPixelIn;		/* bytes per input value, 1 or 2 */
    int sizeofPixelOut;		/* bytes per output value, 1 or 2 */
    void /*PixelIn */  *src;
    void /*PixelOut */ *dst;
    void /*PixelIn */  *tmp;
    gx_dda_int_t dda_x_init;	/* initial setting of dda_x */
    /* The following are updated dynamically. */
    int dst_x;
    uint dst_offset, dst_size;
    gx_dda_int_t dda_x;		/* DDA for dest X in current scan line */
    int src_y;
    uint src_offset, src_size;
    int dst_y;
    gx_dda_int_t dda_y;		/* DDA for dest Y */
} stream_ISpecialDownScale_state;

gs_private_st_ptrs3(st_ISpecialDownScale_state, stream_ISpecialDownScale_state,
    "ImageSpecialDownScaleEncode/Decode state",
    isdscale_state_enum_ptrs, isdscale_state_reloc_ptrs,
    dst, src, tmp);

/* Apply filter to downscale horizontally from src to tmp. */
static void
idownscale_x(void /* PixelIn */ * tmp, const void /* PixelIn */ *src, stream_ISpecialDownScale_state *const ss)
{
    int c, i;
    int Colors = ss->params.Colors;
    int WidthIn = ss->params.WidthIn;
    int prev_y = dda_previous(ss->dda_y);
    int cur_y = dda_next(ss->dda_y);
    bool firstline = prev_y != cur_y;	/* at the start of a new group of lines */
    bool polarity_additive = ss->params.ColorPolarityAdditive;

	/* The following could be a macro, BUT macro's with control */
	/* are not good style and hard to debug */
    if (ss->sizeofPixelIn == 1) {
	for (c = 0; c < Colors; ++c) {
	    byte *tp = (byte *)tmp + c;		/* destination */
	    const byte *pp =  (const byte *)src + c;

	    ss->dda_x = ss->dda_x_init;
	    if_debug1('W', "[W]idownscale_x color %d:", c);

	    for ( i = 0; i < WidthIn; tp += Colors) {
		int endx = dda_next(ss->dda_x);
		if (firstline)
		    *tp = *pp;
		else {
		    if ((polarity_additive && (*pp < *tp)) ||
			(!polarity_additive && (*pp > *tp)) )
			*tp = *pp;
		}
		i++; pp += Colors;
		while (i < endx) {
		   if (*pp < *tp)
			*tp = *pp;
		   i++; pp += Colors;
		}
		if_debug1('W', " %d", *tp);
	    }
	    if_debug0('W', "\n");
	}
    } else {		/* sizeofPixelIn == 2 */
	for (c = 0; c < Colors; ++c) {
	    bits16 *tp = (bits16 *)tmp + c;		/* destination */
	    const bits16 *pp =  (const bits16 *)src + c;

	    ss->dda_x = ss->dda_x_init;
	    if_debug1('W', "[W]idownscale_x color %d:", c);

	    for ( i = 0; i < WidthIn; tp += Colors) {
		int endx = dda_next(ss->dda_x);
		if (firstline)
		    *tp = *pp;
		else {
		    if ((polarity_additive && (*pp < *tp)) ||
			(!polarity_additive && (*pp > *tp)) )
			*tp = *pp;
		}
		i++; pp += Colors;
		while (i < endx) {
		   if (*pp < *tp)
			*tp = *pp;
		   i++; pp += Colors;
		}
		if_debug1('W', " %d", *tp);
	    }
	    if_debug0('W', "\n");
	}
    }
}


/*
 * Copy from tmp to dst, taking into account PixelOut vs. PixelIn sizes
 * We do the conversion from PixelIn to PixelOut here (if needed)
 * since if the Y is scaled down we will convert less often.
 * This is simpler because we can treat all columns identically
 * without regard to the number of samples per pixel.
 */
static void
idownscale_y(void /*PixelOut */ *dst, const void /* PixelIn */ *tmp,
	     stream_ISpecialDownScale_state *const ss)
{
    int kn = ss->params.WidthOut * ss->params.Colors;
    int kc;

    if_debug0('W', "[W]idownscale_y: ");

    if (ss->sizeofPixelOut == 1) {
	if (ss->sizeofPixelIn == 1) {
	    const byte *pp = (byte *)tmp;

	    for ( kc = 0; kc < kn; ++kc, pp++ ) {
		if_debug1('W', " %d", *pp);
		((byte *)dst)[kc] = *pp;
	    }
	} else {	/* sizeofPixelIn == 2 */
	    const bits16 *pp = (bits16 *)tmp;

	    for ( kc = 0; kc < kn; ++kc, pp++ ) {
		if_debug1('W', " %d", *pp);
		((byte *)dst)[kc] = frac2byte(*pp);
	    }
	}
    } else {		/* sizeofPixelOut == 2 */
	if (ss->sizeofPixelIn == 1) {
	    const byte *pp = (byte *)tmp;

	    for ( kc = 0; kc < kn; ++kc, pp++ ) {
		if_debug1('W', " %d", *pp);
		((bits16 *)dst)[kc] = byte2frac(*pp);
	    }
	} else {	/* sizeofPixelIn == 2 */
	    const bits16 *pp = (bits16 *)tmp;

	    for ( kc = 0; kc < kn; ++kc, pp++ ) {
		if_debug1('W', " %d", *pp);
		((bits16 *)dst)[kc] = *pp;
	    }
	}
    }
    if_debug0('W', "n");
}

/* ------ Stream implementation ------ */


/* Forward references */
static void s_ISpecialDownScale_release(stream_state * st);

/* Set default parameter values (actually, just clear pointers). */
static void
s_ISpecialDownScale_set_defaults(stream_state * st)
{
    stream_ISpecialDownScale_state *const ss = (stream_ISpecialDownScale_state *) st;

    ss->src = 0;
    ss->dst = 0;
    ss->tmp = 0;
}

/* Initialize the filter. */
static int
s_ISpecialDownScale_init(stream_state * st)
{
    stream_ISpecialDownScale_state *const ss = (stream_ISpecialDownScale_state *) st;
    gs_memory_t *mem = ss->memory;

    ss->sizeofPixelIn = ss->params.BitsPerComponentIn / 8;
    ss->sizeofPixelOut = ss->params.BitsPerComponentOut / 8;

    ss->src_size = ss->params.WidthIn * ss->sizeofPixelIn * ss->params.Colors;
    ss->dst_size = ss->params.WidthOut * ss->sizeofPixelOut * ss->params.Colors;

    /* Initialize destination DDAs. */
    ss->dst_x = 0;
    ss->src_offset = ss->dst_offset = 0;
    dda_init(ss->dda_x, 0, ss->params.WidthIn, ss->params.WidthOut);
    ss->dda_x_init = ss->dda_x;
    ss->src_y = ss->dst_y = 0;
    dda_init(ss->dda_y, 0, ss->params.HeightOut, ss->params.HeightIn);


    /* create intermediate image to hold horizontal zoom */
    ss->tmp = gs_alloc_byte_array(mem, ss->params.WidthOut * ss->params.Colors,
				  ss->sizeofPixelIn, "image_scale tmp");
    /* Allocate buffers for 1 row of source and destination. */
    ss->dst = gs_alloc_byte_array(mem, ss->params.WidthOut * ss->params.Colors,
				  ss->sizeofPixelOut, "image_scale dst");
    ss->src = gs_alloc_byte_array(mem, ss->params.WidthIn * ss->params.Colors,
				  ss->sizeofPixelIn, "image_scale src");
    if (ss->tmp == 0 || ss->dst == 0 || ss->src == 0) {
	s_ISpecialDownScale_release(st);
	return ERRC;
/****** WRONG ******/
    }

    return 0;

}

/* Process a buffer.  Note that this handles Encode and Decode identically. */
static int
s_ISpecialDownScale_process(stream_state * st, stream_cursor_read * pr,
		 stream_cursor_write * pw, bool last)
{
    stream_ISpecialDownScale_state *const ss = (stream_ISpecialDownScale_state *) st;
    uint cur_y = dda_current(ss->dda_y);

    /* Check whether we need to deliver any output. */

top:
    if (cur_y > ss->dst_y) {
	/* Deliver some or all of the current scaled row. */
	/* to generate a vertically scaled output row. */
	uint wleft = pw->limit - pw->ptr;

	if (ss->dst_y == ss->params.HeightOut)
	    return EOFC;
	if (wleft == 0)
	    return 1;
	if (ss->dst_offset == 0) {
	    byte *row;

	    if (wleft >= ss->dst_size) {	/* We can scale the row directly into the output. */
		row = pw->ptr + 1;
		pw->ptr += ss->dst_size;
	    } else {		/* We'll have to buffer the row. */
		row = ss->dst;
	    }
	    /* Apply filter to zoom vertically from tmp to dst. */
	    idownscale_y(row, ss->tmp, ss);
	    /* Idiotic C coercion rules allow T* and void* to be */
	    /* inter-assigned freely, but not compared! */
	    if ((void *)row != ss->dst)		/* no buffering */
		goto adv;
	} {			/* We're delivering a buffered output row. */
	    uint wcount = ss->dst_size - ss->dst_offset;
	    uint ncopy = min(wleft, wcount);

	    memcpy(pw->ptr + 1, (byte *) ss->dst + ss->dst_offset, ncopy);
	    pw->ptr += ncopy;
	    ss->dst_offset += ncopy;
	    if (ncopy != wcount)
		return 1;
	    ss->dst_offset = 0;
	}
	/* Advance to the next output row. */
adv:	++(ss->dst_y);
    }

    /* Read input data and scale horizontally into tmp. */

    {
	uint rleft = pr->limit - pr->ptr;
	uint rcount = ss->src_size - ss->src_offset;

	if (rleft == 0)
	    return 0;		/* need more input */
	if (ss->src_y >= ss->params.HeightIn)
	    return ERRC;
	if (rleft >= rcount) {	/* We're going to fill up a row. */
	    const byte *row;

	    if (ss->src_offset == 0) {	/* We have a complete row.  Read the data */
		/* directly from the input. */
		row = pr->ptr + 1;
	    } else {		/* We're buffering a row in src. */
		row = ss->src;
		memcpy((byte *) ss->src + ss->src_offset, pr->ptr + 1,
		       rcount);
		ss->src_offset = 0;
	    }
	    /* Apply filter to zoom horizontally from src to tmp. */
	    if_debug2('w', "[w]idownscale_x y = %d to tmp row %d\n",
		      ss->src_y, (ss->src_y % MAX_ISCALE_SUPPORT));
	    idownscale_x(ss->tmp, row, ss);
	    pr->ptr += rcount;
	    ++(ss->src_y);
	    cur_y = dda_next(ss->dda_y);
	    goto top;
	} else {		/* We don't have a complete row.  Copy data to src buffer. */
	    memcpy((byte *) ss->src + ss->src_offset, pr->ptr + 1, rleft);
	    ss->src_offset += rleft;
	    pr->ptr += rleft;
	    return 0;
	}
    }
}

/* Release the filter's storage. */
static void
s_ISpecialDownScale_release(stream_state * st)
{
    stream_ISpecialDownScale_state *const ss = (stream_ISpecialDownScale_state *) st;
    gs_memory_t *mem = ss->memory;

    gs_free_object(mem, (void *)ss->src, "image_scale src");	/* no longer const */
    ss->src = 0;
    gs_free_object(mem, ss->dst, "image_scale dst");
    ss->dst = 0;
    gs_free_object(mem, ss->tmp, "image_scale tmp");
    ss->tmp = 0;
}

/* Stream template */
const stream_template s_ISpecialDownScale_template = {
    &st_ISpecialDownScale_state, s_ISpecialDownScale_init, s_ISpecialDownScale_process, 1, 1,
    s_ISpecialDownScale_release, s_ISpecialDownScale_set_defaults
};
