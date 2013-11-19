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

/* $Id: zfarc4.c 9043 2008-08-28 22:48:19Z giles $ */

/* this is the ps interpreter interface to the arcfour cipher filter
   used in PDF encryption. We provide both Decode and Encode filters;
   the cipher is symmetric, so the call to the underlying routines is
   identical between the two filters */

#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "ialloc.h"
#include "idict.h"
#include "stream.h"
#include "strimpl.h"
#include "ifilter.h"
#include "sarc4.h"

/* <source> <dict> arcfour/filter <file> */

static int
z_arcfour_d(i_ctx_t * i_ctx_p)
{
    os_ptr op = osp;		/* i_ctx_p->op_stack.stack.p defined in osstack.h */
    ref *sop = NULL;
    stream_arcfour_state state;

    /* extract the key from the parameter dictionary */
    check_type(*op, t_dictionary);
    check_dict_read(*op);
    if (dict_find_string(op, "Key", &sop) <= 0)
	return_error(e_rangecheck);

    s_arcfour_set_key(&state, sop->value.const_bytes, r_size(sop));

    /* we pass npop=0, since we've no arguments left to consume */
    /* we pass 0 instead of the usual rspace(sop) will allocate storage for 
       filter state from the same memory pool as the stream it's coding. this
       causes no trouble because we maintain no pointers */
    return filter_read(i_ctx_p, 0, &s_arcfour_template,
		       (stream_state *) & state, 0);
}

/* encode version of the filter */
static int
z_arcfour_e(i_ctx_t * i_ctx_p)
{
    os_ptr op = osp;		/* i_ctx_p->op_stack.stack.p defined in osstack.h */
    ref *sop = NULL;
    stream_arcfour_state state;

    /* extract the key from the parameter dictionary */
    check_type(*op, t_dictionary);
    check_dict_read(*op);
    if (dict_find_string(op, "Key", &sop) <= 0)
	return_error(e_rangecheck);

    s_arcfour_set_key(&state, sop->value.const_bytes, r_size(sop));

    /* we pass npop=0, since we've no arguments left to consume */
    /* we pass 0 instead of the usual rspace(sop) will allocate storage for 
       filter state from the same memory pool as the stream it's coding. this
       causes no trouble because we maintain no pointers */
    return filter_write(i_ctx_p, 0, &s_arcfour_template,
			(stream_state *) & state, 0);
}

/* Match the above routines to their postscript filter names.
   This is how our static routines get called externally. */
const op_def zfarc4_op_defs[] = {
    op_def_begin_filter(),
    {"2ArcfourDecode", z_arcfour_d},
    {"2ArcfourEncode", z_arcfour_e},
    op_def_end(0)
};
