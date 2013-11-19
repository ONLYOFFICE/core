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

/* $Id: gdevpdt.c 9114 2008-09-22 10:56:52Z ken $ */
/* Miscellaneous external entry points for pdfwrite text */
#include "gx.h"
#include "memory_.h"
#include "gdevpdfx.h"
#include "gdevpdtx.h"
#include "gdevpdtf.h"
#include "gdevpdti.h"

/* GC descriptors */
private_st_pdf_text_data();

/* ---------------- Initialization ---------------- */

/*
 * Allocate and initialize the text data structure.
 */
pdf_text_data_t *
pdf_text_data_alloc(gs_memory_t *mem)
{
    pdf_text_data_t *ptd =
	gs_alloc_struct(mem, pdf_text_data_t, &st_pdf_text_data,
			"pdf_text_data_alloc");
    pdf_outline_fonts_t *pofs = pdf_outline_fonts_alloc(mem);
    pdf_bitmap_fonts_t *pbfs = pdf_bitmap_fonts_alloc(mem);
    pdf_text_state_t *pts = pdf_text_state_alloc(mem);

    if (pts == 0 || pbfs == 0 || pofs == 0 || ptd == 0) {
	gs_free_object(mem, pts, "pdf_text_data_alloc");
	gs_free_object(mem, pbfs, "pdf_text_data_alloc");
	gs_free_object(mem, pofs, "pdf_text_data_alloc");
	gs_free_object(mem, ptd, "pdf_text_data_alloc");
	return 0;
    }
    memset(ptd, 0, sizeof(*ptd));
    ptd->outline_fonts = pofs;
    ptd->bitmap_fonts = pbfs;
    ptd->text_state = pts;
    return ptd;
}
