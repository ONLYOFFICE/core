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
	KERNEL_DECL std::vector<std::wstring> GetFiles(std::wstring strDirectory, bool bIsRecursion = false);
	KERNEL_DECL void GetFiles2(std::wstring strDirectory, std::vector<std::wstring>& oArray, bool bIsRecursion = false);

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

#ifdef _IOS
	namespace NSIOS
	{
		KERNEL_DECL void GetFiles2(std::wstring strDirectory, std::vector<std::wstring>& oArray, bool bIsRecursion = false);
	}
#endif
}

#endif //_BUILD_DIRECTORY_CROSSPLATFORM_H_
