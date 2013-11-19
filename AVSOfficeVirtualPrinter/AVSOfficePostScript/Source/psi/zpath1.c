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

/* $Id: zpath1.c 9043 2008-08-28 22:48:19Z giles $ */
/* PostScript Level 1 additional path operators */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "oparc.h"		/* for prototypes */
#include "estack.h"		/* for pathforall */
#include "ialloc.h"
#include "igstate.h"
#include "gsstruct.h"
#include "gspath.h"
#include "store.h"

/* Forward references */
static int common_arc(i_ctx_t *,
	  int (*)(gs_state *, floatp, floatp, floatp, floatp, floatp));
static int common_arct(i_ctx_t *, float *);

/* <x> <y> <r> <ang1> <ang2> arc - */
int
zarc(i_ctx_t *i_ctx_p)
{
    return common_arc(i_ctx_p, gs_arc);
}

/* <x> <y> <r> <ang1> <ang2> arcn - */
int
zarcn(i_ctx_t *i_ctx_p)
{
    return common_arc(i_ctx_p, gs_arcn);
}

/* Common code for arc[n] */
static int
common_arc(i_ctx_t *i_ctx_p,
      int (*aproc)(gs_state *, floatp, floatp, floatp, floatp, floatp))
{
    os_ptr op = osp;
    double xyra[5];		/* x, y, r, ang1, ang2 */
    int code = num_params(op, 5, xyra);

    if (code < 0)
	return code;
    code = (*aproc)(igs, xyra[0], xyra[1], xyra[2], xyra[3], xyra[4]);
    if (code >= 0)
	pop(5);
    return code;
}

/* <x1> <y1> <x2> <y2> <r> arct - */
int
zarct(i_ctx_t *i_ctx_p)
{
    int code = common_arct(i_ctx_p, (float *)0);

    if (code < 0)
	return code;
    pop(5);
    return 0;
}

/* <x1> <y1> <x2> <y2> <r> arcto <xt1> <yt1> <xt2> <yt2> */
static int
zarcto(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    float tanxy[4];		/* xt1, yt1, xt2, yt2 */
    int code = common_arct(i_ctx_p, tanxy);

    if (code < 0)
	return code;
    make_real(op - 4, tanxy[0]);
    make_real(op - 3, tanxy[1]);
    make_real(op - 2, tanxy[2]);
    make_real(op - 1, tanxy[3]);
    pop(1);
    return 0;
}

/* Common code for arct[o] */
static int
common_arct(i_ctx_t *i_ctx_p, float *tanxy)
{
    os_ptr op = osp;
    double args[5];		/* x1, y1, x2, y2, r */
    int code = num_params(op, 5, args);

    if (code < 0)
	return code;
    return gs_arcto(igs, args[0], args[1], args[2], args[3], args[4], tanxy);
}

/* - .dashpath - */
static int
zdashpath(i_ctx_t *i_ctx_p)
{
    return gs_dashpath(igs);
}

/* - flattenpath - */
static int
zflattenpath(i_ctx_t *i_ctx_p)
{
    return gs_flattenpath(igs);
}

/* - reversepath - */
static int
zreversepath(i_ctx_t *i_ctx_p)
{
    return gs_reversepath(igs);
}

/* - strokepath - */
static int
zstrokepath(i_ctx_t *i_ctx_p)
{
    return gs_strokepath(igs);
}

/* - clippath - */
static int
zclippath(i_ctx_t *i_ctx_p)
{
    return gs_clippath(igs);
}

/* <bool> .pathbbox <llx> <lly> <urx> <ury> */
static int
z1pathbbox(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_rect box;
    int code;

    check_type(*op, t_boolean);
    code = gs_upathbbox(igs, &box, op->value.boolval);
    if (code < 0)
	return code;
    push(3);
    make_real(op - 3, box.p.x);
    make_real(op - 2, box.p.y);
    make_real(op - 1, box.q.x);
    make_real(op, box.q.y);
    return 0;
}

/*
 * In order to match Adobe output on a Genoa test, pathbbox must be an
 * operator, not an operator procedure, even though it has a trivial
 * definition as a procedure.
 */
static int
zpathbbox(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code;

    push(1);
    make_false(op);
    code = z1pathbbox(i_ctx_p);
    if (code < 0) {
	pop(1);			/* remove the Boolean */
    }
    return code;
}

/* <moveproc> <lineproc> <curveproc> <closeproc> pathforall - */
static int path_continue(i_ctx_t *);
static int path_cleanup(i_ctx_t *);
static int
zpathforall(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_path_enum *penum;
    int code;

    check_proc(op[-3]);
    check_proc(op[-2]);
    check_proc(op[-1]);
    check_proc(*op);
    check_estack(8);
    if ((penum = gs_path_enum_alloc(imemory, "pathforall")) == 0)
	return_error(e_VMerror);
    code = gs_path_enum_init(penum, igs);
    if (code < 0) {
	ifree_object(penum, "path_cleanup");
	return code;
    }
    /* Push a mark, the four procedures, and the path enumerator. */
    push_mark_estack(es_for, path_cleanup);	/* iterator */
    memcpy(esp + 1, op - 3, 4 * sizeof(ref));	/* 4 procs */
    esp += 5;
    make_istruct(esp, 0, penum);
    push_op_estack(path_continue);
    pop(4);
    op -= 4;
    return o_push_estack;
}
/* Continuation procedure for pathforall */
static void pf_push(i_ctx_t *, gs_point *, int);
static int
path_continue(i_ctx_t *i_ctx_p)
{
    gs_path_enum *penum = r_ptr(esp, gs_path_enum);
    gs_point ppts[3];
    int code;

    /* Make sure we have room on the o-stack for the worst case */
    /* before we enumerate the next path element. */
    check_ostack(6);		/* 3 points for curveto */
    code = gs_path_enum_next(penum, ppts);
    switch (code) {
	case 0:		/* all done */
	    esp -= 6;
	    path_cleanup(i_ctx_p);
	    return o_pop_estack;
	default:		/* error */
	    return code;
	case gs_pe_moveto:
	    esp[2] = esp[-4];	/* moveto proc */
	    pf_push(i_ctx_p, ppts, 1);
	    break;
	case gs_pe_lineto:
	    esp[2] = esp[-3];	/* lineto proc */
	    pf_push(i_ctx_p, ppts, 1);
	    break;
	case gs_pe_curveto:
	    esp[2] = esp[-2];	/* curveto proc */
	    pf_push(i_ctx_p, ppts, 3);
	    break;
	case gs_pe_closepath:
	    esp[2] = esp[-1];	/* closepath proc */
	    break;
    }
    push_op_estack(path_continue);
    ++esp;			/* include pushed procedure */
    return o_push_estack;
}
/* Internal procedure to push one or more points */
static void
pf_push(i_ctx_t *i_ctx_p, gs_point * ppts, int n)
{
    os_ptr op = osp;

    while (n--) {
	op += 2;
	make_real(op - 1, ppts->x);
	make_real(op, ppts->y);
	ppts++;
    }
    osp = op;
}
/* Clean up after a pathforall */
static int
path_cleanup(i_ctx_t *i_ctx_p)
{
    gs_path_enum *penum = r_ptr(esp + 6, gs_path_enum);

    gs_path_enum_cleanup(penum);
    ifree_object(penum, "path_cleanup");
    return 0;
}

/* ------ Initialization procedure ------ */

const op_def zpath1_op_defs[] =
{
    {"5arc", zarc},
    {"5arcn", zarcn},
    {"5arct", zarct},
    {"5arcto", zarcto},
    {"0clippath", zclippath},
    {"0.dashpath", zdashpath},
    {"0flattenpath", zflattenpath},
    {"4pathforall", zpathforall},
    {"0reversepath", zreversepath},
    {"0strokepath", zstrokepath},
    {"1.pathbbox", z1pathbbox},
    {"0pathbbox", zpathbbox},
		/* Internal operators */
    {"0%path_continue", path_continue},
    op_def_end(0)
};
