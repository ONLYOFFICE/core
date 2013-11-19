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

/* $Id: ztrap.c 9043 2008-08-28 22:48:19Z giles $ */
/* Operators for setting trapping parameters and zones */
#include "ghost.h"
#include "oper.h"
#include "ialloc.h"
#include "iparam.h"
#include "gstrap.h"

/* Define the current trap parameters. */
/****** THIS IS BOGUS ******/
gs_trap_params_t i_trap_params;

/* <dict> .settrapparams - */
static int
zsettrapparams(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    dict_param_list list;
    int code;

    check_type(*op, t_dictionary);
    code = dict_param_list_read(&list, op, NULL, false, iimemory);
    if (code < 0)
	return code;
    code = gs_settrapparams(&i_trap_params, (gs_param_list *) & list);
    iparam_list_release(&list);
    if (code < 0)
	return code;
    pop(1);
    return 0;
}

/* - settrapzone - */
static int
zsettrapzone(i_ctx_t *i_ctx_p)
{
/****** NYI ******/
    return_error(e_undefined);
}

/* ------ Initialization procedure ------ */

const op_def ztrap_op_defs[] =
{
    op_def_begin_ll3(),
    {"1.settrapparams", zsettrapparams},
    {"0settrapzone", zsettrapzone},
    op_def_end(0)
};
