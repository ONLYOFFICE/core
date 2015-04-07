
#include "precompiled_utility.h"

#include "Encoding.h"
#include "Utility.h"

#ifdef _WIN32
	#include <windows.h>	
#endif

#include "../../../../Base/unicode_util.h"


const std::wstring Encoding::ansi2unicode(const std::string& line)
{
	return std::wstring(line.begin(), line.end());//cp2unicode(line, CP_ACP);
}

const std::wstring Encoding::cp2unicode(const std::string& sline, const unsigned int codePage)
{
#ifdef _WIN32
	const int nSize = MultiByteToWideChar(codePage, 0, sline.c_str(), sline.size(), NULL, 0);

	wchar_t *sTemp = new wchar_t[nSize];
	if (!sTemp)
		return std::wstring();

	int size = MultiByteToWideChar(codePage, 0, sline.c_str(), sline.size(), sTemp, nSize);

	std::wstring sResult(sTemp, size);
	delete []sTemp;

	return sResult;
#elif __linux__
	return std::wstring();
#else
	return std::wstring();
#endif
}


const std::wstring Encoding::utf82unicode(const std::string& line)
{
	if (sizeof(wchar_t) == 2)//utf8 -> utf16
	{
		unsigned __int32 nLength = line.length();

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
		unsigned __int32 nLength = line.length();

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
			return std::wstring();
		}
		std::wstring utf32Str ((wchar_t *) pStrUtf32);

		delete [] pStrUtf32;
		return utf32Str;
	}
}

const std::string Encoding::unicode2ansi(const std::wstring& line)
{
	return unicode2cp(line, CP_ACP);
}

const std::string Encoding::unicode2utf8(const std::wstring& line)
{
	if (sizeof(wchar_t) == 2)
	{
		UTF16 *pStrUtf16	= (UTF16 *) &line[0];

		unsigned __int32 nLength	= line.length();
		unsigned __int32 nDstLength = 4*nLength + 1;
	    
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

		unsigned __int32 nLength	= line.length();
		unsigned __int32 nDstLength = 4*nLength + 1;
	    
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

const std::string Encoding::unicode2cp(const std::wstring& sLine, const unsigned int codePage)
{
#ifdef _WIN32
	const int nSize = WideCharToMultiByte(codePage, 0, sLine.c_str(), sLine.length(), NULL, 0, NULL, NULL);
	char *sTemp = new char[nSize];
	if (!sTemp)
		return std::string();

	int size = WideCharToMultiByte(codePage, 0, sLine.c_str(), sLine.length(), sTemp, nSize, NULL, NULL);

	std::string sResult(sTemp, size);
	delete []sTemp;

	return sResult;
#elif __linux__
	return std::string();
#else
	return std::string();
#endif
}
