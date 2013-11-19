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

/* $Id: gscpm.h 8022 2007-06-05 22:23:38Z giles $ */
/* Charpath mode and cache device status definitions */

#ifndef gscpm_INCLUDED
#  define gscpm_INCLUDED

typedef enum {
    cpm_show,			/* *show (default, must be 0) */
    cpm_charwidth,		/* stringwidth rmoveto (not standard PS) */
    cpm_false_charpath,		/* false charpath */
    cpm_true_charpath,		/* true charpath */
    cpm_false_charboxpath,	/* false charboxpath (not standard PS) */
    cpm_true_charboxpath	/* true charboxpath (ditto) */
} gs_char_path_mode;

typedef enum {
    CACHE_DEVICE_NONE = 0,	/* default, must be 0 */
    CACHE_DEVICE_NOT_CACHING,	/* setcachedevice done but not caching */
    CACHE_DEVICE_NONE_AND_CLIP, /* setcachedevice done, not caching and applied gsave and clip */
    CACHE_DEVICE_CACHING	/* setcachedevice done and caching */
} gs_in_cache_device_t;

#endif /* gscpm_INCLUDED */
