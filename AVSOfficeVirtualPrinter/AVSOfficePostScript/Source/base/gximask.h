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

/* $Id: gximask.h 8022 2007-06-05 22:23:38Z giles $ */
/* Functions for masked fill optimization. */

#ifndef gximask_INCLUDED
#  define gximask_INCLUDED

#include "gxbitmap.h"
#include "gsropt.h"

#ifndef gx_device_DEFINED
#  define gx_device_DEFINED
typedef struct gx_device_s gx_device;
#endif
#ifndef gx_device_color_DEFINED
#  define gx_device_color_DEFINED
typedef struct gx_device_color_s gx_device_color;
#endif
#ifndef gx_clip_path_DEFINED
#  define gx_clip_path_DEFINED
typedef struct gx_clip_path_s gx_clip_path;
#endif

int gx_image_fill_masked_start(gx_device *dev, const gx_device_color *pdevc, const gx_clip_path *pcpath, 
			   gs_memory_t *mem, gx_device **cdev);

int gx_image_fill_masked_end(gx_device *dev, gx_device *tdev, const gx_device_color *pdevc);

int gx_image_fill_masked(gx_device *dev,
    const byte *data, int data_x, int raster, gx_bitmap_id id,
    int x, int y, int width, int height,
    const gx_device_color *pdcolor, int depth,
    gs_logical_operation_t lop, const gx_clip_path *pcpath);


#endif /* gximask_INCLUDED */
