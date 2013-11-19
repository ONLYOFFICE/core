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

/* $Id: gxshade.c 8810 2008-06-30 00:05:40Z alexcher $ */
/* Shading rendering support */
#include "math_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsrect.h"
#include "gxcspace.h"
#include "gscindex.h"
#include "gscie.h"		/* requires gscspace.h */
#include "gxdevcli.h"
#include "gxistate.h"
#include "gxdht.h"		/* for computing # of different colors */
#include "gxpaint.h"
#include "gxshade.h"
#include "gxshade4.h"
#include "gsicc.h"

/* Define a maximum smoothness value. */
/* smoothness > 0.2 produces severely blocky output. */
#define MAX_SMOOTHNESS 0.2

/* ================ Packed coordinate streams ================ */

/* Forward references */
static int cs_next_packed_value(shade_coord_stream_t *, int, uint *);
static int cs_next_array_value(shade_coord_stream_t *, int, uint *);
static int cs_next_packed_decoded(shade_coord_stream_t *, int,
				   const float[2], float *);
static int cs_next_array_decoded(shade_coord_stream_t *, int,
				  const float[2], float *);
static void cs_packed_align(shade_coord_stream_t *cs, int radix);
static void cs_array_align(shade_coord_stream_t *cs, int radix);
static bool cs_eod(const shade_coord_stream_t * cs);

/* Initialize a packed value stream. */
void
shade_next_init(shade_coord_stream_t * cs,
		const gs_shading_mesh_params_t * params,
		const gs_imager_state * pis)
{
    cs->params = params;
    cs->pctm = &pis->ctm;
    if (data_source_is_stream(params->DataSource)) {
	/*
	 * Rewind the data stream iff it is reusable -- either a reusable
	 * file or a reusable string.
	 */
	stream *s = cs->s = params->DataSource.data.strm;

	if ((s->file != 0 && s->file_limit != max_long) ||
	    (s->file == 0 && s->strm == 0)
	    )
	    sseek(s, 0);
    } else {
	s_init(&cs->ds, NULL);
        sread_string(&cs->ds, params->DataSource.data.str.data,
		     params->DataSource.data.str.size);
	cs->s = &cs->ds;
    }
    if (data_source_is_array(params->DataSource)) {
	cs->get_value = cs_next_array_value;
	cs->get_decoded = cs_next_array_decoded;
	cs->align = cs_array_align;
    } else {
	cs->get_value = cs_next_packed_value;
	cs->get_decoded = cs_next_packed_decoded;
	cs->align = cs_packed_align;
    }
    cs->is_eod = cs_eod;
    cs->left = 0;
    cs->ds_EOF = false;
}

/* Check for the End-Of-Data state form a stream. */
static bool
cs_eod(const shade_coord_stream_t * cs)
{
    return cs->ds_EOF;
}

/* Get the next (integer) value from a packed value stream. */
/* 1 <= num_bits <= sizeof(uint) * 8. */
static int
cs_next_packed_value(shade_coord_stream_t * cs, int num_bits, uint * pvalue)
{
    uint bits = cs->bits;
    int left = cs->left;

    if (left >= num_bits) {
	/* We can satisfy this request with the current buffered bits. */
	cs->left = left -= num_bits;
	*pvalue = (bits >> left) & ((1 << num_bits) - 1);
    } else {
	/* We need more bits. */
	int needed = num_bits - left;
	uint value = bits & ((1 << left) - 1);	/* all the remaining bits */

	for (; needed >= 8; needed -= 8) {
	    int b = sgetc(cs->s);

	    if (b < 0) {
	        cs->ds_EOF = true;
		return_error(gs_error_rangecheck);
	    }
	    value = (value << 8) + b;
	}
	if (needed == 0) {
	    cs->left = 0;
	    *pvalue = value;
	} else {
	    int b = sgetc(cs->s);

	    if (b < 0) {
	        cs->ds_EOF = true;
		return_error(gs_error_rangecheck);
	    }
	    cs->bits = b;
	    cs->left = left = 8 - needed;
	    *pvalue = (value << needed) + (b >> left);
	}
    }
    return 0;
}

/*
 * Get the next (integer) value from an unpacked array.  Note that
 * num_bits may be 0 if we are reading a coordinate or color value.
 */
static int
cs_next_array_value(shade_coord_stream_t * cs, int num_bits, uint * pvalue)
{
    float value;
    uint read;

    if (sgets(cs->s, (byte *)&value, sizeof(float), &read) < 0 ||
	read != sizeof(float)) {
	cs->ds_EOF = true;
	return_error(gs_error_rangecheck);
    }
    if (value < 0 || (num_bits != 0 && num_bits < sizeof(uint) * 8 &&
	 value >= (1 << num_bits)) ||
	value != (uint)value
	)
	return_error(gs_error_rangecheck);
    *pvalue = (uint) value;
    return 0;
}

/* Get the next decoded floating point value. */
static int
cs_next_packed_decoded(shade_coord_stream_t * cs, int num_bits,
		       const float decode[2], float *pvalue)
{
    uint value;
    int code = cs->get_value(cs, num_bits, &value);
    double max_value = (double)(uint)
	(num_bits == sizeof(uint) * 8 ? ~0 : ((1 << num_bits) - 1));

    if (code < 0)
	return code;
    *pvalue =
	(decode == 0 ? value / max_value :
	 decode[0] + value * (decode[1] - decode[0]) / max_value);
    return 0;
}

/* Get the next floating point value from an array, without decoding. */
static int
cs_next_array_decoded(shade_coord_stream_t * cs, int num_bits,
		      const float decode[2], float *pvalue)
{
    float value;
    uint read;

    if (sgets(cs->s, (byte *)&value, sizeof(float), &read) < 0 ||
	read != sizeof(float)
    ) {
	cs->ds_EOF = true;
	return_error(gs_error_rangecheck);
    }
    *pvalue = value;
    return 0;
}

static void
cs_packed_align(shade_coord_stream_t *cs, int radix)
{
    cs->left = cs->left / radix * radix;
}

static void
cs_array_align(shade_coord_stream_t *cs, int radix)
{
}

/* Get the next flag value. */
/* Note that this always starts a new data byte. */
int
shade_next_flag(shade_coord_stream_t * cs, int BitsPerFlag)
{
    uint flag;
    int code;

    cs->left = 0;		/* start a new byte if packed */
    code = cs->get_value(cs, BitsPerFlag, &flag);
    return (code < 0 ? code : flag);
}

/* Get one or more coordinate pairs. */
int
shade_next_coords(shade_coord_stream_t * cs, gs_fixed_point * ppt,
		  int num_points)
{
    int num_bits = cs->params->BitsPerCoordinate;
    const float *decode = cs->params->Decode;
    int code = 0;
    int i;

    for (i = 0; i < num_points; ++i) {
	float x, y;

	if ((code = cs->get_decoded(cs, num_bits, decode, &x)) < 0 ||
	    (code = cs->get_decoded(cs, num_bits, decode + 2, &y)) < 0 ||
	    (code = gs_point_transform2fixed(cs->pctm, x, y, &ppt[i])) < 0
	    )
	    break;
    }
    return code;
}

/* Get a color.  Currently all this does is look up Indexed colors. */
int
shade_next_color(shade_coord_stream_t * cs, float *pc)
{
    const float *decode = cs->params->Decode + 4;	/* skip coord decode */
    const gs_color_space *pcs = cs->params->ColorSpace;
    gs_color_space_index index = gs_color_space_get_index(pcs);
    int num_bits = cs->params->BitsPerComponent;

    if (index == gs_color_space_index_Indexed) {
	int ncomp = gs_color_space_num_components(gs_cspace_base_space(pcs));
	int ci;
        float cf;
	int code = cs->get_decoded(cs, num_bits, decode, &cf);
        gs_client_color cc;
	int i;

	if (code < 0)
	    return code;
	if (cf < 0)
	    return_error(gs_error_rangecheck);
	ci = (int)cf;
        if (ci >= gs_cspace_indexed_num_entries(pcs))
	    return_error(gs_error_rangecheck);
	code = gs_cspace_indexed_lookup(pcs, ci, &cc);
	if (code < 0)
	    return code;
	for (i = 0; i < ncomp; ++i)
	    pc[i] = cc.paint.values[i];
    } else {
	int i, code;
	int ncomp = (cs->params->Function != 0 ? 1 :
		     gs_color_space_num_components(pcs));

	for (i = 0; i < ncomp; ++i) {
	    if ((code = cs->get_decoded(cs, num_bits, decode + i * 2, &pc[i])) < 0)
		return code;
	    if (cs->params->Function) {
		gs_function_params_t *params = &cs->params->Function->params;

		if (pc[i] < params->Domain[i + i])
		    pc[i] = params->Domain[i + i];
		else if (pc[i] > params->Domain[i + i + 1])
		    pc[i] = params->Domain[i + i + 1];
	    }
	}
    }
    return 0;
}

/* Get the next vertex for a mesh element. */
int
shade_next_vertex(shade_coord_stream_t * cs, shading_vertex_t * vertex, patch_color_t *c)
{   /* Assuming p->c == c, provides a non-const access. */
    int code = shade_next_coords(cs, &vertex->p, 1);

    if (code >= 0)
	code = shade_next_color(cs, c->cc.paint.values);
    if (code >= 0)
	cs->align(cs, 8); /* CET 09-47J.PS SpecialTestI04Test01. */
    return code;
}

/* ================ Shading rendering ================ */

/* Initialize the common parts of the recursion state. */
void
shade_init_fill_state(shading_fill_state_t * pfs, const gs_shading_t * psh,
		      gx_device * dev, gs_imager_state * pis)
{
    const gs_color_space *pcs = psh->params.ColorSpace;
    float max_error = min(pis->smoothness, MAX_SMOOTHNESS);
    /*
     * There's no point in trying to achieve smoothness beyond what
     * the device can implement, i.e., the number of representable
     * colors times the number of halftone levels.
     */
    long num_colors =
	max(dev->color_info.max_gray, dev->color_info.max_color) + 1;
    const gs_range *ranges = 0;
    int ci;

    pfs->dev = dev;
    pfs->pis = pis;
top:
    pfs->direct_space = pcs;
    pfs->num_components = gs_color_space_num_components(pcs);
    switch ( gs_color_space_get_index(pcs) )
	{
	case gs_color_space_index_Indexed:
	    pcs = gs_cspace_base_space(pcs);
	    goto top;
	case gs_color_space_index_CIEDEFG:
	    ranges = pcs->params.defg->RangeDEFG.ranges;
	    break;
	case gs_color_space_index_CIEDEF:
	    ranges = pcs->params.def->RangeDEF.ranges;
	    break;
	case gs_color_space_index_CIEABC:
	    ranges = pcs->params.abc->RangeABC.ranges;
	    break;
	case gs_color_space_index_CIEA:
	    ranges = &pcs->params.a->RangeA;
	    break;
        case gs_color_space_index_CIEICC:
            ranges = pcs->params.icc.picc_info->Range.ranges;
	default:
	    break;
	}
    if (num_colors <= 32) {
	gx_ht_order_component *components = pis->dev_ht->components;
	if (components && components[0].corder.wts)
	    num_colors = 256;
	else
	    /****** WRONG FOR MULTI-PLANE HALFTONES ******/
	    num_colors *= pis->dev_ht->components[0].corder.num_levels;
    }
    if (psh->head.type == 2 || psh->head.type == 3) {
	max_error *= 0.25;
	num_colors *= 2;
    }
    if (max_error < 1.0 / num_colors)
	max_error = 1.0 / num_colors;
    for (ci = 0; ci < pfs->num_components; ++ci)
	pfs->cc_max_error[ci] =
	    (ranges == 0 ? max_error :
	     max_error * (ranges[ci].rmax - ranges[ci].rmin));
}

/* Fill one piece of a shading. */
int
shade_fill_path(const shading_fill_state_t * pfs, gx_path * ppath,
		gx_device_color * pdevc, const gs_fixed_point *fill_adjust)
{
    gx_fill_params params;

    params.rule = -1;		/* irrelevant */
    params.adjust = *fill_adjust;
    params.flatness = 0;	/* irrelevant */
    params.fill_zero_width = false;
    return (*dev_proc(pfs->dev, fill_path)) (pfs->dev, pfs->pis, ppath,
					     &params, pdevc, NULL);
}
