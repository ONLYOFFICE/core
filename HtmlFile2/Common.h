#ifndef COMMON_H
#define COMMON_H

#include <string>
#include "../DesktopEditor/common/StringBuilder.h"

namespace HTML
{
#define MAX_STRING_BLOCK_SIZE (size_t)10485760

using XmlString = NSStringUtils::CStringBuilder;

inline void replace_all(std::wstring& s, const std::wstring& s1, const std::wstring& s2)
{
	size_t pos = s.find(s1);
	size_t l = s2.length();
	while (pos != std::string::npos)
	{
		if (!(s1 == L"&" && s2 == L"&amp;" && s.length() > pos + 4 && s[pos] == L'&' && s[pos + 1] == L'a' && s[pos + 2] == L'm' && s[pos + 3] == L'p' && s[pos + 4] == L';'))
			s.replace(pos, s1.length(), s2);
		pos = s.find(s1, pos + l);
	}
}

inline std::wstring EncodeXmlString(const std::wstring& s)
{
	std::wstring sRes = s;

	replace_all(sRes, L"&", L"&amp;");
	replace_all(sRes, L"<", L"&lt;");
	replace_all(sRes, L">", L"&gt;");
	replace_all(sRes, L"\"", L"&quot;");
	replace_all(sRes, L"\'", L"&#39;");
	replace_all(sRes, L"\n", L"&#xA;");
	replace_all(sRes, L"\r", L"&#xD;");
	replace_all(sRes, L"\t", L"&#x9;");

	return sRes;
}

inline void WriteToStringBuilder(NSStringUtils::CStringBuilder& oSrcStringBuilder, NSStringUtils::CStringBuilder& oDstStringBuilder)
{
	if (oSrcStringBuilder.GetCurSize() < MAX_STRING_BLOCK_SIZE)
	{
		oDstStringBuilder.Write(oSrcStringBuilder);
		return;
	}

	size_t ulSize = oSrcStringBuilder.GetCurSize();
	size_t ulCurrentBlockSize = 0, ulPosition = 0;

	while (ulSize > 0)
	{
		ulCurrentBlockSize = std::min(ulSize, MAX_STRING_BLOCK_SIZE);
		oDstStringBuilder.WriteString(oSrcStringBuilder.GetSubData(ulPosition, ulCurrentBlockSize));

		ulSize -= ulCurrentBlockSize;
		ulPosition += ulCurrentBlockSize;
	}
}

}

#endif // COMMON_H
