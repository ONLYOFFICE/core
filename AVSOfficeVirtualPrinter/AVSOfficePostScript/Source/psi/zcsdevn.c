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

/* $Id: zcsdevn.c 9043 2008-08-28 22:48:19Z giles $ */
/* DeviceN color space support */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gxcspace.h"		/* must precede gscolor2.h */
#include "gscolor2.h"
#include "gscdevn.h"
#include "gxcdevn.h"
#include "estack.h"
#include "ialloc.h"
#include "icremap.h"
#include "ifunc.h"
#include "igstate.h"
#include "iname.h"
#include "zht2.h"

/* Imported from gscdevn.c */
extern const gs_color_space_type gs_color_space_type_DeviceN;

#if 0
/* <array> .setdevicenspace - */
/* The current color space is the alternate space for the DeviceN space. */
static int
zsetdevicenspace(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    const ref *pcsa;
    gs_separation_name *names;
    gs_device_n_map *pmap;
    uint num_components;
    gs_color_space *pcs;
    gs_color_space *pacs;
    ref_colorspace cspace_old;
    gs_function_t *pfn;
    int code;

    /* Verify that we have an array as our input parameter */
    check_read_type(*op, t_array);
    if (r_size(op) < 4 || r_size(op) > 5)
	return_error(e_rangecheck);

    /* pcsa is a pointer to the color names array (element 1 in input array) */
    pcsa = op->value.const_refs + 1;
    if (!r_is_array(pcsa))
	return_error(e_typecheck);
    num_components = r_size(pcsa);
    if (num_components == 0)
	return_error(e_rangecheck);
    if (num_components > GS_CLIENT_COLOR_MAX_COMPONENTS)
	return_error(e_limitcheck);

    /* Check tint transform procedure.  Note: Cheap trick to get pointer to it.
       The tint transform procedure is element 3 in the input array */
    check_proc(pcsa[2]);
    
    /* The alternate color space has been selected as the current color space */
    pacs = gs_currentcolorspace(igs);

    code = gs_cspace_new_DeviceN(&pcs, num_components, pacs, imemory);
    if (code < 0)
	return code;
    names = pcs->params.device_n.names;
    pmap = pcs->params.device_n.map;
    pcs->params.device_n.get_colorname_string = gs_get_colorname_string;

    /* Pick up the names of the components */
    {
	uint i;
	ref sname;

	for (i = 0; i < num_components; ++i) {
	    array_get(imemory, pcsa, (long)i, &sname);
	    switch (r_type(&sname)) {
		case t_string:
		    code = name_from_string(imemory, &sname, &sname);
		    if (code < 0) {
			rc_decrement(pcs, ".setdevicenspace");
			return code;
		    }
		    /* falls through */
		case t_name:
		    names[i] = name_index(imemory, &sname);
		    break;
		default:
		    rc_decrement(pcs, ".setdevicenspace");
		    return_error(e_typecheck);
	    }
	}
    }

    /* Now set the current color space as DeviceN */

    cspace_old = istate->colorspace;
    /*
     * pcsa is a pointer to element 1 (2nd element)  in the DeviceN
     * description array.  Thus pcsa[2] is element #3 (4th element)
     * which is the tint transform.
     */
    istate->colorspace.procs.special.device_n.layer_names = pcsa[0];
    istate->colorspace.procs.special.device_n.tint_transform = pcsa[2];    
    pfn = ref_function(pcsa + 2);	/* See comment above */
    if (!pfn)
	code = gs_note_error(e_rangecheck);

    if (code < 0) {
	istate->colorspace = cspace_old;
	rc_decrement_only(pcs, "zsetdevicenspace");
	return code;
    }
    gs_cspace_set_devn_function(pcs, pfn);
    code = gs_setcolorspace(igs, pcs);
    /* release reference from construction */
    rc_decrement_only(pcs, "zsetdevicenspace");
    if (code < 0) {
	istate->colorspace = cspace_old;
	return code;
    }
    pop(1);
    return 0;
}

/* <name> .attachdevicenattributespace - */
/*
 * DeviceN and NChannel color spaces can have an attributes dict.  In the
 * attribute dict can be a Colorants dict which contains Separation color
 * spaces.  If the Colorant dict is present, the PS logic will build each of
 * the Separation color spaces in a temp gstate and then call this procedure
 * to attach the Separation color space to the DeviceN color space.
 * The parameter to this procedure is a colorant name.  The Separation
 * color space is in the current (temp) gstate.  The DeviceN color space is
 * in the next gstate down in the gstate list (pgs->saved).
 */
static int
zattachdevicenattributespace(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_separation_name sep_name;
    int code;

    /* Pick up the names of the component */
    switch (r_type(op)) {
	case t_string:
	    code = name_from_string(imemory, op, op);
	    if (code < 0) 
		return code;
	    /* falls through */
	case t_name:
	    sep_name = name_index(imemory, op);
	    break;
	default:
	    return_error(e_typecheck);
    }
    code = gs_attachattributecolorspace(sep_name, igs);
    pop(1);
    return code;
}
#endif

/* ------ Initialization procedure ------ */

const op_def zcsdevn_op_defs[] =
{
    op_def_begin_ll3(),
#if 0
    {"1.setdevicenspace", zsetdevicenspace},
    {"1.attachdevicenattributespace", zattachdevicenattributespace},
#endif
    op_def_end(0)
};
