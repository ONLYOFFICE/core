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
#include <deque>
#include <fstream>
#include <vector>
#include <list>

#if defined(_WIN32) || defined (_WIN64)
#include <direct.h>
#include <windows.h>
#endif

#include "OfficeUtilsCommon.h"
#include "../../DesktopEditor/common/Types.h"

using namespace std;

#include "zlib-1.2.11/contrib/minizip/unzip.h"
#include "zlib-1.2.11/contrib/minizip/zip.h"
#include "zlib-1.2.11/contrib/minizip/ioapibuf.h"
#if defined(_WIN32) || defined (_WIN64)
#include "zlib-1.2.11/contrib/minizip/iowin32.h"
#endif
#include <zlib.h>

namespace ZLibZipUtils
{
	zipFile zipOpenHelp(const wchar_t* filename);
	unzFile unzOpenHelp(const wchar_t* filename);

	int ZipDir( const WCHAR* dir, const WCHAR* outputFile, const OnProgressCallback* progress, bool sorted = false, int method = Z_DEFLATED, int compressionLevel = -1, bool bDateTime = false);
	int ZipFile( const WCHAR* inputFile, const WCHAR* outputFile, int method = Z_DEFLATED, int compressionLevel = -1, bool bDateTime = false );
	bool ClearDirectory( const WCHAR* dir, bool delDir = false );

	int UnzipToDir( unzFile uf, const WCHAR* unzipDir, const OnProgressCallback* progress, const WCHAR* password, bool opt_extract_without_path, bool clearOutputDirectory );
	int UnzipToDir( const WCHAR* zipFile, const WCHAR* unzipDir, const OnProgressCallback* progress, const WCHAR* password = NULL, bool opt_extract_without_path = false, bool clearOutputDirectory = false );
	int UnzipToDir( BYTE* data, size_t len, const WCHAR* unzipDir, const OnProgressCallback* progress, const WCHAR* password = NULL, bool opt_extract_without_path = false, bool clearOutputDirectory = false );

	int UncompressBytes( BYTE* destBuf, ULONG* destSize, const BYTE* sourceBuf, ULONG sourceSize );
	int CompressBytes( BYTE* destBuf, ULONG* destSize, const BYTE* sourceBuf, ULONG sourceSize, SHORT level );

	bool IsArchive(const WCHAR* filename);
	bool IsArchive(BYTE* data, size_t len);

	bool IsFileExistInArchive(const WCHAR* zipFile, const WCHAR* filePathInZip);
	bool IsFileExistInArchive(BYTE* data, size_t len, const WCHAR* filePathInZip);

	bool LoadFileFromArchive(const WCHAR* zipFile, const WCHAR* filePathInZip, BYTE** fileInBytes, ULONG& nFileSize);
	bool LoadFileFromArchive(BYTE* data, size_t len, const WCHAR* filePathInZip, BYTE** fileInBytes, ULONG& nFileSize);

	bool ExtractFiles(const WCHAR* zip_file_path, const ExtractedFileCallback& callback, void* pParam);
	bool CompressFiles(const WCHAR* zip_file_path, const RequestFileCallback& callback, void* pParam, int compression_level);
	bool GetFilesSize(const WCHAR* zip_file_path, const std::wstring& searchPattern, ULONG64& nCommpressed, ULONG64& nUncommpressed);
}
