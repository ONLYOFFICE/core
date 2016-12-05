/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#if defined(_WIN32) || defined (_WIN64)
#pragma warning( disable : 4996 )


#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE	1
#endif

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE	1
#endif

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#ifndef _ATL_APARTMENT_THREADED
#define _ATL_APARTMENT_THREADED
#endif

#ifndef _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#endif

#ifndef _ATL_ALL_WARNINGS
// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS
#endif

#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>

// windows-stype separator for paths i.e. 'c:\home\documents\file.ext'
#ifndef FILE_SEPARATOR
#define FILE_SEPARATOR
#define FILE_SEPARATOR_CHAR '\\'
#define FILE_SEPARATOR_STR _T("\\")
#endif

#include "../../../ASCUtils.h"

using namespace ATL;

#else
#include "ASCString.h"
#include "stdint.h"

// linux-stype separator for paths i.e. '/home/documents/file.ext'
#ifndef FILE_SEPARATOR
#define FILE_SEPARATOR
#define FILE_SEPARATOR_CHAR '/'
#define FILE_SEPARATOR_STR _T("/")
#endif


#ifndef _wtof
    #define _wtof std::stod
    //#define _wtoi std::stoi
    #define _wtoi64(p) std::wcstoll((p),NULL,10)

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

