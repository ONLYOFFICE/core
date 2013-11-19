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

/* $Id: gxcllzw.c 8250 2007-09-25 13:31:24Z giles $ */
/* LZW filter initialization for RAM-based band lists */
#include "std.h"
#include "gstypes.h"
#include "gsmemory.h"
#include "gxclmem.h"
#include "slzwx.h"

static stream_LZW_state cl_LZWE_state;
static stream_LZW_state cl_LZWD_state;

/* Initialize the states to be copied. */
int
gs_cl_lzw_init(gs_memory_t * mem)
{
    s_LZW_set_defaults((stream_state *) & cl_LZWE_state);
    cl_LZWE_state.template = &s_LZWE_template;
    s_LZW_set_defaults((stream_state *) & cl_LZWD_state);
    cl_LZWD_state.template = &s_LZWD_template;
    return 0;
}

/* Return the prototypes for compressing/decompressing the band list. */
const stream_state *
clist_compressor_state(void *client_data)
{
    return (const stream_state *)&cl_LZWE_state;
}
const stream_state *
clist_decompressor_state(void *client_data)
{
    return (const stream_state *)&cl_LZWD_state;
}
