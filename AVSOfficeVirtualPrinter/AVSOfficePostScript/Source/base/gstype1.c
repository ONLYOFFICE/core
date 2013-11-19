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

/* $Id: gstype1.c 8313 2007-10-24 07:31:09Z ken $ */
/* Adobe Type 1 charstring interpreter */
#include "math_.h"
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsstruct.h"
#include "gxarith.h"
#include "gxfixed.h"
#include "gxmatrix.h"
#include "gxcoord.h"
#include "gxistate.h"
#include "gxpath.h"
#include "gxfont.h"
#include "gxfont1.h"
#include "gxtype1.h"
#include "gxhintn.h"

/*
 * Define whether to always do Flex segments as curves.
 * This is only an issue because some old Adobe DPS fonts
 * seem to violate the Flex specification in a way that requires this.
 * We changed this from 1 to 0 in release 5.02: if it causes any
 * problems, we'll implement a more sophisticated test.
 */
#define ALWAYS_DO_FLEX_AS_CURVE 0

/* ------ Interpreter subroutines ------ */

/*
 * According to "Adobe Type 1 Font Format", 
 * Section 6.2 "CharString Number Encoding", in particular the Note at 
 * the end of the section:
 * "Numbers with absolute values larger than 32,000 must be followed by a 
 * div operator such that the result of the div is less than 32,000."
 * 
 * This function looks ahead for the div operator
 * and applies it in the compile time, so that the big numbers
 * are not placed onto the stack.
 */
int gs_type1_check_float(crypt_state *state, bool encrypted, const byte **ci, cs_ptr csp, long lw)
{
    long denom;
    uint c0;
    int c;
    const byte *cip = *ci;

    c0 = *cip++;
    charstring_next(c0, *state, c, encrypted);
    if (c < c_num1)
	return_error(gs_error_rangecheck);
    if (c < c_pos2_0)
	decode_num1(denom, c);
    else if (c < cx_num4)
	decode_num2(denom, c, cip, *state, encrypted);
    else if (c == cx_num4)
	decode_num4(denom, cip, *state, encrypted);
    else
	return_error(gs_error_invalidfont);
    c0 = *cip++;
    charstring_next(c0, *state, c, encrypted);
    if (c != cx_escape)
	return_error(gs_error_rangecheck);
    c0 = *cip++;
    charstring_next(c0, *state, c, encrypted);
    if (c != ce1_div)
	return_error(gs_error_rangecheck);
    /* Rather "Adobe Type 1 Font Format" restricts the div result with 32,000, 
       We don't want to check it here as a compatibility to the old code,
       and because Type 2 doesn't set this limitation.
       Instead that we're checking here just for 'fixed' overflow,
       which is a weaker limit.
     */
    if (any_abs(lw / denom) > max_int_in_fixed) {
	return_error(gs_error_rangecheck);
    }
    *csp = float2fixed((double)lw / denom);
    *ci = cip;
    return(0);
}

/* ------ Main interpreter ------ */

/*
 * Continue interpreting a Type 1 charstring.  If str != 0, it is taken as
 * the byte string to interpret.  Return 0 on successful completion, <0 on
 * error, or >0 when client intervention is required (or allowed).  The int*
 * argument is where the othersubr # is stored for callothersubr.
 */
int
gs_type1_interpret(gs_type1_state * pcis, const gs_glyph_data_t *pgd,
		   int *pindex)
{
    gs_font_type1 *pfont = pcis->pfont;
    gs_type1_data *pdata = &pfont->data;
    t1_hinter *h = &pcis->h;
    bool encrypted = pdata->lenIV >= 0;
    fixed cstack[ostack_size];

#define cs0 cstack[0]
#define ics0 fixed2int_var(cs0)
#define cs1 cstack[1]
#define ics1 fixed2int_var(cs1)
#define cs2 cstack[2]
#define ics2 fixed2int_var(cs2)
#define cs3 cstack[3]
#define ics3 fixed2int_var(cs3)
#define cs4 cstack[4]
#define ics4 fixed2int_var(cs4)
#define cs5 cstack[5]
#define ics5 fixed2int_var(cs5)
    cs_ptr csp;
#define clear CLEAR_CSTACK(cstack, csp)
    ip_state_t *ipsp = &pcis->ipstack[pcis->ips_count - 1];
    const byte *cip;
    crypt_state state;
    register int c;
    int code = 0;
    fixed ftx = pcis->origin.x, fty = pcis->origin.y;

    switch (pcis->init_done) {
	case -1:
	    t1_hinter__init(h, pcis->path);
	    break;
	case 0:
	    gs_type1_finish_init(pcis);	/* sets origin */
	    ftx = pcis->origin.x, fty = pcis->origin.y;
            code = t1_hinter__set_mapping(h, &pcis->pis->ctm,
			    &pfont->FontMatrix, &pfont->base->FontMatrix,
			    pcis->scale.x.log2_unit, pcis->scale.x.log2_unit,
			    pcis->scale.x.log2_unit - pcis->log2_subpixels.x,
			    pcis->scale.y.log2_unit - pcis->log2_subpixels.y,
			    pcis->origin.x, pcis->origin.y, 
			    gs_currentaligntopixels(pfont->dir));
	    if (code < 0)
	    	return code;
	    code = t1_hinter__set_font_data(h, 1, pdata, pcis->no_grid_fitting, 
			    pcis->pfont->is_resource);
	    if (code < 0)
	    	return code;
	    break;
	default /*case 1 */ :
	    break;
    }
    INIT_CSTACK(cstack, csp, pcis);

    if (pgd == 0)
	goto cont;
    ipsp->cs_data = *pgd;
    cip = pgd->bits.data;
  call:state = crypt_charstring_seed;
    if (encrypted) {
	int skip = pdata->lenIV;

	/* Skip initial random bytes */
	for (; skip > 0; ++cip, --skip)
	    decrypt_skip_next(*cip, state);
    }
    goto top;
  cont:cip = ipsp->ip;
    state = ipsp->dstate;
  top:for (;;) {
	uint c0 = *cip++;

	charstring_next(c0, state, c, encrypted);
	if (c >= c_num1) {
	    /* This is a number, decode it and push it on the stack. */

	    if (c < c_pos2_0) {	/* 1-byte number */
		decode_push_num1(csp, cstack, c);
	    } else if (c < cx_num4) {	/* 2-byte number */
		decode_push_num2(csp, cstack, c, cip, state, encrypted);
	    } else if (c == cx_num4) {	/* 4-byte number */
		long lw;

		decode_num4(lw, cip, state, encrypted);
		CS_CHECK_PUSH(csp, cstack);
		*++csp = int2fixed(lw);
		if (lw != fixed2long(*csp)) {
		    /*
		     * The integer was too large to handle in fixed point.
		     * Handle this case specially.
		     */
		    code = gs_type1_check_float(&state, encrypted, &cip, csp, lw);
		    if (code < 0)
		        return code;
	        }
	    } else		/* not possible */
		return_error(gs_error_invalidfont);
	  pushed:if_debug3('1', "[1]%d: (%d) %f\n",
		      (int)(csp - cstack), c, fixed2float(*csp));
	    continue;
	}
#ifdef DEBUG
	if (gs_debug['1']) {
	    static const char *const c1names[] =
	    {char1_command_names};

	    if (c1names[c] == 0)
		dlprintf2("[1]0x%lx: %02x??\n", (ulong) (cip - 1), c);
	    else
		dlprintf3("[1]0x%lx: %02x %s\n", (ulong) (cip - 1), c,
			  c1names[c]);
	}
#endif
	switch ((char_command) c) {
#define cnext clear; goto top
#define inext goto top

		/* Commands with identical functions in Type 1 and Type 2, */
		/* except for 'escape'. */

	    case c_undef0:
	    case c_undef2:
	    case c_undef17:
		return_error(gs_error_invalidfont);
	    case c_callsubr:
		c = fixed2int_var(*csp) + pdata->subroutineNumberBias;
		code = pdata->procs.subr_data
		    (pfont, c, false, &ipsp[1].cs_data);
		if (code < 0)
		    return_error(code);
		--csp;
		ipsp->ip = cip, ipsp->dstate = state;
		++ipsp;
		cip = ipsp->cs_data.bits.data;
		goto call;
	    case c_return:
		gs_glyph_data_free(&ipsp->cs_data, "gs_type1_interpret");
		--ipsp;
		goto cont;
	    case c_undoc15:
		/* See gstype1.h for information on this opcode. */
		cnext;

		/* Commands with similar but not identical functions */
		/* in Type 1 and Type 2 charstrings. */

	    case cx_hstem:
                code = t1_hinter__hstem(h, cs0, cs1);
		if (code < 0)
		    return code;
		cnext;
	    case cx_vstem:
                code = t1_hinter__vstem(h, cs0, cs1);
		if (code < 0)
		    return code;
		cnext;
	    case cx_vmoveto:
		cs1 = cs0;
		cs0 = 0;
	      move:		/* cs0 = dx, cs1 = dy for hint checking. */
                code = t1_hinter__rmoveto(h, cs0, cs1);
		goto cc;
	    case cx_rlineto:
	      line:		/* cs0 = dx, cs1 = dy for hint checking. */
                code = t1_hinter__rlineto(h, cs0, cs1);
	      cc:if (code < 0)
		    return code;
		cnext;
	    case cx_hlineto:
		cs1 = 0;
		goto line;
	    case cx_vlineto:
		cs1 = cs0;
		cs0 = 0;
		goto line;
	    case cx_rrcurveto:
                code = t1_hinter__rcurveto(h, cs0, cs1, cs2, cs3, cs4, cs5);
		goto cc;
	    case cx_endchar:
                if (pcis->seac_accent < 0) {
                    code = t1_hinter__endglyph(h);
		    if (code < 0)
			return code;
		    code = gx_setcurrentpoint_from_path(pcis->pis, pcis->path);
		    if (code < 0)
			return code;
		} else {
		    code = t1_hinter__end_subglyph(h);
		    if (code < 0)
			return code;
		    pcis->seac_flag = true;
		}
		code = gs_type1_endchar(pcis);
		if (code == 1) {
		    /* do accent of seac */
		    ipsp = &pcis->ipstack[pcis->ips_count - 1];
		    cip = ipsp->cs_data.bits.data;
		    goto call;
		}
		return code;
	    case cx_rmoveto:
		goto move;
	    case cx_hmoveto:
		cs1 = 0;
		goto move;
	    case cx_vhcurveto:
                code = t1_hinter__rcurveto(h, 0, cs0, cs1, cs2, cs3, 0);
		goto cc;
	    case cx_hvcurveto:
                code = t1_hinter__rcurveto(h, cs0, 0, cs1, cs2, 0, cs3);
		goto cc;

		/* Commands only recognized in Type 1 charstrings, */
		/* plus 'escape'. */

	    case c1_closepath:
                code = t1_hinter__closepath(h);
		goto cc;
	    case c1_hsbw:
                if (!pcis->seac_flag) {
		    fixed sbx = cs0, sby = fixed_0, wx = cs1, wy = fixed_0;

		    if (pcis->seac_accent < 0) {
			if (pcis->sb_set) {
			    pcis->origin_offset.x = pcis->lsb.x - sbx;
			    pcis->origin_offset.y = pcis->lsb.y - sby;
			    sbx = pcis->lsb.x;
			    sby = pcis->lsb.y;
			}
			if (pcis->width_set) {
			    wx = pcis->width.x;
			    wy = pcis->width.y;
			}
		    } else
			pcis->base_lsb = sbx;
		    code = t1_hinter__sbw(h, sbx, sby, wx, wy);
		} else {
		    fixed accent_lsb = cs0;
		    fixed overall_x_offset = pcis->compound_lsb.x + pcis->adxy.x - pcis->asb + accent_lsb - pcis->base_lsb;

                    code = t1_hinter__sbw_seac(h, overall_x_offset, pcis->adxy.y);
		}
		if (code < 0)
		    return code;
		gs_type1_sbw(pcis, cs0, fixed_0, cs1, fixed_0);
		cs1 = fixed_0;
rsbw:		/* Give the caller the opportunity to intervene. */
		pcis->os_count = 0;	/* clear */
		ipsp->ip = cip, ipsp->dstate = state;
		pcis->ips_count = ipsp - &pcis->ipstack[0] + 1;
		/* If we aren't in a seac, do nothing else now; */
		/* finish_init will take care of the rest. */
		if (pcis->init_done < 0) {
		    /* Finish init when we return. */
		    pcis->init_done = 0;
		}
		return type1_result_sbw;
	    case cx_escape:
		charstring_next(*cip, state, c, encrypted);
		++cip;
#ifdef DEBUG
		if (gs_debug['1'] && c < char1_extended_command_count) {
		    static const char *const ce1names[] =
		    {char1_extended_command_names};

		    if (ce1names[c] == 0)
			dlprintf2("[1]0x%lx: %02x??\n", (ulong) (cip - 1), c);
		    else
			dlprintf3("[1]0x%lx: %02x %s\n", (ulong) (cip - 1), c,
				  ce1names[c]);
		}
#endif
		switch ((char1_extended_command) c) {
		    case ce1_dotsection:
                        code = t1_hinter__dotsection(h);
			if (code < 0)
			    return code;
			cnext;
		    case ce1_vstem3:
                        code = t1_hinter__vstem3(h, cs0, cs1, cs2, cs3, cs4, cs5);
			if (code < 0)
			    return code;
			cnext;
		    case ce1_hstem3:
                        code = t1_hinter__hstem3(h, cs0, cs1, cs2, cs3, cs4, cs5);
			if (code < 0)
			    return code;
			cnext;
		    case ce1_seac:
			code = gs_type1_seac(pcis, cstack + 1, cstack[0],
					     ipsp);
			if (code != 0) {
			    *pindex = ics3;
			    return code;
			}
			clear;
			cip = ipsp->cs_data.bits.data;
			goto call;
		    case ce1_sbw:
                        if (!pcis->seac_flag)
                            code = t1_hinter__sbw(h, cs0, cs1, cs2, cs3);
                        else 
                            code = t1_hinter__sbw_seac(h, cs0 + pcis->adxy.x , cs1 + pcis->adxy.y);
			if (code < 0)
			    return code;
			gs_type1_sbw(pcis, cs0, cs1, cs2, cs3);
			goto rsbw;
		    case ce1_div:
			csp[-1] = float2fixed((double)csp[-1] / (double)*csp);
			--csp;
			goto pushed;
		    case ce1_undoc15:
			/* See gstype1.h for information on this opcode. */
			cnext;
		    case ce1_callothersubr:
			{
			    int num_results;
			    /* We must remember to pop both the othersubr # */
			    /* and the argument count off the stack. */
			    switch (*pindex = fixed2int_var(*csp)) {
				case 0:
				    {	
					fixed fheight = csp[-4];
					/* Assume the next two opcodes */
					/* are `pop' `pop'.  Unfortunately, some */
					/* Monotype fonts put these in a Subr, */
					/* so we can't just look ahead in the */
					/* opcode stream. */
					pcis->ignore_pops = 2;
					csp[-4] = csp[-3] - pcis->asb_diff;
					csp[-3] = csp[-2];
					csp -= 3;
					code = t1_hinter__flex_end(h, fheight);
				    }
				    if (code < 0)
					return code;
				    pcis->flex_count = flex_max;	/* not inside flex */
				    inext;
				case 1:
				    code = t1_hinter__flex_beg(h);
				    if (code < 0)
					return code;
				    pcis->flex_count = 1;
				    csp -= 2;
				    inext;
				case 2:
				    if (pcis->flex_count >= flex_max)
					return_error(gs_error_invalidfont);
				    code = t1_hinter__flex_point(h);
				    if (code < 0)
					return code;
				    csp -= 2;
				    inext;
				case 3:
				    /* Assume the next opcode is a `pop'. */
				    /* See above as to why we don't just */
				    /* look ahead in the opcode stream. */
				    pcis->ignore_pops = 1;
                                    code = t1_hinter__drop_hints(h);
				    if (code < 0)
					return code;
				    csp -= 2;
				    inext;
				case 12:
				case 13:
				    /* Counter control isn't implemented. */
				    cnext;
				case 14:
				    num_results = 1;
				  blend:
				    code = gs_type1_blend(pcis, csp,
							  num_results);
				    if (code < 0)
					return code;
				    csp -= code;
				    inext;
				case 15:
				    num_results = 2;
				    goto blend;
				case 16:
				    num_results = 3;
				    goto blend;
				case 17:
				    num_results = 4;
				    goto blend;
				case 18:
				    num_results = 6;
				    goto blend;
			    }
			}
			/* Not a recognized othersubr, */
			/* let the client handle it. */
			{
			    int scount = csp - cstack;
			    int n;

			    /* Copy the arguments to the caller's stack. */
			    if (scount < 1 || csp[-1] < 0 ||
				csp[-1] > int2fixed(scount - 1)
				)
				return_error(gs_error_invalidfont);
			    n = fixed2int_var(csp[-1]);
			    code = (*pdata->procs.push_values)
				(pcis->callback_data, csp - (n + 1), n);
			    if (code < 0)
				return_error(code);
			    scount -= n + 1;
			    /* Exit to caller */
			    ipsp->ip = cip, ipsp->dstate = state;
			    pcis->os_count = scount;
			    pcis->ips_count = ipsp - &pcis->ipstack[0] + 1;
			    if (scount)
				memcpy(pcis->ostack, cstack, scount * sizeof(fixed));
			    return type1_result_callothersubr;
			}
		    case ce1_pop:
			/* Check whether we're ignoring the pops after */
			/* a known othersubr. */
			if (pcis->ignore_pops != 0) {
			    pcis->ignore_pops--;
			    inext;
			}
			CS_CHECK_PUSH(csp, cstack);
			++csp;
			code = (*pdata->procs.pop_value)
			    (pcis->callback_data, csp);
			if (code < 0)
			    return_error(code);
			goto pushed;
		    case ce1_setcurrentpoint:
			cs0 += pcis->adxy.x + pcis->origin_offset.x;
			cs1 += pcis->adxy.y + pcis->origin_offset.y;
			t1_hinter__setcurrentpoint(h, cs0, cs1);
			cnext;
		    default:
			return_error(gs_error_invalidfont);
		}
		/*NOTREACHED */

		/* Fill up the dispatch up to 32. */

	      case_c1_undefs:
	    default:		/* pacify compiler */
		return_error(gs_error_invalidfont);
	}
    }
}
