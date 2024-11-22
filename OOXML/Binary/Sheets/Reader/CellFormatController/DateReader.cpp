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

    L"%X %p",

    L"%d %B %Y %X", L"%d %B, %Y %X", L"%d %b %Y %X",
    L"%d %b, %Y %X", L"%B %d %Y %X", L"%B %d, %Y %X", L"%b %d %Y %X",
    L"%b %d, %Y %X",

    L"%d.%m.%Y %X %p", L"%d.%m.%y %X %p", L"%Y-%m-%d %X %p", L"%Y-%m-%d %X %p", L"%d/%m/%Y %X %p",L"%d/%m/%y %X %p",
    L"%m/%d/%Y %X %p", L"%m/%d/%y %X %p", L"%Y/%m/%d %X %p", L"%Y/%d/%m %X %p", L"%m-%d-%Y %X %p",
    L"%m-%d-%y %X %p", L"%d-%m-%Y %X %p", L"%d-%m-%y %X %p",

    L"%d %B %Y %X %p", L"%d %B, %Y %X %p", L"%d %b %Y %X %p",
    L"%d %b, %Y %X %p", L"%B %d %Y %X %p", L"%B %d, %Y %X %p", L"%b %d %Y %X %p",
    L"%b %d, %Y %X %p",

    L"%d.%m.%Y %X", L"%d.%m.%y %X", L"%Y-%m-%d %X", L"%Y-%m-%d %X", L"%d/%m/%Y %X",L"%d/%m/%y %X", L"%m/%d/%Y %X",
    L"%m/%d/%y %X", L"%Y/%m/%d %X", L"%Y/%d/%m %X", L"%m-%d-%Y %X",
    L"%m-%d-%y %X", L"%d-%m-%Y %X", L"%d-%m-%y %X",

    L"%d %B %Y", L"%d %B, %Y", L"%d %b %Y",
    L"%d %b, %Y", L"%B %d %Y", L"%B %d, %Y", L"%b %d %Y",
    L"%b %d, %Y"
};

// короткие форматы
std::vector<std::wstring> DateFormatsShort = {

   L"%X",

   L"%d.%m.%Y", L"%d.%m.%y", L"%Y-%m-%d", L"%Y-%m-%d", L"%d/%m/%Y",L"%d/%m/%y", L"%m/%d/%Y",
   L"%m/%d/%y", L"%Y/%m/%d", L"%Y/%d/%m", L"%m-%d-%Y",
   L"%m-%d-%y", L"%d-%m-%Y", L"%d-%m-%y", L"%x"
};

bool DateReader::GetDigitalDate(const std::wstring &date, double &result, bool &Hasdate, bool &Hastime)
{
    //делим форматы на длинные и короткие для уменьшения перебора
    std::vector<std::wstring> * requiredFormats;
    if(date.size() > 10)
        requiredFormats = &DateFormats;
    else
        requiredFormats = &DateFormatsShort;

    // Перебор форматов даты, пока не найдется подходящий
    for (const auto& format : *requiredFormats) {
        std::wistringstream ss(date);

        // Пытаемся спарсить дату в локальном формате
        tm time = {};
        ss >> std::get_time(&time, format.c_str());

        if (ss.fail()) {
            continue;
        }

        //дата без времени
        if(time.tm_year > 0 && time.tm_hour == 0 && time.tm_min == 0 && time.tm_sec == 0)
        {
            //определяем стандартная ли дата
            if(time.tm_year >= 70)
                result = getStandartDate(time);
            else
              result = getNonUnixDate(time);
            Hasdate = true;
            Hastime = false;
            return true;
        }
        //время без даты
        else if(time.tm_year == 0 && time.tm_mday == 0 && time.tm_mon == 0)
        {
            result = getStandartTime(time);
            Hasdate = false;
            Hastime = true;
            return true;
        }
        else //дата и время
        {

            if(time.tm_year >= 70)
                result = getStandartDate(time);
            else
              result = getNonUnixDate(time);
            result += getStandartTime(time);
            Hasdate = true;
            Hastime = true;
            return true;
        }
        return false;
    }

    // Если не найден подходящий формат даты, возвращаем false
    return false;
}

_INT32 DateReader::getStandartDate(tm date)
{
    // обнуление времени, чтобы оно не влияло на дату
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;
    // Преобразование даты в формат excel
    auto timeT = mktime(&date);
    auto tp = std::chrono::system_clock::from_time_t(timeT);
    auto excelTime = tp.time_since_epoch().count();
    #if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
        excelTime = excelTime / 10000000;
    #else
        excelTime = excelTime / 1000000000;
    #endif
    excelTime += 2209161600;
    _INT32 tempTime = round(excelTime / 86400.0);
    return tempTime;
}

double DateReader::getStandartTime(tm date)
{
    if(date.tm_hour == 24)
        date.tm_hour = 0;
    double result = ((date.tm_sec + (60 * date.tm_min) + (3600*date.tm_hour))/ 86400.0);
    return result;
}


// Функция для определения високосного года
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

_INT32 DateReader::getNonUnixDate(tm date)
{
    const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

       long days = 1;

       // Добавляем количество дней за предыдущие годы
       for (int year = 1900; year < date.tm_year + 1900; ++year) {
           days += isLeapYear(year) ? 366 : 365;
       }

       // Добавляем количество дней до начала текущего года
       for (int month = 0; month < date.tm_mon; ++month) {
           days += daysInMonth[month];
           if (month == 1 && isLeapYear(date.tm_year + 1900))
               days++; // добавляем 1 день для февраля в високосном году
       }

       // Добавляем количество дней текущего месяца
       days += date.tm_mday;

       return days;
}
