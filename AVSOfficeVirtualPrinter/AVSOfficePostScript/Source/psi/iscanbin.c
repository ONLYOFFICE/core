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

/* $Id: iscanbin.c 9043 2008-08-28 22:48:19Z giles $ */
/* Ghostscript binary token scanner and writer */
#include "math_.h"
#include "memory_.h"
#include "ghost.h"
#include "gsutil.h"
#include "gxalloc.h"		/* for names_array in allocator */
#include "stream.h"
#include "strimpl.h"		/* for sfilter.h */
#include "sfilter.h"		/* for iscan.h */
#include "ierrors.h"
#include "ialloc.h"
#include "iddict.h"
#include "dstack.h"		/* for immediately evaluated names */
#include "ostack.h"		/* must precede iscan.h */
#include "iname.h"
#include "iscan.h"		/* for scan_Refill */
#include "iscanbin.h"
#include "iutil.h"
#include "ivmspace.h"
#include "store.h"
#include "btoken.h"
#include "ibnum.h"

/* Define the binary token types. */
typedef enum {
    BT_SEQ = 128,		/* binary object sequence: */
    BT_SEQ_IEEE_MSB = 128,	/* IEEE floats, big-endian */
    BT_SEQ_IEEE_LSB = 129,	/* IEEE float, little-endian */
    BT_SEQ_NATIVE_MSB = 130,	/* native floats, big-endian */
    BT_SEQ_NATIVE_LSB = 131,	/* native floats, little-endian */
#define BT_IS_SEQ(btype) (((btype) & ~3) == BT_SEQ)
    BT_INT32_MSB = 132,
    BT_INT32_LSB = 133,
    BT_INT16_MSB = 134,
    BT_INT16_LSB = 135,
    BT_INT8 = 136,
    BT_FIXED = 137,
    BT_FLOAT_IEEE_MSB = 138,
    BT_FLOAT_IEEE_LSB = 139,
    BT_FLOAT_NATIVE = 140,
    BT_BOOLEAN = 141,
    BT_STRING_256 = 142,
    BT_STRING_64K_MSB = 143,
    BT_STRING_64K_LSB = 144,
    BT_LITNAME_SYSTEM = 145,
    BT_EXECNAME_SYSTEM = 146,
    BT_LITNAME_USER = 147,
    BT_EXECNAME_USER = 148,
    BT_NUM_ARRAY = 149
} bin_token_type_t;

#define MIN_BIN_TOKEN_TYPE 128
#define MAX_BIN_TOKEN_TYPE 159
#define NUM_BIN_TOKEN_TYPES (MAX_BIN_TOKEN_TYPE - MIN_BIN_TOKEN_TYPE + 1)

/* Define the number of required initial bytes for binary tokens. */
static const byte bin_token_bytes[NUM_BIN_TOKEN_TYPES] =
{
    4, 4, 4, 4, 5, 5, 3, 3, 2, 2, 5, 5, 5,
    2, 2, 3, 3, 2, 2, 2, 2, 4,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1	/* undefined */
};

/* Define the number formats for those binary tokens that need them. */
static const byte bin_token_num_formats[NUM_BIN_TOKEN_TYPES] =
{
    num_msb + num_float_IEEE,	/* BT_SEQ_IEEE_MSB */
    num_lsb + num_float_IEEE,	/* BT_SEQ_IEEE_LSB */
#if ARCH_FLOATS_ARE_IEEE && BYTE_SWAP_IEEE_NATIVE_REALS
    /* Treat native floats like IEEE floats for byte swapping. */
    num_msb + num_float_IEEE,	/* BT_SEQ_NATIVE_MSB */
    num_lsb + num_float_IEEE,	/* BT_SEQ_NATIVE_LSB */
#else
    num_msb + num_float_native,	/* BT_SEQ_NATIVE_MSB */
    num_lsb + num_float_native,	/* BT_SEQ_NATIVE_LSB */
#endif
    num_msb + num_int32,	/* BT_INT32_MSB */
    num_lsb + num_int32,	/* BT_INT32_LSB */
    num_msb + num_int16,	/* BT_INT16_MSB */
    num_lsb + num_int16,	/* BT_INT16_LSB */
    0,				/* BT_INT8, not used */
    0,				/* BT_FIXED, not used */
    num_msb + num_float_IEEE,	/* BT_FLOAT_IEEE_MSB */
    num_lsb + num_float_IEEE,	/* BT_FLOAT_IEEE_LSB */
    num_float_native,		/* BT_FLOAT_NATIVE */
    0,				/* BT_BOOLEAN, not used */
    0,				/* BT_STRING_256, not used */
    num_msb,			/* BT_STRING_64K_MSB */
    num_lsb			/* BT_STRING_64K_LSB */
    /* rest not used */
};

/* Binary object sequence element types */
typedef enum {
    BS_TYPE_NULL = 0,
    BS_TYPE_INTEGER = 1,
    BS_TYPE_REAL = 2,
    BS_TYPE_NAME = 3,
    BS_TYPE_BOOLEAN = 4,
    BS_TYPE_STRING = 5,
    BS_TYPE_EVAL_NAME = 6,
    BS_TYPE_ARRAY = 9,
    BS_TYPE_MARK = 10,
    /*
     * We extend the PostScript language definition by allowing
     * dictionaries in binary object sequences.  The data for
     * a dictionary is like that for an array, with the following
     * changes:
     *      - If the size is an even number, the value is the index of
     * the first of a series of alternating keys and values.
     *      - If the size is 1, the value is the index of another
     * object (which must also be a dictionary, and must not have
     * size = 1); this object represents the same object as that one.
     */
    BS_TYPE_DICTIONARY = 15
} bin_seq_type_t;

#define BS_EXECUTABLE 128
#define SIZEOF_BIN_SEQ_OBJ ((uint)8)

/* Forward references */
static int scan_bos(i_ctx_t *, ref *, scanner_state *);
static void scan_bos_error(scanner_state *, const char *);
static int scan_bin_scalar(i_ctx_t *, ref *, scanner_state *);
static int scan_bin_get_name(scanner_state *, const gs_memory_t *mem, const ref *, int, ref *, const char *);
static int scan_bin_num_array_continue(i_ctx_t *, ref *, scanner_state *);
static int scan_bin_string_continue(i_ctx_t *, ref *, scanner_state *);
static int scan_bos_continue(i_ctx_t *, ref *, scanner_state *);
static byte *scan_bos_resize(i_ctx_t *, scanner_state *, uint, uint);
static int scan_bos_string_continue(i_ctx_t *, ref *, scanner_state *);

/* Scan a binary token.  Called from the main scanner */
/* when it encounters an ASCII code 128-159, */
/* if binary tokens are being recognized (object format != 0). */
int
scan_binary_token(i_ctx_t *i_ctx_p, ref *pref, scanner_state *pstate)
{
    stream *const s = pstate->s_file.value.pfile;
    scan_binary_state *const pbs = &pstate->s_ss.binary;
    s_declare_inline(s, p, rlimit);
    int btype, code;
    uint wanted;

    s_begin_inline(s, p, rlimit);
    pbs->token_type = btype = *p;
    wanted = bin_token_bytes[btype - MIN_BIN_TOKEN_TYPE] - 1;
    if (rlimit - p < wanted) {
	s_end_inline(s, p - 1, rlimit);
	pstate->s_scan_type = scanning_none;
	code = scan_Refill;
    } else {
	pbs->num_format = bin_token_num_formats[btype - MIN_BIN_TOKEN_TYPE];
	if (BT_IS_SEQ(btype))
	    code = scan_bos(i_ctx_p, pref, pstate);
	else
	    code = scan_bin_scalar(i_ctx_p, pref, pstate);
    }
    if (code == scan_Refill && s->end_status == EOFC)
	code = gs_note_error(e_syntaxerror);
    if (code < 0 && pstate->s_error.string[0] == 0)
	snprintf(pstate->s_error.string, sizeof(pstate->s_error.string),
		 "binary token, type=%d", btype);
    return code;
}

/* Scan a binary object sequence. */
static int
scan_bos(i_ctx_t *i_ctx_p, ref *pref, scanner_state *pstate)
{
    stream *const s = pstate->s_file.value.pfile;
    scan_binary_state *const pbs = &pstate->s_ss.binary;
    s_declare_inline(s, p, rlimit);
    int num_format = pbs->num_format;
    int code;

    s_begin_inline(s, p, rlimit);
    {
	uint rcnt = rlimit - p;
	uint top_size = p[1];
	uint hsize, size;

	if (top_size == 0) {
	    /* Extended header (2-byte array size, 4-byte length) */
	    ulong lsize;

	    if (rcnt < 7) {
		s_end_inline(s, p - 1, rlimit);
		pstate->s_scan_type = scanning_none;
		return scan_Refill;
	    }
	    pbs->top_size = top_size = sdecodeushort(p + 2, num_format);
	    pbs->lsize = lsize = sdecodelong(p + 4, num_format);
	    if ((size = lsize) != lsize) {
		scan_bos_error(pstate, "bin obj seq length too large");
		return_error(e_limitcheck);
	    }
	    hsize = 8;
	} else {
	    /* Normal header (1-byte array size, 2-byte length). */
	    /* We already checked rcnt >= 3. */
	    pbs->top_size = top_size;
	    pbs->lsize = size = sdecodeushort(p + 2, num_format);
	    hsize = 4;
	}
	if (size < hsize || (size - hsize) >> 3 < top_size) {
	    scan_bos_error(pstate, "sequence too short");
	    return_error(e_syntaxerror); /* size too small */
	}
        { /* Preliminary syntax check to avoid potentialy large
           * memory allocation on junk data. Bug 688833
           */
          const unsigned char *q, *rend = p + hsize + top_size*8;

          if (rend > rlimit)
              rend = rlimit; 
          for (q = p + hsize + 1; q < rend; q += 8) {
             int c = q[-1] & 0x7f;
             if (c > 10 && c != BS_TYPE_DICTIONARY) {
		scan_bos_error(pstate, "invalid object type");
		return_error(e_syntaxerror);
             }
             if (*q != 0) {
		scan_bos_error(pstate, "non-zero unused field");
		return_error(e_syntaxerror);
             }
          }
        }
	/*
	 * Preallocate an array large enough for the worst case,
	 * namely, all objects and no strings.  Note that we must
	 * divide size by 8, not sizeof(ref), since array elements
	 * in binary tokens always occupy 8 bytes regardless of the
	 * size of a ref.
	 */
	code = ialloc_ref_array(&pbs->bin_array,
				a_all + a_executable, size / 8,
				"binary object sequence(objects)");
	if (code < 0)
	    return code;
	p += hsize - 1;
	size -= hsize;
	s_end_inline(s, p, rlimit);
	pbs->max_array_index = pbs->top_size = top_size;
	pbs->min_string_index = pbs->size = size;
	pbs->index = 0;
	pstate->s_da.is_dynamic = false;
	pstate->s_da.base = pstate->s_da.next =
	    pstate->s_da.limit = pstate->s_da.buf;
	code = scan_bos_continue(i_ctx_p, pref, pstate);
	if (code == scan_Refill || code < 0) {
	    /* Clean up array for GC. */
	    uint index = pbs->index;

	    refset_null(pbs->bin_array.value.refs + index,
			r_size(&pbs->bin_array) - index);
	}
	return code;
    }
}

/* Report an error in a binary object sequence. */
static void
scan_bos_error(scanner_state *pstate, const char *msg)
{
    snprintf(pstate->s_error.string, sizeof(pstate->s_error.string),
	     "bin obj seq, type=%d, elements=%u, size=%lu, %s",
	     pstate->s_ss.binary.token_type,
	     pstate->s_ss.binary.top_size,
	     pstate->s_ss.binary.lsize, msg);
}

/* Scan a non-sequence binary token. */
static int
scan_bin_scalar(i_ctx_t *i_ctx_p, ref *pref, scanner_state *pstate)
{
    stream *const s = pstate->s_file.value.pfile;
    scan_binary_state *const pbs = &pstate->s_ss.binary;
    s_declare_inline(s, p, rlimit);
    int num_format = pbs->num_format, code;
    uint wanted, arg;

    s_begin_inline(s, p, rlimit);
    wanted = bin_token_bytes[*p - MIN_BIN_TOKEN_TYPE] - 1;
    switch (*p) {
	case BT_INT8:
	    make_int(pref, (p[1] ^ 128) - 128);
	    s_end_inline(s, p + 1, rlimit);
	    return 0;
	case BT_FIXED:
	    num_format = p[1];
	    if (!num_is_valid(num_format))
		return_error(e_syntaxerror);
	    wanted = 1 + encoded_number_bytes(num_format);
	    if (rlimit - p < wanted) {
		s_end_inline(s, p - 1, rlimit);
		pstate->s_scan_type = scanning_none;
		return scan_Refill;
	    }
	    code = sdecode_number(p + 2, num_format, pref);
	    goto rnum;
	case BT_INT32_MSB:
	case BT_INT32_LSB:
	case BT_INT16_MSB:
	case BT_INT16_LSB:
	case BT_FLOAT_IEEE_MSB:
	case BT_FLOAT_IEEE_LSB:
	case BT_FLOAT_NATIVE:
	    code = sdecode_number(p + 1, num_format, pref);
	  rnum:
	    switch (code) {
		case t_integer:
		case t_real:
		    r_set_type(pref, code);
		    break;
		case t_null:
		    return_error(e_syntaxerror);
		default:
		    return code;
	    }
	    s_end_inline(s, p + wanted, rlimit);
	    return 0;
	case BT_BOOLEAN:
	    arg = p[1];
	    if (arg & ~1)
		return_error(e_syntaxerror);
	    make_bool(pref, arg);
	    s_end_inline(s, p + 1, rlimit);
	    return 0;
	case BT_STRING_256:
	    arg = *++p;
	    goto str;
	case BT_STRING_64K_MSB:
	case BT_STRING_64K_LSB:
	    arg = sdecodeushort(p + 1, num_format);
	    p += 2;
	  str:
	    if (s->foreign && rlimit - p >= arg) {
		/*
		 * Reference the string directly in the buffer.  It is
		 * marked writable for consistency with the non-direct
		 * case, but since the "buffer" may be data compiled into
		 * the executable, it is probably actually read-only.
		 */
		s_end_inline(s, p, rlimit);
		make_const_string(pref, a_all | avm_foreign, arg, sbufptr(s));
		sbufskip(s, arg);
		return 0;
	    } else {
		byte *str = ialloc_string(arg, "string token");

		if (str == 0)
		    return_error(e_VMerror);
		s_end_inline(s, p, rlimit);
		pstate->s_da.base = pstate->s_da.next = str;
		pstate->s_da.limit = str + arg;
		code = scan_bin_string_continue(i_ctx_p, pref, pstate);
		if (code == scan_Refill || code < 0) {
		    pstate->s_da.is_dynamic = true;
		    make_null(&pbs->bin_array);		/* clean up for GC */
		    pbs->cont = scan_bin_string_continue;
		}
		return code;
	    }
	case BT_LITNAME_SYSTEM:
	    code = scan_bin_get_name(pstate, imemory, system_names_p, p[1],
				     pref, "system");
	    goto lname;
	case BT_EXECNAME_SYSTEM:
	    code = scan_bin_get_name(pstate, imemory, system_names_p, p[1],
				     pref, "system");
	    goto xname;
	case BT_LITNAME_USER:
	    code = scan_bin_get_name(pstate, imemory, user_names_p, p[1],
				     pref, "user");
	  lname:
	    if (code < 0)
		return code;
	    s_end_inline(s, p + 1, rlimit);
	    return 0;
	case BT_EXECNAME_USER:
	    code = scan_bin_get_name(pstate, imemory, user_names_p, p[1],
				     pref, "user");
	  xname:
	    if (code < 0)
		return code;
	    r_set_attrs(pref, a_executable);
	    s_end_inline(s, p + 1, rlimit);
	    return 0;
	case BT_NUM_ARRAY:
	    num_format = p[1];
	    if (!num_is_valid(num_format))
		return_error(e_syntaxerror);
	    arg = sdecodeushort(p + 2, num_format);
	    code = ialloc_ref_array(&pbs->bin_array, a_all, arg,
				    "number array token");
	    if (code < 0)
		return code;
	    pbs->num_format = num_format;
	    pbs->index = 0;
	    p += 3;
	    s_end_inline(s, p, rlimit);
	    code = scan_bin_num_array_continue(i_ctx_p, pref, pstate);
	    if (code == scan_Refill || code < 0) {
		/* Make sure the array is clean for the GC. */
		refset_null(pbs->bin_array.value.refs + pbs->index,
			    arg - pbs->index);
		pbs->cont = scan_bin_num_array_continue;
	    }
	    return code;
    }
    return_error(e_syntaxerror);
}

/* Get a system or user name. */
static int
scan_bin_get_name(scanner_state *pstate, const gs_memory_t *mem,
		  const ref *pnames /*t_array*/, int index, ref *pref,
		  const char *usstring)
{
    /* Convert all errors to e_undefined to match Adobe. */
    if (pnames == 0 || array_get(mem, pnames, (long)index, pref) < 0 ||
	!r_has_type(pref, t_name)) {
	snprintf(pstate->s_error.string,
		 sizeof(pstate->s_error.string),
		 "%s%d", usstring, index);
	pstate->s_error.is_name = true;
	return_error(e_undefined);
    }
    return 0;
}

/* Continue collecting a binary string. */
static int
scan_bin_string_continue(i_ctx_t *i_ctx_p, ref * pref, scanner_state * pstate)
{
    stream *const s = pstate->s_file.value.pfile;
    byte *q = pstate->s_da.next;
    uint wanted = pstate->s_da.limit - q;
    uint rcnt;

    /* We don't check the return status from 'sgets' here.
       If there is an error in sgets, the condition rcnt==wanted
       would be false and this function will return scan_Refill.
    */
    sgets(s, q, wanted, &rcnt);
    if (rcnt == wanted) {
	/* Finished collecting the string. */
	make_string(pref, a_all | icurrent_space,
		    pstate->s_da.limit - pstate->s_da.base,
		    pstate->s_da.base);
	return 0;
    }
    pstate->s_da.next = q + rcnt;
    pstate->s_scan_type = scanning_binary;
    return scan_Refill;
}

/* Continue scanning a binary number array. */
static int
scan_bin_num_array_continue(i_ctx_t *i_ctx_p, ref * pref,
			    scanner_state * pstate)
{
    stream *const s = pstate->s_file.value.pfile;
    scan_binary_state *const pbs = &pstate->s_ss.binary;
    uint index = pbs->index;
    ref *np = pbs->bin_array.value.refs + index;
    uint wanted = encoded_number_bytes(pbs->num_format);

    for (; index < r_size(&pbs->bin_array); index++, np++) {
	int code;

	if (sbufavailable(s) < wanted) {
	    pbs->index = index;
	    pstate->s_scan_type = scanning_binary;
	    return scan_Refill;
	}
	code = sdecode_number(sbufptr(s), pbs->num_format, np);
	switch (code) {
	    case t_integer:
	    case t_real:
		r_set_type(np, code);
		sbufskip(s, wanted);
		break;
	    case t_null:
		scan_bos_error(pstate, "bad number format");
		return_error(e_syntaxerror);
	    default:
		return code;
	}
    }
    *pref = pbs->bin_array;
    return 0;
}

/*
 * Continue scanning a binary object sequence.  We preallocated space for
 * the largest possible number of objects, but not for strings, since
 * the latter would probably be a gross over-estimate.  Instead,
 * we wait until we see the first string or name, and allocate string space
 * based on the hope that its string index is the smallest one we will see.
 * If this turns out to be wrong, we may have to reallocate, and adjust
 * all the pointers.
 */
static int
scan_bos_continue(i_ctx_t *i_ctx_p, ref * pref, scanner_state * pstate)
{
    stream *const s = pstate->s_file.value.pfile;
    scan_binary_state *const pbs = &pstate->s_ss.binary;
    s_declare_inline(s, p, rlimit);
    uint max_array_index = pbs->max_array_index;
    uint min_string_index = pbs->min_string_index;
    int num_format = pbs->num_format;
    uint index = pbs->index;
    uint size = pbs->size;
    ref *abase = pbs->bin_array.value.refs;
    int code;

    pbs->cont = scan_bos_continue;  /* in case of premature return */
    s_begin_inline(s, p, rlimit);
    for (; index < max_array_index; p += SIZEOF_BIN_SEQ_OBJ, index++) {
	ref *op = abase + index;
	uint osize;
	long value;
	uint atype, attrs;

	s_end_inline(s, p, rlimit);	/* in case of error */
	if (rlimit - p < SIZEOF_BIN_SEQ_OBJ) {
	    pbs->index = index;
	    pbs->max_array_index = max_array_index;
	    pbs->min_string_index = min_string_index;
	    pstate->s_scan_type = scanning_binary;
	    return scan_Refill;
	}
	if (p[2] != 0) { /* reserved, must be 0 */
	    scan_bos_error(pstate, "non-zero unused field");
	    return_error(e_syntaxerror);
	}
	attrs = (p[1] & 128 ? a_executable : 0);
	/*
	 * We always decode all 8 bytes of the object, so we can signal
	 * syntaxerror if any unused field is non-zero (per PLRM).
	 */
  	osize = sdecodeushort(p + 3, num_format);
	value = sdecodelong(p + 5, num_format);
	switch (p[1] & 0x7f) {
	    case BS_TYPE_NULL:
		if (osize | value) { /* unused */
		    scan_bos_error(pstate, "non-zero unused field");
		    return_error(e_syntaxerror);
		}
		make_null(op);
		break;
	    case BS_TYPE_INTEGER:
		if (osize) {	/* unused */
		    scan_bos_error(pstate, "non-zero unused field");
		    return_error(e_syntaxerror);
		}
		make_int(op, value);
		break;
	    case BS_TYPE_REAL:{
		    float vreal;

		    if (osize != 0) {	/* fixed-point number */
			if (osize > 31) {
			    scan_bos_error(pstate, "invalid number format");
			    return_error(e_syntaxerror);
			}
			/* ldexp requires a signed 2nd argument.... */
			vreal = (float)ldexp((double)value, -(int)osize);
		    } else {
			code = sdecode_float(p + 5, num_format, &vreal);
			if (code < 0) {
			    scan_bos_error(pstate, "invalid real number");
			    return code;
			}
		    }
		    make_real(op, vreal);
		    break;
		}
	    case BS_TYPE_BOOLEAN:
		if (osize) {	/* unused */
		    scan_bos_error(pstate, "non-zero unused field");
		    return_error(e_syntaxerror);
		}
		make_bool(op, value != 0);
		break;
	    case BS_TYPE_STRING:
		attrs |= a_all;
	      str:
		if (osize == 0) {
		    /* For zero-length strings, the offset */
		    /* doesn't matter, and may be zero. */
		    make_empty_string(op, attrs);
		    break;
		}
		if (value < max_array_index * SIZEOF_BIN_SEQ_OBJ ||
		    value + osize > size
		    ) {
		    scan_bos_error(pstate, "invalid string offset");
		    return_error(e_syntaxerror);
		}
		if (value < min_string_index) {
		    /* We have to (re)allocate the strings. */
		    uint str_size = size - value;
		    byte *sbase;

		    if (pstate->s_da.is_dynamic)
			sbase = scan_bos_resize(i_ctx_p, pstate, str_size,
						index);
		    else
			sbase = ialloc_string(str_size,
					      "bos strings");
		    if (sbase == 0)
			return_error(e_VMerror);
		    pstate->s_da.is_dynamic = true;
		    pstate->s_da.base = pstate->s_da.next = sbase;
		    pstate->s_da.limit = sbase + str_size;
		    min_string_index = value;
		}
		make_string(op, attrs | icurrent_space, osize,
			    pstate->s_da.base +
			    (value - min_string_index));
		break;
	    case BS_TYPE_EVAL_NAME:
		attrs |= a_readonly;	/* mark as executable for later */
		/* falls through */
	    case BS_TYPE_NAME:
		switch (osize) {
		    case 0:
			code = scan_bin_get_name(pstate, imemory,
						 user_names_p, value, op,
						 "user");
			goto usn;
		    case 0xffff:
			code = scan_bin_get_name(pstate, imemory,
						 system_names_p, value, op,
						 "system");
		      usn:
			if (code < 0)
			    return code;
			r_set_attrs(op, attrs);
			break;
		    default:
			goto str;
		}
		break;
	    case BS_TYPE_ARRAY:
		atype = t_array;
	      arr:
		if (value + osize > min_string_index ||
		    value & (SIZEOF_BIN_SEQ_OBJ - 1)
		    ) {
		    scan_bos_error(pstate, "bad array offset");
		    return_error(e_syntaxerror);
		}
		{
		    uint aindex = value / SIZEOF_BIN_SEQ_OBJ;

		    max_array_index =
			max(max_array_index, aindex + osize);
		    make_tasv_new(op, atype,
				  attrs | a_all | icurrent_space,
				  osize, refs, abase + aindex);
		}
		break;
	    case BS_TYPE_DICTIONARY:	/* EXTENSION */
		if ((osize & 1) != 0 && osize != 1)
		    return_error(e_syntaxerror);
		atype = t_mixedarray;	/* mark as dictionary */
		goto arr;
	    case BS_TYPE_MARK:
		if (osize | value) { /* unused */
		    scan_bos_error(pstate, "non-zero unused field");
		    return_error(e_syntaxerror);
		}
		make_mark(op);
		break;
	    default:
		scan_bos_error(pstate, "invalid object type");
		return_error(e_syntaxerror);
	}
    }
    s_end_inline(s, p, rlimit);
    /* Shorten the objects to remove the space that turned out */
    /* to be used for strings. */
    pbs->index = max_array_index;
    iresize_ref_array(&pbs->bin_array, max_array_index,
		      "binary object sequence(objects)");
    code = scan_bos_string_continue(i_ctx_p, pref, pstate);
    if (code == scan_Refill)
	pbs->cont = scan_bos_string_continue;
    return code;
}

/* Reallocate the strings for a binary object sequence, */
/* adjusting all the pointers to them from objects. */
static byte *
scan_bos_resize(i_ctx_t *i_ctx_p, scanner_state * pstate, uint new_size,
		uint index)
{
    scan_binary_state *const pbs = &pstate->s_ss.binary;
    uint old_size = da_size(&pstate->s_da);
    byte *old_base = pstate->s_da.base;
    byte *new_base = iresize_string(old_base, old_size, new_size,
				    "scan_bos_resize");
    byte *relocated_base = new_base + (new_size - old_size);
    uint i;
    ref *aptr = pbs->bin_array.value.refs;

    if (new_base == 0)
	return 0;
    /* Since the allocator normally extends strings downward, */
    /* it's quite possible that new and old addresses are the same. */
    if (relocated_base != old_base)
	for (i = index; i != 0; i--, aptr++)
	    if (r_has_type(aptr, t_string) && r_size(aptr) != 0)
		aptr->value.bytes =
		    aptr->value.bytes - old_base + relocated_base;
    return new_base;
}

/* Continue reading the strings for a binary object sequence. */
static int
scan_bos_string_continue(i_ctx_t *i_ctx_p, ref * pref,
			 scanner_state * pstate)
{
    scan_binary_state *const pbs = &pstate->s_ss.binary;
    ref rstr;
    ref *op;
    int code = scan_bin_string_continue(i_ctx_p, &rstr, pstate);
    uint space = ialloc_space(idmemory);
    bool rescan = false;
    uint i;

    if (code != 0)
	return code;

    /* Fix up names.  We must do this before creating dictionaries. */

    for (op = pbs->bin_array.value.refs, i = r_size(&pbs->bin_array);
	 i != 0; i--, op++
	 )
	switch (r_type(op)) {
	    case t_string:
		if (r_has_attr(op, a_write))	/* a real string */
		    break;
		/* This is actually a name; look it up now. */
		{
		    uint attrs = r_type_attrs(op) & (a_read | a_executable);

		    code = name_ref(imemory, op->value.bytes, r_size(op), op, 1);
		    if (code < 0)
			return code;
		    r_set_attrs(op, attrs);
		}
		/* falls through */
	    case t_name:
		if (r_has_attr(op, a_read)) {	/* BS_TYPE_EVAL_NAME */
		    ref *defp = dict_find_name(op);

		    if (defp == 0)
			return_error(e_undefined);
		    store_check_space(space, defp);
		    ref_assign(op, defp);
		}
		break;
	    case t_mixedarray:	/* actually a dictionary */
		rescan = true;
	}

    /* Create dictionaries, if any. */

    if (rescan) {
	rescan = false;
	for (op = pbs->bin_array.value.refs, i = r_size(&pbs->bin_array);
	     i != 0; i--, op++
	     )
	    switch (r_type(op)) {
	    case t_mixedarray:	/* actually a dictionary */
		{
		    uint count = r_size(op);
		    ref rdict;

		    if (count == 1) {
			/* Indirect reference. */
			if (op->value.refs < op)
			    ref_assign(&rdict, op->value.refs);
			else {
			    rescan = true;
			    continue;
			}
		    } else {
			code = dict_create(count >> 1, &rdict);
			if (code < 0)
			    return code;
			while (count) {
			    count -= 2;
			    code = idict_put(&rdict,
					     &op->value.refs[count],
					     &op->value.refs[count + 1]);
			    if (code < 0)
				return code;
			}
		    }
		    r_set_attrs(&rdict, a_all);
		    r_copy_attrs(&rdict, a_executable, op);
		    ref_assign(op, &rdict);
		}
		break;
	    }
    }

    /* If there were any forward indirect references, fix them up now. */

    if (rescan)
	for (op = pbs->bin_array.value.refs, i = r_size(&pbs->bin_array);
	     i != 0; i--, op++
	    )
	    if (r_has_type(op, t_mixedarray)) {
		const ref *piref = op->value.const_refs;
		ref rdict;

		if (r_has_type(piref, t_mixedarray))	/* ref to indirect */
		    return_error(e_syntaxerror);
		ref_assign(&rdict, piref);
		r_copy_attrs(&rdict, a_executable, op);
		ref_assign(op, &rdict);
	    }

    ref_assign(pref, &pbs->bin_array);
    r_set_size(pref, pbs->top_size);
    return scan_BOS;
}

/* ---------------- Writing ---------------- */

/*
 * Encode a single object for a binary object sequence, for printobject and
 * write object.  Note that this does not modify the always-unused byte (1),
 * but it always write bytes 0 and 2-7.
 */
int
encode_binary_token(i_ctx_t *i_ctx_p, const ref *obj, long *ref_offset,
		    long *char_offset, byte *str)
{
    bin_seq_type_t type;
    uint size = 0;
    int format = (int)ref_binary_object_format.value.intval;
    long value = 0;
    ref nstr;

    switch (r_type(obj)) {
	case t_null:
	    type = BS_TYPE_NULL;
	    break;		/* always set all fields */
	case t_mark:
	    type = BS_TYPE_MARK;
	    break;		/* always set all fields */
	case t_integer:
	    type = BS_TYPE_INTEGER;
	    value = obj->value.intval;
	    break;
	case t_real:
	    type = BS_TYPE_REAL;
	    if (sizeof(obj->value.realval) != sizeof(int)) {
		/* The PLRM allocates exactly 4 bytes for reals. */
		return_error(e_rangecheck);
	    }
	    value = *(const int *)&obj->value.realval;
#if !(ARCH_FLOATS_ARE_IEEE && BYTE_SWAP_IEEE_NATIVE_REALS)
	    if (format >= 3) {
		/* Never byte-swap native reals -- use native byte order. */
		format = 4 - ARCH_IS_BIG_ENDIAN;
	    }
#endif
	    break;
	case t_boolean:
	    type = BS_TYPE_BOOLEAN;
	    value = obj->value.boolval;
	    break;
	case t_array:
	    type = BS_TYPE_ARRAY;
	    size = r_size(obj);
	    goto aod;
	case t_dictionary:	/* EXTENSION */
	    type = BS_TYPE_DICTIONARY;
	    size = dict_length(obj) << 1;
	  aod:value = *ref_offset;
	    *ref_offset += size * (ulong) SIZEOF_BIN_SEQ_OBJ;
	    break;
	case t_string:
	    type = BS_TYPE_STRING;
nos:
	    size = r_size(obj);
	    value = *char_offset;
	    *char_offset += size;
	    break;
	case t_name:
	    type = BS_TYPE_NAME;
	    name_string_ref(imemory, obj, &nstr);
	    r_copy_attrs(&nstr, a_executable, obj);
	    obj = &nstr;
	    goto nos;
	default:
	    return_error(e_rangecheck);
    }
    {
	byte s0 = (byte) size, s1 = (byte) (size >> 8);
	byte v0 = (byte) value, v1 = (byte) (value >> 8),
	    v2 = (byte) (value >> 16), v3 = (byte) (value >> 24);

	if (format & 1) {
	    /* Store big-endian */
	    str[2] = s1, str[3] = s0;
	    str[4] = v3, str[5] = v2, str[6] = v1, str[7] = v0;
	} else {
	    /* Store little-endian */
	    str[2] = s0, str[3] = s1;
	    str[4] = v0, str[5] = v1, str[6] = v2, str[7] = v3;
	}
    }
    if (r_has_attr(obj, a_executable))
	type += BS_EXECUTABLE;
    str[0] = (byte) type;
    return 0;
}
