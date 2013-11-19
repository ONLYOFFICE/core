/*
  Copyright (C)1998 Ivan Schreter

  This file is part of GNU Ghostscript.
  
  GNU Ghostscript is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY.  No author or distributor accepts responsibility to
  anyone for the consequences of using it or for whether it serves any
  particular purpose or works at all, unless he says so in writing.  Refer
  to the GNU General Public License for full details.

  This source is partially based on deskjet device driver for Ghostscript.
*/

/* gdevop4w.c */
/* OkiPage 4w/4w+ LED printer driver for Ghostscript */

#include "gdevprn.h"
#include "gdevpcl.h"	/* for mode 2 compression */

/*
 * Thanks for various improvements to:
 *	(none so far)
 */

/*
 * TODO: You may select a default resolution of 150, 300 or 600 DPI
 * TODO: in the makefile, or an actual resolution on the gs command line.
 *
 * If the preprocessor symbol A4 is defined, the default paper size is
 * the European A4 size; otherwise it is the U.S. letter size (8.5"x11").
 *
 * You may find the following test page useful in determining the exact
 * margin settings on your printer.  It prints four big arrows which
 * point exactly to the for corners of an A4 sized paper. Of course the
 * arrows cannot appear in full on the paper, and they are truncated by
 * the margins. The margins measured on the testpage must match those
 * in gdevdjet.c.  So the testpage indicates two facts: 1) the page is
 * not printed in the right position 2) the page is truncated too much
 * because the margins are wrong. Setting wrong margins in gdevop4w.c
 * will also move the page, so both facts should be matched with the
 * real world.

%!
	newpath 
	0 0 moveto 144 72 lineto 72 144 lineto
	closepath fill stroke 0 0 moveto 144 144 lineto stroke

	595.27 841.88 moveto 451.27 769.88 lineto 523.27 697.88 lineto
	closepath fill stroke 595.27 841.88 moveto 451.27 697.88 lineto stroke

	0 841.88 moveto 144 769.88 lineto 72 697.88 lineto
	closepath fill stroke 0 841.88 moveto 144 697.88 lineto stroke

	595.27 0 moveto 451.27 72 lineto 523.27 144 lineto
	closepath fill stroke 595.27 0 moveto 451.27 144 lineto stroke

	/Helvetica findfont
	14 scalefont setfont
	100 600 moveto
	(This is an A4 testpage. The arrows should point exactly to the) show
	100 580 moveto
	(corners and the margins should match those given in gdev*.c) show
	showpage

 */

/* Define the default, maximum resolutions. */
#ifndef X_DPI
#  define X_DPI 300
#endif
#ifndef Y_DPI
#  define Y_DPI 300
#endif

/* Margins are left, bottom, right, top. */
/* from Frans van Hoesel hoesel@rugr86.rug.nl. */
/* A4 has a left margin of 1/8 inch and at a printing width of
 * 8 inch this give a right margin of 0.143. The 0.09 top margin is
 * not the actual margin - which is 0.07 - but compensates for the
 * inexact paperlength which is set to 117 10ths.
 * Somebody should check for letter sized paper. I left it at 0.07".
 */
#define OKI4W_MARGINS_LETTER	0.125, 0.25, 0.125, 0.07
#define OKI4W_MARGINS_A4	0.125, 0.25, 0.125, 0.07

/* We round up the LINE_SIZE to a multiple of a ulong for faster scanning. */
#define word ulong
#define W sizeof(word)
#define byte unsigned char

/* The device descriptors */
static dev_proc_open_device(oki4w_open);
static dev_proc_close_device(oki4w_close);
static dev_proc_print_page(oki4w_print_page);

static gx_device_procs prn_hp_procs =
  prn_params_procs(oki4w_open, gdev_prn_output_page, oki4w_close,
		   gdev_prn_get_params, gdev_prn_put_params);

gx_device_printer far_data gs_oki4w_device =
  prn_device(prn_hp_procs, "oki4w",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	X_DPI, Y_DPI,
	0, 0, 0, 0,		/* margins filled in by oki4w_open */
	1, oki4w_print_page);

#define ppdev ((gx_device_printer *)pdev)

/* Find out paper size code */
static int
oki_paper_size(gx_device *dev)
{
	float height_inches = dev->height / dev->y_pixels_per_inch;
	return
		height_inches >= 15.9 ? 0x1b/*PAPER_SIZE_A3*/ :
		height_inches >= 11.8 ? 0x03/*PAPER_SIZE_LEGAL*/ :
		height_inches >= 11.1 ? 0x1a /*PAPER_SIZE_A4*/ :
		height_inches >= 8.3 ? 0x02 /*PAPER_SIZE_LETTER*/ :
			0x19 /*PAPER_SIZE_A5*/;
}

/* Open the printer, adjusting the margins if necessary. */
static int
oki4w_open(gx_device *pdev)
{	/* Change the margins if necessary. */
	const float *m = 0;
	static const float m_a4[4] = { OKI4W_MARGINS_A4 };
	static const float m_letter[4] = { OKI4W_MARGINS_LETTER };
	m = (oki_paper_size(pdev) == 0x1a /*PAPER_SIZE_A4*/ ? m_a4 : m_letter);
	if ( m != 0 )
	  gx_device_set_margins(pdev, m, true);
	return gdev_prn_open(pdev);
}

/* oki4w_close is only here to eject odd numbered pages in duplex mode. */
static int
oki4w_close(gx_device *pdev)
{
/*
	if ( pdev->Duplex_set >= 0 && pdev->Duplex )
	  {	gdev_prn_open_printer(pdev, 1);
		fputs("\033$B\033\177", ppdev->file);
		fputc(0, ppdev->file);
		return gdev_prn_close_printer(pdev);
	  }
*/
	return 0;
}

#undef ppdev

/* ------ Internal routines ------ */

static int
oki_compress(byte *src, byte *dst, int count)
{
	int dcnt = 0;
	byte lastval = *src;
	int run = 1;
	src++;
	count--;
	while (count-- > 0) {
		byte newval = *src++;
		if (newval == lastval) {
			run++;
		} else {
			/* end of run, flush data */
			if (run == 1) {
				byte *backptr = dst++;
				*dst++ = lastval;
				dcnt++;
				lastval = newval;
				while (run < 128 && count > 0) {
					run++;
					newval = *src++;
					*dst++ = newval;
					dcnt++;
					count--;
					if (newval == lastval) {
						break;
					}
				}
				if (newval == lastval) {
					run--;
					dst--;
					dcnt--;
				}
				*backptr = dst - backptr - 2;
				if (newval == lastval) {
					run = 2;
				} else {
					run = 1;
				}
				continue;
			}
			while (run > 128) {
				*dst++ = 0x81;
				*dst++ = lastval;
				run -= 128;
				dcnt += 2;
			}
			if (run > 0) {
				*dst++ = (0x101 - run) & 0xff;
				*dst++ = lastval;
				dcnt += 2;
			}
			lastval = newval;
			run = 1;
		}
	}
	/* end of run, flush data */
	while (run > 128) {
		*dst++ = 0x81;
		*dst++ = lastval;
		run -= 128;
		dcnt += 2;
	}
	if (run > 0) {
		*dst++ = (0x101 - run) & 0xff;
		*dst++ = lastval;
		dcnt += 2;
	}

	return dcnt;
}

/* Send the page to the printer.  For speed, compress each scan line, */
/* since computer-to-printer communication time is often a bottleneck. */
static int
oki4w_print_page(gx_device_printer *pdev, FILE *prn_stream)
{
	int line_size = gdev_mem_bytes_per_scan_line((gx_device *)pdev);
	int line_size_words = (line_size + W - 1) / W;
	uint storage_size_words = line_size_words * 8; /* data, out_row, out_row_alt, prev_row */
	word *storage = (ulong *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), storage_size_words, W,
					   "oki4w_print_page");
	word
	  *data_words,
	  *out_row_words,
	  *out_row_alt_words,
	  *prev_row_words;
#define data ((byte *)data_words)
#define out_row ((byte *)out_row_words)
#define out_row_alt ((byte *)out_row_alt_words)
#define prev_row ((byte *)prev_row_words)
	byte *out_data;
	int x_dpi = pdev->x_pixels_per_inch;
	int y_dpi = pdev->y_pixels_per_inch;
	int y_dots_per_pixel = x_dpi / y_dpi;
	int dpi_code, compress_code;
	int num_rows = dev_print_scan_lines(pdev);

	int out_count;
	int paper_size = oki_paper_size((gx_device *)pdev);
	int code = 0;
	/* bool dup = pdev->Duplex;
	bool dupset = pdev->Duplex_set >= 0; */

	if ( storage == 0 )	/* can't allocate working area */
		return_error(gs_error_VMerror);
	data_words = storage;
	out_row_words = data_words + (line_size_words * 2);
	out_row_alt_words = out_row_words + (line_size_words * 2);
	prev_row_words = out_row_alt_words + (line_size_words * 2);
	/* Clear temp storage */
	memset(data, 0, storage_size_words * W);

	out_data = out_row;

	if (y_dpi == 150) {
		dpi_code = 3;
		compress_code = 2;
	} else if (y_dpi == 300) {
		dpi_code = 5;
		compress_code = 2;
	} else {
		dpi_code = 7;
		compress_code = 2;
	}

	/* Initialize printer. */
/*	if ( pdev->PageCount == 0 ) { */
		/* Put out init string before page. */
		fprintf(prn_stream, "\x1B%%-98765X\x1C\x14\x03\x41i\x10\x1C"
			"\x14\x05\x41\x65%cf%c\x1C\x14\x09\x42\x61%cb\x02\x63"
			"\x01\x65%c\x1C\x7F\x39\x1B&B\x1B&A\x07%c\x01%c"
			"\x01%c%c%c%c\x1B$A",
			dpi_code, dpi_code, 0, 0, 0, paper_size,
			0, dpi_code, dpi_code, 0);
/*	} */

	/* Send each scan line in turn */
	   {	int lnum;
		int num_blank_lines = 0;
		word rmask = ~(word)0 << (-pdev->width & (W * 8 - 1));

		/* Transfer raster graphics. */
		for ( lnum = 0; lnum < num_rows; lnum++ )
		   {	register word *end_data =
				data_words + line_size_words;
			int i;
			code = gdev_prn_copy_scan_lines(pdev, lnum,
						 (byte *)data, line_size);
			if ( code < 0 )
				break;
		   	/* Mask off 1-bits beyond the line width. */
			end_data[-1] &= rmask;
			/* Remove trailing 0s. */
			while ( end_data > data_words && end_data[-1] == 0 )
			  end_data--;
			if ( end_data == data_words )
			   {	/* Blank line */
				num_blank_lines++;
				continue;
			   }

			/* We've reached a non-blank line. */
			/* Put out a spacing command if necessary. */
			if ( num_blank_lines == lnum )
			{	/* We're at the top of a page. */
				/* TODO: skip top_margin lines... */
				/* num_blank_lines += xxx */
				/* Skip blank lines if any */
				if (num_blank_lines > 0) {
					fprintf(prn_stream, "\x1b*B%c%c",
						num_blank_lines & 0xff,
						num_blank_lines >> 8);
				}
			}
			else if ( num_blank_lines != 0 )
			{
				/* Skip blank lines if any */
				fprintf(prn_stream, "\x1b*B%c%c",
					num_blank_lines & 0xff,
					num_blank_lines >> 8);
			}
			num_blank_lines = 0;

			/* Compress the data */
			if (compress_code == 6) {
				out_count = oki_compress(data, out_data,
					(end_data - data_words) * W);
			} else {
				out_count = gdev_pcl_mode2compress(data_words,
					end_data, out_data);
			}

			/* Transfer the data */
			for (i = 0; i < y_dots_per_pixel; ++i) {
				fprintf(prn_stream, "\033*A%c%c%c",
					compress_code,
					out_count & 0xff, out_count >> 8);
				fwrite(out_data, sizeof(byte), out_count,
					prn_stream);
			}
		   }
	}

	/* end raster graphics and eject page */
	fprintf(prn_stream, "\x1b$B\x1b\x7f%c", 0);
		
	/* free temporary storage */
	gs_free(gs_lib_ctx_get_non_gc_memory_t(), (char *)storage, storage_size_words, W, "oki4w_print_page");

	return code;
}
