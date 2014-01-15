
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "Encoding.h"
#include "windows.h"
#include "Utility.h"
#include <boost/scoped_array.hpp>


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


const std::string Encoding::wstring2string(const std::wstring& wline, const unsigned int codePage)
{
	boost::scoped_array<char> sline(new char[wline.size() * 4]);
	const int size = WideCharToMultiByte(codePage, 0, wline.c_str(), wline.size(), sline.get(), wline.size() * 4, NULL, NULL);
	return std::string(sline.get(), size);
}


const std::wstring Encoding::string2wstring(const std::string& sline, const unsigned int codePage)
{
	boost::scoped_array<wchar_t> wline(new wchar_t[sline.size()]);
	const int size = MultiByteToWideChar(codePage, 0, sline.c_str(), sline.size(), wline.get(), sline.size());
	return std::wstring(wline.get(), size);
}
const std::wstring Encoding::string2wstring2(const std::string& sline, const unsigned int codePage)
{
	int nBufSize = MultiByteToWideChar(codePage, 0, sline.c_str(), -1, NULL, NULL);
	boost::scoped_array<wchar_t> wline(new wchar_t[nBufSize]);
	MultiByteToWideChar(codePage, 0, sline.c_str(), -1, wline.get(), nBufSize);
	return std::wstring(wline.get(), nBufSize-1);
}
