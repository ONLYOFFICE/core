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

/* $Id: gzcpath.h 8022 2007-06-05 22:23:38Z giles $ */
/* Structure definitions for clipping paths */
/* Requires gzpath.h. */

#ifndef gzcpath_INCLUDED
#  define gzcpath_INCLUDED

#include "gxcpath.h"
#include "gzpath.h"

/*
 * The reference counting considerations for clip paths are the same as
 * for paths.  We need a separate reference count for the clip list,
 * since its existence and lifetime are not necessarily the same as
 * those of the path.
 */

typedef struct gx_clip_rect_list_s {
    rc_header rc;
    gx_clip_list list;
} gx_clip_rect_list;

#define private_st_clip_rect_list()	/* in gxcpath.c */\
  gs_private_st_ptrs_add0(st_clip_rect_list, gx_clip_rect_list,\
    "gx_clip_rect_list", clip_rect_list_enum_ptrs, clip_rect_list_reloc_ptrs,\
    st_clip_list, list)

/*
 * When the clip path consists of the intersection of two or more
 * source paths, we maintain the complete list paths, so that it
 * can be accurately output for high-level devices.
 */

typedef struct gx_cpath_path_list_s gx_cpath_path_list;

struct gx_cpath_path_list_s {
    gx_path path;
    rc_header rc;
    int rule;
    gx_cpath_path_list *next;
};

#define private_st_cpath_path_list() 	/* in gxcpath.c */\
  gs_private_st_suffix_add1(st_cpath_path_list, gx_cpath_path_list,\
    "gs_cpath_list", cpath_path_list_enum_ptrs, cpath_path_list_reloc_ptrs,\
    st_path, next)

/* gx_clip_path is a 'subclass' of gx_path. */
struct gx_clip_path_s {
    gx_path path;
    gx_clip_rect_list local_list;
    int rule;			/* rule for insideness of path */
    /* Anything within the inner_box is guaranteed to fall */
    /* entirely within the clipping path. */
    gs_fixed_rect inner_box;
    /* Anything outside the outer_box is guaranteed to fall */
    /* entirely outside the clipping path.  This is the same */
    /* as the path bounding box, widened to pixel boundaries. */
    gs_fixed_rect outer_box;
    gx_clip_rect_list *rect_list;
    bool path_valid;		/* path representation is valid */
    gx_cpath_path_list *path_list;
    /* The id changes whenever the clipping region changes. */
    gs_id id;
};

extern_st(st_clip_path);
#define public_st_clip_path()	/* in gxcpath.c */\
  gs_public_st_composite(st_clip_path, gx_clip_path, "clip_path",\
    clip_path_enum_ptrs, clip_path_reloc_ptrs)
#define st_clip_path_max_ptrs (st_path_max_ptrs + 1)

/* Inline accessors. */
#define gx_cpath_is_shared(pcpath)\
  ((pcpath)->rect_list->rc.ref_count > 1)

/* Define the structure for enumerating a clipping list. */
typedef enum {
    visit_left = 1,
    visit_right = 2
} cpe_visit_t;
typedef enum {
    cpe_scan, cpe_left, cpe_right, cpe_close, cpe_done
} cpe_state_t;
struct gs_cpath_enum_s {
    gs_path_enum path_enum;	/* used iff clipping path exists as a path, */
    /* must be first for subclassing */
    bool using_path;
    gx_clip_rect *visit;	/* scan pointer for finding next start */
    gx_clip_rect *rp;		/* scan pointer for current rectangle */
    cpe_visit_t first_visit;
    cpe_state_t state;
    bool have_line;
    gs_int_point line_end;
    bool any_rectangles;
};

#define private_st_cpath_enum()		/* in gxcpath.c */\
  gs_private_st_suffix_add2(st_cpath_enum, gs_cpath_enum, "gs_cpath_enum",\
    cpath_enum_enum_ptrs, cpath_enum_reloc_ptrs, st_path_enum,\
    visit, rp)

#endif /* gzcpath_INCLUDED */
