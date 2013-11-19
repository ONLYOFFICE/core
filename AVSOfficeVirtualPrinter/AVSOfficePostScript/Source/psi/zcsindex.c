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

/* $Id: zcsindex.c 9043 2008-08-28 22:48:19Z giles $ */
/* Indexed color space support */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "gscolor.h"
#include "gsmatrix.h"		/* for gxcolor2.h */
#include "gxcspace.h"
#include "gxfixed.h"		/* ditto */
#include "gxcolor2.h"
#include "estack.h"
#include "ialloc.h"
#include "icsmap.h"
#include "igstate.h"
#include "ivmspace.h"
#include "store.h"


/* ------ Internal routines ------ */

/* Allocate, and prepare to load, the index or tint map. */
int
zcs_begin_map(i_ctx_t *i_ctx_p, gs_indexed_map ** pmap, const ref * pproc,
	      int num_entries,  const gs_color_space * base_space,
	      op_proc_t map1)
{
    gs_memory_t *mem = gs_state_memory(igs);
    int space = imemory_space((gs_ref_memory_t *)mem);
    int num_components = cs_num_components(base_space);
    int num_values = num_entries * num_components;
    gs_indexed_map *map;
    int code = alloc_indexed_map(&map, num_values, mem,
				 "setcolorspace(mapped)");
    es_ptr ep;

    if (code < 0)
	return code;
    *pmap = map;
    /* Map the entire set of color indices.  Since the */
    /* o-stack may not be able to hold N*4096 values, we have */
    /* to load them into the cache as they are generated. */
    check_estack(num_csme + 1);	/* 1 extra for map1 proc */
    ep = esp += num_csme;
    make_int(ep + csme_num_components, num_components);
    make_struct(ep + csme_map, space, map);
    ep[csme_proc] = *pproc;
    make_int(ep + csme_hival, num_entries - 1);
    make_int(ep + csme_index, -1);
    push_op_estack(map1);
    return o_push_estack;
}
