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

/* $Id: gdevpdt.h 8022 2007-06-05 22:23:38Z giles $ */
/* Interface for pdfwrite text and fonts */

#ifndef gdevpdt_INCLUDED
#  define gdevpdt_INCLUDED

/*
 * This file defines a largely opaque interface to the text and font
 * handling code for pdfwrite.  This is the only file that pdfwrite code
 * outside the text and font handling subsystem (pdftext.dev) should
 * #include.
 *
 * The declarations in this file deliberately duplicate declarations in
 * various other header files of the pdfwrite text/font subsystem.
 * This allows the compiler to check them for consistency.
 */

/* ================ Procedures ================ */

/* ---------------- Utility (for gdevpdf.c) ---------------- */

/*
 * Allocate and initialize text state bookkeeping.
 */
pdf_text_state_t *pdf_text_state_alloc(gs_memory_t *mem);

/*
 * Allocate and initialize the text data structure.
 */
pdf_text_data_t *pdf_text_data_alloc(gs_memory_t *mem);	/* gdevpdts.h */

/*
 * Reset the text state at the beginning of the page.
 */
void pdf_reset_text_page(pdf_text_data_t *ptd);	/* gdevpdts.h */

/*
 * Reset the text state after a grestore.
 */
void pdf_reset_text_state(pdf_text_data_t *ptd); /* gdevpdts.h */

/*
 * Update text state at the end of a page.
 */
void pdf_close_text_page(gx_device_pdf *pdev); /* gdevpdti.h */

/*
 * Close the text-related parts of a document, including writing out font
 * and related resources.
 */
int pdf_close_text_document(gx_device_pdf *pdev); /* gdevpdtw.h */

/* ---------------- Contents state (for gdevpdfu.c) ---------------- */

/*
 * Transition from stream context to text context.
 */
int pdf_from_stream_to_text(gx_device_pdf *pdev); /* gdevpdts.h */

/*
 * Transition from string context to text context.
 */
int pdf_from_string_to_text(gx_device_pdf *pdev); /* gdevpdts.h */

/*
 * Close the text aspect of the current contents part.
 */
void pdf_close_text_contents(gx_device_pdf *pdev); /* gdevpdts.h */

/* ---------------- Bitmap fonts (for gdevpdfb.c) ---------------- */

/* Return the Y offset for a bitmap character image. */
int pdf_char_image_y_offset(const gx_device_pdf *pdev, int x, int y, int h);/* gdevpdti.h */

/* Begin a CharProc for an embedded (bitmap) font. */
int pdf_begin_char_proc(gx_device_pdf * pdev, int w, int h, int x_width,
			int y_offset, gs_id id, pdf_char_proc_t **ppcp,
			pdf_stream_position_t * ppos); /* gdevpdti.h */

/* End a CharProc. */
int pdf_end_char_proc(gx_device_pdf * pdev,
		      pdf_stream_position_t * ppos); /* gdevpdti.h */

/* Put out a reference to an image as a character in an embedded font. */
int pdf_do_char_image(gx_device_pdf * pdev, const pdf_char_proc_t * pcp,
		      const gs_matrix * pimat);	/* gdevpdti.h */

#endif /* gdevpdt_INCLUDED */
