
#include "precompiled_utility.h"

#include "Encoding.h"
#include "Utility.h"

#if defined (_WIN32) || defined (_WIN64)
    #include <windows.h>
#else
    #include <iconv.h>
#endif

#include "../../../Common/DocxFormat/Source/Base/unicode_util.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../UnicodeConverter/UnicodeConverter_Encodings.h"


const std::wstring Encoding::ansi2unicode(const std::string& line)
{
	return std::wstring(line.begin(), line.end());//cp2unicode(line, CP_ACP);
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
            return cp2unicode(line,0);
		}
		std::wstring utf32Str ((wchar_t *) pStrUtf32);

		delete [] pStrUtf32;
		return utf32Str;
	}
}

const std::string Encoding::unicode2ansi(const std::wstring& line)
{
    return std::string(line.begin(), line.end());
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
