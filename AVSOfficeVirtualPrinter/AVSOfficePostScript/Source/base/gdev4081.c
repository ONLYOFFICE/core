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
/* $Id: gdev4081.c 8250 2007-09-25 13:31:24Z giles $*/
/* Ricoh 4081 laser printer driver */
#include "gdevprn.h"

#define X_DPI 300			/* pixels per inch */
#define Y_DPI 300			/* pixels per inch */

/* The device descriptor */
static dev_proc_print_page(r4081_print_page);
const gx_device_printer far_data gs_r4081_device =
  prn_device(prn_std_procs, "r4081",
	85,				/* width_10ths, 8.5" */
	110,				/* height_10ths, 11" */
	X_DPI, Y_DPI,
	0.25, 0.16, 0.25, 0.16,		/* margins */
	1, r4081_print_page);

/* ------ Internal routines ------ */


/* Send the page to the printer. */
static int
r4081_print_page(gx_device_printer *pdev, FILE *prn_stream)
{	
	int line_size = gdev_mem_bytes_per_scan_line((gx_device *)pdev);
	int out_size = ((pdev->width + 7) & -8) ;
	byte *out = (byte *)gs_malloc(pdev->memory, out_size, 1, "r4081_print_page(out)");
	int lnum = 0;
	int last = pdev->height;

	/* Check allocations */
	if ( out == 0 )
	{	if ( out )
			gs_free(pdev->memory, (char *)out, out_size, 1,
				"r4081_print_page(out)");
		return -1;
	}

	/* find the first line which has something to print */
	while ( lnum < last )
	{
		gdev_prn_copy_scan_lines(pdev, lnum, (byte *)out, line_size);
		if ( out[0] != 0 ||
		     memcmp((char *)out, (char *)out+1, line_size-1)
		   )
			break;
		lnum ++;
	}

	/* find the last line which has something to print */
	while (last > lnum) {
		gdev_prn_copy_scan_lines(pdev, last-1, (byte *)out, line_size);
		if ( out[0] != 0 ||
		     memcmp((char *)out, (char *)out+1, line_size-1)
		   )
			break;
		last --;
	}

	/* Initialize the printer and set the starting position. */
	fprintf(prn_stream,"\033\rP\033\022YB2 \033\022G3,%d,%d,1,1,1,%d@",
			out_size, last-lnum, (lnum+1)*720/Y_DPI);

	/* Print lines of graphics */
	while ( lnum < last )
	   {
		gdev_prn_copy_scan_lines(pdev, lnum, (byte *)out, line_size);
		fwrite(out, sizeof(char), line_size, prn_stream);
		lnum ++;
	   }

	/* Eject the page and reinitialize the printer */
	fputs("\f\033\rP", prn_stream);

	gs_free(pdev->memory, (char *)out, out_size, 1, "r4081_print_page(out)");
	return 0;
}
