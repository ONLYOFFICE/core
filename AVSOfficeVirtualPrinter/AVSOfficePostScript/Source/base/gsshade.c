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

/* $Id: gsshade.c 8250 2007-09-25 13:31:24Z giles $ */
/* Constructors for shadings */
#include "gx.h"
#include "gscspace.h"
#include "gserrors.h"
#include "gsstruct.h"
#include "gsptype2.h"
#include "gxdevcli.h"
#include "gxcpath.h"
#include "gxcspace.h"
#include "gxdcolor.h"		/* for filling background rectangle */
#include "gxistate.h"
#include "gxpaint.h"
#include "gxpath.h"
#include "gxshade.h"
#include "gxshade4.h"
#include "gzpath.h"
#include "gzcpath.h"

/* ================ Initialize shadings ================ */

/* ---------------- Generic services ---------------- */

/* GC descriptors */
private_st_shading();
private_st_shading_mesh();

static
ENUM_PTRS_WITH(shading_mesh_enum_ptrs, gs_shading_mesh_t *psm)
{
    index -= 2;
    if (index < st_data_source_max_ptrs)
	return ENUM_USING(st_data_source, &psm->params.DataSource,
			  sizeof(psm->params.DataSource), index);
    return ENUM_USING_PREFIX(st_shading, st_data_source_max_ptrs);
}
ENUM_PTR2(0, gs_shading_mesh_t, params.Function, params.Decode);
ENUM_PTRS_END

static
RELOC_PTRS_WITH(shading_mesh_reloc_ptrs, gs_shading_mesh_t *psm)
{
    RELOC_PREFIX(st_shading);
    RELOC_USING(st_data_source, &psm->params.DataSource,
		sizeof(psm->params.DataSource));
    RELOC_PTR2(gs_shading_mesh_t, params.Function, params.Decode);
}
RELOC_PTRS_END

/* Check ColorSpace, BBox, and Function (if present). */
/* Free variables: params. */
static int
check_CBFD(const gs_shading_params_t * params,
	   const gs_function_t * function, const float *domain, int m)
{
    int ncomp = gs_color_space_num_components(params->ColorSpace);

    if (ncomp < 0 ||
	(params->have_BBox &&
	 (params->BBox.p.x > params->BBox.q.x ||
	  params->BBox.p.y > params->BBox.q.y))
	)
	return_error(gs_error_rangecheck);
    if (function != 0) {
	if (function->params.m != m || function->params.n != ncomp)
	    return_error(gs_error_rangecheck);
	/*
	 * The Adobe documentation says that the function's domain must
	 * be a superset of the domain defined in the shading dictionary.
	 * However, Adobe implementations apparently don't necessarily
	 * check this ahead of time; therefore, we do the same.
	 */
    }
    return 0;
}

/* Check parameters for a mesh shading. */
static int
check_mesh(const gs_shading_mesh_params_t * params)
{
    const float *domain;

    if (data_source_is_array(params->DataSource))
	domain = 0;
    else {
	domain = params->Decode;
	switch (params->BitsPerCoordinate) {
	    case  1: case  2: case  4: case  8:
	    case 12: case 16: case 24: case 32:
		break;
	    default:
		return_error(gs_error_rangecheck);
	}
	switch (params->BitsPerComponent) {
	    case  1: case  2: case  4: case  8:
	    case 12: case 16:
		break;
	    default:
		return_error(gs_error_rangecheck);
	}
    }
    return check_CBFD((const gs_shading_params_t *)params,
		      params->Function, domain, 1);
}

/* Check the BitsPerFlag value.  Return the value or an error code. */
static int
check_BPF(const gs_data_source_t *pds, int bpf)
{
    if (data_source_is_array(*pds))
	return 2;
    switch (bpf) {
    case 2: case 4: case 8:
	return bpf;
    default:
	return_error(gs_error_rangecheck);
    }
}

/* Initialize common shading parameters. */
static void
shading_params_init(gs_shading_params_t *params)
{
    params->ColorSpace = 0;	/* must be set by client */
    params->cie_joint_caches = 0;
    params->Background = 0;
    params->have_BBox = false;
    params->AntiAlias = false;
}

/* Initialize common mesh shading parameters. */
static void
mesh_shading_params_init(gs_shading_mesh_params_t *params)
{
    shading_params_init((gs_shading_params_t *)params);
    data_source_init_floats(&params->DataSource, NULL, 0);/* client must set */
    /* Client must set BitsPerCoordinate and BitsPerComponent */
    /* if DataSource is not an array. */
    params->Decode = 0;
    params->Function = 0;
}

/* Allocate and initialize a shading. */
/* Free variables: mem, params, ppsh, psh. */
#define ALLOC_SHADING(sttype, stype, sprocs, cname)\
  BEGIN\
    psh = gs_alloc_struct(mem, void, sttype, cname);\
    if ( psh == 0 )\
      return_error(gs_error_VMerror);\
    psh->head.type = stype;\
    psh->head.procs = sprocs;\
    psh->params = *params;\
    *ppsh = (gs_shading_t *)psh;\
  END

/* ---------------- Function-based shading ---------------- */

private_st_shading_Fb();

/* Initialize parameters for a Function-based shading. */
void
gs_shading_Fb_params_init(gs_shading_Fb_params_t * params)
{
    shading_params_init((gs_shading_params_t *)params);
    params->Domain[0] = params->Domain[2] = 0;
    params->Domain[1] = params->Domain[3] = 1;
    gs_make_identity(&params->Matrix);
    params->Function = 0;	/* must be set by client */
}

/* Allocate and initialize a Function-based shading. */
static const gs_shading_procs_t shading_Fb_procs = {
    gs_shading_Fb_fill_rectangle
};
int
gs_shading_Fb_init(gs_shading_t ** ppsh,
		   const gs_shading_Fb_params_t * params, gs_memory_t * mem)
{
    gs_shading_Fb_t *psh;
    gs_matrix imat;
    int code = check_CBFD((const gs_shading_params_t *)params,
			  params->Function, params->Domain, 2);

    if (code < 0 ||
	(code = gs_matrix_invert(&params->Matrix, &imat)) < 0
	)
	return code;
    ALLOC_SHADING(&st_shading_Fb, shading_type_Function_based,
		  shading_Fb_procs, "gs_shading_Fb_init");
    return 0;
}

/* ---------------- Axial shading ---------------- */

private_st_shading_A();

/* Initialize parameters for an Axial shading. */
void
gs_shading_A_params_init(gs_shading_A_params_t * params)
{
    shading_params_init((gs_shading_params_t *)params);
    /* Coords must be set by client */
    params->Domain[0] = 0;
    params->Domain[1] = 1;
    params->Function = 0;	/* must be set by client */
    params->Extend[0] = params->Extend[1] = false;
}

/* Allocate and initialize an Axial shading. */
static const gs_shading_procs_t shading_A_procs = {
    gs_shading_A_fill_rectangle
};
int
gs_shading_A_init(gs_shading_t ** ppsh,
		  const gs_shading_A_params_t * params, gs_memory_t * mem)
{
    gs_shading_A_t *psh;
    int code = check_CBFD((const gs_shading_params_t *)params,
			  params->Function, params->Domain, 1);

    if (code < 0)
	return code;
    ALLOC_SHADING(&st_shading_A, shading_type_Axial,
		  shading_A_procs, "gs_shading_A_init");
    return 0;
}

/* ---------------- Radial shading ---------------- */

private_st_shading_R();

/* Initialize parameters for a Radial shading. */
void
gs_shading_R_params_init(gs_shading_R_params_t * params)
{
    shading_params_init((gs_shading_params_t *)params);
    /* Coords must be set by client */
    params->Domain[0] = 0;
    params->Domain[1] = 1;
    params->Function = 0;	/* must be set by client */
    params->Extend[0] = params->Extend[1] = false;
}

/* Allocate and initialize a Radial shading. */
static const gs_shading_procs_t shading_R_procs = {
    gs_shading_R_fill_rectangle
};
int
gs_shading_R_init(gs_shading_t ** ppsh,
		  const gs_shading_R_params_t * params, gs_memory_t * mem)
{
    gs_shading_R_t *psh;
    int code = check_CBFD((const gs_shading_params_t *)params,
			  params->Function, params->Domain, 1);

    if (code < 0)
	return code;
    if ((params->Domain != 0 && params->Domain[0] == params->Domain[1]) ||
	params->Coords[2] < 0 || params->Coords[5] < 0
	)
	return_error(gs_error_rangecheck);
    ALLOC_SHADING(&st_shading_R, shading_type_Radial,
		  shading_R_procs, "gs_shading_R_init");
    return 0;
}

/* ---------------- Free-form Gouraud triangle mesh shading ---------------- */

private_st_shading_FfGt();

/* Initialize parameters for a Free-form Gouraud triangle mesh shading. */
void
gs_shading_FfGt_params_init(gs_shading_FfGt_params_t * params)
{
    mesh_shading_params_init((gs_shading_mesh_params_t *)params);
    /* Client must set BitsPerFlag if DataSource is not an array. */
}

/* Allocate and initialize a Free-form Gouraud triangle mesh shading. */
static const gs_shading_procs_t shading_FfGt_procs = {
    gs_shading_FfGt_fill_rectangle
};
int
gs_shading_FfGt_init(gs_shading_t ** ppsh,
		     const gs_shading_FfGt_params_t * params,
		     gs_memory_t * mem)
{
    gs_shading_FfGt_t *psh;
    int code = check_mesh((const gs_shading_mesh_params_t *)params);
    int bpf = check_BPF(&params->DataSource, params->BitsPerFlag);

    if (code < 0)
	return code;
    if (bpf < 0)
	return bpf;
    if (params->Decode != 0 && params->Decode[0] == params->Decode[1])
	return_error(gs_error_rangecheck);
    ALLOC_SHADING(&st_shading_FfGt, shading_type_Free_form_Gouraud_triangle,
		  shading_FfGt_procs, "gs_shading_FfGt_init");
    psh->params.BitsPerFlag = bpf;
    return 0;
}

/* -------------- Lattice-form Gouraud triangle mesh shading -------------- */

private_st_shading_LfGt();

/* Initialize parameters for a Lattice-form Gouraud triangle mesh shading. */
void
gs_shading_LfGt_params_init(gs_shading_LfGt_params_t * params)
{
    mesh_shading_params_init((gs_shading_mesh_params_t *)params);
    /* Client must set VerticesPerRow. */
}

/* Allocate and initialize a Lattice-form Gouraud triangle mesh shading. */
static const gs_shading_procs_t shading_LfGt_procs = {
    gs_shading_LfGt_fill_rectangle
};
int
gs_shading_LfGt_init(gs_shading_t ** ppsh,
		 const gs_shading_LfGt_params_t * params, gs_memory_t * mem)
{
    gs_shading_LfGt_t *psh;
    int code = check_mesh((const gs_shading_mesh_params_t *)params);

    if (code < 0)
	return code;
    if (params->VerticesPerRow < 2)
	return_error(gs_error_rangecheck);
    ALLOC_SHADING(&st_shading_LfGt, shading_type_Lattice_form_Gouraud_triangle,
		  shading_LfGt_procs, "gs_shading_LfGt_init");
    return 0;
}

/* ---------------- Coons patch mesh shading ---------------- */

private_st_shading_Cp();

/* Initialize parameters for a Coons patch mesh shading. */
void
gs_shading_Cp_params_init(gs_shading_Cp_params_t * params)
{
    mesh_shading_params_init((gs_shading_mesh_params_t *)params);
    /* Client must set BitsPerFlag if DataSource is not an array. */
}

/* Allocate and initialize a Coons patch mesh shading. */
static const gs_shading_procs_t shading_Cp_procs = {
    gs_shading_Cp_fill_rectangle
};
int
gs_shading_Cp_init(gs_shading_t ** ppsh,
		   const gs_shading_Cp_params_t * params, gs_memory_t * mem)
{
    gs_shading_Cp_t *psh;
    int code = check_mesh((const gs_shading_mesh_params_t *)params);
    int bpf = check_BPF(&params->DataSource, params->BitsPerFlag);

    if (code < 0)
	return code;
    if (bpf < 0)
	return bpf;
    ALLOC_SHADING(&st_shading_Cp, shading_type_Coons_patch,
		  shading_Cp_procs, "gs_shading_Cp_init");
    psh->params.BitsPerFlag = bpf;
    return 0;
}

/* ---------------- Tensor product patch mesh shading ---------------- */

private_st_shading_Tpp();

/* Initialize parameters for a Tensor product patch mesh shading. */
void
gs_shading_Tpp_params_init(gs_shading_Tpp_params_t * params)
{
    mesh_shading_params_init((gs_shading_mesh_params_t *)params);
    /* Client must set BitsPerFlag if DataSource is not an array. */
}

/* Allocate and initialize a Tensor product patch mesh shading. */
static const gs_shading_procs_t shading_Tpp_procs = {
    gs_shading_Tpp_fill_rectangle
};
int
gs_shading_Tpp_init(gs_shading_t ** ppsh,
		  const gs_shading_Tpp_params_t * params, gs_memory_t * mem)
{
    gs_shading_Tpp_t *psh;
    int code = check_mesh((const gs_shading_mesh_params_t *)params);
    int bpf = check_BPF(&params->DataSource, params->BitsPerFlag);

    if (code < 0)
	return code;
    if (bpf < 0)
	return bpf;
    ALLOC_SHADING(&st_shading_Tpp, shading_type_Tensor_product_patch,
		  shading_Tpp_procs, "gs_shading_Tpp_init");
    psh->params.BitsPerFlag = bpf;
    return 0;
}

/* ================ Shading rendering ================ */

/* Add a user-space rectangle to a path. */
int
gs_shading_path_add_box(gx_path *ppath, const gs_rect *pbox,
		     const gs_matrix_fixed *pmat)
{
    gs_fixed_point pt;
    gs_fixed_point pts[3];
    int code;

    if ((code = gs_point_transform2fixed(pmat, pbox->p.x, pbox->p.y,
					 &pt)) < 0 ||
	(code = gx_path_add_point(ppath, pt.x, pt.y)) < 0 ||
	(code = gs_point_transform2fixed(pmat, pbox->q.x, pbox->p.y,
					 &pts[0])) < 0 ||
	(code = gs_point_transform2fixed(pmat, pbox->q.x, pbox->q.y,
					 &pts[1])) < 0 ||
	(code = gs_point_transform2fixed(pmat, pbox->p.x, pbox->q.y,
					 &pts[2])) < 0 ||
	(code = gx_path_add_lines(ppath, pts, 3)) < 0
	)
	DO_NOTHING;
    return code;
}

/* Fill a path with a shading. */
int
gs_shading_do_fill_rectangle(const gs_shading_t *psh,
		     const gs_fixed_rect *prect, gx_device *dev,
		     gs_imager_state *pis, bool fill_background)
{   /* If you need to fill a path, clip the output device before calling this function. */
    const gs_matrix_fixed *pmat = &pis->ctm;
    gs_fixed_rect path_box;
    gs_rect path_rect;
    gs_rect rect;
    int code = 0;

    dev_proc(dev, get_clipping_box)(dev, &path_box);
    if (prect)
	rect_intersect(path_box, *prect);
    if (psh->params.Background && fill_background) {
	const gs_color_space *pcs = psh->params.ColorSpace;
	gs_client_color cc;
	gx_device_color dev_color;

	cc = *psh->params.Background;
	(*pcs->type->restrict_color)(&cc, pcs);
	(*pcs->type->remap_color)(&cc, pcs, &dev_color, pis,
				  dev, gs_color_select_texture);

	/****** WRONG IF NON-IDEMPOTENT RasterOp ******/
	code = gx_shade_background(dev, &path_box, &dev_color, pis->log_op);
    }
    if (code >= 0) {
	path_rect.p.x = fixed2float(path_box.p.x);
	path_rect.p.y = fixed2float(path_box.p.y);
	path_rect.q.x = fixed2float(path_box.q.x);
	path_rect.q.y = fixed2float(path_box.q.y);
	gs_bbox_transform_inverse(&path_rect, (const gs_matrix *)pmat, &rect);
	code = gs_shading_fill_rectangle(psh, &rect, &path_box, dev, pis);
    }
    return code;
}
