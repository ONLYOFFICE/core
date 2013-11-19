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

/* $Id: gdevppla.h 8528 2008-02-17 22:32:15Z leonardo $ */
/* Support for printer devices with planar buffering. */
/* Requires gdevprn.h */

#ifndef gdevppla_INCLUDED
#  define gdevppla_INCLUDED

/* Set the buf_procs in a printer device to planar mode. */
int gdev_prn_set_procs_planar(gx_device *pdev);

/* Open a printer device, conditionally setting it to be planar. */
int gdev_prn_open_planar(gx_device *pdev, bool upb);

/* Augment get/put_params to add UsePlanarBuffer. */
int gdev_prn_get_params_planar(gx_device * pdev, gs_param_list * plist,
			       bool *pupb);
int gdev_prn_put_params_planar(gx_device * pdev, gs_param_list * plist,
			       bool *pupb);

/* Create a planar buffer device. */
/* Use this instead of the default if UsePlanarBuffer is true. */
int gdev_prn_create_buf_planar(gx_device **pbdev, gx_device *target, int y,
			       const gx_render_plane_t *render_plane,
			       gs_memory_t *mem, gx_band_complexity_t *for_band);

/* Determine the space needed by a planar buffer device. */
/* Use this instead of the default if UsePlanarBuffer is true. */
int gdev_prn_size_buf_planar(gx_device_buf_space_t *space,
			     gx_device *target,
			     const gx_render_plane_t *render_plane,
			     int height, bool for_band);

#endif /* gdevppla_INCLUDED */
