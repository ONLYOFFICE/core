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
#include "LocalInfo.h"

#include <iostream>
#include <string>
#include <chrono>
#include <locale>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cwctype>

DateReader::DateReader(_INT32 lcid):lcid_{lcid}
{}

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

    tm time = {};
    //если известна локаль парсим даты отдельно согласно ей
    if(lcid_ != -1)
    {
        auto res =  parseLocalDate(date, time, Hasdate, Hastime );
        if(!res)
            return false;
    }
    else
    {

        //делим форматы на длинные и короткие для уменьшения перебора
        std::vector<std::wstring> * requiredFormats;
        if(date.size() > 10)
            requiredFormats = &DateFormats;
        else
            requiredFormats = &DateFormatsShort;

        bool found = false;
        // Перебор форматов даты, пока не найдется подходящий
        for (const auto& format : *requiredFormats)
        {
            std::wistringstream ss(date);

            // Пытаемся спарсить дату в локальном формате
            ss >> std::get_time(&time, format.c_str());

            if (ss.fail()) {
                continue;
            }
            found = true;

        }
        // Если не найден подходящий формат даты, возвращаем false
        if(!found)
            return false;
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
    }



enum class DateElemTypes
{
    none = 0,
    letter,
    digit,
    delimeter,
    space
};

enum class ParsingElem
{
    none = 0,
    date,
    time
};


void SetDateElem(tm &result, _INT32 value, const std::wstring datePattern,  bool &day, bool &month, bool &year, bool &Berror)
{
    for(auto dateFmtPart : datePattern)
    {
        if((dateFmtPart == L'0' || dateFmtPart == L'1') && !day)
        {
            day = true;
            result.tm_mday = value;
            return;
        }
        else if((dateFmtPart == L'2' || dateFmtPart == L'3') && !month)
        {
            month = true;
            result.tm_mon = value;
            return;
        }
        else if((dateFmtPart == L'4' || dateFmtPart == L'5') && !year)
        {
            year = true;
            result.tm_year = value;
            return;
        }
    }
    Berror = true;
}

void SetTimeElem(tm &result, _INT32 value, bool &BHour, bool &bMin,  bool &bSec, bool &Berror)
{
    if(!BHour)
    {
        result.tm_hour = value;
        BHour = true;
    }
    else if (!bMin)
    {
        result.tm_min = value;
        bMin = true;
    }
    else if (!bSec)
    {
        result.tm_sec = value;
        bSec = true;
    }
    else
        Berror = true;
}

bool DateReader::parseLocalDate(const std::wstring &date, tm &result, bool &Hasdate, bool &Hastime)
{
    bool bError = false;
    auto locInf = lcInfo::getLocalInfo(lcid_);
    ParsingElem parsingNow = ParsingElem::none;

    //разделитель времени отличается только в нескольких локалях
    wchar_t timeSeparator = L':';
    if(lcid_ == 1035 || lcid_ == 11)
        timeSeparator = L'.';

    //флаги собранных частей даты
    bool bSec = false;
    bool bMin = false;
    bool bHour = false;
    bool BDay = false;
    bool Bmonth = false;
    bool Byear = false;
    bool bAmPm = false;


    DateElemTypes CurrentElementType = DateElemTypes::none;
    DateElemTypes PrevType = DateElemTypes::none;
    std::vector<wchar_t> StringBuf;

    //посимвольно парсим дату
    for(auto i = 0; i < date.length(); i++)
    {
        auto charElement = date.at(i);
        DateElemTypes elementType;
        if(charElement == L' ')
            elementType = DateElemTypes::space;
        else if(charElement >= L'0' && charElement<= L'9')
            elementType = DateElemTypes::digit;
        else if(charElement == locInf.DateSeparator.at(0) || charElement == L'.' || charElement == L':')
            elementType = DateElemTypes::delimeter;
        else
            elementType = DateElemTypes::letter;
        if(CurrentElementType == DateElemTypes::none)//первый проход
        {
            StringBuf.push_back(charElement);
        }
        else
        {
            if(CurrentElementType == elementType)
            {
                //проверяем валидность размеров элементов даты
                if(elementType == DateElemTypes::digit && StringBuf.size() < 4)
                {
                    StringBuf.push_back(charElement);
                }
                else if(elementType == DateElemTypes::letter && StringBuf.size() < 9)
                {
                    StringBuf.push_back(charElement);
                }
                else if(elementType == DateElemTypes::space)
                {
                }
                else
                {
                    bError = true;
                }
            }
            else
            {
                if(CurrentElementType == DateElemTypes::digit && elementType == DateElemTypes::delimeter)
                {
                    if(timeSeparator != locInf.DateSeparator[0])
                    {
                        //парсим часть даты
                        _INT32 datePart  = std::stoi(std::wstring(StringBuf.begin(), StringBuf.end()));
                        StringBuf.clear();
                        if(charElement == locInf.DateSeparator.at(0))
                        {
                            Hasdate = true;
                            parsingNow = ParsingElem::date;
                            SetDateElem(result, datePart, locInf.ShortDatePattern, BDay, Bmonth, Byear, bError);
                        }
                        else if(charElement == timeSeparator)
                        {
                            Hastime = true;
                            parsingNow = ParsingElem::time;
                            SetTimeElem(result, datePart, bHour, bMin, bSec, bError);
                        }
                        else
                        {
                            bError = true;
                        }
                    }
                }
                if(CurrentElementType == DateElemTypes::delimeter)
                {
                    //просто добавляем в буфер то что было за разделителем
                    StringBuf.push_back(charElement);
                }
                if(CurrentElementType == DateElemTypes::space)
                {
                    if(PrevType == DateElemTypes::digit)
                    {
                        _INT32 datePart  = std::stoi(std::wstring(StringBuf.begin(), StringBuf.end()));
                        StringBuf.clear();
                        if(parsingNow == ParsingElem::date)
                            SetDateElem(result, datePart, locInf.ShortDatePattern, BDay, Bmonth, Byear, bError);
                        else if (parsingNow == ParsingElem::time)
                            SetTimeElem(result, datePart, bHour, bMin, bSec, bError);
                     }
                    if(PrevType == DateElemTypes::letter)
                    {
                       parseAmPm(StringBuf, result);
                       StringBuf.clear();
                    }
                    if(elementType == DateElemTypes::letter || elementType == DateElemTypes::digit)
                        StringBuf.push_back(charElement);
                }
                //анализируем собранный элемент
                PrevType = CurrentElementType;
            }
        }
        CurrentElementType = elementType;

        if(bError)
            return false;
    }
    //анализируем последний элемент в буфере
    if(parsingNow == ParsingElem::date)
    {
        if(CurrentElementType == DateElemTypes::digit)
        {
            _INT32 datePart  = std::stoi(std::wstring(StringBuf.begin(), StringBuf.end()));
           SetDateElem(result, datePart, locInf.ShortDatePattern, BDay, Bmonth, Byear, bError);
        }
    }
    else if(parsingNow == ParsingElem::time)
    {
        if(CurrentElementType == DateElemTypes::digit)
        {
            _INT32 datePart  = std::stoi(std::wstring(StringBuf.begin(), StringBuf.end()));
            SetTimeElem(result, datePart, bHour, bMin, bSec, bError);
        }
        else if(CurrentElementType == DateElemTypes::letter)
        {
            parseAmPm(StringBuf, result);
            StringBuf.clear();
        }
    }
    //нормализуем год если он есть
    if(Hasdate)
    {
        result.tm_mon--;
        result.tm_year = normalizeYear(result.tm_year);
    }

    return true;
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

_INT32 DateReader::normalizeYear(_INT32 year)
{
    // год полностью
    if(year > 1900)
        return year - 1900;
    else if (year < 69)
        return 100 + year;
    else
        return year;
}

void DateReader::parseAmPm(std::vector<wchar_t> &stringBuf, tm &date)
{
    for(auto bufelemPos = 0; bufelemPos < stringBuf.size(); bufelemPos++)
        stringBuf[bufelemPos] = std::towlower(stringBuf[bufelemPos]);
    std::wstring timePostfix(stringBuf.begin(), stringBuf.end());
    if(timePostfix == L"pm")
    {
        date.tm_hour += 12;
    }
}
