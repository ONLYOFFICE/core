/* Copyright (C) 2009 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id:$ */
/* Unix implementation of gp_defaultpapersize */

#ifdef USE_LIBPAPER
#include <paper.h>
#endif

#include "string_.h"
#include "gx.h"
#include "gp.h"

/* ------ Default paper size ------ */

/* Get the default paper size.  See gp_paper.h for details. */
int 
gp_defaultpapersize(char *ptr, int *plen)
{
#ifdef USE_LIBPAPER
    const char *paper = systempapername();

    if (paper) {
	int len = strlen(paper);

	if (len < *plen) {
	    /* string fits */
	    strcpy(ptr, paper);
	    *plen = len + 1;
	    return 0;
	}
	/* string doesn't fit */
	*plen = len + 1;
	return -1;
    }
#endif

    /* No default paper size */

    if (*plen > 0)
	*ptr = 0;
    *plen = 1;
    return 1;
}


