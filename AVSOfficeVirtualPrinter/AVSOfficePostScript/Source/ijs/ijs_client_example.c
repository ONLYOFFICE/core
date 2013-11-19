/**
 * Copyright (C) 2001-2002 Artifex Software, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ijs.h"
#include "ijs_client.h"

static int
example_list_params (IjsClientCtx *ctx)
{
  int status = 0;
  char buf[4096];
  char ebuf[4096];
  char *p;
  status = ijs_client_list_params (ctx, 0, buf, sizeof(buf) - 1);
  if (status >= 0)
    {
      buf[status] = 0;
      fprintf (stderr, "settable params: %s\n", buf);
      p = strtok(buf, ",");
      while (p)
	{
	  status = ijs_client_enum_param (ctx, 0, p, ebuf, sizeof(ebuf) - 1);
	  if (status >= 0)
	    {
	      ebuf[status] = 0;
	      fprintf (stderr, "  %s: %s\n", p, ebuf);
	    }
	  else
	    {
	      fprintf (stderr, "Error %d getting param %s\n", status, p);
	    }
	  p = strtok(NULL, ",");
        }
    }
  else
    {
      fprintf (stderr, "Error %d listing params\n", status);
    }
  return status;
}

static int
send_pnm_file (IjsClientCtx *ctx, FILE *f, int xres, int yres)
{
  int width, height;
  char *lp, type;
  int total_bytes, bytes_left;
  int n_chan, bps;
  char buf[4096];
  int status = 0;

  lp = fgets (buf, sizeof(buf), f);
  if (lp == NULL)
    {
      fprintf (stderr, "error reading file\n");
      return 1;
    }
  if (lp[0] != 'P' || lp[1] < '4' || lp[1] > '6')
    {
      fprintf (stderr, "need pnmraw file\n");
      return 1;
    }
  type = lp[1];
  do
    {
      lp = fgets (buf, sizeof(buf), f);
    }
  while (lp != NULL && lp[0] == '#');
  if (sscanf (lp, "%d %d", &width, &height) != 2)
    {
      fprintf (stderr, "format error\n");
      return 1;
    }
  if (type >= '5')
    {
      /* skip depth */
      do
	{
	  lp = fgets (buf, sizeof(buf), f);
	}
      while (lp != NULL && lp[0] == '#');
    }

  n_chan = (type == '6') ? 3 : 1;
  bps = (type == '4') ? 1 : 8;

  /* Set required parameters. Note: we should be checking the return
     values. */
  sprintf (buf, "%d", n_chan);
  ijs_client_set_param (ctx, 0, "NumChan", buf, strlen (buf));
  sprintf (buf, "%d", bps);
  ijs_client_set_param (ctx, 0, "BitsPerSample", buf, strlen (buf));
  strcpy (buf, (n_chan == 3) ? "DeviceRGB" : "DeviceGray");
  ijs_client_set_param (ctx, 0, "ColorSpace", buf, strlen (buf));
  sprintf (buf, "%d", width);
  ijs_client_set_param (ctx, 0, "Width", buf, strlen (buf));
  sprintf (buf, "%d", height);
  ijs_client_set_param (ctx, 0, "Height", buf, strlen (buf));
  sprintf (buf, "%dx%d", xres, yres);
  ijs_client_set_param (ctx, 0, "Dpi", buf, strlen (buf));

  ijs_client_begin_page (ctx, 0);

  total_bytes = ((n_chan * bps * width + 7) >> 3) * height;
  bytes_left = total_bytes;
  while (bytes_left)
    {
      int n_bytes = bytes_left;
      if (n_bytes > sizeof(buf))
	n_bytes = sizeof(buf);
      fread (buf, 1, n_bytes, f); /* todo: check error */
      if (type == '4')
	{
	  /* invert pbm so black is 0, as per DeviceGray color space */
	  int i;
	  for (i = 0; i < n_bytes; i++)
	    buf[i] ^= 0xff;
	}
      status = ijs_client_send_data_wait (ctx, 0, buf, n_bytes);
      if (status)
	break;
      bytes_left -= n_bytes;
    }

  ijs_client_end_page (ctx, 0);

  return status;
}

static void
verify_context (IjsClientCtx *ctx)
{
  if (ctx == NULL)
    {
      fprintf (stderr, "Must specify valid server with -s flag\n");
      exit (1);
    }
}


static void
param_usage (void)
{
  fprintf (stderr, "parameter list must be in key=value, key=value format\n");
}

static void
example_set_params (IjsClientCtx *ctx, const char *arg)
{
  int code;
  int i, inext;
  char key[256];
  char buf[4096];
  int buf_ix;

  for (i = 0; arg[i] != 0; i = inext)
    {
      int ibeg, ieq, iend;
      int key_size;

      for (ibeg = i; arg[ibeg] == ' '; ibeg++);

      for (ieq = ibeg; arg[ieq] != 0; ieq++)
	{
	  if (arg[ieq] == '=')
	    break;
	}
      if (arg[ieq] == 0)
	{
	  param_usage ();
	  return;
	}
      for (iend = ieq; iend >= ibeg; iend--)
	if (arg[iend - 1] != ' ')
	  break;
      if (iend == ibeg)
	{
	  param_usage ();
	  return;
	}
      key_size = iend - ibeg;
      if (key_size + 1 > sizeof(key))
	{
	  fprintf (stderr, "Key exceeds %d bytes\n", sizeof(key));
	  return;
	}
      memcpy (key, arg + ibeg, key_size);
      key[key_size] = 0;
      buf_ix = 0;
      for (i = ieq + 1; arg[i] == ' '; i++);
      for (; arg[i] != 0; i++)
	{
	  if (arg[i] == ',')
	    break;
	  if (buf_ix == sizeof(buf))
	    {
	      fprintf (stderr, "Value for %s exceeds %d bytes\n",
		       key, sizeof(buf));
	      return;
	    }
	  if (arg[i] == '\\' && arg[i + 1] != 0)
	    buf[buf_ix++] = arg[++i];
	  else
	    buf[buf_ix++] = arg[i];
	}
      if (arg[i] == ',')
	inext = i + 1;
      else
	inext = i;
      code = ijs_client_set_param (ctx, 0, key, buf, buf_ix);
      if (code < 0)
	fprintf (stderr, "Warning: error %d setting parameter %s\n",
		 code, key);
    }
}

static void
example_get_param (IjsClientCtx *ctx, const char *arg)
{
  char buf[4096];
  int status;

  status = ijs_client_get_param (ctx, 0, arg, buf, sizeof(buf) - 1);
  if (status >= 0)
    {
      buf[status] = 0;
      fprintf (stderr, "value of param %s = %s\n", arg, buf);
    }
  else
    {
      fprintf (stderr, "Error %d getting param %s\n", status, arg);
    }
}

static void
example_enum_param (IjsClientCtx *ctx, const char *arg)
{
  char buf[4096];
  int status;

  status = ijs_client_enum_param (ctx, 0, arg, buf, sizeof(buf) - 1);
  if (status >= 0)
    {
      buf[status] = 0;
      fprintf (stderr, "enumeration of param %s: %s\n", arg, buf);
    }
  else
    {
      fprintf (stderr, "Error %d getting param %s\n", status, arg);
    }
}

static const char *
get_arg (int argc, char **argv, int *pi, const char *arg)
{
  if (arg[0] != 0)
    return arg;
  else
    {
      (*pi)++;
      if (*pi == argc)
	return NULL;
      else
	return argv[*pi];
    }
}

int
main (int argc, char **argv)
{
  IjsClientCtx *ctx;
  int i;
  int xres = 300, yres = 300;

  ctx = NULL;

  for (i = 1; i < argc; i++)
    {
      const char *arg = argv[i];

      if (arg[0] == '-')
	{
	  switch (arg[1])
	    {
	    case  'r':
	      {
		char *tail;

		arg = get_arg (argc, argv, &i, arg + 2);
		xres = strtol (arg, &tail, 10);
		if (tail[0] == 0)
		  yres = xres;
		else if (tail[0] == 'x')
		  yres = strtol (tail + 1, &tail, 10);
	      }
	      break;
	    case 's':
	      arg = get_arg (argc, argv, &i, arg + 2);
	      ctx = ijs_invoke_server (arg);
	      ijs_client_open (ctx);
	      ijs_client_begin_job (ctx, 0);
	      break;
	    case 'p':
	      arg = get_arg (argc, argv, &i, arg + 2);
	      verify_context (ctx);
	      example_set_params (ctx, arg);
	      break;
	    case 'g':
	      arg = get_arg (argc, argv, &i, arg + 2);
	      verify_context (ctx);
	      example_get_param (ctx, arg);
	      break;
	    case 'e':
	      arg = get_arg (argc, argv, &i, arg + 2);
	      verify_context (ctx);
	      example_enum_param (ctx, arg);
	      break;
	    case 'l':
	      verify_context (ctx);
	      example_list_params (ctx);
	      break;
	    case 0:
	      verify_context (ctx);
	      send_pnm_file (ctx, stdin, xres, yres);
	      break;
	    }
	}
      else
	{
	  FILE *f = fopen (arg, "rb");

	  if (f == NULL)
	    {
	      fprintf (stderr, "error opening %s\n", arg);
	      return 1;
	    }
	  verify_context (ctx);
	  send_pnm_file (ctx, f, xres, yres);
	  fclose (f);
	}
    }

  verify_context (ctx);

  ijs_client_end_job (ctx, 0);
  ijs_client_close (ctx);

  /* todo - suffield race, proceduralize */
  ijs_client_begin_cmd (ctx, IJS_CMD_EXIT);
  ijs_client_send_cmd_wait (ctx);

  return 0;
}
