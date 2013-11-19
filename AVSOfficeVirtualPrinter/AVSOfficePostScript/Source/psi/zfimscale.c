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

/* $Id: zfimscale.c 6651 2006-03-13 16:18:19Z raph $ */

/* This is the ps interpreter interface to the image mask interpolating
   filter. */

#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "ialloc.h"
#include "idict.h"
#include "stream.h"
#include "strimpl.h"
#include "ifilter.h"
#include "idparam.h"
#include "sisparam.h"
#include "simscale.h"

/* <source> <dict> imscale/filter <file> */

static int
z_imscale_d(i_ctx_t * i_ctx_p)
{
    os_ptr op = osp;		/* i_ctx_p->op_stack.stack.p defined in osstack.h */
    int width, height;
    stream_imscale_state state;

    /* extract the key from the parameter dictionary */
    check_type(*op, t_dictionary);
    check_dict_read(*op);
    if (dict_int_param(op, "Width", 0, 1<<24, -1, &width) < 0)
	return_error(e_rangecheck);
    if (dict_int_param(op, "Height", 0, 1<<24, -1, &height) < 0)
	return_error(e_rangecheck);

    state.params.Colors = 1;
    state.params.BitsPerComponentIn = 1;
    state.params.MaxValueIn = 1;
    state.params.WidthIn = width;
    state.params.HeightIn = height;
    state.params.BitsPerComponentOut = 1;
    state.params.MaxValueOut = 1;
    state.params.WidthOut = width << 2;
    state.params.HeightOut = height << 2;

    /* we pass npop=0, since we've no arguments left to consume */
    /* we pass 0 instead of the usual rspace(sop) will allocate storage for 
       filter state from the same memory pool as the stream it's coding. this
       causes no trouble because we maintain no pointers */
    return filter_read(i_ctx_p, 0, &s_imscale_template,
		       (stream_state *) & state, 0);
}

/* Match the above routines to their postscript filter names.
   This is how our static routines get called externally. */
const op_def zfimscale_op_defs[] = {
    op_def_begin_filter(),
    {"2ImscaleDecode", z_imscale_d},
    op_def_end(0)
};
