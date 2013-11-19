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

/*$Id: rinkj-epson870.c 8865 2008-07-22 21:03:07Z ray $ */
/* A Rinkj driver for a number of variable-dot Epson devices. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rinkj-byte-stream.h"
#include "rinkj-device.h"
#include "rinkj-config.h"
#include "rinkj-epson870.h"

typedef struct _RinkjEscp RinkjEscp;

struct _RinkjEscp {
  RinkjDevice super;
  RinkjByteStream *out;
  int width;
  int height;
  int y;

  char *manufacturer;
  char *model;

  int num_chan;
  int bps;       /* bits per sample */

  int xres;      /* resolution of input image */
  int yres;

  int head_bps;
  int head_xres; /* x resolution of printhead in dpi */
  int head_yres; /* y resolution of printhead in dpi */

  /* Number of passes for a single scanline */
  int passes_per_scan;

  /* parameters for controlling microweaving */
  int spacing;
  int n_pins;

  int plane_offsets[6];
  int max_offset;

  char *buf;
  unsigned char *buf_linevalid;
  int bufheight;
  int rowstride;
  int planestride;
  int pass;

  int vertpos; /* for ESC ( v */

  /* parameters passed into ESCP2 */
  int autocut;
  int blankskip;
  int microdot;
  int unidir;
  int printer_weave;
};

static int
rinkj_escp_set (RinkjDevice *self, const char *config)
{
  RinkjEscp *z = (RinkjEscp *)self;
  const char *p, *next;
  char *key, *val;

  for (p = config; (key = rinkj_config_keyval (p, &val, &next)); p = next)
    {
      if (!strcmp (key, "Resolution"))
	{
	  char *p;

	  z->xres = atoi (val);
	  p = strchr (val, 'x');
	  if (p != NULL)
	    z->yres = atoi (p + 1);
	  else
	    z->yres = z->xres;
#ifdef VERBOSE
	  fprintf (stderr, "Resolution = %d x %d\n", z->xres, z->yres);
#endif
	}
      else if (!strcmp (key, "Manufacturer"))
	{
	  if (z->manufacturer)
	    free (z->manufacturer);
	  z->manufacturer = val;
	  val = NULL;
	}
      else if (!strcmp (key, "Model"))
	{
	  if (z->model)
	    free (z->model);
	  z->model = val;
	  val = NULL;
	}
      else if (!strcmp (key, "BitsPerSample"))
	{
	  z->bps = atoi (val);
#ifdef VERBOSE
	  fprintf (stderr, "BitsPerSample = %d\n", z->bps);
#endif
	}
      else if (!strcmp (key, "NumChan"))
	{
	  /* This is in params, but should become a setting. */
	  z->num_chan = atoi (val);
#ifdef VERBOSE
	  fprintf (stderr, "NumChan = %d\n", z->num_chan);
#endif
	}
      else if (!strcmp (key, "PrinterWeave"))
	{
	  z->printer_weave = atoi (val);
	}
      else if (!strcmp (key, "Microdot"))
	{
	  z->microdot = atoi (val);
	}
      else if (!strcmp (key, "Unidirectional"))
	{
	  z->unidir = atoi (val);
	}
      else if (!strcmp (key, "AutoCut"))
	{
	  z->autocut = atoi (val);
	}
      else if (!strcmp (key, "BlankSkip"))
	{
	  z->blankskip = atoi (val);
	}
      free (key);
      if (val)
	free (val);
    }
  return 0;
}

static int
rinkj_escp_ytop (RinkjEscp *z, int pass, int *p_x_pass)
{
  int ytop;
  int x_pass, y_pass;
  int passes_per_scan = z->passes_per_scan;
  int spacing = z->spacing;
  int n_cycle = spacing * passes_per_scan;
  int mod_pass = pass % n_cycle;
  int y_modulo;
  const int four[4] = { 0, 3, 1, 2 };
  const int six[6] = { 0, 3, 2, 5, 1, 4 };

  ytop = mod_pass * z->n_pins * spacing / n_cycle;
#if 1
  x_pass = mod_pass / spacing;
  y_pass = mod_pass % spacing;
  if (passes_per_scan == 4)
    x_pass = four[x_pass];
#else
  x_pass = mod_pass % passes_per_scan;
  y_pass = (mod_pass / passes_per_scan + x_pass * (spacing - 1)) % spacing;
#endif

  switch (spacing)
    {
    case 4:
      y_modulo = four[y_pass];
      break;
    case 6:
      y_modulo = six[y_pass];
      break;
    case 8:
      y_modulo = y_pass * 3;
      break;
    default:
      y_modulo = y_pass;
      break;
    }

  ytop += (spacing + y_modulo - ytop % spacing) % spacing;
  ytop += (pass / n_cycle) * spacing * z->n_pins;

  if (spacing == 4 && passes_per_scan == 4 && z->n_pins == 96)
    {
      const int sixteen[] = { 0, 3, 1, 0, 3, 1, 2, 3, 1, 2, 0, 1, 2, 0, 3, 2 };
      x_pass = sixteen[mod_pass & 15];
      ytop = pass * 23;
    }

#ifdef VERBOSE
  fprintf (stderr, "pass %d: ytop = %d, x_pass = %d\n", pass, ytop, x_pass);
#endif

  if (p_x_pass)
    *p_x_pass = x_pass;
  return ytop;
}

static int
rinkj_epson_headres (RinkjEscp *z, int baseres)
{
  return rinkj_byte_stream_printf (z->out, "\033(D\004%c%c%c%c%c", 0,
				   baseres & 255, baseres >> 8,
				   baseres / z->head_yres,
				   baseres / z->head_xres);
}

static int
rinkj_epson_units (RinkjEscp *z, int xres, int yres, int baseres)
{
  return rinkj_byte_stream_printf (z->out, "\033(U\005%c%c%c%c%c%c", 0,
				   baseres / yres,
				   baseres / yres,
				   baseres / xres,
				   baseres & 255, baseres >> 8);
}

static int
rinkj_epson_set_common (RinkjEscp *z)
{
  int status = 0;

  if (z->printer_weave >= 0)
    /* set microweave */
    status = rinkj_byte_stream_printf (z->out, "\033(i\001%c%c", 0,
				       z->printer_weave);

  if (status == 0 && z->unidir >= 0)
    /* set unidirectional */
    status = rinkj_byte_stream_printf (z->out, "\033U%c", z->unidir);

  if (status == 0 && z->microdot >= 0)
    /* set dot size */
    status = rinkj_byte_stream_printf (z->out, "\033(e\002%c%c%c", 0, 0,
				       z->microdot);

  return status;
}

/**
 * Spit out a command string to resemble the gimp-print output as much
 * as possible.
 **/
static int
rinkj_epson870_init (RinkjDevice *self, const RinkjDeviceParams *params)
{
  RinkjEscp *z = (RinkjEscp *)self;

  rinkj_byte_stream_printf (z->out,
			    "%c%c%c\033\001@EJL 1284.4\n", 0, 0, 0);
  rinkj_byte_stream_printf (z->out, "@EJL     \n\033@\033@");

  /* remote string goes here, but is probably optional */

  /* set graphics mode */
  rinkj_byte_stream_printf (z->out, "\033(G\001%c\001", 0);

  /* set units to 1/720" */
  rinkj_byte_stream_printf (z->out, "\033(U\005%c\002\002\002\240\005", 0);

  rinkj_epson_set_common(z);

  /* set page length to about 22.377 inches */
  rinkj_byte_stream_printf (z->out, "\033(C\002%cx\037", 0);

  /* ESC ( c */

  /* ESC ( S */

  /* set resolution to 360 x 120 dpi - should probably generate this
     string from head_xres and head_yres */
  rinkj_byte_stream_printf (z->out, "\033(D\004%c@8x(", 0);

  /* ESC ( v */

  /* ESC \ */
  return 0;
}

/**
 * Spit out a command string to resemble the gimp-print output as much
 * as possible.
 **/
static int
rinkj_epson2200_init (RinkjDevice *self, const RinkjDeviceParams *params)
{
  RinkjEscp *z = (RinkjEscp *)self;

  rinkj_byte_stream_printf (z->out,
			    "%c%c%c\033\001@EJL 1284.4\n", 0, 0, 0);
  rinkj_byte_stream_printf (z->out, "@EJL     \n\033@\033@");

  /* remote string */
  rinkj_byte_stream_printf( z->out,
			    "\033(R%c%c%cREMOTE1", 8, 0, 0);
  rinkj_byte_stream_printf( z->out,
			    "PP\003%c%c\002%cPH\002%c%c\001SN\003%c%c\004k",
			    0, 0, 0, 0, 0, 0, 0);
  rinkj_byte_stream_printf( z->out,
			    "\033%c%c%c", 0, 0, 0);

  /* set graphics mode */
  rinkj_byte_stream_printf (z->out, "\033(G\001%c\001", 0);

  rinkj_epson_units(z, z->xres, z->yres, 2880);

  rinkj_epson_set_common(z);

  /* set page length to 40 inches */
  rinkj_byte_stream_printf (z->out, "\033(C\002%c\200p", 0);

  /* ESC ( c */

  /* ESC ( S */

  rinkj_epson_headres (z, 2880);

  /* ESC ( v */

  /* ESC \ */
  return 0;
}

/**
 * Spit out a command string to resemble the gimp-print output as much
 * as possible.
 **/
static int
rinkj_epson7600_init (RinkjDevice *self, const RinkjDeviceParams *params)
{
  RinkjEscp *z = (RinkjEscp *)self;
  int pl;
  int pw = 720 * 24; /* hardcode to 24 inch */

#if 0
  rinkj_byte_stream_printf (z->out,
			    "%c%c%c\033\001@EJL 1284.4\n", 0, 0, 0);
  rinkj_byte_stream_printf (z->out, "@EJL     \n");
#endif
  rinkj_byte_stream_printf (z->out, "\033@\033@");

#if 1
  /* remote string */
  rinkj_byte_stream_printf( z->out,
			    "\033(R%c%c%cREMOTE1", 8, 0, 0);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 1, 0);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 2, 6);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 3, 0);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 4, 129);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 5, 51);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 8, 0);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 9, 2);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 10, 0);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 128, 1);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 129, 0);
  if (z->autocut >= 0)
    rinkj_byte_stream_printf( z->out, "AC%c%c%c%c", 2, 0, 0, z->autocut);
  if (z->blankskip >= 0)
    rinkj_byte_stream_printf( z->out, "AC%c%c%c%c", 2, 0, 0, 64 + z->blankskip);
  rinkj_byte_stream_printf( z->out, "DR%c%c%c%c%c%c", 4, 0, 0, 1, 0, 0);
  rinkj_byte_stream_printf( z->out, "DR%c%c%c%c%c%c", 4, 0, 0, 0, 0, 0);
  rinkj_byte_stream_printf( z->out, "PH%c%c%c%c", 2, 0, 0, 0);
  rinkj_byte_stream_printf( z->out, "FP%c%c%c%c%c", 3, 0, 0, 0, 0);
  rinkj_byte_stream_printf( z->out, "AC%c%c%c%c", 2, 0, 0, 64);
  rinkj_byte_stream_printf( z->out, "SN%c%c%c%c%c", 3, 0, 0, 132, 1);
  rinkj_byte_stream_printf( z->out, "PP%c%c%c%c%c", 3, 0, 0, 3, 0);
  rinkj_byte_stream_printf( z->out, "IK%c%c%c%c", 2, 0, 0, 1);
  rinkj_byte_stream_printf( z->out, "EX%c%c%c%c%c%c%c%c", 6, 0, 0, 0, 0, 0, 20, 0);
  rinkj_byte_stream_printf( z->out,
			    "\033%c%c%c", 0, 0, 0);
#endif

  /* set graphics mode */
  rinkj_byte_stream_printf (z->out, "\033(G\001%c\001", 0);

  /* set units to 1/720" */
  rinkj_byte_stream_printf (z->out, "\033(U\005%c\002\002\002\240\005", 0);

  rinkj_epson_set_common(z);

  pl = z->height * 720 / z->yres + 180;
  /* set page length to page height + 1/4 inch */
  rinkj_byte_stream_printf (z->out, "\033(S\010%c%c%c%c%c%c%c%c%c", 0,
			    pw & 255, (pw >> 8) & 255, (pw >> 16) & 255, pw >> 24,
			    pl & 255, (pl >> 8) & 255, (pl >> 16) & 255, pl >> 24);

  rinkj_byte_stream_printf (z->out, "\033(c\010%c%c%c%c%c%c%c%c%c", 0,
			    0, 0, 0, 0,
			    pl & 255, (pl >> 8) & 255, (pl >> 16) & 255, pl >> 24);

  rinkj_epson_headres (z, 2880);

  /* ESC ( v */

  /* ESC \ */
  return 0;
}

/**
 * Spit out a command string to resemble the gimp-print output as much
 * as possible.
 **/
static int
rinkj_epsonc80_init (RinkjDevice *self, const RinkjDeviceParams *params)
{
  RinkjEscp *z = (RinkjEscp *)self;

  rinkj_byte_stream_printf (z->out,
			    "%c%c%c\033\001@EJL 1284.4\n", 0, 0, 0);
  rinkj_byte_stream_printf (z->out, "@EJL     \n\033@\033@");

  /* remote string goes here, but is probably optional */

  /* set graphics mode */
  rinkj_byte_stream_printf (z->out, "\033(G\001%c\001", 0);

  /* set units to 1/720" */
  rinkj_byte_stream_printf (z->out, "\033(U\005%c\002\002\002\240\005", 0);

  rinkj_epson_set_common(z);

  /* set page length to about 22.377 inches */
  rinkj_byte_stream_printf (z->out, "\033(C\002%cx\037", 0);

  /* set margins (magic) */
  rinkj_byte_stream_printf (z->out, "\033(c\010%c\040\376\377\377\376\036%c%c",
			    0, 0, 0);

  /* ESC ( c */

  /* ESC ( S */

  /* set resolution to 360 x 180 dpi - should probably generate this
     string from head_xres and head_yres */
  rinkj_byte_stream_printf (z->out, "\033(D\004%c@8P(", 0);

  /* ESC ( v */

  /* ESC \ */
  return 0;
}

static int
rinkj_escp_init (RinkjDevice *self, const RinkjDeviceParams *params)
{
  RinkjEscp *z = (RinkjEscp *)self;
  int resolution = 720; /* todo: make settable */
  int uweave;
  int height, top, bottom;
  int i;

  z->width = params->width;
  z->height = params->height;
  z->num_chan = params->n_planes;

  /* weaving stuff */
  z->pass = 0;

  /* 0 inch margins on top, 0.5 on bottom */
  top = 0 * resolution;
  bottom = params->height + 0.5 * resolution;
  height = params->height * resolution + resolution;

  /* some defaults */
  for (i = 0; i < sizeof(z->plane_offsets) / sizeof(z->plane_offsets[0]); i++)
    z->plane_offsets[i] = 0;

#ifdef VERBOSE
  fprintf (stderr, "Manufacturer: %s; Model; %s\n", z->manufacturer,
	   z->model);
#endif

  if (z->model && !strcmp (z->model, "Stylus Photo 870"))
    {
      z->head_xres = 360;
      z->head_yres = 120;
      z->head_bps = 2;
      z->n_pins = 48;
      z->printer_weave = 0;
    }
  else if (z->model && !strcmp (z->model, "Stylus Photo 2200"))
    {
      z->head_xres = 360;
      z->head_yres = 180;
      z->head_bps = 2;
      if (z->xres == 2880)
	{
	  z->head_xres = 720;
	  z->head_bps = 1;
	}
      z->n_pins = 96;
      z->printer_weave = 0;
      z->plane_offsets[4] = z->yres / 360;
      z->plane_offsets[5] = z->yres / 360;
      z->plane_offsets[6] = z->yres / 360;
    }
  else if (z->model && !strcmp (z->model, "Stylus Photo 7600"))
    {
      z->head_xres = z->xres;
      z->head_yres = z->yres;
      z->head_bps = z->bps;
      z->n_pins = 1;
    }
  else if (z->model && !strcmp (z->model, "Stylus C80"))
    {
      z->head_xres = 360;
      z->head_yres = 180;
      z->head_bps = 2;
      z->n_pins = 60;
      z->printer_weave = 0;
      z->plane_offsets[0] = 480;
      z->plane_offsets[1] = 240;
      z->plane_offsets[2] = 480;
    }
  else
    {
      z->spacing = 1;
      z->n_pins = 1;
    }

  z->spacing = z->yres / z->head_yres;
  z->passes_per_scan = z->xres / z->head_xres;

  /* microweave */
  uweave = (z->n_pins == 1);

  z->max_offset = 0;
  for (i = 0; i < sizeof(z->plane_offsets) / sizeof(z->plane_offsets[0]); i++)
    if (z->plane_offsets[i] > z->max_offset)
      z->max_offset = z->plane_offsets[i];

  z->y = rinkj_escp_ytop (z, z->spacing * z->passes_per_scan - 1, NULL) -
    (z->spacing - 1) + z->max_offset;

  z->planestride = (z->width * z->bps + 7) >> 3;
  z->rowstride = z->planestride * z->num_chan;
  z->bufheight = 2048; /* todo: compute */
  z->buf = (char *)calloc (z->rowstride,  z->bufheight);
  z->buf_linevalid = (unsigned char *)calloc (z->num_chan, z->bufheight);
  z->vertpos = -1;

  if (z->model && !strcmp (z->model, "Stylus Photo 870"))
    rinkj_epson870_init (self, params);
  else if (z->model && !strcmp (z->model, "Stylus Photo 2200"))
    rinkj_epson2200_init (self, params);
  else if (z->model && !strcmp (z->model, "Stylus Photo 7600"))
    rinkj_epson7600_init (self, params);
  else if (z->model && !strcmp (z->model, "Stylus C80"))
    rinkj_epsonc80_init (self, params);

  /* todo: error checking */
  return 0;
    
}

/**
 * rinkj_escp_shuffle_dblx: Shuffle bits for doubled X resolution.
 * @dst: Where to store shuffled bits.
 * @src: Source of bits.
 * @pass: Pass number.
 * @n_bytes: Number of bytes in @src.
 *
 * Samples half of the bits in @src, finely interleaved for Epson Stylus
 * Color 1440 x 720 modes. If @pass is 0, then it selects bits 7, 5, 3, 1.
 * If @pass is 1, then 6, 4, 2, 0.
 *
 * The number of bytes in @dst should be (@n_bytes + 1) >> 1.
 **/
static void
rinkj_escp_shuffle_dblx (char *dst, const char *src, int pass, int n_bytes)
{
  int i;
  int n_dst;
  unsigned char s0, s1;

  n_dst = n_bytes >> 1;
  for (i = 0; i < n_dst; i++)
    {
      s0 = src[i * 2] << pass;
      s1 = src[i * 2 + 1] << pass;
      dst[i] = (s0 & 0x80) | ((s0 & 0x20) << 1) | ((s0 & 8) << 2) | ((s0 & 2) << 3) |
	((s1 & 0x80) >> 4) | ((s1 & 0x20) >> 3) | ((s1 & 8) >> 2) | ((s1 & 2) >> 1);
    }
  if (n_bytes & 1)
    {
      s0 = src[n_bytes - 1] << pass;
      dst[n_dst] = (s0 & 0x80) | ((s0 & 0x20) << 1) | ((s0 & 8) << 2) | ((s0 & 2) << 3);
    }
}

/**
 * rinkj_escp_shuffle_4pass_2bit: Shuffle bits for quadrupled X resolution.
 * @dst: Where to store shuffled bits.
 * @src: Source of bits.
 * @pass: Pass number.
 * @n_bytes: Number of bytes in @src.
 *
 * Samples the bits in @src for 4-pass, 1-bit operation.
 *
 * The number of bytes in @dst should be (@n_bytes + 1) >> 1.
 **/
static void
rinkj_escp_shuffle_4pass_1bit (char *dst, const char *src, int pass, int n_bytes)
{
  int i;
  int n_dst;
  unsigned char s0, s1, s2, s3;
  int shift = pass;

  n_dst = n_bytes >> 2;
  for (i = 0; i < n_dst; i++)
    {
      s0 = src[i * 4] << shift;
      s1 = src[i * 4 + 1] << shift;
      s2 = src[i * 4 + 2] << shift;
      s3 = src[i * 4 + 3] << shift;
      dst[i] = (s0 & 0x80) | ((s0 & 8) << 3) |
	((s1 & 0x80) >> 2) | ((s1 & 8) << 1) |
	((s2 & 0x80) >> 4) | ((s2 & 8) >> 1) |
	((s3 & 0x80) >> 6) | ((s3 & 8) >> 3);
    }
  if (n_bytes & 3)
    {
      char d = 0;

      for (i = 0; i < (n_bytes & 3); i++)
	{
	  s0 = src[n_dst * 4 + i] << shift;
	  d |= ((s0 & 0x80) | ((s0 & 8) << 3)) >> (i << 1);
	}
      dst[n_dst] = d;
    }
}

/**
 * rinkj_escp_shuffle_2pass_2bit: Shuffle bits for doubled X resolution.
 * @dst: Where to store shuffled bits.
 * @src: Source of bits.
 * @pass: Pass number.
 * @n_bytes: Number of bytes in @src.
 *
 * Samples the bits in @src for 2-pass, 2-bit operation.
 *
 * The number of bytes in @dst should be (@n_bytes + 1) >> 1.
 **/
static void
rinkj_escp_shuffle_2pass_2bit (char *dst, const char *src, int pass, int n_bytes)
{
  int i;
  int n_dst;
  unsigned char s0, s1;
  int shift = pass << 1;

  n_dst = n_bytes >> 1;
  for (i = 0; i < n_dst; i++)
    {
      s0 = src[i * 2] << shift;
      s1 = src[i * 2 + 1] << shift;
      dst[i] = (s0 & 0xc0) | ((s0 & 0x0c) << 2) |
	((s1 & 0xc0) >> 4) | ((s1 & 0x0c) >> 2);
    }
  if (n_bytes & 1)
    {
      s0 = src[n_bytes - 1] << shift;
      dst[i] = (s0 & 0xc0) | ((s0 & 0x0c) << 2);
    }
}

/**
 * rinkj_escp_shuffle_4pass_2bit: Shuffle bits for quadrupled X resolution.
 * @dst: Where to store shuffled bits.
 * @src: Source of bits.
 * @pass: Pass number.
 * @n_bytes: Number of bytes in @src.
 *
 * Samples the bits in @src for 4-pass, 2-bit operation.
 *
 * The number of bytes in @dst should be (@n_bytes + 1) >> 1.
 **/
static void
rinkj_escp_shuffle_4pass_2bit (char *dst, const char *src, int pass, int n_bytes)
{
  int i;
  int n_dst;
  unsigned char s0, s1, s2, s3;
  int shift = pass << 1;

  n_dst = n_bytes >> 2;
  for (i = 0; i < n_dst; i++)
    {
      s0 = src[i * 4] << shift;
      s1 = src[i * 4 + 1] << shift;
      s2 = src[i * 4 + 2] << shift;
      s3 = src[i * 4 + 3] << shift;
      dst[i] = (s0 & 0xc0) | ((s1 & 0xc0) >> 2) |
	((s2 & 0xc0) >> 4) | ((s3 & 0xc0) >> 6);
    }
  if (n_bytes & 3)
    {
      char d = 0;

      for (i = 0; i < (n_bytes & 3); i++)
	d |= ((src[n_dst * 4 + i] << shift) & 0xc0) >> (i << 1);
      dst[n_dst] = d;
    }
}

#define DOT 3

static void
rinkj_escp_1pass_dblx (char *dst, const char *src, int n_bytes)
{
  int i;

  /* todo: may need half-byte fixup */
  for (i = 0; i < n_bytes; i ++)
    {
      unsigned char s = src[i];
      dst[i * 2] = (((s & 0x80) >> 1) | ((s & 0x40) >> 2) |
		    ((s & 0x20) >> 3) | ((s & 0x10) >> 4)) * DOT;
      dst[i * 2 + 1] = (((s & 8) << 3) | ((s & 4) << 2) |
		    ((s & 2) << 1) | (s & 1)) * DOT;
    }
}

static void
rinkj_escp_select_dblx (char *dst, const char *src, int x_pass, int n_bytes)
{
  int i;

  for (i = 0; i < n_bytes; i++)
    dst[i] = (src[i] >> (1 - x_pass) & 0x55) * DOT;
}

static void
rinkj_escp_sel_shuffle_dblx (char *dst, const char *src, int pass, int n_bytes)
{
  int i;
  int n_dst;
  unsigned char s0, s1;

  n_dst = n_bytes >> 1;
  for (i = 0; i < n_dst; i++)
    {
      s0 = src[i * 2] << pass;
      s1 = src[i * 2 + 1] << pass;
      dst[i] = (((s0 & 0x80) >> 1) | ((s0 & 8) << 1) |
	((s1 & 0x80) >> 5) | ((s1 & 0x8) >> 3)) * DOT;
    }
  if (n_bytes & 1)
    {
      s0 = src[n_bytes - 1] << pass;
      dst[n_dst] = (((s0 & 0x80) >> 1) | ((s0 & 8) << 1)) * DOT;
    }
}

static void
rinkj_escp_shuffle (char *dst, const char *src, int pass, int n_bytes,
		    int passes_per_scan, int bps, int head_bps)
{
  if (bps == 2 && head_bps == 2)
    {
      if (passes_per_scan == 1)
	memcpy (dst, src, n_bytes);
      else if (passes_per_scan == 2)
	rinkj_escp_shuffle_2pass_2bit (dst, src, pass, n_bytes);
      else if (passes_per_scan == 4)
	rinkj_escp_shuffle_4pass_2bit (dst, src, pass, n_bytes);
    }
  else if (bps == 1 && head_bps == 2)
    {
      if (passes_per_scan == 1)
	rinkj_escp_1pass_dblx (dst, src, n_bytes);
      if (passes_per_scan == 2)
	rinkj_escp_select_dblx (dst, src, pass, n_bytes);
      else if (passes_per_scan == 4)
	rinkj_escp_sel_shuffle_dblx (dst, src, pass, n_bytes);
    }
  else if (bps == 1 && head_bps == 1)
    {
      if (passes_per_scan == 1)
	memcpy (dst, src, n_bytes);
      else if (passes_per_scan == 2)
	rinkj_escp_shuffle_dblx (dst, src, pass, n_bytes);
      else if (passes_per_scan == 4)
	rinkj_escp_shuffle_4pass_1bit (dst, src, pass, n_bytes);
    }
}

/**
 * rinkj_escp_compress_rle: Compress a chunk of data using runlengths.
 * @dst: Where to store compressed data.
 * @src: Source of data.
 * @n: Size of @src in bytes.
 *
 * Compresses a chunk of data using ESCP/2 runlength encoding. @dst must be
 * at least @n + ((@n + 127) >> 7) bytes long.
 *
 * Return value: size of @dst in bytes.
 **/
static int
rinkj_escp_compress_rle (char *dst, const char *src, int n)
{
  int i, j;
  int b;
  int run;

  j = 0;
  for (i = 0; i < n; i += run)
    {
      b = src[i];
      for (run = 1; run < 129 && i + run < n; run++)
	if (b != src[i + run])
	  break;
      if (run > 2)
	{
	  dst[j++] = 257 - run;
	  dst[j++] = b;
	}
      else
	{
	  for (run = 1; run < 128 && i + run < n; run++)
	    {
	      b = src[i + run];
	      if (i + run + 2 < n &&
		  b == src[i + run + 1] && b == src[i + run + 2])
		break;
	    }
	  dst[j++] = run - 1;
	  memcpy (dst + j, src + i, run);
	  j += run;
	}
    }

  return j;
}

static int
rinkj_escp_flush (RinkjEscp *z)
{
  int xsb, xsb_out;
  int xs_out;
  int status;
  const int plane[7] = {3, 1, 0, 2, 5, 4, 6};
  const int color[7] = {0, 1, 2, 4, 17, 18, 16};
  int i, j;
  int ytop, ysc;
  int bufy;
  char *thisbuf;
  char *compress_buf = NULL;
  int rle = 1;
  int x_pass;
  char pass_mask;
  int m;

  ytop = rinkj_escp_ytop (z, z->pass, &x_pass);
  pass_mask = 1 << x_pass;

#ifdef VERBOSE
  fprintf (stderr, "flush pass %d: ytop = %d (= %d mod %d), x_pass = %d\n",
	   z->pass, ytop, ytop % z->spacing, z->spacing, x_pass);
#endif

  xsb = (z->width * z->bps + 7) >> 3;

  xs_out = (z->width + z->passes_per_scan - 1) / (z->passes_per_scan);
  xsb_out = (((z->width * z->head_bps + 7) >> 3) + z->passes_per_scan - 1) /
    (z->passes_per_scan);

  thisbuf = malloc (xsb_out);
  if (rle)
    compress_buf = malloc (xsb_out + ((xsb_out + 127) >> 7));
  ysc = ytop;
  if (z->vertpos == -1)
    status = rinkj_byte_stream_printf (z->out, "\033(V%c%c%c%c",
				       2, 0, ysc & 0xff, (ysc >> 8) & 0xff);
  else
    {
      int yrel = ysc - z->vertpos;
      status = rinkj_byte_stream_printf (z->out, "\033(v%c%c%c%c%c%c",
					 4, 0, yrel & 0xff, (yrel >> 8) & 0xff, (yrel >> 16) & 0xff, (yrel >> 24) & 0xff);
    }

  z->vertpos = ysc;

  if (status < 0) return status;

  for (i = 0; i < z->num_chan; i++)
    {
      int plane_off = z->plane_offsets[i];

      m = (z->y - ytop + z->spacing - 1 - plane_off) / z->spacing;
      if (m > z->n_pins)
	m = z->n_pins;

      if (m <= 0)
	continue;

      if (1)
	{
	  /* todo: make this calculation fully agile */
	  int x = (x_pass & 3);
	  status = rinkj_byte_stream_printf (z->out, "\033($\4%c%c%c%c%c",
					     0, x & 0xff,
					     (x >> 8) & 0xff,
					     (x >> 16) & 0xff,
					     (x >> 24) & 0xff);

	  if (status < 0) return status;
	}

      status = rinkj_byte_stream_printf (z->out, "\033i%c%c%c%c%c%c%c",
					 color[i],
					 rle,
					 z->head_bps,
					 xsb_out & 0xff,
					 (xsb_out >> 8) & 0xff,
					 m & 0xff, m >> 8);
      if (status < 0) return status;
      for (j = 0; j < m; j++)
	{
	  const char *line;

	  bufy = (ytop + j * z->spacing + plane_off) % z->bufheight;
	  line = z->buf + bufy * z->rowstride + plane[i] * z->planestride;
	  if (z->buf_linevalid[bufy * z->num_chan + i] & pass_mask)
	    rinkj_escp_shuffle (thisbuf, line, x_pass, xsb,
				z->passes_per_scan, z->bps, z->head_bps);
	  else
	    memset (thisbuf, 0, xsb_out);
	  z->buf_linevalid[bufy * z->num_chan + i] &= ~pass_mask;
#ifdef VERBOSE
	  if (i == 0 && j == 0)
	    {
	      fprintf (stderr, "flush line[0] = %d shuffled[0] = %d\n",
		       line[0], thisbuf[0]);
	    }
#endif
	  if (rle)
	    {
	      int nbytes;

	      nbytes = rinkj_escp_compress_rle (compress_buf, thisbuf, xsb_out);
	      status = rinkj_byte_stream_write (z->out, compress_buf, nbytes);
	    }
	  else
	    status = rinkj_byte_stream_write (z->out, thisbuf, xsb_out);
	}
      if (status < 0) return status;
#if 0
      status = rinkj_byte_stream_puts (z->out, "\r");
      if (status < 0) return status;
#endif
    }

  z->pass++;
  if (rle)
    free (compress_buf);
  free (thisbuf);
  return status;
}

static int
rinkj_escp_flush_bottom (RinkjEscp *z)
{
  int ytop;
  int status;

  for (;;)
    {
      ytop = rinkj_escp_ytop (z, z->pass, NULL);
      if (ytop >= z->y)
	break;
      status = rinkj_escp_flush (z);
      if (status != 0)
	return status;
    }
  return 0;
}

static void
rinkj_escp_free (RinkjEscp *z)
{
  if (z->manufacturer)
    free (z->manufacturer);
  if (z->model)
    free (z->model);
  free (z->buf);
  free (z->buf_linevalid);
  free (z);
}

static int
rinkj_escp_write (RinkjDevice *self, const char **data)
{
  RinkjEscp *z = (RinkjEscp *)self;
  int xsb;
  int status;
  int i;
  int ytop;
  int bufy;
  int dblx_pass;

  if (data == NULL)
    {
      status = rinkj_escp_flush_bottom (z);
      /* todo: check error */
      status = rinkj_byte_stream_puts (z->out, "\f\033@");
      /* todo: check error */
      status = rinkj_byte_stream_close (z->out);
      rinkj_escp_free (z);
      return status;
    }

  xsb = (z->width * z->bps + 7) >> 3;
  bufy = z->y % z->bufheight;
  for (i = 0; i < z->num_chan; i++)
    {
      memcpy (z->buf + bufy * z->rowstride + i * z->planestride, data[i], xsb);
      z->buf_linevalid[bufy * z->num_chan + i] = 0xff;
    }

  z->y++;

  ytop = rinkj_escp_ytop (z, z->pass, &dblx_pass);

  if (z->y < ytop + (z->n_pins - 1) * z->spacing + 1 + z->max_offset)
    return 0;

  return rinkj_escp_flush (z);
}

RinkjDevice *
rinkj_epson870_new (RinkjByteStream *out)
{
  RinkjEscp *result;

  result = (RinkjEscp *)malloc (sizeof (RinkjEscp));

  result->super.set = rinkj_escp_set;
  result->super.write = rinkj_escp_write;
  result->super.init = rinkj_escp_init;
  result->super.init_happened = 0;
  result->width = 0;
  result->out = out;

  result->num_chan = 4;
  result->bps = 1;

  result->manufacturer = NULL;
  result->model = NULL;

  result->autocut = -1;
  result->microdot = -1;
  result->unidir = -1;
  result->printer_weave = -1;

  return &result->super;
}
