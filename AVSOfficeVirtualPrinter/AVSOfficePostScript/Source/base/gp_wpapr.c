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
/* MS Windows implementation of gp_defaultpapersize */

#include <windows.h>
#include <string.h>
#include "gx.h"
#include "gp.h"

/* For compilation on Windows NT  */
#ifndef LOCALE_IPAPERSIZE
#  define LOCALE_IPAPERSIZE 0x100A
#endif

/* ------ Default paper size ------ */

/* Get the default paper size.  See gp_paper.h for details. */
int 
gp_defaultpapersize(char *ptr, int *plen)
{
    char buf[6];
    char *paper = NULL; 
  
    /* Determine the default paper size using the Windows locale.
     * LOCALE_IPAPERSIZE is only supported on Windows 2000 or later.
     */
    if  (GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IPAPERSIZE, 
	    buf, sizeof(buf))) {
	int val = atoi(buf);
	if (val == 1)
	    paper = "letter";
	else if (val == 5)
	    paper = "legal";
	else if (val == 8)
	    paper = "a3";
	else if (val == 9)
	    paper = "a4";
    }

    /* Fall back to the default paper size method described in
     * http://msdn.microsoft.com/en-us/library/ms801585.aspx
     */
    if  ((paper == 0) && 
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_ICOUNTRY, 
	    buf, sizeof(buf))) {
	int country = atoi(buf);
 	if ((country == CTRY_UNITED_STATES) ||
	    (country == CTRY_CANADA) ||
	    ((country >= 50) && (country < 60) && (country != CTRY_BRAZIL)) ||
	    ((country >= 500) && (country < 600)) ) {
	    /* Imperial measurement system */
	    paper = "letter";
	}
	else {
	    /* Metric measurement system */
	    paper = "a4";
	}
    }

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

    /* No default paper size */

    if (*plen > 0)
	*ptr = 0;
    *plen = 1;
    return 1;
}


