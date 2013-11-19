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

/* $Id: zfrsd.c 9224 2008-11-17 19:43:47Z alexcher $ */
/* ReusableStreamDecode filter support */
#include "memory_.h"
#include "ghost.h"
#include "gsfname.h"		/* for gs_parse_file_name */
#include "gxiodev.h"
#include "oper.h"
#include "stream.h"
#include "strimpl.h"
#include "sfilter.h"		/* for SubFileDecode */
#include "files.h"
#include "idict.h"
#include "idparam.h"
#include "iname.h"
#include "istruct.h"
#include "store.h"
#include "zfile.h"

/* ---------------- Reusable streams ---------------- */

/*
 * The actual work of constructing the filter is done in PostScript code.
 * The operators in this file are internal ones that handle the dirty work.
 */

/* <dict|null> .rsdparams <filters> <decodeparms|null> */
/* filters is always an array; decodeparms is always either an array */
/* of the same length as filters, or null. */
static int
zrsdparams(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    ref *pFilter;
    ref *pDecodeParms;
    int Intent = 0;
    bool AsyncRead;
    ref empty_array, filter1_array, parms1_array;
    uint i;
    int code;

    make_empty_array(&empty_array, a_readonly);
    if (dict_find_string(op, "Filter", &pFilter) > 0) {
	if (!r_is_array(pFilter)) {
	    if (!r_has_type(pFilter, t_name))
		return_error(e_typecheck);
	    make_array(&filter1_array, a_readonly, 1, pFilter);
	    pFilter = &filter1_array;
	}
    } else
	pFilter = &empty_array;
    /* If Filter is undefined, ignore DecodeParms. */
    if (pFilter != &empty_array &&
	dict_find_string(op, "DecodeParms", &pDecodeParms) > 0
	) {
	if (pFilter == &filter1_array) {
	    make_array(&parms1_array, a_readonly, 1, pDecodeParms);
	    pDecodeParms = &parms1_array;
	} else if (!r_is_array(pDecodeParms))
	    return_error(e_typecheck);
	else if (r_size(pFilter) != r_size(pDecodeParms))
	    return_error(e_rangecheck);
    } else
	pDecodeParms = 0;
    for (i = 0; i < r_size(pFilter); ++i) {
	ref f, fname, dp;

	array_get(imemory, pFilter, (long)i, &f);
	if (!r_has_type(&f, t_name))
	    return_error(e_typecheck);
	name_string_ref(imemory, &f, &fname);
	if (r_size(&fname) < 6 ||
	    memcmp(fname.value.bytes + r_size(&fname) - 6, "Decode", 6)
	    )
	    return_error(e_rangecheck);
	if (pDecodeParms) {
	    array_get(imemory, pDecodeParms, (long)i, &dp);
	    if (!(r_has_type(&dp, t_dictionary) || r_has_type(&dp, t_null)))
		return_error(e_typecheck);
	}
    }
    code = dict_int_param(op, "Intent", 0, 3, 0, &Intent);
    if (code < 0 && code != e_rangecheck) /* out-of-range int is ok, use 0 */
	return code;
    if ((code = dict_bool_param(op, "AsyncRead", false, &AsyncRead)) < 0
	)
	return code;
    push(1);
    op[-1] = *pFilter;
    if (pDecodeParms)
	*op = *pDecodeParms;
    else
	make_null(op);
    return 0;
}

/* <file|string> <CloseSource> .reusablestream <filter> */
/*
 * The file|string operand must be a "reusable source", either:
 *      - A string or bytestring;
 *      - An array of strings;
 *      - A readable, positionable file stream;
 *      - A readable string stream;
 *      - A SubFileDecode filter with an empty EODString and a reusable
 *      source.
 * Reusable streams are also reusable sources, but they look just like
 * ordinary file or string streams.
 */
static int make_rss(i_ctx_t *i_ctx_p, os_ptr op, const byte * data,
		     uint size, uint space, long offset, long length,
		     bool is_bytestring);
static int make_rfs(i_ctx_t *i_ctx_p, os_ptr op, stream *fs,
		     long offset, long length);

static int make_aos(i_ctx_t *i_ctx_p, os_ptr op,
                    int blk_sz, int blk_sz_last, unsigned int file_sz);

static int
zreusablestream(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    os_ptr source_op = op - 1;
    long length = max_long;
    bool close_source;
    int code;

    check_type(*op, t_boolean);
    close_source = op->value.boolval;
    if (r_has_type(source_op, t_string)) {
	uint size = r_size(source_op);

	check_read(*source_op);
	code = make_rss(i_ctx_p, source_op, source_op->value.const_bytes,
			size, r_space(source_op), 0L, size, false);
    } else if (r_has_type(source_op, t_astruct)) {
	uint size = gs_object_size(imemory, source_op->value.pstruct);

	if (gs_object_type(imemory, source_op->value.pstruct) != &st_bytes)
	    return_error(e_rangecheck);
	check_read(*source_op);
	code = make_rss(i_ctx_p, source_op,
			(const byte *)source_op->value.pstruct, size,
			r_space(source_op), 0L, size, true);
    } else if (r_has_type(source_op, t_array)) {  /* no packedarrays */
	int i, blk_cnt, blk_sz;
        ref *blk_ref;
        ulong filelen = 0;

        check_read(*source_op);
        blk_cnt = r_size(source_op);
	blk_ref = source_op->value.refs;
        if (blk_cnt > 0) {
            blk_sz = r_size(blk_ref);
            for (i = 0; i < blk_cnt; i++) {
                int len;

                check_read_type(blk_ref[i], t_string);
                len = r_size(&blk_ref[i]);
                if (len > blk_sz || (len < blk_sz && i < blk_cnt - 1))
                   return_error(e_rangecheck); /* last block can be smaller */
                filelen += len;
            }
        }
        if (filelen == 0) {
           code = make_rss(i_ctx_p, source_op, (unsigned char *)"", 0,
	       r_space(source_op), 0, 0, false);
        } else {
           code = make_aos(i_ctx_p, source_op, blk_sz, r_size(&blk_ref[blk_cnt - 1]), filelen);
        }
    } else {
	long offset = 0;
	stream *source;
	stream *s;

	check_read_file(source, source_op);
	s = source;
rs:
	if (s->cbuf_string.data != 0) {	/* string stream */
	    long pos = stell(s);
	    long avail = sbufavailable(s) + pos;

	    offset += pos;
	    code = make_rss(i_ctx_p, source_op, s->cbuf_string.data,
			    s->cbuf_string.size,
			    imemory_space((const gs_ref_memory_t *)s->memory),
			    offset, min(avail, length), false);
	} else if (s->file != 0) { /* file stream */
	    if (~s->modes & (s_mode_read | s_mode_seek))
		return_error(e_ioerror);
	    code = make_rfs(i_ctx_p, source_op, s, offset + stell(s), length);
	} else if (s->state->template == &s_SFD_template) {
	    /* SubFileDecode filter */
	    const stream_SFD_state *const sfd_state =
		(const stream_SFD_state *)s->state;

	    if (sfd_state->eod.size != 0)
		return_error(e_rangecheck);
	    offset += sfd_state->skip_count - sbufavailable(s);
	    if (sfd_state->count != 0) {
		long left = max(sfd_state->count, 0) + sbufavailable(s);

		if (left < length)
		    length = left;
	    }
	    s = s->strm;
	    goto rs;
	}
	else			/* some other kind of stream */
	    return_error(e_rangecheck);
	if (close_source) {
	    stream *rs = fptr(source_op);

	    rs->strm = source;	/* only for close_source */
	    rs->close_strm = true;
	}
    }
    if (code >= 0)
	pop(1);
    return code;
}

/* Make a reusable string stream. */
static int
make_rss(i_ctx_t *i_ctx_p, os_ptr op, const byte * data, uint size,
	 uint string_space, long offset, long length, bool is_bytestring)
{
    uint save_space = icurrent_space;
    stream *s;
    long left = min(length, size - offset);

    ialloc_set_space(idmemory, string_space);
    s = file_alloc_stream(imemory, "make_rss");
    ialloc_set_space(idmemory, save_space);
    if (s == 0)
	return_error(e_VMerror);
    sread_string_reusable(s, data + offset, max(left, 0));
    if (is_bytestring)
	s->cbuf_string.data = 0;	/* byte array, not string */
    make_stream_file(op, s, "r");
    return 0;
}

/* Make a reusable file stream. */
static int
make_rfs(i_ctx_t *i_ctx_p, os_ptr op, stream *fs, long offset, long length)
{
    uint save_space = icurrent_space;
    uint stream_space = imemory_space((const gs_ref_memory_t *)fs->memory);
    gs_const_string fname;
    gs_parsed_file_name_t pname;
    stream *s;
    int code;

    if (sfilename(fs, &fname) < 0)
	return_error(e_ioerror);
    code = gs_parse_file_name(&pname, (const char *)fname.data, fname.size);
    if (code < 0)
	return code;
    if (pname.len == 0)		/* %stdin% etc. won't have a filename */
	return_error(e_invalidfileaccess); /* can't reopen */
    if (pname.iodev == NULL)
	pname.iodev = iodev_default;
    /* Open the file again, to be independent of the source. */
    ialloc_set_space(idmemory, stream_space);
    code = zopen_file(i_ctx_p, &pname, "r", &s, imemory);
    ialloc_set_space(idmemory, save_space);
    if (code < 0)
	return code;
    if (sread_subfile(s, offset, length) < 0) {
	sclose(s);
	return_error(e_ioerror);
    }
    s->close_at_eod = false;
    make_stream_file(op, s, "r");
    return 0;
}
/* ----------- Reusable array-of-strings stream ------------- */

static int  s_aos_available(stream *, long *);
static int  s_aos_seek(stream *, long);
static void s_aos_reset(stream *s);
static int  s_aos_flush(stream *s);
static int  s_aos_close(stream *);
static int  s_aos_process(stream_state *, stream_cursor_read *,
	                stream_cursor_write *, bool);

/* Stream state */
typedef struct aos_state_s {
    stream_state_common;
    ref   blocks;
    stream *s;	
    int   blk_sz;
    int   blk_sz_last;
    uint  file_sz; 
} aos_state_t;

/* GC procedures */
static 
CLEAR_MARKS_PROC(aos_clear_marks)
{   aos_state_t *const pptr = vptr;

    r_clear_attrs(&pptr->blocks, l_mark);
}
static 
ENUM_PTRS_WITH(aos_enum_ptrs, aos_state_t *pptr) return 0;
ENUM_PTR(0, aos_state_t, s);
case 1:
ENUM_RETURN_REF(&pptr->blocks);
ENUM_PTRS_END
static RELOC_PTRS_WITH(aos_reloc_ptrs, aos_state_t *pptr);
RELOC_PTR(aos_state_t, s);
RELOC_REF_VAR(pptr->blocks);
r_clear_attrs(&pptr->blocks, l_mark);
RELOC_PTRS_END

gs_private_st_complex_only(st_aos_state, aos_state_t,
    "aos_state", aos_clear_marks, aos_enum_ptrs, aos_reloc_ptrs, 0);

/* Stream template */
static const stream_template s_aos_template = {	
     &st_aos_state, 0, s_aos_process, 1, 1, 0, 0 };

/* Stream procs */
static const stream_procs s_aos_procs = { 
     s_aos_available, s_aos_seek, s_aos_reset,
     s_aos_flush, s_aos_close, s_aos_process,
     NULL		/* no s_aos_switch */
};

static int
make_aos(i_ctx_t *i_ctx_p, os_ptr op, int blk_sz, int blk_sz_last, uint file_sz)
{
    stream *s;
    aos_state_t *ss;
    byte *buf;
    const int aos_buf_size = 1024; /* arbitrary */
    uint save_space = icurrent_space;
    ialloc_set_space(idmemory, r_space(op));
    
    s = s_alloc(imemory, "aos_stream");
    ss = (aos_state_t *)s_alloc_state(imemory, &st_aos_state, "st_aos_state");
    buf = gs_alloc_bytes(imemory, aos_buf_size, "aos_stream_buf");
    if (s == 0 || ss == 0 || buf == 0) {
        gs_free_object(imemory, buf, "aos_stream_buf");
        gs_free_object(imemory, ss, "st_aos_state");
        gs_free_object(imemory, s, "aos_stream");
        ialloc_set_space(idmemory, save_space);
        return_error(e_VMerror);
    }
    ialloc_set_space(idmemory, save_space);
    ss->template = &s_aos_template;
    ss->blocks = *op;
    ss->s = s;
    ss->blk_sz = blk_sz;
    ss->blk_sz_last = blk_sz_last;
    ss->file_sz = file_sz;
    s_std_init(s, buf, aos_buf_size, &s_aos_procs, s_mode_read + s_mode_seek);
    s->state = (stream_state *)ss;
    s->file_offset = 0;
    s->file_limit = max_long;
    s->close_at_eod = false;
    s->read_id = 1;
    make_stream_file(op, s, "r");
    return 0;
}

/* Return the number of available bytes */
static int
s_aos_available(stream *s, long *pl)
{
    *pl = ((aos_state_t *)s->state)->file_sz - stell(s);
    return 0;
}

/* Seek in a string being read.  Return 0 if OK, ERRC if not. */
static int
s_aos_seek(register stream * s, long pos)
{
    uint end = s->srlimit - s->cbuf + 1;
    long offset = pos - s->position;

    if (offset >= 0 && offset <= end) {  /* Staying within the same buffer */
	s->srptr = s->cbuf + offset - 1;
	return 0;
    }
    if (pos < 0 || pos > s->file_limit)
	return ERRC;
    s->srptr = s->srlimit = s->cbuf - 1;
    s->end_status = 0;
    s->position = pos;
    return 0;
}

static void
s_aos_reset(stream *s)
{
    /* PLRM definition of reset operator is strange. */
    /* Rewind the file and discard the buffer. */
    s->position = 0; 
    s->srptr = s->srlimit = s->cbuf - 1;
    s->end_status = 0;
}

static int
s_aos_flush(stream *s)
{
    s->position = ((aos_state_t *)s->state)->file_sz;
    s->srptr = s->srlimit = s->cbuf - 1;
    return 0;
}

static int
s_aos_close(stream * s)
{
    gs_free_object(s->memory, s->cbuf, "s_aos_close(buffer)");
    s->cbuf = 0;
    /* Increment the IDs to prevent further access. */
    s->read_id = s->write_id = (s->read_id | s->write_id) + 1;
    return 0;
}

static int
s_aos_process(stream_state * st, stream_cursor_read * ignore_pr,
		      stream_cursor_write * pw, bool last)
{
    int blk_i, blk_off, blk_cnt, status = 1;
    uint count;
    aos_state_t *ss = (aos_state_t *)st;
    uint max_count = pw->limit - pw->ptr;
    uint pos = stell(ss->s);
    unsigned const char *data;
    ref *blk_ref;

    if (pos >= ss->file_sz)
	return EOFC;
    pos += sbufavailable(ss->s);
    blk_i   = pos / ss->blk_sz;
    blk_off = pos % ss->blk_sz;
    blk_cnt = r_size(&ss->blocks);
    count = blk_i < blk_cnt - 1 ? ss->blk_sz : ss->blk_sz_last;
    blk_ref = ss->blocks.value.refs;
    data = blk_ref[blk_i].value.bytes;

    if (max_count > count - blk_off) {
        max_count = count - blk_off;
        if (blk_i == blk_cnt - 1)
	     status = EOFC;	
    }
    memcpy(pw->ptr+1, data + blk_off, max_count);
    pw->ptr += max_count;
    return status;
}

/* ---------------- Initialization procedure ---------------- */

const op_def zfrsd_op_defs[] =
{
    {"2.reusablestream", zreusablestream},
    {"2.rsdparams", zrsdparams},
    op_def_end(0)
};
