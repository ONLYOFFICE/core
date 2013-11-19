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

/* $Id: zfcid.c 9043 2008-08-28 22:48:19Z giles $ */
/* CID-keyed font utilities */
#include "ghost.h"
#include "oper.h"
#include "gsmatrix.h"
#include "gxfcid.h"
#include "bfont.h"
#include "icid.h"
#include "idict.h"
#include "idparam.h"
#include "ifcid.h"
#include "store.h"

/* Get the CIDSystemInfo of a CIDFont. */
int
cid_font_system_info_param(gs_cid_system_info_t *pcidsi, const ref *prfont)
{
    ref *prcidsi;

    if (dict_find_string(prfont, "CIDSystemInfo", &prcidsi) <= 0)
	return_error(e_rangecheck);
    return cid_system_info_param(pcidsi, prcidsi);
}

/* Get the additional information for a CIDFontType 0 or 2 CIDFont. */
int
cid_font_data_param(os_ptr op, gs_font_cid_data *pdata, ref *pGlyphDirectory)
{
    int code;
    ref *pgdir;

    check_type(*op, t_dictionary);
    if ((code = cid_font_system_info_param(&pdata->CIDSystemInfo, op)) < 0 ||
	(code = dict_int_param(op, "CIDCount", 0, max_int, -1,
			       &pdata->CIDCount)) < 0
	)
	return code;
    /*
     * If the font doesn't have a GlyphDirectory, GDBytes is required.
     * If it does have a GlyphDirectory, GDBytes may still be needed for
     * CIDMap: it's up to the client to check this.
     */
    if (dict_find_string(op, "GlyphDirectory", &pgdir) <= 0) {
	/* Standard CIDFont, require GDBytes. */
	make_null(pGlyphDirectory);
	return dict_int_param(op, "GDBytes", 1, MAX_GDBytes, 0,
			      &pdata->GDBytes);
    }
    if (r_has_type(pgdir, t_dictionary) || r_is_array(pgdir)) {
	/* GlyphDirectory, GDBytes is optional. */
	*pGlyphDirectory = *pgdir;
	code = dict_int_param(op, "GDBytes", 0, MAX_GDBytes, 0,
			      &pdata->GDBytes);
	return code;
    } else {
	return_error(e_typecheck);
    }
}
