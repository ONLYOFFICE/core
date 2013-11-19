/*
  This file is part of GNU Ghostscript.
  
  GNU Ghostscript is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY.  No author or distributor accepts responsibility to
  anyone for the consequences of using it or for whether it serves any
  particular purpose or works at all, unless he says so in writing.  Refer to
  the GNU General Public License for full details.
  
  Everyone is granted permission to copy, modify and redistribute GNU
  Ghostscript, but only under the conditions described in the GNU General
  Public License.  A copy of this license is supposed to have been given to
  you along with GNU Ghostscript so you can know your rights and
  responsibilities.  It should be in a file named COPYING.  Among other
  things, the copyright notice and this notice must be preserved on all
  copies.
  
  Aladdin Enterprises is not affiliated with the Free Software Foundation or
  the GNU Project.  GNU Ghostscript, as distributed by Aladdin Enterprises,
  does not depend on any other GNU software.
*/

/* gdevgdi.c */
/* SAMSUNG GDI driver for Ghostscript */
#include "gdevprn.h"
#include "gdevpcl.h"


/*
 * You may select a default resolution of 300 or 600 DPI
 * in the makefile, or an actual resolution
 * on the gs command line.
 *
 * If the preprocessor symbol A4 is defined, the default paper size is
 * the European A4 size; otherwise it is the U.S. letter size (8.5"x11").
 *
 * To determine the proper "margin" settings for your printer, see the
 * file align.ps.
 */

/* Define the default, maximum resolutions. */
#ifdef X_DPI
#  define X_DPI2 X_DPI
#else
#  define X_DPI 300
#  define X_DPI2 600
#endif
#ifdef Y_DPI
#  define Y_DPI2 Y_DPI
#else
#  define Y_DPI 300
#  define Y_DPI2 600
#endif

#define GDI_FALSE 0
#define GDI_TRUE  1
#define GDI_MAX_COUNT     31
#define GDI_NO_REPEAT_IDX 0x80000000L
#define GDI_DATA_LENGTH   11
#define GDI_REPEAT_LENGTH 2
#define GDI_BAND_HEIGHT   128
#define GDI_MAX_BAND      66
/*#define GDI_BAND_WIDTH    4928*/
/*#define GDI_BAND_WIDTH_BYTES (((GDI_BAND_WIDTH + 31)/32)*4)*/

#define GDI_PRE_COMP      2
#define GDI_REAL_COMP     0

#define GDI_COMP_NONE     0
#define GDI_COMP_TIFF     3
#define GDI_COMP_SCANLINE 4
#define GDI_COMP_MODITIFF 6
#define GDI_COMP_NOSEND   0x7f

#define GDI_MARGINS_A4	        0.167, 0.167, 0.167, 0.167
#define GDI_MARGINS_LETTER	0.167, 0.167, 0.167, 0.167
/*#define GDI_MARGINS_A4	0.0, 0.0, 0.0, 0.0*/
/*#define GDI_MARGINS_LETTER	0.0, 0.0, 0.0, 0.0*/

/* The number of blank lines that make it worthwhile to reposition */
/* the cursor. */
#define MIN_SKIP_LINES 7

/* We round up the LINE_SIZE to a multiple of a unsigned long for faster scanning. */
#define W sizeof(word)

int GDI_BAND_WIDTH[] = {4768, 4928};

static int gdi_print_page(gx_device_printer *pdev, FILE *prn_stream);
static int gdi_open(gx_device *pdev);
static int gdi_close(gx_device *pdev);

/* The device descriptors */
static dev_proc_open_device(gdi_open);
static dev_proc_close_device(gdi_close);
static dev_proc_print_page(gdi_print_page);

static gx_device_procs prn_gdi_procs =
    prn_params_procs(gdi_open, gdev_prn_output_page, gdi_close,
		   gdev_prn_get_params, gdev_prn_put_params);

gx_device_printer far_data gs_gdi_device =
  prn_device(prn_gdi_procs, "gdi",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, /* paper size (unit : 10/72 inch size) */
	X_DPI2, Y_DPI2,
	0.20, 0.25, 0.25, 0.25,		/* margins filled in by gdi_open */
	1,                      /* color bit */ 
        gdi_print_page);

gx_device_printer far_data gs_samsunggdi_device =
  prn_device(prn_gdi_procs, "samsunggdi",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, /* paper size (unit : 10/72 inch size) */
	X_DPI2, Y_DPI2,
	0.20, 0.25, 0.25, 0.25,		/* margins filled in by gdi_open */
	1,                      /* color bit */ 
        gdi_print_page);

static FILE *WritePJLHeaderData(gx_device_printer *pdev, FILE *fp);
static FILE *WriteBandHeader(FILE *fp, unsigned int usBandNo, 
                     unsigned char ubCompMode, unsigned int usBandWidth, 
                     unsigned int usBandHeight, unsigned long ulBandSize);
static FILE *WriteTrailerData(FILE *fp);
static unsigned long FrameTiffComp(unsigned char *pubDest, unsigned char *pubSrc, 
                               unsigned int usTotalLines, unsigned int usBytesPerLine, 
                               unsigned char ubMode);
static unsigned int  FrameTiff_Comp(unsigned char *lpSrcBuf, unsigned char *lpTgtBuf, 
                               unsigned int nSrcBytes);
static unsigned int  PreTiffComp(unsigned char *lpSrcBuf, unsigned int nSrcBytes);
static long bmp2run(unsigned char *out_buf, unsigned char *in_buf, unsigned int sizeY, unsigned int sizeX, unsigned char ubMode);

#define ppdev ((gx_device_printer *)pdev)

/* Open the printer, adjusting the margins if necessary. */
static int
gdi_open(gx_device *pdev)
{	/* Change the margins if necessary. */
	const float *m = 0;
	bool move_origin = true;

	static const float m_a4[4] = { GDI_MARGINS_A4 };
	static const float m_letter[4] = { GDI_MARGINS_LETTER };
	m = (gdev_pcl_paper_size(pdev) == PAPER_SIZE_A4 ? m_a4 :
	     m_letter);
	move_origin = false;
	
	if ( m != 0 )
	  gx_device_set_margins(pdev, m, move_origin);

	return gdev_prn_open(pdev);
}

/* gdi_close is only here to eject odd numbered pages in duplex mode. */
static int
gdi_close(gx_device *pdev)
{	if ( ppdev->Duplex_set >= 0 && ppdev->Duplex )
	  {	gdev_prn_open_printer(pdev, 1);
		fputs("\033&l0H", ppdev->file) ;
	  }
	return gdev_prn_close(pdev);
}

#undef ppdev

/* ------ Internal routines ------ */

/* Samsung SmartGDI series compresses, and it needs a special sequence to */
/* allow it to specify coordinates at 600 dpi. */
/* It too needs its coordinate system translated slightly. */

static int
gdi_print_page(gx_device_printer *pdev, FILE *prn_stream)
{
        int band_width_bytes;
        int band_height;
        int code=0, i, j, y, num_rows=0, band_num=0;
        int dots_per_inch = (int)pdev->y_pixels_per_inch;
        int raster = gx_device_raster((gx_device *)pdev, true);
        int real_line_width;
        long ul_band_size, ul_comp_size, ul_tiff_size, ul_min_size;
        byte *ibp=NULL, *obp=NULL, *tmp=NULL;
	byte paper_type=0, compression_type;

        switch (gdev_pcl_paper_size((gx_device*)pdev))
	{
	    case PAPER_SIZE_A4 :     paper_type = 0;
	                             break;
	    case PAPER_SIZE_LETTER : paper_type = 1;
	                             break;
	    case PAPER_SIZE_LEGAL :  paper_type = 1;
	                             break;
	    default:
	                             paper_type = 1;
				     break;
	}
	if (dots_per_inch == 600) { /* 600dpi */
            band_width_bytes = (GDI_BAND_WIDTH[paper_type]+31)/32*4;
            band_height      = GDI_BAND_HEIGHT;
	} else {                    /* 300dpi */
            band_width_bytes = (GDI_BAND_WIDTH[paper_type]+31)/32*4/2;
            band_height      = GDI_BAND_HEIGHT*2;
	}

        ul_band_size = band_width_bytes * band_height;
        ibp = (byte *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), ul_band_size, 1, "gdi_print_page");
        obp = (byte *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), ul_band_size*13/10, 1, "gdi_print_page");
        tmp = (byte *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), raster, 1, "gdi_print_page");

        if (!ibp) return_error(gs_error_VMerror);
        if (!obp) return_error(gs_error_VMerror);
        if (!tmp) return_error(gs_error_VMerror);

        if (ibp ==0 || obp == 0) return_error(gs_error_VMerror);

        /* Header Output */
        prn_stream = WritePJLHeaderData(pdev, prn_stream);
        num_rows = dev_print_scan_lines(pdev);
        band_num = (num_rows + band_height -1)/band_height;

        if (raster > band_width_bytes)
            real_line_width = band_width_bytes;
        else 
            real_line_width = raster;
        
        /* Real Data Output */ 
	y = 0;
        for (i=0; i< band_num; i++) {
            memset(ibp, 0x00, ul_band_size);
            memset(obp, 0x00, ul_band_size*13/10);
            for (j=0; j<band_height; j++) {
                memset(tmp, 0x00, raster);
                /*code = gdev_prn_copy_scan_lines(pdev, i*band_height+j, */
		if (y == num_rows) break;
                code = gdev_prn_copy_scan_lines(pdev, y++, 
                                 (byte*)tmp, raster);
                if (code < 0) break;
                memcpy(ibp+j*band_width_bytes, tmp, real_line_width);
            }

	    if ( i>= GDI_MAX_BAND) continue;

            /* Write Band Data 
	       Because of Scanline compression, extract Scanline compression mode */
            /*ul_tiff_size = FrameTiffComp(obp, ibp, band_height, band_width_bytes, GDI_PRE_COMP);*/
            /*ul_scan_size = (unsigned long)bmp2run(obp, ibp, band_height, band_width_bytes, GDI_PRE_COMP);*/
	    /*ul_min_size =  (ul_scan_size > ul_tiff_size) ? ul_tiff_size : ul_scan_size;*/
	    ul_min_size = ul_tiff_size;
	    compression_type = GDI_COMP_MODITIFF;
	    /*compression_type =  (ul_scan_size > ul_tiff_size) ? GDI_COMP_MODITIFF : GDI_COMP_SCANLINE;*/
            switch (compression_type) {
	    case GDI_COMP_MODITIFF:
#define FUDGE_BIG_BANDS
#ifndef FUDGE_BIG_BANDS
                ul_comp_size = FrameTiffComp(obp, ibp, band_height, band_width_bytes, GDI_REAL_COMP);
#else
	      {
		/* Very ugly.  The printer will hose if the compressed
                   band size is over 65536, so we "fudge" the data in
                   this case repeatedly until we get what we want. 

		   The fudge algorithm is simple, this is kinda-sorta
		   RLE, so we just round groups of bits in groups of
		   2, then 3, then 4, etc until the thing works.  */
#define MAXBAND 0xffff
#define ASSERT(x)
		int fudge=0;
		byte *use_band=ibp;
		do {
		  ul_comp_size = FrameTiffComp(obp, use_band, 
					       band_height, band_width_bytes, 
					       GDI_REAL_COMP);
		  if (ul_comp_size > MAXBAND-8) {
		    int f, g, h;
		    if (!fudge) {
		      ASSERT(use_band == ibp);
		      use_band = (byte*)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), ul_band_size, 1, "gdi_print_page/fudge");
		      fudge=1;
		    }
		    memcpy(use_band, ibp, ul_band_size);
		    fudge++;
		    ASSERT(fudge>=2);
		    {
#define FUDGE2(x) ( (((((x)>>6)&0x3)?3:0)<<6)	\
		    | (((((x)>>4)&0x3)?3:0)<<4)	\
		    | (((((x)>>2)&0x3)?3:0)<<2)	\
		    | (((((x)>>0)&0x3)?3:0)) )
#define FUDGE4(x) ( (((((x)>>4)&0xf)?0xf:0)<<4)	\
		    | (((((x)>>0)&0xf)?0xf:0)) )
#define FUDGE8(x) ( (((((x)>>0)&0xff)?0xf:0)) )
#define FUDGE(fudge, x) ( (fudge == 2 ? FUDGE2(x) 	\
			   : fudge == 3 ? FUDGE4(x)	\
			   : fudge == 4 ? FUDGE8(x)	\
			   : 0 ) )

		      for(f=0;f<ul_band_size; f++) {
			use_band[f] = FUDGE(fudge, ibp[f]);
		      }
		    }
		  }
		} while (ul_comp_size > MAXBAND-8);
	      oh_well:
		if (fudge > 1) {
		  ASSERT(use_band != ibp);
		  gs_free(gs_lib_ctx_get_non_gc_memory_t(), use_band, ul_band_size, 1, "gdi_print_page/fudge");
		  /*dprintf2("smartgdi: band %d fudge factor is %d\n", i, fudge);*/
		}
	      }
#endif
		break;
	    case GDI_COMP_SCANLINE:
                ul_comp_size = bmp2run(obp, ibp, band_height, band_width_bytes, GDI_REAL_COMP);
	        break;
	    default:
                ul_comp_size = FrameTiffComp(obp, ibp, band_height, band_width_bytes, GDI_REAL_COMP);
		compression_type = GDI_COMP_MODITIFF;
		break;
	    }

            prn_stream = WriteBandHeader(prn_stream, i, compression_type, (band_width_bytes * 8),
                                         band_height, ul_comp_size); 
            /*dprintf2(prn_stream, "[%d] band, size : %d\n", i, ul_tiff_size);*/
            fwrite(obp, ul_comp_size, 1, prn_stream);
        }
    
        /* Trailer Output */
        WriteTrailerData(prn_stream);
        gs_free(gs_lib_ctx_get_non_gc_memory_t(), ibp, ul_band_size, 1, "gdi_line_buffer");
        gs_free(gs_lib_ctx_get_non_gc_memory_t(), obp, ul_band_size*13/10, 1, "gdi_line_buffer");
        gs_free(gs_lib_ctx_get_non_gc_memory_t(), tmp, raster, 1, "gdi_line_buffer");
        return code;
}

FILE *WritePJLHeaderData(gx_device_printer *pdev, FILE *fp)
{
  unsigned long ulSize;
  unsigned char buffer[300];
  int dots_per_inch = (int)pdev->y_pixels_per_inch;

  strcpy(buffer, "\033%-12345X");
  
  /* Paper Type*/
  strcat(buffer, "@PJL SET PAPERTYPE = NORMAL ON\015\012");
  /*Density*/
  strcat(buffer, "@PJL SET DENSITY = 1\015\012");
  /* Toner Save*/
  strcat(buffer, "@PJL SET TONERSAVE = OFF\015\012");
  /* Enter Language SMART*/
  strcat(buffer, "@PJL ENTER LANGUAGE = SMART\015\012");
  /* JobStart*/
  strcat(buffer, "$PJL JOB START\015\012");

  /* Resolution*/
  if (dots_per_inch == 600)
      strcat(buffer, "$PJL RESOLUTION = 600\015\012");
  else
      strcat(buffer, "$PJL RESOLUTION = 300\015\012");

  /* Copies*/
  strcat(buffer, "$PJL COPIES = 1\015\012");
  /* Paper Size*/
  switch (gdev_pcl_paper_size((gx_device*)pdev)) 
  {
	case PAPER_SIZE_A4:
  			strcat(buffer, "$PJL PAGE A4 AUTO\015\012");
			break;
	case PAPER_SIZE_LETTER:
  			strcat(buffer, "$PJL PAGE LETTER AUTO\015\012");
			break;
	case PAPER_SIZE_LEGAL:
  			strcat(buffer, "$PJL PAGE LEGAL AUTO\015\012");
			break;
	default:
  			strcat(buffer, "$PJL PAGE LETTER AUTO\015\012");
			break;
  }
  /* bitmap start*/
  strcat(buffer, "$PJL BITMAP START\015\012");
  /* write buffer to file.*/
  ulSize = strlen(buffer);
  fwrite(buffer, 1, ulSize, fp );
  return(fp);
} /* WritePJLHeaderData()     */


FILE *WriteBandHeader
(
FILE *fp,
unsigned int  usBandNo,
unsigned char ubCompMode,
unsigned int  usBandWidth,
unsigned int  usBandHeight,
unsigned long ulBandSize
)
{
  unsigned char ubLeft=0;
  unsigned int i = 0;
  unsigned char buf[50];

  memset(buf, 0x00, 50);

  ulBandSize += 8;

  /* bandsize*/
  buf[i++] = (unsigned char)((ulBandSize >> 24) & 0xff);
  buf[i++] = (unsigned char)((ulBandSize >> 16) & 0xff);
  buf[i++] = (unsigned char)((ulBandSize >> 8) & 0xff);
  buf[i++] = (unsigned char)(ulBandSize & 0xff);
  
  /* id        */                    
  buf[i++] = (unsigned char)((usBandNo >> 8) & 0xff);
  buf[i++] = (unsigned char)(usBandNo & 0xff);

  /* compress mode         */
  buf[i++] = (unsigned char)(ubCompMode & 0xff);

  /* ubLeft                  */
  buf[i++] = (unsigned char)(ubLeft & 0xff);

  /* height*/
  buf[i++] = (unsigned char)((usBandHeight >> 8) & 0xff);
  buf[i++] = (unsigned char)(usBandHeight & 0xff);

  /* width   */                  
  buf[i++] = (unsigned char)((usBandWidth >> 8) & 0xff);
  buf[i++] = (unsigned char)(usBandWidth & 0xff);

  fwrite(buf, 1, i, fp);
  return(fp);
} /* end of WriteBandHeader()*/

FILE *WriteTrailerData(FILE *fp)
{
  unsigned long ulSize;
  unsigned long buffer[200];

  memset((char*)buffer, 0x00, 200);
  strcpy((char*)buffer, "$PJL PRINT 4\015\012");
  strcat((char*)buffer, "$PJL EOJ\015\012");
  strcat((char*)buffer, "$PJL SYNC\015\012");
  strcat((char*)buffer, "$PJL RELEASE 0 2047\015\012");
  strcat((char*)buffer, "$PJL GARBAGE\015\012");
  strcat((char*)buffer, "\033%-12345X\015\012");

  ulSize = strlen((char*)buffer);
  fwrite(buffer, 1, ulSize, fp);

  return(fp);
} /* WriteTrailerData()*/

unsigned long FrameTiffComp(unsigned char *pubDest, 
                            unsigned char *pubSrc, 
                            unsigned int usTotalLines, 
                            unsigned int usBytesPerLine,
                            unsigned char ubMode)
{
  unsigned char *TgtPtr, *SrcPtr;
  unsigned int  usLineSize;
  unsigned long ulret;
  unsigned int  i;

  SrcPtr = pubSrc;
  TgtPtr = pubDest;
  ulret = 0;

  for (i = 0; i < usTotalLines; i++)
  {
    if (!(ubMode & 0x02))
    {
      usLineSize = FrameTiff_Comp(SrcPtr, TgtPtr, usBytesPerLine);
    }
    else
    {
      if(i == 0x253)
      {
        i = i;
      }
      usLineSize = PreTiffComp(SrcPtr, usBytesPerLine);
    }
    SrcPtr += usBytesPerLine;
    TgtPtr += usLineSize;
    ulret += usLineSize;
  }

  if (!(ubMode & 0x02))
  {
    switch (ulret%4)
    {
    case 1:
      *TgtPtr++ = 0x00;
      ulret++;
    case 2:
      *TgtPtr++ = 0x00;
      ulret++;
    case 3:
      *TgtPtr++ = 0x00;
      ulret++;
    default:
      break;
    }
  }
  else
  {
    switch (ulret%4)
    {
    case 1:
      ulret++;
    case 2:
      ulret++;
    case 3:
      ulret++;
    default:
      break;
    }
  }
  return(ulret);
}  /* FrameTiffComp()*/

unsigned int FrameTiff_Comp(unsigned char *lpSrcBuf, unsigned char *lpTgtBuf, unsigned int nSrcBytes)
{
  unsigned int usret;
  unsigned int usCount, usEndCnt;
  unsigned int usControl;
  unsigned int usCnt;
  unsigned char ubFirst, ubSecond, ubMisCnt;
  unsigned char *pubDst, *pubSrc, *pubOrg;

  pubDst = lpTgtBuf;
  pubSrc = lpSrcBuf;
  usCount = nSrcBytes;
  while(1)
  {
    if(!usCount)
    {
      break;  /* exit while(1) loop */
    }
    else if (usCount == 1)
    {
      *pubDst++ = 0x00;
      *pubDst++ = *pubSrc++;
      break;
    }

    pubOrg = pubSrc;
    ubFirst = *pubSrc++;
    ubSecond = *pubSrc++;

    if(ubFirst == ubSecond)  /* case of data match */
    {
      usEndCnt = usCount;
      if (usCount > 16384)
      {
        usEndCnt = 16384;
      }
      usEndCnt = usCount - 2;
      while (usEndCnt--)
      {
        if (ubFirst != *pubSrc++)
        {
          pubSrc--;
          break;
        }
      } /* of while */

      /* Save compressed data */
      usCnt = (unsigned int) (pubSrc - pubOrg);
      usCount -= usCnt;
      usCnt -=2;
      if (usCnt >= 64)
      {
        /* save control code code 1100 0000 0000 0000  | usCnt */
        usCnt = (~usCnt & 0x3fff) | 0xc000;
        *pubDst++ = (unsigned char)((usCnt & 0xff00) >> 8);
        *pubDst++ = (unsigned char)(usCnt & 0x00ff);
        *pubDst++ = ubFirst;
      }
      else
      {
        /* save control code 0100 0000 | (unsigned char)(usCnt) */
        usCnt = (~usCnt & 0x7f);
        *pubDst++ = (unsigned char)usCnt;
        *pubDst++ = ubFirst;
      }
    } /* of if (ubFirst == ubSecond) */

    else /* case of data mismatch */
    {
      ubMisCnt = 0;
      if (usCount > 2)
      {
        usEndCnt = usCount;
        if (usCount > 16384)
        {
          usEndCnt = 16384;
        }
        usEndCnt = usCount - 2;
        /* usEndCnt = usCount - 2; original*/
        /* 19990824 by LSM : for end file while (usEndCnt--)*/
        while (usEndCnt--)
        {
          /* read next data */
          ubFirst = ubSecond;
          ubSecond = *pubSrc++; /* read 3rd Data*/
          if (ubFirst == ubSecond)
          {
            if (usEndCnt <= 1)
            {
              ubMisCnt = 2;
              break;
            }
            else
            {
              ubSecond = *pubSrc++; /* read 4th Data*/
              usEndCnt--;
              if (ubFirst == ubSecond)
              {
                ubMisCnt = 3;
                break;
              }
            }
          }
        } /* of while */
      } /* of if (usCount > 2) */
      /* save data */
      usControl = (unsigned int) (pubSrc - pubOrg);
      usControl -= (unsigned int) ubMisCnt;
      if (usControl > usCount)
      {
        usCount = usControl;
      }
      usCount -= usControl;
      usCnt = usControl - 1;
      if ( usCnt >= 64)
      {
        /* save control code 1000 0000 0000 0000 | usCnt */
        usCnt = ((usCnt & 0xbfff) | 0x8000);
        *pubDst++ = (unsigned char)((usCnt & 0xff00) >> 8);
        *pubDst++ = (unsigned char)(usCnt & 0x00ff);
      }
      else
      {
        /* save control code 0000 0000 | (BYTE)usCnt */
        /* and invert it */
        *pubDst++ = (unsigned char)(usCnt & 0x003f);
      }
      pubSrc = pubOrg;
      while (usControl--)
      {
        *pubDst++ = *pubSrc++;
      } /* of while */
    } /* of else */
  } /* of while(1) */

  usret = (unsigned int) (pubDst - lpTgtBuf);
  return (usret);
}

unsigned int PreTiffComp(unsigned char *lpSrcBuf, unsigned int nSrcBytes)
{
  unsigned int usret =0;
  unsigned int usCount, usEndCnt;
  unsigned int usControl;
  unsigned int usCnt;
  unsigned char ubFirst, ubSecond, ubMisCnt;
  unsigned char *pubSrc, *pubOrg;

  pubSrc = lpSrcBuf;
  usCount = nSrcBytes;
  while(1)
  {
    if(!usCount)
    {
      break;  /* exit while(1) loop */
    }
    else if (usCount == 1)
    {
      usret +=2;
      pubSrc++;
      break;
    }

    pubOrg = pubSrc;
    ubFirst = *pubSrc++;
    ubSecond = *pubSrc++;

    if(ubFirst == ubSecond)  /* case of data match */
    {
      usEndCnt = usCount;
      if (usCount > 16384)
      {
        usEndCnt = 16384;
      }
      usEndCnt = usCount - 2;
      while (usEndCnt--)
      {
        if (ubFirst != *pubSrc++)
        {
          pubSrc--;
          break;
        }
      } /* of while */

      /* Save compressed data */
      usCnt = (unsigned int) (pubSrc - pubOrg);
      usCount -= usCnt;
      usCnt -=2;
      if (usCnt >= 64)
      {
        /* save control code code 1100 0000 0000 0000  | usCnt */
        usret +=3;
      }
      else
      {
        /* save control code 0100 0000 | (unsigned char)(usCnt) */
        usret += 2;
      }
    } /* of if (ubFirst == ubSecond) */

    else /* case of data mismatch */
    {
      ubMisCnt = 0;
      if (usCount > 2)
      {
        usEndCnt = usCount;
        if (usCount > 16384)
        {
          usEndCnt = 16384;
        }
        /* usEndCnt = usCount - 2;*/
        usEndCnt = usCount - 2;
        /* 19990824 by LSM : for Last file while (usEndCnt--)*/
        while (usEndCnt--)
        {
          /* read next data */
          ubFirst = ubSecond;
          ubSecond = *pubSrc++; /* read 3rd Data*/
          if (ubFirst == ubSecond)
          {
            if (usEndCnt <= 1)
            {
              ubMisCnt = 2;
              break;
            }
            else
            {
              ubSecond = *pubSrc++; /* read 4th Data*/
              usEndCnt--; /* 19990824 by LSM*/
              if (ubFirst == ubSecond)
              {
                ubMisCnt = 3;
                break;
              }
            }
          }
        } /* of while */
      } /* of if (usCount > 2) */
      /* save data */
      usControl = (unsigned int) (pubSrc - pubOrg);
      usControl -= ubMisCnt;
      /* 19990824 by LSM : for fixing GPF on Photoshop*/
      if (usControl > usCount)
      {
        usControl = usCount;
      }
      usCount -= usControl;
      usCnt = usControl - 1;
      if ( usCnt >= 64)
      {
        /* save control code 1000 0000 0000 0000 | usCnt */
        usret += 2;
      }
      else
      {
        /* save control code 0000 0000 | (BYTE)usCnt */
        /* and invert it */
        usret++;
      }
      pubSrc = pubOrg;
      while (usControl--)
      {
        usret++;
        pubSrc++;
      } /* of while */
    } /* of else */
  } /* of while(1) */
  return (usret);
}

typedef struct 
{
  unsigned char ubDx;
  unsigned char ubRl;
  unsigned char ubLastBit;
}   sc_tbl;

static sc_tbl  gdi_ScanTbl[256] = {
{ 8, 0, 0 }, { 7, 1, 1 }, { 6, 1, 0 }, { 6, 2, 1 },   /* 0x00*/
{ 5, 1, 0 }, { 0, 0, 1 }, { 5, 2, 0 }, { 5, 3, 1 }, 
{ 4, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 4, 2, 0 }, { 0, 0, 1 }, { 4, 3, 0 }, { 4, 4, 1 }, 
{ 3, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x10*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 3, 2, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 3, 3, 0 }, { 0, 0, 1 }, { 3, 4, 0 }, { 3, 5, 1 }, 
{ 2, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x20*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 2, 2, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x30*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 2, 3, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 2, 4, 0 }, { 0, 0, 1 }, { 2, 5, 0 }, { 2, 6, 1 }, 
{ 1, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x40*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x50*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 1, 2, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x60*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 1, 3, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x70*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 1, 4, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 1, 5, 0 }, { 0, 0, 1 }, { 1, 6, 0 }, { 1, 7, 1 }, 
{ 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x80*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x90*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0xa0*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0xb0*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 2, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0xc0*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0xd0*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 3, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0xe0*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 4, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0xf0*/
{ 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 5, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 }, 
{ 0, 6, 0 }, { 0, 0, 1 }, { 0, 7, 0 }, { 0, 8, 1 }, 
};

static sc_tbl  gdi_ScanTbl4[16] = {
{ 4, 0, 0 }, { 3, 1, 1 }, { 2, 1, 0 }, { 2, 2, 1 },   /* 0x00*/
{ 1, 1, 0 }, { 0, 0, 1 }, { 1, 2, 0 }, { 1, 3, 1 },   /* 0x04*/
{ 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 }, { 0, 0, 1 },   /* 0x08*/
{ 0, 2, 0 }, { 0, 0, 1 }, { 0, 3, 0 }, { 0, 4, 1 }    /* 0x0c*/
};

long SaveScanData( unsigned char *, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short );
long UpdateScanSize( unsigned char *, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short );
typedef long (*funcptr)( unsigned char *, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short );

funcptr UpdateScanLine[2] = { SaveScanData, UpdateScanSize };

long Save6Bytes(unsigned char *out_buf, unsigned short usDy, unsigned short usRl, short sDx, unsigned short usWarp)
{
  unsigned long ultmp_dat;
  long  lWarp, lDis;
  unsigned short  ustmp_dat;

  lWarp = (long)(usWarp << 3);
  lDis  = ((long)usDy * lWarp) + (long)sDx;

  /* 1st, 2nd, 3rd & 4th byte*/
  ultmp_dat = 0xc0000000ul;
  if (lDis < 0)
  {
    ultmp_dat |= 0x20000000ul;
  }
  ultmp_dat |= (lDis & 0x1ffffffful);
  *out_buf++ = (unsigned char)((ultmp_dat & 0xff000000ul) >> 24);
  *out_buf++ = (unsigned char)((ultmp_dat & 0xff0000ul) >> 16);
  *out_buf++ = (unsigned char)((ultmp_dat & 0xff00ul) >> 8);
  *out_buf++ = (unsigned char)(ultmp_dat & 0xfful);

  /* 5th & 6th byte*/
  ustmp_dat = 0xc000;
  ustmp_dat |= (usRl & 0x3fff);
  *out_buf++ = (unsigned char)((ustmp_dat & 0xff00) >> 8);
  *out_buf++ = (unsigned char)(ustmp_dat & 0xff);

  return(6);
} /* Save6Bytes()*/

long  Save4Bytes(unsigned char *out_buf, unsigned short usDy, unsigned short usRl, short sDx)
{
  unsigned short  ustmp_dat;

  /* 1st & 2nd byte*/
  ustmp_dat = 0x8000;
  if (sDx < 0)
  {
    ustmp_dat |= 0x2000;
  }
  ustmp_dat |= (sDx & 0x1fff);
  *out_buf++ = (unsigned char)((ustmp_dat & 0xff00) >> 8);
  *out_buf++ = (unsigned char)(ustmp_dat & 0xff);

  /* 3rd & 4th byte*/
  ustmp_dat = 0x8000;
  ustmp_dat |= ((usDy & 0x03) << 12);
  ustmp_dat |= (usRl & 0xfff);
  *out_buf++ = (unsigned char)((ustmp_dat & 0xff00) >> 8);
  *out_buf++ = (unsigned char)(ustmp_dat & 0xff);

  return(4);
}   /* end of Save4Bytes()*/

long  Save2Bytes(unsigned char *out_buf, unsigned short usDy, unsigned short usRl, short sDx)
{
  unsigned char ubtmp_dat;

  /* 1st byte*/
  ubtmp_dat = 0x00;
  if (usDy == 1)
  {
    ubtmp_dat |= 0x40;
  }
  ubtmp_dat |= (usRl & 0x3f);
  *out_buf++ = ubtmp_dat;

  /* 2nd byte*/
  if (sDx < 0)
  {
    ubtmp_dat = 0x80;
  }
  else
  {
    ubtmp_dat = 0x00;
  }
  ubtmp_dat |= ((unsigned char)sDx & 0x7f);
  *out_buf++ = ubtmp_dat;
  return(2);
}   /* end of Save2Bytes()*/

long SaveScanData (unsigned char *out_buf, 
                   unsigned short us1Cnt, 
                   unsigned short usDy, 
                   unsigned short usPosX10, 
                   unsigned short usPosX01, 
                   unsigned short usWarp)
{
  short   sDisX;
  long  lRet;

  sDisX = (int)usPosX01 - (int)usPosX10;

  /* 48 bit*/
  if ( (usDy > 3) || (us1Cnt > 4095) ) 
  {
    Save6Bytes(out_buf, usDy, us1Cnt, sDisX, usWarp);
    lRet = 6;
  }
  /* 32 bit*/
  else if ( (usDy > 1) || (us1Cnt > 63) || (sDisX > 127) || (sDisX < -128) ) 
  {
    Save4Bytes(out_buf, usDy, us1Cnt, sDisX);
    lRet = 4;
  }
  /* 16 bit*/
  else 
  {
    Save2Bytes(out_buf, usDy, us1Cnt, sDisX);
    lRet = 2;
  }
  return(lRet);
} /* end of SaveScanData()*/


long UpdateScanSize (unsigned char *out_buf, 
                     unsigned short us1Cnt, 
                     unsigned short usDy, 
                     unsigned short usPosX10, 
                     unsigned short usPosX01, 
                     unsigned short usWarp)
{
  short  sDisX;
  long  lRet;

  sDisX = usPosX01 - usPosX10;

  /* 48 bit*/
  if ( (usDy > 3) || (us1Cnt > 4095) ) 
  {
    lRet = 6;
  }
  /* 32 bit*/
  else if ( (usDy > 1) || (us1Cnt > 63) || (sDisX > 127) || (sDisX < -128) )
  {
    lRet = 4;
  }
  /* 16 bit*/
  else
  {
    lRet = 2;
  }
  return(lRet);
} /* end of UpdateScanSize() by bglee 19981224*/

long GetSimpleScan(unsigned char *out_buf,
                   unsigned char ubSizeMode, 
                   unsigned short  *us1Count, 
                   unsigned short  *usDy, 
                   unsigned short  *usPosX10, 
                   unsigned short  *usPosX01, 
                   unsigned short  usBytePos,
                   unsigned char ubCrtByte,
                   unsigned char ubSize,
                   unsigned char ubPreBit,
                   unsigned short  usWidth)
{
  long  lScanSize;
  unsigned char ubDx, ubRl, ubLastBit;

  lScanSize = 0;
  if (ubSize == 8) 
  {
    ubDx = gdi_ScanTbl[ubCrtByte].ubDx;
    ubRl = gdi_ScanTbl[ubCrtByte].ubRl;
    ubLastBit = gdi_ScanTbl[ubCrtByte].ubLastBit;
  }
  else 
  {
    ubCrtByte &= 0x0f;
    ubDx = gdi_ScanTbl4[ubCrtByte].ubDx;
    ubRl = gdi_ScanTbl4[ubCrtByte].ubRl;
    ubLastBit = gdi_ScanTbl4[ubCrtByte].ubLastBit;
  }

  
  /* 1 X 1 X*/
  if (ubPreBit) 
  {
    /* 1 0 1 X*/
    if (ubDx) 
    {
      lScanSize += (*UpdateScanLine[ubSizeMode])(out_buf, *us1Count, *usDy, *usPosX10, *usPosX01, usWidth);
      *usPosX10 = usBytePos - *us1Count;
      *usPosX01 = usBytePos + ubDx;
      *us1Count = ubRl;
      *usDy = 0;
      /* 1 0 1 0*/
      if (!ubLastBit) 
      {
        /* 19990330 by bglee*/
        out_buf = out_buf + lScanSize;
        
        lScanSize += (*UpdateScanLine[ubSizeMode])(out_buf, *us1Count, *usDy, *usPosX10, *usPosX01, usWidth);
        *usPosX10 = *usPosX01 ;
        *us1Count = 0;
      }
      /* 1 0 1 1*/
    }
    /* 1 1 1 X*/
    else 
    {
      *us1Count += ubRl;
      /* 1 1 1 0*/
      if (!ubLastBit) 
      {
        lScanSize += (*UpdateScanLine[ubSizeMode])(out_buf, *us1Count, *usDy, *usPosX10, *usPosX01, usWidth);
        *usPosX10 = usBytePos + ubRl - *us1Count;
        *us1Count = 0;
        *usDy = 0;
      }
      /* 1 1 1 1*/
    }
  }
  /* 0 X 1 X*/
  else 
  {
    /* 0 X 1 X*/
    *usPosX01 = usBytePos + ubDx;
    *us1Count += ubRl;
    /* 0 X 1 0*/
    if (!ubLastBit) 
    {
      lScanSize += (*UpdateScanLine[ubSizeMode])(out_buf, *us1Count, *usDy, *usPosX10, *usPosX01, usWidth);
      *usPosX10 = *usPosX01 + ubRl - *us1Count;
      *us1Count = 0;
      *usDy = 0;
    }
    /* 0 X 1 1*/
  }

  return(lScanSize);
} /* end of GetSimpleScan() */


long scan_map (unsigned char *in_buf, 
               unsigned char *out_buf,
               unsigned short usWidth, 
               unsigned short usHeight, 
               unsigned char ubMode) 
{
  unsigned int  i, j, k;
  unsigned char ubPreBit, ubCrtByte;/*, ubLastBit;*/
  long  lScanSize, lTmp;
  long lCrtSize;
  unsigned short  us1Count;
  unsigned short  usPosX01, usPosX10;
  unsigned short  usDy, usBytePos;
  unsigned char ubRevMode, ubSizeMode;
  unsigned char ubTemp;

  usDy = 0;
  usPosX01 = usPosX10 = 0;
  lScanSize = 0;
  ubRevMode = ubMode & 0x01;
  ubSizeMode = (ubMode & 0x02) >> 1;
  for (i = 0; i < usHeight; i++) 
  {
    ubPreBit = 0;
    us1Count = 0;
    for (j = 0; j < usWidth; j++) 
    {
      ubCrtByte = *in_buf++;
      if (ubRevMode)
      {
        ubCrtByte = ~ubCrtByte;
      }

      switch (ubCrtByte) 
      {
        case 0x00:
          /* 1 0 */
          if (ubPreBit) 
          {
            lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, us1Count, usDy, usPosX10, usPosX01, usWidth);
            out_buf = out_buf + lTmp;
            lScanSize += lTmp;
            usPosX10 = (j << 3) - us1Count; /*by pkb*/
            us1Count = 0;
            usDy = 0;
          }
          /* 0 0*/
          break;

        case 0xff:
          /* 1 1*/
          if (ubPreBit)
          {
            us1Count += 8;
          }
          /* 0 1*/
          else 
          {
            us1Count = 8;
            usPosX01 = (j << 3); 
          }
          break;

        default:
          /* X X 1 X*/
          if (gdi_ScanTbl[ubCrtByte].ubRl) 
          {
            usBytePos = (j << 3);
            lTmp = GetSimpleScan(out_buf, ubSizeMode, &us1Count, &usDy, &usPosX10, &usPosX01, usBytePos, ubCrtByte, 8, ubPreBit, usWidth);
            out_buf = out_buf + lTmp;
            lScanSize += lTmp;
          }
          /* complex pattern*/
          else 
          {
            for (k = 0; k < 2; k++) 
            { /*we calculate 4bit*/
              ubTemp = (ubCrtByte >> (4 - (k * 4)) ) & 0x0f;
              usBytePos = (j << 3) + (k << 2);
              switch (ubTemp) 
              {
                case 0x00:
                  /* 1 0*/
                  if (ubPreBit) 
                  {
                    lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, us1Count, usDy, usPosX10, usPosX01, usWidth);
                    out_buf = out_buf + lTmp;
                    lScanSize += lTmp;
                    usPosX10 = usBytePos -  us1Count; 
                    us1Count = 0;
                    usDy = 0;
                  }
                  /* 0 0*/
                  break;

                case 0x0f:
                  /* 1 1*/
                  if (ubPreBit)
                  {
                    us1Count += 4;
                  }
                  /* 0 1*/
                  else 
                  {
                    us1Count = 4;
                    usPosX01 = (j << 3) + (k << 2); 
                  }
                  break;

                case 0x05:
                  /* 1 0101*/
                  if (ubPreBit) 
                  {
                    lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, us1Count, usDy, usPosX10, usPosX01, usWidth);
                    out_buf = out_buf + lTmp;
                    lScanSize += lTmp;

                    usPosX10 = usBytePos - us1Count;
                    usDy = 0;
                  }
                  /* 0 0101*/
                  usPosX01 = usBytePos + 1;
                  lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, 1, usDy, usPosX10, usPosX01, usWidth);
                  out_buf = out_buf + lTmp;
                  lScanSize += lTmp;

                  /* next*/
                  usPosX10 = 0;
                  usPosX01 = 2;
                  usDy = 0;
                  us1Count = 1;
                            break;

                case 0x09:
                  /* 1 1001*/
                  if (ubPreBit)
                  {
                    us1Count++;
                  }
                  /* 0 1001*/
                  else 
                  {
                    usPosX01 = usBytePos;
                    us1Count = 1;
                  }
                  lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, us1Count, usDy, usPosX10, usPosX01, usWidth);
                  out_buf = out_buf + lTmp;
                  lScanSize += lTmp;

                  /* next*/
                  if (ubPreBit)
                  {       
                    usPosX10 = usBytePos - us1Count + 1;
                    usPosX01 = usBytePos + 3;
                  }
                  else
                  {
                    usPosX10 = 0;
                    usPosX01 = 3;
                  } 
                  usDy = 0;
                  us1Count = 1;
                  break;

                case 0x0a:
                  /* 1 1010*/
                  if (ubPreBit)
                  {
                    us1Count++;
                  }
                  /* 0 1010*/
                  else 
                  {
                    us1Count = 1;
                    usPosX01 = usBytePos;
                  }
                  lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, us1Count, usDy, usPosX10, usPosX01, usWidth);
                  out_buf = out_buf + lTmp;
                  lScanSize += lTmp;

                  /* next*/
                  usPosX10 = usBytePos - us1Count + 1;
                  usPosX01 = usBytePos + 2;
                  lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, 1, 0, usPosX10, usPosX01, usWidth);
                  out_buf = out_buf + lTmp;
                  lScanSize += lTmp;
                  /* next*/
                  usPosX10 = usBytePos + 2;
                  usDy = 0;
                  us1Count = 0;
                  break;

                case 0x0b:
                  /* 1 1011*/
                  if (ubPreBit)
                  {
                    us1Count++;
                  }
                  /* 0 1011*/
                  else 
                  {
                    us1Count = 1;
                    usPosX01 = usBytePos;
                  }
                  lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, us1Count, usDy, usPosX10, usPosX01, usWidth);
                  out_buf = out_buf + lTmp;
                  lScanSize += lTmp;

                  /* next*/
                  if (ubPreBit)
                  {       
                    usPosX10 = usBytePos - us1Count + 1;
                    usPosX01 = usBytePos + 2;
                  }
                  else
                  {
                    usPosX10 = 0;
                    usPosX01 = 2;
                  } 

                  usDy = 0;
                  us1Count = 2;
                  break;

                case 0x0d:
                  /* 1 1101*/
                  if (ubPreBit)
                  {
                    us1Count += 2;
                  }
                  /* 0 1101*/
                  else 
                  {
                    us1Count = 2;
                    usPosX01 = usBytePos;
                  }
                  lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, us1Count, usDy, usPosX10, usPosX01, usWidth);
                  out_buf = out_buf + lTmp;
                  lScanSize += lTmp;

                  /* next*/
                  if (ubPreBit)
                  {
                    usPosX10 = usBytePos - us1Count + 2;
                    usPosX01 = usBytePos + 3;
                  }
                  else
                  {                           
                    usPosX10 = 0;
                    usPosX01 = 3;
                  } 
                  usDy = 0;
                  us1Count = 1;
                  break;

                default:
                  /* X X 1 X*/
                  lTmp = GetSimpleScan(out_buf, ubSizeMode, &us1Count, &usDy, &usPosX10, &usPosX01, usBytePos, ubTemp, 4, ubPreBit, usWidth);
                  out_buf = out_buf + lTmp;
                  lScanSize += lTmp;
                  break;
              } /* end of switch()*/
              ubPreBit = ubTemp & 0x01;
            } /* end of k-loop*/
          }
          break;
      } /* end of switch()*/

      ubPreBit = ubCrtByte & 0x01;
    } /*for  usWidth */

    if (us1Count) 
    {
      lTmp = (*UpdateScanLine[ubSizeMode])(out_buf, us1Count, usDy, usPosX10, usPosX01, usWidth);
      out_buf = out_buf + lTmp;
      lScanSize += lTmp;
      usPosX10 = (j << 3) - us1Count;

      us1Count = 0;
      usDy = 0;
      usPosX01 = 0;
    }
    usDy++;

    /* check size over*/
    if ( (i % 5) == 4 ) 
    {
      lCrtSize = (long)((long)usWidth * (long)(i + 1));
      if ( lScanSize >= lCrtSize )
      {
        return(-1);
      }
    }
  }    /* for   usHeight */
  lCrtSize = (long)((long)usWidth * (long)usHeight);
  if ( (lScanSize + 4) >= lCrtSize )
  {
    lScanSize = -1;
  }
  return(lScanSize);
} /* end of scan_map() */

/*****************************************************************
 *  H : bmp2run
 *  I : unsigned char *in_buf - input buffer pointer
 *      unsigned char *out_buf - output buffer pointer
 *      unsigned int  sizeX   - image width by byte
 *      unsigned int  sizeY   - image height by scanline
 *      unsigned char ubMode  - bit 0 & 1
 *            0 0 - normal compression
 *            0 1 - image reverse
 *            1 X - you get scanline table size
 *  O : unsigned long lScanSize - scanline table size
 *  P : scanline table compression
 ****************************************************************/
long bmp2run(unsigned char *out_buf, unsigned char *in_buf, unsigned int sizeY, unsigned int sizeX, unsigned char ubMode)
{    
  unsigned char *tmp_buf1, *tmp_buf2;
  long  scan_size;

  /*return(-1);*/ /* 19990323 by bglee - request from SM Lee*/
  
  tmp_buf1 = in_buf;
  tmp_buf2 = out_buf;
  scan_size = scan_map(tmp_buf1, tmp_buf2, sizeX, sizeY, ubMode);
  if (scan_size == -1) 
  {
    return(-1);
  }

  if ( !(ubMode & 0x02) )  /* real compression */
  {
    out_buf = tmp_buf2 + scan_size;
    *out_buf++ = 0x00;
    *out_buf++ = 0x00;
    scan_size += 2;
    if (scan_size % 4) 
    {
      *out_buf++ = 0x00;
      *out_buf++ = 0x00;
      scan_size += 2;
    }
  }
  else    /* pre-compression*/
  {
    scan_size += 2;
    if (scan_size % 4) 
    {
      scan_size += 2;
    }
  }
  return(scan_size);                  
}              

