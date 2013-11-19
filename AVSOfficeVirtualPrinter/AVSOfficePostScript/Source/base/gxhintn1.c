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

/* $Id: gxhintn1.c 8250 2007-09-25 13:31:24Z giles $ */
/* A stuff for reconnizing and fixing wrong contour signs. */

#include "memory_.h"
#include "math_.h"
#include "gx.h"
#include "gxfixed.h"
#include "gxarith.h"
#include "gstypes.h"
#include "gxmatrix.h"
#include "gxhintn.h"
#include "gzpath.h"
#include "vdtrace.h"

#define CURVE_FLATTENING (fixed_1) /* Design units in 'fixed'. */

static double inline line_area_2(fixed p0x, fixed p0y, fixed p1x, fixed p1y)
{   /* Returns area * 2.*/
    return ((double)p0x*p1y - (double)p0y*p1x);
}

static double inline bezier_area_2(fixed p0x, fixed p0y, fixed p1x, fixed p1y, 
				  fixed p2x, fixed p2y, fixed p3x, fixed p3y)
{   /* Returns area * 2.*/
    return (-(p0y*(6.0*p1x + 3.0*p2x + p3x)) + p0x*(6.0*p1y + 3.0*p2y + p3y) - 
     3*((double)p1y*p2x + (double)p1y*p3x + 2.0*p2y*p3x - 2.0*p2x*p3y - (double)p1x*(p2y + p3y)))/10;
}

static void t1_hinter__reverse_contour(t1_hinter * this, int c)
{
    int b = this->contour[c];
    int e = this->contour[c + 1] - 1; /* Skip 'close'. */
    int e2 = (b + e + 1) / 2;
    int i;
    t1_pole p;

    /* Reverse all except endpoint ('close') : */
    for (i = b + 1; i < e2; i++) {
	int j = e - (i - b);

	p = this->pole[i];
	this->pole[i] = this->pole[j];
	this->pole[j] = p;
    }
}

#define CONTACT_SIGNAL -100000.0

static double inline bar_winding_angle(fixed x0, fixed y0, fixed x1, fixed y1)
{
    double vp = (double)x0 * y1 - (double)y0 * x1;
    double sp = (double)x0 * x1 + (double)y0 * y1;
    double A;
    
    if (sp == 0) {
	if (vp == 0)
	    return CONTACT_SIGNAL; /* Contours contact. */
	A = 1.57079632679489661923; /* pi/2. */
	if (vp < 0)
	    A = -A;
    } else
	A = atan2(vp, sp);
    return A;
}

static double
curve_winding_angle_rec(int k, fixed x0, fixed y0, fixed x1, fixed y1, fixed x2, fixed y2, fixed x3, fixed y3)
{
    if (k <= 1)
	return bar_winding_angle(x0, y0, x3, y3);
    else {
	/* Assuming the trapezoid is not self-intersecting and 
	   the curve is inside the trapezoid 
	   due to Type 1 constraints. */
	double a0 = bar_winding_angle(x0, y0, x1, y1);
	double a1 = bar_winding_angle(x1, y1, x2, y2);
	double a2 = bar_winding_angle(x2, y2, x3, y3);
	double a3 = bar_winding_angle(x3, y3, x0, y0);
	double a = a0 + a1 + a2 + a3;

	if (any_abs(a) < 0.1 && a0 != CONTACT_SIGNAL &&
		a1 != CONTACT_SIGNAL &&
		a2 != CONTACT_SIGNAL &&
		a3 != CONTACT_SIGNAL) {
	    /* The center is outside the trapezoid. */
	    return -a3;
	} else {
	    fixed x01 = (x0 + x1) / 2, y01 = (y0 + y1) / 2;
	    fixed x12 = (x1 + x2) / 2, y12 = (y1 + y2) / 2;
	    fixed x23 = (x2 + x3) / 2, y23 = (y2 + y3) / 2;
	    fixed x012 = (x01 + x12) / 2, y012 = (y01 + y12) / 2;
	    fixed x123 = (x12 + x23) / 2, y123 = (y12 + y23) / 2;
	    fixed x0123 = (x012 + x123) / 2, y0123 = (y012 + y123) / 2;
	    double A0, A1;

	    A0 = curve_winding_angle_rec(k - 1, x0, y0, x01, y01, x012, y012, x0123, y0123);
	    if (A0 == CONTACT_SIGNAL)
		return CONTACT_SIGNAL;
	    A1 = curve_winding_angle_rec(k - 1, x0123, y0123, x123, y123, x23, y23, x3, y3);
	    if (A1 == CONTACT_SIGNAL)
		return CONTACT_SIGNAL;
	    return A0 + A1;
	}
    }
}

static int curve_log2_samples(fixed x0, fixed y0, fixed x1, fixed y1, fixed x2, fixed y2, fixed x3, fixed y3)
{
    curve_segment s;

    s.p1.x = x1;
    s.p1.y = y1;
    s.p2.x = x2;
    s.p2.y = y2;
    s.pt.x = x3;
    s.pt.y = y3;
    return gx_curve_log2_samples(x0, y0, &s, (fixed)CURVE_FLATTENING);
}

static double curve_winding_angle(fixed x0, fixed y0, fixed x1, fixed y1, fixed x2, fixed y2, fixed x3, fixed y3)
{
    int k = curve_log2_samples(x0, y0, x1, y1, x2, y2, x3, y3);

    return curve_winding_angle_rec(k, x0, y0, x1, y1, x2, y2, x3, y3);
}

static int t1_hinter__is_inside(t1_hinter * this, t1_glyph_space_coord gx, t1_glyph_space_coord gy, int c)
{
    int b = this->contour[c];
    int e = this->contour[c + 1] - 1;
    double a = 0, A;
    int i;

    for (i = b; i < e;) {
	if (this->pole[i + 1].type != offcurve) {  /* line or close. */
	    A = bar_winding_angle(this->pole[i + 0].gx - gx, this->pole[i + 0].gy - gy, 
				    this->pole[i + 1].gx - gx, this->pole[i + 1].gy - gy);
	    i++;
	} else {
	    A = curve_winding_angle(this->pole[i + 0].gx - gx, this->pole[i + 0].gy - gy, 
				    this->pole[i + 1].gx - gx, this->pole[i + 1].gy - gy, 
				    this->pole[i + 2].gx - gx, this->pole[i + 2].gy - gy, 
				    this->pole[i + 3].gx - gx, this->pole[i + 3].gy - gy);
	    i += 3;
	}
	if (A == CONTACT_SIGNAL)
	    return -1;
	a += A;
    }
    if (any_abs(a) < 0.1)
	return 0;
    return 1;
}

static inline bool
intersect_bar_bar(fixed q0x, fixed q0y, fixed q1x, fixed q1y, fixed q2x, fixed q2y, fixed q3x, fixed q3y)
{
    if (q1x == q0x && q1y == q0y)
	return false;
    if (q1x == q2x && q1y == q2y)
	return false;
    if (q0x == q2x && q0y == q2y)
	return true;
    if (q0x == q3x && q0y == q3y)
	return true;
    if (q1x == q2x && q1y == q2y)
	return true;
    if (q1x == q3x && q1y == q3y)
	return true;
    else {
	fixed dx1 = q1x - q0x;
	fixed dy1 = q1y - q0y;
	fixed dx2 = q2x - q0x;
	fixed dy2 = q2y - q0y;
	fixed dx3 = q3x - q0x;
	fixed dy3 = q3y - q0y;
	fixed dx1a = any_abs(dx1);
	fixed dy1a = any_abs(dy1);
	fixed dx2a = any_abs(dx2);
	fixed dy2a = any_abs(dy2);
	fixed dx3a = any_abs(dx3);
	fixed dy3a = any_abs(dy3);
	fixed d = dx1a | dy1a | dx2a | dy2a | dx3a | dy3a;
	fixed ry, ey; /* stubs only - don't use them, they are whong here. */

	/* gx_intersect_small_bars needs cubes of distances to fit into 62 bits,
	   Drop extra bits here. 
	   We don't need ry, so don't bother with absolute coordinates. */
	while (d >= (1 << (60 / 3))) {
	    d >>= 1;
	    dx1 = (dx1 + 1) / 2;
	    dy1 = (dy1 + 1) / 2;
	    dx2 = (dy2 + 1) / 2;
	    dy2 = (dy2 + 1) / 2;
	    dx3 = (dy3 + 1) / 2;
	    dy3 = (dy3 + 1) / 2;
	}
	/* Well, when we drop bits, the intersection isn't precise.
	   But it happens with big characters only, 
	   which unlikely have close oncurve poles
	   which belong to different contours. 
	   Due to that we believe the boolean result is precise
	   with a very high probablility. */
	return gx_intersect_small_bars(0, 0, dx1, dy1, dx2, dy2, dx3, dy3, &ry, &ey);
    }
}

static inline bool
t1_hinter__intersect_bar_bar(t1_hinter * this, int i, int j)
{
    fixed q0x = this->pole[i + 0].gx;
    fixed q0y = this->pole[i + 0].gy;
    fixed q1x = this->pole[i + 1].gx;
    fixed q1y = this->pole[i + 1].gy;
    fixed q2x = this->pole[j + 0].gx;
    fixed q2y = this->pole[j + 0].gy;
    fixed q3x = this->pole[j + 1].gx;
    fixed q3y = this->pole[j + 1].gy;

    return intersect_bar_bar(q0x, q0y, q1x, q1y, q2x, q2y, q3x, q3y);
}

static bool intersect_curve_bar_rec(int m, int k, fixed X1, fixed Y1, 
				     fixed x0, fixed y0, fixed x1, fixed y1, fixed x2, fixed y2, fixed x3, fixed y3)
{
    if (m <= 1)
	return intersect_bar_bar(0, 0, X1, Y1, x0, y0, x3, y3);
    else {
	gs_rect box0, box1;

	if (X1 < 0)
	    box0.p.x = X1, box0.q.x = 0;
	else
	    box0.p.x = 0, box0.q.x = X1;
	if (Y1 < 0)
	    box0.p.y = Y1, box0.q.y = 0;
	else
	    box0.p.y = 0, box0.q.y = Y1;

	box1.p.x = box1.q.x = x0;
	box1.p.y = box1.q.y = y0;
	if (box1.p.x > x1)
	    box1.p.x = x1;
	if (box1.q.x < x1)
	    box1.q.x = x1;
	if (box1.p.y > y1)
	    box1.p.y = y1;
	if (box1.q.y < y1)
	    box1.q.y = y1;
	if (box1.p.x > x2)
	    box1.p.x = x2;
	if (box1.q.x < x2)
	    box1.q.x = x2;
	if (box1.p.y > y2)
	    box1.p.y = y2;
	if (box1.q.y < y2)
	    box1.q.y = y2;
	if (box1.p.x > x3)
	    box1.p.x = x3;
	if (box1.q.x < x3)
	    box1.q.x = x3;
	if (box1.p.y > y3)
	    box1.p.y = y3;
	if (box1.q.y < y3)
	    box1.q.y = y3;
	if (box0.p.x > box1.q.x)
	    return false;
	if (box0.q.x < box1.p.x)
	    return false;
	if (box0.p.y > box1.q.y)
	    return false;
	if (box0.q.y < box1.p.y)
	    return false;
    }
    {	fixed x01 = (x0 + x1) / 2, y01 = (y0 + y1) / 2;
	fixed x12 = (x1 + x2) / 2, y12 = (y1 + y2) / 2;
	fixed x23 = (x2 + x3) / 2, y23 = (y2 + y3) / 2;
	fixed x012 = (x01 + x12) / 2, y012 = (y01 + y12) / 2;
	fixed x123 = (x12 + x23) / 2, y123 = (y12 + y23) / 2;
	fixed x0123 = (x012 + x123) / 2, y0123 = (y012 + y123) / 2;

	if (k <= 1) {
	    if (intersect_curve_bar_rec(m - 1, k, X1, Y1, x0, y0, x01, y01, x012, y012, x0123, y0123))
		return true;
	    if (intersect_curve_bar_rec(m - 1, k, X1, Y1, x0123, y0123, x123, y123, x23, y23, x3, y3))
		return true;
	} else {
	    fixed X01 = X1 / 2;
	    fixed Y01 = Y1 / 2;

	    if (intersect_curve_bar_rec(m - 1, k - 1, X01, Y01, x0, y0, x01, y01, x012, y012, x0123, y0123))
		return true;
	    if (intersect_curve_bar_rec(m - 1, k - 1, X01, Y01, x0123, y0123, x123, y123, x23, y23, x3, y3))
		return true;
	    if (intersect_curve_bar_rec(m - 1, k - 1, X1 - X01, Y1 - Y01, x0 - X01, y0 - Y01, x01 - X01, y01 - Y01, 
						x012 - X01, y012 - Y01, x0123 - X01, y0123 - Y01))
		return true;
	    if (intersect_curve_bar_rec(m - 1, k - 1, X1 - X01, Y1 - Y01, x0123 - X01, y0123 - Y01, 
						x123 - X01, y123 - Y01, x23 - X01, y23 - Y01, x3 - X01, y3 - Y01))
		return true;
	}
    }
    return false;
}

static int bar_samples(fixed dx, fixed dy)
{
    int l = (any_abs(dx) | any_abs(dy)) / CURVE_FLATTENING, m = 0;
    while (l) {
	l >>= 1;
	m++;
    }
    return m;
}

static bool t1_hinter__intersect_curve_bar(t1_hinter * this, int i, int j)
{
    fixed X0 = this->pole[j].gx;
    fixed Y0 = this->pole[j].gy;
    fixed X1 = this->pole[j + 1].gx - X0;
    fixed Y1 = this->pole[j + 1].gy - Y0;
    fixed x0 = this->pole[i].gx - X0;
    fixed y0 = this->pole[i].gy - Y0;
    fixed x1 = this->pole[i + 1].gx - X0;
    fixed y1 = this->pole[i + 1].gy - Y0;
    fixed x2 = this->pole[i + 2].gx - X0;
    fixed y2 = this->pole[i + 2].gy - Y0;
    fixed x3 = this->pole[i + 3].gx - X0;
    fixed y3 = this->pole[i + 3].gy - Y0;
    int k = curve_log2_samples(0, 0, x1, y1, x2, y2, x3, y3);
    int m = bar_samples(X1, Y1);

    return intersect_curve_bar_rec(m, k, X1, Y1, x0, y0, x1, y1, x2, y2, x3, y3);
}

static bool intersect_curve_curve_rec(int ka, int kb,
				     fixed ax0, fixed ay0, fixed ax1, fixed ay1, fixed ax2, fixed ay2, fixed ax3, fixed ay3,
				     fixed bx0, fixed by0, fixed bx1, fixed by1, fixed bx2, fixed by2, fixed bx3, fixed by3)
{
    if (ka <= 1 && kb <= 1)
	return intersect_bar_bar(ax0, ay0, ax3, ay3, bx0, by0, bx3, by3);
    else if (ka <= 1) {
	int m = bar_samples(ax3 - ax0, ay3 - ay0);

	return intersect_curve_bar_rec(m, kb, ax3 - ax0, ay3 - ay0, 
				     bx0 - ax0, by0 - ay0, bx1 - ax0, by1 - ay0, bx2 - ax0, by2 - ay0, bx3 - ax0, by3 - ay0);
    } else if (kb <= 1) {
	int m = bar_samples(bx3 - bx0, by3 - by0);

	return intersect_curve_bar_rec(m, ka, bx3 - bx0, by3 - by0, 
				     ax0 - bx0, ay0 - by0, ax1 - bx0, ay1 - by0, ax2 - bx0, ay2 - by0, ax3 - bx0, ay3 - by0);
    } else {
	gs_rect box0, box1;
	
	box0.p.x = box0.q.x = ax0;
	box0.p.y = box0.q.y = ay0;
	if (box0.p.x > ax1)
	    box0.p.x = ax1;
	if (box0.q.x < ax1)
	    box0.q.x = ax1;
	if (box0.p.y > ay1)
	    box0.p.y = ay1;
	if (box0.q.y < ay1)
	    box0.q.y = ay1;
	if (box0.p.x > ax2)
	    box0.p.x = ax2;
	if (box0.q.x < ax2)
	    box0.q.x = ax2;
	if (box0.p.y > ay2)
	    box0.p.y = ay2;
	if (box0.q.y < ay2)
	    box0.q.y = ay2;
	if (box0.p.x > ax3)
	    box0.p.x = ax3;
	if (box0.q.x < ax3)
	    box0.q.x = ax3;
	if (box0.p.y > ay3)
	    box0.p.y = ay3;
	if (box0.q.y < ay3)
	    box0.q.y = ay3;
	box1.p.x = box1.q.x = bx0;
	box1.p.y = box1.q.y = by0;
	if (box1.p.x > bx1)
	    box1.p.x = bx1;
	if (box1.q.x < bx1)
	    box1.q.x = bx1;
	if (box1.p.y > by1)
	    box1.p.y = by1;
	if (box1.q.y < by1)
	    box1.q.y = by1;
	if (box1.p.x > bx2)
	    box1.p.x = bx2;
	if (box1.q.x < bx2)
	    box1.q.x = bx2;
	if (box1.p.y > by2)
	    box1.p.y = by2;
	if (box1.q.y < by2)
	    box1.q.y = by2;
	if (box1.p.x > bx3)
	    box1.p.x = bx3;
	if (box1.q.x < bx3)
	    box1.q.x = bx3;
	if (box1.p.y > by3)
	    box1.p.y = by3;
	if (box1.q.y < by3)
	    box1.q.y = by3;
	if (box0.p.x > box1.q.x)
	    return false;
	if (box0.q.x < box1.p.x)
	    return false;
	if (box0.p.y > box1.q.y)
	    return false;
	if (box0.q.y < box1.p.y)
	    return false;
    } 
    { 	fixed ax01 = (ax0 + ax1) / 2, ay01 = (ay0 + ay1) / 2;
	fixed ax12 = (ax1 + ax2) / 2, ay12 = (ay1 + ay2) / 2;
	fixed ax23 = (ax2 + ax3) / 2, ay23 = (ay2 + ay3) / 2;
	fixed ax012 = (ax01 + ax12) / 2, ay012 = (ay01 + ay12) / 2;
	fixed ax123 = (ax12 + ax23) / 2, ay123 = (ay12 + ay23) / 2;
	fixed ax0123 = (ax012 + ax123) / 2, ay0123 = (ay012 + ay123) / 2;
    	fixed bx01 = (bx0 + bx1) / 2, by01 = (by0 + by1) / 2;
	fixed bx12 = (bx1 + bx2) / 2, by12 = (by1 + by2) / 2;
	fixed bx23 = (bx2 + bx3) / 2, by23 = (by2 + by3) / 2;
	fixed bx012 = (bx01 + bx12) / 2, by012 = (by01 + by12) / 2;
	fixed bx123 = (bx12 + bx23) / 2, by123 = (by12 + by23) / 2;
	fixed bx0123 = (bx012 + bx123) / 2, by0123 = (by012 + by123) / 2;

	if (intersect_curve_curve_rec(ka - 1, kb - 1, ax0, ay0, ax01, ay01, ax012, ay012, ax0123, ay0123,
						      bx0, by0, bx01, by01, bx012, by012, bx0123, by0123))
	    return true;
	if (intersect_curve_curve_rec(ka - 1, kb - 1, ax0, ay0, ax01, ay01, ax012, ay012, ax0123, ay0123,
						      bx0123, by0123, bx123, by123, bx23, by23, bx3, by3))
	    return true;
	if (intersect_curve_curve_rec(ka - 1, kb - 1, ax0123, ay0123, ax123, ay123, ax23, ay23, ax3, ay3,
						      bx0, by0, bx01, by01, bx012, by012, bx0123, by0123))
	    return true;
	if (intersect_curve_curve_rec(ka - 1, kb - 1, ax0123, ay0123, ax123, ay123, ax23, ay23, ax3, ay3,
						      bx0123, by0123, bx123, by123, bx23, by23, bx3, by3))
	    return true;

    }
    return false;
}

static bool t1_hinter__intersect_curve_curve(t1_hinter * this, int i, int j)
{
    fixed ax0 = this->pole[i].gx;
    fixed ay0 = this->pole[i].gy;
    fixed ax1 = this->pole[i + 1].gx;
    fixed ay1 = this->pole[i + 1].gy;
    fixed ax2 = this->pole[i + 2].gx;
    fixed ay2 = this->pole[i + 2].gy;
    fixed ax3 = this->pole[i + 3].gx;
    fixed ay3 = this->pole[i + 3].gy;
    fixed bx0 = this->pole[j].gx;
    fixed by0 = this->pole[j].gy;
    fixed bx1 = this->pole[j + 1].gx;
    fixed by1 = this->pole[j + 1].gy;
    fixed bx2 = this->pole[j + 2].gx;
    fixed by2 = this->pole[j + 2].gy;
    fixed bx3 = this->pole[j + 3].gx;
    fixed by3 = this->pole[j + 3].gy;
    int ka = curve_log2_samples(ax0, ay0, ax1, ay1, ax2, ay2, ax3, ay3);
    int kb = curve_log2_samples(bx0, by0, bx1, by1, bx2, by2, bx3, by3);

    return intersect_curve_curve_rec(ka, kb,
				     ax0, ay0, ax1, ay1, ax2, ay2, ax3, ay3,
				     bx0, by0, bx1, by1, bx2, by2, bx3, by3);
}

static bool t1_hinter__contour_intersection(t1_hinter * this, int c0, int c1)
{
    int b0 = this->contour[c0];
    int e0 = this->contour[c0 + 1] - 1;
    int b1 = this->contour[c1];
    int e1 = this->contour[c1 + 1] - 1;
    int i, j;

    for (i = b0; i < e0;) {
	if (this->pole[i + 1].type != offcurve) {  /* line or close. */
	    for (j = b1; j < e1;) {
		if (this->pole[j + 1].type != offcurve) {  /* line or close. */
		    if (t1_hinter__intersect_bar_bar(this, i, j))
			return true;
		    j++;
		} else {
		    if (t1_hinter__intersect_curve_bar(this, j, i))
			return true;
		    j += 3;
		}
	    }
	    i++;
	} else {
	    for (j = b1; j < e1;) {
		if (this->pole[j + 1].type != offcurve) {  /* line or close. */
		    if (t1_hinter__intersect_curve_bar(this, i, j))
			return true;
		    j++;
		} else {
		    if (t1_hinter__intersect_curve_curve(this, j, i))
			return true;
		    j += 3;
		}
	    }
	    i += 3;
	}
    }
    return false;
}

#define MAX_NORMALIZING_CONTOURS 5

static void t1_hinter__fix_subglyph_contour_signs(t1_hinter * this, int first_contour, int last_contour)
{
    double area[MAX_NORMALIZING_CONTOURS];
    int i, j, k, l, m;
    double a = 0;
    byte inside[MAX_NORMALIZING_CONTOURS][MAX_NORMALIZING_CONTOURS];
    int nesting[MAX_NORMALIZING_CONTOURS];
    gs_rect bbox[MAX_NORMALIZING_CONTOURS];
    byte isolated[MAX_NORMALIZING_CONTOURS];
    int nesting_sum;

    if (first_contour == last_contour) {
	/* Don't fix a single contour. */
	return;
    }
    /* Compute contour bboxes : */
    k = 0;
    for(i =  first_contour; i <= last_contour; i++) {
	int b = this->contour[i];
	int e = this->contour[i + 1] - 1;

	bbox[k].p.x = bbox[k].q.x = this->pole[b].gx;
	bbox[k].p.y = bbox[k].q.y = this->pole[b].gy;
	/* 'close' has same coords as the starting point. */
	for (j = b; j < e; j++) {
	    t1_glyph_space_coord x = this->pole[j].gx;
	    t1_glyph_space_coord y = this->pole[j].gy;

	    if (bbox[k].p.x > x)
		bbox[k].p.x = x;
	    if (bbox[k].q.x < x)
		bbox[k].q.x = x;
	    if (bbox[k].p.y > y)
		bbox[k].p.y = y;
	    if (bbox[k].q.y < y)
		bbox[k].q.y = y;
	}
	k++;
    }
    /* mark contacting bboxes : */
    memset(isolated, 0, sizeof(isolated));
    for (i = 0; i < k; i++) {
	for (j = i + 1; j < k; j++) {
	    if (bbox[i].p.x > bbox[j].q.x)
		continue;
	    if (bbox[i].q.x < bbox[j].p.x)
		continue;
	    if (bbox[i].p.y > bbox[j].q.y)
		continue;
	    if (bbox[i].q.y < bbox[j].p.y)
		continue;
	    isolated[i] = isolated[j] = 1; /* mark not isolated. */
	}
    }
    /* Make a list of non-isolated contours : */
    j = 0;
    for (i = 0; i < k; i++) {
	if (isolated[i]) {
	    isolated[j] = first_contour + i;
	    j++;
	}
    }
    k = j;
    /* So far we skip isolated contours. */
    if (k <= 1)
	return; /* Nothing to fix. */
    /* Compute contour signes : */
    for(i = 0; i < k; i++) {
	int c = isolated[i];
	int b = this->contour[c];
	int e = this->contour[c + 1] - 1;

	a = 0;
	/* 'close' has same coords as the starting point. */
	for (j = b; j < e; ) {
	    if (this->pole[j + 1].type != offcurve) { /* line or close. */
		a += line_area_2(this->pole[j + 0].gx, this->pole[j + 0].gy, 
				 this->pole[j + 1].gx, this->pole[j + 1].gy);
		j++;
	    } else {
		a += bezier_area_2(this->pole[j + 0].gx, this->pole[j + 0].gy, 
				   this->pole[j + 1].gx, this->pole[j + 1].gy,
				   this->pole[j + 2].gx, this->pole[j + 2].gy, 
				   this->pole[j + 3].gx, this->pole[j + 3].gy);
		j += 3;
	    }
	}
	area[i] = a;
    }
    /* If contours have different signs, don't adjust. */
    for (i = 1; i < k; i++) {
	if (area[0] * area[i] < 0)
	    return;
    }
    /* Compute the insideness matrix :
       For any contoor pair A, B,
       check if some point of A is inside B. */
    for (i = 0; i < k; i++) {
	inside[i][i] = 0;
	for (j = 0; j < k; j++) {
	    if (i != j) {
		int b = this->contour[isolated[i]];
		int code = t1_hinter__is_inside(this, this->pole[b].gx, this->pole[b].gy, isolated[j]);

		if (code < 0) {
		    /* Contours have a common point - don't fix. */
		    return; 
		}
		inside[i][j] = (byte)code;
		if (i > j && inside[j][i]) {
		    /* Contours intersect, don't fix. */
		    return;
		}
	    }
	}
    }
    /* Transitive closure : */
    do {
	m = 0;
	for (i = 0; i < k; i++) {
	    for (j = 0; j < k; j++) {
		if (i != j) {
		    for (l = 0; l < k; l++) {
			if (j != l && inside[i][j] && inside[j][l]) {
			    if (inside[l][i]) {
				/* Cycled - don't fix. */
				return;
			    }
			    if (!inside[i][l])
				m = 1;
			    inside[i][l] = 1;
			}
		    }
		}
	    }
	}
    } while(m);
    /* Compute nesting numbers : */
    nesting_sum = 0;
    memset(nesting, 0, sizeof(nesting));
    for (i = 0; i < k; i++) {
	for (j = 0; j < k; j++) {
	    if (inside[i][j]) {
		nesting[i]++;
		nesting_sum++;
	    }
	}
    }
    if (nesting_sum == 0) {
	/* No nesting contours - don't fix. 
	   We want to save time from computing contour intersections. */
	return;
    }
    /* Check contour intersections : */
    for (i = 0; i < k; i++) {
	for (j = 0; j < k; j++) {
	    if (inside[i][j]) {
		if (t1_hinter__contour_intersection(this, isolated[i], isolated[j])) {
		    /* Contours intersect - don't fix. */
		    return;
		}
	    }
	}
    }
    /* Fix signs : */
    for (i = 0; i < k; i++) {
	if ((nesting[i] & 1) != (area[i] < 0))
	    t1_hinter__reverse_contour(this, isolated[i]);
    }
    /* Note we didn't fix negative isolated contours. 
       We never meet such cases actually. */
}

void t1_hinter__fix_contour_signs(t1_hinter * this)
{
    int i;

    if (this->subglyph_count >= 3) {
	/* 3 or more subglyphs.
	   We didn't meet so complex characters with wrong contours signs. 
	   Skip it for saving the CPU time. */
	return;
    }
    for (i = 1; i <= this->subglyph_count; i++) {
	int first_contour = this->subglyph[i - 1];
	int last_contour  = this->subglyph[i] - 1;

	if (last_contour - first_contour >= MAX_NORMALIZING_CONTOURS) { 
	    /* 4 or more contours.
	       We didn't meet so complex characters with wrong contours signs. 
	       Skip it for saving the CPU time. */
	    continue;
	}
	t1_hinter__fix_subglyph_contour_signs(this, first_contour, last_contour);
    }
}
