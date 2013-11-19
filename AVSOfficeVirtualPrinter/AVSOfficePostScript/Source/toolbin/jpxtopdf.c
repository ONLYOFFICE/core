/* Copyright (C) 2004-2008 Artifex Software, Inc.  All rights reserved.

  This software is provided AS-IS with no warranty, either express or
  implied.

  This software is distributed under license and may not be copied,
  modified or distributed except as expressly authorized under the terms
  of the license contained in the file LICENSE in this distribution.

  For more information about licensing, please refer to
  http://www.ghostscript.com/licensing/. For information on
  commercial licensing, go to http://www.artifex.com/licensing/ or
  contact Artifex Software, Inc., 101 Lucas Valley Road #110,
  San Rafael, CA  94903, U.S.A., +1(415)492-9861.
*/

/* $Id: jpxtopdf.c 8999 2008-08-19 04:07:40Z giles $ */
/* encapsulates jp2/jpx JPEG2000 images in PDF 1.5 files */
/* based on jpegtopdf by Tor Andersson */

/* compile with 'cc -o jpxtopdf jpxtopdf.c -lm' */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int pages[8000];
int npages = 0;

int xref[8000*3+100];
int nxref = 0;

typedef struct {
  char     *filename;             /* name of image file			 */
  int      filesize;		  /* data length of image file           */
  int      width;                 /* pixels per line			 */
  int      height;                /* rows				 */
  int      numcmpts;              /* number of color components		 */
  int      depth;                 /* bits per color component		 */
  double   dpi;                   /* image resolution in dots per inch   */
} imagedata;

#define BUFSIZE 4096
#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

enum { FALSE, TRUE };

typedef struct box_s {
  unsigned int length;	/* box length */
  char type[5];		/* box type */
  const unsigned char *data;	/* pointer to the contained data */
  unsigned int size;		/* size of the contained data */
  void *rez;	/* pointer to the parsed result, if any */
  struct box_s *next;
} box_t;

typedef struct {
  char brand[5];
  unsigned int version;
} box_ftyp_t;

typedef struct {
  unsigned int width, height;
  unsigned int nc;	/* number of components */
  int bpc;		/* bits per component */
  int sign;		/* whether components are unsigned or signed */
  int C;		/* compression type */
  int UnkC;		/* colour space is unknown */
  int IPR;		/* intellectual property block is present */
} box_ihdr_t;

static unsigned int read_uint32(const unsigned char *p)
{
  return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

static unsigned int read_uint16(const unsigned char *p)
{
  return (p[0] << 8) | p[1];
}

void box_free(box_t *box)
{
  if (box) {
    if (box->rez) free(box->rez);
    free(box);
  }
}

box_t *box_read(const unsigned char *buf, const long size)
{
  box_t *box;

  if (size < 8) return NULL;

  box = malloc(sizeof(*box));
  if (box) {
    box->length = read_uint32(buf);
    memcpy(box->type, buf + 4, 4);
    box->type[4] = '\0';
    box->data = buf + 8;
    box->size = MIN(box->length, size);
    box->rez = NULL;
    box->next = NULL;

    if (!memcmp(box->type, "ftyp", 4)) {
      /* File type box */
      box_ftyp_t *ftyp = malloc(sizeof(*ftyp));
      if (ftyp) {
        memcpy(ftyp->brand, box->data, 4);
        ftyp->brand[4] = '\0';
        ftyp->version = read_uint32(box->data + 4);
      }
      box->rez = ftyp;
    } else if (!memcmp(box->type, "ihdr", 4)) {
      /* image header box */
      box_ihdr_t *ihdr = malloc(sizeof(*ihdr));
      if (ihdr) {
        ihdr->height = read_uint32(box->data);
        ihdr->width = read_uint32(box->data + 4);
        ihdr->nc = read_uint16(box->data + 8);
        ihdr->bpc = (box->data[10] & 0x7f) + 1;
        ihdr->sign = box->data[10] >> 7;
        ihdr->C = box->data[11];
        ihdr->UnkC = box->data[12];
        ihdr->IPR = box->data[13];
      }
      box->rez = ihdr;
    }

  }

  return box;
}

#ifdef DEBUG
void box_print(box_t *box)
{
  if (box->length)
    fprintf(stderr, "box '%s' (%u bytes)", box->type, box->length);
  else
    fprintf(stderr, "box '%s' EOF", box->type);

  if (!memcmp(box->type, "ftyp", 4)) {
    /* File type box */
    box_ftyp_t *ftyp = box->rez;
    if (ftyp)
      fprintf(stderr, " brand '%s' version %u",
        ftyp->brand, ftyp->version);
    else
      fprintf(stderr, " no parse result");
  } else if (!memcmp(box->type, "ihdr", 4)) {
    /* image header box */
    box_ihdr_t *ihdr = box->rez;
    if (ihdr)
        fprintf(stderr, " %ux%u %u component image with %d bit %s samples",
	ihdr->width, ihdr->height, ihdr->nc,
        ihdr->bpc, ihdr->sign ? "signed" : "unsigned");
    else
      fprintf(stderr, " no parse result");
  }

  fprintf(stderr, "\n");
}
#endif /* DEBUG */

/* parse the image header in a jpx file and fill out imagedata struct */
static int jpx_read_imagedata(imagedata * jpx)
{
  FILE *in;
  unsigned char buf[BUFSIZE];
  int n, offset;
  int header = FALSE;

  /* open the file for reading */
  in = fopen(jpx->filename, "rb");
  if (in == NULL) {
    fprintf(stderr, "Error: could not open input file '%s'\n",
	jpx->filename);
    return FALSE;
  }

  n = fread(buf, 1, BUFSIZE, in);

  /* check the file signature */
  if (n < 12) {
    fprintf(stderr, "Error: file too short\n");
    return FALSE;
  }
  if (memcmp(buf, "\x00\x00\x00\x0CjP  \x0D\x0A\x87\x0A", 12)) {
    fprintf(stderr, "Skipping '%s': not a JPX file\n", jpx->filename);
    return FALSE;
  }

  /* initialize the resultion in case the file doesn't set it */
  jpx->dpi = 0.0;

  /* parse the file for the image header */
  offset = 0;
  while (n - offset >= 8) {
    box_t *box = box_read(buf + offset, n - offset);
    if (box) {
#ifdef DEBUG
      box_print(box);
#endif
      if (!memcmp(box->type, "jp2h", 4)) {
        box->next = box_read(buf + offset + 8, box->size);
        if (box->next) {
	  box_ihdr_t *ihdr = box->next->rez;
#ifdef DEBUG
          box_print(box->next);
#endif
	  jpx->width = ihdr->width;
	  jpx->height = ihdr->height;
	  jpx->numcmpts = ihdr->nc;
	  jpx->depth = ihdr->bpc;
          box_free(box->next);
          header = TRUE;
        }
      }
      if (box->length <= 0) break;
      offset += box->length;
      box_free(box);
    }
  }

  if (!header) {
    fprintf(stderr, "Skipping '%s': couldn't parse image header\n",
      jpx->filename);
    return FALSE;
  }

  fprintf(stderr,
	"Note on file '%s': %dx%d pixel, %d color component%s, dpi %lf\n",
		jpx->filename, jpx->width, jpx->height,
		jpx->numcmpts, (jpx->numcmpts == 1 ? "" : "s"),
		jpx->dpi);

  if (jpx->dpi == 0.0)
	jpx->dpi = 100.0;

  return TRUE;
}

static int jpx_pdf(imagedata * jpx, FILE * out)
{
	FILE *in;
	unsigned char buf[BUFSIZE];
	int n;
	double scale;

	if ( !jpx_read_imagedata(jpx) ) return -1;

	in = fopen(jpx->filename, "rb");
	if (in == NULL) {
		fprintf(stderr, "Error: unable to open input file '%s'\n", jpx->filename);
		return -1;
	}
	fseek(in, 0, SEEK_END);
	jpx->filesize = ftell(in);
	fseek(in, 0, SEEK_SET);

	xref[nxref++] = ftell(out);
	fprintf(out, "%d 0 obj\n", nxref);
	fprintf(out, "<</Type/XObject/Subtype/Image\n");
	fprintf(out, "/Width %d /Height %d\n", jpx->width, jpx->height);
	fprintf(out, "/ColorSpace/%s\n", jpx->numcmpts == 1 ? "DeviceGray" : "DeviceRGB");
	fprintf(out, "/BitsPerComponent %d\n", jpx->depth);
	fprintf(out, "/Length %d\n", jpx->filesize);
	fprintf(out, "/Filter/JPXDecode\n");
	fprintf(out, ">>\n");
	fprintf(out, "stream\n");

	/* copy data from jpx file */
	while ((n = fread(buf, 1, sizeof(buf), in)) != 0)
		fwrite(buf, 1, n, out);

	fprintf(out, "endstream\n");
	fprintf(out, "endobj\n");
	fprintf(out, "\n");

	fclose(in);

	scale = 72.0 / jpx->dpi;
	sprintf((char *)buf, "%d 0 0 %d 0 0 cm /x%d Do\n",
		(int)ceil(jpx->width * scale),
		(int)ceil(jpx->height * scale),
		nxref);

	xref[nxref++] = ftell(out);
	fprintf(out, "%d 0 obj\n<</Length %lu>>\n",
		nxref, (unsigned long)strlen((char*)buf));
	fprintf(out, "stream\n");
	fprintf(out, "%s", buf);
	fprintf(out, "endstream\n");
	fprintf(out, "endobj\n");
	fprintf(out, "\n");

	xref[nxref++] = ftell(out);
	fprintf(out, "%d 0 obj\n", nxref);
	fprintf(out, "<</Type/Page/Parent 3 0 R\n");
	fprintf(out, "/Resources << /XObject << /x%d %d 0 R >> >>\n", nxref-2, nxref-2);
	fprintf(out, "/MediaBox [0 0 %d %d]\n",
		(int)ceil(jpx->width * scale),
		(int)ceil(jpx->height * scale));
	fprintf(out, "/Contents %d 0 R\n", nxref-1);
	fprintf(out, ">>\n");
	fprintf(out, "endobj\n");
	fprintf(out, "\n");

	return nxref;
}

int main(int argc, char **argv)
{
	imagedata image;
	FILE *outfile;
	int i;
	int startxref;

	image.filename = NULL;

	outfile = fopen("out.pdf", "w");

	fprintf(outfile, "%%PDF-1.5\n\n");

	xref[nxref++] = ftell(outfile);
	fprintf(outfile, "1 0 obj\n");
	fprintf(outfile, "<</Type/Catalog/Pages 3 0 R>>\n");
	fprintf(outfile, "endobj\n\n");

	xref[nxref++] = ftell(outfile);
	fprintf(outfile, "2 0 obj\n");
	fprintf(outfile, "<</Creator(jpxtopdf)/Title(%s)>>\n", getenv("TITLE"));
	fprintf(outfile, "endobj\n\n");

	/* delay obj #3 (pages) until later */
	nxref++;

	for (i = 1; i < argc; i++) {
	  image.filename = argv[i];

	  /* copy the jpx stream wrapped in a pdf object */
	  pages[npages++] = jpx_pdf(&image, outfile);
	}

	xref[2] = ftell(outfile);
	fprintf(outfile, "3 0 obj\n");
	fprintf(outfile, "<</Type/Pages/Count %d/Kids[\n", npages);
	for (i = 0; i < npages; i++)
		fprintf(outfile, "%d 0 R\n", pages[i]);
	fprintf(outfile, "]>>\nendobj\n\n");

	startxref = ftell(outfile);
	fprintf(outfile, "xref\n0 %d\n", nxref + 1);
	fprintf(outfile, "0000000000 65535 f \n");
	for (i = 0; i < nxref; i++)
		fprintf(outfile, "%010d 00000 n \n", xref[i]);
	fprintf(outfile, "trailer\n<< /Size %d /Root 1 0 R /Info 2 0 R >>\n", nxref + 1);
	fprintf(outfile, "startxref\n%d\n%%%%EOF\n", startxref);

	fclose(outfile);

	return 0;
}

