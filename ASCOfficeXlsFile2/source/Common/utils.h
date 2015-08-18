#pragma once

#include <string>
#include <string.h>

#include "../../../Common/DocxFormat/Source/Base/unicode_util.h"

static std::wstring convertUtf16ToWString(const UTF16 * Data, int nLength)
{
    UTF32 *pStrUtf32 = new UTF32 [nLength + 1];
    memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength + 1));

    // this values will be modificated
    const UTF16 *pStrUtf16_Conv = Data;
    UTF32 *pStrUtf32_Conv = pStrUtf32;

    ConversionResult eUnicodeConversionResult =
            ConvertUTF16toUTF32 (&pStrUtf16_Conv,
                                 &Data[nLength]
            , &pStrUtf32_Conv
            , &pStrUtf32 [nLength]
            , strictConversion);

    if (conversionOK != eUnicodeConversionResult)
    {
        delete [] pStrUtf32;
        return std::wstring();
    }

    std::wstring wstr ((wchar_t *) pStrUtf32);

    delete [] pStrUtf32;
    return wstr;
}

namespace xml {

namespace utils {

	namespace details {
		std::wstring replace_text_to_xml_ImplRegEx(const std::wstring & Text);
		std::string replace_text_to_xml_ImplRegEx(const std::string & Text);

		std::wstring replace_xml_to_text_ImplRegEx(const std::wstring & Text);
		std::string replace_xml_to_text_ImplRegEx(const std::string & Text);

		std::wstring replace_text_to_xml_ImplReplace(const std::wstring & Text);
		std::string replace_text_to_xml_ImplReplace(const std::string & Text);

		std::wstring replace_amp_text_to_xml_ImplReplace(const std::wstring & Text);
		std::string replace_amp_text_to_xml_ImplReplace(const std::string & Text);

		std::wstring replace_xml_to_text_ImplReplace(const std::wstring & Text);
		std::string replace_xml_to_text_ImplReplace(const std::string & Text);
	}

	std::wstring replace_text_to_xml(const std::wstring & Text);
	std::string replace_text_to_xml(const std::string & Text);

	std::wstring replace_amp_text_to_xml(const std::wstring & Text);

	std::wstring replace_lt_gt(const std::wstring & Text);

	std::wstring replace_xml_to_text(const std::wstring & Text);
	std::string replace_xml_to_text(const std::string & Text);
}

}
