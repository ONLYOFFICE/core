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
    size_t length = value.length();
    wchar_t *pEndPtr;
	double dValue = wcstod(value.c_str(), &pEndPtr);


    if ((std::isnan(dValue) || std::isinf(dValue)))
    {
		return false;
    }

	if ((0 == *pEndPtr) || (pEndPtr != value.c_str() && (value.c_str() + length  - pEndPtr) <= 1))
	{
	std::wstring postfix;
	auto length = value.length();

		if (0 != *pEndPtr)
		{
			size_t sz = length - (pEndPtr - value.c_str());

			while (sz > 0)
			{
				if (pEndPtr[sz - 1] != L' ')
					break;
				sz--;
			}

			if (sz > 0)
			{
				postfix = std::wstring(pEndPtr, sz);
			}
		}

        auto data_format = createFractionFormat(value, postfix);
		double fractionValue = 0;

		if (0 != *pEndPtr)
		{
			if (false == postfix.empty())
			{
				CurrencyReader currency{};
				if (postfix[0] == L'%')
				{
					double PercentValue = dValue / 100.;
					digit = Double2wstr(PercentValue);
					format = DefaultPercentFormat;
                    return true;
				}
				else
					return false;
				/*
				else if(currency.CheckPostfix(postfix))
				{
					digit = std::to_wstring(dValue);
					format = currency.GetCurrencyFormat(data_format,postfix);
				}
				else if(checkCommonFractionFormat(dValue, postfix, fractionValue, format))
				{
					digit = std::to_wstring(fractionValue);
				}
				else
				{
					digit = std::to_wstring(dValue);

						// Remove trailing zeros after decimal point
					size_t dotPos = digit.find('.');
					if (dotPos != std::wstring::npos)
					{
						size_t lastNonZeroPos = digit.find_last_not_of(L'0');
                        if(lastNonZeroPos+3 < digit.size())
                            digit.erase(lastNonZeroPos + 2);
					}
                    data_format = createFractionFormat(digit, L"");
					for (size_t i = 0; i < postfix.size(); ++i)
					{
						data_format += std::wstring(L"\\") + postfix[i];
					}
					format = data_format;
				}
				*/
			}
			else
			{
				digit = Double2wstr(dValue);
				format = data_format;
			}
		}
		else
		{

			if(value.find(L'.') == std::wstring::npos)
			{
				std::wostringstream oss;
				oss << (_INT64)dValue;
				oss << std::setprecision(std::numeric_limits<double>::digits10 + 1);
				digit = oss.str();
			}
			else
				digit = Double2wstr(dValue);
			format = data_format;
        }
        return true;
    }
    return false;
}

std::wstring DigitReader::createFractionFormat(const std::wstring &value, const std::wstring &postfix)
{
    size_t pos = value.find(L".");
    auto length = value.length();
    std::wstring data_format = L"";
        if (pos != std::wstring::npos && pos < (value.size() - postfix.size() - 1))
		{
            size_t fraction = length - pos - 1 - postfix.size();
			for (size_t i = 0; i < fraction && fraction != std::wstring::npos; ++i)
				data_format += L"0";
		}
		if (false == data_format.empty()) data_format = L"." + data_format;
        if(pos != std::string::npos && pos < (value.size() - postfix.size() - 1))
		{
            std::wstring wholePart(pos, '0');
			data_format = wholePart + data_format;
		}
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
}

bool DigitReader::ReadScientific(const std::wstring &value, std::wstring &digit, std::wstring &format)
{
    if(nonscientificCellsCounter_ > NonDigitcellLimit && !scientificFound_)
        return false;
    {
        std::wregex scientificRegex(L"(^[+-]?(\\d+\\.?\\d*)([eE][+-]?\\d+))$");
        if(std::regex_search(value, scientificRegex))
        {
            try
            {
                auto doubleVal = std::stod(value);

                std::wstringstream ss;
                _INT32 MainPartSize = value.find(L"E");
                if(MainPartSize < 1)
                    MainPartSize = value.find(L"e");
                ss.precision(MainPartSize); // Set precision
                ss.setf(std::ios::scientific);
                ss << doubleVal;
                digit = ss.str();

                format = L"0.0";
                if(MainPartSize > 3)
                format += std::wstring(MainPartSize - 3, L'0');
                format +=L"E+00";
                scientificFound_ = true;
                return true;

            }
            catch (std::exception)
            {
                nonscientificCellsCounter_++;
                return false;
            }

        }
    }
    nonscientificCellsCounter_++;
	return false;
}
