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

/* $Id: gsptype1.c 8952 2008-08-07 19:20:04Z leonardo $ */
/* PatternType 1 pattern implementation */
#include "math_.h"
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsrop.h"
#include "gsstruct.h"
#include "gsutil.h"		/* for gs_next_ids */
#include "gxarith.h"
#include "gxfixed.h"
#include "gxmatrix.h"
#include "gxcoord.h"		/* for gs_concat, gx_tr'_to_fixed */
#include "gxcspace.h"		/* for gscolor2.h */
#include "gxcolor2.h"
#include "gxdcolor.h"
#include "gxdevice.h"
#include "gxdevmem.h"
#include "gxclip2.h"
#include "gspath.h"
#include "gxpath.h"
#include "gxpcolor.h"
#include "gxp1impl.h"		/* requires gxpcolor.h */
#include "gxclist.h"
#include "gzstate.h"
#include "gsimage.h"
#include "gsiparm4.h"
#include "gsovrc.h"

/* Temporary switches for experimanting with Adobe compatibility. */
#define ADJUST_SCALE_FOR_THIN_LINES 0	/* Old code = 0 */
#define ADJUST_SCALE_BY_GS_TRADITION 0	/* Old code = 1 */
#define ADJUST_AS_ADOBE 1		/* Old code = 0 *//* This one is closer to Adobe. */

/* GC descriptors */
private_st_pattern1_template();
private_st_pattern1_instance();

/* GC procedures */
static ENUM_PTRS_BEGIN(pattern1_instance_enum_ptrs) {
    if (index < st_pattern1_template_max_ptrs) {
	gs_ptr_type_t ptype =
	    ENUM_SUPER_ELT(gs_pattern1_instance_t, st_pattern1_template,
			   template, 0);

	if (ptype)
	    return ptype;
	return ENUM_OBJ(NULL);	/* don't stop early */
    }
    ENUM_PREFIX(st_pattern_instance, st_pattern1_template_max_ptrs);
} ENUM_PTRS_END
static RELOC_PTRS_BEGIN(pattern1_instance_reloc_ptrs) {
    RELOC_PREFIX(st_pattern_instance);
    RELOC_SUPER(gs_pattern1_instance_t, st_pattern1_template, template);
} RELOC_PTRS_END

/* Define a PatternType 1 pattern. */
static pattern_proc_uses_base_space(gs_pattern1_uses_base_space);
static pattern_proc_make_pattern(gs_pattern1_make_pattern);
static pattern_proc_get_pattern(gs_pattern1_get_pattern);
static pattern_proc_set_color(gs_pattern1_set_color);
static const gs_pattern_type_t gs_pattern1_type = {
    1, {
	gs_pattern1_uses_base_space, gs_pattern1_make_pattern,
	gs_pattern1_get_pattern, gs_pattern1_remap_color,
	gs_pattern1_set_color
    }
};

/*
 * Build a PatternType 1 Pattern color space.
 */
int
gs_cspace_build_Pattern1(gs_color_space ** ppcspace,
		    gs_color_space * pbase_cspace, gs_memory_t * pmem)
{
    gs_color_space *pcspace = 0;

    if (pbase_cspace != 0) {
	if (gs_color_space_num_components(pcspace) < 0)		/* Pattern space */
	    return_error(gs_error_rangecheck);
    }
    pcspace = gs_cspace_alloc(pmem, &gs_color_space_type_Pattern);
    if (pcspace == NULL)
	return_error(gs_error_VMerror);
    if (pbase_cspace != 0) {
	pcspace->params.pattern.has_base_space = true;
	/* reference to base space shifts from pgs to pcs with no net change */
	pcspace->base_space = pbase_cspace;
    } else
	pcspace->params.pattern.has_base_space = false;
    *ppcspace = pcspace;
    return 0;
}

/* Initialize a PatternType 1 pattern template. */
void
gs_pattern1_init(gs_pattern1_template_t * ppat)
{
    gs_pattern_common_init((gs_pattern_template_t *)ppat, &gs_pattern1_type);
}

/* Make an instance of a PatternType 1 pattern. */
static int compute_inst_matrix(gs_pattern1_instance_t * pinst,
	gs_state * saved, gs_rect * pbbox, int width, int height);
int
gs_makepattern(gs_client_color * pcc, const gs_pattern1_template_t * pcp,
	       const gs_matrix * pmat, gs_state * pgs, gs_memory_t * mem)
{
    return gs_pattern1_make_pattern(pcc, (const gs_pattern_template_t *)pcp,
				    pmat, pgs, mem);
}
static int
gs_pattern1_make_pattern(gs_client_color * pcc,
			 const gs_pattern_template_t * ptemp,
			 const gs_matrix * pmat, gs_state * pgs,
			 gs_memory_t * mem)
{
    const gs_pattern1_template_t *pcp = (const gs_pattern1_template_t *)ptemp;
    gs_pattern1_instance_t inst;
    gs_pattern1_instance_t *pinst;
    gs_state *saved;
    gs_rect bbox;
    gs_fixed_rect cbox;
    gx_device * pdev = pgs->device;
    int dev_width = pdev->width;
    int dev_height = pdev->height;
    int code = gs_make_pattern_common(pcc, (const gs_pattern_template_t *)pcp,
				      pmat, pgs, mem,
				      &st_pattern1_instance);

    if (code < 0)
	return code;
    if (mem == 0)
	mem = gs_state_memory(pgs);
    pinst = (gs_pattern1_instance_t *)pcc->pattern;
    *(gs_pattern_instance_t *)&inst = *(gs_pattern_instance_t *)pinst;
    saved = inst.saved;
    switch (pcp->PaintType) {
	case 1:		/* colored */
	    gs_set_logical_op(saved, lop_default);
	    break;
	case 2:		/* uncolored */
	    gx_set_device_color_1(saved);
	    break;
	default:
	    code = gs_note_error(gs_error_rangecheck);
	    goto fsaved;
    }
    inst.template = *pcp;
    code = compute_inst_matrix(&inst, saved, &bbox, dev_width, dev_height);
    if (code < 0)
	goto fsaved;

#define mat inst.step_matrix
    if_debug6('t', "[t]step_matrix=[%g %g %g %g %g %g]\n",
	      inst.step_matrix.xx, inst.step_matrix.xy, inst.step_matrix.yx, 
	      inst.step_matrix.yy, inst.step_matrix.tx, inst.step_matrix.ty);
    if_debug4('t', "[t]bbox=(%g,%g),(%g,%g)\n",
	      bbox.p.x, bbox.p.y, bbox.q.x, bbox.q.y);
    {
	float bbw = bbox.q.x - bbox.p.x;
	float bbh = bbox.q.y - bbox.p.y;

	/* If the step and the size agree to within 1/2 pixel, */
	/* make them the same. */
	if (ADJUST_SCALE_BY_GS_TRADITION) {
	    inst.size.x = (int)(bbw + 0.8);		/* 0.8 is arbitrary */
	    inst.size.y = (int)(bbh + 0.8);
	} else {
	    inst.size.x = (int)ceil(bbw);
	    inst.size.y = (int)ceil(bbh);
	}

	if (inst.size.x == 0 || inst.size.y == 0) {
	    /*
	     * The pattern is empty: the stepping matrix doesn't matter.
	     */
	    gs_make_identity(&inst.step_matrix);
	    bbox.p.x = bbox.p.y = bbox.q.x = bbox.q.y = 0;
	} else {
	    /* Check for singular stepping matrix. */
	    if (fabs(inst.step_matrix.xx * inst.step_matrix.yy - inst.step_matrix.xy * inst.step_matrix.yx) < 1.0e-6) {
		code = gs_note_error(gs_error_rangecheck);
		goto fsaved;
	    }
	    if (ADJUST_SCALE_BY_GS_TRADITION &&
	        inst.step_matrix.xy == 0 && inst.step_matrix.yx == 0 &&
		fabs(fabs(inst.step_matrix.xx) - bbw) < 0.5 &&
		fabs(fabs(inst.step_matrix.yy) - bbh) < 0.5
		) {
		gs_scale(saved, fabs(inst.size.x / inst.step_matrix.xx),
			 fabs(inst.size.y / inst.step_matrix.yy));
		code = compute_inst_matrix(&inst, saved, &bbox,
						dev_width, dev_height);
		if (code < 0)
		    goto fsaved;
		if (ADJUST_SCALE_FOR_THIN_LINES) {
		    /* To allow thin lines at a cell boundary 
		       to be painted inside the cell,
		       we adjust the scale so that 
		       the scaled width is in fixed_1 smaller */
		    gs_scale(saved, (fabs(inst.size.x) - 1.0 / fixed_scale) / fabs(inst.size.x),
				    (fabs(inst.size.y) - 1.0 / fixed_scale) / fabs(inst.size.y));
		}
		if_debug2('t',
			  "[t]adjusted XStep & YStep to size=(%d,%d)\n",
			  inst.size.x, inst.size.y);
		if_debug4('t', "[t]bbox=(%g,%g),(%g,%g)\n",
			  bbox.p.x, bbox.p.y, bbox.q.x, bbox.q.y);
	    } else if (ADJUST_AS_ADOBE) {
		if (inst.step_matrix.xy == 0 && inst.step_matrix.yx == 0 &&
		    fabs(fabs(inst.step_matrix.xx) - bbw) < 0.5 &&
		    fabs(fabs(inst.step_matrix.yy) - bbh) < 0.5
		    ) {
		    if (inst.step_matrix.xx <= 2) { 
			/* Prevent a degradation - see -r72 mspro.pdf */
			gs_scale(saved, fabs(inst.size.x / inst.step_matrix.xx), 1);
			inst.step_matrix.xx = (float)inst.size.x;
		    } else {
			inst.step_matrix.xx = (float)floor(inst.step_matrix.xx + 0.5);
			/* To allow thin lines at a cell boundary 
			   to be painted inside the cell,
			   we adjust the scale so that 
			   the scaled width is in fixed_1 smaller */
			if (bbw >= inst.size.x - 1.0 / fixed_scale)
			    gs_scale(saved, (fabs(inst.size.x) - 1.0 / fixed_scale) / fabs(inst.size.x), 1);
		    }
		    if (inst.step_matrix.yy <= 2) {
			gs_scale(saved, 1, fabs(inst.size.y / inst.step_matrix.yy));
			inst.step_matrix.yy = (float)inst.size.y;
		    } else {
			inst.step_matrix.yy = (float)floor(inst.step_matrix.yy + 0.5);
			if (bbh >= inst.size.y - 1.0 / fixed_scale)
			    gs_scale(saved, 1, (fabs(inst.size.y) - 1.0 / fixed_scale) / fabs(inst.size.y));
		    }
		    code = gs_bbox_transform(&inst.template.BBox, &ctm_only(saved), &bbox);
		    if (code < 0)
			goto fsaved;
		}
	    }
	}
    }
    if ((code = gs_bbox_transform_inverse(&bbox, &inst.step_matrix, &inst.bbox)) < 0)
	goto fsaved;
    if_debug4('t', "[t]ibbox=(%g,%g),(%g,%g)\n",
	      inst.bbox.p.x, inst.bbox.p.y, inst.bbox.q.x, inst.bbox.q.y);
    inst.is_simple = (fabs(inst.step_matrix.xx) == inst.size.x && inst.step_matrix.xy == 0 &&
		      inst.step_matrix.yx == 0 && fabs(inst.step_matrix.yy) == inst.size.y);
    if_debug6('t',
	      "[t]is_simple? xstep=(%g,%g) ystep=(%g,%g) size=(%d,%d)\n",
	      inst.step_matrix.xx, inst.step_matrix.xy,
	      inst.step_matrix.yx, inst.step_matrix.yy,
	      inst.size.x, inst.size.y);
    /* Absent other information, instances always require a mask. */
    inst.uses_mask = true;
    gx_translate_to_fixed(saved, float2fixed_rounded(inst.step_matrix.tx - bbox.p.x),
			         float2fixed_rounded(inst.step_matrix.ty - bbox.p.y));
    inst.step_matrix.tx = bbox.p.x;
    inst.step_matrix.ty = bbox.p.y;
#undef mat
    cbox.p.x = fixed_0;
    cbox.p.y = fixed_0;
    cbox.q.x = int2fixed(inst.size.x);
    cbox.q.y = int2fixed(inst.size.y);
    code = gx_clip_to_rectangle(saved, &cbox);
    if (code < 0)
	goto fsaved;
    if (!inst.is_simple) {
	code = gs_newpath(saved);
	if (code >= 0)
	    code = gs_moveto(saved, inst.template.BBox.p.x, inst.template.BBox.p.y);
	if (code >= 0)
	    code = gs_lineto(saved, inst.template.BBox.q.x, inst.template.BBox.p.y);
	if (code >= 0)
	    code = gs_lineto(saved, inst.template.BBox.q.x, inst.template.BBox.q.y);
	if (code >= 0)
	    code = gs_lineto(saved, inst.template.BBox.p.x, inst.template.BBox.q.y);
	if (code >= 0)
	    code = gs_clip(saved);
	if (code < 0)
	    goto fsaved;
    }
    code = gs_newpath(saved);
    if (code < 0)
	goto fsaved;
    inst.id = gs_next_ids(mem, 1);
    *pinst = inst;
    return 0;
  fsaved:gs_state_free(saved);
    gs_free_object(mem, pinst, "gs_makepattern");
    return code;
}

/*
 * Clamp the bound box for a pattern to the region of the pattern that will
 * actually be on our page.  We need to do this becuase some applications
 * create patterns which specify a bounding box which is much larger than
 * the page.  We allocate a buffer for holding the pattern.  We need to
 * prevent this buffer from getting too large.
 */
static int
clamp_pattern_bbox(gs_pattern1_instance_t * pinst, gs_rect * pbbox,
		    int width, int height, const gs_matrix * pmat)
{
    double xstep = pinst->template.XStep;
    double ystep = pinst->template.YStep;
    double xmin = pbbox->q.x;
    double xmax = pbbox->p.x;
    double ymin = pbbox->q.y;
    double ymax = pbbox->p.y;
    int ixpat, iypat, iystart;
    double xpat, ypat;
    double xlower, xupper, ylower, yupper;
    double xdev, ydev;
    gs_rect dev_page, pat_page;
    gs_point dev_pat_origin, dev_step;
    int code;

    /*
     * Scan across the page.  We determine the region to be scanned
     * by working in the pattern coordinate space.  This is logically
     * simpler since XStep and YStep are on axis in the pattern space.
     */
    /*
     * Convert the page dimensions from device coordinates into the
     * pattern coordinate frame.
     */
    dev_page.p.x = dev_page.p.y = 0;
    dev_page.q.x = width;
    dev_page.q.y = height;
    code = gs_bbox_transform_inverse(&dev_page, pmat, &pat_page);
    if (code < 0)
	return code;
    /*
     * Determine the location of the pattern origin in device coordinates.
     */
    gs_point_transform(0.0, 0.0, pmat, &dev_pat_origin);
    /*
     * Determine our starting point.  We start with a postion that puts the
     * pattern below and to the left of the page (in pattern space) and scan
     * until the pattern is above and right of the page.
     */
    ixpat = (int) floor((pat_page.p.x - pinst->template.BBox.q.x) / xstep);
    iystart = (int) floor((pat_page.p.y - pinst->template.BBox.q.y) / ystep);

    /* Now do the scan */
    for (; ; ixpat++) {
        xpat = ixpat * xstep;
	for (iypat = iystart; ; iypat++) {
            ypat = iypat * ystep;
            /*
	     * Calculate the shift in the pattern's location.
	     */
	    gs_point_transform(xpat, ypat, pmat, &dev_step);
	    xdev = dev_step.x - dev_pat_origin.x;
	    ydev = dev_step.y - dev_pat_origin.y;
	    /*
	     * Check if the pattern bounding box intersects the page.
	     */
	    xlower = (xdev + pbbox->p.x > 0) ? pbbox->p.x : -xdev;
	    xupper = (xdev + pbbox->q.x < width) ? pbbox->q.x : -xdev + width;
	    ylower = (ydev + pbbox->p.y > 0) ? pbbox->p.y : -ydev;
	    yupper = (ydev + pbbox->q.y < height) ? pbbox->q.y : -ydev + height;
	    if (xlower < xupper && ylower < yupper) {
		/*
		 * The pattern intersects the page.  Expand required area if
		 * needed.
		 */
		if (xlower < xmin)
		    xmin = xlower;
		if (xupper > xmax)
		    xmax = xupper;
		if (ylower < ymin)
		    ymin = ylower;
		if (yupper > ymax)
		    ymax = yupper;
	    }
	    if (ypat > pat_page.q.y - pinst->template.BBox.p.y)
	        break;
	}
	if (xpat > pat_page.q.x - pinst->template.BBox.p.x)
	    break;
    }
    /* Update the bounding box. */
    if (xmin < xmax && ymin < ymax) {
	pbbox->p.x = xmin;
	pbbox->q.x = xmax;
	pbbox->p.y = ymin;
	pbbox->q.y = ymax;
    } else {
	/* The pattern is never on the page.  Set bbox = 1, 1 */
	pbbox->p.x = pbbox->p.y = 0;
	pbbox->q.x = pbbox->q.y = 1;
    }
    return 0;
}

/* Compute the stepping matrix and device space instance bounding box */
/* from the step values and the saved matrix. */
static int
compute_inst_matrix(gs_pattern1_instance_t * pinst, gs_state * saved,
			    gs_rect * pbbox, int width, int height)
{
    float xx, xy, yx, yy, dx, dy, temp;
    int code;

    code = gs_bbox_transform(&pinst->template.BBox, &ctm_only(saved), pbbox);
    if (code < 0)
	return code;
    /*
     * Adjust saved.ctm to map the bbox origin to pixels.
     */
    dx = pbbox->p.x - floor(pbbox->p.x + 0.5);
    dy = pbbox->p.y - floor(pbbox->p.y + 0.5);
    pbbox->p.x -= dx;
    pbbox->p.y -= dy;
    pbbox->q.x -= dx;
    pbbox->q.y -= dy;
    code = gx_translate_to_fixed(saved, float2fixed_rounded(saved->ctm.tx - dx), 
					float2fixed_rounded(saved->ctm.ty - dy));
    if (code < 0)
	return code;

    /* The stepping matrix : */
    xx = pinst->template.XStep * saved->ctm.xx;
    xy = pinst->template.XStep * saved->ctm.xy;
    yx = pinst->template.YStep * saved->ctm.yx;
    yy = pinst->template.YStep * saved->ctm.yy;

    /* Adjust the stepping matrix so all coefficients are >= 0. */
    if (xx == 0 || yy == 0) { /* We know that both xy and yx are non-zero. */
	temp = xx, xx = yx, yx = temp;
	temp = xy, xy = yy, yy = temp;
    }
    if (xx < 0)
	xx = -xx, xy = -xy;
    if (yy < 0)
	yx = -yx, yy = -yy;
    /* Now xx > 0, yy > 0. */
    pinst->step_matrix.xx = xx;
    pinst->step_matrix.xy = xy;
    pinst->step_matrix.yx = yx;
    pinst->step_matrix.yy = yy;
    pinst->step_matrix.tx = saved->ctm.tx;
    pinst->step_matrix.ty = saved->ctm.ty;
    /*
     * Some applications produce patterns that are larger than the page.
     * If the bounding box for the pattern is larger than the page. clamp
     * the pattern to the page size.
     */
    if ((pbbox->q.x - pbbox->p.x > width || pbbox->q.y - pbbox->p.y > height))
	code = clamp_pattern_bbox(pinst, pbbox, width,
					height, &ctm_only(saved));

    return code;
}

/* Test whether a PatternType 1 pattern uses a base space. */
static bool
gs_pattern1_uses_base_space(const gs_pattern_template_t *ptemp)
{
    return ((const gs_pattern1_template_t *)ptemp)->PaintType == 2;
}

/* getpattern for PatternType 1 */
/* This is only intended for the benefit of pattern PaintProcs. */
static const gs_pattern_template_t *
gs_pattern1_get_pattern(const gs_pattern_instance_t *pinst)
{
    return (const gs_pattern_template_t *)
	&((const gs_pattern1_instance_t *)pinst)->template;
}

/* Check device color for Pattern Type 1. */
bool
gx_dc_is_pattern1_color(const gx_device_color *pdevc)
{
    return pdevc->type == &gx_dc_pattern;
}

/* Check device color for Pattern Type 1. */
bool
gx_dc_is_pattern1_color_clist_based(const gx_device_color *pdevc)
{
    if (pdevc->type != &gx_dc_pattern)
	return false;
    return gx_pattern_tile_is_clist(pdevc->colors.pattern.p_tile);
}

/* Get pattern id (type 1 pattern only) */
gs_id
gs_dc_get_pattern_id(const gx_device_color *pdevc)
{
    if (pdevc->type != &gx_dc_pattern)
	return gs_no_id;
    if (pdevc->colors.pattern.p_tile == NULL)
	return gs_no_id;
    return pdevc->colors.pattern.p_tile->id;
}


/*
 * Perform actions required at setcolor time. This procedure resets the
 * overprint information (almost) as required by the pattern. The logic
 * behind this operation is a bit convoluted:
 *
 *  1. Both PatternType 1 and 2 "colors" occur within the pattern color
 *     space.
 *
 *  2. Nominally, the set of drawn components is a property of the color
 *     space, and is set at the time setcolorspace is called. This is
 *     not the case for patterns, so overprint information must be set
 *     at setcolor time for them.
 *
 *  3. PatternType 2 color spaces incorporate their own color space, so
 *     the set of drawn components is determined by that color space.
 *     For PatternType 1 color spaces, the PaintType determines the
 *     appropriate color space to use. If PaintType is 2 (uncolored),
 *     the pattern makes use of the base color space of the current
 *     pattern color space, so overprint is set as appropriate for
 *     that color space.
 *
 *  4. For PatternType 1 color spaces with PaintType 1 (colored), the
 *     appropriate color space to use is determined by the pattern's
 *     PaintProc. This cannot be handled by the current graphic
 *     library mechanism, because color space information is lost when
 *     the pattern tile is cached (and the pattern tile is essentially
 *     always cached). We punt in this case and list all components
 *     as drawn components. (This feature could be support by retaining
 *     per-component pattern masks, but complete re-design of the
 *     pattern mechanism is probably more appropriate.)
 *
 *  5. Once overprint information has been set for a particular color,
 *     it must be reset to the proper value when that color is no
 *     longer in use. "Normal" (non-pattern) colors do not have a
 *     "set_color" action, both for performance and logical reasons.
 *     This does not, however, cause significant difficulty, as the
 *     change in color space required to set a normal color will
 *     reset the overprint information as required.
 */
static int
gs_pattern1_set_color(const gs_client_color * pcc, gs_state * pgs)
{
    gs_pattern1_instance_t * pinst = (gs_pattern1_instance_t *)pcc->pattern;
    gs_pattern1_template_t * ptmplt = &pinst->template;

    if (ptmplt->PaintType == 2) {
        const gs_color_space *  pcs = pgs->color_space;

        pcs = pcs->base_space;
        return pcs->type->set_overprint(pcs, pgs);
    } else {
        gs_overprint_params_t   params;

        params.retain_any_comps = false;
        pgs->effective_overprint_mode = 0;
        return gs_state_update_overprint(pgs, &params);
    }
}


const gs_pattern1_template_t *
gs_getpattern(const gs_client_color * pcc)
{
    const gs_pattern_instance_t *pinst = pcc->pattern;

    return (pinst == 0 || pinst->type != &gs_pattern1_type ? 0 :
	    &((const gs_pattern1_instance_t *)pinst)->template);
}

/*
 *  Code for generating patterns from bitmaps and pixmaps.
 */

/*
 *  The following structures are realized here only because this is the
 *  first location in which they were needed. Otherwise, there is nothing
 *  about them that is specific to patterns.
 */
public_st_gs_bitmap();
public_st_gs_tile_bitmap();
public_st_gs_depth_bitmap();
public_st_gs_tile_depth_bitmap();
public_st_gx_strip_bitmap();

/*
 *  Structure for holding a gs_depth_bitmap and the corresponding depth and
 *  colorspace information.
 *
 *  The free_proc pointer is needed to hold the original value of the pattern
 *  instance free structure. This pointer in the pattern instance will be
 *  overwritten with free_pixmap_pattern, which will free the pixmap info
 *  structure when it is freed.
 */
typedef struct pixmap_info_s {
    gs_depth_bitmap bitmap;	/* must be first */
    gs_color_space *pcspace;
    uint white_index;
    void (*free_proc)(gs_memory_t *, void *, client_name_t);
} pixmap_info;

gs_private_st_suffix_add1(st_pixmap_info,
			  pixmap_info,
			  "pixmap info. struct",
			  pixmap_enum_ptr,
			  pixmap_reloc_ptr,
			  st_gs_depth_bitmap,
			  pcspace
);

#define st_pixmap_info_max_ptrs (1 + st_tile_bitmap_max_ptrs)

/*
 *  Free routine for pattern instances created from pixmaps. This overwrites
 *  the free procedure originally stored in the pattern instance, and stores
 *  the pointer to that procedure in the pixmap_info structure. This procedure
 *  will call the original procedure, then free the pixmap_info structure. 
 *
 *  Note that this routine does NOT release the data in the original pixmap;
 *  that remains the responsibility of the client.
 */
static void
free_pixmap_pattern(
    gs_memory_t *           pmem,
    void *                  pvpinst,
    client_name_t           cname
)
{
    gs_pattern1_instance_t *pinst = (gs_pattern1_instance_t *)pvpinst;
    pixmap_info *ppmap = pinst->template.client_data;

    ppmap->free_proc(pmem, pvpinst, cname);
    gs_free_object(pmem, ppmap, cname);
}

/*
 *  PaintProcs for bitmap and pixmap patterns.
 */
static int bitmap_paint(gs_image_enum * pen, gs_data_image_t * pim,
			 const gs_depth_bitmap * pbitmap, gs_state * pgs);
static int
mask_PaintProc(const gs_client_color * pcolor, gs_state * pgs)
{
    const pixmap_info *ppmap = gs_getpattern(pcolor)->client_data;
    const gs_depth_bitmap *pbitmap = &(ppmap->bitmap);
    gs_image_enum *pen =
    gs_image_enum_alloc(gs_state_memory(pgs), "mask_PaintProc");
    gs_image1_t mask;

    if (pen == 0)
	return_error(gs_error_VMerror);
    gs_image_t_init_mask(&mask, true);
    mask.Width = pbitmap->size.x;
    mask.Height = pbitmap->size.y;
    gs_image_init(pen, &mask, false, pgs);
    return bitmap_paint(pen, (gs_data_image_t *) & mask, pbitmap, pgs);
}
static int
image_PaintProc(const gs_client_color * pcolor, gs_state * pgs)
{
    const pixmap_info *ppmap = gs_getpattern(pcolor)->client_data;
    const gs_depth_bitmap *pbitmap = &(ppmap->bitmap);
    gs_image_enum *pen =
        gs_image_enum_alloc(gs_state_memory(pgs), "image_PaintProc");
    gs_color_space *pcspace;
    gx_image_enum_common_t *pie;
    /*
     * If the image is transparent then we want to do image type4 processing.
     * Otherwise we want to use image type 1 processing.
     */
    int transparent = ppmap->white_index < (1 << (pbitmap->num_comps * pbitmap->pix_depth));

    /*
     * Note: gs_image1_t and gs_image4_t sre nearly identical structure
     * definitions.  From our point of view, the only significant difference
     * is MaskColor in gs_image4_t.  The fields are generally loaded using
     * the gs_image1_t version of the union and then used for either type
     * of image processing.
     */
    union {
        gs_image1_t i1;
	gs_image4_t i4;
    } image;
    int code;

    if (pen == 0)
	return_error(gs_error_VMerror);

    if (ppmap->pcspace == 0) {
	pcspace = gs_cspace_new_DeviceGray(pgs->memory);
    } else
        pcspace = ppmap->pcspace;
    gs_gsave(pgs);
    gs_setcolorspace(pgs, pcspace);
    if (transparent)
        gs_image4_t_init( (gs_image4_t *) &image, pcspace);
    else
        gs_image_t_init_adjust( (gs_image_t *) &image, pcspace, 0);
    image.i1.Width = pbitmap->size.x;
    image.i1.Height = pbitmap->size.y;
    if (transparent) {
        image.i4.MaskColor_is_range = false;
        image.i4.MaskColor[0] = ppmap->white_index;
    }
    image.i1.Decode[0] = 0.0;
    image.i1.Decode[1] = (float)((1 << pbitmap->pix_depth) - 1);
    image.i1.BitsPerComponent = pbitmap->pix_depth;
    /* backwards compatibility */
    if (ppmap->pcspace == 0) {
	image.i1.Decode[0] = 1.0;
	image.i1.Decode[1] = 0.0;
    }

    if ( (code = gs_image_begin_typed( (const gs_image_common_t *)&image,
                                       pgs,
		                       false,
                                       &pie )) >= 0 &&
         (code = gs_image_enum_init( pen,
                                     pie,
                                     (gs_data_image_t *)&image,
                                     pgs )) >= 0      )
	code = bitmap_paint(pen, (gs_data_image_t *) & image, pbitmap, pgs);
    gs_grestore(pgs);
    return code;
}
/* Finish painting any kind of bitmap pattern. */
static int
bitmap_paint(gs_image_enum * pen, gs_data_image_t * pim,
	     const gs_depth_bitmap * pbitmap, gs_state * pgs)
{
    uint raster = pbitmap->raster;
    uint nbytes = (pim->Width * pbitmap->pix_depth + 7) >> 3;
    uint used;
    const byte *dp = pbitmap->data;
    int n;
    int code = 0, code1;

    if (nbytes == raster)
	code = gs_image_next(pen, dp, nbytes * pim->Height, &used);
    else
	for (n = pim->Height; n > 0 && code >= 0; dp += raster, --n)
	    code = gs_image_next(pen, dp, nbytes, &used);
    code1 = gs_image_cleanup_and_free_enum(pen, pgs);
    if (code >= 0 && code1 < 0)
	code = code1;
    return code;
}

/*
 * Make a pattern from a bitmap or pixmap. The pattern may be colored or
 * uncolored, as determined by the mask operand. This code is intended
 * primarily for use by PCL.
 *
 * See the comment prior to the declaration of this function in gscolor2.h
 * for further information.
 */
int
gs_makepixmappattern(
			gs_client_color * pcc,
			const gs_depth_bitmap * pbitmap,
			bool mask,
			const gs_matrix * pmat,
			long id,
			gs_color_space * pcspace,
			uint white_index,
			gs_state * pgs,
			gs_memory_t * mem
)
{

    gs_pattern1_template_t pat;
    pixmap_info *ppmap;
    gs_matrix mat, smat;
    int code;

    /* check that the data is legitimate */
    if ((mask) || (pcspace == 0)) {
	if (pbitmap->pix_depth != 1)
	    return_error(gs_error_rangecheck);
	pcspace = 0;
    } else if (gs_color_space_get_index(pcspace) != gs_color_space_index_Indexed)
	return_error(gs_error_rangecheck);
    if (pbitmap->num_comps != 1)
	return_error(gs_error_rangecheck);

    /* allocate and initialize a pixmap_info structure for the paint proc */
    if (mem == 0)
	mem = gs_state_memory(pgs);
    ppmap = gs_alloc_struct(mem,
			    pixmap_info,
			    &st_pixmap_info,
			    "makepximappattern"
	);
    if (ppmap == 0)
	return_error(gs_error_VMerror);
    ppmap->bitmap = *pbitmap;
    ppmap->pcspace = pcspace;
    ppmap->white_index = white_index;

    /* set up the client pattern structure */
    gs_pattern1_init(&pat);
    uid_set_UniqueID(&pat.uid, (id == no_UniqueID) ? gs_next_ids(mem, 1) : id);
    pat.PaintType = (mask ? 2 : 1);
    pat.TilingType = 1;
    pat.BBox.p.x = 0;
    pat.BBox.p.y = 0;
    pat.BBox.q.x = pbitmap->size.x;
    pat.BBox.q.y = pbitmap->size.y;
    pat.XStep = (float)pbitmap->size.x;
    pat.YStep = (float)pbitmap->size.y;
    pat.PaintProc = (mask ? mask_PaintProc : image_PaintProc);
    pat.client_data = ppmap;

    /* set the ctm to be the identity */
    gs_currentmatrix(pgs, &smat);
    gs_make_identity(&mat);
    gs_setmatrix(pgs, &mat);

    /* build the pattern, restore the previous matrix */
    if (pmat == NULL)
	pmat = &mat;
    if ((code = gs_makepattern(pcc, &pat, pmat, pgs, mem)) != 0)
	gs_free_object(mem, ppmap, "makebitmappattern_xform");
    else {
	/*
	 * If this is not a masked pattern and if the white pixel index
	 * is outside of the representable range, we don't need to go to
	 * the trouble of accumulating a mask that will just be all 1s.
	 */
	gs_pattern1_instance_t *pinst =
	    (gs_pattern1_instance_t *)pcc->pattern;

	if (!mask && (white_index >= (1 << pbitmap->pix_depth)))
	    pinst->uses_mask = false;

        /* overwrite the free procedure for the pattern instance */
        ppmap->free_proc = pinst->rc.free;
        pinst->rc.free = free_pixmap_pattern;

	/*
	 * Since the PaintProcs don't reference the saved color space or
	 * color, clear these so that there isn't an extra retained
	 * reference to the Pattern object.
	 */
	gs_setgray(pinst->saved, 0.0);

    }
    gs_setmatrix(pgs, &smat);
    return code;
}

/*
 *  Backwards compatibility.
 */
int
gs_makebitmappattern_xform(
			      gs_client_color * pcc,
			      const gx_tile_bitmap * ptile,
			      bool mask,
			      const gs_matrix * pmat,
			      long id,
			      gs_state * pgs,
			      gs_memory_t * mem
)
{
    gs_depth_bitmap bitmap;

    /* build the bitmap the size of one repetition */
    bitmap.data = ptile->data;
    bitmap.raster = ptile->raster;
    bitmap.size.x = ptile->rep_width;
    bitmap.size.y = ptile->rep_height;
    bitmap.id = ptile->id;	/* shouldn't matter */
    bitmap.pix_depth = 1;
    bitmap.num_comps = 1;

    return gs_makepixmappattern(pcc, &bitmap, mask, pmat, id, 0, 0, pgs, mem);
}


/* ------ Color space implementation ------ */

/*
 * Defined the Pattern device color types.  We need a masked analogue of
 * each of the non-pattern types, to handle uncolored patterns.  We use
 * 'masked_fill_rect' instead of 'masked_fill_rectangle' in order to limit
 * identifier lengths to 32 characters.
 */
static dev_color_proc_get_dev_halftone(gx_dc_pattern_get_dev_halftone);
static dev_color_proc_load(gx_dc_pattern_load);
/*dev_color_proc_fill_rectangle(gx_dc_pattern_fill_rectangle); *//*gxp1fill.h */
static dev_color_proc_equal(gx_dc_pattern_equal);
static dev_color_proc_load(gx_dc_pure_masked_load);

static dev_color_proc_get_dev_halftone(gx_dc_pure_masked_get_dev_halftone);
/*dev_color_proc_fill_rectangle(gx_dc_pure_masked_fill_rect); *//*gxp1fill.h */
static dev_color_proc_equal(gx_dc_pure_masked_equal);
static dev_color_proc_load(gx_dc_binary_masked_load);

static dev_color_proc_get_dev_halftone(gx_dc_binary_masked_get_dev_halftone);
/*dev_color_proc_fill_rectangle(gx_dc_binary_masked_fill_rect); *//*gxp1fill.h */
static dev_color_proc_equal(gx_dc_binary_masked_equal);
static dev_color_proc_load(gx_dc_colored_masked_load);

static dev_color_proc_get_dev_halftone(gx_dc_colored_masked_get_dev_halftone);
/*dev_color_proc_fill_rectangle(gx_dc_colored_masked_fill_rect); *//*gxp1fill.h */
static dev_color_proc_equal(gx_dc_colored_masked_equal);

/* The device color types are exported for gxpcmap.c. */
gs_private_st_composite(st_dc_pattern, gx_device_color, "dc_pattern",
			dc_pattern_enum_ptrs, dc_pattern_reloc_ptrs);
const gx_device_color_type_t gx_dc_pattern = {
    &st_dc_pattern,
    gx_dc_pattern_save_dc, gx_dc_pattern_get_dev_halftone,
    gx_dc_ht_get_phase,
    gx_dc_pattern_load, gx_dc_pattern_fill_rectangle,
    gx_dc_default_fill_masked, gx_dc_pattern_equal,
    gx_dc_pattern_write, gx_dc_pattern_read, 
    gx_dc_pattern_get_nonzero_comps
};

extern_st(st_dc_ht_binary);
gs_private_st_composite(st_dc_pure_masked, gx_device_color, "dc_pure_masked",
			dc_masked_enum_ptrs, dc_masked_reloc_ptrs);
const gx_device_color_type_t gx_dc_pure_masked = {
    &st_dc_pure_masked,
    gx_dc_pattern_save_dc, gx_dc_pure_masked_get_dev_halftone,
    gx_dc_no_get_phase,
    gx_dc_pure_masked_load, gx_dc_pure_masked_fill_rect,
    gx_dc_default_fill_masked, gx_dc_pure_masked_equal,
    gx_dc_cannot_write, gx_dc_cannot_read, 
    gx_dc_pure_get_nonzero_comps
};

gs_private_st_composite(st_dc_binary_masked, gx_device_color,
			"dc_binary_masked", dc_binary_masked_enum_ptrs,
			dc_binary_masked_reloc_ptrs);
const gx_device_color_type_t gx_dc_binary_masked = {
    &st_dc_binary_masked,
    gx_dc_pattern_save_dc, gx_dc_binary_masked_get_dev_halftone,
    gx_dc_ht_get_phase,
    gx_dc_binary_masked_load, gx_dc_binary_masked_fill_rect,
    gx_dc_default_fill_masked, gx_dc_binary_masked_equal,
    gx_dc_cannot_write, gx_dc_cannot_read, 
    gx_dc_ht_binary_get_nonzero_comps
};

gs_private_st_composite_only(st_dc_colored_masked, gx_device_color,
			     "dc_colored_masked",
			     dc_masked_enum_ptrs, dc_masked_reloc_ptrs);
const gx_device_color_type_t gx_dc_colored_masked = {
    &st_dc_colored_masked,
    gx_dc_pattern_save_dc, gx_dc_colored_masked_get_dev_halftone,
    gx_dc_ht_get_phase,
    gx_dc_colored_masked_load, gx_dc_colored_masked_fill_rect,
    gx_dc_default_fill_masked, gx_dc_colored_masked_equal,
    gx_dc_cannot_write, gx_dc_cannot_read, 
    gx_dc_ht_colored_get_nonzero_comps
};

#undef gx_dc_type_pattern
const gx_device_color_type_t *const gx_dc_type_pattern = &gx_dc_pattern;
#define gx_dc_type_pattern (&gx_dc_pattern)

/* GC procedures */
static 
ENUM_PTRS_WITH(dc_pattern_enum_ptrs, gx_device_color *cptr)
{
    return ENUM_USING(st_dc_pure_masked, vptr, size, index - 1);
}
case 0:
{
    gx_color_tile *tile = cptr->colors.pattern.p_tile;

    ENUM_RETURN((tile == 0 ? tile : tile - tile->index));
}
ENUM_PTRS_END
static RELOC_PTRS_WITH(dc_pattern_reloc_ptrs, gx_device_color *cptr)
{
    gx_color_tile *tile = cptr->colors.pattern.p_tile;

    if (tile != 0) {
	uint index = tile->index;

	RELOC_TYPED_OFFSET_PTR(gx_device_color, colors.pattern.p_tile, index);
    }
    RELOC_USING(st_dc_pure_masked, vptr, size);
}
RELOC_PTRS_END
static ENUM_PTRS_WITH(dc_masked_enum_ptrs, gx_device_color *cptr)
ENUM_SUPER(gx_device_color, st_client_color, ccolor, 1);
case 0:
{
    gx_color_tile *mask = cptr->mask.m_tile;

    ENUM_RETURN((mask == 0 ? mask : mask - mask->index));
}
ENUM_PTRS_END
static RELOC_PTRS_WITH(dc_masked_reloc_ptrs, gx_device_color *cptr)
{
    gx_color_tile *mask = cptr->mask.m_tile;

    RELOC_SUPER(gx_device_color, st_client_color, ccolor);
    if (mask != 0) {
	uint index = mask->index;

	RELOC_TYPED_OFFSET_PTR(gx_device_color, mask.m_tile, index);
    }
}
RELOC_PTRS_END
static ENUM_PTRS_BEGIN(dc_binary_masked_enum_ptrs)
{
    return ENUM_USING(st_dc_ht_binary, vptr, size, index - 2);
}
case 0:
case 1:
return ENUM_USING(st_dc_pure_masked, vptr, size, index);
ENUM_PTRS_END
static RELOC_PTRS_BEGIN(dc_binary_masked_reloc_ptrs)
{
    RELOC_USING(st_dc_pure_masked, vptr, size);
    RELOC_USING(st_dc_ht_binary, vptr, size);
}
RELOC_PTRS_END


/*
 * Currently patterns cannot be passed through the command list,
 * however vector devices need to save a color for comparing
 * it with another color, which appears later.
 * We provide a minimal support, which is necessary
 * for the current implementation of pdfwrite.
 * It is not sufficient for restoring the pattern from the saved color.
 */
void
gx_dc_pattern_save_dc(
    const gx_device_color * pdevc, 
    gx_device_color_saved * psdc )
{
    psdc->type = pdevc->type;
    if (pdevc->ccolor_valid) {
	psdc->colors.pattern.id = pdevc->ccolor.pattern->pattern_id;
	psdc->colors.pattern.phase = pdevc->phase;
    } else {
	/* The client color has been changed to a non-pattern color,
	   but device color has not been created yet. 
	 */
	psdc->colors.pattern.id = gs_no_id;
	psdc->colors.pattern.phase.x = psdc->colors.pattern.phase.y = 0;
    }
}

/*
 * Colored Type 1 patterns cannot provide a halftone, as multiple
 * halftones may be used by the PaintProc procedure. Hence, we can only
 * hope this is a contone device.
 */
static const gx_device_halftone *
gx_dc_pattern_get_dev_halftone(const gx_device_color * pdevc)
{
    return 0;
}

/*
 * Uncolored Type 1 halftones make use of the halftone impplied by their
 * base color. Ideally this would be returned via an inhereted method,
 * but the device color structure does not support such an arrangement.
 */
static const gx_device_halftone *
gx_dc_pure_masked_get_dev_halftone(const gx_device_color * pdevc)
{
    return 0;
}

static const gx_device_halftone *
gx_dc_binary_masked_get_dev_halftone(const gx_device_color * pdevc)
{
    return pdevc->colors.binary.b_ht;
}

static const gx_device_halftone *
gx_dc_colored_masked_get_dev_halftone(const gx_device_color * pdevc)
{
    return pdevc->colors.colored.c_ht;
}


/* Macros for pattern loading */
#define FINISH_PATTERN_LOAD\
	while ( !gx_pattern_cache_lookup(pdevc, pis, dev, select) )\
	 { code = gx_pattern_load(pdevc, pis, dev, select);\
	   if ( code < 0 ) break;\
	 }\
	return code;

/* Ensure that a colored Pattern is loaded in the cache. */
static int
gx_dc_pattern_load(gx_device_color * pdevc, const gs_imager_state * pis,
		   gx_device * dev, gs_color_select_t select)
{
    int code = 0;

    FINISH_PATTERN_LOAD
}
/* Ensure that an uncolored Pattern is loaded in the cache. */
static int
gx_dc_pure_masked_load(gx_device_color * pdevc, const gs_imager_state * pis,
		       gx_device * dev, gs_color_select_t select)
{
    int code = (*gx_dc_type_data_pure.load) (pdevc, pis, dev, select);

    if (code < 0)
	return code;
    FINISH_PATTERN_LOAD
}
static int
gx_dc_binary_masked_load(gx_device_color * pdevc, const gs_imager_state * pis,
			 gx_device * dev, gs_color_select_t select)
{
    int code = (*gx_dc_type_data_ht_binary.load) (pdevc, pis, dev, select);

    if (code < 0)
	return code;
    FINISH_PATTERN_LOAD
}
static int
gx_dc_colored_masked_load(gx_device_color * pdevc, const gs_imager_state * pis,
			  gx_device * dev, gs_color_select_t select)
{
    int code = (*gx_dc_type_data_ht_colored.load) (pdevc, pis, dev, select);

    if (code < 0)
	return code;
    FINISH_PATTERN_LOAD
}

/* Look up a pattern color in the cache. */
bool
gx_pattern_cache_lookup(gx_device_color * pdevc, const gs_imager_state * pis,
			gx_device * dev, gs_color_select_t select)
{
    gx_pattern_cache *pcache = pis->pattern_cache;
    gx_bitmap_id id = pdevc->mask.id;

    if (id == gx_no_bitmap_id) {
	color_set_null_pattern(pdevc);
	return true;
    }
    if (pcache != 0) {
	gx_color_tile *ctile = &pcache->tiles[id % pcache->num_tiles];
	bool internal_accum = true;
	if (pis->have_pattern_streams) {
	    int code = dev_proc(dev, pattern_manage)(dev, id, NULL, pattern_manage__load);
	    internal_accum = (code == 0);
	    if (code < 0)
		return false;
	}
	if (ctile->id == id &&
	    ctile->is_dummy == !internal_accum &&
	    (pdevc->type != &gx_dc_pattern ||
	     ctile->depth == dev->color_info.depth)
	    ) {
	    int px = pis->screen_phase[select].x;
	    int py = pis->screen_phase[select].y;

	    if (pdevc->type == &gx_dc_pattern) {	/* colored */
		pdevc->colors.pattern.p_tile = ctile;
#	    if 0 /* Debugged with Bug688308.ps and applying patterns after clist.
		    Bug688308.ps has a step_matrix much bigger than pattern bbox;
		    rep_width, rep_height can't be used as mod. 
		    Would like to use step_matrix instead. */
		color_set_phase_mod(pdevc, px, py,
				    ctile->tbits.rep_width,
				    ctile->tbits.rep_height);
#		else
		color_set_phase(pdevc, -px, -py);
#		endif
	    }
	    pdevc->mask.m_tile =
		(ctile->tmask.data == 0 ? (gx_color_tile *) 0 :
		 ctile);
	    pdevc->mask.m_phase.x = -px;
	    pdevc->mask.m_phase.y = -py;
	    return true;
	}
    }
    return false;
}

#undef FINISH_PATTERN_LOAD

/* Compare two Pattern colors for equality. */
static bool
gx_dc_pattern_equal(const gx_device_color * pdevc1,
		    const gx_device_color * pdevc2)
{
    return pdevc2->type == pdevc1->type &&
	pdevc1->phase.x == pdevc2->phase.x &&
	pdevc1->phase.y == pdevc2->phase.y &&
	pdevc1->mask.id == pdevc2->mask.id;
}

/*
 * For shading and colored tiling patterns, it is not possible to say
 * which color components have non-zero values. The following routine
 * indicates this by just returning 1. The procedure is exported for
 * the benefit of gsptype2.c.
 */
int
gx_dc_pattern_get_nonzero_comps(
    const gx_device_color * pdevc_ignored,
    const gx_device *       dev_ignored,
    gx_color_index *        pcomp_bits_ignored )
{
    return 1;
}


static bool
gx_dc_pure_masked_equal(const gx_device_color * pdevc1,
			const gx_device_color * pdevc2)
{
    return (*gx_dc_type_pure->equal) (pdevc1, pdevc2) &&
	pdevc1->mask.id == pdevc2->mask.id;
}
static bool
gx_dc_binary_masked_equal(const gx_device_color * pdevc1,
			  const gx_device_color * pdevc2)
{
    return (*gx_dc_type_ht_binary->equal) (pdevc1, pdevc2) &&
	pdevc1->mask.id == pdevc2->mask.id;
}
static bool
gx_dc_colored_masked_equal(const gx_device_color * pdevc1,
			   const gx_device_color * pdevc2)
{
    return (*gx_dc_type_ht_colored->equal) (pdevc1, pdevc2) &&
	pdevc1->mask.id == pdevc2->mask.id;
}


typedef struct gx_dc_serialized_tile_s { 
    gs_id id;
    int size_b, size_c;
    gs_int_point size;
    gs_matrix step_matrix;
    gs_rect bbox;
    byte is_clist;
    byte depth;
    byte tiling_type;
    byte is_simple;
} gx_dc_serialized_tile_t;

static int
gx_dc_pattern_write_raster(gx_color_tile *ptile, uint offset, byte *data, uint *psize)
{
    int size_b, size_c;
    byte *dp = data;
    int left = *psize;
    int offset1 = offset;

    size_b = sizeof(gx_strip_bitmap) + ptile->tbits.size.y * ptile->tbits.raster;
    size_c = ptile->tmask.data ? sizeof(gx_strip_bitmap) + ptile->tmask.size.y * ptile->tmask.raster : 0;
    if (data == NULL) {
	*psize = sizeof(gx_dc_serialized_tile_t) + size_b + size_c;
	return 0;
    }
    if (offset1 == 0) {	/* Serialize tile parameters: */
	gx_dc_serialized_tile_t buf;
	gx_strip_bitmap buf1;

    	buf.id = ptile->id;
	buf.size.x = 0; /* fixme: don't write with raster patterns. */
	buf.size.y = 0; /* fixme: don't write with raster patterns. */
	buf.size_b = size_b;
	buf.size_c = size_c;
	buf.step_matrix = ptile->step_matrix;
	buf.bbox = ptile->bbox;
	buf.is_clist = false;
	buf.depth = ptile->depth;
	buf.tiling_type = ptile->tiling_type;
	buf.is_simple = ptile->is_simple;
	if (sizeof(buf) > left) {
	    /* For a while we require the client to provide enough buffer size. */
	    return_error(gs_error_unregistered); /* Must not happen. */
	}
	memcpy(dp, &buf, sizeof(buf));
	left -= sizeof(buf);
	dp += sizeof(buf);
	offset1 += sizeof(buf);

	buf1 = ptile->tbits;
	buf1.data = NULL; /* fixme: we don't need to write it actually. */
	if (sizeof(buf1) > left) {
	    /* For a while we require the client to provide enough buffer size. */
	    return_error(gs_error_unregistered); /* Must not happen. */
	}
	memcpy(dp, &buf1, sizeof(buf1));
	left -= sizeof(buf1);
	dp += sizeof(buf1);
	offset1 += sizeof(buf1);
    }
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b) {
	int l = min((size_b - sizeof(gx_strip_bitmap)) - (offset1 - sizeof(gx_dc_serialized_tile_t) -  sizeof(gx_strip_bitmap)), left);

	memcpy(dp, ptile->tbits.data + (offset1 - sizeof(gx_dc_serialized_tile_t) -  sizeof(gx_strip_bitmap)), l);
	left -= l;
	dp += l;
	offset1 += l;
    }
    if (left == 0)
	return 0;
    if (size_c == 0)
	return 0;
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b + sizeof(gx_strip_bitmap)) {
	gx_strip_bitmap buf;

	if (left < sizeof(buf))
	    return_error(gs_error_unregistered); /* Not implemented yet because cmd_put_drawing_color provides a big buffer. */
	buf = ptile->tmask;
	buf.data = NULL; /* fixme: we don't need to write it actually. */
	memcpy(dp, &buf, sizeof(buf));
	left -= sizeof(buf);
	dp += sizeof(buf);
	offset1 += sizeof(buf);
    }
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b + size_c) {
	int l = min(size_c - sizeof(gx_strip_bitmap), left);

	memcpy(dp, ptile->tmask.data + (offset1 - sizeof(gx_dc_serialized_tile_t) - size_b - sizeof(gx_strip_bitmap)), l);
    }
    return 0;
}

/* Write a pattern into command list, possibly dividing intoi portions. */
int
gx_dc_pattern_write(
    const gx_device_color *         pdevc,
    const gx_device_color_saved *   psdc,
    const gx_device *               dev,
    uint			    offset,
    byte *                          data,
    uint *                          psize )
{
    gx_color_tile *ptile = pdevc->colors.pattern.p_tile;
    int size_b, size_c;
    byte *dp = data;
    int left = *psize;
    int offset1 = offset;
    int code, l;

    if (ptile == NULL)
	return 0;
    if (psdc->type == pdevc->type) {
	if (psdc->colors.pattern.id == ptile->id) {
	    /* fixme : Do we need to check phase ? How ? */
	    return 1; /* Same as saved one, don't write. */
	}
    }
    if (offset1 == 0 && left == sizeof(gs_id)) {
	/* A special case for writing a known pattern : 
	   Just write the tile id. */
	gs_id id = ptile->id; /* Ensure sizeof(gs_id). */

	memcpy(dp, &ptile->id, sizeof(id));
	*psize = sizeof(gs_id);
	return 0;
    }
    if (ptile->cdev == NULL)
	return gx_dc_pattern_write_raster(ptile, offset, data, psize);
    size_b = clist_data_size(ptile->cdev, 0);
    if (size_b < 0)
	return_error(gs_error_unregistered);
    size_c = clist_data_size(ptile->cdev, 1);
    if (size_c < 0)
	return_error(gs_error_unregistered);
    if (data == NULL) {
	*psize = sizeof(gx_dc_serialized_tile_t) + size_b + size_c;
	return 0;
    }
    if (offset1 == 0) {	/* Serialize tile parameters: */
	gx_dc_serialized_tile_t buf;

	buf.id = ptile->id;
	buf.size.x = ptile->cdev->common.width;
	buf.size.y = ptile->cdev->common.height;
	buf.size_b = size_b;
	buf.size_c = size_c;
	buf.step_matrix = ptile->step_matrix;
	buf.bbox = ptile->bbox;
	buf.is_clist = true;
	buf.depth = ptile->depth;
	buf.tiling_type = ptile->tiling_type;
	buf.is_simple = ptile->is_simple;
	if (sizeof(buf) > left) {
	    /* For a while we require the client to provide enough buffer size. */
	    return_error(gs_error_unregistered); /* Must not happen. */
	}
	memcpy(dp, &buf, sizeof(gx_dc_serialized_tile_t));
	left -= sizeof(buf);
	dp += sizeof(buf);
	offset1 += sizeof(buf);
    }
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b) {
	l = min(left, size_b - (offset1 - sizeof(gx_dc_serialized_tile_t)));
	code = clist_get_data(ptile->cdev, 0, offset1 - sizeof(gx_dc_serialized_tile_t), dp, l);
	if (code < 0)
	    return code;
	left -= l;
	offset1 += l;
	dp += l;
    }
    if (left > 0) {
	l = min(left, size_c - (offset1 - sizeof(gx_dc_serialized_tile_t) - size_b));
	code = clist_get_data(ptile->cdev, 1, offset1 - sizeof(gx_dc_serialized_tile_t) - size_b, dp, l);
	if (code < 0)
	    return code;
    }
    return 0;
}

static int
gx_dc_pattern_read_raster(gx_color_tile *ptile, const gx_dc_serialized_tile_t *buf, 
			  uint offset, const byte *data, uint size, gs_memory_t *mem)
{
    const byte *dp = data;
    int left = size;
    int offset1 = offset;
    int size_b, size_c;

    if (buf != NULL) {
	size_b = buf->size_b;
	size_c = buf->size_c;
	ptile->tbits.data = gs_alloc_bytes(mem, size_b - sizeof(gx_strip_bitmap), "gx_dc_pattern_read_raster");
	if (ptile->tbits.data == NULL)
	    return_error(gs_error_VMerror);
	if (size_c) {
	    ptile->tmask.data = gs_alloc_bytes(mem, size_c - sizeof(gx_strip_bitmap), "gx_dc_pattern_read_raster");
	    if (ptile->tmask.data == NULL)
		return_error(gs_error_VMerror);
	} else
	    ptile->tmask.data =	NULL;
	ptile->cdev = NULL;
    } else {
	size_b = gs_object_size(mem, ptile->tbits.data) + sizeof(gx_strip_bitmap);
	size_c = ptile->tmask.data != NULL ? gs_object_size(mem, ptile->tmask.data) + sizeof(gx_strip_bitmap) : 0;
    }
    /* Read tbits : */
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + sizeof(gx_strip_bitmap)) {
	int l = min(sizeof(gx_strip_bitmap), left);
	byte *save = ptile->tbits.data;

	memcpy((byte*)&ptile->tbits + (offset1 - sizeof(gx_dc_serialized_tile_t)), dp, l);
	ptile->tbits.data = save;
	left -= l;
	offset1 += l;
	dp += l;
    }
    if (left == 0)
	return 0;
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b) {
	int l = min(sizeof(gx_dc_serialized_tile_t) + size_b - offset1, left);

	memcpy(ptile->tbits.data + 
		(offset1 - sizeof(gx_dc_serialized_tile_t) - sizeof(gx_strip_bitmap)), dp, l);
	left -= l;
	offset1 += l;
	dp += l;
    }
    if (left == 0 || size_c == 0)
	return size - left;
    /* Read tmask : */
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b + sizeof(gx_strip_bitmap)) {
	int l = min(sizeof(gx_dc_serialized_tile_t) + size_b + sizeof(gx_strip_bitmap) - offset1, left);
	byte *save = ptile->tmask.data;

	memcpy((byte*)&ptile->tmask + (offset1 - sizeof(gx_dc_serialized_tile_t) - size_b), dp, l);
	ptile->tmask.data = save;
	left -= l;
	offset1 += l;
	dp += l;
    }
    if (left == 0)
	return 0;
    if (offset1 <= sizeof(gx_dc_serialized_tile_t) + size_b + size_c) {
	int l = min(sizeof(gx_dc_serialized_tile_t) + size_b + size_c - offset1, left);

	memcpy(ptile->tmask.data + 
		(offset1 - sizeof(gx_dc_serialized_tile_t) - size_b - sizeof(gx_strip_bitmap)), dp, l);
	left -= l;
	offset1 += l;
	dp += l;
    }
    return size - left;
}

int
gx_dc_pattern_read(
    gx_device_color *       pdevc,
    const gs_imager_state * pis,
    const gx_device_color * prior_devc,
    const gx_device *       dev,
    uint		    offset,
    const byte *            data,
    uint                    size,
    gs_memory_t *           mem )
{
    gx_dc_serialized_tile_t buf;
    int size_b, size_c = -1;
    const byte *dp = data;
    int left = size;
    int offset1 = offset;
    gx_color_tile *ptile;
    int code, l;

    if (offset == 0) {
	pdevc->mask.id = gx_no_bitmap_id;
	pdevc->mask.m_tile = NULL;
	if (size == 0) {
	    /* Null pattern. */
	    pdevc->type = &gx_dc_pattern;
	    pdevc->colors.pattern.p_tile = NULL;
	    pdevc->mask.id = gs_no_id;
	    return 0;
	}
	if (size == sizeof(gs_id)) {
	    /* A special case for restoring a known (cached) pattern :
	       read the tile id only. */
	    gs_id id; /* Ensure data size == sizeof(gs_id). */

	    memcpy(&id, dp, sizeof(id));
	    pdevc->type = &gx_dc_pattern;
	    pdevc->mask.id = id; /* See gx_dc_pattern_load, gx_pattern_cache_lookup. */
	    return size;
	}
	if (sizeof(buf) > size) {
	    /* For a while we require the client to provide enough buffer size. */
	    return_error(gs_error_unregistered); /* Must not happen. */
	}
	memcpy(&buf, dp, sizeof(buf));
	dp += sizeof(buf);
	left -= sizeof(buf);
	offset1 += sizeof(buf);

	code = gx_pattern_cache_get_entry((gs_imager_state *)pis, /* Break 'const'. */
			buf.id, &ptile);
	if (code < 0)
	    return code;
	pdevc->type = &gx_dc_pattern;
	pdevc->colors.pattern.p_tile = ptile;
	ptile->id = buf.id;
	pdevc->mask.id = buf.id;
	ptile->step_matrix = buf.step_matrix;
	ptile->bbox = buf.bbox;
	ptile->depth = buf.depth;
	ptile->tiling_type = buf.tiling_type;
	ptile->is_simple = buf.is_simple;
	ptile->is_dummy = 0;
	if (!buf.is_clist) {
	    code = gx_dc_pattern_read_raster(ptile, &buf, offset1, dp, left, mem);
	    if (code < 0)
		return code;
	    return code + sizeof(buf);
	}
	size_b = buf.size_b;
	size_c = buf.size_c;
	ptile->tbits.size.x = size_b; /* HACK: Use unrelated field for saving size_b between calls. */
	ptile->tbits.size.y = size_c; /* HACK: Use unrelated field for saving size_c between calls. */
	{   /* HACK: Artificial arguments for gx_pattern_accum_alloc 
	       to force a clist-based accummulator. 
	       A better way would be to split gx_pattern_accum_alloc.
	     */
	    gs_state state;
	    gs_pattern1_instance_t inst;

	    memset(&state, 0, sizeof(state));
	    memset(&inst, 0, sizeof(inst));
#	    if 0 /* wrong. Currently PaintType 2 can't pass here. */
	    if (buf.paint_type == 2) {
		/* Convert to a raster pattern cell cropped with the current band. */
		gx_device_memory *mdev = gs_alloc_struct(mem, gx_device_memory,
				       &st_device_memory,
				       "gx_dc_pattern_read");

		if (mdev == 0)
		    return_error(gs_error_VMerror);
		gs_make_mem_device(mdev, gdev_mem_device_for_bits(1), mem, -1, NULL);
		state.device = (gx_device *)mdev;
		inst.template.PaintType = 2;
	    } else 
#	    endif
	    {
		state.device = (gx_device *)dev; /* Break 'const'. */
		inst.template.PaintType = 1;
	    }
	    inst.size.x = buf.size.x;
	    inst.size.y = buf.size.y;
	    inst.saved = &state;
	    ptile->cdev = (gx_device_clist *)gx_pattern_accum_alloc(mem, mem, 
			       &inst, "gx_dc_pattern_read");
	    if (ptile->cdev == NULL)
		return_error(gs_error_VMerror);
	    code = dev_proc(&ptile->cdev->writer, open_device)((gx_device *)&ptile->cdev->writer);
	    if (code < 0)
		return code;
	}
    } else {
	ptile = pdevc->colors.pattern.p_tile;
	if (ptile->cdev == NULL)
	    return gx_dc_pattern_read_raster(ptile, NULL, offset1, dp, left, mem);
	size_b = ptile->tbits.size.x;
	size_c = ptile->tbits.size.y;
    }
    if (offset1 <= sizeof(buf) + size_b) {
	l = min(left, size_b - (offset1 - sizeof(buf)));
	code = clist_put_data(ptile->cdev, 0, offset1 - sizeof(buf), dp, l);
	if (code < 0)
	    return code;
	l = code;
	left -= l;
	offset1 += l;
	dp += l;
	ptile->cdev->common.page_bfile_end_pos = offset1 - sizeof(buf);
    }
    if (left > 0) {
	l = left;
	code = clist_put_data(ptile->cdev, 1, offset1 - sizeof(buf) - size_b, dp, l);
	if (code < 0)
	    return code;
	l = code;
	left -= l;
	offset1 += l;
    }
    return size - left;
}
