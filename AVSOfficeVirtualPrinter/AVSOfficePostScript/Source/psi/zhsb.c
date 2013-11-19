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

/* $Id: zhsb.c 9043 2008-08-28 22:48:19Z giles $ */
/* HSB color operators */
#include "ghost.h"
#include "oper.h"
#include "igstate.h"
#include "store.h"
#include "gshsb.h"

/* - currenthsbcolor <hue> <saturation> <brightness> */
static int
zcurrenthsbcolor(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    float par[3];

    gs_currenthsbcolor(igs, par);
    push(3);
    make_floats(op - 2, par, 3);
    return 0;
}

/* <hue> <saturation> <brightness> sethsbcolor - */
static int
zsethsbcolor(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    double par[3];
    int code;

    if ((code = num_params(op, 3, par)) < 0 ||
	(code = gs_sethsbcolor(igs, par[0], par[1], par[2])) < 0
	)
	return code;
    make_null(&istate->colorspace.array);
    pop(3);
    return 0;
}

/* ------ Initialization procedure ------ */

const op_def zhsb_op_defs[] =
{
    {"0currenthsbcolor", zcurrenthsbcolor},
    {"3sethsbcolor", zsethsbcolor},
    op_def_end(0)
};
