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

/* $Id: gpmisc.c 8250 2007-09-25 13:31:24Z giles $ */
/* Miscellaneous support for platform facilities */

#include "unistd_.h"
#include "fcntl_.h"
#include "stdio_.h"
#include "stat_.h"
#include "memory_.h"
#include "string_.h"
#include "gp.h"
#include "gpgetenv.h"
#include "gpmisc.h"

/*
 * Get the name of the directory for temporary files, if any.  Currently
 * this checks the TMPDIR and TEMP environment variables, in that order.
 * The return value and the setting of *ptr and *plen are as for gp_getenv.
 */
int
gp_gettmpdir(char *ptr, int *plen)
{
    int max_len = *plen;
    int code = gp_getenv("TMPDIR", ptr, plen);

    if (code != 1)
	return code;
    *plen = max_len;
    return gp_getenv("TEMP", ptr, plen);
}

/*
 * Open a temporary file, using O_EXCL and S_I*USR to prevent race
 * conditions and symlink attacks.
 */
static FILE *
gp_fopentemp_generic(const char *fname, const char *mode, bool b64)
{
    int flags = O_EXCL;
    /* Scan the mode to construct the flags. */
    const char *p = mode;
    int fildes;
    FILE *file;

#if defined (O_LARGEFILE)
    /* It works for Linux/gcc. */
    if (b64)
	flags |= O_LARGEFILE;
#else
    /* fixme : Not sure what to do. Unimplemented. */
    /* MSVC has no O_LARGEFILE, but MSVC build never calls this function. */
#endif
    while (*p)
	switch (*p++) {
	case 'a':
	    flags |= O_CREAT | O_APPEND;
	    break;
	case 'r':
	    flags |= O_RDONLY;
	    break;
	case 'w':
	    flags |= O_CREAT | O_WRONLY | O_TRUNC;
	    break;
#ifdef O_BINARY
	    /* Watcom C insists on this non-ANSI flag being set. */
	case 'b':
	    flags |= O_BINARY;
	    break;
#endif
	case '+':
	    flags = (flags & ~(O_RDONLY | O_WRONLY)) | O_RDWR;
	    break;
	default:		/* e.g., 'b' */
	    break;
	}
    fildes = open(fname, flags, S_IRUSR | S_IWUSR);
    if (fildes < 0)
	return 0;
    /*
     * The DEC VMS C compiler incorrectly defines the second argument of
     * fdopen as (char *), rather than following the POSIX.1 standard,
     * which defines it as (const char *).  Patch this here.
     */
    file = fdopen(fildes, (char *)mode); /* still really const */
    if (file == 0)
	close(fildes);
    return file;
}

FILE *gp_fopentemp_64(const char *fname, const char *mode)
{
    return gp_fopentemp_generic(fname, mode, true);
}

FILE *gp_fopentemp(const char *fname, const char *mode)
{
    return gp_fopentemp_generic(fname, mode, false);
}

/* Append a string to buffer. */
static inline bool
append(char **bp, const char *bpe, const char **ip, uint len)
{
    if (bpe - *bp < len)
	return false;
    memcpy(*bp, *ip, len);
    *bp += len;
    *ip += len;
    return true;
}

/* Search a separator forward. */
static inline uint
search_separator(const char **ip, const char *ipe, const char *item, int direction)
{   uint slen = 0;
    for (slen = 0; (*ip - ipe) * direction < 0; (*ip) += direction)
	if((slen = gs_file_name_check_separator(*ip, ipe - *ip, item)) != 0)
	    break;
    return slen;
}


/*
 * Combine a file name with a prefix.
 * Concatenates two paths and reduce parent references and current 
 * directory references from the concatenation when possible.
 * The trailing zero byte is being added.
 *
 * Returns "gp_combine_success" if OK and sets '*blen' to the length of
 * the combined string. If the combined string is too small for the buffer
 * length passed in (as defined by the initial value of *blen), then the
 * "gp_combine_small_buffer" code is returned.
 *
 * Also tolerates/skips leading IODevice specifiers such as %os% or %rom%
 * When there is a leading '%' in the 'fname' no other processing is done.
 *
 * Examples : 
 *	"/gs/lib" + "../Resource/CMap/H" --> "/gs/Resource/CMap/H"
 *	"C:/gs/lib" + "../Resource/CMap/H" --> "C:/gs/Resource/CMap/H"
 *	"hard disk:gs:lib" + "::Resource:CMap:H" --> 
 *		"hard disk:gs:Resource:CMap:H"
 *	"DUA1:[GHOSTSCRIPT.LIB" + "-.RESOURCE.CMAP]H" --> 
 *		"DUA1:[GHOSTSCRIPT.RESOURCE.CMAP]H"
 *      "\\server\share/a/b///c/../d.e/./" + "../x.e/././/../../y.z/v.v" --> 
 *		"\\server\share/a/y.z/v.v"
 *	"%rom%lib/" + "gs_init.ps" --> "%rom%lib/gs_init.ps
 *	"" + "%rom%lib/gs_init.ps" --> "%rom%lib/gs_init.ps"
 */
gp_file_name_combine_result
gp_file_name_combine_generic(const char *prefix, uint plen, const char *fname, uint flen, 
		    bool no_sibling, char *buffer, uint *blen)
{
    /*
     * THIS CODE IS SHARED FOR MULTIPLE PLATFORMS.
     * PLEASE DON'T CHANGE IT FOR A SPECIFIC PLATFORM.
     * Change gp_file_name_combine instead.
     */
    char *bp = buffer, *bpe = buffer + *blen;
    const char *ip, *ipe;
    uint slen;
    uint infix_type = 0; /* 0=none, 1=current, 2=parent. */
    uint infix_len = 0;
    uint rlen = gp_file_name_root(fname, flen);
    /* We need a special handling of infixes only immediately after a drive. */

    if ( flen > 0 && fname[0] == '%') {
    	/* IoDevice -- just return the fname as-is since this */
	/* function only handles the default file system */
	/* NOTE: %os% will subvert the normal processing of prefix and fname */
	ip = fname;
	*blen = flen;
	if (!append(&bp, bpe, &ip, flen))
	    return gp_combine_small_buffer;
	return gp_combine_success;
    }
    if (rlen != 0) {
        /* 'fname' is absolute, ignore the prefix. */
	ip = fname;
	ipe = fname + flen;
    } else {
        /* Concatenate with prefix. */
	ip = prefix;
	ipe = prefix + plen;
	rlen = gp_file_name_root(prefix, plen);
    }
    if (!append(&bp, bpe, &ip, rlen))
	return gp_combine_small_buffer;
    slen = gs_file_name_check_separator(bp, buffer - bp, bp); /* Backward search. */
    if (rlen != 0 && slen == 0) {
	/* Patch it against names like "c:dir" on Windows. */
	const char *sep = gp_file_name_directory_separator();

	slen = strlen(sep);
	if (!append(&bp, bpe, &sep, slen))
	    return gp_combine_small_buffer;
	rlen += slen;
    }
    for (;;) {
	const char *item = ip;
	uint ilen;

	slen = search_separator(&ip, ipe, item, 1);
	ilen = ip - item;
	if (ilen == 0 && !gp_file_name_is_empty_item_meanful()) {
	    ip += slen;
	    slen = 0;
	} else if (gp_file_name_is_current(item, ilen)) {
	    /* Skip the reference to 'current', except the starting one.
	     * We keep the starting 'current' for platforms, which
	     * require relative paths to start with it.
	     */
	    if (bp == buffer) {
		if (!append(&bp, bpe, &item, ilen))
		    return gp_combine_small_buffer;
		infix_type = 1;
		infix_len = ilen;
	    } else {
		ip += slen;
		slen = 0;
	    }
	} else if (!gp_file_name_is_parent(item, ilen)) {
	    if (!append(&bp, bpe, &item, ilen))
		return gp_combine_small_buffer;
	    /* The 'item' pointer is now broken; it may be restored using 'ilen'. */
	} else if (bp == buffer + rlen + infix_len) {
	    /* Input is a parent and the output only contains a root and an infix. */
	    if (rlen != 0)
		return gp_combine_cant_handle;
	    switch (infix_type) {
		case 1:
		    /* Replace the infix with parent. */
		    bp = buffer + rlen; /* Drop the old infix, if any. */
		    infix_len = 0;
		    /* Falls through. */
		case 0:
		    /* We have no infix, start with parent. */
		    if ((no_sibling && ipe == fname + flen && flen != 0) || 
			    !gp_file_name_is_partent_allowed())
			return gp_combine_cant_handle;
		    /* Falls through. */
		case 2:
		    /* Append one more parent - falls through. */
		    DO_NOTHING;
	    }
	    if (!append(&bp, bpe, &item, ilen))
		return gp_combine_small_buffer;
	    infix_type = 2;
	    infix_len += ilen;
	    /* Recompute the separator length. We cannot use the old slen on Mac OS. */
	    slen = gs_file_name_check_separator(ip, ipe - ip, ip);
	} else {
	    /* Input is a parent and the output continues after infix. */
	    /* Unappend the last separator and the last item. */
	    uint slen1 = gs_file_name_check_separator(bp, buffer + rlen - bp, bp); /* Backward search. */
	    char *bie = bp - slen1;

	    bp = bie;
	    DISCARD(search_separator((const char **)&bp, buffer + rlen, bp, -1));
	    /* The cast above quiets a gcc warning. We believe it's a bug in the compiler. */
	    /* Skip the input with separator. We cannot use slen on Mac OS. */
	    ip += gs_file_name_check_separator(ip, ipe - ip, ip);
	    if (no_sibling) {
		const char *p = ip;

		DISCARD(search_separator(&p, ipe, ip, 1));
		if (p - ip != bie - bp || memcmp(ip, bp, p - ip))    
		    return gp_combine_cant_handle;
	    }
	    slen = 0;
	}
	if (slen) {
	    if (bp == buffer + rlen + infix_len)
		infix_len += slen;
	    if (!append(&bp, bpe, &ip, slen))
		return gp_combine_small_buffer;
	}
	if (ip == ipe) {
	    if (ipe == fname + flen) {
		/* All done.
		 * Note that the case (prefix + plen == fname && flen == 0)
		 * falls here without appending a separator.
		 */
		const char *zero="";

		if (bp == buffer) {
		    /* Must not return empty path. */
		    const char *current = gp_file_name_current();
		    int clen = strlen(current);

		    if (!append(&bp, bpe, &current, clen))
			return gp_combine_small_buffer;
		}
		*blen = bp - buffer;
		if (!append(&bp, bpe, &zero, 1))
		    return gp_combine_small_buffer;
		return gp_combine_success;
	    } else {
	        /* ipe == prefix + plen */
		/* Switch to fname. */
		ip = fname;
		ipe = fname + flen;
		if (slen == 0) {
		    /* Insert a separator. */
		    const char *sep;
    
		    slen = search_separator(&ip, ipe, fname, 1);
		    sep = (slen != 0 ? gp_file_name_directory_separator() 
		                    : gp_file_name_separator());
		    slen = strlen(sep);
		    if (bp == buffer + rlen + infix_len)
			infix_len += slen;
		    if (!append(&bp, bpe, &sep, slen))
			return gp_combine_small_buffer;
		    ip = fname; /* Switch to fname. */
		}
	    }
	}
    }
}

/*
 * Reduces parent references and current directory references when possible.
 * The trailing zero byte is being added.
 *
 * Examples : 
 *	"/gs/lib/../Resource/CMap/H" --> "/gs/Resource/CMap/H"
 *	"C:/gs/lib/../Resource/CMap/H" --> "C:/gs/Resource/CMap/H"
 *	"hard disk:gs:lib::Resource:CMap:H" --> 
 *		"hard disk:gs:Resource:CMap:H"
 *	"DUA1:[GHOSTSCRIPT.LIB.-.RESOURCE.CMAP]H" --> 
 *		"DUA1:[GHOSTSCRIPT.RESOURCE.CMAP]H"
 *      "\\server\share/a/b///c/../d.e/./../x.e/././/../../y.z/v.v" --> 
 *		"\\server\share/a/y.z/v.v"
 *
 */
gp_file_name_combine_result
gp_file_name_reduce(const char *fname, uint flen, char *buffer, uint *blen)
{
    return gp_file_name_combine(fname, flen, fname + flen, 0, false, buffer, blen);
}

/* 
 * Answers whether a file name is absolute (starts from a root). 
 */
bool 
gp_file_name_is_absolute(const char *fname, uint flen)
{
    return (gp_file_name_root(fname, flen) > 0);
}

/* 
 * Returns length of all starting parent references.
 */
static uint 
gp_file_name_prefix(const char *fname, uint flen, 
		bool (*test)(const char *fname, uint flen))
{
    uint plen = gp_file_name_root(fname, flen), slen;
    const char *ip, *ipe; 
    const char *item = fname; /* plen == flen could cause an indeterminizm. */

    if (plen > 0)
	return 0;
    ip = fname + plen;
    ipe = fname + flen;
    for (; ip < ipe; ) {
	item = ip;
	slen = search_separator(&ip, ipe, item, 1);
	if (!(*test)(item, ip - item))
	    break;
	ip += slen;
    }
    return item - fname;
}

/* 
 * Returns length of all starting parent references.
 */
uint 
gp_file_name_parents(const char *fname, uint flen)
{
    return gp_file_name_prefix(fname, flen, gp_file_name_is_parent); 
}

/* 
 * Returns length of all starting cwd references.
 */
uint 
gp_file_name_cwds(const char *fname, uint flen)
{
    return gp_file_name_prefix(fname, flen, gp_file_name_is_current); 
}
