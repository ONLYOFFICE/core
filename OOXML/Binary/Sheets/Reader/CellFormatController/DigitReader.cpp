/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "DigitReader.h"
#include "CurrencyReader.h"

#include <cmath>

const std::wstring DefaultPercentFormat = L"0.0%";

bool DigitReader::ReadDigit(const std::wstring &value, std::wstring &digit, std::wstring &format)
{
    size_t length = value.length();
    wchar_t *pEndPtr;
	double dValue = wcstod(value.c_str(), &pEndPtr);


    if ((std::isnan(dValue) || std::isinf(dValue)))
    {
		return false;
    }

	if ((0 == *pEndPtr) || (pEndPtr != value.c_str() && (value.c_str() + length  - pEndPtr) < 3))
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

        auto data_format = createFractionFormat(value, pEndPtr);
		double fractionValue = 0;

		if (0 != *pEndPtr)
		{
			if (false == postfix.empty())
			{
				CurrencyReader currency{};
				if (postfix[0] == L'%')
				{
					digit =  std::to_wstring(dValue / 100.);
					format = DefaultPercentFormat;
                    return true;
				}
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

						// Удаление лишних нулей после запятой
					size_t dotPos = digit.find('.');
					if (dotPos != std::wstring::npos)
					{
						size_t lastNonZeroPos = digit.find_last_not_of(L'0');
						digit.erase(lastNonZeroPos + 2);
					}

					data_format = createFractionFormat(digit, pEndPtr);
					for (size_t i = 0; i < postfix.size(); ++i)
					{
						data_format += std::wstring(L"\\") + postfix[i];
					}
					format = data_format;
				}
			}
		}
		else
        {
			digit = std::to_wstring(dValue);
            format = data_format;
        }
        return true;
    }
    return false;
}

std::wstring DigitReader::createFractionFormat(const std::wstring &value, wchar_t *pEndPtr)
{
    size_t pos = value.find(L".");
    auto length = value.length();
    std::wstring data_format = L"";
		if (pos != std::wstring::npos)
		{
			size_t fraction = length - pos - ((0 != *pEndPtr) ? 2 : 1);
			for (size_t i = 0; i < fraction && fraction != std::wstring::npos; ++i)
				data_format += L"0";
		}
		if (false == data_format.empty()) data_format = L"." + data_format;
		if(pos != std::string::npos)
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
