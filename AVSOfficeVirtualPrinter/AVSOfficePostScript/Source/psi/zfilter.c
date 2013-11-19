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

/* $Id: zfilter.c 9043 2008-08-28 22:48:19Z giles $ */
/* Filter creation */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "ialloc.h"
#include "idict.h"
#include "idparam.h"
#include "ilevel.h"		/* SubFileDecode is different in LL3 */
#include "stream.h"
#include "strimpl.h"
#include "sfilter.h"
#include "srlx.h"
#include "sstring.h"
#include "ifilter.h"
#include "files.h"		/* for filter_open, file_d'_buffer_size */

/* <source> ASCIIHexEncode/filter <file> */
/* <source> <dict> ASCIIHexEncode/filter <file> */
static int
zAXE(i_ctx_t *i_ctx_p)
{
    return filter_write_simple(i_ctx_p, &s_AXE_template);
}

/* <target> ASCIIHexDecode/filter <file> */
/* <target> <dict> ASCIIHexDecode/filter <file> */
static int
zAXD(i_ctx_t *i_ctx_p)
{
    return filter_read_simple(i_ctx_p, &s_AXD_template);
}

/* <target> NullEncode/filter <file> */
/* <target> <dict_ignored> NullEncode/filter <file> */
static int
zNullE(i_ctx_t *i_ctx_p)
{
    return filter_write_simple(i_ctx_p, &s_NullE_template);
}

/* <source> <bool> PFBDecode/filter <file> */
/* <source> <dict> <bool> PFBDecode/filter <file> */
static int
zPFBD(i_ctx_t *i_ctx_p)
{
    os_ptr sop = osp;
    stream_PFBD_state state;

    check_type(*sop, t_boolean);
    state.binary_to_hex = sop->value.boolval;
    return filter_read(i_ctx_p, 1, &s_PFBD_template, (stream_state *)&state, 0);
}

/* <target> PSStringEncode/filter <file> */
/* <target> <dict> PSStringEncode/filter <file> */
static int
zPSSE(i_ctx_t *i_ctx_p)
{
    return filter_write_simple(i_ctx_p, &s_PSSE_template);
}

/* ------ RunLength filters ------ */

/* Common setup for RLE and RLD filters. */
static int
rl_setup(os_ptr dop, bool * eod)
{
    if (r_has_type(dop, t_dictionary)) {
	int code;

	check_dict_read(*dop);
	if ((code = dict_bool_param(dop, "EndOfData", true, eod)) < 0)
	    return code;
	return 1;
    } else {
	*eod = true;
	return 0;
    }
}

/* <target> <record_size> RunLengthEncode/filter <file> */
/* <target> <dict> <record_size> RunLengthEncode/filter <file> */
static int
zRLE(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream_RLE_state state;
    int code;

    check_op(2);
    code = rl_setup(op - 1, &state.EndOfData);
    if (code < 0)
	return code;
    check_int_leu(*op, max_uint);
    state.record_size = op->value.intval;
    return filter_write(i_ctx_p, 1, &s_RLE_template, (stream_state *) & state, 0);
}

/* <source> RunLengthDecode/filter <file> */
/* <source> <dict> RunLengthDecode/filter <file> */
static int
zRLD(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream_RLD_state state;
    int code = rl_setup(op, &state.EndOfData);

    if (code < 0)
	return code;
    return filter_read(i_ctx_p, 0, &s_RLD_template, (stream_state *) & state, 0);
}

/* <source> <EODcount> <EODstring> SubFileDecode/filter <file> */
/* <source> <dict> <EODcount> <EODstring> SubFileDecode/filter <file> */
/* <source> <dict> SubFileDecode/filter <file> *//* (LL3 only) */
int				/* exported for zsysvm.c */
zSFD(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream_SFD_state state;
    ref *sop = op;
    int npop;

    if (s_SFD_template.set_defaults)
	s_SFD_template.set_defaults((stream_state *)&state);
    if (LL3_ENABLED && r_has_type(op, t_dictionary)) {
	int count;
	int code;

	check_dict_read(*op);
	/*
	 * The PLRM-3rd says that EODCount is a required parameter.  However
	 * Adobe accepts files without this value and apparently defaults to
	 * zero.  Thus we are doing the same.
	 */
	if ((code = dict_int_param(op, "EODCount", 0, max_int, 0, &count)) < 0)
	    return code;
	if (dict_find_string(op, "EODString", &sop) <= 0)
	    return_error(e_rangecheck);
	state.count = count;
	npop = 0;
    } else {
	check_type(sop[-1], t_integer);
	if (sop[-1].value.intval < 0)
	    return_error(e_rangecheck);
	state.count = sop[-1].value.intval;
	npop = 2;
    }
    check_read_type(*sop, t_string);
    state.eod.data = sop->value.const_bytes;
    state.eod.size = r_size(sop);
    return filter_read(i_ctx_p, npop, &s_SFD_template,
		       (stream_state *)&state, r_space(sop));
}

/* ------ Utilities ------ */

/* Forward references */
static int filter_ensure_buf(stream **, uint, gs_ref_memory_t *, bool, int );

/* Set up an input filter. */
int
filter_read(i_ctx_t *i_ctx_p, int npop, const stream_template * template,
	    stream_state * st, uint space)
{
    os_ptr op = osp;
    uint min_size = template->min_out_size + max_min_left;
    uint save_space = ialloc_space(idmemory);
    /* PLRM3 requires the following, *not* max(space, save_space). */
    uint use_space = max(space, avm_system); /* don't alloc in foreign space */
    os_ptr sop = op - npop;
    stream *s;
    stream *sstrm;
    bool close = false;
    int code;

    /* Skip over an optional dictionary parameter. */
    if (r_has_type(sop, t_dictionary)) {
	check_dict_read(*sop);
	if ((code = dict_bool_param(sop, "CloseSource", false, &close)) < 0)
	    return code;
	--sop;
    }
    /*
     * Check to make sure that the underlying data
     * can function as a source for reading.
     */
    use_space = max(use_space, r_space(sop));
    switch (r_type(sop)) {
	case t_string:
	    check_read(*sop);
	    ialloc_set_space(idmemory, use_space);
	    sstrm = file_alloc_stream(imemory, "filter_read(string stream)");
	    if (sstrm == 0) {
		code = gs_note_error(e_VMerror);
		goto out;
	    }
	    sread_string(sstrm, sop->value.bytes, r_size(sop));
	    sstrm->is_temp = 1;
	    break;
	case t_file:
            check_read_known_file_else(sstrm, sop, return, DO_NOTHING);
	    ialloc_set_space(idmemory, use_space);
	    goto ens;
	default:
	    check_proc(*sop);
	    ialloc_set_space(idmemory, use_space);
	    code = sread_proc(sop, &sstrm, iimemory);
	    if (code < 0)
		goto out;
	    sstrm->is_temp = 2;
	  ens:
	    code = filter_ensure_buf(&sstrm,
				     template->min_in_size +
				     sstrm->state->template->min_out_size,
				     iimemory, false, close);
	    if (code < 0)
		goto out;
	    break;
    }
    if (min_size < 128)
	min_size = file_default_buffer_size;
    code = filter_open("r", min_size, (ref *) sop,
		       &s_filter_read_procs, template, st, imemory);
    if (code < 0)
	goto out;
    s = fptr(sop);
    s->strm = sstrm;
    s->close_strm = close;
    pop(op - sop);
out:
    ialloc_set_space(idmemory, save_space);
    return code;
}
int
filter_read_simple(i_ctx_t *i_ctx_p, const stream_template * template)
{
    return filter_read(i_ctx_p, 0, template, NULL, 0);
}

/* Set up an output filter. */
int
filter_write(i_ctx_t *i_ctx_p, int npop, const stream_template * template,
	     stream_state * st, uint space)
{
    os_ptr op = osp;
    uint min_size = template->min_in_size + max_min_left;
    uint save_space = ialloc_space(idmemory);
    /* PLRM3 requires the following, *not* max(space, save_space). */
    uint use_space = max(space, avm_system); /* don't alloc in foreign space */
    os_ptr sop = op - npop;
    stream *s;
    stream *sstrm;
    bool close = false;
    int code;

    /* Skip over an optional dictionary parameter. */
    if (r_has_type(sop, t_dictionary)) {
	check_dict_read(*sop);
	if ((code = dict_bool_param(sop, "CloseTarget", false, &close)) < 0)
	    return code;
	--sop;
    }
    /*
     * Check to make sure that the underlying data
     * can function as a sink for writing.
     */
    use_space = max(use_space, r_space(sop));
    switch (r_type(sop)) {
	case t_string:
	    check_write(*sop);
	    ialloc_set_space(idmemory, use_space);
	    sstrm = file_alloc_stream(imemory, "filter_write(string)");
	    if (sstrm == 0) {
		code = gs_note_error(e_VMerror);
		goto out;
	    }
	    swrite_string(sstrm, sop->value.bytes, r_size(sop));
	    sstrm->is_temp = 1;
	    break;
	case t_file:
	    check_write_known_file(sstrm, sop, return);
	    ialloc_set_space(idmemory, use_space);
	    goto ens;
	default:
	    check_proc(*sop);
	    ialloc_set_space(idmemory, use_space);
	    code = swrite_proc(sop, &sstrm, iimemory);
	    if (code < 0)
		goto out;
	    sstrm->is_temp = 2;
	  ens:
	    code = filter_ensure_buf(&sstrm,
				     template->min_out_size +
				     sstrm->state->template->min_in_size,
				     iimemory, true, close);
	    if (code < 0)
		goto out;
	    break;
    }
    if (min_size < 128)
	min_size = file_default_buffer_size;
    code = filter_open("w", min_size, (ref *) sop,
		       &s_filter_write_procs, template, st, imemory);
    if (code < 0)
	goto out;
    s = fptr(sop);
    s->strm = sstrm;
    s->close_strm = close;
    pop(op - sop);
out:
    ialloc_set_space(idmemory, save_space);
    return code;
}
int
filter_write_simple(i_ctx_t *i_ctx_p, const stream_template * template)
{
    return filter_write(i_ctx_p, 0, template, NULL, 0);
}

/* Define a byte-at-a-time NullDecode filter for intermediate buffers. */
/* (The standard NullDecode filter can read ahead too far.) */
static int
s_Null1D_process(stream_state * st, stream_cursor_read * pr,
		 stream_cursor_write * pw, bool last)
{
    if (pr->ptr >= pr->limit)
	return 0;
    if (pw->ptr >= pw->limit)
	return 1;
    *++(pw->ptr) = *++(pr->ptr);
    return 1;
}
static const stream_template s_Null1D_template = {
    &st_stream_state, NULL, s_Null1D_process, 1, 1
};

/* A utility filter that returns an immediate EOF without consuming */
/* any data from its source. Used by PDF interpreter for unknown    */
/* filter types.                                                    */
static int
s_EOFD_process(stream_state * st, stream_cursor_read * pr,
		 stream_cursor_write * pw, bool last)
{
    return EOFC;
}
static const stream_template s_EOFD_template = {
    &st_stream_state, NULL, s_EOFD_process, 1, 1
};

/* <target> /.EOFDecode filter <file> */
/* <target> <dict> /.EOFDecode filter <file> */
static int
zEOFD(i_ctx_t *i_ctx_p)
{
    return filter_read_simple(i_ctx_p, &s_EOFD_template);
}


/* Ensure a minimum buffer size for a filter. */
/* This may require creating an intermediate stream. */
static int
filter_ensure_buf(stream ** ps, uint min_buf_size, gs_ref_memory_t *imem,
		  bool writing, int close)
{
    stream *s = *ps;
    uint min_size = min_buf_size + max_min_left;
    stream *bs;
    ref bsop;
    int code;

    if (s->modes == 0 /* stream is closed */  || s->bsize >= min_size)
	return 0;
    /* Otherwise, allocate an intermediate stream. */
    if (s->cbuf == 0) {
	/* This is a newly created procedure stream. */
	/* Just allocate a buffer for it. */
	uint len = max(min_size, 128);
	byte *buf = gs_alloc_bytes((gs_memory_t *)imem, len,
				   "filter_ensure_buf");

	if (buf == 0)
	    return_error(e_VMerror);
	s->cbuf = buf;
	s->srptr = s->srlimit = s->swptr = buf - 1;
	s->swlimit = buf - 1 + len;
	s->bsize = s->cbsize = len;
	return 0;
    } else {
	/* Allocate an intermediate stream. */
	if (writing)
	    code = filter_open("w", min_size, &bsop, &s_filter_write_procs,
			       &s_NullE_template, NULL, (gs_memory_t *)imem);
	else
	    code = filter_open("r", min_size, &bsop, &s_filter_read_procs,
			       &s_Null1D_template, NULL, (gs_memory_t *)imem);
	if (code < 0)
	    return code;
	bs = fptr(&bsop);
	bs->strm = s;
	bs->is_temp = 2;
        bs->close_strm = close;
	*ps = bs;
	return code;
    }
}

/* Mark a (filter) stream as temporary. */
/* We define this here to avoid importing stream.h into zf*.c. */
void
filter_mark_temp(const ref * fop, int is_temp)
{
    fptr(fop)->is_temp = is_temp;
}

/* Mark the source or target of a filter as temporary, and propagate */
/* close_strm from the temporary stream to the filter. */
void
filter_mark_strm_temp(const ref * fop, int is_temp)
{
    stream *s = fptr(fop);
    stream *strm = s->strm;

    strm->is_temp = is_temp;
    s->close_strm = strm->close_strm;
}

/* ------ Initialization procedure ------ */

const op_def zfilter_op_defs[] = {
		/* We enter PSStringEncode and SubFileDecode (only) */
		/* as separate operators. */
    {"1.psstringencode", zPSSE},
    {"2.subfiledecode", zSFD},
    op_def_begin_filter(),
    {"1ASCIIHexEncode", zAXE},
    {"1ASCIIHexDecode", zAXD},
    {"1NullEncode", zNullE},
    {"2PFBDecode", zPFBD},
    {"1PSStringEncode", zPSSE},
    {"2RunLengthEncode", zRLE},
    {"1RunLengthDecode", zRLD},
    {"3SubFileDecode", zSFD},
    {"1.EOFDecode", zEOFD},
    op_def_end(0)
};
