/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#ifndef ASC_STL_UTILS_INCLUDE_H_
#define ASC_STL_UTILS_INCLUDE_H_

#include <string>

namespace StlUtils
{
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
#if defined(_WIN32) || defined(_WIN64)
        return atoi(strValue.c_str());
#else
		return std::stoi(strValue);
#endif
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
#if defined(_WIN32) || defined(_WIN64)
        return atof(strValue.c_str());
#else
		return std::stod(strValue.c_str());
#endif
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
