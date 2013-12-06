
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "Encoding.h"
#include <windows.h>
#include "Utility.h"

const std::string Encoding::ansi2utf8(const std::string& line)
{
	return wstring2string(string2wstring(line, CP_ACP), CP_UTF8);
}

const std::string Encoding::cp2utf8(const std::string& line, const unsigned int codePage)
{
	return wstring2string(string2wstring(line, codePage), CP_UTF8);
}

const std::wstring Encoding::ansi2unicode(const std::string& line)
{
	return string2wstring(line, CP_ACP);
}

const std::wstring Encoding::cp2unicode(const std::string& line, const unsigned int codePage)
{
	return string2wstring(line, codePage);
}

const std::string Encoding::utf82ansi(const std::string& line)
{
	return wstring2string(string2wstring(line, CP_UTF8), CP_ACP);
}

const std::wstring Encoding::utf82unicode(const std::string& line)
{
	return string2wstring(line, CP_UTF8);
}

const std::string Encoding::unicode2ansi(const std::wstring& line)
{
	return wstring2string(line, CP_ACP);
}

const std::string Encoding::unicode2utf8(const std::wstring& line)
{
	return wstring2string(line, CP_UTF8);
}

const std::string Encoding::wstring2string(const std::wstring& sLine, const unsigned int codePage)
{
	const int nSize = WideCharToMultiByte(codePage, 0, sLine.c_str(), sLine.length(), NULL, 0, NULL, NULL);
	char *sTemp = new char[nSize];
	if (!sTemp)
		return std::string();

	int size = WideCharToMultiByte(codePage, 0, sLine.c_str(), sLine.length(), sTemp, nSize, NULL, NULL);

	std::string sResult(sTemp, size);
	delete []sTemp;

	return sResult;
}

const std::wstring Encoding::string2wstring(const std::string& sline, const unsigned int codePage)
{
	const int nSize = MultiByteToWideChar(codePage, 0, sline.c_str(), sline.size(), NULL, 0);

	wchar_t *sTemp = new wchar_t[nSize];
	if (!sTemp)
		return std::wstring();

	int size = MultiByteToWideChar(codePage, 0, sline.c_str(), sline.size(), sTemp, nSize);

	std::wstring sResult(sTemp, size);
	delete []sTemp;

	return sResult;
}