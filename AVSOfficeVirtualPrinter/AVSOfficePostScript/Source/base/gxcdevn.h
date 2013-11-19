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

/* $Id: gxcdevn.h 8022 2007-06-05 22:23:38Z giles $ */
/* Internal definitions for DeviceN color spaces */

#ifndef gxcdevn_INCLUDED
#  define gxcdevn_INCLUDED

#include "gsrefct.h"
#include "gxcindex.h"

/* Cache for DeviceN color.  Note that currently this is a 1-entry cache. */
#ifndef gs_device_n_map_DEFINED
#  define gs_device_n_map_DEFINED
typedef struct gs_device_n_map_s gs_device_n_map;
#endif
struct gs_device_n_map_s {
    rc_header rc;
    int (*tint_transform)(const float *in, float *out,
                          const gs_imager_state *pis, void *data);
    void *tint_transform_data;
    bool cache_valid;
    float tint[GS_CLIENT_COLOR_MAX_COMPONENTS];
    frac conc[GX_DEVICE_COLOR_MAX_COMPONENTS];
};
#define private_st_device_n_map() /* in gscdevn.c */\
  gs_private_st_ptrs1(st_device_n_map, gs_device_n_map, "gs_device_n_map",\
    device_n_map_enum_ptrs, device_n_map_reloc_ptrs, tint_transform_data)

/* Allocate and initialize a DeviceN map. */
int alloc_device_n_map(gs_device_n_map ** ppmap, gs_memory_t * mem,
                       client_name_t cname);

#ifndef gs_device_n_attributes_DEFINED
#  define gs_device_n_attributes_DEFINED
typedef struct gs_device_n_attributes_s gs_device_n_attributes;
#endif
struct gs_device_n_attributes_s {
    rc_header rc;
    gs_separation_name colorant_name;
    gs_color_space *cspace;
    struct gs_device_n_attributes_s * next;	/* Linked list */
};
#define private_st_device_n_attributes() /* in gscdevn.c */\
  gs_private_st_ptrs2(st_device_n_attributes, gs_device_n_attributes, "gs_device_n_attributes",\
    device_n_attributes_enum_ptrs, device_n_attributes_reloc_ptrs, cspace, next)

/* Check if we are using the alternate color space */
bool using_alt_color_space(const gs_state * pgs);

#endif /* gxcdevn_INCLUDED */
