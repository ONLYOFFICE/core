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
    return std::to_string(value);
}

const std::wstring ToWString(const int value)
{
    return std::to_wstring(value);
}

const std::string ToString(const size_t value)
{
    return std::to_string(value);
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
    return std::to_wstring(value);
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
