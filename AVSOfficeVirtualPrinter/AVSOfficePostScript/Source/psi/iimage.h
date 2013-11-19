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

/* $Id: iimage.h 9043 2008-08-28 22:48:19Z giles $ */
/* Image operator entry points */
/* Requires gscspace.h, gxiparam.h */

#ifndef iimage_INCLUDED
#  define iimage_INCLUDED

/* These procedures are exported by zimage.c for other modules. */

/*
 * Define a structure for image parameters other than those defined
 * in the gs_*image*_t structure.
 */
typedef struct image_params_s {
    bool MultipleDataSources;
    ref DataSource[GS_IMAGE_MAX_COMPONENTS];
    const float *pDecode;
} image_params;

/* Extract and check parameters for an image. */
int data_image_params(const gs_memory_t *mem, 
		      const ref *op, gs_data_image_t *pim,
                      image_params *pip, bool require_DataSource,
                      int num_components, int max_bits_per_component,
                      bool has_alpha);
int pixel_image_params(i_ctx_t *i_ctx_p, const ref *op,
                       gs_pixel_image_t *pim, image_params * pip,
                       int max_bits_per_component, bool has_alpha,
                       gs_color_space *csp);

/* Exported for zimage3.c and ztrans.c */
int zimage_setup(i_ctx_t *i_ctx_p, const gs_pixel_image_t * pim,
                 const ref * sources, bool uses_color, int npop);

/* Exported for zdpnext.c */
int image1_setup(i_ctx_t * i_ctx_p, bool has_alpha);

#endif /* iimage_INCLUDED */
