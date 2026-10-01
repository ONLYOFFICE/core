/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "DigitReader.h"
#include "CurrencyReader.h"

#include <cmath>
#include <regex>
#include <sstream>
#include <iomanip>

const std::wstring DefaultPercentFormat = L"0.0%";
const auto NonDigitcellLimit = 1000;
std::wstring Double2wstr(double dValue)
{
	std::wostringstream oss;
	oss << std::setprecision(std::numeric_limits<double>::digits10 + 1);
	oss << dValue;
	return oss.str();
}
bool DigitReader::ReadDigit(const std::wstring &value, std::wstring &digit, std::wstring &format)
{
	const wchar_t* pStart = value.c_str();
	size_t length = value.length();
	wchar_t *pEndPtr = nullptr;
	double dValue = wcstod(pStart, &pEndPtr);

	if (std::isnan(dValue) || std::isinf(dValue))
	{
		return false;
	}

	__int64 unparsedLen = (pStart + length) - pEndPtr;

	if (*pEndPtr != L'\0' && unparsedLen > 1)
	{
		return false;
	}

	std::wstring postfix;
	if (*pEndPtr != L'\0')
	{
		size_t sz = unparsedLen;
		while (sz > 0 && pEndPtr[sz - 1] == L' ')
		{
			sz--;
		}

		if (sz > 0)
		{
			postfix.assign(pEndPtr, sz);
		}
	}

	auto data_format = createFractionFormat(value, postfix);

	if (*pEndPtr != L'\0')
	{
		if (!postfix.empty())
		{
			if (postfix[0] == L'%')
			{
				double PercentValue = dValue / 100.;
				digit = Double2wstr(PercentValue);
				format = DefaultPercentFormat;
				return true;
			}
			return false;
		}
		else
		{
			digit = Double2wstr(dValue);
			format = data_format;
		}
	}
	else
	{
		if (value.find(L'.') == std::wstring::npos)
		{
			wchar_t buf[64];
			swprintf(buf, sizeof(buf) / sizeof(buf[0]), L"%lld", (_INT64)dValue);
			digit = buf;
		}
		else
		{
			digit = Double2wstr(dValue);
		}
		format = data_format;
	}

	return true;
}
std::wstring DigitReader::createFractionFormat(const std::wstring &value, const std::wstring &postfix)
{
	const size_t val_size = value.size();
	const size_t post_size = postfix.size();
	const size_t pos = value.find(L'.');

	// fast exit without point
	if (pos == std::wstring::npos || (pos + post_size + 1 >= val_size))
	{
		return std::wstring();
	}

	const size_t whole_len = pos;
	const size_t frac_len = val_size - pos - 1 - post_size;

	std::wstring data_format;

	data_format.reserve(whole_len + 1 + frac_len);

	data_format.append(whole_len, L'0');
	data_format.push_back(L'.');
	data_format.append(frac_len, L'0');

	return data_format;
}

bool DigitReader::checkCommonFractionFormat(const double &numerator, const std::wstring &postfix, double &dvalue, std::wstring &format)
{
	if (postfix[0] != L'/')
	{
		return false;
	}

	wchar_t *pEndPtr;
	auto denominator = wcstod(postfix.substr(1, postfix.size()-1).c_str(), &pEndPtr);

	if ((std::isnan(denominator) || std::isinf(denominator)) || denominator == 0 || *pEndPtr != 0)
    {
		return false;
    }
    format = L"?/?";
	dvalue = numerator/denominator;
	return true;
}

bool DigitReader::ReadScientific(const std::wstring &value, std::wstring &digit, std::wstring &format)
{
	if (nonscientificCellsCounter_ > NonDigitcellLimit && !scientificFound_)
		return false;

	static const std::wregex scientificRegex(L"^[+-]?(\\d+\\.?\\d*)([eE][+-]?\\d+)$");

	if (std::regex_search(value, scientificRegex))
	{
		double doubleVal = 0;
		try
		{
			doubleVal = std::stod(value);
		}
		catch (const std::exception&)
		{
			nonscientificCellsCounter_++;
			return false;
		}
		size_t MainPartSize = value.find(L'E');
		if (MainPartSize == std::wstring::npos)
		{
			MainPartSize = value.find(L'e');
		}

		std::wstringstream ss;
		ss.precision(MainPartSize);
		ss.setf(std::ios::scientific);
		ss << doubleVal;
		digit = ss.str();

		format = L"0.0";
		if (MainPartSize > 3)
		{
			format.append(MainPartSize - 3, L'0');
		}
		format += L"E+00";

		scientificFound_ = true;
		return true;
	}

	nonscientificCellsCounter_++;
	return false;
}
