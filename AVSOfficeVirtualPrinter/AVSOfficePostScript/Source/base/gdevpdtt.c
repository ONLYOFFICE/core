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

/* $Id: gdevpdtt.c 9200 2008-11-06 19:09:39Z giles $ */
/* Text processing for pdfwrite. */
#include "math_.h"
#include "string_.h"
#include "gx.h"
#include "gserrors.h"
#include "gscencs.h"
#include "gscedata.h"
#include "gsmatrix.h"
#include "gzstate.h"
#include "gxfcache.h"		/* for orig_fonts list */
#include "gxfont.h"
#include "gxfont0.h"
#include "gxfcid.h"
#include "gxfcopy.h"
#include "gxfcmap.h"
#include "gxpath.h"		/* for getting current point */
#include "gxchar.h"		
#include "gxstate.h"		
#include "gdevpdfx.h"
#include "gdevpdfg.h"
#include "gdevpdfo.h"
#include "gdevpdtx.h"
#include "gdevpdtd.h"
#include "gdevpdtf.h"
#include "gdevpdts.h"
#include "gdevpdtt.h"
#include "gdevpdti.h"
#include "gxhldevc.h"
#include "gzpath.h"

/* ================ Text enumerator ================ */

/* GC descriptor */
private_st_pdf_text_enum();

/* A static data for pdf_reserve_char_code_in_pdfont.
   No, this isn't a reenterability error,
   because all threads will set same value,
   which actually depends on static data only. */
static gs_glyph standard_glyph_code_for_notdef = GS_NO_GLYPH;

/* Define the auxiliary procedures for text processing. */
static int
pdf_text_resync(gs_text_enum_t *pte, const gs_text_enum_t *pfrom)
{
    pdf_text_enum_t *const penum = (pdf_text_enum_t *)pte;

    if ((pte->text.operation ^ pfrom->text.operation) & ~TEXT_FROM_ANY)
	return_error(gs_error_rangecheck);
    if (penum->pte_default) {
	int code = gs_text_resync(penum->pte_default, pfrom);

	if (code < 0)
	    return code;
    }
    pte->text = pfrom->text;
    gs_text_enum_copy_dynamic(pte, pfrom, false);
    return 0;
}
static bool
pdf_text_is_width_only(const gs_text_enum_t *pte)
{
    const pdf_text_enum_t *const penum = (const pdf_text_enum_t *)pte;

    if (penum->pte_default)
	return gs_text_is_width_only(penum->pte_default);
    return false;
}
static int
pdf_text_current_width(const gs_text_enum_t *pte, gs_point *pwidth)
{
    const pdf_text_enum_t *const penum = (const pdf_text_enum_t *)pte;

    if (penum->pte_default)
	return gs_text_current_width(penum->pte_default, pwidth);
    return_error(gs_error_rangecheck); /* can't happen */
}
static int
pdf_text_set_cache(gs_text_enum_t *pte, const double *pw,
		   gs_text_cache_control_t control)
{
    pdf_text_enum_t *const penum = (pdf_text_enum_t *)pte;
    gx_device_pdf *pdev = (gx_device_pdf *)pte->dev;

    if (pdev->type3charpath)
	return gs_text_set_cache(penum->pte_default, pw, control);

    switch (control) {
    case TEXT_SET_CHAR_WIDTH:
    case TEXT_SET_CACHE_DEVICE:
	gs_distance_transform(pw[0], pw[1], &ctm_only(pte->pis), &pdev->char_width);
	break;
    case TEXT_SET_CACHE_DEVICE2:
	/*
	 * pdev->char_width is used with synthesized Type 3 fonts only.
	 * Since they are simple fonts, we only need the horisontal
	 * width for Widths array. Therefore we don't check 
	 * gs_rootfont(pgs)->WMode and don't use pw[6:7].
	 */
	gs_distance_transform(pw[0], pw[1], &ctm_only(pte->pis), &pdev->char_width);
	if (penum->cdevproc_callout) {
	    memcpy(penum->cdevproc_result, pw, sizeof(penum->cdevproc_result));
	    return 0;
	}
	break;
    default:
	return_error(gs_error_rangecheck);
    }
    if (penum->pte_default == NULL)
	return_error(gs_error_unregistered); /* Must not happen. */

    if (penum->current_font->FontType == ft_user_defined && 
	    penum->outer_CID == GS_NO_GLYPH &&
	    !(penum->pte_default->text.operation & TEXT_DO_CHARWIDTH)) {
	int code;
	gs_glyph glyph;

        glyph = penum->returned.current_glyph;
	if (glyph != GS_NO_GLYPH && penum->output_char_code != GS_NO_CHAR) {
	    gs_show_enum *penum_s;
	    extern_st(st_gs_show_enum);
	    gs_fixed_rect clip_box;
	    double pw1[10];
	    int narg = (control == TEXT_SET_CHAR_WIDTH ? 2 : 
			control == TEXT_SET_CACHE_DEVICE ? 6 : 10), i;

	    /* Check to verify the structure type is really gs_show_enum */
	    if (gs_object_type(penum->pte_default->memory, penum->pte_default) != &st_gs_show_enum) {
		/* Must not happen with PS interpreter. 
		   Other clients should conform. */
		return_error(gs_error_unregistered); 
	    }
	    penum_s = (gs_show_enum *)penum->pte_default;
	    /* BuildChar could change the scale before calling setcachedevice (Bug 687290). 
	       We must scale the setcachedevice arguments because we assumed
	       identity scale before entering the charproc.
	       For now we only handle scaling matrices.
	    */
	    for (i = 0; i < narg; i += 2) {
		gs_point p;

		gs_point_transform(pw[i], pw[i + 1], &ctm_only(penum_s->pgs), &p);
		pw1[i] = p.x;
		pw1[i + 1] = p.y;
	    }
	    if (control != TEXT_SET_CHAR_WIDTH) {
		clip_box.p.x = float2fixed(pw1[2]);
		clip_box.p.y = float2fixed(pw1[3]);
		clip_box.q.x = float2fixed(pw1[4]);
		clip_box.q.y = float2fixed(pw1[5]);
	    } else {
		/*
		 * We have no character bbox, but we need one to install the clipping
		 * to the graphic state of the PS interpreter. Since some fonts don't
		 * provide a proper FontBBox (Bug 687239 supplies a zero one),
		 * we set an "infinite" clipping here.
		 * We also detected that min_int, max_int don't work here with
		 * comparefiles/Bug687044.ps, therefore we divide them by 2.
		 */
		clip_box.p.x = clip_box.p.y = min_int / 2;
		clip_box.q.x = clip_box.q.y = max_int / 2;
	    }
	    code = gx_clip_to_rectangle(penum_s->pgs, &clip_box);
	    if (code < 0)
		return code;
	    code = pdf_set_charproc_attrs(pdev, pte->current_font, 
			pw1, narg, control, penum->output_char_code);
	    if (code < 0)
		return code;
	    /* Prevent writing the clipping path to charproc.
	       See the comment above and bugs 687678, 688327.
	       Note that the clipping in the graphic state will be used while 
	       fallbacks to default implementations of graphic objects. 
	       Hopely such fallbacks are rare. */
	    pdev->clip_path_id = gx_get_clip_path_id(penum_s->pgs);
	    return code;
	} else {
	    gs_matrix m;
	    pdf_resource_t *pres = pdev->accumulating_substream_resource;

	    /* pdf_text_process started a charproc stream accumulation,
	       but now we re-decided to go with the default implementation.
	       Cancel the stream now.
	     */
	    code = pdf_exit_substream(pdev);
	    if (code < 0)
		return code;
	    code = pdf_cancel_resource(pdev, pres, resourceCharProc);
	    if (code < 0)
		return code;
	    pdf_forget_resource(pdev, pres, resourceCharProc);
	    /* pdf_text_process had set an identity CTM for the
	       charproc stream accumulation, but now we re-decided
	       to go with the default implementation.
	       Need to restore the correct CTM and add
	       changes, which the charproc possibly did. */
	    gs_matrix_multiply((gs_matrix *)&pdev->charproc_ctm, (gs_matrix *)&penum->pis->ctm, &m);
	    gs_matrix_fixed_from_matrix(&penum->pis->ctm, &m);
	    penum->charproc_accum = false;
	}
    }
    if (penum->pte_default) {
	if (penum->pte_default->text.operation & TEXT_DO_CHARWIDTH /* See process_cmap_text.*/)
	    return gs_text_set_cache(penum->pte_default, pw, TEXT_SET_CHAR_WIDTH);
	else
	    return gs_text_set_cache(penum->pte_default, pw, control);
    }
    return_error(gs_error_unregistered); /* can't happen */
}
static int
pdf_text_retry(gs_text_enum_t *pte)
{
    pdf_text_enum_t *const penum = (pdf_text_enum_t *)pte;

    if (penum->pte_default)
	return gs_text_retry(penum->pte_default);
    return_error(gs_error_rangecheck); /* can't happen */
}
static void
pdf_text_release(gs_text_enum_t *pte, client_name_t cname)
{
    pdf_text_enum_t *const penum = (pdf_text_enum_t *)pte;

    if (penum->pte_default) {
	gs_text_release(penum->pte_default, cname);
	penum->pte_default = 0;
    }
    pdf_text_release_cgp(penum);
    gx_default_text_release(pte, cname);
}
void
pdf_text_release_cgp(pdf_text_enum_t *penum)
{
    if (penum->cgp) {
	gs_free_object(penum->memory, penum->cgp, "pdf_text_release");
	penum->cgp = 0;
    }
}

/* Begin processing text. */
static text_enum_proc_process(pdf_text_process);
static const gs_text_enum_procs_t pdf_text_procs = {
    pdf_text_resync, pdf_text_process,
    pdf_text_is_width_only, pdf_text_current_width,
    pdf_text_set_cache, pdf_text_retry,
    pdf_text_release
};

static int
pdf_prepare_text_drawing(gx_device_pdf *const pdev, gs_text_enum_t *pte)
{
    gs_imager_state * pis = pte->pis;
    const gx_device_color * pdcolor = pte->pdcolor;
    const gx_clip_path * pcpath = pte->pcpath;
    const gs_text_params_t *text = &pte->text;
    bool new_clip = false; /* Quiet compiler. */
    int code;

    if (!(text->operation & TEXT_DO_NONE) || pis->text_rendering_mode == 3) {
	new_clip = pdf_must_put_clip_path(pdev, pcpath);
	if (new_clip)
	    code = pdf_unclip(pdev);
	else if (pdev->context == PDF_IN_NONE)
	    code = pdf_open_page(pdev, PDF_IN_STREAM);
	else
	    code = 0;
	if (code < 0)
	    return code;
	code = pdf_prepare_fill(pdev, pis);
	if (code < 0)
	    return code;
    }
    if (text->operation & TEXT_DO_DRAW) {
	/*
	 * Set the clipping path and drawing color.  We set both the fill
	 * and stroke color, because we don't know whether the fonts will be
	 * filled or stroked, and we can't set a color while we are in text
	 * mode.  (This is a consequence of the implementation, not a
	 * limitation of PDF.)
	 */

	if (new_clip) {
	    code = pdf_put_clip_path(pdev, pcpath);
	    if (code < 0)
		return code;
	}

	if ((code =
	     pdf_set_drawing_color(pdev, pis, pdcolor, &pdev->saved_stroke_color,
				   &pdev->stroke_used_process_color,
				   &psdf_set_stroke_color_commands)) < 0 ||
	    (code =
	     pdf_set_drawing_color(pdev, pis, pdcolor, &pdev->saved_fill_color,
				   &pdev->fill_used_process_color,
				   &psdf_set_fill_color_commands)) < 0
	    )
	    return code;
    }
    return 0;
}

int
gdev_pdf_text_begin(gx_device * dev, gs_imager_state * pis,
		    const gs_text_params_t *text, gs_font * font,
		    gx_path * path0, const gx_device_color * pdcolor,
		    const gx_clip_path * pcpath,
		    gs_memory_t * mem, gs_text_enum_t ** ppte)
{
    gx_device_pdf *const pdev = (gx_device_pdf *)dev;
    gx_path *path = path0;
    pdf_text_enum_t *penum;
    int code;

    /* Track the dominant text rotation. */
    {
	gs_matrix tmat;
	int i;

	gs_matrix_multiply(&font->FontMatrix, &ctm_only(pis), &tmat);
	if (is_xxyy(&tmat))
	    i = (tmat.xx >= 0 ? 0 : 2);
	else if (is_xyyx(&tmat))
	    i = (tmat.xy >= 0 ? 1 : 3);
	else
	    i = 4;
	pdf_current_page(pdev)->text_rotation.counts[i] += text->size;
    }

    pdev->last_charpath_op = 0;
    if ((text->operation & TEXT_DO_ANY_CHARPATH) && !path0->first_subpath) {
	if (pdf_compare_text_state_for_charpath(pdev->text->text_state, pdev, pis, font, text))
	    pdev->last_charpath_op = text->operation & TEXT_DO_ANY_CHARPATH;
    }

    if (font->FontType != ft_user_defined || !(text->operation & TEXT_DO_ANY_CHARPATH)) {
        if (font->FontType == ft_user_defined &&
	    (text->operation & TEXT_DO_NONE) && (text->operation & TEXT_RETURN_WIDTH)) {
	    /* This is stringwidth, see gx_default_text_begin.
	     * We need to prevent writing characters to PS cache,
	     * otherwise the font converts to bitmaps.
	     * So pass through even with stringwidth.
	     */
	    code = gx_hld_stringwidth_begin(pis, &path);
	    if (code < 0)
		return code;
	} else if ((!(text->operation & TEXT_DO_DRAW) && pis->text_rendering_mode != 3) 
		    || path == 0 || !path_position_valid(path)
		    || pdev->type3charpath) 
	    return gx_default_text_begin(dev, pis, text, font, path, pdcolor,
					 pcpath, mem, ppte);
	else if (text->operation & TEXT_DO_ANY_CHARPATH)
	    return gx_default_text_begin(dev, pis, text, font, path, pdcolor,
					 pcpath, mem, ppte);
    }

    /* Allocate and initialize the enumerator. */

    rc_alloc_struct_1(penum, pdf_text_enum_t, &st_pdf_text_enum, mem,
		      return_error(gs_error_VMerror), "gdev_pdf_text_begin");
    penum->rc.free = rc_free_text_enum;
    penum->pte_default = 0; 
    penum->charproc_accum = false;
    penum->cdevproc_callout = false;
    penum->returned.total_width.x = penum->returned.total_width.y = 0;
    penum->cgp = NULL;
    penum->output_char_code = GS_NO_CHAR;
    code = gs_text_enum_init((gs_text_enum_t *)penum, &pdf_text_procs,
			     dev, pis, text, font, path, pdcolor, pcpath, mem);
    if (code < 0) {
	gs_free_object(mem, penum, "gdev_pdf_text_begin");
	return code;
    }
    if (pdev->font3 != 0) {
	/* A text operation happens while accumulating a charproc.
	   This is a case when source document uses a Type 3 font,
	   which's charproc uses another font.
	   Since the text operation is handled by the device,
	   the font isn't converting to a raster (i.e. to a bitmap font).
	   Disable the grid fitting for the convertion to get a proper outlines, 
	   because the viewer resolution is not known during the accumulation.
	   Note we set identity CTM in pdf_text_set_cache for the accumilation,
	   and therefore the font may look too small while the source charproc 
	   interpretation. The document tpc2.ps of the bug 687087 is an example.
	*/
	penum->device_disabled_grid_fitting = true; 
    }

    *ppte = (gs_text_enum_t *)penum;

    return 0;
}

/* ================ Font cache element ================ */

/* GC descriptor */
private_st_pdf_font_cache_elem();

/*
 * Compute id for a font cache element.
 */
static ulong 
pdf_font_cache_elem_id(gs_font *font)
{
#if 0
    /*
     *	For compatibility with Ghostscript rasterizer's
     *	cache logic we use UniqueID to identify fonts.
     *  Note that with buggy documents, which don't
     *	undefine UniqueID redefining a font,
     *	Ghostscript PS interpreter can occasionaly
     *	replace cache elements on insufficient cache size,
     *	taking glyphs from random fonts with random metrics,
     *	therefore the compatibility isn't complete.
     */
    /*
     *	This branch is incompatible with pdf_notify_remove_font.
     */
    if (font->FontType == ft_composite || font->PaintType != 0 ||
	!uid_is_valid(&(((gs_font_base *)font)->UID)))
	return font->id;
    else
	return ((gs_font_base *)font)->UID.id; 
#else
    return font->id;
#endif
}

pdf_font_cache_elem_t **
pdf_locate_font_cache_elem(gx_device_pdf *pdev, gs_font *font)
{
    pdf_font_cache_elem_t **e = &pdev->font_cache;
    long id = pdf_font_cache_elem_id(font);

    for (; *e != 0; e = &(*e)->next)
	if ((*e)->font_id == id) {
	    return e;
	}
    return 0;
}

static void
pdf_remove_font_cache_elem(pdf_font_cache_elem_t *e0)
{
    gx_device_pdf *pdev = e0->pdev;
    pdf_font_cache_elem_t **e = &pdev->font_cache;

    for (; *e != 0; e = &(*e)->next)
	if (*e == e0) {
	    *e = e0->next;
	    gs_free_object(pdev->pdf_memory, e0->glyph_usage, 
				"pdf_remove_font_cache_elem");
	    gs_free_object(pdev->pdf_memory, e0->real_widths, 
				"pdf_remove_font_cache_elem");
	    /* Clean pointers, because gs_free_object below may work idle
	       when this function is called by a garbager notification.
	       Leaving unclean pointers may cause 
	       a further heap validation to fail
	       since the unfreed structure points to freed areas.
	       For instance, e0->next may point to an element, 
	       which is really freed with a subsequent 'restore'.
	       Bug 688837. */
	    e0->next = 0;
	    e0->glyph_usage = 0;
	    e0->real_widths = 0;
	    e0->pdev = 0;
	    gs_free_object(pdev->pdf_memory, e0, 
				"pdf_remove_font_cache_elem");
	    return;
	}
}

static void
font_cache_elem_array_sizes(gx_device_pdf *pdev, gs_font *font,
			    int *num_widths, int *num_chars) 
{
    switch (font->FontType) {
    case ft_composite:
	*num_widths = 0; /* Unused for Type 0 */
	*num_chars = 65536; /* No chance to determine, use max. */
	break;
    case ft_encrypted:
    case ft_encrypted2:
    case ft_user_defined:
    case ft_disk_based:
    case ft_Chameleon:
    case ft_TrueType:
	*num_widths = *num_chars = 256; /* Assuming access to glyph_usage by character codes */
	break;
    case ft_CID_encrypted:
	*num_widths = *num_chars = ((gs_font_cid0 *)font)->cidata.common.CIDCount;
	break;
    case ft_CID_TrueType:
	*num_widths = *num_chars = ((gs_font_cid2 *)font)->cidata.common.CIDCount;
	break;
    default:
	*num_widths = *num_chars = 65536; /* No chance to determine, use max. */
    }
}

static int 
alloc_font_cache_elem_arrays(gx_device_pdf *pdev, pdf_font_cache_elem_t *e,
			     gs_font *font)
{
    int num_widths, num_chars, len;

    font_cache_elem_array_sizes(pdev, font, &num_widths, &num_chars);
    len = (num_chars + 7) / 8;
    e->glyph_usage = gs_alloc_bytes(pdev->pdf_memory, 
			len, "alloc_font_cache_elem_arrays");

    e->real_widths = (num_widths > 0 ? (double *)gs_alloc_bytes(pdev->pdf_memory, 
			num_widths * sizeof(*e->real_widths) *
			    (font->FontType == ft_user_defined ? 2 : 1),
			"alloc_font_cache_elem_arrays") : NULL);
    if (e->glyph_usage == NULL || (num_widths !=0 && e->real_widths == NULL)) {
	gs_free_object(pdev->pdf_memory, e->glyph_usage, 
			    "pdf_attach_font_resource");
	gs_free_object(pdev->pdf_memory, e->real_widths, 
			    "alloc_font_cache_elem_arrays");
	return_error(gs_error_VMerror);
    }
    e->num_chars = num_chars;
    e->num_widths = num_widths;
    memset(e->glyph_usage, 0, len);
    memset(e->real_widths, 0, num_widths * sizeof(*e->real_widths));
    return 0;
}

int
pdf_free_font_cache(gx_device_pdf *pdev)
{
    /* fixme : release elements. */
    pdev->font_cache = NULL;
    return 0;
}


/*
 * Retrive font resource attached to a font,
 * allocating glyph_usage and real_widths on request.
 */
int
pdf_attached_font_resource(gx_device_pdf *pdev, gs_font *font, 
			    pdf_font_resource_t **pdfont, byte **glyph_usage, 
			    double **real_widths, int *num_chars, int *num_widths)
{
    pdf_font_cache_elem_t **e = pdf_locate_font_cache_elem(pdev, font);

    if (e != NULL && (((*e)->glyph_usage == NULL && glyph_usage !=NULL) ||
		      ((*e)->real_widths == NULL && real_widths !=NULL))) {
	int code = alloc_font_cache_elem_arrays(pdev, *e, font);

	if (code < 0)
	    return code;
    }
    *pdfont = (e == NULL ? NULL : (*e)->pdfont);
    if (glyph_usage != NULL)
	*glyph_usage = (e == NULL ? NULL : (*e)->glyph_usage);
    if (real_widths != NULL)
	*real_widths = (e == NULL ? NULL : (*e)->real_widths);
    if (num_chars != NULL)
	*num_chars = (e == NULL ? 0 : (*e)->num_chars);
    if (num_widths != NULL)
	*num_widths = (e == NULL ? 0 : (*e)->num_widths);
    return 0;
}

static int 
pdf_notify_remove_font(void *proc_data, void *event_data)
{   /* gs_font_finalize passes event_data == NULL, so check it here. */
    if (event_data == NULL)
	pdf_remove_font_cache_elem((pdf_font_cache_elem_t *)proc_data);
    return 0;
}

/*
 * Attach font resource to a font.
 */
int
pdf_attach_font_resource(gx_device_pdf *pdev, gs_font *font, 
			 pdf_font_resource_t *pdfont)
{
    int num_chars, num_widths, len;
    pdf_font_cache_elem_t *e, **pe = pdf_locate_font_cache_elem(pdev, font);

    if (pdfont->FontType != font->FontType)
	return_error(gs_error_unregistered); /* Must not happen. */
    font_cache_elem_array_sizes(pdev, font, &num_widths, &num_chars);
    len = (num_chars + 7) / 8;
    if (pe != NULL) {
	e = *pe;
	if (e->pdfont == pdfont)
	    return 0;
	e->pdfont = pdfont;
	/* Reset glyph cache because e->pdfont had changed. */
	memset(e->glyph_usage, 0, len);
	memset(e->real_widths, 0, num_widths * sizeof(*e->real_widths));
    } else {
	int code;
	e = (pdf_font_cache_elem_t *)gs_alloc_struct(pdev->pdf_memory,
		pdf_font_cache_elem_t, &st_pdf_font_cache_elem,
			    "pdf_attach_font_resource");
	if (e == NULL)
	    return_error(gs_error_VMerror);
	e->pdfont = pdfont;
	e->font_id = pdf_font_cache_elem_id(font);
	e->num_chars = 0;
	e->glyph_usage = NULL;
	e->real_widths = NULL;
	e->pdev = pdev;
	e->next = pdev->font_cache;
	pdev->font_cache = e;
	code = gs_notify_register(&font->notify_list, pdf_notify_remove_font, e);
	if (code < 0)
	    return code;
    }
    return 0;
}

/* ================ Process text ================ */

/* ---------------- Internal utilities ---------------- */

/*
 * Compute and return the orig_matrix of a font.
 */
int
pdf_font_orig_matrix(const gs_font *font, gs_matrix *pmat)
{
    switch (font->FontType) {
    case ft_composite:		/* subfonts have their own FontMatrix */
    case ft_TrueType:
    case ft_CID_TrueType:
	/* The TrueType FontMatrix is 1 unit per em, which is what we want. */
	gs_make_identity(pmat);
	return 0;
    case ft_encrypted:
    case ft_encrypted2:
    case ft_CID_encrypted:
    case ft_user_defined:
	/*
         * Type 1 fonts are supposed to use a standard FontMatrix of
         * [0.001 0 0 0.001 0 0], with a 1000-unit cell.  However,
         * Windows NT 4.0 creates Type 1 fonts, apparently derived from
         * TrueType fonts, that use a 2048-unit cell and corresponding
         * FontMatrix.  Also, some PS programs perform font scaling by
         * replacing FontMatrix like this :
         *
         *   /f12 /Times-Roman findfont
         *   copyfont	  % (remove FID)
         *   dup /FontMatrix [0.012 0 0 0.012 0 0] put
         *   definefont
         *   /f12 1 selectfont
         *
         * Such fonts are their own "base font", but the orig_matrix
         * must still be set to 0.001, not 0.012 .
         *
         * The old code used a heuristic to detect and correct for this here.
	 * Unfortunately it doesn't work properly when it meets a font 
	 * with FontMatrix like this : 
	 *
	 *   /FontMatrix [1 2288 div 0 0 1 2288 div 0 0 ] def
	 *
	 * (the bug 686970). Also comparefiles\455690.pdf appears to
	 * have similar problem. Therefore we added a support to lib/gs_fonts.ps,
	 * src/zbfont.c, src/gsfont.c that provides an acces to the original
	 * font via a special key OrigFont added to the font dictionary while definefont.
	 * Now we work through this access with PS interpreter, 
	 * but keep the old heuristic for other clients.
	 */
	{
	    const gs_font *base_font = font;

	    while (base_font->base != base_font)
		base_font = base_font->base;
	    if (font->FontType == ft_user_defined)
		*pmat = base_font->FontMatrix;
	    else if (base_font->orig_FontMatrix.xx != 0 || base_font->orig_FontMatrix.xy != 0 ||
	        base_font->orig_FontMatrix.yx != 0 || base_font->orig_FontMatrix.yy != 0)
		*pmat = base_font->orig_FontMatrix;
	    else {
		/*  Must not happen with PS interpreter. 
		    Provide a hewuristic for other clients.
		*/
		if (base_font->FontMatrix.xx == 1.0/2048 &&
		    base_font->FontMatrix.xy == 0 &&
		    base_font->FontMatrix.yx == 0 &&
		    any_abs(base_font->FontMatrix.yy) == 1.0/2048
		    )
		    *pmat = base_font->FontMatrix;
		else
		    gs_make_scaling(0.001, 0.001, pmat);
	    }
	}
	return 0;
    default:
	return_error(gs_error_rangecheck);
    }
}

/* 
 * Check the Encoding compatibility 
 */
bool
pdf_check_encoding_compatibility(const pdf_font_resource_t *pdfont, 
	    const pdf_char_glyph_pair_t *pairs, int num_chars)
{
    int i;

    for (i = 0; i < num_chars; ++i) {
	gs_char ch = pairs[i].chr;
	pdf_encoding_element_t *pet = &pdfont->u.simple.Encoding[ch];

	if (pairs[i].glyph == pet->glyph)
	    continue;
	if (pet->glyph != GS_NO_GLYPH) /* encoding conflict */
	    return false;
    }
    return true;
}

/* 
 * Check цруерук the Encoding has listed glyphs.
 */
static bool
pdf_check_encoding_has_glyphs(const pdf_font_resource_t *pdfont, 
	    const pdf_char_glyph_pair_t *pairs, int num_chars)
{
    /* This function is pretty slow, but we can't find a better algorithm.
       It works for the case of glyphshow with no proper encoding,
       which we believe comes from poorly designed documents.
     */
    int i, ch;

    for (i = 0; i < num_chars; ++i) {
	for (ch = 0; ch < 256; ch++) {
	    pdf_encoding_element_t *pet = &pdfont->u.simple.Encoding[ch];

	    if (pairs[i].glyph == pet->glyph)
		return true;
	}
    }
    return false;
}


/*
 * Check font resource for encoding compatibility.
 */
static bool
pdf_is_compatible_encoding(gx_device_pdf *pdev, pdf_font_resource_t *pdfont,
			   gs_font *font, const pdf_char_glyph_pair_t *pairs, int num_chars)
{   
    /*
     * This crude version of the code ignores
     * the possibility of re-encoding characters.
     */
    switch (pdfont->FontType) {
    case ft_composite:
	{   /*
	     * We assume that source document don't redefine CMap
	     * resources and that incremental CMaps do not exist.
	     * Therefore we don't maintain stable CMap copies,
	     * but just compare CMap names for equality.
	     * A better implementation should compare the chars->glyphs
	     * translation against the stable copy of CMap,
	     * which to be handled with PDF CMap resource.
	     */
	    gs_font_type0 *pfont = (gs_font_type0 *)font;

	    if (pfont->data.FMapType == fmap_CMap) {
		const gs_cmap_t *pcmap = pfont->data.CMap;
		const gs_const_string *s0 = &pdfont->u.type0.CMapName;
		const gs_const_string *s1 = &pcmap->CMapName;

		return (s0->size == s1->size &&
			!memcmp(s0->data, s1->data, s0->size));
	    }
	}
	return false;
    case ft_user_defined:
	if (pdfont->u.simple.Encoding == NULL)
	    return false; /* Not sure. Happens with 020-01.ps . */
	/* fall through */
    case ft_encrypted:
    case ft_encrypted2:
    case ft_TrueType:
	return pdf_check_encoding_compatibility(pdfont, pairs, num_chars);
    case ft_CID_encrypted:
    case ft_CID_TrueType:
	{
	    gs_font *font1 = (gs_font *)pdf_font_resource_font(pdfont, false);

	    return gs_is_CIDSystemInfo_compatible( 
				gs_font_cid_system_info(font), 
				gs_font_cid_system_info(font1));
	}
    default:
	return false;
    }
}

/*
 * Check whethet the font resource has glyphs.
 */
static bool
pdf_font_has_glyphs(gx_device_pdf *pdev, pdf_font_resource_t *pdfont,
			   gs_font *font, const pdf_char_glyph_pair_t *pairs, int num_chars)
{   
    /*
     * This crude version of the code ignores
     * the possibility of re-encoding characters.
     */
    switch (pdfont->FontType) {
    case ft_composite:
    case ft_user_defined:
	/* Unused case. */
	return false;
    case ft_encrypted:
    case ft_encrypted2:
    case ft_TrueType:
	return pdf_check_encoding_has_glyphs(pdfont, pairs, num_chars);
    case ft_CID_encrypted:
    case ft_CID_TrueType:
	/* Unused case. */
	return false;
    default:
	return false;
    }
}


/* 
 * Find a font resource compatible with a given font. 
 */
static int
pdf_find_font_resource(gx_device_pdf *pdev, gs_font *font,
		       pdf_resource_type_t type,
		       pdf_font_resource_t **ppdfont,
		       pdf_char_glyph_pairs_t *cgp,
		       bool compatible_encoding)
{
    pdf_resource_t **pchain = pdev->resources[type].chains;
    pdf_resource_t *pres;
    int i;
    
    for (i = 0; i < NUM_RESOURCE_CHAINS; i++) {
	for (pres = pchain[i]; pres != 0; pres = pres->next) {
	    pdf_font_resource_t *pdfont = (pdf_font_resource_t *)pres;
	    const gs_font_base *cfont;
	    gs_font *ofont = font;
	    int code;

	    if (font->FontType != pdfont->FontType)
		continue;
	    if (pdfont->FontType == ft_composite) {
		gs_font_type0 *font0 = (gs_font_type0 *)font;

		ofont = font0->data.FDepVector[0]; /* See pdf_make_font_resource. */
		cfont = pdf_font_resource_font(pdfont->u.type0.DescendantFont, false);
		if (font0->data.CMap->WMode != pdfont->u.type0.WMode)
		    continue;
	    } else
		cfont = pdf_font_resource_font(pdfont, false);
	    if (!pdf_is_CID_font(ofont) &&
		(compatible_encoding 
		    ? !pdf_is_compatible_encoding(pdev, pdfont, font, cgp->s, cgp->num_all_chars)
		    : !pdf_font_has_glyphs(pdev, pdfont, font, cgp->s, cgp->num_all_chars)))
		continue;
	    if (cfont == 0)
		continue;
	    code = gs_copied_can_copy_glyphs((const gs_font *)cfont, ofont, 
			    &cgp->s[cgp->unused_offset].glyph, cgp->num_unused_chars, 
			    sizeof(pdf_char_glyph_pair_t), true);
	    if (code == gs_error_unregistered) /* Debug purpose only. */
		return code;
	    if(code > 0) {
		*ppdfont = pdfont;
		return 1;
	    } 
	}
    }
    return 0;
}

/* 
 * Find a type0 font resource for a gived descendent name and CMap name. 
 */
static int
pdf_find_type0_font_resource(gx_device_pdf *pdev, const pdf_font_resource_t *pdsubf, 
	    const gs_const_string *CMapName, uint font_index, pdf_font_resource_t **ppdfont)
{
    pdf_resource_t **pchain = pdev->resources[resourceFont].chains;
    pdf_resource_t *pres;
    int i;
    
    for (i = 0; i < NUM_RESOURCE_CHAINS; i++) {
	for (pres = pchain[i]; pres != 0; pres = pres->next) {
	    pdf_font_resource_t *pdfont = (pdf_font_resource_t *)pres;

	    if (pdfont->FontType != ft_composite)
		continue;
	    if (pdfont->u.type0.DescendantFont != pdsubf)
		continue;
	    if (pdfont->u.type0.font_index != font_index)
		continue;
	    if (pdfont->BaseFont.size != pdsubf->BaseFont.size + CMapName->size + 1)
		continue;
	    if (memcmp(pdfont->BaseFont.data + pdsubf->BaseFont.size + 1, 
			CMapName->data, CMapName->size))
		continue;
	    *ppdfont = pdfont;
	    return 1;
	}
    }
    return 0;
}


static int pdf_make_font_resource(gx_device_pdf *pdev, gs_font *font,
		       pdf_font_resource_t **ppdfont, 
		       pdf_char_glyph_pairs_t *cgp);

/*
 * Create or find a CID font resource object for a glyph set.
 */
int
pdf_obtain_cidfont_resource(gx_device_pdf *pdev, gs_font *subfont, 
			    pdf_font_resource_t **ppdsubf, 
			    pdf_char_glyph_pairs_t *cgp)
{
    int code = 0;

    pdf_attached_font_resource(pdev, subfont, ppdsubf, NULL, NULL, NULL, NULL);
    if (*ppdsubf != NULL) {
	const gs_font_base *cfont = pdf_font_resource_font(*ppdsubf, false);

	code = gs_copied_can_copy_glyphs((const gs_font *)cfont, subfont, 
			&cgp->s[cgp->unused_offset].glyph, cgp->num_unused_chars, 
			sizeof(pdf_char_glyph_pair_t), true);
	if (code > 0)
	    return 0;
	if (code < 0)
	    return code;
	*ppdsubf = NULL;
    }
    code = pdf_find_font_resource(pdev, subfont,
				  resourceCIDFont, ppdsubf, cgp, true);
    if (code < 0)
	return code;
    if (*ppdsubf == NULL) {
	code = pdf_make_font_resource(pdev, subfont, ppdsubf, cgp);
	if (code < 0)
	    return code;
    }
    return pdf_attach_font_resource(pdev, subfont, *ppdsubf);
}

/*
 * Refine index of BaseEncoding.
 */
static int 
pdf_refine_encoding_index(const gx_device_pdf *pdev, int index, bool is_standard)
{
    if (pdev->ForOPDFRead) {
	/*
	* Allow Postscript encodings only.
	*/
	switch (index) {

	    case ENCODING_INDEX_STANDARD: return index;
	    case ENCODING_INDEX_ISOLATIN1: return index;
	    default:
		return ENCODING_INDEX_STANDARD;
	}
    }
    /*
     * Per the PDF 1.3 documentation, there are only 3 BaseEncoding
     * values allowed for non-embedded fonts.  Pick one here.
     */
    switch (index) {
    case ENCODING_INDEX_WINANSI:
    case ENCODING_INDEX_MACROMAN:
    case ENCODING_INDEX_MACEXPERT:
	return index;
    case ENCODING_INDEX_STANDARD:
	if (is_standard)
	    return index;
	/* Falls through. */
    default:
	return ENCODING_INDEX_WINANSI;
    }
}

/*
 * Create a font resource object for a gs_font of Type 3.
 */
int
pdf_make_font3_resource(gx_device_pdf *pdev, gs_font *font,
		       pdf_font_resource_t **ppdfont)
{
    const gs_font_base *bfont = (const gs_font_base *)font;
    pdf_font_resource_t *pdfont;
    byte *cached;
    int code;

    cached = gs_alloc_bytes(pdev->pdf_memory, 256/8, "pdf_make_font3_resource");
    if (cached == NULL)
	return_error(gs_error_VMerror);
    code = font_resource_encoded_alloc(pdev, &pdfont, bfont->id, 
		    ft_user_defined, pdf_write_contents_bitmap);
    if (code < 0) {
	gs_free_object(pdev->pdf_memory, cached, "pdf_make_font3_resource");
	return code;
    }
    memset(cached, 0, 256 / 8);
    pdfont->mark_glyph = font->dir->ccache.mark_glyph; /* For pdf_font_resource_enum_ptrs. */
    pdfont->u.simple.s.type3.bitmap_font = false;
    pdfont->u.simple.BaseEncoding = pdf_refine_encoding_index(pdev,
			bfont->nearest_encoding_index, true);
    pdfont->u.simple.s.type3.char_procs = NULL;
    pdfont->u.simple.s.type3.cached = cached;
    pdfont->u.simple.s.type3.FontBBox.p.x = bfont->FontBBox.p.x;
    pdfont->u.simple.s.type3.FontBBox.p.y = bfont->FontBBox.p.y;
    pdfont->u.simple.s.type3.FontBBox.q.x = bfont->FontBBox.q.x;
    pdfont->u.simple.s.type3.FontBBox.q.y = bfont->FontBBox.q.y;
    pdfont->u.simple.s.type3.FontMatrix = bfont->FontMatrix;
    pdfont->u.simple.s.type3.Resources = cos_dict_alloc(pdev, "pdf_make_font3_resource");
    if (pdfont->u.simple.s.type3.Resources == NULL)
	return_error(gs_error_VMerror);
    /* Adobe viewers have a precision problem with small font matrices : */
    while (any_abs(pdfont->u.simple.s.type3.FontMatrix.xx) < 0.001 &&
	   any_abs(pdfont->u.simple.s.type3.FontMatrix.xy) < 0.001 &&
	   any_abs(pdfont->u.simple.s.type3.FontMatrix.yx) < 0.001 &&
	   any_abs(pdfont->u.simple.s.type3.FontMatrix.yy) < 0.001) {
	pdfont->u.simple.s.type3.FontMatrix.xx *= 10;
	pdfont->u.simple.s.type3.FontMatrix.xy *= 10;
	pdfont->u.simple.s.type3.FontMatrix.yx *= 10;
	pdfont->u.simple.s.type3.FontMatrix.yy *= 10;
    }
    *ppdfont = pdfont;
    return 0;
}

/*
 * Create a font resource object for a gs_font.  Return 1 iff the
 * font was newly created (it's a roudiment, keeping reverse compatibility).
 * This procedure is only intended to be called
 * from a few places in the text code.
 */
static int
pdf_make_font_resource(gx_device_pdf *pdev, gs_font *font,
		       pdf_font_resource_t **ppdfont, 
		       pdf_char_glyph_pairs_t *cgp)
{
    int index = -1;
    int BaseEncoding = ENCODING_INDEX_UNKNOWN;
    pdf_font_embed_t embed;
    pdf_font_descriptor_t *pfd = 0;
    int (*font_alloc)(gx_device_pdf *, pdf_font_resource_t **,
		      gs_id, pdf_font_descriptor_t *);
    gs_font *base_font = font; /* A roudiment from old code. Keep it for a while. */
    pdf_font_resource_t *pdfont;
    pdf_standard_font_t *const psfa =
	pdev->text->outline_fonts->standard_fonts;
    int code = 0;

    if (pdev->version < psdf_version_level2_with_TT) {
	switch(font->FontType) {
	    case ft_TrueType:
	    case ft_CID_TrueType:
		return_error(gs_error_undefined);
	    default:
		break;
	}
    }
    if (pdev->ForOPDFRead && !pdev->HaveCIDSystem) {
	switch(font->FontType) {
	    case ft_CID_encrypted:
	    case ft_CID_TrueType:
		return_error(gs_error_undefined);
	    default:
		break;
	}
    }
    if (!pdev->HaveCFF) {
	if (font->FontType == ft_encrypted2)
	    return_error(gs_error_undefined);
    }
    embed = pdf_font_embed_status(pdev, base_font, &index, cgp->s, cgp->num_all_chars);
    if (pdev->CompatibilityLevel < 1.3)
	if (embed != FONT_EMBED_NO && font->FontType == ft_CID_TrueType)
	    return_error(gs_error_rangecheck);
    if (embed == FONT_EMBED_STANDARD) {
	pdf_standard_font_t *psf = &psfa[index];

	if (psf->pdfont == NULL ||
		!pdf_is_compatible_encoding(pdev, psf->pdfont, font,
			cgp->s, cgp->num_all_chars)) {
	    code = pdf_font_std_alloc(pdev, ppdfont, (psf->pdfont == NULL), base_font->id,
				      (gs_font_base *)base_font, index);
	    if (code < 0)
		return code;
	    if (psf->pdfont == NULL)
		psf->pdfont = *ppdfont;
	    (*ppdfont)->u.simple.BaseEncoding = pdf_refine_encoding_index(pdev,
		((const gs_font_base *)base_font)->nearest_encoding_index, true);
	    code = 1;
	} else
	    *ppdfont = psf->pdfont;
	return code;
    } 

    switch (font->FontType) {
    case ft_CID_encrypted:
    case ft_CID_TrueType:
	font_alloc = pdf_font_cidfont_alloc;
	break;
    case ft_encrypted:
    case ft_encrypted2:
    case ft_TrueType:
	font_alloc = pdf_font_simple_alloc;
	break;
    case ft_user_defined:
	code = pdf_make_font3_resource(pdev, font, ppdfont);
	if (code < 0)
	    return code;
	return 1;
    default:
	return_error(gs_error_invalidfont);
    }

    /* Create an appropriate font resource and descriptor. */
    if (embed == FONT_EMBED_YES) {
	/*
	 * HACK: Acrobat Reader 3 has a bug that makes cmap formats 4
	 * and 6 not work in embedded TrueType fonts.  Consequently, it
	 * can only handle embedded TrueType fonts if all the glyphs
	 * referenced by the Encoding have numbers 0-255.  Check for
	 * this now.
	 */
	if (font->FontType == ft_TrueType &&
	    pdev->CompatibilityLevel <= 1.2 && !pdev->ForOPDFRead
	    ) {
	    int i;

	    for (i = 0; i <= 0xff; ++i) {
		gs_glyph glyph =
		    font->procs.encode_char(font, (gs_char)i,
					    GLYPH_SPACE_INDEX);

		if (glyph == GS_NO_GLYPH ||
		    (glyph >= GS_MIN_GLYPH_INDEX &&
		     glyph <= GS_MIN_GLYPH_INDEX + 0xff)
		    )
		    continue;
		/* Can't embed, punt. */
		return_error(gs_error_rangecheck);
	    }
	}
    }
    if (font->FontType == ft_encrypted || font->FontType == ft_encrypted2 || 
	font->FontType == ft_TrueType) {
        /*
	 * We write True Types with Symbolic flag set.
	 * PDF spec says that "symbolic font should not specify Encoding entry"
	 * (see section 5.5, the article "Encodings for True Type fonts", paragraph 3).
	 * However Acrobat Reader 4,5,6 fail when TT font with no Encoding
	 * appears in a document together with a CID font with a non-standard CMap
	 * (AR 4 and 5 claim "The encoding (CMap) specified by a font is corrupted."
	 * (we read it as "The encoding or CMap specified by a font is corrupted.",
	 * and apply the 1st alternative)). We believe that AR is buggy, 
	 * and therefore we write an Encoding with non-CID True Type fonts.
	 * Hopely other viewers can ignore Encoding in such case. Actually in this case 
	 * an Encoding doesn't add an useful information.
	 */
	BaseEncoding = pdf_refine_encoding_index(pdev,
	    ((const gs_font_base *)base_font)->nearest_encoding_index, false);
    }
    if ((code = pdf_font_descriptor_alloc(pdev, &pfd,
					  (gs_font_base *)base_font,
					  embed == FONT_EMBED_YES)) < 0 ||
	(code = font_alloc(pdev, &pdfont, base_font->id, pfd)) < 0
	)
	return code;
    code = 1;

    if (!pdf_is_CID_font(font)) {
	pdfont->u.simple.BaseEncoding = BaseEncoding;
	pdfont->mark_glyph = font->dir->ccache.mark_glyph;
    }

    if (pdev->PDFA && font->FontType == ft_TrueType) {
	/* The Adobe preflight tool for PDF/A
	   checks whether Widht or W include elements
	   for all characters in the True Type font.
	   Due to that we need to provide a width
	   for .notdef glyph.
	   (It's a part of the bug 688790).
	 */
	gs_font_base *cfont = pdf_font_descriptor_font(pfd, false/*any*/);
	gs_glyph notdef_glyph = copied_get_notdef((const gs_font *)cfont);
	pdf_glyph_widths_t widths;
	double cdevproc_result[10] = {0,0,0,0,0, 0,0,0,0,0};
	double *w, *v, *w0;

	if (notdef_glyph != GS_NO_GLYPH) {
	    code = pdf_obtain_cidfont_widths_arrays(pdev, pdfont, font->WMode, &w, &w0, &v);
	    if (code < 0)
		return code;
	    widths.Width.w = 0;
	    code = pdf_glyph_widths(pdfont, font->WMode, notdef_glyph,
		 font, &widths, cdevproc_result);
	    w[0] = widths.Width.w;
	    pdfont->used[0] |= 0x80;
	}
    }
    *ppdfont = pdfont;
    return 1;
}

/*
 * Check for simple font.
 */
bool 
pdf_is_simple_font(gs_font *font)
{ 
    return (font->FontType == ft_encrypted ||
	    font->FontType == ft_encrypted2 ||
	    font->FontType == ft_TrueType ||
	    font->FontType == ft_user_defined);
}

/*
 * Check for CID font.
 */
bool 
pdf_is_CID_font(gs_font *font)
{ 
    return (font->FontType == ft_CID_encrypted ||
	    font->FontType == ft_CID_TrueType);
}

/*
 * Enumerate glyphs for a text.
 */
static int
pdf_next_char_glyph(gs_text_enum_t *penum, const gs_string *pstr, 
	       /* const */ gs_font *font, bool font_is_simple, 
	       gs_char *char_code, gs_char *cid, gs_glyph *glyph)
{
    int code = font->procs.next_char_glyph(penum, char_code, glyph);

    if (code == 2)		/* end of string */
	return code;
    if (code < 0)
	return code;
    if (font_is_simple) {
	*cid = *char_code;
	*glyph = font->procs.encode_char(font, *char_code, GLYPH_SPACE_NAME);
	if (*glyph == GS_NO_GLYPH)
	    return 3;
    } else {
	if (*glyph < GS_MIN_CID_GLYPH)
	    return 3; /* Not sure why, copied from scan_cmap_text. */
	*cid = *glyph - GS_MIN_CID_GLYPH; /* CID */
    }
    return 0;
}

static void
store_glyphs(pdf_char_glyph_pairs_t *cgp, 
	     byte *glyph_usage, int char_cache_size,
	     gs_char char_code, gs_char cid, gs_glyph glyph)
{
    int j;

    for (j = 0; j < cgp->num_all_chars; j++)
	if (cgp->s[j].chr == cid)
	    break;
    if (j < cgp->num_all_chars)
	return;
    cgp->s[cgp->num_all_chars].glyph = glyph;
    cgp->s[cgp->num_all_chars].chr = char_code;
    cgp->num_all_chars++;
    if (glyph_usage == 0 || !(glyph_usage[cid / 8] & (0x80 >> (cid & 7)))) {
	cgp->s[cgp->unused_offset + cgp->num_unused_chars].glyph = glyph;
    	cgp->s[cgp->unused_offset + cgp->num_unused_chars].chr = char_code;
	cgp->num_unused_chars++;
    }
    /* We are disliked that gs_copied_can_copy_glyphs can get redundant
     * glyphs, if Encoding specifies several codes for same glyph. 
     * But we need the positional correspondence
     * of glyphs to codes for pdf_is_compatible_encoding.
     * Redundant glyphs isn't a big payment for it
     * because they happen seldom.
     */
}

static gs_char
pdf_new_char_code_in_pdfont(pdf_char_glyph_pairs_t *cgp, gs_glyph glyph, int *last_reserved_char)
{   /* Returns 256 if encoding overflows. */
    int j, ch;

    for (j = 0; j < cgp->num_all_chars; j++)
	if (cgp->s[j].glyph == glyph)
	    break;
    if (j < cgp->num_all_chars)
	return cgp->s[j].chr;
    ch = ++*last_reserved_char;
    cgp->s[cgp->num_all_chars].glyph = glyph;
    cgp->s[cgp->num_all_chars].chr = ch;
    cgp->num_all_chars++;
    cgp->s[cgp->unused_offset + cgp->num_unused_chars].glyph = glyph;
    cgp->s[cgp->unused_offset + cgp->num_unused_chars].chr = ch;
    cgp->num_unused_chars++;
    return ch;
}

static gs_char
pdf_reserve_char_code_in_pdfont(pdf_font_resource_t *pdfont, pdf_char_glyph_pairs_t *cgp, gs_glyph glyph, 
				int *last_reserved_char)
{   /* Returns 256 if encoding overflows. */
    /* This function is pretty slow, but we can't find a better algorithm.
       It works for the case of glyphshow with no proper encoding,
       which we believe comes from poorly designed documents.
     */
    int j, ch;

    for (j = 0; j < cgp->num_all_chars; j++)
	if (cgp->s[j].glyph == glyph)
	    break;
    if (j < cgp->num_all_chars)
	return cgp->s[j].chr;

    for (ch = 0; ch < 256; ch++) {
	pdf_encoding_element_t *pet = &pdfont->u.simple.Encoding[ch];

	if (glyph == pet->glyph)
	    return ch;
    }
    /* If the font has a known encoding, prefer .notdef codes. */
    if (pdfont->u.simple.preferred_encoding_index != -1) {
	const ushort *enc = gs_c_known_encodings[pdfont->u.simple.preferred_encoding_index];

	if (standard_glyph_code_for_notdef == GS_NO_GLYPH)
	    standard_glyph_code_for_notdef = 
		    gs_c_name_glyph((const byte *)".notdef", 7) - gs_c_min_std_encoding_glyph;
	for (ch = *last_reserved_char + 1; ch < 256; ch++) {
	    pdf_encoding_element_t *pet = &pdfont->u.simple.Encoding[ch];

	    if (pet->glyph == GS_NO_GLYPH && enc[ch] == standard_glyph_code_for_notdef) {
		*last_reserved_char = ch;
		break;
	    }
	}
    }
    /* Otherwise use any code unused in the font. */
    if (ch > 255) {
	ch = *last_reserved_char + 1;
	for (; ch < 255; ch++) {
	    pdf_encoding_element_t *pet = &pdfont->u.simple.Encoding[ch];

	    if (pet->glyph == GS_NO_GLYPH)
		break;
	}
	*last_reserved_char = ch;
    }
    cgp->s[cgp->num_all_chars].glyph = glyph;
    cgp->s[cgp->num_all_chars].chr = ch;
    cgp->num_all_chars++;
    cgp->s[cgp->unused_offset + cgp->num_unused_chars].glyph = glyph;
    cgp->s[cgp->unused_offset + cgp->num_unused_chars].chr = ch;
    cgp->num_unused_chars++;
    return ch;
}


/* Allocate storage for the glyph set of the text. */
static int
pdf_alloc_text_glyphs_table(gx_device_pdf *pdev, pdf_text_enum_t *penum, const gs_string *pstr)
{
    const int go = (pstr != NULL ? pstr->size : penum->text.size);
    const int struct_size = sizeof(pdf_char_glyph_pairs_t) + 
			    sizeof(pdf_char_glyph_pair_t) * (2 * go - 1);
    pdf_char_glyph_pairs_t *cgp = (pdf_char_glyph_pairs_t *)gs_alloc_bytes(penum->memory, 
		struct_size, "pdf_alloc_text_glyphs_table");
    if (cgp == NULL)
	return_error(gs_error_VMerror);
    penum->cgp = cgp;
    cgp->unused_offset = go;
    cgp->num_all_chars = 0;
    cgp->num_unused_chars = 0;
    return 0;
}

/* Build the glyph set of the text. */
static int
pdf_make_text_glyphs_table(pdf_text_enum_t *penum, const gs_string *pstr, 
		byte *glyph_usage, int char_cache_size)
{
    gs_text_enum_t scan = *(gs_text_enum_t *)penum;
    gs_font *font = (gs_font *)penum->current_font;
    bool font_is_simple = pdf_is_simple_font(font);
    pdf_char_glyph_pairs_t *cgp = penum->cgp;
    gs_char char_code, cid;
    gs_glyph glyph;
    int code;

    cgp->num_unused_chars = 0;
    cgp->num_all_chars = 0;
    if (pstr != NULL) {
	scan.text.data.bytes = pstr->data;
	scan.text.size = pstr->size;
	scan.index = 0;
        /* if TEXT_FROM_CHARS the data was converted to bytes earlier */
        if ( scan.text.operation & TEXT_FROM_CHARS )
            scan.text.operation = ((scan.text.operation & ~TEXT_FROM_CHARS) | TEXT_FROM_STRING);
    }
    for (;;) {
	code = pdf_next_char_glyph(&scan, pstr, font, font_is_simple, 
				   &char_code, &cid, &glyph);
	if (code == 2)		/* end of string */
	    break;
	if (code == 3)		/* no glyph */
	    continue;
	if (code < 0)
	    return code;
	if (cgp->num_all_chars > cgp->unused_offset)
	    return_error(gs_error_unregistered); /* Must not happen. */
	if (glyph_usage != 0 && cid > char_cache_size)
	    continue;
	store_glyphs(cgp, glyph_usage, char_cache_size,
		     char_code, cid, glyph);
    }
    return 0;
}

/* Build the glyph set of the glyphshow text, and re_encode the text. */
static int
pdf_make_text_glyphs_table_unencoded(gx_device_pdf *pdev, pdf_char_glyph_pairs_t *cgp,
		gs_font *font, const gs_string *pstr, const gs_glyph *gdata,
		int *ps_encoding_index) 
{
    int i, j, code;
    gs_char ch;
    gs_const_string gname;
    gs_glyph *gid = (gs_glyph *)pstr->data; /* pdf_text_process allocs enough space. */
    gs_font_base *bfont;
    bool unknown = false;
    pdf_font_resource_t *pdfont;
    int ei, start_ei = -1;

    code = pdf_attached_font_resource(pdev, font, &pdfont, NULL, NULL, NULL, NULL);
    if (code < 0)
	return code;
    /* Translate glyph name indices into gscencs.c indices. */
    for (i = 0; i < pstr->size; i++) {
	int code = font->procs.glyph_name(font, gdata[i], &gname);

	if (code < 0)
	    return code;
	gid[i] = gs_c_name_glyph(gname.data, gname.size);
	if (gid[i] == GS_NO_GLYPH) {
	    /* Use global glyph name. */
	    /* Assuming this can't fail in a middle of a text,
	       because TEXT_FROM_GLYPHS never works for Postscript. */
	    gid[i] = gdata[i];
	    unknown = true;
	}
    }
do_unknown:
    if (unknown) {
	int last_reserved_char = -1;
	 /* Using global glyph names. */

	/* Try to find an existing font resource, which has necessary glyphs. 
	   Doing so to prevent creating multiple font copies.
	 */
	cgp->num_unused_chars = 0;
	cgp->num_all_chars = 0;
	for (i = 0; i < pstr->size; i++) {
	    /* Temporary stub gid instead cid and char_code : */
	    store_glyphs(cgp, NULL, 0, gdata[i], gdata[i], gdata[i]);
	}
	code = pdf_find_font_resource(pdev, font, resourceFont, &pdfont, cgp, false);
	if (code < 0)
	    return code;
	if (code) {
	    /* Found one - make it be current. */
    	    code = pdf_attach_font_resource(pdev, font, pdfont);
	    if (code < 0)
		return code;
	}
	/* Try to add glyphs to the current font resource. . */
	cgp->num_unused_chars = 0;
	cgp->num_all_chars = 0;

	if(pdfont != NULL)
		last_reserved_char = pdfont->u.simple.last_reserved_char;

	for (i = 0; i < pstr->size; i++) {
		
	    if (pdfont == NULL)
		ch = 256; /* Force new encoding. */
	    else
		ch = pdf_reserve_char_code_in_pdfont(pdfont, cgp, gdata[i], &last_reserved_char);
	    if (ch > 255) {
		if(pdfont != NULL)
			last_reserved_char = pdfont->u.simple.last_reserved_char;
		/* Start a new font/encoding. */
		last_reserved_char = -1;

		cgp->num_unused_chars = 0;
		cgp->num_all_chars = 0;
		for (i = 0; i < pstr->size; i++) {
		    ch = pdf_new_char_code_in_pdfont(cgp, gdata[i], &last_reserved_char);
		    if (ch > 255) {
			/* More than 255 unknown characters in a text.
			   It must not happen because TEXT_FROM_GLYPHS
			   never works for Postscript. */
			return_error(gs_error_unregistered);
		    }
		}
	}
    }
	if (pdfont != NULL)
	    pdfont->u.simple.last_reserved_char = last_reserved_char;
	/* Change glyphs to char codes in the text : */
	for (i = 0; i < pstr->size; i++) {
	    /* Picked up by Coverity, if pdfont is NULL then the call would dereference it */
	    if (pdfont != NULL) {
		/* A trick : pdf_reserve_char_code_in_pdfont here simply encodes with cgp. */
		ch = pdf_reserve_char_code_in_pdfont(pdfont, cgp, gdata[i], &pdfont->u.simple.last_reserved_char);
		pstr->data[i] = ch;
	    }
	}
	return 0;
    }
    /* Now we know it's a base font, bcause it has glyph names. */
    bfont = (gs_font_base *)font;
    if (start_ei < 0)
	start_ei = bfont->nearest_encoding_index;
    if (start_ei < 0)
	start_ei = 0;
    /* Find an acceptable encodng, starting from start_ei. 
       We need a conservative search to minimize the probability 
       of encoding conflicts.
     */
    for (j = 0, ei = start_ei; gs_c_known_encodings[j]; j++, ei++) {
	if (!gs_c_known_encodings[ei])
	    ei = 0;
	/* Restrict with PDF encodings, because others give frequent conflicts. */
	if (ei > 5) /* Hack : gscedata.c must provide a constant. */
	    continue;
	cgp->num_unused_chars = 0;
	cgp->num_all_chars = 0;
	for (i = 0; i < pstr->size; i++) {
	    ch = gs_c_decode(gid[i], ei);
	    if (ch == GS_NO_CHAR)
		break;
	    if (ch > 255)
		break; /* MacGlyphEncoding defines extra glyphs. */
	    /* pstr->data[i] = (byte)ch; Can't do because pstr->data and gid 
	       are same pointer. Will do in a separate pass below. */
	    store_glyphs(cgp, NULL, 0, ch, ch, gdata[i]);
	}
	*ps_encoding_index = ei;
	if (i == pstr->size) {
	    /* Change glyphs to char codes in the text : */
	    for (i = 0; i < pstr->size; i++)
		pstr->data[i] = (byte)gs_c_decode(gid[i], ei);
	    return 0;
	}
    }
    unknown = true;
    goto do_unknown;
}


/* Get/make font resource for the font with a known encoding. */
static int
pdf_obtain_font_resource_encoded(gx_device_pdf *pdev, gs_font *font,
	pdf_font_resource_t **ppdfont, pdf_char_glyph_pairs_t *cgp)
{
    int code;
    pdf_font_resource_t *pdfont_not_allowed = NULL;

    if (*ppdfont != 0) {
	gs_font_base *cfont = pdf_font_resource_font(*ppdfont, false);
        
	if (font->FontType != ft_user_defined) {
	    code = gs_copied_can_copy_glyphs((gs_font *)cfont, font, 
			&cgp->s[cgp->unused_offset].glyph, cgp->num_unused_chars, 
			sizeof(pdf_char_glyph_pair_t), true);
	    if (code < 0)
		return code;
	} else
	    code = 1;
	if (code == 0) {
	    pdfont_not_allowed = *ppdfont;
	    *ppdfont = 0;
	} else if(!pdf_is_compatible_encoding(pdev, *ppdfont, font,
			cgp->s, cgp->num_all_chars)) {
	    pdfont_not_allowed = *ppdfont;
	    *ppdfont = 0;
	}
    }
    if (*ppdfont == 0) {
	gs_font *base_font = font;
	gs_font *below;
	bool same_encoding = true;

	/* 
	 * Find the "lowest" base font that has the same outlines.
	 * We use its FontName for font resource. 
	 */
	while ((below = base_font->base) != base_font &&
	       base_font->procs.same_font(base_font, below, FONT_SAME_OUTLINES))
	    base_font = below;
	if (base_font != font)
	    same_encoding = ((base_font->procs.same_font(base_font, font, 
	                      FONT_SAME_ENCODING) & FONT_SAME_ENCODING) != 0);
	/* Find or make font resource. */
	pdf_attached_font_resource(pdev, base_font, ppdfont, NULL, NULL, NULL, NULL);
	if (*ppdfont != NULL && base_font != font) {
	    if (pdfont_not_allowed == *ppdfont)
		*ppdfont = NULL;	
	    else if(!pdf_is_compatible_encoding(pdev, *ppdfont, 
				    base_font, cgp->s, cgp->num_all_chars))
		*ppdfont = NULL;
	}
	if (*ppdfont == NULL || *ppdfont == pdfont_not_allowed) {
	    pdf_resource_type_t type = 
		(pdf_is_CID_font(base_font) ? resourceCIDFont 
					    : resourceFont);
	    *ppdfont = NULL;
    	    code = pdf_find_font_resource(pdev, base_font, type, ppdfont, cgp, true);
	    if (code < 0)
		return code;
	    if (*ppdfont == NULL) {
		code = pdf_make_font_resource(pdev, base_font, ppdfont, cgp);
		if (code < 0)
		    return code;
	    }
	    if (base_font != font && same_encoding) {
		code = pdf_attach_font_resource(pdev, base_font, *ppdfont);
		if (code < 0) 		    
		    return code;
	    }
	}
	code = pdf_attach_font_resource(pdev, font, *ppdfont);
	if (code < 0)
	    return code;
    }
    return 0;
}

/* Mark glyphs used in the text with the font resource. */
static int
pdf_mark_text_glyphs(const gs_text_enum_t *penum, const gs_string *pstr,
	    byte *glyph_usage, int char_cache_size)
{
    gs_text_enum_t scan = *penum;
    gs_font *font = (gs_font *)penum->current_font;
    bool font_is_simple = pdf_is_simple_font(font);
    gs_char char_code, cid;
    gs_glyph glyph;

    if (glyph_usage == NULL)
	return 0;

    if (pstr != NULL) {
	scan.text.data.bytes = pstr->data;
	scan.text.size = pstr->size;
	scan.index = 0;
        /* if TEXT_FROM_CHARS the data was converted to bytes earlier */
        if ( scan.text.operation & TEXT_FROM_CHARS )
            scan.text.operation = 
                ((scan.text.operation & ~TEXT_FROM_CHARS) | TEXT_FROM_STRING);
    }
    for (;;) {
	int code = pdf_next_char_glyph(&scan, pstr, font, font_is_simple, 
				       &char_code, &cid, &glyph);

	if (code == 2)		/* end of string */
	    break;
	if (code == 3)		/* no glyph */
	    continue;
	if (code < 0)
	    return code;
	if (cid >= char_cache_size)
	    continue;
	glyph_usage[cid / 8] |= 0x80 >> (cid & 7);
    }
    return 0;
}

/* Mark glyphs used in the glyphshow text with the font resource. */
static int
pdf_mark_text_glyphs_unencoded(const gs_text_enum_t *penum, const gs_string *pstr,
	    byte *glyph_usage, int char_cache_size)
{
    int i;

    for(i = 0; i < pstr->size; i++) {
	byte ch = pstr->data[i];

	if (ch >= char_cache_size)
	    return_error(gs_error_rangecheck);
	glyph_usage[ch / 8] |= 0x80 >> (ch & 7);
    }
    return 0;
}

/*
 * Create or find a font resource object for a text.
 */
int
pdf_obtain_font_resource(pdf_text_enum_t *penum, 
	    const gs_string *pstr, pdf_font_resource_t **ppdfont)
{
    gx_device_pdf *pdev = (gx_device_pdf *)penum->dev;
    gs_font *font = (gs_font *)penum->current_font;
    byte *glyph_usage = 0;
    double *real_widths;
    int char_cache_size, width_cache_size;
    int code;

    if (font->FontType == ft_composite) {
	/* Must not happen, because we always split composite fonts into descendents. */
	return_error(gs_error_unregistered);
    }
    code = pdf_attached_font_resource(pdev, font, ppdfont,
			       &glyph_usage, &real_widths, &char_cache_size, &width_cache_size);
    /* *ppdfont is NULL if no resource attached. */
    if (code < 0)
	return code;
    if (penum->cgp == NULL) {
	code = pdf_alloc_text_glyphs_table(pdev, penum, pstr);
	if (code < 0)
	    return code;
	code = pdf_make_text_glyphs_table(penum, pstr,
			    glyph_usage, char_cache_size);
	if (code < 0)
	    return code;
    }
    code = pdf_obtain_font_resource_encoded(pdev, font, ppdfont, penum->cgp);
    if (code < 0)
	return code;
    code = pdf_attached_font_resource(pdev, font, ppdfont, 
			       &glyph_usage, &real_widths, &char_cache_size, &width_cache_size);
    if (code < 0)
	return code;
    return pdf_mark_text_glyphs((const gs_text_enum_t *)penum, pstr, glyph_usage, char_cache_size);
}

/*
 * Create or find a font resource object for a glyphshow text.
 */
int
pdf_obtain_font_resource_unencoded(pdf_text_enum_t *penum, 
	    const gs_string *pstr, pdf_font_resource_t **ppdfont, const gs_glyph *gdata)
{
    gx_device_pdf *pdev = (gx_device_pdf *)penum->dev;
    gs_font *font = (gs_font *)penum->current_font;
    byte *glyph_usage = 0;
    double *real_widths = 0;
    int char_cache_size = 0, width_cache_size = 0;
    int code, ps_encoding_index = -1;

    if (font->FontType == ft_composite) {
	/* Must not happen, because we always split composite fonts into descendents. */
	return_error(gs_error_unregistered);
    }
    code = pdf_attached_font_resource(pdev, font, ppdfont,
			       &glyph_usage, &real_widths, &char_cache_size, &width_cache_size);
    if (code < 0)
	return code;
    /* *ppdfont is NULL if no resource attached. */
    if (*ppdfont != NULL)
	ps_encoding_index = (*ppdfont)->u.simple.preferred_encoding_index;
    if (penum->cgp == NULL) {
	code = pdf_alloc_text_glyphs_table(pdev, penum, pstr);
	if (code < 0)
	    return code;
	code = pdf_make_text_glyphs_table_unencoded(pdev, penum->cgp, font, pstr, gdata, &ps_encoding_index);
	if (code < 0)
	    return code;
    }
    code = pdf_obtain_font_resource_encoded(pdev, font, ppdfont, penum->cgp);
    if (code < 0)
	return code;
    code = pdf_attached_font_resource(pdev, font, ppdfont, 
			       &glyph_usage, &real_widths, &char_cache_size, &width_cache_size);
    if (code < 0)
	return code;
    (*ppdfont)->u.simple.preferred_encoding_index = ps_encoding_index;
    return pdf_mark_text_glyphs_unencoded((const gs_text_enum_t *)penum, 
		    pstr, glyph_usage, char_cache_size);
}

static inline bool
strings_equal(const gs_const_string *s1, const gs_const_string *s2)
{
    return s1->size == s2->size &&
	    !memcmp(s1->data, s2->data, s1->size);
}

/*
 * Create or find a parent Type 0 font resource object for a CID font resource.
 */
int
pdf_obtain_parent_type0_font_resource(gx_device_pdf *pdev, pdf_font_resource_t *pdsubf, 
		uint font_index, const gs_const_string *CMapName, pdf_font_resource_t **pdfont)
{
    if (pdsubf->u.cidfont.parent != 0 &&
	    font_index == pdsubf->u.cidfont.parent->u.type0.font_index &&
	    strings_equal(CMapName, &pdsubf->u.cidfont.parent->u.type0.CMapName))
	*pdfont = pdsubf->u.cidfont.parent;
    else {
	/*
	 * PDF spec 1.4 section 5.6 "Composite Fonts" says :
	 *
	 * PDF 1.2 introduces a general architecture for composite fonts that theoretically
	 * allows a Type 0 font to have multiple descendants,which might themselves be
	 * Type 0 fonts.However,in versions up to and including PDF 1.4,only a single
	 * descendant is allowed,which must be a CIDFont (not a font).This restriction
	 * may be relaxed in a future PDF version.
	 */

	if (pdsubf->u.cidfont.parent == NULL || 
		pdf_find_type0_font_resource(pdev, pdsubf, CMapName, font_index, pdfont) <= 0) {
	    int code = pdf_font_type0_alloc(pdev, pdfont, gs_no_id, pdsubf, CMapName);

	    if (code < 0)
		return code;
	    (*pdfont)->u.type0.font_index = font_index;
	}
	pdsubf->u.cidfont.parent = *pdfont;
    }
    return 0;
}

gs_char
pdf_find_glyph(pdf_font_resource_t *pdfont, gs_glyph glyph)
{
    if (pdfont->FontType != ft_user_defined)
	return GS_NO_CHAR;
    else {
	pdf_encoding_element_t *pet = pdfont->u.simple.Encoding;
	int i, i0 = -1;
	
	if (pdfont->u.simple.FirstChar > pdfont->u.simple.LastChar)
	    return (gs_char)0;
	for (i = pdfont->u.simple.FirstChar; i <= pdfont->u.simple.LastChar; i++, pet++) {
	    if (pet->glyph == glyph)
		return (gs_char)i;
	    if (i0 == -1 && pet->glyph == GS_NO_GLYPH)
		i0 = i;
	}
	if (i0 != -1)
	    return (gs_char)i0;
	if (i < 256)
	    return (gs_char)i;
	return GS_NO_CHAR;	
    }
}

/*
 * Compute the cached values in the text processing state from the text
 * parameters, current_font, and pis->ctm.  Return either an error code (<
 * 0) or a mask of operation attributes that the caller must emulate.
 * Currently the only such attributes are TEXT_ADD_TO_ALL_WIDTHS and
 * TEXT_ADD_TO_SPACE_WIDTH.  Note that this procedure fills in all the
 * values in ppts->values, not just the ones that need to be set now.
 */
static int
transform_delta_inverse(const gs_point *pdelta, const gs_matrix *pmat,
			gs_point *ppt)
{
    int code = gs_distance_transform_inverse(pdelta->x, pdelta->y, pmat, ppt);
    gs_point delta;

    if (code < 0)
	return code;
    if (ppt->y == 0)
	return 0;
    /* Check for numerical fuzz. */
    code = gs_distance_transform(ppt->x, 0.0, pmat, &delta);
    if (code < 0)
	return 0;		/* punt */
    if (fabs(delta.x - pdelta->x) < 0.01 && fabs(delta.y - pdelta->y) < 0.01) {
	/* Close enough to y == 0: device space error < 0.01 pixel. */
	ppt->y = 0;
    }
    return 0;
}

float pdf_calculate_text_size(gs_imager_state *pis, pdf_font_resource_t *pdfont, 
			      const gs_matrix *pfmat, gs_matrix *smat, gs_matrix *tmat,
			      gs_font *font, gx_device_pdf *pdev)
{
    gs_matrix orig_matrix;
    double
	sx = pdev->HWResolution[0] / 72.0,
	sy = pdev->HWResolution[1] / 72.0;
    float size;


    /* Get the original matrix of the base font. */

    {
	gs_font_base *cfont = pdf_font_resource_font(pdfont, false);

	if (pdfont->FontType == ft_user_defined)
	    orig_matrix = pdfont->u.simple.s.type3.FontMatrix;
	else if (cfont != 0) {
	    /*
	     * The text matrix to be computed relatively to the 
	     * embedded font matrix.
	     */
	    orig_matrix = cfont->FontMatrix;
	} else {
	    /*
	     * We don't embed the font.
	     * The text matrix to be computed relatively to
	     * standard font matrix.
	     */
	    pdf_font_orig_matrix(font, &orig_matrix);
	}
    }

    /* Compute the scaling matrix and combined matrix. */

    gs_matrix_invert(&orig_matrix, smat);
    gs_matrix_multiply(smat, pfmat, smat);
    *tmat = ctm_only(pis);
    tmat->tx = tmat->ty = 0;
    gs_matrix_multiply(smat, tmat, tmat);

    /* Try to find a reasonable size value.  This isn't necessary, */
    /* but it's worth a little effort. */

    size = hypot(tmat->yx, tmat->yy) / sy;
    if (size < 0.01)
	size = hypot(tmat->xx, tmat->xy) / sx;
    if (size < 0.01)
	size = 1;

    return(size);
}

int
pdf_update_text_state(pdf_text_process_state_t *ppts,
		      const pdf_text_enum_t *penum,
		      pdf_font_resource_t *pdfont, const gs_matrix *pfmat)
{
    gx_device_pdf *const pdev = (gx_device_pdf *)penum->dev;
    gs_font *font = penum->current_font;
    gs_fixed_point cpt;
    gs_matrix smat, tmat;
    float size;
    float c_s = 0, w_s = 0;
    int mask = 0;
    int code = gx_path_current_point(penum->path, &cpt);

    if (code < 0)
	return code;


    size = pdf_calculate_text_size(penum->pis, pdfont, pfmat, &smat, &tmat, penum->current_font, pdev);
    /* Check for spacing parameters we can handle, and transform them. */

    if (penum->text.operation & TEXT_ADD_TO_ALL_WIDTHS) {
	if (penum->current_font->WMode == 0) {
	    gs_point pt;

	    code = transform_delta_inverse(&penum->text.delta_all, &smat, &pt);
	    if (code >= 0 && pt.y == 0)
		c_s = pt.x * size;
	    else
		mask |= TEXT_ADD_TO_ALL_WIDTHS;
	}
	else
	    mask |= TEXT_ADD_TO_ALL_WIDTHS;
    }
    
    if (penum->text.operation & TEXT_ADD_TO_SPACE_WIDTH) {
	gs_point pt;

	code = transform_delta_inverse(&penum->text.delta_space, &smat, &pt);
	if (code >= 0 && pt.y == 0 && penum->text.space.s_char == 32)
	    w_s = pt.x * size;
	else
	    mask |= TEXT_ADD_TO_SPACE_WIDTH;
    }
    /* Store the updated values. */

    tmat.xx /= size;
    tmat.xy /= size;
    tmat.yx /= size;
    tmat.yy /= size;
    tmat.tx += fixed2float(cpt.x);
    tmat.ty += fixed2float(cpt.y);

    ppts->values.character_spacing = c_s;
    ppts->values.pdfont = pdfont;
    ppts->values.size = size;
    ppts->values.matrix = tmat;
    ppts->values.render_mode = penum->pis->text_rendering_mode;
    ppts->values.word_spacing = w_s;
    ppts->font = font;

    if (font->PaintType == 2 && penum->pis->text_rendering_mode == 0)
    {
	gs_imager_state *pis = penum->pis;
	gs_font *font = penum->current_font;
	double scaled_width = font->StrokeWidth != 0 ? font->StrokeWidth : 0.001;
	double saved_width = pis->line_params.half_width;
	/*
	 * See stream_to_text in gdevpdfu.c re the computation of
	 * the scaling value.
	 */
	double scale = 72.0 / pdev->HWResolution[1];
	
	if (font->FontMatrix.yy != 0)
	    scaled_width *= fabs(font->orig_FontMatrix.yy) * size * tmat.yy * scale;
	else
	    scaled_width *= fabs(font->orig_FontMatrix.xy) * size * tmat.xy * scale;

	ppts->values.render_mode = 1;	

	/* Sort out any pending glyphs */
	code = pdf_set_PaintType0_params(pdev, pis, size, scaled_width, &ppts->values);

	pis->line_params.half_width = scaled_width / 2;
	code = pdf_set_text_process_state(pdev, (const gs_text_enum_t *)penum,
				      ppts);

	pis->line_params.half_width = saved_width;
    } else {
	code = pdf_set_text_process_state(pdev, (const gs_text_enum_t *)penum,
				      ppts);
    }
    return (code < 0 ? code : mask);
}

/*
 * Set up commands to make the output state match the processing state.
 * General graphics state commands are written now; text state commands
 * are written later.
 */
int
pdf_set_text_process_state(gx_device_pdf *pdev,
			   const gs_text_enum_t *pte,	/* for pdcolor, pis */
			   pdf_text_process_state_t *ppts)
{
    /*
     * Setting the stroke parameters may exit text mode, causing the
     * settings of the text parameters to be lost.  Therefore, we set the
     * stroke parameters first.
     */
    if (pdf_render_mode_uses_stroke(pdev, &ppts->values)) {
	/* Write all the parameters for stroking. */
	gs_imager_state *pis = pte->pis;
	float save_width = pis->line_params.half_width;
	int code;

	code = pdf_open_contents(pdev, PDF_IN_STRING);
	code = pdf_prepare_stroke(pdev, pis);
	if (code >= 0) 
	    code = gdev_vector_prepare_stroke((gx_device_vector *)pdev,
					      pis, NULL, NULL, 1);
	pis->line_params.half_width = save_width;
	if (code < 0)
	    return code;
    }

    /* Now set all the other parameters. */

    return pdf_set_text_state_values(pdev, &ppts->values);
}

static int
store_glyph_width(pdf_glyph_width_t *pwidth, int wmode, const gs_matrix *scale,
		  const gs_glyph_info_t *pinfo)
{
    double w, v;

    gs_distance_transform(pinfo->width[wmode].x, pinfo->width[wmode].y, scale, &pwidth->xy);
    if (wmode)
	w = pwidth->xy.y, v = pwidth->xy.x;
    else
	w = pwidth->xy.x, v = pwidth->xy.y;
    if (v != 0)
	return 1;
    pwidth->w = w;
    gs_distance_transform(pinfo->v.x, pinfo->v.y, scale, &pwidth->v);
    return 0;
}

static int
get_missing_width(gs_font_base *cfont, int wmode, const gs_matrix *scale_c,
		    pdf_glyph_widths_t *pwidths)
{
    gs_font_info_t finfo;
    int code;

    code = cfont->procs.font_info((gs_font *)cfont, NULL,
				  FONT_INFO_MISSING_WIDTH, &finfo);
    if (code < 0)
	return code;
    if (wmode) {
	gs_distance_transform(0.0, -finfo.MissingWidth, scale_c, &pwidths->real_width.xy);
	pwidths->Width.xy.x = 0;
	pwidths->Width.xy.y = pwidths->real_width.xy.y;
	pwidths->Width.w = pwidths->real_width.w =
		pwidths->Width.xy.y;
	pwidths->Width.v.x = - pwidths->Width.xy.y / 2;
	pwidths->Width.v.y = - pwidths->Width.xy.y;
    } else {
	gs_distance_transform(finfo.MissingWidth, 0.0, scale_c, &pwidths->real_width.xy);
	pwidths->Width.xy.x = pwidths->real_width.xy.x;
	pwidths->Width.xy.y = 0;
	pwidths->Width.w = pwidths->real_width.w =
		pwidths->Width.xy.x;
	pwidths->Width.v.x = pwidths->Width.v.y = 0;
    }
    /*
     * Don't mark the width as known, just in case this is an
     * incrementally defined font.
     */
    return 1;
}


/*
 * Get the widths (unmodified from the copied font,
 * and possibly modified from the original font) of a given glyph.
 * Return 1 if the width was defaulted to MissingWidth.
 * Return TEXT_PROCESS_CDEVPROC if a CDevProc callout is needed.
 * cdevproc_result != NULL if we restart after a CDevProc callout.
 */
int
pdf_glyph_widths(pdf_font_resource_t *pdfont, int wmode, gs_glyph glyph,
		 gs_font *orig_font, pdf_glyph_widths_t *pwidths, 
		 const double cdevproc_result[10])
{
    gs_font_base *cfont = pdf_font_resource_font(pdfont, false);
    gs_font *ofont = orig_font;
    gs_glyph_info_t info;
    gs_matrix scale_c, scale_o;
    int code, rcode = 0;
    gs_point v;
    int allow_cdevproc_callout = (orig_font->FontType == ft_CID_TrueType 
		|| orig_font->FontType == ft_CID_encrypted 
		? GLYPH_INFO_CDEVPROC : 0); /* fixme : allow more font types. */

    if (ofont->FontType == ft_composite)
	return_error(gs_error_unregistered); /* Must not happen. */
    code = pdf_font_orig_matrix((const gs_font *)cfont, &scale_c);
    if (code < 0)
	return code;
    code = pdf_font_orig_matrix(ofont, &scale_o);
    if (code < 0)
	return code;
    gs_matrix_scale(&scale_c, 1000.0, 1000.0, &scale_c);
    gs_matrix_scale(&scale_o, 1000.0, 1000.0, &scale_o);
    pwidths->Width.v.x = pwidths->Width.v.y = 0;
    pwidths->real_width.v.x = pwidths->real_width.v.y = 0;
    pwidths->replaced_v = false;
    if (glyph == GS_NO_GLYPH)
	return get_missing_width(cfont, wmode, &scale_c, pwidths);
    code = cfont->procs.glyph_info((gs_font *)cfont, glyph, NULL,
				    GLYPH_INFO_WIDTH0 |
				    (GLYPH_INFO_WIDTH0 << wmode) |
				    GLYPH_INFO_OUTLINE_WIDTHS |
				    (GLYPH_INFO_VVECTOR0 << wmode),
				    &info);
    /* For CID fonts the PDF spec requires the x-component of v-vector
       to be equal to half glyph width, and AR5 takes it from W, DW.
       So make a compatibe data here.
     */
    if (code == gs_error_undefined || !(info.members & (GLYPH_INFO_WIDTH0 << wmode))) {
	code = get_missing_width(cfont, wmode, &scale_c, pwidths);
	if (code < 0)
	    v.y = 0;
	else 
	    v.y = pwidths->Width.v.y;
	if (wmode && pdf_is_CID_font(ofont)) {
	    pdf_glyph_widths_t widths1;

	    if (get_missing_width(cfont, 0, &scale_c, &widths1) < 0)
		v.x = 0;
	    else
		v.x = widths1.Width.w / 2;
	} else
	    v.x = pwidths->Width.v.x;
    } else if (code < 0)
	return code;
    else {
	code = store_glyph_width(&pwidths->Width, wmode, &scale_c, &info);
	if (code < 0)
	    return code;
	rcode |= code;
	if (info.members  & (GLYPH_INFO_VVECTOR0 << wmode))
	    gs_distance_transform(info.v.x, info.v.y, &scale_c, &v);
	else 
	    v.x = v.y = 0;
	if (wmode && pdf_is_CID_font(ofont)) {
	    if (info.members & (GLYPH_INFO_WIDTH0 << wmode)) {
		gs_point xy;

		gs_distance_transform(info.width[0].x, info.width[0].y, &scale_c, &xy);
		v.x = xy.x / 2;
	    } else {
		pdf_glyph_widths_t widths1;
		
		if (get_missing_width(cfont, 0, &scale_c, &widths1) < 0)
		    v.x = 0;
		else
		    v.x = widths1.Width.w / 2;
	    }
	}
    }
    pwidths->Width.v = v;
#if 0
    if (code > 0)
	pwidths->Width.xy.x = pwidths->Width.xy.y = pwidths->Width.w = 0;
#else /* Skip only if not paralel to the axis. */
    if (code > 0 && !pdf_is_CID_font(ofont))
	pwidths->Width.xy.x = pwidths->Width.xy.y = pwidths->Width.w = 0;
#endif
    if (cdevproc_result == NULL) {
	code = ofont->procs.glyph_info(ofont, glyph, NULL,
					    (GLYPH_INFO_WIDTH0 << wmode) |
					    (GLYPH_INFO_VVECTOR0 << wmode) | 
					    allow_cdevproc_callout,
					    &info);
	/* fixme : Move this call before cfont->procs.glyph_info. */
	if (info.members & GLYPH_INFO_CDEVPROC) {
	    if (allow_cdevproc_callout)
		return TEXT_PROCESS_CDEVPROC;
	    else
		return_error(gs_error_rangecheck);
	}
    } else {
	info.width[0].x = cdevproc_result[0];
	info.width[0].y = cdevproc_result[1];
	info.width[1].x = cdevproc_result[6];
	info.width[1].y = cdevproc_result[7];
	info.v.x = (wmode ? cdevproc_result[8] : 0);
	info.v.y = (wmode ? cdevproc_result[9] : 0);
	info.members = (GLYPH_INFO_WIDTH0 << wmode) | 
		       (wmode ? GLYPH_INFO_VVECTOR1 : 0);
	code = 0;
    }
    if (code == gs_error_undefined || !(info.members & (GLYPH_INFO_WIDTH0 << wmode)))
	pwidths->real_width = pwidths->Width;
    else if (code < 0)
	return code;
    else {
	if ((info.members & (GLYPH_INFO_VVECTOR0 | GLYPH_INFO_VVECTOR1)) != 0)
	    pwidths->replaced_v = true;
	else 
	    info.v.x = info.v.y = 0;
	code = store_glyph_width(&pwidths->real_width, wmode, &scale_o, &info);
	if (code < 0)
	    return code;
	rcode |= code;
	gs_distance_transform(info.v.x, info.v.y, &scale_o, &pwidths->real_width.v);
    }
    return rcode;
}

static int
pdf_choose_output_char_code(gx_device_pdf *pdev, pdf_text_enum_t *penum, gs_char *pch)
{
    gs_char ch;
    gs_font *font = penum->current_font;

    if (penum->text.operation & TEXT_FROM_SINGLE_GLYPH) {
	byte buf[1];
	int char_code_length;
	gs_glyph glyph = penum->text.data.d_glyph;
	int code = pdf_encode_glyph((gs_font_base *)font, glyph, 
		    buf, sizeof(buf), &char_code_length);

	if (code < 0) {
	    /* Must not happen, becuse pdf_encode_glyph was passed in process_plain_text.*/
	    ch = GS_NO_CHAR;
	} else if (char_code_length != 1) {
	    /* Must not happen with type 3 fonts.*/
	    ch = GS_NO_CHAR;
	} else
	    ch = buf[0];
    } else if (penum->orig_font->FontType == ft_composite) {
	gs_font_type0 *font0 = (gs_font_type0 *)penum->orig_font;
	gs_glyph glyph = penum->returned.current_glyph;
	
	if (font0->data.FMapType == fmap_CMap) {
	    pdf_font_resource_t *pdfont;
	    int code = pdf_attached_font_resource(pdev, font, &pdfont, NULL, NULL, NULL, NULL); 

	    if (code < 0)
		return code;
	    ch = pdf_find_glyph(pdfont, glyph);
	} else
	    ch = penum->returned.current_char;
    } else {
	ch = penum->returned.current_char;
	/* Keep for records : glyph = font->procs.encode_char(font, ch, GLYPH_SPACE_NAME); */
	/*
	 * If glyph == GS_NO_GLYPH, we should replace it with 
	 * a notdef glyph, but we don't know how to do with Type 3 fonts.
	 */
    }
    *pch = ch;
    return 0;
}

static int
pdf_choose_output_glyph_hame(gx_device_pdf *pdev, pdf_text_enum_t *penum, gs_const_string *gnstr, gs_glyph glyph)
{
    if (penum->orig_font->FontType == ft_composite) {
	char buf[6];
	byte *p;

	gnstr->size = 5;
	p = (byte *)gs_alloc_string(pdev->pdf_memory, gnstr->size, "pdf_text_set_cache");
	if (p == NULL)
	    return_error(gs_error_VMerror);
	sprintf(buf, "g%04x", glyph & 0xFFFF);
	memcpy(p, buf, 5);
	gnstr->data = p;
    } else {
	int code = penum->orig_font->procs.glyph_name(penum->orig_font, glyph, gnstr);

	if (code < 0)
	    return_error(gs_error_unregistered); /* Must not happen. */
    }
    return 0;
}

/* ---------------- Main entry ---------------- */

/*
 * Fall back to the default text processing code when needed.
 */
int
pdf_default_text_begin(gs_text_enum_t *pte, const gs_text_params_t *text,
		       gs_text_enum_t **ppte)
{
    gs_text_params_t text1 = *text;

    if(pte->current_font->FontType == 3 && (text1.operation & TEXT_DO_NONE)) {
	/* We need a real drawing to accumulate charproc. */
	text1.operation &= ~TEXT_DO_NONE;
	text1.operation |= TEXT_DO_DRAW;
    }
    return gx_default_text_begin(pte->dev, pte->pis, &text1, pte->current_font,
				 pte->path, pte->pdcolor, pte->pcpath,
				 pte->memory, ppte);
}

/*
 * Continue processing text.  This is the 'process' procedure in the text
 * enumerator.  Per the check in pdf_text_begin, we know the operation is
 * not a charpath, but it could be anything else.
 */
int
pdf_text_process(gs_text_enum_t *pte)
{
    pdf_text_enum_t *const penum = (pdf_text_enum_t *)pte;
    uint operation = pte->text.operation;
    uint size = pte->text.size - pte->index;
    gs_text_enum_t *pte_default;
    PROCESS_TEXT_PROC((*process));
    int code;
    gx_device_pdf *pdev = (gx_device_pdf *)penum->dev;
#define BUF_SIZE 100		/* arbitrary > 0 */
    /* Use a union to ensure alignment. */
    union bu_ {
	byte bytes[BUF_SIZE];
	gs_char chars[BUF_SIZE / sizeof(gs_char)];
	gs_glyph glyphs[BUF_SIZE / sizeof(gs_glyph)];
    } buf;

     if (!penum->pte_default && !penum->charproc_accum) {
	/* Don't need to sync before exiting charproc. */
	code = pdf_prepare_text_drawing(pdev, pte);
	if (code == gs_error_rangecheck) {
	    /* Fallback to the default implermentation for handling 
	       a transparency with CompatibilityLevel<=1.3 . */
	    goto default_impl;
	}
	if (code < 0)
	    return code;
    }
    if (!penum->pte_default) {
	pdev->charproc_just_accumulated = false;
	if (penum->cdevproc_callout) {
	    /* Restore after TEXT_PROCESS_CDEVPROC in scan_cmap_text. */
	    penum->current_font = penum->orig_font;
	}
    }
    if (penum->current_font->FontType == ft_user_defined && (penum->text.operation & TEXT_DO_ANY_CHARPATH)
	&& !pdev->type3charpath) {
	pdev->type3charpath = true;
	goto default_impl;
    }

    code = -1;		/* to force default implementation */

    /*
     * If we fell back to the default implementation, continue using it.
     */
 top:
    pte_default = penum->pte_default;
    if (pte_default) {
	if (penum->charproc_accum) {
	    gs_const_string gnstr;

	    code = pdf_choose_output_glyph_hame(pdev, penum, &gnstr, pte_default->returned.current_glyph);
	    if (code < 0)
		return code;

	    if ((penum->current_font->FontType == ft_user_defined) && stell(pdev->strm) == 0)
	    {
		char glyph[256], FontName[gs_font_name_max + 1], KeyName[256];
		int len;

		len = min(gs_font_name_max, gnstr.size);
		memcpy(glyph, gnstr.data, len);
		glyph[len] = 0x00;
		len = min(gs_font_name_max, penum->current_font->font_name.size);
		memcpy(FontName, penum->current_font->font_name.chars, len);
		FontName[len] = 0x00;
		len = min(gs_font_name_max, penum->current_font->key_name.size);
		memcpy(KeyName, penum->current_font->key_name.chars, len);
		KeyName[len] = 0x00;

		eprintf4("ERROR: Page %d used undefined glyph '%s' from type 3 font '%s', key '%s'\n",
		    pdev->next_page, glyph, FontName, KeyName);
	        stream_puts(pdev->strm, "0 0 0 0 0 0 d1\n");
	    }
	    
	    code = pdf_end_charproc_accum(pdev, penum->current_font, penum->cgp, 
			pte_default->returned.current_glyph, penum->output_char_code, &gnstr);
	    if (code < 0)
		return code;
	    penum->charproc_accum = false;
	    code = gx_default_text_restore_state(pte_default);
	    if (code < 0)
		return code;
	    gs_text_release(pte_default, "pdf_text_process");
	    penum->pte_default = 0;
	    goto top;
	}
	pdev->pte = pte_default; /* CAUTION: See comment in gdevpdfx.h . */
	code = gs_text_process(pte_default);
	pdev->pte = NULL;	 /* CAUTION: See comment in gdevpdfx.h . */
	pdev->charproc_just_accumulated = false;
	if (code == TEXT_PROCESS_RENDER && !pdev->type3charpath) {
	    penum->returned.current_char = pte_default->returned.current_char;
	    penum->returned.current_glyph = pte_default->returned.current_glyph;
	    pdev->charproc_ctm = penum->pis->ctm;
	    if (penum->current_font->FontType == ft_user_defined && 
		    penum->outer_CID == GS_NO_GLYPH &&
		    !(penum->pte_default->text.operation & TEXT_DO_CHARWIDTH)) {
		/* The condition above must be consistent with one in pdf_text_set_cache,
		   which decides to apply pdf_set_charproc_attrs. */
		gs_matrix m;
		pdf_font_resource_t *pdfont;

		code = pdf_start_charproc_accum(pdev);
		if (code < 0)
		    return code;
		pdf_viewer_state_from_imager_state(pdev, pte->pis, pte->pdcolor);
		/* Set line params to unallowed values so that
		   they'll synchronize with writing them out on the first use. 
		   Doing so because PDF viewer inherits them from the 
		   contents stream when executing the charproc,
		   but at this moment we don't know in what contexts
		   it will be used. */
		pdev->state.line_params.half_width = -1;
		pdev->state.line_params.cap = gs_cap_unknown;
		pdev->state.line_params.join = gs_join_unknown;
		pdev->state.line_params.miter_limit = -1;
		pdev->state.line_params.dash.pattern_size = -1;
		/* Must set an identity CTM for the charproc accumulation.
		   The function show_proceed (called from gs_text_process above) 
		   executed gsave, so we are safe to change CTM now.
		   Note that BuildChar may change CTM before calling setcachedevice. */
		gs_make_identity(&m);
		gs_matrix_fixed_from_matrix(&penum->pis->ctm, &m);
		/* Choose a character code to use with the charproc. */
		code = pdf_choose_output_char_code(pdev, penum, &penum->output_char_code);
		if (code < 0)
		    return code;
		code = pdf_attached_font_resource(pdev, penum->current_font, &pdfont, NULL, NULL, NULL, NULL);
		if (code < 0)
		    return code;
		pdev->font3 = (pdf_resource_t *)pdfont;
		pdev->substream_Resources = pdfont->u.simple.s.type3.Resources;
		penum->charproc_accum = true;
		return TEXT_PROCESS_RENDER;
	    } else
		code += 0; /* A fgood place for breakpoint. */
	}
	gs_text_enum_copy_dynamic(pte, pte_default, true);
	if (code)
	    return code;
	gs_text_release(pte_default, "pdf_text_process");
	penum->pte_default = 0;
	if (pdev->type3charpath)
	    pdev->type3charpath = false;
	return 0;
    }
    {
	gs_font *font = pte->orig_font; /* Not sure. Changed for CDevProc callout. Was pte->current_font */

	switch (font->FontType) {
	case ft_CID_encrypted:
	case ft_CID_TrueType:
	    process = process_cid_text;
	    break;
	case ft_encrypted:
	case ft_encrypted2:
	case ft_TrueType:
	case ft_user_defined:
	    /* The data may be either glyphs or characters. */
	    process = process_plain_text;
	    break;
	case ft_composite:
	    process =
		(((gs_font_type0 *)font)->data.FMapType == fmap_CMap ?
		 process_cmap_text :
		 process_composite_text);
	    break;
	default:
	    goto skip;
	}
    }

    /*
     * We want to process the entire string in a single call, but we may
     * need to modify it.  Copy it to a buffer.  Note that it may consist
     * of bytes, gs_chars, or gs_glyphs.
     */

    if (operation & (TEXT_FROM_STRING | TEXT_FROM_BYTES)) {
	if (size < sizeof(gs_glyph))
	    size = sizeof(gs_glyph); /* for process_cid_text */
    } else if (operation & TEXT_FROM_CHARS)
	size *= sizeof(gs_char);
    else if (operation & TEXT_FROM_SINGLE_CHAR)
	size = sizeof(gs_char);
    else if (operation & TEXT_FROM_GLYPHS)
	size *= sizeof(gs_glyph);
    else if (operation & TEXT_FROM_SINGLE_GLYPH)
	size = sizeof(gs_glyph);
    else
	goto skip;

    /* Now that we are using the working buffer for text in composite fonts, we must make sure
     * it is large enough. Each input character code may be promoted to a gs_glyph, in scan_cmap_text
     * when processing a Type 0 font with a type 1 descendant font. This routine uses
     * pdf_make_text_glyphs_table_unencoded (called from pdf_obtain_font_resource_unencoded) which  
     * also may require an identically sized buffer, so we need:
     * num__input_characters * sizeof(gs_glyph) * 2.
     */
    if (pte->orig_font->FontType == ft_composite) {
	if (size < (pte->text.size - pte->index) * sizeof(gs_glyph) * 2)
	    size = (pte->text.size - pte->index) * sizeof(gs_glyph) * 2;
    }

    if (size <= sizeof(buf)) {
	code = process(pte, buf.bytes, size);
    } else {
	byte *buf = gs_alloc_string(pte->memory, size, "pdf_text_process");

	if (buf == 0)
	    return_error(gs_error_VMerror);
	code = process(pte, buf, size);
	gs_free_string(pte->memory, buf, size, "pdf_text_process");
    }
 skip:
    if (code < 0 || 
	    (pte->current_font->FontType == ft_user_defined && 
	     code != TEXT_PROCESS_INTERVENE &&
	    penum->index < penum->text.size)) {
	if (code == gs_error_unregistered) /* Debug purpose only. */
	    return code;
	if (code == gs_error_VMerror)
	    return code;
	if (code == gs_error_invalidfont) /* Bug 688370. */
	    return code;
 default_impl:
	/* Fall back to the default implementation. */
	code = pdf_default_text_begin(pte, &pte->text, &pte_default);
	if (code < 0)
	    return code;
	penum->pte_default = pte_default;
	gs_text_enum_copy_dynamic(pte_default, pte, false);
    }
    /* The 'process' procedure might also have set pte_default itself. */
    if (penum->pte_default && !code)
	goto top;
    return code;
    /*
     * This function uses an unobvious algorithm while handling type 3 fonts.
     * It runs 'process' to copy text until a glyph, which was not copied to
     * output font. Then it installs pte_default and falls back to default
     * implementation with PS interpreter callout. The callout executes 
     * BuildChar/BuildGlyph with setcachedevice. The latter calls
     * pdf_set_charproc_attrs, which sets up an accumulator 
     * of graphic objects to a pdf_begin_resource stream.
     * When the callout completes, pdf_text_process calls pdf_end_charproc_accum
     * and later resumes the normal (non-default) text enumeration, repeating the 
     * the "callouted" glyph AT SECOND TIME. We can't do without the second pass
     * becauase in the first pass the glyph widths is unknown.
     */
     /*
      * Another unobvious thing is a CDevProc callout.
      * If 'process' returns with TEXT_PROCESS_CDEVPROC,
      * an interpreter callout will happen, and the function will be called again
      * with pte->cdevproc_result_valid = true. Then it restatrs with taking
      * glyph metrics from pte->cdevproc_result instead obtaining them with
      * font->procs.glyph_info .
      */
}
