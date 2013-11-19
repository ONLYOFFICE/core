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

/* $Id: zchar2.c 9043 2008-08-28 22:48:19Z giles $ */
/* Type 2 character display operator */
#include "ghost.h"
#include "oper.h"
#include "gxfixed.h"
#include "gxmatrix.h"
#include "gxfont.h"
#include "gxfont1.h"
#include "gxtype1.h"
#include "ichar1.h"

/* <font> <code|name> <name> <charstring> .type2execchar - */
static int
ztype2execchar(i_ctx_t *i_ctx_p)
{
    return charstring_execchar(i_ctx_p, (1 << (int)ft_encrypted2));
}

/* ------ Initialization procedure ------ */

const op_def zchar2_op_defs[] =
{
    {"4.type2execchar", ztype2execchar},
    op_def_end(0)
};
