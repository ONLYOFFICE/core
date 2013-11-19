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

/* $Id: gxshade4.c 8250 2007-09-25 13:31:24Z giles $ */
/* Rendering for Gouraud triangle shadings */
#include "math_.h"
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsmatrix.h"		/* for gscoord.h */
#include "gscoord.h"
#include "gsptype2.h"
#include "gxcspace.h"
#include "gxdcolor.h"
#include "gxdevcli.h"
#include "gxistate.h"
#include "gxpath.h"
#include "gxshade.h"
#include "gxshade4.h"
#include "vdtrace.h"

#define VD_TRACE_TRIANGLE_PATCH 1

/* Initialize the fill state for triangle shading. */
int
mesh_init_fill_state(mesh_fill_state_t * pfs, const gs_shading_mesh_t * psh,
		     const gs_fixed_rect * rect_clip, gx_device * dev,
		     gs_imager_state * pis)
{
    shade_init_fill_state((shading_fill_state_t *) pfs,
			  (const gs_shading_t *)psh, dev, pis);
    pfs->pshm = psh;
    pfs->rect = *rect_clip;
    return 0;
}

/* ---------------- Gouraud triangle shadings ---------------- */

static int
Gt_next_vertex(const gs_shading_mesh_t * psh, shade_coord_stream_t * cs,
	       shading_vertex_t * vertex, patch_color_t *c)
{
    int code = shade_next_vertex(cs, vertex, c);
 
    if (code >= 0 && psh->params.Function) {
	c->t[0] = c->cc.paint.values[0];
	c->t[1] = 0;
	/* Decode the color with the function. */
	code = gs_function_evaluate(psh->params.Function, c->t,
				    c->cc.paint.values);
    } else
	psh->params.ColorSpace->type->restrict_color(&c->cc, psh->params.ColorSpace);
    return code;
}

static inline int
Gt_fill_triangle(patch_fill_state_t * pfs, const shading_vertex_t * va,
		 const shading_vertex_t * vb, const shading_vertex_t * vc)
{
    int code = 0;

    if (INTERPATCH_PADDING) {
	code = mesh_padding(pfs, &va->p, &vb->p, va->c, vb->c);
	if (code >= 0)
	    code = mesh_padding(pfs, &vb->p, &vc->p, vb->c, vc->c);
	if (code >= 0)
	    code = mesh_padding(pfs, &vc->p, &va->p, vc->c, va->c);
    }
    if (code >= 0)
	code = mesh_triangle(pfs, va, vb, vc);
    return code;
}

int
gs_shading_FfGt_fill_rectangle(const gs_shading_t * psh0, const gs_rect * rect,
			       const gs_fixed_rect * rect_clip,
			       gx_device * dev, gs_imager_state * pis)
{
    const gs_shading_FfGt_t * const psh = (const gs_shading_FfGt_t *)psh0;
    patch_fill_state_t pfs;
    const gs_shading_mesh_t *pshm = (const gs_shading_mesh_t *)psh;
    shade_coord_stream_t cs;
    int num_bits = psh->params.BitsPerFlag;
    int flag;
    shading_vertex_t va, vb, vc;
    patch_color_t *c, *C[3], *ca, *cb, *cc; /* va.c == ca && vb.c == cb && vc.c == cc always, 
				        provides a non-const access. */
    int code;

    if (VD_TRACE_TRIANGLE_PATCH && vd_allowed('s')) {
	vd_get_dc('s');
	vd_set_shift(0, 0);
	vd_set_scale(0.01);
	vd_set_origin(0, 0);
    }
    shade_init_fill_state((shading_fill_state_t *)&pfs,
			  (const gs_shading_t *)psh, dev, pis);
    pfs.Function = pshm->params.Function;
    pfs.rect = *rect_clip;
    code = init_patch_fill_state(&pfs);
    if (code < 0)
	return code;
    reserve_colors(&pfs, C, 3); /* Can't fail */
    va.c = ca = C[0];
    vb.c = cb = C[1];
    vc.c = cc = C[2];
    shade_next_init(&cs, (const gs_shading_mesh_params_t *)&psh->params,
		    pis);
    /* CET 09-47J.PS SpecialTestI04Test01 does not need the color data alignment. */
    while ((flag = shade_next_flag(&cs, num_bits)) >= 0) {
	switch (flag) {
	    default:
		return_error(gs_error_rangecheck);
	    case 0:
		if ((code = Gt_next_vertex(pshm, &cs, &va, ca)) < 0 ||
		    (code = shade_next_flag(&cs, num_bits)) < 0 ||
		    (code = Gt_next_vertex(pshm, &cs, &vb, cb)) < 0 ||
		    (code = shade_next_flag(&cs, num_bits)) < 0
		    )
		    break;
		goto v2;
	    case 1:
		c = ca;
		va = vb;
		ca = cb;
		vb.c = cb = c;
	    case 2:
		c = cb;
		vb = vc;
		cb = cc;
		vc.c = cc = c;
v2:		if ((code = Gt_next_vertex(pshm, &cs, &vc, cc)) < 0)
		    break;
		if ((code = Gt_fill_triangle(&pfs, &va, &vb, &vc)) < 0)
		    break;
	}
	cs.align(&cs, 8); /* Debugged with 12-14O.PS page 2. */
    }
    if (VD_TRACE_TRIANGLE_PATCH && vd_allowed('s'))
	vd_release_dc;
    release_colors(&pfs, pfs.color_stack, 3);
    if (term_patch_fill_state(&pfs))
	return_error(gs_error_unregistered); /* Must not happen. */
    if (!cs.is_eod(&cs))
	return_error(gs_error_rangecheck);
    return code;
}

int
gs_shading_LfGt_fill_rectangle(const gs_shading_t * psh0, const gs_rect * rect,
			       const gs_fixed_rect * rect_clip,
			       gx_device * dev, gs_imager_state * pis)
{
    const gs_shading_LfGt_t * const psh = (const gs_shading_LfGt_t *)psh0;
    patch_fill_state_t pfs;
    const gs_shading_mesh_t *pshm = (const gs_shading_mesh_t *)psh;
    shade_coord_stream_t cs;
    shading_vertex_t *vertex = NULL;
    byte *color_buffer = NULL;
    patch_color_t **color_buffer_ptrs = NULL; /* non-const access to vertex[i].c */
    shading_vertex_t next;
    int per_row = psh->params.VerticesPerRow;
    patch_color_t *c, *cn; /* cn == next.c always, provides a non-contst access. */
    int i, code;

    if (VD_TRACE_TRIANGLE_PATCH && vd_allowed('s')) {
	vd_get_dc('s');
	vd_set_shift(0, 0);
	vd_set_scale(0.01);
	vd_set_origin(0, 0);
    }
    shade_init_fill_state((shading_fill_state_t *)&pfs,
			  (const gs_shading_t *)psh, dev, pis);
    pfs.Function = pshm->params.Function;
    pfs.rect = *rect_clip;
    code = init_patch_fill_state(&pfs);
    if (code < 0)
	goto out;
    reserve_colors(&pfs, &cn, 1); /* Can't fail. */
    next.c = cn;
    shade_next_init(&cs, (const gs_shading_mesh_params_t *)&psh->params,
		    pis);
    vertex = (shading_vertex_t *)
	gs_alloc_byte_array(pis->memory, per_row, sizeof(*vertex),
			    "gs_shading_LfGt_render");
    if (vertex == NULL) {
	code = gs_note_error(gs_error_VMerror);
	goto out;
    }
    color_buffer = gs_alloc_bytes(pis->memory, pfs.color_stack_step * per_row, "gs_shading_LfGt_fill_rectangle");
    if (color_buffer == NULL) {
	code = gs_note_error(gs_error_VMerror);
	goto out;
    }
    color_buffer_ptrs = (patch_color_t **)gs_alloc_bytes(pis->memory, 
			    sizeof(patch_color_t *) * per_row, "gs_shading_LfGt_fill_rectangle");
    if (color_buffer_ptrs == NULL) {
	code = gs_note_error(gs_error_VMerror);
	goto out;
    }
    /* CET 09-47K.PS SpecialTestJ02Test05 needs the color data alignment. */
    for (i = 0; i < per_row; ++i) {
	color_buffer_ptrs[i] = (patch_color_t *)(color_buffer + pfs.color_stack_step * i);
	vertex[i].c = color_buffer_ptrs[i];
	if ((code = Gt_next_vertex(pshm, &cs, &vertex[i], color_buffer_ptrs[i])) < 0)
	    goto out;
    }
    while (!seofp(cs.s)) {
	code = Gt_next_vertex(pshm, &cs, &next, cn);
	if (code < 0)
	    goto out;
	for (i = 1; i < per_row; ++i) {
	    code = Gt_fill_triangle(&pfs, &vertex[i - 1], &vertex[i], &next);
	    if (code < 0)
		goto out;
	    c = color_buffer_ptrs[i - 1];
	    vertex[i - 1] = next;
	    color_buffer_ptrs[i - 1] = cn;
	    next.c = cn = c;
	    code = Gt_next_vertex(pshm, &cs, &next, cn);
	    if (code < 0)
		goto out;
	    code = Gt_fill_triangle(&pfs, &vertex[i], &vertex[i - 1], &next);
	    if (code < 0)
		goto out;
	}
	c = color_buffer_ptrs[per_row - 1];
	vertex[per_row - 1] = next;
	color_buffer_ptrs[per_row - 1] = cn;
	next.c = cn = c;
    }
out:
    if (VD_TRACE_TRIANGLE_PATCH && vd_allowed('s'))
	vd_release_dc;
    gs_free_object(pis->memory, vertex, "gs_shading_LfGt_render");
    gs_free_object(pis->memory, color_buffer, "gs_shading_LfGt_render");
    gs_free_object(pis->memory, color_buffer_ptrs, "gs_shading_LfGt_render");
    release_colors(&pfs, pfs.color_stack, 1);
    if (term_patch_fill_state(&pfs))
	return_error(gs_error_unregistered); /* Must not happen. */
    return code;
}
