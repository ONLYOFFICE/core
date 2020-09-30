/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#pragma once

#ifndef _BUILD_TYPES_CROSSPLATFORM_H_
#define _BUILD_TYPES_CROSSPLATFORM_H_

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

typedef unsigned long       DWORD;

#if defined (_WIN32) || defined (_WIN64)
	typedef int                 BOOL;
#else

	#ifndef _IOS
	//	typedef int                 BOOL;
	#else
		#ifndef _XCODE
			#define BOOL int
		#endif
	#endif

#endif
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               REAL, FLOAT, *PFLOAT;

typedef char			CHAR;
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef long			LONG;
typedef int				INT;
typedef unsigned int	UINT, *PUINT;
typedef wchar_t			WCHAR;

#ifdef __linux__
#include <inttypes.h>
typedef int64_t     T_LONG64;
typedef uint64_t    T_ULONG64;
typedef T_LONG64    __int64;

typedef T_ULONG64   ULONG64;
typedef T_LONG64    LONG64;
typedef T_ULONG64   UINT64;
#else

#ifdef __APPLE__
#include <inttypes.h>
typedef int64_t		T_LONG64;
typedef uint64_t	T_ULONG64;
typedef T_LONG64    __int64;

typedef T_ULONG64   ULONG64;
typedef T_LONG64    LONG64;
typedef T_ULONG64   UINT64;

#else

#if (!defined (_MAC) && (!defined(MIDL_PASS) || defined(__midl)) && (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64)))
typedef __int64				T_LONG64;
typedef unsigned __int64	T_ULONG64;
typedef T_ULONG64   ULONG64;
typedef T_LONG64    LONG64;
typedef T_ULONG64   UINT64;
#else
#if defined(_MAC) && defined(_MAC_INT_64)
typedef __int64				T_LONG64;
typedef unsigned __int64	T_ULONG64;
#else
typedef double T_LONG64;
typedef double T_ULONG64;
#endif //_MAC and int64
#endif

#endif

#endif

#ifndef VOID
    typedef void			VOID, *LPVOID;
#endif

#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F        /* smallest such that 1.0+FLT_EPSILON != 1.0 */
#endif

typedef unsigned long		ULONG, ARGB;

typedef long HRESULT;

#if defined(_WIN32) || defined (_WIN64)
    #include "winerror.h"
#else
    #ifndef S_OK
        #define S_OK                                   ((HRESULT)0x00000000L)
        #define S_FALSE                                ((HRESULT)0x00000001L)
    #endif
#endif


#define RELEASEMEM(pobject)\
{\
	if (pobject!=NULL)\
	{\
		free(pobject);\
		pobject=NULL;\
	}\
}
#define RELEASEOBJECT(pobject)\
{\
	if (pobject!=NULL)\
	{\
		delete pobject;\
		pobject=NULL;\
	}\
}
#define RELEASEARRAYOBJECTS(pobject)\
{\
	if (pobject!=NULL)\
	{\
		delete []pobject;\
		pobject=NULL;\
	}\
}

#define ADDREFINTERFACE(pinterface)\
{\
    if (pinterface!=NULL)\
    {\
        pinterface->AddRef();\
    }\
}
#define RELEASEINTERFACE(pinterface)\
{\
    if (pinterface!=NULL)\
    {\
        pinterface->Release();\
        pinterface=NULL;\
    }\
}

#endif //_BUILD_TYPES_CROSSPLATFORM_H_
