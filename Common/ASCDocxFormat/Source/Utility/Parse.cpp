
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "Parse.h"
#include "Encoding.h"
#include <boost/lexical_cast.hpp>


template<> const bool Parse<bool>(const std::string& str) 
{
	return str == "true" || str == "1" || str == "t" || str == "on";
}


template<> const int Parse<int>(const std::string& str) 
{
	if (str.length() == 0)
		return 0;
	return boost::lexical_cast<int>(str);
}


template<> const size_t Parse<size_t>(const std::string& str)
{
	return boost::lexical_cast<size_t>(str);
}


template<> const double Parse<double>(const std::string& str) 
{
	return boost::lexical_cast<double>(str);
}


template<> const std::wstring Parse<std::wstring>(const std::string& str)
{
	return Encoding::utf82unicode(str);
}


template<> const boost::filesystem::path Parse<boost::filesystem::path>(const std::string& str)
{
	return str;
}


template<> const boost::filesystem::wpath Parse<boost::filesystem::wpath>(const std::string& str)
{
	return Encoding::utf82unicode(str);
}


template<> const bool Parse<bool>(const std::wstring& str)
{
	return str == L"true" || str == L"1" || str == L"t" || str == L"on";
}


template<> const int Parse<int>(const std::wstring& str)
{
	return boost::lexical_cast<int>(str);
}


template<> const size_t Parse<size_t>(const std::wstring& str)
{
	return boost::lexical_cast<size_t>(str);
}


template<> const double Parse<double>(const std::wstring& str)
{
	return boost::lexical_cast<double>(str);
}


template<> const std::string Parse<std::string>(const std::wstring& str)
{
	return Encoding::unicode2utf8(str);
}


template<> const boost::filesystem::path Parse<boost::filesystem::path>(const std::wstring& str)
{
	return Encoding::unicode2utf8(str);
}


template<> const boost::filesystem::wpath Parse<boost::filesystem::wpath>(const std::wstring& str)
{
	return str;
}


const int HexChar2Int(const char value)
{
	if (value >= '0' && value <= '9')
		return value - '0';
	if (value >= 'a' && value <= 'f')
		return 10 + value - 'a';
	if (value >= 'A' && value <= 'F')
		return 10 + value - 'A';
	return 0;
}

const int HexString2Int(const std::string& value)
{
	int summa = 0;
	for (int i = 0; i != value.size(); ++i)
		summa += HexChar2Int(value[i]) << (4 * (value.size() - i - 1));
	
	return summa;
}