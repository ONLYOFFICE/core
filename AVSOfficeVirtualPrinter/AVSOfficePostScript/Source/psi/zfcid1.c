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

/* $Id: zfcid1.c 9306 2008-12-26 07:25:31Z alexcher $ */
/* CIDFontType 1 and 2 operators */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsmatrix.h"
#include "gsccode.h"
#include "gsstruct.h"
#include "gsgcache.h"
#include "gsutil.h"
#include "gxfcid.h"
#include "gxfcache.h"
#include "bfont.h"
#include "icid.h"
#include "idict.h"
#include "idparam.h"
#include "ifcid.h"
#include "ichar1.h"
#include "ifont42.h"
#include "store.h"
#include "stream.h"
#include "files.h"

/* ---------------- CIDFontType 1 (FontType 10) ---------------- */

/* <string|name> <font_dict> .buildfont10 <string|name> <font> */
static int
zbuildfont10(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    build_proc_refs build;
    int code = build_gs_font_procs(op, &build);
    gs_cid_system_info_t cidsi;
    gs_font_base *pfont;

    if (code < 0)
	return code;
    code = cid_font_system_info_param(&cidsi, op);
    if (code < 0)
	return code;
    make_null(&build.BuildChar);	/* only BuildGlyph */
    code = build_gs_simple_font(i_ctx_p, op, &pfont, ft_CID_user_defined,
				&st_gs_font_cid1, &build,
				bf_Encoding_optional |
				bf_UniqueID_ignored);
    if (code < 0)
	return code;
    ((gs_font_cid1 *)pfont)->cidata.CIDSystemInfo = cidsi;
    return define_gs_font((gs_font *)pfont);
}

/* ---------------- CIDFontType 2 (FontType 11) ---------------- */

/* ------ Accessing ------ */

/* Map a glyph CID to a TrueType glyph number using the CIDMap. */
static int
z11_CIDMap_proc(gs_font_cid2 *pfont, gs_glyph glyph)
{
    const ref *pcidmap = &pfont_data(pfont)->u.type42.CIDMap;
    ulong cid = glyph - gs_min_cid_glyph;
    int gdbytes = pfont->cidata.common.GDBytes;
    int gnum = 0;
    const byte *data;
    int i, code;
    ref rcid;
    ref *prgnum;

    switch (r_type(pcidmap)) {
    case t_string:
	if (cid >= r_size(pcidmap) / gdbytes)
	    return_error(e_rangecheck);
	data = pcidmap->value.const_bytes + cid * gdbytes;
	break;
    case t_integer:
	return cid + pcidmap->value.intval;
    case t_dictionary:
	make_int(&rcid, cid);
	code = dict_find(pcidmap, &rcid, &prgnum);
	if (code <= 0)
	    return (code < 0 ? code : gs_note_error(e_undefined));
	if (!r_has_type(prgnum, t_integer))
	    return_error(e_typecheck);
	return prgnum->value.intval;
    default:			/* array type */
        code = string_array_access_proc(pfont->memory, pcidmap, 1, cid * gdbytes,
					gdbytes, NULL, NULL, &data);

	if (code < 0)
	    return code;
	if ( code > 0 ) 
	    return_error(e_invalidfont);
    }
    for (i = 0; i < gdbytes; ++i)
	gnum = (gnum << 8) + data[i];
    if (gnum >= pfont->data.trueNumGlyphs)
	return_error(e_invalidfont);
    return gnum;
}

/* Handle MetricsCount when accessing outline or metrics information. */
static int
z11_get_outline(gs_font_type42 * pfont, uint glyph_index,
		gs_glyph_data_t *pgd)
{
    gs_font_cid2 *const pfcid = (gs_font_cid2 *)pfont;
    int skip = pfcid->cidata.MetricsCount << 1;
    int code = pfcid->cidata.orig_procs.get_outline(pfont, glyph_index, pgd);

    if (code >= 0) {
	uint size = pgd->bits.size;

	if (size <= skip) {
	    gs_glyph_data_free(pgd, "z11_get_outline");
	    gs_glyph_data_from_null(pgd);
	} else {
	    gs_glyph_data_substring(pgd, skip, size - skip);
	}
    }
    return code;
}

#define GET_U16_MSB(p) (((uint)((p)[0]) << 8) + (p)[1])
#define GET_S16_MSB(p) (int)((GET_U16_MSB(p) ^ 0x8000) - 0x8000)

static int
z11_get_metrics(gs_font_type42 * pfont, uint glyph_index, 
		gs_type42_metrics_options_t options, float sbw[4])
{
    gs_font_cid2 *const pfcid = (gs_font_cid2 *)pfont;
    int skip = pfcid->cidata.MetricsCount << 1;
    gs_glyph_data_t gdata;
    const byte *pmetrics;
    int lsb, width;
    int code = 0;
    int wmode = gs_type42_metrics_options_wmode(options);

    gdata.memory = pfont->memory;
    if (wmode >= skip >> 2 ||
	(code = pfcid->cidata.orig_procs.get_outline(pfont, glyph_index, &gdata)) < 0 ||
	gdata.bits.size < skip
	)
	return pfcid->cidata.orig_procs.get_metrics(pfont, glyph_index, options, sbw);
    if(gs_type42_metrics_options_bbox_requested(options)) {
	code = pfcid->cidata.orig_procs.get_metrics(pfont, glyph_index, 
			gs_type42_metrics_options_BBOX, sbw);;
	if (code < 0)
	    return code;
    }
    if (gs_type42_metrics_options_sbw_requested(options)) {
	pmetrics = gdata.bits.data + skip - 4 - (wmode << 2);
	lsb = GET_S16_MSB(pmetrics + 2);
	width = GET_U16_MSB(pmetrics + 0);
	{
	    double factor = 1.0 / pfont->data.unitsPerEm;

	    if (wmode) {
		sbw[0] = 0, sbw[1] = -lsb * factor;
		sbw[2] = 0, sbw[3] = -width * factor;
	    } else {
		sbw[0] = lsb * factor, sbw[1] = 0;
		sbw[2] = width * factor, sbw[3] = 0;
	    }
	}
    }
    gs_glyph_data_free(&gdata, "z11_get_metrics");
    return 0;
}

static int
z11_glyph_info_aux(gs_font *font, gs_glyph glyph, const gs_matrix *pmat,
		     int members, gs_glyph_info_t *info)
{
    gs_font_cid2 *fontCID2 = (gs_font_cid2 *)font;
    uint glyph_index;
    int code = (glyph > GS_MIN_GLYPH_INDEX 
	    ? glyph - GS_MIN_GLYPH_INDEX 
	    : fontCID2->cidata.CIDMap_proc(fontCID2, glyph));

    if(code < 0)
	return code;
    glyph_index = (uint)code;
    return gs_type42_glyph_info_by_gid(font, glyph, pmat, members, info, glyph_index);
}

static int
z11_glyph_info(gs_font *font, gs_glyph glyph, const gs_matrix *pmat,
		     int members, gs_glyph_info_t *info)
{
    int wmode = (members & GLYPH_INFO_WIDTH0 ? 0 : 1);

    return z1_glyph_info_generic(font, glyph, pmat, members, info, 
				    &z11_glyph_info_aux, wmode);
}


/* Enumerate glyphs (keys) from GlyphDirectory instead of loca / glyf. */
static int
z11_enumerate_glyph(gs_font *font, int *pindex,
			 gs_glyph_space_t glyph_space, gs_glyph *pglyph)
{
    gs_font_cid2 *pfont = (gs_font_cid2 *)font;
    int code0 = z11_CIDMap_proc(pfont, GS_MIN_CID_GLYPH);
    int code;

    if(*pindex > pfont->cidata.common.CIDCount)
	return_error(e_rangecheck);

    for (;;) {
	code = z11_CIDMap_proc(pfont, GS_MIN_CID_GLYPH + *pindex);

	if (code < 0) {
	    *pindex = 0;
	    return 0;
	}
	(*pindex)++;
	if (*pindex == 1 || code != code0)
	    break;
	/* else skip an underfined glyph */
    }
    if (glyph_space == GLYPH_SPACE_INDEX)
	*pglyph = GS_MIN_GLYPH_INDEX + (uint)code;
    else
	*pglyph = GS_MIN_CID_GLYPH + (uint)(*pindex - 1);
    return 0;
}

static uint
z11_get_glyph_index(gs_font_type42 *pfont, gs_glyph glyph)
{
    int code = z11_CIDMap_proc((gs_font_cid2 *)pfont, glyph);

    return (code < 0 ? 0 /* notdef */: (uint)code);
}


static int
z11_glyph_outline(gs_font *font, int WMode, gs_glyph glyph, const gs_matrix *pmat,
		  gx_path *ppath, double sbw[4])
{
    return gs_type42_glyph_outline(font, WMode, 
	    z11_get_glyph_index((gs_font_type42 *)font, glyph) + GS_MIN_GLYPH_INDEX,
				   pmat, ppath, sbw);
}

static int 
get_subst_CID_on_WMode(gs_subst_CID_on_WMode_t *subst, ref *t, int WMode)
{
    ref r, *a, e;;

    make_int(&r, WMode);
    if (dict_find(t, &r, &a) > 0 && r_type(a) == t_array) {
	int n = r_size(a), i; 
	uint *s;

	s = (uint *)gs_alloc_byte_array(subst->rc.memory, n, sizeof(int), "zbuildfont11");
	if (subst == NULL)
	    return_error(e_VMerror);
	for (i = 0; i < n; i++) {
	    array_get(subst->rc.memory, a, (long)i, &e);
	    if (r_type(&e) != t_integer)
		return_error(e_invalidfont);
	    s[i] = e.value.intval;
	}
	subst->data[WMode] = s;
	subst->size[WMode] = n;
    }
    return 0;
}

static GS_NOTIFY_PROC(release_subst_CID_on_WMode);

static int
release_subst_CID_on_WMode(void *data, void *event)
{
    gs_font_cid2 *pfcid = (gs_font_cid2 *)data;
    gs_subst_CID_on_WMode_t *subst = pfcid->subst_CID_on_WMode;

    gs_font_notify_unregister((gs_font *)pfcid, release_subst_CID_on_WMode, data);
    pfcid->subst_CID_on_WMode = NULL;
    rc_adjust(subst, -2, "release_subst_CID_on_WMode");
    return 0;
}

static uint 
font11_substitute_glyph_index_vertical(gs_font_type42 *pfont, uint glyph_index,
					  int WMode, gs_glyph glyph)
{
    gs_font_cid2 *pfcid = (gs_font_cid2 *)pfont;
    uint cid = (glyph >= GS_MIN_CID_GLYPH ? glyph - GS_MIN_CID_GLYPH : glyph);
    int WMode1 = !WMode;\
    gs_subst_CID_on_WMode_t *s = pfcid->subst_CID_on_WMode;
    
    if (s != NULL) {
	uint *subst = s->data[WMode1];
	int bi, ei, i;

	/* Binary search for cid in subst (2-int elements) : */
	bi = 0;
	ei = pfcid->subst_CID_on_WMode->size[WMode1];

	if (ei > 0) {
	    for (;;) {
		i = ((bi + ei) / 2) & ~1;
		if (subst[i] == cid) {
		    WMode = WMode1;
		    break;
		}
		if (bi + 2 >= ei)
		    break;
		if (subst[i] > cid)
		    ei = i;
		else
		    bi = i;
	    }
	}
    }
    return gs_type42_substitute_glyph_index_vertical(pfont, glyph_index, WMode, glyph);
}

/* ------ Defining ------ */

extern_st(st_subst_CID_on_WMode);

/* <string|name> <font_dict> .buildfont11 <string|name> <font> */
static int
zbuildfont11(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_font_cid_data common;
    gs_font_type42 *pfont;
    gs_font_cid2 *pfcid;
    int MetricsCount;
    ref rcidmap, ignore_gdir, file, *pfile, cfnstr, *pCIDFontName, CIDFontName, *t;
    ulong loca_glyph_pos[2][2];
    int code = cid_font_data_param(op, &common, &ignore_gdir);

    if (code < 0 ||
	(code = dict_find_string(op, "CIDFontName", &pCIDFontName)) <= 0 ||
	(code = dict_int_param(op, "MetricsCount", 0, 4, 0, &MetricsCount)) < 0
	)
	return code;
    /*
     * Since build_gs_simple_font may resize the dictionary and cause
     * pointers to become invalid, save CIDFontName
     */
    CIDFontName = *pCIDFontName;
    if (MetricsCount & 1)	/* only allowable values are 0, 2, 4 */
	return_error(e_rangecheck);
    code = dict_find_string(op, "File", &pfile);
    if (code < 0 && code != e_undefined)
	return code;
    if (code > 0) {
	ref *file_table_pos, *a, v;
	const char *name[2] = {"loca", "glyf"};
	int i, j;
	
	/*
	 * Since build_gs_simple_font may resize the dictionary and cause
	 * pointers to become invalid, save File and CIDFontName
	 */
	file = *pfile;
        check_read_type(file, t_file);
	code = dict_find_string(op, "file_table_pos", &file_table_pos);
	if (code <= 0 || r_type(file_table_pos) != t_dictionary)
	    return_error(e_invalidfont);
	for (i = 0; i < 2; i++) {
	    code = dict_find_string(file_table_pos, name[i], &a);
	    if (code <= 0 || r_type(a) != t_array)
		return_error(e_invalidfont);
	    for (j = 0; j < 2; j++) {
	        code = array_get(imemory, a, j, &v); 
		if (code < 0 || r_type(&v) != t_integer)
		    return_error(e_invalidfont);
		loca_glyph_pos[i][j] = v.value.intval;
	    }
	}
    } else
	pfile = NULL;
    code = font_string_array_param(imemory, op, "CIDMap", &rcidmap);
    switch (code) {
    case 0:			/* in PLRM3 */
    gdb:
	/* GDBytes is required for indexing a string or string array. */
	if (common.GDBytes == 0)
	    return_error(e_rangecheck);
	break;
    default:
	return code;
    case e_typecheck:
	switch (r_type(&rcidmap)) {
	case t_string:		/* in PLRM3 */
	    goto gdb;
	case t_dictionary:	/* added in 3011 */
	case t_integer:		/* added in 3011 */
	    break;
	default:
	    return code;
	}
	break;
    }
    code = build_gs_TrueType_font(i_ctx_p, op, &pfont, ft_CID_TrueType,
				  &st_gs_font_cid2,
				  (const char *)0, "%Type11BuildGlyph",
				  bf_Encoding_optional |
				  bf_UniqueID_ignored |
				  bf_CharStrings_optional |
				  (pfile != NULL ? bf_has_font_file : 0));
    if (code < 0)
	return code;
    pfcid = (gs_font_cid2 *)pfont;
    if (dict_find_string(op, "subst_CID_on_WMode", &t) > 0 && r_type(t) == t_dictionary) {
	gs_subst_CID_on_WMode_t *subst = NULL;
	ref *o;
	gs_font *font;

	if (dict_find_string(t, "Ordering", &o) <= 0 || r_type(o) != t_string)
	    return_error(e_invalidfont);
	for (font = ifont_dir->orig_fonts; font != NULL; font = font->next) {
	    if (font->FontType == ft_CID_TrueType) {
		gs_font_cid2 *pfcid1 = (gs_font_cid2 *)font;
		if (pfcid1->subst_CID_on_WMode != NULL &&
		    bytes_compare(o->value.const_bytes, r_size(o),
			    pfcid1->cidata.common.CIDSystemInfo.Ordering.data, 
			    pfcid1->cidata.common.CIDSystemInfo.Ordering.size)) {
		    subst = pfcid1->subst_CID_on_WMode;
		    break;
		}
	    }
	}
	if (subst == NULL) {
	    rc_alloc_struct_1(subst, gs_subst_CID_on_WMode_t, &st_subst_CID_on_WMode, 
			    pfcid->memory, return_error(e_VMerror), "zbuildfont11");
	    subst->data[0] = subst->data[1] = 0;
	    pfcid->subst_CID_on_WMode = subst;
	    code = get_subst_CID_on_WMode(subst, t, 0);
	    if (code < 0)
		return code;
	    code = get_subst_CID_on_WMode(subst, t, 1);
	    if (code < 0)
		return code;
	} else {
	    pfcid->subst_CID_on_WMode = subst;
	    rc_increment(subst);
	}
	code = gs_font_notify_register((gs_font *)pfcid, release_subst_CID_on_WMode, (void *)pfcid);
	if (code < 0)
	    return code;
	rc_increment(subst);
   }
    pfcid->cidata.common = common;
    pfcid->cidata.MetricsCount = MetricsCount;
    ref_assign(&pfont_data(pfont)->u.type42.CIDMap, &rcidmap);
    pfcid->cidata.CIDMap_proc = z11_CIDMap_proc;
    pfcid->data.substitute_glyph_index_vertical = font11_substitute_glyph_index_vertical;
    pfont->procs.enumerate_glyph = z11_enumerate_glyph;
    pfont->procs.glyph_info = z11_glyph_info;
    pfont->procs.glyph_outline = z11_glyph_outline;
    pfont->data.get_glyph_index = z11_get_glyph_index;
    if (pfcid->font_name.size == 0) {
	get_font_name(imemory, &cfnstr, &CIDFontName);
	copy_font_name(&pfcid->font_name, &cfnstr);
    }
    if (MetricsCount) {
	/* "Wrap" the glyph accessor procedures. */
	pfcid->cidata.orig_procs.get_outline = pfont->data.get_outline;
	pfont->data.get_outline = z11_get_outline;
	pfcid->cidata.orig_procs.get_metrics = pfont->data.get_metrics;
	pfont->data.get_metrics = z11_get_metrics;
    } else if(pfile != NULL) {
        /* 
	 * We assume that disk fonts has no MetricsCount.
	 * We could do not, but the number of virtual function wariants increases.
	 */
	stream *s;

	check_read_file(s, &file);
	pfont->data.loca = loca_glyph_pos[0][0];
	pfont->data.glyf = loca_glyph_pos[1][0];
	pfont->data.get_outline = gs_get_glyph_data_cached;
   	pfont->data.gdcache = gs_glyph_cache__alloc(pfont, s, gs_type42_get_outline_from_TT_file);
    }
    return define_gs_font((gs_font *)pfont);
}

/* <cid11font> <cid> .type11mapcid <glyph_index> */
static int
ztype11mapcid(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_font *pfont;
    int code = font_param(op - 1, &pfont);

    if (code < 0)
	return code;
    check_type(*op, t_integer);
#if defined(TEST)
    /* Allow a Type 42 font here, for testing .wrapfont. */
    if (pfont->FontType == ft_TrueType) {
	/* Use the CID as the glyph index. */
	if (op->value.intval < 0 ||
	    op->value.intval >= ((gs_font_type42 *)pfont)->data.numGlyphs
	    )
	    return_error(e_rangecheck);
	code = (int)op->value.intval;
    } else
#endif
    {
	if (pfont->FontType != ft_CID_TrueType)
	    return_error(e_invalidfont);
	code = z11_CIDMap_proc((gs_font_cid2 *)pfont,
			(gs_glyph)(gs_min_cid_glyph + op->value.intval));
    }
    if (code < 0)
	return code;
    make_int(op - 1, code);
    pop(1);
    return 0;
}

/* <Decoding> <TT_cmap> <SubstNWP> <GDBytes> <CIDMap> .fillCIDMap - */
static int
zfillCIDMap(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    ref *Decoding = op - 4, *TT_cmap = op - 3, *SubstNWP = op - 2, 
        *GDBytes = op - 1, *CIDMap = op;
    int code;

    check_type(*Decoding, t_dictionary);
    check_type(*TT_cmap, t_dictionary);
    check_type(*SubstNWP, t_array);
    check_type(*GDBytes, t_integer);
    check_type(*CIDMap, t_array);
    code = cid_fill_CIDMap(imemory, Decoding, TT_cmap, SubstNWP, GDBytes->value.intval, CIDMap);
    pop(5);
    return code;
}

/* ------ Initialization procedure ------ */

const op_def zfcid1_op_defs[] =
{
    {"2.buildfont10", zbuildfont10},
    {"2.buildfont11", zbuildfont11},
    {"2.type11mapcid", ztype11mapcid},
    {"2.fillCIDMap", zfillCIDMap},
    op_def_end(0)
};
