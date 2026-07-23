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
#if defined(GetTempPath)
#undef GetTempPath
#endif

#if defined(CreateFile)
#undef CreateFile
#endif

#if defined(CopyFile)
#undef CopyFile
#endif

#if defined(DeleteFile)
#undef DeleteFile
#endif

#if defined(GetSystemDirectory)
#undef GetSystemDirectory
#endif

#ifndef _BUILD_FILE_CROSSPLATFORM_H_
#define _BUILD_FILE_CROSSPLATFORM_H_

#include <string>
#include <string.h>
#include <stdio.h>
#include "Base64.h"

#include "../../Common/kernel_config.h"

#define U_TO_UTF8(val) NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(val.c_str(), (LONG)val.length())
#define UTF8_TO_U(val) NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)val.c_str(), (LONG)val.length())

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

std::wstring KERNEL_DECL CorrectPathW(const std::wstring& path);

namespace NSFile
{
#define WriteUtf16_WCHAR(code, p)				\
	if (code < 0x10000)								\
	*p++ = code;								\
	else											\
	{												\
	code -= 0x10000;							\
	*p++ = 0xD800 | ((code >> 10) & 0x03FF);	\
	*p++ = 0xDC00 | (code & 0x03FF);			\
}

	class CStringUtf16
	{
	public:
		BYTE* Data;
		int Length;

	public:
		CStringUtf16()
		{
			Data = NULL;
			Length = 0;
		}
		~CStringUtf16()
		{
			RELEASEARRAYOBJECTS(Data);
		}
	};

	class KERNEL_DECL CUtf8Converter
	{
	public:

		static std::wstring GetUnicodeFromCharPtr(const char* pData, LONG lCount, INT bIsUtf8 = FALSE);
		static std::wstring GetUnicodeFromCharPtr(const std::string& sParam, INT bIsUtf8 = FALSE);

		static std::wstring GetUnicodeStringFromUTF8_4bytes( BYTE* pBuffer, LONG lCount );
		static std::wstring GetUnicodeStringFromUTF8_2bytes( BYTE* pBuffer, LONG lCount );
		static std::wstring GetUnicodeStringFromUTF8( BYTE* pBuffer, LONG lCount );

		static void GetUnicodeStringFromUTF8_4bytes( BYTE* pBuffer, LONG lCount, std::wstring& sOutput );
		static void GetUnicodeStringFromUTF8_2bytes( BYTE* pBuffer, LONG lCount, std::wstring& sOutput );
		static void GetUnicodeStringFromUTF8( BYTE* pBuffer, LONG lCount, std::wstring& sOutput );

		static void GetUnicodeStringFromUTF8WithHHHH_4bytes( const BYTE* pBuffer, LONG lCount, wchar_t*& pUnicodes, LONG& lOutputCount );
		static void GetUnicodeStringFromUTF8WithHHHH_2bytes( const BYTE* pBuffer, LONG lCount, wchar_t*& pUnicodes, LONG& lOutputCount );
		static void GetUnicodeStringFromUTF8WithHHHH( const BYTE* pBuffer, LONG lCount, wchar_t*& pUnicodes, LONG& lOutputCount );

		static LONG GetUnicodeStringFromUTF8BufferSize( LONG lCount );

		static void GetUtf8StringFromUnicode_4bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM = false);
		static void GetUtf8StringFromUnicode_2bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM = false);
		static void GetUtf8StringFromUnicode(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM = false);

		static std::string GetUtf8StringFromUnicode2(const wchar_t* pUnicodes, LONG lCount, bool bIsBOM = false);
		static std::string GetUtf8StringFromUnicode(const std::wstring& sData);

		// utf16
		static void GetUtf16StringFromUnicode_4bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, int& lOutputCount, bool bIsBOM = false);
		static void GetUtf16StringFromUnicode_4bytes2(const wchar_t* pUnicodes, LONG lCount, CStringUtf16& data);
		static long GetUtf16SizeFromUnicode_4bytes(const wchar_t* pUnicodes, LONG lCount, bool bIsBOM = false);
		static long GetUtf16SizeFromUnicode(const wchar_t* pUnicodes, LONG lCount, bool bIsBOM = false);

		static std::wstring GetWStringFromUTF16(const CStringUtf16& data);
		static std::wstring GetWStringFromUTF16(const unsigned short* pUtf16, LONG lCount);

		static long CheckHHHHChar(const BYTE* pBuffer);
		static long CheckHHHHChar(const wchar_t* pBuffer);
	};

	class KERNEL_DECL CFileBinary
	{
	protected:
		FILE* m_pFile;

		long m_lFilePosition;
		long m_lFileSize;

	public:
		CFileBinary();
		virtual ~CFileBinary();

		virtual void CloseFile();

		FILE* GetFileNative();
		long GetFileSize();
		long GetFilePosition();
		unsigned long GetPosition();

		bool OpenFile(const std::wstring& sFileName, bool bRewrite = false);
		bool CreateFileW(const std::wstring& sFileName);
		bool CreateFile(const std::wstring& sFileName);

		bool CreateTempFile();
		bool SetPosition(int lFilePosition);
		bool SeekFile(int lFilePosition, int nSeekMode = 0);
		bool ReadFile(BYTE* pData, DWORD nBytesToRead);
		bool ReadFile(BYTE* pData, DWORD nBytesToRead, DWORD& dwSizeRead);
		bool WriteFile(const void* pData, DWORD nBytesCount);
		bool WriteFile(const BYTE* pData, DWORD nBytesCount);
		bool WriteFile2(const BYTE* pData, DWORD nBytesCount);
		long TellFile();
		long SizeFile();
		void WriteStringUTF8(const std::wstring& strXml, bool bIsBOM = false);

		bool WriteReserved(DWORD dwCount);
		bool WriteReserved2(DWORD dwCount);
		bool WriteReservedTo(DWORD dwPoint);
		bool SkipReservedTo(DWORD dwPoint);
		LONG GetProgress();
		bool SkipBytes(ULONG nCount);

		static bool ReadAllBytes(const std::wstring&  strFileName, BYTE** ppData, DWORD& nBytesCount);
		static bool ReadAllTextUtf8(const std::wstring&  strFileName, std::wstring& sData);
		static bool ReadAllTextUtf8A(const std::wstring&  strFileName, std::string& sData);
		static bool SaveToFile(const std::wstring&  strFileName, const std::wstring& strXml, bool bIsBOM = false);
		static bool Exists(const std::wstring&  strFileName);
		static bool Copy(const std::wstring&  strSrc, const std::wstring&  strDst);
		static bool Remove(const std::wstring& strFileName);
		static bool Move(const std::wstring&  strSrc, const std::wstring&  strDst);
		static bool Truncate(const std::wstring& sPath, size_t nNewSize);

		static void SetTempPath(const std::wstring& strTempPath);
		static std::wstring GetTempPath();
		static bool IsGlobalTempPathUse();

		static std::wstring CreateTempFileWithUniqueName(const std::wstring& strFolderPathRoot, const std::wstring& Prefix);
		static bool OpenTempFile(std::wstring *pwsName, FILE **ppFile, wchar_t *wsMode, wchar_t *wsExt, wchar_t *wsFolder, wchar_t* wsName = NULL);
		static FILE* OpenFileNative(const std::wstring& sFileName, const std::wstring& sMode);

		// returns true if everything is OK;
		// you can set ptmLastWrite / ptmLastAccess to nullptr if you are not going to use them;
		// tm_wday && tm_yday && tm_isdst is unused on windows
		static bool GetTime(const std::wstring& sFilename,
				struct tm* ptmLastWrite = nullptr,
				struct tm* ptmLastAccess = nullptr);

		// returns true if everything is OK;
		// you can set ptmLastWrite / ptmLastAccess to nullptr if you are not going to change them
		static bool SetTime(const std::wstring& sFilename,
				struct tm* ptmLastWrite = nullptr,
				struct tm* ptmLastAccess = nullptr);
	};

	class KERNEL_DECL CBase64Converter
	{
	public:
		static bool Encode(const BYTE* pDataSrc, int nLenSrc, char*& pDataDst, int& nLenDst, DWORD dwFlags = NSBase64::B64_BASE64_FLAG_NONE);
		static bool Decode(const char* pDataSrc, int nLenSrc, BYTE*& pDataDst, int& nLenDst);
	};

#ifdef _IOS
	namespace NSIOS
	{
		KERNEL_DECL std::string GetFileSystemRepresentation(const std::wstring& path);
	}
#endif
}

namespace NSFile
{
#define NS_FILE_MAX_PATH 32768
	KERNEL_DECL std::wstring GetProcessPath();
	KERNEL_DECL std::wstring GetProcessDirectory();

	// CommonFunctions
	KERNEL_DECL std::wstring GetFileExtention(const std::wstring& sPath);
	KERNEL_DECL std::wstring GetFileName(const std::wstring& sPath);
	KERNEL_DECL std::wstring GetDirectoryName(const std::wstring& sPath);
}

#endif //_BUILD_FILE_CROSSPLATFORM_H_
