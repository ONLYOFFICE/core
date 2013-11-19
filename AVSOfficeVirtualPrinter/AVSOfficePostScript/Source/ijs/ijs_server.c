/**
 * Copyright (C) 2001-2004 Artifex Software, Inc.
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

#include <stdlib.h>
#include <stdio.h>
#include "unistd_.h"
#include <string.h>

#include "ijs.h"
#include "ijs_server.h"

#define noVERBOSE

typedef enum {
  IJS_N_CHAN_SET = 1,
  IJS_BPS_SET = 2,
  IJS_CS_SET = 4,
  IJS_WIDTH_SET = 8,
  IJS_HEIGHT_SET = 16,
  IJS_DPI_SET = 32
} IjsFieldsSet;

#define IJS_FIELDS_REQUIRED (IJS_N_CHAN_SET | IJS_BPS_SET | IJS_CS_SET | \
  IJS_WIDTH_SET | IJS_HEIGHT_SET | IJS_DPI_SET)

struct _IjsServerCtx {
  int fd_from;
  int child_pid;
  IjsSendChan send_chan;
  IjsRecvChan recv_chan;
  int version;

  /* callbacks */
  IjsBeginJobCb *begin_cb;
  void *begin_cb_data;
  IjsEndJobCb *end_cb;
  void *end_cb_data;
  IjsQueryStatusCb *status_cb;
  void *status_cb_data;
  IjsListParamsCb *list_cb;
  void *list_cb_data;
  IjsEnumParamCb *enum_cb;
  void *enum_cb_data;
  IjsSetParamCb *set_cb;
  void *set_cb_data;
  IjsGetParamCb *get_cb;
  void *get_cb_data;

  ijs_bool in_job;
  IjsJobId job_id;

  IjsPageHeader *ph;

  /* This should be IjsFieldsSet, but David Suffield reports that this
     causes problems when compiling with g++. */
  int fields_set;
  ijs_bool in_page;

  char *buf;
  int buf_size;
  int buf_ix;
  char *overflow_buf;
  int overflow_buf_size;
  int overflow_buf_ix;
};

static int
ijs_server_dummy_begin_cb (void *begin_cb_data,
			   IjsServerCtx *ctx,
			   IjsJobId job_id)
{
  return 0;
}

static int
ijs_server_dummy_end_cb (void *end_cb_data,
			 IjsServerCtx *ctx,
			 IjsJobId job_id)
{
  return 0;
}

IjsServerCtx *
ijs_server_init (void)
{
  ijs_bool ok = TRUE;
  char helo_buf[8];
  char resp_buf[8];
  int nbytes;
  IjsServerCtx *ctx = (IjsServerCtx *)malloc (sizeof(IjsServerCtx));
  int fd_from, fd_to;

  memcpy (resp_buf, IJS_RESP_STR, sizeof(resp_buf));

  fd_from = 0;
  fd_to = 1;
#ifdef _MSC_VER
  _setmode(fd_from, _O_BINARY);
  _setmode(fd_to, _O_BINARY);
#endif
#ifdef VERBOSE
  fprintf (stderr, "fd_from = %d, fd_to = %d\n", fd_from, fd_to);
#endif
  ijs_recv_init (&ctx->recv_chan, fd_from);
  ijs_send_init (&ctx->send_chan, fd_to);
  nbytes = read (ctx->recv_chan.fd, helo_buf, sizeof(helo_buf));

  if (nbytes != sizeof(helo_buf))
    ok = FALSE;

  if (ok)
    nbytes = write (ctx->send_chan.fd, resp_buf, sizeof(resp_buf));
  if (nbytes != sizeof(resp_buf))
    ok = FALSE;

  ctx->in_job = FALSE;
  ctx->job_id = -1;

  ctx->ph = NULL;
  ctx->in_page = FALSE;
  ctx->buf = NULL;
  ctx->overflow_buf = NULL;

  ctx->begin_cb = ijs_server_dummy_begin_cb;
  ctx->end_cb = ijs_server_dummy_end_cb;

  if (ok)
    return ctx;
  else
    {
      ijs_server_done (ctx);
      return NULL;
    }
}

int
ijs_server_install_begin_cb (IjsServerCtx *ctx,
			     IjsBeginJobCb *begin_cb, void *begin_cb_data)
{
  ctx->begin_cb = begin_cb;
  ctx->begin_cb_data = begin_cb_data;
  return 0;
}

int
ijs_server_install_end_cb (IjsServerCtx *ctx,
			   IjsEndJobCb *end_cb, void *end_cb_data)
{
  ctx->end_cb = end_cb;
  ctx->end_cb_data = end_cb_data;
  return 0;
}

int
ijs_server_install_status_cb (IjsServerCtx *ctx,
			      IjsQueryStatusCb *status_cb,
			      void *status_cb_data)
{
  ctx->status_cb = status_cb;
  ctx->status_cb_data = status_cb_data;
  return 0;
}

int
ijs_server_install_list_cb (IjsServerCtx *ctx,
			   IjsListParamsCb *list_cb, void *list_cb_data)
{
  ctx->list_cb = list_cb;
  ctx->list_cb_data = list_cb_data;
  return 0;
}

int
ijs_server_install_enum_cb (IjsServerCtx *ctx,
			   IjsEnumParamCb *enum_cb, void *enum_cb_data)
{
  ctx->enum_cb = enum_cb;
  ctx->enum_cb_data = enum_cb_data;
  return 0;
}

int
ijs_server_install_set_cb (IjsServerCtx *ctx,
			   IjsSetParamCb *set_cb, void *set_cb_data)
{
  ctx->set_cb = set_cb;
  ctx->set_cb_data = set_cb_data;
  return 0;
}

int
ijs_server_install_get_cb (IjsServerCtx *ctx,
			   IjsGetParamCb *get_cb, void *get_cb_data)
{
  ctx->get_cb = get_cb;
  ctx->get_cb_data = get_cb_data;
  return 0;
}

static int
ijs_server_ack (IjsServerCtx *ctx)
{
  int status;

  status = ijs_send_begin (&ctx->send_chan, IJS_CMD_ACK);
  if (status < 0)
    return status;
  return ijs_send_buf (&ctx->send_chan);
}

void
ijs_server_done (IjsServerCtx *ctx)
{
  /* todo: close channels */
  ijs_server_ack (ctx);

  free (ctx);
}

static int
ijs_server_nak (IjsServerCtx *ctx, int errorcode)
{
  int status;

  status = ijs_send_begin (&ctx->send_chan, IJS_CMD_NAK);
  if (status < 0)
    return status;
  status = ijs_send_int (&ctx->send_chan, errorcode);
  if (status < 0)
    return status;
  return ijs_send_buf (&ctx->send_chan);
}

/* The return code is: 0 if ok, positive on normal exit, negative on error */
typedef int (*ijs_server_proc) (IjsServerCtx *ctx);

static int
ijs_server_proc_ack (IjsServerCtx *ctx)
{
  /* servers should not get ack commands */
  return IJS_EPROTO;
}

static int
ijs_server_proc_nak (IjsServerCtx *ctx)
{
  /* servers should not get nak commands */
  return IJS_EPROTO;
}

static int
ijs_server_proc_ping (IjsServerCtx *ctx)
{
  int status;
  int version;

  status = ijs_recv_int (&ctx->recv_chan, &version);
  if (status < 0)
    return status;
  if (version > IJS_VERSION)
    version = IJS_VERSION;
  ctx->version = version;

#ifdef VERBOSE
  fprintf (stderr, "ping version=%d\n", version);
#endif
  status = ijs_send_begin (&ctx->send_chan, IJS_CMD_PONG);
  if (status < 0)
    return status;
  status = ijs_send_int (&ctx->send_chan, IJS_VERSION);
  if (status < 0)
    return status;
  return ijs_send_buf (&ctx->send_chan);
}

static int
ijs_server_proc_pong (IjsServerCtx *ctx)
{
  /* servers should not get pong commands */
  return IJS_EPROTO;
}

static int
ijs_server_proc_open (IjsServerCtx *ctx)
{
  /* A server might allocate tables here. */
  return ijs_server_ack (ctx);
}

static int
ijs_server_proc_close (IjsServerCtx *ctx)
{
  /* A server might deallocate memory here. */
  return ijs_server_ack (ctx);
}

static int
ijs_server_proc_begin_job (IjsServerCtx *ctx)
{
  int code;
  IjsJobId job_id;

  code = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (code < 0) return code;

  if (ctx->in_job)
    return ijs_server_nak (ctx, IJS_ETOOMANYJOBS);
  ctx->in_job = TRUE;
  ctx->job_id = job_id;

  return ijs_server_ack (ctx);
}

static int
ijs_server_proc_end_job (IjsServerCtx *ctx)
{
  int code;
  IjsJobId job_id;

  code = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (code < 0) return code;

  if (!ctx->in_job || job_id != ctx->job_id)
    return ijs_server_nak (ctx, IJS_EJOBID);
  ctx->in_job = FALSE;

  return ijs_server_ack (ctx);
}

static int
ijs_server_proc_cancel_job (IjsServerCtx *ctx)
{
  int code;
  IjsJobId job_id;

  code = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (code < 0) return code;

  if (!ctx->in_job || job_id != ctx->job_id)
    return ijs_server_nak (ctx, IJS_EJOBID);
  /* todo: call cancel callback here */
  ctx->in_job = FALSE;

  return ijs_server_ack (ctx);
}

static int
ijs_server_proc_query_status (IjsServerCtx *ctx)
{
  int code;
  IjsJobId job_id;

  code = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (code < 0)
    return code;

  if (!ctx->in_job || ctx->job_id != job_id)
    return ijs_server_nak (ctx, IJS_EJOBID);

  code = ctx->status_cb (ctx->list_cb_data, ctx, job_id);
  if (code < 0)
    return ijs_server_nak (ctx, code);
  else
    {
      int status;

      status = ijs_send_begin (&ctx->send_chan, IJS_CMD_ACK);
      if (status < 0)
	return status;
      status = ijs_send_int (&ctx->send_chan, code);
      if (status < 0)
	return status;
      return ijs_send_buf (&ctx->send_chan);
    }
}

static int
ijs_server_proc_list_params (IjsServerCtx *ctx)
{
  int code;
  char buf[4096];
  IjsJobId job_id;

  code = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (code < 0)
    return code;

  if (!ctx->in_job || ctx->job_id != job_id)
    return ijs_server_nak (ctx, IJS_EJOBID);

  code = ctx->list_cb (ctx->list_cb_data, ctx, job_id, buf, sizeof(buf));
  if (code < 0)
    return ijs_server_nak (ctx, code);
  else
    {
      int status;

      status = ijs_send_begin (&ctx->send_chan, IJS_CMD_ACK);
      if (status < 0)
	return status;
      status = ijs_send_block (&ctx->send_chan, buf, code);
      if (status < 0)
	return status;
      return ijs_send_buf (&ctx->send_chan);
    }
}

static int
ijs_server_proc_enum_param (IjsServerCtx *ctx)
{
  const char *key;
  int key_size;
  int code;
  char buf[4096];
  IjsJobId job_id;

  code = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (code < 0)
    return code;

  if (!ctx->in_job || ctx->job_id != job_id)
    return ijs_server_nak (ctx, IJS_EJOBID);

  key = ctx->recv_chan.buf + ctx->recv_chan.buf_idx;
  key_size = ctx->recv_chan.buf_size - ctx->recv_chan.buf_idx;
  if (key_size == 0 || key[key_size - 1])
    return IJS_ESYNTAX;
#ifdef VERBOSE
  fprintf (stderr, "ijs_server_proc_enum_param, key_size = %d\n", key_size);
#endif

  code = ctx->enum_cb (ctx->enum_cb_data, ctx, job_id, key, buf, sizeof(buf));
  if (code < 0)
    return ijs_server_nak (ctx, code);
  else
    {
      int status;

      status = ijs_send_begin (&ctx->send_chan, IJS_CMD_ACK);
      if (status < 0)
	return status;
      status = ijs_send_block (&ctx->send_chan, buf, code);
      if (status < 0)
	return status;
      return ijs_send_buf (&ctx->send_chan);
    }
}

static int
ijs_strnlen (const char *s, int size)
{
  int i;

  for (i = 0; i < size; i++)
    if (s[i] == 0)
      return i;
  return size;
}

static int
ijs_server_parse_int (const char *value, int size, int *result)
{
  int num = 0;
  int i;
  int sign = 1;

  i = 0;
  if (i == size)
    return IJS_ESYNTAX;
  if (value[i] == '-')
    {
      sign = -1;
      i++;
    }

  if (i == size)
    return IJS_ESYNTAX;

  for (; i < size; i++)
    {
      char c = value[i];
      if (c < '0' || c > '9')
	return IJS_ESYNTAX;
      num = (num * 10) + (c - '0');
    }
  *result = num;
  return 0;
}

static int
ijs_server_parse_float (const char *value, int size, double *result)
{
  char buf[256];
  char *tail;

  if (size + 1 > sizeof(buf))
    return IJS_EBUF;
  memcpy (buf, value, size);
  buf[size] = 0;
  *result = strtod (buf, &tail);
  if (tail == buf)
    return IJS_ESYNTAX;
  return 0;
}

static int
ijs_server_set_param (IjsServerCtx *ctx, IjsJobId job_id, const char *key,
		      const char *value, int value_size)
{
  int code;

#ifdef VERBOSE
  fprintf (stderr, "set_param %s = ", key);
  fwrite (value, 1, value_size, stderr);
  fputs ("\n", stderr);
#endif
  if (!strcmp (key, "NumChan"))
    {
      code = ijs_server_parse_int (value, value_size, &ctx->ph->n_chan);
      if (code == 0)
	ctx->fields_set |= IJS_N_CHAN_SET;
      return code;
    }
  else if (!strcmp (key, "BitsPerSample"))
    {
      code = ijs_server_parse_int (value, value_size, &ctx->ph->bps);
      if (code == 0)
	ctx->fields_set |= IJS_BPS_SET;
      return code;
    }
  else if (!strcmp (key, "ColorSpace"))
    {
      int size = value_size;

      if (size > (int)sizeof(ctx->ph->cs) - 1)
	size = sizeof(ctx->ph->cs) - 1;
      memcpy (ctx->ph->cs, value, size);
      ctx->ph->cs[size] = 0;
	ctx->fields_set |= IJS_CS_SET;
      return 0;
    }
  else if (!strcmp (key, "Width"))
    {
      code = ijs_server_parse_int (value, value_size, &ctx->ph->width);
      if (code == 0)
	ctx->fields_set |= IJS_WIDTH_SET;
      return code;
    }
  else if (!strcmp (key, "Height"))
    {
      code = ijs_server_parse_int (value, value_size, &ctx->ph->height);
      if (code == 0)
	ctx->fields_set |= IJS_HEIGHT_SET;
      return code;
    }
  else if (!strcmp (key, "Dpi"))
    {
      int x_ix;

      for (x_ix = 0; x_ix < value_size; x_ix++)
	if (value[x_ix] == 'x')
	  break;
      if (x_ix == value_size)
	return IJS_ESYNTAX;
      code = ijs_server_parse_float (value, x_ix, &ctx->ph->xres);
      if (code < 0)
	return code;
      code = ijs_server_parse_float (value + x_ix + 1, value_size - (x_ix + 1),
				     &ctx->ph->yres);
      if (code < 0)
	return code;
      ctx->fields_set |= IJS_DPI_SET;
      return 0;
    }
  else
    {
      return ctx->set_cb (ctx->set_cb_data, ctx, job_id, key, value, value_size);
    }
}

static int
ijs_server_proc_set_param (IjsServerCtx *ctx)
{
  const char *key, *value;
  int key_size, value_size;
  IjsJobId job_id;
  int param_size;
  int code;

  code = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (code < 0)
    return code;

  if (!ctx->in_job || ctx->job_id != job_id)
    return ijs_server_nak (ctx, IJS_EJOBID);

  code = ijs_recv_int (&ctx->recv_chan, &param_size);
  if (code < 0)
    return code;
  if (param_size != ctx->recv_chan.buf_size - ctx->recv_chan.buf_idx)
      return IJS_EPROTO;
  key = ctx->recv_chan.buf + ctx->recv_chan.buf_idx;
  key_size = ijs_strnlen (key, ctx->recv_chan.buf_size);
  if (key_size == param_size)
    return IJS_EPROTO;
  value = key + key_size + 1;
  value_size = param_size - (key_size + 1);
  code = ijs_server_set_param (ctx, job_id, key, value, value_size);
  if (code)
    return ijs_server_nak (ctx, code);
  else
    return ijs_server_ack (ctx);
}

static int
ijs_server_get_param (IjsServerCtx *ctx, IjsJobId job_id, const char *key,
		      char *value, int value_size)
{
#ifdef VERBOSE
  fprintf (stderr, "ijs_server_get_param %s\n", key);
#endif
  return ctx->get_cb (ctx->get_cb_data, ctx, job_id, key,
		      value, value_size);
}

static int
ijs_server_proc_get_param (IjsServerCtx *ctx)
{
  const char *key;
  int key_size;
  int code;
  char buf[4096];
  IjsJobId job_id;

  code = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (code < 0)
    return code;

  if (!ctx->in_job || ctx->job_id != job_id)
    return ijs_server_nak (ctx, IJS_EJOBID);

  key = ctx->recv_chan.buf + ctx->recv_chan.buf_idx;
  key_size = ctx->recv_chan.buf_size - ctx->recv_chan.buf_idx;
  if (key_size == 0 || key[key_size - 1])
    return IJS_ESYNTAX;
#ifdef VERBOSE
  fprintf (stderr, "ijs_server_proc_get_param, key_size = %d\n", key_size);
#endif

  code = ijs_server_get_param (ctx, job_id, key, buf, sizeof(buf));
  if (code < 0)
    return ijs_server_nak (ctx, code);
  else
    {
      int status;

      status = ijs_send_begin (&ctx->send_chan, IJS_CMD_ACK);
      if (status < 0)
	return status;
      status = ijs_send_block (&ctx->send_chan, buf, code);
      if (status < 0)
	return status;
      return ijs_send_buf (&ctx->send_chan);
    }
}

static int
ijs_server_proc_begin_page (IjsServerCtx *ctx)
{
  IjsPageHeader *ph = ctx->ph;
  int status = 0;

  if (ph == NULL)
    status = IJS_EPROTO;
  if ((ctx->fields_set & IJS_FIELDS_REQUIRED) != IJS_FIELDS_REQUIRED)
    status = IJS_EPROTO;
#ifdef VERBOSE
  fprintf (stderr, "begin page %d %d %d %d %d\n",
	   ph->n_chan, ph->bps, ph->cs, ph->width, ph->height);
#endif
  if (!status)
    {
      ctx->in_page = TRUE;
      return ijs_server_ack (ctx);
    }
  else
    return ijs_server_nak (ctx, status);
}

static int
ijs_server_read_data (IjsServerCtx *ctx, char *buf, int size)
{
  int status;

  status = ijs_recv_read (&ctx->recv_chan, buf, size);
  return (status == size) ? 0 : IJS_EIO;
}

static int
ijs_server_proc_send_data_block (IjsServerCtx *ctx)
{
  int size;
  int status = 0;
  IjsJobId job_id;

  status = ijs_recv_int (&ctx->recv_chan, &job_id);
  if (status < 0) return status;

  if (!ctx->in_job || job_id != ctx->job_id)
    status = IJS_EJOBID;
  else if (ctx->buf == NULL)
    status = IJS_EPROTO;

  if (!status) status = ijs_recv_int (&ctx->recv_chan, &size);

#ifdef VERBOSE
  fprintf (stderr, "status=%d, send data block id=%d, size=%d\n",
	   status, job_id, size);
#endif
  if (status)
    return ijs_server_nak (ctx, status);

  if (size <= ctx->buf_size - ctx->buf_ix)
    {
      status = ijs_server_read_data (ctx, ctx->buf + ctx->buf_ix, size);
      ctx->buf_ix += size;
    }
  else
    {
      ctx->overflow_buf_size = size - (ctx->buf_size - ctx->buf_ix);
      ctx->overflow_buf = (char *)malloc (ctx->overflow_buf_size);
      ctx->overflow_buf_ix = 0;
      status = ijs_server_read_data (ctx, ctx->buf + ctx->buf_ix,
				     ctx->buf_size - ctx->buf_ix);
      ctx->buf_ix = ctx->buf_size;
      if (!status)
	{
	  status = ijs_server_read_data (ctx, ctx->overflow_buf,
					 ctx->overflow_buf_size);
	}
    }
  return ijs_server_ack (ctx);
}

static int
ijs_server_proc_end_page (IjsServerCtx *ctx)
{
#ifdef VERBOSE
  fprintf (stderr, "end page\n");
#endif
  return ijs_server_ack (ctx);
}

static int
ijs_server_proc_exit (IjsServerCtx *ctx)
{
  return 1;
}

ijs_server_proc ijs_server_procs[] = {
  ijs_server_proc_ack,
  ijs_server_proc_nak,
  ijs_server_proc_ping,
  ijs_server_proc_pong,
  ijs_server_proc_open,
  ijs_server_proc_close,
  ijs_server_proc_begin_job,
  ijs_server_proc_end_job,
  ijs_server_proc_cancel_job,
  ijs_server_proc_query_status,
  ijs_server_proc_list_params,
  ijs_server_proc_enum_param,
  ijs_server_proc_set_param,
  ijs_server_proc_get_param,
  ijs_server_proc_begin_page,
  ijs_server_proc_send_data_block,
  ijs_server_proc_end_page,
  ijs_server_proc_exit
};

int
ijs_server_iter (IjsServerCtx *ctx)
{
  int cmd_num;
  int status;

  status = ijs_recv_buf (&ctx->recv_chan);

  if (status < 0)
    return status;

  cmd_num = ijs_get_int (ctx->recv_chan.buf);
#ifdef VERBOSE
  fprintf (stderr, "command %d, %d bytes\n", cmd_num, ctx->recv_chan.buf_size);
#endif
  if (cmd_num < 0 ||
      cmd_num >= (int)sizeof(ijs_server_procs) / sizeof(ijs_server_procs[0]))
    return -1;
  return ijs_server_procs[cmd_num] (ctx);
}

/**
 * ijs_server_get_page_header: Get the page header.
 * @ctx: The server context.
 * @ph: Where to store the page header.
 *
 * Return value: 0 on success, 1 on normal exit, negative on error.
 **/
int
ijs_server_get_page_header (IjsServerCtx *ctx, IjsPageHeader *ph)
{
  int status;

  ctx->ph = ph;
  ctx->in_page = FALSE;

  do 
    {
      status = ijs_server_iter (ctx);
    }
  while (status == 0 && !ctx->in_page);

  ctx->ph = NULL;
  return status;
}

/**
 * ijs_server_get_data: Get data from client.
 * @ctx: The server context.
 * @buf: Buffer for data being read.
 * @size: Size of buf.
 * 
 * Gets data from client. Data is stored in @buf or the
 * overflow_buf.
 *
 * Return value: Number of bytes read, -1 on end of page, or < 0 on
 * error.
 **/
int
ijs_server_get_data (IjsServerCtx *ctx, char *buf, int size)
{
  int buf_ix = 0;
  int status = 0;

#ifdef VERBOSE
  fprintf (stderr, "ijs_server_get_data %d\n", size);
#endif

  if (ctx->overflow_buf != NULL)
    {
      int n_bytes = ctx->overflow_buf_size - ctx->overflow_buf_ix;
      if (n_bytes > size)
	n_bytes = size;
      memcpy (buf, ctx->overflow_buf + ctx->overflow_buf_ix, n_bytes);
      ctx->overflow_buf_ix += n_bytes;
      buf_ix = n_bytes;
      if (ctx->overflow_buf_ix == ctx->overflow_buf_size)
	{
	  free (ctx->overflow_buf);
	  ctx->overflow_buf = NULL;
	  ctx->overflow_buf_size = 0;
	  ctx->overflow_buf_ix = 0;
	}
    }
  ctx->buf = buf;
  ctx->buf_size = size;
  ctx->buf_ix = buf_ix;
  while (!status && ctx->buf_ix < size)
    {
      status = ijs_server_iter (ctx);
    }
  ctx->buf = NULL;
  return status;
}
