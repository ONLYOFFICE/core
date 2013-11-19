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

/*$Id: gsequivc.h 8022 2007-06-05 22:23:38Z giles $ */
/* Header for routines for determining equivalent color for spot colors */

/* For more information, see comment at the start of src/gsequivc.c */

#ifndef gsequivc_INCLUDED
# define gsequivc_INCLUDED

/*
 * Structure for holding a CMYK color.
 */
typedef struct cmyk_color_s {
    bool color_info_valid;
    frac c;
    frac m;
    frac y;
    frac k;
} cmyk_color;

/*
 * Structure for holding parameters for collecting the equivalent CMYK
 * for a spot colorant.
 */
typedef struct equivalent_cmyk_color_params_s {
    bool all_color_info_valid;
    cmyk_color color[GX_DEVICE_MAX_SEPARATIONS];
} equivalent_cmyk_color_params;

/*
 * If possible, update the equivalent CMYK color for spot colors.
 */
void update_spot_equivalent_cmyk_colors(gx_device * pdev,
		const gs_state * pgs, gs_devn_params * pdevn_params,
		equivalent_cmyk_color_params * pparams);

/*
 * Utiliy routine:  Capture equivalent color when given a modified
 * color space.
 */
void capture_spot_equivalent_cmyk_colors(gx_device * pdev,
		const gs_state * pgs, const gs_client_color * pcc,
		const gs_color_space * pcs, int sep_num,
		equivalent_cmyk_color_params * pparams);

#endif		/* define gsequivc_INCLUDED */
