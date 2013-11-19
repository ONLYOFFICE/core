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

/*$Id: zchar.c 9222 2008-11-16 14:47:49Z leonardo $ */
/* Character operators */
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "gstext.h"
#include "gxarith.h"
#include "gxfixed.h"
#include "gxmatrix.h"		/* for ifont.h */
#include "gxdevice.h"		/* for gxfont.h */
#include "gxfont.h"
#include "gxfont42.h"
#include "gxfont0.h"
#include "gzstate.h"
#include "dstack.h"		/* for stack depth */
#include "estack.h"
#include "ialloc.h"
#include "ichar.h"
#include "ichar1.h"
#include "idict.h"
#include "ifont.h"
#include "igstate.h"
#include "ilevel.h"
#include "iname.h"
#include "ipacked.h"
#include "store.h"
#include "zchar42.h"

extern bool CPSI_mode;

/* Forward references */
static bool map_glyph_to_char(const gs_memory_t *mem, 
			       const ref *, const ref *, ref *);
static int finish_show(i_ctx_t *);
static int op_show_cleanup(i_ctx_t *);
static int op_show_return_width(i_ctx_t *, uint, double *);

/* <string> show - */
static int
zshow(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_text_enum_t *penum;
    int code = op_show_setup(i_ctx_p, op);

    if (code != 0 ||
	(code = gs_show_begin(igs, op->value.bytes, r_size(op), imemory, &penum)) < 0)
	return code;
    *(op_proc_t *)&penum->enum_client_data = zshow;
    if ((code = op_show_finish_setup(i_ctx_p, penum, 1, finish_show)) < 0) {
	ifree_object(penum, "op_show_enum_setup");
	return code;
    }
    return op_show_continue_pop(i_ctx_p, 1);
}

/* <ax> <ay> <string> ashow - */
static int
zashow(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_text_enum_t *penum;
    double axy[2];
    int code = num_params(op - 1, 2, axy);

    if (code < 0 ||
	(code = op_show_setup(i_ctx_p, op)) != 0 ||
	(code = gs_ashow_begin(igs, axy[0], axy[1], op->value.bytes, r_size(op), imemory, &penum)) < 0)
	return code;
    *(op_proc_t *)&penum->enum_client_data = zashow;
    if ((code = op_show_finish_setup(i_ctx_p, penum, 3, finish_show)) < 0) {
	ifree_object(penum, "op_show_enum_setup");
	return code;
    }
    return op_show_continue_pop(i_ctx_p, 3);
}

/* <cx> <cy> <char> <string> widthshow - */
static int
zwidthshow(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_text_enum_t *penum;
    double cxy[2];
    int code;

    if ((code = op_show_setup(i_ctx_p, op)) != 0 )
	return code;
    check_type(op[-1], t_integer);
    if (gs_currentfont(igs)->FontType == ft_composite) {
        if ((gs_char) (op[-1].value.intval) != op[-1].value.intval)
	    return_error(e_rangecheck);
    } else {
        if (op[-1].value.intval < 0 || op[-1].value.intval > 255)
	    return_error(e_rangecheck); /* per PLRM and CET 13-26 */
    }
    if ((code = num_params(op - 2, 2, cxy)) < 0 )
        return code;
    if ((code = gs_widthshow_begin(igs, cxy[0], cxy[1],
				   (gs_char) op[-1].value.intval,
				   op->value.bytes, r_size(op),
				   imemory, &penum)) < 0)
	return code;
    *(op_proc_t *)&penum->enum_client_data = zwidthshow;
    if ((code = op_show_finish_setup(i_ctx_p, penum, 4, finish_show)) < 0) {
	ifree_object(penum, "op_show_enum_setup");
	return code;
    }
    return op_show_continue_pop(i_ctx_p, 4);
}

/* <cx> <cy> <char> <ax> <ay> <string> awidthshow - */
static int
zawidthshow(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_text_enum_t *penum;
    double cxy[2], axy[2];
    int code;

    if ((code = op_show_setup(i_ctx_p, op)) != 0 )
	return code;
    if ((code = num_params(op - 1, 2, axy)) < 0 )
	return code;
    check_type(op[-3], t_integer);
    if (gs_currentfont(igs)->FontType == ft_composite) {
        if ((gs_char) (op[-3].value.intval) != op[-3].value.intval)
	    return_error(e_rangecheck);
    } else {
        if (op[-3].value.intval < 0 || op[-3].value.intval > 255)
	    return_error(e_rangecheck); /* per PLRM and CET 13-02 */
    }
    if ((code = num_params(op - 4, 2, cxy)) < 0 )
	return code;
    if ((code = gs_awidthshow_begin(igs, cxy[0], cxy[1],
				    (gs_char) op[-3].value.intval,
				    axy[0], axy[1],
				    op->value.bytes, r_size(op),
				    imemory, &penum)) < 0)
	return code;
    *(op_proc_t *)&penum->enum_client_data = zawidthshow;
    if ((code = op_show_finish_setup(i_ctx_p, penum, 6, finish_show)) < 0) {
	ifree_object(penum, "op_show_enum_setup");
	return code;
    }
    return op_show_continue_pop(i_ctx_p, 6);
}

/* <proc> <string> kshow - */
static int
zkshow(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_text_enum_t *penum;
    int code;

    check_read_type(*op, t_string);
    check_proc(op[-1]);
    /*
     * Per PLRM Section xx.x, kshow is illegal if the current font is a
     * composite font.  The graphics library does not have this limitation,
     * so we check for it here.
     */
    if (gs_currentfont(igs)->FontType == ft_composite)
	return_error(e_invalidfont);
    if ((code = op_show_setup(i_ctx_p, op)) != 0 ||
	(code = gs_kshow_begin(igs, op->value.bytes, r_size(op),
			       imemory, &penum)) < 0)
	return code;
    *(op_proc_t *)&penum->enum_client_data = zkshow;
    if ((code = op_show_finish_setup(i_ctx_p, penum, 2, finish_show)) < 0) {
	ifree_object(penum, "op_show_enum_setup");
	return code;
    }
    sslot = op[-1];		/* save kerning proc */
    return op_show_continue_pop(i_ctx_p, 2);
}

/* Common finish procedure for all show operations. */
/* Doesn't have to do anything. */
static int
finish_show(i_ctx_t *i_ctx_p)
{
    return 0;
}

/* <string> stringwidth <wx> <wy> */
static int
zstringwidth(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_text_enum_t *penum;
    int code = op_show_setup(i_ctx_p, op);

    if (code != 0 ||
	(code = gs_stringwidth_begin(igs, op->value.bytes, r_size(op),
				     imemory, &penum)) < 0)
	return code;
    *(op_proc_t *)&penum->enum_client_data = zstringwidth;
    if ((code = op_show_finish_setup(i_ctx_p, penum, 1, finish_stringwidth)) < 0) {
	ifree_object(penum, "op_show_enum_setup");
	return code;
    }
    return op_show_continue_pop(i_ctx_p, 1);
}
/* Finishing procedure for stringwidth. */
/* Pushes the accumulated width. */
/* This is exported for .glyphwidth (in zcharx.c). */
int
finish_stringwidth(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_point width;

    gs_text_total_width(senum, &width);
    push(2);
    make_real(op - 1, width.x);
    make_real(op, width.y);
    return 0;
}

/* Common code for charpath and .charboxpath. */
static int
zchar_path(i_ctx_t *i_ctx_p, op_proc_t proc, 
	   int (*begin)(gs_state *, const byte *, uint,
			bool, gs_memory_t *, gs_text_enum_t **))
{
    os_ptr op = osp;
    gs_text_enum_t *penum;
    int code;

    check_type(*op, t_boolean);
    code = op_show_setup(i_ctx_p, op - 1);
    if (code != 0 ||
	(code = begin(igs, op[-1].value.bytes, r_size(op - 1),
		      op->value.boolval, imemory, &penum)) < 0)
	return code;
    *(op_proc_t *)&penum->enum_client_data = proc;
    if ((code = op_show_finish_setup(i_ctx_p, penum, 2, finish_show)) < 0) {
	ifree_object(penum, "op_show_enum_setup");
	return code;
    }
    return op_show_continue_pop(i_ctx_p, 2);
}
/* <string> <outline_bool> charpath - */
static int
zcharpath(i_ctx_t *i_ctx_p)
{
    return zchar_path(i_ctx_p, zcharpath, gs_charpath_begin);
}
/* <string> <box_bool> .charboxpath - */
static int
zcharboxpath(i_ctx_t *i_ctx_p)
{
    return zchar_path(i_ctx_p, zcharboxpath, gs_charboxpath_begin);
}

/* <wx> <wy> <llx> <lly> <urx> <ury> setcachedevice - */
int
zsetcachedevice(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    double wbox[6];
    gs_text_enum_t *penum = op_show_find(i_ctx_p);
    int code = num_params(op, 6, wbox);

    if (penum == 0)
	return_error(e_undefined);
    if (code < 0)
	return code;
    if (zchar_show_width_only(penum))
	return op_show_return_width(i_ctx_p, 6, &wbox[0]);
    code = gs_text_setcachedevice(penum, wbox);
    if (code < 0)
	return code;
    pop(6);
    if (code == 1)
	clear_pagedevice(istate);
    return 0;
}

/* <w0x> <w0y> <llx> <lly> <urx> <ury> <w1x> <w1y> <vx> <vy> setcachedevice2 - */
int
zsetcachedevice2(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    double wbox[10];
    gs_text_enum_t *penum = op_show_find(i_ctx_p);
    int code = num_params(op, 10, wbox);

    if (penum == 0)
	return_error(e_undefined);
    if (code < 0)
	return code;
    if (zchar_show_width_only(penum))
	return op_show_return_width(i_ctx_p, 10,
				    (gs_rootfont(igs)->WMode ?
				     &wbox[6] : &wbox[0]));
    code = gs_text_setcachedevice2(penum, wbox);
    if (code < 0)
	return code;
    pop(10);
    if (code == 1)
	clear_pagedevice(istate);
    return 0;
}

/* <wx> <wy> setcharwidth - */
static int
zsetcharwidth(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    double width[2];
    gs_text_enum_t *penum = op_show_find(i_ctx_p);
    int code = num_params(op, 2, width);

    if (penum == 0)
	return_error(e_undefined);
    if (code < 0)
	return code;
    if (zchar_show_width_only(penum))
	return op_show_return_width(i_ctx_p, 2, &width[0]);
    code = gs_text_setcharwidth(penum, width);
    if (code < 0)
	return code;
    pop(2);
    return 0;
}

/* <dict> .fontbbox <llx> <lly> <urx> <ury> -true- */
/* <dict> .fontbbox -false- */
static int
zfontbbox(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    double bbox[4];
    int code;

    check_type(*op, t_dictionary);
    check_dict_read(*op);
    code = font_bbox_param(imemory, op, bbox);
    if (code < 0)
	return code;
    if (bbox[0] < bbox[2] && bbox[1] < bbox[3]) {
	push(4);
	make_reals(op - 4, bbox, 4);
	make_true(op);
    } else {			/* No bbox, or an empty one. */
	make_false(op);
    }
    return 0;
}

/* Export in_cachedevice flag for PDF interpreter, which, unlike
 * PS unterpreter, ignores color operations in the inappropriate context.
 */
/* - .incachedevice <bool> */
static int
zincachedevice(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_bool(op, !!igs->in_cachedevice);
    return 0;
}


/* ------ Initialization procedure ------ */

const op_def zchar_a_op_defs[] =
{
    {"3ashow", zashow},
    {"6awidthshow", zawidthshow},
    {"2charpath", zcharpath},
    {"2.charboxpath", zcharboxpath},
    {"2kshow", zkshow},
    {"6setcachedevice", zsetcachedevice},
    {":setcachedevice2", zsetcachedevice2},
    {"2setcharwidth", zsetcharwidth},
    {"1show", zshow},
    {"1stringwidth", zstringwidth},
    {"4widthshow", zwidthshow},
		/* Extensions */
    {"1.fontbbox", zfontbbox},
		/* Internal operators */
    {"0%finish_show", finish_show},
    {"0%finish_stringwidth", finish_stringwidth},
    {"0%op_show_continue", op_show_continue},
    op_def_end(0)
};

const op_def zchar_b_op_defs[] =
{
    {"0.incachedevice", zincachedevice},
    op_def_end(0)
};

/* ------ Subroutines ------ */

/* Most of these are exported for zchar2.c. */

/* Convert a glyph to a ref. */
void
glyph_ref(const gs_memory_t *mem, gs_glyph glyph, ref * gref)
{
    if (glyph < gs_min_cid_glyph)
        name_index_ref(mem, glyph, gref);
    else
	make_int(gref, glyph - gs_min_cid_glyph);
}

/* Prepare to set up for a text operator. */
/* Don't change any state yet. */
int
op_show_setup(i_ctx_t *i_ctx_p, os_ptr op)
{
    check_read_type(*op, t_string);
    return op_show_enum_setup(i_ctx_p);
}
int
op_show_enum_setup(i_ctx_t *i_ctx_p)
{
    check_estack(snumpush + 2);
    return 0;
}

/* Finish setting up a text operator. */
int
op_show_finish_setup(i_ctx_t *i_ctx_p, gs_text_enum_t * penum, int npop,
		     op_proc_t endproc /* end procedure */ )
{
    gs_text_enum_t *osenum = op_show_find(i_ctx_p);
    es_ptr ep = esp + snumpush;
    gs_glyph glyph;
    extern bool CPSI_mode;

    if (CPSI_mode) {
	/* CET 14-03.PS page 2 emits rangecheck before rendering a character.
	   Early check the text to font compatibility 
	   with decomposing the text into characters.*/
	int code = gs_text_count_chars(igs, gs_get_text_params(penum), imemory);

	if (code < 0)
	    return code;
    }
    /*
     * If we are in the procedure of a cshow for a CID font and this is
     * a show operator, do something special, per the Red Book.
     */
    if (osenum &&
	SHOW_IS_ALL_OF(osenum,
		       TEXT_FROM_STRING | TEXT_DO_NONE | TEXT_INTERVENE) &&
	SHOW_IS_ALL_OF(penum, TEXT_FROM_STRING | TEXT_RETURN_WIDTH) &&
	(glyph = gs_text_current_glyph(osenum)) != gs_no_glyph &&
	glyph >= gs_min_cid_glyph &&

        /* According to PLRM, we don't need to raise a rangecheck error,
           if currentfont is changed in the proc of the operator 'cshow'. */
	gs_default_same_font (gs_text_current_font(osenum), 
			      gs_text_current_font(penum), true)
	) {
	gs_text_params_t text;

	if (!(penum->text.size == 1 &&
	      penum->text.data.bytes[0] ==
	        (gs_text_current_char(osenum) & 0xff))
	    )
	    return_error(e_rangecheck);
	text = penum->text;
	text.operation =
	    (text.operation &
	     ~(TEXT_FROM_STRING | TEXT_FROM_BYTES | TEXT_FROM_CHARS |
	       TEXT_FROM_GLYPHS | TEXT_FROM_SINGLE_CHAR)) |
	    TEXT_FROM_SINGLE_GLYPH;
	text.data.d_glyph = glyph;
	text.size = 1;
	gs_text_restart(penum, &text);
    }
    if (osenum && osenum->current_font->FontType == ft_user_defined &&
	osenum->orig_font->FontType == ft_composite &&
	((const gs_font_type0 *)osenum->orig_font)->data.FMapType == fmap_CMap) {
	/* A special behavior defined in PLRM3 section 5.11 page 389. */
	penum->outer_CID = osenum->returned.current_glyph;
    }
    if (osenum == NULL && !(penum->text.operation & (TEXT_FROM_GLYPHS | TEXT_FROM_SINGLE_GLYPH))) {
        int ft = igs->root_font->FontType;
 
        if ((ft >= ft_CID_encrypted && ft <= ft_CID_TrueType) || ft == ft_CID_bitmap)
            return_error(e_typecheck);
    }
    make_mark_estack(ep - (snumpush - 1), es_show, op_show_cleanup);
    if (endproc == NULL)
	endproc = finish_show;
    make_null(&esslot(ep));
    make_int(&esodepth(ep), ref_stack_count_inline(&o_stack) - npop); /* Save stack depth for */
    make_int(&esddepth(ep), ref_stack_count_inline(&d_stack));        /* correct interrupt processing */
    make_int(&esgslevel(ep), igs->level);
    make_null(&essfont(ep));
    make_null(&esrfont(ep));
    make_op_estack(&eseproc(ep), endproc);
    make_istruct(ep, 0, penum);
    esp = ep;
    return 0;
}

/* Continuation operator for character rendering. */
int
op_show_continue(i_ctx_t *i_ctx_p)
{
    int code = gs_text_update_dev_color(igs, senum);

    if (code >= 0)
	code = op_show_continue_dispatch(i_ctx_p, 0, gs_text_process(senum));
    return code;
}
int
op_show_continue_pop(i_ctx_t *i_ctx_p, int npop)
{
    return op_show_continue_dispatch(i_ctx_p, npop, gs_text_process(senum));
}
/*
 * Note that op_show_continue_dispatch sets osp = op explicitly iff the
 * dispatch succeeds.  This is so that the show operators don't pop anything
 * from the o-stack if they don't succeed.  Note also that if it returns an
 * error, it has freed the enumerator.
 */
int
op_show_continue_dispatch(i_ctx_t *i_ctx_p, int npop, int code)
{
    os_ptr op = osp - npop;
    gs_text_enum_t *penum = senum;

    switch (code) {
	case 0: {		/* all done */
	    os_ptr save_osp = osp;

	    osp = op;
	    code = (*real_opproc(&seproc)) (i_ctx_p);
	    op_show_free(i_ctx_p, code);
	    if (code < 0) {
		osp = save_osp;
		return code;
	    }
	    return o_pop_estack;
	}
	case TEXT_PROCESS_INTERVENE: {
	    ref *pslot = &sslot; /* only used for kshow */

	    push(2);
	    make_int(op - 1, gs_text_current_char(penum)); /* previous char */
	    make_int(op, gs_text_next_char(penum));
	    push_op_estack(op_show_continue);	/* continue after kerning */
	    *++esp = *pslot;	/* kerning procedure */
	    return o_push_estack;
	}
	case TEXT_PROCESS_RENDER: {
	    gs_font *pfont = gs_currentfont(igs);
	    font_data *pfdata = pfont_data(pfont);
	    gs_char chr = gs_text_current_char(penum);
	    gs_glyph glyph = gs_text_current_glyph(penum);

	    push(2);
	    op[-1] = pfdata->dict;	/* push the font */
	    /*
	     * For Type 1 and Type 4 fonts, prefer BuildChar to BuildGlyph
	     * if there is no glyph, or if there is both a character and a
	     * glyph and the glyph is the one that corresponds to the
	     * character in the Encoding, so that PostScript procedures
	     * appearing in the CharStrings dictionary will receive the
	     * character code rather than the character name; for Type 3
	     * fonts, prefer BuildGlyph to BuildChar.  For other font types
	     * (such as CID fonts), only BuildGlyph will be present.
	     */
	    if (pfont->FontType == ft_user_defined) {
		/* Type 3 font, prefer BuildGlyph. */
		if (level2_enabled &&
		    !r_has_type(&pfdata->BuildGlyph, t_null) &&
		    glyph != gs_no_glyph
		    ) {
		    glyph_ref(imemory, glyph, op);
		    esp[2] = pfdata->BuildGlyph;
		} else if (r_has_type(&pfdata->BuildChar, t_null))
		    goto err;
		else if (chr == gs_no_char) {
		    /* glyphshow, reverse map the character */
		    /* through the Encoding */
		    ref gref;
		    const ref *pencoding = &pfdata->Encoding;

		    glyph_ref(imemory, glyph, &gref);
		    if (!map_glyph_to_char(imemory, &gref, pencoding,
					   (ref *) op)
			) {	/* Not found, try .notdef */
		        name_enter_string(imemory, ".notdef", &gref);
			if (!map_glyph_to_char(imemory, &gref,
					       pencoding,
					       (ref *) op)
			    )
			    goto err;
		    }
		    esp[2] = pfdata->BuildChar;
		} else {
		    make_int(op, chr & 0xff);
		    esp[2] = pfdata->BuildChar;
		}
	    } else {
		/*
		 * For a Type 1 or Type 4 font, prefer BuildChar or
		 * BuildGlyph as described above: we know that both
		 * BuildChar and BuildGlyph are present.  For other font
		 * types, only BuildGlyph is available.
		 */
		ref eref, gref;

		if (chr != gs_no_char &&
		    !r_has_type(&pfdata->BuildChar, t_null) &&
		    (glyph == gs_no_glyph ||
		     (array_get(imemory, &pfdata->Encoding, (long)(chr & 0xff), &eref) >= 0 &&
		      (glyph_ref(imemory, glyph, &gref), obj_eq(imemory, &gref, &eref))))
		    ) {
		    make_int(op, chr & 0xff);
		    esp[2] = pfdata->BuildChar;
		} else {
		    /* We might not have a glyph: substitute 0. **HACK** */
		    if (glyph == gs_no_glyph)
			make_int(op, 0);
		    else
		        glyph_ref(imemory, glyph, op);
		    esp[2] = pfdata->BuildGlyph;
		}
	    }
	    /* Save the stack depths in case we bail out. */
	    sodepth.value.intval = ref_stack_count(&o_stack) - 2;
	    sddepth.value.intval = ref_stack_count(&d_stack);
	    push_op_estack(op_show_continue);
	    ++esp;		/* skip BuildChar or BuildGlyph proc */
	    return o_push_estack;
	}
	case TEXT_PROCESS_CDEVPROC:
	    {   gs_font *pfont = penum->current_font;
		ref cnref;
		op_proc_t cont = op_show_continue, exec_cont = 0;
		gs_glyph glyph = penum->returned.current_glyph;
		int code;
    
		pop(npop);
		op = osp;
		glyph_ref(imemory, glyph, &cnref);
		if (pfont->FontType == ft_CID_TrueType) {
		    gs_font_type42 *pfont42 = (gs_font_type42 *)pfont;
		    uint glyph_index = pfont42->data.get_glyph_index(pfont42, glyph);

		    code = zchar42_set_cache(i_ctx_p, (gs_font_base *)pfont42, 
				    &cnref, glyph_index, cont, &exec_cont);
		} else if (pfont->FontType == ft_CID_encrypted)
		    code = z1_set_cache(i_ctx_p, (gs_font_base *)pfont, 
				    &cnref, glyph, cont, &exec_cont);
		else
		    return_error(e_unregistered); /* Unimplemented. */
		if (exec_cont != 0)
		    return_error(e_unregistered); /* Must not happen. */
		return code;
	    }
	default:		/* error */
err:
	    if (code >= 0)
		code = gs_note_error(e_invalidfont);
	    return op_show_free(i_ctx_p, code);
    }
}
/* Reverse-map a glyph name to a character code for glyphshow. */
static bool
map_glyph_to_char(const gs_memory_t *mem, const ref * pgref, const ref * pencoding, ref * pch)
{
    uint esize = r_size(pencoding);
    uint ch;
    ref eref;

    for (ch = 0; ch < esize; ch++) {
        array_get(mem, pencoding, (long)ch, &eref);
	if (obj_eq(mem, pgref, &eref)) {
	    make_int(pch, ch);
	    return true;
	}
    }
    return false;
}

/* Find the index of the e-stack mark for the current show enumerator. */
/* Return 0 if we can't find the mark. */
static uint
op_show_find_index(i_ctx_t *i_ctx_p)
{
    ref_stack_enum_t rsenum;
    uint count = 0;

    ref_stack_enum_begin(&rsenum, &e_stack);
    do {
	es_ptr ep = rsenum.ptr;
	uint size = rsenum.size;

	for (ep += size - 1; size != 0; size--, ep--, count++)
	    if (r_is_estack_mark(ep) && estack_mark_index(ep) == es_show)
		return count;
    } while (ref_stack_enum_next(&rsenum));
    return 0;		/* no mark */
}

/* Find the current show enumerator on the e-stack. */
gs_text_enum_t *
op_show_find(i_ctx_t *i_ctx_p)
{
    uint index = op_show_find_index(i_ctx_p);

    if (index == 0)
	return 0;		/* no mark */
    return r_ptr(ref_stack_index(&e_stack, index - (snumpush - 1)),
		 gs_text_enum_t);
}

/*
 * Return true if we only need the width from the rasterizer
 * and can short-circuit the full rendering of the character,
 * false if we need the actual character bits.  This is only safe if
 * we know the character is well-behaved, i.e., is not defined by an
 * arbitrary PostScript procedure.
 */
bool
zchar_show_width_only(const gs_text_enum_t * penum)
{
    if (!gs_text_is_width_only(penum))
	return false;
    switch (penum->orig_font->FontType) {
    case ft_encrypted:
    case ft_encrypted2:
    case ft_CID_encrypted:
    case ft_CID_TrueType:
    case ft_CID_bitmap:
    case ft_TrueType:
	return true;
    default:
	return false;
    }
}

/* Shortcut the BuildChar or BuildGlyph procedure at the point */
/* of the setcharwidth or the setcachedevice[2] if we are in */
/* a stringwidth or cshow, or if we are only collecting the scalable */
/* width for an xfont character. */
static int
op_show_return_width(i_ctx_t *i_ctx_p, uint npop, double *pwidth)
{
    uint index = op_show_find_index(i_ctx_p);
    es_ptr ep = (es_ptr) ref_stack_index(&e_stack, index - (snumpush - 1));
    int code = gs_text_setcharwidth(esenum(ep), pwidth);
    uint ocount, dsaved, dcount;

    if (code < 0)
	return code;
    /* Restore the operand and dictionary stacks. */
    ocount = ref_stack_count(&o_stack) - (uint) esodepth(ep).value.intval;
    if (ocount < npop)
	return_error(e_stackunderflow);
    dsaved = (uint) esddepth(ep).value.intval;
    dcount = ref_stack_count(&d_stack);
    if (dcount < dsaved)
	return_error(e_dictstackunderflow);
    while (dcount > dsaved) {
	code = zend(i_ctx_p);
	if (code < 0)
	    return code;
	dcount--;
    }
    ref_stack_pop(&o_stack, ocount);
    /* We don't want to pop the mark or the continuation */
    /* procedure (op_show_continue or cshow_continue). */
    pop_estack(i_ctx_p, index - snumpush);
    return o_pop_estack;
}

/*
 * Restore state after finishing, or unwinding from an error within, a show
 * operation.  Note that we assume op == osp, and may reset osp.
 */
static int
op_show_restore(i_ctx_t *i_ctx_p, bool for_error)
{
    register es_ptr ep = esp + snumpush;
    gs_text_enum_t *penum = esenum(ep);
    int saved_level = esgslevel(ep).value.intval;
    int code = 0;

    if (for_error) {
#if 0 /* Disabled for CPSI compatibility for 13-12-4. 
         CPSI doesn't remove cshow, kshow proc operands. */
	uint saved_count = esodepth(ep).value.intval;
	uint count = ref_stack_count(&o_stack);

	if (count > saved_count)	/* if <, we're in trouble */
	    ref_stack_pop(&o_stack, count - saved_count);
#endif
	if (ep[1].value.opproc == op_show_continue && penum->enum_client_data != NULL) {
	    /* Replace the continuation operaton on estack with the right operator : */
	    op_proc_t proc;

	    *(void **)&proc = penum->enum_client_data;
	    make_op_estack(ep + 1, proc);
	}
    }
    if (SHOW_IS_STRINGWIDTH(penum) && igs->text_rendering_mode != 3) {	
	/* stringwidth does an extra gsave */
	--saved_level;
    }
    if (penum->text.operation & TEXT_REPLACE_WIDTHS) {
	gs_free_const_object(penum->memory, penum->text.y_widths, "y_widths");
	if (penum->text.x_widths != penum->text.y_widths)
	    gs_free_const_object(penum->memory, penum->text.x_widths, "x_widths");
    }
    /*
     * We might have been inside a cshow, in which case currentfont was
     * reset temporarily, as though we were inside a BuildChar/ BuildGlyph
     * procedure.  To handle this case, set currentfont back to its original
     * state.  NOTE: this code previously used fstack[0] in the enumerator
     * for the root font: we aren't sure that this change is correct.
     */
    gs_set_currentfont(igs, penum->orig_font);
    while (igs->level > saved_level && code >= 0) {
	if (igs->saved == 0 || igs->saved->saved == 0) {
	    /*
	     * Bad news: we got an error inside a save inside a BuildChar or
	     * BuildGlyph.  Don't attempt to recover.
	     */
	    code = gs_note_error(e_Fatal);
	} else
	    code = gs_grestore(igs);
    }
    gs_text_release(penum, "op_show_restore");
    return code;
}
/* Clean up after an error. */
static int
op_show_cleanup(i_ctx_t *i_ctx_p)
{
    return op_show_restore(i_ctx_p, true);
}
/* Clean up after termination of a show operation. */
int
op_show_free(i_ctx_t *i_ctx_p, int code)
{
    int rcode;

    esp -= snumpush;
    rcode = op_show_restore(i_ctx_p, code < 0);
    return (rcode < 0 ? rcode : code);
}

/* Get a FontBBox parameter from a font dictionary. */
int
font_bbox_param(const gs_memory_t *mem, const ref * pfdict, double bbox[4])
{
    ref *pbbox;

    /*
     * Pre-clear the bbox in case it's invalid.  The Red Books say that
     * FontBBox is required, but old Adobe interpreters don't require
     * it, and a few user-written fonts don't supply it, or supply one
     * of the wrong size (!); also, PageMaker 5.0 (an Adobe product!)
     * sometimes emits an absurd bbox for Type 1 fonts converted from
     * TrueType.
     */
    bbox[0] = bbox[1] = bbox[2] = bbox[3] = 0.0;
    if (dict_find_string(pfdict, "FontBBox", &pbbox) > 0) {
	if (!r_is_array(pbbox))
	    return_error(e_typecheck);
	if (r_size(pbbox) == 4) {
	    const ref_packed *pbe = pbbox->value.packed;
	    ref rbe[4];
	    int i;
	    int code;
	    float dx, dy, ratio;
	    const float max_ratio = 12; /* From the bug 687594. */

	    for (i = 0; i < 4; i++) {
		packed_get(mem, pbe, rbe + i);
		pbe = packed_next(pbe);
	    }
	    if ((code = num_params(rbe + 3, 4, bbox)) < 0)
		return code;
 	    /* Require "reasonable" values. */
	    dx = bbox[2] - bbox[0];
	    dy = bbox[3] - bbox[1];
	    if (dx <= 0 || dy <= 0 ||
		(ratio = dy / dx) < 1 / max_ratio || ratio > max_ratio
		)
		bbox[0] = bbox[1] = bbox[2] = bbox[3] = 0.0;
	}
    } else if (CPSI_mode) {
        return_error(e_invalidfont); /* CPSI requires FontBBox */
    }
    return 0;
}
