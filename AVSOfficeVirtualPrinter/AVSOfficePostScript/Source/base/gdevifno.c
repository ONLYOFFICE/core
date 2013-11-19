/*
 *		Copyright (c) 1998 by Lucent Technologies.
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR LUCENT TECHNOLOGIES MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */

/* $Id: gdevifno.c 9372 2009-01-18 19:57:58Z alexcher $ */
/*
 * gs device to generate inferno bitmaps
 *
 * Russ Cox <rsc@plan9.bell-labs.com>, 3/25/98
 * Updated to fit in the standard GS distribution, 5/14/98
 * Comments edited for automatic TOC generation, 11/4/99
 */

#include "gdevprn.h"
#include "gsparam.h"
#include "gxlum.h"
#include <stdlib.h>

#define nil ((void*)0)

/*
 * ERROR 
 * is used to go up the stack and 
 * eventually return_error(gs_error_Fatal) to gs.
 */
#define ERROR (-2)

typedef struct WImage WImage;
typedef struct Rectangle Rectangle;
typedef struct Point Point;

struct Point {
	int x;
	int y;
};

struct Rectangle {
	Point min;
	Point max;
};
static Point ZP = { 0, 0 };

static WImage* initwriteimage(FILE *f, Rectangle r, int ldepth);
static int writeimageblock(WImage *w, uchar *data, int ndata);
static int bytesperline(Rectangle, int);
static int rgb2cmap(int, int, int);
static long cmap2rgb(int);

#define X_DPI	100
#define Y_DPI	100

static dev_proc_map_rgb_color(inferno_rgb2cmap);
static dev_proc_map_color_rgb(inferno_cmap2rgb);
static dev_proc_open_device(inferno_open);
static dev_proc_close_device(inferno_close);
static dev_proc_print_page(inferno_print_page);

typedef struct inferno_device_s {
	gx_device_common;
	gx_prn_device_common;
	int ldepth;
	int lastldepth;
	int color, gray;
	int cmapcall;
	int nbits;
} inferno_device;

static const gx_device_procs inferno_procs =
	prn_color_params_procs(inferno_open, gdev_prn_output_page, inferno_close,
		inferno_rgb2cmap, inferno_cmap2rgb,
		gdev_prn_get_params, gdev_prn_put_params);


inferno_device far_data gs_inferno_device =
{ prn_device_body(inferno_device, inferno_procs, "inferno",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	X_DPI, Y_DPI,
	0,0,0,0,	/* margins */
	3,		/* 3 = RGB, 1 = gray, 4 = CMYK */
	16,		/* # of bits per pixel -- 16 is easier to handle than 12 */
	255,		/* # of distinct gray levels.  >=31 to fool gs into asking us*/
	255,		/* # of distinct color levels.  >= 31 to fool gs */
	0,		/* dither gray ramp size.  hopefully not used */
	0,    		/* dither color ramp size.  hopefully not used */
	inferno_print_page)
};

/*
 * ghostscript asks us how to convert between
 * rgb and color map entries
 */
static gx_color_index 
inferno_rgb2cmap(gx_device *dev, const gx_color_value cv[]) {
	int shift;
	inferno_device *bdev = (inferno_device*) dev;
	int nbits = bdev->nbits;
	int mask = (1<<nbits)-1;
	gx_color_value red, green, blue;

	red = cv[0]; green = cv[1]; blue = cv[2];
	/* make the colors the size we want */
	if(gx_color_value_bits > nbits) {
		shift = gx_color_value_bits - nbits;
		red >>= shift;
		green >>= shift;
		blue >>= shift;
	} else if(gx_color_value_bits < nbits) {
		shift = nbits - gx_color_value_bits;
		red <<= shift;
		green <<= shift;
		blue <<= shift;
	}

	/* mask them off to be just nbits */
	red   &= mask;
	green &= mask;
	blue  &= mask;

	/*
	 * we keep track of what ldepth bitmap this is by watching
	 * what colors gs asks for.
	 * 
	 * one catch: sometimes print_page gets called more than one
	 * per page (for multiple copies) without cmap calls inbetween.
	 * if bdev->cmapcall is 0 when print_page gets called, it uses
	 * the ldepth of the last page.
	 */
	if(red == green && green == blue && red != 0 && red != mask) {
		if(red == 5 || red == 10) {
			if(bdev->ldepth < 1)
				bdev->ldepth = 1;
		} else {
			if(bdev->ldepth < 2)
				bdev->ldepth = 2;
		}
	} else
		bdev->ldepth = 3;

	bdev->cmapcall = 1;
	return ((((blue<<4)|green)<<4)|red);
}

static int 
inferno_cmap2rgb(gx_device *dev, gx_color_index color,
  gx_color_value rgb[3]) {
	int shift, i;
	inferno_device *bdev = (inferno_device*) dev;
	int nbits = bdev->nbits;
	int mask = (1<<nbits)-1;

	if(color < 0 || color > 255)
		return_error(gs_error_rangecheck);

	rgb[2] = (color >> (2*nbits)) & mask;
	rgb[1] = (color >> nbits) & mask;
	rgb[0] = color & mask;
	if(gx_color_value_bits > nbits) {
		shift = gx_color_value_bits - nbits;
		for(i=0; i<3; i++)
			rgb[i] <<= shift;
	} else if(gx_color_value_bits < nbits) {
		shift = nbits - gx_color_value_bits;
		for(i=0; i<3; i++)
			rgb[i] >>= shift;
	}

	return 0;
}

/*
 * dithering tables courtesy of john hobby
 */
/* The following constants and tables define the mapping from fine-grained RGB
   triplets to 8-bit values based on the standard Plan 9 color map.
*/
#define Rlevs	16		/* number of levels to cut red value into */
#define Glevs	16
#define Blevs	16
#define Mlevs	16
#define Rfactor 1		/* multiple of red level in p9color[] index */
#define Gfactor Rlevs
#define Bfactor	(Rlevs*Glevs)
			
ulong p9color[Rlevs*Glevs*Blevs];	/* index blue most sig, red least sig */

void init_p9color(void)		/* init at run time since p9color[] is so big */
{
	int r, g, b, o;
	ulong* cur = p9color;
	for (b=0; b<16; b++) {
	    for (g=0; g<16; g++) {
		int m0 = (b>g) ? b : g;
		for (r=0; r<16; r++) {
		    int V, M, rM, gM, bM, m;
		    int m1 = (r>m0) ? r : m0;
		    V=m1&3; M=(m1-V)<<1;
		    if (m1==0) m1=1;
		    m = m1 << 3;
		    rM=r*M; gM=g*M; bM=b*M;
		    *cur = 0;
		    for (o=7*m1; o>0; o-=2*m1) {
			int rr=(rM+o)/m, gg=(gM+o)/m, bb=(bM+o)/m;
			int ij = (rr<<6) + (V<<4) + ((V-rr+(gg<<2)+bb)&15);
			*cur = (*cur << 8) + 255-ij;
		    }
		    cur++;
		}
	    }
	}
}


/*
 * inferno_open() is supposed to initialize the device.
 * there's not much to do.
 */
static int
inferno_open(gx_device *dev)
{
	inferno_device *bdev = (inferno_device*) dev;
	bdev->color = bdev->gray = 0;
	bdev->cmapcall = 0;
	bdev->ldepth = 3;
	bdev->nbits = 4;	/* 4 bits per color per pixel (12 bpp, then we dither) */
				/* if you change this, change the entry in gs_inferno_device */
	init_p9color();
	return gdev_prn_open(dev);
}

/* 
 * inferno_close() is called at the end, once everything
 * is finished.  we have nothing to do.
 */
static int
inferno_close(gx_device *dev)
{
	int code;
	code = gdev_prn_close(dev);
	if(code < 0)
		return_error(code);
	return 0;
}

/*
 * inferno_print_page() is called once for each page
 * (actually once for each copy of each page, but we won't
 * worry about that).
 */
static int
inferno_print_page(gx_device_printer *pdev, FILE *f)
{
	uchar buf[16384];	/* == 8192 dots across */
	uchar *p;
	WImage *w;
	int bpl, y;
	int x, xmod;
	int ldepth;
	int ppb[] = {8, 4, 2, 1};	/* pixels per byte */
	int bpp[] = {1, 2, 4, 8};	/* bits per pixel */
	int gsbpl;
	ulong u;
	ushort us;

	inferno_device *bdev = (inferno_device *) pdev;
	Rectangle r;

	gsbpl = gdev_prn_raster(pdev);
	if(gsbpl > sizeof(buf)) {
		errprintf("bitmap far too wide for inferno\n");
		return_error(gs_error_Fatal);
	}

	if(bdev->cmapcall) {
		bdev->lastldepth = bdev->ldepth;
		bdev->ldepth = 0;
		bdev->cmapcall = 0;
	}
	ldepth = bdev->lastldepth;

	r.min = ZP;
	r.max.x = pdev->width;
	r.max.y = pdev->height;
	bpl = bytesperline(r, ldepth);
	w = initwriteimage(f, r, ldepth);
	if(w == nil) {
		errprintf("initwriteimage failed\n");
		return_error(gs_error_Fatal);
	}

	/*
	 * i wonder if it is faster to put the switch around the for loops
	 * to save all the ldepth lookups.
	 */
	for(y=0; y<pdev->height; y++) {
		gdev_prn_get_bits(pdev, y, buf, &p);
		for(x=0; x<pdev->width; x++) {
			us = (p[2*x]<<8) | p[2*x+1];
			switch(ldepth) {
			case 3:
				if(0){
					int r, g, b;
					r = us & 0xf;
					g = (us>>4)&0xf;
					b = (us>>8)&0xf;
					r<<=4;
					g<<=4;
					b<<=4;
					p[x] = rgb2cmap(r,g,b);	
				}
				if(1){
					u = p9color[us];
					/* the ulong in p9color is a 2x2 matrix.  pull the entry
					 * u[x%2][y%2], more or less.
					 */
					p[x] = u >> (8*((y%2)+2*(x%2)));
				}
				break;
			case 2:
				us = ~us;
				if((x%2) == 0)
					p[x/2] = us & 0xf;
				else
					p[x/2] = (p[x/2]<<4)|(us&0xf);
				break;
			case 0:
				us = ~us;
				if((x%8) == 0)
					p[x/8] = us & 0x1;
				else
					p[x/8] = (p[x/8]<<1)|(us&0x1);
				break;
			}
		}

		/* pad last byte over if we didn't fill it */
		xmod = pdev->width % ppb[ldepth];
		if(xmod)
			p[(x-1)/ppb[ldepth]] <<= ((ppb[ldepth]-xmod)*bpp[ldepth]);
		if(writeimageblock(w, p, bpl) == ERROR)
			return_error(gs_error_Fatal);
	}
	if(writeimageblock(w, nil, 0) == ERROR)
		return_error(gs_error_Fatal);

	return 0;
}

/*
 * this is a modified version of the image compressor
 * from fb/bit2enc.  it is modified only in that it 
 * now compiles as part of gs.
 */

/*
 * Compressed image file parameters
 */
#define	NMATCH	3		/* shortest match possible */
#define	NRUN	(NMATCH+31)	/* longest match possible */
#define	NMEM	1024		/* window size */
#define	NDUMP	128		/* maximum length of dump */
#define	NCBLOCK	6000		/* size of compressed blocks */

#define	HSHIFT	3	/* HSHIFT==5 runs slightly faster, but hash table is 64x bigger */
#define	NHASH	(1<<(HSHIFT*NMATCH))
#define	HMASK	(NHASH-1)
#define	hupdate(h, c)	((((h)<<HSHIFT)^(c))&HMASK)

typedef struct Dump	Dump;
typedef struct Hlist Hlist;

struct Hlist{
	ulong p;
	Hlist *next, *prev;
};

struct Dump {
	int ndump;
	uchar *dumpbuf;
	uchar buf[1+NDUMP];
};

struct WImage {
	FILE *f;

	/* image attributes */
	Rectangle origr, r;
	int bpl;

	/* output buffer */
	uchar outbuf[NCBLOCK], *outp, *eout, *loutp;

	/* sliding input window */
	/*
	 * ibase is the pointer to where the beginning of
	 * the input "is" in memory.  whenever we "slide" the
	 * buffer N bytes, what we are actually doing is 
	 * decrementing ibase by N.
	 * the ulongs in the Hlist structures are just
	 * pointers relative to ibase.
	 */
	uchar *inbuf;	/* inbuf should be at least NMEM+NRUN+NMATCH long */
	uchar *ibase;
	int minbuf;	/* size of inbuf (malloc'ed bytes) */
	int ninbuf;	/* size of inbuf (filled bytes) */
	ulong line;	/* the beginning of the line we are currently encoding,
			 * relative to inbuf (NOT relative to ibase) */

	/* raw dump buffer */
	Dump dump;

	/* hash tables */
	Hlist hash[NHASH];
	Hlist chain[NMEM], *cp;
	int h;
	int needhash;
};

static void
zerohash(WImage *w)
{
	memset(w->hash, 0, sizeof(w->hash));
	memset(w->chain, 0, sizeof(w->chain));
	w->cp=w->chain;
	w->needhash = 1;
}

static int
addbuf(WImage *w, uchar *buf, int nbuf)
{
	int n;
	if(buf == nil || w->outp+nbuf > w->eout) {
		if(w->loutp==w->outbuf){	/* can't really happen -- we checked line length above */
			errprintf("buffer too small for line\n");
			return ERROR;
		}
		n=w->loutp-w->outbuf;
		fprintf(w->f, "%11d %11d ", w->r.max.y, n);
		fwrite(w->outbuf, 1, n, w->f);
		w->r.min.y=w->r.max.y;
		w->outp=w->outbuf;
		w->loutp=w->outbuf;
		zerohash(w);
		return -1;
	}

	memmove(w->outp, buf, nbuf);
	w->outp += nbuf;
	return nbuf;
}

/* return 0 on success, -1 if buffer is full */
static int
flushdump(WImage *w)
{
	int n = w->dump.ndump;

	if(n == 0)
		return 0;

	w->dump.buf[0] = 0x80|(n-1);
	if((n=addbuf(w, w->dump.buf, n+1)) == ERROR)
		return ERROR;
	if(n < 0)
		return -1;
	w->dump.ndump = 0;
	return 0;
}

static void
updatehash(WImage *w, uchar *p, uchar *ep)
{
	uchar *q;
	Hlist *cp;
	Hlist *hash;
	int h;

	hash = w->hash;
	cp = w->cp;
	h = w->h;
	for(q=p; q<ep; q++) {
		if(cp->prev)
			cp->prev->next = cp->next;
		cp->next = hash[h].next;
		cp->prev = &hash[h];
		cp->prev->next = cp;
		if(cp->next)
			cp->next->prev = cp;
		cp->p = q - w->ibase;
		if(++cp == w->chain+NMEM)
			cp = w->chain;
		if(&q[NMATCH] < &w->inbuf[w->ninbuf])
			h = hupdate(h, q[NMATCH]);
	}
	w->cp = cp;
	w->h = h;
}

/*
 * attempt to process a line of input,
 * returning the number of bytes actually processed.
 *
 * if the output buffer needs to be flushed, we flush
 * the buffer and return 0.
 * otherwise we return bpl
 */
static int
gobbleline(WImage *w)
{
	int runlen, n, offs;
	uchar *eline, *es, *best, *p, *s, *t;
	Hlist *hp;
	uchar buf[2];
	int rv;

	if(w->needhash) {
		w->h = 0;
		for(n=0; n!=NMATCH; n++)
			w->h = hupdate(w->h, w->inbuf[w->line+n]);
		w->needhash = 0;
	}
	w->dump.ndump=0;
	eline=w->inbuf+w->line+w->bpl;
	for(p=w->inbuf+w->line;p!=eline;){
		es = (eline < p+NRUN) ? eline : p+NRUN;

		best=nil;
		runlen=0;
		/* hash table lookup */
		for(hp=w->hash[w->h].next;hp;hp=hp->next){
			/*
			 * the next block is an optimization of 
			 * for(s=p, t=w->ibase+hp->p; s<es && *s == *t; s++, t++)
			 * 	;
			 */

			{	uchar *ss, *tt;
				s = p+runlen;
				t = w->ibase+hp->p+runlen;
				for(ss=s, tt=t; ss>=p && *ss == *tt; ss--, tt--)
					;
				if(ss < p)
					while(s<es && *s == *t)
						s++, t++;
			}

			n = s-p;

			if(n > runlen) {
				runlen = n;
				best = w->ibase+hp->p;
				if(p+runlen == es)
					break;
			}
		}

		/*
		 * if we didn't find a long enough run, append to 
		 * the raw dump buffer
		 */
		if(runlen<NMATCH){
			if(w->dump.ndump==NDUMP) {
				if((rv = flushdump(w)) == ERROR)
					return ERROR;
				if(rv < 0)
					return 0;
			}
			w->dump.dumpbuf[w->dump.ndump++]=*p;
			runlen=1;
		}else{
		/*
		 * otherwise, assuming the dump buffer is empty,
		 * add the compressed rep.
		 */
			if((rv = flushdump(w)) == ERROR)
				return ERROR;
			if(rv < 0)
				return 0;
			offs=p-best-1;
			buf[0] = ((runlen-NMATCH)<<2)|(offs>>8);
			buf[1] = offs&0xff;
			if(addbuf(w, buf, 2) < 0)
				return 0;
		}

		/*
		 * add to hash tables what we just encoded
		 */
		updatehash(w, p, p+runlen);
		p += runlen;
	}

	if((rv = flushdump(w)) == ERROR)
		return ERROR;
	if(rv < 0)
		return 0;
	w->line += w->bpl;
	w->loutp=w->outp;
	w->r.max.y++;
	return w->bpl;
}

static uchar*
shiftwindow(WImage *w, uchar *data, uchar *edata)
{
	int n, m;

	/* shift window over */
	if(w->line > NMEM) {
		n = w->line-NMEM;
		memmove(w->inbuf, w->inbuf+n, w->ninbuf-n);
		w->line -= n;
		w->ibase -= n;
		w->ninbuf -= n;
	}

	/* fill right with data if available */
	if(w->minbuf > w->ninbuf && edata > data) {
		m = w->minbuf - w->ninbuf;
		if(edata-data < m)
			m = edata-data;
		memmove(w->inbuf+w->ninbuf, data, m);
		data += m;
		w->ninbuf += m;
	}

	return data;
}

static WImage*
initwriteimage(FILE *f, Rectangle r, int ldepth)
{
	WImage *w;
	int n, bpl;

	bpl = bytesperline(r, ldepth);
	if(r.max.y <= r.min.y || r.max.x <= r.min.x || bpl <= 0) {
		errprintf("bad rectangle, ldepth");
		return nil;
	}

	n = NMEM+NMATCH+NRUN+bpl*2;
	w = malloc(n+sizeof(*w));
	if(w == nil)
		return nil;
	w->inbuf = (uchar*) &w[1];
	w->ibase = w->inbuf;
	w->line = 0;
	w->minbuf = n;
	w->ninbuf = 0;
	w->origr = r;
	w->r = r;
	w->r.max.y = w->r.min.y;
	w->eout = w->outbuf+sizeof(w->outbuf);
	w->outp = w->loutp = w->outbuf;
	w->bpl = bpl;
	w->f = f;
	w->dump.dumpbuf = w->dump.buf+1;
	w->dump.ndump = 0;
	zerohash(w);

	fprintf(f, "compressed\n%11d %11d %11d %11d %11d ",
		ldepth, r.min.x, r.min.y, r.max.x, r.max.y);
	return w;
}

static int
writeimageblock(WImage *w, uchar *data, int ndata)
{
	uchar *edata;

	if(data == nil) {	/* end of data, flush everything */
		while(w->line < w->ninbuf)
			if(gobbleline(w) == ERROR)
				return ERROR;
		addbuf(w, nil, 0);
		if(w->r.min.y != w->origr.max.y) {
			errprintf("not enough data supplied to writeimage\n");
		}
		free(w);
		return 0;
	}

	edata = data+ndata;
	data = shiftwindow(w, data, edata);
	while(w->ninbuf >= w->line+w->bpl+NMATCH) {
		if(gobbleline(w) == ERROR)
			return ERROR;
		data = shiftwindow(w, data, edata);
	}
	if(data != edata) {
		fprintf(w->f, "data != edata.  uh oh\n");
		return ERROR; /* can't happen */
	}
	return 0;
}

/*
 * functions from the Plan9/Brazil drawing libraries 
 */
static int
bytesperline(Rectangle r, int ld)
{
	ulong ws, l, t;
	int bits = 8;

	ws = bits>>ld;	/* pixels per unit */
	if(r.min.x >= 0){
		l = (r.max.x+ws-1)/ws;
		l -= r.min.x/ws;
	}else{			/* make positive before divide */
		t = (-r.min.x)+ws-1;
		t = (t/ws)*ws;
		l = (t+r.max.x+ws-1)/ws;
	}
	return l;
}

static int
rgb2cmap(int cr, int cg, int cb)
{
	int r, g, b, v, cv;

	if(cr < 0)
		cr = 0;
	else if(cr > 255)
		cr = 255;
	if(cg < 0)
		cg = 0;
	else if(cg > 255)
		cg = 255;
	if(cb < 0)
		cb = 0;
	else if(cb > 255)
		cb = 255;
	r = cr>>6;
	g = cg>>6;
	b = cb>>6;
	cv = cr;
	if(cg > cv)
		cv = cg;
	if(cb > cv)
		cv = cb;
	v = (cv>>4)&3;
	return 255-((((r<<2)+v)<<4)+(((g<<2)+b+v-r)&15));
}

/*
 * go the other way; not currently used.
 *
static long
cmap2rgb(int c)
{
	int j, num, den, r, g, b, v, rgb;

	c = 255-c;
	r = c>>6;
	v = (c>>4)&3;
	j = (c-v+r)&15;
	g = j>>2;
	b = j&3;
	den=r;
	if(g>den)
		den=g;
	if(b>den)
		den=b;
	if(den==0) {
		v *= 17;
		rgb = (v<<16)|(v<<8)|v;
	}
	else{
		num=17*(4*den+v);
		rgb = ((r*num/den)<<16)|((g*num/den)<<8)|(b*num/den);
	}
	return rgb;
}
 *
 *
 */

