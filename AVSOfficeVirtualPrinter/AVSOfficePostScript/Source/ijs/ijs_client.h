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

typedef struct _IjsClientCtx IjsClientCtx;

IjsClientCtx *
ijs_invoke_server (const char *server_cmd);

int 
ijs_exec_server (const char *server_cmd, int *pfd_to, int *pfd_from, 
    int *pchild_pid);

int
ijs_client_begin_cmd (IjsClientCtx *ctx, IjsCommand cmd);

int
ijs_client_send_int (IjsClientCtx *ctx, int val);

int
ijs_client_send_cmd (IjsClientCtx *ctx);

int
ijs_client_send_cmd_wait (IjsClientCtx *ctx);

int
ijs_client_send_data_wait (IjsClientCtx *ctx, IjsJobId job_id,
			   const char *buf, int size);

int
ijs_client_open (IjsClientCtx *ctx);

int
ijs_client_close (IjsClientCtx *ctx);

int
ijs_client_begin_job (IjsClientCtx *ctx, IjsJobId job_id);

int
ijs_client_end_job (IjsClientCtx *ctx, IjsJobId job_id);

int
ijs_client_list_params (IjsClientCtx *ctx, IjsJobId job_id,
		       char *value, int value_size);

int
ijs_client_enum_param (IjsClientCtx *ctx, IjsJobId job_id,
		       const char *key, char *value,
		       int value_size);

int
ijs_client_set_param (IjsClientCtx *ctx, IjsJobId job_id,
		      const char *key, const char *value,
		      int value_size);

int
ijs_client_get_param (IjsClientCtx *ctx, IjsJobId job_id,
		      const char *key, char *value,
		      int value_size);

int
ijs_client_begin_page (IjsClientCtx *ctx, IjsJobId job_id);

int
ijs_client_end_page (IjsClientCtx *ctx, IjsJobId job_id);

int
ijs_client_get_version (IjsClientCtx *ctx);

#ifdef __cplusplus
}
#endif
