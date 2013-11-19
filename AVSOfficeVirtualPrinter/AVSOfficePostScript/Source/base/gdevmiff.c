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
/* $Id: gdevmiff.c 8250 2007-09-25 13:31:24Z giles $ */
/* MIFF file format driver */
#include "gdevprn.h"

/* ------ The device descriptor ------ */

/*
 * Default X and Y resolution.
 */
#define X_DPI 72
#define Y_DPI 72

static dev_proc_print_page(miff24_print_page);

static const gx_device_procs miff24_procs =
prn_color_procs(gdev_prn_open, gdev_prn_output_page, gdev_prn_close,
		gx_default_rgb_map_rgb_color, gx_default_rgb_map_color_rgb);
const gx_device_printer gs_miff24_device =
prn_device(miff24_procs, "miff24",
	   DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	   X_DPI, Y_DPI,
	   0, 0, 0, 0,		/* margins */
	   24, miff24_print_page);

/* Print one page in 24-bit RLE direct color format. */
static int
miff24_print_page(gx_device_printer * pdev, FILE * file)
{
    int raster = gx_device_raster((gx_device *) pdev, true);
    byte *line = gs_alloc_bytes(pdev->memory, raster, "miff line buffer");
    int y;
    int code = 0;		/* return code */

    if (line == 0)		/* can't allocate line buffer */
	return_error(gs_error_VMerror);
    fputs("id=ImageMagick\n", file);
    fputs("class=DirectClass\n", file);
    fprintf(file, "columns=%d\n", pdev->width);
    fputs("compression=RunlengthEncoded\n", file);
    fprintf(file, "rows=%d\n", pdev->height);
    fputs(":\n", file);
    for (y = 0; y < pdev->height; ++y) {
	byte *row;
	byte *end;

	code = gdev_prn_get_bits(pdev, y, line, &row);
	if (code < 0)
	    break;
	end = row + pdev->width * 3;
	while (row < end) {
	    int count = 0;

	    while (count < 255 && row < end - 3 &&
		   row[0] == row[3] && row[1] == row[4] &&
		   row[2] == row[5]
		)
		++count, row += 3;
	    putc(row[0], file);
	    putc(row[1], file);
	    putc(row[2], file);
	    putc(count, file);
	    row += 3;
	}
    }
    gs_free_object(pdev->memory, line, "miff line buffer");

    return code;
}
