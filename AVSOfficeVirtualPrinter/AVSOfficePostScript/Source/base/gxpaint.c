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

/* $Id: gxpaint.c 8250 2007-09-25 13:31:24Z giles $ */
/* Graphics-state-aware fill and stroke procedures */
#include "gx.h"
#include "gzstate.h"
#include "gxdevice.h"
#include "gxhttile.h"
#include "gxpaint.h"
#include "gxpath.h"
#include "gxfont.h"

static bool caching_an_outline_font(const gs_state * pgs)
{
    return pgs->in_cachedevice > 1 &&
	    pgs->font != NULL &&
	    pgs->font->FontType != ft_user_defined && 
	    pgs->font->FontType != ft_CID_user_defined;
}

/* Fill a path. */
int
gx_fill_path(gx_path * ppath, gx_device_color * pdevc, gs_state * pgs,
	     int rule, fixed adjust_x, fixed adjust_y)
{
    gx_device *dev = gs_currentdevice_inline(pgs);
    gx_clip_path *pcpath;
    int code = gx_effective_clip_path(pgs, &pcpath);
    gx_fill_params params;

    if (code < 0)
	return code;
    params.rule = rule;
    params.adjust.x = adjust_x;
    params.adjust.y = adjust_y;
    params.flatness = (caching_an_outline_font(pgs) ? 0.0 : pgs->flatness);
    params.fill_zero_width = (adjust_x | adjust_y) != 0;
    return (*dev_proc(dev, fill_path))
	(dev, (const gs_imager_state *)pgs, ppath, &params, pdevc, pcpath);
}

/* Stroke a path for drawing or saving. */
int
gx_stroke_fill(gx_path * ppath, gs_state * pgs)
{
    gx_device *dev = gs_currentdevice_inline(pgs);
    gx_clip_path *pcpath;
    int code = gx_effective_clip_path(pgs, &pcpath);
    gx_stroke_params params;

    if (code < 0)
	return code;
    params.flatness = (caching_an_outline_font(pgs) ? 0.0 : pgs->flatness);
    return (*dev_proc(dev, stroke_path))
	(dev, (const gs_imager_state *)pgs, ppath, &params,
	 pgs->dev_color, pcpath);
}

int
gx_stroke_add(gx_path * ppath, gx_path * to_path,
	      const gs_state * pgs)
{
    gx_stroke_params params;

    params.flatness = (caching_an_outline_font(pgs) ? 0.0 : pgs->flatness);
    return gx_stroke_path_only(ppath, to_path, pgs->device,
			       (const gs_imager_state *)pgs,
			       &params, NULL, NULL);
}

int
gx_imager_stroke_add(gx_path *ppath, gx_path *to_path,
		     gx_device *dev, const gs_imager_state *pis)
{
    gx_stroke_params params;

    params.flatness = pis->flatness;
    return gx_stroke_path_only(ppath, to_path, dev, pis,
			       &params, NULL, NULL);
}
