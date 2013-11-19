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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _IjsServerCtx IjsServerCtx;

typedef struct {
  int n_chan;        /* number of channels */
  int bps;           /* bits per sample, one of 1, 2, 4, 8, 16 */
  char cs[256];      /* color space, as string */
  int width;         /* in pixels */
  int height;
  double xres;          /* in dpi */
  double yres;
} IjsPageHeader;

/* callbacks, lots of callbacks */

typedef int IjsBeginJobCb (void *begin_cb_data,
			   IjsServerCtx *ctx,
			   IjsJobId job_id);

typedef int IjsEndJobCb (void *end_cb_data,
			 IjsServerCtx *ctx,
			 IjsJobId job_id);

typedef int IjsQueryStatusCb (void *status_cb_data,
			      IjsServerCtx *ctx,
			      IjsJobId job_id);

typedef int IjsListParamsCb (void *list_cb_data,
			     IjsServerCtx *ctx,
			     IjsJobId job_id,
			     char *val_buf,
			     int val_size);

typedef int IjsEnumParamCb (void *enum_cb_data,
			    IjsServerCtx *ctx,
			    IjsJobId job_id,
			    const char *key,
			    char *val_buf,
			    int val_size);

typedef int IjsSetParamCb (void *set_cb_data,
			   IjsServerCtx *ctx,
			   IjsJobId job_id,
			   const char *key,
			   const char *value,
			   int value_size);

typedef int IjsGetParamCb (void *get_cb_data,
			   IjsServerCtx *ctx,
			   IjsJobId job_id,
			   const char *key,
			   char *val_buf,
			   int val_size);

IjsServerCtx *
ijs_server_init (void);

int
ijs_server_install_begin_cb (IjsServerCtx *ctx,
			     IjsBeginJobCb *begin_cb, void *begin_cb_data);

int
ijs_server_install_end_cb (IjsServerCtx *ctx,
			   IjsEndJobCb *end_cb, void *end_cb_data);

int
ijs_server_install_status_cb (IjsServerCtx *ctx,
			      IjsQueryStatusCb *status_cb,
			      void *status_cb_data);

int
ijs_server_install_list_cb (IjsServerCtx *ctx,
			    IjsListParamsCb *list_cb, void *list_cb_data);

int
ijs_server_install_enum_cb (IjsServerCtx *ctx,
			    IjsEnumParamCb *enum_cb, void *enum_cb_data);

int
ijs_server_install_set_cb (IjsServerCtx *ctx,
			   IjsSetParamCb *set_cb, void *set_cb_data);

int
ijs_server_install_get_cb (IjsServerCtx *ctx,
			   IjsGetParamCb *get_cb, void *get_cb_data);

void
ijs_server_done (IjsServerCtx *ctx);

int
ijs_server_iter (IjsServerCtx *ctx);

int
ijs_server_get_page_header (IjsServerCtx *ctx, IjsPageHeader *ph);

int
ijs_server_get_data (IjsServerCtx *ctx, char *buf, int size);

#ifdef __cplusplus
}
#endif
