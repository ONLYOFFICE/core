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

/* $Id: iht.h 9043 2008-08-28 22:48:19Z giles $ */
/* Procedures exported by zht.c for zht1.c and zht2.c */

#ifndef iht_INCLUDED
#  define iht_INCLUDED

int zscreen_params(os_ptr op, gs_screen_halftone * phs);

int zscreen_enum_init(i_ctx_t *i_ctx_p, const gx_ht_order * porder,
		      gs_screen_halftone * phs, ref * pproc, int npop,
		      op_proc_t finish_proc, int space_index);

#endif /* iht_INCLUDED */
