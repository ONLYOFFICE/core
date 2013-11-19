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

/* $Id: zcidtest.c 9043 2008-08-28 22:48:19Z giles $ */
/* Operators for testing CIDFont and CMap facilities */
#include "string_.h"
#include "ghost.h"
#include "gxfont.h"
#include "gxfont0c.h"
#include "gdevpsf.h"
#include "stream.h"
#include "spprint.h"
#include "oper.h"
#include "files.h"
#include "idict.h"
#include "ifont.h"
#include "igstate.h"
#include "iname.h"
#include "store.h"

/* - .wrapfont - */
static int
zwrapfont(i_ctx_t *i_ctx_p)
{
    gs_font *font = gs_currentfont(igs);
    gs_font_type0 *font0;
    int wmode = 0;
    int code;

    switch (font->FontType) {
    case ft_TrueType:
	code = gs_font_type0_from_type42(&font0, (gs_font_type42 *)font, wmode,
					 true, font->memory);
	if (code < 0)
	    return code;
	/*
	 * Patch up BuildChar and CIDMap.  This isn't necessary for
	 * TrueType fonts in general, only for Type 42 fonts whose
	 * BuildChar is implemented in PostScript code.
	 */
	{
	    font_data *pdata = pfont_data(font);
	    const char *bgstr = "%Type11BuildGlyph";
	    ref temp;

	    make_int(&temp, 0);
	    ref_assign(&pdata->u.type42.CIDMap, &temp);
	    code = name_ref((const byte *)bgstr, strlen(bgstr), &temp, 1);
	    if (code < 0)
		return code;
	    r_set_attrs(&temp, a_executable);
	    ref_assign(&pdata->BuildGlyph, &temp);
	}
	break;
    case ft_CID_encrypted:
    case ft_CID_user_defined:
    case ft_CID_TrueType:
	code = gs_font_type0_from_cidfont(&font0, font, wmode, NULL,
					  font->memory);
	break;
    default:
	return_error(e_rangecheck);
    }
    if (code < 0)
	return code;
    gs_setfont(igs, (gs_font *)font0);
    return 0;
}

/* <file> <cmap> .writecmap - */
static int
zfcmap_put_name_default(stream *s, const byte *str, uint size)
{
    stream_putc(s, '/');
    stream_write(s, str, size);
    return 0;
}
static int
zwritecmap(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    ref *pcodemap;
    gs_cmap_t *pcmap;
    int code;
    stream *s;

    check_type(*op, t_dictionary);
    if (dict_find_string(op, "CodeMap", &pcodemap) <= 0 ||
	!r_is_struct(pcodemap)
	)
	return_error(e_typecheck);
    check_write_file(s, op - 1);
    pcmap = r_ptr(pcodemap, gs_cmap_t);
    code = psf_write_cmap(s, pcmap, zfcmap_put_name_default, NULL, -1);
    if (code >= 0)
	pop(2);
    return code;
}

/* <file> <cid9font> .writefont9 - */
static int
zwritefont9(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_font *pfont;
    gs_font_cid0 *pfcid;
    int code = font_param(op, &pfont);
    stream *s;

    if (code < 0)
	return code;
    if (pfont->FontType != ft_CID_encrypted)
	return_error(e_invalidfont);
    check_write_file(s, op - 1);
    pfcid = (gs_font_cid0 *)pfont;
    code = psf_write_cid0_font(s, pfcid,
			       WRITE_TYPE2_NO_LENIV | WRITE_TYPE2_CHARSTRINGS,
			       NULL, 0, NULL);
    if (code >= 0)
	pop(2);
    return code;
}

/* ------ Initialization procedure ------ */

const op_def zcidtest_op_defs[] =
{
    {"1.wrapfont", zwrapfont},
    {"2.writecmap", zwritecmap},
    {"2.writefont9", zwritefont9},
    op_def_end(0)
};
