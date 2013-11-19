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

/* $Id: sjpx_luratech.h 8816 2008-07-03 18:25:52Z giles $ */
/* Definitions for JPXDecode filter (JPEG 2000) */
/* we link to the Luratech CSDK for the actual decoding */

#ifndef sjpx_luratech_INCLUDED
#  define sjpx_luratech_INCLUDED

/* Requires scommon.h; strimpl.h if any templates are referenced */

#include "scommon.h"
#include <lwf_jp2.h>

/* define colorspace enumeration for the decompressed image data */
typedef enum {
  gs_jpx_cs_unset,  /* colorspace hasn't been set */
  gs_jpx_cs_gray,   /* single component grayscale image */
  gs_jpx_cs_rgb,    /* three component (s)RGB image */
  gs_jpx_cs_cmyk,   /* four component CMYK image */
  gs_jpx_cs_indexed /* PDF image wants raw index values */
} gs_jpx_cs;

/* Stream state for the Luratech jp2 codec
 * We rely on our finalization call to free the
 * associated handle and pointers.
 */
typedef struct stream_jpxd_state_s
{
    stream_state_common;	/* a define from scommon.h */
    const gs_memory_t *jpx_memory;
    JP2_Decomp_Handle handle;	/* library decoder handle */
    unsigned char *inbuf;	/* input data buffer */
    unsigned long inbuf_size;
    unsigned long inbuf_fill;
    gs_jpx_cs colorspace;	/* requested output colorspace */
    int ncomp;			/* number of image components */
    int bpc;			/* sample bits per component */
    unsigned long width, height;
    unsigned long stride;
    unsigned char *image;	/* decoded image buffer */
    long offset; /* offset into the image buffer of the next
                    byte to be returned */
}
stream_jpxd_state;

#define private_st_jpxd_state()	\
  gs_private_st_simple(st_jpxd_state, stream_jpxd_state,\
    "JPXDecode filter state")
extern const stream_template s_jpxd_template;

/* JPX encoder internal state */
typedef struct stream_jpxe_state_s {
    stream_state_common;	/* inherit base object from scommon.h */
    JP2_Comp_Handle handle;	/* compression library context */

    /* the following members must be set by the caller: */
    unsigned long width, height;/* image dimensions */
    gs_jpx_cs colorspace;	/* colorspace of the input image data */
    unsigned int bpc;		/* sample depth of each channel in bits */

    /* the following members can be optionally set by the caller: */
    unsigned int quality;       /* compressed image quality target; 1-100
					leave unset for the default */
    int lossless;		/* set to 1 to specify lossless image
					compression; overrides quality */

    /* the remainder are handled internally: */
    unsigned int components;	/* number of image channels */
    unsigned long stride;	/* line length in bytes */
    unsigned char *inbuf;	/* input image data buffer */
    unsigned long insize;	/* allocated size of buffer */
    unsigned long infill;	/* bytes written to the buffer */

    unsigned char *outbuf;	/* output data buffer */
    unsigned long outsize;	/* size of the data buffer in bytes */
    unsigned long outfill;	/* bytes written to the buffer */
    unsigned long offset;	/* bytes written from the buffer */
} stream_jpxe_state;

#define private_st_jpxe_state() \
  gs_private_st_simple(st_jpxe_state, stream_jpxe_state, \
    "jpx encode filter state")

extern const stream_template s_jpxe_template;

#endif /* sjpx_luratech_INCLUDED */
