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

#include <fstream>
#include <time.h>
#include "errno.h"

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
#include <wchar.h>
#include <windows.h>
#endif

#ifdef _LINUX
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#endif

#ifdef _IOS
const char* fileSystemRepresentation(const std::wstring& sFileName);
#endif

#ifdef _MAC
#include <mach-o/dyld.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

#define READ_WRITE_FULL_BUFFER_SIZE 10000000 // 10mb

#ifdef __ANDROID__
#define READ_WRITE_FULL
#define USE_LINUX_SENDFILE_INSTEAD_STREAMS

// Available since API level 21.
// iostream operation throwing exception when exceptions not enabled
#endif

#ifdef USE_LINUX_SENDFILE_INSTEAD_STREAMS
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#endif

// реализация возможности подмены определения GetTempPath
std::wstring g_overrideTmpPath = L"";

#include "File.h"

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
#include <wchar.h>
#include <windows.h>

std::wstring CorrectPathW(const std::wstring& path)
{
	int len = (int)path.length();
	if (2 > len)
		return path;

	const wchar_t* path_str = path.c_str();
	if (path_str[0] == '\\' || path_str[1] == '/')
		return path;

	// local files: '\\?\' prefix
	// server files: '\\?\UNC\' prefix <== TODO!
	int nLen = GetFullPathNameW(path_str, 0, 0, 0);
	wchar_t* pBuf = new wchar_t[(4 + nLen) * sizeof(wchar_t)];

	pBuf[0] = L'\\', pBuf[1] = L'\\',  pBuf[2] = L'?', pBuf[3] = L'\\';
	GetFullPathNameW(path_str, nLen, pBuf + 4, NULL);

	std::wstring retPath(pBuf);
	delete [] pBuf;
	return retPath;
}
#else
std::wstring CorrectPathW(const std::wstring& path)
{
	return path;
}
#endif

namespace NSFile
{
	std::wstring CUtf8Converter::GetUnicodeFromCharPtr(const char* pData, LONG lCount, INT bIsUtf8)
	{
		if (bIsUtf8)
			return GetUnicodeStringFromUTF8((BYTE*)pData, lCount);

		wchar_t* pUnicode = new wchar_t[lCount + 1];
		for (LONG i = 0; i < lCount; ++i)
			pUnicode[i] = (wchar_t)(BYTE)pData[i];

		pUnicode[lCount] = 0;

		std::wstring s(pUnicode, lCount);
		RELEASEARRAYOBJECTS(pUnicode);

		return s;
	}
	std::wstring CUtf8Converter::GetUnicodeFromCharPtr(const std::string& sParam, INT bIsUtf8)
	{
		return GetUnicodeFromCharPtr(sParam.c_str(), (LONG)sParam.length(), bIsUtf8);
	}
	LONG CUtf8Converter::GetUnicodeStringFromUTF8BufferSize(LONG lCount)
	{
		return lCount + 1;
	}
	std::wstring CUtf8Converter::GetUnicodeStringFromUTF8_4bytes( BYTE* pBuffer, LONG lCount )
	{
		std::wstring strRes;
		GetUnicodeStringFromUTF8_4bytes(pBuffer, lCount, strRes);
		return strRes;
	}
	std::wstring CUtf8Converter::GetUnicodeStringFromUTF8_2bytes( BYTE* pBuffer, LONG lCount )
	{
		std::wstring strRes;
		GetUnicodeStringFromUTF8_2bytes(pBuffer, lCount, strRes);
		return strRes;
	}

	std::wstring CUtf8Converter::GetUnicodeStringFromUTF8( BYTE* pBuffer, LONG lCount )
	{
		std::wstring strRes;
		GetUnicodeStringFromUTF8(pBuffer, lCount, strRes);
		return strRes;
	}

	void CUtf8Converter::GetUnicodeStringFromUTF8_4bytes( BYTE* pBuffer, LONG lCount, std::wstring& sOutput )
	{
		WCHAR* pUnicodeString = new WCHAR[lCount + 1];
		LONG lIndexUnicode = 0;

		LONG lIndex = 0;
		while (lIndex < lCount)
		{
			BYTE byteMain = pBuffer[lIndex];
			if (0x00 == (byteMain & 0x80))
			{
				// 1 byte
				pUnicodeString[lIndexUnicode++] = (WCHAR)byteMain;
				++lIndex;
			}
			else if (0x00 == (byteMain & 0x20))
			{
				// 2 byte
				int val = 0;
				if ((lIndex + 1) < lCount)
				{
					val = (int)(((byteMain & 0x1F) << 6) |
								(pBuffer[lIndex + 1] & 0x3F));
				}

				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 2;
			}
			else if (0x00 == (byteMain & 0x10))
			{
				// 3 byte
				int val = 0;
				if ((lIndex + 2) < lCount)
				{
					val = (int)(((byteMain & 0x0F) << 12) |
								((pBuffer[lIndex + 1] & 0x3F) << 6) |
							(pBuffer[lIndex + 2] & 0x3F));
				}

				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 3;
			}
			else if (0x00 == (byteMain & 0x0F))
			{
				// 4 byte
				int val = 0;
				if ((lIndex + 3) < lCount)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((pBuffer[lIndex + 1] & 0x3F) << 12) |
							((pBuffer[lIndex + 2] & 0x3F) << 6) |
							(pBuffer[lIndex + 3] & 0x3F));
				}

				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 4;
			}
			else if (0x00 == (byteMain & 0x08))
			{
				// 4 byte
				int val = 0;
				if ((lIndex + 3) < lCount)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((pBuffer[lIndex + 1] & 0x3F) << 12) |
							((pBuffer[lIndex + 2] & 0x3F) << 6) |
							(pBuffer[lIndex + 3] & 0x3F));
				}

				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 4;
			}
			else if (0x00 == (byteMain & 0x04))
			{
				// 5 byte
				int val = 0;
				if ((lIndex + 4) < lCount)
				{
					val = (int)(((byteMain & 0x03) << 24) |
								((pBuffer[lIndex + 1] & 0x3F) << 18) |
							((pBuffer[lIndex + 2] & 0x3F) << 12) |
							((pBuffer[lIndex + 3] & 0x3F) << 6) |
							(pBuffer[lIndex + 4] & 0x3F));
				}

				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 5;
			}
			else
			{
				// 6 byte
				int val = 0;
				if ((lIndex + 5) < lCount)
				{
					val = (int)(((byteMain & 0x01) << 30) |
								((pBuffer[lIndex + 1] & 0x3F) << 24) |
							((pBuffer[lIndex + 2] & 0x3F) << 18) |
							((pBuffer[lIndex + 3] & 0x3F) << 12) |
							((pBuffer[lIndex + 4] & 0x3F) << 6) |
							(pBuffer[lIndex + 5] & 0x3F));
				}

				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 5;
			}
		}

		pUnicodeString[lIndexUnicode] = 0;

		sOutput.append(pUnicodeString);

		delete [] pUnicodeString;
	}
	void CUtf8Converter::GetUnicodeStringFromUTF8_2bytes( BYTE* pBuffer, LONG lCount, std::wstring& sOutput )
	{
		WCHAR* pUnicodeString = new WCHAR[lCount + 1];
		WCHAR* pStart = pUnicodeString;

		LONG lIndex = 0;
		while (lIndex < lCount)
		{
			BYTE byteMain = pBuffer[lIndex];
			if (0x00 == (byteMain & 0x80))
			{
				// 1 byte
				*pUnicodeString++ = (WCHAR)byteMain;
				++lIndex;
			}
			else if (0x00 == (byteMain & 0x20))
			{
				// 2 byte
				int val = 0;
				if ((lIndex + 1) < lCount)
				{
					val = (int)(((byteMain & 0x1F) << 6) |
								(pBuffer[lIndex + 1] & 0x3F));
				}

				*pUnicodeString++ = (WCHAR)(val);
				lIndex += 2;
			}
			else if (0x00 == (byteMain & 0x10))
			{
				// 3 byte
				int val = 0;
				if ((lIndex + 2) < lCount)
				{
					val = (int)(((byteMain & 0x0F) << 12) |
								((pBuffer[lIndex + 1] & 0x3F) << 6) |
							(pBuffer[lIndex + 2] & 0x3F));
				}

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 3;
			}
			else if (0x00 == (byteMain & 0x0F))
			{
				// 4 byte
				int val = 0;
				if ((lIndex + 3) < lCount)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((pBuffer[lIndex + 1] & 0x3F) << 12) |
							((pBuffer[lIndex + 2] & 0x3F) << 6) |
							(pBuffer[lIndex + 3] & 0x3F));
				}

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 4;
			}
			else if (0x00 == (byteMain & 0x08))
			{
				// 4 byte
				int val = 0;
				if ((lIndex + 3) < lCount)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((pBuffer[lIndex + 1] & 0x3F) << 12) |
							((pBuffer[lIndex + 2] & 0x3F) << 6) |
							(pBuffer[lIndex + 3] & 0x3F));
				}

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 4;
			}
			else if (0x00 == (byteMain & 0x04))
			{
				// 5 byte
				int val = 0;
				if ((lIndex + 4) < lCount)
				{
					val = (int)(((byteMain & 0x03) << 24) |
								((pBuffer[lIndex + 1] & 0x3F) << 18) |
							((pBuffer[lIndex + 2] & 0x3F) << 12) |
							((pBuffer[lIndex + 3] & 0x3F) << 6) |
							(pBuffer[lIndex + 4] & 0x3F));
				}

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 5;
			}
			else
			{
				// 6 byte
				int val = 0;
				if ((lIndex + 5) < lCount)
				{
					val = (int)(((byteMain & 0x01) << 30) |
								((pBuffer[lIndex + 1] & 0x3F) << 24) |
							((pBuffer[lIndex + 2] & 0x3F) << 18) |
							((pBuffer[lIndex + 3] & 0x3F) << 12) |
							((pBuffer[lIndex + 4] & 0x3F) << 6) |
							(pBuffer[lIndex + 5] & 0x3F));
				}

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 5;
			}
		}

		*pUnicodeString++ = 0;

		sOutput.append(pStart);

		delete [] pStart;
	}
	void CUtf8Converter::GetUnicodeStringFromUTF8( BYTE* pBuffer, LONG lCount, std::wstring& sOutput )
	{
		if (sizeof(WCHAR) == 2)
			GetUnicodeStringFromUTF8_2bytes(pBuffer, lCount, sOutput);
		else
			GetUnicodeStringFromUTF8_4bytes(pBuffer, lCount, sOutput);
	}

#define CHECK_HHHH(pBuffer) \
	wchar_t code = 0; \
	if('_' == pBuffer[0] && 'x' == pBuffer[1] && 0 != pBuffer[2] && 0 != pBuffer[3] && 0 != pBuffer[4] && 0 != pBuffer[5]  && '_' == pBuffer[6]) \
	{ \
	int i = 2; \
	for(; i < 6; ++i) \
	{ \
	code *= 16; \
	if('0' <= pBuffer[i] && pBuffer[i] <= '9') \
	{ \
	code += pBuffer[i] - '0'; \
} \
	else if('A' <= pBuffer[i] && pBuffer[i] <= 'F') \
	{ \
	code += pBuffer[i] - 'A' + 10; \
} \
	else if('a' <= pBuffer[i] && pBuffer[i] <= 'f') \
	{ \
	code += pBuffer[i] - 'a' + 10; \
} \
	else \
	{ \
	break; \
} \
} \
	if(i == 6) \
	{ \
	if(0x005F == code) \
	{ \
	code = '_'; \
} \
	return code; \
} \
} \
	return -1;

	long CUtf8Converter::CheckHHHHChar(const BYTE* pBuffer)
	{
		CHECK_HHHH(pBuffer);
	}
	long CUtf8Converter::CheckHHHHChar(const wchar_t* pBuffer)
	{
		CHECK_HHHH(pBuffer);
	}

	void CUtf8Converter::GetUnicodeStringFromUTF8WithHHHH_4bytes( const BYTE* pBuffer, LONG lCount, wchar_t*& pUnicodes, LONG& lOutputCount )
	{
		if (NULL == pUnicodes)
		{
			pUnicodes = new wchar_t[GetUnicodeStringFromUTF8BufferSize(lCount)];
		}
		WCHAR* pUnicodeString = pUnicodes;
		LONG lIndexUnicode = 0;

		LONG lIndex = 0;
		while (lIndex < lCount)
		{
			BYTE byteMain = pBuffer[lIndex];
			if (0x00 == (byteMain & 0x80))
			{
				// 1 byte
				long code = CheckHHHHChar(pBuffer + lIndex);
				if(code < 0)
				{
					pUnicodeString[lIndexUnicode++] = (WCHAR)byteMain;
					++lIndex;
				}
				else
				{
					pUnicodeString[lIndexUnicode++] = (WCHAR)code;
					lIndex += 7;
				}
			}
			else if (0x00 == (byteMain & 0x20))
			{
				// 2 byte
				int val = (int)(((byteMain & 0x1F) << 6) |
								(pBuffer[lIndex + 1] & 0x3F));
				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 2;
			}
			else if (0x00 == (byteMain & 0x10))
			{
				// 3 byte
				int val = (int)(((byteMain & 0x0F) << 12) |
								((pBuffer[lIndex + 1] & 0x3F) << 6) |
						(pBuffer[lIndex + 2] & 0x3F));
				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 3;
			}
			else if (0x00 == (byteMain & 0x0F))
			{
				// 4 byte
				int val = (int)(((byteMain & 0x07) << 18) |
								((pBuffer[lIndex + 1] & 0x3F) << 12) |
						((pBuffer[lIndex + 2] & 0x3F) << 6) |
						(pBuffer[lIndex + 3] & 0x3F));
				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 4;
			}
			else if (0x00 == (byteMain & 0x08))
			{
				// 4 byte
				int val = (int)(((byteMain & 0x07) << 18) |
								((pBuffer[lIndex + 1] & 0x3F) << 12) |
						((pBuffer[lIndex + 2] & 0x3F) << 6) |
						(pBuffer[lIndex + 3] & 0x3F));
				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 4;
			}
			else if (0x00 == (byteMain & 0x04))
			{
				// 5 byte
				int val = (int)(((byteMain & 0x03) << 24) |
								((pBuffer[lIndex + 1] & 0x3F) << 18) |
						((pBuffer[lIndex + 2] & 0x3F) << 12) |
						((pBuffer[lIndex + 3] & 0x3F) << 6) |
						(pBuffer[lIndex + 4] & 0x3F));
				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 5;
			}
			else
			{
				// 6 byte
				int val = (int)(((byteMain & 0x01) << 30) |
								((pBuffer[lIndex + 1] & 0x3F) << 24) |
						((pBuffer[lIndex + 2] & 0x3F) << 18) |
						((pBuffer[lIndex + 3] & 0x3F) << 12) |
						((pBuffer[lIndex + 4] & 0x3F) << 6) |
						(pBuffer[lIndex + 5] & 0x3F));
				pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
				lIndex += 5;
			}
		}

		pUnicodeString[lIndexUnicode] = 0;
		lOutputCount = lIndexUnicode;
	}
	void CUtf8Converter::GetUnicodeStringFromUTF8WithHHHH_2bytes( const BYTE* pBuffer, LONG lCount, wchar_t*& pUnicodes, LONG& lOutputCount )
	{
		if (NULL == pUnicodes)
		{
			pUnicodes = new wchar_t[GetUnicodeStringFromUTF8BufferSize(lCount)];
		}
		WCHAR* pUnicodeString = pUnicodes;
		WCHAR* pStart = pUnicodeString;
		LONG lIndex = 0;
		while (lIndex < lCount)
		{
			BYTE byteMain = pBuffer[lIndex];
			if (0x00 == (byteMain & 0x80))
			{
				// 1 byte
				long code = CheckHHHHChar(pBuffer + lIndex);
				if(code < 0)
				{
					*pUnicodeString++ = (WCHAR)byteMain;
					++lIndex;
				}
				else
				{
					*pUnicodeString++ = (WCHAR)code;
					lIndex += 7;
				}

			}
			else if (0x00 == (byteMain & 0x20))
			{
				// 2 byte
				int val = (int)(((byteMain & 0x1F) << 6) |
								(pBuffer[lIndex + 1] & 0x3F));
				*pUnicodeString++ = (WCHAR)(val);
				lIndex += 2;
			}
			else if (0x00 == (byteMain & 0x10))
			{
				// 3 byte
				int val = (int)(((byteMain & 0x0F) << 12) |
								((pBuffer[lIndex + 1] & 0x3F) << 6) |
						(pBuffer[lIndex + 2] & 0x3F));

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 3;
			}
			else if (0x00 == (byteMain & 0x0F))
			{
				// 4 byte
				int val = (int)(((byteMain & 0x07) << 18) |
								((pBuffer[lIndex + 1] & 0x3F) << 12) |
						((pBuffer[lIndex + 2] & 0x3F) << 6) |
						(pBuffer[lIndex + 3] & 0x3F));

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 4;
			}
			else if (0x00 == (byteMain & 0x08))
			{
				// 4 byte
				int val = (int)(((byteMain & 0x07) << 18) |
								((pBuffer[lIndex + 1] & 0x3F) << 12) |
						((pBuffer[lIndex + 2] & 0x3F) << 6) |
						(pBuffer[lIndex + 3] & 0x3F));

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 4;
			}
			else if (0x00 == (byteMain & 0x04))
			{
				// 5 byte
				int val = (int)(((byteMain & 0x03) << 24) |
								((pBuffer[lIndex + 1] & 0x3F) << 18) |
						((pBuffer[lIndex + 2] & 0x3F) << 12) |
						((pBuffer[lIndex + 3] & 0x3F) << 6) |
						(pBuffer[lIndex + 4] & 0x3F));

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 5;
			}
			else
			{
				// 6 byte
				int val = (int)(((byteMain & 0x01) << 30) |
								((pBuffer[lIndex + 1] & 0x3F) << 24) |
						((pBuffer[lIndex + 2] & 0x3F) << 18) |
						((pBuffer[lIndex + 3] & 0x3F) << 12) |
						((pBuffer[lIndex + 4] & 0x3F) << 6) |
						(pBuffer[lIndex + 5] & 0x3F));

				WriteUtf16_WCHAR(val, pUnicodeString);
				lIndex += 5;
			}
		}

		lOutputCount = pUnicodeString - pStart;
		*pUnicodeString++ = 0;
	}
	void CUtf8Converter::GetUnicodeStringFromUTF8WithHHHH( const BYTE* pBuffer, LONG lCount, wchar_t*& pUnicodes, LONG& lOutputCount )
	{
		if (sizeof(WCHAR) == 2)
			return GetUnicodeStringFromUTF8WithHHHH_2bytes(pBuffer, lCount, pUnicodes, lOutputCount);
		return GetUnicodeStringFromUTF8WithHHHH_4bytes(pBuffer, lCount, pUnicodes, lOutputCount);
	}

	void CUtf8Converter::GetUtf8StringFromUnicode_4bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM)
	{
		if (NULL == pData)
		{
			pData = new BYTE[6 * lCount + 3 + 1 ];
		}

		BYTE* pCodesCur = pData;
		if (bIsBOM)
		{
			pCodesCur[0] = 0xEF;
			pCodesCur[1] = 0xBB;
			pCodesCur[2] = 0xBF;
			pCodesCur += 3;
		}

		const wchar_t* pEnd = pUnicodes + lCount;
		const wchar_t* pCur = pUnicodes;

		while (pCur < pEnd)
		{
			unsigned int code = (unsigned int)*pCur++;

			if (code < 0x80)
			{
				*pCodesCur++ = (BYTE)code;
			}
			else if (code < 0x0800)
			{
				*pCodesCur++ = 0xC0 | (code >> 6);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
			else if (code < 0x10000)
			{
				*pCodesCur++ = 0xE0 | (code >> 12);
				*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
			else if (code < 0x1FFFFF)
			{
				*pCodesCur++ = 0xF0 | (code >> 18);
				*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
				*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
			else if (code < 0x3FFFFFF)
			{
				*pCodesCur++ = 0xF8 | (code >> 24);
				*pCodesCur++ = 0x80 | (code >> 18 & 0x3F);
				*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
				*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
			else if (code < 0x7FFFFFFF)
			{
				*pCodesCur++ = 0xFC | (code >> 30);
				*pCodesCur++ = 0x80 | (code >> 24 & 0x3F);
				*pCodesCur++ = 0x80 | (code >> 18 & 0x3F);
				*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
				*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
		}

		lOutputCount = (LONG)(pCodesCur - pData);
		*pCodesCur++ = 0;
	}

	void CUtf8Converter::GetUtf8StringFromUnicode_2bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM)
	{
		if (NULL == pData)
		{
			pData = new BYTE[6 * lCount + 3 + 1];
		}

		BYTE* pCodesCur = pData;
		if (bIsBOM)
		{
			pCodesCur[0] = 0xEF;
			pCodesCur[1] = 0xBB;
			pCodesCur[2] = 0xBF;
			pCodesCur += 3;
		}

		const wchar_t* pEnd = pUnicodes + lCount;
		const wchar_t* pCur = pUnicodes;

		while (pCur < pEnd)
		{
			unsigned int code = (unsigned int)*pCur++;
			if (code >= 0xD800 && code <= 0xDFFF && pCur < pEnd)
			{
				code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & *pCur++));
			}

			if (code < 0x80)
			{
				*pCodesCur++ = (BYTE)code;
			}
			else if (code < 0x0800)
			{
				*pCodesCur++ = 0xC0 | (code >> 6);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
			else if (code < 0x10000)
			{
				*pCodesCur++ = 0xE0 | (code >> 12);
				*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
			else if (code < 0x1FFFFF)
			{
				*pCodesCur++ = 0xF0 | (code >> 18);
				*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
				*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
			else if (code < 0x3FFFFFF)
			{
				*pCodesCur++ = 0xF8 | (code >> 24);
				*pCodesCur++ = 0x80 | ((code >> 18) & 0x3F);
				*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
				*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
			else if (code < 0x7FFFFFFF)
			{
				*pCodesCur++ = 0xFC | (code >> 30);
				*pCodesCur++ = 0x80 | ((code >> 24) & 0x3F);
				*pCodesCur++ = 0x80 | ((code >> 18) & 0x3F);
				*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
				*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
				*pCodesCur++ = 0x80 | (code & 0x3F);
			}
		}

		lOutputCount = (LONG)(pCodesCur - pData);
		*pCodesCur++ = 0;
	}

	void CUtf8Converter::GetUtf8StringFromUnicode(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM)
	{
		if (NULL == pUnicodes || 0 == lCount)
		{
			pData = NULL;
			lOutputCount = 0;
			return;
		}

		if (sizeof(WCHAR) == 2)
			return GetUtf8StringFromUnicode_2bytes(pUnicodes, lCount, pData, lOutputCount, bIsBOM);
		return GetUtf8StringFromUnicode_4bytes(pUnicodes, lCount, pData, lOutputCount, bIsBOM);
	}

	std::string CUtf8Converter::GetUtf8StringFromUnicode2(const wchar_t* pUnicodes, LONG lCount, bool bIsBOM)
	{
		if (NULL == pUnicodes || 0 == lCount)
			return "";

		BYTE* pData = NULL;
		LONG lLen = 0;

		GetUtf8StringFromUnicode(pUnicodes, lCount, pData, lLen, bIsBOM);

		std::string s((char*)pData, lLen);

		RELEASEARRAYOBJECTS(pData);
		return s;
	}

	std::string CUtf8Converter::GetUtf8StringFromUnicode(const std::wstring& sData)
	{
		return GetUtf8StringFromUnicode2(sData.c_str(), (LONG)sData.length());
	}

	// utf16
	void CUtf8Converter::GetUtf16StringFromUnicode_4bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, int& lOutputCount, bool bIsBOM)
	{
		if (NULL == pData)
		{
			pData = new BYTE[4 * lCount + 3 + 2];
		}

		BYTE* pCodesCur = pData;
		if (bIsBOM)
		{
			pCodesCur[0] = 0xEF;
			pCodesCur[1] = 0xBB;
			pCodesCur[2] = 0xBF;
			pCodesCur += 3;
		}

		const wchar_t* pEnd = pUnicodes + lCount;
		const wchar_t* pCur = pUnicodes;

		while (pCur < pEnd)
		{
			unsigned int code = (unsigned int)*pCur++;

			if (code <= 0xFFFF)
			{
				USHORT usCode = (USHORT)(code & 0xFFFF);
				memcpy(pCodesCur, &usCode, 2);
				pCodesCur += 2;
			}
			else
			{
				code -= 0x10000;
				code &= 0xFFFFF;

				USHORT us1 = 0xD800 | ((code >> 10) & 0x03FF);
				USHORT us2 = 0xDC00 | (code & 0x03FF);

				memcpy(pCodesCur, &us1, 2);
				pCodesCur += 2;

				memcpy(pCodesCur, &us2, 2);
				pCodesCur += 2;
			}
		}

		lOutputCount = (LONG)(pCodesCur - pData);
		*pCodesCur++ = 0;
		*pCodesCur++ = 0;
	}

	void CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(const wchar_t* pUnicodes, LONG lCount, CStringUtf16& data)
	{
		GetUtf16StringFromUnicode_4bytes(pUnicodes, lCount, data.Data, data.Length);
	}

	std::wstring CUtf8Converter::GetWStringFromUTF16(const CStringUtf16& data)
	{
		if (0 == data.Length)
			return L"";

		if (sizeof(wchar_t) == 2)
			return std::wstring((wchar_t*)data.Data, data.Length / 2);

		int nCount = data.Length / 2;
		USHORT* pShort = (USHORT*)data.Data;

		wchar_t* pWChar = new wchar_t[nCount + 1];
		wchar_t* pWCurrent = pWChar;

		int nCurrent = 0;
		while (nCurrent < nCount)
		{
			if (*pShort < 0xD800 || *pShort > 0xDBFF)
			{
				*pWCurrent = (wchar_t)(*pShort);
				++pShort;
				++nCurrent;
			}
			else
			{
				*pWCurrent = (wchar_t)(((((pShort[0] - 0xD800) & 0x03FF) << 10) | ((pShort[1] - 0xDC00) & 0x03FF)) + 0x10000);
				pShort += 2;
				nCurrent += 2;
			}
			++pWCurrent;
		}

		std::wstring sRet(pWChar, pWCurrent - pWChar);

		RELEASEARRAYOBJECTS(pWChar);
		return sRet;
	}
	std::wstring CUtf8Converter::GetWStringFromUTF16(const unsigned short* pUtf16, LONG lCount)
	{
		CStringUtf16 oString;
		oString.Data   = (BYTE*)pUtf16;
		oString.Length = lCount * 2;
		std::wstring wsResult = GetWStringFromUTF16(oString);
		oString.Data = NULL;
		return wsResult;
	}
}

namespace NSFile
{
	CFileBinary::CFileBinary()
	{
		m_pFile = NULL;
		m_lFilePosition = 0;
		m_lFileSize = 0;
	}
	CFileBinary::~CFileBinary()
	{
		CloseFile();
	}

	void CFileBinary::CloseFile()
	{
		m_lFilePosition = 0;
		m_lFileSize = 0;

		if (m_pFile != NULL)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	FILE* CFileBinary::GetFileNative()
	{
		return m_pFile;
	}
	long CFileBinary::GetFileSize()
	{
		return m_lFileSize;
	}
	long CFileBinary::GetFilePosition()
	{
		m_lFilePosition = ftell(m_pFile);
		
		return m_lFilePosition;
	}	
	unsigned long CFileBinary::GetPosition()
	{
		m_lFilePosition = ftell(m_pFile);

		return (unsigned long)m_lFilePosition;
	}

#ifdef _IOS

	bool CFileBinary::OpenFile(const std::wstring& sFileName, bool bRewrite)
	{
		m_pFile = fopen(fileSystemRepresentation(sFileName), bRewrite ? "rb+" : "rb");

		if (NULL == m_pFile) {
#if DEBUG
			//    printf ("NSFile::OpenFile - error open file : %s\n",strerror(errno));
#endif
			return false;
		}

		fseek(m_pFile, 0, SEEK_END);
		m_lFileSize = ftell(m_pFile);
		fseek(m_pFile, 0, SEEK_SET);

		m_lFilePosition = 0;

		if (0 < sFileName.length())
		{
			if (((wchar_t)'/') == sFileName.c_str()[sFileName.length() - 1])
				m_lFileSize = 0x7FFFFFFF;
		}

		unsigned int err = 0x7FFFFFFF;
		unsigned int cur = (unsigned int)m_lFileSize;
		if (err == cur)
		{
			CloseFile();
			return false;
		}

		return true;
	}

	bool CFileBinary::CreateFileW(const std::wstring& sFileName)
	{
		m_pFile = fopen(fileSystemRepresentation(sFileName), "wb");

		if (NULL == m_pFile) {
#if DEBUG
			//    printf ("NSFile::CreateFileW - error create file : %s\n",strerror(errno));
#endif
			return false;
		}

		m_lFilePosition = 0;
		return true;
	}

#else

	bool CFileBinary::OpenFile(const std::wstring& sFileName, bool bRewrite)
	{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		if ( NULL == (m_pFile = _wfsopen( sFileName.c_str(), bRewrite ? L"rb+" : L"rb", _SH_DENYNO)))
			return false;
#else
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		CUtf8Converter::GetUtf8StringFromUnicode(sFileName.c_str(), sFileName.length(), pUtf8, lLen, false);

		struct stat st;
		if ((0 == stat((char*)pUtf8, &st)) && S_ISDIR(st.st_mode))
			return false;

		m_pFile = fopen((char*)pUtf8, bRewrite ? "rb+" : "rb");

		delete [] pUtf8;
#endif
		if (NULL == m_pFile)
			return false;

		fseek(m_pFile, 0, SEEK_END);
		m_lFileSize = ftell(m_pFile);
		fseek(m_pFile, 0, SEEK_SET);

		m_lFilePosition = 0;

		if (0 < sFileName.length())
		{
			if (((wchar_t)'/') == sFileName.c_str()[sFileName.length() - 1])
				m_lFileSize = 0x7FFFFFFF;
		}

		unsigned int err = 0x7FFFFFFF;
		unsigned int cur = (unsigned int)m_lFileSize;
		if (err == cur)
		{
			CloseFile();
			return false;
		}

		return true;
	}

	bool CFileBinary::CreateFileW(const std::wstring& sFileName)
	{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		if ( 0 != _wfopen_s(&m_pFile, sFileName.c_str(), L"wb"))
			return false;
#else
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		CUtf8Converter::GetUtf8StringFromUnicode(sFileName.c_str(), sFileName.length(), pUtf8, lLen, false);
		m_pFile = fopen((char*)pUtf8, "wb");
		delete [] pUtf8;
#endif
		if (NULL == m_pFile)
			return false;

		m_lFilePosition = 0;
		return true;
	}

#endif

	bool CFileBinary::CreateFile(const std::wstring& sFileName)
	{
		return CreateFileW(sFileName);
	}

	bool CFileBinary::CreateTempFile()
	{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		if (0 != tmpfile_s(&m_pFile))
			return false;
#else
		m_pFile = tmpfile();
		if (NULL == m_pFile)
			return false;
#endif
		m_lFilePosition = 0;
		return true;
	}
	bool CFileBinary::SeekFile(int lFilePosition, int nSeekMode)
	{
		if (!m_pFile)
			return false;

		m_lFilePosition = fseek(m_pFile, lFilePosition, nSeekMode);
		return true;
	}
	bool CFileBinary::SetPosition(int lFilePosition)
	{
		return SeekFile(lFilePosition, SEEK_SET);
	}
	bool CFileBinary::ReadFile(BYTE* pData, DWORD nBytesToRead)
	{
		if (!m_pFile)
			return false;

		DWORD dwSizeRead = (DWORD)fread((void*)pData, 1, nBytesToRead, m_pFile);
		
		return (dwSizeRead == nBytesToRead) ? true : false;
	}
	bool CFileBinary::ReadFile(BYTE* pData, DWORD nBytesToRead, DWORD& dwSizeRead)
	{
		if (!m_pFile)
			return false;

		dwSizeRead = (DWORD)fread((void*)pData, 1, nBytesToRead, m_pFile);
		
		return true;
	}
	bool CFileBinary::WriteFile(const void* pData, DWORD nBytesCount)
	{
		if (!m_pFile)
			return false;

		size_t nCountWrite = fwrite(pData, 1, nBytesCount, m_pFile);
		return true;
	}
	bool CFileBinary::WriteFile(const BYTE* pData, DWORD nBytesCount)
	{
		return WriteFile((const void*)pData, nBytesCount);
	}
	bool CFileBinary::WriteFile2(const BYTE* pData, DWORD nBytesToWrite)
	{
		if (!m_pFile)
			return false;

		BYTE* mem = new BYTE[nBytesToWrite];
		memcpy(mem, pData, nBytesToWrite);

		for (size_t index = 0; index < nBytesToWrite / 2; ++index)
		{
			BYTE temp = mem[index];
			mem[index] = mem[nBytesToWrite - index - 1];
			mem[nBytesToWrite - index - 1] = temp;
		}

		bool bRes = WriteFile(mem, nBytesToWrite);
		RELEASEARRAYOBJECTS(mem);
		return bRes;
	}
	long CFileBinary::TellFile()
	{
		if (!m_pFile)
			return 0;

		return ftell(m_pFile);
	}
	long CFileBinary::SizeFile()
	{
		if (!m_pFile)
			return 0;

		long lPos = TellFile();
		fseek(m_pFile, 0, SEEK_END);
		m_lFileSize = ftell(m_pFile);
		fseek(m_pFile, lPos, SEEK_SET);

		return m_lFileSize;
	}
	void CFileBinary::WriteStringUTF8(const std::wstring& strXml, bool bIsBOM)
	{
		BYTE* pData = NULL;
		LONG lLen = 0;

		CUtf8Converter::GetUtf8StringFromUnicode(strXml.c_str(), (LONG)strXml.length(), pData, lLen, bIsBOM);

		WriteFile(pData, lLen);

		RELEASEARRAYOBJECTS(pData);
	}

	bool CFileBinary::WriteReserved(DWORD dwCount)
	{
		BYTE* buf = new BYTE[dwCount];
		memset(buf, 0, (size_t)dwCount);
		bool hr = WriteFile(buf, dwCount);
		RELEASEARRAYOBJECTS(buf);
		return hr;
	}
	bool CFileBinary::WriteReserved2(DWORD dwCount)
	{
		BYTE* buf = new BYTE[dwCount];
		memset(buf, 0xFF, (size_t)dwCount);
		bool hr = WriteFile(buf, dwCount);
		RELEASEARRAYOBJECTS(buf);
		return hr;
	}
	bool CFileBinary::WriteReservedTo(DWORD dwPoint)
	{
		if ((DWORD)m_lFilePosition >= dwPoint)
			return S_OK;

		DWORD dwCount = dwPoint - (DWORD)m_lFilePosition;
		BYTE* buf = new BYTE[dwCount];
		memset(buf, 0, (size_t)dwCount);
		bool hr = WriteFile(buf, dwCount);
		RELEASEARRAYOBJECTS(buf);
		return hr;
	}
	bool CFileBinary::SkipReservedTo(DWORD dwPoint)
	{
		if ((DWORD)m_lFilePosition >= dwPoint)
			return S_OK;

		DWORD dwCount = dwPoint - (DWORD)m_lFilePosition;
		return SkipBytes(dwCount);
	}

	LONG CFileBinary::GetProgress()
	{
		if (0 >= m_lFileSize)
			return -1;

		double dVal = (double)(100 * m_lFilePosition);
		LONG lProgress = (LONG)(dVal / m_lFileSize);
		return lProgress;
	}

	bool CFileBinary::SkipBytes(ULONG nCount)
	{
		return SeekFile(m_lFilePosition + nCount);
	}

	bool CFileBinary::ReadAllBytes(const std::wstring&  strFileName, BYTE** ppData, DWORD& nBytesCount)
	{
		*ppData = NULL;
		nBytesCount = 0;
		bool bRes = false;
		CFileBinary oFileBinary;
		if (oFileBinary.OpenFile(strFileName))
		{
			long nFileSize = oFileBinary.GetFileSize();
			BYTE* pData = new BYTE[nFileSize];
			DWORD dwSizeRead;
			if (oFileBinary.ReadFile(pData, nFileSize, dwSizeRead))
			{
				oFileBinary.CloseFile();
				*ppData = pData;
				nBytesCount = dwSizeRead;
				bRes = true;
			}
			else
				RELEASEARRAYOBJECTS(pData);
		}
		return bRes;
	}
	bool CFileBinary::ReadAllTextUtf8(const std::wstring&  strFileName, std::wstring& sData)
	{
		bool bRes = false;
		BYTE* pData = NULL;
		DWORD nDataSize;
		if (CFileBinary::ReadAllBytes(strFileName, &pData, nDataSize))
		{
			//remove BOM if exist
			BYTE* pDataStart = pData;
			DWORD nBOMSize = 3;
			if (nDataSize > nBOMSize && 0xef == pDataStart[0] && 0xbb == pDataStart[1] && 0xbf == pDataStart[2])
			{
				pDataStart += nBOMSize;
				nDataSize -= nBOMSize;
			}
			sData = CUtf8Converter::GetUnicodeStringFromUTF8(pDataStart, nDataSize);
			RELEASEARRAYOBJECTS(pData);
			bRes = true;
		}
		return bRes;
	}
	bool CFileBinary::ReadAllTextUtf8A(const std::wstring&  strFileName, std::string& sData)
	{
		bool bRes = false;
		BYTE* pData = NULL;
		DWORD nDataSize;
		if (CFileBinary::ReadAllBytes(strFileName, &pData, nDataSize))
		{
			//remove BOM if exist
			BYTE* pDataStart = pData;
			DWORD nBOMSize = 3;
			if (nDataSize > nBOMSize && 0xef == pDataStart[0] && 0xbb == pDataStart[1] && 0xbf == pDataStart[2])
			{
				pDataStart += nBOMSize;
				nDataSize -= nBOMSize;
			}
			sData = std::string((char*)pDataStart, nDataSize);
			RELEASEARRAYOBJECTS(pData);
			bRes = true;
		}
		return bRes;
	}
	bool CFileBinary::SaveToFile(const std::wstring&  strFileName, const std::wstring& strXml, bool bIsBOM)
	{
		CFileBinary oFile;
		oFile.CreateFileW(strFileName);
		oFile.WriteStringUTF8(strXml, bIsBOM);
		oFile.CloseFile();
		return true;
	}
	bool CFileBinary::Exists(const std::wstring&  strFileName)
	{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		FILE* pFile = NULL;
		if ( NULL == (pFile = _wfsopen( strFileName.c_str(), L"rb", _SH_DENYNO)))
			return false;
#else
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		CUtf8Converter::GetUtf8StringFromUnicode(strFileName.c_str(), strFileName.length(), pUtf8, lLen, false);
		FILE* pFile = fopen((char*)pUtf8, "rb");
		delete [] pUtf8;
#endif
		if (NULL != pFile)
		{
			fclose(pFile);
			return true;
		}
		else
			return false;
	}
	bool CFileBinary::Copy(const std::wstring&  strSrc, const std::wstring& strDst)
	{
		if (strSrc == strDst)
			return true;

#ifdef READ_WRITE_FULL
		BYTE* pFileData = NULL;
		DWORD dwChunkSize = READ_WRITE_FULL_BUFFER_SIZE;
		CFileBinary oFileSrc;
		CFileBinary oFileDst;
		if (oFileSrc.OpenFile(strSrc) && oFileDst.CreateFileW(strDst))
		{
			DWORD dwFileSrcSize = (DWORD)oFileSrc.GetFileSize();
			if (dwChunkSize > dwFileSrcSize)
				dwChunkSize = dwFileSrcSize;

			BYTE* pTempBuffer = new BYTE[dwChunkSize];
			DWORD dwProcessedBytes = 0;
			while (dwFileSrcSize != 0)
			{
				oFileSrc.ReadFile(pTempBuffer, dwChunkSize, dwProcessedBytes);
				if (dwProcessedBytes != dwChunkSize)
					break;

				if (!oFileDst.WriteFile(pTempBuffer, dwChunkSize))
					break;

				dwFileSrcSize -= dwChunkSize;
				if (dwChunkSize > dwFileSrcSize)
					dwChunkSize = dwFileSrcSize;
			}

			oFileSrc.CloseFile();
			oFileDst.CloseFile();

			RELEASEARRAYOBJECTS(pTempBuffer);

			if (0 != dwFileSrcSize)
				Remove(strDst);
			else
				return true;
		}
#endif

#if !defined(_WIN32) && !defined(_WIN32_WCE) && !defined(_WIN64)
		std::string strSrcA = U_TO_UTF8(strSrc);
		std::string strDstA = U_TO_UTF8(strDst);
#endif

#ifdef USE_LINUX_SENDFILE_INSTEAD_STREAMS
		int src = open(strSrcA.c_str(), O_RDONLY);
		int dst = open(strDstA.c_str(), O_WRONLY | O_CREAT);

		// struct required, rationale: function stat() exists also
		struct stat stat_source;
		fstat(src, &stat_source);

		if (stat_source.st_size > 0x7FFFFFFF)
			return false;

		off_t offset = 0;
		long long file_size = stat_source.st_size;
		long long file_size_tmp = file_size;
		long long read_size = 0;
		long long read_size_marker = (long long)sendfile(dst, src, &offset, file_size_tmp);
		while (-1 != read_size_marker)
		{
			if (read_size == file_size)
				break;

			file_size_tmp -= read_size_marker;
			read_size += read_size_marker;

			if (read_size != file_size)
				read_size_marker = (long long)sendfile(dst, src, &offset, file_size_tmp);
		}

		close(src);
		close(dst);
		return (-1 != read_size_marker) ? true : false;
#else
		std::ifstream src;
		std::ofstream dst;

		int nLenBuffer = 1024 * 1024; // 10
		CFileBinary oFile;
		if (oFile.OpenFile(strSrc))
		{
			int nFileSize = (int)oFile.GetFileSize();
			if (nFileSize < nLenBuffer)
				nLenBuffer = nFileSize;

			oFile.CloseFile();
		}
		else
		{
#ifdef _WIN32
			return (0 != ::CopyFileW(strSrc.c_str(), strDst.c_str(), 1));
#endif
		}

		char* pBuffer_in = NULL;
		char* pBuffer_out = NULL;

		if (nLenBuffer > 0)
		{
			pBuffer_in = new char[nLenBuffer];
			pBuffer_out = new char[nLenBuffer];

			src.rdbuf()->pubsetbuf(pBuffer_in, nLenBuffer);
			dst.rdbuf()->pubsetbuf(pBuffer_out, nLenBuffer);
		}

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		src.open(strSrc.c_str(), std::ios::binary);
        if (src.is_open())
            dst.open(strDst.c_str(), std::ios::binary);
#else
		src.open(strSrcA.c_str(), std::ios::binary);
        if (src.is_open())
            dst.open(strDstA.c_str(), std::ios::binary);
#endif

		bool bRet = false;

		if (src.is_open() && dst.is_open())
		{
			dst << src.rdbuf();
			src.close();
			dst.close();

			bRet = true;
		}
		RELEASEARRAYOBJECTS(pBuffer_in);
		RELEASEARRAYOBJECTS(pBuffer_out);
		return bRet;
#endif
	}

	bool CFileBinary::Remove(const std::wstring& strFileName)
	{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		int nRes = _wremove(strFileName.c_str());
#else
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		CUtf8Converter::GetUtf8StringFromUnicode(strFileName.c_str(), strFileName.length(), pUtf8, lLen, false);
		int nRes = std::remove((char*)pUtf8);
		delete [] pUtf8;
#endif
		return 0 == nRes;
	}
	bool CFileBinary::Move(const std::wstring&  strSrc, const std::wstring& strDst)
	{
		if (strSrc == strDst)
			return true;
		if (Copy(strSrc, strDst))
			if (Remove(strSrc))
				return true;
		return false;
	}

	bool CFileBinary::Truncate(const std::wstring& sPath, size_t nNewSize)
	{
		bool bIsSuccess = false;

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		HANDLE hFile = ::CreateFileW( sPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
									  NULL, OPEN_EXISTING,
									  FILE_ATTRIBUTE_NORMAL, NULL );
		if ( hFile == INVALID_HANDLE_VALUE )
		{
			return bIsSuccess;
		}

		LARGE_INTEGER Size = { 0 };

		if ( GetFileSizeEx( hFile, &Size ) )
		{
			LARGE_INTEGER Distance = { 0 };
			// Negative values move the pointer backward in the file
			Distance.QuadPart = (LONGLONG)nNewSize - Size.QuadPart;
			bIsSuccess = (SetFilePointerEx(hFile, Distance, NULL, FILE_END) && SetEndOfFile(hFile));
		}

		CloseHandle( hFile );
#else
		std::string sFileUTF8 = U_TO_UTF8(sPath);
		bIsSuccess = (0 == truncate(sFileUTF8.c_str(), nNewSize));
#endif
		return bIsSuccess;
	}

	std::wstring CFileBinary::GetTempPath()
	{
		if (!g_overrideTmpPath.empty())
			return g_overrideTmpPath;

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		wchar_t pBuffer[MAX_PATH + 1];
		memset(pBuffer, 0, sizeof(wchar_t) * (MAX_PATH + 1));
		::GetTempPathW(MAX_PATH, pBuffer);

		std::wstring sRet(pBuffer);

		size_t nSeparatorPos = sRet.find_last_of(wchar_t('/'));
		if (std::wstring::npos == nSeparatorPos)
		{
			nSeparatorPos = sRet.find_last_of(wchar_t('\\'));
		}

		if (std::wstring::npos == nSeparatorPos)
			return L"";

		return sRet.substr(0, nSeparatorPos);
#else
		char *folder = getenv("TEMP");

		if (NULL == folder)
			folder = getenv("TMP");
		if (NULL == folder)
			folder = getenv("TMPDIR");
		if (NULL == folder)
			folder = "/tmp";

		return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)folder, strlen(folder));
#endif
	}
	std::wstring CFileBinary::CreateTempFileWithUniqueName(const std::wstring& strFolderPathRoot, const std::wstring& Prefix)
	{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		wchar_t pBuffer[MAX_PATH + 1];
		::GetTempFileNameW(strFolderPathRoot.c_str(), Prefix.c_str(), 0, pBuffer);
		std::wstring sRet(pBuffer);
		return sRet;
#else
		char pcRes[MAX_PATH];
		BYTE* pData = (BYTE*)pcRes;

		std::wstring sPrefix = strFolderPathRoot + L"/" + Prefix + L"_XXXXXX";
		LONG lLen = 0;
		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sPrefix.c_str(), (LONG)sPrefix.length(), pData, lLen);
		pcRes[lLen] = '\0';

		int res = mkstemp(pcRes);
		if (-1 != res)
			close(res);

		std::string sRes = pcRes;
		return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sRes.c_str(), sRes.length());
#endif
	}
	bool CFileBinary::OpenTempFile(std::wstring *pwsName, FILE **ppFile, wchar_t *wsMode, wchar_t *wsExt, wchar_t *wsFolder, wchar_t* wsName)
	{
		// TODO: Реализовать когда wsName != NULL

		std::wstring wsTemp, wsFileName;
		FILE *pTempFile = NULL;
#if defined(_WIN32) || defined (_WIN64)
		wchar_t *wsTempDir = NULL;
		size_t sz = 0;
		if ( (0 == _wdupenv_s(&wsTempDir, &sz, L"TEMP")) && (wsFolder == NULL))
		{
			wsTemp = std::wstring(wsTempDir, sz-1);
#else
		char *wsTempDirA;
		if ((wsTempDirA = getenv("TEMP")) && (wsFolder == NULL))
		{
			std::wstring wsTempDir = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)wsTempDirA, strlen(wsTempDirA));
			wsTemp = wsTempDir.c_str();
#endif
			wsTemp += L"/";
		}
		else if (wsFolder != NULL)
		{
			wsTemp = std::wstring(wsFolder);
			wsTemp += L"/";
		}
		else
		{
			wsTemp = L"";
		}
		wsTemp += L"x";
		int nTime = (int)time(NULL);
		for (int nIndex = 0; nIndex < 1000; ++nIndex)
		{
			wsFileName = wsTemp;
			wsFileName.append(std::to_wstring(nTime + nIndex));

			if (wsExt)
			{
				wsFileName.append(wsExt);
			}
#if defined (_WIN32) || defined (_WIN64)
			if ( 0 != _wfopen_s(&pTempFile, wsFileName.c_str(), L"r") )
			{
				if (0 != _wfopen_s(&pTempFile, wsFileName.c_str(), wsMode))
#else
			std::string sFileName = U_TO_UTF8(wsFileName);
			if (!(pTempFile = fopen(sFileName.c_str(), "r")))
			{
				std::wstring strMode(wsMode);
				std::string sMode = U_TO_UTF8(strMode);
				if (!(pTempFile = fopen(sFileName.c_str(), sMode.c_str())))
#endif
				{
					return FALSE;
				}
				*pwsName = wsFileName;
				*ppFile = pTempFile;
				return TRUE;
			}

			fclose(pTempFile);
		}

		return FALSE;
	}
	FILE* CFileBinary::OpenFileNative(const std::wstring& sFileName, const std::wstring& sMode)
	{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		FILE* pFile = NULL;
		_wfopen_s(&pFile, sFileName.c_str(), sMode.c_str());

		return pFile;
#else
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		CUtf8Converter::GetUtf8StringFromUnicode(sFileName.c_str(), sFileName.length(), pUtf8, lLen, false);

		BYTE* pMode = NULL;
		LONG lLenMode;
		CUtf8Converter::GetUtf8StringFromUnicode(sMode.c_str(), sMode.length(), pMode, lLenMode, false);

		struct stat st;
		if ((0 == stat((char*)pUtf8, &st)) && S_ISDIR(st.st_mode))
		{
			delete [] pUtf8;
			delete [] pMode;
			return NULL;
		}

		FILE* pFile = fopen((char*)pUtf8, (char*)pMode);

		delete [] pUtf8;
		delete [] pMode;

		return pFile;
#endif
	}

	void CFileBinary::SetTempPath(const std::wstring& strTempPath)
	{
		g_overrideTmpPath = strTempPath;
	}

	unsigned long CFileBinary::GetDateTime(const std::wstring & inputFile)
	{
		unsigned long result = 0;
#if defined(_WIN32) || defined (_WIN64)
		HANDLE hFile;
		hFile = ::CreateFileW(inputFile.c_str(), GENERIC_READ, FILE_SHARE_READ,  NULL,  OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile)
		{
			FILETIME ft; ft.dwLowDateTime = ft.dwHighDateTime = 0;
			if (GetFileTime(hFile, NULL, NULL, &ft))
			{
				WORD fatDate = 0, fatTime = 0;
				if (FileTimeToDosDateTime(&ft, &fatDate,  &fatTime))
				{
					result = (fatDate << 16) + fatTime;
				}
			}
			CloseHandle(hFile);
		}
#else
		std::string inputFileA = U_TO_UTF8(inputFile);
#if defined(__linux__) && !defined(_MAC)
		struct stat attrib;
		stat(inputFileA.c_str(), &attrib);
		result = attrib.st_mtim.tv_nsec;
#else
		struct stat attrib;
		stat(inputFileA.c_str(), &attrib);
		result = (unsigned long)attrib.st_mtimespec.tv_nsec;
#endif
#endif
		return result;
	}
}

namespace NSFile
{
	bool CBase64Converter::Encode(BYTE* pDataSrc, int nLenSrc, char*& pDataDst, int& nLenDst, DWORD dwFlags)
	{
		if (!pDataSrc || nLenSrc < 1)
			return false;

		nLenDst = NSBase64::Base64EncodeGetRequiredLength(nLenSrc, dwFlags);
		pDataDst = new char[nLenDst];

		if (FALSE == NSBase64::Base64Encode(pDataSrc, nLenSrc, (BYTE*)pDataDst, &nLenDst, dwFlags))
		{
			RELEASEARRAYOBJECTS(pDataDst);
			return false;
		}
		return true;
	}
	bool CBase64Converter::Decode(const char* pDataSrc, int nLenSrc, BYTE*& pDataDst, int& nLenDst)
	{
		if (!pDataSrc || nLenSrc < 1)
			return false;

		nLenDst = NSBase64::Base64DecodeGetRequiredLength(nLenSrc);
		pDataDst = new BYTE[nLenDst];

		if (FALSE == NSBase64::Base64Decode(pDataSrc, nLenSrc, pDataDst, &nLenDst))
		{
			RELEASEARRAYOBJECTS(pDataDst);
			return false;
		}
		return true;
	}
}

namespace NSFile
{
	std::wstring GetProcessPath()
	{
#if defined (_WIN64) || defined(_WIN32)
		wchar_t buf [NS_FILE_MAX_PATH];
		GetModuleFileNameW(GetModuleHandle(NULL), buf, NS_FILE_MAX_PATH);
		return std::wstring(buf);
#endif

#if defined(__linux__) || defined(_MAC) && !defined(_IOS)
		char buf[NS_FILE_MAX_PATH];
		memset(buf, 0, NS_FILE_MAX_PATH);
		if (readlink ("/proc/self/exe", buf, NS_FILE_MAX_PATH) <= 0)
		{
#ifdef _MAC
			uint32_t _size = NS_FILE_MAX_PATH;
			_NSGetExecutablePath(buf, &_size);
			std::string sUTF8(buf);
			std::wstring sRet = CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sUTF8.c_str(), sUTF8.length());
			return sRet;
#endif
			return L"";
		}

		std::string sUTF8(buf);
		std::wstring sRet = CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sUTF8.c_str(), sUTF8.length());
		return sRet;
#endif

		return L"";
	}

	std::wstring GetProcessDirectory()
	{
		std::wstring sPath = GetProcessPath();

		size_t pos1 = sPath.find_last_of(wchar_t('/'));
		size_t pos2 = sPath.find_last_of(wchar_t('\\'));

		size_t pos = std::wstring::npos;
		if (pos1 != std::wstring::npos)
			pos = pos1;

		if (pos2 != std::wstring::npos)
		{
			if (pos == std::wstring::npos)
				pos = pos2;
			else if (pos2 > pos)
				pos = pos2;
		}

		if (pos != std::wstring::npos)
		{
			sPath = sPath.substr(0, pos);
		}
		return sPath;
	}

	// CommonFunctions
	std::wstring GetFileExtention(const std::wstring& sPath)
	{
		std::wstring::size_type nPos = sPath.rfind('.');
		if (nPos != std::wstring::npos)
			return sPath.substr(nPos + 1);
		return sPath;
	}
	std::wstring GetFileName(const std::wstring& sPath)
	{
#ifdef _WIN32
		std::wstring::size_type nPos1 = sPath.rfind('\\');
#else
		std::wstring::size_type nPos1 = std::wstring::npos;
#endif
		std::wstring::size_type nPos2 = sPath.rfind('/');
		std::wstring::size_type nPos = std::wstring::npos;

		if (nPos1 != std::wstring::npos)
		{
			nPos = nPos1;
			if (nPos2 != std::wstring::npos && nPos2 > nPos)
				nPos = nPos2;
		}
		else
			nPos = nPos2;

		if (nPos == std::wstring::npos)
			return sPath;
		return sPath.substr(nPos + 1);
	}
	std::wstring GetDirectoryName(const std::wstring& sPath)
	{
#ifdef _WIN32
		std::wstring::size_type nPos1 = sPath.rfind('\\');
#else
		std::wstring::size_type nPos1 = std::wstring::npos;
#endif
		std::wstring::size_type nPos2 = sPath.rfind('/');
		std::wstring::size_type nPos = std::wstring::npos;

		if (nPos1 != std::wstring::npos)
		{
			nPos = nPos1;
			if (nPos2 != std::wstring::npos && nPos2 > nPos)
				nPos = nPos2;
		}
		else
			nPos = nPos2;

		if (nPos == std::wstring::npos)
			return sPath;
		return sPath.substr(0, nPos);
	}
}
