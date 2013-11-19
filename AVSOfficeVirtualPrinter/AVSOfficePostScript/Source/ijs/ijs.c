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

#include <string.h>
#include "unistd_.h"
#include "ijs.h"

static void
ijs_put_int (char *p, int val)
{
  p[0] = (val >> 24) & 0xff;
  p[1] = (val >> 16) & 0xff;
  p[2] = (val >> 8) & 0xff;
  p[3] = val & 0xff;
}

void
ijs_send_init (IjsSendChan *ch, int fd)
{
  ch->fd = fd;
  ch->buf_size = 0;
}

int
ijs_send_int (IjsSendChan *ch, int val)
{
  if ((ch->buf_size + 4) > (int)sizeof(ch->buf))
    return IJS_ERANGE;
  ijs_put_int (ch->buf + ch->buf_size, val);
  ch->buf_size += 4;
  return 0;
}

int
ijs_send_begin (IjsSendChan *ch, IjsCommand cmd)
{
  if (ch->buf_size != 0)
    return IJS_EINTERNAL;
  ijs_send_int (ch, cmd);
  ch->buf_size += 4; /* leave room for size field */
  return 0;
}

int
ijs_send_block (IjsSendChan *ch, const char *buf, int len)
{
  if ((ch->buf_size + len) > (int)sizeof(ch->buf))
    return IJS_ERANGE;
  memcpy (ch->buf + ch->buf_size, buf, len);
  ch->buf_size += len;
  return 0;
}

int
ijs_send_buf (IjsSendChan *ch)
{
  int status;

  ijs_put_int (ch->buf + 4, ch->buf_size);
  status = write (ch->fd, ch->buf, ch->buf_size);
  status = (status == ch->buf_size) ? 0 : IJS_EIO;
  ch->buf_size = 0;
  return status;
}

void
ijs_recv_init (IjsRecvChan *ch, int fd)
{
  ch->fd = fd;
  ch->buf_size = 0;
}

int
ijs_get_int (const char *p)
{
  const unsigned char *up = (const unsigned char *)p;
  return (up[0] << 24) | (up[1] << 16) | (up[2] << 8) | up[3];
}

/* This is a drop-in replacement for read(), but handles partial reads. */
int
ijs_recv_read (IjsRecvChan *ch, char *buf, int size)
{
  int ix = 0;
  int nbytes;

  do
    {
      nbytes = read (ch->fd, buf + ix, size - ix);
      if (nbytes < 0)
	return nbytes;
      else if (nbytes == 0)
	return ix;
      else
	ix += nbytes;
    }
  while (ix < size);
  return ix;
}

int
ijs_recv_buf (IjsRecvChan *ch)
{
  int nbytes;
  int data_size;

  nbytes = ijs_recv_read (ch, ch->buf, 8);
  if (nbytes != 8)
    return IJS_EIO;
  ch->buf_size = ijs_get_int (ch->buf + 4);
  if (ch->buf_size < 8 || ch->buf_size > (int)sizeof(ch->buf))
    return IJS_ERANGE;
  data_size = ch->buf_size - 8;
  if (data_size > 0)
    {
      nbytes = ijs_recv_read (ch, ch->buf + 8, data_size);
      if (nbytes != data_size)
	return IJS_EIO;
    }
  ch->buf_idx = 8;
  return 0;
}

/**
 * ijs_recv_ack: Receive an acknowledgement.
 * @ch: Channel.
 *
 * Receives an acknowledgement code (generally from server to client).
 * Note that the distinction between local error and error generated
 * by the remote peer is lost in this routine. If it is important to
 * preserve this distinction, use ijs_recv_buf directly instead.
 *
 * Return value: 0 on success, negative on error.
 **/
int
ijs_recv_ack (IjsRecvChan *ch)
{
  int status;

  status = ijs_recv_buf (ch);
  if (status == 0)
    {
      int cmd = ijs_get_int (ch->buf);

      if (cmd == IJS_CMD_NAK)
	{
	  if (ch->buf_size != 12)
	    status = IJS_EPROTO;
	  else
	    status = ijs_get_int (ch->buf + 8);
	}
    }
  return status;
}

int
ijs_recv_int (IjsRecvChan *ch, int *val)
{
  if (ch->buf_idx + 4 > ch->buf_size)
    return IJS_EPROTO;
  *val = ijs_get_int (ch->buf + ch->buf_idx);
  ch->buf_idx += 4;
  return 0;
}

/**
 * Return value: data block size if nonnegative, or error code if
 * negative.
 **/
int
ijs_recv_block (IjsRecvChan *ch, char *buf, int buf_size)
{
  int size = ch->buf_size - ch->buf_idx;

  if (size > buf_size)
    return IJS_ERANGE;
  memcpy (buf, ch->buf + ch->buf_idx, size);
  ch->buf_idx = ch->buf_size;
  return size;
}
