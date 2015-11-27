#pragma once

#ifndef ASC_STL_UTILS_INCLUDE_H_
#define ASC_STL_UTILS_INCLUDE_H_

#include <string>

namespace StlUtils
{
    static inline std::wstring IntToWideString(int value)
	{
#if defined(_WIN32) || defined(_WIN64)
        wchar_t strValue[256];
		_itow_s(value, strValue, 256, 10);
		return std::wstring(strValue);
#else
        return std::to_wstring(value);
#endif
	}

	static inline std::wstring DoubleToWideString(double value)
	{
#if defined(_WIN32) || defined(_WIN64)
        wchar_t strValue[256];
		swprintf_s(strValue, 256, L"%f", value);
		return std::wstring(strValue);
#else
        return std::to_wstring(value);
#endif
    }

    static inline std::string IntToString(int value)
	{
#if defined(_WIN32) || defined(_WIN64)
        char strValue[256];
		_itoa_s(value, strValue, 256, 10);
        return std::string(strValue);
#else
        return std::to_string(value);
#endif
	}

	static inline std::string DoubleToString(double value)
	{
#if defined(_WIN32) || defined(_WIN64)
        char strValue[256];
		sprintf_s(strValue, 256, "%f", value);
		return std::string(strValue);
#else
        return std::to_string(value);
#endif
    }

	static int ToInteger(const std::string& strValue)
	{
		return std::stoi(strValue);
	}

	static int ToInteger(const std::wstring& strValue)
	{
#if defined(_WIN32) || defined(_WIN64)
        return _wtoi(strValue.c_str());
#else
        return std::stoi(strValue);
#endif
    }

	static double ToDouble(const std::string& strValue)
	{
		return std::stod(strValue.c_str());
	}

	static double ToDouble(const std::wstring& strValue)
	{
#if defined(_WIN32) || defined(_WIN64)
        return _wtof(strValue.c_str());
#else
        return std::stod(strValue);
#endif
    }
}

#endif // ASC_STL_UTILS_INCLUDE_H_
