/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#include "CurrencyReader.h"

#include <set>

const std::wstring CurrencyFormatTemplate = L"#,##0.00";

const std::set<std::wstring> CurrencySymbols = {
    L"\u0024",    // Доллар США
    L"\u20AC",    // Евро
    L"\u00A5",    // Японская иена
    L"\u00A3",    // Фунт стерлингов Великобритании
    L"\u0024\u0061",   // Австралийский доллар
    L"\u0024\u0063",   // Канадский доллар
    L"\u0046\u0072\u002E",  // Швейцарский франк
    L"\u20BD",    // Российский рубль
    L"\u20B8",    // Казахский тенге
    L"\u5143",   // Китайский юань
    L"\u20B1", // Филиппинское песо
    L"\u20B9", // Индийская рупия
    L"\u20AA", // Израильский новый шекель
    L"\u20A7" // Испанская песета
};

bool CurrencyReader::CheckPostfix(const std::wstring &postfix)
{
    auto find = CurrencySymbols.find(postfix);
    return find != CurrencySymbols.end();
}

std::wstring CurrencyReader::GetCurrencyFormat(const std::wstring &inputFormat,const std::wstring &currency)
{
    std::wstring format;
    if(!inputFormat.empty())
    {
        format = inputFormat + currency;
    }
    else
    {
        format = CurrencyFormatTemplate + currency;
    }
    return format;
}