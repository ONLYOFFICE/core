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

#include <set>
#include <string>
#include <vector>
#include <cmath>
#include <cwctype>
#include <regex>

const auto NonDatecellLimit = 1000;
const auto MaxDateLength = 32;
const auto MinDateLength = 5;

DateReader::DateReader(_INT32 lcid):lcid_{lcid}
{}

bool DateReader::GetDigitalDate(const std::wstring &date, double &result, bool &Hasdate, bool &Hastime)
{

    tm time = {};
    if(date.size() > MaxDateLength || date.size() < MinDateLength)
    {
        cellCounter_++;
        return false;
    }
    ///если не было найдено ни одной даты в n ячейках - перестаем их искать
    if(!dateFound_ && cellCounter_ > NonDatecellLimit)
        return false;
    if(!parseIsoDate(date,time))
    {
        if(!parseLocalDate(date, time, Hasdate, Hastime ))
        {
            cellCounter_++;
            return false;
        }
    }
    else
    {
        Hasdate = true;
        Hastime = true;
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
        dateFound_ = true;
        return true;
    }
    //время без даты
    else if(time.tm_year == 0 && time.tm_mday == 0 && time.tm_mon == 0)
    {
        result = getStandartTime(time);
        Hasdate = false;
        Hastime = true;
        dateFound_ = true;
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
        dateFound_ = true;
        return true;
    }
    }

bool tryGetInt(std::vector<wchar_t> &data, _INT32 &value)
{
    try
    {
        value  = std::stoi(std::wstring(data.begin(), data.end()));
        return true;
    }
     catch (std::exception)
    {
        return false;
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
	_INT16 Partscount = day + month + year;
    for(auto dateFmtPart : datePattern)
    {
        if((dateFmtPart == L'0' || dateFmtPart == L'1') && !day && value <= 31)
        {
            day = true;
            result.tm_mday = value;
            return;
        }
        else if((dateFmtPart == L'2' || dateFmtPart == L'3') && !month && value <= 12)
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
		if(Partscount)
		{
			Partscount--;
		}
		else
		{
			 Berror = true;
			 break;
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

std::wstring spaceCut(const std::wstring &str)
{
        auto first = str.find_first_not_of(L' ');
        if (first == std::wstring::npos) return L"";
        auto last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
}

std::set<wchar_t> separators = {L'.', L'/', L'-', L':'};

bool DateReader::parseLocalDate(const std::wstring &date, tm &result, bool &Hasdate, bool &Hastime)
{
    bool bError = false;
    auto locInf = lcInfo::getLocalInfo(lcid_);
    ParsingElem parsingNow = ParsingElem::none;
    auto cutteddateStr =  spaceCut(date);

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

    DateElemTypes CurrentElementType = DateElemTypes::none;
    DateElemTypes PrevType = DateElemTypes::none;
    std::vector<wchar_t> StringBuf;

    //посимвольно парсим дату
    for(auto i = 0; i < cutteddateStr.length(); i++)
    {
        auto charElement = cutteddateStr.at(i);
        DateElemTypes elementType;
        if(charElement == L' ' || charElement == L' ')
            elementType = DateElemTypes::space;
        else if(charElement >= L'0' && charElement<= L'9')
            elementType = DateElemTypes::digit;
        else if(separators.find(charElement) != separators.end())
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
                        _INT32 datePart;
                        if(!tryGetInt(StringBuf, datePart))
                        {
                            return false;
                        }
                        StringBuf.clear();
                        if(charElement == timeSeparator)
                        {
                            Hastime = true;
                            parsingNow = ParsingElem::time;
                            SetTimeElem(result, datePart, bHour, bMin, bSec, bError);
                        }
                        else
                        {
                            Hasdate = true;
                            parsingNow = ParsingElem::date;
                            SetDateElem(result, datePart, locInf.ShortDatePattern, BDay, Bmonth, Byear, bError);
                        }

                    }
                    ///todo вариант когда и дата и время разделяются "."
                }
                if(CurrentElementType == DateElemTypes::letter && elementType == DateElemTypes::delimeter)
                {
                    if(!parseAmPm(StringBuf, result))
                    {
                        if(parseMonthName(StringBuf, result))
                            Bmonth = true;
                        else
                            bError = true;
                    }
                    StringBuf.clear();
                }
                else if((CurrentElementType == DateElemTypes::delimeter || CurrentElementType == DateElemTypes::space) && elementType != DateElemTypes::space)
                {
                    //просто добавляем в буфер то что было за разделителем
                    StringBuf.push_back(charElement);
                }
                else if(elementType== DateElemTypes::space)
                {
                    if(CurrentElementType == DateElemTypes::digit)
                    {
                        _INT32 datePart;
                        if(!tryGetInt(StringBuf, datePart))
                        {
                            return false;
                        }
                        StringBuf.clear();
                        if(parsingNow == ParsingElem::time)
                        {
                            SetTimeElem(result, datePart, bHour, bMin, bSec, bError);
                            parsingNow = ParsingElem::none;
                        }
                        else if (parsingNow == ParsingElem::date)
                            SetDateElem(result, datePart, locInf.ShortDatePattern, BDay, Bmonth, Byear, bError);
                        else
                        {
                            Hasdate = true;
                            parsingNow = ParsingElem::date;
                            SetDateElem(result, datePart, locInf.ShortDatePattern, BDay, Bmonth, Byear, bError);
                        }
                     }
                    if(CurrentElementType == DateElemTypes::letter)
                    {
                        // если это не am pm то в дате может быть буквенным только имя месяца
                        if(!parseAmPm(StringBuf, result))
                        {
                            if(parseMonthName(StringBuf, result))
                                Bmonth = true;
                            else
                                bError = true;
                        }
                       StringBuf.clear();
                    }
                }
                else if(elementType == DateElemTypes::letter)
                {
                    //невалидная дата
                    bError = true;
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
            _INT32 datePart;
            if(!tryGetInt(StringBuf, datePart))
            {
                return false;
            }
           SetDateElem(result, datePart, locInf.ShortDatePattern, BDay, Bmonth, Byear, bError);
        }
		else if(CurrentElementType == DateElemTypes::letter)
		{
			if(!parseMonthName(StringBuf, result))
				return false;
		}
    }
    else
    {
        if(CurrentElementType == DateElemTypes::digit)
        {
            _INT32 datePart;
            if(!tryGetInt(StringBuf, datePart))
            {
                return false;
            }
            SetTimeElem(result, datePart, bHour, bMin, bSec, bError);
        }
        else if(CurrentElementType == DateElemTypes::letter)
        {
            if(!parseAmPm(StringBuf, result))
            {
                if(parseMonthName(StringBuf, result))
                    Bmonth = true;
            }
            StringBuf.clear();
        }
    }
    //нормализуем год если он есть
    if(Hasdate)
    {
        result.tm_mon--;
        result.tm_year = normalizeYear(result.tm_year);
    }
    if((!Hasdate && !Hastime) || bError)
        return false;

    return true;
}
bool DateReader::parseIsoDate(const std::wstring &date, tm &result)
{
    std::wregex iso_regex(
    LR"(^(\d{4})-(\d{2})-(\d{2})(?:[T ](\d{2}):(\d{2})(?::(\d{2})(?:\.(\d{1,3}))?)?(?:(Z)|([+-])(\d{2})(?::?(\d{2}))?)?)?$)"                        // таймзона: Z или ±ч:мин
    );

    std::wsmatch match;
    if (!std::regex_match(date, match, iso_regex))
        return false;
    if (match.size() < 12) return false;
    result.tm_year = normalizeYear(std::stoi(match[1]));
    result.tm_mon = std::stoi(match[2]) - 1;
    result.tm_mday = std::stoi(match[3]);
    if (match[4].matched) {
        result.tm_hour = std::stoi(match[4]);
        result.tm_min  = std::stoi(match[5]);
        result.tm_sec  = match[6].matched ? std::stoi(match[6]) : 0;

        if (match[9].matched) {
            if (match[8] != L"Z")
            {

                int hours = 0;
                if(match[10].matched)
                    hours = std::stoi(match[10]);
                int mins  = 0;
                if(match[11].matched)
                    mins = std::stoi(match[11]);
                if (match[9] == L"-")
                {
                    result.tm_hour -= hours;
                    result.tm_min -= mins;
                }
                else
                {
                    result.tm_hour += hours;
                    result.tm_min += mins;
                }
            }
        }
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
    auto excelTime = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
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

bool DateReader::parseAmPm(std::vector<wchar_t> &stringBuf, tm &date)
{
    for(auto bufelemPos = 0; bufelemPos < stringBuf.size(); bufelemPos++)
        stringBuf[bufelemPos] = std::towlower(stringBuf[bufelemPos]);
    std::wstring timePostfix(stringBuf.begin(), stringBuf.end());
    if(timePostfix == L"pm")
    {
        date.tm_hour += 12;
    }
    if(timePostfix== L"am" || timePostfix == L"pm")
        return true;
    return false;
}

bool DateReader::parseMonthName(std::vector<wchar_t> &stringBuf, tm &date)
{
    auto locInf = lcInfo::getLocalInfo(lcid_);
    if(stringBuf.at(stringBuf.size()-1) == '.')
        stringBuf.pop_back();
    std::wstring monthName(stringBuf.begin(), stringBuf.end());

    bool isShort = false;
    if(monthName.size() <= locInf.MonthAbrvLen)
        isShort = true;
    auto monthindex = locInf.GetMonthNumber(monthName, isShort)+ 1;
    if(monthindex <= 0)
        return false;
    if(date.tm_mon != 0 && date.tm_mday == 0)
        date.tm_mday = date.tm_mon;
    date.tm_mon = monthindex;
    return true;
}
