/* Copyright (C) 2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: zalg.c 9043 2008-08-28 22:48:19Z giles $ */
/* Operators for general-purpose algorithms. For now, only sorting. */
#include "ghost.h"
#include "gserrors.h"
#include "oper.h"
#include "store.h"
#include "estack.h"

/* ========================================================================= */

/*
 * The "heap sort" algorithm, as implementation of the .sort operator
 *
 * The implementation follows Algorithm H from Donald Knuth's
 * "The Art of Computer Programming", volume 3, section 5.2.3
 *
 * Notes:
 * i.   Execution time: O(n log n) in the average and worst cases.
 * ii.  The sort is not "stable" (the relative order of elements with
 *	equal keys is not necessarily preserved).
 * iii. Status variables:
 *	- stored on the e-stack;
 *	- "l", "r", "i", "j" and "R" correspond directly to variables in
 *	  Algorithm H (including the fact that indices are 1-based);
 *	- variable "K" from Algorithm H is not used here, because we don't
 *	  distinguish a "key part" of the array elements;
 *	- "H" indicates the step to execute; used when resuming after executing
 *	  <lt> (to execute it, we have to return to the interpreter).
 *	- "array" and "lt" are refs to the parameters; avoids using them from the
 *	  o-stack after resuming, in case the predicate has odd side-efects
 */

static int zsort(i_ctx_t *i_ctx_p);
static int zsort_continue(i_ctx_t *i_ctx_p);
static int zsort_cleanup(i_ctx_t *i_ctx_p);

/* <array> <lt> .sort <array> */
static int
zsort(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    uint N;

    /* Check operands for type and access */
    /* we can sort only writable [and unpacked] arrays */
    if (r_type(&op[-1]) == t_mixedarray || r_type(&op[-1]) == t_shortarray)
	return_error(e_invalidaccess);
    check_write_type(op[-1], t_array);
    /* the predicate must be an executable array/ string/ name/ [pseudo-]operator */
    if (!r_has_attr(&op[0], a_executable))
	return_op_typecheck(&op[0]);
    switch (r_btype(&op[0])) {
	case t_array:
	case t_mixedarray:
	case t_shortarray:
	case t_string:
	    if (!r_has_attr(&op[0], a_execute))
		return_error(e_invalidaccess);
	    break;
	case t_name:
	case t_operator:
	case t_oparray:
	    break;
	default:
	    return_op_typecheck(&op[0]);
    }
    /*
     * if array length <= 1, then nothing to sort
     * else prepare the status variables and launch the main sorting routine zsort_continue()
     */
    N = r_size(&op[-1]);
    if (N <= 1) {
	pop(1);
	return 0;
    } else {
	check_estack(11);
	push_mark_estack(es_other, zsort_cleanup);
/*H1:*/	make_int(&esp[1], N / 2 + 1);	/* l */
	make_int(&esp[2], N);	    	/* r */
	make_int(&esp[3], 0);	    	/* i */
	make_int(&esp[4], 0);	    	/* j */
	make_null(&esp[5]);	    	/* R */
	make_int(&esp[6], 2);	    	/* H */
	ref_assign(&esp[7], &op[0]);	/* lt */
	ref_assign(&esp[8], &op[-1]);	/* the array */
	esp += 8;
	make_op_estack(&esp[1], zsort_continue);
	make_null(&op[0]);		/* result of <lt>, not used when H = 2 */
	return zsort_continue(i_ctx_p);
    }
}

/* Continuation operator for .sort */
static int
zsort_continue(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    ref *status;
    ref *Rn;
#   define l	(status[1].value.intval)
#   define r	(status[2].value.intval)
#   define i	(status[3].value.intval)
#   define j	(status[4].value.intval)
#   define R	(status[5])
#   define H	(status[6].value.intval)
#   define lt	(status[7])
#   define arry	(status[8])

    status = esp - 8;
    Rn = arry.value.refs - 1; /* the -1 compensates for using 1-based indices */
    switch (H) {
	case 2:
H2:	    if (l > 1) {
		l--;
		ref_assign(&R, &Rn[l]);
	    } else {
		ref_assign(&R, &Rn[r]);
		ref_assign_old(&arry, &Rn[r], &Rn[1], ".sort(H2-a)");
		r--;
		if (r <= 1) {
		    ref_assign_old(&arry, &Rn[1], &R, ".sort(H2-b)");
		    esp -= 9;
		    pop(1);
		    return o_pop_estack;
		}
	    }
/* H3: */   j = l;
H4:	    i = j;
	    j <<= 1;
	    if (j >= r)
		if (j == r)
		    goto H6;
		else
		    goto H8;
	    else {
/* H5: */	H = 5;
		push(1);
		ref_assign(&op[-1], &Rn[j]);
		ref_assign(&op[0], &Rn[j + 1]);
		break;
	    }
	case 5:
/*H5_cont:*/if (!r_has_type(&op[0], t_boolean))
		return_error(e_typecheck);
	    if (op[0].value.boolval)
		j++;
H6:	    H = 6;
	    push(1);
	    ref_assign(&op[-1], &R);
	    ref_assign(&op[0], &Rn[j]);
	    break;
	case 6:
/*H6_cont:*/if (!r_has_type(&op[0], t_boolean))
		return_error(e_typecheck);
	    if (op[0].value.boolval) {
/* H7: */  	ref_assign_old(&arry, &Rn[i], &Rn[j], ".sort(H7)");
		goto H4;
	    } else {
H8:		ref_assign_old(&arry, &Rn[i], &R, ".sort(H8)");
		goto H2;
	    }
	default:
	    pop(1);
	    return_error(gs_error_unregistered); /* Must not happen. */
    }
    esp += 2;
    ref_assign(esp, &lt);
    return o_push_estack;
#undef l
#undef r
#undef i
#undef j
#undef R
#undef H
#undef lt
}

/* No-op cleanup routine for .sort */
static int
zsort_cleanup(i_ctx_t *i_ctx_p)
{
    return 0;
}

/* ------ Initialization procedure ------ */

const op_def zalg_op_defs[] =
{
    {"2.sort", zsort},
		/* Internal operators */
    {"1%zsort_continue", zsort_continue},
    op_def_end(0)
};
