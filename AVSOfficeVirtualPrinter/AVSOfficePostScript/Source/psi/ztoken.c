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

/* $Id: ztoken.c 9043 2008-08-28 22:48:19Z giles $ */
/* Token reading operators */
#include "string_.h"
#include "stat_.h" /* get system header early to avoid name clash on Cygwin */
#include "ghost.h"
#include "oper.h"
#include "dstack.h"		/* for dict_find_name */
#include "estack.h"
#include "gsstruct.h"		/* for iscan.h */
#include "gsutil.h"
#include "stream.h"
#include "files.h"
#include "store.h"
#include "strimpl.h"		/* for sfilter.h */
#include "sfilter.h"		/* for iscan.h */
#include "idict.h"
#include "iname.h"
#include "iscan.h"
#include "itoken.h"		/* for prototypes */

/* <file> token <obj> -true- */
/* <string> token <post> <obj> -true- */
/* <string|file> token -false- */
static int ztoken_continue(i_ctx_t *);
static int token_continue(i_ctx_t *, scanner_state *, bool);
int
ztoken(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    switch (r_type(op)) {
	default:
	    return_op_typecheck(op);
	case t_file: {
	    stream *s;
	    scanner_state state;

	    check_read_file(s, op);
	    check_ostack(1);
	    scanner_init(&state, op);
	    return token_continue(i_ctx_p, &state, true);
	}
	case t_string: {
	    ref token;
	    /* -1 is to remove the string operand in case of error. */
	    int orig_ostack_depth = ref_stack_count(&o_stack) - 1;
	    int code;

	    /* Don't pop the operand in case of invalidaccess. */
	    if (!r_has_attr(op, a_read))
		return_error(e_invalidaccess);
	    code = scan_string_token(i_ctx_p, op, &token);
	    switch (code) {
	    case scan_EOF:	/* no tokens */
		make_false(op);
		return 0;
	    default:
		if (code < 0) {
		    /*
		     * Clear anything that may have been left on the ostack,
		     * including the string operand.
		     */
	    	    if (orig_ostack_depth < ref_stack_count(&o_stack))
	    		pop(ref_stack_count(&o_stack)- orig_ostack_depth);
		    return code;
		}
	    }
	    push(2);
	    op[-1] = token;
	    make_true(op);
	    return 0;
	}
    }
}
/* Continue reading a token after an interrupt or callout. */
/* *op is the scanner state. */
static int
ztoken_continue(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    scanner_state *pstate;

    check_stype(*op, st_scanner_state);
    pstate = r_ptr(op, scanner_state);
    return token_continue(i_ctx_p, pstate, false);
}
/* Common code for token reading. */
static int
token_continue(i_ctx_t *i_ctx_p, scanner_state * pstate, bool save)
{
    os_ptr op = osp;
    int code;
    ref token;

    /* Note that scan_token may change osp! */
    pop(1);			/* remove the file or scanner state */
again:
    code = scan_token(i_ctx_p, &token, pstate);
    op = osp;
    switch (code) {
	default:		/* error */
	    if (code > 0)	/* comment, not possible */
		code = gs_note_error(e_syntaxerror);
	    scanner_error_object(i_ctx_p, pstate, &i_ctx_p->error_object);
	    break;
	case scan_BOS:
	    code = 0;
	case 0:		/* read a token */
	    push(2);
	    ref_assign(op - 1, &token);
	    make_true(op);
	    break;
	case scan_EOF:		/* no tokens */
	    push(1);
	    make_false(op);
	    code = 0;
	    break;
	case scan_Refill:	/* need more data */
	    code = scan_handle_refill(i_ctx_p, pstate, save,
				      ztoken_continue);
	    switch (code) {
		case 0:	/* state is not copied to the heap */
		    goto again;
		case o_push_estack:
		    return code;
	    }
	    break;		/* error */
    }
    if (code <= 0 && !save) {	/* Deallocate the scanner state record. */
	ifree_object(pstate, "token_continue");
    }
    return code;
}

/* <file> .tokenexec - */
/* Read a token and do what the interpreter would do with it. */
/* This is different from token + exec because literal procedures */
/* are not executed (although binary object sequences ARE executed). */
int ztokenexec_continue(i_ctx_t *);	/* export for interpreter */
static int tokenexec_continue(i_ctx_t *, scanner_state *, bool);
int
ztokenexec(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream *s;
    scanner_state state;

    check_read_file(s, op);
    check_estack(1);
    scanner_init(&state, op);
    return tokenexec_continue(i_ctx_p, &state, true);
}
/* Continue reading a token for execution after an interrupt or callout. */
/* *op is the scanner state. */
/* We export this because this is how the interpreter handles a */
/* scan_Refill for an executable file. */
int
ztokenexec_continue(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    scanner_state *pstate;

    check_stype(*op, st_scanner_state);
    pstate = r_ptr(op, scanner_state);
    return tokenexec_continue(i_ctx_p, pstate, false);
}
/* Common code for token reading + execution. */
static int
tokenexec_continue(i_ctx_t *i_ctx_p, scanner_state * pstate, bool save)
{
    os_ptr op;
    int code;
    /* Note that scan_token may change osp! */
    pop(1);
again:
    check_estack(1);
    code = scan_token(i_ctx_p, (ref *) (esp + 1), pstate);
    op = osp;
    switch (code) {
	case 0:
	    if (r_is_proc(esp + 1)) {	/* Treat procedure as a literal. */
		push(1);
		ref_assign(op, esp + 1);
		code = 0;
		break;
	    }
	    /* falls through */
	case scan_BOS:
	    ++esp;
	    code = o_push_estack;
	    break;
	case scan_EOF:		/* no tokens */
	    code = 0;
	    break;
	case scan_Refill:	/* need more data */
	    code = scan_handle_refill(i_ctx_p, pstate, save,
				      ztokenexec_continue);
	    switch (code) {
		case 0:	/* state is not copied to the heap */
		    goto again;
		case o_push_estack:
		    return code;
	    }
	    break;		/* error */
	case scan_Comment:
	case scan_DSC_Comment:
	    return ztoken_handle_comment(i_ctx_p, pstate, esp + 1, code,
					 save, true, ztokenexec_continue);
	default:		/* error */
	    scanner_error_object(i_ctx_p, pstate, &i_ctx_p->error_object);
	    break;
    }
    if (!save) {		/* Deallocate the scanner state record. */
	ifree_object(pstate, "token_continue");
    }
    return code;
}

/*
 * Handle a scan_Comment or scan_DSC_Comment return from scan_token
 * (scan_code) by calling out to %Process[DSC]Comment.  The continuation
 * procedure expects the scanner state on the o-stack.
 */
int
ztoken_handle_comment(i_ctx_t *i_ctx_p, scanner_state *sstate,
		      const ref *ptoken, int scan_code,
		      bool save, bool push_file, op_proc_t cont)
{
    const char *proc_name;
    scanner_state *pstate;
    os_ptr op;
    ref *ppcproc;
    int code;

    switch (scan_code) {
    case scan_Comment:
	proc_name = "%ProcessComment";
	break;
    case scan_DSC_Comment:
	proc_name = "%ProcessDSCComment";
	break;
    default:
	return_error(e_Fatal);	/* can't happen */
    }
    /*
     * We can't use check_ostack here, because it returns on overflow.
     */
    /*check_ostack(2);*/
    if (ostop - osp < 2) {
	code = ref_stack_extend(&o_stack, 2);
	if (code < 0)
	    return code;
    }
    check_estack(3);
    code = name_enter_string(imemory, proc_name, esp + 3);
    if (code < 0)
	return code;
    if (save) {
	pstate = ialloc_struct(scanner_state, &st_scanner_state,
			       "ztoken_handle_comment");
	if (pstate == 0)
	    return_error(e_VMerror);
	*pstate = *sstate;
    } else
	pstate = sstate;
    /* Save the token now -- it might be on the e-stack. */
    if (!pstate->s_pstack)
	osp[2] = *ptoken;
    /*
     * Push the continuation, scanner state, file, and callout procedure
     * on the e-stack.
     */
    make_op_estack(esp + 1, cont);
    make_istruct(esp + 2, 0, pstate);
    ppcproc = dict_find_name(esp + 3);
    if (ppcproc == 0) {
	/*
	 * This can only happen during initialization.
	 * Pop the comment string from the o-stack if needed (see below).
	 */
	if (pstate->s_pstack)
	    --osp;
	esp += 2;		/* do run the continuation */
    } else {
	/*
	 * Push the file and comment string on the o-stack.
	 * If we were inside { }, the comment string is already on the stack.
	 */
	if (pstate->s_pstack) {
	    op = ++osp;
	    *op = op[-1];
	} else {
	    op = osp += 2;
	    /* *op = *ptoken; */	/* saved above */
	}
	op[-1] = pstate->s_file;
	esp[3] = *ppcproc;
	esp += 3;
    }
    return o_push_estack;
}

typedef struct named_scanner_option_s {
    const char *pname;
    int option;
} named_scanner_option_t;
static const named_scanner_option_t named_options[] = {
    {"PDFScanRules", SCAN_PDF_RULES},
    {"ProcessComment", SCAN_PROCESS_COMMENTS},
    {"ProcessDSCComment", SCAN_PROCESS_DSC_COMMENTS},
    {"PDFScanInvNum", SCAN_PDF_INV_NUM},
    {"PDFScanUnsigned", SCAN_PDF_UNSIGNED}
};

/*
 * Update the cached scanner_options in the context state after doing a
 * setuserparams.  (We might move this procedure somewhere else eventually.)
 */
int
ztoken_scanner_options(const ref *upref, int old_options)
{
    int options = old_options;
    int i;

    for (i = 0; i < countof(named_options); ++i) {
	const named_scanner_option_t *pnso = &named_options[i];
	ref *ppcproc;
	int code = dict_find_string(upref, pnso->pname, &ppcproc);

	/* Update the options only if the parameter has changed. */
	if (code >= 0) {
	    if (r_has_type(ppcproc, t_null))
		options &= ~pnso->option;
	    else
		options |= pnso->option;
	}
    }
    return options;
}
/*
 * Get the value for a scanner option.
 * return -1 if no such option, 1/0 for on/off and option's name in *pname as a C string
 */
int
ztoken_get_scanner_option(const ref *psref, int options, const char **pname)
{
    const named_scanner_option_t *pnso;

    for (pnso = named_options + countof(named_options); pnso-- != named_options;) {
	if (!bytes_compare((const byte *)pnso->pname, strlen(pnso->pname),
			psref->value.const_bytes, r_size(psref))) {
	    *pname = pnso->pname;
	    return (options & pnso->option ? 1 : 0);
	}
    }
    return -1;
}

/* ------ Initialization procedure ------ */

const op_def ztoken_op_defs[] =
{
    {"1token", ztoken},
    {"1.tokenexec", ztokenexec},
		/* Internal operators */
    {"2%ztoken_continue", ztoken_continue},
    {"2%ztokenexec_continue", ztokenexec_continue},
    op_def_end(0)
};
