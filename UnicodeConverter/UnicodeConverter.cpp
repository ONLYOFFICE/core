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
#include "stdint.h"
#include "./UnicodeConverter.h"

#include "unicode/utypes.h"
#include "unicode/ustring.h"
#include "unicode/ucnv.h"     /* C   Converter API    */
#include "unicode/usprep.h"

//#include "../DesktopEditor/common/File.h"

#if !defined (_WIN32) && !defined (_WIN64)
#if defined (_LINUX) && !defined(__ANDROID__) && !defined(_IOS) && !defined(_MAC)
#define USE_ICONV
#include "iconv.h"
#endif
#else
#include <windows.h>
#endif

std::string g_overrideIcuDataPath = "";

namespace NSUnicodeConverter
{
	class CUnicodeConverter_Private
	{
	public:
		CUnicodeConverter_Private()
		{
#if 0
			std::wstring sDumpPath = NSFile::GetProcessDirectory() + L"/codepages.txt";
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sDumpPath);

			int32_t count = ucnv_countAvailable();

			for (int i = 0; i < count; ++i)
			{
				std::string sCodePage = ucnv_getAvailableName(i);

				UErrorCode _error = U_ZERO_ERROR;
				int nCountAliases = ucnv_countAliases(sCodePage.c_str(), &_error);

				char** palices = new char*[nCountAliases];
				ucnv_getAliases(sCodePage.c_str(), (const char**)palices, &_error);

				oFile.WriteStringUTF8(L"-----------------------------\r\n");
				oFile.WriteFile((BYTE*)sCodePage.c_str(), sCodePage.length());
				oFile.WriteStringUTF8(L"\r\n");

				for (int j = 0; j < nCountAliases; ++j)
				{
					oFile.WriteFile((BYTE*)palices[j], strlen(palices[j]));
					oFile.WriteStringUTF8(L"\r\n");
				}
			}

			oFile.CloseFile();
#endif
		}
		~CUnicodeConverter_Private()
		{
		}

		std::string SASLprepToUtf8(const wchar_t* sInput, const unsigned int& nInputLen)
		{
#ifndef DISABLE_ICU
			std::string sRes;
			UErrorCode status = U_ZERO_ERROR;

			int32_t nUCharCapacity = (int32_t)nInputLen;// UTF-16 uses 2 code-points per char

			UChar* pUChar = new UChar[(uint32_t)nUCharCapacity * sizeof(UChar)];
			if (pUChar)
			{
				const UChar* pUCharStart = pUChar;
				int32_t nUCharLength = 0;

				u_strFromWCS(pUChar, nUCharCapacity, &nUCharLength, sInput, (int32_t)nInputLen, &status);
				if (U_SUCCESS(status))
				{
					UStringPrepProfile *profile = usprep_openByType(USPREP_RFC4013_SASLPREP, &status);

					UParseError parseError;

					int32_t nOutputLen = nUCharLength * 2;
					if (U_SUCCESS(status))
					{
						UChar* pOutput = new UChar[(uint32_t)nOutputLen * sizeof(UChar) * 3];
						nOutputLen = usprep_prepare(profile, pUCharStart, nUCharLength, pOutput, nOutputLen, 0, &parseError, &status );

						if (U_SUCCESS(status))
						{
							UConverter* conv = ucnv_open("UTF-8", &status);
							if (U_SUCCESS(status))
							{
								const UChar* pOutputLimit = pOutput + nOutputLen;
								const UChar* pOutputStart = pOutput;

								char *sResStart = new char[nOutputLen * ucnv_getMaxCharSize(conv)];
								char *sResCur = sResStart;
								const char *sResLimit = sResCur + nOutputLen * ucnv_getMaxCharSize(conv);

								ucnv_fromUnicode(conv, &sResCur, sResLimit, &pOutputStart, pOutputLimit, NULL, TRUE, &status);
								if (U_SUCCESS(status))
								{
									sRes = std::string(sResStart, sResCur - sResStart);
								}
								delete []sResStart;
								ucnv_close(conv);
							}
						}
						delete []pOutput;
						usprep_close(profile);
					}

				}
				delete []pUChar;
			}
			return sRes;
#else
			std::wstring strInput(sInput, nInputLen);
			std::string sRes(strInput.begin(), strInput.end());
			return sRes;
#endif
		}

		static std::string fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
		{
			std::string sRes = "";
			UErrorCode status = U_ZERO_ERROR;
			UConverter* conv = ucnv_open(converterName, &status);
			if (U_SUCCESS(status))
			{
				int32_t nUCharCapacity = (int32_t)nInputLen;// UTF-16 uses 2 code-points per char

				//UChar* pUChar = new UChar[nUCharCapacity];
				UChar* pUChar = (UChar*)malloc((uint32_t)nUCharCapacity * sizeof(UChar));
				if (pUChar)
				{
					const UChar* pUCharStart = pUChar;
					int32_t nUCharLength = 0;

					u_strFromWCS(pUChar, nUCharCapacity, &nUCharLength, sInput, (int32_t)nInputLen, &status);
					if (U_SUCCESS(status))
					{
						const UChar* pUCharLimit = pUCharStart + nUCharLength;
						sRes.resize((uint32_t)nUCharLength * (uint8_t)ucnv_getMaxCharSize(conv));// UTF-16 uses 2 code-points per char
						char *sResStart = &sRes[0];
						char *sResCur = sResStart;
						const char *sResLimit = sResCur + sRes.size();

						ucnv_fromUnicode(conv, &sResCur, sResLimit, &pUCharStart, pUCharLimit, NULL, TRUE, &status);
						if (U_SUCCESS(status))
						{
							sRes.resize(sResCur - sResStart);
						}
						else
						{
							sRes.clear();
						}
					}
					//delete []pUCharStart;
				}
				ucnv_close(conv);
			}

			if (sRes.empty() && nInputLen > 0)
			{
				std::wstring ws(sInput, nInputLen);
				sRes = std::string(ws.begin(), ws.end());
			}
			return sRes;
		}
		std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage, bool isExact)
		{
			std::wstring sRes = L"";
			UErrorCode status = U_ZERO_ERROR;
			UConverter* conv = ucnv_openCCSID(nCodePage, UCNV_UNKNOWN, &status);
			if (U_SUCCESS(status))
			{
				//UConverter* conv = ucnv_openCCSID(5347, UCNV_IBM, &status);
				if (U_SUCCESS(status))
				{
					const char* source = sInput;
					const char* sourceLimit = source + nInputLen;

					unsigned int uBufSize = (nInputLen / (uint8_t)ucnv_getMinCharSize(conv));

					UChar* targetStart = new UChar[uBufSize * sizeof(UChar)];
					if (targetStart)
					{
						UChar* target = targetStart;
						UChar* targetLimit = target + uBufSize;

						ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, NULL, TRUE, &status);
						if (U_SUCCESS(status))
						{
							size_t nTargetSize = target - targetStart;
							sRes.resize(nTargetSize * 2);// UTF-16 uses 2 code-points per char
							int32_t nResLen = 0;

							u_strToWCS(&sRes[0], (int32_t)sRes.size(), &nResLen, targetStart, (int32_t)nTargetSize, &status);
							if (U_SUCCESS(status))
							{
								sRes.resize((size_t)nResLen);
							}
							else
							{
								sRes.clear();
							}
						}
						delete []targetStart;
					}
					ucnv_close(conv);
				}
			}
			else
			{//10008 & other
				sRes = convert_string(sInput, nInputLen, nCodePage);
			}

			if (!isExact && sRes.empty() && nInputLen > 0)
			{
				std::string ws(sInput, nInputLen);
				sRes = std::wstring(ws.begin(), ws.end());
			}
			return sRes;
		}
		std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName, bool isExact)
		{
			std::wstring sRes = L"";
			UErrorCode status = U_ZERO_ERROR;
			UConverter* conv = ucnv_open(converterName, &status);
			if (U_SUCCESS(status))
			{
				//UConverter* conv = ucnv_openCCSID(5347, UCNV_IBM, &status);
				if (U_SUCCESS(status))
				{
					const char* source = sInput;
					const char* sourceLimit = source + nInputLen;

					unsigned int uBufSize = (nInputLen / (uint8_t)ucnv_getMinCharSize(conv));

					UChar* targetStart = new UChar[uBufSize * sizeof(UChar)];
					if (targetStart)
					{
						UChar* target = targetStart;
						UChar* targetLimit = target + uBufSize;

						ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, NULL, TRUE, &status);
						if (U_SUCCESS(status))
						{
							size_t nTargetSize = target - targetStart;
							sRes.resize(nTargetSize * 2);// UTF-16 uses 2 code-points per char
							int32_t nResLen = 0;

							u_strToWCS(&sRes[0], (int32_t)sRes.size(), &nResLen, targetStart, (int32_t)nTargetSize, &status);
							if (U_SUCCESS(status))
							{
								sRes.resize((size_t)nResLen);
							}
							else
							{
								sRes.clear();
							}
						}
						delete []targetStart;
					}
					ucnv_close(conv);
				}
			}

			if (isExact && sRes.empty() && nInputLen > 0)
			{
				std::string ws(sInput, nInputLen);
				sRes = std::wstring(ws.begin(), ws.end());
			}
			return sRes;
		}
		std::wstring convert_string(const char* sInput, const unsigned int& nInputLen, int nCodepage = 0)
		{
			bool ansi = true;
			std::wstring sResult;

			size_t insize = (size_t)nInputLen;
			char* inptr = (char*)sInput;

			if (nCodepage > 0)
			{
#if defined (_WIN32) || defined (_WIN64)
				int outsize_with_0 = MultiByteToWideChar(nCodepage, 0, inptr, -1, NULL, NULL);
				sResult.resize(outsize_with_0);
				if (MultiByteToWideChar(nCodepage, 0, inptr, -1, (LPWSTR)sResult.c_str(), outsize_with_0) > 0)
				{
					sResult.erase(outsize_with_0 - 1);
					ansi = false;
				}
#elif defined(USE_ICONV)
				std::string sCodepage =  "CP" + std::to_string(nCodepage);

				iconv_t ic= iconv_open("WCHAR_T", sCodepage.c_str());
				if (ic != (iconv_t) -1)
				{
					sResult.resize(insize);
					char* outptr = (char*)sResult.c_str();

					size_t nconv = 0, avail = (insize) * sizeof(wchar_t), outsize = insize;
					nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
					if (nconv == 0)
					{
						if (avail > 0)
						{
							outsize = outsize - avail/sizeof(wchar_t);
							sResult.erase(sResult.begin() + outsize);
						}
						ansi = false;
					}
					iconv_close(ic);
				}
#endif
			}
			if (ansi)
			{
				std::string inp(sInput, nInputLen);
				sResult = std::wstring(inp.begin(), inp.end());
			}

			return sResult;
		}
		std::string GetUtf8StringFromUnicode(const wchar_t* pUnicodes, size_t lCount)
		{
			if (NULL == pUnicodes || 0 == lCount)
				return "";

			unsigned char* pData = new unsigned char[6 * lCount + 1];
			size_t lOutputCount = 0;

			unsigned char* pCodesCur = pData;

			const wchar_t* pEnd = pUnicodes + lCount;
			const wchar_t* pCur = pUnicodes;

			bool bIsUtf16 = (2 == sizeof(wchar_t)) ? true : false;

			while (pCur < pEnd)
			{
				unsigned int code = (unsigned int)*pCur++;
				if (bIsUtf16)
				{
					if (code >= 0xD800 && code <= 0xDFFF && pCur < pEnd)
					{
						code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & *pCur++));
					}
				}

				if (code < 0x80)
				{
					*pCodesCur++ = (unsigned char)code;
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

			lOutputCount = (size_t)(pCodesCur - pData);
			*pCodesCur++ = 0;

			std::string s((char*)pData, lOutputCount);

			if (pData) delete []pData;
			pData = NULL;
			return s;
		}
	};
}

namespace NSUnicodeConverter
{
	CUnicodeConverter::CUnicodeConverter()
	{
		m_pInternal = new CUnicodeConverter_Private();
	}
	CUnicodeConverter::~CUnicodeConverter()
	{
		delete m_pInternal;
	}

	std::string CUnicodeConverter::fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
	{
		return m_pInternal->fromUnicode(sInput, nInputLen, converterName);
	}
	std::string CUnicodeConverter::fromUnicode(const std::wstring &sInput, const char *converterName)
	{
		return this->fromUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName);
	}
	std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName, bool isExact)
	{
		return m_pInternal->toUnicode(sInput, nInputLen, converterName, isExact);
	}
	std::wstring CUnicodeConverter::toUnicode(const std::string &sInput, const char *converterName, bool isExact)
	{
		return this->toUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName, isExact);
	}
	std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage, bool isExact)
	{
		if (nInputLen < 1) return std::wstring(L"");

		std::wstring result;
		result = m_pInternal->toUnicode(sInput, nInputLen, nCodePage, isExact);

		if (result.empty() && nInputLen > 0)
		{
			result = m_pInternal->convert_string(sInput, nInputLen, nCodePage);
		}
		return result;
	}
	std::wstring CUnicodeConverter::toUnicode(const std::string &sInput, int nCodePage, bool isExact)
	{
		return this->toUnicode(sInput.c_str(), (unsigned int)sInput.size(), nCodePage, isExact);
	}
	std::string CUnicodeConverter::SASLprepToUtf8(const std::wstring &sSrc)
	{
		return m_pInternal->SASLprepToUtf8(sSrc.c_str(), sSrc.length());
	}

	void CUnicodeConverter::setIcuDataPath(const std::wstring& sDirectory)
	{
		if (!g_overrideIcuDataPath.empty())
			return;
		CUnicodeConverter_Private internal;
		g_overrideIcuDataPath = internal.GetUtf8StringFromUnicode(sDirectory.c_str(), sDirectory.length());
		u_setDataDirectory(g_overrideIcuDataPath.c_str());
	}

}
