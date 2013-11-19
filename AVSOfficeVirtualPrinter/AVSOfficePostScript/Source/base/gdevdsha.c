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
/* $Id: gdevdsha.c 9062 2008-09-03 11:42:35Z leonardo $ */
/* Default shading drawing device procedures. */

#include "gx.h"
#include "gserrors.h"
#include "gxdevice.h"
#include "gxcindex.h"
#include "vdtrace.h"

int 
gx_default_fill_linear_color_scanline(gx_device *dev, const gs_fill_attributes *fa,
	int i0, int j, int w,
	const frac31 *c0, const int32_t *c0f, const int32_t *cg_num, int32_t cg_den)
{
    /* This default implementation decomposes the area into constant color rectangles.
       Devices may supply optimized implementations with
       the inversed nesting of the i,k cicles,
       i.e. with enumerating planes first, with a direct writing to the raster,
       and with a fixed bits per component.
     */
    frac31 c[GX_DEVICE_COLOR_MAX_COMPONENTS];
    ulong f[GX_DEVICE_COLOR_MAX_COMPONENTS];
    int i, i1 = i0 + w, bi = i0, k;
    gx_color_index ci0 = 0, ci1;
    const gx_device_color_info *cinfo = &dev->color_info;
    int n = cinfo->num_components;
    int si, ei, di, code;

    if (j < fixed2int(fa->clip->p.y) ||
	    j > fixed2int_ceiling(fa->clip->q.y)) /* Must be compatible to the clipping logic. */
	return 0;
    for (k = 0; k < n; k++) {
	int shift = cinfo->comp_shift[k];
	int bits = cinfo->comp_bits[k];

	c[k] = c0[k];
	f[k] = c0f[k];
	ci0 |= (gx_color_index)(c[k] >> (sizeof(c[k]) * 8 - 1 - bits)) << shift;
    }
    for (i = i0 + 1, di = 1; i < i1; i += di) {
	if (di == 1) {
	    /* Advance colors by 1 pixel. */
	    ci1 = 0;
	    for (k = 0; k < n; k++) {
		int shift = cinfo->comp_shift[k];
		int bits = cinfo->comp_bits[k];

		if (cg_num[k]) {
		    int32_t m = f[k] + cg_num[k];

		    c[k] += m / cg_den;
		    m -= m / cg_den * cg_den;
		    if (m < 0) {
			c[k]--;
			m += cg_den;
		    }
		    f[k] = m;
		}
		ci1 |= (gx_color_index)(c[k] >> (sizeof(c[k]) * 8 - 1 - bits)) << shift;
	    }
	} else {
	    /* Advance colors by di pixels. */
	    ci1 = 0;
	    for (k = 0; k < n; k++) {
		int shift = cinfo->comp_shift[k];
		int bits = cinfo->comp_bits[k];

		if (cg_num[k]) {
		    int64_t M = f[k] + (int64_t)cg_num[k] * di;
		    int32_t m;

		    c[k] += (frac31)(M / cg_den);
		    m = (int32_t)(M - M / cg_den * cg_den);
		    if (m < 0) {
			c[k]--;
			m += cg_den;
		    }
		    f[k] = m;
		}
		ci1 |= (gx_color_index)(c[k] >> (sizeof(c[k]) * 8 - 1 - bits)) << shift;
	    }
	}
	if (ci1 != ci0) {
	    si = max(bi, fixed2int(fa->clip->p.x));	    /* Must be compatible to the clipping logic. */
	    ei = min(i, fixed2int_ceiling(fa->clip->q.x));  /* Must be compatible to the clipping logic. */
	    if (si < ei) {
		if (fa->swap_axes) {
		    vd_rect(int2fixed(j), int2fixed(si), int2fixed(j + 1), int2fixed(ei), 1, (ulong)ci0);
		    code = dev_proc(dev, fill_rectangle)(dev, j, si, 1, ei - si, ci0);
		} else {
		    vd_rect(int2fixed(si), int2fixed(j), int2fixed(ei), int2fixed(j + 1), 1, (ulong)ci0);
		    code = dev_proc(dev, fill_rectangle)(dev, si, j, ei - si, 1, ci0);
		}
		if (code < 0)
		    return code;
	    }
	    bi = i;
	    ci0 = ci1;
	    di = 1;
	} else if (i == i1) {
	    i++;
	    break;
	} else {
	    /* Compute a color change pixel analitically. */
	    di = i1 - i;
	    for (k = 0; k < n; k++) {
		int32_t a;
		int64_t x;
		frac31 v = 1 << (31 - cinfo->comp_bits[k]); /* Color index precision in frac31. */
		frac31 u = c[k] & (v - 1);

		if (cg_num[k] == 0) {
		    /* No change. */
		    continue;
		} if (cg_num[k] > 0) {
		    /* Solve[(f[k] + cg_num[k]*x)/cg_den == v - u, x]  */
		    a = v - u;
		} else {
		    /* Solve[(f[k] + cg_num[k]*x)/cg_den == - u - 1, x]  */
		    a = -u - 1;
		}
		x = ((int64_t)a * cg_den - f[k]) / cg_num[k];
		if (i + x >= i1)
		    continue;
		else if (x < 0)
		    return_error(gs_error_unregistered); /* Must not happen. */
		else if (di > (int)x) {
		    di = (int)x;
		    if (di <= 1) {
			di = 1;
			break;
		    }
		}
	    }
	}
    }
    si = max(bi, fixed2int(fa->clip->p.x));	    /* Must be compatible to the clipping logic. */
    ei = min(i, fixed2int_ceiling(fa->clip->q.x));  /* Must be compatible to the clipping logic. */
    if (si < ei) {
	if (fa->swap_axes) {
	    vd_rect(int2fixed(j), int2fixed(si), int2fixed(j + 1), int2fixed(ei), 1, (ulong)ci0);
	    return dev_proc(dev, fill_rectangle)(dev, j, si, 1, ei - si, ci0);
	} else {
	    vd_rect(int2fixed(si), int2fixed(j), int2fixed(ei), int2fixed(j + 1), 1, (ulong)ci0);
	    return dev_proc(dev, fill_rectangle)(dev, si, j, ei - si, 1, ci0);
	}
    }
    return 0;
}

