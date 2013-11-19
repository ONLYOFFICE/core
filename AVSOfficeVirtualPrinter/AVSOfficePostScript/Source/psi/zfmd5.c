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

/* $Id: zfmd5.c 9043 2008-08-28 22:48:19Z giles $ */
/* MD5Encode filter creation */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "ialloc.h"
#include "stream.h"
#include "strimpl.h"
#include "smd5.h"
#include "ifilter.h"

/* <source> MD5Encode/filter <file> */
/* <source> <dict> MD5Encode/filter <file> */
static int
zMD5E(i_ctx_t *i_ctx_p)
{
    return filter_write_simple(i_ctx_p, &s_MD5E_template);
}

/* ------ Initialization procedure ------ */

const op_def zfmd5_op_defs[] =
{
    op_def_begin_filter(),
    {"1MD5Encode", zMD5E},
    op_def_end(0)
};
