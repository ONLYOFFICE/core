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
#ifndef _BUILD_STRING_CROSSPLATFORM_H_
#define _BUILD_STRING_CROSSPLATFORM_H_

#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <math.h>

#include "../../Common/kernel_config.h"

#ifdef USE_STRING_FORMAT
#include <stdarg.h>
namespace NSStrings
{
    static std::string format(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        std::vector<char> v(1024);
        while (true)
        {
            va_list args2;
            va_copy(args2, args);
            int res = vsnprintf(v.data(), v.size(), fmt, args2);
            if ((res >= 0) && (res < static_cast<int>(v.size())))
            {
                va_end(args);
                va_end(args2);
                return std::string(v.data());
            }
            size_t size;
            if (res < 0)
                size = v.size() * 2;
            else
                size = static_cast<size_t>(res) + 1;
            v.clear();
            v.resize(size);
            va_end(args2);
        }
    }

    static std::wstring format(const wchar_t *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        std::vector<wchar_t> v(1024);
        while (true)
        {
            va_list args2;
            va_copy(args2, args);
            int res = vswprintf(v.data(), v.size(), fmt, args2);
            if ((res >= 0) && (res < static_cast<int>(v.size())))
            {
                va_end(args);
                va_end(args2);
                return std::wstring(v.data());
            }
            size_t size;
            if (res < 0)
                size = v.size() * 2;
            else
                size = static_cast<size_t>(res) + 1;
            v.clear();
            v.resize(size);
            va_end(args2);
        }
    }
}
#endif

namespace NSStringExt
{
    class KERNEL_DECL CConverter
	{
	public:
		typedef enum
		{
			SINGLE_BYTE_ENCODING_DEFAULT = 0x01, // DEFAULT_CHARSET           1 (x01)
			SINGLE_BYTE_ENCODING_SYMBOL  = 0x02, // SYMBOL_CHARSET            2 (x02)
			SINGLE_BYTE_ENCODING_CP1252  = 0x00, // ANSI_CHARSET              0 (x00)
			SINGLE_BYTE_ENCODING_CP1251  = 0xCC, // RUSSIAN_CHARSET         204 (xCC)
			SINGLE_BYTE_ENCODING_CP1250  = 0xEE, // EASTEUROPE_CHARSET      238 (xEE)
			SINGLE_BYTE_ENCODING_CP1253  = 0xA1, // GREEK_CHARSET           161 (xA1)
			SINGLE_BYTE_ENCODING_CP1254  = 0xA2, // TURKISH_CHARSET         162 (xA2)
			SINGLE_BYTE_ENCODING_CP1257  = 0xBA, // BALTIC_CHARSET          186 (xBA)
			SINGLE_BYTE_ENCODING_CP1255  = 0xB1, // HEBREW_CHARSET          177 (xB1)
			SINGLE_BYTE_ENCODING_CP1256  = 0xB2, // ARABIC _CHARSET         178 (xB2)
			SINGLE_BYTE_ENCODING_CP932   = 0x80, // SHIFTJIS_CHARSET        128 (x80)
			SINGLE_BYTE_ENCODING_CP949   = 0x81, // HANGEUL_CHARSET         129 (x81)
			SINGLE_BYTE_ENCODING_CP936   = 0x86, // GB2313_CHARSET          134 (x86)
			SINGLE_BYTE_ENCODING_CP950   = 0x88, // CHINESEBIG5_CHARSET     136 (x88)
			SINGLE_BYTE_ENCODING_CP874   = 0xDE, // THAI_CHARSET            222 (xDE)
			SINGLE_BYTE_ENCODING_CP1361  = 0x82, // JOHAB_CHARSET	        130 (x82)
			SINGLE_BYTE_ENCODING_CP1258  = 0xA3, // VIETNAMESE_CHARSET      163 (xA3)
			SINGLE_BYTE_ENCODING_CP866   = 0xFF  // OEM_CHARSET             255 (xFF) // Проверить, что OEM соответствует CP866
		} ESingleByteEncoding;

        static std::wstring GetUnicodeFromSingleByteString(const unsigned char* pData, long lCount, ESingleByteEncoding eType = SINGLE_BYTE_ENCODING_DEFAULT);
        static std::wstring GetUnicodeFromUTF16(const unsigned short* pData, long lCount);
        static std::wstring GetUnicodeFromUTF32(const unsigned int* pData, long lCount);
        static std::string GetUtf8FromUTF32(const unsigned int* pData, long lCount);
        static unsigned int* GetUtf32FromUnicode(const std::wstring& wsUnicodeText, unsigned int& unLen);
        static unsigned short* GetUtf16FromUnicode(const std::wstring& wsUnicodeText, unsigned int& unLen, const bool& isLE = true);
    };

    class CStringUnicodeIterator_private;
    class KERNEL_DECL CStringUnicodeIterator
    {
    private:
        CStringUnicodeIterator_private* m_internal;

    public:
        CStringUnicodeIterator(const std::wstring& string);
        bool Check();
        void Next();
        unsigned int Value();
    };
}

namespace NSStringExt
{
	static std::vector<std::wstring>& Split(const std::wstring& wsString, wchar_t nDelim, std::vector<std::wstring> &arrElements)
	{
		std::wstringstream wStringStream(wsString);
		std::wstring wsItem;
		while (std::getline(wStringStream, wsItem, nDelim))
		{
			arrElements.push_back(wsItem);
		}
		return arrElements;
	}
	static std::vector<std::wstring>  Split(const std::wstring& wsString, wchar_t nDelim)
	{
		std::vector<std::wstring> wsElements;
		Split(wsString, nDelim, wsElements);
		return wsElements;
	}
	static std::vector<std::wstring>& Split(const std::wstring& wsString, const std::wstring wsDelim, std::vector<std::wstring> &arrElements)
	{
		size_t nDelimLen	= wsDelim.length();
		size_t nPrevPos		= 0;

		if (nDelimLen > 0)
		{
			size_t nPos = wsString.find(wsDelim);
			while (std::wstring::npos != nPos)
			{
				if (nPrevPos != nPos)
					arrElements.push_back(wsString.substr(nPrevPos, nPos - nPrevPos));

				nPrevPos = nPos + nDelimLen;
				nPos = wsString.find(wsDelim, nPrevPos);
			}
		}

		if (nPrevPos < wsString.length())
			arrElements.push_back(wsString.substr(nPrevPos));

		return arrElements;
	}
	static std::vector<std::wstring>  Split(const std::wstring& wsString, const std::wstring& wsDelim, bool bWholeString = true)
	{
		std::vector<std::wstring> arrElements;

		if (bWholeString)
		{
			int nDelimLen = (int)wsDelim.length();
			if (0 == nDelimLen)
				arrElements.push_back(wsString);
			else if (1 == nDelimLen)
				Split(wsString, wchar_t(wsDelim[0]), arrElements);
			else
				Split(wsString, wsDelim, arrElements);
		}
		else
		{
			std::vector<std::wstring> arrCurrent;
			arrCurrent.push_back(wsString);
			arrElements.push_back(wsString);
			int nPos = 0;
			int nLen = (int)wsDelim.length();
			while (nPos < nLen)
			{
				wchar_t wChar = wsDelim.at(nPos++);
				arrElements.clear();
				for (size_t nIndex = 0, nCount = arrCurrent.size(); nIndex < nCount; nIndex++)
				{
					std::vector<std::wstring> arrTemp = Split(arrCurrent.at(nIndex), wChar);
					arrElements.insert(arrElements.end(), arrTemp.begin(), arrTemp.end());
				}
				arrCurrent = arrElements;
			}
		}

		return arrElements;
	}
	static inline void ToLower(std::string& wsString)
	{
			std::transform(wsString.begin(), wsString.end(), wsString.begin(), ::towlower);
	}
	static inline void ToUpper(std::string& wsString)
	{
			std::transform(wsString.begin(), wsString.end(), wsString.begin(), ::towupper);
	}
	static inline void ToLower(std::wstring& wsString)
	{
		std::transform(wsString.begin(), wsString.end(), wsString.begin(), ::towlower);
	}
	static inline void ToUpper(std::wstring& wsString)
	{
		std::transform(wsString.begin(), wsString.end(), wsString.begin(), ::towupper);
	}
	static inline void Replace(std::wstring& wsString, const std::wstring& wsFrom, const std::wstring& wsTo)
	{
		int nFromLen	= (int)wsFrom.length();
		int nToLen		= (int)wsTo.length();
		size_t nPos		= -nToLen;

		while (std::wstring::npos != (nPos = wsString.find(wsFrom, nPos + nToLen)))
		{
			wsString.replace(nPos, nFromLen, wsTo);
		}
	}
	static inline void LTrim(std::wstring &str, const wchar_t* chars)
	{
		str.erase(0, str.find_first_not_of(chars));
	}
	static inline void RTrim(std::wstring &str, const wchar_t* chars)
	{
		str.erase(str.find_last_not_of(chars) + 1);
	}
	static inline long FindFirstNotOfA(const char* str, const char* chars)
	{
		long res = 0;
		while('\0' != str[res])
		{
			long index = 0;
			while('\0' != chars[index] && chars[index] != str[res])
			{
				index++;
			}
			if('\0' == chars[index])
			{
				break;
			}
			res++;
		}
		return res;
	}
	static inline long FindLastNotOf(const wchar_t* str, unsigned int unLen, const wchar_t* chars)
	{
		long res = unLen;
		res--;
		while(res >= 0)
		{
			long index = 0;
			while('\0' != chars[index] && chars[index] != str[res])
			{
				index++;
			}
			if('\0' == chars[index])
			{
				break;
			}
			res--;
		}
		return res;
	}
	static bool FromHumanReadableByteCount(const std::wstring& wsString, long long& res)
	{
		res = 0;
		long long coeff = 0;
		int unit = 1000;
		int exp  = 0;
		// Parse leading numeric factor
		std::size_t pos = 0;
		try
		{
			coeff = std::stoll(wsString, &pos);
		}
		catch(...)
		{
			return false;
		}

		// Skip any intermediate white space
		while (pos < wsString.length() && isspace(wsString[pos])) ++pos;

		// Read off first character which should be an SI prefix
		if (pos < wsString.length())
		{
			switch (toupper(wsString[pos])) {
				case 'B':  exp =  0; break;
				case 'K':  exp =  3; break;
				case 'M':  exp =  6; break;
				case 'G':  exp =  9; break;
				case 'T':  exp = 12; break;
				case 'E':  exp = 15; break;
				case 'Z':  exp = 18; break;
				case 'Y':  exp = 21; break;

				default:   return false;
			}
			++pos;
		}

		// If an 'i' or 'I' is present use IEC standard 1024 units
		if (pos < wsString.length())
		{
			if (toupper(wsString[pos]) == 'I') {
				++pos;
				unit = 1024;
			}
		}

		// Next character must be one of B/empty/whitespace
		if (pos < wsString.length())
		{
			switch (toupper(wsString[pos])) {
				case 'B':
				case ' ':
				case '\t': ++pos;  break;

				default:   return false;
			}
		}

		// Skip any remaining white space
		while (pos < wsString.length() && isspace(wsString[pos])) ++pos;

		// Parse error on anything but a null terminator
		if (pos < wsString.length()) return false;

		res = exp ? (long long)(coeff * pow(unit, exp / 3)) : coeff;
		return true;
	}
}

#endif // _BUILD_STRING_CROSSPLATFORM_H_
