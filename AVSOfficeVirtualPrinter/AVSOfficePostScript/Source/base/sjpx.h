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

/* $Id: sjpx.h 8818 2008-07-03 18:25:55Z giles $ */
/* Definitions for JPXDecode filter (JPEG 2000) */
/* we link to the JasPer library for the actual decoding */

#ifndef sjpx_INCLUDED
#  define sjpx_INCLUDED

/* Requires scommon.h; strimpl.h if any templates are referenced */

#include "scommon.h"
#include <jasper/jasper.h>

/* define colorspace enumeration for the input image data */
typedef enum {
  gs_jpx_cs_unset,  /* colorspace hasn't been set */
  gs_jpx_cs_gray,   /* single component grayscale image */
  gs_jpx_cs_rgb,    /* three component (s)RGB image */
  gs_jpx_cs_cmyk,   /* four component CMYK image */
  gs_jpx_cs_indexed /* PDF image wants raw index values */
} gs_jpx_cs;

/* Our local state consists of pointers to the JasPer library's
 * stream and image structs for sending and retrieving the
 * image data. There's no way to feed a jasper stream with
 * incremental buffers, so we also must spool the entire
 * compressed stream into our own buffer before handing it
 * to the library. We also keep track of how much of the
 * decoded image we have returned.
 */
typedef struct stream_jpxd_state_s
{
    stream_state_common;	/* a define from scommon.h */
    jas_image_t *image;
    long offset; /* offset into the image bitmap of the next
                    byte to be returned */
    const gs_memory_t *jpx_memory;
    unsigned char *buffer; /* temporary buffer for compressed data */
    long bufsize; /* total size of the buffer */
    long buffill; /* number of bytes written into the buffer */
    gs_jpx_cs colorspace; /* external colorspace setting */
}
stream_jpxd_state;

#define private_st_jpxd_state()	\
  gs_private_st_simple(st_jpxd_state, stream_jpxd_state,\
    "JPXDecode filter state")
extern const stream_template s_jpxd_template;

#endif /* sjpx_INCLUDED */
