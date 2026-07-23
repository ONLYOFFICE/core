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
#define FILE_SEPARATOR_WCHAR L'\\'
#define FILE_SEPARATOR_STR L"\\"
#endif
#else
#include "stdint.h"

// linux-stype separator for paths i.e. '/home/documents/file.ext'
#ifndef FILE_SEPARATOR
#define FILE_SEPARATOR
#define FILE_SEPARATOR_CHAR '/'
#define FILE_SEPARATOR_WCHAR L'/'
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
