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
/* $Id: gdevfax.h 8022 2007-06-05 22:23:38Z giles $ */
/* Definitions and interface for fax devices */

#ifndef gdevfax_INCLUDED
#  define gdevfax_INCLUDED

/* Define the default device parameters. */
#define X_DPI 204
#define Y_DPI 196

/* Define the structure for fax devices. */
/* Precede this by gx_device_common and gx_prn_device_common. */
#define gx_fax_device_common\
    int AdjustWidth		/* 0 = no adjust, 1 = adjust to fax values */
typedef struct gx_device_fax_s {
    gx_device_common;
    gx_prn_device_common;
    gx_fax_device_common;
} gx_device_fax;

#define FAX_DEVICE_BODY(dtype, procs, dname, print_page)\
    prn_device_std_body(dtype, procs, dname,\
			DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,\
			X_DPI, Y_DPI,\
			0, 0, 0, 0,	/* margins */\
			1, print_page),\
    1				/* AdjustWidth */

/* Procedures defined in gdevfax.c */

/* Driver procedures */
dev_proc_open_device(gdev_fax_open);
dev_proc_get_params(gdev_fax_get_params); /* adds AdjustWidth */
dev_proc_put_params(gdev_fax_put_params); /* adds AdjustWidth */
extern const gx_device_procs gdev_fax_std_procs;

/* Other procedures */
void gdev_fax_init_state(stream_CFE_state *ss, const gx_device_fax *fdev);
void gdev_fax_init_fax_state(stream_CFE_state *ss,
			     const gx_device_fax *fdev);
int gdev_fax_print_strip(gx_device_printer * pdev, FILE * prn_stream,
			 const stream_template * temp, stream_state * ss,
			 int width, int row_first,
			 int row_end /* last + 1 */);
int gdev_fax_print_page(gx_device_printer *pdev, FILE *prn_stream,
			stream_CFE_state *ss);

#endif /* gdevfax_INCLUDED */
