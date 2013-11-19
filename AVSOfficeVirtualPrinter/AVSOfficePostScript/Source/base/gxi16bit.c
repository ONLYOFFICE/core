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

/* $Id: gxi16bit.c 8250 2007-09-25 13:31:24Z giles $ */
/* 16-bit image procedures */
#include "gx.h"
#include "memory_.h"
#include "gpcheck.h"
#include "gserrors.h"
#include "gxfixed.h"
#include "gxfrac.h"
#include "gxarith.h"
#include "gxmatrix.h"
#include "gsccolor.h"
#include "gspaint.h"
#include "gxdevice.h"
#include "gxcmap.h"
#include "gxdcolor.h"
#include "gxistate.h"
#include "gxdevmem.h"
#include "gxcpath.h"
#include "gximage.h"

/* ---------------- Unpacking procedures ---------------- */

static const byte *
sample_unpack_16(byte * bptr, int *pdata_x, const byte * data,
		 int data_x, uint dsize, const sample_map *ignore_smap, int spread,
		 int ignore_num_components_per_plane)
{
    /* Assuming an identity map for all components. */
    register frac *bufp = (frac *) bptr;
    uint dskip = data_x << 1;
    const byte *psrc = data + dskip;
#define inc_bufp(bp, n) bp = (frac *)((byte *)(bp) + (n))
    uint sample;
    int left = dsize - dskip;

    while (left > 2) {
	sample = ((uint) psrc[0] << 8) + psrc[1];
	*bufp = (frac)((frac_1 * (sample + 1)) >> 16);
	inc_bufp(bufp, spread);
	psrc += 2;
	left -= 2;
    }
    *pdata_x = 0;
    return bptr;
}

const sample_unpack_proc_t sample_unpack_16_proc = sample_unpack_16;

/* ---------------- Rendering procedures ---------------- */

/* After unpacking, 16-bit amples have the same format as 12-bit ones */
/* and can be rendered by the procedures from gxi12bit.c              */
