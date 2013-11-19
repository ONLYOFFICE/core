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

/* $Id: oparc.h 9043 2008-08-28 22:48:19Z giles $ */
/* Arc operator declarations */

#ifndef oparc_INCLUDED
#  define oparc_INCLUDED

/*
 * These declarations are in a separate from, rather than in opextern.h,
 * because these operators are not included in PDF-only configurations.
 */

int zarc(i_ctx_t *);
int zarcn(i_ctx_t *);
int zarct(i_ctx_t *);

#endif /* oparc_INCLUDED */
