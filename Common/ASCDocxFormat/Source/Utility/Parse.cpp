
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "Parse.h"
#include "Encoding.h"

#include "ASCStlUtils.h"

template<> const bool Parse<bool>(const std::string& str) 
{
	return str == "true" || str == "1" || str == "t" || str == "on";
}

template<> const int Parse<int>(const std::string& str) 
{
	if (str.length() == 0)
		return 0;

	return StlUtils::ToInteger(str);
}

template<> const size_t Parse<size_t>(const std::string& str)
{
	return (size_t)StlUtils::ToInteger(str);
}

template<> const double Parse<double>(const std::string& str) 
{
	return StlUtils::ToDouble(str);
}

template<> const std::wstring Parse<std::wstring>(const std::string& str)
{
	return Encoding::utf82unicode(str);
}

template<> const bool Parse<bool>(const std::wstring& str)
{
	return str == L"true" || str == L"1" || str == L"t" || str == L"on";
}

template<> const int Parse<int>(const std::wstring& str)
{
	return StlUtils::ToInteger(str);
}

template<> const size_t Parse<size_t>(const std::wstring& str)
{
	return (size_t)StlUtils::ToInteger(str);
}


template<> const double Parse<double>(const std::wstring& str)
{
	return StlUtils::ToDouble(str);//
}

template<> const std::string Parse<std::string>(const std::wstring& str)
{
	return Encoding::unicode2utf8(str);
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

const int HexString2IntW(const std::wstring& value)
{
	int summa = 0;
	for (int i = 0; i != value.size(); ++i)
		summa += HexChar2Int(value[i]) << (4 * (value.size() - i - 1));

	return summa;
}