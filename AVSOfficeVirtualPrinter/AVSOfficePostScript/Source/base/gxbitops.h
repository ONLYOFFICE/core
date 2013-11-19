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

/* $Id: gxbitops.h 8465 2007-12-29 02:53:21Z giles $ */
/* Internal definitions for bitmap operations */

#ifndef gxbitops_INCLUDED
#  define gxbitops_INCLUDED

#include "gsbitops.h"

/*
 * Macros for processing bitmaps in the largest possible chunks.
 * Bits within a byte are always stored big-endian;
 * bytes are likewise stored in left-to-right order, i.e., big-endian.
 * Note that this is the format used for the source of copy_mono.
 * It used to be the case that bytes were stored in the natural
 * platform order, and the client had force them into big-endian order
 * by calling gdev_mem_ensure_byte_order, but this no longer necessary.
 *
 * Note that we use type uint for register variables holding a chunk:
 * for this reason, the chunk size cannot be larger than uint.
 */
/* Generic macros for chunk accessing. */
#define cbytes(ct) size_of(ct)	/* sizeof may be unsigned */
#  define chunk_bytes cbytes(chunk)
/* The clog2_bytes macro assumes that ints are 2, 4, or 8 bytes in size. */
#define clog2_bytes(ct) (size_of(ct) == 8 ? 3 : size_of(ct)>>1)
#  define chunk_log2_bytes clog2_bytes(chunk)
#define cbits(ct) (size_of(ct)*8)	/* sizeof may be unsigned */
#  define chunk_bits cbits(chunk)
#define clog2_bits(ct) (clog2_bytes(ct)+3)
#  define chunk_log2_bits clog2_bits(chunk)
#define cbit_mask(ct) (cbits(ct)-1)
#  define chunk_bit_mask cbit_mask(chunk)
#define calign_bytes(ct)\
  (sizeof(ct) == 1 ? 1:\
   sizeof(ct) == sizeof(short) ? ARCH_ALIGN_SHORT_MOD :\
   sizeof(ct) == sizeof(int) ? ARCH_ALIGN_INT_MOD : ARCH_ALIGN_LONG_MOD)
#  define chunk_align_bytes calign_bytes(chunk)
#define calign_bit_mask(ct) (calign_bytes(ct)*8-1)
#  define chunk_align_bit_mask calign_bit_mask(chunk)
/*
 * The obvious definition for cmask is:
 *      #define cmask(ct) ((ct)~(ct)0)
 * but this doesn't work on the VAX/VMS compiler, which fails to truncate
 * the value to 16 bits when ct is ushort.
 * Instead, we have to generate the mask with no extra 1-bits.
 * We can't do this in the obvious way:
 *      #define cmask(ct) ((1 << (size_of(ct) * 8)) - 1)
 * because some compilers won't allow a shift of the full type size.
 * Instead, we have to do something really awkward:
 */
#define cmask(ct) ((ct) (((((ct)1 << (size_of(ct)*8-2)) - 1) << 2) + 3))
#  define chunk_all_bits cmask(chunk)
/*
 * The obvious definition for chi_bits is:
 *      #define chi_bits(ct,n) (cmask(ct)-(cmask(ct)>>(n)))
 * but this doesn't work on the DEC/MIPS compilers.
 * Instead, we have to restrict chi_bits to only working for values of n
 * between 0 and cbits(ct)-1, and use
 */
#define chi_bits(ct,n) (ct)(~(ct)1 << (cbits(ct)-1 - (n)))
#  define chunk_hi_bits(n) chi_bits(chunk,n)

/* Pointer arithmetic macros. */
#define inc_ptr(ptr,delta)\
	(ptr = (void *)((byte *)ptr + (delta)))

/* Define macros for setting up left- and right-end masks. */
/* These are used for monobit operations, and for filling */
/* with 2- and 4-bit-per-pixel patterns. */

/*
 * Define the chunk size for monobit copying operations.
 */
#if arch_is_big_endian
#  define mono_copy_chunk uint
#  define set_mono_right_mask(var, w)\
	(var = ((w) == chunk_bits ? chunk_all_bits : chunk_hi_bits(w)))
/*
 * We have to split the following statement because of a bug in the Xenix C
 * compiler (it produces a signed rather than an unsigned shift if we don't
 * split).
 */
#  define set_mono_thin_mask(var, w, bit)\
	set_mono_right_mask(var, w), var >>= (bit)
/*
 * We have to split the following statement in two because of a bug
 * in the DEC VAX/VMS C compiler.
 */
#  define set_mono_left_mask(var, bit)\
	(var = chunk_all_bits, var >>= (bit))
#else
#  define mono_copy_chunk bits16
extern const bits16 mono_copy_masks[17];

#  if mono_fill_chunk_bytes == 2
#    define mono_fill_masks mono_copy_masks
#  else
extern const bits32 mono_fill_masks[33];

#  endif
/*
 * We define mono_masks as either mono_fill_masks or
 * mono_copy_masks before using the following macros.
 */
#  define set_mono_left_mask(var, bit)\
	(var = mono_masks[bit])
#  define set_mono_thin_mask(var, w, bit)\
	(var = ~mono_masks[(w) + (bit)] & mono_masks[bit])
#  define set_mono_right_mask(var, ebit)\
	(var = ~mono_masks[ebit])
#endif

#endif /* gxbitops_INCLUDED */
