/*
 * Lexmark 3200 Color Jetprinter driver
 *
 * version 0.4.1
 *
 * Copyright 2000 by Daniel Gordini (dgordin@tin.it) 
 *
 * --------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * --------------------------------------------------------------------
 *
 * This driver is almost 100% original code but it is based
 * on protocol information partly discovered by Andrew Onifer III 
 * (http://www.mindspring.com/~aonifer) and Peter B. West 
 * (http://www.powerup.com.au/~pbwest) that were used as a starting 
 * base for the reverse-engineering of the protocol.
 *
 * Please mail me bug reports, comments and suggestions.
 *
 * Don't forget to read the release notes before installing !!
 *
 */

#include "gdevprn.h"
#include "gsparams.h"

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* Prototypes for public routines */
static dev_proc_map_rgb_color(lxm3200_map_rgb_color);
static dev_proc_map_color_rgb(lxm3200_map_color_rgb);
static dev_proc_print_page(lxm3200_print_page);
static dev_proc_get_params(lxm3200_get_params);
static dev_proc_put_params(lxm3200_put_params);
static dev_proc_open_device(lxm3200_open);

/* Prototypes for internal routines */
static void freeresources(gx_device *pdev);
static byte calccheck8(byte *data);
static void outputepilogue(void);
static void skiplines(int skipcol, int skipin);
static void fillheader(int head, int numcol, int firstcol, int bytes);
static void finalizeheader(int vskip, int newhead);
static void convbuf(int head, int numcols, int firstcol);
static void encode_bw_buf(void);
static void encode_col_buf(int head);
static int fill_mono_buffer(int vline);
static int init_buffer(void);
static int qualify_buffer(void);
static int roll_buffer(void);
static void calclinemargins(byte *data, int mask, int *left, int *right);
static void calcbufmargins(int head);
static void print_color_page(void);
static void print_mono_page(void);
static void print_photo_page(void);

/* Codes for the color indexes. */
#define WHITE        0x00  /* Pure white */
#define MAGENTA      0x01  /* Standard magenta */
#define CYAN         0x02  /* Standard cyan */
#define YELLOW       0x04  /* Standard yellow */
#define LIGHTCYAN    0x10  /* Light cyan (photo mode) */
#define LIGHTMAGENTA 0x20  /* Light magenta (photo mode) */
#define BLACK        0x40  /* Pure black */

/* Modes for rendering: used by the common procedures to
 * discriminate from mono to color and photo modes.
 */
#define LXM3200_M  0  /* Monochrome mode */
#define LXM3200_C  1  /* Standard color mode */
#define LXM3200_P  2  /* Photo color mode */

/* Initial horizontal position for the printheads, 
 * in 1200ths of an inch. Note that "left" and "right"
 * head here refers to paper margin, and so looking at 
 * them from the front of printer they will appear reversed.
 */

/* Left head (B&W/photo) start position */
#define LHSTART (gendata.leftoffset+6254)
/* added for Lexmark Z12 28.09.2002 */
#define LHSTART_z12 (gendata.leftoffset+5000) 

/* Right head (color) start position. This is relative to 
 * LHSTART so we only need to change one parameter to adjust
 * the head starting position. In the case of the Lexmark Z12
 * we have only one cartridge: black or color, so 
 * LHSTART_Z12 = RHSTART_Z12
 */
#define RHSTART (LHSTART-2120)
#define RHSTART_z12 (LHSTART_z12) /* added for Lexmark Z12 28.9.2002 */

/* Difference in starting position between left-to-right 
 * and right-to-left passes, in 1200ths of an inch.
 * Obviously only used in bidirectional mode.
 */
#define LRPASSHOFS 62

/* Initial vertical position of the printheads,
 * in 1200ths of an inch.
 */
#define BWTOPSTART  (gendata.topoffset+420)
#define COLTOPSTART (gendata.topoffset+476)

/* Base alignment offset between the color cartridge
 * and the B&W cartridge in 192 nozzles mode.
 */
#define COLORVALIGN_V  8
#define BLACKVALIGN_V  (gendata.vertalign+30)
#define PHOTOVALIGN_V  (gendata.vertalign)

/* Values used to index the vertical aligment array */
#define COLORVALIGN  0
#define BLACKVALIGN  1
#define PHOTOVALIGN  2

/* Offset of color pens from first row, in 1/600ths of an inch.
 * Pen 0 is the topmost and is the CYAN or LIGHTCYAN pen (depending
 * on the cartridge: standard color or photo). Pen 1 is the middle 
 * one, which carries MAGENTA or LIGHTMAGENTA color. Pen 2 is the 
 * bottom one, which is YELLOW or BLACK.
 */
#define PEN0OFS  0
#define PEN1OFS  92
#define PEN2OFS  184  

/* Number of nozzles in each pen type */
#define COLORPEN 64   /* Each color pen of a color/photo cartridge */
#define BWCOLPEN 192  /* Black cartridge used in color or 1200dpi modes */
#define BWSTDPEN 208  /* Black cartridge used in mono non-1200dpi mode */

/* Values used by the color mapping routines */
#define HALFTONE (gx_max_color_value/2)
#define FULLTONE (gx_max_color_value)
#define ONETHIRD (FULLTONE/3)
#define TWOTHIRD ((2*FULLTONE)/3)

/* Printer head and printing direction selectors */
#define LEFT   0 /* Left head (B/W or photo) or left-to-right direction */
#define RIGHT  1 /* Right head (colour) or right-to-left direction */

/* Qualificators for the scan buffer */
#define LAST   0x01 /* The buffer is the last on the page */
#define LHDATA 0x02 /* The buffer contains data for the left head */
#define RHDATA 0x04 /* The buffer contains data for the right head */

/* Printer's margins, in inches. The Lexmark 3200 has two settings 
 * for the side margins: one is used with A4-sized paper and one 
 * (here called conventionally "LETTER") is used for all other paper 
 * sizes. Envelopes have different margins as well, but under ghostscript
 * it's quite hard to know, from inside a printer driver, if we are
 * printing on envelopes or on standard paper, so we just ignore that.
 */
#define LXM3200_TOP_MARGIN           0.070
#define LXM3200_BOTTOM_MARGIN        0.200
#define LXM3200_A4_LEFT_MARGIN       0.135
#define LXM3200_LETTER_LEFT_MARGIN   0.250
#define LXM3200_A4_RIGHT_MARGIN      0.135
#define LXM3200_LETTER_RIGHT_MARGIN  0.250

/* Offsets for the top and bootom start of the printing frame. */
#define LXM3200_A4_TOPOFFSET       84
#define LXM3200_A4_LEFTOFFSET      162
#define LXM3200_LETTER_TOPOFFSET   84
#define LXM3200_LETTER_LEFTOFFSET  300


/* 
 * ------ The device descriptor ------ 
 */

/* Device procedures */
static gx_device_procs lxm3200_procs =
  prn_color_params_procs(lxm3200_open, gdev_prn_output_page, gdev_prn_close,
	  lxm3200_map_rgb_color, lxm3200_map_color_rgb, lxm3200_get_params, 
    lxm3200_put_params); 

/* Define an extension (subclass) of gx_device_printer. */
typedef struct lxm_device_s 
{ 
	gx_device_common;
	gx_prn_device_common;
	int rendermode;  /* Rendering mode (BW, CMYK, CcMmYK) */     
	int algnA; /* Horizontal alignment between left and right cartridges */
	int algnB; /* Vertical alignment between left and right cartridges */
	int algnC; /* Nozzle column separation of left cartridge */
	int algnD; /* Nozzle column separation of right cartridge */
	int bidir;   /* Bidirectional printing active ? */
	int numpass; /* Number of head passes for each stripe */
	int leftoffset; /* Offset of first column from left side of paper */
	int topoffset;  /* Offset of first row from top of paper */
        int model; /* Parameter to choose the model - lxm3200=0, z12=1, z31=2 */
        int z31m; /* Alignment parameter for the Z31 */
} lxm_device;

/* Device definition: Lexmark 3200 */
lxm_device far_data gs_lxm3200_device = 
{
	prn_device_body(lxm_device, 
		lxm3200_procs, 
		"lxm3200",
		DEFAULT_WIDTH_10THS, 
		DEFAULT_HEIGHT_10THS,
		600, /* default x dpi */
		600, /* default y dpi */
		0.0, /* left margin, inches (filled-in later) */
		0.0, /* bottom margin, inches (filled-in later) */
		0.0, /* right margin, inches (filled-in later) */
		0.0, /* top margin, inches (filled-in later) */
		1,   /* number of color components (mono) */
		8,   /* bits per pixel */
		1,   /* number of gray levels-1: B&W only */
		0,   /* number of color levels-1: no color */
		2,   /* dither gray: maximum 2 distinct gray levels */
		0,   /* dither rgb: no RGB dithering in this mode */
		lxm3200_print_page),
	LXM3200_C,      /* default printing mode */
	16, 8, 16, 16,  /* default aligment parameters value */
	0, 1,           /* default bidirectional and numpasses value */
	0, 0,           /* left and top offsets (filled-in later) */
        0,     /* default model = Lexmark 3200 */
        100   /* default z31m */
};

/* --------- Static data --------- */

/* Lookup tables to speed up bitwise operations */
static byte bits[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
static byte ibits[8] = { 0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe };

/* Lookup table for masking color pens in color/photo cartridges.
 * This is used to check the raster buffer for the presence of a 
 * particular color in the pixel we are encoding.
 * The first index is the head (LEFT or RIGHT) which is used to
 * distinguish between photo and color cartridges. The second index
 * is the pen number (0 is the upper pen, 1 the middle pen, 2 the
 * lower pen) on that cartridge.
 */
static byte colmask[2][3] =
{	
	{ LIGHTCYAN, LIGHTMAGENTA, BLACK},
	{ CYAN, MAGENTA, YELLOW }
};

/* Lookup table for pen position offsets of color/photo cartridges. 
 * Parameter is the pen number, as defined by the pen offsets above:
 * pen 0 is CYAN/LIGHTCYAN, pen 1 is MAGENTA/LIGHTMAGENTA, pen 2 is 
 * YELLOW/BLACK. This is used to properly take account the position
 * of each color pen relative to the vertical position of the 
 * color/photo cartridge.
 */
static int penofs[3];

/* Lookup table for vertical alignment of the cartridges relative to
 * each other. Parameter is the cartridge type: 0 = color cartridge,
 * 1 = black cartridge in color mode, 2 = photo cartridge.
 * Black cartridge in monochromatic mode is always aligned at 0
 * because in that mode we print with only one cartridge so there can
 * be no alignment problems (a single cartridge is always aligned
 * with itself, otherwise the printer tray is faulty).
 */
static int valign[3];

/* Lookup table for horizontal offsets. First parameter is the
 * head, second parameter the printing direction.
 */
static int hoffset[2][2];

/* Initialization sequence needed at the beginning of the data stream.
 * This is invariant and contains a reset sequence, meaning each single
 * page in a multiple page output is sent to the printer as an independent 
 * print job.
 */
static byte init_sequence[] =
{
	0x1b, 0x2a, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1b, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33,
	0x1b, 0x30, 0x80, 0x0C, 0x02, 0x00, 0x00, 0xbe,
	0x1b, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21
};              

static byte z12_init_sequence[] =
{
        0x1b, 0x2a, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x1b, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33,
        0x1b, 0x63, 0x00, 0x01, 0x40, 0x02, 0x0d, 0xb3,
        0x1b, 0x30, 0x80, 0x0c, 0x01, 0x00, 0x00, 0xbd,
        0x1b, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21
};

/* General global data that must be accessible
 * by all routines. 
 */
typedef struct pagedata_s
{
	/* General invariant data */
	int numbytes;    /* Number of bytes in a scanline of the buffer */
	int numrbytes;   /* Width (in bytes) of one rasterized scanline */
	int goffset;     /* Guard offset at each side of each scanline (columns) */ 
	int numblines;   /* Number of lines in a buffer */
	int numlines;    /* Number of lines in a vertical head pass */
	int rendermode;  /* Type of rendering */
	int numvlines;   /* Number of lines in the page */
	int numcols;     /* Number of columns in a row */
	int numpasses;   /* Number of passes used to print one stripe */
	int bidirprint;  /* Bidirectional printing enabled ? */
	int select;      /* Resolution selector */
        int modelprint;  /* which printer? - lxm3200=0, z12=1, z31=2 */
        int z31margin;   /* margin for the Z31 */

	/* Printing offsets */
	int leftoffset;  /* Start printing offset from left margin */
	int topoffset;   /* Start printing offset from top margin */

	/* Resolution settings */
	int xres;        /* Horizontal dots per inch */
	int yres;        /* Vertical dots per inch */
	int xrmul;       /* Horizontal coordinate multiplier */
	int yrmul;       /* Vertical coordinate multiplier */
	
	/* Pagewide status */
	int curheadpos;  /* Current absolute printhead position */
	int linetoeject; /* Number of lines for the eject command */
	int direction;   /* Printing direction for next stripe */

	/* Alignment data */
	int bwsep;     /* Nozzle columns separation in B&W/photo cartridge */
	int colsep;    /* Nozzle columns separation in color cartridge */
	int vertalign; /* Vertical alignment offset of the two cartridges */
	int lrhalign;  /* Horizontal alignment between left and right cartridges */

	/* Data pointers */
	byte *outdata;    /* Buffer to output data codes for one full stripe */
	byte *scanbuf;    /* Buffer to contain the rasterized scanlines */
	FILE *stream;     /* Output stream */
	lxm_device *dev;  /* Pointer to our device */

	/* Buffer data */
	int left, right;  /* Actual left and right margins */
	int firstline;    /* Head of the circular scanline buffer */
	int lastblack;    /* Line of last black pass rendered in a color print */
	int curvline;     /* Current vertical position */
	
	/* Stripe related data */
	byte header[24];  /* Stripe header data */
	int  fullflag;    /* A stripe is ready to be output */
	int  stripebytes; /* Number of bytes in a stripe */
	int  ileave;      /* Interleaving pass: 0=even lines, 1=odd lines */

} pagedata;

static pagedata gendata;

/* --------- Interface routines --------- */

/* Function called by ghostscript to open the
 * printer device. We set the margins and offsets
 * here. Note that to guess which paper is loaded
 * into the printer, we calculate the line width
 * and then anything between 8.25 and 8.4 inches
 * is considered to be A4.
 * This routine is inspired by the omologous 
 * routine from the "gdevbj10" driver.
 */
static int
lxm3200_open(gx_device *pdev)
{
	float linewidth;

	static const float a4_margins[4] =
	{      
		LXM3200_A4_LEFT_MARGIN, LXM3200_BOTTOM_MARGIN,
		LXM3200_A4_RIGHT_MARGIN, LXM3200_TOP_MARGIN
	};

	static const float letter_margins[4] =
	{
		LXM3200_LETTER_LEFT_MARGIN, LXM3200_BOTTOM_MARGIN,
		LXM3200_LETTER_RIGHT_MARGIN, LXM3200_TOP_MARGIN
	};

	linewidth = (float)(pdev->width) / (float)(pdev->x_pixels_per_inch);

	if(linewidth >= 8.25 && linewidth <= 8.4)
	{
		gx_device_set_margins(pdev, a4_margins, true);
		((lxm_device *)pdev)->topoffset = LXM3200_A4_TOPOFFSET;
		((lxm_device *)pdev)->leftoffset = LXM3200_A4_LEFTOFFSET;
	}
	else
	{
		gx_device_set_margins(pdev, letter_margins, true);
		((lxm_device *)pdev)->topoffset = LXM3200_LETTER_TOPOFFSET;
		((lxm_device *)pdev)->leftoffset = LXM3200_LETTER_LEFTOFFSET;
	}
	
	return gdev_prn_open(pdev);
}

/* Function used by ghostscript to map a RGB
 * value to the driver's internal representation
 * of the nearest color.
 */
static gx_color_index
lxm3200_map_rgb_color(gx_device *dev, const gx_color_value cv[])
{
	gx_color_index col;
	gx_color_value r, g, b;
	int c, m, y;
	gx_color_value tmpcv[3];
	
	r = cv[0]; g = cv[1]; b = cv[2];
	/* In case R, G and B values are equal, ghostscript
	 * prescribes that the color value must be turned 
	 * into a gray shade. In our case this means either
	 * black or white
	 */
	if(r == g && r == b)
	{
		if(r > HALFTONE)
			return(WHITE);
		else
			return(BLACK);
	}

	/* Calculate CMY values from RGB. This is *overly*
	 * simple, but it's enough to print something.
	 */
	c = FULLTONE - r;
	m = FULLTONE - g;
	y = FULLTONE - b;	

	/* Now encode the calculated color into the internal
	 * format. This means simply to turn on or off the
	 * bits representing each color depending on the value
	 * of the appropriate CMY component.
	 * Note that we are not doing black separation or any
	 * other fancy stuff: this is spartane code just to
	 * make the printer work. 
	 */
	col = 0;
	if(y > HALFTONE)col |= YELLOW;
	
	switch(((lxm_device *)dev)->rendermode)
	{
		case LXM3200_C:
			if(c > HALFTONE)col |= CYAN;
			if(m > HALFTONE)col |= MAGENTA;
			break;
				
		case LXM3200_P:
			if(c > TWOTHIRD)
				col |= CYAN;
			else
				if(c > ONETHIRD)col |= LIGHTCYAN;

			if(m > TWOTHIRD)
				col |= MAGENTA;
			else
				if(m > ONETHIRD)col |= LIGHTMAGENTA;
			break;

		default:
		        tmpcv[0] = r; tmpcv[1] = g; tmpcv[2] = b;
			col = gdev_prn_map_rgb_color(dev, tmpcv);
			break;
	}

	return(col);
}
 
/* Function called by ghostscript to map the
 * internal representation of a color to a
 * RGB value. 
 */
static int
lxm3200_map_color_rgb(gx_device *dev, gx_color_index color,
											gx_color_value prgb[3])
{
	int c, m, y;

	if(color == WHITE)
	{
		prgb[0] = FULLTONE; 
		prgb[1] = FULLTONE; 
		prgb[2] = FULLTONE;
		return(0);
	}
	
	if(color & BLACK)
	{
		prgb[0] = 0; 
		prgb[1] = 0; 
		prgb[2] = 0;
		return(0);
	}

	/* Calculate back CMY components from the internal
	 * representation of the color
	 */
	c = 0;
	m = 0;
	y = 0;
	
	switch(((lxm_device *)dev)->rendermode)
	{
		case LXM3200_C:
			color &= (CYAN|MAGENTA|YELLOW);
			if(color & CYAN)c = 2;
			if(color & MAGENTA)m = 2;
			if(color & YELLOW)y = 2;
			break;
			
		case LXM3200_P:
		default:
			color &= (CYAN|MAGENTA|YELLOW|LIGHTCYAN|LIGHTMAGENTA);
			if(color & LIGHTCYAN)c = 1;
			if(color & LIGHTMAGENTA)m = 1;
			if(color & YELLOW)y = 2;
			if(color & CYAN)c = 2;
			if(color & MAGENTA)m = 2;
			break;
	}

	/* And now turn CMY to RGB, in the usual spartane way */

	prgb[0] = (gx_color_value)((2 - c) * HALFTONE);
	prgb[1] = (gx_color_value)((2 - m) * HALFTONE);
	prgb[2] = (gx_color_value)((2 - y) * HALFTONE);

	return(0);
}

/* Main routine of the driver. This takes care of
 * all parameters and static data initialization 
 * and calls the proper page printing routines
 * depending on the selected printing mode. 
 */
static int 
lxm3200_print_page(gx_device_printer *pdev, FILE *prn_stream)
{
	/* Store data passed by ghostscript to the driver */
	gendata.dev = (lxm_device *)pdev;
	gendata.stream = prn_stream;
	gendata.rendermode = (gendata.dev)->rendermode;
		
	/* Snap resolution on one of the three supported setting
	 * (300, 600, 1200 dpi) depending on the input resoution value.
	 * Horizontal and vertical resolution are treated independently. 
   */
	gendata.xres = 600;
	if((gendata.dev)->x_pixels_per_inch < 450)gendata.xres = 300;
	if((gendata.dev)->x_pixels_per_inch > 900)gendata.xres = 1200;
	gendata.xrmul = 1200 / gendata.xres;

	gendata.yres = 600;
	if((gendata.dev)->y_pixels_per_inch < 450)gendata.yres = 300;
	if((gendata.dev)->y_pixels_per_inch > 900)gendata.yres = 1200;
	gendata.yrmul = 1200 / gendata.yres;

	/* Cache horizontal and vertical starting offsets */
	gendata.topoffset = (gendata.dev)->topoffset;
	gendata.leftoffset = (gendata.dev)->leftoffset;

	/* Build lookup table for pen offset, adjusting for
	 * vertical resolution setting 
	 */
	penofs[0] = (PEN0OFS * 2) / gendata.yrmul;
	penofs[1] = (PEN1OFS * 2) / gendata.yrmul;
	penofs[2] = (PEN2OFS * 2) / gendata.yrmul;
	
	/* Build lookup table for vertical heads alignment,
	 * adjusting for vertical resolution setting
	 */
	valign[COLORVALIGN] = (COLORVALIGN_V * 2) / gendata.yrmul;
	valign[BLACKVALIGN] = (BLACKVALIGN_V * 2) / gendata.yrmul;
	valign[PHOTOVALIGN] = (PHOTOVALIGN_V * 2) / gendata.yrmul;
      
	/* Build lookup tables for initial horizontal offsets,
	 * adjusting for horizontal resolution setting
	 */
	 /* choose whether to use lxm3200 or Z12 settings */
      gendata.modelprint=(gendata.dev)->model; /* which model? */
      gendata.z31margin=(gendata.dev)->z31m; /*which additional margin for z31*/
      switch(gendata.modelprint){
      case 1:  /* we use the Lexmark Z12 */
          hoffset[LEFT][LEFT] = LHSTART_z12;
	  hoffset[RIGHT][LEFT] = RHSTART_z12 + gendata.lrhalign;
	  break;
      default: /* default (if one uses the Lexmark 3200 or the Lexmark Z31) */
          hoffset[LEFT][LEFT] = LHSTART;
          hoffset[RIGHT][LEFT] = RHSTART + gendata.lrhalign;
          break;
      }
      hoffset[LEFT][RIGHT] = hoffset[LEFT][LEFT] - LRPASSHOFS; 
      hoffset[RIGHT][RIGHT] = hoffset[RIGHT][LEFT] - LRPASSHOFS;

	/* Initialization of general parameters */
	gendata.outdata = NULL;
	gendata.scanbuf = NULL;
	gendata.curheadpos = 0;
	gendata.left = 0;
	gendata.right = 0;
	gendata.lastblack = 0;
	gendata.curvline = 0;
	gendata.firstline = 0;
	gendata.fullflag = FALSE;
	gendata.direction = LEFT;
	gendata.ileave = 0;

	gendata.bidirprint = (gendata.dev)->bidir;
	gendata.numpasses = (gendata.dev)->numpass;


	/* Set some parameters that depend on resolution and 
	 * printing mode. We calculate all at 600dpi (the native
	 * resolution) and then correct later for different
	 * resolution settings.
	 */
	switch(gendata.rendermode)
	{
		/* In monochrome mode we try to use all 208 nozzles of
		 * the black cartridge to speed up printing. But if we
		 * are printing at 1200 dpi horizontal, only 192 nozzles
		 * are available anyway (it seems an hardware limitation).
		 * We print a full buffer at every pass, so the number of
		 * lines in the buffer is the same as the number of nozzles 
		 * of the head. 
		 */
		case LXM3200_M:
			gendata.numblines = 208;
			gendata.numlines = 208;
			gendata.select = 0x10;
			if(gendata.xres == 1200)
			{
				gendata.numblines = 192;
				gendata.numlines = 192;
				gendata.select = 0x00;
			}
			break;

		/* In color or photo mode we must use 192 nozzles only in
		 * the black cartridge, to cope with the color and photo
		 * cartridges (which have 3 color pen of 64 nozzles each,
		 * for a total of 192 nozzles). But the color pens are 
		 * vertically spaced and misaligned with respect to the
		 * black pen. To solve this problem, we need a buffer which
		 * is larger than 192 lines and then we print only the 
		 * proper "windows" from it. We choose to set the buffer
		 * height to 256, which is the smallest power of two large
		 * enough to hold all the needed data. We use a power of
		 * two for speed, since in this way the modulo operation
		 * in the inner loops (needed to take care of buffer rolling) 
		 * becomes a simple and much faster bitwise AND.
		 */
		case LXM3200_P:
		case LXM3200_C:
			gendata.numblines = 256;
			gendata.numlines = 192;
			gendata.select = 0x00;
			break;
	}

	/* Correct the number of lines of the buffer to take care
	 * of different vertical resolution modes. Since the buffer
	 * does cover a constant vertical spacing, we must double the
	 * number of lines at 1200dpi and half it at 300dpi, to take
	 * into account the different thickness of the lines at the
	 * three different vertical resolutions.
	 */
	gendata.numblines = (gendata.numblines * 2) / gendata.yrmul;

	/* Now correct the "select" field to adjust the horizontal
	 * motor speed depending on position. Meanwhile, if we are
	 * at 1200 dpi, double the number of horizontal passes
	 * because each stripe at 1200 dpi horizontal must be printed
	 * in two passes.
	 */
	switch(gendata.xres)
	{
		case 300:
			gendata.select |= 0x60;
			break;

		case 1200:
			gendata.select |= 0x40;
			gendata.numpasses *= 2;
			break;
	}

	/* Now store some useful info taken from the ghostscript
	 * device structure to speed up access.
	 */
	gendata.numcols = (gendata.dev)->width;
	gendata.numvlines = (gendata.dev)->height;
	gendata.lrhalign = (gendata.dev)->algnA;
	gendata.vertalign = (gendata.dev)->algnB;
	gendata.bwsep = (gendata.dev)->algnC;
	gendata.colsep = (gendata.dev)->algnD;
	gendata.goffset = (max(gendata.bwsep, gendata.colsep) * 2) / gendata.xrmul;
	gendata.numbytes = gendata.numcols + (2 * gendata.goffset);
	gendata.numrbytes = gdev_mem_bytes_per_scan_line(gendata.dev);

	/* Calculate number of lines in the page and initialize the
	 * counter of the lines to eject. At the end of the printing,
	 * to eject the paper sheet we must send to the printer a
	 * command to move the paper forward. The amount to move is
	 * the length of paper which is still inside the printer plus 
	 * two inches (the number is expressed in 1200ths of an inch, 
	 * so "plus two inches" means "add 2400").
	 */
	gendata.linetoeject = gendata.numvlines * gendata.yrmul;
	gendata.linetoeject += 2400;

	/* Allocate memory for the buffers and
	 * verify that the allocation was done properly.
	 */
	gendata.scanbuf = (byte *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), gendata.numbytes, gendata.numblines, 
																			"lxm3200_print_page(scanbuf)");

	gendata.outdata = (byte *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), gendata.numbytes, 30, 
																			"lxm3200_print_page(outdata)");
 
	if(gendata.scanbuf == NULL || 
		 gendata.outdata == NULL)
	{
		freeresources(pdev);
		return_error(gs_error_VMerror);
	}

	/* Send initialization sequence to the printer */
	if(gendata.modelprint==1) fwrite(z12_init_sequence, sizeof(z12_init_sequence), 1, prn_stream);
	else fwrite(init_sequence, sizeof(init_sequence), 1, prn_stream);

	/* Choose the right page printing routine 
	 * depending on the printing mode.
	 */  
	switch(gendata.rendermode)
	{
		case LXM3200_P:
			print_photo_page();
			break;

		case LXM3200_C:
			print_color_page();
			break;

		case LXM3200_M:
		default:
			print_mono_page();
			break;
	}

	/* Output the end-of-page epilogue */
	outputepilogue();

	/* Free the allocated resources */
	freeresources(pdev);

	/* Done. Bye bye, see you on next page. */
	return(0);
}

/* Function that Ghostscript calls to ask the driver
 * the value of its parameters. This function is based 
 * on the equivalent from the HP850 driver (gdevcd8.c)
 * by Uli Wortmann.
 * I won't comment it because I haven't even tried
 * to understand this code... :)
 */
static int
lxm3200_get_params(gx_device *pdev, gs_param_list *plist)
{
  int code;
 
  code = gdev_prn_get_params(pdev, plist);

  if(code < 0)return(code);

	code = param_write_int(plist, "algnA", &((lxm_device *)pdev)->algnA);
	if(code < 0)return(code);

	code = param_write_int(plist, "algnB", &((lxm_device *)pdev)->algnB);
	if(code < 0)return(code);

	code = param_write_int(plist, "algnC", &((lxm_device *)pdev)->algnC);
	if(code < 0)return(code);

	code = param_write_int(plist, "algnD", &((lxm_device *)pdev)->algnD);
	if(code < 0)return(code);

	code = param_write_int(plist, "bidir", &((lxm_device *)pdev)->bidir);
	if(code < 0)return(code);

	code = param_write_int(plist, "numpass", &((lxm_device *)pdev)->numpass);
	if(code < 0)return(code);

	code = param_write_int(plist, "mode", &((lxm_device *)pdev)->rendermode);
        if(code < 0)return(code);

        code = param_write_int(plist, "model", &((lxm_device *)pdev)->model);
        if(code < 0)return(code);

	code = param_write_int(plist, "z31m", &((lxm_device *)pdev)->z31m);

  return code;
}

/* Function that Ghostscript calls to let the driver
 * set the value of its parameters. This function is 
 * based on the equivalent from the HP850 driver 
 * (gdevcd8.c) by Uli Wortmann.
 * I won't comment it because I haven't even tried
 * to understand this code... :)
 */
static int
lxm3200_put_params(gx_device *pdev, gs_param_list *plist)
{       
  int algnA = ((lxm_device *)pdev)->algnA;
  int algnB = ((lxm_device *)pdev)->algnB;
  int algnC = ((lxm_device *)pdev)->algnC;
  int algnD = ((lxm_device *)pdev)->algnD;
  int bidir = ((lxm_device *)pdev)->bidir;
  int numpass = ((lxm_device *)pdev)->numpass;
  int mode = ((lxm_device *)pdev)->rendermode;
  int code = 0;
  int model = ((lxm_device *)pdev)->model; /* for model selection */
  int z31m = ((lxm_device *)pdev)->z31m; /* additional margin for the z31 */

  code = param_read_int(plist, "algnA", &algnA);
	if(code < 0)return(code);
	if(algnA < 0 || algnA > 30)
		param_signal_error(plist, "algnA", gs_error_rangecheck);

  code = param_read_int(plist, "algnB", &algnB);
	if(code < 0)return(code);
	if(algnB < 0 || algnB > 15)
		param_signal_error(plist, "algnB", gs_error_rangecheck);

  code = param_read_int(plist, "algnC", &algnC);
	if(code < 0)return(code);
	if(algnC < 0 || algnC > 30)
		param_signal_error(plist, "algnC", gs_error_rangecheck);

  code = param_read_int(plist, "algnD", &algnD);
	if(code < 0)return(code);
	if(algnD < 0 || algnD > 30)
		param_signal_error(plist, "algnD", gs_error_rangecheck);

  code = param_read_int(plist, "bidir", &bidir);
	if(code < 0)return(code);
	if(bidir != 0 && bidir != 1)
		param_signal_error(plist, "bidir", gs_error_rangecheck);

  code = param_read_int(plist, "numpass", &numpass);
	if(code < 0)return(code);
	if(numpass < 1 || numpass > 16)
		param_signal_error(plist, "numpass", gs_error_rangecheck);

  code = param_read_int(plist, "mode", &mode);
	if(code < 0)return(code);
	if(mode != LXM3200_M && mode != LXM3200_C && mode != LXM3200_P)
		param_signal_error(plist, "mode", gs_error_rangecheck);

   code = param_read_int(plist, "model", &model); /* asking for the model of printer: lxm3200 , Z12, Z31 */
	if(code < 0)return(code);
	if(model < 0 || model > 2 )
		param_signal_error(plist, "model", gs_error_rangecheck);

   code = param_read_int(plist, "z31m", &z31m); /* What additional margin for the Z31 */
	if(code < 0)return(code);

   code = gdev_prn_put_params(pdev, plist);    
        if(code < 0)return code;

  ((lxm_device *)pdev)->algnA = algnA;
  ((lxm_device *)pdev)->algnB = algnB;
  ((lxm_device *)pdev)->algnC = algnC;
  ((lxm_device *)pdev)->algnD = algnD;
  ((lxm_device *)pdev)->bidir = bidir;
  ((lxm_device *)pdev)->numpass = numpass;
  ((lxm_device *)pdev)->rendermode = mode;
  ((lxm_device *)pdev)->model = model; /* Model selection: lxm3200, Z12, Z31. */
  ((lxm_device *)pdev)->z31m = z31m;  /* Additional margin for the Z31 */

	/* Depending on the selected rendering mode, change the
	 * driver's parameters that ghostscript needs for the
	 * dithering. We need to do it here because the "get_params"
	 * and "put_params" are the only routines in the driver that
	 * ghostscript calls before using the dithering parameters.
	 */
	switch(mode)
	{
		case LXM3200_M:
			pdev->color_info.num_components = 1;
			pdev->color_info.max_gray = 1;
			pdev->color_info.max_color = 0;
			pdev->color_info.dither_grays = 2;
			pdev->color_info.dither_colors = 0;
			break;

		case LXM3200_C:
			pdev->color_info.num_components = 3;
			pdev->color_info.max_gray = 1;
			pdev->color_info.max_color = 1;
			pdev->color_info.dither_grays = 2;
			pdev->color_info.dither_colors = 2;
			break;

		case LXM3200_P:
			pdev->color_info.num_components = 3;
			pdev->color_info.max_gray = 1;
			pdev->color_info.max_color = 2;
			pdev->color_info.dither_grays = 2;
			pdev->color_info.dither_colors = 3;
			break;
	}

  return 0;
}


/* --------- Internal routines --------- */

/* Free the resources allocated by the driver */
static void
freeresources(gx_device *pdev)
{
	if(gendata.scanbuf)
		gs_free(gs_lib_ctx_get_non_gc_memory_t(), (char *)gendata.scanbuf, gendata.numbytes, gendata.numblines,
						"lxm3200:freeresources(scanbuf)");

	if(gendata.outdata)
		gs_free(gs_lib_ctx_get_non_gc_memory_t(), (char *)gendata.outdata, gendata.numbytes, 30, 
						"lxm3200:freeresources(outdata)");
}

/* Calculate the checksum of an escape sequence.
 * It is defined as the sum modulo 256 of the 
 * six bytes following the escape character.
 *
 * data: pointer to the first of the 8 characters
 *       of an escape sequence.
 */
static byte 
calccheck8(byte *data)
{
	int ck, i;
	
	ck = 0;
	for(i=1; i<7; i++)ck += data[i];
	
	return(ck);
}

/* Output the page epilogue. This procedure outputs
 * the escape sequence needed to eject the page and
 * take the printheads to the "park" position.
 */
static void 
outputepilogue(void)
{
	byte trailer[24];
	int pos;
	
	/* Page eject sequence */
	trailer[0] = 0x1b;
	trailer[1] = 0x22;
	trailer[2] = 0x80;
	trailer[3] = gendata.linetoeject >> 8;
	trailer[4] = gendata.linetoeject & 0xff;
	trailer[5] = 0x00;
	trailer[6] = 0x00;
	trailer[7] = calccheck8(trailer);

	/* Calculate the value we need to take the head back
	 * to the park position. This is the current head position
	 * if we printed the last stripe left-to-right, and the
	 * current head position minus 168 (0xa8) if we printed the
	 * last stripe right-to-left.
	 */
	pos = gendata.curheadpos;
	if(gendata.bidirprint && gendata.direction == LEFT)pos -= 0xa8;
	if(pos < 0)pos = 0;

	/* Horizontal back sequence */
	trailer[8] = 0x1b;
	trailer[9] = 0x31;
	trailer[10] = 0x10;
	trailer[11] = pos >> 8;
	trailer[12] = pos & 0xff;
	trailer[13] = 0x00;
	trailer[14] = 0x00;
	trailer[15] = calccheck8(trailer+8);
        
	/* Reset sequence */
	trailer[16] = 0x1b;
	trailer[17] = 0x33;
	trailer[18] = 0x00;
	trailer[19] = 0x00;
	trailer[20] = 0x00;
	trailer[21] = 0x00;
	trailer[22] = 0x00;
	trailer[23] = 0x33;
	
	fwrite(trailer, 8, 3, gendata.stream);
}

/* Output a "page forward" escape sequence,
 * needed to move the paper forward some lines.
 *
 * skiprow: number of buffer lines to skip
 * vskip  : fixed offset, in 1200ths of an inch
 */
static void
skiplines(int skiprow, int skipin)
{
	byte escape[8];
	int vskip;

	/* The vertical skip command accepts a spacing expressed in 
	 * 1200ths of an inch, so we must convert lines to 1200ths 
	 * of an inch. After the conversion we sum an offset directly 
	 * expressed in 1200ths of an inch: this way we can use this
	 * routine to skip both a certain amount of lines (which exact
	 * spacing value depends on the vertical resolution) and a 
	 * fixed offset that we directly know in spacing units.
	 */ 
	vskip = skiprow*gendata.yrmul + skipin;
	
	escape[0] = 0x1b;
	escape[1] = 0x23;
	escape[2] = 0x80;
	escape[3] = vskip >> 8;
	escape[4] = vskip & 0xff;
	escape[5] = 0x00;
	escape[6] = 0x00;
	escape[7] = calccheck8(escape);

	/* Adjust the number of lines still inside the printer */
	gendata.linetoeject -= vskip;
	
	fwrite(escape, 8, 1, gendata.stream);
}       

/* Fill a stripe header with data.
 *
 * head:     LEFT or RIGHT, chooses which head we are "feeding"
 * numcol:   number of data columns in this stripe, including blank ones
 * firstcol: position of the first column in the stripe (the *very first*,
 *           even if it's blank).
 * bytes:    total number of bytes in the stripe, including directories
 *           (but excluding the 24 bytes of the header).
 */
static void 
fillheader(int head, int numcol, int firstcol, int bytes)
{
	int len, offs1, startabs;
	int endabs, select, fwd;
	int back, nabspos, sep;
	byte *header;

	header = gendata.header;
	
	/* Correct the measures: firstcol and len need to 
	 * be in 1200ths of an inch.
	 */
	firstcol *= gendata.xrmul;
	len = numcol * gendata.xrmul;
	
	/* Alter select to choose direction */
	select = gendata.select | (gendata.direction == LEFT ? 0x01 : 0x00);

	/* Calculate the proper horizontal offset */
	offs1 = hoffset[head][gendata.direction];

	/* Now calculate the correct separation depending on the
	 * head type and adjust "select" to choose between left
	 * or right head.
	 */
	if(head == LEFT)
	{
		sep = (gendata.bwsep * 2) / gendata.xrmul;
	}
	else
	{
		sep = (gendata.colsep * 2) / gendata.xrmul;		
		select |= 0x80;
	}

	/* Now calculate absolute starting and ending positions
	 * of this stripe, taking into account the printing direction
	 */
	startabs = firstcol + offs1;

	if(gendata.direction == LEFT)
		endabs = startabs + len;
	else
		endabs = startabs - len;

	/* And now, basing on current head position,
	 * transform the absolute coordinates in a
	 * relative movement of the head.
	 * The formulas used for this are "black magic",
	 * since this is a part of the protocol which is
	 * still not well known. What you see here is an
	 * empyrical formula devised by examination and 
	 * parameter fitting on the data output by the
	 * Windows driver.
	 */
	if(gendata.direction == LEFT)
	{
		nabspos = (((endabs - 3600) >> 3) & 0xfff0) + 9;
		fwd = nabspos - gendata.curheadpos;
	}
	else
	{
		if(endabs > 4816)
			nabspos = (((endabs - 4800) >> 3) & 0xfff0) + 9;
		else
			nabspos = (((endabs - 3600) >> 3) & 0xfff0) + 9;
		fwd = gendata.curheadpos - nabspos;
	}

	gendata.curheadpos += (gendata.direction == LEFT ? fwd : -fwd);

	/* If we are printing unidirectionally, calculate
	 * the backward movement to return the printing head
	 * at the beginning of this stripe.
	 */
	back = 0;
	if(gendata.bidirprint == FALSE)
	{
		if(startabs > 4816)
			nabspos = ((startabs - 4800) >> 3) & 0xfff0;
		else
			nabspos = ((startabs - 3600) >> 3) & 0xfff0;	 

		if(gendata.direction == LEFT)
			back = gendata.curheadpos - nabspos;
		else
			back = nabspos - gendata.curheadpos;
	}

	gendata.curheadpos -= (gendata.direction == LEFT ? back : -back);

	/* First part of the header */
	header[0] = 0x1b;
	header[1] = 0x40;
	header[2] = select;        /* Printing type flags */
	header[3] = numcol >> 8;   /* MSB of the number of columns to print */
	header[4] = numcol & 0xff; /* LSB of the number of columns to print */
	header[5] = fwd >> 8;      /* MSB of the relative forward head motion */
	header[6] = fwd & 0xff;    /* LSB of the relative forward head motion */
	header[7] = calccheck8(&header[0]);
	
	/* Second part of the header */
	header[8] = 0x1b;
	header[9] = 0x42;
	header[10] = 0x00;
	if(gendata.modelprint==1) header[10] = 0x10; /* Lexmark Z12 protocol */
	header[11] = back >> 8;  /* MSB of the relative backward head motion */
	header[12] = back & 0xff;  /* LSB of the relative backward head motion */
	header[13] = 0x00;  /* MSB of the relative downward head motion */
	header[14] = 0x00;  /* LSB of the relative downward head motion */
	header[15] = calccheck8(&header[8]);
	
	/* Third (and last) part of the header */
	header[16] = 0x1b;
	header[17] = 0x43;
	header[18] = (bytes >> 16) & 0xff;
	header[19] = (bytes >> 8) & 0xff; 
	header[20] = bytes & 0xff;    /* LSB of the number of bytes in this stripe */
	header[21] = startabs >> 8;   /* MSB of the starting column of this stripe */
	header[22] = startabs & 0xff; /* LSB of the starting column of this stripe */
	header[23] = calccheck8(&header[16]);

	/* Signal to other routines that the output buffer
	 * is full and how many bytes it is long.
	 */
	gendata.stripebytes = bytes;
	gendata.fullflag = TRUE;

	/* If bidirectional printing is in effect, change 
	 * the printing direction for the next stripe
	 */
	if(gendata.bidirprint)
		gendata.direction = (gendata.direction == LEFT ? RIGHT : LEFT);
}

/* Set final information in the header and output all 
 * the data passes. This routine is needed because the
 * actual values of two fields of the header (final
 * head position and number of vertical lines to reach
 * the next stripe) depend on the next stripe and are
 * therefore unknown when "fillheader" is called.
 *
 * vskip  : number of lines to skip to reach next stripe
 * newhead: head used for the next stripe (LEFT or RIGHT)
 */
static void 
finalizeheader(int vskip, int newhead)
{
	int offs2, nstartabs, back, fwd;
	int habs, p, dir, endabs, col;
	int newstart, sep;
	byte *header;

	header = gendata.header;

	/* Check the printing direction this stripe
	 * was originally intended for.
	 */
	dir = (header[2] & 0x01 ? LEFT : RIGHT);

	/* Retrieve the horizontal offset for the next stripe */
	offs2 = hoffset[newhead][gendata.direction];

	/* Calculate the separation adjust in 1200ths of an inch */
	if(newhead == LEFT)
		sep = (gendata.bwsep * 2) / gendata.xrmul;
	else
		sep = (gendata.colsep * 2) / gendata.xrmul;

	/* Now calculate the correct starting column
	 * of the next stripe
	 */
	if(gendata.direction == LEFT)
		newstart = (gendata.left * gendata.xrmul) - sep;
	else
		newstart = (gendata.right * gendata.xrmul);
	
	vskip *= gendata.yrmul;
	
	/* Calculate absolute starting position of new stripe */
	nstartabs = newstart + offs2;
	
	/* Calculate absolute ending position of this stripe 
	 * by summing (with proper sign) the starting position
	 * and the width.
	 */
	endabs = header[21]*256 + header[22]; /* Starting position */
	col = (header[3]*256 + header[4]); /* Width in columns */
	col *= gendata.xrmul; /* Transformed in 1200ths of an inch */

	if(dir == LEFT)
		endabs += col; /* Printing left-to-right */
	else
		endabs -= col; /* Printing right-to-left */

	/* Correct head position neutralizing the effect
	 * of the last issued head movement commands. The
	 * head movement for this stripe needs to be
	 * recalculated from scratch to take into account 	 
	 * the correct beginning position of the next stripe.
	 */
	if(dir == LEFT)
	{
		gendata.curheadpos += header[11]*256 + header[12]; /* Back movement */
		gendata.curheadpos -= header[5]*256 + header[6];   /* Forward movement */
	}
	else
	{
		gendata.curheadpos -= header[11]*256 + header[12]; /* Back movement */
		gendata.curheadpos += header[5]*256 + header[6];   /* Forward movement */
	}
	
	/* We use a convention of passing a negative value for
	 * "newhead" to mean that this is the last stripe of the
	 * sheet, so we don't need to care for the next stripe.
	 */
	if(newhead < 0)
	{
		/* Last stripe: we only need to calculate proper forward 
		 * motion to print all the current stripe.
		 */
		fwd = ((header[5]*256 + header[6]) & 0xfff0) + 9;
	}
	else
	{
		/* This is not the last stripe, so we need to calculate
		 * the forward (in the printing direction) movement
		 * that will take the printing head at the end of the
		 * current stripe or at the beginning of the next, 
		 * whichever is farther. Note that we are always 
		 * talking relative to printing direction, so we must
		 * take into proper account if we are printing from left
		 * to right or from right to left.
		 */
		if(dir == LEFT)
		{
			p = max(endabs, nstartabs);
			habs = (((p - 3600) >> 3) & 0xfff0) + 9;
			fwd = habs - gendata.curheadpos;

			/* part for the Lexmark Z31!!! */
                 	if(gendata.modelprint==2) fwd += gendata.z31margin;

		}
		else
		{
			p = min(endabs, nstartabs);
			if(p > 4816)
				habs = (((p - 4800) >> 3) & 0xfff0);
			else
				habs = (((p - 3600) >> 3) & 0xfff0);			
			fwd = gendata.curheadpos - habs;
		}
	}
	
	/* Now update the current head position to take into 
	 * account the forward movement just computed
	 */
	gendata.curheadpos += (dir == LEFT ? fwd : -fwd);

	/* Now calculate the value of the needed backward movement 
	 * to poisition the head correctly for the start of the
	 * next stripe.
	 */
	if(newhead < 0 || gendata.bidirprint)
	{
		/* If this is the last stripe of the page,
		 * there is no need to take back the head:
		 * it will be done by the parking command.
		 * Or if we are printing bidirectionally,
		 * the forward command has taken the head to
		 * the correct position, so no need to move it.
		 */
		back = 0;
	}
	else
	{
		/* Calculate the right backward movement basing 
		 * on the start of the next stripe.
		 */
		if(nstartabs > 4856)
			habs = ((nstartabs - 4840) >> 3) & 0xfff0;
		else
			habs = ((nstartabs - 3600) >> 3) & 0xfff0;			
		
		back = gendata.curheadpos - habs;

		/* If the next stripe starts at the right 
		 * of this one, "back" will be too small or
		 * negative, so correct it. 
		 * It appears that 16 is the minimum allowable
		 * backward movement that does not make the 3200
		 * misbehave in the next stripe. This does not hold
		 * if we are changing printing direction (in such a
		 * case backward movement may be zero). This means
		 * we are moving the head a little more than needed,
		 * but it seems unavoidable.
		 */
		if(back < 16)back = 16;
	}

  /* Lastly, update the current head position with the
	 * backward movement just calculated.
	 */
	gendata.curheadpos -= (dir == LEFT ? back : -back);

	/* Modify first part of the header */
	header[5] = fwd >> 8;
	header[6] = fwd & 0xff;
	header[7] = calccheck8(&header[0]);

	/* Modify second part of the header */
	header[8] = 0x1b;
	header[9] = 0x42;
	header[10] = 0x00;
	if(gendata.modelprint==1) header[10] = 0x10; /* Lexmark Z12 protocol */
	header[11] = back >> 8;    /* MSB of the relative backward head motion */
	header[12] = back & 0xff;  /* LSB of the relative backward head motion */
	header[13] = vskip >> 8;   /* MSB of the relative downward head motion */
	header[14] = vskip & 0xff; /* LSB of the relative downward head motion */
	header[15] = calccheck8(&header[8]);

	/* Now output the data, signalling that the output
	 * buffer is now empty.
	 */
	fwrite(header, 3, 8, gendata.stream);
	fwrite(gendata.outdata, gendata.stripebytes, 1, gendata.stream);
	gendata.fullflag = FALSE;
}

/* Convert a buffer data stream into 
 * directory/data representation, using the
 * shortest coding (either direct or RLE)
 *
 * head    : head we are printing with (left or right).
 * numcols : number of columns in the buffer.
 * firstcol: first column to print.
 */
static void
convbuf(int head, int numcols, int firstcol)
{
	byte *read, *write;
	int x, i, c, p, q, cnt, rle, std;
	int nby, ofs, dts0, dtr0, dtr1;
	int bytes;

	/* Initialize the pointers. We use the same buffer 
	 * for both input and output (we can do it because 
	 * the output data is at most as long as the input).
	 * Note that the encode routines skipped 4 bytes at
	 * each column to make room for the directory word.
	 */
	read = gendata.outdata + 4;
	write = gendata.outdata;

	/* Set the parameters that will be used to create the directory and 
	 * to access the data. These parameters define the structure of the 
	 * directory word (32 bit) and depend on the number of nozzles that 
	 * are used. Note that the directory bitfield is initialized to all 
	 * ones (but read below for further info) because it works in negative 
	 * logic (i.e. a change is marked by a zero bit).
	 * Below, nby is the number of bytes needed in the input data to map 
	 * a column (each nozzle is 1 bit, so 208 nozzles are 26 bytes and 
	 * 192 nozzles are 24 bytes). Ofs is the number of the first bit of 
	 * the directory bitfield in the directory word (with 208 nozzles we 
	 * need 26 bits, from 6 to 31, with 192 nozzles we need 24 bits, from 
	 * 8 to 31). The other three parameters are the values needed to 
	 * initialize the directory word properly: the key is the first two 
	 * bits, which must be "10" for a directly encoded stripe and "01" for 
	 * a RLE encoded one. In the lexmark directory, each bit represents a 
	 * group of 8 nozzles: in a directly encoded stripe if the bit is "1" 
	 * it means none of the nozzles in the group are used, if it is a "0" 
	 * it means at least one is used and we need to append a data byte to 
	 * define the exact usage pattern. So, for direct encoded stripes we 
	 * start with the first two bits set to "10" and all the directory
	 * bitfield set to "1" (we will unset to "0" only the needed bits in
	 * the encoding loop). If we are using RLE encoding, each "0" bits
	 * means that there is a data byte that defines a pattern for the 
	 * group of 8 nozzles associated to that bit, and an "1" means that 
	 * the pattern for the associated group of nozzles is the same as the
	 * previous group. This means that for RLE encoded stripes we start the
	 * directory word with two bits set to "01" and then initialize to "1"
	 * all the directory bitfield, except the first one which must be 0
	 * because we must have at least one data byte to define the initial
	 * pattern that will be eventually repeated.
	 */
	if(gendata.numlines == 208)
	{
		nby = 26;    
		ofs = 6;    
		dts0 = 0x83; 
		dtr0 = 0x41;
		dtr1 = 0xff;
	}
	else
	{
		nby = 24;
		ofs = 8;
		dts0 = 0x80;
		dtr0 = 0x40;
		dtr1 = 0x7f;
	}

	/* The variable "bytes" will contain the total 
	 * number of output bytes in the data stripe.
	 */
	bytes = 0;

	/* For all the columns in the stripe */
	for(x = 0; x < numcols; x++)
	{
		/* Calculate which representation is smaller by counting
		 * the number of non zero data bytes for the direct encoding
		 * and the number of changes between data bytes for the RLE.
		 * At the end we have in "std" the length of the output data
		 * if encoded with standard encoding, and in "rle" the length
		 * of the output data if encoded with RLE.
		 */
		rle = 1;
		c = read[0];
		std = (c ? 1 : 0);
		for(i=1; i<nby; i++)
		{
			if((p = read[i]))std++;
			if(p != c)
			{
				rle++;
				c = read[i];
			}
		}

		/* Now initialize the last two bytes in the directory
		 * word. These always belong to the directory bitfield
		 * and must be set to all ones.
		 */
		write[2] = 0xff;
		write[3] = 0xff;

		/* And now encode the column, using the shortest encoding.
		 * If the two encodings are of equal length we prefer the
		 * standard encoding to the RLE one. No real reason for 
		 * this: it could have been done the other way, but it
		 * seems the Windows driver does this way as well...
		 */
		if(std > rle)
		{
			/* Run-length encoding */

			write[0] = dtr0;
			write[1] = dtr1;

			p = read[0];
			write[4] = p;
			cnt = 5;
			q = ofs + 1;

			for(i=1; i<nby; i++)
			{
				if(read[i] != p)
				{
					p = read[i];
					write[cnt] = p;
					write[q>>3] &= ibits[q & 7];
					cnt++;
				}
				q++;
			}
		}
		else
		{
			/* Standard encoding */

			write[0] = dts0;
			write[1] = 0xff;

			cnt = 4;
			q = ofs;

			for(i=0; i<nby; i++)
			{
				p = read[i];
				if(p)
				{
					write[cnt] = p;
					write[q>>3] &= ibits[q & 7];
					cnt++;
				}
				q++;
			}
		}

		/* Update the counters and pointers. Note that when 
		 * we are here "cnt" is the number of bytes that we 
		 * have actually output, including the directory word.
		 */
		read += (nby + 4);
		write += cnt;
		bytes += cnt;
	}

	fillheader(head, numcols, firstcol, bytes);
}

/* This routine takes one full buffer of data and
 * prints the black part, which of course is the only
 * one if we are printing in monochrome mode.
 */
static void
encode_bw_buf(void)
{
	int left, right, x, y, nn, mod;
	int nxp, yy, numcols, incr;
	int dy, dy2, csep, pass, f1;
	int f2, start, s1, s2, yincr;
	int q, mask, lines;
	byte *scan, *data;

	/* Set some parameters that depend on resolution and are
	 * used in the inner loop to select lines to print.
	 * We basically encode all the even nozzles in a loop and
	 * all the odd nozzles in another loop. The values of s1
	 * and s2 are the starting offset in the line buffer for
	 * the first and second loop, and yincr is the number of lines
	 * in the buffer we move on at each cycle.
	 */ 
	switch(gendata.yres)
	{
		/* At 300 dpi we use only one nozzle column, and
		 * each line in the buffer is printed. So both offsets
		 * are zero (only one is used, actually) and yincr is 1.
		 * The mask is set to 127 because the buffer is 128 lines.
		 */
		case 300:
			yincr = 1;
			s1 = 0;
			s2 = 0;
			mask = 127;
			break;
			
		/* At 600 dpi we use both nozzle columns: each row goes
		 * alternatively to the left or right nozzle column. So
		 * the even offset is zero, the odd offset is 1 and the
		 * increment is 2: in this way the even loop scans lines
		 * 0, 2, 4, ... and the odd loop lines 1, 3, 5, ...
		 * Here the buffer is 256 lines so mask is set to 255.
		 */
		default:
		case 600:
			yincr = 2;
			s1 = 0;
			s2 = 1;
			mask = 255;
			break;
			
		/* At 1200 dpi we are printing two interleaved passes. Each
		 * nozzle column sees every fourth line in the buffer (so
		 * yincr is 4) and the starting offset varies depending on
		 * which interleaved pass we are doing. 
		 * During the first pass, even nozzles are used to print 
		 * lines 0, 4, 8, ... and odd nozzles are used to print
		 * lines 2, 6, 10, ... while in the second pass the even
		 * nozzles print lines 1, 5, 9, ... and the odd nozzles
		 * print lines 3, 7, 11, ...
		 * The buffer is 512 lines, so mask is set to 511 */
		case 1200:
			yincr = 4;
			s1 = (gendata.ileave ? 1 : 0);
			s2 = (gendata.ileave ? 3 : 2);
			mask = 511;
			break;
	}
	
	/* Now we must calculate the offset q from the beginning of
	 * the buffer of the first line to print in this pass, and
	 * the total number of lines to be printed. We print as many
	 * lines as we can in a single pass, i.e. the value of "lines"
	 * is simply the number of lines that at the current vertical
	 * resolution fully cover the printing pen.
	 * Note that in case of monochrome printing we print all
	 * buffer lines, from first to last, so we also need to set
	 * the mask to a neutral value because we don't use wrapping.
	 */
	if(gendata.rendermode == LXM3200_M)
	{
		mask = 511;
		q = 0;
		lines = gendata.numblines;
	}
	else
	{
		q = gendata.firstline + valign[BLACKVALIGN];
		lines = (BWCOLPEN * 2) / gendata.yrmul;
	}

	/* Adjust the value of the nozzle column separation to the
	 * horizontal resolution we are using now.
	 */
	csep = (gendata.bwsep * 2) / gendata.xrmul;

	/* Here we calculate how many "real" passes we are doing.
	 * A "real" pass is a pass where a full column is printed
	 * and then some columns (maybe zero) are skipped before 
	 * printing another one. If we are at 1200 dpi horizontal
	 * then we must use only one nozzle column at a time, so each
	 * real pass comprises two subpasses, one where we print with
	 * even nozzles only and another where we print with odd
	 * nozzles only. So at 1200 dpi the "real" passes are half the
	 * total number of passes. Another way of looking at it: the
	 * "nxp" variable holds the separation, in columns, between two
	 * dot columns printed in the same head sweep.
	 */
	nxp = gendata.numpasses;
	if(gendata.xres == 1200)nxp /= 2; 

	/* Now calculate the byte increments for the *output* data
	 * buffer (i.e. the encoded buffer). The first variable,
	 * dy, is the number of bytes taken by a single data burst
	 * (both even and odd nozzle columns). The second variable,
	 * dy2, tells how many bytes we must skip from one column
	 * to the other (if we are printing multipass we skip some
	 * columns that will be printed in subsequent passes).
	 */
	dy = (gendata.numlines / 8) + 4;
	dy2 = dy * nxp;

	/* Calculate the starting and ending horizontal positions for
	 * this head pass. There are the margins corrected to take
	 * into account the displacement between the odd and even
	 * nozzle columns (csep). Moreover we start "csep" pixels
	 * before the margin to give the head a little more room to
	 * accelerate properly (not sure if this really works, but it
	 * doesn't cost much, so I've left it in).
	 */
	if(gendata.direction == LEFT)
	{
		left = gendata.left - 2*csep;
		right = gendata.right + csep;
	}
	else
	{
		left = gendata.left - csep;
		right = gendata.right + 2*csep;
	}
	
	/* Number of columns in a full row */
	numcols = right - left;

	/* Calculate the last pixel of the first pass of the
	 * stripe. If we are printing bidirectionally, this 
	 * will be the base to calculate the start of the 
	 * passes printed right-to-left.
	 */ 
	mod = numcols - (numcols % nxp);
	
	/* f1 and f2 are two flags that define which nozzle columns 
	 * we are using in this stripe, f1 for the even nozzle column 
	 * and f2 for the odd nozzle column: if they are zero that 
	 * nozzle column is not used in this pass.
	 */
	f1 = 1;
	f2 = 1;
	if(gendata.yres == 300)
	{
		/* At 300 dpi we use only one nozzle column. As of now this 
		 * is always the even one, but maybe it could be tried to 
		 * alternate between columns at each pass, to improve the 
		 * output quality.
		 */
		f1 = 1;
		f2 = 0;
	}
	
	/* Now start the passes to fill all the stripe */
	for(pass = 0; pass < gendata.numpasses; pass++)
	{
		/* If there is data in the buffer which has not been
		 * sent to the printer yet, send it now.
		 */
		if(gendata.fullflag)
		{
			fwrite(gendata.header, 3, 8, gendata.stream);
			fwrite(gendata.outdata, gendata.stripebytes, 1, gendata.stream);
			gendata.fullflag = FALSE;
		}
		
		/* Clear the output buffer to avoid problems with the bitwise
		 * operations we will do later on.
		 */
		memset(gendata.outdata, 0, gendata.numbytes * 30);
		
		/* Calculate standard increments, starting column
		 * and start of output data. They will be corrected
		 * later for 1200dpi or right-to-left printing direction.
		 */
		incr = nxp;
		start = left + pass;
		data = gendata.outdata + (pass*dy) + 4;
		
		/* It appears that at 1200dpi, in addition of not being able 
		 * to use 208 nozzles mode for the black cartridge, the Lexmark 
		 * 3200 cannot print at full rate with all the 192 useable nozzles.
		 * Maybe the reason is that the data rate of 1200dpi horizontal
		 * resolution exceeds the mechanical/thermal limits of the heads.
		 * So if we are printing at 1200dpi we need to use alternatively
		 * only odd or even numbered nozzles, for each printed column, to 
		 * half the data rate towards the head.
		 * This obviously means that, at 1200dpi horizontal, a minimum of
		 * two passes are required to print each stripe. Since if we are 
		 * printing at 1200dpi vertical we need two interlaced passes, a 
		 * minimum grand total of 4 passes are needed to print one full 
		 * 1200x1200 dpi stripe with the Lexmark 3200.
		 */
		if(gendata.xres == 1200)
		{
			f1 = pass & 1;
			f2 = 1 - f1;
			
			start = left + (pass/2);
			data = gendata.outdata + ((pass/2)*dy) + 4;
		}
		
		/* If printing right-to-left we need to present data
		 * to the printer in that direction, inverting the
		 * normal flow of data.
		 */
		if(gendata.direction == RIGHT)
		{
			incr = -nxp;
			start += mod;
		}
		
		/* Start column scanning */
		x = start;

		/* Now we split the behaviour depending on the printing
		 * direction. To be honest, the inner loops are almost
		 * identical between left-to-right and right-to-left
		 * directions. The only difference is where is computed
		 * the contribute of the nozzle columns separation ("csep"), 
		 * but having the "if" outside the loop it's somehow better.
		 */
		if(gendata.direction == LEFT)
		{
			/* For all the columns in this pass */
			for(nn = 0; nn < numcols; nn += nxp)
			{
				/* Encode the even numbered nozzles */
				if((x >= 0) && f1)
				{
					scan = gendata.scanbuf + x;
					yy = 0;
					for(y = s1; y < lines; y += yincr)
					{
						if(scan[((y+q) & mask) * gendata.numbytes] & BLACK)
							data[yy/8] |= bits[yy&7];
						yy += 2;
					}
				}
				
				/* Encode the odd numbered nozzles */ 
				if(((x+csep) < gendata.numbytes) && f2)
				{
					scan = gendata.scanbuf + x + csep;
					yy = 1;
					for(y = s2; y < lines; y += yincr)
					{
						if(scan[((y+q) & mask) * gendata.numbytes] & BLACK)
							data[yy/8] |= bits[yy&7];
						yy += 2;
					}
				}

				/* If we are in 1200dpi horizontal resolution,
				 * alternate between nozzle columns to avoid
				 * overstressing the printing head.
				 */
				if(gendata.xres == 1200)
				{
					f1 = 1 - f1;
					f2 = 1 - f2;
				}
				
				/* Adjust data pointers */
				data += dy2;
				x += incr;
			}
		}			
		else /* direction == RIGHT */
		{
			/* For all the columns in this pass */
			for(nn = 0; nn < numcols; nn += nxp)
			{			
				/* Encode the odd numbered nozzles */ 
				if((x < gendata.numbytes) && f1)
				{
					scan = gendata.scanbuf + x;
					yy = 1;
					for(y = s1; y < lines; y += yincr)
					{
						if(scan[((y+q) & mask) * gendata.numbytes] & BLACK)
							data[yy/8] |= bits[yy&7];
						yy += 2;
					}
				}

				/* Encode the even numbered nozzles */				
				if(((x-csep) >= 0) && f2)
				{
					scan = gendata.scanbuf + x - csep;
					yy = 0;
					for(y = s2; y < lines; y += yincr)
					{
						if(scan[((y+q) & mask)*gendata.numbytes] & BLACK)
							data[yy/8] |= bits[yy&7];
						yy += 2;
					}
				}

				/* If we are in 1200dpi horizontal resolution,
				 * alternate between nozzle columns to avoid 
				 * overstressing the printing head.
				 */
				if(gendata.xres == 1200)
				{
					f1 = 1 - f1;
					f2 = 1 - f2;
				}
				
				/* Adjust data pointers */
				data += dy2;
				x += incr;
			}
		}

		/* Convert the buffer to the shortest output format.
		 * Which is the first column of the output depends 
		 * on the printing direction: it will be the left
		 * margin if we are printing left to right or the
		 * right margin if we are printing right to left.
		 */
		if(gendata.direction == LEFT)
			convbuf(LEFT, numcols, left);
		else
			convbuf(LEFT, numcols, right);
	}
}

/* This routine is the equivalent of encode_bw_buf() but
 * for color or photo cartridge. Since this routine is
 * heavily based on the B/W one, the comments here will
 * be somewhat less esaurient. Please have a look at 
 * encode_bw_buf() to understand better how this routine
 * works: I will only pinpoint the differences between this
 * routine and encode_bw_buf().
 *
 * head: the head we are calculating the buffer for. It will
 *       be LEFT for a photo cartridge or RIGHT for a color one.
 */
static void 
encode_col_buf(int head)
{
	int left, right, x, y, nn, mod;
	int nxp, yy, numcols, incr;
	int dy, dy2, csep, pass, f1;
	int f2, start, s1, s2, yincr;
	int q, mask, k, align, lines;
	byte *scan, *data;

	/* Here there are two more parameters: mask and lines, that
	 * for color cartridges are both dependent on vertical
	 * resolution. Since the buffer is "rolling", i.e. it is
	 * implemented as a circular array, all the coordinates of
	 * the buffer lines must be taken modulo the buffer length.
	 * We choose a buffer length that is a power of two to be
	 * able to turn the modulo operation into a bitwise AND, so
	 * we need to set "mask" to the correct value for the AND.
	 * Another difference is that "lines", i.e. the number of
	 * lines to print in each pass, is based on the height of a 
	 * color pen. Since there are three color pens in each cartridge,
	 * each color pen is treated separately to fully cover the
	 * printing head.
	 */
	switch(gendata.yres)
	{
		case 300:
			yincr = 1;
			s1 = 0;
			s2 = 0;
			mask = 127;
			lines = COLORPEN/2;
			break;
			
		default:
		case 600:
			yincr = 2;
			s1 = 0;
			s2 = 1;
			mask = 255;
			lines = COLORPEN;
			break;
			
		case 1200:
			yincr = 4;
			s1 = (gendata.ileave ? 1 : 0);
			s2 = (gendata.ileave ? 3 : 2);
			mask = 511;
			lines = COLORPEN*2;
			break;
	}
	
	/* Choose the vertical alignment depending on the head.
	 * This is needed to vertically align the color cartridge 
	 * with the photo or black cartridge.
	 */
	if(head == LEFT)
		align = valign[PHOTOVALIGN];
	else
		align = valign[COLORVALIGN];		

	/* All the stuff below is exactly the same as in
	 * encode_bw_buf(), and is therefore commented there.
	 */

	csep = (gendata.bwsep * 2) / gendata.xrmul;
	nxp = gendata.numpasses;
	if(gendata.xres == 1200)nxp /= 2; 

	dy = (gendata.numlines / 8) + 4;
	dy2 = dy * nxp;

	if(gendata.direction == LEFT)
	{
		left = gendata.left - 2*csep;
		right = gendata.right + csep;
	}
	else
	{
		left = gendata.left - csep;
		right = gendata.right + 2*csep;
	}
	
	numcols = right - left;
	mod = numcols - (numcols % nxp);
	
	f1 = 1;
	f2 = 1;
	if(gendata.yres == 300)
	{
		f1 = 1;
		f2 = 0;
	}

	/* For all passes */
	for(pass = 0; pass < gendata.numpasses; pass++)
	{
		/* If there is data in the buffer which has not been
		 * sent to the printer yet, do it now.
		 */
		if(gendata.fullflag)
		{
			fwrite(gendata.header, 3, 8, gendata.stream);
			fwrite(gendata.outdata, gendata.stripebytes, 1, gendata.stream);
			gendata.fullflag = FALSE;
		}

		/* All the stuff below is exactly the same as in
		 * encode_bw_buf(), and is therefore commented there.
		 */

		memset(gendata.outdata, 0, gendata.numbytes * 30);
		
		incr = nxp;
		start = left + pass;
		data = gendata.outdata + (pass*dy) + 4;
		
		if(gendata.xres == 1200)
		{
			f1 = pass & 1;
			f2 = 1 - f1;
			
			start = left + (pass/2);
			data = gendata.outdata + ((pass/2)*dy) + 4;
		}
		
		if(gendata.direction == RIGHT)
		{
			incr = -nxp;
			start += mod;
		}
		
		/* Start column scanning */
		x = start;

		if(gendata.direction == LEFT)
		{
			/* For all the columns */
			for(nn = 0; nn < numcols; nn += nxp)
			{
				/* Encode the even numbered nozzles */
				if((x >= 0) && f1)
				{
					scan = gendata.scanbuf + x;
					yy = 0;

					/* In color printing there is one more loop to scan 
					 * all three color pens. We have to do exactly the 
					 * same things for all pens: the only differences are 
					 * the color encoding bit we are testing and the offset 
					 * from the beginning of the buffer and the offset of the 
					 * output data. All of this is stored into arrays. The 
					 * "penofs" array stores the offset of the first line of 
					 * each pen in the raster buffer. The array "colmask" stores
					 * the encoding bits for the color of each pen, and it
					 * is bidimensional because pen masks are different between
					 * a color cartridge (where pens are Cyan, Magenta, Yellow)
					 * and a photo cartridge (where pens are LightCyan, 
					 * LightMagenta and Black).
					 */
					for(k=0; k<3; k++)
					{
						q = gendata.firstline + align + penofs[k];
						for(y = s1; y < lines; y += yincr)
						{
							if(scan[((y+q) & mask) * gendata.numbytes] & colmask[head][k])
								data[yy/8] |= bits[yy&7];
							yy += 2;
						}
					}
				}

				/* Encode the odd numbered nozzles */ 
				if(((x+csep) < gendata.numbytes) && f2)
				{
					scan = gendata.scanbuf + x + csep;
					yy = 1;
					for(k=0; k<3; k++)
					{
						q = gendata.firstline + align + penofs[k];
						for(y = s2; y < lines; y += yincr)
						{
							if(scan[((y+q) & mask) * gendata.numbytes] & colmask[head][k])
								data[yy/8] |= bits[yy&7];
							yy += 2;
						}
					}
				}

				/* If we are in 1200dpi horizontal resolution,
				 * alternate between nozzle columns to avoid
				 * overstressing the printing head.
				 */
				if(gendata.xres == 1200)
				{
					f1 = 1 - f1;
					f2 = 1 - f2;
				}
				
				/* Adjust data pointers */
				data += dy2;
				x += incr;
			}
		}			
		else
		{
			/* For all the columns */
			for(nn = 0; nn < numcols; nn += nxp)
			{			
				/* Encode the odd numbered nozzles */ 
				if((x < gendata.numbytes) && f1)
				{
					scan = gendata.scanbuf + x;
					yy = 1;
					for(k=0; k<3; k++)
					{
						q = gendata.firstline + align + penofs[k];
						for(y = s1; y < lines; y += yincr)
						{
							if(scan[((y+q) & mask) * gendata.numbytes] & colmask[head][k])
								data[yy/8] |= bits[yy&7];
							yy += 2;
						}
					}
				}

				/* Encode the even numbered nozzles */				
				if(((x-csep) >= 0) && f2)
				{
					scan = gendata.scanbuf + x - csep;
					yy = 0;
					for(k=0; k<3; k++)
					{
						q = gendata.firstline + align + penofs[k];
						for(y = s2; y < lines; y += yincr)
						{
							if(scan[((y+q) & mask) * gendata.numbytes] & colmask[head][k])
								data[yy/8] |= bits[yy&7];
							yy += 2;
						}
					}
				}

				/* If we are in 1200dpi horizontal resolution,
				 * alternate between nozzle columns to avoid 
				 * overstressing the printing head.
				 */
				if(gendata.xres == 1200)
				{
					f1 = 1 - f1;
					f2 = 1 - f2;
				}
				
				/* Adjust data pointers */
				data += dy2;
				x += incr;
			}
		}
		
		if(gendata.direction == LEFT)
			convbuf(head, numcols, left);
		else
			convbuf(head, numcols, right);
	}
}

/* Fill monochrome buffer: this routine fills the buffer 
 * with rasterized lines, skipping over vertical spacing 
 * (i.e. completely blank lines). The routine is only 
 * used in monochrome mode, where we print a full buffer 
 * at each stripe. The color printing needs a different 
 * routine which makes use of a circular buffer.
 * 
 * vline: the line from which to start searching for data.
 */
static int
fill_mono_buffer(int vline)
{
	byte *in_data, *data;
	int i, ret, ofs;

	/* Initialize the "data" pointer, that will be used to
	 * scan all the lines in the buffer, and the "ofs" pointer 
	 * that will be used to mark the start of the "real" rasterized 
	 * data into the buffer (see below). To compensate for the offsets 
	 * caused by the horizontal spacing between nozzle columns on a 
	 * cartridge, the head must start before the horizontal margin, so 
	 * the buffer width is slightly bigger than the width of the
	 * rasterized lines. The difference is the "guard offset", and the 
	 * variables gendata.numbytes and gendata.numrbytes hold respectively 
	 * the number of bytes in a buffer line and the number of bytes in a 
	 * rasterized scanline, while gendata.goffset contains the number of
	 * bytes reserved to the guard offset on each side of the scanline.
	 */
	data = gendata.scanbuf;
	ofs = gendata.goffset;

	/* Cycle until we have no more lines on the page */
	while(vline < gendata.numvlines)
	{
		/* Ask Ghostscript for one rasterized line */
		gdev_prn_get_bits((gx_device_printer *)gendata.dev,
											vline, data+ofs, &in_data);

		/* And check if it's all zero: if not, break out of
		 * the loop. This nice trick with memcpy it's by Stephen
		 * Taylor (if I'm not wrong...)

		 */
		if(in_data[0] != 0 || 
			 memcmp(in_data, in_data+1,gendata.numrbytes-1))break;
		vline++;
	}
	
	/* If we are here because no non-empty lines were found before 
	 * the end of the page, our work is over. Return to the caller 
	 * saying that this is the last buffer (LAST bit set) and it's 
	 * empty (no LHDATA or RHDATA bit set).
	 */
	if(vline >= gendata.numvlines)return(LAST);
	
	/* This buffer contains at least one non-empty line.
	 * Adjust the current vertical position and load the first
	 * line into the buffer.
	 */
	gendata.curvline = vline;
	memset(data, 0, gendata.numbytes);
	if(in_data != data+ofs)memcpy(data+ofs, in_data, gendata.numrbytes);
	
	vline++;
	data += gendata.numbytes;
	
	/* Now initialize the return value to LHDATA (since at least
	 * one non-blank line was found, this buffer contains data, and
	 * it is obviously left-head data because we are in monochromatic 
	 * mode and so we are printing with left head only).
	 * After that, get as many rasterized lines as needed to fill the
	 * buffer, checking if in the process we have reached the end of 
	 * the page.
	 */
	ret = LHDATA;
	for(i=1; i<gendata.numblines; i++)
	{
		memset(data, 0, gendata.numbytes);
		if(vline > gendata.numvlines)
		{
			/* Ok, we are at the end of the page, so set the LAST bit
			 * in the return value but don't exit the loop because we
			 * need to make sure all remaining lines in the buffer will
			 * be blanked (exiting now would leave them untouched from
			 * the previous stripe). This is needed to avoid printing
			 * random data under the bottom margin.
			 */
			ret = LHDATA | LAST;
		}
		else
		{
			/* If we are not at the end of the page, copy one more
			 * scanline into the buffer.
			 */
			gdev_prn_get_bits((gx_device_printer *)gendata.dev,
												vline, data+ofs, &in_data);       
			if(in_data != data+ofs)memcpy(data+ofs, in_data, gendata.numrbytes);
		}

		vline++;
		data += gendata.numbytes;

	}
	
	return(ret);
}

/* Fill the buffer with initial data.
 * This routine is used to load the first buffer at the
 * beginning of the page. If we are printing in monochromatic
 * mode, we just call fill_mono_buffer for the first line.
 * If we are printing in color mode, we have a problem to
 * solve: since the color pen are stacked vertically, we
 * need multiple head passes to print all colors on the 
 * same line. So, to simplify all, we start with the paper
 * at a fixed vertical position, even if it's blank, and
 * then we go down in fixed increments, equal to the height
 * of a color pen. This means we check all buffers without
 * skipping over blank ones, but since we actually send the
 * printing commands to the printer only when there is something
 * to print, there is no speed impact. 
 */
static int 
init_buffer(void)
{
	byte *in_data, *data;
	int i, ret, p1, p2, ofs;

	data = gendata.scanbuf;
	ofs = gendata.goffset;

	if(gendata.rendermode == LXM3200_M)return(fill_mono_buffer(0));

	/* We position the heads with the bottom color pen (the
	 * yellow one in the color cartridge and the black one
	 * in the photo cartridge) just covering the first lines
	 * of the paper sheet. So the first buffer is divided in
	 * two parts: "p1" is the number of lines above the top
	 * border and "p2" the number of lines below.
	 */
	p1 = 368 / gendata.yrmul;
	p2 = 144 / gendata.yrmul;

	/* Initialize the counters */
	gendata.curvline = -p1;
	gendata.lastblack = gendata.curvline - 1;
	data = gendata.scanbuf;

	/* Clear the lines of the buffer that correspond to
	 * lines above the top margin: of course we don't
	 * want to print anything on the air... 
	 */
	for(i=0; i<p1; i++)
	{
		memset(data, 0, gendata.numbytes);
		data += gendata.numbytes;
	}

	/* And now load the last part of the buffer.
	 * Note that here we don't check for blank lines,
	 * this will be cared for later.
	 */
	for(i=0; i<p2; i++)
	{
		memset(data, 0, gendata.numbytes);

		if(i < gendata.numvlines)
		{
			gdev_prn_get_bits((gx_device_printer *)gendata.dev,
												i, data+ofs, &in_data);					
			if(in_data != data+ofs)memcpy(data+ofs, in_data, gendata.numrbytes);
		}

		data += gendata.numbytes;
	}

	gendata.firstline = 0;

	/* Now check the return value. If by chance we are under
	 * the bottom margin, add the LAST bit to the return value.
	 * Of course, since this is the first buffer of the page,
	 * it's not likely we will reach the bottom margin in 
	 * this pass. Anyway this is code that will be executed
	 * only once per page, so better safe than sorry.
	 */
	ret = (gendata.numvlines < p2 ? LAST : 0) | qualify_buffer();

	return(ret);
}

/* This function checks if the current buffer contains
 * data to be printed with the left or right head.
 * It assumes that we are printing in color mode, and it
 * is useful to minimize the number of needed passes.
 * When we are printing in monochrome mode we directly skip 
 * over blank lines, so this routine is not needed.
 */
static int
qualify_buffer(void)
{
	int i, j, k, ret;
	int rmsk, q, v1;
	int bpsz, cpsz;
	byte *data;

	ret = 0;

	/* Set variables which contains the size, in rows, of 
	 * each color pen and of the black pen in color mode,
	 * adjusting for different resolution settings.
	 * Also set the mask used to rollover the buffer.
	 */
	cpsz = (COLORPEN * 2) / gendata.yrmul;
	bpsz = (BWCOLPEN * 2) / gendata.yrmul;
	rmsk = gendata.numblines - 1;

	/* Check the right head data, it is always a color cartridge */
	for(k=0; k<3 && ret==0; k++)
	{
		/* For each pen, scan all the bytes on each row of 
		 * the buffer that is covered by the current pen,
		 * ORing together all the bits.
		 */
		v1 = 0;
		q = gendata.firstline + valign[COLORVALIGN] + penofs[k];
		for(i=0; i<cpsz; i++)
		{
			data = gendata.scanbuf + ((q+i) & rmsk)*gendata.numbytes;
			for(j=0; j<gendata.numbytes; j++)v1 |= *data++;
		}
		/* If the result of the OR has the proper color bit
		 * set, it means that this buffer contains at least
		 * one pixel of this pen, so we need a color pass.
		 * Note that we exit as soon as we find a color bit
		 * set, because if at least one color pen is used
		 * in this buffer we need to do a color pass anyway,
		 * so there's no need to check the other two pens.
		 */
		if(v1 & colmask[RIGHT][k])ret |= RHDATA;
	}

	/* Check the left head data: it could be a black or
	 * a photo cartridge, depending on the printing mode.
	 */
	if(gendata.rendermode == LXM3200_C)
	{	
		/* We are in standard color mode: the left cartridge 
		 * is a black cartridge used in 192 nozzles mode.
		 * This is done exactly in the same way as above, but
		 * without the outer loop because we have only one
		 * color pen on this cartridge.
		 */
		v1 = 0;
		q = gendata.firstline + valign[BLACKVALIGN]; 
		for(i=0; i<bpsz; i++)
		{
			data = gendata.scanbuf + ((q+i) & rmsk)*gendata.numbytes;
			for(j=0; j<gendata.numbytes; j++)v1 |= *data++;
		}
		if(v1 & BLACK)ret |= LHDATA;
	}
	else
	{
		/* If we are here we need to check for a photo cartridge
		 * (this routine is never called in monochrome mode, so
		 * if we are not in color mode we must be in photo mode).
		 * This routine is identical to the color routine above.
		 */
		for(k=0; k<3 && !(ret & LHDATA); k++)
		{
			v1 = 0;
			q = gendata.firstline + valign[PHOTOVALIGN] + penofs[k];
			for(i=0; i<cpsz; i++)
			{
				data = gendata.scanbuf + ((q+i) & rmsk)*gendata.numbytes;
				for(j=0; j<gendata.numbytes; j++)v1 |= *data++;
			}
			if(v1 & colmask[LEFT][k])ret |= LHDATA;
		}
	}
	
	return(ret);
}

/* This functions rolls the circular buffer by the
 * number of lines of one color pen, reading new
 * lines to refill the buffer.
 * In color mode we use a circular buffer because 
 * we need to read the same lines more than once.
 * So when we are forwarding to the next pass we
 * simply read in the new lines and then update the
 * pointers, without actually moving data into memory.
 * The need to read the same data more than once arises
 * from the fact that the color pens are vertically
 * stacked, so we need to read a data line to lay down
 * the yellow component at the first pass. We need to
 * read it again at the next pass to lay down magenta,
 * and on the last pass we read the same line once more
 * to lay down the cyan component.
 */
static int
roll_buffer(void)
{
	int i, ret, fline, vl, ofs;
	int cpen, cmask, lline;
	byte *data, *in_data;

	/* Adjust the size of the color pen and the
	 * mask to take into account the current resolution
	 */
	cpen = (COLORPEN * 2) / gendata.yrmul;
	cmask = (gendata.numblines) - 1;

	/* Calculate the line number corresponding to 
	 * the last buffer we can print before being
	 * forced to eject the page. At 600dpi this
	 * has been experimentally determined to be
	 * 112 lines from the bottom of the page.
	 */
	lline = gendata.numvlines - (224 / gendata.yrmul);

	/* Roll the buffer by advancing the first line
	 * pointer by the height of one color pen.
	 */
	fline = gendata.firstline;
	gendata.firstline = (fline + cpen) & cmask;

	/* Now calculate the pointer to the first "fresh"
	 * line on the page, i.e. the first line we must
	 * read into the buffer at this pass.
	 */
	vl = gendata.curvline + cmask + 1;

	/* Take into account the guard offset */
	ofs = gendata.goffset;

	/* Initialize the return value and update the
	 * current vertical position on the page, while
	 * checking if we have reached the last printable
	 * buffer.
	 */
	ret = 0;
	gendata.curvline += cpen;
	if(gendata.curvline >= lline)ret = LAST;

	/* Now read "fresh" rasterized scanlines into the
	 * input buffer.
	 */ 
	for(i=0; i<cpen; i++)
	{
		data = gendata.scanbuf + ((fline + i) & cmask) * gendata.numbytes;

		memset(data, 0, gendata.numbytes);		
		if(vl < gendata.numvlines)
		{
			gdev_prn_get_bits((gx_device_printer *)gendata.dev, 
												vl, data+ofs, &in_data);
			if(in_data != data+ofs)memcpy(data+ofs, in_data, gendata.numrbytes);
		}
		vl++;
	}
	
	/* And test for the presence of actual data to print */
	ret |= qualify_buffer();

	return(ret);
}

/* Calculate the margins of one line, i.e. the leftmost 
 * and the rightmost non-blank pixel on the line.
 *
 * data:  the pointer to the data for this line
 * mask:  the mask with the bits to check for: if the buffer
 *        contains data which is not on the mask it will be
 *        ignored (for the purpose of calculating margins)
 * left:  calculated left margin (output variable)
 * right: calculated right margin (output variable)
 */
static void 
calclinemargins(byte *data, int mask, int *left, int *right)
{
	int l,r,num;

	num = gendata.numbytes - 1;

	l = 0;
	while((l <= num) && ((data[l] & mask) == 0))l++;
	
	r = num;
	while((r >= 0) && ((data[r] & mask) == 0))r--;

	*left = l;
	*right = r;
}

/* Calculate the margins of the whole buffer. The
 * calculation accounts separately for the data to
 * be printed with the left or with the right head,
 * so we can try to minimize the head movement even 
 * on multiple passes.
 * 
 * head: the code of the head we are calculating
 *       margins for (LEFT or RIGHT)
 */
static void
calcbufmargins(int head)
{
	int i, l1, r1, q, k;
	int mleft, mright, nl;
	int cpen, cmask, al;
	byte *scan;

	/* Adjust mask and pen height according to vertical resolution */
	cpen = (COLORPEN * 2) / gendata.yrmul;
	cmask = (gendata.numblines) - 1;

	/* Calculate margins for a color or photo cartridge */
	if(head == RIGHT || (gendata.rendermode == LXM3200_P))
	{				
		/* Get correct vertical aligment */
		al = (head == LEFT ? PHOTOVALIGN : COLORVALIGN);

		q = gendata.firstline + valign[al];

		/* Calculate margins for first line, using those values
		 * to initialize the max and min values.
		 */
		scan = gendata.scanbuf + ((q+penofs[0]) & cmask)*gendata.numbytes;
		calclinemargins(scan, colmask[head][0], &mleft, &mright);

		/* And now scan all the remaining buffer. We scan according
		 * to color pens, i.e. we calculate the margin on the rows
		 * where magenta will be laid down taking into account magenta
		 * pixels only, and this will be the magenta submargin. After 
		 * having done that for cyan and yellow as well, we take as the 
		 * global margin the smaller space that contains all the three 
		 * submargins.
		 */
		for(k=0; k<3; k++)
		{
			for(i=0; i<cpen; i++)
			{
				scan = gendata.scanbuf + ((q+i+penofs[k]) & cmask)*gendata.numbytes;
				calclinemargins(scan, colmask[head][k], &l1, &r1);
				mleft = min(mleft, l1);
				mright = max(mright, r1);
			}
		}

		gendata.left = mleft;
		gendata.right = mright;

		return;
	}

	/* Calculate buffer margins for a black head. This is
	 * almost exactly the same as before, but now we do
	 * a single pass because we have only one black pen.
	 */
	if(gendata.rendermode == LXM3200_M)
	{
		/* Monochromatic mode: we use 208 nozzles and
		 * all the buffer, so the initial offset is zero.
		 */

		scan = gendata.scanbuf;
		calclinemargins(scan, BLACK, &mleft, &mright);

		for(i=1; i<gendata.numblines; i++)
		{
			scan += gendata.numbytes;
			calclinemargins(scan, BLACK, &l1, &r1);
			mleft = min(mleft, l1);
			mright = max(mright, r1);
		}

		gendata.left = mleft;
		gendata.right = mright;

		return;
	}

	/* Standard color mode: we use 192 nozzles and must 
	 * take into account the vertical alignment.
	 */
	
	nl = (gendata.numlines * 2) / gendata.yrmul;
	q = gendata.firstline + valign[BLACKVALIGN];

	scan = gendata.scanbuf + (q & cmask)*gendata.numbytes;
	calclinemargins(scan, BLACK, &mleft, &mright);

	for(i=1; i<nl; i++)
	{
		scan = gendata.scanbuf + ((q+i) & cmask)*gendata.numbytes;
		calclinemargins(scan, BLACK, &l1, &r1);
		mleft = min(mleft, l1);
		mright = max(mright, r1);
	}
	gendata.left = mleft;
	gendata.right = mright;	
}

/*
 * This is the main routine that prints in
 * standard color mode.
 */
static void
print_color_page(void)
{
	int res, lline, cmask;
	int i, j, nl, q, sk;
	byte *scan;

	/* Set the blackskip value depending on vertical resolution.
	 * Since we have a black pen which is 3 times as high as
	 * each color pen, we must print black only once every three 
	 * passes. So we take into account on which line we printed
	 * the last black stripe and then we print another only if
	 * the current line is at least "sk" lines after that.
	 */
	sk = (BWCOLPEN * 2) / gendata.yrmul;
	
	/* Get the first buffer, and if it's empty continue
	 * to skip forward without doing anything.
	 */
	res = init_buffer();
	while(res == 0)res = roll_buffer();

	/* If this buffer happens to be the last one, 
	 * and empty as well, we had a blank page. 
	 * Just exit without ado.
	 */
	if(res == LAST)return;

	/* This is the first non-blank line of the
	 * page: issue a vertical skip command to
	 * advance the paper to proper position.
	 */
	skiplines(gendata.curvline, COLTOPSTART);

	/* "lline" holds the number of the first line of
	 * the last buffer printed, either with left or 
	 * right head. This is needed to keep track of 
	 * how many lines we must skip from one stripe to 
	 * the next (if we encounter blank buffers we just
	 * ignore them without moving the head, so we need
	 * to do the proper vertical motion in one single
	 * pass as soon as we encounter a non-blank buffer). 
	 */
	lline = gendata.curvline;
	
	/* Now depending on the data we have into the 
	 * buffer, print with the left head, right
	 * head or both.
	 * NOTE: this is the first buffer, and it needs
	 * to be treated specially from the others.
	 * The main difference is that we usually finalize
	 * the buffer (issuing the print commands) at the
	 * start of the next buffer, and not at the end of
	 * the current one. This is because the Lexmark 3200
	 * wants to know where to leave the printing head
	 * at the end of each printing command, but we can't
	 * know that until we start the next buffer so discovering
	 * its margins and position. The solution is that we keep
	 * "suspended" each buffer until we find another valid one.
	 * The first buffer is special since there is no previous
	 * buffer to finalize.
	 * NOTE: I will comment the general case below, because
	 * this code is simply a subset of the main loop.
	 */
	switch(res)
	{
		case LHDATA:
			calcbufmargins(LEFT);
			gendata.ileave = 0;
			encode_bw_buf();
			gendata.lastblack = gendata.curvline + sk;
			lline = gendata.curvline;
			if(gendata.yres == 1200)
			{
				finalizeheader(1, LEFT);
				gendata.ileave = 1;
				encode_bw_buf();
				lline++;
			}
			break;

		case RHDATA:
			calcbufmargins(RIGHT);
			gendata.ileave = 0;
			encode_col_buf(RIGHT);
			lline = gendata.curvline;
			if(gendata.yres == 1200)
			{
				finalizeheader(1, RIGHT);
				gendata.ileave = 1;
				encode_col_buf(RIGHT);
				lline++;
			}
			break;

		case LHDATA|RHDATA:
			calcbufmargins(LEFT);
			gendata.ileave = 0;
			encode_bw_buf();
			gendata.lastblack = gendata.curvline + sk;
			calcbufmargins(RIGHT);
			finalizeheader(0, RIGHT);
			encode_col_buf(RIGHT);
			lline = gendata.curvline;
			if(gendata.yres == 1200)
			{
				calcbufmargins(LEFT);
				finalizeheader(1, LEFT);
				gendata.ileave = 1;
				encode_bw_buf();
				calcbufmargins(RIGHT);
				finalizeheader(0, RIGHT);
				encode_col_buf(RIGHT);
				lline++;
			}
			break;
	}

	/* Skip to next buffer */
	res = roll_buffer();

	/* Start the main loop. Here we do all the stuff required
	 * to print buffers properly.
	 */
	while(!(res & LAST))
	{
		/* If we haven't forwarded until "lastblack", do not
		 * print black data because it has been printed on
		 * previous passes. So, if we are below gendata.lastblack
		 * clear the LHDATA flag to ignore left-head data.
		 */
		if(gendata.curvline < gendata.lastblack)res &= ~LHDATA;

		/* And now start examining the buffer for data */
		switch(res)
		{
			/* We have left head data */
			case LHDATA:

				/* Calculate the margins of this buffer */
				calcbufmargins(LEFT);

				/* And then finalize the previous buffer. We can't 
				 * do this until now, because only now we know the 
				 * margins and vertical position of the next buffer,
				 * which are required data to calculate the final 
				 * head position at the end of the previous buffer.
				 */
				finalizeheader(gendata.curvline - lline, LEFT);

				/* Set interleave to zero (only meaningful in 1200dpi
				 * vertical mode.
				 */
				gendata.ileave = 0;

				/* Encode this buffer making it the current buffer */
				encode_bw_buf();
				
				/* Since we are printing a black buffer, update
				 * gendata.lastblack to point to the first line
				 * not covered by this black pass.
				 */
				gendata.lastblack = gendata.curvline + sk;

				/* And update "lline" as well */
				lline = gendata.curvline;

				/* If we are printing at 1200 dpi vertical, we must
				 * do one more pass, interleaved with the one before.
				 */
				if(gendata.yres == 1200)
				{
					/* Finalize previous buffer, moving down 1/1200th
					 * of an inch to properly interleave the two passes.
					 * This is naive: we should do something here, because
					 * this way two adjacent lines are printed by the same
					 * nozzle, and there is the danger of having a slight
					 * banding on output (no more than 1/600th of an inch,
					 * but maybe noticeable).
					 */
					finalizeheader(1, LEFT);

					/* Set interleave to 1 to start an interleaved pass */
					gendata.ileave = 1;
					
					/* Encode the buffer, and not finalize it: we leave
					 * the buffer suspended until we find another buffer
					 * to print.
					 */
					encode_bw_buf();

					/* And adjust "lline" because to print the interleaved
					 * pass we have moved down one line, so we need to 
					 * skip one line less to print the next buffer.
					 */
					lline++;
				}
				break;

			/* Right head data. This is absolutely identical to the
			 * code above for left head data, with two exceptions: all
			 * the "LEFT" codes are changed to "RIGHT" and we don't
			 * update gendata.lastblack because we are printing a
			 * color stripe and not a black one.
			 */
		  case RHDATA:
				calcbufmargins(RIGHT);
				finalizeheader(gendata.curvline - lline, RIGHT);
				gendata.ileave = 0;
				encode_col_buf(RIGHT);
				lline = gendata.curvline;
				if(gendata.yres == 1200)
				{
					finalizeheader(1, RIGHT);
					gendata.ileave = 1;
					encode_col_buf(RIGHT);
					lline++;
				}
				break;
				
			/* We have both left and right head data (i.e. black and
			 * color on the same stripe).
			 * The code here is identical to the code for the left data
			 * only and right data only cases above. But they are
			 * interleaved, because since we can't take back the paper
			 * once it's advanced, in case we are printing at 1200 dpi
			 * vertical (and so we need two interlaced passes) we need
			 * to do both the first black and the first color pass,
			 * advance the paper and then do the second black and the
			 * second color pass. Simply appendig the two code pieces
			 * above would not work.
			 */
			case LHDATA|RHDATA:
				calcbufmargins(LEFT);
				finalizeheader(gendata.curvline - lline, LEFT);
				gendata.ileave = 0;
				encode_bw_buf();
				gendata.lastblack = gendata.curvline + sk;
				calcbufmargins(RIGHT);
				finalizeheader(0, RIGHT);
				encode_col_buf(RIGHT);
				lline = gendata.curvline;		
				if(gendata.yres == 1200)
				{
					calcbufmargins(LEFT);
					finalizeheader(1, LEFT);
					gendata.ileave = 1;
					encode_bw_buf();
					calcbufmargins(RIGHT);
					finalizeheader(0, RIGHT);
					encode_col_buf(RIGHT);
					lline++;
				}
				break;
		}
		
		/* Get another buffer */
		res = roll_buffer();
	}
	
	/* Last buffer. We treat this one specially as well,
	 * because we don't have a subsequent buffer to print,
	 * and so we need to finalize this buffers as soon as
	 * possible.
	 */
	res = qualify_buffer();

	/* Void the printed blacks. Since we are printing the
	 * last buffer, it could happen that we have advanced
	 * from the last time we printed a black stripe but
	 * we are not yet at the point where another black
	 * stripe would have been triggered. This would cause
	 * an eventual black component in the last lines of
	 * the page to be ignored.
	 * To avoid the problem we do an unconditional black
	 * pass, but we also must clear the black bits from the
	 * lines we have already printed otherwise we would
	 * print them twice.
	 */
	if((res & LHDATA) && (gendata.curvline <= gendata.lastblack))
	{ 
		/* Find how many black lines we have yet printed
		 * are still in the buffer
		 */
		nl = gendata.lastblack - gendata.curvline;

		/* And now remove the BLACK bit from them */

		q = gendata.firstline + valign[BLACKVALIGN];
		cmask = (gendata.numblines) - 1;	
		for(i=0; i<nl; i++)
		{
			scan = gendata.scanbuf + ((i+q) & cmask)*gendata.numbytes;
			for(j=0; j<gendata.numbytes; j++)
			{
				*scan &= ~BLACK;
				scan++;
			}
		}
	}
	
	/* Now we can print the last buffer as usual.
	 * This is perfectly identical to the code
	 * into the loop above: we are replicating it 
	 * only because we need the blanking code above
	 * to be executed before this code.
	 * Maybe there is a better way to do it...
	 */
	switch(res)
	{	
		case LHDATA:
			calcbufmargins(LEFT);
			finalizeheader(gendata.curvline - lline, LEFT);
			gendata.ileave = 0;
			encode_bw_buf();
			if(gendata.yres == 1200)
			{
				finalizeheader(1, LEFT);
				gendata.ileave = 1;
				encode_bw_buf();
				lline++;
			}
			break;
			
		case RHDATA:
			calcbufmargins(RIGHT);
			finalizeheader(gendata.curvline - lline, RIGHT);
			gendata.ileave = 0;
			encode_col_buf(RIGHT);
			if(gendata.yres == 1200)
			{
				finalizeheader(1, RIGHT);
				gendata.ileave = 1;
				encode_col_buf(RIGHT);
				lline++;
			}
			break;
				
		case LHDATA|RHDATA:
			calcbufmargins(LEFT);
			finalizeheader(gendata.curvline - lline, LEFT);
			gendata.ileave = 0;
			encode_bw_buf();
			calcbufmargins(RIGHT);
			finalizeheader(0, RIGHT);
			encode_col_buf(RIGHT);
			if(gendata.yres == 1200)
			{
				calcbufmargins(LEFT);
				finalizeheader(1, LEFT);
				gendata.ileave = 1;
				encode_bw_buf();
				calcbufmargins(RIGHT);
				finalizeheader(0, RIGHT);
				encode_col_buf(RIGHT);
				lline++;
			}
			break;
	}

	/* Now finalize the header using a value of "0" for
	 * the vertical skip (no need to move down: the
	 * paper is about to be ejected) and -1 for the
	 * head (meaning: last buffer, don't care for the
	 * final head position, it will be reset unconditionally
	 * by the trailing sequence).
	 */
	finalizeheader(0, -1);
}

/* This is the equivalent of print_color_page() 
 * for monochrome output. It is almost identical,
 * only much simpler because now we are printing
 * with only one head.
 */
static void
print_mono_page(void)
{	
	int res, lline;

	/* Load the first buffer, skipping over
	 * blank lines (if any).
	 */
	res = init_buffer();
	
	/* If we happen to have a buffer which is LAST
	 * and empty, we have a blank page to print:
	 * just say goodbye.
	 */
	if(res == LAST)return;

	/* Skip enough lines to reach the start of
	 * the first stripe to print.
	 */
	skiplines(gendata.curvline, BWTOPSTART);
	lline = gendata.curvline;
	
	/* And now print the first buffer. This part of
	 * the code is identical to the LHDATA part in
	 * print_color_page()
	 */
	calcbufmargins(LEFT);
	gendata.ileave = 0;
	encode_bw_buf();
	lline = gendata.curvline;
	if(gendata.yres == 1200)
	{
		finalizeheader(1, LEFT);
		gendata.ileave = 1;
		encode_bw_buf();
		lline++;
	}

	/* And now load another buffer, starting to
	 * look for it from the first line after the 
	 * pass we have just done.
	 */
	res = fill_mono_buffer(gendata.curvline + gendata.numblines);

	/* Now loop. Even this code is identical
	 * to the code above: the only difference
	 * is that here we also finalize the previous
	 * buffer before encoding this one. No need
	 * to check if the buffer is empty because
	 * a buffer is reported only if it's full
	 * or if it is the last one, so if it's not
	 * the last it must be full.
	 */
	while(!(res & LAST))
	{
		calcbufmargins(LEFT);
		finalizeheader(gendata.curvline - lline, LEFT);
		gendata.ileave = 0;
		encode_bw_buf();
		lline = gendata.curvline;
		if(gendata.yres == 1200)
		{
			finalizeheader(1, LEFT);
			gendata.ileave = 1;
			encode_bw_buf();
			lline++;
		}

		/* Get another buffer, and so on */
		res = fill_mono_buffer(gendata.curvline + gendata.numblines);
	}

	/* Last buffer. This can be either empty or full.
	 * If it's not empty (LHDATA bit set), print it.
	 */
	if(res & LHDATA)
	{
		calcbufmargins(LEFT);
		finalizeheader(gendata.curvline - lline, LEFT);
		encode_bw_buf();	
		if(gendata.yres == 1200)
		{
			finalizeheader(1, LEFT);
			gendata.ileave = 1;
			encode_bw_buf();
			lline++;
		}
	}

	/* Finalize the last buffer */
	finalizeheader(0, -1);
}

/* This is the equivalent of print_color_page() 
 * for photoquality output. They are almost identical,
 * the only real difference is that we now are
 * printing with two identical heads, so there is
 * no need to care for different heights of the
 * printing pens (i.e.: no "lastblack" tricks).
 */ 
static void
print_photo_page(void)
{
	int res, lline;

	res = init_buffer();
	while(res == 0)res = roll_buffer();

	if(res == LAST)return;

	skiplines(gendata.curvline, COLTOPSTART);
	lline = gendata.curvline;
	
	switch(res)
	{
		case LHDATA:
			calcbufmargins(LEFT);
			gendata.ileave = 0;
			encode_col_buf(LEFT);
			lline = gendata.curvline;
			if(gendata.yres == 1200)
			{
				finalizeheader(1, LEFT);
				gendata.ileave = 1;
				encode_col_buf(LEFT);
				lline++;
			}
			break;

		case RHDATA:
			calcbufmargins(RIGHT);
			gendata.ileave = 0;
			encode_col_buf(RIGHT);
			lline = gendata.curvline;
			if(gendata.yres == 1200)
			{
				finalizeheader(1, RIGHT);
				gendata.ileave = 1;
				encode_col_buf(RIGHT);
				lline++;
			}
			break;

		case LHDATA|RHDATA:
			calcbufmargins(LEFT);
			gendata.ileave = 0;
			encode_col_buf(LEFT);
			calcbufmargins(RIGHT);
			finalizeheader(0, RIGHT);
			encode_col_buf(RIGHT);
			lline = gendata.curvline;
			if(gendata.yres == 1200)
			{
				calcbufmargins(LEFT);
				finalizeheader(1, LEFT);
				gendata.ileave = 1;
				encode_col_buf(LEFT);
				calcbufmargins(RIGHT);
				finalizeheader(0, RIGHT);
				encode_col_buf(RIGHT);
				lline++;
			}

			break;
	}
	
	res = roll_buffer();

	while(!(res & LAST))
	{
		switch(res)
		{
			case LHDATA:
				calcbufmargins(LEFT);
				finalizeheader(gendata.curvline - lline, LEFT);
				gendata.ileave = 0;
				encode_col_buf(LEFT);
				lline = gendata.curvline;
				if(gendata.yres == 1200)
				{
					finalizeheader(1, LEFT);
					gendata.ileave = 1;
					encode_col_buf(LEFT);
					lline++;
				}
				break;

			case RHDATA:
				calcbufmargins(RIGHT);
				finalizeheader(gendata.curvline - lline, RIGHT);
				gendata.ileave = 0;
				encode_col_buf(RIGHT);
				lline = gendata.curvline;
				if(gendata.yres == 1200)
				{
					finalizeheader(1, RIGHT);
					gendata.ileave = 1;
					encode_col_buf(RIGHT);
					lline++;
				}
				break;
				
			case LHDATA|RHDATA:
				calcbufmargins(LEFT);
				finalizeheader(gendata.curvline - lline, LEFT);
				gendata.ileave = 0;
				encode_col_buf(LEFT);
				calcbufmargins(RIGHT);
				finalizeheader(0, RIGHT);
				encode_col_buf(RIGHT);
				lline = gendata.curvline;
				if(gendata.yres == 1200)
				{
					calcbufmargins(LEFT);
					finalizeheader(1, LEFT);
					gendata.ileave = 1;
					encode_col_buf(LEFT);
					calcbufmargins(RIGHT);
					finalizeheader(0, RIGHT);
					encode_col_buf(RIGHT);
					lline++;
				}
				break;
		}
		
		res = roll_buffer();
	}

	switch(res)
	{
		case LHDATA:
			calcbufmargins(LEFT);
			finalizeheader(gendata.curvline - lline, LEFT);
			gendata.ileave = 0;
			encode_col_buf(LEFT);
			if(gendata.yres == 1200)
			{
				finalizeheader(1, LEFT);
				gendata.ileave = 1;
				encode_col_buf(LEFT);
				lline++;
			}

		case RHDATA:
			calcbufmargins(RIGHT);
			finalizeheader(gendata.curvline - lline, RIGHT);
			gendata.ileave = 0;
			encode_col_buf(RIGHT);
			if(gendata.yres == 1200)
			{
				finalizeheader(1, RIGHT);
				gendata.ileave = 1;
				encode_col_buf(RIGHT);
				lline++;
			}
			break;
			
		case LHDATA|RHDATA:
			calcbufmargins(LEFT);
			finalizeheader(gendata.curvline - lline, LEFT);
			gendata.ileave = 0;
			encode_col_buf(LEFT);
			calcbufmargins(RIGHT);
			finalizeheader(0, RIGHT);
			encode_col_buf(RIGHT);
			if(gendata.yres == 1200)
			{
				calcbufmargins(LEFT);
				finalizeheader(1, LEFT);
				gendata.ileave = 1;
				encode_col_buf(LEFT);
				calcbufmargins(RIGHT);
				finalizeheader(0, RIGHT);
				encode_col_buf(RIGHT);
				lline++;
			}
			break;
	}

	finalizeheader(0, -1);
}
