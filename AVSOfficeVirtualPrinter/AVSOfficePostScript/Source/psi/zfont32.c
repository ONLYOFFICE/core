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

/* $Id: zfont32.c 9043 2008-08-28 22:48:19Z giles $ */
/* Type 32 font operators */
#include "ghost.h"
#include "oper.h"
#include "gsccode.h"		/* for gxfont.h */
#include "gsmatrix.h"
#include "gsutil.h"
#include "gxfont.h"
#include "gxtext.h"
#include "bfont.h"
#include "store.h"
#include "ichar.h"

/* The encode_char procedure of a Type 32 font should never be called. */
static gs_glyph
zfont_no_encode_char(gs_font *pfont, gs_char chr, gs_glyph_space_t ignored)
{
    return gs_no_glyph;
}

/* <string|name> <font_dict> .buildfont32 <string|name> <font> */
/* Build a type 32 (bitmap) font. */
static int
zbuildfont32(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code;
    build_proc_refs build;
    gs_font_base *pfont;

    check_type(*op, t_dictionary);
    code = build_proc_name_refs(imemory, &build, NULL, "%Type32BuildGlyph");
    if (code < 0)
	return code;
    code = build_gs_simple_font(i_ctx_p, op, &pfont, ft_CID_bitmap,
				&st_gs_font_base, &build,
				bf_Encoding_optional);
    if (code < 0)
	return code;
    /* Always transform cached bitmaps. */
    pfont->BitmapWidths = true;
    pfont->ExactSize = fbit_transform_bitmaps;
    pfont->InBetweenSize = fbit_transform_bitmaps;
    pfont->TransformedChar = fbit_transform_bitmaps;
    /* The encode_char procedure of a Type 32 font */
    /* should never be called. */
    pfont->procs.encode_char = zfont_no_encode_char;
    return define_gs_font((gs_font *) pfont);
}

/* - .getshowoperator <oper|null> */
/* Get the calling operator for error reporting in %Type32BuildGlyph */
static int
zgetshowoperator(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_text_enum_t *osenum = op_show_find(i_ctx_p);
  
    push(1);
    if (osenum == NULL)
        make_null(op);
    else {
        op_proc_t proc;
        *(void **)&proc = osenum->enum_client_data;
        make_oper(op, 0, proc);
    }
    return 0;
}


/* ------ Initialization procedure ------ */

const op_def zfont32_op_defs[] =
{
    {"2.buildfont32", zbuildfont32},
    {"0.getshowoperator", zgetshowoperator},
    op_def_end(0)
};
