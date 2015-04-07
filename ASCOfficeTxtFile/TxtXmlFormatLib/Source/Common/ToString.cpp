
#include "precompiled_utility.h"

#include "ToString.h"
#include "Encoding.h"
#include "StlUtils.h"

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
	return StlUtils::IntToString(value);
}

const std::wstring ToWString(const int value)
{
	return StlUtils::IntToWideString(value);
}

const std::string ToString(const size_t value)
{
	return StlUtils::IntToString(value);
}

const std::string ToString(const std::wstring& value)
{
	return Encoding::unicode2utf8(value);
}

const std::string ToString(const wchar_t* value)
{
	return Encoding::unicode2utf8(value);
}

const std::wstring ToWString(const size_t value)
{
	return StlUtils::IntToWideString(value);
}

const std::string ToString(const double value)
{
	return StlUtils::DoubleToString(value);
}

const std::wstring ToWString(const double value)
{
	return StlUtils::DoubleToWideString(value);
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
