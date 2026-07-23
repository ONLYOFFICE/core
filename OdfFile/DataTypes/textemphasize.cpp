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

#include "textemphasize.h"
#include "../Common/errors.h"

#include <boost/algorithm/string.hpp>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const text_emphasize & _Val)
{
    switch(_Val.get_type())
    {
    case text_emphasize::None:
        _Wostream << L"none";
        break;
    case text_emphasize::Accent:
        _Wostream << L"accent";
        break;
    case text_emphasize::Dot:
        _Wostream << L"dot";
        break;
    case text_emphasize::Circle:
        _Wostream << L"circle";
        break;
    case text_emphasize::Disc:
        _Wostream << L"disc";
        break;
    default:
        break;
    }

    if (_Val.get_type() != text_emphasize::None)
    {
        switch (_Val.get_type_2())
        {
        case text_emphasize::Above:
            _Wostream << L" above";
            break;
        case text_emphasize::Below:
            _Wostream << L" below";
            break;
        }
    }

    return _Wostream;
}

text_emphasize text_emphasize::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    boost::algorithm::trim(tmp);
    std::vector< std::wstring > splitted;
    boost::algorithm::split(splitted, tmp, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

    text_emphasize::type type1_;
   
    if (splitted.size() > 0)
    {
        if (splitted[0] == L"none")
            type1_ = None;
        else if (splitted[0] == L"accent")
            type1_ = Accent;
        else if (splitted[0] == L"dot")
            type1_ = Dot;
        else if (splitted[0] == L"circle")
            type1_ = Circle;
        else if (splitted[0] == L"disc")
            type1_ = Disc;
        else
        {
            return text_emphasize(None, None);
        }
    }

    text_emphasize::type type2_ = None;
    if (splitted.size() > 1)
    {
        if (splitted[1] == L"above")
            type2_ = Above;
        else if (splitted[1] == L"below")
            type2_ = Below;
        else
        {
            return text_emphasize(None, None);
        }        

    }

    if (type1_ == None)
        return text_emphasize(None, None);
    else if (type2_ != None)
        return text_emphasize(type1_, type2_);
    else
    {
        return text_emphasize(None, None);
    }

}

} }

