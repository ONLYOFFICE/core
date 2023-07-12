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

#include "DateReader.h"

#include <iostream>
#include <string>
#include <chrono>
#include <locale>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

 // Определение основных форматов даты
std::vector<std::wstring> DateFormats = {
    L"%d.%m.%Y", L"%d.%m.%y", L"%Y-%m-%d", L"%m/%d/%Y",
    L"%m/%d/%y", L"%d %B %Y", L"%d %B, %Y", L"%d %b %Y",
    L"%d %b, %Y", L"%B %d %Y", L"%B %d, %Y", L"%b %d %Y",
    L"%b %d, %Y", L"%Y/%m/%d", L"%Y/%d/%m", L"%m-%d-%Y",
    L"%m-%d-%y", L"%d-%m-%Y", L"%d-%m-%y"
};

bool DateReader::GetDigitalDate(const std::wstring &date, _INT32 &result)
{
    // Перебор форматов даты, пока не найдется подходящий
    for (const auto& format : DateFormats) {
        std::wistringstream ss(date);

        tm time = {};
        ss >> std::get_time(&time, format.c_str());

        if (ss.fail()) {
            continue;
        }

        //определяем стандартная ли дата

        if(time.tm_year > 0)
        {
            result = getStandartDate(time);
            return true;
        }
        return false;
    }

    // Если не найден подходящий формат даты, возвращаем false
    return false;
}

_INT32 DateReader::getStandartDate(tm &date)
{
    // Преобразование даты в формат excel
    auto tp = std::chrono::system_clock::from_time_t(mktime(&date));
    auto excelTime = (tp.time_since_epoch().count() / 10000000) + 2209161600;
    _INT32 tempTime = round(excelTime / 86400.0);
    return tempTime;
}