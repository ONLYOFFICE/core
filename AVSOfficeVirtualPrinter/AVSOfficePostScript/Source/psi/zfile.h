/* Copyright (C) 2008 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: zfile.h 9043 2008-08-28 22:48:19Z giles $ */
/* Export zopen_file() for zfrsd.c */

#ifndef zfile_INCLUDED
#  define zfile_INCLUDED

int zopen_file(i_ctx_t *i_ctx_p, const gs_parsed_file_name_t *pfn,
           const char *file_access, stream **ps, gs_memory_t *mem);

#endif
