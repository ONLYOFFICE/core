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

#include "unicode_util.h"

#include "../../../../DesktopEditor/common/File.h"

#define _T(x)       __T(x)
#define __T(x)      L##x

#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)

#if defined(_WIN32) || defined (_WIN64)
    #include <TCHAR.H>
    // windows-stype separator for paths i.e. 'c:\home\documents\file.ext'

    #ifndef FILE_SEPARATOR
	#define FILE_SEPARATOR
	#define FILE_SEPARATOR_CHAR '\\'
	#define FILE_SEPARATOR_STR _T("\\")
#endif

#else
	#include "stdint.h"

	// linux-stype separator for paths i.e. '/home/documents/file.ext'
	#ifndef FILE_SEPARATOR
	#define FILE_SEPARATOR
	#define FILE_SEPARATOR_CHAR '/'
	#define FILE_SEPARATOR_STR _T("/")
#endif

#ifndef _wtof
    #define _wtof       std::stod
    #define _wtoi64(p)  std::wcstoll((p),NULL,10)

    static int _wtoi(std::wstring sVal)
    {
        int iVal = 0;
        try
        {
            iVal = std::stod(sVal.c_str());
        }catch(...)
        {
            try
            {
                iVal = std::wcstoll(sVal.c_str(), NULL, 10);
            }catch(...)
            {
            }
        }
        return iVal;
    }
    #ifdef UNICODE
        #define _ttoi _wtoi
        #define _tstof _wtof
        #define _stscanf swscanf
    #else
        #define _ttoi atoi
        #define _tstof atof
        #define _stscanf scanf
    #endif // #ifdef UNICODE
#endif // #ifndef _wtof

#endif // #if defined(_WIN32) || defined (_WIN64)

#ifndef AVSINLINE
#if defined(_MSC_VER)
#define AVSINLINE __forceinline
#else
#define AVSINLINE inline
#endif
#endif

