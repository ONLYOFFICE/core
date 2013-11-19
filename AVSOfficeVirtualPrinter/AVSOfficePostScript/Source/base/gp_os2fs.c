/* Copyright (C) 2001-2008 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id$ */
/* File access and enumeration for OS/2 */

#define INCL_DOS
#define INCL_BASE
#define INCL_ERRORS
#include <os2.h>

#include "pipe_.h"
#include "stdio_.h"
#include "string_.h"
#include <fcntl.h>

#include "memory_.h"
#include "string_.h"
#include "gx.h"
#include "gsexit.h"
#include "gsmemory.h"
#include "gsstruct.h"
#include "gp.h"
#include "gpmisc.h"
#include "stdlib.h"		/* need _osmode, exit */
#include "time_.h"
#include <time.h>		/* should this be in time_.h? */
#include "gp_os2.h"
#ifdef __EMX__
#include <sys/emxload.h>
#endif

#ifdef __DLL__
#define isos2 TRUE
#else
#define isos2 (_osmode == OS2_MODE)
#endif

/* ------ File naming and accessing ------ */

/* Define the character used for separating file names in a list. */
const char gp_file_name_list_separator = ';';

/* Define the default scratch file name prefix. */
const char gp_scratch_file_name_prefix[] = "gs";

/* Define the name of the null output file. */
const char gp_null_file_name[] = "nul";

/* Define the name that designates the current directory. */
const char gp_current_directory_name[] = ".";

/* Define the string to be concatenated with the file mode */
/* for opening files without end-of-line conversion. */
const char gp_fmode_binary_suffix[] = "b";

/* Define the file modes for binary reading or writing. */
const char gp_fmode_rb[] = "rb";
const char gp_fmode_wb[] = "wb";

/* ------ File enumeration ------ */


struct file_enum_s {
    FILEFINDBUF3 findbuf;
    HDIR hdir;
    char *pattern;
    int patlen;			/* orig pattern length */
    int pat_size;		/* allocate space for pattern */
    int head_size;		/* pattern length through last */
    /* :, / or \ */
    int first_time;
    gs_memory_t *memory;
};
gs_private_st_ptrs1(st_file_enum, struct file_enum_s, "file_enum",
		    file_enum_enum_ptrs, file_enum_reloc_ptrs, pattern);

/* Initialize an enumeration.  may NEED WORK ON HANDLING * ? \. */
file_enum *
gp_enumerate_files_init(const char *pat, uint patlen, gs_memory_t * mem)
{
    file_enum *pfen = gs_alloc_struct(mem, file_enum, &st_file_enum, "gp_enumerate_files");
    int pat_size = 2 * patlen + 1;
    char *pattern;
    int hsize = 0;
    int i;

    if (pfen == 0)
	return 0;

    /* pattern could be allocated as a string, */
    /* but it's simpler for GC and freeing to allocate it as bytes. */

    pattern = (char *)gs_alloc_bytes(mem, pat_size,
				     "gp_enumerate_files(pattern)");
    if (pattern == 0)
	return 0;
    memcpy(pattern, pat, patlen);
    /* find directory name = header */
    for (i = 0; i < patlen; i++) {
	switch (pat[i]) {
	    case '\\':
		if (i + 1 < patlen && pat[i + 1] == '\\')
		    i++;
		/* falls through */
	    case ':':
	    case '/':
		hsize = i + 1;
	}
    }
    pattern[patlen] = 0;
    pfen->pattern = pattern;
    pfen->patlen = patlen;
    pfen->pat_size = pat_size;
    pfen->head_size = hsize;
    pfen->memory = mem;
    pfen->first_time = 1;
    pfen->hdir = HDIR_CREATE;
    return pfen;
}

/* Enumerate the next file. */
uint
gp_enumerate_files_next(file_enum * pfen, char *ptr, uint maxlen)
{
    APIRET rc;
    ULONG cFilenames = 1;

    if (!isos2) {
	/* CAN'T DO IT SO JUST RETURN THE PATTERN. */
	if (pfen->first_time) {
	    char *pattern = pfen->pattern;
	    uint len = strlen(pattern);

	    pfen->first_time = 0;
	    if (len > maxlen)
		return maxlen + 1;
	    strcpy(ptr, pattern);
	    return len;
	}
	return -1;
    }
    /* else OS/2 */
    if (pfen->first_time) {
	rc = DosFindFirst(pfen->pattern, &pfen->hdir, FILE_NORMAL,
			  &pfen->findbuf, sizeof(pfen->findbuf),
			  &cFilenames, FIL_STANDARD);
	pfen->first_time = 0;
    } else {
	rc = DosFindNext(pfen->hdir, &pfen->findbuf, sizeof(pfen->findbuf),
			 &cFilenames);
    }
    if (rc)
	return -1;

    if (pfen->head_size + pfen->findbuf.cchName < maxlen) {
	memcpy(ptr, pfen->pattern, pfen->head_size);
	strcpy(ptr + pfen->head_size, pfen->findbuf.achName);
	return pfen->head_size + pfen->findbuf.cchName;
    }
    if (pfen->head_size >= maxlen)
	return 0;		/* no hope at all */

    memcpy(ptr, pfen->pattern, pfen->head_size);
    strncpy(ptr + pfen->head_size, pfen->findbuf.achName,
	    maxlen - pfen->head_size - 1);
    return maxlen;
}

/* Clean up the file enumeration. */
void
gp_enumerate_files_close(file_enum * pfen)
{
    gs_memory_t *mem = pfen->memory;

    if (isos2)
	DosFindClose(pfen->hdir);
    gs_free_object(mem, pfen->pattern,
		   "gp_enumerate_files_close(pattern)");
    gs_free_object(mem, pfen, "gp_enumerate_files_close");
}


/* ------ File naming and accessing ------ */

/* Create and open a scratch file with a given name prefix. */
/* Write the actual file name at fname. */
FILE *
gp_open_scratch_file(const char *prefix, char fname[gp_file_name_sizeof],
		     const char *mode)
{
    FILE *f;
#ifdef __IBMC__
    char *temp = 0;
    char *tname;
    int prefix_length = strlen(prefix);

    if (!gp_file_name_is_absolute(prefix, prefix_length)) {
	temp = getenv("TMPDIR");
	if (temp == 0)
	    temp = getenv("TEMP");
    }
    *fname = 0;
    tname = _tempnam(temp, (char *)prefix);
    if (tname) {
	if (strlen(tname) > gp_file_name_sizeof - 1) {
	    free(tname);
	    return 0;		/* file name too long */
	}
	strcpy(fname, tname);
	free(tname);
    }
#else
    /* The -7 is for XXXXXX plus a possible final \. */
    int prefix_length = strlen(prefix);
    int len = gp_file_name_sizeof - prefix_length - 7;

    if (gp_file_name_is_absolute(prefix, prefix_length) ||
	gp_gettmpdir(fname, &len) != 0)
	*fname = 0;
    else {
	char last = '\\';
	char *temp;

	/* Prevent X's in path from being converted by mktemp. */
	for (temp = fname; *temp; temp++)
	    *temp = last = tolower(*temp);
	switch (last) {
	default:
	    strcat(fname, "\\");
	case ':':
	case '\\':
	    ;
	}
    }
    if (strlen(fname) + prefix_length + 7 >= gp_file_name_sizeof)
	return 0;		/* file name too long */
    strcat(fname, prefix);
    strcat(fname, "XXXXXX");
    mktemp(fname);
#endif
    f = gp_fopentemp(fname, mode);
    return f;
}

/* Open a file with the given name, as a stream of uninterpreted bytes. */
FILE *
gp_fopen(const char *fname, const char *mode)
{
    return fopen(fname, mode);
}

/* -------------- Helpers for gp_file_name_combine_generic ------------- */

uint gp_file_name_root(const char *fname, uint len)
{   int i = 0;
    
    if (len == 0)
	return 0;
    if (len > 1 && fname[0] == '\\' && fname[1] == '\\') {
	/* A network path: "\\server\share\" */
	int k = 0;

	for (i = 2; i < len; i++)
	    if (fname[i] == '\\' || fname[i] == '/')
		if (k++) {
		    i++;
		    break;
		}
    } else if (fname[0] == '/' || fname[0] == '\\') {
	/* Absolute with no drive. */
	i = 1;
    } else if (len > 1 && fname[1] == ':') {
	/* Absolute with a drive. */
	i = (len > 2 && (fname[2] == '/' || fname[2] == '\\') ? 3 : 2);
    }
    return i;
}

uint gs_file_name_check_separator(const char *fname, int len, const char *item)
{   if (len > 0) {
	if (fname[0] == '/' || fname[0] == '\\')
	    return 1;
    } else if (len < 0) {
	if (fname[-1] == '/' || fname[-1] == '\\')
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


