/* Copyright (C) 2001-2008 Artifex Software, Inc.
   All Rights Reserved.

   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: zcolor.c 9380 2009-01-21 15:34:55Z ken $ */
/* Color operators */
#include "memory_.h"
#include "math_.h"
#include "ghost.h"
#include "oper.h"
#include "dstack.h"	/* for systemdict */
#include "estack.h"
#include "ialloc.h"
#include "igstate.h"
#include "iutil.h"
#include "store.h"
#include "gscolor.h"	/* for gs_setgray and gs_setrgbcolor */
#include "gscsepr.h"	/* For declarartion of Separation functions */
#include "gscdevn.h"	/* For declarartion of DeviceN functions */
#include "gscpixel.h"	/* For declarartion of DevicePixel functions */
#include "gxfixed.h"
#include "gxmatrix.h"
#include "gzstate.h"
#include "gxdcolor.h"		/* for gxpcolor.h */
#include "gxdevice.h"
#include "gxdevmem.h"		/* for gxpcolor.h */
#include "gxcmap.h"
#include "gxcspace.h"
#include "gxcolor2.h"
#include "gxpcolor.h"
#include "idict.h"
#include "icolor.h"
#include "idparam.h"
#include "iname.h"
#include "iutil.h"
#include "ifunc.h"	/* For declaration of buildfunction */
#include "icsmap.h"
#include "ifunc.h"
#include "zht2.h"
#include "zcolor.h"	/* For the PS_colour_space_t structure */
#include "zcie.h"	/* For CIE space function declarations */
#include "zicc.h"	/* For declaration of seticc */

/* imported from gsht.c */
extern  void    gx_set_effective_transfer(gs_state *);

/* Essential forward declarations */
static int validate_spaces(i_ctx_t *i_ctx_p, ref *arr, int *depth);
static int setcolorspace_cont(i_ctx_t *i_ctx_p);
static int setcolor_cont(i_ctx_t *i_ctx_p);

/* define the number of stack slots needed for zcolor_remap_one */
const int   zcolor_remap_one_ostack = 4;
const int   zcolor_remap_one_estack = 3;


/* utility to test whether a Pattern instance uses a base space */
static inline bool
pattern_instance_uses_base_space(const gs_pattern_instance_t * pinst)
{
    return pinst->type->procs.uses_base_space(
                   pinst->type->procs.get_pattern(pinst) );
}

/*
 *  -   currentcolor   <param1>  ...  <paramN>
 *
 * Return the current color. <paramN> may be a dictionary or a null
 * object, if the current color space is a pattern color space. The
 * other parameters will be numeric.
 *
 * Note that the results of this operator differ slightly from those of
 * most currentcolor implementations. If a color component value is 
 * integral (e.g.: 0, 1), it will be pushed on the stack as an integer.
 * Most currentcolor implementations, including the earlier
 * implementation in Ghostscript, would push real objects for all
 * color spaces except indexed color space. The approach taken here is
 * equally legitimate, and avoids special handling of indexed color
 * spaces.
 */
static int
zcurrentcolor(i_ctx_t * i_ctx_p)
{
    os_ptr                  op = osp;
    const gs_color_space *  pcs = gs_currentcolorspace(igs);
    const gs_client_color * pcc = gs_currentcolor(igs);
    int                     i, n = cs_num_components(pcs);
    bool                    push_pattern = n < 0;

    /* check for pattern */
    if (push_pattern) {
        gs_pattern_instance_t * pinst = pcc->pattern;

        if (pinst == 0 || !pattern_instance_uses_base_space(pinst))
            n = 1;
        else
            n = -n;
    }

    /* check for sufficient space on the stack */
    push(n);
    op -= n - 1;

    /* push the numeric operands, if any */
    if (push_pattern)
        --n;
    for (i = 0; i < n; i++, op++) {
        float   rval = pcc->paint.values[i];
        int     ival = (int)rval;

        /* the following handles indexed color spaces */
        if (rval == ival && pcs->type->index == gs_color_space_index_Indexed)
            make_int(op, ival);
        else
            make_real(op, rval);
    }

    /* push the pattern dictionary or null object, if appropriate */
    if (push_pattern)
        *op = istate->pattern;

    return 0;
}

/*
 *  -   .currentcolorspace   <array>
 *
 * Return the current color space. Unlike the prior implementation, the
 * istate->color_space.array field will now always have a legitimate
 * (array) value.
 */
static int
zcurrentcolorspace(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;   /* required by "push" macro */
    int code;
    ref namestr,stref;
    byte *body;

    /* Adobe applications expect that the Device spaces (DeviceGray
     * DeviceRGB and DeviceCMYK) will always return the same array.
     * Not merely the same content but the same actual array. To do 
     * this we define the arrays at startup (see gs_cspace.ps), and 
     * recover them here by executing PostScript.
     */
    if (r_has_type(&istate->colorspace.array, t_name)) {
	name_string_ref(imemory, &istate->colorspace.array, &namestr);
	if (r_size(&namestr) == 10 && !memcmp(namestr.value.bytes, "DeviceGray", 10)) {
	    body = ialloc_string(32, "string");
	    if (body == 0)
		return_error(e_VMerror);
	    memcpy(body, "systemdict /DeviceGray_array get", 32);
	    make_string(&stref, a_all | icurrent_space, 32, body);
	} else {
	    if (r_size(&namestr) == 9 && !memcmp(namestr.value.bytes, "DeviceRGB", 9)) {
		body = ialloc_string(31, "string");
		if (body == 0)
		    return_error(e_VMerror);
		memcpy(body, "systemdict /DeviceRGB_array get", 31);
		make_string(&stref, a_all | icurrent_space, 31, body);
	    } else {
		if (r_size(&namestr) == 10 && !memcmp(namestr.value.bytes, "DeviceCMYK", 10)) {
		    body = ialloc_string(32, "string");
		    if (body == 0)
			return_error(e_VMerror);
		    memcpy(body, "systemdict /DeviceCMYK_array get", 32);
		    make_string(&stref, a_all | icurrent_space, 32, body);
		} else {
		    /* Not one of the Device spaces, but still just a name. Give 
		     * up and return the name on the stack.
		     */
		    push(1);
		    code = ialloc_ref_array(op, a_all, 1, "currentcolorspace");
		    if (code < 0)
			return code;
		    refset_null(op->value.refs, 1);
		    ref_assign_old(op, op->value.refs, &istate->colorspace.array, "currentcolorspace");
		    return 0;
		}
	    }
	}
	r_set_attrs(&stref, a_executable);
	esp++;
	ref_assign(esp, &stref);
	return o_push_estack;
    } else {
	/* If the space isn't a simple name, then we don't need any special
	 * action and can simply use it.
	 */
	push(1);
	*op = istate->colorspace.array;
    }
    return 0;
}

/*
 *  -   .getuseciecolor   <bool>
 *
 * Return the current setting of the use_cie_color graphic state parameter,
 * which tracks the UseCIEColor page device parameter. This parameter may be
 * read (via this operator) at all language leves, but may only be set (via
 * the .setuseciecolor operator; see zcolor3.c) only in language level 3.
 *
 * We handle this parameter separately from the page device primarily for
 * performance reasons (the parameter may be queried frequently), but as a
 * side effect achieve proper behavior relative to the language level. The
 * interpreter is always initialized with this parameter set to false, and
 * it can only be updated (via setpagedevice) in language level 3.
 */
static int
zgetuseciecolor(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;

    push(1);
    *op = istate->use_cie_color;
    return 0;
}

/*
 *  <param1>  ...  <paramN>   setcolor   -
 *
 * Set the current color. All of the parameters except the topmost (paramN) are
 * numbers; the topmost (and possibly only) entry may be pattern dictionary or
 * a null object.
 *
 * The use of one operator to set both patterns and "normal" colors is
 * consistent with Adobe's documentation, but primarily reflects the use of
 * gs_setcolor for both purposes in the graphic library. An alternate
 * implementation would use a .setpattern operator, which would interface with
 * gs_setpattern.
 *
 * This operator is hidden by a pseudo-operator of the same name, so it will
 * only be invoked under controlled situations. Hence, it does no operand
 * checking.
 */
static int
zsetcolor(i_ctx_t * i_ctx_p)
{
    os_ptr                  op = osp;
    es_ptr ep = esp;
    const gs_color_space *  pcs = gs_currentcolorspace(igs);
    gs_client_color         cc;
    int                     n_comps, n_numeric_comps, num_offset = 0, code, depth;
    bool                    is_ptype2 = 0;
    PS_colour_space_t *space;

    /* initialize the client color pattern pointer for GC */
    cc.pattern = 0; 

    /* check for a pattern color space */
    if ((n_comps = cs_num_components(pcs)) < 0) {
        n_comps = -n_comps;
        if (r_has_type(op, t_dictionary)) {
            ref     *pImpl, pPatInst;
            int     ptype;

	    code = dict_find_string(op, "Implementation", &pImpl);
	    if (code < 0)
		return code;
	    code = array_get(imemory, pImpl, 0, &pPatInst);
	    if (code < 0)
		return code;
	    cc.pattern = r_ptr(&pPatInst, gs_pattern_instance_t);
            n_numeric_comps = ( pattern_instance_uses_base_space(cc.pattern)
                                  ? n_comps - 1
                                  : 0 );
            (void)dict_int_param(op, "PatternType", 1, 2, 1, &ptype);
            is_ptype2 = ptype == 2;
        } else
            n_numeric_comps = 0;
        num_offset = 1;
    } else
        n_numeric_comps = n_comps;

    /* gather the numeric operands */
    code = float_params(op - num_offset, n_numeric_comps, cc.paint.values);
    if (code < 0)
	return code;

    code = get_space_object(i_ctx_p, &istate->colorspace.array, &space);
    if (code < 0)
	return code;
    if (space->validatecomponents) {
	code = space->validatecomponents(i_ctx_p, &istate->colorspace.array, cc.paint.values, n_numeric_comps);
	if (code < 0)
	    return code;
    }

    /* pass the color to the graphic library */
    if ((code = gs_setcolor(igs, &cc)) >= 0) {

        if (n_comps > n_numeric_comps) {
            istate->pattern = *op;      /* save pattern dict or null */
            n_comps = n_numeric_comps + 1;
        }
    }

    /* Check the color spaces, to see if we need to run any tint transform
     * procedures. Some Adobe applications *eg Photoshop) expect that the 
     * tint transform will be run and use this to set up duotone DeviceN
     * spaces.
     */
    code = validate_spaces(i_ctx_p, &istate->colorspace.array, &depth);
    if (code < 0)
	return code;
    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    /* A place holder for data potentially used by transform functions */
    ep = esp += 1;
    make_int(ep, 0);
    /* Store the 'depth' of the space returned during checking above */
    ep = esp += 1;
    make_int(ep, 0);
    /* Store the 'stage' of processing (initially 0) */
    ep = esp += 1;
    make_int(ep, 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    ep = esp += 1;
    *ep = istate->colorspace.array;
    /* Finally, the actual continuation routine */
    push_op_estack(setcolor_cont);
    return o_push_estack;
}

/*
 * Given two color space arrays, attempts to determine if they are the 
 * same space by comparing their contents recursively. For some spaces,
 * especially CIE based color spaces, it can significantly improve
 * performance if the same space is frequently re-used.
 */
static int is_same_colorspace(i_ctx_t * i_ctx_p, ref *space1, ref *space2)
{
    PS_colour_space_t *oldcspace = 0, *newcspace = 0;
    ref oldspace, *poldspace = &oldspace, newspace, *pnewspace = &newspace;
    int code, CIESubst;

    /* Silence compiler warnings */
    oldspace.tas.type_attrs = 0;
    oldspace.tas.type_attrs = 0;

    ref_assign(poldspace, space1);
    ref_assign(pnewspace, space2);
    do {
	if (r_type(poldspace) != r_type(pnewspace))
	    return 0;

	code = get_space_object(i_ctx_p, poldspace, &oldcspace);
	if (code < 0)
	    return 0;

	code = get_space_object(i_ctx_p, pnewspace, &newcspace);
	if (code < 0)
	    return 0;

	/* Check the two color space types are the same 
	 * (Indexed, Separation, DeviceCMYK etc).
	 */
	if (strcmp(oldcspace->name, newcspace->name) != 0)
	    return 0;

	/* Call the space-specific comparison routine */
	if (!oldcspace->compareproc(i_ctx_p, poldspace, pnewspace))
	    return 0;

	/* The current space is OK, if there is no alternate, then that's
	 * good enough. 
	 */
	if (!oldcspace->alternateproc)
	    break;

	/* Otherwise, retrieve the alternate space for each, and continue
	 * round the loop, checking those.
	 */
	code = oldcspace->alternateproc(i_ctx_p, poldspace, &poldspace, &CIESubst);
	if (code < 0)
	    return 0;

	code = newcspace->alternateproc(i_ctx_p, pnewspace, &pnewspace, &CIESubst);
	if (code < 0)
	    return 0;
    }
    while(1);   

    return 1;
}

/*
 *  <array>   setcolorspace   -
 *
 * Set the nominal color space. This color space will be pushd by the
 * currentcolorspace operator, but is not directly used to pass color
 * space information to the graphic library.
 *
 */
static int
zsetcolorspace(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;
    es_ptr ep = esp;
    int code, depth;

    /* Make sure we have an operand... */
    check_op(1);
    /* Check its either a name (base space) or an array */
    if (!r_has_type(op, t_name)) 
	if (!r_is_array(op)) 
	    return_error(e_typecheck);

    code = validate_spaces(i_ctx_p, op, &depth);
    if (code < 0)
	return code;

    /* See if its the same as the current space */
    if (is_same_colorspace(i_ctx_p, op, &istate->colorspace.array)) {
	PS_colour_space_t *cspace;

	/* Even if its the same space, we still need to set the correct
	 * initial color value.
	 */
	code = get_space_object(i_ctx_p, &istate->colorspace.array, &cspace);
	if (code < 0)
	    return 0;
	if (cspace->initialcolorproc) {
	    cspace->initialcolorproc(i_ctx_p, &istate->colorspace.array);
	}
	/* Pop the space off the stack */
	pop(1);
	return 0;
    }
    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    /* Store the initial value of CIE substitution (not substituting) */
    ep = esp += 1;
    make_int(ep, 0);
    /* Store the 'depth' of the space returned during checking above */
    ep = esp += 1;
    make_int(ep, depth);
    /* Store the 'stage' of processing (initially 0) */
    ep = esp += 1;
    make_int(ep, 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    ep = esp += 1;
    *ep = *op;
    /* Finally, the actual continuation routine */
    push_op_estack(setcolorspace_cont);
    return o_push_estack;
}

/*
 * A sepcial version of the setcolorspace operation above. This sets the 
 * CIE substitution flag to true before starting, which prevents any further
 * CIE substitution taking place.
 */
static int
setcolorspace_nosubst(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;
    es_ptr ep = esp;
    int code, depth;

    /* Make sure we have an operand... */
    check_op(1);
    /* Check its either a name (base space) or an array */
    if (!r_has_type(op, t_name)) 
	if (!r_is_array(op)) 
	    return_error(e_typecheck);

    code = validate_spaces(i_ctx_p, op, &depth);
    if (code < 0)
	return code;

    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    /* Store the initial value of CIE substitution (substituting) */
    ep = esp += 1;
    make_int(ep, 1);
    /* Store the 'depth' of the space returned during checking above */
    ep = esp += 1;
    make_int(ep, depth);
    /* Store the 'stage' of processing (initially 0) */
    ep = esp += 1;
    make_int(ep, 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    ep = esp += 1;
    *ep = *op;
    /* Finally, the actual continuation routine */
    push_op_estack(setcolorspace_cont);
    return o_push_estack;
}

/*
 *  <name> .includecolorspace -
 *
 * See the comment for gs_includecolorspace in gscolor2.c .
 */
static int
zincludecolorspace(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;
    ref nsref;
    int code;

    check_type(*op, t_name);
    name_string_ref(imemory, op, &nsref);
    code =  gs_includecolorspace(igs, nsref.value.const_bytes, r_size(&nsref));
    if (!code)
	pop(1);
    return code;
}

/*  -   currenttransfer   <proc> */
static int
zcurrenttransfer(i_ctx_t *i_ctx_p)
{
    os_ptr  op = osp;

    push(1);
    *op = istate->transfer_procs.gray;
    return 0;
}

/*
 *  -   processcolors   <int>  -
 *
 * Note: this is an undocumented operator that is not supported
 * in Level 2.
 */
static int
zprocesscolors(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;

    push(1);
    make_int(op, gs_currentdevice(igs)->color_info.num_components);
    return 0;
}

/* <proc> settransfer - */
static int
zsettransfer(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;
    int     code;

    check_proc(*op);
    check_ostack(zcolor_remap_one_ostack - 1);
    check_estack(1 + zcolor_remap_one_estack);
    istate->transfer_procs.red =
        istate->transfer_procs.green =
        istate->transfer_procs.blue =
        istate->transfer_procs.gray = *op;
    if ((code = gs_settransfer_remap(igs, gs_mapped_transfer, false)) < 0)
        return code;
    push_op_estack(zcolor_reset_transfer);
    pop(1);
    return zcolor_remap_one( i_ctx_p,
                             &istate->transfer_procs.gray,
                             igs->set_transfer.gray,
                             igs,
                             zcolor_remap_one_finish );
}


/*
 * Internal routines
 */

/*
 * Prepare to remap one color component (also used for black generation
 * and undercolor removal). Use the 'for' operator to gather the values.
 * The caller must have done the necessary check_ostack and check_estack.
 */
int
zcolor_remap_one(
    i_ctx_t *           i_ctx_p,
    const ref *         pproc,
    gx_transfer_map *   pmap,
    const gs_state *    pgs,
    op_proc_t           finish_proc )
{
    os_ptr              op;

    /*
     * Detect the identity function, which is a common value for one or
     * more of these functions.
     */
    if (r_size(pproc) == 0) {
	gx_set_identity_transfer(pmap);
	/*
	 * Even though we don't actually push anything on the e-stack, all
	 * clients do, so we return o_push_estack in this case.  This is
	 * needed so that clients' finishing procedures will get run.
	 */
	return o_push_estack;
    }
    op = osp += 4;
    make_real(op - 3, 0);
    make_int(op - 2, transfer_map_size - 1);
    make_real(op - 1, 1);
    *op = *pproc;
    ++esp;
    make_struct(esp, imemory_space((gs_ref_memory_t *) pgs->memory),
		pmap);
    push_op_estack(finish_proc);
    push_op_estack(zfor_samples);
    return o_push_estack;
}

/* Store the result of remapping a component. */
static int
zcolor_remap_one_store(i_ctx_t *i_ctx_p, floatp min_value)
{
    int i;
    gx_transfer_map *pmap = r_ptr(esp, gx_transfer_map);

    if (ref_stack_count(&o_stack) < transfer_map_size)
	return_error(e_stackunderflow);
    for (i = 0; i < transfer_map_size; i++) {
	double v;
	int code =
	    real_param(ref_stack_index(&o_stack, transfer_map_size - 1 - i),
		       &v);

	if (code < 0)
	    return code;
	pmap->values[i] =
	    (v < min_value ? float2frac(min_value) :
	     v >= 1.0 ? frac_1 :
	     float2frac(v));
    }
    ref_stack_pop(&o_stack, transfer_map_size);
    esp--;			/* pop pointer to transfer map */
    return o_pop_estack;
}
int
zcolor_remap_one_finish(i_ctx_t *i_ctx_p)
{
    return zcolor_remap_one_store(i_ctx_p, 0.0);
}
int
zcolor_remap_one_signed_finish(i_ctx_t *i_ctx_p)
{
    return zcolor_remap_one_store(i_ctx_p, -1.0);
}

/* Finally, reset the effective transfer functions and */
/* invalidate the current color. */
int
zcolor_reset_transfer(i_ctx_t *i_ctx_p)
{
    gx_set_effective_transfer(igs);
    return zcolor_remap_color(i_ctx_p);
}
int
zcolor_remap_color(i_ctx_t *i_ctx_p)
{
    gx_unset_dev_color(igs);
    return 0;
}

/* 
 * <param1> ... <paramN> .color_test <param1> ... <paramN>
 *
 * encode and decode color to allow mapping to be tested.
 */
static int
zcolor_test(i_ctx_t *i_ctx_p)
{
    gx_color_value cv[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_device *dev = gs_currentdevice(igs);
    int ncomp = dev->color_info.num_components;
    gx_color_index color;
    os_ptr op = osp - (ncomp-1);
    int i;
    if (ref_stack_count(&o_stack) < ncomp)
	return_error(e_stackunderflow);
    for (i = 0; i < ncomp; i++) {
	if (r_has_type(op+i, t_real))
	    cv[i] = (gx_color_value)
		(op[i].value.realval * gx_max_color_value);
	else if (r_has_type(op+i, t_integer))
	    cv[i] = (gx_color_value)
		(op[i].value.intval * gx_max_color_value);
	else
	    return_error(e_typecheck);
    }
    color = (*dev_proc(dev, encode_color)) (dev, cv);
    (*dev_proc(dev, decode_color)) (dev, color, cv);
    for (i = 0; i < ncomp; i++)
        make_real(op+i, (float)cv[i] / (float)gx_max_color_value);
    return 0;
}

/* 
 * <levels> .color_test_all <value0> ... <valueN>
 *
 * Test encode/decode color procedures for a range of values.
 * Return value with the worst error in a single component.
 */
static int
zcolor_test_all(i_ctx_t *i_ctx_p)
{
    os_ptr                  op = osp;
    gx_color_value cv[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_value cvout[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_value cvbad[GX_DEVICE_COLOR_MAX_COMPONENTS];
    int counter[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_device *dev = gs_currentdevice(igs);
    int ncomp = dev->color_info.num_components;
    int steps;
    int maxerror = 0;
    int err;
    int acceptable_error;
    int linsep = dev->color_info.separable_and_linear == GX_CINFO_SEP_LIN;
    int linsepfailed = 0;
    int lsmaxerror = 0;
    gx_color_index color, lscolor;
    int i, j, k;
    int finished = 0;

    if (ncomp == 1) 
	acceptable_error = gx_max_color_value / dev->color_info.max_gray + 1;
    else
	acceptable_error = gx_max_color_value / dev->color_info.max_color + 1;

    if (ref_stack_count(&o_stack) < 1)
	return_error(e_stackunderflow);
    if (!r_has_type(&osp[0], t_integer))
        return_error(e_typecheck);
    steps = osp[0].value.intval;
    for (i = 0; i < ncomp; i++) {
        counter[i] = 0; 
	cvbad[i] = 0;
    }

    dprintf1("Number of components = %d\n", ncomp);
    dprintf1("Depth = %d\n", dev->color_info.depth);
    dprintf2("max_gray = %d   dither_grays = %d\n", 
	dev->color_info.max_gray, dev->color_info.dither_grays);
    dprintf2("max_color = %d   dither_colors = %d\n", 
 	dev->color_info.max_color, dev->color_info.dither_colors);
    dprintf1("polarity = %s\n", 
      dev->color_info.polarity == GX_CINFO_POLARITY_ADDITIVE ? "Additive" :
      dev->color_info.polarity == GX_CINFO_POLARITY_SUBTRACTIVE ?"Subtractive":
      "Unknown");
    /* Indicate color index value with all colorants = zero */
    for (i = 0; i < ncomp; i++)
	cv[i] = 0;
    color = (*dev_proc(dev, encode_color)) (dev, cv);
    if (sizeof(color) <= sizeof(ulong))
        dprintf1("Zero color index:  %8lx\n", (ulong)color);
    else
        dprintf2("Zero color index:  %8lx%08lx\n",
            (ulong)(color >> 8*(sizeof(color) - sizeof(ulong))), (ulong)color);

    dprintf1("separable_and_linear = %s\n", 
      linsep == GX_CINFO_SEP_LIN_NONE ? "No" :
      linsep == GX_CINFO_SEP_LIN ? "Yes" :
      "Unknown");
    if (dev->color_info.gray_index == GX_CINFO_COMP_INDEX_UNKNOWN)
        dprintf("gray_index is unknown\n");
    else
        dprintf1("gray_index = %d\n", dev->color_info.gray_index);
    if (linsep) {
        dprintf(" Shift     Mask  Bits\n");
        for (i = 0; i < ncomp; i++) {
            dprintf3(" %5d %8x  %4d\n",
		(int)(dev->color_info.comp_shift[i]),
		(int)(dev->color_info.comp_mask[i]),
		(int)(dev->color_info.comp_bits[i]));
        }
    }

    while (!finished) {
	for (j = 0; j <= steps; j++) {
	    for (i = 0; i < ncomp; i++)
		cv[i] = counter[i] * gx_max_color_value / steps;
	    color = (*dev_proc(dev, encode_color)) (dev, cv);
	    if (linsep) {
		/* Derive it the other way */
		lscolor = gx_default_encode_color(dev, cv);
		if ((color != lscolor) && (linsepfailed < 5)) {
		    linsepfailed++;
		    dprintf("Failed separable_and_linear for");
		    for (i = 0; i < ncomp; i++)
			dprintf1(" %d", cv[i]);
		    dprintf("\n");
		    dprintf2("encode_color=%x  gx_default_encode_color=%x\n",
			(int)color, (int)lscolor);
		}
	    }
	    (*dev_proc(dev, decode_color)) (dev, color, cvout);
	    for (i = 0; i < ncomp; i++) {
		err = (int)cvout[i] - (int)cv[i];
		if (err < 0)
		    err = -err;
		if (err > maxerror) {
		    maxerror = err;
		    for (k=0; k < ncomp; k++)
			cvbad[k] = cv[k];
		}
	    }
	    if (linsep) {
	        gx_default_decode_color(dev, color, cvout);
		for (i = 0; i < ncomp; i++) {
		    err = (int)cvout[i] - (int)cv[i];
		    if (err < 0)
			err = -err;
		    if (err > lsmaxerror) {
			lsmaxerror = err;
		    }
		}
	    }
	    counter[0] += 1;
	}
	counter[0] = 0;
	i = 1;
	while (i < ncomp) {
	    counter[i] += 1;
	    if (counter[i] > steps) {
		counter[i] = 0;
		i++;
	    }
	    else
		break;
	}
	if (i >= ncomp)
	    finished = 1;
    }

    dprintf2("Maximum error %g %s\n", 
	(float)maxerror / (float)gx_max_color_value,
	maxerror <= acceptable_error ? "is Ok" :
	maxerror <= 3*acceptable_error/2 ? "is POOR" : "FAILED");

    if (linsep)
      dprintf2("Maximum linear_and_separable error %g %s\n", 
	(float)lsmaxerror / (float)gx_max_color_value,
	lsmaxerror <= acceptable_error ? "is Ok" :
	lsmaxerror <= 3*acceptable_error/2 ? "is POOR" : "FAILED");

    /* push worst value */
    push(ncomp-1);
    op -= ncomp - 1;
    for (i = 0; i < ncomp; i++)
        make_real(op+i, (float)cvbad[i] / (float)gx_max_color_value);

    return 0;
}

/* Convert between RGB and HSB colors, using the hexcone approach (see
 * Rogers, David, "Procedureal Elements For Computer Graphics",
 * (McGraw-Hill, 1985), pp. 402 - 3).
 *
 * The rgb ==> hsb calculation is:
 *
 *   br = max(r, g, b)
 *
 *   if (br == 0)
 *       h = 0, s = 0;
 *   else {
 *       v = min(r, g, b)
 *       diff = br - v;
 *       sat = diff / br;
 *       if (r == br)
 *           h = (g - b) / (6 * diff) + (b > g ? 1 : 0);
 *       else if (g == br)
 *           h = 1/3 + (b - r) / (6 * diff);
 *       else    
 *           h = 2/3 + (r - g) / (6 * diff);
 *   }
 */
static int rgb2hsb(float *RGB)
{
    float HSB[3], v, diff;
    int i, j=0;

    v = 1.0;
    for (i=0;i<3;i++)
	HSB[i] = 0.0;
    for (i=0;i<3;i++) {
	if (RGB[i] > HSB[2]) {
	    HSB[2] = RGB[i];
	    j = i;
	}
	if (RGB[i] < v)
	    v = RGB[i];
    }
    if (HSB[2] != 0) {
	diff = HSB[2] - v;
	HSB[1] = diff / HSB[2];
	switch (j) {
	    case 0 : /* R == Brightness */
		/* diff can only be zero if r == br, so we need to make sure here we
		 * don't divide by zero
		 */
		if (diff)
		    HSB[0] = ((RGB[1] - RGB[2]) / (6.0 * diff)) + (RGB[2] > RGB[1] ? 1.0 : 0.0);
		else
		    HSB[0] = (RGB[1] - RGB[2]) + (RGB[2] > RGB[1] ? 1.0 : 0.0);
		break;
	    case 1 : /* G == Brightness */
		HSB[0] = (1.0 / 3.0) + (RGB[2] - RGB[0]) / (6.0 * diff);
		break;
	    case 2 : /* B == Brightness */
		HSB[0] = (2.0 / 3.0) + (RGB[0] - RGB[1]) / (6.0 * diff);
		break;
	}
    }
    for (i=0;i<3;i++) {
	if (HSB[i] < 0)
	    HSB[i] = 0;
	if (RGB[i] > 1)
	    HSB[i] = 1;
	RGB[i] = HSB[i];
    }
    return 0;
}
/* The hsb ==> rgb conversion is:
 *
 *    mn = (1 - s) * br, md = 6 * s * br;
 *
 *    switch ((int)floor(6 * h)) {
 *      case 0:   %% r >= g >= b 
 *        r = br;
 *        g = mn + h * md;
 *        b = mn;
 *        break;
 *
 *      case 1:  %% g >= r >= b 
 *        r = mn + md * (1/3 - h);
 *        g = br;
 *        b = mn;
 *        break;
 *
 *      case 2:  %% g >= b >= r 
 *        r = mn;
 *        g = br;
 *        b = mn + (h - 1/3) * md;
 *        break;
 *
 *      case 3:  %% b >= g >= r 
 *        r = mn;
 *        g = mn + (2/3 - h) * md;
 *        b = br;
 *        break;
 *
 *      case 4:  %% b >= r >= g 
 *        r = mn + (h - 2/3) * md;
 *        g = mn;
 *        b = br;
 *        break;
 *
 *      case 5:  %% r >= b >= g 
 *        r = br;
 *        g = mn;
 *        b = mn + (1 - h) * md;
 *        break;
 *
 *      case 6:  %% We have wrapped around the hexcone.  Thus this case is
 *      		   the same as case 0 with h = 0 
 *        h = 0;
 *        r = br;
 *        g = mn + h * md = mn;
 *        b = mn;
 *        break;
 *    }
 */
static int hsb2rgb(float *HSB)
{
    float RGB[3], mn, md;
    int i;

    mn = (1.0 - HSB[1]) * HSB[2];
    md = 6.0 * HSB[1] * HSB[2];

    switch ((int)floor(6.0 * HSB[0])) {
	case 6:
	    HSB[0] = (float)0;
	case 0:
	    RGB[0] = HSB[2];
	    RGB[1] = mn + (HSB[0] * md);
	    RGB[2] = mn;
	    break;
	case 1:
	    RGB[0] = mn + (md * ((1.0 / 3.0) - HSB[0]));
	    RGB[1] = HSB[2];
	    RGB[2] = mn;
	    break;
	case 2:
	    RGB[0] = mn;
	    RGB[1] = HSB[2];
	    RGB[2] = mn + ((HSB[0] - (1.0 / 3.0)) * md);
	    break;
	case 3:
	    RGB[0] = mn;
	    RGB[1] = mn + (((2.0 / 3.0f) - HSB[0]) * md);
	    RGB[2] = HSB[2];
	    break;
	case 4:
	    RGB[0] = mn + ((HSB[0] - (2.0 / 3.0)) * md);
	    RGB[1] = mn;
	    RGB[2] = HSB[2];
	    break;
	case 5:
	    RGB[0] = HSB[2];
	    RGB[1] = mn;
	    RGB[2] = mn + ((1.0 - HSB[0]) * md);
	    break;
    }
    for (i=0;i<3;i++) {
	if (RGB[i] < 0)
	    RGB[i] = 0;
	if (RGB[i] > 1)
	    RGB[i] = 1;
	HSB[i] = RGB[i];
    }
    return 0;
}

/* The routiens for handling colors and color spaces, moved from 
 * PostScript to C, start here.
 */

/* DeviceGray */
static int setgrayspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;
    gs_color_space  *pcs;
    int code=0;
    ref stref;

    do {
	switch (*stage) {
	    case 0:
		if (istate->use_cie_color.value.boolval && !CIESubst) {
		    byte *body;
		    ref *nosubst;

		    code = dict_find_string(systemdict, "NOSUBSTDEVICECOLORS", &nosubst);
		    if (code < 0)
			return code;
		    if (!r_has_type(nosubst, t_boolean))
    			return_error(e_typecheck);
		    if (nosubst->value.boolval) {
			*stage = 4;
			*cont = 1;
			body = ialloc_string(32, "string");
			if (body == 0)
			    return_error(e_VMerror);
			memcpy(body, "/DefaultGray ..nosubstdevicetest",32);
			make_string(&stref, a_all | icurrent_space, 32, body);
			r_set_attrs(&stref, a_executable);
			esp++;
			ref_assign(esp, &stref);
			return o_push_estack;
		    } else {
			*stage = 2;
			*cont = 1;
			body = ialloc_string(47, "string");
			if (body == 0)
			    return_error(e_VMerror);
			memcpy(body, "{/DefaultGray /ColorSpace findresource} stopped",47);
			make_string(&stref, a_all | icurrent_space, 47, body);
			r_set_attrs(&stref, a_executable);
			esp++;
			ref_assign(esp, &stref);
			return o_push_estack;
		    }
		    break;
		}
		/* fall through */
	    case 1:
		pcs = gs_cspace_new_DeviceGray(imemory);
		if (pcs == NULL)
		    return_error(e_VMerror);
		code = gs_setcolorspace(igs, pcs);
		if (code >= 0) {
		    gs_client_color *   pcc = igs->ccolor;

		    cs_adjust_color_count(igs, -1); /* not strictly necessary */
		    pcc->paint.values[0] = (0);
		    pcc->pattern = 0;		/* for GC */
		    gx_unset_dev_color(igs);
		}
		rc_decrement_only(pcs, "zsetdevcspace");
		*cont = 0;
		*stage = 0;
		break;
	    case 2:
		if (!r_has_type(op, t_boolean))
		    return_error(e_typecheck);
		if (op->value.boolval) {
		    /* Failed to find the /DefaultGray CSA, so give up and
		     * just use DeviceGray
		     */
		    pop(1);
		    *stage = 1;
		    break;
		}
		pop(1);
		*cont = 1;
    		*stage = 3;
		code = setcolorspace_nosubst(i_ctx_p);
		if (code != 0)
		    return code;
		break;
	    case 3:
		/* We end up here after setting the DefaultGray space
		 * We've finished setting the gray color space, so we
		 * just exit now
		 */
		*cont = 0;
		*stage = 0;
		break;
	    case 4:
		/* We come here if /UseCIEColor is true, and NOSUBSTDEVICECOLORS
		 * is also true. We will have a boolean on the stack, if its true
		 * then we need to set the space (also on the stack), invoke
		 * .includecolorspace, and set /DeviceGray, otherwise we just need
		 * to set DeviceGray. See gs_cspace.ps.
		 */
		if (!r_has_type(op, t_boolean))
		    return_error(e_typecheck);
		pop(1);
		*stage = 1;
		*cont = 1;
		if (op->value.boolval) {
		    *stage = 5;
		    code = setcolorspace_nosubst(i_ctx_p);
		    if (code != 0)
			return code;
		} 
		break;
	    case 5:
		/* After stage 4 above, if we had to set a color space, we come
		 * here. Now we need to use .includecolorspace to register the space
		 * with any high-level devices which want it.
		 */
		*stage = 1;
		*cont = 1;
		code = zincludecolorspace(i_ctx_p);
		if (code != 0)
		    return code;
		break;
	}
    } while (*stage);
    return code;
}
static int graydomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    ptr[0] = 0;
    ptr[1] = 1;
    return 0;
}
static int grayrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    ptr[0] = 0;
    ptr[1] = 1;
    return 0;
}
/* This routine converts a Gray value into its equivalent in a different
 * device space, required by currentgray, currentrgb, currenthsb and
 * currentcmyk. The actual color value will have been processed through
 * the tint transform(s) of the parent space(s) until it reaches a device
 * space. This converts that final value into the requested space.
 */
static int graybasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op = osp;
    float Gray, RGB[3];

    *cont = 0;
    *stage = 0;
    check_op(1);
    if (!r_has_type(op, t_integer)) {
	if (r_has_type(op, t_real)) {
	    Gray = op->value.realval;
	} else
	    return_error(e_typecheck);
    } else
	Gray = (float)op->value.intval;

    if (Gray < 0 || Gray > 1)
	return_error(e_rangecheck);

    switch (base) {
	case 0:
	    /* Requested space is DeviceGray, just use the value */
            make_real(op, Gray);
	    break;
	case 1:
	    /* Requested space is HSB */
	case 2:
	    /* Requested space is RGB, set all the components
	     * to the gray value
	     */
	    push(2);
	    RGB[0] = RGB[1] = RGB[2] = Gray;
	    if (base == 1)
		/* If the requested space is HSB, convert the RGB to HSB */
		rgb2hsb((float *)&RGB);
            make_real(&op[-2], RGB[0]);
            make_real(&op[-1], RGB[1]);
            make_real(op, RGB[2]);
	    break;
	case 3:
	    /* Requested space is CMYK, use the gray value to set the 
	     * black channel.
	     */
	    push(3);
            make_real(&op[-3], (float)0);
            make_real(&op[-2], (float)0);
            make_real(&op[-1], (float)0);
            make_real(op, (float)1.0 - Gray);
	    break;
	default:
	    return_error(e_undefined);
    }
    return 0;
}
static int grayvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;

    if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	return_error(e_typecheck);

    if (num_comps < 1)
	return_error(e_stackunderflow);

    if (*values > 1.0)
	*values = 1.0;

    if ( *values < 0.0)
	*values = 0.0;

    return 0;
}
static int grayinitialproc(i_ctx_t *i_ctx_p, ref *space)
{
    gs_client_color cc;

    cc.pattern = 0x00;
    cc.paint.values[0] = 0;
    return gs_setcolor(igs, &cc);
}

/* DeviceRGB */
static int setrgbspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;
    gs_color_space  *pcs;
    int code=0;
    ref stref;

    do {
	switch (*stage) {
	    case 0:
		if (istate->use_cie_color.value.boolval && !CIESubst) {
		    byte *body;
		    ref *nosubst;

		    code = dict_find_string(systemdict, "NOSUBSTDEVICECOLORS", &nosubst);
		    if (code < 0)
			return code;
		    if (!r_has_type(nosubst, t_boolean))
    			return_error(e_typecheck);
		    if (nosubst->value.boolval) {
			*stage = 4;
			*cont = 1;
			body = ialloc_string(31, "string");
			if (body == 0)
			    return_error(e_VMerror);
			memcpy(body, "/DefaultRGB ..nosubstdevicetest",31);
			make_string(&stref, a_all | icurrent_space, 31, body);
			r_set_attrs(&stref, a_executable);
			esp++;
			ref_assign(esp, &stref);
			return o_push_estack;
		    } else {
    			*stage = 2;
			*cont = 1;
			body = ialloc_string(46, "string");
			if (body == 0)
			    return_error(e_VMerror);
			memcpy(body, "{/DefaultRGB /ColorSpace findresource} stopped", 46);
			make_string(&stref, a_all | icurrent_space, 46, body);
			r_set_attrs(&stref, a_executable);
			esp++;
			ref_assign(esp, &stref);
			return o_push_estack;
		    }
		}
		/* fall through */
	    case 1:
		pcs = gs_cspace_new_DeviceRGB(imemory);
		if (pcs == NULL)
		    return_error(e_VMerror);
		code = gs_setcolorspace(igs, pcs);
		if (code >= 0) {
		    gs_client_color *   pcc = igs->ccolor;

		    cs_adjust_color_count(igs, -1); /* not strictly necessary */
		    pcc->paint.values[0] = 0;
		    pcc->paint.values[1] = 0;
		    pcc->paint.values[2] = 0;
		    pcc->pattern = 0;		/* for GC */
		    gx_unset_dev_color(igs);
		}
		rc_decrement_only(pcs, "zsetdevcspace");
		*cont = 0;
		*stage = 0;
		break;
	    case 2:
		if (!r_has_type(op, t_boolean))
		    return_error(e_typecheck);
		if (op->value.boolval) {
		    /* Failed to find the /DefaultRGB CSA, so give up and
		     * just use DeviceRGB
		     */
		    pop(1);
		    *stage = 1;
		    break;
		}
		pop(1);
    		*stage = 3;
		code = setcolorspace_nosubst(i_ctx_p);
		if (code != 0)
		    return code;
		break;
	    case 3:
		/* We end up here after setting the DefaultGray CIE space
		 * We've finished setting the gray color space, so we
		 * just exit now
		 */
		*cont = 0;
		*stage = 0;
		break;
	    case 4:
		/* We come here if /UseCIEColor is true, and NOSUBSTDEVICECOLORS
		 * is also true. We will have a boolean on the stack, if its true
		 * then we need to set the space (also on the stack), invoke
		 * .includecolorspace, and set /DeviceGray, otherwise we just need
		 * to set DeviceGray. See gs-cspace.ps.
		 */
		if (!r_has_type(op, t_boolean))
		    return_error(e_typecheck);
		pop(1);
		*stage = 1;
		*cont = 1;
		if (op->value.boolval) {
		    *stage = 5;
		    code = setcolorspace_nosubst(i_ctx_p);
		    if (code != 0)
			return code;
		} 
		break;
	    case 5:
		/* After stage 4 above, if we had to set a color space, we come
		 * here. Now we need to use .includecolorspace to register the space
		 * with any high-level devices which want it.
		 */
		*stage = 1;
		*cont = 1;
		code = zincludecolorspace(i_ctx_p);
		if (code != 0)
		    return code;
		break;
	}
    } while (*stage);
    return code;
}
static int rgbdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i;

    for (i = 0;i < 6;i+=2) {
	ptr[i] = 0;
	ptr[i+1] = 1;
    }
    return 0;
}
static int rgbrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i;

    for (i = 0;i < 6;i+=2) {
	ptr[i] = 0;
	ptr[i+1] = 1;
    }
    return 0;
}
/* This routine converts an RGB value into its equivalent in a different
 * device space, required by currentgray, currentrgb, currenthsb and
 * currentcmyk. The actual color value will have been processed through
 * the tint transform(s) of the parent space(s) until it reaches a device
 * space. This converts that final value into the requested space.
 */
static int rgbbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op = osp;
    float RGB[3], CMYK[4], Gray, UCR, BG;
    int i;

    switch (*stage) {
	case 0:
	    *cont = 0;
	    check_op(3);
	    op -= 2;
	    for (i=0;i<3;i++) {
		if (!r_has_type(op, t_integer)) {
    		    if (r_has_type(op, t_real)) {
			RGB[i] = op->value.realval;
		    } else
			return_error(e_typecheck);
		} else
		    RGB[i] = (float)op->value.intval;
		if (RGB[i] < 0 || RGB[i] > 1)
		    return_error(e_rangecheck);
		op++;
	    }
	    op = osp;

	    switch (base) {
		case 0:
		    pop(2);
		    op = osp;
		    /* If R == G == B, then this is gray, so just use it. Avoids
		     * rounding errors.
		     */
		    if (RGB[0] == RGB[1] && RGB[1] == RGB[2])
			Gray = RGB[0];
		    else
			Gray = (0.3 * RGB[0]) + (0.59 * RGB[1]) + (0.11 * RGB[2]);
		    make_real(op, Gray);
		    return 0;
		    break;
		case 1:
		    rgb2hsb((float *)&RGB);
		    make_real(&op[-2], RGB[0]);
		    make_real(&op[-1], RGB[1]);
		    make_real(op, RGB[2]);
		    return 0;
		    break;
		case 2:
		    make_real(&op[-2], RGB[0]);
		    make_real(&op[-1], RGB[1]);
		    make_real(op, RGB[2]);
		    return 0;
		    break;
		case 3:
		    *stage = 1;
		    *cont = 1;
		    for (i=0;i<3;i++)
			CMYK[i] = 1 - RGB[i];
		    if (CMYK[0] < CMYK[1]) {
			if (CMYK[0] < CMYK[2])
			    CMYK[3] = CMYK[0];
			else
			    CMYK[3] = CMYK[2];
		    } else {
			if (CMYK[1] < CMYK[2])
    			    CMYK[3] = CMYK[1];
			else
			    CMYK[3] = CMYK[2];
		    }
		    check_estack(1);
		    push(2);
		    op = osp - 4;
		    for (i=0;i<4;i++) {
			make_real(op, CMYK[i]);
			op++;
		    }
		    make_real(op, CMYK[3]);
		    esp++;
		    *esp = istate->undercolor_removal;	
		    return o_push_estack;
		    break;
		default:
		    return_error(e_undefined);
		    break;
	    }
	    break;
	case 1:
	    (*stage)++;
	    *cont = 1;
	    check_estack(1);
	    check_op(5);
	    op -= 4;
	    for (i=0;i<4;i++) {
		if (!r_has_type(op, t_integer)) {
    		    if (r_has_type(op, t_real)) {
			CMYK[i] = op->value.realval;
		    } else
    			return_error(e_typecheck);
		} else
		    CMYK[i] = (float)op->value.intval;
		op++;
	    }
	    if (!r_has_type(op, t_integer)) {
    	        if (r_has_type(op, t_real)) {
		    UCR = op->value.realval;
		} else
    		    return_error(e_typecheck);
	    } else
	        UCR = (float)op->value.intval;
	    for (i=0;i<3;i++) {
		CMYK[i] = CMYK[i] - UCR;
		if (CMYK[i] < 0)
		    CMYK[i] = 0;
		if (CMYK[i] > 1)
		    CMYK[i] = 1.0;
	    }
	    op -= 4;
	    for (i=0;i<4;i++) {
		make_real(op, CMYK[i]);
		op++;
	    }
	    make_real(op, CMYK[3]);
	    esp++;
	    *esp = istate->black_generation;	
	    return o_push_estack;
	    break;
	case 2:
	    *stage = 0;
	    *cont = 0;
	    check_op(5);
	    if (!r_has_type(op, t_integer)) {
    	        if (r_has_type(op, t_real)) {
		    BG = op->value.realval;
		} else
    		    return_error(e_typecheck);
	    } else
	        BG = (float)op->value.intval;
	    pop(1);
	    op = osp;
	    if (BG < 0)
		BG = 0;
	    if (BG > 1)
		BG = 1;
	    make_real(op, BG);
	    break;
    }
    return 0;
}
static int rgbvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;
    int i;

    if (num_comps < 3)
	return_error(e_stackunderflow);

    op -= 2;
    for (i=0;i<3;i++) {
	if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	    return_error(e_typecheck);
	op++;
    }

    for (i=0;i < 3; i++) {
	if (values[i] > 1.0)
	    values[i] = 1.0;

	if (values[i] < 0.0)
	    values[i] = 0.0;
    }

    return 0;
}
static int rgbinitialproc(i_ctx_t *i_ctx_p, ref *space)
{
    gs_client_color cc;

    cc.pattern = 0x00;
    cc.paint.values[0] = 0;
    cc.paint.values[1] = 0;
    cc.paint.values[2] = 0;
    return gs_setcolor(igs, &cc);
}

/* DeviceCMYK */
static int setcmykspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;
    gs_color_space  *pcs;
    int code=0;
    ref stref;

    do {
	switch (*stage) {
	    case 0:
		if (istate->use_cie_color.value.boolval && !CIESubst) {
		    byte *body;
		    ref *nosubst;

		    code = dict_find_string(systemdict, "NOSUBSTDEVICECOLORS", &nosubst);
		    if (code < 0)
			return code;
		    if (!r_has_type(nosubst, t_boolean))
    			return_error(e_typecheck);
		    if (nosubst->value.boolval) {
			*stage = 4;
			*cont = 1;
			body = ialloc_string(32, "string");
			if (body == 0)
			    return_error(e_VMerror);
			memcpy(body, "/DefaultCMYK ..nosubstdevicetest",32);
			make_string(&stref, a_all | icurrent_space, 32, body);
			r_set_attrs(&stref, a_executable);
			esp++;
			ref_assign(esp, &stref);
			return o_push_estack;
		    } else {
			*stage = 2;
			*cont = 1;
			body = ialloc_string(47, "string");
			if (body == 0)
			    return_error(e_VMerror);
			memcpy(body, "{/DefaultCMYK /ColorSpace findresource} stopped", 47);
			make_string(&stref, a_all | icurrent_space, 47, body);
			r_set_attrs(&stref, a_executable);
			esp++;
			ref_assign(esp, &stref);
			return o_push_estack;
		    }
		}
		/* fall through */
	    case 1:
		pcs = gs_cspace_new_DeviceCMYK(imemory);
		if (pcs == NULL)
		    return_error(e_VMerror);
		code = gs_setcolorspace(igs, pcs);
		if (code >= 0) {
		    gs_client_color *   pcc = igs->ccolor;

		    cs_adjust_color_count(igs, -1); /* not strictly necessary */
		    pcc->paint.values[0] = 0;
		    pcc->paint.values[1] = 0;
		    pcc->paint.values[2] = 0;
		    pcc->paint.values[3] = 1;
		    pcc->pattern = 0;		/* for GC */
		    gx_unset_dev_color(igs);
		}
		rc_decrement_only(pcs, "zsetdevcspace");
		*cont = 0;
		*stage = 0;
		break;
	    case 2:
		if (!r_has_type(op, t_boolean))
		    return_error(e_typecheck);
		if (op->value.boolval) {
		    /* Failed to find the /DefaultCMYK CSA, so give up and
		     * just use DeviceCMYK
		     */
		    pop(1);
		    *stage = 1;
		    break;
		}
		pop(1);
    		*stage = 3;
		code = setcolorspace_nosubst(i_ctx_p);
		if (code != 0)
		    return code;
		break;
	    case 3:
		/* We end up here after setting the DefaultGray CIE space
		 * We've finished setting the gray color space, so we
		 * just exit now
		 */
		*cont = 0;
		*stage = 0;
		break;
	    case 4:
		/* We come here if /UseCIEColor is true, and NOSUBSTDEVICECOLORS
		 * is also true. We will have a boolean on the stack, if its true
		 * then we need to set the space (also on the stack), invoke
		 * .includecolorspace, and set /DeviceGray, otherwise we just need
		 * to set DeviceGray. See gs-cspace.ps.
		 */
		if (!r_has_type(op, t_boolean))
		    return_error(e_typecheck);
		pop(1);
		*stage = 1;
		*cont = 1;
		if (op->value.boolval) {
		    *stage = 5;
		    code = setcolorspace_nosubst(i_ctx_p);
		    if (code != 0)
			return code;
		} 
		break;
	    case 5:
		/* After stage 4 above, if we had to set a color space, we come
		 * here. Now we need to use .includecolorspace to register the space
		 * with any high-level devices which want it.
		 */
		*stage = 1;
		*cont = 1;
		code = zincludecolorspace(i_ctx_p);
		if (code != 0)
		    return code;
		break;
	}
    } while (*stage);
    return code;
}
static int cmykdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i;

    for (i = 0;i < 8;i+=2) {
	ptr[i] = 0;
	ptr[i+1] = 1;
    }
    return 0;
}
static int cmykrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i;

    for (i = 0;i < 8;i+=2) {
	ptr[i] = 0;
	ptr[i+1] = 1;
    }
    return 0;
}
/* This routine converts a CMYK value into its equivalent in a different
 * device space, required by currentgray, currentrgb, currenthsb and
 * currentcmyk. The actual color value will have been processed through
 * the tint transform(s) of the parent space(s) until it reaches a device
 * space. This converts that final value into the requested space.
 */
static int cmykbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op = osp;
    float CMYK[4], Gray, RGB[3];
    int i;

    *cont = 0;
    *stage = 0;
    check_op(4);
    op -= 3;
    for (i=0;i<4;i++) {
        if (!r_has_type(op, t_integer)) {
    	    if (r_has_type(op, t_real)) {
		CMYK[i] = op->value.realval;
	    } else
		return_error(e_typecheck);
	} else
	    CMYK[i] = (float)op->value.intval;
        if (CMYK[i] < 0 || CMYK[i] > 1)
	    return_error(e_rangecheck);
	op++;
    }

    switch (base) {
	case 0:
	    pop(3);
	    op = osp;
	    Gray = (0.3 * CMYK[0]) + (0.59 * CMYK[1]) + (0.11 * CMYK[2]) + CMYK[3];
	    if (Gray > 1.0)
		Gray = 0;
	    else
		Gray = 1.0 - Gray;
            make_real(op, Gray);
	    break;
	case 1:
	case 2:
	    pop(1);
	    op = osp;
	    RGB[0] = 1.0 - (CMYK[0] + CMYK[3]);
	    if (RGB[0] < 0)
		RGB[0] = 0;
	    RGB[1] = 1.0 - (CMYK[1] + CMYK[3]);
	    if (RGB[1] < 0)
		RGB[1] = 0;
	    RGB[2] = 1.0 - (CMYK[2] + CMYK[3]);
	    if (RGB[2] < 0)
		RGB[2] = 0;
	    if (base == 1)
		rgb2hsb((float *)&RGB);
            make_real(&op[-2], RGB[0]);
            make_real(&op[-1], RGB[1]);
            make_real(op, RGB[2]);
	    break;
	case 3:
	    op = osp;
            make_real(&op[-3], CMYK[0]);
            make_real(&op[-2], CMYK[1]);
            make_real(&op[-1], CMYK[2]);
            make_real(op, CMYK[3]);
	    break;
	default:
	    return_error(e_undefined);
    }
    return 0;
}
static int cmykvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;
    int i;

    if (num_comps < 4)
	return_error(e_stackunderflow);

    op -= 3;
    for (i=0;i < 4;i++) {
	if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	    return_error(e_typecheck);
	op++;
    }

    for (i=0;i < 4; i++) {
	if (values[i] > 1.0)
	    values[i] = 1.0;

	if (values[i] < 0.0)
	    values[i] = 0.0;
    }

    return 0;
}
static int cmykinitialproc(i_ctx_t *i_ctx_p, ref *space)
{
    gs_client_color cc;

    cc.pattern = 0x00;
    cc.paint.values[0] = 0;
    cc.paint.values[1] = 0;
    cc.paint.values[2] = 0;
    cc.paint.values[3] = 1;
    return gs_setcolor(igs, &cc);
}

/* CIEBased */
/* A utility routine to check whether two arrays contain the same
 * contents. Used to check whether two color spaces are the same
 * Note that this can be recursive if the array contains arrays.
 */
static int comparearrays(i_ctx_t * i_ctx_p, ref *m1, ref *m2)
{
    int i, code;
    ref ref1, ref2;

    if (r_size(m1) != r_size(m2))
	return 0;

    for (i=0;i < r_size(m1);i++) {
	code = array_get(imemory, m1, i, &ref1);
	if (code < 0)
	    return 0;
	code = array_get(imemory, m2, i, &ref2);
	if (code < 0)
	    return 0;

	if (r_type(&ref1) != r_type(&ref2))
	    return 0;

	code = r_type(&ref1);
	switch(r_type(&ref1)) {
	    case t_null:
		break;
	    case t_boolean:
		if (ref1.value.boolval != ref2.value.boolval)
		    return 0;
		break;
	    case t_integer:
		if (ref1.value.intval != ref2.value.intval)
		    return 0;
		break;
	    case t_real:
		if (ref1.value.realval != ref2.value.realval)
		    return 0;
		break;
	    case t_name:
		if (!name_eq(&ref1, &ref2))
		    return 0;
		break;
	    case t_string:
		if (r_size(&ref1) != r_size(&ref2))
		    return 0;
		if (strncmp((const char *)ref1.value.const_bytes, (const char *)ref2.value.const_bytes, r_size(&ref1)) != 0)
		    return 0;
		break;
	    case t_array:
	    case t_mixedarray:
	    case t_shortarray:
		if (!comparearrays(i_ctx_p, &ref1, &ref2))
		    return 0;
		break;
	    case t_oparray:
		break;
	    case t_operator:
		if (ref1.value.opproc != ref2.value.opproc)
		    return 0;
		break;
	    case t__invalid:
	    case t_dictionary:
	    case t_file:
	    case t_unused_array_:
	    case t_struct:
	    case t_astruct:
	    case t_fontID:
	    case t_save:
	    case t_mark:
	    case t_device:
		return 0;
	    default:
		/* Some high frequency operators are defined starting at t_next_index
		 * I think as long as the 'type' of each is the same, we are OK
		 */
		break;
	}
    }
    return 1;
}
/* A utility routine to check whether two dictionaries contain the same
 * arrays. This is a simple routine, unlike comparearrays above it is only
 * used by the CIE comparison code and expects only to check that the
 * dictionary contains an array, and checks the arrays.
 */
static int comparedictkey(i_ctx_t * i_ctx_p, ref *CIEdict1, ref *CIEdict2, char *key)
{
    int code, code1;
    ref *tempref1, *tempref2;

    code = dict_find_string(CIEdict1, key, &tempref1);
    code1 = dict_find_string(CIEdict2, key, &tempref2);
    if (code != code1)
	return 0;

    if (code < 0)
	return 1;

    if (r_type(tempref1) != r_type(tempref2))
	return 0;

    if (r_type(tempref1) == t_null)
	return 1;

    return comparearrays(i_ctx_p, tempref1, tempref2);
}

/* Check that the WhitePoint of a CIE space is valid */
static int checkWhitePoint(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code = 0, i;
    float value[3];
    ref *tempref, valref;

    code = dict_find_string(CIEdict, "WhitePoint", &tempref);
    if (code < 0 || r_has_type(tempref, t_null))
	return code;

    if (!r_is_array(tempref)) 
	return_error(e_typecheck);
    if (r_size(tempref) != 3)
	return_error(e_rangecheck);

    for (i=0;i<3;i++) {
	code = array_get(imemory, tempref, i, &valref);
	if (code < 0)
	    return code;
	if (r_has_type(&valref, t_integer))
	    value[i] = (float)valref.value.intval;
	else if (r_has_type(&valref, t_real))
	    value[i] = (float)valref.value.realval;
	else
	    return_error(e_typecheck);
    }
    /* Xw and Zw must be positive and Yw must be 1 (3rd edition PLRM p230) */
    if (value[0] < 0 || value[1] != 1 || value[2] < 0 )
	    return_error(e_rangecheck);

    return 0;
}
/* Check that the BlackPoint of a CIE space is valid */
static int checkBlackPoint(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code = 0, i;
    float value[3];
    ref *tempref, valref;

    code = dict_find_string(CIEdict, "BlackPoint", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 3)
	    return_error(e_rangecheck);

	for (i=0;i<3;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    }
    return 0;
}
/* Check that the RangeLMN of a CIE space is valid */
static int checkRangeLMN(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code = 0, i;
    float value[6];
    ref *tempref, valref;

    code = dict_find_string(CIEdict, "RangeLMN", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 6)
	    return_error(e_rangecheck);

	for (i=0;i<6;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
	if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4])
	    return_error(e_rangecheck);
    }
    return 0;
}
/* Check that the DecodeLMN of a CIE space is valid */
static int checkDecodeLMN(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code = 0, i;
    ref *tempref, valref;

    code = dict_find_string(CIEdict, "DecodeLMN", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 3)
	    return_error(e_rangecheck);

	for (i=0;i<3;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    check_proc(valref);
	}
    }
    return 0;
}
/* Check that the MatrixLMN of a CIE space is valid */
static int checkMatrixLMN(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code = 0, i;
    float value[9];
    ref *tempref, valref;

    code = dict_find_string(CIEdict, "MatrixLMN", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 9)
	    return_error(e_rangecheck);

	for (i=0;i<9;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
	        value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    }
    return 0;
}

/* CIEBasedA */
static int setcieaspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    int code = 0;
    ref CIEDict, *nocie;

    if (i_ctx_p->language_level < 2)
	return_error(e_undefined);

    code = dict_find_string(systemdict, "NOCIE", &nocie);
    if (code < 0)
	return code;
    if (!r_has_type(nocie, t_boolean))
	return_error(e_typecheck);
    if (nocie->value.boolval)
	return setgrayspace(i_ctx_p, r, stage, cont, 1);

    *cont = 0;
    code = array_get(imemory, r, 1, &CIEDict);
    if (code < 0)
	return code;
    if ((*stage) > 0) {
	gs_client_color cc;

	cc.pattern = 0x00;
	cc.paint.values[0] = 0;
	code = gs_setcolor(igs, &cc);
        *stage = 0;
	return code;
    }
    code = cieaspace(i_ctx_p, &CIEDict);
    (*stage)++;
    *cont = 1;
    return code;
}
static int validatecieaspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code = 0, i;
    float value[9];
    ref     CIEdict, *CIEspace = *r, *tempref, valref;

    if (!r_is_array(CIEspace))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(CIEspace) != 2)
	return_error(e_rangecheck);

    code = array_get(imemory, CIEspace, 1, &CIEdict);
    if (code < 0)
	return code;

    check_read_type(CIEdict, t_dictionary);

    /* Check white point exists, and is an array of three numbers */
    code = checkWhitePoint(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    /* Remaining parameters are optional, but we must validate
     * them if they are present
     */
    code = dict_find_string(&CIEdict, "RangeA", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	/* Array of two numbers A0 < A1 */
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 2)
	    return_error(e_rangecheck);

	for (i=0;i<2;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
	if (value[1] < value[0])
	    return_error(e_rangecheck);
    }

    code = dict_find_string(&CIEdict, "DecodeA", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	check_proc(*tempref);
    }

    code = dict_find_string(&CIEdict, "MatrixA", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 3)
	    return_error(e_rangecheck);

	for (i=0;i<3;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    }

    code = checkRangeLMN(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    code = checkDecodeLMN(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    code = checkMatrixLMN(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    code = checkBlackPoint(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    *r = 0;
    return 0;
}
static int cieadomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
	return code;

    /* If we have a RangeA entry in the dictionary, get the
     * values from that 
     */
    code = dict_find_string(&CIEdict, "RangeA", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<2;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		ptr[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    } else {
	/* Default values for CIEBasedA */
	ptr[0] = 0;
	ptr[1] = 1;
    }
    return 0;
}
static int ciearange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
	return code;

    /* If we have a RangeA entry in the dictionary, get the
     * values from that 
     */
    code = dict_find_string(&CIEdict, "RangeA", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<2;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		ptr[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    } else {
	/* Default values for CIEBasedA */
	ptr[0] = 0;
	ptr[1] = 1;
    }
    return 0;
}
static int cieavalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;

    if (num_comps < 1)
	return_error(e_stackunderflow);

    if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	return_error(e_typecheck);

    return 0;
}
static int cieacompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    int code = 0;
    ref CIEdict1, CIEdict2;

    code = array_get(imemory, space, 1, &CIEdict1);
    if (code < 0)
	return 0;
    code = array_get(imemory, testspace, 1, &CIEdict2);
    if (code < 0)
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"WhitePoint"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"BlackPoint"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeA"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeA"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixA"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeLMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeLMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixMN"))
	return 0;
    return 1;
}

/* CIEBasedABC */
static int setcieabcspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    int code = 0;
    ref CIEDict, *nocie;
    
    if (i_ctx_p->language_level < 2)
	return_error(e_undefined);

    code = dict_find_string(systemdict, "NOCIE", &nocie);
    if (code < 0)
	return code;
    if (!r_has_type(nocie, t_boolean))
	return_error(e_typecheck);
    if (nocie->value.boolval)
	return setrgbspace(i_ctx_p, r, stage, cont, 1);

    *cont = 0;
    code = array_get(imemory, r, 1, &CIEDict);
    if (code < 0)
	return code;

    if ((*stage) > 0) {
	gs_client_color cc;
	int i;

	cc.pattern = 0x00;
	for (i=0;i<3;i++) 
	    cc.paint.values[i] = 0;
	code = gs_setcolor(igs, &cc);
        *stage = 0;
	return code;
    }
    code = cieabcspace(i_ctx_p, &CIEDict);
    *cont = 1;
    (*stage)++;
    return code;
}
static int validatecieabcspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code = 0, i;
    float value[9];
    ref     CIEdict, *CIEspace = *r, *tempref, valref;

    if (!r_is_array(CIEspace))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(CIEspace) != 2)
	return_error(e_rangecheck);

    code = array_get(imemory, CIEspace, 1, &CIEdict);
    if (code < 0)
	return code;
    check_read_type(CIEdict, t_dictionary);

    /* Check white point exists, and is an array of three numbers */
    code = checkWhitePoint(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    /* Remaining parameters are optional, but we must validate
     * them if they are present
     */
    code = dict_find_string(&CIEdict, "RangeABC", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 6)
	    return_error(e_rangecheck);

	for (i=0;i<6;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
	if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4])
	    return_error(e_rangecheck);
    }

    code = dict_find_string(&CIEdict, "DecodeABC", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 3)
	    return_error(e_rangecheck);

	for (i=0;i<3;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    check_proc(valref);
	}
    }

    code = dict_find_string(&CIEdict, "MatrixABC", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) != 9)
	    return_error(e_rangecheck);

	for (i=0;i<9;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    }


    code = checkRangeLMN(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    code = checkDecodeLMN(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    code = checkMatrixLMN(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    code = checkBlackPoint(i_ctx_p, &CIEdict);
    if (code != 0)
	return code;

    *r = 0;
    return 0;
}
static int cieabcdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
	return code;

    /* If we have a RangeABC, get the values from that */
    code = dict_find_string(&CIEdict, "RangeABC", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<6;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		ptr[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    } else {
	/* Default values for CIEBasedABC */
	for (i=0;i<3;i++) {
	    ptr[2 * i] = 0;
	    ptr[(2 * i) + 1] = 1;
	}
    }
    return 0;
}
static int cieabcrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
	return code;

    /* If we have a RangeABC, get the values from that */
    code = dict_find_string(&CIEdict, "RangeABC", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<6;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		ptr[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    } else {
	/* Default values for CIEBasedABC */
	for (i=0;i<3;i++) {
	    ptr[2 * i] = 0;
	    ptr[(2 * i) + 1] = 1;
	}
    }
    return 0;
}
static int cieabcvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;
    int i;

    if (num_comps < 3)
	return_error(e_stackunderflow);

    op -= 2;
    for (i=0;i<3;i++) {
	if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	    return_error(e_typecheck);
	op++;
    }

    return 0;
}
static int cieabccompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    int code = 0;
    ref CIEdict1, CIEdict2;

    code = array_get(imemory, space, 1, &CIEdict1);
    if (code < 0)
	return 0;
    code = array_get(imemory, testspace, 1, &CIEdict2);
    if (code < 0)
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"WhitePoint"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"BlackPoint"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeLMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeLMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixMN"))
	return 0;
    return 1;
}

/* CIEBasedDEF */
static int setciedefspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    int code = 0;
    ref CIEDict, *nocie;
    
    if (i_ctx_p->language_level < 3)
	return_error(e_undefined);

    code = dict_find_string(systemdict, "NOCIE", &nocie);
    if (code < 0)
	return code;
    if (!r_has_type(nocie, t_boolean))
	return_error(e_typecheck);
    if (nocie->value.boolval)
	return setrgbspace(i_ctx_p, r, stage, cont, 1);

    *cont = 0;
    code = array_get(imemory, r, 1, &CIEDict);
    if (code < 0)
	return code;
    if ((*stage) > 0) {
	gs_client_color cc;
	int i;

	cc.pattern = 0x00;
	for (i=0;i<3;i++) 
	    cc.paint.values[i] = 0;
	code = gs_setcolor(igs, &cc);
        *stage = 0;
	return code;
    }
    code = ciedefspace(i_ctx_p, &CIEDict);
    *cont = 1;
    (*stage)++;
    return code;
}
static int validateciedefspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code = 0, i;
    float value[6];
    ref     CIEdict, *pref, *CIEspace = *r, tempref, valref;

    if (!r_is_array(CIEspace))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(CIEspace) != 2)
	return_error(e_rangecheck);

    code = array_get(imemory, CIEspace, 1, &CIEdict);
    if (code < 0)
	return code;
    check_read_type(CIEdict, t_dictionary);

    code = validatecieabcspace(i_ctx_p, r);
    if (code != 0)
	return code;

    pref = &tempref;
    code = dict_find_string(&CIEdict, "Table", &pref);
    if (code >= 0) {
	if (!r_is_array(pref))
	    return_error(e_typecheck);
	if (r_size(pref) != 4)
	    return_error(e_rangecheck);

	for (i=0;i<3;i++) {
	    code = array_get(imemory, pref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else
	        return_error(e_typecheck);
	}
	if (value[0] <= 1 || value[1] <= 1 || value[2] <= 1)
	    return_error(e_rangecheck);

	code = array_get(imemory, pref, 3, &valref);
	if (code < 0)
	    return code;
	if (!r_is_array(&valref))
	    return_error(e_typecheck);
	if (r_size(&valref) != value[0])
	    return_error(e_rangecheck);

	for (i=0;i<value[0];i++) {
	    code = array_get(imemory, &valref, i, &tempref);
	    if (code < 0)
		return code;
	    if (!r_has_type(&tempref, t_string))
		return_error(e_typecheck);

	    if (r_size(&tempref) != (3 * value[1] * value[2]))
		return_error(e_rangecheck);
	}
    } else {
	return_error(e_rangecheck);
    }

    /* Remaining parameters are optional, but we must validate
     * them if they are present
     */
    code = dict_find_string(&CIEdict, "RangeDEF", &pref);
    if (code >= 0 && !r_has_type(&tempref, t_null)) {
	if (!r_is_array(pref))
	    return_error(e_typecheck);
	if (r_size(pref) != 6)
	    return_error(e_rangecheck);

	for (i=0;i<6;i++) {
	    code = array_get(imemory, pref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
	if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4])
	    return_error(e_rangecheck);
    }

    code = dict_find_string(&CIEdict, "DecodeDEF", &pref);
    if (code >= 0 && !r_has_type(pref, t_null)) {
	if (!r_is_array(pref))
	    return_error(e_typecheck);
	if (r_size(pref) != 3)
	    return_error(e_rangecheck);

	for (i=0;i<3;i++) {
	    code = array_get(imemory, pref, i, &valref);
	    if (code < 0)
		return code;
	    check_proc(valref);
	}
    }

    code = dict_find_string(&CIEdict, "RangeHIJ", &pref);
    if (code >= 0 && !r_has_type(pref, t_null)) {
	if (!r_is_array(pref))
	    return_error(e_typecheck);
	if (r_size(pref) != 6)
	    return_error(e_rangecheck);

	for (i=0;i<6;i++) {
	    code = array_get(imemory, pref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
	if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4])
	    return_error(e_rangecheck);
    }

    *r = 0;
    return 0;
}
static int ciedefdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
	return code;

    /* If we have a RangeDEF, get the values from that */
    code = dict_find_string(&CIEdict, "RangeDEF", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<6;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		ptr[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    } else {
	/* Default values for a CIEBasedDEF */
	for (i=0;i<3;i++) {
	    ptr[2 * i] = 0;
	    ptr[(2 * i) + 1] = 1;
	}
    }
    return 0;
}
static int ciedefrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
	return code;

    /* If we have a RangeDEF, get the values from that */
    code = dict_find_string(&CIEdict, "RangeDEF", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<6;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		ptr[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    } else {
	/* Default values for a CIEBasedDEF */
	for (i=0;i<3;i++) {
	    ptr[2 * i] = 0;
	    ptr[(2 * i) + 1] = 1;
	}
    }
    return 0;
}
static int ciedefvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;
    int i;

    if (num_comps < 3)
	return_error(e_stackunderflow);

    op -= 2;
    for (i=0;i<3;i++) {
	if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	    return_error(e_typecheck);
	op++;
    }

    return 0;
}
static int ciedefcompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    int code = 0;
    ref CIEdict1, CIEdict2;

    code = array_get(imemory, space, 1, &CIEdict1);
    if (code < 0)
	return 0;
    code = array_get(imemory, testspace, 1, &CIEdict2);
    if (code < 0)
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"WhitePoint"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"BlackPoint"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeLMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeLMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeDEF"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeDEF"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeHIJ"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"Table"))
	return 0;
    return 1;
}

/* CIEBasedDEFG */
static int setciedefgspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    int code = 0;
    ref CIEDict, *nocie;
    
    if (i_ctx_p->language_level < 3)
	return_error(e_undefined);

    code = dict_find_string(systemdict, "NOCIE", &nocie);
    if (code < 0)
	return code;
    if (!r_has_type(nocie, t_boolean))
	return_error(e_typecheck);
    if (nocie->value.boolval)
	return setcmykspace(i_ctx_p, r, stage, cont, 1);

    *cont = 0;
    code = array_get(imemory, r, 1, &CIEDict);
    if (code < 0)
	return code;
    if ((*stage) > 0) {
	gs_client_color cc;
	int i;

	cc.pattern = 0x00;
	for (i=0;i<4;i++) 
	    cc.paint.values[i] = 0;
	code = gs_setcolor(igs, &cc);
        *stage = 0;
	return code;
    }
    code = ciedefgspace(i_ctx_p, &CIEDict);
    *cont = 1;
    (*stage)++;
    return code;
}
static int validateciedefgspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code = 0, i, j;
    float value[8];
    ref     CIEdict, *CIEspace = *r, tempref, arrayref, valref, *pref = &tempref;

    if (!r_is_array(CIEspace))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(CIEspace) != 2)
	return_error(e_rangecheck);

    code = array_get(imemory, CIEspace, 1, &CIEdict);
    if (code < 0)
	return code;
    check_read_type(CIEdict, t_dictionary);

    code = validatecieabcspace(i_ctx_p, r);
    if (code != 0)
	return code;

    code = dict_find_string(&CIEdict, "Table", &pref);
    if (code >= 0) {
	if (!r_is_array(pref))
	    return_error(e_typecheck);
	if (r_size(pref) != 5)
	    return_error(e_rangecheck);

	for (i=0;i<4;i++) {
	    code = array_get(imemory, pref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else
	        return_error(e_typecheck);
	}
	if (value[0] <= 1 || value[1] <= 1 || value[2] <= 1 || value[3] <= 1)
	    return_error(e_rangecheck);

	code = array_get(imemory, pref, 4, &arrayref);
	if (code < 0)
	    return code;
	if (!r_is_array(&arrayref))
	    return_error(e_typecheck);
	if (r_size(&arrayref) != value[0])
	    return_error(e_rangecheck);

	for (i=0;i<value[0];i++) {
	    code = array_get(imemory, &arrayref, i, &tempref);
	    if (code < 0)
		return code;
	    for (j=0;j<value[1];j++) {
		code = array_get(imemory, &tempref, i, &valref);
		if (code < 0)
		    return code;
		if (!r_has_type(&valref, t_string))
		    return_error(e_typecheck);

		if (r_size(&valref) != (3 * value[2] * value[3]))
		    return_error(e_rangecheck);
	    }
	}
    } else {
	return_error(e_rangecheck);
    }

    /* Remaining parameters are optional, but we must validate
     * them if they are present
     */
    code = dict_find_string(&CIEdict, "RangeDEFG", &pref);
    if (code >= 0 && !r_has_type(pref, t_null)) {
	if (!r_is_array(pref))
	    return_error(e_typecheck);
	if (r_size(pref) != 8)
	    return_error(e_rangecheck);

	for (i=0;i<8;i++) {
	    code = array_get(imemory, pref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
	if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4] || value[7] < value[6])
	    return_error(e_rangecheck);
    }

    code = dict_find_string(&CIEdict, "DecodeDEFG", &pref);
    if (code >= 0 && !r_has_type(pref, t_null)) {
	if (!r_is_array(pref))
	    return_error(e_typecheck);
	if (r_size(pref) != 4)
	    return_error(e_rangecheck);

	for (i=0;i<4;i++) {
	    code = array_get(imemory, pref, i, &valref);
	    if (code < 0)
		return code;
	    check_proc(valref);
	}
    }

    code = dict_find_string(&CIEdict, "RangeHIJK", &pref);
    if (code >= 0 && !r_has_type(pref, t_null)) {
	if (!r_is_array(pref))
	    return_error(e_typecheck);
	if (r_size(pref) != 8)
	    return_error(e_rangecheck);

	for (i=0;i<8;i++) {
	    code = array_get(imemory, pref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		value[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		value[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
	if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4] || value[7] < value[6])
	    return_error(e_rangecheck);
    }

    *r = 0;
    return 0;
}
static int ciedefgdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
	return code;

    /* If we have a RangeDEFG, get the values from that */
    code = dict_find_string(&CIEdict, "RangeDEFG", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<8;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		ptr[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    } else {
	/* Default values for a CIEBasedDEFG */
	for (i=0;i<4;i++) {
	    ptr[2 * i] = 0;
	    ptr[(2 * i) + 1] = 1;
	}
    }
    return 0;
}
static int ciedefgrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
	return code;

    /* If we have a RangeDEFG, get the values from that */
    code = dict_find_string(&CIEdict, "RangeDEFG", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<8;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i] = (float)valref.value.intval;
	    else if (r_has_type(&valref, t_real))
		ptr[i] = (float)valref.value.realval;
	    else
	        return_error(e_typecheck);
        }
    } else {
	/* Default values for a CIEBasedDEFG */
	for (i=0;i<4;i++) {
	    ptr[2 * i] = 0;
	    ptr[(2 * i) + 1] = 1;
	}
    }
    return 0;
}
static int ciedefgvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;
    int i;

    if (num_comps < 4)
	return_error(e_stackunderflow);

    op -= 3;
    for (i=0;i < 4;i++) {
	if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	    return_error(e_typecheck);
	op++;
    }
    return 0;
}
static int ciedefgcompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    /* If the problems mentioned above are resolved, then this code could
     * be re-instated.
     */
    int code = 0;
    ref CIEdict1, CIEdict2;

    code = array_get(imemory, space, 1, &CIEdict1);
    if (code < 0)
	return 0;
    code = array_get(imemory, testspace, 1, &CIEdict2);
    if (code < 0)
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"WhitePoint"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"BlackPoint"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixABC"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeLMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeLMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixMN"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeDEFG"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeDEFG"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeHIJK"))
	return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"Table"))
	return 0;
    return 1;
}

static char *CIESpaces[] = {
    (char *)"CIEBasedA", 
    (char *)"CIEBasedABC", 
    (char *)"CIEBasedDEF", 
    (char *)"CIEBasedDEFG"
};
/* This routine returns the Device space equivalents for a CIEBased space.
 * Used by currentgray, currentrgb and currentcmyk, the PLRM says that CIE 
 * spaces return 0 for all components.
 */
static int ciebasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op;
    ref *spacename, nref;
    int i, components=1, code;

    /* If the spaece is an array, the first element is always the name */
    if (r_is_array(space))
        spacename = space->value.refs;
    else
	spacename = space;
    /* Check that it really is a name */
    if (!r_has_type(spacename, t_name))
	return_error(e_typecheck);

    /* Find the relevant color space object */
    for (i=0;i<4;i++) {
	code = names_ref(imemory->gs_lib_ctx->gs_name_table, (const byte *)CIESpaces[i], strlen(CIESpaces[i]), &nref, 0);
	if (code < 0)
	    return code;
	if (name_eq(spacename, &nref)) {
	    break;
	}
    }
    /* Find out how many values are on the stack, which depends
     * on what kind of CIEBased space this is.
     */
    switch(i){
	case 0:
	    components = 1;
	    break;
	case 1:
	case 2:
	    components = 3;
	    break;
	case 3:
	    components = 4;
	    break;
    }
    /* Remove teh requisite number of values */
    pop(components);
    op = osp;
    /* Find out how many values we need to return, which
     * depends on the requested space.
     */
    switch(base) {
	case 0:
	    components = 1;
	    break;
	case 1:
	case 2:
	    components = 3;
	    break;
	case 3:
	    components = 4;
	    break;
    }
    push(components);
    /* The PLRM says that all the components should be returned as 0.0 */
    op -= components-1;
    for (i=0;i<components;i++) {
	make_real(op, (float)0);
	op++;
    }
    /* However, Adobe implementations actually return 1.0 for the black
     * channel of CMYK...
     */
    if (components == 4) {
	op--;
	make_real(op, (float)1);
    }
    *stage = 0;
    *cont = 0;
    return 0;
}

/* This routine used by both Separatin and DeviceN spaces to convert
 * a PostScript tint transform into a Function, either a type 4
 * 'PostScript calculator (see PDF reference) or a type 0 'sampled'
 * function.
 */
static int convert_transform(i_ctx_t * i_ctx_p, ref *arr, ref *pproc)
{
    os_ptr op = osp;   /* required by "push" macro */
    int code;

    /* buildfunction returns an operand on the stack. In fact
     * it replaces the lowest operand, so make sure there is an
     * empty sacrificial one present.
     */
    push(1);
    /* Start by trying a type 4 function */
    code = buildfunction(i_ctx_p, arr, pproc, 4);

    if (code < 0)
	/* If that fails, try a type 0. We prefer a type 4
	 * because a type 0 will require us to sample the
	 * space, which is expensive
	 */
	code = buildfunction(i_ctx_p, arr, pproc, 0);

    return code;
}

/* Separation */
static int setseparationspace(i_ctx_t * i_ctx_p, ref *sepspace, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;   /* required by "push" macro */
    int code = 0;
    ref sname, proc;
    ref name_none, name_all;
    separation_type sep_type;
    ref_colorspace cspace_old;
    gs_color_space *pcs;
    gs_color_space * pacs;
    gs_function_t *pfn = NULL;
    gs_client_color cc;
    
    if (i_ctx_p->language_level < 2)
	return_error(e_undefined);

    *cont = 0;
    if ((*stage) == 0) {
	code = array_get(imemory, sepspace, 3, &proc);
	if (code < 0)
	    return code;
	/* Check to see if we already have a function (eg from a PDF file) */
	pfn = ref_function(&proc);
	if (pfn == NULL) {
	    /* Convert tint transform to a PostScript function */
	    code = convert_transform(i_ctx_p, sepspace, &proc);
	    if (code < 0)
		return code;
	    if (code > 0) {
		*cont = 1;
		(*stage)++;
		return code;
	    }
	    /* We can only get here if the transform converted to a function
	     * without requiring a continuation. Most likely this means its a 
	     * type 4 function. If so then it is still on the stack.
	     */
	    op = osp;
	    pfn = ref_function(op);
	    pop (1);
	}
    } else {
	/* The function is returned on the operand stack */
	op = osp;
	pfn = ref_function(op);
	pop (1);
    }

    *stage = 0;
    if ((code = name_ref(imemory, (const byte *)"All", 3, &name_all, 0)) < 0)
	return code;
    if ((code = name_ref(imemory, (const byte *)"None", 4, &name_none, 0)) < 0)
	return code;
    /* Check separation name is a string or name object */
    code = array_get(imemory, sepspace, 1, &sname);
    if (code < 0)
	return code;

    if (r_has_type(&sname, t_string)) {
        code = name_from_string(imemory, &sname, &sname);
        if (code < 0)
	    return code;
    }
    sep_type = ( name_eq(&sname, &name_all) ? SEP_ALL :
	         name_eq(&sname, &name_none) ? SEP_NONE : SEP_OTHER);

    /* The alternate color space has been selected as the current color space */
    pacs = gs_currentcolorspace(igs);

    cspace_old = istate->colorspace;
    /* Now set the current color space as Separation */
    code = gs_cspace_new_Separation(&pcs, pacs, imemory);
    if (code < 0)
	return code;
    pcs->params.separation.sep_type = sep_type;
    pcs->params.separation.sep_name = name_index(imemory, &sname);
    pcs->params.separation.get_colorname_string = gs_get_colorname_string;
    code = array_get(imemory, sepspace, 1, &proc);
    if (code < 0)
	return code;
    istate->colorspace.procs.special.separation.layer_name = proc;
    code = array_get(imemory, sepspace, 3, &proc);
    if (code < 0)
	return code;
    istate->colorspace.procs.special.separation.tint_transform = proc;
    if (code >= 0)
        code = gs_cspace_set_sepr_function(pcs, pfn);
    if (code >= 0)
	code = gs_setcolorspace(igs, pcs);
    /* release reference from construction */
    rc_decrement_only(pcs, "setseparationspace");
    if (code < 0) {
	istate->colorspace = cspace_old;
	return code;
    }
    cc.pattern = 0x00;
    cc.paint.values[0] = 1.0;
    code = gs_setcolor(igs, &cc);
    return code;
}
static int validateseparationspace(i_ctx_t * i_ctx_p, ref **space)
{
    int code = 0;
    ref *sepspace = *space;
    ref nameref, sref, sname, altspace, tref;

    if (!r_is_array(sepspace))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(sepspace) != 4)
	return_error(e_rangecheck);

    /* Check separation name is a string or name object */
    code = array_get(imemory, sepspace, 1, &sname);
    if (code < 0)
	return code;
    if (!r_has_type(&sname, t_name)) {
	if (!r_has_type(&sname, t_string))
	    return_error(e_typecheck);
	else {
	    code = name_from_string(imemory, &sname, &sname);
	    if (code < 0)
		return code;
	}
    }

    /* Check the tint transform is a procedure */
    code = array_get(imemory, sepspace, 3, &tref);
    if (code < 0)
	return code;
    check_proc(tref);

    /* Get the name of the alternate space */
    code = array_get(imemory, sepspace, 2, &altspace);
    if (code < 0)
	return code;
    if (r_has_type(&altspace, t_name))
	ref_assign(&nameref, &altspace);
    else {
	/* Make sure the alternate space is an array */
	if (!r_is_array(&altspace))
	    return_error(e_typecheck);
	/* And has a name for its type */
	code = array_get(imemory, &altspace, 0, &tref);
	if (code < 0)
	    return code;
	if (!r_has_type(&tref, t_name))
	    return_error(e_typecheck);
	ref_assign(&nameref, &tref);
    }

    /* Convert alternate space name to string */
    name_string_ref(imemory, &nameref, &sref);
    /* Check its not /Indexed or /Pattern or /DeviceN */
    if (r_size(&sref) == 7) {
	if (strncmp((const char *)sref.value.const_bytes, "Indexed", 7) == 0)
	    return_error(e_typecheck);
	if (strncmp((const char *)sref.value.const_bytes, "Pattern", 7) == 0)
	    return_error(e_typecheck);
	if (strncmp((const char *)sref.value.const_bytes, "DeviceN", 7) == 0)
	    return_error(e_typecheck);
    }
    /* and also not /Separation */
    if (r_size(&sref) == 9 && strncmp((const char *)sref.value.const_bytes, "Separation", 9) == 0)
	return_error(e_typecheck);

    ref_assign(*space, &altspace);
    return 0;
}
static int separationalternatespace(i_ctx_t * i_ctx_p, ref *sepspace, ref **r, int *CIESubst)
{
    ref tref;
    int code;

    code = array_get(imemory, sepspace, 2, &tref);
    if (code < 0)
	return code;
    ref_assign(*r, &tref);
    return 0;
}
static int sepdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    ptr[0] = 0;
    ptr[1] = 1;
    return 0;
}
static int seprange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    ptr[0] = 0;
    ptr[1] = 1;
    return 0;
}
static int septransform(i_ctx_t *i_ctx_p, ref *sepspace, int *usealternate, int *stage, int *stack_depth)
{
    gx_device * dev = igs->device;
    ref sname, proc;
    int code, colorant_number;

    code = array_get(imemory, sepspace, 1, &sname);
    if (code < 0)
	return code;
    if (r_has_type(&sname, t_name)) {
        name_string_ref(imemory, &sname, &sname);
    }

    /* Check for /All and /None, never need the alternate for these */
    if (r_size(&sname) == 3 &&
	strncmp("All", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 0;
	return 0;
    }
    if (r_size(&sname) == 4 &&
	strncmp("None", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 0;
	return 0;
    }
    /*
     * Compare the colorant name to the device's.  If the device's
     * compare routine returns GX_DEVICE_COLOR_MAX_COMPONENTS then the
     * colorant is in the SeparationNames list but not in the
     * SeparationOrder list.
     */
    colorant_number = (*dev_proc(dev, get_color_comp_index))
		(dev, (const char *)sname.value.bytes, r_size(&sname), SEPARATION_NAME);
    if (colorant_number >= 0) {		/* If valid colorant name */
	*usealternate = 0;
    } else
	*usealternate = 1;

    if (r_size(&sname) == 4 &&
	strncmp("Gray", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 1;
    }
    if (r_size(&sname) == 4 &&
	strncmp("Cyan", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 1;
    }
    if (r_size(&sname) == 7 &&
	strncmp("Magenta", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 1;
    }
    if (r_size(&sname) == 6 &&
	strncmp("Yellow", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 1;
    }
    if (r_size(&sname) == 5 &&
	strncmp("Black", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 1;
    }
    if (r_size(&sname) == 3 &&
	strncmp("Red", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 1;
    }
    if (r_size(&sname) == 5 &&
	strncmp("Green", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 1;
    }
    if (r_size(&sname) == 4 &&
	strncmp("Blue", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	*usealternate = 1;
    }
    if (*usealternate && *stage == 0) {
	(*stage)++;
	esp++;
	code = array_get(imemory, sepspace, 3, &proc);
	if (code < 0)
	    return code;
	*esp = proc;
	return o_push_estack;
    }
    *stage = 0;
    return 0;
}
static int sepbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op = osp;   /* required by "push" macro */
    int use, code;

	code = septransform(i_ctx_p, space, &use, stage, stack_depth);
	if (code != 0)
	    return code;
	if (!use) {
	    *stage = 0;
	    *cont = 0;
	    pop(1);
	    op = osp;
	    switch(base) {
		case 0:
		    push(1);
		    make_real(op, 0.0);
		    break;
		case 1:
		case 2:
		    push(3);
		    make_real(&op[-2], 0.0);
		    make_real(&op[-1], 0.0);
		    make_real(op, 0.0);
		    break;
		case 3:
		    push(4);
		    make_real(&op[-3], 0.0);
		    make_real(&op[-2], 0.0);
		    make_real(&op[-1], 0.0);
		    make_real(op, 0.0);
		    break;
	    }
	} else {
	    *stage = 0;
	    *cont = 1;
	}
    return 0;
}
static int sepvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;

    if (num_comps < 1)
	return_error(e_stackunderflow);

    if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	return_error(e_typecheck);

    if (*values > 1.0)
	*values = 1.0;

    if (*values < 0.0)
	*values = 0.0;

    return 0;
}
static int sepcompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    ref sname1, sname2;
    int code;

    code = array_get(imemory, space, 1, &sname1);
    if (code < 0)
	return 0;

    code = array_get(imemory, testspace, 1, &sname2);
    if (code < 0)
	return 0;

    if (r_type(&sname1) != r_type(&sname2))
	return 0;

    switch(r_type(&sname1)) {
	case t_name:
	    if (!name_eq(&sname1, &sname2))
		return 0;
	    break;
	case t_string:
	    if (r_size(&sname1) != r_size(&sname2))
		return 0;
	    if (strncmp((const char *)sname1.value.const_bytes, (const char *)sname2.value.const_bytes, r_size(&sname1)) != 0)
		return 0;
	    break;
	default:
	    return 0;
    }
    code = array_get(imemory, testspace, 2, &sname1);
    if (code < 0)
	return 0;
    code = array_get(imemory, testspace, 2, &sname2);
    if (code < 0)
	return 0;
    if (r_type(&sname1) != r_type(&sname2))
	return 0;

    if (r_is_array(&sname1)) {
	if (!comparearrays(i_ctx_p, &sname1, &sname2))
	    return 0;
    } else {
	if (!r_has_type(&sname1, t_name))
	    return 0;
	if (!name_eq(&sname1, &sname2))
	    return 0;
    }
    code = array_get(imemory, space, 3, &sname1);
    if (code < 0)
	return 0;
    code = array_get(imemory, testspace, 3, &sname2);
    if (code < 0)
	return 0;
    return(comparearrays(i_ctx_p, &sname1, &sname2));
}
static int sepinitialproc(i_ctx_t *i_ctx_p, ref *space)
{
    gs_client_color cc;

    cc.pattern = 0x00;
    cc.paint.values[0] = 1.0;
    return gs_setcolor(igs, &cc);
}

/* DeviceN */
static int devicencolorants_cont(i_ctx_t *i_ctx_p)
{
    ref dict, *pdict = &dict, space[2], sname;
    int index, code, depth, stage;
    es_ptr ep = esp, pindex, pstage;
    os_ptr op = osp;
    gs_separation_name sep_name;

    pindex = &ep[-2];
    pstage = &ep[-1];
    index = (int)pindex->value.intval;
    stage = (int)pstage->value.intval;
    ref_assign(&dict, ep);

    do {
        if (index >= dict_length(pdict)) {
	    esp -= 4;
	    return o_pop_estack;
	}

	if (stage == 0) {
	    code = gs_gsave(igs);
	    if (code < 0)
		return code;

	    code = dict_index_entry(pdict, index, (ref *)&space);
	    if (code < 0) {
		make_int(pindex, ++index);
		code = gs_grestore(igs);
		if (code < 0)
		    return code;
		continue;
	    }

	    code = validate_spaces(i_ctx_p, &space[1], &depth);
	    if (code < 0) {
		make_int(pindex, ++index);
		code = gs_grestore(igs);
		if (code < 0)
		    return code;
		return o_push_estack;
	    }

	    /* If we get a continuation from a sub-procedure, we will want to come back
	     * here afterward, to do any remaining stages. We need to set up for that now.
	     * so that our continuation is ahead of the sub-proc's continuation.
	     */
	    check_estack(1);
	    /* The push_op_estack macro increments esp before use, so we don't need to */
	    push_op_estack(devicencolorants_cont);

	    make_int(pstage, 1);
	    push(1);
	    *op = space[1];
	    code = zsetcolorspace(i_ctx_p);
	    if (code != 0)
		return code;
	} else {
	    stage = 0;
	    code = dict_index_entry(pdict, index, (ref *)&space);
	    if (code == 0) {
		switch (r_type(&space[0])) {
		    case t_string:
			code = name_from_string(imemory, &space[0], &sname);
			if (code == 0)
			    sep_name = name_index(imemory, &sname);
			break;
		    case t_name:
			sep_name = name_index(imemory, &space[0]);
			break;
		    default:
			code = e_typecheck;
		}
	    }
	    make_int(pindex, ++index);
	    make_int(pstage, stage);
	    if (code == 0)
		gs_attachattributecolorspace(sep_name, igs);
	    code = gs_grestore(igs);
	    if (code < 0)
		return code;
	}
    }
    while(1);
}

static int setdevicenspace(i_ctx_t * i_ctx_p, ref *devicenspace, int *stage, int *cont, int CIESubst)
{
    os_ptr  op = osp;   /* required by "push" macro */
    int code = 0, num_components, i;
    ref namesarray, proc, sname, tname, sref, tempref[2];
    ref_colorspace cspace_old;
    gs_color_space *pcs;
    gs_color_space * pacs;
    gs_function_t *pfn = NULL;
    gs_separation_name *names;
    gs_device_n_map *pmap;
    gs_client_color cc;

    if (i_ctx_p->language_level < 3)
	return_error(e_undefined);

    *cont = 0;
    if ((*stage) == 2) {
	if (r_size(devicenspace) == 5) {
	    /* We have a Colorants dictionary from a PDF file. We need to handle this by
	     * temporarily setting each of the spaces in the dict, and attaching the
	     * resulting space to the DeviceN array. This is complicated, because 
	     * each space must be fully set up, and may result in running tint transform
	     * procedures and caching results. We need to handle this in yet another 
	     * layering of continuation procedures.
	     */
	    code = array_get(imemory, devicenspace, 4, &sref);
	    if (code < 0)
		return code;
	    if (!r_has_type(&sref, t_dictionary)) {
		*stage = 0;
		return 0;
	    }
	    if (dict_length(&sref) == 0)
		return(0);

	    code = dict_index_entry(&sref, 0, (ref *)&tempref);
	    if (code < 0)
		return code;
	    name_string_ref(imemory, &tempref[0], &sname);
	    if (r_size(&sname) != 9 || strncmp((const char *)sname.value.const_bytes, "Colorants", r_size(&sname)) != 0) {
		*stage = 0;
		return 0;
	    }

	    *stage = 3;
	    *cont = 1;
	    check_estack(5);
	    push_mark_estack(es_other, 0);
	    esp++;
	    /* variable to hold index of the space we are dealing with */
	    make_int(esp, 0);
	    esp++;
	    /* variable to hold processing step */
	    make_int(esp, 0);
	    esp++;
	    /* Store a pointer to the Colorants dictionary
	     */
	    ref_assign(esp, &tempref[1]);
	    push_op_estack(devicencolorants_cont);
	    return o_push_estack;
	} else {
	    *stage = 0;
	    return 0;
	}
    }
    if ((*stage) == 3) {
	*stage = 0;
	return 0;
    }
    if ((*stage) == 0) {
	code = array_get(imemory, devicenspace, 3, &proc);
	if (code < 0)
	    return code;
	pfn = ref_function(&proc);
	if (pfn == NULL) {
	    /* Convert tint transform to a PostScript function */
	    code = convert_transform(i_ctx_p, devicenspace, &proc);
	    if (code < 0)
		return code;
	    if (code > 0) {
		*cont = 1;
		*stage = 1;
		return code;
	    }
	    /* We can only get here if the transform converted to a function
	     * without requiring a continuation. Most likely this means its a 
	     * type 4 function. If so then it is still on the stack.
	     */
	    op = osp;
	    pfn = ref_function(op);
	    pop (1);
	}
    } else {
	/* The function is returned on the operand stack */
	op = osp;
	pfn = ref_function(op);
	pop (1);
    }

    *stage = 2;

    code = array_get(imemory, devicenspace, 1, &namesarray);
    if (code < 0)
	return code;
    num_components = r_size(&namesarray);
    /* The alternate color space has been selected as the current color space */
    pacs = gs_currentcolorspace(igs);

    if (num_components == 1) {
	array_get(imemory, &namesarray, (long)0, &sname);
	switch (r_type(&sname)) {
	    case t_string:
		tname = sname;
		break;
	    case t_name:
		name_string_ref(imemory, &sname, &tname);
		break;
	    default:
		return_error(e_typecheck);
		break;
	}
	if (strncmp((const char *)tname.value.const_bytes, "All", 3) == 0 && r_size(&tname) == 3) {
	    separation_type sep_type;

	    /* Sigh, Acrobat allows this, even though its contra the spec. Convert to
	     * a /Separation space and go on
	     */
	    sep_type = SEP_ALL;

	    /* The alternate color space has been selected as the current color space */
	    pacs = gs_currentcolorspace(igs);

	    cspace_old = istate->colorspace;
	    /* Now set the current color space as Separation */
	    code = gs_cspace_new_Separation(&pcs, pacs, imemory);
	    if (code < 0)
		return code;
	    pcs->params.separation.sep_type = sep_type;
	    pcs->params.separation.sep_name = name_index(imemory, &sname);
	    pcs->params.separation.get_colorname_string = gs_get_colorname_string;
	    code = array_get(imemory, &namesarray, (long)0, &sname);
	    if (code < 0)
		return code;
	    istate->colorspace.procs.special.separation.layer_name = sname;
	    code = array_get(imemory, devicenspace, 3, &proc);
	    if (code < 0)
		return code;
	    istate->colorspace.procs.special.separation.tint_transform = proc;
	    if (code >= 0)
		code = gs_cspace_set_sepr_function(pcs, pfn);
	    if (code >= 0)
		code = gs_setcolorspace(igs, pcs);
	    /* release reference from construction */
	    rc_decrement_only(pcs, "setseparationspace");
	    if (code < 0) {
		istate->colorspace = cspace_old;
		return code;
	    }
	    cc.pattern = 0x00;
	    cc.paint.values[0] = 1.0;
	    code = gs_setcolor(igs, &cc);
	    return code;
	}
    }
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
	    array_get(imemory, &namesarray, (long)i, &sname);
	    switch (r_type(&sname)) {
		case t_string:
		    code = name_from_string(imemory, &sname, &sname);
		    if (code < 0) {
			rc_decrement(pcs, "setdevicenspace");
			return code;
		    }
		    /* falls through */
		case t_name:
		    names[i] = name_index(imemory, &sname);
		    break;
		default:
		    rc_decrement(pcs, "setdevicenspace");
		    return_error(e_typecheck);
	    }
	}
    }

    /* Now set the current color space as DeviceN */

    cspace_old = istate->colorspace;
    istate->colorspace.procs.special.device_n.layer_names = namesarray;
    code = array_get(imemory, devicenspace, 3, &proc);
    if (code < 0)
	return code;
    istate->colorspace.procs.special.device_n.tint_transform = proc;    
    gs_cspace_set_devn_function(pcs, pfn);
    code = gs_setcolorspace(igs, pcs);
    /* release reference from construction */
    rc_decrement_only(pcs, "setdevicenspace");
    if (code < 0) {
	istate->colorspace = cspace_old;
	return code;
    }

    cc.pattern = 0x00;
    for (i=0;i<num_components;i++) 
	cc.paint.values[i] = 1.0;
    code = gs_setcolor(igs, &cc);
    *cont = 1;
    return code;
}
static int validatedevicenspace(i_ctx_t * i_ctx_p, ref **space)
{
    int i, code = 0;
    ref *devicenspace = *space, proc;
    ref nameref, sref, altspace, namesarray, sname;

    /* Check enough arguments in the space */
    if (r_size(devicenspace) < 4)
	return_error(e_rangecheck);
    /* Check the names parameter is an array */
    code = array_get(imemory, devicenspace, 1, &namesarray);
    if (code < 0)
	return code;
    if (!r_is_array(&namesarray))
	return_error(e_typecheck);
    /* Ensure we have at least one ink */
    if (r_size(&namesarray) < 1)
	return_error(e_typecheck);
    /* Make sure no more inks than we can cope with */
    if (r_size(&namesarray) > GS_CLIENT_COLOR_MAX_COMPONENTS)
	return_error(e_limitcheck);
    /* Check the tint transform is a procedure */
    code = array_get(imemory, devicenspace, 3, &proc);
    if (code < 0)
	return code;
    check_proc(proc);

    /* Check the array of ink names only contains names or strings */
    for (i = 0; i < r_size(&namesarray); ++i) {
	array_get(imemory, &namesarray, (long)i, &sname);
	switch (r_type(&sname)) {
		case t_string:
		case t_name:
		    break;
		default:
		    return_error(e_typecheck);
	}
    }

    /* Get the name of the alternate space */
    code = array_get(imemory, devicenspace, 2, &altspace);
    if (code < 0)
	return code;
    if (r_has_type(&altspace, t_name))
	ref_assign(&nameref, &altspace);
    else {
	/* Make sure the alternate space is an array */
	if (!r_is_array(&altspace))
	    return_error(e_typecheck);
	/* And has a name for its type */
	code = array_get(imemory, &altspace, 0, &nameref);
	if (code < 0)
	    return code;
	if (!r_has_type(&nameref, t_name))
	    return_error(e_typecheck);
    }
    /* Convert alternate space name to string */
    name_string_ref(imemory, &nameref, &sref);
    /* Check its not /Indexed, /Pattern, /DeviceN */
    if (r_size(&sref) == 7) {
	if (strncmp((const char *)sref.value.const_bytes, "Indexed", 7) == 0)
	    return_error(e_typecheck);
	if (strncmp((const char *)sref.value.const_bytes, "Pattern", 7) == 0)
	    return_error(e_typecheck);
	if (strncmp((const char *)sref.value.const_bytes, "DeviceN", 7) == 0)
	    return_error(e_typecheck);
    }
    /* and also not /Separation */
    if (r_size(&sref) == 9 && strncmp((const char *)sref.value.const_bytes, "Separation", 9) == 0)
	   return_error(e_typecheck);

    ref_assign(*space, &altspace);
    return 0;
}
static int devicenalternatespace(i_ctx_t * i_ctx_p, ref *space, ref **r, int *CIESubst)
{
    ref altspace;
    int code;

    code = array_get(imemory, space, 2, &altspace);
    if (code < 0)
	return code;
    ref_assign(*r, &altspace);
    return 0;
}
static int devicencomponents(i_ctx_t * i_ctx_p, ref *space, int *n)
{
    ref namesarray;
    int code;

    code = array_get(imemory, space, 1, &namesarray);
    if (code < 0)
	return code;
    *n = r_size(&namesarray); 
    return 0;
}
static int devicendomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, limit, code;
    ref namesarray;

    code = array_get(imemory, space, 1, &namesarray);
    if (code < 0)
	return code;

    limit = r_size(&namesarray) * 2;
    for (i = 0;i < limit;i+=2) {
	ptr[i] = 0;
	ptr[i+1] = 1;
    }
    return 0;
}
static int devicenrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, limit, code;
    PS_colour_space_t *cspace;

    ref altspace;

    code = array_get(imemory, space, 1, &altspace);
    if (code < 0)
	return code;

    code = get_space_object(i_ctx_p, &altspace, &cspace);
    if (code < 0)
	return code;

    code = cspace->numcomponents(i_ctx_p, &altspace, &limit);
    if (code < 0)
	return code;

    for (i = 0;i < limit * 2;i+=2) {
	ptr[i] = 0;
	ptr[i+1] = 1;
    }
    return 0;
}
static int devicentransform(i_ctx_t *i_ctx_p, ref *devicenspace, int *usealternate, int *stage, int *stack_depth)
{
    gx_device * dev = igs->device;
    ref narray, sname, proc;
    int i, code, colorant_number;

    *usealternate = 0;
    code = array_get(imemory, devicenspace, 1, &narray);
    if (code < 0)
	return code;
    if (!r_is_array(&narray)) 
	return_error(e_typecheck);

    for (i=0;i<r_size(&narray);i++) {
	code = array_get(imemory, &narray, i, &sname);
	if (code < 0)
	    return code;
	if (r_has_type(&sname, t_name)) {
	    name_string_ref(imemory, &sname, &sname);
	}

	/* Check for /All and /None, never need the alternate for these */
	if (r_size(&sname) == 3 &&
	    strncmp("All", (const char *)sname.value.bytes, r_size(&sname)) == 0)
	    continue;
	if (r_size(&sname) == 4 &&
	    strncmp("None", (const char *)sname.value.bytes, r_size(&sname)) == 0)
	    continue;
	/*
	 * Compare the colorant name to the device's.  If the device's
	 * compare routine returns GX_DEVICE_COLOR_MAX_COMPONENTS then the
	 * colorant is in the SeparationNames list but not in the
	 * SeparationOrder list.
	 */
	colorant_number = (*dev_proc(dev, get_color_comp_index))
		(dev, (const char *)sname.value.bytes, r_size(&sname), SEPARATION_NAME);
	if (colorant_number < 0) {		/* If not valid colorant name */
	    *usealternate = 1;
	    break;
	}
	if (r_size(&sname) == 4 &&
	    strncmp("Gray", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	    *usealternate = 1;
	    break;
	}
	if (r_size(&sname) == 4 &&
	    strncmp("Cyan", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	    *usealternate = 1;
	    break;
	}
	if (r_size(&sname) == 7 &&
	    strncmp("Magenta", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	    *usealternate = 1;
	    break;
	}
	if (r_size(&sname) == 6 &&
	    strncmp("Yellow", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	    *usealternate = 1;
	    break;
	}
	if (r_size(&sname) == 5 &&
	    strncmp("Black", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	    *usealternate = 1;
	    break;
	}
	if (r_size(&sname) == 3 &&
	    strncmp("Red", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	    *usealternate = 1;
	    break;
	}
	if (r_size(&sname) == 5 &&
	    strncmp("Green", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	    *usealternate = 1;
	    break;
	}
	if (r_size(&sname) == 4 &&
	    strncmp("Blue", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
	    *usealternate = 1;
	    break;
	}
    }
    if (*usealternate && *stage == 0) {
	(*stage)++;
	esp++;
	code = array_get(imemory, devicenspace, 3, &proc);
	if (code < 0)
	    return code;
	*esp = proc;
	return o_push_estack;
    }

    if (*stage == 1){
	*stack_depth = 0;
	*stage = 0;
    }
    return 0;
}
static int devicenbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr  op = osp;   /* required by "push" macro */
    int code, use, n_comp;
    ref narray;

    code = devicentransform(i_ctx_p, space, &use, stage, stack_depth);
    if (code !=  0)
	return code;
    if (!use) {
	*stage = 0;
	*cont = 0;
	code = array_get(imemory, space, 1, &narray);
	if (code < 0)
	    return code;
	n_comp = r_size(&narray);
	pop(n_comp);
	op = osp;
	switch(base) {
	case 0:
	    push(1);
	    make_real(op, 0.0);
	    break;
	case 1:
	case 2:
	    push(3);
	    make_real(&op[-2], 0.0);
	    make_real(&op[-1], 0.0);
	    make_real(op, 0.0);
	    break;
	case 3:
	    push(4);
	    make_real(&op[-3], 0.0);
	    make_real(&op[-2], 0.0);
	    make_real(&op[-1], 0.0);
	    make_real(op, 0.0);
	    break;
	}
    } else {
	*stage = 0;
	*cont = 1;
    }
    return 0;
}
static int devicenvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    int i, code;
    ref narray;
    os_ptr op = osp;

    code = array_get(imemory, space, 1, &narray);
    if (code < 0)
	return code;
    if (!r_is_array(&narray)) 
	return_error(e_typecheck);

    if (num_comps < r_size(&narray))
	return_error(e_stackunderflow);

    op -= r_size(&narray) - 1;

    for (i=0;i < r_size(&narray); i++) {
	if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	    return_error(e_typecheck);

	if (values[i] > 1.0)
	    values[i] = 1.0;

	if (values[i] < 0.0)
	    values[i] = 0.0;
	op++;
    }

    return 0;
}
static int devicencompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    ref sname1, sname2;
    int code;

    code = array_get(imemory, space, 1, &sname1);
    if (code < 0)
	return 0;

    code = array_get(imemory, testspace, 1, &sname2);
    if (code < 0)
	return 0;

    if (!r_is_array(&sname1))
	return 0;
    if (!r_is_array(&sname2))
	return 0;
    
    if (!comparearrays(i_ctx_p, &sname1, &sname2))
	return 0;

    code = array_get(imemory, testspace, 2, &sname1);
    if (code < 0)
	return 0;
    code = array_get(imemory, testspace, 2, &sname2);
    if (code < 0)
	return 0;
    if (r_type(&sname1) != r_type(&sname2))
	return 0;

    if (r_is_array(&sname1)) {
	if (!comparearrays(i_ctx_p, &sname1, &sname2))
	    return 0;
    } else {
	if (!r_has_type(&sname1, t_name))
	    return 0;
	if (!name_eq(&sname1, &sname2))
	    return 0;
    }
    code = array_get(imemory, space, 3, &sname1);
    if (code < 0)
	return 0;
    code = array_get(imemory, testspace, 3, &sname2);
    if (code < 0)
	return 0;
    return(comparearrays(i_ctx_p, &sname1, &sname2));
}
static int deviceninitialproc(i_ctx_t *i_ctx_p, ref *space)
{
    gs_client_color cc;
    int i, num_components, code;
    ref namesarray;

    code = array_get(imemory, space, 1, &namesarray);
    if (code < 0)
	return code;
    num_components = r_size(&namesarray);
    cc.pattern = 0x00;
    for (i=0;i<num_components;i++) 
	cc.paint.values[i] = 1.0;
    return gs_setcolor(igs, &cc);
}

/* Indexed */
/*
 * This routine samples the indexed space, it pushes values from -1
 * to 'hival', then executes the tint transform procedure. Returns here
 * to store the resulting value(s) in the indexed map.
 */
static int
indexed_cont(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    es_ptr ep = esp;
    int i = (int)ep[csme_index].value.intval;

    if (i >= 0) {		/* i.e., not first time */
	int m = (int)ep[csme_num_components].value.intval;
	int code = float_params(op, m, &r_ptr(&ep[csme_map], gs_indexed_map)->values[i * m]);

	if (code < 0)
	    return code;
	pop(m);
	op -= m;
	if (i == (int)ep[csme_hival].value.intval) {	/* All done. */
	    esp -= num_csme;
	    return o_pop_estack;
	}
    }
    push(1);
    ep[csme_index].value.intval = ++i;
    make_int(op, i);
    make_op_estack(ep + 1, indexed_cont);
    ep[2] = ep[csme_proc];	/* lookup proc */
    esp = ep + 2;
    return o_push_estack;
}
static int setindexedspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    ref *pproc = &istate->colorspace.procs.special.index_proc;
    int code = 0;
    uint edepth = ref_stack_count(&e_stack);
    ref_colorspace cspace_old;
    ref hival, lookup;
    gs_color_space *pcs;
    gs_color_space *pcs_base;

    if (i_ctx_p->language_level < 2)
	return_error(e_undefined);

    *cont = 0;
    if (*stage == 1) {
	*stage = 0;
	return 0;
    }

    cspace_old = istate->colorspace;

    pcs_base = gs_currentcolorspace(igs);

    code = array_get(imemory, r, 3, &lookup);
    if (code < 0)
	return code;
    code = array_get(imemory, r, 2, &hival);
    if (code < 0)
	return code;
    if (r_has_type(&lookup, t_string)) {
	int num_values = (hival.value.intval + 1) * cs_num_components(pcs_base);

	check_read(lookup);
	/*
	 * The PDF and PS specifications state that the lookup table must have
	 * the exact number of of data bytes needed.  However we have found
	 * PDF files from Amyuni with extra data bytes.  Acrobat 6.0 accepts
	 * these files without complaint, so we ignore the extra data.
	 */
	if (r_size(&lookup) < num_values)
	    return_error(e_rangecheck);
	pcs = gs_cspace_alloc(imemory, &gs_color_space_type_Indexed);
	pcs->base_space = pcs_base;
	rc_increment(pcs_base);
	pcs->params.indexed.lookup.table.data = lookup.value.const_bytes;
	pcs->params.indexed.lookup.table.size = num_values;
	pcs->params.indexed.use_proc = 0;
	make_null(pproc);
	code = 0;
    } else {
	gs_indexed_map *map;

	/*
	 * We have to call zcs_begin_map before moving the parameters,
	 * since if the color space is a DeviceN or Separation space,
	 * the memmove will overwrite its parameters.
	 */
	code = zcs_begin_map(i_ctx_p, &map, &lookup, (hival.value.intval + 1),
			     pcs_base, indexed_cont);
	if (code < 0)
	    return code;
	pcs = gs_cspace_alloc(imemory, &gs_color_space_type_Indexed);
	pcs->base_space = pcs_base;
	rc_increment(pcs_base);
	pcs->params.indexed.use_proc = 1;
	*pproc = lookup;
	map->proc.lookup_index = lookup_indexed_map;
	pcs->params.indexed.lookup.map = map;
    }
    pcs->params.indexed.hival = hival.value.intval;
    pcs->params.indexed.n_comps = cs_num_components(pcs_base);
    code = gs_setcolorspace(igs, pcs);
    /* release reference from construction */
    rc_decrement_only(pcs, "setindexedspace");
    if (code < 0) {
	istate->colorspace = cspace_old;
	ref_stack_pop_to(&e_stack, edepth);
	return code;
    }
    *stage = 0;
    if (ref_stack_count(&e_stack) == edepth) {
	return 0;
    } else {
	*cont = 1;
	*stage = 1;
	return o_push_estack; /* installation will load the caches */
    }
}
static int validateindexedspace(i_ctx_t * i_ctx_p, ref **space)
{
    int code = 0;
    ref *r = *space;
    ref nameref, sref, hival, lookup, altspace;

    if (!r_is_array(r))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(r) != 4)
	return_error(e_rangecheck);
    /* Check operand type(s) */
    /* Make sure 'hival' is an integer */
    code = array_get(imemory, r, 2, &hival);
    if (code < 0)
	return code;
    if (!r_has_type(&hival, t_integer))
	return_error(e_typecheck);
    /* Make sure 'hival' lies between 0 and 4096 */
    if (hival.value.intval < 0 || hival.value.intval > 4096)
	return_error(e_rangecheck);
    /* Ensure the 'lookup' is either a string or a procedure */
    code = array_get(imemory, r, 3, &lookup);
    if (code < 0)
	return code;
    if (!r_has_type(&lookup, t_string))
	check_proc(lookup);

    /* Get the name of the alternate space */
    code = array_get(imemory, r, 1, &altspace);
    if (code < 0)
	return code;
    if (r_has_type(&altspace, t_name))
	ref_assign(&nameref, &altspace);
    else {
	if (!r_is_array(&altspace))
	    return_error(e_typecheck);
	code = array_get(imemory, &altspace, 0, &nameref);
	if (code < 0) 
	    return code;
    }
    /* Convert alternate space name to string */
    name_string_ref(imemory, &nameref, &sref);
    /* Check its not /Indexed or /Pattern */
    if (r_size(&sref) == 7) {
	if (strncmp((const char *)sref.value.const_bytes, "Indexed", 7) == 0)
	    return_error(e_typecheck);
	if (strncmp((const char *)sref.value.const_bytes, "Pattern", 7) == 0)
	    return_error(e_typecheck);
    }
    ref_assign(*space, &altspace);
    return 0;
}
static int indexedalternatespace(i_ctx_t * i_ctx_p, ref *space, ref **r, int *CIESubst)
{
    ref alt;
    int code;

    code = array_get(imemory, *r, 1, &alt);
    if (code < 0)
	return code;
    ref_assign(*r, &alt);
    return 0;
}
static int indexeddomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    ref hival;
    int code;

    code = array_get(imemory, space, 2, &hival);
    if (code < 0)
	return code;
    ptr[0] = 0;
    ptr[1] = (float)hival.value.intval;
    return 0;
}
static int indexedrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    ref hival;
    int code;

    code = array_get(imemory, space, 2, &hival);
    if (code < 0)
	return code;
    ptr[0] = 0;
    ptr[1] = (float)hival.value.intval;
    return 0;
}
static int indexedbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    es_ptr ep = ++esp;
    ref proc;
    int code;

    if (*stage == 0) {
	*stage = 1;
	*cont = 1;
	check_estack(1);
	code = array_get(imemory, space, 3, &proc);
	if (code < 0)
	    return code;
        *ep = proc;	/* lookup proc */
	return o_push_estack;
    } else {
	*stage = 0;
	*cont = 1;
	return 0;
    }
}
static int indexedvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    int code;
    ref hival;
    os_ptr op = osp;

    if (num_comps < 1) 
	return_error(e_stackunderflow);

    if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
	return_error(e_typecheck);

    code = array_get(imemory, space, 2, &hival);
    if (code < 0)
	return code;

    if (*values > hival.value.intval)
	*values = (float)hival.value.intval;

    if (*values < 0)
	*values = 0;

    /* The PLRM says 'If it is a real number, it is rounded to the nearest integer
     * but in fact Acrobat simply floors the value.
     */
    *values = floor(*values);

    return 0;
}

/* Pattern */
static int setpatternspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    gs_color_space *pcs;
    gs_color_space *pcs_base;
    uint edepth = ref_stack_count(&e_stack);
    int code = 0;

    if (i_ctx_p->language_level < 2)
	return_error(e_undefined);

    *cont = 0;
    pcs_base = NULL;
    if (r_is_array(r)) {
	check_read(*r);

	switch (r_size(r)) {
	    case 1:		/* no base space */
		pcs_base = NULL;
		break;
	    default:
		return_error(e_rangecheck);
	    case 2:
		pcs_base = gs_currentcolorspace(igs);
		if (cs_num_components(pcs_base) < 0)       /* i.e., Pattern space */
		    return_error(e_rangecheck);
	}
    }
    pcs = gs_cspace_alloc(imemory, &gs_color_space_type_Pattern);
    pcs->base_space = pcs_base;
    pcs->params.pattern.has_base_space = (pcs_base != NULL);
    rc_increment(pcs_base);
    code = gs_setcolorspace(igs, pcs);
    /* release reference from construction */
    rc_decrement_only(pcs, "zsetpatternspace");
    if (code < 0) {
	ref_stack_pop_to(&e_stack, edepth);
	return code;
    }
    make_null(&istate->pattern); /* PLRM: initial color value is a null object */
    *stage = 0;
    return (ref_stack_count(&e_stack) == edepth ? 0 : o_push_estack);	/* installation will load the caches */
}
static int validatepatternspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code;
    ref tref;

    /* since makepattern has already been run, we don't need to do much validation */
    if (!r_has_type(*r, t_name)) {
	if (r_is_array(*r)) {
	    if (r_size(*r) > 1) {
		code = array_get(imemory, *r, 1, &tref);
		if (code < 0)
		    return code;
		ref_assign(*r, &tref);
	    } else
	        *r = 0;
	} else
	    return_error(e_typecheck);
    } else
	*r = 0;
    return 0;
}
static int patternalternatespace(i_ctx_t * i_ctx_p, ref *space, ref **r, int *CIESubst)
{
    ref tref;
    int code;

    if (!r_has_type(*r, t_name)) {
	if (r_is_array(*r)) {
	    if (r_size(*r) > 1) {
		code = array_get(imemory, space, 1, &tref);
		if (code < 0)
		    return code;
		ref_assign(*r, &tref);
	    } else
	        *r = 0;
	} else
	    return_error(e_typecheck);
    } else
	*r = 0;
    return 0;
}
static int patterncomponent(i_ctx_t * i_ctx_p, ref *space, int *n)
{
    os_ptr op = osp;
    int n_comps, code;
    const gs_color_space *  pcs = gs_currentcolorspace(igs);
    gs_client_color         cc;

    /* check for a pattern color space */
    if ((n_comps = cs_num_components(pcs)) < 0) {
        n_comps = -n_comps;
        if (r_has_type(op, t_dictionary)) {
            ref     *pImpl, pPatInst;

	    code = dict_find_string(op, "Implementation", &pImpl);
	    if (code < 0)
		return code;
	    code = array_get(imemory, pImpl, 0, &pPatInst);
	    if (code < 0)
		return code;
	    cc.pattern = r_ptr(&pPatInst, gs_pattern_instance_t);
	    if (pattern_instance_uses_base_space(cc.pattern))
		*n = n_comps;
	    else
		*n = 1;
        } else
	    *n = 1;
    } else
        return_error(e_typecheck);

    return 0;
}
static int patternbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op;
    int i, components=0;

    if (r_size(space) > 1) {
	const gs_color_space *  pcs = gs_currentcolorspace(igs);
	const gs_client_color * pcc = gs_currentcolor(igs);
	int                     n = cs_num_components(pcs);
	bool                    push_pattern = n < 0;
	gs_pattern_instance_t * pinst = pcc->pattern;

	if (pinst != 0 && pattern_instance_uses_base_space(pinst)) {
	    /* check for pattern */
	    if (push_pattern) 
		pop(1);	    /* The pattern instance */
	    *stage = 0;
	    *cont = 1;
	    return 0;
	}
	/* If the pattern isn't yet initialised, or doesn't use the
	 * base space, treat as uncolored and return defaults below
	 * Fall Through.
	 */
    } 

    pop(1);
    op = osp;
    switch(base) {
	case 0:
	    components = 1;
	    break;
	case 1:
	case 2:
	    components = 3;
	    break;
	case 3:
	    components = 4;
	    break;
    }
    push(components);
    op -= components-1;
    for (i=0;i<components;i++) {
	make_real(op, (float)0);
	op++;
    }
    if (components == 4) {
	op--;
	make_real(op, (float)1);
    }
    *stage = 0;
    *cont = 0;
    return 0;
}
static int patternvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;

    check_op(1);

    if (!r_has_type(op, t_dictionary) && !r_has_type(op, t_null))
	return_error(e_typecheck);

    return 0;
}

/* DevicePixel */
static int setdevicepspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    int code = 0;
    gs_color_space *pcs;
    ref bpp;

    /* The comment in the original PostScript (gs_lev2.ps) said
     * "DevicePixel is actually a LanguageLevel 3 feature; it is here for
     *  historical reasons." Actually DevicePixel is a Display PostScript
     * space, as far as I can tell. It certainly isn't a level 3 space.
     * Preserve the old behaviour anyway.
     */
    if (i_ctx_p->language_level < 2)
	return_error(e_undefined);

    *cont = 0;
    code = array_get(imemory, r, 1, &bpp);
    if (code < 0)
	return code;
    if (!r_has_type(&bpp, t_integer))
	return_error(e_typecheck);
    code = gs_cspace_new_DevicePixel(imemory, &pcs, (int)bpp.value.intval);
    if (code < 0)
	return code;
    code = gs_setcolorspace(igs, pcs);
    /* release reference from construction */
    *stage = 0;
    rc_decrement_only(pcs, "setseparationspace");
    return code;
}
static int validatedevicepspace(i_ctx_t * i_ctx_p, ref **space)
{
    int code = 0;
    ref *r = *space, bpp;

    if (!r_is_array(r))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(r) != 2)
	return_error(e_rangecheck);
    /* Make sure 'bits per pixel' is an integer */
    code = array_get(imemory, r, 1, &bpp);
    if (code < 0)
	return code;
    if (!r_has_type(&bpp, t_integer))
	return_error(e_typecheck);

    /* Make sure 'bits per pixel' lies between 0 and 31 */
    if (bpp.value.intval < 0 || bpp.value.intval > 31)
	return_error(e_rangecheck);

    *space = 0;
    return code;
}
static int devicepdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int code;
    ref tref;

    code = array_get(imemory, space, 1, &tref);
    if (code < 0)
	return code;
    ptr[0] = 0;
    ptr[1] = (float)(1 << tref.value.intval);
    return 0;
}
static int deviceprange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int code;
    ref tref;

    code = array_get(imemory, space, 1, &tref);
    if (code < 0)
	return code;
    ptr[0] = 0;
    ptr[1] = (float)(1 << tref.value.intval);
    return 0;
}
static int devicepbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr  op = osp;

    *stage = 0;
    *cont = 0;
    make_int(op, 0);
    return 0;
}
static int devicepvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    return 0;
}

/* ICCBased */
static int iccrange(i_ctx_t * i_ctx_p, ref *space, float *ptr);
static int seticcspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;
    ref     ICCdict, *tempref, *altref=NULL, *icc, *nocie;
    int components, code;
    float range[8];

    code = dict_find_string(systemdict, ".seticcspace", &icc);
    if (code < 0)
	return_error(e_undefined);

    code = dict_find_string(systemdict, "NOCIE", &nocie);
    if (code < 0)
	return code;
    if (!r_has_type(nocie, t_boolean))
	return_error(e_typecheck);

    *cont = 0;
    do {
	switch(*stage) {
	    case 0:
		(*stage)++;
		code = array_get(imemory, r, 1, &ICCdict);
		if (code < 0)
		    return code;

		code = dict_find_string(&ICCdict, "N", &tempref);
		if (code < 0)
		    return code;
		components = tempref->value.intval;

		/* Don't allow ICCBased spaces if NOCIE is true */
		if (nocie->value.boolval) {
		    code = dict_find_string(&ICCdict, "Alternate", &altref); /* Alternate is optional */
		    if (code < 0)
			return code;
		    if ((altref != NULL) && (r_type(altref) != t_null)) {
			/* The PDF interpreter sets a null Alternate. If we have an
			 * Alternate, and its not null, and NOCIE is true, then use the 
			 * Alternate instead of the ICC
			 */
			push(1);
			ref_assign(op, altref);
			/* If CIESubst, we are already substituting for CIE, so use nosubst 
			 * to prevent further substitution!
			 */
			return setcolorspace_nosubst(i_ctx_p);
		    } else {
			/* There's no /Alternate (or it is null), set a default space
			 * based on the number of components in the ICCBased space
			 */
			int stage1 = 1, cont1 = 0;
			switch(components) {
			    case 1:
				code = setgrayspace(i_ctx_p, (ref *)0x00, &stage1, &cont1, 1);
				if (code != 0)
				    return code;
				*stage = 0;
				break;
			    case 3:
				code = setrgbspace(i_ctx_p, (ref *)0x00, &stage1, &cont1, 1);
				if (code != 0)
				    return code;
				*stage = 0;
				break;
			    case 4:
				code = setcmykspace(i_ctx_p, (ref *)0x00, &stage1, &cont1, 1);
				if (code != 0)
				    return code;
				*stage = 0;
				break;
			    default:
				return_error(e_rangecheck);
				break;
			}
		    }
		} else {
		    code = iccrange(i_ctx_p, r, (float *)&range);
		    if (code < 0)
			return code;
		    code = dict_find_string(&ICCdict, "DataSource", &tempref);
		    if (code < 0)
			return code;
		    /* Check for string based ICC and convert to a file (stage = 2) */
		    if (!r_has_type(tempref, t_file)){
			ref stref;
			byte *body;

			(*stage)++;
			body = ialloc_string(48, "string");
			if (body == 0)
			    return_error(e_VMerror);
			memcpy(body, "{systemdict /.convertICCSource get exec} stopped",48);
			make_string(&stref, a_all | icurrent_space, 48, body);
			r_set_attrs(&stref, a_executable);
			esp++;
			ref_assign(esp, &stref);
			return o_push_estack;
		    }
		    /* Make space on operand stack to pass the ICC dictionary */
		    push(1);
		    ref_assign(op, &ICCdict);
		    code = seticc(i_ctx_p, components, op, (float *)&range);
		    if (code < 0) {
			if (altref) {
			    /* We have a /Alternate in the ICC space */
			    /* Our ICC dictionary still on operand stack, we can reuse the
			     * slot on the stack to hold the alternate space.
			     */
			    ref_assign(op, (ref *)&altref);
			    /* If CIESubst, we are already substituting for CIE, so use nosubst 
			     * to prevent further substitution!
			     */
			    if (CIESubst) 
				return setcolorspace_nosubst(i_ctx_p);
			    else
				return zsetcolorspace(i_ctx_p);
			} else {
			    /* We have no /Alternate in the ICC space, use hte /N key to
			     * determine an 'appropriate' default space.
			     */
			    int stage1 = 1, cont1 = 0;
			    switch(components) {
				case 1:
				    code = setgrayspace(i_ctx_p, (ref *)0x00, &stage1, &cont1, 1);
				    if (code != 0)
					return code;
				    *stage = 0;
				    break;
				case 3:
				    code = setrgbspace(i_ctx_p, (ref *)0x00, &stage1, &cont1, 1);
				    if (code != 0)
					return code;
				    *stage = 0;
				    break;
				case 4:
				    code = setcmykspace(i_ctx_p, (ref *)0x00, &stage1, &cont1, 1);
				    if (code != 0)
					return code;
				    *stage = 0;
				    break;
				default:
				    return_error(e_rangecheck);
				    break;
			    }
			}
			pop(1);
		    }
		    if (code != 0)
			return code;
		}
		break;
	    case 1:
		/* All done, exit */
		*stage = 0;
		code = 0;
		break;
	    case 2:
		/* Converted a string DataSource ICC profile into a file
		 * using ReusableStreamDecode. See gs_cspace.ps.
		 */
		*stage = 1;
		code = array_get(imemory, r, 1, &ICCdict);
		if (code < 0)
		    return code;
		code = iccrange(i_ctx_p, r, (float *)&range);
		if (code < 0)
		    return code;
		code = dict_find_string(&ICCdict, "N", &tempref);
		if (code < 0)
		    return code;
		components = tempref->value.intval;

		/* Make space on operand stack to pass the ICC dictionary */
		push (1);
		ref_assign(op, &ICCdict);
		code = seticc(i_ctx_p, components, op, (float *)&range);
		if (code < 0) {
    		    code = dict_find_string(&ICCdict, "Alternate", &altref);
		    if (code < 0)
			make_null(altref);	/* no Alternate -- just use null */
		    /* Our dictionary still on operand stack, we can reuse the
		     * slot on the stack to hold the alternate space.
		     */
		    ref_assign(op, altref);
		    /* If CIESubst, we are already substituting for CIE, so use nosubst 
		     * to prevent further substitution!
		     */
		    if (CIESubst) 
		        return setcolorspace_nosubst(i_ctx_p);
		    else
		        return zsetcolorspace(i_ctx_p);
		}
		break;
	    default:
		return_error (e_rangecheck);
		break;
	}
    }while(*stage);
    return code;
}
static int validateiccspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code=0, i, components = 0;
    ref *space, *tempref, valref, ICCdict, sref;
    
    space = *r;
    if (!r_is_array(space))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(space) != 2)
	return_error(e_rangecheck);

    code = array_get(imemory, space, 1, &ICCdict);
    if (code < 0)
	return code;

    code = dict_find_string(&ICCdict, "N", &tempref);
    if (code <= 0)
	return code;
    if (!r_has_type(tempref, t_null)) {
	if (!r_has_type(tempref, t_integer))
	    return_error(e_typecheck);
	components = tempref->value.intval;
    } else
	return_error(e_typecheck);
    code = dict_find_string(&ICCdict, "DataSource", &tempref);
    if (code <= 0)
	return_error(e_typecheck);
    if (!r_has_type(tempref, t_null)) {
	if (!r_has_type(tempref, t_string) && !r_has_type(tempref, t_file))
	    return_error(e_typecheck);
    } else
	return_error(e_typecheck);

    /* Following are optional entries */
    code = dict_find_string(&ICCdict, "Range", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	if (!r_is_array(tempref))
	    return_error(e_typecheck);
	if (r_size(tempref) < (components * 2))
	    return_error(e_rangecheck);
	for (i=0;i<components * 2;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (!r_has_type(&valref, t_integer) && !r_has_type(&valref, t_real))
		return_error(e_typecheck);
	}
    }
    code = dict_find_string(&ICCdict, "Alternate", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	*r = tempref;
	if (r_has_type(tempref, t_name)) {
	    name_string_ref(imemory, tempref, &sref);
	    if (sref.value.bytes && strncmp((const char *)sref.value.bytes, "Pattern", 7) == 0)
		return_error(e_typecheck);
	} else {
	    if (r_is_array(tempref)) {
		code = array_get(imemory, tempref, 0, &valref);
		if (code < 0)
		    return code;
		if (!r_has_type(&valref, t_name) && !r_has_type(&valref, t_string))
		    return_error(e_typecheck);
		if (r_has_type(&valref, t_name)) 
		    name_string_ref(imemory, &valref, &sref);
		else
		    sref.value.bytes = valref.value.bytes;
		if (sref.value.bytes && strncmp((const char *)sref.value.bytes, "Pattern", 7) == 0)
		    return_error(e_typecheck);
	    } else 
		return_error(e_typecheck);
	}
    } else {
	switch (components) {
	    case 1:
		code = name_enter_string(imemory, "DeviceGray", *r);
		break;
	    case 3:
		code = name_enter_string(imemory, "DeviceRGB", *r);
		break;
	    case 4:
		code = name_enter_string(imemory, "DeviceCMYK", *r);
		break;
	    default:
		return_error(e_rangecheck);
	}
    }
    return code;
}
static int iccalternatespace(i_ctx_t * i_ctx_p, ref *space, ref **r, int *CIESubst)
{
    int components, code = 0;
    ref *tempref, ICCdict;

    if (!r_is_array(space))
	return_error(e_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(space) != 2)
	return_error(e_rangecheck);

    code = array_get(imemory, space, 1, &ICCdict);
    if (code < 0)
	return code;

    code = dict_find_string(&ICCdict, "N", &tempref);
    if (code <= 0)
	return code;

    components = tempref->value.intval;

    code = dict_find_string(&ICCdict, "Alternate", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	*r = tempref;
    } else {
	switch (components) {
	    case 1:
		code = name_enter_string(imemory, "DeviceGray", *r);
		break;
	    case 3:
		code = name_enter_string(imemory, "DeviceRGB", *r);
		break;
	    case 4:
		code = name_enter_string(imemory, "DeviceCMYK", *r);
		break;
	    default:
		return_error(e_rangecheck);
	}
    }
    *CIESubst = 1;
    return code;
}
static int icccomponents(i_ctx_t * i_ctx_p, ref *space, int *n)
{
    int code = 0;
    ref *tempref, ICCdict;

    code = array_get(imemory, space, 1, &ICCdict);
    if (code < 0)
	return code;

    code = dict_find_string(&ICCdict, "N", &tempref);
    *n = tempref->value.intval;
    return 0;
}
static int iccdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int components, i, code = 0;
    ref *tempref, ICCdict, valref;

    code = array_get(imemory, space, 1, &ICCdict);
    if (code < 0)
	return code;

    code = dict_find_string(&ICCdict, "N", &tempref);
    components = tempref->value.intval;

    code = dict_find_string(&ICCdict, "Range", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<components * 2;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i * 2] = (float)valref.value.intval;
	    else
		ptr[i * 2] = valref.value.realval;
	}
    } else {
	for (i=0;i<components;i++) {
	    ptr[i * 2] = 0;
	    ptr[(i * 2) + 1] = 1;
	}
    }
    return 0;
}
static int iccrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int components, i, code = 0;
    ref *tempref, ICCdict, valref;

    code = array_get(imemory, space, 1, &ICCdict);
    if (code < 0)
	return code;

    code = dict_find_string(&ICCdict, "N", &tempref);
    components = tempref->value.intval;

    code = dict_find_string(&ICCdict, "Range", &tempref);
    if (code >= 0 && !r_has_type(tempref, t_null)) {
	for (i=0;i<components * 2;i++) {
	    code = array_get(imemory, tempref, i, &valref);
	    if (code < 0)
		return code;
	    if (r_has_type(&valref, t_integer))
		ptr[i * 2] = (float)valref.value.intval;
	    else
		ptr[i * 2] = (float)valref.value.realval;
	}
    } else {
	for (i=0;i<components;i++) {
	    ptr[i * 2] = 0;
	    ptr[(i * 2) + 1] = 1;
	}
    }
    return 0;
}
static int iccbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    *stage = 0;
    *cont = 1;
    return 0;
}
static int iccvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    return 0;
}

static int dummydomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    return 0;
}
static int dummyrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    return 0;
}
static int onecomponent(i_ctx_t * i_ctx_p, ref *space, int *n)
{
    *n = 1;
    return 0;
}
static int threecomponent(i_ctx_t * i_ctx_p, ref *space, int *n)
{
    *n = 3;
    return 0;
}
static int fourcomponent(i_ctx_t * i_ctx_p, ref *space, int *n)
{
    *n = 4;
    return 0;
}
static int truecompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    return 1;
}
static int falsecompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    return 0;
}

PS_colour_space_t colorProcs[] = {
    {(char *)"DeviceGray", setgrayspace, 0, 0, onecomponent, grayrange, graydomain, 
    graybasecolor, 0, grayvalidate, truecompareproc, grayinitialproc}, 
    {(char *)"DeviceRGB", setrgbspace, 0, 0, threecomponent, rgbrange, rgbdomain, 
    rgbbasecolor, 0, rgbvalidate, truecompareproc, rgbinitialproc},
    {(char *)"DeviceCMYK", setcmykspace, 0, 0, fourcomponent, cmykrange, cmykdomain, 
    cmykbasecolor, 0, cmykvalidate, truecompareproc, cmykinitialproc},
    {(char *)"CIEBasedA", setcieaspace, validatecieaspace, 0, onecomponent, ciearange, cieadomain,
    ciebasecolor, 0, cieavalidate, cieacompareproc, 0},
    {(char *)"CIEBasedABC", setcieabcspace, validatecieabcspace, 0, threecomponent, cieabcrange, cieabcdomain,
    ciebasecolor, 0, cieabcvalidate, cieabccompareproc, 0},
    {(char *)"CIEBasedDEF", setciedefspace, validateciedefspace, 0, threecomponent, ciedefrange, ciedefdomain,
    ciebasecolor, 0, ciedefvalidate, ciedefcompareproc, 0},
    {(char *)"CIEBasedDEFG", setciedefgspace, validateciedefgspace, 0, fourcomponent, ciedefgrange, ciedefgdomain,
    ciebasecolor, 0, ciedefgvalidate, ciedefgcompareproc, 0},
    {(char *)"Separation", setseparationspace, validateseparationspace, separationalternatespace, onecomponent, seprange, sepdomain,
    sepbasecolor, septransform, sepvalidate, sepcompareproc, sepinitialproc},
    {(char *)"DeviceN", setdevicenspace, validatedevicenspace, devicenalternatespace, devicencomponents, devicenrange, devicendomain,
    devicenbasecolor, devicentransform, devicenvalidate, devicencompareproc, deviceninitialproc},
    {(char *)"Indexed", setindexedspace, validateindexedspace, indexedalternatespace, onecomponent, indexedrange, indexeddomain,
    indexedbasecolor, 0, indexedvalidate, falsecompareproc, 0},
    {(char *)"Pattern", setpatternspace, validatepatternspace, patternalternatespace, patterncomponent, dummyrange, dummydomain,
    patternbasecolor, 0, patternvalidate, falsecompareproc, 0},
    {(char *)"DevicePixel", setdevicepspace, validatedevicepspace, 0, onecomponent, deviceprange, devicepdomain,
    devicepbasecolor, 0, devicepvalidate, falsecompareproc, 0},
    {(char *)"ICCBased", seticcspace, validateiccspace, iccalternatespace, icccomponents, iccrange, iccdomain,
    iccbasecolor, 0, iccvalidate, falsecompareproc, 0},
};

/*
 * Given a color space, this finds the appropriate object from the list above 
 */
int get_space_object(i_ctx_t *i_ctx_p, ref *arr, PS_colour_space_t **obj)
{
    ref spacename, nref;
    int i, nprocs = sizeof(colorProcs) / sizeof(PS_colour_space_t), code;

    /* If the spaece is an array, the first element is always the name */
    if (r_is_array(arr))
	code = array_get(imemory, arr, 0, &spacename);
    else
	ref_assign(&spacename, arr);

    /* Check that it really is a name */
    if (!r_has_type(&spacename, t_name))
	return_error(e_typecheck);

    /* Find the relevant color space object */
    for (i=0;i<nprocs;i++) {
	code = names_ref(imemory->gs_lib_ctx->gs_name_table, (const byte *)colorProcs[i].name, strlen(colorProcs[i].name), &nref, 0);
	if (code < 0)
	    return code;
	if (name_eq(&spacename, &nref)) {
	    *obj = &colorProcs[i];
	    return 0;
	}
    }
    return_error(e_undefined);
}
/*
 * This routine checks all the color spaces in an operand by 
 * calling the specific 'validate' method for each in turn. It also 
 * returns the 'depth' which is the number of nested spaces.
 */
static int validate_spaces(i_ctx_t *i_ctx_p, ref *arr, int *depth)
{
    ref space, *sp = &space;
    int code = 0;
    PS_colour_space_t *obj;

    ref_assign(&space, arr);
    *depth = 0;
    do {
	code = get_space_object(i_ctx_p, sp, &obj);
	if (code < 0)
	    return code;

	(*depth)++;
	if (!obj->validateproc)
	    break;

	code = obj->validateproc(i_ctx_p, &sp);
	if (code < 0)
	    return code;
    }while(sp);
    return 0;
}
/*
 * The routine which does all the setcolor dispatching. This is initially set up by
 * zsetcolor above. Because setcolorspace samples the space and converts the tint
 * transform to a function, we don't need to run the PS tint transform in order to
 * set the color. However, some applications, notably Photoshop 5 and above, rely
 * on the tint transform being executed, so we must do so if the normal PostScript
 * processing would result in the tintr transform being executed.
 *
 *  We check each space in turn to see whether we would normally run the tint 
 * transform, eg Indexed is always executed, Separation and DeviceN only if the 
 * required ink(s) aren't present in the device. If we discover that any space
 * doesn't require a tint transform, then we can short-circuit the processing.
 * Otherwise we set up to execute the tint transform.
 */
static int
setcolor_cont(i_ctx_t *i_ctx_p)
{
    ref arr, *parr = &arr;
    es_ptr ep = esp;
    int i=0, code = 0,depth, usealternate, stage, stack_depth, CIESubst = 0;
    PS_colour_space_t *obj;
    
    stack_depth = (int)ep[-3].value.intval;
    depth = (int)ep[-2].value.intval;
    stage = (int)ep[-1].value.intval;
    /* If we get a continuation from a sub-procedure, we will want to come back
     * here afterward, to do any remaining spaces. We need to set up for that now.
     * so that our continuation is ahead of the sub-proc's continuation.
     */
    check_estack(1);
    push_op_estack(setcolor_cont);
   
    while (code == 0) {
	ref_assign(&arr, ep);
	/* Run along the nested color spaces until we get to the first one
	 * that we haven't yet processed (given by 'depth')
	 */
	for (i=0;i<=depth;i++) {
	    code = get_space_object(i_ctx_p, parr, &obj);
	    if (code < 0) 
	        return code;

	    if (i < (depth)) {
		if (!obj->alternateproc) {
		    return_error(e_typecheck);
		}
		code = obj->alternateproc(i_ctx_p, parr, &parr, &CIESubst);
	        if (code < 0) 
		    return code;
	    }
	}
	if (obj->runtransformproc) {
	    code = obj->runtransformproc(i_ctx_p, &istate->colorspace.array, &usealternate, &stage, &stack_depth);
	    make_int(&ep[-3], stack_depth);
	    make_int(&ep[-1], stage);
	    if (code != 0) {
		return code;
	    }
	    make_int(&ep[-2], ++depth);
	    if (!usealternate)
		break;
	} else
	    break;
    }
    /* Remove our next continuation and our data */
    obj->numcomponents(i_ctx_p, parr, &i);
    pop(i);
    esp -= 5;
    return o_pop_estack;
}
/*
 * The routine which does all the setcolorspace dispatching. This is initially set up by
 * zsetcolorspace above. It starts by descending to the bottom-most space
 * and setting that as the current space. It then descends the array again
 * to the next-to-bottom- space and sets that as the current, and so on.
 *
 * The 'stage' parameter is passed in to each 'set' method. If a method needs
 * to do a continuation itself (eg sample a space) then it should set the stage
 * to a non-zero value. When the continuation is complete we return here, and
 * attempt to 'set' the same space again. This time stage will be whatever was
 * set the first time, which is a signal to the 'set' routine that a continuation
 * took place, and is complete. Stage must always be set to 0 when a 'set'
 * of a color space is complete.
 */
static int
setcolorspace_cont(i_ctx_t *i_ctx_p)
{
    ref arr, *parr = &arr;
    os_ptr op = osp;
    es_ptr ep = esp, pdepth, pstage, pCIESubst;
    int i, code = 0,depth, stage, cont, CIESubst = 0;
    PS_colour_space_t *obj;

    pCIESubst = &ep[-3];
    pdepth = &ep[-2];
    pstage = &ep[-1];

    CIESubst = (int)pCIESubst->value.intval;
    depth = (int)pdepth->value.intval;
    stage = (int)pstage->value.intval;
    /* If we get a continuation from a sub-procedure, we will want to come back
     * here afterward, to do any remaining stages. We need to set up for that now.
     * so that our continuation is ahead of the sub-proc's continuation.
     */
    check_estack(1);
    push_op_estack(setcolorspace_cont);
   
    while (code == 0 && depth) {
	ref_assign(&arr, ep);
	/* Run along the nested color spaces until we get to the lowest one
	 * that we haven't yet processed (given by 'depth')
	 */
	for (i = 0;i < depth;i++) {
	    code = get_space_object(i_ctx_p, parr, &obj);
 	    if (code < 0) 
	        return code;

	    if (i < (depth - 1)) {
		if (!obj->alternateproc) {
		    return_error(e_typecheck);
		}
		code = obj->alternateproc(i_ctx_p, parr, &parr, &CIESubst);
	        if (code < 0) 
		    return code;
	    }
	}

	code = obj->setproc(i_ctx_p, parr, &stage, &cont, CIESubst);
	make_int(pstage, stage);
	if (code != 0) 
	    return code;
	if (!cont) {
	    /* Completed that space, decrement the 'depth' */
	    make_int(pdepth, --depth);
	    parr = &arr;
	}
    }
    if (code == 0) {
        /* Remove our next continuation and our data */
	esp -= 5;
	op = osp;
	istate->colorspace.array = *op;
	/* Remove the colorspace array form the operand stack */
	pop(1);
	code = o_pop_estack;
    }
    return code;
}
/*
 * The routine which does all the dispatching for the device-space specific
 * operators below (eg setgray). This is initially set up by the routines below.
 *
 * It would seem unnecessary to have a continuation procedure, because at first
 * sight these can only be a single space with no alternate and can't require
 * sampling, because they are device space. However if UseCIEColor is true, then
 * we will actually use a Default Color Space Array in place of the requested color
 * space. These are often CIEBased spaces, and these do need to be sampled. So 
 * actually we do need a continuation procedure, unfortunately.
 *
 * Also, we need to set the initial color value after we have set the color space.
 */
static int
setdevicecolor_cont(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    es_ptr ep = esp, pstage;
    int code = 0, stage, base;

    pstage = ep;
    base = (int)ep[-1].value.intval;
    stage = (int)pstage->value.intval;
    /* If we get a continuation from a sub-procedure, we will want to come back
     * here afterward, to do any remaining stages. We need to set up for that now.
     * so that our continuation is ahead of the sub-proc's continuation.
     */
    check_estack(1);
    /* The push_op_estack macro increments esp before use, so we don't need to */
    push_op_estack(setdevicecolor_cont);

    do {
	switch(stage) {
	    case 0:
		make_int(pstage, ++stage);
		push(1);
		switch(base) {
		    case 0: /* DeviceGray */
			code = name_enter_string(imemory, "DeviceGray", op);
			break;
		    case 1: /* DeviceRGB */
			code = name_enter_string(imemory, "DeviceRGB", op);
			break;
		    case 2: /* DeviceCMYK */
			code = name_enter_string(imemory, "DeviceCMYK", op);
			break;
		}
		if (code < 0)
		    return code;
		code = zsetcolorspace(i_ctx_p);
		if (code != 0)
		    return code;
		break;
	    case 1:
		make_int(pstage, ++stage);
		code = zsetcolor(i_ctx_p);
		if (code != 0)
		    return code;
		break;
	    case 2:
		esp -= 3;
		return o_pop_estack;
		break;
        }
    }while(1);
    return 0;
}

/* These routines implement the device-space set color routines
 * These set both the space and the color in a single operation.
 * Previously these were implemented in PostScript.
 */
static int
zsetgray(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;   /* required by "push" macro */
    float value;
    int code;

    /* Gather numeric operand value(s) */
    code = float_params(op, 1, &value);
    if (code < 0)
	return code;
    /* Clamp numeric operand range(s) */
    if (value < 0)
	value = 0;
    else if (value > 1)
	value = 1;
    code = make_floats(op, &value, 1);
    if (code < 0)
	return code;

    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    push_mark_estack(es_other, 0);
    esp++;
    /* variable to hold base type (0 = gray) */
    make_int(esp, 0);
    esp++;
    /* Store the 'stage' of processing (initially 0) */
    make_int(esp, 0);
    /* Finally, the actual continuation routine */
    push_op_estack(setdevicecolor_cont);
    return o_push_estack;
}
static int
zsethsbcolor(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;   /* required by "push" macro */
    int code, i;
    float values[3];

    /* Gather numeric operand value(s) (also checks type) */
    code = float_params(op, 3, (float *)&values);
    if (code < 0)
	return code;
    /* Clamp numeric operand range(s) */
    for (i = 0;i < 3; i++) {
        if (values[i] < 0)
    	    values[i] = 0;
	else if (values[i] > 1)
	    values[i] = 1;
    }

    hsb2rgb((float *)&values);

    code = make_floats(&op[-2], (const float *)&values, 3);
    if (code < 0)
	return code;

    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    push_mark_estack(es_other, 0);
    esp++;
    /* variable to hold base type (1 = RGB) */
    make_int(esp, 1);
    esp++;
    /* Store the 'stage' of processing (initially 0) */
    make_int(esp, 0);
    /* Finally, the actual continuation routine */
    push_op_estack(setdevicecolor_cont);
    return o_push_estack;
}
static int
zsetrgbcolor(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;   /* required by "push" macro */
    int code, i;
    float values[3];

    /* Gather numeric operand value(s) (also checks type) */
    code = float_params(op, 3, (float *)&values);
    if (code < 0)
	return code;
    /* Clamp numeric operand range(s) */
    for (i = 0;i < 3; i++) {
        if (values[i] < 0)
    	    values[i] = 0;
	else if (values[i] > 1)
	    values[i] = 1;
    }

    code = make_floats(&op[-2], (const float *)&values, 3);
    if (code < 0)
	return code;

    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    push_mark_estack(es_other, 0);
    esp++;
    /* variable to hold base type (1 = RGB) */
    make_int(esp, 1);
    esp++;
    /* Store the 'stage' of processing (initially 0) */
    make_int(esp, 0);
    /* Finally, the actual continuation routine */
    push_op_estack(setdevicecolor_cont);
    return o_push_estack;
}
static int
zsetcmykcolor(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;   /* required by "push" macro */
    int code, i;
    float values[4];

    /* Gather numeric operand value(s) (also checks type) */
    code = float_params(op, 4, (float *)&values);
    if (code < 0)
	return code;
    /* Clamp numeric operand range(s) */
    for (i = 0;i < 4; i++) {
        if (values[i] < 0)
    	    values[i] = 0;
	else if (values[i] > 1)
	    values[i] = 1;
    }

    code = make_floats(&op[-3], (const float *)&values, 4);
    if (code < 0)
	return code;

    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    push_mark_estack(es_other, 0);
    esp++;
    /* variable to hold base type (2 = CMYK) */
    make_int(esp, 2);
    esp++;
    /* Store the 'stage' of processing (initially 0) */
    make_int(esp, 0);
    /* Finally, the actual continuation routine */
    push_op_estack(setdevicecolor_cont);
    return o_push_estack;
}
/*
 * The routine which does all the dispatching for the device-space specific
 * 'current color' routines currentgray, currentrgbcolo and currentcmykcolor.
 * 
 * Starting with the top-level color space we need to take the current color 
 * value(s) and pass it through the tint transform procedure (actually we use the
 * converted function) to get equivalent components for the next space. We then
 * repeat with each alternate space in turn until we reach a 'terminal' space.
 * That can be a device space (eg DeviceGray), a CIEBased or ICCBased space, or
 * a Separation or DeviceN space which is not using its alternate space.
 *
 * Depending on which kind of terminal space we reach we will either return 
 * fixed values (all 0.0) or we will convert the terminal device space components
 * into the requested device space.
 *
 * Because we might need to run a tint transform procedure, this requires a 
 * continuation procedure.
 */
static int
currentbasecolor_cont(i_ctx_t *i_ctx_p)
{
    ref arr, *parr = &arr;
    es_ptr ep = esp;
    int i, code = 0,depth, stage, base, cont=1, stack_depth = 0, CIESubst=0;
    PS_colour_space_t *obj;
    
    stack_depth = (int)ep[-4].value.intval;
    base = (int)ep[-3].value.intval;
    depth = (int)ep[-2].value.intval;
    stage = (int)ep[-1].value.intval;
    /* If we get a continuation from a sub-procedure, we will want to come back
     * here afterward, to do any remaining stages. We need to set up for that now.
     * so that our continuation is ahead of the sub-proc's continuation.
     */
    check_estack(1);
    /* The push_op_estack macro increments esp before use, so we don't need to */
    push_op_estack(currentbasecolor_cont);
   
    while (code == 0 && cont) {
	ref_assign(&arr, ep);
	/* Run along the nested color spaces until we get to the lowest one
	 * that we haven't yet processed (given by 'depth')
	 */
	for (i = 0;i < depth;i++) {
	    code = get_space_object(i_ctx_p, parr, &obj);
	    if (code < 0) 
	        return code;

	    if (i < (depth - 1)) {
		if (!obj->alternateproc) {
		    return_error(e_typecheck);
		}
		code = obj->alternateproc(i_ctx_p, parr, &parr, &CIESubst);
	        if (code < 0) 
		    return code;
	    }
	}

	code = obj->basecolorproc(i_ctx_p, parr, base, &stage, &cont, &stack_depth);
	make_int(&ep[-4], stack_depth);
	make_int(&ep[-1], stage);
	if (code != 0) 
	    return code;
	/* Completed that space, increment the 'depth' */
	make_int(&ep[-2], ++depth);
    }
    if (code == 0) {
        /* Remove our next continuation and our data */
	esp -= 7;
	code = o_pop_estack;
    }
    return code;
}

/* These routines implement the device-space 'current' color routines.
 * Previously these were implemented in PostScript.
 */
static int
zcurrentgray(i_ctx_t * i_ctx_p)
{
    int code, depth;

    code = validate_spaces(i_ctx_p, &istate->colorspace.array, &depth);
    if (code < 0)
	return code;

    code = zcurrentcolor(i_ctx_p);
    if (code < 0)
	return code;
    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(7);
    push_mark_estack(es_other, 0);
    esp++;
    /* variable to hold stack depth for tint transform */
    make_int(&esp[0], 0);
    esp++;
    /* Store the 'base' type color wanted, in this case Gray */
    make_int(&esp[0], 0);
    make_int(&esp[1], 1);
    /* Store the 'stage' of processing (initially 0) */
    make_int(&esp[2], 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    esp[3] = istate->colorspace.array;
    esp += 3; /* The push_op_estack macro increments esp before using it */
    /* Finally, the actual continuation routine */
    push_op_estack(currentbasecolor_cont);
    return o_push_estack;
}
static int
zcurrenthsbcolor(i_ctx_t * i_ctx_p)
{
    int code, depth;

    code = validate_spaces(i_ctx_p, &istate->colorspace.array, &depth);
    if (code < 0)
	return code;

    code = zcurrentcolor(i_ctx_p);
    if (code < 0)
	return code;
    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(7);
    push_mark_estack(es_other, 0);
    esp++;
    /* variable to hold stack depth for tint transform */
    make_int(&esp[0], 0);
    esp++;
    /* Store the 'base' type color wanted, in this case HSB */
    make_int(&esp[0], 1);
    make_int(&esp[1], 1);
    /* Store the 'stage' of processing (initially 0) */
    make_int(&esp[2], 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    esp[3] = istate->colorspace.array;
    esp += 3; /* The push_op_estack macro increments esp before using it */
    /* Finally, the actual continuation routine */
    push_op_estack(currentbasecolor_cont);
    return o_push_estack;
}
static int
zcurrentrgbcolor(i_ctx_t * i_ctx_p)
{
    int code;

    code = zcurrentcolor(i_ctx_p);
    if (code < 0)
	return code;
    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(7);
    push_mark_estack(es_other, 0);
    esp++;
    /* variable to hold stack depth for tint transform */
    make_int(&esp[0], 0);
    esp++;
    /* Store the 'base' type color wanted, in this case RGB */
    make_int(&esp[0], 2);
    make_int(&esp[1], 1);
    /* Store the 'stage' of processing (initially 0) */
    make_int(&esp[2], 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    esp[3] = istate->colorspace.array;
    esp += 3; /* The push_op_estack macro increments esp before using it */
    /* Finally, the actual continuation routine */
    push_op_estack(currentbasecolor_cont);
    return o_push_estack;
}
static int
zcurrentcmykcolor(i_ctx_t * i_ctx_p)
{
    int code;

    code = zcurrentcolor(i_ctx_p);
    if (code < 0)
	return code;
    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(7);
    push_mark_estack(es_other, 0);
    esp++;
    /* variable to hold stack depth for tint transform */
    make_int(&esp[0], 0);
    esp++;
    /* Store the 'base' type color wanted, in this case CMYK */
    make_int(&esp[0], 3);
    make_int(&esp[1], 1);
    /* Store the 'stage' of processing (initially 0) */
    make_int(&esp[2], 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    esp[3] = istate->colorspace.array;
    esp += 3; /* The push_op_estack macro increments esp before using it */
    /* Finally, the actual continuation routine */
    push_op_estack(currentbasecolor_cont);
    return o_push_estack;
}

/* ------ Initialization procedure ------ */

/* We need to split the table because of the 16-element limit. */
const op_def    zcolor_op_defs[] = 
{
    { "0currentcolor", zcurrentcolor },
    { "0currentcolorspace", zcurrentcolorspace },
    { "0.getuseciecolor", zgetuseciecolor },
    { "1setcolor", zsetcolor },
    { "1setcolorspace", zsetcolorspace },

    /* basic transfer operators */
    { "0currenttransfer", zcurrenttransfer },
    { "0processcolors", zprocesscolors },
    { "1settransfer", zsettransfer },

    /* internal operators */
    { "1%zcolor_remap_one_finish", zcolor_remap_one_finish },
    { "1%zcolor_remap_one_signed_finish", zcolor_remap_one_signed_finish },
    { "0%zcolor_reset_transfer", zcolor_reset_transfer },
    { "0%zcolor_remap_color", zcolor_remap_color },
    { "0.color_test", zcolor_test },
    { "1.color_test_all", zcolor_test_all },

    /* high level device support */
    { "0.includecolorspace", zincludecolorspace },
    op_def_end(0)
};

const op_def    zcolor_ext_op_defs[] = 
{
    {"0currentgray", zcurrentgray },
    {"1setgray", zsetgray },
    {"0currenthsbcolor", zcurrenthsbcolor },
    {"3sethsbcolor", zsethsbcolor },
    {"0currentrgbcolor", zcurrentrgbcolor },
    {"3setrgbcolor", zsetrgbcolor },
    {"0currentcmykcolor", zcurrentcmykcolor },
    {"4setcmykcolor", zsetcmykcolor },
    op_def_end(0)
};

