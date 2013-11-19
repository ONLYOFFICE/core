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

/* $Id: gsalpha.c 8022 2007-06-05 22:23:38Z giles $ */
/* Graphics state alpha value access */
#include "gx.h"
#include "gsalpha.h"
#include "gxdcolor.h"
#include "gzstate.h"

/* setalpha */
int
gs_setalpha(gs_state * pgs, floatp alpha)
{
    pgs->alpha =
	(gx_color_value) (alpha < 0 ? 0 : alpha > 1 ? gx_max_color_value :
			  alpha * gx_max_color_value);
    gx_unset_dev_color(pgs);
    return 0;
}

/* currentalpha */
float
gs_currentalpha(const gs_state * pgs)
{
    return (float)pgs->alpha / gx_max_color_value;
}
