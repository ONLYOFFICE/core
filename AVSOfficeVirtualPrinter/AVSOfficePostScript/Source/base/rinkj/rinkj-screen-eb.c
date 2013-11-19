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

/*$Id: rinkj-screen-eb.c 8022 2007-06-05 22:23:38Z giles $ */
/* A Rinkj module encapsulating ETS screening. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "rinkj-device.h"
#include "rinkj-config.h"
#include "rinkj-dither.h"
#include "evenbetter-rll.h"
#include "rinkj-screen-eb.h"

#define MAX_CHAN 16

typedef struct _RinkjScreenEb RinkjScreenEb;

struct _RinkjScreenEb {
  RinkjDevice super;
  int width_out;
  int height_in, height_out;
  int n_planes;
  RinkjDevice *dev_out;
  EvenBetterCtx *dither;
  int **lut;
  double xscale, yscale;
  int yrem;
  int aspect;
  int bps;
};

static int
rinkj_screen_eb_set (RinkjDevice *self, const char *config)
{
  RinkjScreenEb *z = (RinkjScreenEb *)self;
  const char *p, *next;
  char *key, *val;

  for (p = config; (key = rinkj_config_keyval (p, &val, &next)); p = next)
    {
      if (!strcmp (key, "Dither"))
	{
	  if (!strcmp (val, "1"))
	    z->bps = 1;
	  else if (!strcmp (val, "2"))
	    {
	      z->bps = 2;
	      rinkj_device_set_param_int (z->dev_out, "BitsPerSample", 2);
	    }
	}
      else if (!strcmp (key, "Aspect"))
	{
	  z->aspect = atoi (val);
	}
      free (key);
      free (val);
    }
  return 0;
}

static int
rinkj_screen_eb_init (RinkjDevice *self, const RinkjDeviceParams *params)
{
  RinkjScreenEb *z = (RinkjScreenEb *)self;
  int n_planes = params->n_planes;
  EvenBetterParams ebp;
  int strengths4[] = { 128, 50, 50, 10 };
  int strengths6[] = { 128, 50, 50, 25, 25, 10 };
  int strengths7[] = { 128, 80, 50, 50, 25, 26, 10 };
  int *strengths;

  RinkjDeviceParams out_params;

  out_params.width = floor (z->xscale * params->width + 0.5);
  out_params.height = floor (z->yscale * params->height + 0.5);
  out_params.n_planes = n_planes;
  out_params.plane_names = params->plane_names;

  z->yrem = params->height >> 1;

  if (n_planes == 4)
    strengths = strengths4;
  else if (n_planes == 6)
    strengths = strengths6;
  else if (n_planes == 7)
    strengths = strengths7;

  ebp.source_width = params->width;
  ebp.dest_width = out_params.width;
  ebp.levels = 1 << z->bps;
  ebp.luts = z->lut;
  ebp.n_planes = n_planes;
  ebp.aspect = z->aspect;
  ebp.rbscale = 0.0;
  ebp.strengths = strengths;
  ebp.rand_scale = 0;
  ebp.even_c1_scale = 1;
  ebp.do_shadows = 0;
  ebp.dump_file = NULL;
  ebp.gamma = 0;
  z->dither = even_better_new (&ebp);

  z->width_out = out_params.width;
  z->height_in = params->height;
  z->height_out = out_params.height;
  z->n_planes = n_planes;
#ifdef DEBUG_OUT
  fprintf (stdout, "P5\n%d %d\n255\n", params->width, params->height);
#endif
  return rinkj_device_init (z->dev_out, &out_params);
}

static int
rinkj_screen_eb_write (RinkjDevice *self, const char **data)
{
  RinkjScreenEb *z = (RinkjScreenEb *)self;
  uchar **out_data;
  uchar **out_buf;
  int i;
  int n_planes = z->n_planes;
  int xs = z->width_out;
  int xsb;
  int status;
  uchar **data_permuted;
  int cmyk_permutation[] = { 3, 0, 1, 2 };
  int ccmmyk_permutation[] = { 3, 0, 1, 4, 5, 2 };
  int ccmmykk_permutation[] = { 3, 6, 0, 1, 4, 5, 2 };
  int *permutation;

  if (data == NULL)
    {
      status = rinkj_device_write (z->dev_out, NULL);
      even_better_free (z->dither);
      free (self);
      return status;
    }

  if (n_planes == 4)
    permutation = cmyk_permutation;
  else if (n_planes == 6)
    permutation = ccmmyk_permutation;
  else if (n_planes == 7)
    permutation = ccmmykk_permutation;
  else
    /* NYI */
    return -1;

#ifdef DEBUG_OUT
  fwrite (data[0], 1, z->width, stdout);
#endif

  xsb = (xs * z->bps + 7) >> 3;

  out_data = (uchar **)malloc (n_planes * sizeof(char *));
  out_buf = (uchar **)malloc (n_planes * sizeof(char *));
  data_permuted = (uchar **)malloc (n_planes * sizeof(char *));

  for (i = 0; i < n_planes; i++)
    {
      out_data[i] = malloc (xsb);
      out_buf[i] = malloc (xs);
      data_permuted[i] = data[permutation[i]];
    }

  status = 0;
  for (; status >= 0 && z->yrem < z->height_out; z->yrem += z->height_in)
    {
      even_better_line (z->dither,
			(unsigned char *)out_buf,
			(unsigned char *)data_permuted);

      for (i = 0; i < n_planes; i++)
	{
	  uchar *pd = out_data[permutation[i]];
	  uchar *pb = out_buf[i];
	  int x;

	  if (z->bps == 2)
	    {
	      for (x = 0; x < xs - 3; x += 4)
		{
		  pd[x >> 2] = (pb[x] << 6) | (pb[x + 1] << 4) |
		    (pb[x + 2] << 2) | pb[x + 3];
		}
	      if (x < xs)
		{
		  int j;
		  uchar b = 0;

		  for (j = 0; j < xs - x; j++)
		    b |= pb[x + j] << ((3 - j) << 1);
		  pd[x >> 2] = b;
		}
	    }
	  else if (z->bps == 1)
	    {
	      for (x = 0; x < xs - 7; x += 8)
		{
		  pd[x >> 3] = (pb[x] << 7) | (pb[x + 1] << 6) |
		    (pb[x + 2] << 5) | (pb[x + 3] << 4) |
		    (pb[x + 4] << 3) | (pb[x + 5] << 2) |
		    (pb[x + 6] << 1) | (pb[x + 7] << 0);
		}
	      if (x < xs)
		{
		  int j;
		  uchar b = 0;

		  for (j = 0; j < xs - x; j++)
		    b |= pb[x + j] << (7 - j);
		  pd[x >> 3] = b;
		}
	    }
	}

      status = rinkj_device_write (z->dev_out, (const char **)out_data);
    }
  z->yrem -= z->height_out;

  for (i = 0; i < n_planes; i++)
    {
      free (out_data[i]);
      free (out_buf[i]);
    }
  free (out_data);
  free (out_buf);
  free (data_permuted);
  return status;
}

/**
 * rinkj_screen_eb_new: Create a new Screen object.
 * @dev_out: Output device to take screened image.
 *
 * Creates a new Screen object. This implementation currently just
 * iterates a Dither for all planes in the image, although other
 * implementations (such as a dithering algorithm that works on
 * multiple planes concurrently) are also possible.
 *
 * Return value: a #RinkjDevice to take separated contone image.
 **/
RinkjDevice *
rinkj_screen_eb_new (RinkjDevice *dev_out)
{
  RinkjScreenEb *result;

  result = (RinkjScreenEb *)malloc (sizeof(RinkjScreenEb));

  result->super.set = rinkj_screen_eb_set;
  result->super.write = rinkj_screen_eb_write;
  result->super.init = rinkj_screen_eb_init;
  result->super.init_happened = 0;
  result->dev_out = dev_out;
  result->lut = NULL;
  result->xscale = 1.0;
  result->yscale = 1.0;
  result->aspect = 1;
  result->bps = 1;

  return &result->super;
}

/* todo: move to _set */
void
rinkj_screen_eb_set_scale (RinkjDevice *self, double xscale, double yscale)
{
  RinkjScreenEb *z = (RinkjScreenEb *)self;
  z->xscale = xscale;
  z->yscale = yscale;
}

void
rinkj_screen_eb_set_gamma (RinkjDevice *self, int plane, double gamma, double max)
{
  RinkjScreenEb *z = (RinkjScreenEb *)self;
  int i;

  if (plane >= MAX_CHAN)
    return;

  if (z->lut == NULL)
    z->lut = (int **)malloc (MAX_CHAN * sizeof (int *));

  z->lut[plane] = (int *)malloc (256 * sizeof (int));
  for (i = 0; i < 256; i++)
    {
      double v;
      v = pow (i * (1.0 / 255.0), gamma);
      v += (1 - v) * (1 - max);
      z->lut[plane][i] = floor (0.5 + 0x1000000 * v);
    }
}

void
rinkj_screen_eb_set_lut (RinkjDevice *self, int plane, const double *lut)
{
  RinkjScreenEb *z = (RinkjScreenEb *)self;
  int i;

  if (plane >= MAX_CHAN)
    return;

  if (z->lut == NULL)
    z->lut = (int **)malloc (MAX_CHAN * sizeof (int *));

  z->lut[plane] = (int *)malloc (256 * sizeof (int));
  for (i = 0; i < 256; i++)
    {
      double v;
      v = 1.0 - lut[i];
      z->lut[plane][i] = floor (0.5 + 0x1000000 * v);
    }
}
