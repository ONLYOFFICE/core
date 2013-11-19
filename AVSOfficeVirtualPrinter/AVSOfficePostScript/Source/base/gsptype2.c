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

/* $Id: gsptype2.c 8655 2008-04-21 14:53:38Z leonardo $ */
/* PatternType 2 implementation */
#include "gx.h"
#include "gserrors.h"
#include "gscspace.h"
#include "gsshade.h"
#include "gsmatrix.h"           /* for gspcolor.h */
#include "gsstate.h"            /* for set/currentfilladjust */
#include "gxcolor2.h"
#include "gxdcolor.h"
#include "gsptype2.h"
#include "gxpcolor.h"
#include "gxstate.h"            /* for gs_state_memory */
#include "gzpath.h"
#include "gzcpath.h"
#include "gzstate.h"

/* GC descriptors */
private_st_pattern2_template();
private_st_pattern2_instance();

/* GC procedures */
static ENUM_PTRS_BEGIN(pattern2_instance_enum_ptrs) {
    if (index < st_pattern2_template_max_ptrs) {
        gs_ptr_type_t ptype =
            ENUM_SUPER_ELT(gs_pattern2_instance_t, st_pattern2_template,
                           template, 0);

        if (ptype)
            return ptype;
        return ENUM_OBJ(NULL);  /* don't stop early */
    }
    ENUM_PREFIX(st_pattern_instance, st_pattern2_template_max_ptrs);
}
ENUM_PTRS_END
static RELOC_PTRS_BEGIN(pattern2_instance_reloc_ptrs) {
    RELOC_PREFIX(st_pattern_instance);
    RELOC_SUPER(gs_pattern2_instance_t, st_pattern2_template, template);
} RELOC_PTRS_END

/* Define a PatternType 2 pattern. */
static pattern_proc_uses_base_space(gs_pattern2_uses_base_space);
static pattern_proc_make_pattern(gs_pattern2_make_pattern);
static pattern_proc_get_pattern(gs_pattern2_get_pattern);
static pattern_proc_remap_color(gs_pattern2_remap_color);
static pattern_proc_set_color(gs_pattern2_set_color);
static const gs_pattern_type_t gs_pattern2_type = {
    2, {
        gs_pattern2_uses_base_space, gs_pattern2_make_pattern,
        gs_pattern2_get_pattern, gs_pattern2_remap_color,
        gs_pattern2_set_color,
    }
};

/* Initialize a PatternType 2 pattern. */
void
gs_pattern2_init(gs_pattern2_template_t * ppat)
{
    gs_pattern_common_init((gs_pattern_template_t *)ppat, &gs_pattern2_type);
}

/* Test whether a PatternType 2 pattern uses a base space. */
static bool
gs_pattern2_uses_base_space(const gs_pattern_template_t *ptemp)
{
    return false;
}

/* Make an instance of a PatternType 2 pattern. */
static int
gs_pattern2_make_pattern(gs_client_color * pcc,
                         const gs_pattern_template_t * pcp,
                         const gs_matrix * pmat, gs_state * pgs,
                         gs_memory_t * mem)
{
    const gs_pattern2_template_t *ptemp =
        (const gs_pattern2_template_t *)pcp;
    int code = gs_make_pattern_common(pcc, pcp, pmat, pgs, mem,
                                      &st_pattern2_instance);
    gs_pattern2_instance_t *pinst;

    if (code < 0)
        return code;
    pinst = (gs_pattern2_instance_t *)pcc->pattern;
    pinst->template = *ptemp;
    pinst->shfill = false;
    return 0;
}

/* Get the template of a PatternType 2 pattern instance. */
static const gs_pattern_template_t *
gs_pattern2_get_pattern(const gs_pattern_instance_t *pinst)
{
    return (const gs_pattern_template_t *)
        &((const gs_pattern2_instance_t *)pinst)->template;
}

/* Set the 'shfill' flag to a PatternType 2 pattern instance. */
int
gs_pattern2_set_shfill(gs_client_color * pcc)
{
    gs_pattern2_instance_t *pinst;

    if (pcc->pattern->type != &gs_pattern2_type)
	return_error(gs_error_unregistered); /* Must not happen. */
    pinst = (gs_pattern2_instance_t *)pcc->pattern;
    pinst->shfill = true;
    return 0;
}


/* ---------------- Rendering ---------------- */

/* GC descriptor */
gs_private_st_ptrs_add0(st_dc_pattern2, gx_device_color, "dc_pattern2",
                        dc_pattern2_enum_ptrs, dc_pattern2_reloc_ptrs,
                        st_client_color, ccolor);

static dev_color_proc_get_dev_halftone(gx_dc_pattern2_get_dev_halftone);
static dev_color_proc_load(gx_dc_pattern2_load);
static dev_color_proc_fill_rectangle(gx_dc_pattern2_fill_rectangle);
static dev_color_proc_equal(gx_dc_pattern2_equal);
static dev_color_proc_save_dc(gx_dc_pattern2_save_dc);
/*
 * Define the PatternType 2 Pattern device color type.  This is public only
 * for testing when writing PDF or PostScript.
 */
const gx_device_color_type_t gx_dc_pattern2 = {
    &st_dc_pattern2,
    gx_dc_pattern2_save_dc, gx_dc_pattern2_get_dev_halftone,
    gx_dc_ht_get_phase,
    gx_dc_pattern2_load, gx_dc_pattern2_fill_rectangle,
    gx_dc_default_fill_masked, gx_dc_pattern2_equal,
    gx_dc_cannot_write, gx_dc_cannot_read, 
    gx_dc_pattern_get_nonzero_comps
};

/* Check device color for Pattern Type 2. */
bool
gx_dc_is_pattern2_color(const gx_device_color *pdevc)
{
    return pdevc->type == &gx_dc_pattern2;
}

/*
 * The device halftone used by a PatternType 2 patter is that current in
 * the graphic state at the time of the makepattern call.
 */
static const gx_device_halftone *
gx_dc_pattern2_get_dev_halftone(const gx_device_color * pdevc)
{
    return ((gs_pattern2_instance_t *)pdevc->ccolor.pattern)->saved->dev_ht;
}

/* Load a PatternType 2 color into the cache.  (No effect.) */
static int
gx_dc_pattern2_load(gx_device_color *pdevc, const gs_imager_state *ignore_pis,
                    gx_device *ignore_dev, gs_color_select_t ignore_select)
{
    return 0;
}

/* Remap a PatternType 2 color. */
static int
gs_pattern2_remap_color(const gs_client_color * pc, const gs_color_space * pcs,
                        gx_device_color * pdc, const gs_imager_state * pis,
                        gx_device * dev, gs_color_select_t select)
{
    /* We don't do any actual color mapping now. */
    pdc->type = &gx_dc_pattern2;
    pdc->ccolor = *pc;
    pdc->ccolor_valid = true;
    return 0;
}

/*
 * Perform actions required at set_color time. Since PatternType 2
 * patterns specify a color space, we must update the overprint
 * information as required by that color space. We temporarily disable
 * overprint_mode, as it is never applicable when using shading patterns.
 */
static int
gs_pattern2_set_color(const gs_client_color * pcc, gs_state * pgs)
{
    gs_pattern2_instance_t * pinst = (gs_pattern2_instance_t *)pcc->pattern;
    gs_color_space * pcs = pinst->template.Shading->params.ColorSpace;
    int code, save_overprint_mode = pgs->overprint_mode;

    pgs->overprint_mode = 0;
    code = pcs->type->set_overprint(pcs, pgs);
    pgs->overprint_mode = save_overprint_mode;
    return code;
}

/* Fill a rectangle with a PatternType 2 color. */
/* WARNING: This function doesn't account the shading BBox
   to allow the clipent to optimize the clipping 
   with changing the order of clip patrhs and rects.
   The client must clip with the shading BBOx before calling this function. */
static int
gx_dc_pattern2_fill_rectangle(const gx_device_color * pdevc, int x, int y,
                              int w, int h, gx_device * dev,
                              gs_logical_operation_t lop,
                              const gx_rop_source_t * source)
{
    if (dev_proc(dev, pattern_manage)(dev, gs_no_id, NULL, pattern_manage__is_cpath_accum)) {
	/* Performing a conversion of imagemask into a clipping path.
	   Fall back to the device procedure. */
	return dev_proc(dev, fill_rectangle)(dev, x, y, w, h, (gx_color_index)0/*any*/);
    } else {
	gs_fixed_rect rect;
	gs_pattern2_instance_t *pinst =
	    (gs_pattern2_instance_t *)pdevc->ccolor.pattern;

	rect.p.x = int2fixed(x);
	rect.p.y = int2fixed(y);
	rect.q.x = int2fixed(x + w);
	rect.q.y = int2fixed(y + h);
	return gs_shading_do_fill_rectangle(pinst->template.Shading, &rect, dev,
				    (gs_imager_state *)pinst->saved, !pinst->shfill);
    }
}

/* Compare two PatternType 2 colors for equality. */
static bool
gx_dc_pattern2_equal(const gx_device_color * pdevc1,
                     const gx_device_color * pdevc2)
{
    return pdevc2->type == pdevc1->type &&
        pdevc1->ccolor.pattern == pdevc2->ccolor.pattern;
}

/*
 * Currently patterns cannot be passed through the command list,
 * however vector devices need to save a color for comparing
 * it with another color, which appears later.
 * We provide a minimal support, which is necessary
 * for the current implementation of pdfwrite.
 * It is not sufficient for restoring the pattern from the saved color.
 */
static void
gx_dc_pattern2_save_dc(
    const gx_device_color * pdevc, 
    gx_device_color_saved * psdc )
{
    gs_pattern2_instance_t * pinst = (gs_pattern2_instance_t *)pdevc->ccolor.pattern;

    psdc->type = pdevc->type;
    psdc->colors.pattern2.id = pinst->pattern_id;
    psdc->colors.pattern2.shfill = pinst->shfill;
}

/* Transform a shading bounding box into device space. */
/* This is just a bridge to an old code. */
int
gx_dc_pattern2_shade_bbox_transform2fixed(const gs_rect * rect, const gs_imager_state * pis,
			   gs_fixed_rect * rfixed)
{
    gs_rect dev_rect;
    int code = gs_bbox_transform(rect, &ctm_only(pis), &dev_rect);

    if (code >= 0) {
	rfixed->p.x = float2fixed(dev_rect.p.x);
	rfixed->p.y = float2fixed(dev_rect.p.y);
	rfixed->q.x = float2fixed(dev_rect.q.x);
	rfixed->q.y = float2fixed(dev_rect.q.y);
    }
    return code;
}

/* Get a shading bbox. Returns 1 on success. */
int
gx_dc_pattern2_get_bbox(const gx_device_color * pdevc, gs_fixed_rect *bbox)
{
    gs_pattern2_instance_t *pinst =
        (gs_pattern2_instance_t *)pdevc->ccolor.pattern;
    int code;

    if (!pinst->template.Shading->params.have_BBox)
	return 0;
    code = gx_dc_pattern2_shade_bbox_transform2fixed(
		&pinst->template.Shading->params.BBox, (gs_imager_state *)pinst->saved, bbox);
    if (code < 0)
	return code;
    return 1;
}

int
gx_dc_pattern2_color_has_bbox(const gx_device_color * pdevc)
{
    gs_pattern2_instance_t *pinst = (gs_pattern2_instance_t *)pdevc->ccolor.pattern;
    const gs_shading_t *psh = pinst->template.Shading;

    return psh->params.have_BBox;
}

/* Create a path from a PatternType 2 shading BBox to a path. */
static int
gx_dc_shading_path_add_box(gx_path *ppath, const gx_device_color * pdevc)
{
    gs_pattern2_instance_t *pinst = (gs_pattern2_instance_t *)pdevc->ccolor.pattern;
    const gs_shading_t *psh = pinst->template.Shading;

    if (!psh->params.have_BBox)
	return_error(gs_error_unregistered); /* Do not call in this case. */
    else {
	gs_state *pis = pinst->saved;

	return gs_shading_path_add_box(ppath, &psh->params.BBox, &pis->ctm);
    }
}

/* Intersect a clipping path a shading BBox. */
int
gx_dc_pattern2_clip_with_bbox_simple(const gx_device_color * pdevc, gx_device * pdev, 
			      gx_clip_path *cpath_local)
{
    int code = 0;

    if (gx_dc_is_pattern2_color(pdevc) && gx_dc_pattern2_color_has_bbox(pdevc) &&
	    (*dev_proc(pdev, pattern_manage))(pdev, gs_no_id, NULL, pattern_manage__shading_area) == 0) {
	gs_pattern2_instance_t *pinst = (gs_pattern2_instance_t *)pdevc->ccolor.pattern;
	gx_path box_path;
	gs_memory_t *mem = cpath_local->path.memory;

	gx_path_init_local(&box_path, mem);
	code = gx_dc_shading_path_add_box(&box_path, pdevc);
	if (code == gs_error_limitcheck) {
	    /* Ignore huge BBox - bug 689027. */
	    code = 0;
	} else if (code >= 0) {
	    code = gx_cpath_intersect(cpath_local, &box_path, gx_rule_winding_number, (gs_imager_state *)pinst->saved);
	}
	gx_path_free(&box_path, "gx_default_fill_path(path_bbox)");
    }
    return code;
}

/* Check whether color is a shading with BBox. */
int
gx_dc_pattern2_is_rectangular_cell(const gx_device_color * pdevc, gx_device * pdev, gs_fixed_rect *rect)
{
    if (gx_dc_is_pattern2_color(pdevc) && gx_dc_pattern2_color_has_bbox(pdevc) &&
	    (*dev_proc(pdev, pattern_manage))(pdev, gs_no_id, NULL, pattern_manage__shading_area) == 0) {
	gs_pattern2_instance_t *pinst = (gs_pattern2_instance_t *)pdevc->ccolor.pattern;
	const gs_shading_t *psh = pinst->template.Shading;
	gs_fixed_point p, q; 

	if (is_xxyy(&ctm_only(pinst->saved)))
	    if (psh->params.have_BBox) {
		int code = gs_point_transform2fixed(&pinst->saved->ctm, 
			    psh->params.BBox.p.x, psh->params.BBox.p.y, &p);
		if (code < 0)
		    return code;
		code = gs_point_transform2fixed(&pinst->saved->ctm, 
			    psh->params.BBox.q.x, psh->params.BBox.q.y, &q);
		if (code < 0)
		    return code;
		if (p.x > q.x) {
		    p.x ^= q.x; q.x ^= p.x; p.x ^= q.x;
		}
		if (p.y > q.y) {
		    p.y ^= q.y; q.y ^= p.y; p.y ^= q.y;
		}
		rect->p = p;
		rect->q = q;
		return 1;
	    }
    }
    return 0;
}


/* Get a shading color space. */
const gs_color_space *
gx_dc_pattern2_get_color_space(const gx_device_color * pdevc)
{
    gs_pattern2_instance_t *pinst =
        (gs_pattern2_instance_t *)pdevc->ccolor.pattern;
    const gs_shading_t *psh = pinst->template.Shading;

    return psh->params.ColorSpace;
}


/* Check device color for a possibly self-overlapping shading. */
bool
gx_dc_pattern2_can_overlap(const gx_device_color *pdevc)
{
    gs_pattern2_instance_t * pinst;

    if (pdevc->type != &gx_dc_pattern2)
	return false;
    pinst = (gs_pattern2_instance_t *)pdevc->ccolor.pattern;
    switch (pinst->template.Shading->head.type) {
	case 3: case 6: case 7:
	    return true;
	default:
	    return false;
    }
}

/* Check whether a pattern color has a background. */
bool gx_dc_pattern2_has_background(const gx_device_color *pdevc)
{
    gs_pattern2_instance_t * pinst;
    const gs_shading_t *Shading;

    if (pdevc->type != &gx_dc_pattern2)
	return false;
    pinst = (gs_pattern2_instance_t *)pdevc->ccolor.pattern;
    Shading = pinst->template.Shading;
    return !pinst->shfill && Shading->params.Background != NULL;
}
