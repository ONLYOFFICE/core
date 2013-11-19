/*
   Copyright (C) 2000 <Rene Harsch, Switzerland> 
   written by <Rene Harsch rene@harsch.net>
	 modification by Mark Burton <markb@ordern.com>

   Portions Copyright (C) 1999 Aladdin Enterprises.  All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 U.S.A.

   This program may also be distributed as part of Aladdin Ghostscript,
   under the terms of the Aladdin Free Public License (the "License").

   Every copy of Aladdin Ghostscript must include a copy of the
   License, normally in a plain ASCII text file named PUBLIC.  The
   License grants you the right to copy, modify and redistribute
   Aladdin Ghostscript, but only under certain conditions described in
   the License.  Among other things, the License requires that the
   copyright notice and this notice be preserved on all copies.
 */

/*$Id: gdevdj9.c,v 1.4 2002/09/03 02:23:05 easysw Exp $*/

/*
   A printer driver for the HP970Cxi color printer. 

   To be used with the Ghostscript printing system.
   Please report all problems to rene@harsch.net

   CREDITS: Much of the driver is based on ideas derived
            from the cdj550 driver of George Cameron
            and cdj850 of Uli Wortmann

	
	2000-04-18 Version 1.0

	The driver implements the following switches:
	Please note this switches are all copied from Uli Wortmann driver 
	for hp850 (gdevcd8.c).

  -dPapertype= 0  plain paper [default]
   1  bond paper
   2  special paper
   3  glossy film
   4  transparency film

   Note, currently the lookuptables are not suited
   for printing on special paper or transperencies.
   Please revert to the gamma functions in this case.

   -dQuality=  -1 draft     
   0 normal       [default]
   1 presentation 

   -dRetStatus= 0 C-RET off
   1 C-RET on [default]

   -dMasterGamma= 3.0 [default = 1.0]
   __Note__: To take advantage of the calibrated color-transfer
   functions, be sure not to have any Gamma-Statements
   left! If you need to (i.e. overhead sheets), 
   you still can use the gamma-functions, but they will 
   override the built-in calibration. To use gamma in the 
   traditional way, set MasterGamma to any value greater
   1.0 and less 10.0. To adjust individual gamma-values,
   you have to additionally set MasterGamma to a value
   greater 1.0 and less 10.0

		new switches:
		-dDuplex= 0 [default] None Duplex Mode
		1 Duplex Print Block Mode
		2 Duplex Print Book Mode


		known bug: Black planes in duplex mode are still "clumsy"

		for further information please look on my homepage @
		http://www.harsch.net

	  Have Fun.

    Rene

		2001-12-24: Version 1.1
		Changes made by Mark Burton <markb@ordern.com>
		(many thanks :-)

   	I have modified gdevdj9.c so that both the Quality and
    the Duplex parameter can be set using setpagedevice. 

    One other minor fix was to the initialisation values for
    gs_cdj970_device which seemed to use PRESENTATION rather than NORMAL.
*/

#include "math_.h"
#include "string.h"
#include <stdlib.h>		/* for rand() */
#include <assert.h>
#include "gdevprn.h"
#include "gdevpcl.h"
#include "gsparam.h"

/* Conversion stuff. */
#include "gxlum.h"

/* this holds the initialisation data of the hp970 */
typedef struct hp970_cmyk_init_s {
    byte a[26];
} hp970_cmyk_init_t;
static const hp970_cmyk_init_t hp970_cmyk_init =
{
    {
	0x02,			/* format */
	0x04,			/* number of components */
      /* black */
	0x01,			/* MSB x resolution */
	0x2c,			/* LSB x resolution */
	0x01,			/* MSB y resolution */
	0x2c,			/* LSB y resolution */
	0x00,			/* MSB intensity levels */
	0x02,			/* LSB intensity levels */

      /* cyan */
	0x01,			/* MSB x resolution */
	0x2c,			/* LSB x resolution */
	0x01,			/* MSB y resolution */
	0x2c,			/* LSB y resolution */
	0x00,			/* MSB intensity levels */
	0x02,			/* LSB intensity levels */

      /* magenta */
	0x01,			/* MSB x resolution */
	0x2c,			/* LSB x resolution */
	0x01,			/* MSB y resolution */
	0x2c,			/* LSB y resolution */
	0x00,			/* MSB intensity levels */
	0x02,			/* LSB intensity levels */

      /* yellow */
	0x01,			/* MSB x resolution */
	0x2c,			/* LSB x resolution */
	0x01,			/* MSB y resolution */
	0x2c,			/* LSB y resolution */
	0x00,			/* MSB intensity levels */
	0x02			/* LSB intensity levels */
    }
};

/* this holds the color lookuptable data of the hp970 */
typedef struct {
    byte c[256];		/* Lookuptable for cyan */
    byte m[256];		/* dito for magenta */
    byte y[256];		/* dito for yellow */
    byte k[256];		/* dito for black  */
    int correct[256];		/* potential undercolor black correction */
} Gamma;

static const Gamma gammat970 =
{
  /* Lookup values for cyan */
    {0, 0, 0, 2, 2, 2, 3, 3, 3, 5, 5, 5, 7, 7, 6, 7, 7, 6, 7, 7, 7, 8, 8,
     8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 11, 11, 12, 12, 12,
     12, 12, 12, 13, 13, 14, 14, 14, 15, 15, 16, 16, 15, 16, 16, 17, 17,
     17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 20, 20, 21, 21, 21, 22,
     22, 23, 23, 23, 23, 23, 24, 24, 25, 25, 26, 26, 26, 26, 26, 27, 27,
     27, 27, 28, 28, 29, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 34,
     35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 42,
     43, 43, 43, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 50, 50, 51, 51,
     52, 52, 53, 54, 54, 54, 55, 55, 56, 57, 58, 58, 59, 60, 60, 61, 62,
     62, 63, 65, 65, 66, 67, 67, 68, 69, 69, 70, 72, 73, 73, 74, 75, 75,
     76, 77, 79, 79, 80, 81, 82, 83, 83, 84, 86, 87, 88, 88, 89, 90, 91,
     92, 93, 94, 95, 96, 97, 97, 99, 100, 101, 102, 103, 104, 105, 106,
     108, 109, 110, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125, 127,
     129, 131, 132, 135, 136, 138, 140, 142, 144, 146, 147, 150, 152, 154,
     157, 159, 162, 164, 166, 168, 171, 174, 176, 180, 182, 187, 192, 197,
     204, 215, 255},
  /* Lookup values for magenta */
    {0, 0, 0, 1, 1, 1, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 7,
     7, 8, 8, 8, 9, 9, 10, 10, 9, 10, 10, 10, 11, 11, 11, 11, 11, 12, 12,
     12, 13, 13, 13, 14, 14, 15, 15, 15, 16, 16, 16, 16, 16, 17, 17, 17,
     17, 17, 18, 18, 19, 19, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22,
     23, 23, 24, 24, 25, 25, 25, 26, 26, 27, 27, 28, 29, 29, 29, 29, 30,
     30, 31, 30, 31, 31, 32, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36,
     36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44,
     45, 45, 46, 46, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 53, 53, 54,
     54, 55, 55, 56, 57, 57, 58, 58, 59, 60, 60, 61, 61, 62, 63, 64, 65,
     66, 66, 67, 68, 68, 70, 71, 71, 72, 73, 73, 74, 76, 77, 77, 78, 79,
     79, 80, 81, 82, 83, 84, 85, 86, 87, 87, 88, 89, 90, 91, 91, 92, 93,
     94, 95, 96, 97, 98, 99, 100, 100, 101, 102, 103, 105, 106, 107, 108,
     109, 112, 113, 114, 115, 116, 118, 119, 121, 123, 124, 125, 128, 129,
     130, 133, 134, 135, 138, 139, 142, 144, 145, 148, 150, 152, 154, 157,
     159, 162, 164, 168, 169, 170, 172, 175, 177, 179, 182, 185, 189, 193,
     198, 204, 215, 255},
  /* Lookup values for yellow */
    {0, 0, 0, 2, 2, 2, 3, 3, 3, 5, 5, 5, 7, 7, 6, 7, 7, 6, 7, 7, 7, 8, 8,
     8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11,
     12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 16, 16, 16, 17, 17, 18, 18,
     18, 19, 18, 19, 19, 19, 20, 20, 21, 21, 21, 22, 22, 22, 22, 22, 23,
     23, 24, 24, 25, 25, 25, 26, 27, 28, 28, 29, 29, 29, 30, 30, 30, 30,
     31, 31, 32, 32, 33, 33, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37,
     38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 45,
     45, 46, 46, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 53, 53, 54, 54,
     55, 55, 56, 57, 58, 59, 59, 60, 61, 61, 62, 62, 63, 64, 65, 66, 67,
     67, 68, 69, 69, 70, 71, 72, 73, 74, 74, 75, 76, 77, 77, 78, 79, 79,
     80, 81, 82, 83, 84, 85, 86, 87, 87, 88, 89, 90, 91, 91, 93, 94, 95,
     96, 97, 98, 100, 101, 102, 102, 103, 104, 106, 107, 108, 109, 110,
     111, 113, 114, 115, 116, 117, 118, 119, 121, 123, 124, 126, 128, 130,
     131, 134, 135, 137, 139, 140, 143, 145, 146, 148, 150, 152, 154, 156,
     158, 160, 163, 166, 167, 169, 171, 173, 176, 178, 181, 184, 188, 192,
     198, 204, 215, 255},
  /* Lookup values for black */
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 2, 4, 3, 3, 3, 3, 3, 4, 4,
     4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 9, 9, 8,
     8, 8, 9, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13,
     12, 12, 12, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 15, 15, 16, 16,
     16, 17, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 20, 20, 21, 21,
     21, 21, 22, 22, 22, 22, 23, 22, 23, 23, 24, 24, 24, 24, 25, 25, 26,
     26, 26, 26, 27, 27, 28, 28, 28, 28, 29, 29, 30, 30, 31, 31, 31, 32,
     32, 33, 33, 34, 34, 35, 36, 36, 36, 37, 37, 37, 38, 38, 40, 40, 40,
     41, 41, 42, 43, 43, 43, 43, 44, 45, 45, 46, 47, 47, 48, 49, 49, 50,
     52, 52, 53, 54, 54, 56, 56, 57, 58, 59, 60, 60, 61, 62, 63, 63, 64,
     65, 66, 67, 68, 69, 70, 71, 72, 72, 73, 75, 75, 76, 77, 78, 80, 81,
     82, 82, 83, 84, 85, 86, 88, 89, 90, 91, 94, 95, 96, 98, 99, 100, 101,
     103, 105, 106, 107, 110, 111, 112, 115, 116, 118, 120, 121, 124, 126,
     127, 131, 133, 134, 138, 140, 141, 146, 148, 151, 154, 156, 160, 163,
     166, 169, 174, 177, 182, 187, 194, 203, 215, 255}
};

static const Gamma * const gammat[] =
{
    &gammat970,			/* CDJ970Cxi */
};

static int
    rescale_byte_wise1x1(int bytecount, const byte * inbytea,
			 const byte * inbyteb, byte * outbyte);
static int
    rescale_byte_wise2x1(int bytecount, const byte * inbytea,
			 const byte * inbyteb, byte * outbyte);
static int
    rescale_byte_wise1x2(int bytecount, const byte * inbytea,
			 const byte * inbyteb, byte * outbyte);
static int
    rescale_byte_wise2x2(int bytecount, const byte * inbytea,
			 const byte * inbyteb, byte * outbyte);

static int (* const rescale_color_plane[2][2]) (int, const byte *, const byte *, byte *) = {
    {
	rescale_byte_wise1x1, rescale_byte_wise1x2
    },
    {
	rescale_byte_wise2x1, rescale_byte_wise2x2
    }
};
/*
static int (* const rescale_color_plane[2][2]) (int, const byte *, const byte *, byte *) = {
    {
	rescale_byte_wise1x1, rescale_byte_wise1x1
    },
    {
	rescale_byte_wise1x1, rescale_byte_wise1x1
    }
};
*/
static int cdj970_write_header(gx_device *, FILE *);
static int cdj970_write_trailer(gx_device *, FILE *);

/*
 * Drivers stuff.
 *
 */

#define DUPLEX_BOTTOM_MARGIN_A4 205
#define DUPLEX_LEFT_MARGIN_A4 120

#define DUPLEX_BOTTOM_MARGIN_LETTER 205   /* ???? */
#define DUPLEX_LEFT_MARGIN_LETTER 120			/* ????  */

#define DESKJET_PRINT_LIMIT  0.04	/* 'real' top margin? */
/* Margins are left, bottom, right, top. */
#define DESKJET_MARGINS_LETTER   0.25, 0.50, 0.25, 0.167
#define DESKJET_MARGINS_A4       0.13, 0.46, 0.13, 0.04
/* Define bits-per-pixel - default is 32-bit cmyk-mode */
#ifndef BITSPERPIXEL
#  define BITSPERPIXEL 32
#endif
#define DOFFSET (dev_t_margin(pdev) - DESKJET_PRINT_LIMIT)	/* Print position */


#define W sizeof(word)
#define I sizeof(int)

/* paper types */
typedef enum {
    PLAIN_PAPER, BOND_PAPER, SPECIAL_PAPER, GLOSSY_FILM, TRANSPARENCY_FILM
} cdj_paper_type_t;

/* quality */
typedef enum {
    DRAFT = -1, NORMAL = 0, PRESENTATION = 1
} cdj_quality_t;

/* duplex mode */
typedef enum {
	NONE, BLOCK, BOOK
} cdj_duplex_t;

/* Printer types */
typedef enum {
	DJ970C
} cdj_printer_type_t;

/* No. of ink jets (used to minimise head movements) */
#define HEAD_ROWS_MONO 50
#define HEAD_ROWS_COLOUR 16

/* Colour mapping procedures */
static dev_proc_map_cmyk_color(gdev_cmyk_map_cmyk_color);
static dev_proc_map_rgb_color(gdev_cmyk_map_rgb_color);
static dev_proc_map_color_rgb(gdev_cmyk_map_color_rgb);

static dev_proc_map_rgb_color(gdev_pcl_map_rgb_color);
static dev_proc_map_color_rgb(gdev_pcl_map_color_rgb);

/* Print-page, parameters and miscellaneous procedures */
static dev_proc_open_device(hp_colour_open);
static dev_proc_close_device(cdj970_close);

static dev_proc_get_params(cdj970_get_params);
static dev_proc_put_params(cdj970_put_params);

static dev_proc_print_page(cdj970_print_page);

/* The device descriptors */

/* The basic structure for all printers. Note the presence of the cmyk, depth
   and correct fields even if some are not used by all printers. */

#define prn_colour_device_body(dtype, procs, dname, w10, h10, xdpi, ydpi, lm, bm, rm, tm, ncomp, depth, mg, mc, dg, dc, print_page, cmyk, correct)\
    prn_device_body(dtype, procs, dname, w10, h10, xdpi, ydpi, lm, bm, rm, tm, ncomp, depth, mg, mc, dg, dc, print_page), cmyk, depth /* default */, correct



#define gx_prn_colour_device_common \
    gx_prn_device_common; \
    int cmyk;	  	/* 0: not CMYK-capable, > 0: printing CMYK, */ \
		  	/* < 0 : CMYK-capable, not printing CMYK */ \
    uint default_depth;	/* Used only for CMYK-capable printers now. */ \
    uint correction


/* some definitions needed later */
struct error_val_field {
    int c;		/* Current value of Cyan error during dithering */
    int m;		/* Current value of Magenta error during dithering */
    int y;		/* Current value of Yellow error during dithering */
    int k;		/* Current value of Black error during dithering */
};

/* this structure holds all the pointers to the different values
   in all those data fields */
 /*
    * The principal data pointers are stored as pairs of values, with
    * the selection being made by the 'scan' variable. The function of the
    * scan variable is overloaded, as it controls both the alternating
    * raster scan direction used in the Floyd-Steinberg dithering and also
    * the buffer alternation required for line-difference compression.
    *
    * Thus, the number of pointers required is as follows:
  */

struct ptr_arrays {
    byte *data[4];		/* 4 600dpi data, scan direction and alternating buffers */
    byte *data_c[4];		/* 4 300dpi data, as above, */
    byte *plane_data[4][4];	/*4 b/w-planes, scan direction and alternating buffers */
    byte *plane_data_c[4][8];	/* as above, but for 8 planes */
    byte *out_data;		/* output buffer for the b/w data, one 600dpi plane */
    byte *test_data[4];		/* holds a copy of the last plane */
    int *errors[2];		/* 2 b/w dithering erros (scan direction only) */
    int *errors_c[2];		/* 2 color dithering errors (scan direction only) */
    word *storage;		/* pointer to the beginning of the b/w-buffer */
    word *storage_start;	/* used for debugging */
    word *storage_end;		/* used for debugging */
    word *storage_size;		/* used for debugging */
};

/* Some miscellaneous variables */
struct misc_struct {
    int line_size;		/* size of scan_line */
    int line_size_c;		/* size of rescaled scan_line */
    int line_size_words;	/* size of scan_line in words */
    int paper_size;		/* size of paper */
    int num_comps;		/* number of color components (1 - 4) */
    int bits_per_pixel;		/* bits per pixel 1,4,8,16,24,32 */
    int storage_bpp;		/* = bits_per_pixel */
    int expanded_bpp;		/* = bits_per_pixel */
    int plane_size;		/* size of b/w bit plane */
    int plane_size_c;		/* size of color bit plane */
    int databuff_size;		/* size of databuffer for b/w data */
    int databuff_size_c;	/* size of databuffer for color data */
    int errbuff_size;		/* size of error buffer b/w -data */
    int errbuff_size_c;		/* size of error buffer color -data */
    int outbuff_size;		/* size of output buffer for b/w data */
    int scan;			/* scan-line variable [0,1] */
    int cscan;			/* dito for the color-planes */
    int is_two_pass;		/* checks if b/w data has already been printed */
    int zero_row_count;		/* How many empty lines */
    uint storage_size_words;	/* size of storage in words for b/w data */
    uint storage_size_words_c;	/* size of storage in words for c-data */
    int is_color_data;		/* indicates whether there is color data */
};

    /* function pointer typedefs for device driver struct */
typedef void (*StartRasterMode) (gx_device_printer * pdev, int paper_size,
				 FILE * prn_stream);
typedef void (*PrintNonBlankLines) (gx_device_printer * pdev,
				    struct ptr_arrays *data_ptrs,
				    struct misc_struct *misc_vars,
				    struct error_val_field *error_values,
				    const Gamma *gamma,
				    FILE * prn_stream);

typedef void (*TerminatePage) (gx_device_printer * pdev, FILE * prn_stream);

typedef struct gx_device_cdj970_s {
    gx_device_common;
    gx_prn_colour_device_common;
    int /*cdj_quality_t*/ quality;  /* -1 draft, 0 normal, 1 best */
    int /*cdj_paper_type_t*/ papertype;  /* papertype [0,4] */
    int /*cdj_duplex_t*/ duplex;
    int intensities;		/* intensity values per pixel [2,4] */
    int xscal;			/* boolean to indicate x scaling by 2 */
    int yscal;			/* boolean to indicate y scaling by 2 */
    int /*cdj_printer_type_t*/ ptype;  /* printer type, one of DJ670C, DJ970C, DJ890C, DJ1600C */
    int compression;		/* compression level */
    float mastergamma;		/* Gammavalue applied to all colors */
    float gammavalc;		/* range to which gamma-correction is
				   applied to bw values */
    float gammavalm;		/* amount of gamma correction for bw */
    float gammavaly;		/* range to which gamma-correction i
				   applied to color values */
    float gammavalk;		/* amount of gamma correction for color */
    float blackcorrect;		/* amount of gamma correction for color */
    StartRasterMode start_raster_mode;	/* output function to start raster mode */
    PrintNonBlankLines print_non_blank_lines;	/* output function to print a non blank line */
    TerminatePage terminate_page;	/* page termination output function */
} gx_device_cdj970;

typedef struct {
    gx_device_common;
    gx_prn_colour_device_common;
} gx_device_colour_prn;


/* Use the cprn_device macro to access generic fields (like cmyk,
   default_depth and correction), and specific macros for specific
   devices. */

#define cprn_device     ((gx_device_colour_prn*) pdev)
#define cdj970    			((gx_device_cdj970 *)pdev)

#define prn_cmyk_colour_device(dtype, procs, dev_name, x_dpi, y_dpi, bpp, print_page, correct)\
    prn_colour_device_body(dtype, procs, dev_name,\
    DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, x_dpi, y_dpi, 0, 0, 0, 0,\
    ((bpp == 1 || bpp == 4) ? 1 : 4), bpp,\
    (bpp > 8 ? 255 : 1), (1 << (bpp >> 2)) - 1, /* max_gray, max_color */\
    (bpp > 8 ? 5 : 2), (bpp > 8 ? 5 : bpp > 1 ? 2 : 0),\
    print_page, 1 /* cmyk */, correct)


#define prn_cmy_colour_device(dtype, procs, dev_name, x_dpi, y_dpi, bpp, print_page, correct)\
    prn_colour_device_body(dtype, procs, dev_name,\
    DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, x_dpi, y_dpi, 0, 0, 0, 0,\
    ((bpp == 1 || bpp == 4) ? 1 : 3), bpp,\
    (bpp > 8 ? 255 : 1), (bpp > 8 ? 255 : 1), /* max_gray, max_color */\
    (bpp > 8 ? 5 : 2), (bpp > 8 ? 5 : bpp > 1 ? 2 : 0),\
    print_page, -1 /* cmyk */, correct)


#define cdj_970_device(procs, dev_name, x_dpi, y_dpi, bpp, print_page, correction, quality, papertype, duplex, intensities,ptype,compression,mastergamma,gammavalc,gammavalm,gammavaly,gammavalk,blackcorrect,start_raster_mode,print_non_blank_line,terminate_page)\
{ prn_cmyk_colour_device(gx_device_cdj970, procs, dev_name, x_dpi, y_dpi, bpp, print_page, correction),\
    quality,\
    papertype,\
    duplex,\
    intensities,\
    0, 0, /* xscal, yscal */\
    ptype,\
    compression,\
    mastergamma,\
    gammavalc,\
    gammavalm,\
    gammavaly,\
    gammavalk,\
    blackcorrect,\
    start_raster_mode,\
    print_non_blank_line,\
    terminate_page\
}

#define cmyk_colour_procs(proc_colour_open, proc_get_params, proc_put_params, \
			  proc_colour_close, map_rgb_color, map_color_rgb, map_cmyk_color) {\
	proc_colour_open,\
	gx_default_get_initial_matrix,\
	gx_default_sync_output,\
	gdev_prn_output_page,\
	proc_colour_close,\
	map_rgb_color,\
	map_color_rgb,\
	NULL /* fill_rectangle */,\
	NULL /* tile_rectangle */,\
	NULL /* copy_mono */,\
	NULL /* copy_color */,\
	NULL /* draw_line */,\
	gx_default_get_bits,\
	proc_get_params,\
	proc_put_params,\
        map_cmyk_color\
}

static void
     cdj970_start_raster_mode(gx_device_printer * pdev,
			      int papersize, FILE * prn_stream);

static void
     cdj970_print_non_blank_lines(gx_device_printer * pdev,
				  struct ptr_arrays *data_ptrs,
				  struct misc_struct *misc_vars,
				  struct error_val_field *error_values,
				  const Gamma *gamma,
				  FILE * prn_stream);
static void
     cdj970_terminate_page(gx_device_printer * pdev, FILE * prn_stream);


static const gx_device_procs cdj970_procs =
cmyk_colour_procs(hp_colour_open, cdj970_get_params, cdj970_put_params,
		  cdj970_close, NULL, gdev_cmyk_map_color_rgb, gdev_cmyk_map_cmyk_color);

const gx_device_cdj970 gs_cdj970_device =
				cdj_970_device(cdj970_procs, "cdj970", 600, 600, 32, cdj970_print_page, 0,
				NORMAL, PLAIN_PAPER, NONE, 4, DJ970C, 2,
				1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
				cdj970_start_raster_mode, cdj970_print_non_blank_lines,
				cdj970_terminate_page);

/* Forward references */
static int cdj_put_param_int(gs_param_list *, gs_param_name,
			      int *, int, int, int);
static int cdj_put_param_float(gs_param_list *, gs_param_name, float
				*, float, float, int);
static int cdj_put_param_bpp(gx_device *, gs_param_list *, int, int, int);
static int cdj_set_bpp(gx_device *, int, int);

static int PageCtr = 0;

/**********************************************************************************/
/*                                                                                */
/*																	private functions                             */
/*                                                                                */
/**********************************************************************************/

/* hp_colour_open:  Open the printer and set up the margins. 
----------------------------------------------------------------------------------*/
static int hp_colour_open(gx_device * pdev) 
{				
	int   retCode;

	/* Set up colour params if put_params has not already done so */
	if (pdev->color_info.num_components == 0) {
		int code = cdj_set_bpp(pdev, pdev->color_info.depth, 
									pdev->color_info.num_components);

		if (code < 0)
			return code;
	}
	
	retCode = gdev_prn_open (pdev);
	if (retCode < 0)
			return (retCode);
	else {
		retCode = gdev_prn_open_printer(pdev, true);
		if (retCode < 0)
			return (retCode);
	}
	
	return 0;
}

/* cdj970_get_params:  Added parameters for DeskJet 970C
----------------------------------------------------------------------------------*/
static int cdj970_get_params(gx_device * pdev, gs_param_list * plist)
{
	int code = gdev_prn_get_params(pdev, plist);

	if (code < 0 ||
		(code = param_write_int(plist, "Quality", &cdj970->quality)) < 0 ||
		(code = param_write_int(plist, "Duplex", &cdj970->duplex)) < 0 ||
		(code = param_write_int(plist, "Papertype", &cdj970->papertype)) < 0 ||
		(code = param_write_float(plist, "MasterGamma", &cdj970->gammavalc))
		< 0 ||
		(code = param_write_float(plist, "GammaValC", &cdj970->gammavalc)) <
		0 ||
		(code = param_write_float(plist, "GammaValM", &cdj970->gammavalm)) <
		0 ||
		(code = param_write_float(plist, "GammaValY", &cdj970->gammavaly)) <
		0 ||
		(code = param_write_float(plist, "GammaValK", &cdj970->gammavalk)) <
		0 ||
		(code = param_write_float(plist, "BlackCorrect",
				  &cdj970->blackcorrect)) < 0)

			return (code);
	return (code);
}

/* cdj970_put_params:  
----------------------------------------------------------------------------------*/
static int cdj970_put_params(gx_device * pdev, gs_param_list * plist)
{
    int quality = cdj970->quality;
    int papertype = cdj970->papertype;
    int duplex = cdj970->duplex;
    float mastergamma = cdj970->mastergamma;
    float gammavalc = cdj970->gammavalc;
    float gammavalm = cdj970->gammavalm;
    float gammavaly = cdj970->gammavaly;
    float gammavalk = cdj970->gammavalk;
    float blackcorrect = cdj970->blackcorrect;
    
    int bpp = 0;
    int code = 0;

    code = cdj_put_param_int(plist, "BitsPerPixel", &bpp, 1, 32, code);
    code = cdj_put_param_int(plist, "Quality", &quality, 0, 2, code);
    code = cdj_put_param_int(plist, "Papertype", &papertype, 0, 4, code);
    code = cdj_put_param_int(plist, "Duplex", &duplex, 0, 2, code);
    code = cdj_put_param_float(plist, "MasterGamma", &mastergamma, 0.1, 9.0, code);
    code = cdj_put_param_float(plist, "GammaValC", &gammavalc, 0.0, 9.0, code);
    code = cdj_put_param_float(plist, "GammaValM", &gammavalm, 0.0, 9.0, code);
    code = cdj_put_param_float(plist, "GammaValY", &gammavaly, 0.0, 9.0, code);
    code = cdj_put_param_float(plist, "GammaValK", &gammavalk, 0.0, 9.0, code);
    code = cdj_put_param_float(plist, "BlackCorrect", &blackcorrect, 0.0,
			       9.0, code);

    if (code < 0)
			return code;
    
    code = cdj_put_param_bpp(pdev, plist, bpp, bpp, 0);
    
    if (code < 0)
			return code;

    cdj970->quality = quality;
    cdj970->papertype = papertype;
    cdj970->duplex = duplex;
    cdj970->mastergamma = mastergamma;
    cdj970->gammavalc = gammavalc;
    cdj970->gammavalm = gammavalm;
    cdj970->gammavaly = gammavaly;
    cdj970->gammavalk = gammavalk;
    cdj970->blackcorrect = blackcorrect;
    
    return 0;
}

/**********************************************************************************/
/*                                                                                */
/*																	Internal routines                             */
/*                                                                                */
/**********************************************************************************/

/* The DeskJet970C can compress (mode 9) */


/* Some convenient shorthand .. */
#define x_dpi        (pdev->x_pixels_per_inch)
#define y_dpi        (pdev->y_pixels_per_inch)

/* To calculate buffer size as next greater multiple of both parameter and W */
#define calc_buffsize(a, b) (((((a) + ((b) * W) - 1) / ((b) * W))) * W)

/* internal functions */
static void
     FSDlinebw(int scan, int plane_size,
	       struct error_val_field *error_values,
	       byte * kP,
	       int n, int *ep, byte * dp);
		  
static void
     FSDlinec2(int scan, int plane_size,
	       struct error_val_field *error_values,
	       byte * cPa, byte * mPa, byte * yPa, int n,
	       byte * dp, int *ep);
		  
static void
     FSDlinec3(int scan, int plane_size,
	       struct error_val_field *error_values,
	       byte * cPa, byte * mPa, byte * yPa,
	       byte * cPb, byte * mPb, byte * yPb,
	       int n, byte * dp, int *ep);
		   
static void
     FSDlinec4(int scan, int plane_size,
	       struct error_val_field *error_values,
	       byte * cPa, byte * mPa, byte * yPa,
	       byte * cPb, byte * mPb, byte * yPb,
	       int n, byte * dp, int *ep);

static void
     init_error_buffer(struct misc_struct *misc_vars,
		       struct ptr_arrays *data_ptrs);
		       
static void
     do_floyd_steinberg(int scan, int cscan, int plane_size,
			int plane_size_c, int n,
			struct ptr_arrays *data_ptrs,
			gx_device_printer * pdev,
			struct error_val_field *error_values);

			   	   
static int
    do_gcr(int bytecount, byte * inbyte, const byte * kvalues,
	   const byte * cvalues, const byte * mvalues,
	   const byte * yvalues, const int *kcorrect);
	      
static void
     send_scan_lines(gx_device_printer * pdev,
		     struct ptr_arrays *data_ptrs,
		     struct misc_struct *misc_vars,
		     struct error_val_field *error_values,
		     const Gamma *gamma,
		     FILE * prn_stream);
			
static void
     do_gamma(float mastergamma, float gammaval, byte * values);
     
static void
     do_black_correction(float kvalue, int *kcorrect);

static void
     init_data_structure(gx_device_printer * pdev,
			 struct ptr_arrays *data_ptrs,
			 struct misc_struct *misc_vars);
			    
static void
     calculate_memory_size(gx_device_printer * pdev,
			   struct misc_struct *misc_vars);


/* assign_dpi:  
----------------------------------------------------------------------------------*/
static void assign_dpi(int dpi, byte * msb)
{
	if (dpi == 600) {
		msb[0] = 0x02;
		msb[1] = 0x58;
	} else {
		msb[0] = 0x01;
		msb[1] = 0x2c;
	}
}

/* cdj970_terminate_page:  
----------------------------------------------------------------------------------*/
static void cdj970_terminate_page(gx_device_printer * pdev, FILE * prn_stream)
{
	fputs("\033*rC\f\033&l-2H", prn_stream);	/* End Graphics, Reset */
}

/* cdj970_one_time_initialisation:  
----------------------------------------------------------------------------------*/
static void cdj970_one_time_initialisation(gx_device_printer * pdev)
{
    /* Change the margins if necessary. */
    static const float dj_a4[4] = {
	DESKJET_MARGINS_A4
    };
	
    static const float dj_letter[4] = {
	DESKJET_MARGINS_LETTER
    };
    const float *m = (float *)0;

    /* quality setup */
    if (cdj970->quality == DRAFT) {
	gx_device_set_resolution((gx_device *)pdev, 300.0, 300.0);
	cdj970->xscal = 0;
	cdj970->yscal = 0;
	cdj970->intensities = 2; 
    } else if (cdj970->quality == NORMAL) {
	gx_device_set_resolution((gx_device *)pdev, 600.0, 600.0);
	cdj970->xscal = 1;
	cdj970->yscal = 1;
	/* intensities = 4 from initialization */
    } else {		/* quality == PRESENTATION */
	gx_device_set_resolution((gx_device *)pdev, 600.0, 600.0);
	cdj970->xscal = 0;
	cdj970->yscal = 0 ;
	/* intensities = 4 from initialization */
    }
    
    m = (gdev_pcl_paper_size((gx_device *)pdev) == PAPER_SIZE_A4 ? dj_a4 : dj_letter);

    gx_device_set_margins((gx_device *)pdev, m, true);

    cdj970_write_header ((gx_device *)pdev, pdev->file);
}

/* cdj970_print_page: Here comes the hp970 output routine
----------------------------------------------------------------------------------*/
static int cdj970_print_page(gx_device_printer * pdev, FILE * prn_stream)
{
	struct error_val_field error_values;
	struct ptr_arrays data_ptrs;
	struct misc_struct misc_vars;

	Gamma gamma;

	if (PageCtr == 0 && cdj970->ptype == DJ970C) {
		cdj970_one_time_initialisation(pdev);
	}

	/* make a local writable copy of the Gamma tables */
	memcpy(&gamma, gammat[cdj970->ptype], sizeof(Gamma));
    
	/* if mastergamma, don't use the built in functions */
	if (cdj970->mastergamma > 1.0) {
		/* prepare the bw lookup table */
		do_gamma(cdj970->mastergamma, cdj970->gammavalk, gamma.k);
		/* prepare the color lookup table */
		do_gamma(cdj970->mastergamma, cdj970->gammavalc, gamma.c);
		do_gamma(cdj970->mastergamma, cdj970->gammavalm, gamma.m);
		do_gamma(cdj970->mastergamma, cdj970->gammavaly, gamma.y);
	}
    
	/* prepare the black correction table for the unbunt mask */
	do_black_correction(cdj970->blackcorrect, gamma.correct);

	/* Calculate the needed memory */
	calculate_memory_size(pdev, &misc_vars);

	/* and allocate the memory */

	/* Since we need 600 and 300 dpi, we set up several buffers:
     storage contains the data as copied from gs, as well as the
		 plane-data and the out_row buffer.
		 storagec will contain the rescaled color data. It also contains the
		 plane_data for the color-planes - these are needed by the
		 compression routine, but would be overwritten by the
		 b/w-dithering. The color planes allow for overwriting the
		 color-data by the error-data. Since we might use the
		 2bpp feature of the hp970 someday, it is sized like storage.
		 storagee contains the errors from b/w fs-ditherng */

	data_ptrs.storage = (ulong *) gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), misc_vars.storage_size_words, W,
					    "cdj970_print_page");

	/* if we can't allocate working area */
	if (data_ptrs.storage == 0) {
		return_error(gs_error_VMerror);
	}
    
	/* Initialise the needed pointers */
	init_data_structure(pdev, &data_ptrs, &misc_vars);

	/* Start Raster mode */
	(*cdj970->start_raster_mode) (pdev, misc_vars.paper_size, prn_stream);

	/* Send each scan line in turn */
	send_scan_lines(pdev, &data_ptrs, &misc_vars, &error_values, &gamma, prn_stream);

	/* terminate page and eject paper */
	(*cdj970->terminate_page) (pdev, prn_stream);

	/* Free Memory */
	gs_free(gs_lib_ctx_get_non_gc_memory_t(), (char *)data_ptrs.storage, misc_vars.storage_size_words, W,
	    "hp970_print_page");

	PageCtr ++;

	return (0);
}

#define odd(i) ((i & 01) != 0)
#define BUFFER_SIZE 5000

/* GetScanLine:  
----------------------------------------------------------------------------------*/
static int GetScanLine(	gx_device_printer 	* pdev, 
													int 								*lnum,
													struct ptr_arrays 	*data_ptrs,
													struct misc_struct 	*misc_vars,
													word 								rmask)
{
	word *data_words = (word *) data_ptrs->data[misc_vars->scan];
	register word *end_data = data_words + misc_vars->line_size_words;
	unsigned long			tempBuffer[BUFFER_SIZE];
	int				i = 0;
	register word *enddata2;

	if ((cdj970->duplex == BOOK) && (PageCtr%2 == 1)) --(*lnum);
	else ++(*lnum);

	gdev_prn_copy_scan_lines(pdev, *lnum, (byte *) data_words, misc_vars->line_size);

	if (cdj970->quality == DRAFT)
		enddata2 = (unsigned long*)end_data - ((gdev_pcl_paper_size((gx_device*)pdev) == 
							PAPER_SIZE_A4 ? DUPLEX_LEFT_MARGIN_A4/2 : DUPLEX_LEFT_MARGIN_LETTER/2));

	else
		enddata2 = (unsigned long*)end_data - ((gdev_pcl_paper_size((gx_device*)pdev) == 
							PAPER_SIZE_A4 ? DUPLEX_LEFT_MARGIN_A4 : DUPLEX_LEFT_MARGIN_LETTER));

	end_data = enddata2;

	if ((cdj970->duplex == BOOK) && (PageCtr%2 == 1)) {
		memset (tempBuffer, 0, BUFFER_SIZE*sizeof(unsigned long));

		while (enddata2 > data_words) {
			tempBuffer[i++] = *((unsigned long*)enddata2);
			enddata2 --;
		} 

		memcpy (data_words, tempBuffer, i*sizeof(unsigned long));

	}

	misc_vars->scan = 1 - misc_vars->scan;	/* toggle scan direction */
	misc_vars->is_two_pass = odd(*lnum);	/* color output for odd lines */

	/* Mask off 1-bits beyond the line width. */
	end_data[-1] &= rmask;


	/* Remove trailing 0s. */
	while (end_data > data_words && end_data[-1] == 0) 
		end_data--;


	return (end_data - data_words);
}


#define PAGE_CTR_OK ( ((cdj970->duplex < BOOK) && (lnum < lend)) || ((cdj970->duplex == BOOK) && (lnum >= 0) && (PageCtr%2 == 1)) || ((cdj970->duplex == BOOK) && (PageCtr%2 == 0) && (lnum < lend))) 

/* send_scan_lines: Send the scan lines to the printer
----------------------------------------------------------------------------------*/
static void send_scan_lines(	gx_device_printer 			*pdev,
															struct ptr_arrays 			*data_ptrs,
															struct misc_struct 			*misc_vars,
															struct error_val_field 	*error_values,
															const Gamma 						*gamma,
															FILE 										*prn_stream)
{
	int lnum, lend, llen;
	int num_blank_lines = 0;

	word rmask = ~(word) 0 << ((-pdev->width * misc_vars->storage_bpp) & (W * 8 - 1));

	lend = pdev->height - (dev_t_margin(pdev) + dev_b_margin(pdev)) * y_dpi;

	error_values->c = error_values->m = error_values->y = error_values->k = 0;

	/* init the error buffer */
	init_error_buffer(misc_vars, data_ptrs);

	misc_vars->zero_row_count = 0;

	if ((cdj970->duplex == BOOK) && (PageCtr%2==1)) {
		lnum = lend;
		
		if (cdj970->quality == DRAFT)
			num_blank_lines = (gdev_pcl_paper_size((gx_device*)pdev) == PAPER_SIZE_A4 
													? DUPLEX_BOTTOM_MARGIN_A4/2 : DUPLEX_BOTTOM_MARGIN_LETTER/2);
		else
			num_blank_lines = (gdev_pcl_paper_size((gx_device*)pdev) == PAPER_SIZE_A4 
													? DUPLEX_BOTTOM_MARGIN_A4 : DUPLEX_BOTTOM_MARGIN_LETTER);
	} else lnum = -1;

	llen = GetScanLine(pdev, &lnum, data_ptrs, misc_vars, rmask);
	
		while ( PAGE_CTR_OK ) {
		
			while (PAGE_CTR_OK && llen == 0) {
	    	++num_blank_lines;
	    	llen = GetScanLine(pdev, &lnum, data_ptrs, misc_vars, rmask);
			}
		
			if (! PAGE_CTR_OK) {
	    	break;
			}
		
			/* Skip blank lines if any */
			if (num_blank_lines > 0) {
	    	fprintf(prn_stream, "%dy", num_blank_lines / (cdj970->yscal + 1));
	    	memset(data_ptrs->plane_data[0][0], 0, (misc_vars->plane_size * 2 * misc_vars->num_comps));
	    	memset(data_ptrs->plane_data_c[0][0], 0, (misc_vars->plane_size_c * 2 * misc_vars->num_comps));

			}
		
			/* all blank lines printed, now for the non-blank lines */
			if (cdj970->yscal && odd(lnum)) {
	    	/* output a blank black plane for odd lines */
	    	putc('v', prn_stream);
			}
			/* now output all non blank lines */
			while (PAGE_CTR_OK && llen != 0) {
				misc_vars->is_color_data = 0;	/* maybe we have color ? */
	    	(*cdj970->print_non_blank_lines) (pdev, data_ptrs, misc_vars,
																					error_values, gamma, prn_stream);
	    	llen = GetScanLine(pdev, &lnum, data_ptrs, misc_vars, rmask);
			}
			if (cdj970->yscal && odd(lnum)) {	/* output empty line for odd lines */
	    	(*cdj970->print_non_blank_lines) (pdev, data_ptrs, misc_vars,
					      error_values, gamma, prn_stream);
			}
			/* the current line is empty => run the next iteration */
			num_blank_lines = 0;
		}
}

/* print_c2plane: Sprint_line compresses (mode 2) and outputs one plane
----------------------------------------------------------------------------------*/
static void print_c2plane(	FILE 				*prn_stream, 
														char 				plane_code, 
														int 				plane_size,
														const byte 	*curr, 
														const byte 	*prev, 
														byte 				*out_data)
{
	const word *row = (const word *)curr;
	const word *end_row = row + plane_size/W;
	int out_count;

	out_count = gdev_pcl_mode2compress(row, end_row, out_data);

	/* and output the data */
	if (out_count > 0) {
		fprintf(prn_stream, "%d%c", out_count, plane_code);
		fwrite(out_data, sizeof(byte), out_count, prn_stream);
	} else {
		putc(plane_code, prn_stream);
	}
}

#define SET_BLACK(n) *cPa=*cPa|n; *mPa=*mPa|n; *yPa=*yPa|n;

/* cdj970_print_non_blank_lines: Printing non-blank lines 
----------------------------------------------------------------------------------*/
static void cdj970_print_non_blank_lines(	 gx_device_printer 			*pdev,
																						 struct ptr_arrays 			*data_ptrs,
																						 struct misc_struct 		*misc_vars,
																						 struct error_val_field *error_values,
																						 const Gamma 						*gamma,
																						 FILE 									*prn_stream)
{
	static const char *const plane_code[2] = {"vvvv", "wvvv"};

	int 	i;
	byte 	*kP = data_ptrs->plane_data[misc_vars->scan + 2][3];
	byte 	*dp = data_ptrs->data[misc_vars->scan + 2];
	int 	*ep = data_ptrs->errors[misc_vars->scan];

	/* we need cmyk color separation befor all the rest, since
		 black may be contained in the color fields. This needs to
		 be done on all pixel-rows, since even unused color-bytes
		 might generate black */

	misc_vars->is_color_data =
								do_gcr(misc_vars->databuff_size, data_ptrs->data[misc_vars->scan],
								gamma->k, gamma->c, gamma->m, gamma->y, gamma->correct);

	/* dithering the black-plane */
	FSDlinebw(misc_vars->scan, misc_vars->plane_size,
							error_values, kP, misc_vars->num_comps, ep, dp);
	/* output the black plane */
	print_c2plane(prn_stream, 'v', misc_vars->plane_size,
												data_ptrs->plane_data[misc_vars->scan][3],
												data_ptrs->plane_data[1 - misc_vars->scan][3],
												data_ptrs->out_data);


	/* since color resolution is only half of the b/w-resolution,
		 we only output every second row */
	if (!cdj970->yscal || misc_vars->is_two_pass) {
		int plane_size_c = (*rescale_color_plane[cdj970->xscal][cdj970->yscal])
													(misc_vars->databuff_size,
													data_ptrs->data[misc_vars->scan],
													data_ptrs->data[1 - misc_vars->scan],
													data_ptrs->data_c[misc_vars->cscan]) / misc_vars->storage_bpp;

		/* dither the color planes */
		do_floyd_steinberg(misc_vars->scan, misc_vars->cscan,
													misc_vars->plane_size, plane_size_c,
													misc_vars->num_comps, data_ptrs, pdev, error_values);


		/* the color pointers, lower byte */
		/* Transfer raster graphics in the order C, M, Y, that is planes 2,1,0 */
		for (i = misc_vars->num_comps - 2; i >= 0; i--) {

			/* output the lower color planes */
			if (cdj970->intensities > 2) {
	    	/* output the upper color planes */
				print_c2plane(prn_stream, plane_code[0][i], plane_size_c,
													data_ptrs->plane_data_c[misc_vars->cscan][i + 4],
													data_ptrs->plane_data_c[1 - misc_vars->cscan][i + 4],
													data_ptrs->out_data);

			}
			print_c2plane(prn_stream, plane_code[1][i],
													plane_size_c,
													data_ptrs->plane_data_c[misc_vars->cscan][i],
													data_ptrs->plane_data_c[1 - misc_vars->cscan][i],
													data_ptrs->out_data);

		}	/* End For i = num_comps */
		
		misc_vars->cscan = 1 - misc_vars->cscan;
		
	}	/* End of is_two_pass */
	
	return;
}

/* do_floyd_steinberg: 	moved that code into his own subroutine, 
 *											otherwise things get somewhat clumsy
----------------------------------------------------------------------------------*/
static void do_floyd_steinberg(	int 										scan, 
																	int 										cscan, 
																	int 										plane_size,
																	int 										plane_size_c, 
																	int 										n,
																	struct ptr_arrays 			*data_ptrs,
																	gx_device_printer 			*pdev,
																	struct error_val_field 	*error_values)
{
	/* the color pointers */
	byte *cPa, *mPa, *yPa, *cPb, *mPb, *yPb;
	byte *dpc;
	int *epc;

	/* the b/w pointers */
	byte *kP, *dp;
	int *ep;

	/* the color pointers, lower byte */
	cPa = data_ptrs->plane_data_c[cscan + 2][2];
	mPa = data_ptrs->plane_data_c[cscan + 2][1];
	yPa = data_ptrs->plane_data_c[cscan + 2][0];
    
	/* upper byte */
	cPb = data_ptrs->plane_data_c[cscan + 2][6];
	mPb = data_ptrs->plane_data_c[cscan + 2][5];
	yPb = data_ptrs->plane_data_c[cscan + 2][4];
    
	/* data and error */
	dpc = data_ptrs->data_c[cscan + 2];
	epc = data_ptrs->errors_c[cscan];
    
	/* the b/w pointers */
	kP = data_ptrs->plane_data[scan + 2][3];
	dp = data_ptrs->data[scan + 2];
	ep = data_ptrs->errors[scan];

	switch (cdj970->intensities) {
		case 2:
			FSDlinec2(cscan, plane_size_c, error_values,
									cPa, mPa, yPa, n, dpc, epc);
			break;
			
		case 3:
			FSDlinec3(cscan, plane_size_c, error_values,
									cPa, mPa, yPa, cPb, mPb, yPb, n, dpc, epc);
			break;
			
		case 4:
			FSDlinec4(cscan, plane_size_c, error_values,
									cPa, mPa, yPa, cPb, mPb, yPb, n, dpc, epc);
			break;
			
		default:
			assert(0);
	}
	
	return;
}


/* do_gamma: here we do our own gamma-correction 
----------------------------------------------------------------------------------*/
static void do_gamma(float mastergamma, float gammaval, byte values[256])
{
	int i;
	float gamma;

	if (gammaval > 0.0) {
		gamma = gammaval;
	} else {
		gamma = mastergamma;
	}

	for (i = 0; i < 256; i++) {
		values[i] = (byte) (255.0 * (1.0 - pow(((double)(255.0 - (float)i) / 255.0),
			     (double)(1.0 / gamma))));
	}

	return;
}


/* do_black_correction: here we calculate a lookup-table which is used to 
 *											compensate the relativ loss of color due to 
 *											undercolor-removal
----------------------------------------------------------------------------------*/
static void do_black_correction(float kvalue, int kcorrect[256])
{
	int i;

	for (i = 0; i < 256; i++) {
		kcorrect[i] = (int)(100.0 * kvalue * (pow(10.0, pow((i / 255.0), 3.0)) - 1.0));
	}

	return;
}


/* DOUCR: For Better Performance we use a macro here 
----------------------------------------------------------------------------------*/
#define DOUCR(col1, col2, col3, col4)\
{\
  /* determine how far we are from the grey axis. This is  */\
  /* traditionally done by computing MAX(CMY)-MIN(CMY).    */\
  /* However, if two colors are very similar, we could     */\
  /* as either CMYRGB and K. Therefore we calculate the    */\
  /* the distance col1-col2 and col2-col3, and use the     */\
  /* smaller one.                                          */\
  a = *col1 - *col2;\
  b = *col2 - *col3;\
  if (a >= b) {\
    grey_distance = 1.0 - (b/255.0);\
  } else {\
    grey_distance = 1.0 - (a/255.0);\
  }\
  ucr   = (byte) (*col3 * grey_distance); \
  *col4 = *col4 + ucr;  /* add removed black to black */\
  /* remove only as much color as black is surviving the   */\
  /* gamma correction */\
  ucr   = *(kvalues + ucr);\
  *col1 = *col1 - ucr ;\
  *col2 = *col2 - ucr ;\
  *col3 = *col3 - ucr ;\
}


/* DOGCR: For Better Performance we use a macro here 
----------------------------------------------------------------------------------*/
#define DOGCR(col1, col2, col3, col4)\
{\
  ucr = (int) *col3;\
  *col1 -= ucr ;\
  *col2 -= ucr ;\
  *col3 -= ucr ;\
  *col4 += ucr;  /* add removed black to black */\
  kadd  = ucr + *(kcorrect + ucr);\
  uca_fac = 1.0 + (kadd/255.0);\
  *col1 *= uca_fac;\
  *col2 *= uca_fac;\
}


/* do_gcr: Since resolution can be different on different planes, we need to
 *				 do real color separation, here we try a real grey component
 *				 replacement
----------------------------------------------------------------------------------*/
static int do_gcr(	int 				bytecount, 
										byte 				*inbyte,
										const byte 	kvalues[256],
										const byte 	cvalues[256], 
										const byte 	mvalues[256],
										const byte 	yvalues[256], 
										const int 	kcorrect[256])
{
	int i, ucr, kadd, is_color = 0;
	byte *black, *cyan, *magenta, *yellow;
	float uca_fac;

	/* Grey component replacement */
	for (i = 0; i < bytecount; i += 4) {
		black = inbyte++;	/* Assign to black the current address of  inbyte */
		cyan = inbyte++;
		magenta = inbyte++;
		yellow = inbyte++;

		if (*magenta + *yellow + *cyan > 0) {	/* if any color at all */
	    is_color = 1;

	    if ((*cyan >= *magenta) && (*magenta >= *yellow) && (*yellow > 0)) {	/* if any grey component */
				DOGCR(cyan, magenta, yellow, black);
	    } else if ((*cyan >= *yellow) && (*yellow >= *magenta) && (*magenta > 0)) {
				DOGCR(cyan, yellow, magenta, black);
	    } else if ((*yellow >= *magenta) && (*magenta >= *cyan) && (*cyan > 0)) {
				DOGCR(yellow, magenta, cyan, black);
	    } else if ((*yellow >= *cyan) && (*cyan >= *magenta) && (*magenta > 0)) {
				DOGCR(yellow, cyan, magenta, black);
	    } else if ((*magenta >= *yellow) && (*yellow >= *cyan) && (*cyan > 0)) {
				DOGCR(magenta, yellow, cyan, black);
	    } else if ((*magenta >= *cyan) && (*cyan >= *yellow) && (*yellow > 0)) {
				DOGCR(magenta, cyan, yellow, black);
	    } else {		/* do gamma only if no black */
				;
	    }
	    
	    *cyan = *(cvalues + *cyan);
	    *magenta = *(mvalues + *magenta);
	    *yellow = *(yvalues + *yellow);
		}	/* end of if c+m+y > 0 */
		
		*black = *(kvalues + *black);
	}	/* end of for bytecount */
	
	return (is_color);
}

/* rescale_byte_wise2x2: 	Since resolution can be different on different planes, 
 *												we need to rescale the data byte by byte
----------------------------------------------------------------------------------*/
static int rescale_byte_wise2x2(	int 				bytecount, 
																	const byte 	*inbytea, 
																	const byte 	*inbyteb,
																	byte 				*outbyte)
{
	register int i, j;
	int max = bytecount / 2;

	for (i = 0; i < max; i += 4) {
		j = 2 * i;
		/* cyan */
		outbyte[i + 1] = (inbytea[j + 1] + inbytea[j + 5] + inbyteb[j + 1] + inbyteb[j + 5]) / 4;
		
		/* magenta */
		outbyte[i + 2] = (inbytea[j + 2] + inbytea[j + 6] + inbyteb[j + 2] + inbyteb[j + 6]) / 4;
		
		/* yellow */
		outbyte[i + 3] = (inbytea[j + 3] + inbytea[j + 7] + inbyteb[j + 3] + inbyteb[j + 7]) / 4;
	}
	
	return (max);
}


/* rescale_byte_wise2x1: 	Since resolution can be different on different planes, 
 *												we need to rescale the data byte by byte
----------------------------------------------------------------------------------*/
static int rescale_byte_wise2x1(	int 				bytecount, 
																	const byte 	*inbytea, 
																	const byte 	*inbyteb,
																	byte 				*outbyte)
{
	register int i, j;
	int max = bytecount / 2;

	for (i = 0; i < max; i += 4) {
		j = 2 * i;
		
		/* cyan */
		outbyte[i + 1] = (inbytea[j + 1] + inbytea[j + 5]) / 2;
		
		/* magenta */
		outbyte[i + 2] = (inbytea[j + 2] + inbytea[j + 6]) / 2;
		
		/* yellow */
		outbyte[i + 3] = (inbytea[j + 3] + inbytea[j + 7]) / 2;
	}
	
	return (max);
}

/* rescale_byte_wise1x2: 	Since resolution can be different on different planes, 
 *												we need to rescale the data byte by byte
----------------------------------------------------------------------------------*/
static int rescale_byte_wise1x2(	int 				bytecount, 
																	const byte 	*inbytea, 
																	const byte 	*inbyteb,
																	byte 				*outbyte)
{
	register int i;

	for (i = 0; i < bytecount; i += 4) {
		/* cyan */
		outbyte[i + 1] = (inbytea[i + 1] + inbyteb[i + 1]) / 2;
		
		/* magenta */
		outbyte[i + 2] = (inbytea[i + 2] + inbyteb[i + 2]) / 2;
		
		/* yellow */
		outbyte[i + 3] = (inbytea[i + 3] + inbyteb[i + 3]) / 2;
	}
	
	return bytecount;
}

/* rescale_byte_wise1x1: 	Since resolution can be different on different planes, 
 *												we need to rescale the data byte by byte
----------------------------------------------------------------------------------*/
static int rescale_byte_wise1x1(	int 					bytecount, 
																	const byte 		*inbytea, 
																	const byte 		*inbyteb,
																	byte 					*outbyte)
{
	register int i;

	for (i = 0; i < bytecount; i += 4) {
		/* cyan */
		outbyte[i + 1] = inbytea[i + 1];
		
		/* magenta */
		outbyte[i + 2] = inbytea[i + 2];
		
		/* yellow */
		outbyte[i + 3] = inbytea[i + 3];
	}
	
	return (bytecount);
}

/* MACROS FOR DITHERING (we use macros for compact source and faster code) */
/* Floyd-Steinberg dithering. Often results in a dramatic improvement in
 * subjective image quality, but can also produce dramatic increases in
 * amount of printer data generated and actual printing time!! Mode 9 2D
 * compression is still useful for fairly flat colour or blank areas but its
 * compression is much less effective in areas where the dithering has
 * effectively randomised the dot distribution. */

#define RSHIFT ((I * 8) - 16)
#define SHIFT ((I * 8) - 13)
#define MAXVALUE  (255 << SHIFT)
#define RANDOM (((rand() << RSHIFT) % (MAXVALUE / 2))  - MAXVALUE /4);
#define MINVALUE  0
#define C 8

#define THRESHOLD (128 << SHIFT)

/* --- needed for the hp970 -- */
#define SHIFTS ((I * 8) - 14)
#define SHIFTM ((I * 8) - 13)
#define SHIFTL ((I * 8) - 12)

#define MAXVALUES  (160 << SHIFTM)
#define MAXVALUEM  (226 << SHIFTM)
#define MAXVALUEL  (255 << SHIFTM)

#define THRESHOLDS (128 << SHIFTM)
#define THRESHOLDM (192 << SHIFTM)
#define THRESHOLDL (226 << SHIFTM)


/* --------------------------- */

/* init_error_buffer: 	initialise the error_buffer  
----------------------------------------------------------------------------------*/
static void init_error_buffer(	struct misc_struct 	*misc_vars,
																struct ptr_arrays 	*data_ptrs)
{
	int i;
	int *ep;
	int *epc;

	ep = data_ptrs->errors[0];
	epc = data_ptrs->errors_c[0];

	if (misc_vars->bits_per_pixel > 4) {	/* Randomly seed initial error buffer */
		/* Otherwise, the first dithered rows would look rather uniform */
		for (i = 0; i < misc_vars->databuff_size; i++) {	/* 600dpi planes */
	    *ep++ = RANDOM;
		}

		/* Now for the 2 * 300dpi color planes */
		for (i = 0; i < misc_vars->databuff_size_c; i++) {
	    *epc++ = RANDOM;
		}
	}
	
	return;
}

/* FSdither: 	
----------------------------------------------------------------------------------*/
#define FSdither(inP, out, errP, Err, Bit, Offset, Element)\
{\
	oldErr = Err;\
	Err = (*(errP + Element)\
	       + ((Err * 7 + C) >> 4)\
	       + ((int)*(inP + Element) << SHIFT));\
	if (Err > THRESHOLD) {\
	  out |= Bit;\
	  Err -= MAXVALUE;\
	}\
	*(errP + (Element + Offset)) += ((Err * 3 + C) >> 4);\
	*(errP + Element) = ((Err * 5 + oldErr + C) >> 4);\
}


/* FSDlinebw: The hp970 has 600dpi black and 300 dpi color.
----------------------------------------------------------------------------------*/
static void FSDlinebw(	int 										scan, 
												int 										plane_size,
												struct error_val_field 	*error_values,
												byte 										*kP, 
												int 										n, 
												int 										*ep, 
												byte 										*dp)
{
	if (scan == 0) {		/* going_up */
		byte k, bitmask;	/* k = outbyte byte, whereas bitmask defines the
												 bit to be set within k */
		int oldErr, i;

		for (i = 0; i < plane_size; i++) {
		  bitmask = 0x80;
		  
	    for (k = 0; bitmask != 0; bitmask >>= 1) {
			/* dp points to the first word of the input data which is in  kcmy-format */
			/* k points to the beginning of the first outbut byte, which
				 is filled up, bit by bit while looping over bytemask */
			/* ep points to the first word of the error-plane which
				 contains the errors kcmy format */
			/* err_values->k tempararily holds the error-value */
			/* bitmask selects the bit to be set in the outbyte */
			/* n gives the offset for the byte selection within
				 words. With simple cmyk-printing, this should be 4 */
			/* 0 points to the active color within the input-word, i.e. 0
				 = black, 1 = cyan, 2 = yellow, 3 = magenta */

				FSdither(dp, k, ep, error_values->k, bitmask, -n, 0);
				dp += n, ep += n;	/* increment the input and error pointer one
														 word (=4 byte) further, in order to
														 convert the next word into an bit */
			}
			*kP++ = k;		/* fill the output-plane byte with the computet byte
											 and increment the output plane pointer  one byte */
		}
	} else {			/* going_down */
		byte k, bitmask;
		int oldErr, i;

		for (i = 0; i < plane_size; i++) {
	    bitmask = 0x01;
	    
			for (k = 0; bitmask != 0; bitmask <<= 1) {
				dp -= n, ep -= n;
				FSdither(dp, k, ep, error_values->k, bitmask, n, 0);
			}
			*--kP = k;
		}
	}
	
	return;
}


/* FSDlinec2: Since bw has already been dithered for the hp970c, we need
 * 						an adapted dither algorythm
----------------------------------------------------------------------------------*/
static void FSDlinec2(	int 										scan, 
												int 										plane_size,
												struct error_val_field 	*error_values,
												byte 										*cPa, 
												byte 										*mPa, 
												byte 										*yPa, 
												int 										n,
												byte 										*dp, 
												int 										*ep)
{
	if (scan == 0) {		/* going_up */
		int oldErr, i;
		byte ca, ya, ma, bitmask;

		for (i = 0; i < plane_size; i++) {
	    bitmask = 0x80;
	    ca = ya = ma = 0;
	    
			for (ca = 0; bitmask != 0; bitmask >>= 1) {
				FSdither(dp, ca, ep, error_values->c, bitmask, -n, n - 3);
				FSdither(dp, ma, ep, error_values->m, bitmask, -n, n - 2);
				FSdither(dp, ya, ep, error_values->y, bitmask, -n, n - 1);
				dp += n, ep += n;
	    }
	    
	    *cPa++ = ca;
	    *mPa++ = ma;
	    *yPa++ = ya;
		}
	} else {			/* going_down */
		byte ca, ya, ma, bitmask;
		int oldErr, i;

		for (i = 0; i < plane_size; i++) {
	    bitmask = 0x01;
	    ca = ya = ma = 0;
	    
	    for (ca = 0; bitmask != 0; bitmask <<= 1) {
				dp -= n, ep -= n;
				FSdither(dp, ya, ep, error_values->y, bitmask, n, n - 1);
				FSdither(dp, ma, ep, error_values->m, bitmask, n, n - 2);
				FSdither(dp, ca, ep, error_values->c, bitmask, n, n - 3);
	    }
	    
	    *--yPa = ya;
	    *--mPa = ma;
	    *--cPa = ca;
		}
	}
	
	return;
}


/* FSdither9703: while printing on paper, we only use 3 -intensities
----------------------------------------------------------------------------------*/
#define FSdither9703(inP, outa, outb, errP, Err, Bit, Offset, Element)\
{\
	oldErr = Err;\
	Err = (*(errP + Element)\
	       + ((Err * 7 + C) >> 4)\
	       + ((int) *(inP + Element) << SHIFT));\
	if ((Err > THRESHOLDS) && (Err <= THRESHOLDM)) {\
	  outa |= Bit;\
	  Err -= MAXVALUES;\
	}\
	if (Err > THRESHOLDM) {\
	  outb |= Bit;\
	  Err -= MAXVALUEM;\
	}\
	*(errP + (Element + Offset)) += ((Err * 3 + C) >> 4);\
	*(errP + Element) = ((Err * 5 + oldErr + C) >> 4);\
}


/* FSDlinec3: On ordinary paper, we'll only use 3 intensities with the hp970
----------------------------------------------------------------------------------*/
static void FSDlinec3(	int 										scan, 
												int 										plane_size,
												struct error_val_field 	*error_values,
												byte 										*cPa, 
												byte 										*mPa, 
												byte 										*yPa,
												byte 										*cPb, 
												byte 										*mPb, 
												byte 										*yPb,
												int 										n, 
												byte 										*dp, 
												int 										*ep)
{
	if (scan == 0) {		/* going_up */
		byte ca, ya, ma, cb, yb, mb, bitmask;
		int oldErr, i;

		for (i = 0; i < plane_size; i++) {
	    bitmask = 0x80;
	    ca = ya = ma = cb = yb = mb = 0;
	    
	    for (ca = 0; bitmask != 0; bitmask >>= 1) {
				FSdither9703(dp, ca, cb, ep, error_values->c, bitmask, -n, n - 3);
				FSdither9703(dp, ma, mb, ep, error_values->m, bitmask, -n, n - 2);
				FSdither9703(dp, ya, yb, ep, error_values->y, bitmask, -n, n - 1);
				dp += n, ep += n;
	    }
	    
	    *cPa++ = ca;
	    *mPa++ = ma;
	    *yPa++ = ya;
	    *cPb++ = cb;
	    *mPb++ = mb;
	    *yPb++ = yb;
		}
	} else {			/* going_down */
		byte ca, ya, ma, cb, yb, mb, bitmask;
		int oldErr, i;

		for (i = 0; i < plane_size; i++) {
	    bitmask = 0x01;
	    ca = ya = ma = cb = yb = mb = 0;
	    
	    for (ca = 0; bitmask != 0; bitmask <<= 1) {
				dp -= n, ep -= n;
				FSdither9703(dp, ya, yb, ep, error_values->y, bitmask, n, n - 1);
				FSdither9703(dp, ma, mb, ep, error_values->m, bitmask, n, n - 2);
				FSdither9703(dp, ca, cb, ep, error_values->c, bitmask, n, n - 3);
	    }
	    
	    *--yPa = ya;
	    *--mPa = ma;
	    *--cPa = ca;
	    *--yPb = yb;
	    *--mPb = mb;
	    *--cPb = cb;
		}
	}
	
	return;
}


/* FSdither9704: the hp970 knows about 4 different color intensities per color
----------------------------------------------------------------------------------*/
#define FSdither9704(inP, outa, outb, errP, Err, Bit, Offset, Element)\
{\
	oldErr = Err;\
	Err = (*(errP + Element)\
	       + ((Err * 7 + C) >> 4)\
	       + ((int) *(inP + Element) << SHIFT));\
	if ((Err > THRESHOLDS) && (Err <= THRESHOLDM)) {\
	  outa |= Bit;\
	  Err -= MAXVALUES;\
	}\
	if ((Err > THRESHOLDM) && (Err <= THRESHOLDL)) {\
	  outb |= Bit;\
	  Err -= MAXVALUEM;\
	}\
	if (Err > THRESHOLDL) {\
          outa |= Bit;\
	  outb |= Bit;\
	  Err -= MAXVALUEL;\
	}\
	*(errP + (Element + Offset)) += ((Err * 3 + C) >> 4);\
	*(errP + Element) = ((Err * 5 + oldErr + C) >> 4);\
}


/* FSDlinec4: The hp970c knows about 4 intensity levels per color. Once more, 
 *						we need an adapted dither algorythm
----------------------------------------------------------------------------------*/
static void FSDlinec4(	int 										scan, 
												int 										plane_size,
												struct error_val_field 	*error_values,
												byte 										*cPa, 
												byte 										*mPa, 
												byte 										*yPa,
												byte 										*cPb, 
												byte 										*mPb, 
												byte 										*yPb,
												int 										n, 
												byte 										*dp, 
												int 										*ep)
{
	if (scan == 0) {		/* going_up */
		byte ca, ya, ma, cb, yb, mb, bitmask;
		int oldErr, i;

		for (i = 0; i < plane_size; i++) {
	    bitmask = 0x80;
	    ca = ya = ma = cb = yb = mb = 0;
	    
	    for (ca = 0; bitmask != 0; bitmask >>= 1) {
				FSdither9704(dp, ca, cb, ep, error_values->c, bitmask, -n, n - 3);
				FSdither9704(dp, ma, mb, ep, error_values->m, bitmask, -n, n - 2);
				FSdither9704(dp, ya, yb, ep, error_values->y, bitmask, -n, n - 1);
				dp += n, ep += n;
				
	    }
	    
	    *cPa++ = ca;
	    *mPa++ = ma;
	    *yPa++ = ya;
	    *cPb++ = cb;
	    *mPb++ = mb;
	    *yPb++ = yb;
		}
	} else {			/* going_down */
		byte ca, ya, ma, cb, yb, mb, bitmask;
		int oldErr, i;

		for (i = 0; i < plane_size; i++) {
	    bitmask = 0x01;
	    ca = ya = ma = cb = yb = mb = 0;
	    
	    for (ca = 0; bitmask != 0; bitmask <<= 1) {
				dp -= n, ep -= n;
				FSdither9704(dp, ya, yb, ep, error_values->y, bitmask, n, n - 1);
				FSdither9704(dp, ma, mb, ep, error_values->m, bitmask, n, n - 2);
				FSdither9704(dp, ca, cb, ep, error_values->c, bitmask, n, n - 3);
	    }
	    *--yPa = ya;
	    *--mPa = ma;
	    *--cPa = ca;
	    *--yPb = yb;
	    *--mPb = mb;
	    *--cPb = cb;
		}
	}
	
	return;
}

/* calculate_memory_size: calculate the needed memory
----------------------------------------------------------------------------------*/
static void calculate_memory_size(	gx_device_printer 	*pdev,
																		struct misc_struct 	*misc_vars)
{
	int xfac = cdj970->xscal ? 2 : 1;

	misc_vars->line_size = gdev_prn_raster(pdev);
	misc_vars->line_size_c = misc_vars->line_size / xfac;
	misc_vars->line_size_words = (misc_vars->line_size + W - 1) / W;
	misc_vars->paper_size = gdev_pcl_paper_size((gx_device *) pdev);
	misc_vars->num_comps = pdev->color_info.num_components;
	misc_vars->bits_per_pixel = pdev->color_info.depth;
	misc_vars->storage_bpp = misc_vars->num_comps * 8;
	misc_vars->expanded_bpp = misc_vars->num_comps * 8;
	misc_vars->errbuff_size = 0;
	misc_vars->errbuff_size_c = 0;

	misc_vars->plane_size = calc_buffsize(misc_vars->line_size, misc_vars->storage_bpp);

	/* plane_size_c is dependedend on the bits used for
		 dithering. Currently 2 bits are sufficient  */
	misc_vars->plane_size_c = 2 * misc_vars->plane_size / xfac;

	/* 4n extra values for line ends */
	/* might be wrong, see gdevcdj.c */
	misc_vars->errbuff_size =
					calc_buffsize((misc_vars->plane_size * misc_vars->expanded_bpp +
					misc_vars->num_comps * 4) * I, 1);

	/* 4n extra values for line ends */
	misc_vars->errbuff_size_c =
					calc_buffsize((misc_vars->plane_size_c / 2 * misc_vars->expanded_bpp
					+ misc_vars->num_comps * 4) * I, 1);

	misc_vars->databuff_size =
					misc_vars->plane_size * misc_vars->storage_bpp;

	misc_vars->databuff_size_c =
					misc_vars->plane_size_c / 2 * misc_vars->storage_bpp;


	misc_vars->outbuff_size = misc_vars->plane_size * 4;

	misc_vars->storage_size_words = (((misc_vars->plane_size) * 2 
					* misc_vars->num_comps)
					+ misc_vars->databuff_size
					+ misc_vars->errbuff_size
					+ misc_vars->outbuff_size
					+ ((misc_vars->plane_size_c)
					* 2
					* misc_vars->num_comps)
					+ misc_vars->databuff_size_c
					+ misc_vars->errbuff_size_c
					+ (4 * misc_vars->plane_size_c))
					/ W;

	return;
}

/* init_data_structure: Initialise the needed pointers
----------------------------------------------------------------------------------*/
static void init_data_structure(	gx_device_printer 	*pdev,
																		struct ptr_arrays 	*data_ptrs,
																		struct misc_struct	*misc_vars)
{
	int i;
	byte *p = (byte *) data_ptrs->storage;

	misc_vars->scan = 0;
	misc_vars->cscan = 0;
	misc_vars->is_two_pass = 1;

	/* the b/w pointer */
	data_ptrs->data[0] = data_ptrs->data[1] = data_ptrs->data[2] = p;
	data_ptrs->data[3] = p + misc_vars->databuff_size;
	/* Note: The output data will overwrite part of the input-data */

	if (misc_vars->bits_per_pixel > 1) {
		p += misc_vars->databuff_size;
	}
	
	if (misc_vars->bits_per_pixel > 4) {
		data_ptrs->errors[0] = (int *)p + misc_vars->num_comps * 2;
		data_ptrs->errors[1] = data_ptrs->errors[0] + misc_vars->databuff_size;
		p += misc_vars->errbuff_size;
	}
	
	for (i = 0; i < misc_vars->num_comps; i++) {
		data_ptrs->plane_data[0][i] = data_ptrs->plane_data[2][i] = p;
		p += misc_vars->plane_size;
	}
	
	for (i = 0; i < misc_vars->num_comps; i++) {
		data_ptrs->plane_data[1][i] = p;
		data_ptrs->plane_data[3][i] = p + misc_vars->plane_size;
		p += misc_vars->plane_size;
	}
	
	data_ptrs->out_data = p;
	p += misc_vars->outbuff_size;

	/* ---------------------------------------------------------
		 now for the color pointers 
	   --------------------------------------------------------- */

	data_ptrs->data_c[0] = data_ptrs->data_c[1] = data_ptrs->data_c[2] = p;
	data_ptrs->data_c[3] = p + misc_vars->databuff_size_c;
	/* Note: The output data will overwrite part of the input-data */

	if (misc_vars->bits_per_pixel > 1) {
		p += misc_vars->databuff_size_c;
	}
	
	if (misc_vars->bits_per_pixel > 4) {
		data_ptrs->errors_c[0] = (int *)p + misc_vars->num_comps * 2;
		data_ptrs->errors_c[1] = data_ptrs->errors_c[0] + misc_vars->databuff_size_c;
		p += misc_vars->errbuff_size_c;
	}
	
	/* pointer for the lower bits of the output data */
	for (i = 0; i < misc_vars->num_comps; i++) {
		data_ptrs->plane_data_c[0][i] = data_ptrs->plane_data_c[2][i] = p;
		p += misc_vars->plane_size_c / 2;
	}
	
	for (i = 0; i < misc_vars->num_comps; i++) {
		data_ptrs->plane_data_c[1][i] = p;
		data_ptrs->plane_data_c[3][i] = p + misc_vars->plane_size_c / 2;
		p += misc_vars->plane_size_c / 2;
	}

	/* pointer for the upper bits of the output data */
	for (i = 0; i < misc_vars->num_comps; i++) {
		data_ptrs->plane_data_c[0][i + 4] = data_ptrs->plane_data_c[2][i + 4] = p;
		p += misc_vars->plane_size_c / 2;
	}
	
	for (i = 0; i < misc_vars->num_comps; i++) {
		data_ptrs->plane_data_c[1][i + 4] = p;
		data_ptrs->plane_data_c[3][i + 4] = p + misc_vars->plane_size_c / 2;
		p += misc_vars->plane_size_c / 2;
	}

	for (i = 0; i < misc_vars->num_comps; i++) {
		data_ptrs->test_data[i] = p;
		p += misc_vars->plane_size_c / 2;
	}

	/* Clear temp storage */
	memset(data_ptrs->storage, 0, misc_vars->storage_size_words * W);

	return;
}	


/* cdj970_start_raster_mode: Configure the printer and start Raster mode 
----------------------------------------------------------------------------------*/
static void cdj970_start_raster_mode(	gx_device_printer *pdev, 
																				int 							paper_size,
																				FILE 							*prn_stream)
{
	int xres, yres;		/* x,y resolution for color planes */
	hp970_cmyk_init_t init;

	init = hp970_cmyk_init;

	init.a[13] = cdj970->intensities;	/* Intensity levels cyan */
	init.a[19] = cdj970->intensities;	/* Intensity levels magenta */
	init.a[25] = cdj970->intensities;	/* Intensity levels yellow */

	/* black plane resolution */
	assign_dpi(cdj970->x_pixels_per_inch, init.a + 2);
	assign_dpi(cdj970->y_pixels_per_inch, init.a + 4);
	
	/* color plane resolution */
	xres = cdj970->x_pixels_per_inch / (cdj970->xscal + 1);
	yres = cdj970->y_pixels_per_inch / (cdj970->yscal + 1);
	
	/* cyan */
	assign_dpi(xres, init.a + 8);
	assign_dpi(yres, init.a + 10);
	
	/* magenta */
	assign_dpi(xres, init.a + 14);  
	assign_dpi(yres, init.a + 16);
	
	/* yellow */
	assign_dpi(xres, init.a + 20);
	assign_dpi(yres, init.a + 22);

	/* Page size, orientation, top margin & perforation skip */
	fprintf(prn_stream, "\033&l%dA", paper_size);

	/* Print Quality, -1 = draft, 0 = normal, 1 = presentation */
	fprintf(prn_stream, "\033*o%dM", cdj970->quality);
	
	/* Media Type,0 = plain paper, 1 = bond paper, 2 = special
		 paper, 3 = glossy film, 4 = transparency film */
	fprintf(prn_stream, "\033&l%dM", cdj970->papertype);

	fprintf (prn_stream, "\033u%dD\033&l0e0L", xres);

	/* fputs("\033u%dD\033&l0e0L", prn_stream); */

	fprintf(prn_stream, "\033*p%dY", (int)(600 * DOFFSET));

	/* This will start and configure the raster-mode */
	fprintf(prn_stream, "\033*g%dW", (int)sizeof(init.a));	/* The new configure
																													   raster data comand */
	fwrite(init.a, sizeof(byte), sizeof(init.a), prn_stream);		/* Transmit config
																																 data */
	fputs("\033&l0H", prn_stream);
	fputs("\033*r1A", prn_stream);																															 
	/* From now on, all escape commands start with \033*b, so we
	 * combine them (if the printer supports this). */
	fputs("\033*b", prn_stream);
	
	/* Set compression if the mode has been defined. */
	if (cdj970->compression)
		fprintf(prn_stream, "%dm", cdj970->compression);

	return;
}	

/* cdj_put_param_int: 
----------------------------------------------------------------------------------*/
static int cdj_put_param_int(	gs_param_list 	*plist, 
																gs_param_name 	pname, 
																int 						*pvalue,
																int 						minval, 
																int 						maxval, 
																int 						ecode)
{
	int code, value;

	switch (code = param_read_int(plist, pname, &value)) {
		default:
			return code;
			
		case 1:
			return ecode;
			
		case 0:
			if (value < minval || value > maxval)
				param_signal_error(plist, pname, gs_error_rangecheck);
			*pvalue = value;
			return (ecode < 0 ? ecode : 1);
	}
}

/* cdj_put_param_float: 
----------------------------------------------------------------------------------*/
static int cdj_put_param_float(	gs_param_list 	*plist, 
																	gs_param_name 	pname, 
																	float 					*pvalue,
																	float 					minval, 
																	float 					maxval, 
																	int 						ecode)
{
	int code;
	float value;

	switch (code = param_read_float(plist, pname, &value)) {
		default:
			return code;
			
		case 1:
			return ecode;
			
		case 0:
			if (value < minval || value > maxval)
				param_signal_error(plist, pname, gs_error_rangecheck);
			*pvalue = value;
			return (ecode < 0 ? ecode : 1);
	}
}

/* cdj_set_bpp: 
----------------------------------------------------------------------------------*/
static int cdj_set_bpp(gx_device * pdev, int bpp, int ccomps)
{
	gx_device_color_info *ci = &pdev->color_info;

	if (ccomps && bpp == 0) {
		if (cprn_device->cmyk) {
	    switch (ccomps) {
	    
				default:
					return gs_error_rangecheck;
					/*NOTREACHED */
				break;

				case 1:
					bpp = 1;
					break;

				case 3:
					bpp = 24;
					break;

				case 4:
					switch (ci->depth) {
						case 8:
						case 16:
						case 24:
						case 32:
						break;

						default:
							bpp = cprn_device->default_depth;
						break;
					}
					break;
			}
		}
	}
	
	if (bpp == 0) {
		bpp = ci->depth;	/* Use the current setting. */
	}
	
	if (cprn_device->cmyk < 0) {
		/* Reset procedures because we may have been in another mode. */
		dev_proc(pdev, map_cmyk_color) = gdev_cmyk_map_cmyk_color;
		dev_proc(pdev, map_rgb_color) = NULL;
		dev_proc(pdev, map_color_rgb) = gdev_cmyk_map_color_rgb;

		if (pdev->is_open)
	    gs_closedevice(pdev);
    }
    /* Check for valid bpp values */

    switch (bpp) {
			case 16:
			case 32:
				if (cprn_device->cmyk && ccomps && ccomps != 4)
					goto bppe;
					break;

			case 24:
				if (!cprn_device->cmyk || ccomps == 0 || ccomps == 4) {
					break;
				} else if (ccomps == 1) {
					goto bppe;
				} else {
					/* 3 components 24 bpp printing for CMYK device. */
					cprn_device->cmyk = -1;
				}
				break;

			case 8:
				if (cprn_device->cmyk) {
					if (ccomps) {
						if (ccomps == 3) {
							cprn_device->cmyk = -1;
							bpp = 3;
						} else if (ccomps != 1 && ccomps != 4) {
							goto bppe;
						}
					}
					
					if (ccomps != 1)
						break;
				} else {
					break;
				}
			case 1:
				if (ccomps != 1)
					goto bppe;

				if (cprn_device->cmyk && bpp != pdev->color_info.depth) {
					dev_proc(pdev, map_cmyk_color) = NULL;
					dev_proc(pdev, map_rgb_color) = gdev_cmyk_map_rgb_color;

					if (pdev->is_open) {
						gs_closedevice(pdev);
					}
				}
				break;

			case 3:
				if (!cprn_device->cmyk) {
					break;
				}
			default:
				bppe:return gs_error_rangecheck;
    }


    if (cprn_device->cmyk == -1) {
			dev_proc(pdev, map_cmyk_color) = NULL;
			dev_proc(pdev, map_rgb_color) = gdev_pcl_map_rgb_color;
			dev_proc(pdev, map_color_rgb) = gdev_pcl_map_color_rgb;

			if (pdev->is_open) {
				gs_closedevice(pdev);
			}
    }
    
    switch (ccomps) {
			case 0:
				break;

			case 1:
				if (bpp != 1 && bpp != 8)
					goto cce;
				break;

			case 4:
				if (cprn_device->cmyk) {
					if (bpp >= 8)
						break;
				}
				
			case 3:
				if (bpp == 1 || bpp == 3 || bpp == 8 || bpp == 16 || bpp == 24 || bpp == 32) {
					break;
				}
				
			cce: 
			default:
				return gs_error_rangecheck;
    }

    if (cprn_device->cmyk) {
			if (cprn_device->cmyk > 0) {
				ci->num_components = ccomps ? ccomps : (bpp < 8 ? 1 : 4);
			} else {
				ci->num_components = ccomps ? ccomps : (bpp < 8 ? 1 : 3);
			}
			
			if (bpp != 1 && ci->num_components == 1) {	/* We do dithered grays. */
				bpp = bpp < 8 ? 8 : bpp;
		}
		
		ci->max_color = (1 << (bpp >> 2)) - 1;
		ci->max_gray = (bpp >= 8 ? 255 : 1);

		if (ci->num_components == 1) {
	    ci->dither_grays = (bpp >= 8 ? 5 : 2);
	    ci->dither_colors = (bpp >= 8 ? 5 : bpp > 1 ? 2 : 0);
		} else {
	    ci->dither_grays = (bpp > 8 ? 5 : 2);
	    ci->dither_colors = (bpp > 8 ? 5 : bpp > 1 ? 2 : 0);
		}
	} else {
		ci->num_components = (bpp == 1 || bpp == 8 ? 1 : 3);
		ci->max_color = (bpp >= 8 ? 255 : bpp > 1 ? 1 : 0);
		ci->max_gray = (bpp >= 8 ? 255 : 1);
		ci->dither_grays = (bpp >= 8 ? 5 : 2);
		ci->dither_colors = (bpp >= 8 ? 5 : bpp > 1 ? 2 : 0);
	}

	ci->depth = ((bpp > 1) && (bpp < 8) ? 8 : bpp);

	return (0);
}

/*
 * Map a CMYK color to a color index. We just use depth / 4 bits per color
 * to produce the color index.
 *
 * Important note: CMYK values are stored in the order K, C, M, Y because of
 * the way the HP drivers work.
 *
 */

#define gx_color_value_to_bits(cv, b) \
    ((cv) >> (gx_color_value_bits - (b)))
#define gx_bits_to_color_value(cv, b) \
    ((cv) << (gx_color_value_bits - (b)))

#define gx_cmyk_value_bits(c, m, y, k, b) \
    ((gx_color_value_to_bits((k), (b)) << (3 * (b))) | \
     (gx_color_value_to_bits((c), (b)) << (2 * (b))) | \
     (gx_color_value_to_bits((m), (b)) << (b)) | \
     (gx_color_value_to_bits((y), (b))))

#define gx_value_cmyk_bits(v, c, m, y, k, b) \
    (k) = gx_bits_to_color_value(((v) >> (3 * (b))) & ((1 << (b)) - 1), (b)), \
    (c) = gx_bits_to_color_value(((v) >> (2 * (b))) & ((1 << (b)) - 1), (b)), \
    (m) = gx_bits_to_color_value(((v) >> (b)) & ((1 << (b)) - 1), (b)), \
    (y) = gx_bits_to_color_value((v) & ((1 << (b)) - 1), (b))

/* gdev_cmyk_map_cmyk_color: 
----------------------------------------------------------------------------------*/
static gx_color_index gdev_cmyk_map_cmyk_color(gx_device * pdev,
						const gx_color_value cv[])
{
	gx_color_index color;
	gx_color_value cyan, magenta, yellow, black;

	cyan = cv[0]; magenta = cv[1]; yellow = cv[2]; black = cv[3];
	switch (pdev->color_info.depth) {
		case 1:
			color = (cyan | magenta | yellow | black) > gx_max_color_value / 2 ?
	    (				gx_color_index) 1 : (gx_color_index) 0;
			break;

		default:{
	    int nbits = pdev->color_info.depth;

	    if (cyan == magenta && magenta == yellow) {
				/* Convert CMYK to gray -- Red Book 6.2.2 */
				float bpart = ((float)cyan) * (lum_red_weight / 100.) +
											((float)magenta) * (lum_green_weight / 100.) +
											((float)yellow) * (lum_blue_weight / 100.) +
											(float)black;

				cyan = magenta = yellow = (gx_color_index) 0;
				black = (gx_color_index) (bpart > gx_max_color_value ?
											gx_max_color_value : bpart);
	    }
	    color = gx_cmyk_value_bits(cyan, magenta, yellow, black,
				       nbits >> 2);
		}
	}

	return (color);
}


/* gdev_cmyk_map_rgb_color: Mapping of RGB colors to gray values. 
----------------------------------------------------------------------------------*/
static gx_color_index gdev_cmyk_map_rgb_color(	gx_device * pdev,
						const gx_color_value cv[])
{
        gx_color_value r, g, b;

	r = cv[0]; g = cv[1]; b = cv[2];
	if (gx_color_value_to_byte(r & g & b) == 0xff) {
		return (gx_color_index) 0;	/* White */
	} else {
		gx_color_value c = gx_max_color_value - r;
		gx_color_value m = gx_max_color_value - g;
		gx_color_value y = gx_max_color_value - b;

		switch (pdev->color_info.depth) {
	    case 1:
				return (c | m | y) > gx_max_color_value / 2 ? (gx_color_index) 1 : (gx_color_index) 0;
				/*NOTREACHED */
				break;

	    case 8:
				return ((ulong) c * lum_red_weight * 10
									+ (ulong) m * lum_green_weight * 10
									+ (ulong) y * lum_blue_weight * 10)
									>> (gx_color_value_bits + 2);
				/*NOTREACHED */
				break;
		}
	}

	return (gx_color_index) 0;	/* This should never happen. */
}


/* gdev_cmyk_map_rgb_color: Mapping of CMYK colors. 
----------------------------------------------------------------------------------*/
static int gdev_cmyk_map_color_rgb(	gx_device 			*pdev, 
																			gx_color_index 	color,
																			gx_color_value 	prgb[3])
{
	switch (pdev->color_info.depth) {
		case 1:
			prgb[0] = prgb[1] = prgb[2] = gx_max_color_value * (1 - color);
			break;

		case 8:
			if (pdev->color_info.num_components == 1) {
				gx_color_value value = (gx_color_value) color ^ 0xff;
				prgb[0] = prgb[1] = prgb[2] = (value << 8) + value;
				break;
			}
			
		default:{
	    unsigned long bcyan, bmagenta, byellow, black;
	    int nbits = pdev->color_info.depth;

	    gx_value_cmyk_bits(color, bcyan, bmagenta, byellow, black, nbits >> 2);

#			ifdef USE_ADOBE_CMYK_RGB

	    /* R = 1.0 - min(1.0, C + K), etc. */

	    bcyan += black, bmagenta += black, byellow += black;
	    prgb[0] = (bcyan > gx_max_color_value ? (gx_color_value) 0 :
		       gx_max_color_value - bcyan);
	    prgb[1] = (bmagenta > gx_max_color_value ? (gx_color_value) 0 :
		       gx_max_color_value - bmagenta);
	    prgb[2] = (byellow > gx_max_color_value ? (gx_color_value) 0 :
		       gx_max_color_value - byellow);

#			else

	    /* R = (1.0 - C) * (1.0 - K), etc. */

	    prgb[0] = (gx_color_value)((ulong) (gx_max_color_value - bcyan) *
									 (gx_max_color_value - black) / gx_max_color_value);
			prgb[1] = (gx_color_value)((ulong) (gx_max_color_value - bmagenta) *
									 (gx_max_color_value - black) / gx_max_color_value);
	    prgb[2] = (gx_color_value)((ulong) (gx_max_color_value - byellow) *
									 (gx_max_color_value - black) / gx_max_color_value);

#endif

		}
	}

	return (0);
}

#define gx_color_value_to_1bit(cv) ((cv) >> (gx_color_value_bits - 1))
#define red_weight 306
#define green_weight 601
#define blue_weight 117
#define gx_color_value_to_5bits(cv) ((cv) >> (gx_color_value_bits - 5))
#define gx_color_value_to_6bits(cv) ((cv) >> (gx_color_value_bits - 6))

/* gdev_pcl_map_rgb_color: 
----------------------------------------------------------------------------------*/
static gx_color_index gdev_pcl_map_rgb_color( gx_device * pdev,
					       const gx_color_value cv[])
{
        gx_color_value r, g, b;

	r = cv[0]; g = cv[1]; b = cv[2];
	if (gx_color_value_to_byte(r & g & b) == 0xff)
		return (gx_color_index) 0;	/* white */
	else {
		gx_color_value c = gx_max_color_value - r;
		gx_color_value m = gx_max_color_value - g;
		gx_color_value y = gx_max_color_value - b;

		switch (pdev->color_info.depth) {
	    case 1:
				return ((c | m | y) > gx_max_color_value / 2 ? (gx_color_index) 1 : (gx_color_index) 0);
				
	    case 8:
		    if (pdev->color_info.num_components >= 3)
					return (gx_color_value_to_1bit(c) 
											+ (gx_color_value_to_1bit(m) << 1) 
											+ (gx_color_value_to_1bit(y) << 2));
				else
					return ((((ulong) c * red_weight + (ulong) m * green_weight + (ulong) y * blue_weight)
											>> (gx_color_value_bits + 2)));
											
			case 16:
					return (gx_color_value_to_5bits(y) +
										(gx_color_value_to_6bits(m) << 5) +
										(gx_color_value_to_5bits(c) << 11));
	    case 24:
					return (gx_color_value_to_byte(y) +
										(gx_color_value_to_byte(m) << 8) +
										((ulong) gx_color_value_to_byte(c) << 16));
	    case 32: {
					return ((c == m && c == y) ? ((ulong) gx_color_value_to_byte(c) << 24)
										: 		(gx_color_value_to_byte(y) +
													 (gx_color_value_to_byte(m) << 8) +
													 ((ulong) gx_color_value_to_byte(c) << 16)));
	    }
		}
	}
	
	return ((gx_color_index) 0);	/* This never happens */
}

#define  gx_maxcol gx_color_value_from_byte(gx_color_value_to_byte(gx_max_color_value))

/* gdev_pcl_map_color_rgb: Map a color index to a r-g-b color.
----------------------------------------------------------------------------------*/
static int gdev_pcl_map_color_rgb(		gx_device 			*pdev, 
																			gx_color_index 	color,
																		 gx_color_value 	prgb[3])
{
	/* For the moment, we simply ignore any black correction */
	switch (pdev->color_info.depth) {
		case 1:
			prgb[0] = prgb[1] = prgb[2] = -((gx_color_value) color ^ 1);
			break;
			
		case 8:
			if (pdev->color_info.num_components >= 3) {
				gx_color_value c = (gx_color_value) color ^ 7;

				prgb[0] = -(c & 1);
				prgb[1] = -((c >> 1) & 1);
				prgb[2] = -(c >> 2);
			} else {
				gx_color_value value = (gx_color_value) color ^ 0xff;
				prgb[0] = prgb[1] = prgb[2] = (value << 8) + value;
			}
			break;
		case 16: {
	    gx_color_value c = (gx_color_value) color ^ 0xffff;
	    ushort value = c >> 11;

	    prgb[0] = ((value << 11) + (value << 6) + (value << 1) +
								 (value >> 4)) >> (16 - gx_color_value_bits);
	    value = (c >> 6) & 0x3f;
	    prgb[1] = ((value << 10) + (value << 4) + (value >> 2))
								>> (16 - gx_color_value_bits);
	    value = c & 0x1f;
	    prgb[2] = ((value << 11) + (value << 6) + (value << 1) +
								 (value >> 4)) >> (16 - gx_color_value_bits);
		}
		break;
		
		case 24: {
	    gx_color_value c = (gx_color_value) color ^ 0xffffff;

	    prgb[0] = gx_color_value_from_byte(c >> 16);
	    prgb[1] = gx_color_value_from_byte((c >> 8) & 0xff);
	    prgb[2] = gx_color_value_from_byte(c & 0xff);
		}
		break;
		
		case 32: {
	    gx_color_value w = gx_maxcol - gx_color_value_from_byte(color >> 24);

	    prgb[0] = w - gx_color_value_from_byte((color >> 16) & 0xff);
	    prgb[1] = w - gx_color_value_from_byte((color >> 8) & 0xff);
	    prgb[2] = w - gx_color_value_from_byte(color & 0xff);
		}
		break;
	}
	
	return (0);
}

#define save_ccomps save_info.num_components


/* cdj_put_param_bpp: new_bpp == save_bpp or new_bpp == 0 means don't change bpp.
 *   									ccomps == 0 means don't change number of color comps.
 *   									If new_bpp != 0, it must be the value of the BitsPerPixel element of
 *   									the plist; real_bpp may differ from new_bpp.
----------------------------------------------------------------------------------*/
static int cdj_put_param_bpp(	gx_device 		*pdev, 
																gs_param_list *plist, 
																int 					new_bpp,
																int 					real_bpp, 
																int 					ccomps)
{
	if (new_bpp == 0 && ccomps == 0)
		return gdev_prn_put_params(pdev, plist);
	else {
		gx_device_color_info save_info;
		int save_bpp;
		int code;

		save_info = pdev->color_info;
		save_bpp = save_info.depth;
		
		if (save_bpp == 8 && save_ccomps == 3 && !cprn_device->cmyk) 
			save_bpp = 3;
			
		code = cdj_set_bpp(pdev, real_bpp, ccomps);
		
		if (code < 0) {
	    param_signal_error(plist, "BitsPerPixel", code);
	    param_signal_error(plist, "ProcessColorModel", code);
	    return (code);
		}
		
		pdev->color_info.depth = new_bpp;	/* cdj_set_bpp maps 3/6 to 8 */
		code = gdev_prn_put_params(pdev, plist);
		
		if (code < 0) {
	    cdj_set_bpp(pdev, save_bpp, save_ccomps);
	    return (code);
		}
		
		cdj_set_bpp(pdev, real_bpp, ccomps);	/* reset depth if needed */
		if ((cdj970->color_info.depth != save_bpp 
								|| (ccomps != 0 && ccomps != save_ccomps))
								&& pdev->is_open)
	    return (gs_closedevice(pdev));
	    
	return (0);
	
#undef save_ccomps
	}
}


/* cdj970_write_header: 
----------------------------------------------------------------------------------*/
static int cdj970_write_header (gx_device *pdev, FILE * prn_stream)
{

	char  startbuffer[1260];
	
	memset (startbuffer, 0, 1260);

	sprintf (&(startbuffer[600]), "\033E\033%%-12345X@PJL JOB NAME = \"GHOST BY RENE HARSCH\"\n@PJL ENTER LANGUAGE=PCL3GUI\n"); 

	fwrite (startbuffer, sizeof(char), 678, prn_stream); 
	
	fputs("\033&l1H\033&l-2H", prn_stream); /* reverse engineering */
	
	/* enter duplex mode / reverse engineering */
	if (cdj970->duplex > NONE) {
		fputs("\033&l2S\033&b16WPML", prn_stream); 
	
		fputc (0x20, prn_stream);
		fputc (0x04, prn_stream);
		fputc (0x00, prn_stream);
		fputc (0x06, prn_stream);
		fputc (0x01, prn_stream);
		fputc (0x04, prn_stream);
		fputc (0x01, prn_stream);
		fputc (0x04, prn_stream);
		fputc (0x01, prn_stream);
		fputc (0x06, prn_stream);
		fputc (0x08, prn_stream);
		fputc (0x01, prn_stream);
		fputc (0x00, prn_stream);
	}

	return 0;
}

/* cdj970_write_trailer: 
----------------------------------------------------------------------------------*/
static int cdj970_write_trailer (gx_device *pdev, FILE * prn_stream)
{
	fprintf(prn_stream, "\033E\033%%-12345X");  /* reverse engineering */
	
	return 0;
}


/* cdj970_close: 
----------------------------------------------------------------------------------*/
static int cdj970_close(gx_device *pdev)
{	
		gx_device_printer *const ppdev = (gx_device_printer *)pdev;
		int retCode = gdev_prn_open_printer (pdev, true);
		
		if (retCode < 0)
				return (retCode);
		
		cdj970_write_trailer (pdev, ppdev->file);
		
		return gdev_prn_close(pdev);

}

