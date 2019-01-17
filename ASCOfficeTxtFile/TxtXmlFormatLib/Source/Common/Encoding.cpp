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

#include "precompiled_utility.h"

#include "Encoding.h"
#include "Utility.h"

#include "../../../Common/DocxFormat/Source/Base/unicode_util.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../UnicodeConverter/UnicodeConverter_Encodings.h"

#if !defined(_WIN32) && !defined(_WIN64)
    #include <string.h>
#endif

#include <locale>

const std::wstring Encoding::ansi2unicode(const std::string& line)
{
    if (line.empty())
        return std::wstring();
    
	std::locale loc("");
	std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);

    std::wstring result;
	result.resize(line.size());
    
    facet.widen(line.c_str(), line.c_str() + line.size(), &result[0]);
	return result;
}

const std::wstring Encoding::cp2unicode(const std::string& sline, const unsigned int nCodepage)
{
    const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[nCodepage];
    NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;
    return oUnicodeConverter.toUnicode(sline, oEncodindId.Name);
}


const std::wstring Encoding::utf82unicode(const std::string& line)
{
	if (sizeof(wchar_t) == 2)//utf8 -> utf16
	{
        unsigned int nLength = line.length();

		UTF16 *pStrUtf16 = new UTF16 [nLength+1];
		memset ((void *) pStrUtf16, 0, sizeof (UTF16) * (nLength+1));

		UTF8 *pStrUtf8 = (UTF8 *) &line[0];

		// this values will be modificated
		const UTF8 *pStrUtf8_Conv = pStrUtf8;
		UTF16 *pStrUtf16_Conv = pStrUtf16;

		ConversionResult eUnicodeConversionResult = ConvertUTF8toUTF16 (&pStrUtf8_Conv,	 &pStrUtf8[nLength]
				, &pStrUtf16_Conv, &pStrUtf16 [nLength]
				, strictConversion);

		if (conversionOK != eUnicodeConversionResult)
		{
			delete [] pStrUtf16;
			return std::wstring();
		}
		std::wstring utf16Str ((wchar_t *) pStrUtf16);

		delete [] pStrUtf16;
		return utf16Str;
	}
	else //utf8 -> utf32
	{
        unsigned int nLength = line.length();

		UTF32 *pStrUtf32 = new UTF32 [nLength+1];
		memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength+1));


		UTF8 *pStrUtf8 = (UTF8 *) &line[0];

		// this values will be modificated
		const UTF8 *pStrUtf8_Conv = pStrUtf8;
		UTF32 *pStrUtf32_Conv = pStrUtf32;

		ConversionResult eUnicodeConversionResult = ConvertUTF8toUTF32 (&pStrUtf8_Conv, &pStrUtf8[nLength]
				, &pStrUtf32_Conv, &pStrUtf32 [nLength]
				, strictConversion);

		if (conversionOK != eUnicodeConversionResult)
		{
			delete [] pStrUtf32;
            return ansi2unicode(line);
		}
		std::wstring utf32Str ((wchar_t *) pStrUtf32);

		delete [] pStrUtf32;
		return utf32Str;
	}
}

const std::string Encoding::unicode2ansi(const std::wstring& line)
{
    if (line.empty())
        return std::string();
    
	std::locale loc("");
	std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);

    std::string result;
	result.resize(line.size());
    
	facet.narrow(line.c_str(), line.c_str() + line.size(), '?', &result[0]);
	return result;
}

const std::string Encoding::unicode2utf8(const std::wstring& line)
{
	if (sizeof(wchar_t) == 2)
	{
		UTF16 *pStrUtf16	= (UTF16 *) &line[0];

        unsigned int nLength	= line.length();
        unsigned int nDstLength = 4*nLength + 1;
	    
		UTF8 *pStrUtf8		= new UTF8 [nDstLength];
		memset ((void *) pStrUtf8, 0, sizeof (UTF8) * (nDstLength));

		// this values will be modificated
		const UTF16 *pStrUtf16_Conv = pStrUtf16;
		UTF8 *pStrUtf8_Conv = pStrUtf8;

		ConversionResult eUnicodeConversionResult = ConvertUTF16toUTF8 (&pStrUtf16_Conv, &pStrUtf16[nLength]
					, &pStrUtf8_Conv , &pStrUtf8 [nDstLength]
					, strictConversion);

		if (conversionOK != eUnicodeConversionResult)
		{
			delete [] pStrUtf8;
			return std::string();
		}

		std::string utf8Str ((char *) pStrUtf8);

		delete [] pStrUtf8;
		return utf8Str;
	}
	else //utf32 -> utf8
	{
		UTF32 *pStrUtf32	= (UTF32 *) &line[0];

        unsigned int nLength	= line.length();
        unsigned int nDstLength = 4*nLength + 1;
	    
		UTF8 *pStrUtf8		= new UTF8 [nDstLength];
		memset ((void *) pStrUtf8, 0, sizeof (UTF8) * (nDstLength));

		// this values will be modificated
		const UTF32 *pStrUtf32_Conv = pStrUtf32;
		UTF8 *pStrUtf8_Conv = pStrUtf8;

		ConversionResult eUnicodeConversionResult = ConvertUTF32toUTF8 (&pStrUtf32_Conv, &pStrUtf32[nLength]
					, &pStrUtf8_Conv , &pStrUtf8 [nDstLength]
					, strictConversion);

		if (conversionOK != eUnicodeConversionResult)
		{
			delete [] pStrUtf8;
			return std::string();
		}

		std::string utf8Str ((char *) pStrUtf8);

		delete [] pStrUtf8;
		return utf8Str;
	}
}

const std::string Encoding::unicode2cp(const std::wstring& sLine, const unsigned int nCodepage)
{
    const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[nCodepage];
    NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;
    return oUnicodeConverter.fromUnicode(sLine, oEncodindId.Name);
}
