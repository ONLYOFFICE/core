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

/* This file contains common data types for IJS */

/* IJS_VERSION is decimal version number times 100 */
#define IJS_VERSION 35

typedef int ijs_bool;

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef enum {
  IJS_CMD_ACK,
  IJS_CMD_NAK,
  IJS_CMD_PING,
  IJS_CMD_PONG,
  IJS_CMD_OPEN,
  IJS_CMD_CLOSE,
  IJS_CMD_BEGIN_JOB,
  IJS_CMD_END_JOB,
  IJS_CMD_CANCEL_JOB,
  IJS_CMD_QUERY_STATUS,
  IJS_CMD_LIST_PARAMS,
  IJS_CMD_ENUM_PARAM,
  IJS_CMD_SET_PARAM,
  IJS_CMD_GET_PARAM,
  IJS_CMD_BEGIN_PAGE,
  IJS_CMD_SEND_DATA_BLOCK,
  IJS_CMD_END_PAGE,
  IJS_CMD_EXIT
} IjsCommand;

typedef int IjsJobId;

#define IJS_EIO            -2  /* I/O error */
#define IJS_EPROTO         -3  /* protocol error */
#define IJS_ERANGE         -4  /* out of range */
#define IJS_EINTERNAL      -5  /* internal error */
#define IJS_ENYI           -6  /* not yet implemented */
#define IJS_ESYNTAX        -7  /* syntax error */
#define IJS_ECOLORSPACE    -8  /* unknown color space */
#define IJS_EUNKPARAM      -9  /* unknown parameter */
#define IJS_EJOBID        -10  /* job id doesn't match */
#define IJS_ETOOMANYJOBS  -11  /* reached limit of server's #jobs */
#define IJS_EBUF          -12  /* buffer isn't big enough */

#define IJS_HELO_STR "IJS\n\252v1\n"
#define IJS_RESP_STR "IJS\n\253v1\n"

typedef struct {
  int fd;
  char buf[4096];
  int buf_size;
} IjsSendChan;

typedef struct {
  int fd;
  char buf[4096];
  int buf_size;
  int buf_idx;
} IjsRecvChan;

void
ijs_send_init (IjsSendChan *ch, int fd);

int
ijs_send_int (IjsSendChan *ch, int val);

int
ijs_send_begin (IjsSendChan *ch, IjsCommand cmd);

int
ijs_send_block (IjsSendChan *ch, const char *buf, int len);

int
ijs_send_buf (IjsSendChan *ch);

int
ijs_get_int (const char *p);

void
ijs_recv_init (IjsRecvChan *ch, int fd);

int
ijs_recv_read (IjsRecvChan *ch, char *buf, int size);

int
ijs_recv_buf (IjsRecvChan *ch);

int
ijs_recv_ack (IjsRecvChan *ch);

int
ijs_recv_int (IjsRecvChan *ch, int *val);

int
ijs_recv_block (IjsRecvChan *ch, char *buf, int buf_size);
