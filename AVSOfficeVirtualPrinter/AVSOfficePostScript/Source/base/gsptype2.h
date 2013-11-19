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

/* $Id: gsptype2.h 8513 2008-01-29 17:02:50Z leonardo $ */
/* Client interface to PatternType 2 Patterns */

#ifndef gsptype2_INCLUDED
#  define gsptype2_INCLUDED

#include "gspcolor.h"
#include "gsdcolor.h"
#include "gxfixed.h"

/* ---------------- Types and structures ---------------- */

/* PatternType 2 template */

#ifndef gs_shading_t_DEFINED
#  define gs_shading_t_DEFINED
typedef struct gs_shading_s gs_shading_t;
#endif

typedef struct gs_pattern2_template_s {
    gs_pattern_template_common;
    const gs_shading_t *Shading;
} gs_pattern2_template_t;

#define private_st_pattern2_template() /* in gsptype2.c */\
  gs_private_st_suffix_add1(st_pattern2_template,\
    gs_pattern2_template_t, "gs_pattern2_template_t",\
    pattern2_template_enum_ptrs, pattern2_template_reloc_ptrs,\
    st_pattern_template, Shading)
#define st_pattern2_template_max_ptrs (st_pattern_template_max_ptrs + 1)

/* PatternType 2 instance */

#ifndef gx_device_color_DEFINED
#  define gx_device_color_DEFINED
typedef struct gx_device_color_s gx_device_color;
#endif

typedef struct gs_pattern2_instance_s {
    gs_pattern_instance_common;
    gs_pattern2_template_t template;
    bool shfill;
} gs_pattern2_instance_t;

#define private_st_pattern2_instance() /* in gsptype2.c */\
  gs_private_st_composite(st_pattern2_instance, gs_pattern2_instance_t,\
    "gs_pattern2_instance_t", pattern2_instance_enum_ptrs,\
    pattern2_instance_reloc_ptrs)

#ifndef gx_path_DEFINED
#define gx_path_DEFINED
typedef struct gx_path_s gx_path;
#endif

#ifndef gx_device_DEFINED
#define gx_device_DEFINED
typedef struct gx_device_s gx_device;
#endif

#ifndef gs_color_space_DEFINED
#  define gs_color_space_DEFINED
typedef struct gs_color_space_s gs_color_space;
#endif

#ifndef gx_clip_path_DEFINED
#  define gx_clip_path_DEFINED
typedef struct gx_clip_path_s gx_clip_path;
#endif

extern const gx_device_color_type_t gx_dc_pattern2;

#define gx_dc_type_pattern2 (&gx_dc_pattern2)

/* ---------------- Procedures ---------------- */

/*
 * We should provide a gs_cspace_build_Pattern2 procedure to construct
 * the color space, but we don't.
 */

/* Initialize a PatternType 2 pattern. */
void gs_pattern2_init(gs_pattern2_template_t *);

/* Check device color for Pattern Type 2. */
bool gx_dc_is_pattern2_color(const gx_device_color *pdevc);

/* Set the 'shfill' flag to a PatternType 2 pattern instance. */
int gs_pattern2_set_shfill(gs_client_color * pcc);

/* Transform a shading bounding box into device space. */
int gx_dc_pattern2_shade_bbox_transform2fixed(const gs_rect * rect,
	const gs_imager_state * pis, gs_fixed_rect * rfixed);

/* Get a shading bbox. Returns 1 on success. */
int gx_dc_pattern2_get_bbox(const gx_device_color * pdevc, gs_fixed_rect *bbox);

/* Checks whether a PatternType 2 has a shading BBox. */
int gx_dc_pattern2_color_has_bbox(const gx_device_color * pdevc);

/* Intersect a clipping path a shading BBox. */
int gx_dc_pattern2_clip_with_bbox_simple(const gx_device_color * pdevc, gx_device * pdev, 
				  gx_clip_path *cpath);
/* Check whether color is a shading with BBox. */
int gx_dc_pattern2_is_rectangular_cell(const gx_device_color * pdevc, gx_device * pdev, gs_fixed_rect *rect);

/* Get a shading color space. */
const gs_color_space *gx_dc_pattern2_get_color_space(const gx_device_color * pdevc);

/* Check device color for a possibly self-overlapping shading. */
bool gx_dc_pattern2_can_overlap(const gx_device_color *pdevc);

/* Check whether a pattern color has a background. */
bool gx_dc_pattern2_has_background(const gx_device_color *pdevc);
#endif /* gsptype2_INCLUDED */
