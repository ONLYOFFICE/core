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

#if defined(CreateDirectory)
#undef CreateDirectory
#endif
#if defined(CopyDirectory)
#undef CopyDirectory
#endif

#ifndef _BUILD_DIRECTORY_CROSSPLATFORM_H_
#define _BUILD_DIRECTORY_CROSSPLATFORM_H_

#include <stdio.h>
#include <string>
#include <vector>
#include "File.h"

#ifndef FILE_SEPARATOR
	#if defined(_WIN32) || defined(_WIN64)
		#define FILE_SEPARATOR
		#define FILE_SEPARATOR_CHAR '\\'
        #define FILE_SEPARATOR_STR L"\\"
        #define FILE_SEPARATOR_STRA "\\"
    #else
		#define FILE_SEPARATOR
		#define FILE_SEPARATOR_CHAR '/'
        #define FILE_SEPARATOR_STR L"/"
        #define FILE_SEPARATOR_STRA "/"
    #endif
#endif

#include "../../Common/kernel_config.h"
namespace NSDirectory
{
#ifdef _IOS
    KERNEL_DECL void GetFiles2_ios(std::wstring strDirectory, std::vector<std::wstring>& oArray, bool bIsRecursion);
#endif
    KERNEL_DECL void GetFiles2(std::wstring strDirectory, std::vector<std::wstring>& oArray, bool bIsRecursion = false);

    KERNEL_DECL std::vector<std::wstring> GetFiles(std::wstring strDirectory, bool bIsRecursion = false);
    KERNEL_DECL std::vector<std::wstring> GetDirectories(std::wstring strDirectory);
    KERNEL_DECL bool Exists(const std::wstring& strDirectory);
    KERNEL_DECL bool CreateDirectory(const std::wstring& strDirectory);
    KERNEL_DECL bool CreateDirectories(const std::wstring& strDirectory);
    KERNEL_DECL bool CopyDirectory(const std::wstring& strSrc, const std::wstring& strDst, bool bIsRecursion = true);
    KERNEL_DECL void DeleteDirectory(const std::wstring& strDirectory, bool deleteRoot = true);
    KERNEL_DECL std::wstring GetFolderPath(const std::wstring& wsFolderPath);
    KERNEL_DECL std::wstring CreateTempFileWithUniqueName (const std::wstring & strFolderPathRoot, std::wstring Prefix);
    KERNEL_DECL std::wstring CreateDirectoryWithUniqueName (const std::wstring & strFolderPathRoot);
    KERNEL_DECL std::wstring GetTempPath();

    KERNEL_DECL int GetFilesCount(const std::wstring& path, const bool& recursive);
    KERNEL_DECL bool PathIsDirectory(const std::wstring& pathName);
}

#endif //_BUILD_DIRECTORY_CROSSPLATFORM_H_
