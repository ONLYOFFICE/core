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

/* $Id: zarray.c 9043 2008-08-28 22:48:19Z giles $ */
/* Array operators */
#include "memory_.h"
#include "ghost.h"
#include "ialloc.h"
#include "ipacked.h"
#include "oper.h"
#include "store.h"

/* The generic operators (copy, get, put, getinterval, putinterval, */
/* length, and forall) are implemented in zgeneric.c. */

/* <int> array <array> */
int
zarray(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    uint size;
    int code;

    check_type(*op, t_integer);
    if (op->value.intval < 0)
	return_error(e_rangecheck);
    if (op->value.intval > max_array_size)
	return_error(e_limitcheck);
    size = op->value.intval;
    code = ialloc_ref_array((ref *)op, a_all, size, "array");
    if (code < 0)
	return code;
    refset_null(op->value.refs, size);
    return 0;
}

/* <array> aload <obj_0> ... <obj_n-1> <array> */
static int
zaload(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    ref aref;
    uint asize;

    ref_assign(&aref, op);
    if (!r_is_array(&aref))
	return_op_typecheck(op);
    check_read(aref);
    asize = r_size(&aref);
    if (asize > ostop - op) {	/* Use the slow, general algorithm. */
	int code = ref_stack_push(&o_stack, asize);
	uint i;
	const ref_packed *packed = aref.value.packed;

	if (code < 0)
	    return code;
	for (i = asize; i > 0; i--, packed = packed_next(packed))
	    packed_get(imemory, packed, ref_stack_index(&o_stack, i));
	*osp = aref;
	return 0;
    }
    if (r_has_type(&aref, t_array))
	memcpy(op, aref.value.refs, asize * sizeof(ref));
    else {
	uint i;
	const ref_packed *packed = aref.value.packed;
	os_ptr pdest = op;

	for (i = 0; i < asize; i++, pdest++, packed = packed_next(packed))
	    packed_get(imemory, packed, pdest);
    }
    push(asize);
    ref_assign(op, &aref);
    return 0;
}

/* <obj_0> ... <obj_n-1> <array> astore <array> */
static int
zastore(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    uint size;
    int code;

    if (!r_is_array(op))
	return_op_typecheck(op);
    size = r_size(op);
    /* Amazingly, the following is valid: 0 array noaccess astore */
    if (size == 0)
        return 0;
    if (!r_has_type_attrs(op, t_array, a_write)) 
        return_error(e_invalidaccess);
    if (size > op - osbot) {
	/* The store operation might involve other stack segments. */
	ref arr;

	if (size >= ref_stack_count(&o_stack))
	    return_error(e_stackunderflow);
	arr = *op;
	code = ref_stack_store(&o_stack, &arr, size, 1, 0, true, idmemory,
			       "astore");
	if (code < 0)
	    return code;
	ref_stack_pop(&o_stack, size);
	*ref_stack_index(&o_stack, 0) = arr;
    } else {
	code = refcpy_to_old(op, 0, op - size, size, idmemory, "astore");
	if (code < 0)
	    return code;
	op[-(int)size] = *op;
	pop(size);
    }
    return 0;
}

/* ------ Initialization procedure ------ */

const op_def zarray_op_defs[] =
{
    {"1aload", zaload},
    {"1array", zarray},
    {"1astore", zastore},
    op_def_end(0)
};
