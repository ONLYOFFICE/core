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

/* $Id: gscsepr.c 8488 2008-01-17 13:37:19Z leonardo $ */
/* Separation color space and operation definition */
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsfunc.h"
#include "gsrefct.h"
#include "gsmatrix.h"		/* for gscolor2.h */
#include "gscsepr.h"
#include "gxcspace.h"
#include "gxfixed.h"		/* for gxcolor2.h */
#include "gxcolor2.h"		/* for gs_indexed_map */
#include "gzstate.h"		/* for pgs->overprint */
#include "gscdevn.h"		/* for alloc_device_n_map */
#include "gxcdevn.h"		/* for gs_device_n_map_s */
#include "gxcmap.h"
#include "gxdevcli.h"
#include "gsovrc.h"
#include "stream.h"
#include "gsnamecl.h"

/* ---------------- Color space ---------------- */

gs_private_st_composite(st_color_space_Separation, gs_color_space,
			"gs_color_space_Separation",
			cs_Separation_enum_ptrs, cs_Separation_reloc_ptrs);

/* Define the Separation color space type. */
static cs_proc_init_color(gx_init_Separation);
static cs_proc_concrete_space(gx_concrete_space_Separation);
static cs_proc_concretize_color(gx_concretize_Separation);
static cs_proc_remap_concrete_color(gx_remap_concrete_Separation);
static cs_proc_remap_color(gx_remap_Separation);
static cs_proc_install_cspace(gx_install_Separation);
static cs_proc_set_overprint(gx_set_overprint_Separation);
static cs_proc_final(gx_final_Separation);
static cs_proc_serialize(gx_serialize_Separation);
const gs_color_space_type gs_color_space_type_Separation = {
    gs_color_space_index_Separation, true, false,
    &st_color_space_Separation, gx_num_components_1,
    gx_init_Separation, gx_restrict01_paint_1,
    gx_concrete_space_Separation,
    gx_concretize_Separation, gx_remap_concrete_Separation,
    gx_remap_Separation, gx_install_Separation,
    gx_set_overprint_Separation,
    gx_final_Separation, gx_no_adjust_color_count,
    gx_serialize_Separation,
    gx_cspace_is_linear_default
};

/* GC procedures */

static 
ENUM_PTRS_BEGIN(cs_Separation_enum_ptrs) return 0;
    ENUM_PTR(0, gs_color_space, params.separation.map);
ENUM_PTRS_END
static RELOC_PTRS_BEGIN(cs_Separation_reloc_ptrs)
{
    RELOC_PTR(gs_color_space, params.separation.map);
}
RELOC_PTRS_END

/* Get the concrete space for a Separation space. */
static const gs_color_space *
gx_concrete_space_Separation(const gs_color_space * pcs,
			     const gs_imager_state * pis)
{
#ifdef DEBUG
    /* 
     * Verify that the color space and imager state info match.
     */
    if (pcs->id != pis->color_component_map.cspace_id)
	dprintf("gx_concretze_space_Separation: color space id mismatch");
#endif

    /*
     * Check if we are using the alternate color space.
     */
    if (pis->color_component_map.use_alt_cspace) {
        return cs_concrete_space(pcs->base_space, pis);
    }
    /*
     * Separation color spaces are concrete (when not using alt. color space).
     */
    return pcs;
}

static int
check_Separation_component_name(const gs_color_space * pcs, gs_state * pgs);

/* Install a Separation color space. */
static int
gx_install_Separation(gs_color_space * pcs, gs_state * pgs)
{
    int code;
#if ENABLE_CUSTOM_COLOR_CALLBACK
    /*
     * Check if we want to use the custom color callback color processing for
     * this color space.
     */
    bool use_custom_color_callback =
	    custom_color_callback_install_Separation(pcs, pgs);

    if (use_custom_color_callback) {
	/*
	 * We are using the callback instead of the alternate tint transform
	 * for this color space.
	 */
        pgs->color_space->params.separation.use_alt_cspace =
            pgs->color_component_map.use_alt_cspace = false;
        pgs->color_component_map.cspace_id = pcs->id;
        return 0;
    }
#endif

    code = check_Separation_component_name(pcs, pgs);
    if (code < 0)
       return code;
    pgs->color_space->params.separation.use_alt_cspace =
	using_alt_color_space(pgs);
    if (pgs->color_space->params.separation.use_alt_cspace)
        code = (pcs->base_space->type->install_cspace)
	    (pcs->base_space, pgs);
    /*
     * Give the device an opportunity to capture equivalent colors for any
     * spot colors which might be present in the color space.
     */
    if (code >= 0)
        code = dev_proc(pgs->device, update_spot_equivalent_colors)
							(pgs->device, pgs);
    return code;
}

/* Set the overprint information appropriate to a separation color space */
static int
gx_set_overprint_Separation(const gs_color_space * pcs, gs_state * pgs)
{
    gs_devicen_color_map *  pcmap = &pgs->color_component_map;

    if (pcmap->use_alt_cspace)
        return gx_spot_colors_set_overprint( pcs->base_space, pgs );
    else {
        gs_overprint_params_t   params;

        params.retain_any_comps = pgs->overprint &&
                                  pcs->params.separation.sep_type != SEP_ALL;
        if (params.retain_any_comps) {
            params.retain_spot_comps = false;
            params.drawn_comps = 0;
            if (pcs->params.separation.sep_type != SEP_NONE) {
                int     mcomp = pcmap->color_map[0];

                if (mcomp >= 0)
		    gs_overprint_set_drawn_comp( params.drawn_comps, mcomp);
            }
        }

        pgs->effective_overprint_mode = 0;
        return gs_state_update_overprint(pgs, &params);
    }
}

/* Finalize contents of a Separation color space. */
static void
gx_final_Separation(const gs_color_space * pcs)
{
    rc_adjust_const(pcs->params.separation.map, -1,
		    "gx_adjust_Separation");
}

/* ------ Constructors/accessors ------ */

/*
 * Construct a new separation color space.
 */
int
gs_cspace_new_Separation(
    gs_color_space **ppcs,
    gs_color_space * palt_cspace,
    gs_memory_t * pmem
)
{
    gs_color_space *pcs;
    int code;

    if (palt_cspace == 0 || !palt_cspace->type->can_be_alt_space)
	return_error(gs_error_rangecheck);

    pcs = gs_cspace_alloc(pmem, &gs_color_space_type_Separation);
    if (pcs == NULL)
	return_error(gs_error_VMerror);
    pcs->params.separation.map = NULL;

    code = alloc_device_n_map(&pcs->params.separation.map, pmem,
			      "gs_cspace_build_Separation");
    if (code < 0) {
	gs_free_object(pmem, pcs, "gs_cspace_build_Separation");
	return_error(code);
    }
    pcs->base_space = palt_cspace;
    rc_increment(palt_cspace);
    *ppcs = pcs;
    return 0;
}

#if 0 /* Unused; Unsupported by gx_serialize_device_n_map. */
/*
 * Set the tint transformation procedure used by a Separation color space.
 */
int
gs_cspace_set_sepr_proc(gs_color_space * pcspace,
			int (*proc)(const float *,
                                    float *,
                                    const gs_imager_state *,
                                    void *
				    ),
			void *proc_data
			)
{
    gs_device_n_map *pimap;

    if (gs_color_space_get_index(pcspace) != gs_color_space_index_Separation)
	return_error(gs_error_rangecheck);
    pimap = pcspace->params.separation.map;
    pimap->tint_transform = proc;
    pimap->tint_transform_data = proc_data;
    pimap->cache_valid = false;

    return 0;
}
#endif

/*
 * Set the Separation tint transformation procedure to a Function.
 */
int
gs_cspace_set_sepr_function(const gs_color_space *pcspace, gs_function_t *pfn)
{
    gs_device_n_map *pimap;

    if (gs_color_space_get_index(pcspace) != gs_color_space_index_Separation ||
	pfn->params.m != 1 || pfn->params.n !=
	  gs_color_space_num_components(pcspace->base_space)
	)
	return_error(gs_error_rangecheck);
    pimap = pcspace->params.separation.map;
    pimap->tint_transform = map_devn_using_function;
    pimap->tint_transform_data = pfn;
    pimap->cache_valid = false;
    return 0;
}

/*
 * If the Separation tint transformation procedure is a Function,
 * return the function object, otherwise return 0.
 */
gs_function_t *
gs_cspace_get_sepr_function(const gs_color_space *pcspace)
{
    if (gs_color_space_get_index(pcspace) == gs_color_space_index_Separation &&
	pcspace->params.separation.map->tint_transform ==
	  map_devn_using_function)
	return pcspace->params.separation.map->tint_transform_data;
    return 0;
}

/* ------ Internal procedures ------ */

/* Initialize a Separation color. */
static void
gx_init_Separation(gs_client_color * pcc, const gs_color_space * pcs)
{
    pcc->paint.values[0] = 1.0;
}

/* Remap a Separation color. */

static int
gx_remap_Separation(const gs_client_color * pcc, const gs_color_space * pcs,
	gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
		       gs_color_select_t select)
{
    int code = 0;

    if (pcs->params.separation.sep_type != SEP_NONE)
	code = gx_default_remap_color(pcc, pcs, pdc, pis, dev, select);
    else {
        color_set_null(pdc);
    }
    /* Save original color space and color info into dev color */
    pdc->ccolor.paint.values[0] = pcc->paint.values[0];
    pdc->ccolor_valid = true;
    return code;
}

static int
gx_concretize_Separation(const gs_client_color *pc, const gs_color_space *pcs,
			 frac *pconc, const gs_imager_state *pis)
{
    int code;
    gs_client_color cc;
    const gs_color_space *pacs = pcs->base_space;
    
    if (pcs->params.separation.sep_type == SEP_OTHER &&
	pcs->params.separation.use_alt_cspace) {
        gs_device_n_map *map = pcs->params.separation.map;

	/* Check the 1-element cache first. */
	if (map->cache_valid && map->tint[0] == pc->paint.values[0]) {
	    int i, num_out = gs_color_space_num_components(pacs);

	    for (i = 0; i < num_out; ++i)
		pconc[i] = map->conc[i];
	    return 0;
	}
        code = (*pcs->params.separation.map->tint_transform)
	    (pc->paint.values, &cc.paint.values[0],
	     pis, pcs->params.separation.map->tint_transform_data);
        if (code < 0)
	    return code;
	return cs_concretize_color(&cc, pacs, pconc, pis);
    }
    else {
    	pconc[0] = gx_unit_frac(pc->paint.values[0]);
    }
    return 0;
}

static int
gx_remap_concrete_Separation(const frac * pconc,  const gs_color_space * pcs,
	gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
			     gs_color_select_t select)
{
#ifdef DEBUG
    /* 
     * Verify that the color space and imager state info match.
     */
    if (pcs->id != pis->color_component_map.cspace_id)
	dprintf("gx_remap_concrete_Separation: color space id mismatch");
#endif

#if ENABLE_CUSTOM_COLOR_CALLBACK
    {
	int code = gx_remap_concrete_custom_color_Separation(pconc, pcs, pdc,
						       	pis, dev, select);
	if (code >= 0)
	    return code;
    }
#endif

    if (pis->color_component_map.use_alt_cspace) {
        const gs_color_space *pacs = pcs->base_space;

	return (*pacs->type->remap_concrete_color)
				(pconc, pacs, pdc, pis, dev, select);
    }
    else {
        gx_remap_concrete_separation(pconc[0], pdc, pis, dev, select);
        return 0;
    }
}

/*
 * Check that the color component name for a Separation color space
 * matches the device colorant names.  Also build a gs_devicen_color_map
 * structure.
 */
static int
check_Separation_component_name(const gs_color_space * pcs, gs_state * pgs)
{
    const gs_separation_name name = pcs->params.separation.sep_name;
    int colorant_number;
    byte * pname;
    uint name_size;
    gs_devicen_color_map * pcolor_component_map
	= &pgs->color_component_map;
    gx_device * dev = pgs->device;

    pcolor_component_map->num_components = 1;
    pcolor_component_map->cspace_id = pcs->id;
    pcolor_component_map->num_colorants = dev->color_info.num_components;
    pcolor_component_map->sep_type = pcs->params.separation.sep_type;
    /*
     * If this is a None or All separation then we do not need to
     * use the alternate color space.
     */
    if (pcs->params.separation.sep_type != SEP_OTHER) {
	pcolor_component_map->use_alt_cspace = false;
	return 0;
    }
    /*
     * Always use the alternate color space if the current device is
     * using an additive color model.  Separations are only for use
     * with a subtractive color model.
     */
    if (dev->color_info.polarity == GX_CINFO_POLARITY_ADDITIVE) {
	pcolor_component_map->use_alt_cspace = true;
	return 0;
    }
    /*
     * Get the character string and length for the component name.
     */
    pcs->params.separation.get_colorname_string(dev->memory, name, &pname, &name_size);
    /*
     * Compare the colorant name to the device's.  If the device's
     * compare routine returns GX_DEVICE_COLOR_MAX_COMPONENTS then the
     * colorant is in the SeparationNames list but not in the
     * SeparationOrder list.
     */
    colorant_number = (*dev_proc(dev, get_color_comp_index))
		(dev, (const char *)pname, name_size, SEPARATION_NAME);
    if (colorant_number >= 0) {		/* If valid colorant name */
	pcolor_component_map->color_map[0] =
		    (colorant_number == GX_DEVICE_COLOR_MAX_COMPONENTS) ? -1
		    					   : colorant_number;
	pcolor_component_map->use_alt_cspace = false;
    }
    else
	pcolor_component_map->use_alt_cspace = true;
    return 0;
}


/* ---------------- Notes on real Separation colors ---------------- */

typedef ulong gs_separation;	/* BOGUS */

#define gs_no_separation ((gs_separation)(-1L))

#define dev_proc_lookup_separation(proc)\
  gs_separation proc(gx_device *dev, const byte *sname, uint len,\
    gx_color_value *num_levels)

#define dev_proc_map_tint_color(proc)\
  gx_color_index proc(gx_device *dev, gs_separation sepr, bool overprint,\
    gx_color_value tint)

/*
 * This next comment is outdated since the Separation color space no longer
 * has the multi element cache (lookup table) however the remainder is
 * still appropriate.
 *
 * In principle, setting a Separation color space, or setting the device
 * when the current color space is a Separation space, calls the
 * lookup_separation device procedure to obtain the separation ID and
 * the number of achievable levels.  Currently, the only hooks for doing
 * this are unsuitable: gx_set_cmap_procs isn't called when the color
 * space changes, and doing it in gx_remap_Separation is inefficient.
 * Probably the best approach is to call gx_set_cmap_procs whenever the
 * color space changes.  In fact, if we do this, we can probably short-cut
 * two levels of procedure call in color remapping (gx_remap_color, by
 * turning it into a macro, and gx_remap_DeviceXXX, by calling the
 * cmap_proc procedure directly).  Some care will be required for the
 * implicit temporary resetting of the color space in [color]image.
 */

/* ---------------- Serialization. -------------------------------- */

static int 
gx_serialize_Separation(const gs_color_space * pcs, stream * s)
{
    const gs_separation_params * p = &pcs->params.separation;
    uint n;
    int code = gx_serialize_cspace_type(pcs, s);

    if (code < 0)
	return code;
    code = sputs(s, (const byte *)&p->sep_name, sizeof(p->sep_name), &n);
    if (code < 0)
	return code;
    code = cs_serialize(pcs->base_space, s);
    if (code < 0)
	return code;
    code = gx_serialize_device_n_map(pcs, p->map, s);
    if (code < 0)
	return code;
    return sputs(s, (const byte *)&p->sep_type, sizeof(p->sep_type), &n);
    /* p->use_alt_cspace isn't a property of the space. */
}
