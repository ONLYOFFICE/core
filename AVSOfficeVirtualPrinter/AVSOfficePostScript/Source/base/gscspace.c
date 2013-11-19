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

/* $Id: gscspace.c 9184 2008-10-25 04:34:30Z mvrhel $ */
/* Color space operators and support */
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsstruct.h"
#include "gsccolor.h"
#include "gsutil.h"		/* for gs_next_ids */
#include "gxcmap.h"
#include "gxcspace.h"
#include "gxistate.h"
#include "gsovrc.h"
#include "gsstate.h"
#include "gsdevice.h"
#include "gxdevcli.h"
#include "gzstate.h"
#include "stream.h"
#include "gsnamecl.h"  /* Custom color call back define */

static cs_proc_install_cspace(gx_install_DeviceGray);
static cs_proc_install_cspace(gx_install_DeviceRGB);
static cs_proc_install_cspace(gx_install_DeviceCMYK);
/*
 * Define the standard color space types.  We include DeviceCMYK in the base
 * build because it's too awkward to omit it, but we don't provide any of
 * the PostScript operator procedures (setcmykcolor, etc.) for dealing with
 * it.
 */
static const gs_color_space_type gs_color_space_type_DeviceGray = {
    gs_color_space_index_DeviceGray, true, true,
    &st_base_color_space, gx_num_components_1,
    gx_init_paint_1, gx_restrict01_paint_1,
    gx_same_concrete_space,
    gx_concretize_DeviceGray, gx_remap_concrete_DGray,
    gx_remap_DeviceGray, gx_install_DeviceGray,
    gx_spot_colors_set_overprint,
    NULL, gx_no_adjust_color_count,
    gx_serialize_cspace_type,
    gx_cspace_is_linear_default
};
static const gs_color_space_type gs_color_space_type_DeviceRGB = {
    gs_color_space_index_DeviceRGB, true, true,
    &st_base_color_space, gx_num_components_3,
    gx_init_paint_3, gx_restrict01_paint_3,
    gx_same_concrete_space,
    gx_concretize_DeviceRGB, gx_remap_concrete_DRGB,
    gx_remap_DeviceRGB, gx_install_DeviceRGB,
    gx_spot_colors_set_overprint,
    NULL, gx_no_adjust_color_count,
    gx_serialize_cspace_type,
    gx_cspace_is_linear_default
};

static cs_proc_set_overprint(gx_set_overprint_DeviceCMYK);

static const gs_color_space_type gs_color_space_type_DeviceCMYK = {
    gs_color_space_index_DeviceCMYK, true, true,
    &st_base_color_space, gx_num_components_4,
    gx_init_paint_4, gx_restrict01_paint_4,
    gx_same_concrete_space,
    gx_concretize_DeviceCMYK, gx_remap_concrete_DCMYK,
    gx_remap_DeviceCMYK, gx_install_DeviceCMYK,
    gx_set_overprint_DeviceCMYK,
    NULL, gx_no_adjust_color_count,
    gx_serialize_cspace_type,
    gx_cspace_is_linear_default
};

/* Structure descriptors */
public_st_color_space();
public_st_base_color_space();

/* ------ Create/copy/destroy ------ */


static void
gs_cspace_final(void *vptr)
{
    gs_color_space *pcs = (gs_color_space *)vptr;

    if (pcs->type->final)
	pcs->type->final(pcs);
    if_debug2('c', "[c]cspace final %08x %d\n", pcs, pcs->id);
    
#if ENABLE_CUSTOM_COLOR_CALLBACK
    {
	client_color_space_data_t *pclient_data = pcs->pclient_color_space_data;
	if ( pclient_data )
		pclient_data->client_adjust_cspace_count( pcs, -1 );
    }
#endif /*ENABLE_CUSTOM_COLOR_CALLBACK*/
    
    rc_decrement_only(pcs->base_space, "gs_cspace_final");
}

static gs_color_space *
gs_cspace_alloc_with_id(gs_memory_t *mem, ulong id,
		   const gs_color_space_type *pcstype)
{
    gs_color_space *pcs;

    rc_alloc_struct_1(pcs, gs_color_space, &st_color_space, mem, return NULL,
		      "gs_cspace_alloc_with_id");
    if_debug3('c', "[c]cspace alloc %08x %s %d\n",
	      pcs, pcstype->stype->sname, pcstype->index);
    pcs->type = pcstype;
    pcs->id = id;
    pcs->base_space = NULL;
    pcs->pclient_color_space_data = NULL;
    return pcs;
}

static cs_proc_install_cspace(gx_install_DeviceGray);
static cs_proc_install_cspace(gx_install_DeviceRGB);
static cs_proc_install_cspace(gx_install_DeviceCMYK);

/*
 * Generic allocation function for colorspace implementations. Return
 * NULL on allocation failure.
 */
gs_color_space *
gs_cspace_alloc(gs_memory_t *mem, const gs_color_space_type *pcstype)
{
    return gs_cspace_alloc_with_id(mem, gs_next_ids(mem, 1), pcstype);
}

/* Constructors for simple device color spaces. */

gs_color_space *
gs_cspace_new_DeviceGray(gs_memory_t *mem)
{
    return gs_cspace_alloc_with_id(mem, cs_DeviceGray_id,
				   &gs_color_space_type_DeviceGray);
}

gs_color_space *
gs_cspace_new_DeviceRGB(gs_memory_t *mem)
{
    return gs_cspace_alloc_with_id(mem, cs_DeviceRGB_id,
				   &gs_color_space_type_DeviceRGB);
}
gs_color_space *
gs_cspace_new_DeviceCMYK(gs_memory_t *mem)
{
    return gs_cspace_alloc_with_id(mem, cs_DeviceCMYK_id,
				   &gs_color_space_type_DeviceCMYK);
}


/* ------ Accessors ------ */

/* Get the index of a color space. */
gs_color_space_index
gs_color_space_get_index(const gs_color_space * pcs)
{
    return pcs->type->index;
}

/* See if the space is CIE based */
bool gs_color_space_is_CIE(const gs_color_space * pcs)
{

    switch(gs_color_space_get_index(pcs)){

        case gs_color_space_index_CIEDEFG:
        case gs_color_space_index_CIEDEF:
        case gs_color_space_index_CIEABC:
        case gs_color_space_index_CIEA:
        case gs_color_space_index_CIEICC:

            return true;

        break;

        default:
            
            return false;

    }

}


/* Get the number of components in a color space. */
int
gs_color_space_num_components(const gs_color_space * pcs)
{
    return cs_num_components(pcs);
}

/* Restrict a color to its legal range. */
void
gs_color_space_restrict_color(gs_client_color *pcc, const gs_color_space *pcs)
{
    cs_restrict_color(pcc, pcs);
}

/* Install a DeviceGray color space. */
static int
gx_install_DeviceGray(gs_color_space * pcs, gs_state * pgs)
{
#if ENABLE_CUSTOM_COLOR_CALLBACK
    /*
     * Check if we want to use the callback color processing for this
     * color space.
     */
    client_custom_color_params_t * pcb =
	(client_custom_color_params_t *) pgs->memory->gs_lib_ctx->custom_color_callback;

    if (pcb != NULL) 
	pcb->client_procs->install_DeviceGray(pcb, pcs, pgs);
#endif
    return 0;
}

int
gx_num_components_1(const gs_color_space * pcs)
{
    return 1;
}
int
gx_num_components_3(const gs_color_space * pcs)
{
    return 3;
}
int
gx_num_components_4(const gs_color_space * pcs)
{
    return 4;
}

/*
 * For color spaces that have a base or alternative color space, return that
 * color space. Otherwise return null.
 */
const gs_color_space *
gs_cspace_base_space(const gs_color_space * pcspace)
{
    return pcspace->base_space;
}

/* ------ Other implementation procedures ------ */

/* Null color space installation procedure. */
int
gx_no_install_cspace(gs_color_space * pcs, gs_state * pgs)
{
    return 0;
}
  
/* Install a DeviceRGB color space. */
static int
gx_install_DeviceRGB(gs_color_space * pcs, gs_state * pgs)
{
#if ENABLE_CUSTOM_COLOR_CALLBACK
    /*
     * Check if we want to use the callback color processing for this
     * color space.
     */
    client_custom_color_params_t * pcb =
	(client_custom_color_params_t *) pgs->memory->gs_lib_ctx->custom_color_callback;

    if (pcb != NULL) 
	pcb->client_procs->install_DeviceRGB(pcb, pcs, pgs);
#endif
    return 0;
}

/* Install a DeviceCMYK color space. */
static int
gx_install_DeviceCMYK(gs_color_space * pcs, gs_state * pgs)
{
#if ENABLE_CUSTOM_COLOR_CALLBACK
    /*
     * Check if we want to use the callback color processing for this
     * color space.
     */
    client_custom_color_params_t * pcb =
	(client_custom_color_params_t *) pgs->memory->gs_lib_ctx->custom_color_callback;

    if (pcb != NULL) 
	pcb->client_procs->install_DeviceCMYK(pcb, pcs, pgs);
#endif
    return 0;
}

/*
 * Push an overprint compositor onto the current device indicating that,
 * at most, the spot color parameters are to be preserved.
 *
 * This routine should be used for all Device, CIEBased, and ICCBased
 * color spaces, except for DeviceCMKY. The latter color space requires a
 * special verson that supports overprint mode.
 */
int
gx_spot_colors_set_overprint(const gs_color_space * pcs, gs_state * pgs)
{
    gs_imager_state *       pis = (gs_imager_state *)pgs;
    gs_overprint_params_t   params;

    if ((params.retain_any_comps = pis->overprint))
        params.retain_spot_comps = true;
    pgs->effective_overprint_mode = 0;
    return gs_state_update_overprint(pgs, &params);
}


static bool
check_single_comp(int comp, frac targ_val, int ncomps, const frac * pval)
{
    int     i;

    for (i = 0; i < ncomps; i++) {
        if ( (i != comp && pval[i] != frac_0)  ||
             (i == comp && pval[i] != targ_val)  )
            return false;
    }
    return true;
}

/*
 * Determine if the current color model is a "DeviceCMYK" color model, and
 * if so what are its process color components. This information is required
 * when PLRM defines special rules for CMYK devices. This includes:
 * 1. DeviceGray to CMYK color conversion
 * 2. when overprint is true and overprint mode is set to 1.
 *
 * A color model is considered a "DeviceCMYK" color model if it supports the
 * cyan, magenta, yellow, and black color components, and maps the DeviceCMYK
 * color model components directly to these color components. Note that this
 * does not require any particular component order, allows for additional
 * spot color components, and does admit DeviceN color spaces if they have
 * the requisite behavior.
 *
 * If the color model is a "DeviceCMYK" color model, return the set of
 * process color components; otherwise return 0.
 */
gx_color_index
check_cmyk_color_model_comps(gx_device * dev)
{
    gx_device_color_info *          pcinfo = &dev->color_info;
    int                             ncomps = pcinfo->num_components;
    int                             cyan_c, magenta_c, yellow_c, black_c;
    const gx_cm_color_map_procs *   pprocs;
    cm_map_proc_cmyk((*map_cmyk));
    frac                            frac_14 = frac_1 / 4;
    frac                            out[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_index                  process_comps;

    /* check for the appropriate components */
    if ( ncomps < 4                                       ||
         (cyan_c = dev_proc(dev, get_color_comp_index)(
                       dev,
                       "Cyan",
                       sizeof("Cyan") - 1,
                       NO_COMP_NAME_TYPE )) < 0           ||
         cyan_c == GX_DEVICE_COLOR_MAX_COMPONENTS         ||
         (magenta_c = dev_proc(dev, get_color_comp_index)(
                          dev,
                          "Magenta",
                          sizeof("Magenta") - 1,
                          NO_COMP_NAME_TYPE )) < 0        ||
         magenta_c == GX_DEVICE_COLOR_MAX_COMPONENTS      ||
         (yellow_c = dev_proc(dev, get_color_comp_index)(
                        dev,
                        "Yellow",
                        sizeof("Yellow") - 1,
                        NO_COMP_NAME_TYPE )) < 0               ||
         yellow_c == GX_DEVICE_COLOR_MAX_COMPONENTS       ||
         (black_c = dev_proc(dev, get_color_comp_index)(
                        dev,
                        "Black",
                        sizeof("Black") - 1,
                        NO_COMP_NAME_TYPE )) < 0                         ||
         black_c == GX_DEVICE_COLOR_MAX_COMPONENTS          )
        return 0;

    /* check the mapping */
    if ( (pprocs = dev_proc(dev, get_color_mapping_procs)(dev)) == 0 ||
         (map_cmyk = pprocs->map_cmyk) == 0                            )
        return 0;

    map_cmyk(dev, frac_14, frac_0, frac_0, frac_0, out);
    if (!check_single_comp(cyan_c, frac_14, ncomps, out))
        return 0;
    map_cmyk(dev, frac_0, frac_14, frac_0, frac_0, out);
    if (!check_single_comp(magenta_c, frac_14, ncomps, out))
        return 0;
    map_cmyk(dev, frac_0, frac_0, frac_14, frac_0, out);
    if (!check_single_comp(yellow_c, frac_14, ncomps, out))
        return false;
    map_cmyk(dev, frac_0, frac_0, frac_0, frac_14, out);
    if (!check_single_comp(black_c, frac_14, ncomps, out))
        return 0;

    process_comps =  ((gx_color_index)1 << cyan_c)
                   | ((gx_color_index)1 << magenta_c)
                   | ((gx_color_index)1 << yellow_c)
                   | ((gx_color_index)1 << black_c);
    pcinfo->opmode = GX_CINFO_OPMODE;
    pcinfo->process_comps = process_comps;
    pcinfo->black_component = black_c;
    return process_comps;
}

/*
 * This set_overprint method is unique. If overprint is true, overprint
 * mode is set to 1, the process color model has DeviceCMYK behavior (see
 * the comment ahead of gx_is_cmyk_color_model above), and the device
 * color is set, the device color needs to be considered in setting up
 * the set of drawn components.
 */
static int
gx_set_overprint_DeviceCMYK(const gs_color_space * pcs, gs_state * pgs)
{
    gx_device *             dev = pgs->device;
    gx_device_color_info *  pcinfo = (dev == 0 ? 0 : &dev->color_info);
    gx_color_index          drawn_comps = 0;
    gs_overprint_params_t   params;

    /* check if we require special handling */
    if ( !pgs->overprint                      ||
         pgs->overprint_mode != 1             ||
         pcinfo == 0                          ||
         pcinfo->opmode == GX_CINFO_OPMODE_NOT  )
        return gx_spot_colors_set_overprint(pcs, pgs);

    /* check if color model behavior must be determined */
    if (pcinfo->opmode == GX_CINFO_OPMODE_UNKNOWN)
        drawn_comps = check_cmyk_color_model_comps(dev);
    else
        drawn_comps = pcinfo->process_comps;
    if (drawn_comps == 0)
        return gx_spot_colors_set_overprint(pcs, pgs);

    /* correct for any zero'ed color components */
    pgs->effective_overprint_mode = 1;
    if (color_is_set(pgs->dev_color)) {
        gx_color_index  nz_comps;
        int             code;
        dev_color_proc_get_nonzero_comps((*procp));

        procp = pgs->dev_color->type->get_nonzero_comps;
        if ((code = procp(pgs->dev_color, dev, &nz_comps)) < 0)
            return code;
        drawn_comps &= nz_comps;
    }

    params.retain_any_comps = true;
    params.retain_spot_comps = false;
    params.drawn_comps = drawn_comps;
    return gs_state_update_overprint(pgs, &params);
}


/* A stub for a color mapping linearity check, when it is inapplicable. */
int
gx_cspace_no_linear(const gs_color_space *cs, const gs_imager_state * pis,
		gx_device * dev, 
		const gs_client_color *c0, const gs_client_color *c1,
		const gs_client_color *c2, const gs_client_color *c3,
		float smoothness)
{
    return_error(gs_error_rangecheck);
}

static inline int
cc2dc(const gs_color_space *cs, const gs_imager_state * pis, gx_device *dev,
	    gx_device_color *dc, const gs_client_color *cc)
{
    return cs->type->remap_color(cc, cs, dc, pis, dev, gs_color_select_texture);
}

static inline void
interpolate_cc(gs_client_color *c, 
	const gs_client_color *c0, const gs_client_color *c1, double t, int n)
{
    int i;

    for (i = 0; i < n; i++)
	c->paint.values[i] = c0->paint.values[i] * t + c1->paint.values[i] * (1 - t);
}

static inline bool
is_dc_nearly_linear(const gx_device *dev, const gx_device_color *c, 
	const gx_device_color *c0, const gx_device_color *c1, 
	double t, int n, float smoothness)
{

    if (c0->type == &gx_dc_type_data_pure) {
	int i;
	gx_color_index pure0 = c0->colors.pure;
	gx_color_index pure1 = c1->colors.pure;
	gx_color_index pure = c->colors.pure;

	for (i = 0; i < n; i++) {
	    int shift = dev->color_info.comp_shift[i];
	    int mask = (1 << dev->color_info.comp_bits[i]) - 1;
	    int max_color = (i == dev->color_info.gray_index ? dev->color_info.max_gray 
							     : dev->color_info.max_color);
	    float max_diff = max(1, max_color * smoothness);
	    int b0 = (pure0 >> shift) & mask, b1 = (pure1 >> shift) & mask; 
	    int b = (pure >> shift) & mask;
	    double bb = b0 * t + b1 * (1 - t);

	    if (any_abs(b - bb) > max_diff)
		return false;
	}
	return true;
    } else {
	/* Halftones must not paint with fill_linear_color_*. */
	return false;
    }
}

/* Default color mapping linearity check, a 2-points case. */
static int
gx_cspace_is_linear_in_line(const gs_color_space *cs, const gs_imager_state * pis,
		gx_device *dev, 
		const gs_client_color *c0, const gs_client_color *c1,
		float smoothness)
{
    gs_client_color c01a, c01b;
    gx_device_color d[2], d01a, d01b;
    int n = cs->type->num_components(cs);
    int ndev = dev->color_info.num_components;
    int code;

    code = cc2dc(cs, pis, dev, &d[0], c0);
    if (code < 0)
	return code;
    code = cc2dc(cs, pis, dev, &d[1], c1);
    if (code < 0)
	return code;
    interpolate_cc(&c01a, c0, c1, 0.3, n);
    code = cc2dc(cs, pis, dev, &d01a, &c01a);
    if (code < 0)
	return code;
    if (!is_dc_nearly_linear(dev, &d01a, &d[0], &d[1], 0.3, ndev, smoothness))
	return 0;
    interpolate_cc(&c01b, c0, c1, 0.7, n);
    code = cc2dc(cs, pis, dev, &d01b, &c01b);
    if (code < 0)
	return code;
    if (!is_dc_nearly_linear(dev, &d01b, &d[0], &d[1], 0.7, ndev, smoothness))
	return 0;
    return 1;
}

/* Default color mapping linearity check, a triangle case. */
static int
gx_cspace_is_linear_in_triangle(const gs_color_space *cs, const gs_imager_state * pis,
		gx_device *dev, 
		const gs_client_color *c0, const gs_client_color *c1,
		const gs_client_color *c2, float smoothness)
{
    /* We check 4 points - the median center, and middle points of 3 sides. 
       Hopely this is enough for reasonable color spaces and color renderings. 
       Note it gives 7 points for a quadrangle. */
    gs_client_color c01, c12, c20, c012;
    gx_device_color d[3], d01, d12, d20, d012;

    /* Note that the device and the client color space
       can have a different number of components */

    int n = cs->type->num_components(cs);
    int ndev = dev->color_info.num_components;

    int code;

    code = cc2dc(cs, pis, dev, &d[0], c0);
    if (code < 0)
	return code;
    code = cc2dc(cs, pis, dev, &d[1], c1);
    if (code < 0)
	return code;
    code = cc2dc(cs, pis, dev, &d[2], c2);
    if (code < 0)
	return code;

    interpolate_cc(&c01, c0, c1, 0.5, n);
    code = cc2dc(cs, pis, dev, &d01, &c01);
    if (code < 0)
	return code;
    if (!is_dc_nearly_linear(dev, &d01, &d[0], &d[1], 0.5, ndev, smoothness))
	return 0;

    interpolate_cc(&c012, c2, &c01, 2.0 / 3, n);
    code = cc2dc(cs, pis, dev, &d012, &c012);
    if (code < 0)
	return code;
    if (!is_dc_nearly_linear(dev, &d012, &d[2], &d01, 2.0 / 3, ndev, smoothness))
	return 0;

    interpolate_cc(&c12, c1, c2, 0.5, n);
    code = cc2dc(cs, pis, dev, &d12, &c12);
    if (code < 0)
	return code;
    if (!is_dc_nearly_linear(dev, &d12, &d[1], &d[2], 0.5, ndev, smoothness))
	return 0;

    interpolate_cc(&c20, c2, c0, 0.5, n);
    code = cc2dc(cs, pis, dev, &d20, &c20);
    if (code < 0)
	return code;
    if (!is_dc_nearly_linear(dev, &d20, &d[2], &d[0], 0.5, ndev, smoothness))
	return 0;
    return 1;
}

/* Default color mapping linearity check. */
int
gx_cspace_is_linear_default(const gs_color_space *cs, const gs_imager_state * pis,
		gx_device *dev, 
		const gs_client_color *c0, const gs_client_color *c1,
		const gs_client_color *c2, const gs_client_color *c3,
		float smoothness)
{
    /* Assuming 2 <= nc <= 4. We don't need other cases. */
    /* With nc == 4 assuming a convex plain quadrangle in the client color space. */
    int code;

    if (dev->color_info.separable_and_linear != GX_CINFO_SEP_LIN)
	return_error(gs_error_rangecheck);
    if (c2 == NULL)
	return gx_cspace_is_linear_in_line(cs, pis, dev, c0, c1, smoothness);
    code = gx_cspace_is_linear_in_triangle(cs, pis, dev, c0, c1, c2, smoothness);
    if (code <= 0)
	return code;
    if (c3 == NULL)
	return 1;
    return gx_cspace_is_linear_in_triangle(cs, pis, dev, c1, c2, c3, smoothness);
}

/* Serialization. */
int 
gx_serialize_cspace_type(const gs_color_space * pcs, stream * s)
{
    const gs_color_space_type * type = pcs->type;
    uint n;
    return sputs(s, (const byte *)&type->index, sizeof(type->index), &n);
}

/* GC procedures */

static 
ENUM_PTRS_BEGIN_PROC(color_space_enum_ptrs)
{
    EV_CONST gs_color_space *pcs = vptr;

    if (index == 0)
	return ENUM_OBJ(pcs->base_space);
    if (index == 1)
	return ENUM_OBJ(pcs->pclient_color_space_data);
    return ENUM_USING(*pcs->type->stype, vptr, size, index - 2);
    ENUM_PTRS_END_PROC
}
static 
RELOC_PTRS_WITH(color_space_reloc_ptrs, gs_color_space *pcs)
{
    RELOC_VAR(pcs->base_space);
    RELOC_VAR(pcs->pclient_color_space_data);
    RELOC_USING(*pcs->type->stype, vptr, size);
}
RELOC_PTRS_END
