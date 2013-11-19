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

/* $Id: gscpixel.c 8250 2007-09-25 13:31:24Z giles $ */
/* DevicePixel color space and operation definition */
#include "gx.h"
#include "gserrors.h"
#include "gsrefct.h"
#include "gxcspace.h"
#include "gscpixel.h"
#include "gxdevice.h"
#include "gxistate.h"
#include "gsovrc.h"
#include "gsstate.h"
#include "gzstate.h"
#include "stream.h"

/* Define the DevicePixel color space type. */
static cs_proc_restrict_color(gx_restrict_DevicePixel);
static cs_proc_remap_concrete_color(gx_remap_concrete_DevicePixel);
static cs_proc_concretize_color(gx_concretize_DevicePixel);
static cs_proc_set_overprint(gx_set_overprint_DevicePixel);
static cs_proc_serialize(gx_serialize_DevicePixel);
static const gs_color_space_type gs_color_space_type_DevicePixel = {
    gs_color_space_index_DevicePixel, true, false,
    &st_base_color_space, gx_num_components_1,
    gx_init_paint_1, gx_restrict_DevicePixel,
    gx_same_concrete_space,
    gx_concretize_DevicePixel, gx_remap_concrete_DevicePixel,
    gx_default_remap_color, gx_no_install_cspace,
    gx_set_overprint_DevicePixel,
    NULL, gx_no_adjust_color_count,
    gx_serialize_DevicePixel,
    gx_cspace_is_linear_default
};

/* Create a DevicePixel color space. */
int
gs_cspace_new_DevicePixel(gs_memory_t *mem, gs_color_space **ppcs, int depth)
{
    gs_color_space *pcs;

    switch (depth) {
	case 1:
	case 2:
	case 4:
	case 8:
	case 16:
	case 24:
	case 32:
	    break;
	default:
	    return_error(gs_error_rangecheck);
    }
    pcs = gs_cspace_alloc(mem, &gs_color_space_type_DevicePixel);
    if (pcs == NULL)
	return_error(gs_error_VMerror);
    pcs->params.pixel.depth = depth;
    *ppcs = pcs;
    return 0;
}

/* ------ Internal routines ------ */

/* Force a DevicePixel color into legal range. */
static void
gx_restrict_DevicePixel(gs_client_color * pcc, const gs_color_space * pcs)
{
    /****** NOT ENOUGH BITS IN float OR frac ******/
    floatp pixel = pcc->paint.values[0];
    ulong max_value = (1L << pcs->params.pixel.depth) - 1;

    pcc->paint.values[0] = (pixel < 0 ? 0 : min(pixel, max_value));
}


/* Remap a DevicePixel color. */

static int
gx_concretize_DevicePixel(const gs_client_color * pc, const gs_color_space * pcs,
			  frac * pconc, const gs_imager_state * pis)
{
    /****** NOT ENOUGH BITS IN float OR frac ******/
    pconc[0] = (frac) (ulong) pc->paint.values[0];
    return 0;
}

static int
gx_remap_concrete_DevicePixel(const frac * pconc, const gs_color_space * pcs,
	gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
			      gs_color_select_t select)
{
    color_set_pure(pdc, pconc[0] & ((1 << dev->color_info.depth) - 1));
    return 0;
}

/* DevicePixel disables overprint */
static int
gx_set_overprint_DevicePixel(const gs_color_space * pcs, gs_state * pgs)
{
    gs_overprint_params_t   params;

    params.retain_any_comps = false;
    pgs->effective_overprint_mode = 0;
    return gs_state_update_overprint(pgs, &params);
}


/* ---------------- Serialization. -------------------------------- */

static int 
gx_serialize_DevicePixel(const gs_color_space * pcs, stream * s)
{
    const gs_device_pixel_params * p = &pcs->params.pixel;
    uint n;
    int code = gx_serialize_cspace_type(pcs, s);

    if (code < 0)
	return code;
    return sputs(s, (const byte *)&p->depth, sizeof(p->depth), &n);
}
