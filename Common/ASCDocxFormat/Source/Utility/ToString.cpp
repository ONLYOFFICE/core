
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "ToString.h"
#include <boost/lexical_cast.hpp>
#include "Encoding.h"
//#include <boost/algorithm/string/case_conv.hpp>
#include <wchar.h>


const std::string ToString(const bool value)
{
	if (value)
		return "true";
	return "false";
}


const std::wstring ToWString(const bool value)
{
	if (value)
		return L"true";
	return L"false";
}


const std::string ToString(const int value)
{
	return boost::lexical_cast<std::string>(value);
}


const std::wstring ToWString(const int value)
{
	return boost::lexical_cast<std::wstring>(value);
}


const std::string ToString(const size_t value)
{
	return boost::lexical_cast<std::string>(value);
}


const std::string ToString(const std::wstring& value)
{
	return Encoding::unicode2utf8(value);
}


const std::string ToString(const wchar_t* value)
{
	return Encoding::unicode2utf8(value);
}


const std::string ToString(const boost::filesystem::path& value)
{
	return value.string();
}


const std::string ToString(const boost::filesystem::wpath& value)
{
	return Encoding::unicode2utf8(value.string());
}


const std::wstring ToWString(const size_t value)
{
	return boost::lexical_cast<std::wstring>(value);
}


const std::string ToString(const double value)
{
	return boost::lexical_cast<std::string>(value);
}


const std::wstring ToWString(const double value)
{
	return boost::lexical_cast<std::wstring>(value);
}


const std::string ToString(const std::string& value)
{
	return value;
}


const std::wstring ToWString(const std::wstring& value)
{
	return value;
}


const std::wstring ToWString(const std::string& value)
{
	return Encoding::utf82unicode(value);
}


const std::wstring ToWString(const char* value)
{
	return Encoding::utf82unicode(value);
}


const std::string ToString(const char* value)
{
	return value;
}


const std::wstring ToWString(const wchar_t* value)
{
	return value;
}


const std::wstring ToWString(const boost::filesystem::path& value)
{
	return Encoding::utf82unicode(value.string());
}


const std::wstring ToWString(const boost::filesystem::wpath& value)
{
	return value.string();
}


const std::string ToUpper(const char* value)
{
	return ToUpper(ToString(value));
}


const std::string ToUpper(const std::string& value)
{
	std::string result = "";
	result.reserve(value.size());
	for(size_t i = 0; i < value.size(); i++)
		result += char(towupper(value[i]));
	//boost::to_upper(value);
	return result;
}


const std::wstring ToUpper(const wchar_t* value)
{
	return ToUpper(ToWString(value));
}


const std::wstring ToUpper(const std::wstring& value)
{
	std::wstring result = L"";
	result.reserve(value.size());
	for(size_t i = 0; i < value.size(); i++)
		result += wchar_t(towupper(value[i]));
	//boost::to_upper(value);
	return result;
}


const std::string ToLower(const char* value)
{
	return ToLower(ToString(value));
}


const std::string ToLower(std::string value)
{
	std::string result = "";
	result.reserve(value.size());
	for(size_t i = 0; i < value.size(); i++)
		result += char(towlower(value[i]));
	//boost::to_lower(value);
	return result;
}


const std::wstring ToLower(const wchar_t* value)
{
	return ToLower(ToWString(value));
}


const std::wstring ToLower(std::wstring value)
{
	std::wstring result = L"";
	result.reserve(value.size());
	for(size_t i = 0; i < value.size(); i++)
		result += wchar_t(towlower(value[i]));
	//boost::to_lower(value);
	return result;
}