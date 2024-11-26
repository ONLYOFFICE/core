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

#pragma once

#include "LocalInfo.h"
#include <map>

namespace lcInfo
{


std::vector<std::vector<std::wstring>> MonthNames
{
    {L"january", L"jebruary", L"march", L"april", L"may", L"june", L"july", L"august", L"september", L"october", L"november", L"december"},
    {L"январь", L"февраль", L"март", L"апрель", L"май", L"июнь", L"июль", L"август", L"сентябрь", L"октябрь", L"ноябрь", L"декабрь"},
    {L"enero", L"febrero", L"marzo", L"abril", L"mayo", L"junio", L"julio", L"agosto", L"septiembre", L"octubre", L"noviembre", L"diciembre"},
    {L"一月", L"二月", L"三月", L"四月", L"五月", L"六月", L"七月", L"八月", L"九月", L"十月", L"十一月", L"十二月"},
    {L"leden", L"únor", L"březen", L"duben", L"květen", L"červen", L"červenec", L"srpen", L"září", L"říjen", L"listopad", L"prosinec"},
    {L"januar", L"februar", L"marts", L"april", L"maj", L"juni", L"juli", L"august", L"september", L"oktober", L"november", L"december"},
    {L"Januar", L"Februar", L"März", L"April", L"Mai", L"Juni", L"Juli", L"August", L"September", L"Oktober", L"November", L"Dezember"},
    {L"Ιανουάριος", L"Φεβρουάριος", L"Μάρτιος", L"Απρίλιος", L"Μάιος", L"Ιούνιος", L"Ιούλιος", L"Αύγουστος", L"Σεπτέμβριος", L"Οκτώβριος", L"Νοέμβριος", L"Δεκέμβριος"},
    {L"tammikuu", L"helmikuu", L"maaliskuu", L"huhtikuu", L"toukokuu", L"kesäkuu", L"heinäkuu", L"elokuu", L"syyskuu", L"lokakuu", L"marraskuu", L"joulukuu"},
    {L"janvier", L"février", L"mars", L"avril", L"mai", L"juin", L"juillet", L"août", L"septembre", L"octobre", L"novembre", L"décembre"},
    {L"január", L"február", L"március", L"április", L"május", L"június", L"július", L"augusztus", L"szeptember", L"október", L"november", L"december"},
    {L"gennaio", L"febbraio", L"marzo", L"aprile", L"maggio", L"giugno", L"luglio", L"agosto", L"settembre", L"ottobre", L"novembre", L"dicembre"},
    {L"1月", L"2月", L"3月", L"4月", L"5月", L"6月", L"7月", L"8月", L"9月", L"10月", L"11月", L"12月"},
    {L"1월", L"2월", L"3월", L"4월", L"5월", L"6월", L"7월", L"8월", L"9월", L"10월", L"11월", L"12월"},
    {L"styczeń", L"luty", L"marzec", L"kwiecień", L"maj", L"czerwiec", L"lipiec", L"sierpień", L"wrzesień", L"październik", L"listopad", L"grudzień"},
    {L"janeiro", L"fevereiro", L"março", L"abril", L"maio", L"junho", L"julho", L"agosto", L"setembro", L"outubro", L"novembro", L"dezembro"}
};

std::vector<std::vector<std::wstring>> ShortMonthNames
{
    {L"jan", L"feb", L"mar", L"apr", L"may", L"jun", L"jul", L"aug", L"sep", L"oct", L"nov", L"dec"},
    {L"янв", L"фев", L"мар", L"апр", L"май", L"июн", L"июл", L"авг", L"сен", L"окт", L"ноя", L"дек"},
    {L"ene", L"feb", L"mar", L"abr", L"may", L"jun", L"jul", L"ago", L"sep", L"oct", L"nov", L"dic"},
    {L"1月", L"2月", L"3月", L"4月", L"5月", L"6月", L"7月", L"8月", L"9月", L"10月", L"11月", L"12月"},
    {L"led", L"úno", L"bře", L"dub", L"kvě", L"čvn", L"čvc", L"srp", L"zář", L"říj", L"lis", L"pro"},
    {L"jan", L"feb", L"mar", L"apr", L"maj", L"jun", L"jul", L"aug", L"sep", L"okt", L"nov", L"dec"},
    {L"Jan", L"Feb", L"Mrz", L"Apr", L"Mai", L"Jun", L"Jul", L"Aug", L"Sep", L"Okt", L"Nov", L"Dez"},
    {L"Ιαν", L"Φεβ", L"Μαρ", L"Απρ", L"Μαϊ", L"Ιουν", L"Ιουλ", L"Αυγ", L"Σεπ", L"Οκτ", L"Νοε", L"Δεκ"},
    {L"tammi", L"helmi", L"maalis", L"huhti", L"touko", L"kesä", L"heinä", L"elo", L"syys", L"loka", L"marras", L"joulu"},
    {L"janv", L"févr", L"mars", L"avr", L"mai", L"juin", L"juil", L"août", L"sept", L"oct", L"nov", L"déc"},
    {L"jan", L"febr", L"márc", L"ápr", L"máj", L"jún", L"júl", L"aug", L"szept", L"okt", L"nov", L"dec"},
    {L"gen", L"feb", L"mar", L"apr", L"mag", L"giu", L"lug", L"ago", L"set", L"ott", L"nov", L"dic"},
    {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12"},
    {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12"},
    {L"sty", L"lut", L"mar", L"kwi", L"maj", L"cze", L"lip", L"sie", L"wrz", L"paź", L"lis", L"gru"},
    {L"jan", L"fev", L"mar", L"abr", L"mai", L"jun", L"jul", L"ago", L"set", L"out", L"nov", L"dez"}
};

std::map<_INT32, LocalInfo> InfoMap
{
    {-1, LocalInfo{-1, L"default", L"-", L"215", 0, 3}},
    {4, LocalInfo{4, L"zh-Hans", L"/", L"520", 3, 3}},
    {5, LocalInfo{5, L"cs", L".", L"135", 4, 3}},
    {6, LocalInfo{6, L"da", L"-", L"135",  5, 3}},
    {7, LocalInfo{7, L"de", L".", L"135", 6, 3}},
    {8, LocalInfo{8, L"el", L"/", L"025", 7, 3}},
    {9, LocalInfo{9, L"en", L"/", L"205", 0, 3}},
    {10, LocalInfo{10, L"es", L"/", L"135", 2, 3}},
    {11, LocalInfo{11, L"fi", L".", L"025", 8, 6}},
    {12, LocalInfo{12, L"fr", L"/", L"135", 9, 4}},
    {14, LocalInfo{14, L"hu", L".", L"531", 10, 5}},
    {16, LocalInfo{16, L"it", L"/", L"135", 11, 3}},
    {17, LocalInfo{17, L"ja", L"/", L"531", 12, 2}},
    {18, LocalInfo{18, L"ko", L"-", L"531", 13, 2}},
    {21, LocalInfo{21, L"pl", L".", L"135", 14, 3}},
    {22, LocalInfo{22, L"pt", L"/", L"135", 15, 3}},
    {25, LocalInfo{25, L"ru", L".", L"135", 1, 3}},

    {12298, LocalInfo{12298, L"es-EC", L"/", L"025", 2, 3}}
};


LocalInfo getLocalInfo(const _INT32 lcid)
{
    auto result = InfoMap.find(lcid);
    if(result == InfoMap.end())
        return InfoMap.at(-1);
    return InfoMap.at(lcid);
}


//LocalInfo methods

//todo сборка кода формата из сокращенного формата и разделителя
std::wstring LocalInfo::GetShortDateFormat()
{
    std::wstring result;
    for(auto i = 0; i < ShortDatePattern.size(); i++ )
    {
        switch (ShortDatePattern[i])
        {
            case L'0':
                result+= L"d";
                break;
            case L'1':
                result+= L"dd";
                break;
            case L'2':
                result+= L"mm";
                break;
            case L'3':
                result+= L"mmm";
                break;
            case L'4':
                result+= L"yy";
                break;
            case L'5':
                result+= L"yyyy";
                break;
        };
        if (i != ShortDatePattern.size() - 1)
            result+= DateSeparator;
    }
        return result;
}

std::vector<std::wstring> LocalInfo::GetMonthNames(const _INT16 &index, const bool isShortName)
{
    if(isShortName)
    {
        if(index < ShortMonthNames.size() && index >= 0)
            return ShortMonthNames.at(index);
        return std::vector<std::wstring>{};
    }
    if(index < MonthNames.size() && index >= 0)
        return MonthNames.at(index);
    return std::vector<std::wstring>{};
}

_INT16 LocalInfo::GetMonthNumber(const std::wstring &monthName, const bool isShortName)
{
    auto months = GetMonthNames(MonthNamesIndex, isShortName);
    for(auto i = 0; i < months.size(); i++)
        if(months.at(i) == monthName)
            return i;
    return -1;
}
std::wstring LocalInfo::GetLocMonthName(const _INT16 &index, bool shortName)
{
    std::vector<std::wstring> months;
    std::wstring monthName = L"";
    months = GetMonthNames(MonthNamesIndex, shortName);
    if(months.size() > index)
        monthName = months.at(index);
    if(shortName)
    {
        if (MonthNamesIndex == 2 && MonthNamesIndex == 10)
                monthName+= L".";
    }
    return monthName;
}
}
