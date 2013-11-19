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

/* $Id: zfunc3.c 9043 2008-08-28 22:48:19Z giles $ */
/* PostScript language interface to LL3 Functions */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsfunc3.h"
#include "gsstruct.h"
#include "stream.h"		/* for files.h */
#include "files.h"
#include "ialloc.h"
#include "idict.h"
#include "idparam.h"
#include "ifunc.h"
#include "store.h"

/* Check prototypes */
build_function_proc(gs_build_function_2);
build_function_proc(gs_build_function_3);

/* Finish building a FunctionType 2 (ExponentialInterpolation) function. */
int
gs_build_function_2(i_ctx_t *i_ctx_p, const ref *op, const gs_function_params_t * mnDR,
		    int depth, gs_function_t ** ppfn, gs_memory_t *mem)
{
    gs_function_ElIn_params_t params;
    int code, n0, n1;

    *(gs_function_params_t *)&params = *mnDR;
    params.C0 = 0;
    params.C1 = 0;
    if ((code = dict_float_param(op, "N", 0.0, &params.N)) != 0 ||
	(code = n0 = fn_build_float_array_forced(op, "C0", false, &params.C0, mem)) < 0 ||
	(code = n1 = fn_build_float_array_forced(op, "C1", false, &params.C1, mem)) < 0
	)
	goto fail;
    if (params.C0 == 0)
	n0 = 1;			/* C0 defaulted */
    if (params.C1 == 0)
	n1 = 1;			/* C1 defaulted */
    if (params.Range == 0)
	params.n = n0;		/* either one will do */
    if (n0 != n1 || n0 != params.n)
	goto fail;
    code = gs_function_ElIn_init(ppfn, &params, mem);
    if (code >= 0)
	return 0;
fail:
    gs_function_ElIn_free_params(&params, mem);
    return (code < 0 ? code : gs_note_error(e_rangecheck));
}

/* Finish building a FunctionType 3 (1-Input Stitching) function. */
int
gs_build_function_3(i_ctx_t *i_ctx_p, const ref *op, const gs_function_params_t * mnDR,
		    int depth, gs_function_t ** ppfn, gs_memory_t *mem)
{
    gs_function_1ItSg_params_t params;
    int code;
    extern bool CPSI_mode;

    *(gs_function_params_t *) & params = *mnDR;
    params.Functions = 0;
    params.Bounds = 0;
    params.Encode = 0;
    {
	ref *pFunctions;
	gs_function_t **ptr;
	int i;

	if ((code = dict_find_string(op, "Functions", &pFunctions)) <= 0)
	    return (code < 0 ? code : gs_note_error(e_rangecheck));
	check_array_only(*pFunctions);
	params.k = r_size(pFunctions);
	code = alloc_function_array(params.k, &ptr, mem);
	if (code < 0)
	    return code;
	params.Functions = (const gs_function_t * const *)ptr;
	for (i = 0; i < params.k; ++i) {
	    ref subfn;

	    array_get(mem, pFunctions, (long)i, &subfn);
	    code = fn_build_sub_function(i_ctx_p, &subfn, &ptr[i], depth, mem, 0, 0);
	    if (code < 0)
		goto fail;
	}
    }
    if ((code = fn_build_float_array(op, "Bounds", true, false, &params.Bounds, mem)) != params.k - 1)
	goto fail;
    if (CPSI_mode) {
	/* Adobe implementation doesn't check the Encode length. */
	/* Extra elements are ignored; missing elements are filled with 0. */
	/* CET 12-14m.ps depends on this bug */
	uint sz, k2 = 2 * params.k;
	ref *encode;
	float *p = (float *)gs_alloc_byte_array(mem, k2, sizeof(float), "Encode");

	params.Encode = p;
	if (p == 0) {
	    code = gs_note_error(e_VMerror);
	    goto fail;
	}
	if (dict_find_string(op, "Encode", &encode) <= 0) {
	    code = gs_note_error(e_undefined);
	    goto fail;
	}
	if (!r_is_array(encode)) {
	    code = gs_note_error(e_typecheck);
	    goto fail;
	}
	sz =  min(k2, r_size(encode));
	code = process_float_array(mem, encode, sz, p);
	if (code < 0)
	    goto fail;
	while (sz < k2)
	    p[sz++] = 0.0;
    } else if ((code = fn_build_float_array(op, "Encode", true, true, &params.Encode, mem)) != 2 * params.k)
	goto fail;
    if (params.Range == 0)
	params.n = params.Functions[0]->params.n;
    code = gs_function_1ItSg_init(ppfn, &params, mem);
    if (code >= 0)
	return 0;
fail:
    gs_function_1ItSg_free_params(&params, mem);
    return (code < 0 ? code : gs_note_error(e_rangecheck));
}
