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

/* $Id: zupath.c 9043 2008-08-28 22:48:19Z giles $ */
/* Operators related to user paths */
#include "ghost.h"
#include "oper.h"
#include "oparc.h"
#include "idict.h"
#include "dstack.h"
#include "igstate.h"
#include "iname.h"
#include "iutil.h"
#include "store.h"
#include "stream.h"
#include "ibnum.h"
#include "gsmatrix.h"
#include "gsstate.h"
#include "gscoord.h"
#include "gspaint.h"
#include "gxfixed.h"
#include "gxdevice.h"
#include "gspath.h"
#include "gzpath.h"		/* for saving path */
#include "gzstate.h"		/* for accessing path */

/* Imported data */
extern const gx_device gs_hit_device;
extern const int gs_hit_detected;

/*
 * CPSI mode affects two algorithms in this file:
 * - CPSI allows ucache to appear anywhere in user paths, even though the
 *   PLRM says ucache must appear (if at all) at the beginning
 *   (PLRM3 p, 199);
 * - After appending an empty user path, in CPSI the current point is
 *   defined, even though the PLRM strongly implies this is incorrect
 *   (PLRM3 p. 712).
 * The 'upath_compat' Boolean controls this behavior.
 */

extern bool CPSI_mode;

/* Forward references */
static int upath_append(os_ptr, i_ctx_t *, bool);
static int upath_stroke(i_ctx_t *, gs_matrix *, bool);

/* ---------------- Insideness testing ---------------- */

/* Forward references */
static int in_test(i_ctx_t *, int (*)(gs_state *));
static int in_path(os_ptr, i_ctx_t *, gx_device *);
static int in_path_result(i_ctx_t *, int, int);
static int in_utest(i_ctx_t *, int (*)(gs_state *));
static int in_upath(i_ctx_t *, gx_device *);
static int in_upath_result(i_ctx_t *, int, int);

/* <x> <y> ineofill <bool> */
/* <userpath> ineofill <bool> */
static int
zineofill(i_ctx_t *i_ctx_p)
{
    return in_test(i_ctx_p, gs_eofill);
}

/* <x> <y> infill <bool> */
/* <userpath> infill <bool> */
static int
zinfill(i_ctx_t *i_ctx_p)
{
    return in_test(i_ctx_p, gs_fill);
}

/* <x> <y> instroke <bool> */
/* <userpath> instroke <bool> */
static int
zinstroke(i_ctx_t *i_ctx_p)
{
    return in_test(i_ctx_p, gs_stroke);
}

/* <x> <y> <userpath> inueofill <bool> */
/* <userpath1> <userpath2> inueofill <bool> */
static int
zinueofill(i_ctx_t *i_ctx_p)
{
    return in_utest(i_ctx_p, gs_eofill);
}

/* <x> <y> <userpath> inufill <bool> */
/* <userpath1> <userpath2> inufill <bool> */
static int
zinufill(i_ctx_t *i_ctx_p)
{
    return in_utest(i_ctx_p, gs_fill);
}

/* <x> <y> <userpath> inustroke <bool> */
/* <x> <y> <userpath> <matrix> inustroke <bool> */
/* <userpath1> <userpath2> inustroke <bool> */
/* <userpath1> <userpath2> <matrix> inustroke <bool> */
static int
zinustroke(i_ctx_t *i_ctx_p)
{	/* This is different because of the optional matrix operand. */
    os_ptr op = osp;
    int code = gs_gsave(igs);
    int spop, npop;
    gs_matrix mat;
    gx_device hdev;

    if (code < 0)
	return code;
    if ((spop = upath_stroke(i_ctx_p, &mat, false)) < 0) {
	gs_grestore(igs);
	return spop;
    }
    if ((npop = in_path(op - spop, i_ctx_p, &hdev)) < 0) {
	gs_grestore(igs);
	return npop;
    }
    if (npop > 1)		/* matrix was supplied */
	code = gs_concat(igs, &mat);
    if (code >= 0)
	code = gs_stroke(igs);
    return in_upath_result(i_ctx_p, npop + spop, code);
}

/* ------ Internal routines ------ */

/* Do the work of the non-user-path insideness operators. */
static int
in_test(i_ctx_t *i_ctx_p, int (*paintproc)(gs_state *))
{
    os_ptr op = osp;
    gx_device hdev;
    int npop = in_path(op, i_ctx_p, &hdev);
    int code;

    if (npop < 0)
	return npop;
    code = (*paintproc)(igs);
    return in_path_result(i_ctx_p, npop, code);
}

/* Set up a clipping path and device for insideness testing. */
static int
in_path(os_ptr oppath, i_ctx_t *i_ctx_p, gx_device * phdev)
{
    int code = gs_gsave(igs);
    int npop;
    double uxy[2];

    if (code < 0)
	return code;
    code = num_params(oppath, 2, uxy);
    if (code >= 0) {		/* Aperture is a single pixel. */
	gs_point dxy;
	gs_fixed_rect fr;

	gs_transform(igs, uxy[0], uxy[1], &dxy);
	fr.p.x = fixed_floor(float2fixed(dxy.x));
	fr.p.y = fixed_floor(float2fixed(dxy.y));
	fr.q.x = fr.p.x + fixed_1;
	fr.q.y = fr.p.y + fixed_1;
	code = gx_clip_to_rectangle(igs, &fr);
	npop = 2;
    } else if (code == e_stackunderflow) {
	/* If 0 elements, definitely a stackunderflow; otherwise, */
	/* only 1 number, also a stackunderflow. */
	npop = code;
    } else {			/* Aperture is a user path. */
	/* We have to set the clipping path without disturbing */
	/* the current path. */
	gx_path *ipath = igs->path;
	gx_path save;

	gx_path_init_local(&save, imemory);
	gx_path_assign_preserve(&save, ipath);
	gs_newpath(igs);
	code = upath_append(oppath, i_ctx_p, false);
	if (code >= 0)
	    code = gx_clip_to_path(igs);
	gx_path_assign_free(igs->path, &save);
	npop = 1;
    }
    if (code < 0) {
	gs_grestore(igs);
	return code;
    }
    /* Install the hit detection device. */
    gx_set_device_color_1(igs);
    gx_device_init((gx_device *) phdev, (const gx_device *)&gs_hit_device,
		   NULL, true);
    phdev->width = phdev->height = max_int;
    gx_device_fill_in_procs(phdev);
    gx_set_device_only(igs, phdev);
    return npop;
}

/* Finish an insideness test. */
static int
in_path_result(i_ctx_t *i_ctx_p, int npop, int code)
{
    os_ptr op = osp;
    bool result;

    gs_grestore(igs);		/* matches gsave in in_path */
    if (code == gs_hit_detected)
	result = true;
    else if (code == 0)		/* completed painting without a hit */
	result = false;
    else			/* error */
	return code;
    npop--;
    pop(npop);
    op -= npop;
    make_bool(op, result);
    return 0;

}

/* Do the work of the user-path insideness operators. */
static int
in_utest(i_ctx_t *i_ctx_p, int (*paintproc)(gs_state *))
{
    gx_device hdev;
    int npop = in_upath(i_ctx_p, &hdev);
    int code;

    if (npop < 0)
	return npop;
    code = (*paintproc)(igs);
    return in_upath_result(i_ctx_p, npop, code);
}

/* Set up a clipping path and device for insideness testing */
/* with a user path. */
static int
in_upath(i_ctx_t *i_ctx_p, gx_device * phdev)
{
    os_ptr op = osp;
    int code = gs_gsave(igs);
    int npop;

    if (code < 0)
	return code;
    if ((code = upath_append(op, i_ctx_p, false)) < 0 ||
	(code = npop = in_path(op - 1, i_ctx_p, phdev)) < 0
	) {
	gs_grestore(igs);
	return code;
    }
    return npop + 1;
}

/* Finish an insideness test with a user path. */
static int
in_upath_result(i_ctx_t *i_ctx_p, int npop, int code)
{
    gs_grestore(igs);		/* matches gsave in in_upath */
    return in_path_result(i_ctx_p, npop, code);
}

/* ---------------- User paths ---------------- */

/* User path operator codes */
typedef enum {
    upath_op_setbbox = 0,
    upath_op_moveto = 1,
    upath_op_rmoveto = 2,
    upath_op_lineto = 3,
    upath_op_rlineto = 4,
    upath_op_curveto = 5,
    upath_op_rcurveto = 6,
    upath_op_arc = 7,
    upath_op_arcn = 8,
    upath_op_arct = 9,
    upath_op_closepath = 10,
    upath_op_ucache = 11
} upath_op;

/* User path interpretation states */
typedef enum {
    UPS_INITIAL = 1,		/* (no ops yet) */
    UPS_UCACHE = 2,		/* ucache */
    UPS_SETBBOX = 4,		/* [ucache] setbbox */
    UPS_PATH = 8		/* (within path) */
} upath_state;

typedef struct up_data_s {
    byte num_args;
    byte states_before;
    byte state_after;
} up_data_t;
#define UP_DATA_PATH(n) {n, UPS_SETBBOX | UPS_PATH, UPS_PATH}

#define UPATH_MAX_OP 11
#define UPATH_REPEAT 32
static const up_data_t up_data[UPATH_MAX_OP + 1] = {
    {4, UPS_INITIAL | UPS_UCACHE, UPS_SETBBOX}, /* setbbox */
    UP_DATA_PATH(2),
    UP_DATA_PATH(2),
    UP_DATA_PATH(2),
    UP_DATA_PATH(2),
    UP_DATA_PATH(6),
    UP_DATA_PATH(6),
    UP_DATA_PATH(5),
    UP_DATA_PATH(5),
    UP_DATA_PATH(5),
    UP_DATA_PATH(0),
    {0, UPS_INITIAL, UPS_UCACHE}	/* ucache */
};

/* Declare operator procedures not declared in opextern.h. */
int zsetbbox(i_ctx_t *);
static int zucache(i_ctx_t *);

#undef zp
static const op_proc_t up_ops[UPATH_MAX_OP + 1] = {
    zsetbbox, zmoveto, zrmoveto, zlineto, zrlineto,
    zcurveto, zrcurveto, zarc, zarcn, zarct,
    zclosepath, zucache
};

/* - ucache - */
static int
zucache(i_ctx_t *i_ctx_p)
{
    /* A no-op for now. */
    return 0;
}

/* <userpath> uappend - */
static int
zuappend(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code = gs_gsave(igs);

    if (code < 0)
	return code;
    if ((code = upath_append(op, i_ctx_p, false)) >= 0)
	code = gs_upmergepath(igs);
    gs_grestore(igs);
    if (code < 0)
	return code;
    pop(1);
    return 0;
}

/* <userpath> ueofill - */
static int
zueofill(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code = gs_gsave(igs);

    if (code < 0)
	return code;
    if ((code = upath_append(op, i_ctx_p, CPSI_mode)) >= 0)
	code = gs_eofill(igs);
    gs_grestore(igs);
    if (code < 0)
	return code;
    pop(1);
    return 0;
}

/* <userpath> ufill - */
static int
zufill(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code = gs_gsave(igs);

    if (code < 0)
	return code;
    if ((code = upath_append(op, i_ctx_p, CPSI_mode)) >= 0)
	code = gs_fill(igs);
    gs_grestore(igs);
    if (code < 0)
	return code;
    pop(1);
    return 0;
}

/* <userpath> ustroke - */
/* <userpath> <matrix> ustroke - */
static int
zustroke(i_ctx_t *i_ctx_p)
{
    int code = gs_gsave(igs);
    int npop;

    if (code < 0)
	return code;
    if ((code = npop = upath_stroke(i_ctx_p, NULL, CPSI_mode)) >= 0)
	code = gs_stroke(igs);
    gs_grestore(igs);
    if (code < 0)
	return code;
    pop(npop);
    return 0;
}

/* <userpath> ustrokepath - */
/* <userpath> <matrix> ustrokepath - */
static int
zustrokepath(i_ctx_t *i_ctx_p)
{
    gx_path save;
    gs_matrix saved_matrix;
    int npop, code = gs_currentmatrix(igs, &saved_matrix);

    if (code < 0)
	return code;
    /* Save and reset the path. */
    gx_path_init_local(&save, imemory);
    gx_path_assign_preserve(&save, igs->path);
    if ((code = npop = upath_stroke(i_ctx_p, NULL, false)) < 0 ||
	(code = gs_strokepath(igs)) < 0
	) {
	gx_path_assign_free(igs->path, &save);
	return code;
    }
    /*
     * If a matrix was specified then restore the previous matrix.
     */
    if (npop > 1) {
        if ((code = gs_setmatrix(igs, &saved_matrix)) < 0) {
	    gx_path_assign_free(igs->path, &save);
	    return code;
        }
    }
    gx_path_free(&save, "ustrokepath");
    pop(npop);
    return 0;
}

/* <with_ucache> upath <userpath> */
/* We do all the work in a procedure that is also used to construct */
/* the UnpaintedPath user path for ImageType 2 images. */
int make_upath(i_ctx_t *i_ctx_p, ref *rupath, gs_state *pgs, gx_path *ppath,
	       bool with_ucache);
static int
zupath(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_boolean);
    return make_upath(i_ctx_p, op, igs, igs->path, op->value.boolval);
}

/* Compute the path length for user path purposes. */
static int 
path_length_for_upath(const gx_path *ppath)
{
    gs_path_enum penum;
    int op, size = 0;
    gs_fixed_point pts[3];

    gx_path_enum_init(&penum, ppath);
    while ((op = gx_path_enum_next(&penum, pts)) != 0) {
	switch (op) {
	    case gs_pe_moveto:
	    case gs_pe_lineto:
		size += 3;
		continue;
	    case gs_pe_curveto:
		size += 7;
		continue;
	    case gs_pe_closepath:
		size += 1;
		continue;
	    default:
		return_error(e_unregistered);
	}
    }
    return size;
}

int
make_upath(i_ctx_t *i_ctx_p, ref *rupath, gs_state *pgs, gx_path *ppath,
	   bool with_ucache)
{
    int size = (with_ucache ? 6 : 5);
    gs_path_enum penum;
    gs_rect bbox;
    int op;
    ref *next;
    int code;


    /* Compute the bounding box. */
    if ((code = gs_upathbbox(pgs, &bbox, true)) < 0) {
	/*
	 * Note: Adobe throws 'nocurrentpoint' error, but the PLRM does
	 * not list this as a possible error from 'upath', so if we are
	 * not in CPSI compatibility mode, we set a reasonable default
	 * bbox instead.
	 */
	if (code != e_nocurrentpoint || CPSI_mode)
	    return code;
	bbox.p.x = bbox.p.y = bbox.q.x = bbox.q.y = 0;
    }

    code = path_length_for_upath(ppath);
    if (code < 0)
        return code;
    size += code;
    if (size >= 65536)
        return_error(e_limitcheck);

    code = ialloc_ref_array(rupath, a_all | a_executable, size,
			    "make_upath");
    if (code < 0)
	return code;
    /* Construct the path. */
    next = rupath->value.refs;
    if (with_ucache) {
        if ((code = name_enter_string(pgs->memory, "ucache", next)) < 0)
	    return code;
	r_set_attrs(next, a_executable | l_new);
	++next;
    }
    make_real_new(next, bbox.p.x);
    make_real_new(next + 1, bbox.p.y);
    make_real_new(next + 2, bbox.q.x);
    make_real_new(next + 3, bbox.q.y);
    next += 4;
    if ((code = name_enter_string(pgs->memory, "setbbox", next)) < 0)
	return code;
    r_set_attrs(next, a_executable | l_new);
    ++next;
    {
	gs_point pts[3];

	/* Patch the path in the gstate to set up the enumerator. */
	gx_path *save_path = pgs->path;

	pgs->path = ppath;
	gs_path_enum_copy_init(&penum, pgs, false);
	pgs->path = save_path;
	while ((op = gs_path_enum_next(&penum, pts)) != 0) {
	    const char *opstr;

	    switch (op) {
		case gs_pe_moveto:
		    opstr = "moveto";
		    goto ml;
		case gs_pe_lineto:
		    opstr = "lineto";
		  ml:make_real_new(next, pts[0].x);
		    make_real_new(next + 1, pts[0].y);
		    next += 2;
		    break;
		case gs_pe_curveto:
		    opstr = "curveto";
		    make_real_new(next, pts[0].x);
		    make_real_new(next + 1, pts[0].y);
		    make_real_new(next + 2, pts[1].x);
		    make_real_new(next + 3, pts[1].y);
		    make_real_new(next + 4, pts[2].x);
		    make_real_new(next + 5, pts[2].y);
		    next += 6;
		    break;
		case gs_pe_closepath:
		    opstr = "closepath";
		    break;
		default:
		    return_error(e_unregistered);
	    }
	    if ((code = name_enter_string(pgs->memory, opstr, next)) < 0)
		return code;
	    r_set_attrs(next, a_executable);
	    ++next;
	}
    }
    return 0;
}


static int
zgetpath(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int i, code, path_size, leaf_count;
    ref *main_ref, *operators[5];

    push(1);
    path_size = code = path_length_for_upath(igs->path);
    if (code < 0)
        return code;
    leaf_count = (path_size + max_array_size - 1) / max_array_size;
    code = ialloc_ref_array(op, a_all, leaf_count, "zgetpath_master");
    if (code < 0)
        return code;
    if (path_size == 0)
        return 0;

    if (dict_find_string(systemdict, "moveto", &operators[1]) <= 0 ||
  	dict_find_string(systemdict, "lineto", &operators[2]) <= 0 ||
	dict_find_string(systemdict, "curveto", &operators[3]) <= 0 ||
	dict_find_string(systemdict, "closepath", &operators[4]) <= 0)
          return_error(e_undefined);

    main_ref = op->value.refs;
    for (i = 0; i < leaf_count; i++) {
       int leaf_size = ( i == leaf_count - 1) ? path_size - i * max_array_size : max_array_size;
       code = ialloc_ref_array(&main_ref[i], a_all | a_executable, leaf_size, "zgetpath_leaf");
       if (code < 0)
            return code;
    }

    {
        int pe, j, k;
        gs_path_enum penum;
        static const int oper_count[5] = { 0, 2, 2, 6, 0 };
	gs_point pts[3];
        const double  *fts[6]; 

        fts[0] = &pts[0].x;
        fts[1] = &pts[0].y;
        fts[2] = &pts[1].x;
        fts[3] = &pts[1].y;
        fts[4] = &pts[2].x;
        fts[5] = &pts[2].y;

        main_ref = op->value.refs;
	gs_path_enum_copy_init(&penum, igs, false);
        pe = gs_path_enum_next(&penum, pts);
        k = 0;

        for (i = 0; i < leaf_count; i++) {
            int leaf_size = ( i == leaf_count - 1) ? path_size - i * max_array_size : max_array_size;
            ref *leaf_ref = main_ref[i].value.refs;
            
            for (j = 0; j < leaf_size; j++) {
                if (k < oper_count[pe])
                   make_real_new(&leaf_ref[j], (float)*fts[k++]);
                else {
                    k = 0;
                    ref_assign(&leaf_ref[j], operators[pe]);
                    pe = gs_path_enum_next(&penum, pts);
                    if (pe == 0)
                        goto out;
                    if (pe >= 5)
                        return_error(e_unregistered);
                }
            }
        }
     }
  out:
  return 0;
}

/* ------ Internal routines ------ */

/* Append a user path to the current path. */
static inline int
upath_append_aux(os_ptr oppath, i_ctx_t *i_ctx_p, int *pnargs, bool upath_compat)
{
    upath_state ups = UPS_INITIAL;
    ref opcodes;

    if (r_has_type(oppath, t__invalid))
	return_error(e_stackunderflow);
    if (!r_is_array(oppath))
	return_error(e_typecheck);
    check_read(*oppath);
    gs_newpath(igs);
    /****** ROUND tx AND ty ******/
    
    if ( r_size(oppath) == 2 &&
	 array_get(imemory, oppath, 1, &opcodes) >= 0 &&
         r_has_type(&opcodes, t_string)
	) {			/* 1st element is operands, 2nd is operators */
	ref operands;
	int code, format;
	int repcount = 1;
	const byte *opp;
	uint ocount, i = 0;

        array_get(imemory, oppath, 0, &operands);
        code = num_array_format(&operands);
	if (code < 0)
	    return code;
	format = code;
	check_read(opcodes);
	opp = opcodes.value.bytes;
	ocount = r_size(&opcodes);
	while (ocount--) {
	    byte opx = *opp++;

	    if (opx > UPATH_REPEAT)
		repcount = opx - UPATH_REPEAT;
	    else if (opx > UPATH_MAX_OP)
		return_error(e_rangecheck);
	    else {		/* operator */
		const up_data_t data = up_data[opx];

		*pnargs = 0;	/* in case of error */
		if (upath_compat && opx == upath_op_ucache) {
		    /* CPSI does not complain about incorrect ucache
		       placement, even though PLRM3 says it's illegal. */
		    ups = ups > UPS_UCACHE ? ups : data.state_after;
		} else {
		    if (!(ups & data.states_before))
			return_error(e_typecheck);
		    ups = data.state_after;
		}
		do {
		    os_ptr op = osp;
		    byte opargs = data.num_args;

		    while (opargs--) {
			push(1);
			(*pnargs)++; /* in case of error */
			code = num_array_get(imemory, &operands, format, i++, op);
			switch (code) {
			    case t_integer:
				r_set_type_attrs(op, t_integer, 0);
				break;
			    case t_real:
				r_set_type_attrs(op, t_real, 0);
				break;
			    default:
				return_error(e_typecheck);
			}
		    }
		    code = (*up_ops[opx])(i_ctx_p);
		    if (code < 0)
			return code;
		}
		while (--repcount);
		repcount = 1;
	    }
	}
    } else {	/* Ordinary executable array. */
	const ref *arp = oppath;
	uint ocount = r_size(oppath);
	long index = 0;
	int argcount = 0;
	op_proc_t oproc;
	int opx, code;

	for (; index < ocount; index++) {
	    ref rup;
	    ref *defp;
	    os_ptr op = osp;
	    up_data_t data;

	    *pnargs = argcount;
	    array_get(imemory, arp, index, &rup);
	    switch (r_type(&rup)) {
		case t_integer:
		case t_real:
		    argcount++;
		    push(1);
		    *op = rup;
		    break;
		case t_name:
		    if (!r_has_attr(&rup, a_executable) ||
			dict_find(systemdict, &rup, &defp) <= 0 ||
			r_btype(defp) != t_operator)
			return_error(e_typecheck); /* all errors = typecheck */
		    goto xop;
		case t_operator:
		    defp = &rup;
		  xop:if (!r_has_attr(defp, a_executable))
			return_error(e_typecheck);
		    oproc = real_opproc(defp);
		    for (opx = 0; opx <= UPATH_MAX_OP; opx++)
			if (oproc == up_ops[opx])
			    break;
		    if (opx > UPATH_MAX_OP)
			return_error(e_typecheck);
		    data = up_data[opx];
		    if (argcount != data.num_args)
			return_error(e_typecheck);
		    if (upath_compat && opx == upath_op_ucache) {
			/* CPSI does not complain about incorrect ucache
			   placement, even though PLRM3 says it's illegal. */
			ups = ups > UPS_UCACHE ? ups : data.state_after;
		    } else {
			if (!(ups & data.states_before))
			    return_error(e_typecheck);
			ups = data.state_after;
		    }
		    code = (*up_ops[opx])(i_ctx_p);
		    if (code < 0) {
			if (code == e_nocurrentpoint)
                            return_error(e_rangecheck); /* CET 11-22 */
                        return code;
		    }
                    argcount = 0;
		    break;
		default:
		    return_error(e_typecheck);
	    }
	}
	if (argcount) {
	    *pnargs = argcount;
	    return_error(e_typecheck);	/* leftover args */
	}
    }
    if (ups < UPS_SETBBOX)
	return_error(e_typecheck);	/* no setbbox */
    if (ups == UPS_SETBBOX && upath_compat) {
	/*
	 * In CPSI compatibility mode, an empty path with a setbbox also
	 * does a moveto (but only if the path is empty).  Since setbbox
	 * was the last operator, its operands are still on the o-stack.
	 */
	osp += 2;
	return zmoveto(i_ctx_p);
    }
    return 0;
}
static int
upath_append(os_ptr oppath, i_ctx_t *i_ctx_p, bool upath_compat)
{
    int nargs = 0;
    int code = upath_append_aux(oppath, i_ctx_p, &nargs, upath_compat);

    if (code < 0) {
	/* Pop args on error, to match Adobe interpreters. */
	pop(nargs);
	return code;
    }
    return 0;
}

/* Append a user path to the current path, and then apply or return */
/* a transformation if one is supplied. */
static int
upath_stroke(i_ctx_t *i_ctx_p, gs_matrix *pmat, bool upath_compat)
{
    os_ptr op = osp;
    int code, npop;
    gs_matrix mat;

    if ((code = read_matrix(imemory, op, &mat)) >= 0) {
	if ((code = upath_append(op - 1, i_ctx_p, upath_compat)) >= 0) {
	    if (pmat)
		*pmat = mat;
	    else
		code = gs_concat(igs, &mat);
	}
	npop = 2;
    } else {
	if ((code = upath_append(op, i_ctx_p, upath_compat)) >= 0)
	    if (pmat)
		gs_make_identity(pmat);
	npop = 1;
    }
    return (code < 0 ? code : npop);
}

/* ---------------- Initialization procedure ---------------- */

const op_def zupath_l2_op_defs[] =
{
    op_def_begin_level2(),
		/* Insideness testing */
    {"1ineofill", zineofill},
    {"1infill", zinfill},
    {"1instroke", zinstroke},
    {"2inueofill", zinueofill},
    {"2inufill", zinufill},
    {"2inustroke", zinustroke},
		/* User paths */
    {"1uappend", zuappend},
    {"0ucache", zucache},
    {"1ueofill", zueofill},
    {"1ufill", zufill},
    {"1upath", zupath},
    {"1ustroke", zustroke},
    {"1ustrokepath", zustrokepath},
		/* Path access for PDF */
    {"0.getpath", zgetpath},
    op_def_end(0)
};
