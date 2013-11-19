/* BJC-210/240/250/265/1000 Bubble Jet Printer driver for GhostScript
 * utility subroutines and dithering code
 *
 * Copyright 2000, 2001, 2002 Gergely Szász (Gergely Sza'sz)
 * mailto://szaszg@hu.inter.net http://bjc250gs.sourceforge.net
 *
 *   This program may be distributed and/or modified under the terms of
 *   the GNU General Public License as published by the Free Software
 *   Foundation (the "GPL"); either version 2 of the GPL, or (at your option)
 *   any later version.
 *
 *   When distributed under the terms of the GPL, this program is distributed
 *   in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 *   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *   PURPOSE.  See the GPL for more details.
 *
 *   If this program is being distributed under the terms of the GPL, you
 *   should have received a copy of the GPL along with this program, normally
 *   in a plain ASCII text file named COPYING; if not, write to the Free
 *   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111
 *   U.S.A.
 */
 
/* Copyright (C) 1989, 2000 Aladdin Enterprises.  All rights reserved.

   This program may also be distributed as part of AFPL Ghostscript, under the
   terms of the Aladdin Free Public License (the "License").

   AFPL Ghostscript is distributed with NO WARRANTY OF ANY KIND.  No
   author or distributor accepts any responsibility for the consequences of
   using it, or for whether it serves any particular purpose or works at
   all, unless he or she says so in writing.  Refer to the License for full
   details.

   Every copy of AFPL Ghostscript must include a copy of the License,
   normally in a plain ASCII text file named PUBLIC.  The License grants you
   the right to copy, modify and redistribute AFPL Ghostscript, but only
   under certain conditions described in the License.  Among other things,
   the License requires that the copyright notice and this notice be
   preserved on all copies.
*/

/* BJC printer drivers utilities */
#include "gdevprn.h"
#include "gdevbjc_.h"
#include <math.h>
#include <time.h>

static void bjc_put_bytes(FILE *file, const char *data, int count);
static void bjc_put_hi_lo(FILE *file, int value);
static void bjc_put_lo_hi(FILE *file, int value);
static void bjc_put_command(FILE *file, char command, int count);

/* ---------------- Utilities ---------------- */

static void
bjc_put_bytes(FILE *file, const char *data, int count)
{

    fwrite(data, count, 1, file);
}

static void
bjc_put_hi_lo(FILE *file, int value)
{
    fputc(((value & 0xffff) >> 8), file);
    fputc(value & 0xff, file);
}

static void
bjc_put_lo_hi(FILE *file, int value)
{
    fputc(value & 0xff, file);
    fputc(((value & 0xffff) >> 8), file);
}

static void
bjc_put_command(FILE *file, char command, int count)
{   char tmp[3] = { '\033', '(', ' '};
    tmp[2] = command;
    fwrite( tmp, 3, 1, file);
    bjc_put_lo_hi(file, count);
}

/* ---------------- Commands ---------------- */

/* Line feed (^J) */
void
bjc_put_LF(FILE *file)
{
    fputc(0x0a, file);
}

/* Form feed (^L) */
void
bjc_put_FF(FILE *file)
{
    fputc(0x0c, file);
}

/* Carriage return (^M) */
void
bjc_put_CR(FILE *file)
{
    fputc(0x0d, file);
}

/* Return to initial condition (ESC @) */
void
bjc_put_initialize(FILE *file)
{
    bjc_put_bytes(file, "\033@", 2);
}

/* Set initial condition (ESC [ K <count> <init> <id> <parm1> <parm2>) */
void
bjc_put_set_initial(FILE *file)
{
    bjc_put_bytes(file, "\033[K\002\000\000\017", 7);
}

/* Set data compression (ESC [ b <count> <state>) */
void
bjc_put_set_compression(FILE *file, char compression)
{
    bjc_put_command(file, 'b', 1);
    fputc(compression, file);
}

/* Select print method (ESC ( c <count> <parm1> <parm2> [<parm3>]) */
void
bjc_put_print_method_short(FILE *file, char color)
{
    bjc_put_command(file, 'c', 1);
    fputc(color, file);
}
void
bjc_put_print_method(FILE *file, char color, char media, char quality,
		     char density)
{
    bjc_put_command(file, 'c', 2 + (density != 0));
    fputc(color, file);
    fputc(media | quality, file);
    if (density) 
	fputc(density, file);
}

/* Set raster resolution (ESC ( d <count> <y_res> [<x_res>]) */
void
bjc_put_raster_resolution(FILE *file, int x_resolution, int y_resolution)
{
    if (x_resolution == y_resolution) {
	bjc_put_command(file, 'd', 2);
    } else {
	bjc_put_command(file, 'd', 4);
	bjc_put_hi_lo(file, y_resolution);
    }
    bjc_put_hi_lo(file, x_resolution);
}

/* Raster skip (ESC ( e <count> <skip>) */
void
bjc_put_raster_skip(FILE *file, int skip)
{
    bjc_put_command(file, 'e', 2);
    bjc_put_hi_lo(file, skip);
}

/* Set page margins (ESC ( g <count> <length> <lm> <rm> <top>) */
void
bjc_put_page_margins(FILE *file, int length, int lm, int rm, int top)
{
    char parms[4];

    parms[0] = length, parms[1] = lm, parms[2] = rm, parms[3] = top;
/*    count = 4;       */ 	/* could be 1..3 */
    bjc_put_command(file, 'g', 4);
    bjc_put_bytes(file, parms, 4);
}

/* Set media supply method (ESC * l <count> <parm1> <parm2>) */
void
bjc_put_media_supply(FILE *file, char supply, char type)
{
    bjc_put_command(file, 'l', 2);
    fputc(supply, file);
    fputc(type << 4, file);
}

/* Identify ink cartridge (ESC ( m <count> <type>) */ /*
void
bjc_put_identify_cartridge(FILE *file,
			   bjc_identify_cartridge_command_t command)
{
    bjc_put_command(s, 'm', 1);
    spputc(s, command);
}                      */

/* CMYK raster image (ESC ( A <count> <color>) */
void
bjc_put_cmyk_image(FILE *file, char component,
		   const char *data, int count)
{
    bjc_put_command(file, 'A', count + 1);
    fputc(component, file);
    bjc_put_bytes(file, data, count);
}

/* Move by raster lines (ESC ( n <count> <lines>) */
void
bjc_put_move_lines(FILE *file, int lines)
{
    bjc_put_command(file, 'n', 2);
    bjc_put_hi_lo(file, lines);
}

/* Set unit for movement by raster lines (ESC ( o <count> <unit>) */
void
bjc_put_move_lines_unit(FILE *file, int unit)
{
    bjc_put_command(file, 'o', 2);
    bjc_put_hi_lo(file, unit);
}

/* Set extended margins (ESC ( p <count> <length60ths> <lm60ths> */
/*   <rm60ths> <top60ths>) */
void
bjc_put_extended_margins(FILE *file, int length, int lm, int rm, int top)
{
    bjc_put_command(file, 'p', 8);
    bjc_put_hi_lo(file, length);
    bjc_put_hi_lo(file, lm);
    bjc_put_hi_lo(file, rm);
    bjc_put_hi_lo(file, top);
}

/* Set image format (ESC ( t <count> <depth> <format> <ink>) */
void
bjc_put_image_format(FILE *file, char depth, char format, char ink)
{
    bjc_put_command(file, 't', 3);
    fputc(depth, file);
    fputc(format, file);
    fputc(ink, file);
}

/* Page ID (ESC ( q <count> <id>) */
void
bjc_put_page_id(FILE *file, int id)
{
    bjc_put_command(file, 'q', 1);
    fputc(id, file);
}

/* Continue raster image (ESC ( F <count> <data>) */
void
bjc_put_continue_image(FILE *file, const char *data, int count)
{
    bjc_put_command(file, 'F', count);
    bjc_put_bytes(file, data, count);
}

/* BJ indexed image (ESC ( f <count> R <dot_rows> <dot_cols> <layers> */
/*   <index>) */
void
bjc_put_indexed_image(FILE *file, int dot_rows, int dot_cols, int layers)
{
    bjc_put_command(file, 'f', 5);
    fputc('R', file);			/* per spec */
    fputc(dot_rows, file);
    fputc(dot_cols, file);
    fputc(layers, file);
}


/* ------------------------------------------------------------------ */

/* Invert a raster line ( we need it for Black -> K ) */
bool
bjc_invert_bytes(byte *row, uint raster, bool inverse, byte lastmask)
{   bool ret=false;

    for(; raster > 1; row++, raster--) {
	if(!(inverse)) *row = ~(*row);
	if(*row) ret = true;
    }
	if(!(inverse)) *row ^= 0xff;
                       *row &= lastmask;
    return ret;
}

bool
bjc_invert_cmyk_bytes(byte *rowC, byte *rowM, byte *rowY, byte *rowK,
                      uint raster, bool inverse, byte lastmask,
                     skip_t *skip)
{   bool ret=false;
    byte tmpC, tmpM, tmpY;

    skip->skipC=false;
    skip->skipM=false;
    skip->skipY=false;
    skip->skipK=false;

    for(; raster > 1; rowC++, rowM++, rowY++, rowK++, raster--) {
	if(inverse) {
	              tmpC = ~(*rowC|*rowK);
	              tmpM = ~(*rowM|*rowK);
		      tmpY = ~(*rowY|*rowK);
		     *rowK = ~(*rowC|*rowM|*rowY|*rowK);
                     *rowC = tmpC;
                     *rowM = tmpM;
                     *rowY = tmpY;
        }
        if(*rowC) skip->skipC=true;
        if(*rowM) skip->skipM=true;
        if(*rowY) skip->skipY=true;
        if(*rowK) skip->skipK=true;
	if(*rowC|*rowM|*rowY|*rowK) ret = true;
    }
    return ret;
}

/* "1D runlength compression for BJC-600
 *  this code is borrowed from gdevpcl.c:gdev_pcl_mode2compress."
 * I copy it from gdevcdj.c
 * It is return with the compressed length. 'compressed' point to the
 * compression buffer
 */
uint
bjc_compress(const byte *row, uint raster, byte *compressed)
{
  const byte *end_row = row;
  register const byte *exam = row;
  register byte *cptr = compressed; /* output pointer into compressed bytes */
  
  end_row += raster;

  while ( exam < end_row ) {
    /* Search ahead in the input looking for a run */
    /* of at least 4 identical bytes. */
    const byte *compr = exam;
    const byte *end_dis;
    const byte *next;
    register byte test, test2;

    test = *exam;
    while ( exam < end_row ) {
      test2 = *++exam;
      if ( test == test2 )
	  break;
      test = test2;
    }
    

    /* Find out how long the run is */
    end_dis = exam - 1;
    if ( exam == end_row ) { /* no run */
      next = --end_row;
    } else { 

      next = exam + 1;
      while ( next < end_row && *next == test ) next++;
    }
    

    /* Now [compr..end_dis) should be encoded as dissimilar, */
    /* and [end_dis..next) should be encoded as similar. */
    /* Note that either of these ranges may be empty. */
    

    for ( ; ; ) {	/* Encode up to 128 dissimilar bytes */
      uint count = end_dis - compr; /* uint for faster switch */
      switch ( count ) { /* Use memcpy only if it's worthwhile. */
      case 6: cptr[6] = compr[5];
      case 5: cptr[5] = compr[4];
      case 4: cptr[4] = compr[3];
      case 3: cptr[3] = compr[2];
      case 2: cptr[2] = compr[1];
      case 1: cptr[1] = compr[0];
	*cptr = count - 1;
	cptr += count + 1;
      case 0: /* all done */
	break;
      default:
	if ( count > 128 ) count = 128;
	*cptr++ = count - 1;
	memcpy(cptr, compr, count);
	cptr += count, compr += count;
	continue;
      }
      break;
    }
    

    {	/* Encode up to 128 similar bytes. */
      /* Note that count may be <0 at end of row. */
      int count = next - end_dis;
      if (next < end_row || test != 0)
	while ( count > 0 ) { 

	  int this = (count > 128 ? 128 : count);
	  *cptr++ = 257 - this;
	  *cptr++ = (byte)test;
	  count -= this;
	}
      exam = next;
    }
  }
  return (uint)(cptr - compressed);
}

void bjc_rgb_to_cmy(byte r, byte g, byte b,
                     int *c, int *m, int *y)
{   *c=255-r;
    *m=255-g;
    *y=255-b;
}

void bjc_rgb_to_gray(byte r, byte g, byte b,
		     int *k)
{
    *k = ( (int)r * 77 + (int)g * 151 + (int)b * 28) >> 8;
}

int bjc_gamma_tableC[256];
int bjc_gamma_tableM[256];
int bjc_gamma_tableY[256];
#define bjc_gamma_tableK bjc_gamma_tableC

void bjc_build_gamma_table(float gamma, char color)
{ int i;
  int *table;

    switch(color) {
    case CMYK_C:
	table = bjc_gamma_tableC;
	break;
    case CMYK_M:
	table = bjc_gamma_tableM;
	break;
    case CMYK_Y:
	table = bjc_gamma_tableY;
	break;
    case CMYK_K:
    default:
        table = bjc_gamma_tableK;
        break;
    }

    if(gamma == 1.0) for (i = 0; i < 256; i++) table[i] = (255 - i) << 4;
    else for (i = 0; i < 256; i++) table[i] =
                  4080 - (int)(pow((double)i / 255.0, gamma) * 4080.0 + .5);
}

/* -------------------------------------------------------------------------*/
/*                 Subroutines and tables for randomization                 */
/* -------------------------------------------------------------------------*/

int bjc_rand_seed[55] = {
3627, 15177, 6104, 15555, 14210, 9940, 11987, 7070, 6147, 15691, 14536, 12896,
8959, 14926, 9034, 13544, 13665, 3175, 10177, 14856, 16042, 4265, 13976, 10805,
14817, 8216,  695, 8656, 9189, 15304, 1469, 9641, 1648, 16218, 12421, 5451,
255, 11268, 16121, 11645, 1855, 5982, 9983, 1052, 5255, 15264, 6123, 3577,
9712, 14629, 4593, 15670
};
int bjc_j=0, bjc_k=31;

int bjc_treshold[1024];

uint bjc_rand(void)
{
    uint ret=bjc_rand_seed[bjc_j] = bjc_rand_seed[bjc_j++] +
                                        bjc_rand_seed[bjc_k++];
    if(bjc_j==55) bjc_j = 0;
    if(bjc_k==55) bjc_k = 0;
    return ret & 0x03ff;
}                                             /* random numbers 0-1023 */


void bjc_init_tresh(int rnd)
{
    int i=(int)(time(NULL) & 0x0ff);
    float delta=40.64*rnd;
    for(;i>0;i--) bjc_rand();
    for(i=-512; i<512; i++) bjc_treshold[i+512] =
                                (int)(delta * i / 1024.0 + 2040);
}                      /* init treshold array ~rnd% around halfway (127*16) */


/* Declarations for Floyd-Steinberg dithering.
 *
 * Errors are accumulated into the array fserrors[], at a resolution of
 * 1/16th of a pixel count.  The error at a given pixel is propagated
 * to its not-yet-processed neighbors using the standard F-S fractions,
 *		...	(here)	7/16
 *		3/16	5/16	1/16
 * We work left-to-right on even rows, right-to-left on odd rows.
 *
 * We can get away with a single array (holding one row's worth of errors)
 * by using it to store the current row's errors at pixel columns not yet
 * processed, but the next row's errors at columns already processed.  We
 * need only a few extra variables to hold the errors immediately around the
 * current column.  (If we are lucky, those variables are in registers, but
 * even if not, they're probably cheaper to access than array elements are.)
 *
 * The fserrors[] array has (#columns + 2) entries; the extra entry at
 * each end saves us from special-casing the first and last pixels.
 * Each entry is three values long, one value for each color component.
 */

bool FloydSteinbergDirectionForward = true;

int *FloydSteinbergErrorsC;
int *FloydSteinbergErrorsM;
int *FloydSteinbergErrorsY;
int *FloydSteinbergErrorsK;
int *FloydSteinbergErrorsG;

int FloydSteinbergC;
int FloydSteinbergM;
int FloydSteinbergY;
int FloydSteinbergK;
int FloydSteinbergG;

int
FloydSteinbergInitG(gx_device_printer * pdev)
{  int i;
#define ppdev ((gx_device_bjc_printer *) pdev)

FloydSteinbergErrorsG = (int *) gs_alloc_bytes(pdev->memory,
                                              sizeof(int)*(pdev->width+3),
                                              "bjc error buffer");
if (FloydSteinbergErrorsG == 0)		/* can't allocate error buffer */
	return -1;
FloydSteinbergDirectionForward=true;

for (i=0; i < pdev->width+3; i++) FloydSteinbergErrorsG[i] = 0;
                                                              /* clear */
bjc_rgb_to_gray(ppdev->paperColor.red,
		ppdev->paperColor.green,
		ppdev->paperColor.blue,
                &FloydSteinbergG);
FloydSteinbergG = (255 - FloydSteinbergG) << 4;  /* Maybe */
bjc_init_tresh(ppdev->rnd);
return 0;
#undef ppdev
}

void
FloydSteinbergDitheringG(byte *row, byte *dithered, uint width,
			uint raster, bool limit_extr)
{ 
    byte byteG=0, bitmask = 0x80; /* first bit */
    int i;
    int error = 0, delta;
    int err_corr;
    int *err_vect;

    if (FloydSteinbergDirectionForward) {
        /* First  point */
        err_vect = FloydSteinbergErrorsG + 1;

	for( i=width; i>0; i--, row++, err_vect++) { /* i, sample, error */
	    err_corr = bjc_gamma_tableK[255-(*row)] + FloydSteinbergG;
            if(err_corr > 4080 && limit_extr) err_corr = 4080;
	    error += err_corr + *(err_vect+1);     /* the error in 1/16 */

            if(error > bjc_treshold[bjc_rand()])  {
                error -=  4080;
                byteG |=  bitmask;
            }

            *(err_vect+1) = (error + 8) >> 4;
            delta = error << 1;                                 /* 2 err */
            error += delta;                                     /* 3/16  */
            *(err_vect-1) += (error + 8) >> 4;
            error += delta;                                     /* 5/16  */
            *err_vect += (error + 8) >> 4;
            error += delta + 8;                                 /* 7/16  */
            error >>= 4;

            if (bitmask == 0x01) {
                *dithered = byteG;
                bitmask = 0x80;
                byteG = 0;
                dithered++;
            }
            else if (i == 1) {
                *dithered = byteG;
            }
            else bitmask >>= 1;
	}
        FloydSteinbergDirectionForward=false;
    }
    else {
        row += width - 1;                   /* point to the end of the row */
        dithered += raster - 1;
        bitmask = 1 << ((raster << 3 ) - width) ;
        err_vect = FloydSteinbergErrorsG + width + 1;

	for( i=width; i>0; i--, row--, err_vect--) {
	    err_corr = bjc_gamma_tableK[255-(*row)] + FloydSteinbergG;
            if(err_corr > 4080 && limit_extr) err_corr = 4080;

            error += err_corr + *(err_vect - 1);

            if(error > bjc_treshold[bjc_rand()])  {
                error -=  4080;
                byteG |=  bitmask;
            }

            *(err_vect-1) = (error + 8) >> 4;                  /* 1/16 */
            delta = error << 1;                                 /* 2 err */
            error += delta;
            *(err_vect+1) += (error +8)  >> 4;                   /* 3/16  */
            error += delta;
            *err_vect += (error + 8)  >> 4;                      /* 5/16  */
            error += delta + 8;                                  /* 7/16  */
            error >>= 4;

            if (bitmask == 0x80) {
                *dithered = byteG;
                bitmask = 0x01;
                byteG = 0;
                dithered--;
            } else if(i==1) {
                *dithered = byteG;
            }
            else bitmask <<= 1;
	}
        FloydSteinbergDirectionForward=true;
    }
}

void FloydSteinbergCloseG(gx_device_printer *pdev)
{
    gs_free_object(pdev->memory, FloydSteinbergErrorsG, "bjc error buffer");
}

int
FloydSteinbergInitC(gx_device_printer * pdev)
{ int i;
#define ppdev ((gx_device_bjc_printer *) pdev)

FloydSteinbergErrorsC = (int *) gs_alloc_bytes(pdev->memory,
                                            3*sizeof(int)*(pdev->width+3),
                                              "bjc CMY error buffer");
if (FloydSteinbergErrorsC == 0 )      	/* can't allocate error buffer */
	return -1;

for (i=0; i < 3 * (pdev->width+3); i++) FloydSteinbergErrorsC[i] = 0;

FloydSteinbergDirectionForward=true;
bjc_rgb_to_cmy(ppdev->paperColor.red,
	       ppdev->paperColor.green,
	       ppdev->paperColor.blue,
	       &FloydSteinbergC,
	       &FloydSteinbergM,
               &FloydSteinbergY);

FloydSteinbergC <<= 4;
FloydSteinbergM <<= 4;
FloydSteinbergY <<= 4;
bjc_init_tresh(ppdev->rnd);
return 0;
#undef ppdev
}

void
FloydSteinbergDitheringC(byte *row, byte *dithered, uint width,
			uint raster, bool limit_extr, bool composeK)
{   byte byteC=0, byteM=0, byteY=0, byteK=0, bitmask = 0x80; /* first bit */
    int i;
    int errorC = 0, errorM = 0, errorY = 0, delta;
    int err_corrC, err_corrM, err_corrY;
    int *err_vect;

    if (FloydSteinbergDirectionForward) {
        err_vect = FloydSteinbergErrorsC + 3;         /* errCMY */
	/* First  point */

        for( i=width; i>0; i--, row+=4, err_vect+=3) { /*separate components */

/*                                          C     +       K           */
            err_corrC = bjc_gamma_tableC[ (*row)    + (*(row+3))]
                          + FloydSteinbergC;
            err_corrM = bjc_gamma_tableM[(*(row+1)) + (*(row+3))]
                          + FloydSteinbergM;
            err_corrY = bjc_gamma_tableY[(*(row+2)) + (*(row+3))]
                          + FloydSteinbergY;

            if(err_corrC > 4080 && limit_extr) err_corrC = 4080;
            if(err_corrM > 4080 && limit_extr) err_corrM = 4080;
            if(err_corrY > 4080 && limit_extr) err_corrY = 4080;

            errorC += err_corrC + (*(err_vect + 3));       /* CMYCMYCMY */
            errorM += err_corrM + (*(err_vect + 4));       /* |  ^  !   */
            errorY += err_corrY + (*(err_vect + 5));

            if(errorC > bjc_treshold[bjc_rand()])  {
                errorC -=  4080;
                byteC |=  bitmask;
            }

            if(errorM > bjc_treshold[bjc_rand()])  {
                errorM -=  4080;
                byteM |=  bitmask;
            }

            if(errorY > bjc_treshold[bjc_rand()])  {
                errorY -=  4080;
                byteY |=  bitmask;
            }

            *(err_vect+3) = (errorC + 8) >> 4;                   /* 1/16 */
            delta = errorC << 1;                                 /* 2 err */
            errorC += delta;
            *(err_vect-3) += (errorC + 8) >> 4;                  /* 3/16  */
            errorC += delta;
            *err_vect += (errorC + 8) >> 4;                           /* 5/16  */
            errorC += delta + 8;                                  /* 7/16  */
            errorC >>= 4;

            *(err_vect+4) = (errorM + 8) >> 4;                    /* 1/16 */
            delta = errorM << 1;                                 /* 2 err */
            errorM += delta;
            *(err_vect-2) += (errorM + 8) >> 4;                       /* 3/16  */
            errorM += delta;
            *(err_vect+1) += (errorM + 8) >> 4;                           /* 5/16  */
            errorM += delta + 8;                                     /* 7/16  */
            errorM >>= 4;

            *(err_vect+5) = (errorY + 8) >> 4;                      /* 1/16 */
            delta = errorY << 1;                                 /* 2 err */
            errorY += delta;
            *(err_vect-1) += (errorY + 8) >> 4;                       /* 3/16  */
            errorY += delta;
            *(err_vect+2) += (errorY + 8) >> 4;                           /* 5/16  */
            errorY += delta + 8;                                     /* 7/16  */
            errorY >>= 4;

            if (bitmask == 0x01) {
                bitmask = 0x80;
                if(composeK) {
                    byteK = byteC & byteM & byteY;
                    byteC = byteC & ~byteK;
                    byteM = byteM & ~byteK;
                    byteY = byteY & ~byteK;
                }                               /* if no K byteK always 0 */
                *dithered            = byteC;
                *(dithered+  raster) = byteM;
                *(dithered+2*raster) = byteY;
                *(dithered+3*raster) = byteK;
                byteC = byteM = byteY = byteK = 0;
                dithered++;
            }
            else if(i == 1) {
                if(composeK) {
                    byteK = byteC & byteM & byteY;
                    byteC = byteC & ~byteK;
                    byteM = byteM & ~byteK;
                    byteY = byteY & ~byteK;
                }                               /* if no K byteK always 0 */
                *dithered            = byteC;
                *(dithered+  raster) = byteM;
                *(dithered+2*raster) = byteY;
                *(dithered+3*raster) = byteK;
            }
            else bitmask >>= 1;
        }
        FloydSteinbergDirectionForward=false;
    }
    else {
	row += (width << 2) - 4;   /* point to the end of the row */
        dithered += raster - 1;
        err_vect = FloydSteinbergErrorsC + 3 * width + 3;       /* errCMY */
        bitmask = 1 << ((raster << 3 ) - width) ;

	for( i=width; i>0; i--, row-=4, err_vect-=3) {

            err_corrC = bjc_gamma_tableC[  (*row)   + (*(row+3))]
                          + FloydSteinbergC;
            err_corrM = bjc_gamma_tableM[(*(row+1)) + (*(row+3))]
                          + FloydSteinbergM;
            err_corrY = bjc_gamma_tableY[(*(row+2)) + (*(row+3))]
                          + FloydSteinbergY;

            if(err_corrC > 4080 && limit_extr) err_corrC = 4080;
            if(err_corrM > 4080 && limit_extr) err_corrM = 4080;
            if(err_corrY > 4080 && limit_extr) err_corrY = 4080;

            errorC += err_corrC + (*(err_vect - 3));       /* CMYCMYCMY */
            errorM += err_corrM + (*(err_vect - 2));       /* !  ^  |   */
            errorY += err_corrY + (*(err_vect - 1));

            if(errorC > bjc_treshold[bjc_rand()])  {
                errorC -=  4080;
                byteC |=  bitmask;
            }

            if(errorM > bjc_treshold[bjc_rand()])  {
                errorM -=  4080;
                byteM |=  bitmask;
            }

            if(errorY > bjc_treshold[bjc_rand()])  {
                errorY -=  4080;
                byteY |=  bitmask;
            }

            *(err_vect-3) = (errorC + 8) >> 4;                      /* 1/16 */
            delta = errorC << 1;                                 /* 2 err */
            errorC += delta;
            *(err_vect+3) += (errorC + 8) >> 4;                       /* 3/16  */
            errorC += delta;
            *err_vect += (errorC + 8) >> 4;                           /* 5/16  */
            errorC += delta + 8;                                     /* 7/16  */
            errorC >>= 4;

            *(err_vect-2) = (errorM + 8) >> 4;                      /* 1/16 */
            delta = errorM << 1;                                 /* 2 err */
            errorM += delta;
            *(err_vect+4) += (errorM + 8) >> 4;                       /* 3/16  */
            errorM += delta;
            *(err_vect+1) += (errorM + 8) >> 4;                           /* 5/16  */
            errorM += delta + 8;                                     /* 7/16  */
            errorM >>= 4;

            *(err_vect-1) = (errorY + 8) >> 4;                      /* 1/16 */
            delta = errorY << 1;                                 /* 2 err */
            errorY += delta;
            *(err_vect+5) += (errorY + 8) >> 4;                       /* 3/16  */
            errorY += delta;
            *(err_vect+2) += (errorY + 8) >> 4;                           /* 5/16  */
            errorY += delta + 8;                                     /* 7/16  */
            errorY >>= 4;

            if (bitmask == 0x80) {
                bitmask = 0x01;
                if(composeK) {
                    byteK = byteC & byteM & byteY;
                    byteC = byteC & ~byteK;
                    byteM = byteM & ~byteK;
                    byteY = byteY & ~byteK;
                }                               /* if no K byteK always 0 */
                *dithered            = byteC;
                *(dithered+  raster) = byteM;
                *(dithered+2*raster) = byteY;
                *(dithered+3*raster) = byteK;
                byteC = byteM = byteY = byteK = 0;
                dithered--;
            }
            else if(i == 1) {
                if(composeK) {
                    byteK = byteC & byteM & byteY;
                    byteC = byteC & ~byteK;
                    byteM = byteM & ~byteK;
                    byteY = byteY & ~byteK;
                }                               /* if no K byteK always 0 */
                *dithered            = byteC;
                *(dithered+  raster) = byteM;
                *(dithered+2*raster) = byteY;
                *(dithered+3*raster) = byteK;
            }
	    else bitmask <<= 1;
        }
        FloydSteinbergDirectionForward=true;
    }
}

void FloydSteinbergCloseC(gx_device_printer *pdev)
{
    gs_free_object(pdev->memory, FloydSteinbergErrorsC,
                                           "bjc CMY error buffer");
}
