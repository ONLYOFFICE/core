#include "gdevprn.h"
/* Copyright (C) 1989-1994, 1998, 1999 Aladdin Enterprises.  All rights reserved.
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
/*
 * $Id: gdevlx7.c,v 1.5 2002/08/03 03:08:26 tillkamppeter Exp $
 * Lexmark 7000 ink-jet  "GDI" printer driver
 *   * just started...
 * 
 * Henryk Paluch paluch@bimbo.fjfi.cvut.cz
 * http://bimbo.fjfi.cvut.cz/~paluch/l7kdriver/
 * 
 * BIG thanks to Peter B. West that discovered and described many
 * "funny" things of the Lexmark 5xxx protocol.
 * 
 * THIS GHOSTSCRIPT DRIVER USES MANY THINGS FROM 
 * STEPHEN'S LEXMARK 5700 GHOSCRIPT DRIVER:
 *   
 * Stephen Taylor  setaylor@ma.ultranet.com  staylor@cs.wpi.edu
 * http://www.ultranet.com/~setaylor/papers.htm
 *  
 * ALL INTRODUCED BUGS ARE PROPERTY OF HENRYK :-)
 */

/* our private driver constants */
   /* Lexmark 7xxx,5xxx swipe height of Black & White cartridge */
#define LX7_BSW_H 208

   /* Lexmark 7xxx,5xxx swipe height of Colour cartridge */
#define LX7_CSW_H 192

  /* maximum data bytes per packet: 13 * 2 = 26
   * 13 - compression bits, 2 = data words 
   */
#define LX7_MAX_SWBYTES 26
 /* 13 * 2 databytes + 2 bytes for header = 28 bytes */
#define LX7_MAX_PACKET  28
#define LX7_MAX_SWWORDS 13

/* interlaced printing for 1200dpi - vertical offset of lines */
/* they must be odd. The sum must be 208 */
/* these are for Ink Jet distance */
#define LXH_SKIP1   99
#define LXH_SKIP2   109
/* doubled values for real paper shift */
#define LXH_DSKIP1 (LXH_SKIP1*2-1)
#define LXH_DSKIP2 (LXH_SKIP2*2+1)
/* Lexmark 7000 prologue */
/* 1st time initialization - INIT1 is common for all modes */
#define LX7_INIT1 0x1b,0x2a,0x6d,0x00,0x40,0x10,0x03,0x10,0x11 

/* LX_INIT2 - for 1200dpi - currently not needed */
#define LX7_INIT2 0xa5,0x00,0x06,0x50,0x03,0x03,0xc0,0x0f,0x0f, \
                  0xa5,0x00,0x03,0x50,0x04,0x05, \
		  0xa5,0x00,0x03,0x50,0x04,0x06, \
		  0xa5,0x00,0x03,0x50,0x04,0x07, \
		  0xa5,0x00,0x03,0x50,0x04,0x08, \
		  0xa5,0x00,0x04,0x50,0x04,0x0c,0x00, \
		  0xa5,0x00,0x04,0x50,0xe0,0x0b,0x03 
/* LX_INIT3 - after 1200dpi prologue, or immediately after INIT1
 * in other modes
 */
#define  LX7_INIT3 0xa5,0x00,0x0b,0x50,0xe0,0x41,0x00,\
                        0x00,0x00,0x00,0x00,0x00,0x00,0x02,\
		   0xa5,0x00,0x06,0x50,0x05,0x00,0x00,0x80,0x00, \
                   0xa5,0x00,0x04,0x50,0x04,0x0c,0x00 
/* this is used also for new page prologue ... */
#define LX7_INIT4 0x1b,0x2a,0x07,0x73,0x30 
#define LX7_INIT5 0x1b,0x2a,0x07,0x63
/* mysterious reinitialization ???? */
#define LX7_INIT6 0x1b,0x2a,0x6d,0x00,0x42,0x00,0x00
#define LX7_INIT7 0xa5,0x00,0x05,0x40,0xe0,0x80,0x07,0x08 
		  
static byte lx7_fullinit[]={LX7_INIT1, /*LX7_INIT2, LX7_INIT3,*/ LX7_INIT4,
                             LX7_INIT5, LX7_INIT6};
static byte lx7_pageinit[]={LX7_INIT4, LX7_INIT4, LX7_INIT5, LX7_INIT6};

#ifndef MIN
#define MIN(x,y) ( (x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x,y) ( (x) > (y) ? (x) : (y))
#endif

/* The procedure descriptors */
/* declare functions */
static dev_proc_print_page(lxmgen_print_page);
static dev_proc_get_params(lxm_get_params);
static dev_proc_put_params(lxm_put_params);

/* set up dispatch table.  I follow gdevdjet in using gdev_prn_output_page */
static const gx_device_procs lxm7000m_procs = 
    prn_params_procs(gdev_prn_open, gdev_prn_output_page, gdev_prn_close,
                     lxm_get_params, lxm_put_params);

/* The device descriptors */

/* define a subclass with useful state in it. */
typedef struct lxm_device_s { /* a sub-class of gx_device_printer */
    gx_device_common;
    gx_prn_device_common;
    int headSeparation;
    byte *fullInit;
    int  nfullInit;
    byte *pageInit;
    int npageInit;
    int printertype;
} lxm_device;

/* Lexmark types (faster lookup than strcmp("lex7000",...) ...
 * stored in 'printertype' variable
 */

#define LXT_7000 0
#define LXT_5700 1
#define LXT_3200 2
#define LXT_2050 3

/* resolution map - used by "associate" arrays - do not modify */
#define LXR_300  0
#define LXR_600  1
#define LXR_1200 2

lxm_device far_data gs_lex7000_device = {
    prn_device_std_body(lxm_device, lxm7000m_procs, "lex7000",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, 
	/* total width & height in 10x " - A4 or letter compiled in.
	 * may be overriden by -sPAPERSIZE=a4 of -sPAPERSIZE=letter
	 */
	600, 600,	/* x dpi, y dpi */
	0.0, 0.1, 0.3, 0.1,			/* margins */
	/* unlike most other Ink printers Lexmark is able to print at
	 * whole top and bottom of paper :-)
	 */
	1, lxmgen_print_page),
	/* 1 = bits per color, generic routines*/
	
    /* our extended attributes follow...*/
    16,   /* default headSeparation value */
    lx7_fullinit,
    sizeof(lx7_fullinit),
    lx7_pageinit,
    sizeof(lx7_pageinit),
    LXT_7000
};

lxm_device far_data gs_lex5700_device = {
    prn_device_std_body(lxm_device, lxm7000m_procs, "lex5700",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, 
	/* total width & height in 10x " - A4 or letter compiled in.
	 * may be overriden by -sPAPERSIZE=a4 of -sPAPERSIZE=letter
	 */
	600, 600,	/* x dpi, y dpi */
	0.1, 0.1, 0.1, 0.0,			/* margins */
	/* unlike most other Ink printers Lexmark is able to print at
	 * whole top and bottom of paper :-)
	 */
	1, lxmgen_print_page),
	/* 1 = bits per color, generic routines*/
	
    /* our extended attributes follow...*/
    16,   /* default headSeparation value */
    lx7_fullinit,
    sizeof(lx7_fullinit),
    lx7_pageinit,
    sizeof(lx7_pageinit),
    LXT_5700
};

lxm_device far_data gs_lex3200_device = {
    prn_device_std_body(lxm_device, lxm7000m_procs, "lex3200",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, 
	/* total width & height in 10x " - A4 or letter compiled in.
	 * may be overriden by -sPAPERSIZE=a4 of -sPAPERSIZE=letter
	 */
	600, 600,	/* x dpi, y dpi */
	0.1, 0.1, 0.1, 0.0,			/* margins */
	/* unlike most other Ink printers Lexmark is able to print at
	 * whole top and bottom of paper :-)
	 */
	1, lxmgen_print_page),
	/* 1 = bits per color, generic routines*/
	
    /* our extended attributes follow...*/
    16,   /* default headSeparation value */
    lx7_fullinit,
    sizeof(lx7_fullinit),
    lx7_pageinit,
    sizeof(lx7_pageinit),
    LXT_3200
};

lxm_device far_data gs_lex2050_device = {
    prn_device_std_body(lxm_device, lxm7000m_procs, "lex2050",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, 
	/* total width & height in 10x " - A4 or letter compiled in.
	 * may be overriden by -sPAPERSIZE=a4 of -sPAPERSIZE=letter
	 */
	600, 600,	/* x dpi, y dpi */
	0.1, 0.1, 0.1, 0.0,			/* margins */
	/* unlike most other Ink printers Lexmark is able to print at
	 * whole top and bottom of paper :-)
	 */
	1, lxmgen_print_page),
	/* 1 = bits per color, generic routines*/
	
    /* our extended attributes follow...*/
    16,   /* default headSeparation value */
    lx7_fullinit,
    sizeof(lx7_fullinit),
    lx7_pageinit,
    sizeof(lx7_pageinit),
    LXT_2050
};




#define get_lx_type(pdev) \
  (((lxm_device*)pdev)->printertype)



/* ------ Local Lexmark printer utilities ------*/

#define LX_LINE_EMPTY(buf,len) \
   (buf[0]==0 && memcmp(buf,buf+1,len-1)==0)

static byte ofs8[8]={128,64,32,16,8,4,2,1};
/* Lexmark 5xxx, 7xxx page eject */
static void lex_eject(FILE *out)
{
   byte buf[4]={0x1b,0x2a,0x7,0x65};
#ifdef DEBUG
   dprintf("Sending page eject.\n");
#endif
   fwrite(buf,sizeof(buf),1,out);
}

/*
 * shift paper (skip empty lines)
 * offset is in pixels in 1200 dpi resolution, i. e.,
 * 384 for 192 pixels or
 * 416 for 208 pixels etc...
 */
static void paper_shift(FILE *out,int offset)
{
   byte buf[5]={0x1b,0x2a,0x3,0x0,0x0};
   buf[3]=(byte)(offset >> 8);   /* just to be endian safe we don't use short */
   buf[4]=(byte)(offset & 0xFF);
#ifdef DEBUG
   dprintf3("paper_shift() %d 1200dpi = %d 600dpi = %d 300dpi pixels\n",
	 offset,offset/2,offset/4);
#endif
   fwrite(buf,sizeof(buf),1,out);
}

/* return coordinate of leftmost pixel (in pixels) */
static int leftmost_pixel(byte *buf, int bytelen)
{
   int i;
   byte *r=buf;
   byte b;
   if (LX_LINE_EMPTY(buf,bytelen)) /* catch empty line */
      return bytelen*8-1;

   while( *r==0 && bytelen>0)
   {
      r++;
      bytelen--;
   }

   b= *r;
   for(i=0;i<8;i++)
      if ( ( b & ofs8[i])!=0)
	 break;
   return  (r-buf)*8+i; 
}

/* return coordinate of rightmost pixel (in pixels) */
static int rightmost_pixel(byte *buf, int bytelen)
{
   int i;
   byte *r=buf+(bytelen-1);
   byte b;
   if (LX_LINE_EMPTY(buf,bytelen)) /* catch empty line */
      return 0;

   while( *r==0 && bytelen>1)
   {
      r--;
      bytelen--;
   }

   b= *r;
   for(i=7;i>=0;i--)
      if ( ( b & ofs8[i])!=0)
	 break;
   return  (r-buf)*8+i; 
}

/* find leftmost and rightmost pixel in whole pass
 * buf        - pixel buffer
 * bytelen    - width of line (in bytes)
 * bufheight  - buffer height (pixels)
 * interlaced - if even lines have different horizontal offset
 *              than odd lines
 * intershift - horizontal offset between even/odd lines
 * leftmost,
 *  rightmost - output
 */
static void find_lr_pixels(byte *buf[],int bytelen,int bufheight,
      int interlaced, int intershift,
      int *leftmost, int *rightmost)
{
   int maxright=bytelen*8-1;
   int left=maxright;
   int right=0;
   int i;
   
   for(i=0;i<bufheight;i++)
   {
      int ltmp,rtmp;
      ltmp=leftmost_pixel(buf[i],bytelen);
      rtmp=rightmost_pixel(buf[i],bytelen);
#if 0
      dprintf3("buf[%d]: leftmost %d, rightmost %d\n",
	    i,ltmp,rtmp);
#endif
      if (interlaced && (i & 1)==1) /* interlaced && line is odd */
      {
	 ltmp=MAX((ltmp-intershift),0);
	 rtmp=MIN((rtmp+intershift),maxright);
	 if (ltmp==maxright)
	    ltmp--; /* print at least one pixel to avoid races ?? */
      }
      if (ltmp<left)
	 left=ltmp;
      if (rtmp>right)
	 right=rtmp;
   }

   *leftmost  = left;
   *rightmost = right; 
   return;
}


/* ------ Driver procedures ------ */

/*** THIS NEED TO BE REWORKED SOON ***/
static const int LEFT_MARGIN=50;
static const int VERTSIZE=LX7_BSW_H;
/* offsets to print line sequence (defined in outbuf)
 */
static const int IDX_SEQLEN=5;
static const int IDX_HORRES=8;
static const int IDX_PACKETS=13;
static const int IDX_5700DIF=12;
static const int IDX_HSTART=15;
static const int IDX_HEND=17;
static const int IDX_DATA=26;
static const int IDX_CARTRIDGE=10;

#define DIV8(x) ( (x) >> 3 )
#define MOD8(x) ( (x) & 0x7 )
#define DIV16(x) ( (x) >> 4 )
#define MOD16(x) ( (x) & 0xf )
   /* too large buffer may hang the printer ?! */
#define OUT_BUF_SIZE 256000
/* most important print packet
 * see lexmarkprotocol.txt for more information */
/*          length of complete sequence ---  vvvvvvvvv */
/* this is template now. Outbuf is allocated and initialized
 * by lxmgen_print_page() to avoid large static array
 * in gs binary
 */
static byte outb[]={0x1B,0x2A,0x04,0x00,0x00,0xFF,0xFF,
   /* number of packets ----     vvvvvvvvv */ 
   0x00,0x02,0x01,0x01,0x1A,0x11,0xFF,0xFF,
   /* horiz start, horiz end: packets = (horiz end - horiz start) +1 */
   0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x22,0x33,0x44,0x55,0x01};

#define BITSTART12 4096

static int print_cols(FILE *prn_stream,gx_device_printer *pdev,
      byte *outbuf,
      int left,int right,int vstart, int vend,byte *buf[],
      int width,int LR_SHIFT)
{
   unsigned char mask[8]={128,64,32,16,8,4,2,1};
   unsigned int  mask16[16]={0x8000,0x4000,0x2000,0x1000,
                             0x0800,0x0400,0x0200,0x0100,
			     0x0080,0x0040,0x0020,0x0010,
			     0x0008,0x0004,0x0002,0x0001};
   int l8,r8,packets;
   byte *p=outbuf+IDX_DATA;
   int clen;
   int i,k;
   unsigned int vbuf[LX7_MAX_SWWORDS]; 
   int vi;
   unsigned char *vp,*vp2;
   int smask,smask2;

   l8=left+LEFT_MARGIN;
   r8=right+LEFT_MARGIN;

   packets=r8-l8+1;


   outbuf[IDX_PACKETS]  =(unsigned char)(packets >> 8);
   outbuf[IDX_PACKETS+1]=(unsigned char)(packets & 0xFF);
   outbuf[IDX_HSTART]   =(unsigned char)(l8 >> 8);
   outbuf[IDX_HSTART+1] =(unsigned char)(l8 & 0xFF);
   outbuf[IDX_HEND]     =(unsigned char)(r8 >> 8);
   outbuf[IDX_HEND+1]   =(unsigned char)(r8 & 0xFF);

   outbuf[IDX_5700DIF]= ( get_lx_type(pdev)==LXT_7000 ? 0x11 : 0x1 );
   /* fill columns */
   for(i=left;i<=right;i++)
   {
      int rle1size,rle2size;
      int bits=0;
      byte *tbits;

      if ( (p-outbuf)>=OUT_BUF_SIZE-28 ) /* 13*2+2 */
      {
	 return -1;
      }
      tbits=p;
      p+=2; /* add room for RLE packet (2 bytes) */

      /* clear buffer of vertical points */
      memset(vbuf,0,sizeof(vbuf));

      smask=mask[MOD8(i)];
      smask2=mask[MOD8(i+LR_SHIFT)];

      vi=vstart*2; /* vbuf index in pixels (not array index) */
      for(k=vstart;k<vend;k++)
      {
	 vp=buf[k*2]+DIV8(i);
	 /* vp points to current column for Left Ink */
	 vp2=buf[k*2+1]+DIV8(i+LR_SHIFT);
	 /* vp2 for right Ink jets */
	 if ( ( vp[0] & smask) != 0)
	    vbuf[DIV16(vi)] |= mask16[MOD16(vi)];
	 vi++;
	 if (i+LR_SHIFT<width*8)
	 {
	    if ( (vp2[0] & smask2) !=0 )
	       vbuf[DIV16(vi)] |=mask16[MOD16(vi)];
	 }
	 vi++;
      }

      /* every packet contains 13 info bits
       * 1 = 8x left white, 8x right white
       * 0 = 2 data bytes follow (8 bits left, 8 bits right)
       */
      
      for(k=0;k<LX7_MAX_SWWORDS;k++)
      {
	 unsigned int t=vbuf[k];

	 bits= (bits>>1);
	 if (t==0) /* packet is empty */
	 {
	    bits+=BITSTART12; 
	 }
	 else
	 {
	    *(p++)=(t >> 8) & 0xFF;
	    *(p++)=(t & 0xFF);
	 }
      }
     tbits[1]=(unsigned char)( (bits) & 0xFF);
     *tbits  =(unsigned char)( ((bits>>8) & 0x1f) | 0x20 );
     rle1size=(p-tbits);
#if 0
     dprintf1("RLE1(%d), ",rle1size);
#endif
     if (rle1size>6) /* try to use RLE2 compression for larger packets */
     {
	byte ob[LX7_MAX_PACKET];
	unsigned int lastword=0x8FFF; /* impossible value */
	int byts=0;
	byte *pp=ob+2;

	memset(ob,0,LX7_MAX_PACKET);
	for(k=0;k<LX7_MAX_SWWORDS;k++)
	{
	 unsigned int t=vbuf[k];

	 byts= (byts>>1);
	 if (t==lastword)
	 {
	   byts+=BITSTART12; 
	 }
	 else
	 {
	    *(pp++)=(t >> 8) & 0xFF;
	    *(pp++)=(t & 0xFF);
	    lastword=t;
	 }
	}
     ob[1]=(unsigned char)( (byts) & 0xFF);
     ob[0]=(unsigned char)( (byts>>8) & 0x1f );
     rle2size=(pp-ob);
#if 0
     dprintf1("RLE2(%d), ",rle2size);
#endif

     if (rle1size>rle2size)
     {
#if 0
	dprintf2("\n**RLE2 WIN** %d > %d \n",rle1size,rle2size);
	dprintf("\nUsing RLE2 compression\n");
#endif
	memcpy(tbits,ob,rle2size);
	p=tbits+rle2size;
     }
     }
   }


   /* ------------ */
   clen=p-outbuf; 
   outbuf[IDX_SEQLEN-1]  =(unsigned char)(clen >> 16);
   outbuf[IDX_SEQLEN]  =(unsigned char)(clen >> 8);
   outbuf[IDX_SEQLEN+1]=(unsigned char)(clen & 0xFF);
   fwrite(outbuf,1,clen,prn_stream);
#ifdef DEBUG
   dprintf1("\nSent %d data bytes\n",clen);
#endif
   return 0;
}


/*** THIS NEED TO BE REWORKED SOON - END ***/

/* Send the page to the printer. */
/* Lexmark generic print page routine */
static int
lxmgen_print_page(gx_device_printer *pdev, FILE *prn_stream)
{	
   int pheight=pdev->height; /* page height (pixels) */
#ifdef DEBUG
   int pwidth=pdev->width;   /* page width (pixels) */
#endif
   int bwidth=gdev_mem_bytes_per_scan_line((gx_device*)pdev);
   /* page width (bytes) */

   int prest=pheight;        /* Page Rest number of lines to remain ... */
   int brest;                /* Buffer rest for buffer copy */
   int skipline=208;           /* number of empty lines, e.g. skip them */
   /* we need to move paper under cartridge..
    * about 208 pixels... */
   byte *pbuf;               /* printer buffer - dynamic allocation */
   byte *ppbuf,*ppbuf2;              /* pointer returned by _prn_get_bits() */
   byte *outbuf;             /* output buffer - used by print_cols(),
			      * but allocated here, to avoid unnecessary
			      * de & allocation
			      */
   int pbufsize,rpbufsize;             /* printer buffer size */
   int hres,vres;
   int interlaced;
   int lr_shift=((lxm_device*)pdev)->headSeparation;
   byte *obp[LX7_BSW_H];    /* pointers to buffer lines */
   int bufHeight;

   /* initiate vres mapping variable */
   vres=LXR_600; /* default vertical resolution */
   if (pdev->y_pixels_per_inch<301.0)
      vres=LXR_300;
   if (pdev->y_pixels_per_inch>=601.0)
      vres=LXR_1200;

   bufHeight=LX7_BSW_H;
   if (vres==LXR_300)
   {
      bufHeight /= 2;
      skipline  /= 2;
   }
   if (vres==LXR_1200)
   {
      bufHeight *= 2;
      skipline *=2;
   }

   pbufsize = bwidth * bufHeight;
   rpbufsize = bwidth * (bufHeight+1);
#ifdef DEBUG
   dprintf1("[%s] print_page() start\n",pdev->dname);
   dprintf1("Is first page? %d\n",gdev_prn_file_is_new(pdev));
   dprintf1("Head Separation %d\n",((lxm_device*)pdev)->headSeparation);
   dprintf2("Width %d pixels, %d bytes\n",pwidth,bwidth);
   dprintf1("Height %d pixels\n",pheight);
   dprintf1("One pass buffer size %d\n",pbufsize);
   dprintf1("Output buffer size %d\n",OUT_BUF_SIZE);
   dprintf2("Current resolution is %f width x %f height dpi\n",
	 pdev->x_pixels_per_inch, pdev->y_pixels_per_inch );
#endif
   pbuf = (byte *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), rpbufsize, 1, "lxmgen_print_page(pbuf)");
   if (pbuf == NULL)
      return_error(gs_error_VMerror);

   outbuf = (byte *)gs_malloc(gs_lib_ctx_get_non_gc_memory_t(), OUT_BUF_SIZE, 1, "lxmgen_print_page(outbuf)");
   if (outbuf == NULL)
   {
      gs_free(gs_lib_ctx_get_non_gc_memory_t(), (char*)pbuf,pbufsize, 1, "lxmgen_print_page(pbuf)");
      return_error(gs_error_VMerror);
   }
   /* initialize begin of outbuf ... */
   memcpy(outbuf,outb,sizeof(outb));

   /* zero fake line (used for non-interlaced resolutions etc)...*/
   memset(pbuf+pbufsize,0,bwidth);


   /* initiate hres mapping variable */
   hres=LXR_600; /* default horizontal resolution */
   if (pdev->x_pixels_per_inch<301.0)
      hres=LXR_300;
   if (pdev->x_pixels_per_inch>=601.0)
      hres=LXR_1200;
   /* adjust headSeparation according to horizontal resolution */
   if (hres==LXR_300)
      lr_shift >>=1;
   if (hres==LXR_1200)
      lr_shift <<=1;

   interlaced=1; /* all resolutions except vert 300dpi are interlaced*/

   /* adjust horizontal motor speed for current resolution */
   if (get_lx_type(pdev)==LXT_7000 || get_lx_type(pdev)==LXT_5700)
   {
      byte mspeed[3]={1,2,5}; /* LXR_300=0, LXR_600=1, LXR_1200=2 */
      outbuf[IDX_HORRES]=mspeed[hres];
   }

#ifdef DEBUG
   dprintf1("Choosed motor speed %d\n",(int)outbuf[IDX_HORRES]);
#endif


   if(vres==LXR_600)
   {
      int i;
      for(i=0;i<LX7_BSW_H;i++)
	 obp[i]=pbuf+(i*bwidth);
   }

   if(vres==LXR_300)
   {
      int i;
      for(i=0;i<LX7_BSW_H;i++)
	 if ( (i & 1)!=0)
	    obp[i]=pbuf+pbufsize; /* odd line is empty for 300dpi */
	 else
	    obp[i]=pbuf+(i/2*bwidth);
   }




   if(gdev_prn_file_is_new(pdev))
      fwrite(((lxm_device*)pdev)->fullInit,
	    ((lxm_device*)pdev)->nfullInit,
	    1,prn_stream);
   else
      fwrite(((lxm_device*)pdev)->pageInit,
	    ((lxm_device*)pdev)->npageInit,
	    1,prn_stream);


   while(prest>0)
   {
      int i;
      int leftmost;  /* position of leftmost pixel */
      int rightmost; /* position of rightmost pixel */
      int c1200;     /* testing empty line for 1200dpi... */

      /* copy one line & test for all zeroes */
      gdev_prn_get_bits(pdev, pheight-prest, /* current line No. */
	    pbuf,                /* our buffer if needed */
	    &ppbuf);             /* returns pointer to scanline
				  * either our buffer or
				  * gs internal data buffer
				  */
      if (vres==LXR_1200 && (pheight-prest+LXH_DSKIP1<pheight))
      {
	 gdev_prn_get_bits(pdev, pheight-prest+LXH_DSKIP1, 
	                          /* current line No. */
	    pbuf+bwidth,                /* our buffer if needed */
	    &ppbuf2);            
       c1200=LX_LINE_EMPTY(ppbuf2,bwidth);
      }
      else 
	 c1200=1;

      /* test for empty line (nice Stephen's trick w/ memcmp() :-) */
      if (LX_LINE_EMPTY(ppbuf,bwidth) && c1200) 
      {  /* line empty */
	 prest--;
	 skipline++;
	 continue; /* Loop thgrough the while(prest>0) ... */
      } 

      /* 1pass printing for 300 & 600 dpi */
      /* 2pass printing for 1200dpi */
      for(i=0;i<(vres==LXR_1200 ? 2 : 1);i++)
      {
	 /* skip empty lines on paper, if any */
	 if (skipline>0)
	 {
	    int mult[3]={4,2,1}; /* multiply: 300dpi=4, 600dpi=2, 1200dpi=1 */
	    paper_shift(prn_stream,skipline*mult[vres]);
	    skipline=0;
	 }
	 
	 /* for 1200dpi we need to setup interlaced buffer entries */
	 if (vres==LXR_1200)
	 {
	    int j;
	    for(j=0;j<LX7_BSW_H;j++)
	       if ( (j & 1)!=i) /* i==0 for 1st pass i==1 for 2nd pass */
		  obp[j]=pbuf+pbufsize; /* "odd" line is empty for 300dpi */
	       else
		  obp[j]=pbuf+(j*2*bwidth);
	 }
	 

	 /* copy remaining lines to buffer */
	 brest=MIN(bufHeight,prest);

#ifdef DEBUG
	 dprintf2("Copying %d lines to buffer, vertpos %d\n",brest,
	       pheight-prest);
#endif
	 gdev_prn_copy_scan_lines(pdev,pheight-prest,pbuf,pbufsize);
	 /* zero unused lines (on bottom of page) */
	 if (bufHeight-brest>0)
	    memset(pbuf+(brest*bwidth),0,(bufHeight-brest)*bwidth);

	 /* look for leftmost and rightmost pixels */
	 find_lr_pixels(obp,bwidth,LX7_BSW_H,interlaced,
	       ((lxm_device*)pdev)->headSeparation,
	       &leftmost,&rightmost);
#ifdef DEBUG
	 dprintf2("Leftmost pixel %d, rightmost pixel %d\n",
	       leftmost,rightmost);
#endif

	 /* print the data */
	 if (leftmost<rightmost)
	 if (print_cols(prn_stream,pdev,outbuf,leftmost,rightmost,
		  0,VERTSIZE/2,obp,bwidth,lr_shift)==-1)
	 {
	    print_cols(prn_stream,pdev,outbuf,leftmost,rightmost,0,
		  VERTSIZE/4,obp,bwidth,lr_shift);

	    print_cols(prn_stream,pdev,outbuf,leftmost,rightmost,
		  VERTSIZE/4,VERTSIZE/2,
		  obp,bwidth,lr_shift);

#ifdef DEBUG
	    dprintf("Overflow workaround used\n");
#endif
	 }

	 if (vres!=LXR_1200)
	 {
	    skipline=brest; /* skip already printed lines */
	 }
	 else
	 {
	    skipline+=( i==0 ? LXH_DSKIP1 : LXH_DSKIP2 );
	 }
         prest-=skipline; /* decrease number of remaining lines */ 
	 if (prest<=0)
	    break;
      }
   }

   /* eject page */
   lex_eject(prn_stream);
   gs_free(gs_lib_ctx_get_non_gc_memory_t(), (char*)pbuf,rpbufsize, 1, "lxmgen_print_page(pbuf)");
   gs_free(gs_lib_ctx_get_non_gc_memory_t(), (char*)outbuf,OUT_BUF_SIZE, 1, "lxmgen_print_page(outbuf)");

#ifdef DEBUG
   dprintf1("[%s] print_page() end\n",pdev->dname);
#endif
   return 0;
}

   static int
lxm_get_params(gx_device *pdev, gs_param_list *plist)
{       
    lxm_device* const ldev = (lxm_device*)pdev;
    int code = gdev_prn_get_params(pdev, plist);

    if ( code < 0 ) return code;
    code = param_write_int(plist, 
			   "HeadSeparation",
			   (int *)&(ldev->headSeparation));
           
    return code;
}

/* put_params is supposed to check all the parameters before setting any. */
static int
lxm_put_params(gx_device *pdev, gs_param_list *plist)
{
    int ecode;
    lxm_device* const ldev = (lxm_device*)pdev;
    int trialHeadSeparation=ldev->headSeparation;
    int code = param_read_int(plist, "HeadSeparation", &trialHeadSeparation);

    if ( trialHeadSeparation < 1 || trialHeadSeparation > 32 )
	param_signal_error(plist, "HeadSeparation", gs_error_rangecheck);
    /* looks like param_signal_error is not expected to return */
    ecode = gdev_prn_put_params(pdev, plist);	/* call super class put_params */
    if ( code < 0 ) return code;
    if (ecode < 0) return ecode;

    /* looks like everything okay; go ahead and set headSeparation */
    ldev->headSeparation = trialHeadSeparation;
    if ( code == 1) return ecode; /* I guess this means there is no "HeadSeparation" parameter */
    return 0;
}

