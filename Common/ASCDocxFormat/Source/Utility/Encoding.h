#pragma once
#ifndef UTILITY_ENCODING_INCLUDE_H_
#define UTILITY_ENCODING_INCLUDE_H_

#include <string>

class Encoding
{
public:
	static const std::string ansi2utf8(const std::string& line);
	static const std::string cp2utf8(const std::string& line, const unsigned int codePage);
	static const std::wstring ansi2unicode(const std::string& line);
	static const std::wstring cp2unicode(const std::string& line, const unsigned int codePage);
	static const std::string utf82ansi(const std::string& line);
	static const std::wstring utf82unicode(const std::string& line);
	static const std::string unicode2ansi(const std::wstring& line);
	static const std::string unicode2utf8(const std::wstring& line);

private:
	static const std::string wstring2string(const std::wstring& line, const unsigned int codePage);
	static const std::wstring string2wstring(const std::string& line, const unsigned int codePage);
};	
#endif // UTILITY_ENCODING_INCLUDE_H_