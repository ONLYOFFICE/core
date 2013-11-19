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

/* $Id: zpaint.c 9043 2008-08-28 22:48:19Z giles $ */
/* Painting operators */
#include "ghost.h"
#include "oper.h"
#include "gspaint.h"
#include "igstate.h"

/* - fill - */
static int
zfill(i_ctx_t *i_ctx_p)
{
    return gs_fill(igs);
}

/* - eofill - */
static int
zeofill(i_ctx_t *i_ctx_p)
{
    return gs_eofill(igs);
}

/* - stroke - */
static int
zstroke(i_ctx_t *i_ctx_p)
{
    return gs_stroke(igs);
}

/* ------ Non-standard operators ------ */

/* - .fillpage - */
static int
zfillpage(i_ctx_t *i_ctx_p)
{
    return gs_fillpage(igs);
}

/* <width> <height> <data> .imagepath - */
static int
zimagepath(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code;

    check_type(op[-2], t_integer);
    check_type(op[-1], t_integer);
    check_read_type(*op, t_string);
    if (r_size(op) < ((op[-2].value.intval + 7) >> 3) * op[-1].value.intval)
	return_error(e_rangecheck);
    code = gs_imagepath(igs,
			(int)op[-2].value.intval, (int)op[-1].value.intval,
			op->value.const_bytes);
    if (code >= 0)
	pop(3);
    return code;
}

/* ------ Initialization procedure ------ */

const op_def zpaint_op_defs[] =
{
    {"0eofill", zeofill},
    {"0fill", zfill},
    {"0stroke", zstroke},
		/* Non-standard operators */
    {"0.fillpage", zfillpage},
    {"3.imagepath", zimagepath},
    op_def_end(0)
};
