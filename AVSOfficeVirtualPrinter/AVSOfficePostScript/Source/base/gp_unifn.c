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

/* $Id: gp_unifn.c 8022 2007-06-05 22:23:38Z giles $ */
/* Unix-like file name syntax platform routines for Ghostscript */
#include "gx.h"
#include "gp.h"
#include "gpmisc.h"
#include "gsutil.h"

/* Define the character used for separating file names in a list. */
const char gp_file_name_list_separator = ':';

/* Define the string to be concatenated with the file mode */
/* for opening files without end-of-line conversion. */
const char gp_fmode_binary_suffix[] = "";

/* Define the file modes for binary reading or writing. */
const char gp_fmode_rb[] = "r";
const char gp_fmode_wb[] = "w";

/* -------------- Helpers for gp_file_name_combine_generic ------------- */

uint gp_file_name_root(const char *fname, uint len)
{   if (len > 0 && fname[0] == '/')
	return 1;
    return 0;
}

uint gs_file_name_check_separator(const char *fname, int len, const char *item)
{   if (len > 0) {
	if (fname[0] == '/')
	    return 1;
    } else if (len < 0) {
	if (fname[-1] == '/')
	    return 1;
    }
    return 0;
}

bool gp_file_name_is_parent(const char *fname, uint len)
{   return len == 2 && fname[0] == '.' && fname[1] == '.';
}

bool gp_file_name_is_current(const char *fname, uint len)
{   return len == 1 && fname[0] == '.';
}

const char *gp_file_name_separator(void)
{   return "/";
}

const char *gp_file_name_directory_separator(void)
{   return "/";
}

const char *gp_file_name_parent(void)
{   return "..";
}

const char *gp_file_name_current(void)
{   return ".";
}

bool gp_file_name_is_partent_allowed(void)
{   return true;
}

bool gp_file_name_is_empty_item_meanful(void)
{   return false;
}

gp_file_name_combine_result
gp_file_name_combine(const char *prefix, uint plen, const char *fname, uint flen, 
		    bool no_sibling, char *buffer, uint *blen)
{
    return gp_file_name_combine_generic(prefix, plen, 
	    fname, flen, no_sibling, buffer, blen);
}
