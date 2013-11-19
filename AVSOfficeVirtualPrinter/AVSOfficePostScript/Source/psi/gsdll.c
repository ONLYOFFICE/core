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
/* Portions Copyright (C) 1994-2000 Ghostgum Software Pty Ltd.  All rights reserved. */


/* $Id: gsdll.c 9043 2008-08-28 22:48:19Z giles $ */
/* Dynamic Link Library interface for OS/2 and MS-Windows Ghostscript */
/* front end to gs.c */

/* This has been reimplemented to call the new DLL interface in iapi.h */


#ifdef _Windows
#include <windows.h>
#endif
#ifdef __OS2__
#define INCL_DOS
#define INCL_WIN
#include <os2.h>
#endif

#include "stdpre.h"
#include "iapi.h"	/* Ghostscript interpreter public interface */
#include "string_.h"
#include "ierrors.h"
#include "gscdefs.h"
#include "gstypes.h"
#include "iref.h"
#include "iminst.h"
#include "imain.h"

#include "gsdll.h"	/* old DLL public interface */

/* MacGSView still requires that hwnd be exported
   through the old dll interface. We do that here,
   but expect to remove it when that client has been
   ported to the gsapi interface. */
#ifdef __MACOS__
extern HWND hwndtext;
#endif

/****** SINGLE-INSTANCE HACK ******/
/* GLOBAL WARNING */
GSDLL_CALLBACK pgsdll_callback = NULL;	/* callback for messages and stdio to caller */

static gs_main_instance *pgs_minst = NULL;


/****** SINGLE-INSTANCE HACK ******/



/* end gsdll.c */
