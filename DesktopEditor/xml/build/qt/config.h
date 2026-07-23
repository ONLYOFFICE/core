/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef __LIBXML_WIN32_CONFIG__
#define __LIBXML_WIN32_CONFIG__

#define HAVE_CTYPE_H
#define HAVE_STDARG_H
#define HAVE_MALLOC_H
#define HAVE_ERRNO_H
#define HAVE_STDINT_H

#if defined(_WIN32_WCE)
#undef HAVE_ERRNO_H
#include <windows.h>
#include "wincecompat.h"
#else
#define HAVE_SYS_STAT_H
#define HAVE__STAT
#define HAVE_STAT
#define HAVE_STDLIB_H
#define HAVE_TIME_H
#define HAVE_FCNTL_H
//#include <io.h>
//#include <direct.h>
#endif

#include <libxml/xmlversion.h>

#ifndef ICONV_CONST
#define ICONV_CONST const
#endif

//#ifdef NEED_SOCKETS
//#include <wsockcompat.h>
//#endif

/*
 * Windows platforms may define except 
 */
#undef except

#define HAVE_ISINF
#define HAVE_ISNAN
#include <math.h>
#if defined(_MSC_VER) || defined(__BORLANDC__)
/* MS C-runtime has functions which can be used in order to determine if
   a given floating-point variable contains NaN, (+-)INF. These are 
   preferred, because floating-point technology is considered propriatary
   by MS and we can assume that their functions know more about their 
   oddities than we do. */
#include <float.h>
/* Bjorn Reese figured a quite nice construct for isinf() using the _fpclass
   function. */
#ifndef isinf
#define isinf(d) ((_fpclass(d) == _FPCLASS_PINF) ? 1 \
	: ((_fpclass(d) == _FPCLASS_NINF) ? -1 : 0))
#endif
/* _isnan(x) returns nonzero if (x == NaN) and zero otherwise. */
#ifndef isnan
#define isnan(d) (_isnan(d))
#endif
#else /* _MSC_VER */
#ifdef WIN32
#ifndef isinf
int isinf (double d) {
    int expon = 0;
    double val = frexp (d, &expon);
    if (expon == 1025) {
        if (val == 0.5) {
            return 1;
        } else if (val == -0.5) {
            return -1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}
#endif
#ifndef isnan
int isnan (double d) {
    int expon = 0;
    double val = frexp (d, &expon);
    if (expon == 1025) {
        if (val == 0.5) {
            return 0;
        } else if (val == -0.5) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
#endif
#endif
#endif /* _MSC_VER */

#if defined(_MSC_VER)
#define mkdir(p,m) _mkdir(p)
#if _MSC_VER < 1900
#define snprintf _snprintf
#endif
#if _MSC_VER < 1500
#define vsnprintf(b,c,f,a) _vsnprintf(b,c,f,a)
#endif
#elif defined(__MINGW32__)
#define mkdir(p,m) _mkdir(p)
#endif

/* Threading API to use should be specified here for compatibility reasons.
   This is however best specified on the compiler's command-line. */
#if defined(LIBXML_THREAD_ENABLED)
#if !defined(HAVE_PTHREAD_H) && !defined(HAVE_WIN32_THREADS) && !defined(_WIN32_WCE)
#define HAVE_WIN32_THREADS
#endif
#endif

#ifndef LIBXML_READER_ENABLED
#define LIBXML_READER_ENABLED
#endif

#ifndef LIBXML_PUSH_ENABLED
#define LIBXML_PUSH_ENABLED
#endif

#ifndef LIBXML_HTML_ENABLED
#define LIBXML_HTML_ENABLED
#endif

/* Some third-party libraries far from our control assume the following
   is defined, which it is not if we don't include windows.h. */
#if !defined(FALSE)
#define FALSE 0
#endif
#if !defined(TRUE)
#define TRUE (!(FALSE))
#endif

#endif /* __LIBXML_WIN32_CONFIG__ */
