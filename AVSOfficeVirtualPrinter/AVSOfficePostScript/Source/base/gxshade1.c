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

/* $Id: gxshade1.c 8250 2007-09-25 13:31:24Z giles $ */
/* Rendering for non-mesh shadings */
#include "math_.h"
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsmatrix.h"		/* for gscoord.h */
#include "gscoord.h"
#include "gspath.h"
#include "gsptype2.h"
#include "gxcspace.h"
#include "gxdcolor.h"
#include "gxfarith.h"
#include "gxfixed.h"
#include "gxistate.h"
#include "gxpath.h"
#include "gxshade.h"
#include "gxdevcli.h"
#include "gxshade4.h"
#include "vdtrace.h"

#define VD_TRACE_AXIAL_PATCH 1
#define VD_TRACE_RADIAL_PATCH 1
#define VD_TRACE_FUNCTIONAL_PATCH 1


/* ---------------- Function-based shading ---------------- */

typedef struct Fb_frame_s {	/* A rudiment of old code. */
    gs_rect region;
    gs_client_color cc[4];	/* colors at 4 corners */
    int state;
} Fb_frame_t;

typedef struct Fb_fill_state_s {
    shading_fill_state_common;
    const gs_shading_Fb_t *psh;
    gs_matrix_fixed ptm;	/* parameter space -> device space */
    Fb_frame_t frame;
} Fb_fill_state_t;
/****** NEED GC DESCRIPTOR ******/

static inline void
make_other_poles(patch_curve_t curve[4])
{
    int i, j;

    for (i = 0; i < 4; i++) {
	j = (i + 1) % 4;
	curve[i].control[0].x = (curve[i].vertex.p.x * 2 + curve[j].vertex.p.x) / 3;
	curve[i].control[0].y = (curve[i].vertex.p.y * 2 + curve[j].vertex.p.y) / 3;
	curve[i].control[1].x = (curve[i].vertex.p.x + curve[j].vertex.p.x * 2) / 3;
	curve[i].control[1].y = (curve[i].vertex.p.y + curve[j].vertex.p.y * 2) / 3;
	curve[i].straight = true;
    }
}

static int
Fb_fill_region(Fb_fill_state_t * pfs, const gs_fixed_rect *rect)
{
    patch_fill_state_t pfs1;
    patch_curve_t curve[4];
    Fb_frame_t * fp = &pfs->frame;
    int code;

    if (VD_TRACE_FUNCTIONAL_PATCH && vd_allowed('s')) {
	vd_get_dc('s');
	vd_set_shift(0, 0);
	vd_set_scale(0.01);
	vd_set_origin(0, 0);
    }
    memcpy(&pfs1, (shading_fill_state_t *)pfs, sizeof(shading_fill_state_t));
    pfs1.Function = pfs->psh->params.Function;
    code = init_patch_fill_state(&pfs1);
    if (code < 0)
	return code;
    pfs1.maybe_self_intersecting = false;
    pfs1.n_color_args = 2;
    pfs1.rect = *rect;
    gs_point_transform2fixed(&pfs->ptm, fp->region.p.x, fp->region.p.y, &curve[0].vertex.p);
    gs_point_transform2fixed(&pfs->ptm, fp->region.q.x, fp->region.p.y, &curve[1].vertex.p);
    gs_point_transform2fixed(&pfs->ptm, fp->region.q.x, fp->region.q.y, &curve[2].vertex.p);
    gs_point_transform2fixed(&pfs->ptm, fp->region.p.x, fp->region.q.y, &curve[3].vertex.p);
    make_other_poles(curve);
    curve[0].vertex.cc[0] = fp->region.p.x;   curve[0].vertex.cc[1] = fp->region.p.y;
    curve[1].vertex.cc[0] = fp->region.q.x;   curve[1].vertex.cc[1] = fp->region.p.y;
    curve[2].vertex.cc[0] = fp->region.q.x;   curve[2].vertex.cc[1] = fp->region.q.y;
    curve[3].vertex.cc[0] = fp->region.p.x;   curve[3].vertex.cc[1] = fp->region.q.y;
    code = patch_fill(&pfs1, curve, NULL, NULL);
    if (term_patch_fill_state(&pfs1))
	return_error(gs_error_unregistered); /* Must not happen. */
    if (VD_TRACE_FUNCTIONAL_PATCH && vd_allowed('s'))
	vd_release_dc;
    return code;
}

int
gs_shading_Fb_fill_rectangle(const gs_shading_t * psh0, const gs_rect * rect, 
			     const gs_fixed_rect * rect_clip,
			     gx_device * dev, gs_imager_state * pis)
{
    const gs_shading_Fb_t * const psh = (const gs_shading_Fb_t *)psh0;
    gs_matrix save_ctm;
    int xi, yi;
    float x[2], y[2];
    Fb_fill_state_t state;

    shade_init_fill_state((shading_fill_state_t *) & state, psh0, dev, pis);
    state.psh = psh;
    /****** HACK FOR FIXED-POINT MATRIX MULTIPLY ******/
    gs_currentmatrix((gs_state *) pis, &save_ctm);
    gs_concat((gs_state *) pis, &psh->params.Matrix);
    state.ptm = pis->ctm;
    gs_setmatrix((gs_state *) pis, &save_ctm);
    /* Compute the parameter X and Y ranges. */
    {
	gs_rect pbox;

	gs_bbox_transform_inverse(rect, &psh->params.Matrix, &pbox);
	x[0] = max(pbox.p.x, psh->params.Domain[0]);
	x[1] = min(pbox.q.x, psh->params.Domain[1]);
	y[0] = max(pbox.p.y, psh->params.Domain[2]);
	y[1] = min(pbox.q.y, psh->params.Domain[3]);
    }
    if (x[0] > x[1] || y[0] > y[1]) {
	/* The region is outside the shading area. */
	return 0;
    }
    for (xi = 0; xi < 2; ++xi)
	for (yi = 0; yi < 2; ++yi) {
	    float v[2];

	    v[0] = x[xi], v[1] = y[yi];
	    gs_function_evaluate(psh->params.Function, v,
				 state.frame.cc[yi * 2 + xi].paint.values);
	}
    state.frame.region.p.x = x[0];
    state.frame.region.p.y = y[0];
    state.frame.region.q.x = x[1];
    state.frame.region.q.y = y[1];
    return Fb_fill_region(&state, rect_clip);
}

/* ---------------- Axial shading ---------------- */

typedef struct A_fill_state_s {
    const gs_shading_A_t *psh;
    gs_point delta;
    double length;
    double t0, t1;
    double v0, v1, u0, u1;
} A_fill_state_t;
/****** NEED GC DESCRIPTOR ******/

/* Note t0 and t1 vary over [0..1], not the Domain. */

static int
A_fill_region(A_fill_state_t * pfs, patch_fill_state_t *pfs1)
{
    const gs_shading_A_t * const psh = pfs->psh;
    double x0 = psh->params.Coords[0] + pfs->delta.x * pfs->v0;
    double y0 = psh->params.Coords[1] + pfs->delta.y * pfs->v0;
    double x1 = psh->params.Coords[0] + pfs->delta.x * pfs->v1;
    double y1 = psh->params.Coords[1] + pfs->delta.y * pfs->v1;
    double h0 = pfs->u0, h1 = pfs->u1;
    patch_curve_t curve[4];

    gs_point_transform2fixed(&pfs1->pis->ctm, x0 + pfs->delta.y * h0, y0 - pfs->delta.x * h0, &curve[0].vertex.p);
    gs_point_transform2fixed(&pfs1->pis->ctm, x1 + pfs->delta.y * h0, y1 - pfs->delta.x * h0, &curve[1].vertex.p);
    gs_point_transform2fixed(&pfs1->pis->ctm, x1 + pfs->delta.y * h1, y1 - pfs->delta.x * h1, &curve[2].vertex.p);
    gs_point_transform2fixed(&pfs1->pis->ctm, x0 + pfs->delta.y * h1, y0 - pfs->delta.x * h1, &curve[3].vertex.p);
    curve[0].vertex.cc[0] = pfs->t0; /* The element cc[1] is set to a dummy value against */
    curve[1].vertex.cc[0] = pfs->t1; /* interrupts while an idle priocessing in gxshade.6.c .  */
    curve[2].vertex.cc[0] = pfs->t1;
    curve[3].vertex.cc[0] = pfs->t0;
    curve[0].vertex.cc[1] = 0; /* The element cc[1] is set to a dummy value against */
    curve[1].vertex.cc[1] = 0; /* interrupts while an idle priocessing in gxshade.6.c .  */
    curve[2].vertex.cc[1] = 0;
    curve[3].vertex.cc[1] = 0;
    make_other_poles(curve);
    return patch_fill(pfs1, curve, NULL, NULL);
}

static inline int
gs_shading_A_fill_rectangle_aux(const gs_shading_t * psh0, const gs_rect * rect,
			    const gs_fixed_rect *clip_rect,
			    gx_device * dev, gs_imager_state * pis)
{
    const gs_shading_A_t *const psh = (const gs_shading_A_t *)psh0;
    gs_function_t * const pfn = psh->params.Function;
    gs_matrix cmat;
    gs_rect t_rect;
    A_fill_state_t state;
    patch_fill_state_t pfs1;
    float d0 = psh->params.Domain[0], d1 = psh->params.Domain[1];
    float dd = d1 - d0;
    double t0, t1;
    gs_point dist;
    int code;

    state.psh = psh;
    shade_init_fill_state((shading_fill_state_t *)&pfs1, psh0, dev, pis);
    pfs1.Function = pfn;
    pfs1.rect = *clip_rect;
    code = init_patch_fill_state(&pfs1);
    if (code < 0)
	return code;
    pfs1.maybe_self_intersecting = false;
    pfs1.function_arg_shift = 1;
    /*
     * Compute the parameter range.  We construct a matrix in which
     * (0,0) corresponds to t = 0 and (0,1) corresponds to t = 1,
     * and use it to inverse-map the rectangle to be filled.
     */
    cmat.tx = psh->params.Coords[0];
    cmat.ty = psh->params.Coords[1];
    state.delta.x = psh->params.Coords[2] - psh->params.Coords[0];
    state.delta.y = psh->params.Coords[3] - psh->params.Coords[1];
    cmat.yx = state.delta.x;
    cmat.yy = state.delta.y;
    cmat.xx = cmat.yy;
    cmat.xy = -cmat.yx;
    gs_bbox_transform_inverse(rect, &cmat, &t_rect);
    t0 = min(max(t_rect.p.y, 0), 1);
    t1 = max(min(t_rect.q.y, 1), 0);
    state.v0 = t0;
    state.v1 = t1;
    state.u0 = t_rect.p.x;
    state.u1 = t_rect.q.x;
    state.t0 = t0 * dd + d0;
    state.t1 = t1 * dd + d0;
    gs_distance_transform(state.delta.x, state.delta.y, &ctm_only(pis),
			  &dist);
    state.length = hypot(dist.x, dist.y);	/* device space line length */
    code = A_fill_region(&state, &pfs1);
    if (psh->params.Extend[0] && t0 > t_rect.p.y) {
	if (code < 0)
	    return code;
	/* Use the general algorithm, because we need the trapping. */
	state.v0 = t_rect.p.y;
	state.v1 = t0;
	state.t0 = state.t1 = t0 * dd + d0;
	code = A_fill_region(&state, &pfs1);
    }
    if (psh->params.Extend[1] && t1 < t_rect.q.y) {
	if (code < 0)
	    return code;
	/* Use the general algorithm, because we need the trapping. */
	state.v0 = t1;
	state.v1 = t_rect.q.y;
	state.t0 = state.t1 = t1 * dd + d0;
	code = A_fill_region(&state, &pfs1);
    }
    if (term_patch_fill_state(&pfs1))
	return_error(gs_error_unregistered); /* Must not happen. */
    return code;
}

int
gs_shading_A_fill_rectangle(const gs_shading_t * psh0, const gs_rect * rect,
			    const gs_fixed_rect * rect_clip,
			    gx_device * dev, gs_imager_state * pis)
{
    int code;

    if (VD_TRACE_AXIAL_PATCH && vd_allowed('s')) {
	vd_get_dc('s');
	vd_set_shift(0, 0);
	vd_set_scale(0.01);
	vd_set_origin(0, 0);
    }
    code = gs_shading_A_fill_rectangle_aux(psh0, rect, rect_clip, dev, pis);
    if (VD_TRACE_AXIAL_PATCH && vd_allowed('s'))
	vd_release_dc;
    return code;
}

/* ---------------- Radial shading ---------------- */

static int 
R_tensor_annulus(patch_fill_state_t *pfs, const gs_rect *rect,
    double x0, double y0, double r0, double t0,
    double x1, double y1, double r1, double t1)
{   
    double dx = x1 - x0, dy = y1 - y0;
    double d = hypot(dx, dy);
    gs_point p0, p1, pc0, pc1;
    int k, j, code;
    bool inside = 0;

    pc0.x = x0, pc0.y = y0; 
    pc1.x = x1, pc1.y = y1;
    if (r0 + d <= r1 || r1 + d <= r0) {
	/* One circle is inside another one. 
	   Use any subdivision, 
	   but don't depend on dx, dy, which may be too small. */
	p0.x = 0, p0.y = -1;
	/* Align stripes along radii for faster triangulation : */
	inside = 1;
    } else {
        /* Must generate canonic quadrangle arcs,
	   because we approximate them with curves. */
	if(any_abs(dx) >= any_abs(dy)) {
	    if (dx > 0)
		p0.x = 0, p0.y = -1;
	    else
		p0.x = 0, p0.y = 1;
	} else {
	    if (dy > 0)
		p0.x = 1, p0.y = 0;
	    else
		p0.x = -1, p0.y = 0;
	}
    }
    /* fixme: wish: cut invisible parts off. 
       Note : when r0 != r1 the invisible part is not a half circle. */
    for (k = 0; k < 4; k++, p0 = p1) {
	gs_point p[12];
	patch_curve_t curve[4];

	p1.x = -p0.y; p1.y = p0.x;
	if ((k & 1) == k >> 1) {
	    make_quadrant_arc(p + 0, &pc0, &p1, &p0, r0);
	    make_quadrant_arc(p + 6, &pc1, &p0, &p1, r1);
	} else {
	    make_quadrant_arc(p + 0, &pc0, &p0, &p1, r0);
	    make_quadrant_arc(p + 6, &pc1, &p1, &p0, r1);
	}
	p[4].x = (p[3].x * 2 + p[6].x) / 3;
	p[4].y = (p[3].y * 2 + p[6].y) / 3;
	p[5].x = (p[3].x + p[6].x * 2) / 3;
	p[5].y = (p[3].y + p[6].y * 2) / 3;
	p[10].x = (p[9].x * 2 + p[0].x) / 3;
	p[10].y = (p[9].y * 2 + p[0].y) / 3;
	p[11].x = (p[9].x + p[0].x * 2) / 3;
	p[11].y = (p[9].y + p[0].y * 2) / 3;
	for (j = 0; j < 4; j++) {
	    int jj = (j + inside) % 4;

	    code = gs_point_transform2fixed(&pfs->pis->ctm, 
			p[j * 3 + 0].x, p[j * 3 + 0].y, &curve[jj].vertex.p);
	    if (code < 0)
		return code;
	    code = gs_point_transform2fixed(&pfs->pis->ctm, 
			p[j * 3 + 1].x, p[j * 3 + 1].y, &curve[jj].control[0]);
	    if (code < 0)
		return code;
	    code = gs_point_transform2fixed(&pfs->pis->ctm, 
			p[j * 3 + 2].x, p[j * 3 + 2].y, &curve[jj].control[1]);
	    if (code < 0)
		return code;
	    curve[j].straight = (((j + inside) & 1) != 0);
	}
	curve[(0 + inside) % 4].vertex.cc[0] = t0;
	curve[(1 + inside) % 4].vertex.cc[0] = t0;
	curve[(2 + inside) % 4].vertex.cc[0] = t1;
	curve[(3 + inside) % 4].vertex.cc[0] = t1;
	curve[0].vertex.cc[1] = curve[1].vertex.cc[1] = 0; /* Initialize against FPE. */
	curve[2].vertex.cc[1] = curve[3].vertex.cc[1] = 0; /* Initialize against FPE. */
	code = patch_fill(pfs, curve, NULL, NULL);
	if (code < 0)
	    return code;
    }
    return 0;
}


static int
R_outer_circle(patch_fill_state_t *pfs, const gs_rect *rect, 
	double x0, double y0, double r0, 
	double x1, double y1, double r1, 
	double *x2, double *y2, double *r2)
{
    double dx = x1 - x0, dy = y1 - y0;
    double sp, sq, s;

    /* Compute a cone circle, which contacts the rect externally. */
    /* Don't bother with all 4 sides of the rect, 
       just do with the X or Y span only,
       so it's not an exact contact, sorry. */
    if (any_abs(dx) > any_abs(dy)) {
	/* Solving :
	    x0 + (x1 - x0) * sq + r0 + (r1 - r0) * sq == bbox_px
	    (x1 - x0) * sp + (r1 - r0) * sp == bbox_px - x0 - r0
	    sp = (bbox_px - x0 - r0) / (x1 - x0 + r1 - r0)

	    x0 + (x1 - x0) * sq - r0 - (r1 - r0) * sq == bbox_qx
	    (x1 - x0) * sq - (r1 - r0) * sq == bbox_x - x0 + r0
	    sq = (bbox_x - x0 + r0) / (x1 - x0 - r1 + r0)
	 */
	if (x1 - x0 + r1 - r0 ==  0) /* We checked for obtuse cone. */
	    return_error(gs_error_unregistered); /* Must not happen. */
	if (x1 - x0 - r1 + r0 ==  0) /* We checked for obtuse cone. */
	    return_error(gs_error_unregistered); /* Must not happen. */
	sp = (rect->p.x - x0 - r0) / (x1 - x0 + r1 - r0);
	sq = (rect->q.x - x0 + r0) / (x1 - x0 - r1 + r0);
    } else {
	/* Same by Y. */
	if (y1 - y0 + r1 - r0 ==  0) /* We checked for obtuse cone. */
	    return_error(gs_error_unregistered); /* Must not happen. */
	if (y1 - y0 - r1 + r0 ==  0) /* We checked for obtuse cone. */
	    return_error(gs_error_unregistered); /* Must not happen. */
	sp = (rect->p.y - y0 - r0) / (y1 - y0 + r1 - r0);
	sq = (rect->q.y - y0 + r0) / (y1 - y0 - r1 + r0);
    }
    if (sp >= 1 && sq >= 1)
	s = max(sp, sq);
    else if(sp >= 1)
	s = sp;
    else if (sq >= 1)
	s = sq;
    else {
	/* The circle 1 is outside the rect, use it. */
        s = 1;
    }
    if (r0 + (r1 - r0) * s < 0) {
	/* Passed the cone apex, use the apex. */
	s = r0 / (r0 - r1);
	*r2 = 0;
    } else
	*r2 = r0 + (r1 - r0) * s;
    *x2 = x0 + (x1 - x0) * s;
    *y2 = y0 + (y1 - y0) * s;
    return 0;
}

static double 
R_rect_radius(const gs_rect *rect, double x0, double y0)
{
    double d, dd;

    dd = hypot(rect->p.x - x0, rect->p.y - y0);
    d = hypot(rect->p.x - x0, rect->q.y - y0);
    dd = max(dd, d);
    d = hypot(rect->q.x - x0, rect->q.y - y0);
    dd = max(dd, d);
    d = hypot(rect->q.x - x0, rect->p.y - y0);
    dd = max(dd, d);
    return dd;
}

static int
R_fill_triangle_new(patch_fill_state_t *pfs, const gs_rect *rect, 
    double x0, double y0, double x1, double y1, double x2, double y2, double t)
{
    shading_vertex_t p0, p1, p2;
    patch_color_t *c;
    int code;
    reserve_colors(pfs, &c, 1); /* Can't fail */

    p0.c = c;
    p1.c = c;
    p2.c = c;
    code = gs_point_transform2fixed(&pfs->pis->ctm, x0, y0, &p0.p);
    if (code >= 0)
	code = gs_point_transform2fixed(&pfs->pis->ctm, x1, y1, &p1.p);
    if (code >= 0)
	code = gs_point_transform2fixed(&pfs->pis->ctm, x2, y2, &p2.p);
    if (code >= 0) {
	c->t[0] = c->t[1] = t;
	patch_resolve_color(c, pfs);
	code = mesh_triangle(pfs, &p0, &p1, &p2);
    }
    release_colors(pfs, pfs->color_stack, 1);
    return code;
}

static int
R_obtuse_cone(patch_fill_state_t *pfs, const gs_rect *rect,
	double x0, double y0, double r0, 
	double x1, double y1, double r1, double t0, double r_rect)
{
    double dx = x1 - x0, dy = y1 - y0, dr = any_abs(r1 - r0);
    double d = hypot(dx, dy);
    /* Assuming dr > d / 3 && d > dr + 1e-7 * (d + dr), see the caller. */
    double r = r_rect * 1.4143; /* A few bigger than sqrt(2). */
    double ax, ay, as; /* Cone apex. */
    double g0; /* The distance from apex to the tangent point of the 0th circle. */
    int code;

    as = r0 / (r0 - r1);
    ax = x0 + (x1 - x0) * as;
    ay = y0 + (y1 - y0) * as;
    g0 = sqrt(dx * dx + dy * dy - dr * dr) * as;
    if (g0 < 1e-7 * r0) {
	/* Nearly degenerate, replace with half-plane. */
	/* Restrict the half plane with triangle, which covers the rect. */
	gs_point p0, p1, p2; /* Right tangent limit, apex limit, left tangent linit,
				(right, left == when looking from the apex). */

	p0.x = ax - dy * r / d;
	p0.y = ay + dx * r / d;
	p1.x = ax - dx * r / d;
	p1.y = ay - dy * r / d;
	p2.x = ax + dy * r / d;
	p2.y = ay - dx * r / d;
	/* Split into 2 triangles at the apex,
	   so that the apex is preciselly covered.
	   Especially important when it is not exactly degenerate. */
	code = R_fill_triangle_new(pfs, rect, ax, ay, p0.x, p0.y, p1.x, p1.y, t0);
	if (code < 0)
	    return code;
	return R_fill_triangle_new(pfs, rect, ax, ay, p1.x, p1.y, p2.x, p2.y, t0);
    } else {
	/* Compute the "limit" circle so that its
	   tangent points are outside the rect. */
	/* Note: this branch is executed when the condition above is false :
	   g0 >= 1e-7 * r0 .
	   We believe that computing this branch with doubles
	   provides enough precision after converting coordinates into 'fixed',
	   and that the limit circle radius is not dramatically big.
	 */
	double es, er; /* The limit circle parameter, radius. */
	double ex, ey; /* The limit circle centrum. */

	es = as - as * r / g0; /* Always negative. */
	er = r * r0 / g0 ;
	ex = x0 + dx * es;
	ey = y0 + dy * es;
	/* Fill the annulus: */
	code = R_tensor_annulus(pfs, rect, x0, y0, r0, t0, ex, ey, er, t0);
	if (code < 0)
	    return code;
	/* Fill entire ending circle to ewnsure antire rect is covered : */
	return R_tensor_annulus(pfs, rect, ex, ey, er, t0, ex, ey, 0, t0);
    }
}

static int
R_tensor_cone_apex(patch_fill_state_t *pfs, const gs_rect *rect,
	double x0, double y0, double r0, 
	double x1, double y1, double r1, double t)
{
    double as = r0 / (r0 - r1);
    double ax = x0 + (x1 - x0) * as;
    double ay = y0 + (y1 - y0) * as;

    return R_tensor_annulus(pfs, rect, x1, y1, r1, t, ax, ay, 0, t);
}


static int
R_extensions(patch_fill_state_t *pfs, const gs_shading_R_t *psh, const gs_rect *rect, 
	double t0, double t1, bool Extend0, bool Extend1)
{
    float x0 = psh->params.Coords[0], y0 = psh->params.Coords[1];
    floatp r0 = psh->params.Coords[2];
    float x1 = psh->params.Coords[3], y1 = psh->params.Coords[4];
    floatp r1 = psh->params.Coords[5];
    double dx = x1 - x0, dy = y1 - y0, dr = any_abs(r1 - r0);
    double d = hypot(dx, dy), r;
    int code;

    if (dr >= d - 1e-7 * (d + dr)) {
	/* Nested circles, or degenerate. */
	if (r0 > r1) {
	    if (Extend0) {
		r = R_rect_radius(rect, x0, y0);
		if (r > r0) {
		    code = R_tensor_annulus(pfs, rect, x0, y0, r, t0, x0, y0, r0, t0);
		    if (code < 0)
			return code;
		}
	    }
	    if (Extend1 && r1 > 0)
		return R_tensor_annulus(pfs, rect, x1, y1, r1, t1, x1, y1, 0, t1);
	} else {
	    if (Extend1) {
		r = R_rect_radius(rect, x1, y1);
		if (r > r1) {
		    code = R_tensor_annulus(pfs, rect, x1, y1, r, t1, x1, y1, r1, t1);
		    if (code < 0)
			return code;
		}
	    }
	    if (Extend0 && r0 > 0)
		return R_tensor_annulus(pfs, rect, x0, y0, r0, t0, x0, y0, 0, t0);
	}
    } else if (dr > d / 3) {
	/* Obtuse cone. */
	if (r0 > r1) {
	    if (Extend0) {
		r = R_rect_radius(rect, x0, y0);
		code = R_obtuse_cone(pfs, rect, x0, y0, r0, x1, y1, r1, t0, r);
		if (code < 0)
		    return code;
	    }
	    if (Extend1 && r1 != 0)
		return R_tensor_cone_apex(pfs, rect, x0, y0, r0, x1, y1, r1, t1);
	    return 0;
	} else {
	    if (Extend1) {
		r = R_rect_radius(rect, x1, y1);
		code = R_obtuse_cone(pfs, rect, x1, y1, r1, x0, y0, r0, t1, r);
		if (code < 0)
		    return code;
	    }
	    if (Extend0 && r0 != 0)
		return R_tensor_cone_apex(pfs, rect, x1, y1, r1, x0, y0, r0, t0);
	}
    } else {
	/* Acute cone or cylinder. */
	double x2, y2, r2, x3, y3, r3;

	if (Extend0) {
	    code = R_outer_circle(pfs, rect, x1, y1, r1, x0, y0, r0, &x3, &y3, &r3);
	    if (code < 0)
		return code;
	    if (x3 != x1 || y3 != y1) {
		code = R_tensor_annulus(pfs, rect, x0, y0, r0, t0, x3, y3, r3, t0);
		if (code < 0)
		    return code;
	    }
	}
	if (Extend1) {
	    code = R_outer_circle(pfs, rect, x0, y0, r0, x1, y1, r1, &x2, &y2, &r2);
	    if (code < 0)
		return code;
	    if (x2 != x0 || y2 != y0) {
		code = R_tensor_annulus(pfs, rect, x1, y1, r1, t1, x2, y2, r2, t1);
		if (code < 0)
		    return code;
	    }
	}
    }
    return 0;
}

static int
R_fill_rect_with_const_color(patch_fill_state_t *pfs, const gs_fixed_rect *clip_rect, float t)
{
#if 0 /* Disabled because the clist writer device doesn't pass 
         the clipping path with fill_recatangle. */
    patch_color_t pc;
    const gs_color_space *pcs = pfs->direct_space;
    gx_device_color dc;
    int code;

    code = gs_function_evaluate(pfs->Function, &t, pc.cc.paint.values);
    if (code < 0)
	return code;
    pcs->type->restrict_color(&pc.cc, pcs);
    code = patch_color_to_device_color(pfs, &pc, &dc);
    if (code < 0)
	return code;
    return gx_fill_rectangle_device_rop(fixed2int_pixround(clip_rect->p.x), fixed2int_pixround(clip_rect->p.y), 
					fixed2int_pixround(clip_rect->q.x) - fixed2int_pixround(clip_rect->p.x),
					fixed2int_pixround(clip_rect->q.y) - fixed2int_pixround(clip_rect->p.y), 
					&dc, pfs->dev, pfs->pis->log_op);
#else
    /* Can't apply fill_rectangle, because the clist writer device doesn't pass 
       the clipping path with fill_recatangle. Convert into trapezoids instead.
    */
    quadrangle_patch p;
    shading_vertex_t pp[2][2];
    const gs_color_space *pcs = pfs->direct_space;
    patch_color_t pc;
    int code;

    code = gs_function_evaluate(pfs->Function, &t, pc.cc.paint.values);
    if (code < 0)
	return code;
    pcs->type->restrict_color(&pc.cc, pcs);
    pc.t[0] = pc.t[1] = t;
    pp[0][0].p = clip_rect->p;
    pp[0][1].p.x = clip_rect->q.x;
    pp[0][1].p.y = clip_rect->p.y;
    pp[1][0].p.x = clip_rect->p.x;
    pp[1][0].p.y = clip_rect->q.y;
    pp[1][1].p = clip_rect->q;
    pp[0][0].c = pp[0][1].c = pp[1][0].c = pp[1][1].c = &pc;
    p.p[0][0] = &pp[0][0];
    p.p[0][1] = &pp[0][1];
    p.p[1][0] = &pp[1][0];
    p.p[1][1] = &pp[1][1];
    return constant_color_quadrangle(pfs, &p, false);
#endif
}

typedef struct radial_shading_attrs_s {
    double x0, y0;
    double x1, y1;
    double span[2][2];
    double apex;
    bool have_apex;
    bool have_root[2]; /* ongoing contact, outgoing contact. */
    bool outer_contact[2];
    gs_point p[6]; /* 4 corners of the rectangle, p[4] = p[0], p[5] = p[1] */
} radial_shading_attrs_t;

#define Pw2(a) ((a)*(a))

static void
radial_shading_external_contact(radial_shading_attrs_t *rsa, int point_index, double t, double r0, double r1, bool at_corner, int root_index)
{
    double cx = rsa->x0 + (rsa->x1 - rsa->x0) * t;
    double cy = rsa->y0 + (rsa->y1 - rsa->y0) * t;
    double rx = rsa->p[point_index].x - cx;
    double ry = rsa->p[point_index].y - cy;
    double dx = rsa->p[point_index - 1].x - rsa->p[point_index].x;
    double dy = rsa->p[point_index - 1].y - rsa->p[point_index].y;

    if (at_corner) {
	double Dx = rsa->p[point_index + 1].x - rsa->p[point_index].x;
	double Dy = rsa->p[point_index + 1].y - rsa->p[point_index].y;
	bool b1 = (dx * rx + dy * ry >= 0);
	bool b2 = (Dx * rx + Dy * ry >= 0);

	if (b1 & b2)
	    rsa->outer_contact[root_index] = true;
    } else {
	if (rx * dy - ry * dx < 0)
	    rsa->outer_contact[root_index] = true;
    }
}

static void
store_roots(radial_shading_attrs_t *rsa, const bool have_root[2], const double t[2], double r0, double r1, int point_index, bool at_corner)
{
    int i;

    for (i = 0; i < 2; i++) {
	bool good_root;

	if (!have_root[i])
	    continue;
	good_root = (!rsa->have_apex || (rsa->apex <= 0 || r0 == 0 ? t[i] >= rsa->apex : t[i] <= rsa->apex));
	if (good_root) {
	    radial_shading_external_contact(rsa, point_index, t[i], r0, r1, at_corner, i);
	    if (!rsa->have_root[i]) {
		rsa->span[i][0] = rsa->span[i][1] = t[i];
		rsa->have_root[i] = true;
	    } else {
		if (rsa->span[i][0] > t[i])
		    rsa->span[i][0] = t[i];
		if (rsa->span[i][1] < t[i])
		    rsa->span[i][1] = t[i];
	    }
	}
    }
}

static void
compute_radial_shading_span_extended_side(radial_shading_attrs_t *rsa, double r0, double r1, int point_index)
{
    double cc, c;
    bool have_root[2] = {false, false};
    double t[2];
    bool by_x = (rsa->p[point_index].x == rsa->p[point_index + 1].x);
    int i;

    /* Assuming x0 = y0 = 0 : 
       cc * t +- (r0 + (r1 - r0) * t) == c 
       t0 := (c - r0) / (cc + (r1 - r0))
       t1 := (c + r0) / (cc - (r1 - r0))
     */
    if (by_x) {
	c = rsa->p[point_index].x - rsa->x0;
	cc = rsa->x1 - rsa->x0;
    } else {
	c = rsa->p[point_index].y - rsa->y0;
	cc = rsa->y1 - rsa->y0;
    }
    t[0] = (c - r0) / (cc + r1 - r0);
    t[1] = (c + r0) / (cc - r1 + r0);
    if (t[0] > t[1]) {
	t[0] = t[1];
	t[1] = (c - r0) / (cc + r1 - r0);
    }
    for (i = 0; i < 2; i++) {
	double d, d0, d1;

	if (by_x) {
	    d = rsa->y1 - rsa->y0 + r0 + (r1 - r0) * t[i];
	    d0 = rsa->p[point_index].y;
	    d1 = rsa->p[point_index + 1].y;
	} else {
	    d = rsa->x1 - rsa->x0 + r0 + (r1 - r0) * t[i];
	    d0 = rsa->p[point_index].x;
	    d1 = rsa->p[point_index + 1].x;
	}
	if (d1 > d0 ? d0 <= d && d <= d1 : d1 <= d && d <= d0)
	    have_root[i] = true;
    }
    store_roots(rsa, have_root, t, r0, r1, point_index, false);
}

static int
compute_radial_shading_span_extended_point(radial_shading_attrs_t *rsa, double r0, double r1, int point_index)
{
    double p1x = rsa->x1 - rsa->x0;
    double p1y = rsa->y1 - rsa->y0;
    double qx = rsa->p[point_index].x - rsa->x0;
    double qy = rsa->p[point_index].y - rsa->y0;
    double div = (Pw2(p1x) + Pw2(p1y) - Pw2(r0 - r1));
    bool have_root[2] = {false, false};
    double t[2];

    if (fabs(div) < 1e-8) {
	/* Linear equation. */
	/* This case is always the ongoing eclipese contact. */
	double cx = rsa->x0 - (rsa->x1 - rsa->x0) * r0 / (r1 - r0);
	double cy = rsa->y0 - (rsa->y1 - rsa->y0) * r0 / (r1 - r0);
	
	t[0] = (Pw2(qx) + Pw2(qy))/(cx*qx + cy*qy) / 2;
	have_root[0] = true;
    } else {
	/* Square equation. */
	double desc2 = -((Pw2(qx) + Pw2(qy) - Pw2(r0))*(Pw2(p1x) + Pw2(p1y) - Pw2(r0 - r1))) + Pw2(p1x*qx + p1y*qy + r0*(-r0 + r1));

	if (desc2 < 0) {
	    return -1; /* The point is outside the shading coverage. 
		          Do not shorten, because we didn't observe it in practice. */
	} else {
	    double desc1 = sqrt(desc2);

	    if (div > 0) {
		t[0] = (p1x*qx + p1y*qy + r0*(-r0 + r1) - desc1) / div;
		t[1] = (p1x*qx + p1y*qy + r0*(-r0 + r1) + desc1) / div;
	    } else {
		t[0] = (p1x*qx + p1y*qy + r0*(-r0 + r1) + desc1) / div;
		t[1] = (p1x*qx + p1y*qy + r0*(-r0 + r1) - desc1) / div;
	    }
	    have_root[0] = have_root[1] = true;
	}
    }
    store_roots(rsa, have_root, t, r0, r1, point_index, true);
    if (have_root[0] && have_root[1]) 
	return 15;
    if (have_root[0])
	return 15 - 4;
    if (have_root[1])
	return 15 - 2;
    return -1;
}

#undef Pw2

static int
compute_radial_shading_span_extended(radial_shading_attrs_t *rsa, double r0, double r1)
{
    int span_type0, span_type1;
    
    span_type0 = compute_radial_shading_span_extended_point(rsa, r0, r1, 1);
    if (span_type0 == -1)
	return -1;
    span_type1 = compute_radial_shading_span_extended_point(rsa, r0, r1, 2);
    if (span_type0 != span_type1)
	return -1;
    span_type1 = compute_radial_shading_span_extended_point(rsa, r0, r1, 3);
    if (span_type0 != span_type1)
	return -1;
    span_type1 = compute_radial_shading_span_extended_point(rsa, r0, r1, 4);
    if (span_type0 != span_type1)
	return -1;
    compute_radial_shading_span_extended_side(rsa, r0, r1, 1);
    compute_radial_shading_span_extended_side(rsa, r0, r1, 2);
    compute_radial_shading_span_extended_side(rsa, r0, r1, 3);
    compute_radial_shading_span_extended_side(rsa, r0, r1, 4);
    return span_type0;
}

static int
compute_radial_shading_span(radial_shading_attrs_t *rsa, float x0, float y0, floatp r0, float x1, float y1, floatp r1, const gs_rect * rect)
{
    /* If the shading area is much larger than the path bbox,
       we want to shorten the shading for a faster rendering.
       If any point of the path bbox falls outside the shading area,
       our math is not applicable, and we render entire shading.
       If the path bbox is inside the shading area,
       we compute 1 or 2 'spans' - the shading parameter intervals,
       which covers the bbox. For doing that we need to resolve 
       a square eqation by the shading parameter 
       for each corner of the bounding box,
       and for each side of the shading bbox.
       Note the equation to be solved in the user space.
       Since each equation gives 2 roots (because the points are
       strongly inside the shading area), we will get 2 parameter intervals -
       the 'lower' one corresponds to the first (ongoing) contact of
       the running circle, and the second one corresponds to the last (outgoing) contact
       (like in a sun eclipse; well our sun is rectangular).

       Here are few exceptions. 

       First, the equation degenerates when the distance sqrt((x1-x0)^2 + (y1-y0)^2)
       appears equal to r0-r1. In this case the base circles do contact,
       and the running circle does contact at the same point.
       The equation degenerates to a linear one.
       Since we don't want float precision noize to affect the result,
       we compute this condition in 'fixed' coordinates.

       Second, Postscript approximates any circle with 3d order beziers.
       This approximation may give a 2% error.
       Therefore using the precise roots may cause a dropout.
       To prevetn them, we slightly modify the base radii.
       However the sign of modification smartly depends
       on the relative sizes of the base circles,
       and on the contact number. Currently we don't want to
       define and debug the smart optimal logic for that,
       so we simply try all 4 variants for each source equation,
       and use the union of intervals.

       Third, we could compute which quarter of the circle
       really covers the path bbox. Using it we could skip
       rendering of uncovering quarters. Currently we do not
       implement this optimization. The general tensor patch algorithm
       will skip uncovering parts.

       Fourth, when one base circle is (almost) inside the other, 
       the parameter interval must include the shading apex.
       To know that, we determine whether the contacting circle
       is outside the rectangle (the "outer" contact),
       or it is (partially) inside the rectangle.

       At last, a small shortening of a shading won't give a
       sensible speedup, but it may replace a symmetric function domain
       with an assymmetric one, so that the rendering 
       would be asymmetyric for a symmetric shading.
       Therefore we do not perform a small sortening.
       Instead we shorten only if the shading span
       is much smaller that the shading domain.
     */
    const double extent = 1.02;
    int span_type0, span_type1, span_type;

    memset(rsa, 0, sizeof(*rsa));
    rsa->x0 = x0;
    rsa->y0 = y0;
    rsa->x1 = x1;
    rsa->y1 = y1;
    rsa->p[0] = rsa->p[4] = rect->p;
    rsa->p[1].x = rsa->p[5].x = rect->p.x;
    rsa->p[1].y = rsa->p[5].y = rect->q.y;
    rsa->p[2] = rect->q;
    rsa->p[3].x = rect->q.x;
    rsa->p[3].y = rect->p.y;
    rsa->have_apex = any_abs(r1 - r0) > 1e-7 * any_abs(r1 + r0);
    rsa->apex = (rsa->have_apex ? -r0 / (r1 - r0) : 0);
    span_type0 = compute_radial_shading_span_extended(rsa, r0 / extent, r1 * extent);
    if (span_type0 == -1)
	return -1;
    span_type1 = compute_radial_shading_span_extended(rsa, r0 / extent, r1 / extent);
    if (span_type0 != span_type1)
	return -1;
    span_type1 = compute_radial_shading_span_extended(rsa, r0 * extent, r1 * extent);
    if (span_type0 != span_type1)
	return -1;
    span_type1 = compute_radial_shading_span_extended(rsa, r0 * extent, r1 / extent);
    if (span_type1 == -1)
	return -1;
    if (r0 < r1) {
	if (rsa->have_root[0] && !rsa->outer_contact[0])
	    rsa->span[0][0] = rsa->apex; /* Likely never happens. Remove ? */
	if (rsa->have_root[1] && !rsa->outer_contact[1])
	    rsa->span[1][0] = rsa->apex;
    } else if (r0 > r1) {
	if (rsa->have_root[0] && !rsa->outer_contact[0])
	    rsa->span[0][1] = rsa->apex;
	if (rsa->have_root[1] && !rsa->outer_contact[1])
	    rsa->span[1][1] = rsa->apex; /* Likely never happens. Remove ? */
    }
    span_type = 0;
    if (rsa->have_root[0] && rsa->span[0][0] < 0)
	span_type |= 1;
    if (rsa->have_root[1] && rsa->span[1][0] < 0)
	span_type |= 1;
    if (rsa->have_root[0] && rsa->span[0][1] > 0 && rsa->span[0][0] < 1)
	span_type |= 2;
    if (rsa->have_root[1] && rsa->span[1][1] > 0 && rsa->span[1][0] < 1)
	span_type |= 4;
    if (rsa->have_root[0] && rsa->span[0][1] > 1)
	span_type |= 8;
    if (rsa->have_root[1] && rsa->span[1][1] > 1)
	span_type |= 8;
    return span_type;
}

static bool
shorten_radial_shading(float *x0, float *y0, floatp *r0, float *d0, float *x1, float *y1, floatp *r1, float *d1, double span_[2])
{
    double s0 = span_[0], s1 = span_[1], w;

    if (s0 < 0)
	s0 = 0;
    if (s1 < 0)
	s1 = 0;
    if (s0 > 1)
	s0 = 1;
    if (s1 > 1)
	s1 = 1;
    w = s1 - s0;
    if (w == 0)
	return false; /* Don't pass a degenerate shading. */
    if (w > 0.3)
	return false; /* The span is big, don't shorten it. */
    {	/* Do shorten. */
	double R0 = *r0, X0 = *x0, Y0 = *y0, D0 = *d0;
	double R1 = *r1, X1 = *x1, Y1 = *y1, D1 = *d1;

	*r0 = R0 + (R1 - R0) * s0;
	*x0 = X0 + (X1 - X0) * s0;
	*y0 = Y0 + (Y1 - Y0) * s0;
	*d0 = D0 + (D1 - D0) * s0;
	*r1 = R0 + (R1 - R0) * s1;
	*x1 = X0 + (X1 - X0) * s1;
	*y1 = Y0 + (Y1 - Y0) * s1;
	*d1 = D0 + (D1 - D0) * s1;
    }
    return true;
}

static bool inline
is_radial_shading_large(double x0, double y0, double r0, double d0, double x1, double y1, double r1, const gs_rect * rect)
{
    const double d = hypot(x1 - x0, y1 - y0);
    const double area0 = M_PI * r0 * r0 / 2;
    const double area1 = M_PI * r1 * r1 / 2;
    const double area2 = (r0 + r1) / 2 * d;
    const double arbitrary = 8;
    double areaX, areaY;

    /* The shading area is not equal to area0 + area1 + area2
       when one circle is (almost) inside the other.
       We believe that the 'arbitrary' coefficient recovers that
       when it is set greater than 2. */
    /* If one dimension is large enough, the shading parameter span is wide. */
    areaX = (rect->q.x - rect->p.x) * (rect->q.x - rect->p.x);
    if (areaX * arbitrary < area0 + area1 + area2)
	return true;
    areaY = (rect->q.y - rect->p.y) * (rect->q.y - rect->p.y);
    if (areaY * arbitrary < area0 + area1 + area2)
	return true;
    return false;
}

static int
gs_shading_R_fill_rectangle_aux(const gs_shading_t * psh0, const gs_rect * rect,
			    const gs_fixed_rect *clip_rect,
			    gx_device * dev, gs_imager_state * pis)
{
    const gs_shading_R_t *const psh = (const gs_shading_R_t *)psh0;
    float d0 = psh->params.Domain[0], d1 = psh->params.Domain[1];
    float x0 = psh->params.Coords[0], y0 = psh->params.Coords[1];
    floatp r0 = psh->params.Coords[2];
    float x1 = psh->params.Coords[3], y1 = psh->params.Coords[4];
    floatp r1 = psh->params.Coords[5];
    radial_shading_attrs_t rsa;
    int span_type; /* <0 - don't shorten, 1 - extent0, 2 - first contact, 4 - last contact, 8 - extent1. */
    int code;
    patch_fill_state_t pfs1;

    if (r0 == 0 && r1 == 0)
	return 0; /* PLRM requires to paint nothing. */
    shade_init_fill_state((shading_fill_state_t *)&pfs1, psh0, dev, pis);
    pfs1.Function = psh->params.Function;
    code = init_patch_fill_state(&pfs1);
    if (code < 0)
	return code;
    pfs1.function_arg_shift = 1;
    pfs1.rect = *clip_rect;
    pfs1.maybe_self_intersecting = false;
    if (is_radial_shading_large(x0, y0, r0, d0, x1, y1, r1, rect))
	span_type = compute_radial_shading_span(&rsa, x0, y0, r0, x1, y1, r1, rect);
    else
	span_type = -1;
    if (span_type < 0) {
	code = R_extensions(&pfs1, psh, rect, d0, d1, psh->params.Extend[0], false);
	if (code >= 0)
	    code = R_tensor_annulus(&pfs1, rect, x0, y0, r0, d0, x1, y1, r1, d1);
	if (code >= 0)
	    code = R_extensions(&pfs1, psh, rect, d0, d1, false, psh->params.Extend[1]);
    } else if (span_type == 1) {
	code = R_fill_rect_with_const_color(&pfs1, clip_rect, d0);
    } else if (span_type == 8) {
	code = R_fill_rect_with_const_color(&pfs1, clip_rect, d1);
    } else {
	bool second_interval = true; 

	code = 0;
	if (span_type & 1)
	    code = R_extensions(&pfs1, psh, rect, d0, d1, psh->params.Extend[0], false);
	if (code >= 0) {
	    float X0 = x0, Y0 = y0, D0 = d0, X1 = x1, Y1 = y1, D1 = d1;
	    floatp R0 = r0, R1 = r1;

	    if ((span_type & 2) && (span_type & 4) && rsa.span[0][1] >= rsa.span[1][0]) {
		double united[2];

		united[0] = rsa.span[0][0];
		united[1] = rsa.span[1][1];
		shorten_radial_shading(&X0, &Y0, &R0, &D0, &X1, &Y1, &R1, &D1, united);
		second_interval = false;
		code = R_tensor_annulus(&pfs1, rect, X0, Y0, R0, D0, X1, Y1, R1, D1);
	    } else if (span_type & 2) {
		second_interval = shorten_radial_shading(&X0, &Y0, &R0, &D0, &X1, &Y1, &R1, &D1, rsa.span[0]);
		code = R_tensor_annulus(&pfs1, rect, X0, Y0, R0, D0, X1, Y1, R1, D1);
	    }
	}
	if (code >= 0 && second_interval) {
	    if (span_type & 4) {
		float X0 = x0, Y0 = y0, D0 = d0, X1 = x1, Y1 = y1, D1 = d1;
		floatp R0 = r0, R1 = r1;

		shorten_radial_shading(&X0, &Y0, &R0, &D0, &X1, &Y1, &R1, &D1, rsa.span[1]);
		code = R_tensor_annulus(&pfs1, rect, X0, Y0, R0, D0, X1, Y1, R1, D1);
	    }
	}
	if (code >= 0 && (span_type & 8))
	    code = R_extensions(&pfs1, psh, rect, d0, d1, false, psh->params.Extend[1]);
    }
    if (term_patch_fill_state(&pfs1))
	return_error(gs_error_unregistered); /* Must not happen. */
    return code;
}

int
gs_shading_R_fill_rectangle(const gs_shading_t * psh0, const gs_rect * rect,
			    const gs_fixed_rect * rect_clip,
			    gx_device * dev, gs_imager_state * pis)
{   
    int code;

    if (VD_TRACE_RADIAL_PATCH && vd_allowed('s')) {
	vd_get_dc('s');
	vd_set_shift(0, 0);
	vd_set_scale(0.01);
	vd_set_origin(0, 0);
    }
    code = gs_shading_R_fill_rectangle_aux(psh0, rect, rect_clip, dev, pis);
    if (VD_TRACE_FUNCTIONAL_PATCH && vd_allowed('s'))
	vd_release_dc;
    return code;
}
