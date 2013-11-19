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

/* $Id: gsfname.c 8022 2007-06-05 22:23:38Z giles $ */
/* File name utilities */
#include "memory_.h"
#include "gserror.h"
#include "gserrors.h"
#include "gsmemory.h"
#include "gstypes.h"
#include "gsfname.h"
#include "gxiodev.h"

/* Parse a file name into device and individual name. */
/* The device may be NULL, or the name may be NULL, but not both. */
/* According to the Adobe documentation, %device and %device% */
/* are equivalent; both return name==NULL. */
int
gs_parse_file_name(gs_parsed_file_name_t * pfn, const char *pname, uint len)
{
    uint dlen;
    const char *pdelim;
    gx_io_device *iodev;

    if (len == 0)
	return_error(gs_error_undefinedfilename); /* null name not allowed */
    if (pname[0] != '%') {	/* no device */
	pfn->memory = 0;
	pfn->iodev = NULL;
	pfn->fname = pname;
	pfn->len = len;
	return 0;
    }
    pdelim = memchr(pname + 1, '%', len - 1);
    if (pdelim == NULL)		/* %device */
	dlen = len;
    else if (pdelim[1] == 0) {	/* %device% */
	pdelim = NULL;
	dlen = len;
    } else {
	dlen = pdelim - pname;
	pdelim++, len--;
    }
    iodev = gs_findiodevice((const byte *)pname, dlen);
    if (iodev == 0)
	return_error(gs_error_undefinedfilename);
    pfn->memory = 0;
    pfn->iodev = iodev;
    pfn->fname = pdelim;
    pfn->len = len - dlen;
    return 0;
}

/* Parse a real (non-device) file name and convert to a C string. */
int
gs_parse_real_file_name(gs_parsed_file_name_t * pfn, const char *pname,
			uint len, gs_memory_t *mem, client_name_t cname)
{
    int code = gs_parse_file_name(pfn, pname, len);

    if (code < 0)
	return code;
    if (pfn->len == 0)  /* device only */
	return_error(gs_error_undefinedfilename); /* for CET 23-23.ps */
    return gs_terminate_file_name(pfn, mem, cname);
}

/* Convert a file name to a C string by adding a null terminator. */
int
gs_terminate_file_name(gs_parsed_file_name_t * pfn, gs_memory_t *mem,
		       client_name_t cname)
{
    uint len = pfn->len;
    char *fname;

    if (pfn->iodev == NULL)	/* no device */
	pfn->iodev = iodev_default;
    if (pfn->memory)
	return 0;		/* already copied */
    /* Copy the file name to a C string. */
    fname = (char *)gs_alloc_string(mem, len + 1, cname);
    if (fname == 0)
	return_error(gs_error_VMerror);
    memcpy(fname, pfn->fname, len);
    fname[len] = 0;
    pfn->memory = mem;
    pfn->fname = fname;
    pfn->len = len + 1;		/* null terminator */
    return 0;
}

/* Free a file name that was copied to a C string. */
void
gs_free_file_name(gs_parsed_file_name_t * pfn, client_name_t cname)
{
    if (pfn->fname != 0)
	gs_free_const_string(pfn->memory, (const byte *)pfn->fname, pfn->len,
			     cname);
}
