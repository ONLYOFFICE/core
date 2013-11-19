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

/* $Id: gdevpdtd.c 9368 2009-01-16 18:38:48Z ken $ */
/* FontDescriptor implementation for pdfwrite */
#include "math_.h"
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsrect.h"		/* for rect_merge */
#include "gscencs.h"
#include "gdevpdfx.h"
#include "gdevpdfo.h"		/* for object->written */
#include "gdevpdtb.h"
#include "gdevpdtd.h"
#include "gdevpdtf.h"

/* ================ Types and structures ================ */

/*
 * There are many different flavors of FontDescriptor.  The first division
 * is between FontDescriptors for actual fonts, and character class entries
 * in the FD dictionary of a CID-keyed font.  The latter include metrics and
 * a FontName but nothing else.  We represent these with a different C
 * structure (pdf_sub_font_descriptor_t).
 *
 * Descriptors for actual fonts have three major orthogonal properties:
 *
 *	- Whether they represent a CID-keyed font or a name-keyed
 *	  (non-CID-keyed) font.  We distinguish this by the FontType
 *	  of the saved font (see below).
 *
 *	- Whether the font they represent is embedded.
 *
 *	- Whether the font they represent is a complete font or a subset.
 *	  We always track which glyphs in a font are used: client code
 *	  decides whether to treat the font as a subset when the
 *	  descriptor (and, if embedded, the font) is written.
 *
 * Note that non-embedded fonts in the base set of 14 do not have
 * descriptors, nor do Type 0 or (synthetic bitmap) Type 3 fonts.
 */
/*
 * Start by defining the elements common to font descriptors and sub-font
 * (character class) descriptors.
 */
typedef struct pdf_font_descriptor_values_s {
    /* Required elements */
    int Ascent, CapHeight, Descent, ItalicAngle, StemV;
    gs_int_rect FontBBox;
    gs_string FontName;
    uint Flags;
    /* Optional elements (default to 0) */
    int AvgWidth, Leading, MaxWidth, MissingWidth, StemH, XHeight;
} pdf_font_descriptor_values_t;
typedef struct pdf_font_descriptor_common_s pdf_font_descriptor_common_t;
struct pdf_font_descriptor_common_s {
    pdf_resource_common(pdf_font_descriptor_common_t);
    pdf_font_descriptor_values_t values;
};
/* Flag bits */
/*#define FONT_IS_FIXED_WIDTH (1<<0)*/  /* defined in gxfont.h */
#define FONT_IS_SERIF (1<<1)
#define FONT_IS_SYMBOLIC (1<<2)
#define FONT_IS_SCRIPT (1<<3)
/*
 * There is confusion over the meaning of the 1<<5 bit.  According to the
 * published PDF documentation, in PDF 1.1, it meant "font uses
 * StandardEncoding", and as of PDF 1.2, it means "font uses (a subset of)
 * the Adobe standard Latin character set"; however, Acrobat Reader 3 and 4
 * seem to use the former interpretation, even if the font is embedded and
 * the file is identified as a PDF 1.2 file.  We have to use the former
 * interpretation in order to produce output that Acrobat will handle
 * correctly.
 */
#define FONT_USES_STANDARD_ENCODING (1<<5) /* always used */
#define FONT_IS_ADOBE_ROMAN (1<<5) /* never used */
#define FONT_IS_ITALIC (1<<6)
#define FONT_IS_ALL_CAPS (1<<16)
#define FONT_IS_SMALL_CAPS (1<<17)
#define FONT_IS_FORCE_BOLD (1<<18)

/*
 * Define a (top-level) FontDescriptor.  CID-keyed vs. non-CID-keyed fonts
 * are distinguished by their FontType.
 */
#ifndef pdf_base_font_DEFINED
#  define pdf_base_font_DEFINED
typedef struct pdf_base_font_s pdf_base_font_t;
#endif
struct pdf_font_descriptor_s {
    pdf_font_descriptor_common_t common;
    pdf_base_font_t *base_font;
    font_type FontType;		/* (copied from base_font) */
    bool embed;
    struct cid_ {		/* (CIDFonts only) */
	cos_dict_t *Style;
	char Lang[3];		/* 2 chars + \0 */
	cos_dict_t *FD;		/* value = COS_VALUE_RESOURCE */
    } cid;
};
/*
 * Define a sub-descriptor for a character class (FD dictionary element).
 */
typedef struct pdf_sub_font_descriptor_s {
    pdf_font_descriptor_common_t common;
} pdf_sub_font_descriptor_t;

/*
 * Font descriptors are pseudo-resources, so their GC descriptors
 * must be public.
 */
BASIC_PTRS(pdf_font_descriptor_ptrs) {
    GC_STRING_ELT(pdf_font_descriptor_t, common.values.FontName),
    GC_OBJ_ELT(pdf_font_descriptor_t, base_font),
    GC_OBJ_ELT(pdf_font_descriptor_t, cid.Style),
    GC_OBJ_ELT(pdf_font_descriptor_t, cid.FD)
};
gs_public_st_basic_super(st_pdf_font_descriptor, pdf_font_descriptor_t,
  "pdf_font_descriptor_t", pdf_font_descriptor_ptrs,
  pdf_font_descriptor_data, &st_pdf_resource, 0);

/*
 * Sub-font descriptors are also pseudo-resources.
 */
BASIC_PTRS(pdf_sub_font_descriptor_ptrs) {
    GC_STRING_ELT(pdf_sub_font_descriptor_t, common.values.FontName)
};
gs_public_st_basic_super(st_pdf_sub_font_descriptor,
  pdf_sub_font_descriptor_t, "pdf_sub_font_descriptor_t",
  pdf_sub_font_descriptor_ptrs, pdf_sub_font_descriptor_data,
  &st_pdf_resource, 0);

/* ================ Procedures ================ */

/* ---------------- Private ---------------- */

/* Get the ID of font descriptor metrics. */
static inline long
pdf_font_descriptor_common_id(const pdf_font_descriptor_common_t *pfdc)
{
    return pdf_resource_id((const pdf_resource_t *)pfdc);
}

/* Write the common part of a FontDescriptor, aside from the final >>. */
static int
write_FontDescriptor_common(gx_device_pdf *pdev,
			    const pdf_font_descriptor_common_t *pfd)
{
    stream *s;
    int code;
    param_printer_params_t params;
    printer_param_list_t rlist;
    gs_param_list *const plist = (gs_param_list *)&rlist;

    pdf_open_separate(pdev, pdf_font_descriptor_common_id(pfd));
    s = pdev->strm;
    stream_puts(s, "<</Type/FontDescriptor/FontName");
    pdf_put_name(pdev, pfd->values.FontName.data, pfd->values.FontName.size);
    pdf_write_font_bbox(pdev, &pfd->values.FontBBox);
    params = param_printer_params_default;
    code = s_init_param_printer(&rlist, &params, s);
    if (code >= 0) {
#define DESC_INT(str, memb)\
 {str, gs_param_type_int, offset_of(pdf_font_descriptor_common_t, values.memb)}
	static const gs_param_item_t required_items[] = {
	    DESC_INT("Ascent", Ascent),
	    DESC_INT("CapHeight", CapHeight),
	    DESC_INT("Descent", Descent),
	    DESC_INT("ItalicAngle", ItalicAngle),
	    DESC_INT("StemV", StemV),
	    gs_param_item_end
	};
	static const gs_param_item_t optional_items[] = {
	    DESC_INT("AvgWidth", AvgWidth),
	    DESC_INT("Leading", Leading),
	    DESC_INT("MaxWidth", MaxWidth),
	    DESC_INT("MissingWidth", MissingWidth),
	    DESC_INT("StemH", StemH),
	    DESC_INT("XHeight", XHeight),
	    gs_param_item_end
	};
#undef DESC_INT
	int Flags = pfd->values.Flags;
	pdf_font_descriptor_t defaults;

	code = param_write_int(plist, "Flags", &Flags);
	if (code < 0) 
	    return code;
	code = gs_param_write_items(plist, pfd, NULL, required_items);
	if (code < 0) 
	    return code;
	memset(&defaults, 0, sizeof(defaults));
	code = gs_param_write_items(plist, pfd, &defaults, optional_items);
	if (code < 0) 
	    return code;
	s_release_param_printer(&rlist);
    }
    return 0;
}

/* ---------------- Public ---------------- */

/*
 * Allocate a FontDescriptor, initializing the FontType and rid from the
 * gs_font.
 */
int
pdf_font_descriptor_alloc(gx_device_pdf *pdev, pdf_font_descriptor_t **ppfd,
			  gs_font_base *font, bool embed)
{
    pdf_font_descriptor_t *pfd;
    pdf_base_font_t *pbfont;
    int code = pdf_base_font_alloc(pdev, &pbfont, font, 
		(font->orig_FontMatrix.xx == 0 && font->orig_FontMatrix.xy == 0 
		    ? &font->FontMatrix : &font->orig_FontMatrix), false);

    if (code < 0)
	return code;
    code = pdf_alloc_resource(pdev, resourceFontDescriptor,
			      font->id, (pdf_resource_t **)&pfd, -1L);
    if (code < 0) {
	gs_free_object(pdev->pdf_memory, pbfont,
		       "pdf_font_descriptor_alloc(base_font)");
	return code;
    }
    memset(&pfd->common.values, 0,
	   sizeof(*pfd) - offset_of(pdf_font_descriptor_t, common.values));
    pfd->base_font = pbfont;
    pfd->FontType = font->FontType;
    pfd->embed = embed;
    *ppfd = pfd;
    return 0;
}

/* Get the object ID of a FontDescriptor. */
long
pdf_font_descriptor_id(const pdf_font_descriptor_t *pfd)
{
    return pdf_resource_id((const pdf_resource_t *)pfd);
}

/*
 * Get the FontType of a FontDescriptor.
 */
font_type
pdf_font_descriptor_FontType(const pdf_font_descriptor_t *pfd)
{
    return pfd->FontType;
}

/*
 * Get the embedding status of a FontDescriptor.
 */
bool
pdf_font_descriptor_embedding(const pdf_font_descriptor_t *pfd)
{
    return pfd->embed;
}

/*
 * Check for subset font.
 */
bool
pdf_font_descriptor_is_subset(const pdf_font_descriptor_t *pfd)
{
    return pdf_base_font_is_subset(pfd->base_font);
}

/*
 * Return a reference to the FontName of a FontDescriptor, similar to
 * pdf_base_font_name.
 */
gs_string *pdf_font_descriptor_name(pdf_font_descriptor_t *pfd)
{
    return &pfd->common.values.FontName;
}

/*
 * Return the (copied, subset or complete) font associated with a FontDescriptor.
 * This procedure probably shouldn't exist....
 */
gs_font_base *
pdf_font_descriptor_font(const pdf_font_descriptor_t *pfd, bool complete)
{
    return pdf_base_font_font(pfd->base_font, complete);
}

/*
 * Drop the copied complete font associated with a FontDescriptor.
 */
void
pdf_font_descriptor_drop_complete_font(const pdf_font_descriptor_t *pfd)
{
    pdf_base_font_drop_complete(pfd->base_font);
}

/*
 * Return a reference to the name of a FontDescriptor's base font, per
 * pdf_base_font_name.
 */
gs_string *pdf_font_descriptor_base_name(const pdf_font_descriptor_t *pfd)
{
    return pdf_base_font_name(pfd->base_font);
}

/*
 * Copy a glyph from a font to the stable copy.  Return 0 if this is a
 * new glyph, 1 if it was already copied.
 */
int
pdf_font_used_glyph(pdf_font_descriptor_t *pfd, gs_glyph glyph,
		    gs_font_base *font)
{
    return pdf_base_font_copy_glyph(pfd->base_font, glyph, font);
}

/* Compute the FontDescriptor metrics for a font. */
int
pdf_compute_font_descriptor(gx_device_pdf *pdev, pdf_font_descriptor_t *pfd)
{
    gs_font_base *bfont = pdf_base_font_font(pfd->base_font, false);
    gs_glyph glyph, notdef;
    int index;
    int wmode = bfont->WMode;
    int members = (GLYPH_INFO_WIDTH0 << wmode) |
	GLYPH_INFO_BBOX | GLYPH_INFO_NUM_PIECES;
    pdf_font_descriptor_values_t desc;
    gs_matrix smat;
    gs_matrix *pmat = NULL;
    int fixed_width = 0;
    int small_descent = 0, small_height = 0;
    bool small_present = false;
    int x_height = 0;
    int cap_height = 0;
    gs_rect bbox_colon, bbox_period, bbox_I;
    bool is_cid = (bfont->FontType == ft_CID_encrypted ||
		   bfont->FontType == ft_CID_TrueType);
    bool have_colon = false, have_period = false, have_I = false;
    int code;

    memset(&bbox_colon, 0, sizeof(bbox_colon)); /* quiet gcc warnings. */
    memset(&bbox_period, 0, sizeof(bbox_period)); /* quiet gcc warnings. */
    memset(&bbox_I, 0, sizeof(bbox_I)); /* quiet gcc warnings. */
    memset(&desc, 0, sizeof(desc));
    if (is_cid && bfont->FontBBox.p.x != bfont->FontBBox.q.x &&
		  bfont->FontBBox.p.y != bfont->FontBBox.q.y) {
	int scale = (bfont->FontType == ft_TrueType || bfont->FontType == ft_CID_TrueType ? 1000 : 1);

	desc.FontBBox.p.x = (int)(bfont->FontBBox.p.x * scale);
	desc.FontBBox.p.y = (int)(bfont->FontBBox.p.y * scale);
	desc.FontBBox.q.x = (int)(bfont->FontBBox.q.x * scale);
	desc.FontBBox.q.y = (int)(bfont->FontBBox.q.y * scale);
	desc.Ascent = desc.FontBBox.q.y;
	members &= ~GLYPH_INFO_BBOX;
    } else {
	desc.FontBBox.p.x = desc.FontBBox.p.y = max_int;
	desc.FontBBox.q.x = desc.FontBBox.q.y = min_int;
    }
    /*
     * Embedded TrueType fonts use a 1000-unit character space, but the
     * font itself uses a 1-unit space.  Compensate for this here.
     */
    switch (bfont->FontType) {
    case ft_TrueType:
    case ft_CID_TrueType:
	gs_make_scaling(1000.0, 1000.0, &smat);
	pmat = &smat;
	/* Type 3 fonts may use a FontMatrix in PDF, so we don't 
	 * need to deal with non-standard matrices
	 */
    case ft_user_defined:
	break;
	/* Other font types may use a non-standard (not 1000x1000) design grid
	 * The FontMatrix is used to map to the unit square. However PDF files
	 * don't allow FontMatrix entries, all fonts are nominally 1000x1000.
	 * If we have a font with a non-standard matrix we must account for that
	 * here by scaling the font outline.
	 */
    default:	
	gs_matrix_scale(&bfont->FontMatrix, 1000.0, 1000.0, &smat);
	pmat = &smat;
	break;
    }

    /*
     * Scan the entire glyph space to compute Ascent, Descent, FontBBox, and
     * the fixed width if any.  For non-symbolic fonts, also note the
     * bounding boxes for Latin letters and a couple of other characters,
     * for computing the remaining descriptor values (CapHeight,
     * ItalicAngle, StemV, XHeight, and flags SERIF, SCRIPT, ITALIC,
     * ALL_CAPS, and SMALL_CAPS).  (The algorithms are pretty crude.)
     */
    notdef = GS_NO_GLYPH;
    for (index = 0;
	 (code = bfont->procs.enumerate_glyph((gs_font *)bfont, &index, 
		(is_cid ? GLYPH_SPACE_INDEX : GLYPH_SPACE_NAME), &glyph)) >= 0 &&
	     index != 0;
	 ) {
	gs_glyph_info_t info;
	gs_const_string gname;
	gs_glyph glyph_known_enc;

	code = bfont->procs.glyph_info((gs_font *)bfont, glyph, pmat, members, &info);
	if (code == gs_error_VMerror)
	    return code;
	if (code < 0) {
	    /*
	     * Since this function may be indirtectly called from gx_device_finalize,
	     * we are unable to propagate error code to the interpreter.
	     * Therefore we skip it here hoping that few errors can be
	     * recovered by the integration through entire glyph set.
	     */
	    continue;
	}
	if (members & GLYPH_INFO_BBOX) {
	    /* rect_merge(desc.FontBBox, info.bbox); Expanding due to type cast :*/
	    if (info.bbox.p.x < desc.FontBBox.p.x) desc.FontBBox.p.x = (int)info.bbox.p.x;
	    if (info.bbox.q.x > desc.FontBBox.q.x) desc.FontBBox.q.x = (int)info.bbox.q.x;
	    if (info.bbox.p.y < desc.FontBBox.p.y) desc.FontBBox.p.y = (int)info.bbox.p.y;
	    if (info.bbox.q.y > desc.FontBBox.q.y) desc.FontBBox.q.y = (int)info.bbox.q.y;
	    if (!info.num_pieces)
		desc.Ascent = max(desc.Ascent, (int)info.bbox.q.y);
	}
	if (notdef == GS_NO_GLYPH && gs_font_glyph_is_notdef(bfont, glyph)) {
	    notdef = glyph;
	    desc.MissingWidth = (int)info.width[wmode].x;
	}
	if (info.width[wmode].y != 0)
	    fixed_width = min_int;
	else if (fixed_width == 0)
	    fixed_width = (int)info.width[wmode].x;
	else if (info.width[wmode].x != fixed_width)
	    fixed_width = min_int;
	if (desc.Flags & FONT_IS_SYMBOLIC)
	    continue;		/* skip Roman-only computation */
	if (is_cid)
	    continue;
	code = bfont->procs.glyph_name((gs_font *)bfont, glyph, &gname);
	if (code < 0) {
	    /* If we fail to get the glyph name, best assume this is a symbolic font */
	    desc.Flags |= FONT_IS_SYMBOLIC;
	    continue;
	}
	/* See if the glyph name is in any of the known encodings */
        glyph_known_enc = gs_c_name_glyph(gname.data, gname.size);
	if (glyph_known_enc == gs_no_glyph) {
	    desc.Flags |= FONT_IS_SYMBOLIC;
	    continue;
	}
	/* Finally check if the encoded glyph is in Standard Encoding */
	if (gs_c_decode(glyph_known_enc, 0) == gs_no_glyph) {
	    desc.Flags |= FONT_IS_SYMBOLIC;
	    continue;
	}
        switch (gname.size) {
	case 5:
	    if (!memcmp(gname.data, "colon", 5))
		bbox_colon = info.bbox, have_colon = true;
	    continue;
	case 6:
	    if (!memcmp(gname.data, "period", 6))
		bbox_period = info.bbox, have_period = true;
	    continue;
	case 1:
	    break;
	default:
	    continue;
	}

	if (gname.data[0] >= 'A' && gname.data[0] <= 'Z') {
	    cap_height = max(cap_height, (int)info.bbox.q.y);
	    if (gname.data[0] == 'I')
		bbox_I = info.bbox, have_I = true;
	} else if (gname.data[0] >= 'a' && gname.data[0] <= 'z') {
	    int y0 = (int)(info.bbox.p.y), y1 = (int)(info.bbox.q.y);

	    small_present = true;
	    switch (gname.data[0]) {
	    case 'b': case 'd': case 'f': case 'h':
	    case 'k': case 'l': case 't': /* ascender */
		small_height = max(small_height, y1);
	    case 'i':		/* anomalous ascent */
		break;
	    case 'j':		/* descender with anomalous ascent */
		small_descent = min(small_descent, y0);
		break;
	    case 'g': case 'p': case 'q': case 'y': /* descender */
		small_descent = min(small_descent, y0);
	    default:		/* no ascender or descender */
		x_height = max(x_height, y1);		
	    }
	}
    }
    if (!(desc.Flags & FONT_IS_SYMBOLIC)) {
	desc.Flags |= FONT_IS_ADOBE_ROMAN; /* required if not symbolic */
	desc.XHeight = (int)x_height;
	if (!small_present && (!pdev->PDFA || bfont->FontType != ft_TrueType))
	    desc.Flags |= FONT_IS_ALL_CAPS;
	desc.CapHeight = cap_height;
	/*
	 * Look at various glyphs to determine ItalicAngle, StemV,
	 * SERIF, SCRIPT, and ITALIC.
	 */
	if (have_colon && have_period) {
	    /* Calculate the dominant angle. */
	    int angle = 
		(int)(atan2((bbox_colon.q.y - bbox_colon.p.y) -
			      (bbox_period.q.y - bbox_period.p.y),
			    (bbox_colon.q.x - bbox_colon.p.x) -
			      (bbox_period.q.x - bbox_period.p.x)) *
		      radians_to_degrees) - 90;

	    /* Normalize to [-90..90]. */
	    while (angle > 90)
		angle -= 180;
	    while (angle < -90)
		angle += 180;
	    if (angle < -30)
		angle = -30;
	    else if (angle > 30)
		angle = 30;
	    /*
	     * For script or embellished fonts, we can get an angle that is
	     * slightly off from zero even for non-italic fonts.
	     * Compensate for this now.
	     */
	    if (angle <= 2 && angle >= -2)
		angle = 0;
	    desc.ItalicAngle = angle;
	}
	if (desc.ItalicAngle)
	    desc.Flags |= FONT_IS_ITALIC;
	if (have_I) {
	    double wdot = bbox_period.q.x - bbox_period.p.x;
	    double wcolon = bbox_I.q.x - bbox_I.p.x;
	    double wI = bbox_period.q.x - bbox_period.p.x;

	    desc.StemV = (int)wdot;
	    if (wI > wcolon * 2.5 || wI > (bbox_period.q.y - bbox_period.p.y) * 0.25)
		desc.Flags |= FONT_IS_SERIF;
	}
    }
    if (desc.Ascent == 0)
	desc.Ascent = desc.FontBBox.q.y;
    desc.Descent = desc.FontBBox.p.y;
    if (!(desc.Flags & (FONT_IS_SYMBOLIC | FONT_IS_ALL_CAPS)) &&
	(small_descent > desc.Descent / 3 || desc.XHeight > small_height * 0.9) &&
	(!pdev->PDFA || bfont->FontType != ft_TrueType)
	)
	desc.Flags |= FONT_IS_SMALL_CAPS;
    if (fixed_width > 0 && (!pdev->PDFA || bfont->FontType != ft_TrueType)) {
	desc.Flags |= FONT_IS_FIXED_WIDTH;
	desc.AvgWidth = desc.MaxWidth = desc.MissingWidth = fixed_width;
    }
    if (desc.CapHeight == 0)
	desc.CapHeight = desc.Ascent;
    if (desc.StemV == 0)
	desc.StemV = (int)(desc.FontBBox.q.x * 0.15);
    pfd->common.values = desc;
    return 0;
}

/*
 * Finish a FontDescriptor by computing the metric values, and then
 * writing the associated embedded font if any.
 */
int
pdf_finish_FontDescriptor(gx_device_pdf *pdev, pdf_resource_t *pres)
{
    pdf_font_descriptor_t *pfd = (pdf_font_descriptor_t *)pres;
    int code = 0;
    cos_dict_t *pcd = 0;
    if (pfd->common.object->id == -1)
	return 0;
    if (!pfd->common.object->written &&
	(code = pdf_compute_font_descriptor(pdev, pfd)) >= 0 &&
	(!pfd->embed ||
	 (code = pdf_write_embedded_font(pdev, pfd->base_font, 
				pfd->FontType,
				&pfd->common.values.FontBBox, 
				pfd->common.rid, &pcd)) >= 0)
	) {
        pdf_set_FontFile_object(pfd->base_font, pcd);
    }
    return code;
}

/* Write a FontDescriptor. */
int
pdf_write_FontDescriptor(gx_device_pdf *pdev, pdf_resource_t *pres)
{
    pdf_font_descriptor_t *pfd = (pdf_font_descriptor_t *)pres;
    font_type ftype = pfd->FontType;
    long cidset_id = 0;
    int code = 0;
    stream *s;

    if (pfd->common.object->written)
	return 0;
    if (pfd->common.object->id == -1)
	return 0;

    /* If this is a CIDFont subset, write the CIDSet now. */
    switch (ftype) {
    case ft_CID_encrypted:
    case ft_CID_TrueType:
	if (pdf_do_subset_font(pdev, pfd->base_font, pfd->common.rid)) {
	    code = pdf_write_CIDSet(pdev, pfd->base_font, &cidset_id);
	    if (code < 0)
		return code;
	}
    default:
	break;
    }

    {
	/*
	 * Hack: make all embedded subset TrueType fonts "symbolic" to
	 * work around undocumented assumptions in Acrobat Reader.
	 */
	pdf_font_descriptor_common_t fd;

	fd = pfd->common;
	if (pfd->embed && pfd->FontType == ft_TrueType && !pdev->PDFA &&
	    pdf_do_subset_font(pdev, pfd->base_font, pfd->common.rid)
	    )
	    fd.values.Flags =
		(fd.values.Flags & ~(FONT_IS_ADOBE_ROMAN)) | FONT_IS_SYMBOLIC;
	code = write_FontDescriptor_common(pdev, &fd);
    }
    if (code < 0)
	return code;
    s = pdev->strm;
    if (cidset_id != 0)
	pprintld1(s, "/CIDSet %ld 0 R\n", cidset_id);
    else if (pdf_do_subset_font(pdev, pfd->base_font, pfd->common.rid) &&
	     (ftype == ft_encrypted || ftype == ft_encrypted2)
	     ) {
	stream_puts(s, "/CharSet");
	code = pdf_write_CharSet(pdev, pfd->base_font);
	if (code < 0)
	    return code;
    }
    if (pfd->embed) {
	code = pdf_write_FontFile_entry(pdev, pfd->base_font);
	if (code < 0)
	    return code;
    }
    if (pfd->cid.Style) {
	stream_puts(s, "/Style");
	COS_WRITE(pfd->cid.Style, pdev);
    }
    if (pfd->cid.Lang[0]) {
	pprints1(s, "/Lang(%s)", pfd->cid.Lang);
    }
    if (pfd->cid.FD) {
	stream_puts(s, "/FD");
	COS_WRITE(pfd->cid.FD, pdev);
    }
    stream_puts(s, ">>\n");
    pdf_end_separate(pdev);
    pfd->common.object->written = true;
    {	const cos_object_t *pco = (const cos_object_t *)pdf_get_FontFile_object(pfd->base_font);
	if (pco != NULL) {
	    code = COS_WRITE_OBJECT(pco, pdev);
	    if (code < 0)
		return code;
	}
    }
    return 0;
}

/*
 * Release a FontDescriptor components.
 */
int
pdf_release_FontDescriptor_components(gx_device_pdf *pdev, pdf_resource_t *pres)
{
    pdf_font_descriptor_t *pfd = (pdf_font_descriptor_t *) pres;

    gs_free_object(pdev->pdf_memory, pfd->base_font, "pdf_release_FontDescriptor_components");
    pfd->base_font = NULL;
    /* fixme: underimplemented. */
    return 0;
}

/*
 * Mark a FontDescriptor used in a text.
 */
int 
pdf_mark_font_descriptor_used(gx_device_pdf *pdev, pdf_font_descriptor_t *pfd)
{
    if (pfd != NULL && pfd->common.object->id == -1)
	pdf_reserve_object_id(pdev, (pdf_resource_t *)&pfd->common, 0);
    return 0;
}

/*
 * Convert True Type font descriptor into CID font descriptor for PDF/A.
 */
int 
pdf_convert_truetype_font_descriptor(gx_device_pdf *pdev, pdf_font_resource_t *pdfont)
{
    pdf_font_descriptor_t *pfd = pdfont->FontDescriptor;
    pdf_base_font_t *pbfont = pfd->base_font;
    int num_CIDs = pbfont->num_glyphs;
    int length_CIDSet = (num_CIDs + 7) / 8;
    int length_CIDToGIDMap = num_CIDs * sizeof(ushort);
    gs_font *pfont = (gs_font *)pbfont->copied;
    gs_char ch;
    /* Save the simple font descriptor data because CID font data overlap them. */
    int FirstChar = pdfont->u.simple.FirstChar, LastChar = pdfont->u.simple.LastChar;
    pdf_encoding_element_t *Encoding = pdfont->u.simple.Encoding;

    pfd->FontType = ft_CID_TrueType;
    pdfont->u.simple.Encoding = NULL; /* Drop due to overlapping against a garbager problem. */
    pbfont->CIDSet = gs_alloc_bytes(pdev->pdf_memory, length_CIDSet, 
			"pdf_convert_truetype_font_descriptor");
    if (pbfont->CIDSet == NULL)
	return_error(gs_error_VMerror);
    memset(pbfont->CIDSet, 0, length_CIDSet);
    pdfont->u.cidfont.CIDToGIDMap = (ushort *)gs_alloc_bytes(pdev->pdf_memory, 
			length_CIDToGIDMap, "pdf_convert_truetype_font_descriptor");
    if (pdfont->u.cidfont.CIDToGIDMap == NULL)
	return_error(gs_error_VMerror);
    memset(pdfont->u.cidfont.CIDToGIDMap, 0, length_CIDToGIDMap);
    for (ch = FirstChar; ch <= LastChar; ch++) {
	if (Encoding[ch].glyph != GS_NO_GLYPH) {
	    gs_glyph glyph = pfont->procs.encode_char(pfont, ch, GLYPH_SPACE_INDEX);

	    pbfont->CIDSet[ch / 8] |= 0x80 >> (ch % 8);
	    pdfont->u.cidfont.CIDToGIDMap[ch] = glyph - GS_MIN_GLYPH_INDEX;
	}
    }
    pdfont->u.cidfont.Widths2 = NULL;
    pdfont->u.cidfont.used2 = NULL;
    pdfont->u.cidfont.v = NULL;
    return 0;
}
