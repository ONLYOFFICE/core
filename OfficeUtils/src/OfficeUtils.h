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
#ifndef OFFICE_UTILS_H
#define OFFICE_UTILS_H

#include "OfficeUtilsCommon.h"
#include "../../Common/kernel_config.h"

#define ZLIB_ADDON_FLAG_ZERO                    0
#define ZLIB_ADDON_FLAG_READ_ONLY               1
#define ZLIB_ADDON_FLAG_WINDOWS_SHARED_WRITE    2

#ifndef Z_DEFLATED
#define Z_DEFLATED 8
#endif

class KERNEL_DECL COfficeUtils
{
private:
	OnProgressCallback* m_fCallback;
public:
	COfficeUtils(OnProgressCallback* bCallback = NULL);

	HRESULT ExtractToDirectory		(const std::wstring& zipFile, const std::wstring& unzipDir, wchar_t* password, short extract_without_path);
	HRESULT ExtractToDirectory		(BYTE* data, size_t len, const std::wstring& unzipDir, wchar_t* password, short extract_without_path);

	HRESULT CompressFileOrDirectory	(const std::wstring& name, const std::wstring& outputFile, bool bSorted = false, int method = Z_DEFLATED, short level = -1, bool bDateTime = true);
	
	HRESULT Uncompress	(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize);
	HRESULT Compress	(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize, short level = -1);

	HRESULT IsArchive				(const std::wstring& filename);
	HRESULT IsArchive				(BYTE* data, size_t len);

	HRESULT IsFileExistInArchive	(const std::wstring& zipFile, const std::wstring& filePath);
	HRESULT IsFileExistInArchive	(BYTE* data, size_t len, const std::wstring& filePath);

	HRESULT LoadFileFromArchive		(const std::wstring& zipFile, const std::wstring& filePath, BYTE** fileInBytes, ULONG& nFileSize);
	HRESULT LoadFileFromArchive		(BYTE* data, size_t len, const std::wstring& filePath, BYTE** fileInBytes, ULONG& nFileSize);

	HRESULT ExtractFilesToMemory	(const std::wstring& zipFile, const ExtractedFileCallback& data_receiver, void* pParam, bool* result);
	HRESULT CompressFilesFromMemory	(const std::wstring& zipFile, const RequestFileCallback& data_source, void* pParam, short compression_level, bool* result);
	HRESULT GetFilesSize			(const std::wstring& zipFile, const std::wstring& searchPattern, ULONG64& nCommpressed, ULONG64& nUncommpressed);

	static int GetAddonFlag();
	static void SetAddonFlag(int flag);
};

#define DEFLATE_NO_FLUSH      0
#define DEFLATE_PARTIAL_FLUSH 1 /* will be removed, use DEFLATE_SYNC_FLUSH instead */
#define DEFLATE_SYNC_FLUSH    2
#define DEFLATE_FULL_FLUSH    3
#define DEFLATE_FINISH        4
#define DEFLATE_BLOCK         5

#define DEFLATE_OK             0
#define DEFLATE_STREAM_END     1
#define DEFLATE_NEED_DICT      2
#define DEFLATE_ERRNO         (-1)
#define DEFLATE_STREAM_ERROR  (-2)
#define DEFLATE_DATA_ERROR    (-3)
#define DEFLATE_MEM_ERROR     (-4)
#define DEFLATE_BUF_ERROR     (-5)
#define DEFLATE_VERSION_ERROR (-6)

#define DEFLATE_NO_COMPRESSION         0
#define DEFLATE_BEST_SPEED             1
#define DEFLATE_BEST_COMPRESSION       9
#define DEFLATE_DEFAULT_COMPRESSION  (-1)

class CDeflate_private;
class KERNEL_DECL CDeflate
{
private:
	CDeflate_private* m_internal;

public:
	CDeflate();
	~CDeflate();

public:
	void SetIn(BYTE* next_in, UINT avail_in, ULONG total_in = -1);
	void SetOut(BYTE* next_out, UINT avail_out, ULONG total_out = -1);

	UINT GetAvailIn();
	UINT GetAvailOut();

	void Init(int level, int stream_size = -1);
	int Process(int flush);
	void End();
};

class CInflate_private;
class KERNEL_DECL CInflate
{
private:
	CInflate_private* m_internal;

public:
	CInflate();
	~CInflate();

public:
	void SetIn(BYTE* next_in, UINT avail_in, ULONG total_in = -1);
	void SetOut(BYTE* next_out, UINT avail_out, ULONG total_out = -1);

	UINT GetAvailIn();
	UINT GetAvailOut();

	void ClearFuncs();

	void Init();
	void Init2();
	int Process(int flush);
	void End();
};

namespace NSZip
{
	bool KERNEL_DECL Decompress(const BYTE* pSrcBuffer, const ULONG& lSrcBufferLen, BYTE* pDstBuffer, ULONG& lDstBufferLen);
}

#endif
