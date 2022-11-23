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

#include <string>
#include <stdlib.h>
#include <string.h>

#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)

#define _T(x)       __T(x)
#define __T(x)      L##x

#if defined(_WIN32) || defined (_WIN64)
#include <TCHAR.H>
// windows-stype separator for paths i.e. 'c:\home\documents\file.ext'

#ifndef FILE_SEPARATOR
#define FILE_SEPARATOR
#define FILE_SEPARATOR_CHAR '\\'
#define FILE_SEPARATOR_STR L"\\"
#endif
#else
#include "stdint.h"

// linux-stype separator for paths i.e. '/home/documents/file.ext'
#ifndef FILE_SEPARATOR
#define FILE_SEPARATOR
#define FILE_SEPARATOR_CHAR '/'
#define FILE_SEPARATOR_STR L"/"
#endif
#endif

#ifndef AVSINLINE
#if defined(_MSC_VER)
#define AVSINLINE __forceinline
#else
#define AVSINLINE inline
#endif
#endif

#if defined(_WIN32) || defined (_WIN64)
typedef __int16				_INT16;
typedef __int32				_INT32;
typedef __int64				_INT64;
typedef unsigned __int16	_UINT16;
typedef unsigned __int32	_UINT32;
typedef unsigned __int64	_UINT64;
#else
typedef int16_t             _INT16;
typedef int32_t             _INT32;
typedef int64_t             _INT64;
typedef uint16_t            _UINT16;
typedef uint32_t            _UINT32;
typedef uint64_t            _UINT64;
#endif
