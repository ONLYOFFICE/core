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

/* $Id: zimage2.c 9043 2008-08-28 22:48:19Z giles $ */
/* image operator extensions for Level 2 PostScript */
#include "memory_.h"
#include "ghost.h"
#include "gsimage.h"
#include "gxiparam.h"
#include "icstate.h"
#include "iimage2.h"
#include "igstate.h"		/* for igs */


/*
 * Process an image that has no explicit source data.  This isn't used by
 * standard Level 2, but it's a very small procedure and is needed by
 * both zdps.c and zdpnext.c.
 */
int
process_non_source_image(i_ctx_t *i_ctx_p, const gs_image_common_t * pic,
			 client_name_t cname)
{
    gx_image_enum_common_t *pie;
    int code = gs_image_begin_typed(pic, igs, false /****** WRONG ******/ ,
				    &pie);

    /* We didn't pass any data, so there's nothing to clean up. */
    return code;
}
