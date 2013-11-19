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

/* $Id: ifont.h 9043 2008-08-28 22:48:19Z giles $ */
/* Interpreter internal font representation */

#ifndef ifont_INCLUDED
#  define ifont_INCLUDED

#include "gsccode.h"		/* for gs_glyph, NUM_KNOWN_ENCODINGS */
#include "gsstype.h"		/* for extern_st */

/* The external definition of fonts is given in the PostScript manual, */
/* pp. 91-93. */

/* The structure given below is 'client data' from the viewpoint */
/* of the library.  font-type objects (t_struct/st_font, "t_fontID") */
/* point directly to a gs_font.  */
/* Note: From the GC point of view, it is handled as an array of refs, followed by */
/*       non-relocatable data starting with u.type42.mru_sfnts_index; this also    */
/*       means all members of union _fs must start with the same number of refs    */

typedef struct font_data_s {
    ref dict;			/* font dictionary object */
    ref BuildChar;
    ref BuildGlyph;
    ref Encoding;
    ref CharStrings;
    ref GlyphNames2Unicode;
    union _fs {
	struct _f1 {
	    ref OtherSubrs;	/* from Private dictionary */
	    ref Subrs;		/* from Private dictionary */
	    ref GlobalSubrs;	/* from Private dictionary, */
	    /* for Type 2 charstrings */
	} type1;
	struct _f42 {
	    ref sfnts;
	    ref CIDMap;		/* for CIDFontType 2 fonts */
	    ref GlyphDirectory;
	    /* the following are used to optimize lookups into sfnts */
	    uint mru_sfnts_index;   /* index of most recently used sfnts string */
	    ulong mru_sfnts_pos;    /* data bytes before sfnts string at index mru_sfnts_index */
	} type42;
	struct _fc0 {
	    ref GlyphDirectory;
	    ref GlyphData;	/* (if preloaded) string or array of strings */
	    ref DataSource;	/* (if not preloaded) reusable stream */
	} cid0;
    } u;
} font_data;

/*
 * Even though the interpreter's part of the font data actually
 * consists of refs, allocating it as refs tends to create sandbars;
 * since it is always allocated and freed as a unit, we can treat it
 * as an ordinary structure.
 */
/* st_font_data is exported for zdefault_make_font in zfont.c. */
extern_st(st_font_data);
#define public_st_font_data()	/* in zbfont.c */\
  static struct_proc_clear_marks(font_data_clear_marks);\
  static struct_proc_enum_ptrs(font_data_enum_ptrs);\
  static struct_proc_reloc_ptrs(font_data_reloc_ptrs);\
  gs_public_st_complex_only(st_font_data, font_data, "font_data",\
    font_data_clear_marks, font_data_enum_ptrs, font_data_reloc_ptrs, 0)
#define pfont_data(pfont) ((font_data *)((pfont)->client_data))
#define pfont_dict(pfont) (&pfont_data(pfont)->dict)

/* ================Internal procedures shared across files ================ */

/* ---------------- Exported by zchar.c ---------------- */

/*
 * Get the FontBBox from a font dictionary, if any; if none, or if invalid,
 * return 4 zeros.
 */
int font_bbox_param(const gs_memory_t *mem, const ref *pfdict, double bbox[4]);

/* ---------------- Exported by zfont.c ---------------- */

#ifndef gs_font_DEFINED
#  define gs_font_DEFINED
typedef struct gs_font_s gs_font;
#endif

/*
 * Check a parameter that should be a valid font dictionary, and return
 * the gs_font stored in its FID entry.
 */
int font_param(const ref * pfdict, gs_font ** ppfont);

/*
 * Mark a glyph as a PostScript name (if it isn't a CID) for the garbage
 * collector.  Return true if a mark was just added.  This procedure is
 * intended to be used as the mark_glyph procedure in the character cache.
 */
bool zfont_mark_glyph_name(const gs_memory_t *mem, gs_glyph glyph, void *ignore_data);

/*
 * Return information about a font, including information from the FontInfo
 * dictionary.  This procedure is intended to be used as the font_info
 * procedure in all PostScript fonts.
 */
font_proc_font_info(zfont_info);

#endif /* ifont_INCLUDED */
