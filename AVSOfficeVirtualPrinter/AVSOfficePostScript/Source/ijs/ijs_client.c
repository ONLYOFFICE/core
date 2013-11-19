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

#include "unistd_.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ijs.h"
#include "ijs_client.h"

struct _IjsClientCtx {
  int fd_from;
  int child_pid;
  IjsSendChan send_chan;
  IjsRecvChan recv_chan;
  int version;
};

IjsClientCtx *
ijs_invoke_server (const char *server_cmd)
{
  IjsClientCtx *ctx;
  int fds_to[2], fds_from[2];
  int child_pid;
  char helo_buf[8] = IJS_HELO_STR;
  char resp_buf[8];
  const char exp_resp_buf[8] = IJS_RESP_STR;
  ijs_bool ok = TRUE;
  int nbytes;
  int version;

  if (ijs_exec_server(server_cmd, &fds_to[1], &fds_from[0], &child_pid) < 0)
    return NULL;

  ctx = (IjsClientCtx *)malloc (sizeof(IjsClientCtx));
  ctx->fd_from = fds_from[0];
  ctx->child_pid = child_pid;
  ijs_send_init (&ctx->send_chan, fds_to[1]);
  ijs_recv_init (&ctx->recv_chan, fds_from[0]);

  nbytes = write (ctx->send_chan.fd, helo_buf, sizeof(helo_buf));
  if (nbytes != sizeof(helo_buf))
    ok = FALSE;

  if (ok)
    {
      nbytes = read (ctx->recv_chan.fd, resp_buf, sizeof(resp_buf));
      if (nbytes != sizeof(resp_buf) ||
	  memcmp (resp_buf, exp_resp_buf, sizeof(resp_buf)))
	ok = FALSE;
    }

  /* exchange version information with server */
  if (ok)
    ok = ijs_client_begin_cmd (ctx, IJS_CMD_PING) >= 0;
  if (ok)
    ok = ijs_send_int (&ctx->send_chan, IJS_VERSION) >= 0;
  if (ok)
    ok = ijs_client_send_cmd_wait (ctx) >= 0;
  if (ok)
    ok = ijs_recv_int (&ctx->recv_chan, &version) >= 0;
  if (ok)
    {
      if (version > IJS_VERSION)
	version = IJS_VERSION;
      ctx->version = version;
    }

  if (!ok)
    {
      close (ctx->send_chan.fd);
      close (ctx->recv_chan.fd);
      free (ctx);
      ctx = NULL;
    }

  return ctx;
}

int
ijs_client_begin_cmd (IjsClientCtx *ctx, IjsCommand cmd)
{
  return ijs_send_begin (&ctx->send_chan, cmd);
}

int
ijs_client_send_int (IjsClientCtx *ctx, int val)
{
  return ijs_send_int (&ctx->send_chan, val);
}

int
ijs_client_send_cmd (IjsClientCtx *ctx)
{
  return ijs_send_buf (&ctx->send_chan);
}

/**
 * ijs_client_send_cmd_wait: Send command and wait for ack.
 * @ctx: IJS client context.
 *
 * Sends the command in the client context's buffer, and waits for ack.
 *
 * Return value: 0 on successful ack, otherwise negative.
 **/
int
ijs_client_send_cmd_wait (IjsClientCtx *ctx)
{
  int status;

  status = ijs_client_send_cmd (ctx);
  if (status >= 0)
    {
      status = ijs_recv_ack (&ctx->recv_chan);
    }
  return status;
}

/* This is the blocking version; it's not likely to be efficient */
int
ijs_client_send_data_wait (IjsClientCtx *ctx, IjsJobId job_id,
			   const char *buf, int size)
{
  int status;

  ijs_client_begin_cmd (ctx, IJS_CMD_SEND_DATA_BLOCK);
  ijs_send_int (&ctx->send_chan, job_id);
  ijs_send_int (&ctx->send_chan, size);
  status = ijs_client_send_cmd (ctx);
  if (status)
    return status;
  status = write (ctx->send_chan.fd, buf, size);
  if (status != size)
    return IJS_EIO;
  status = ijs_recv_ack (&ctx->recv_chan);
  return status;
}

int
ijs_client_open (IjsClientCtx *ctx)
{
  ijs_client_begin_cmd (ctx, IJS_CMD_OPEN);
  return ijs_client_send_cmd_wait (ctx);
}

int
ijs_client_close (IjsClientCtx *ctx)
{
  ijs_client_begin_cmd (ctx, IJS_CMD_CLOSE);
  return ijs_client_send_cmd_wait (ctx);
}

int
ijs_client_begin_job (IjsClientCtx *ctx, IjsJobId job_id)
{
  ijs_client_begin_cmd (ctx, IJS_CMD_BEGIN_JOB);
  ijs_send_int (&ctx->send_chan, job_id);
  return ijs_client_send_cmd_wait (ctx);
}

int
ijs_client_end_job (IjsClientCtx *ctx, IjsJobId job_id)
{
  ijs_client_begin_cmd (ctx, IJS_CMD_END_JOB);
  ijs_send_int (&ctx->send_chan, job_id);
  return ijs_client_send_cmd_wait (ctx);
}

/**
 * Return value: data block size if nonnegative, or error code if
 * negative.
 **/
int
ijs_client_list_params (IjsClientCtx *ctx, IjsJobId job_id,
		      char *value, int value_size)
{
  int status;

  ijs_client_begin_cmd (ctx, IJS_CMD_LIST_PARAMS);
  ijs_send_int (&ctx->send_chan, job_id);
  status = ijs_client_send_cmd (ctx);
  if (status)
    return status;
  status = ijs_recv_ack (&ctx->recv_chan);
  if (status)
    return status;
  status = ijs_recv_block (&ctx->recv_chan, value, value_size);
  return status;
}

/**
 * Return value: data block size if nonnegative, or error code if
 * negative.
 **/
int
ijs_client_enum_param (IjsClientCtx *ctx, IjsJobId job_id,
		      const char *key,
		      char *value, int value_size)
{
  int key_size = strlen (key);
  int status;

  ijs_client_begin_cmd (ctx, IJS_CMD_ENUM_PARAM);
  ijs_send_int (&ctx->send_chan, job_id);
  status = ijs_send_block (&ctx->send_chan, key, key_size + 1);
  if (status < 0)
    return IJS_EIO;
  status = ijs_client_send_cmd (ctx);
  if (status)
    return status;
  status = ijs_recv_ack (&ctx->recv_chan);
  if (status)
    return status;
  status = ijs_recv_block (&ctx->recv_chan, value, value_size);
  return status;
}

int
ijs_client_set_param (IjsClientCtx *ctx, IjsJobId job_id,
		      const char *key,
		      const char *value, int value_size)
{
  int key_size = strlen (key);
  int status;

  ijs_client_begin_cmd (ctx, IJS_CMD_SET_PARAM);
  ijs_send_int (&ctx->send_chan, job_id);
  ijs_send_int (&ctx->send_chan, key_size + 1 + value_size);
  status = ijs_send_block (&ctx->send_chan, key, key_size+1);
  if (status)
    return status;
  status = ijs_send_block (&ctx->send_chan, value, value_size);
  if (status)
    return status;
  status = ijs_client_send_cmd (ctx);
  if (status)
    return status;
  status = ijs_recv_ack (&ctx->recv_chan);
  return status;
}

/**
 * Return value: data block size if nonnegative, or error code if
 * negative.
 **/
int
ijs_client_get_param (IjsClientCtx *ctx, IjsJobId job_id,
		      const char *key,
		      char *value, int value_size)
{
  int key_size = strlen (key);
  int status;

  ijs_client_begin_cmd (ctx, IJS_CMD_GET_PARAM);
  ijs_send_int (&ctx->send_chan, job_id);
  status = ijs_send_block (&ctx->send_chan, key, key_size + 1);
  if (status < 0)
    return IJS_EIO;
  status = ijs_client_send_cmd (ctx);
  if (status)
    return status;
  status = ijs_recv_ack (&ctx->recv_chan);
  if (status)
    return status;
  status = ijs_recv_block (&ctx->recv_chan, value, value_size);
  return status;
}

int
ijs_client_begin_page (IjsClientCtx *ctx, IjsJobId job_id)
{
  ijs_client_begin_cmd (ctx, IJS_CMD_BEGIN_PAGE);
  ijs_send_int (&ctx->send_chan, job_id);
  return ijs_client_send_cmd_wait (ctx);
}

int
ijs_client_end_page (IjsClientCtx *ctx, IjsJobId job_id)
{
  ijs_client_begin_cmd (ctx, IJS_CMD_END_PAGE);
  ijs_send_int (&ctx->send_chan, job_id);
  return ijs_client_send_cmd_wait (ctx);
}

int
ijs_client_get_version (IjsClientCtx *ctx)
{
  return ctx->version;
}
