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
std::map<_INT32, LocalInfo> InfoMap
{
    {-1, LocalInfo{-1, L"default", L".", L"135", L"%d %B %Y"}},
    {25, LocalInfo{25, L"ru", L".", L"135", L"%d %B %Y г."}},
    {9, LocalInfo{9, L"en", L"/", L"205", L"%A, %B %d, %Y"}},
    {12298, LocalInfo{12298, L"es-EC", L"/", L"025", L"%A, %d de %B de %Y"}}
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
std::wstring LocalInfo::GetshorDateFormat()
{
    std::wstring result;
    for(auto i = 0; i < ShortDatePattern.size(); i++ )
    {
        switch (ShortDatePattern[i])
        {
            case L'0':
            case L'1':
                result+= L"%d";
                break;
            case L'2':
            case L'3':
                result+= L"%m";
                break;
            case L'4':
                result+= L"%y";
                break;
            case L'5':
                result+= L"%Y";
                break;
        };
        if (i != ShortDatePattern.size() - 1)
            result+= DateSeparator;
    }
        return result;
}

}
