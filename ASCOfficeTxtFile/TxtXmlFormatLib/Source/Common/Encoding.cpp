
#include "precompiled_utility.h"

#include "Encoding.h"
#include "Utility.h"

#if defined (_WIN32) || defined (_WIN64)
    #include <windows.h>
#else
    #include <iconv.h>
#endif

#include "../../../Common/DocxFormat/Source/Base/unicode_util.h"


const std::wstring Encoding::ansi2unicode(const std::string& line)
{
	return std::wstring(line.begin(), line.end());//cp2unicode(line, CP_ACP);
}

const std::wstring Encoding::cp2unicode(const std::string& sline, const unsigned int nCodepage)
{
#if defined (_WIN32) || defined (_WIN64)
	const int nSize = MultiByteToWideChar(nCodepage, 0, sline.c_str(), sline.size(), NULL, 0);

	wchar_t *sTemp = new wchar_t[nSize];
	if (!sTemp)
		return std::wstring();

    int size = MultiByteToWideChar(nCodepage, 0, sline.c_str(), sline.size(), sTemp, nSize);

	std::wstring sResult(sTemp, size);
	delete []sTemp;

	return sResult;
#else
    bool ansi = true;

    size_t insize = sline.length();
    std::wstring w_out;

    char *inptr = (char*)sline.c_str();

    if (nCodepage >= 0)
    {
        std::string sCodepage =  "CP" + std::to_string(nCodepage);

        iconv_t ic= iconv_open("WCHAR_T", sCodepage.c_str());
        if (ic != (iconv_t) -1)
        {
            size_t nconv = 0, avail = (insize + 1) * sizeof(wchar_t);

            char* out_str   = new char[avail];
            char* outptr    = out_str;

            nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
            if (nconv == 0)
            {
                insize = sline.length();
                ((wchar_t*)out_str)[insize] = 0;
                w_out = std::wstring((wchar_t*)out_str, insize);
                ansi = false;
            }
            iconv_close(ic);
            delete []out_str;
        }
    }
    if (ansi)
        w_out = std::wstring(sline.begin(), sline.end());

    return w_out;
#endif
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
#if defined (_WIN32) || defined (_WIN64)
	const int nSize = WideCharToMultiByte(nCodepage, 0, sLine.c_str(), sLine.length(), NULL, 0, NULL, NULL);
	char *sTemp = new char[nSize];
	if (!sTemp)
		return std::string();

    int size = WideCharToMultiByte(nCodepage, 0, sLine.c_str(), sLine.length(), sTemp, nSize, NULL, NULL);

	std::string sResult(sTemp, size);
	delete []sTemp;

	return sResult;
#else
    std::string out;
    bool ansi = true;

    size_t insize = sLine.length();
    out.reserve(insize);

    char *inptr = (char*)sLine.c_str();
    char* outptr = (char*)out.c_str();

    if (nCodepage >= 0)
    {
        std::string sCodepage =  "CP" + std::to_string(nCodepage);

        iconv_t ic= iconv_open(sCodepage.c_str(), "WCHAR_T");
        if (ic != (iconv_t) -1)
        {
            size_t nconv = 0, avail = insize * sizeof(wchar_t);

            nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
            if (nconv == 0) ansi = false;
            iconv_close(ic);
        }
    }

    if (ansi)
        out = std::string(sLine.begin(), sLine.end());

    return out;

#endif
}
