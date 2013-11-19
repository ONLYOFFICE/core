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

/* $Id: zfunc.h 9062 2008-09-03 11:42:35Z leonardo $ */
/* Level 2 sethalftone support */

#ifndef zfunc_INCLUDED
#  define zfunc_INCLUDED

/* imported from zfsample.c */
int make_sampled_function(i_ctx_t * i_ctx_p, ref *arr, ref *pproc, gs_function_t **func);
/* imported from zfunc4.c */
int make_type4_function(i_ctx_t * i_ctx_p, ref *arr, ref *pproc, gs_function_t **func);

#endif /* zfunc_INCLUDED */
