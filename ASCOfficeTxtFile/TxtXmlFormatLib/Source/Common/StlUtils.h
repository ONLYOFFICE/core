#pragma once

#ifndef ASC_STL_UTILS_INCLUDE_H_
#define ASC_STL_UTILS_INCLUDE_H_

#include <string>

namespace StlUtils
{
	static inline std::wstring IntToWideString(int value, int radix = 10)
	{
		wchar_t strValue[256];
		_itow_s(value, strValue, 256, radix);
		return std::wstring(strValue);
	}

	static inline std::wstring DoubleToWideString(double value)
	{
		wchar_t strValue[256];
		swprintf_s(strValue, 256, L"%f", value);
		return std::wstring(strValue);
	}

	static inline std::string IntToString(int value, int radix = 10)
	{
		char strValue[256];
		_itoa_s(value, strValue, 256, radix);
		return std::string(strValue);
	}

	static inline std::string DoubleToString(double value)
	{
		char strValue[256];
		sprintf_s(strValue, 256, "%f", value);
		return std::string(strValue);
	}

	static int ToInteger(const std::string& strValue)
	{
		return atoi(strValue.c_str());
	}

	static int ToInteger(const std::wstring& strValue)
	{
		return _wtoi(strValue.c_str());
	}

	static double ToDouble(const std::string& strValue)
	{
		return atof(strValue.c_str());
	}

	static double ToDouble(const std::wstring& strValue)
	{
		return _wtof(strValue.c_str());
	}
}

#endif // ASC_STL_UTILS_INCLUDE_H_