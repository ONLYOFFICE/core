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

/* $Id: zgstate.c 9043 2008-08-28 22:48:19Z giles $ */
/* Graphics state operators */
#include "math_.h"
#include "ghost.h"
#include "oper.h"
#include "ialloc.h"
#include "icremap.h"
#include "idict.h"
#include "istruct.h"
#include "igstate.h"
#include "gsmatrix.h"
#include "store.h"
#include "gscspace.h"
#include "iname.h"

/* Structure descriptors */
private_st_int_gstate();
private_st_int_remap_color_info();

/* ------ Utilities ------ */

static int
zset_real(i_ctx_t *i_ctx_p, int (*set_proc)(gs_state *, floatp))
{
    os_ptr op = osp;
    double param;
    int code = real_param(op, &param);

    if (code < 0)
	return_op_typecheck(op);
    code = set_proc(igs, param);
    if (!code)
	pop(1);
    return code;
}

static int
zset_bool(i_ctx_t *i_ctx_p, void (*set_proc)(gs_state *, bool))
{
    os_ptr op = osp;

    check_type(*op, t_boolean);
    set_proc(igs, op->value.boolval);
    pop(1);
    return 0;
}

static int
zcurrent_bool(i_ctx_t *i_ctx_p, bool (*current_proc)(const gs_state *))
{
    os_ptr op = osp;

    push(1);
    make_bool(op, current_proc(igs));
    return 0;
}

static int
zset_uint(i_ctx_t *i_ctx_p, void (*set_proc)(gs_state *, uint))
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    set_proc(igs, op->value.intval);
    pop(1);
    return 0;
}

static int
zcurrent_uint(i_ctx_t *i_ctx_p, uint (*current_proc)(const gs_state *))
{
    os_ptr op = osp;

    push(1);
    make_int(op, current_proc(igs));
    return 0;
}

/* ------ Operations on the entire graphics state ------ */

/* "Client" procedures */
static void *gs_istate_alloc(gs_memory_t * mem);
static int gs_istate_copy(void *to, const void *from);
static void gs_istate_free(void *old, gs_memory_t * mem);
static const gs_state_client_procs istate_procs = {
    gs_istate_alloc,
    gs_istate_copy,
    gs_istate_free,
    0,			/* copy_for */
};

/* Initialize the graphics stack. */
gs_state *
int_gstate_alloc(const gs_dual_memory_t * dmem)
{
    int_gstate *iigs;
    ref proc0;
    int_remap_color_info_t *prci;
    gs_ref_memory_t *lmem = dmem->space_local;
    gs_ref_memory_t *gmem = dmem->space_global;
    gs_state *pgs = gs_state_alloc((gs_memory_t *)lmem);

    iigs = gs_alloc_struct((gs_memory_t *)lmem, int_gstate, &st_int_gstate,
			   "int_gstate_alloc(int_gstate)");
    int_gstate_map_refs(iigs, make_null);
    make_empty_array(&iigs->dash_pattern_array, a_all);
    gs_alloc_ref_array(lmem, &proc0, a_readonly + a_executable, 2,
		       "int_gstate_alloc(proc0)");
    make_oper(proc0.value.refs, 0, zpop);
    make_real(proc0.value.refs + 1, 0.0);
    iigs->black_generation = proc0;
    iigs->undercolor_removal = proc0;
    make_false(&iigs->use_cie_color);
    /*
     * Even though the gstate itself is allocated in local VM, the
     * container for the color remapping procedure must be allocated in
     * global VM so that the gstate can be copied into global VM.
     */
    prci = gs_alloc_struct((gs_memory_t *)gmem, int_remap_color_info_t,
			   &st_int_remap_color_info,
			   "int_gstate_alloc(remap color info)");
    make_struct(&iigs->remap_color_info, imemory_space(gmem), prci);
    clear_pagedevice(iigs);
    gs_state_set_client(pgs, iigs, &istate_procs, true);
    /* PostScript code wants limit clamping enabled. */
    gs_setlimitclamp(pgs, true);
    /*
     * gsave and grestore only work properly
     * if there are always at least 2 entries on the stack.
     * We count on the PostScript initialization code to do a gsave.
     */
    return pgs;
}

/* - gsave - */
int
zgsave(i_ctx_t *i_ctx_p)
{
    return gs_gsave(igs);
}

/* - grestore - */
int
zgrestore(i_ctx_t *i_ctx_p)
{
    return gs_grestore(igs);
}

/* - grestoreall - */
int
zgrestoreall(i_ctx_t *i_ctx_p)
{
    return gs_grestoreall(igs);
}

/* - initgraphics - */
static int
zinitgraphics(i_ctx_t *i_ctx_p)
{
    /*
     * gs_initigraphics does not reset the colorspace;
     * this is now handled in the PostScript code.
     */
     make_empty_array(&istate->dash_pattern_array, a_all);
     return gs_initgraphics(igs);
}

/* ------ Operations on graphics state elements ------ */

/* <num> setlinewidth - */
static int
zsetlinewidth(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
	/*
	 * The Red Book doesn't say anything about this, but Adobe
	 * interpreters return (or perhaps store) the absolute value
	 * of the width.
	 */
    double width;
    int code = real_param(op, &width);

    if (code < 0)
	return_op_typecheck(op);
    code = gs_setlinewidth(igs, fabs(width));
    if (code >= 0)
	pop(1);
    return code;
}

/* - currentlinewidth <num> */
static int
zcurrentlinewidth(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_real(op, gs_currentlinewidth(igs));
    return 0;
}

/* <cap_int> .setlinecap - */
static int
zsetlinecap(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int param;
    int code = int_param(op, max_int, &param);

    if (code < 0 || (code = gs_setlinecap(igs, (gs_line_cap) param)) < 0)
	return code;
    pop(1);
    return 0;
}

/* - currentlinecap <cap_int> */
static int
zcurrentlinecap(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_int(op, (int)gs_currentlinecap(igs));
    return 0;
}

/* <join_int> .setlinejoin - */
static int
zsetlinejoin(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int param;
    int code = int_param(op, max_int, &param);

    if (code < 0 || (code = gs_setlinejoin(igs, (gs_line_join) param)) < 0)
	return code;
    pop(1);
    return 0;
}

/* - currentlinejoin <join_int> */
static int
zcurrentlinejoin(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_int(op, (int)gs_currentlinejoin(igs));
    return 0;
}

/* <num> setmiterlimit - */
static int
zsetmiterlimit(i_ctx_t *i_ctx_p)
{
    return zset_real(i_ctx_p, gs_setmiterlimit);
}

/* - currentmiterlimit <num> */
static int
zcurrentmiterlimit(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_real(op, gs_currentmiterlimit(igs));
    return 0;
}

/* <array> <offset> setdash - */
static int
zsetdash(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    os_ptr op1 = op - 1;
    double offset;
    int code = real_param(op, &offset);
    uint i, n;
    gs_memory_t *mem = imemory;
    float *pattern;

    if (code < 0)
	return_op_typecheck(op);
    if (!r_is_array(op1))
	return_op_typecheck(op1);
    /* Adobe interpreters apparently don't check the array for */
    /* read access, so we won't either. */
    /*check_read(*op1); */
    /* Unpack the dash pattern and check it */
    n = r_size(op1);
    pattern =
	(float *)gs_alloc_byte_array(mem, n, sizeof(float), "setdash");

    if (pattern == 0)
	return_error(e_VMerror);
    for (i = 0, code = 0; i < n && code >= 0; ++i) {
	ref element;

	array_get(mem, op1, (long)i, &element);
	code = float_param(&element, &pattern[i]);
    }
    if (code >= 0)
	code = gs_setdash(igs, pattern, n, offset);
    gs_free_object(mem, pattern, "setdash");	/* gs_setdash copies this */
    if (code < 0)
	return code;
    ref_assign(&istate->dash_pattern_array, op1);
    pop(2);
    return code;
}

/* - currentdash <array> <offset> */
static int
zcurrentdash(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(2);
    ref_assign(op - 1, &istate->dash_pattern_array);
    make_real(op, gs_currentdash_offset(igs));
    return 0;
}

/* <num> setflat - */
static int
zsetflat(i_ctx_t *i_ctx_p)
{
    return zset_real(i_ctx_p, gs_setflat);
}

/* - currentflat <num> */
static int
zcurrentflat(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_real(op, gs_currentflat(igs));
    return 0;
}

/* ------ Extensions ------ */

/* <bool> .setaccuratecurves - */
static int
zsetaccuratecurves(i_ctx_t *i_ctx_p)
{
    return zset_bool(i_ctx_p, gs_setaccuratecurves);
}

/* - .currentaccuratecurves <bool> */
static int
zcurrentaccuratecurves(i_ctx_t *i_ctx_p)
{
    return zcurrent_bool(i_ctx_p, gs_currentaccuratecurves);
}

/* <join_int|-1> .setcurvejoin - */
static int
zsetcurvejoin(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code;

    check_type(*op, t_integer);
    if (op->value.intval < -1 || op->value.intval > max_int)
	return_error(e_rangecheck);
    code = gs_setcurvejoin(igs, (int)op->value.intval);
    if (code < 0)
	return code;
    pop(1);
    return 0;
}

/* - .currentcurvejoin <join_int|-1> */
static int
zcurrentcurvejoin(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_int(op, gs_currentcurvejoin(igs));
    return 0;
}

/* <adjust.x> <adjust.y> .setfilladjust2 - */
static int
zsetfilladjust2(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    double adjust[2];
    int code = num_params(op, 2, adjust);

    if (code < 0)
	return code;
    code = gs_setfilladjust(igs, adjust[0], adjust[1]);
    if (code < 0)
	return code;
    pop(2);
    return 0;
}

/* - .currentfilladjust2 <adjust.x> <adjust.y> */
static int
zcurrentfilladjust2(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_point adjust;

    push(2);
    gs_currentfilladjust(igs, &adjust);
    make_real(op - 1, adjust.x);
    make_real(op, adjust.y);
    return 0;
}

/* <bool> .setdashadapt - */
static int
zsetdashadapt(i_ctx_t *i_ctx_p)
{
    return zset_bool(i_ctx_p, gs_setdashadapt);
}

/* - .currentdashadapt <bool> */
static int
zcurrentdashadapt(i_ctx_t *i_ctx_p)
{
    return zcurrent_bool(i_ctx_p, gs_currentdashadapt);
}

/* <num> <bool> .setdotlength - */
static int
zsetdotlength(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    double length;
    int code = real_param(op - 1, &length);

    if (code < 0)
	return code;
    check_type(*op, t_boolean);
    code = gs_setdotlength(igs, length, op->value.boolval);
    if (code < 0)
	return code;
    pop(2);
    return 0;
}

/* - .currentdotlength <num> <bool> */
static int
zcurrentdotlength(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(2);
    make_real(op - 1, gs_currentdotlength(igs));
    make_bool(op, gs_currentdotlength_absolute(igs));
    return 0;
}

/* - .setdotorientation - */
static int
zsetdotorientation(i_ctx_t *i_ctx_p)
{
    return gs_setdotorientation(igs);
}

/* - .dotorientation - */
static int
zdotorientation(i_ctx_t *i_ctx_p)
{
    return gs_dotorientation(igs);
}

/* <bool> .setlimitclamp - */
static int
zsetlimitclamp(i_ctx_t *i_ctx_p)
{
    return zset_bool(i_ctx_p, gs_setlimitclamp);
}

/* - .currentlimitclamp <bool> */
static int
zcurrentlimitclamp(i_ctx_t *i_ctx_p)
{
    return zcurrent_bool(i_ctx_p, gs_currentlimitclamp);
}

/* <int> .settextrenderingmode - */
static int
zsettextrenderingmode(i_ctx_t *i_ctx_p)
{
    return zset_uint(i_ctx_p, gs_settextrenderingmode);
}

/* - .currenttextrenderingmode <int> */
static int
zcurrenttextrenderingmode(i_ctx_t *i_ctx_p)
{
    return zcurrent_uint(i_ctx_p, gs_currenttextrenderingmode);
}

/* ------ Initialization procedure ------ */

/* We need to split the table because of the 16-element limit. */
const op_def zgstate1_op_defs[] = {
    {"0.currentaccuratecurves", zcurrentaccuratecurves},
    {"0.currentcurvejoin", zcurrentcurvejoin},
    {"0currentdash", zcurrentdash},
    {"0.currentdashadapt", zcurrentdashadapt},
    {"0.currentdotlength", zcurrentdotlength},
    {"0.currentfilladjust2", zcurrentfilladjust2},
    {"0currentflat", zcurrentflat},
    {"0.currentlimitclamp", zcurrentlimitclamp},
    {"0currentlinecap", zcurrentlinecap},
    {"0currentlinejoin", zcurrentlinejoin},
    {"0currentlinewidth", zcurrentlinewidth},
    {"0currentmiterlimit", zcurrentmiterlimit},
    {"0.dotorientation", zdotorientation},
    {"0grestore", zgrestore},
    {"0grestoreall", zgrestoreall},
    op_def_end(0)
};
const op_def zgstate2_op_defs[] = {
    {"0gsave", zgsave},
    {"0initgraphics", zinitgraphics},
    {"1.setaccuratecurves", zsetaccuratecurves},
    {"1.setcurvejoin", zsetcurvejoin},
    {"2setdash", zsetdash},
    {"1.setdashadapt", zsetdashadapt},
    {"2.setdotlength", zsetdotlength},
    {"0.setdotorientation", zsetdotorientation},
    {"2.setfilladjust2", zsetfilladjust2},
    {"1.setlimitclamp", zsetlimitclamp},
    {"1setflat", zsetflat},
    {"1.setlinecap", zsetlinecap},
    {"1.setlinejoin", zsetlinejoin},
    {"1setlinewidth", zsetlinewidth},
    {"1setmiterlimit", zsetmiterlimit},
    op_def_end(0)
};
const op_def zgstate3_op_defs[] = {
    {"0.settextrenderingmode", zsettextrenderingmode},
    {"0.currenttextrenderingmode", zcurrenttextrenderingmode},
    op_def_end(0)
};

/* ------ Internal routines ------ */

/* Allocate the interpreter's part of a graphics state. */
static void *
gs_istate_alloc(gs_memory_t * mem)
{
    return gs_alloc_struct(mem, int_gstate, &st_int_gstate, "int_gsave");
}

/* Copy the interpreter's part of a graphics state. */
static int
gs_istate_copy(void *to, const void *from)
{
    *(int_gstate *) to = *(const int_gstate *)from;
    return 0;
}

/* Free the interpreter's part of a graphics state. */
static void
gs_istate_free(void *old, gs_memory_t * mem)
{
    gs_free_object(mem, old, "int_grestore");
}
