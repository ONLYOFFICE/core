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
#include "ijs_server.h"

#define BUF_SIZE 4096

typedef struct _ExampleParamList ExampleParamList;

struct _ExampleParamList {
  ExampleParamList *next;
  char *key;
  char *value;
  int value_size;
};

static int
example_status_cb (void *status_cb_data,
		  IjsServerCtx *ctx,
		  IjsJobId job_id)
{
  return 0;
}

static int
example_list_cb (void *list_cb_data,
		 IjsServerCtx *ctx,
		 IjsJobId job_id,
		 char *val_buf,
		 int val_size)
{
  const char *param_list = "OutputFile,DeviceManufacturer,DeviceModel,PageImageFormat,Dpi,Width,Height,BitsPerSample,ColorSpace,NumChan,PaperSize,PrintableArea,PrintableTopLeft,TopLeft";
  int size = strlen (param_list);

  fprintf (stderr, "example_list_cb\n");

  if (size > val_size)
    return IJS_EBUF;

  memcpy (val_buf, param_list, size);
  return size;
}

static int
example_enum_cb (void *enum_cb_data,
		 IjsServerCtx *ctx,
		 IjsJobId job_id,
		 const char *key,
		 char *val_buf,
		 int val_size)
{
  const char *val = NULL;
  if (!strcmp (key, "ColorSpace"))
    val = "DeviceRGB,DeviceGray,DeviceCMYK";
  else if (!strcmp (key, "DeviceManufacturer"))
    val = "IJS Distribution";
  else if (!strcmp (key, "DeviceModel"))
    val = "ijs_server_example";
  else if (!strcmp (key, "PageImageFormat"))
    val = "Raster";

  if (val == NULL)
    return IJS_EUNKPARAM;
  else
    {
      int size = strlen (val);

      if (size > val_size)
	return IJS_EBUF;
      memcpy (val_buf, val, size);
      return size;
    }
}

/* A C implementation of /^(\d\.+\-eE)+x(\d\.+\-eE)+$/ */
static int
example_parse_wxh (const char *val, int size,
		   double *pw, double *ph)
{
  char buf[256];
  char *tail;
  int i;

  for (i = 0; i < size; i++)
    if (val[i] == 'x')
      break;

  if (i + 1 >= size)
    return IJS_ESYNTAX;

  if (i >= sizeof(buf))
    return IJS_EBUF;

  memcpy (buf, val, i);
  buf[i] = 0;
  *pw = strtod (buf, &tail);
  if (tail == buf)
    return IJS_ESYNTAX;

  if (size - i > sizeof(buf))
    return IJS_EBUF;

  memcpy (buf, val + i + 1, size - i - 1);
  buf[size - i - 1] = 0;
  *ph = strtod (buf, &tail);
  if (tail == buf)
    return IJS_ESYNTAX;

  return 0;
}

/**
 * example_find_key: Search parameter list for key.
 *
 * @key: key to look up
 *
 * Return value: ExampleParamList entry matching @key, or NULL.
 **/
static ExampleParamList *
example_find_key (ExampleParamList *pl, const char *key)
{
  ExampleParamList *curs;

  for (curs = pl; curs != NULL; curs = curs->next)
    {
      if (!strcmp (curs->key, key))
	return curs;
    }
  return NULL;
}

/**
 * @printable: An array in which to store the printable area.
 *
 * On return, @printable = PrintableArea[0:1] + TopLeft[0:1]
 **/
static int
example_compute_printable (ExampleParamList *pl, double printable[4])
{
  ExampleParamList *curs;
  double width, height;
  int code;
  double margin = 0.5;

  curs = example_find_key (pl, "PaperSize");
  if (curs == NULL)
    return -1;
  code = example_parse_wxh (curs->value, curs->value_size, &width, &height);

  if (code == 0)
    {
      printable[0] = width - 2 * margin;
      printable[1] = height - 2 * margin;
      printable[2] = margin;
      printable[3] = margin;
    }

  return code;
}

static int
example_compute_offset (ExampleParamList *pl, IjsPageHeader *ph,
			double *px0, double *py0)
{
  ExampleParamList *curs;
  double width, height;
  double top, left;
  int code;

  *px0 = 0;
  *py0 = 0;

  curs = example_find_key (pl, "PaperSize");
  if (curs == NULL)
    return -1;

  code = example_parse_wxh (curs->value, curs->value_size, &width, &height);

  if (code == 0)
    {
      curs = example_find_key (pl, "TopLeft");
      if (curs != NULL)
	{
	  code = example_parse_wxh (curs->value, curs->value_size,
				    &top, &left);
	}
      else
	{
	  double printable[4];

	  code = example_compute_printable (pl, printable);
	  if (code == 0)
	    {
	      top = printable[2];
	      left = printable[3];
	    }
	}
    }

  if (code == 0)
    {
      *px0 = left;
      *py0 = height - ph->height / ph->yres - top;
    }

  return code;
}

static int
example_get_cb (void *get_cb_data,
		 IjsServerCtx *ctx,
		 IjsJobId job_id,
		 const char *key,
		 char *val_buf,
		 int val_size)
{
  ExampleParamList *pl = *(ExampleParamList **)get_cb_data;
  ExampleParamList *curs;
  const char *val;
  char buf[256];
  int code;

  fprintf (stderr, "example_get_cb: %s\n", key);
  curs = example_find_key (pl, key);
  if (curs != NULL)
    {
      if (curs->value_size > val_size)
	return IJS_EBUF;
      memcpy (val_buf, curs->value, curs->value_size);
      return curs->value_size;
    }

  if (!strcmp (key, "PrintableArea") || !strcmp (key, "PrintableTopLeft"))
    {
      double printable[4];
      int off = !strcmp (key, "PrintableArea") ? 0 : 2;

      code = example_compute_printable (pl, printable);
      if (code == 0)
	{
	  sprintf (buf, "%gx%g", printable[off + 0], printable[off + 1]);
	  val = buf;
	}
    }

  if (!strcmp (key, "DeviceManufacturer"))
    val = "IJS Distribution";
  else if (!strcmp (key, "DeviceModel"))
    val = "ijs_server_example";
  else if (!strcmp (key, "PageImageFormat"))
    val = "Raster";

  if (val == NULL)
    return IJS_EUNKPARAM;
  else
    {
      int size = strlen (val);

      if (size > val_size)
	return IJS_EBUF;
      memcpy (val_buf, val, size);
      return size;
    }
}

static int
example_set_cb (void *set_cb_data, IjsServerCtx *ctx, IjsJobId job_id,
		const char *key, const char *value, int value_size)
{
  ExampleParamList **ppl = (ExampleParamList **)set_cb_data;
  ExampleParamList *pl;
  int key_len = strlen (key);
  int code;

  fprintf (stderr, "example_set_cb: %s=", key);

  if (!strcmp (key, "PaperSize"))
    {
      double width, height;

      code = example_parse_wxh (value, value_size, &width, &height);
      if (code < 0)
	return code;
    }

  fwrite (value, 1, value_size, stderr);
  fputs ("\n", stderr);

  pl = example_find_key (*ppl, key);

  if (pl == NULL)
    {
      pl = (ExampleParamList *)malloc (sizeof (ExampleParamList));
      pl->next = *ppl;
      pl->key = malloc (key_len + 1);
      memcpy (pl->key, key, key_len + 1);
      *ppl = pl;
    }
  else
    {
      free (pl->value);
    }

  pl->value = malloc (value_size);
  memcpy (pl->value, value, value_size);
  pl->value_size = value_size;
  return 0;
}

/**
 * Finds a parameter in the param list, and allocates a null terminated
 * string with the value.
 **/
static char *
find_param (ExampleParamList *pl, const char *key)
{
  ExampleParamList *curs;
  char *result;

  curs = example_find_key (pl, key);
  if (curs == NULL)
    return NULL;

  result = malloc (curs->value_size + 1);
  memcpy (result, curs->value, curs->value_size);
  result[curs->value_size] = 0;
  return result;
}

static void
free_param_list (ExampleParamList *pl)
{
  ExampleParamList *next;

  for (; pl != NULL; pl = next)
    {
      next = pl->next;
      free (pl->key);
      free (pl->value);
      free (pl);
    }
}

int
main (int argc, char **argv)
{
  IjsServerCtx *ctx;
  IjsPageHeader ph;
  int status;
  char buf[BUF_SIZE];
  char hexbuf[BUF_SIZE * 3];
  char *fn;
  FILE *f = NULL;
  double xscale, yscale;
  double x0, y0;
  ExampleParamList *pl = NULL;

  ctx = ijs_server_init ();
  if (ctx == NULL)
    return (1);
  ijs_server_install_status_cb (ctx, example_status_cb, &pl);
  ijs_server_install_list_cb (ctx, example_list_cb, &pl);
  ijs_server_install_enum_cb (ctx, example_enum_cb, &pl);
  ijs_server_install_set_cb (ctx, example_set_cb, &pl);
  ijs_server_install_get_cb (ctx, example_get_cb, &pl);

  do 
    {
      int total_bytes, bytes_left;
      ExampleParamList *curs;

      status = ijs_server_get_page_header (ctx, &ph);
      if (status) break;
      fprintf (stderr, "got page header, %d x %d\n",
	      ph.width, ph.height);

      if (f == NULL)
	{
	  fn = find_param (pl, "OutputFile");
	  /* todo: check error! */

	  if (fn == NULL)
	    {
	      fn = find_param (pl, "OutputFD");
	      if (fn != NULL)
		{
		  f = fdopen (atoi (fn), "w");
		}
	    }
	  else
	    {
	      f = fopen (fn, "w");
	    }
	  if (f == NULL)
	    {
	      fprintf (stderr, "can't open output file %s\n", fn);
	      fclose (stdin);
	      fclose (stdout);
	      break;
	    }
	  if (fn != NULL)
	    free (fn);
	}

      fprintf (f, "%%!PS-Adobe-2.0\n");

      example_compute_offset (pl, &ph, &x0, &y0);

      xscale = 72.0 / ph.xres;
      yscale = 72.0 / ph.yres;

      fprintf (f, "%%%%BoundingBox: %d %d %d %d\n",
	       (int)(x0 * 72), (int)(y0 * 72),
	       (int)(x0 * 72 + xscale * ph.width + 0.999),
	       (int)(y0 * 72 + yscale * ph.height + 0.999));

      fprintf (f, "/rhex { currentfile exch readhexstring pop } bind def\n");
      fprintf (f, "/picstr %d string def\n", ph.width);

      for (curs = pl; curs != NULL; curs = curs->next)
	{
	  fprintf (f, "%% IJS parameter: %s = ", curs->key);
	  fwrite (curs->value, 1, curs->value_size, f);
	  fputs ("\n", f);
	}

      fprintf (f,
	       "gsave\n"
	       "%f %f translate\n"
	       "%f %f scale\n"
	       "%d %d %d\n"
	       "[ %d 0 0 %d 0 %d ]\n",
	       x0 * 72, y0 * 72,
	       xscale * ph.width, yscale * ph.height,
	       ph.width, ph.height, ph.bps,
	       ph.width, -ph.height, ph.height);
      if (ph.n_chan == 1)
	fprintf (f, "{ picstr rhex } image\n");
      else
	{
	  fprintf (f, "{ picstr rhex }\n"
		   "false %d colorimage\n", ph.n_chan);
	}
      total_bytes = ((ph.n_chan * ph.bps * ph.width + 7) >> 3) * ph.height;
      bytes_left = total_bytes;
      while (bytes_left)
	{
	  int n_bytes = bytes_left;
	  int i, j;

	  if (n_bytes > sizeof(buf))
	    n_bytes = sizeof(buf);
#ifdef VERBOSE
	  fprintf (stderr, "%d bytes left, reading %d\n", bytes_left, n_bytes);
#endif
	  status = ijs_server_get_data (ctx, buf, n_bytes);
	  if (status)
	    {
	      fprintf (stderr, "page aborted!\n");
	      break;
	    }
	  j = 0;
	  for (i = 0; i < n_bytes; i++)
	    {
	      const char hex[16] = "0123456789AbCdEf";
	      unsigned char c = ((unsigned char *)buf)[i];

	      hexbuf[j++] = hex[c >> 4];
	      hexbuf[j++] = hex[c & 0xf];
	      if ((i & 31) == 31)
		hexbuf[j++] = '\n';
	    }
	  if ((n_bytes & 31) != 0)
	    hexbuf[j++] = '\n';
	  fwrite (hexbuf, 1, j, f);
	  bytes_left -= n_bytes;
	}
      fprintf (f, "grestore\nshowpage\n");
    }
  while (status == 0);

  if (status > 0) status = 0; /* normal exit */

  if (f)
    fclose (f);
  ijs_server_done (ctx);

  free_param_list (pl);

#ifdef VERBOSE
  fprintf (stderr, "server exiting with status %d\n", status);
#endif
  return status;
}
