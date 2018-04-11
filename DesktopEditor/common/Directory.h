/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
	#else
		#define FILE_SEPARATOR
		#define FILE_SEPARATOR_CHAR '/'
        #define FILE_SEPARATOR_STR L"/"
	#endif
#endif

#ifndef DIRECTORY_USE_DYNAMIC_LIBRARY
#define DIRECTORY_DECL_EXPORT
#else
#include "./base_export.h"
#define DIRECTORY_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace NSDirectory
{
#ifdef _IOS
    DIRECTORY_DECL_EXPORT void GetFiles2_ios(std::wstring strDirectory, std::vector<std::wstring>& oArray, bool bIsRecursion);
#endif
    DIRECTORY_DECL_EXPORT void GetFiles2(std::wstring strDirectory, std::vector<std::wstring>& oArray, bool bIsRecursion = false);

    DIRECTORY_DECL_EXPORT std::vector<std::wstring> GetFiles(std::wstring strDirectory, bool bIsRecursion = false);
    DIRECTORY_DECL_EXPORT std::vector<std::wstring> GetDirectories(std::wstring strDirectory);
    DIRECTORY_DECL_EXPORT bool Exists(const std::wstring& strDirectory);
    DIRECTORY_DECL_EXPORT bool CreateDirectory(const std::wstring& strDirectory);
    DIRECTORY_DECL_EXPORT bool CreateDirectories(const std::wstring& strDirectory);
    DIRECTORY_DECL_EXPORT void DeleteDirectory(const std::wstring& strDirectory, bool deleteRoot = true);
    DIRECTORY_DECL_EXPORT std::wstring GetFolderPath(const std::wstring& wsFolderPath);
    DIRECTORY_DECL_EXPORT std::wstring CreateTempFileWithUniqueName (const std::wstring & strFolderPathRoot, std::wstring Prefix);
    DIRECTORY_DECL_EXPORT std::wstring CreateDirectoryWithUniqueName (const std::wstring & strFolderPathRoot);
    DIRECTORY_DECL_EXPORT std::wstring GetTempPath();

    DIRECTORY_DECL_EXPORT int GetFilesCount(const std::wstring& path, const bool& recursive);
    DIRECTORY_DECL_EXPORT bool PathIsDirectory(const std::wstring& pathName);
}

#endif //_BUILD_DIRECTORY_CROSSPLATFORM_H_
