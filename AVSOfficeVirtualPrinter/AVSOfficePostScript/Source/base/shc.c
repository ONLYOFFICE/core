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

/* $Id: shc.c 8022 2007-06-05 22:23:38Z giles $ */
/* Support code for shc.h */
#include "std.h"
#include "scommon.h"
#include "shc.h"

/* ------ Encoding ------ */

/* Empty the 1-word buffer onto the output stream. */
/* q has already been incremented. */
void
hc_put_code_proc(bool reverse_bits, byte * q, uint cw)
{
#define cb(n) ((byte)(cw >> (n * 8)))
    if (reverse_bits) {
#if hc_bits_size > 16
	q[-3] = byte_reverse_bits[cb(3)];
	q[-2] = byte_reverse_bits[cb(2)];
#endif
	q[-1] = byte_reverse_bits[cb(1)];
	q[0] = byte_reverse_bits[cb(0)];
    } else {
#if hc_bits_size > 16
	q[-3] = cb(3);
	q[-2] = cb(2);
#endif
	q[-1] = cb(1);
	q[0] = cb(0);
    }
#undef cb
}

/* Put out any final bytes. */
/* Note that this does a store_state, but not a load_state. */
byte *
hc_put_last_bits_proc(stream_hc_state * ss, byte * q, uint bits, int bits_left)
{
    while (bits_left < hc_bits_size) {
	byte c = (byte) (bits >> (hc_bits_size - 8));

	if (ss->FirstBitLowOrder)
	    c = byte_reverse_bits[c];
	*++q = c;
	bits <<= 8;
	bits_left += 8;
    }
    ss->bits = bits;
    ss->bits_left = bits_left;
    return q;
}
