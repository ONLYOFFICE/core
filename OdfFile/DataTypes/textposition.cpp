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

#include "textposition.h"
#include "../Common/errors.h"

#include <boost/algorithm/string.hpp>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const text_position & _Val)
{
    switch(_Val.get_type())
    {
    case text_position::Sub:
        _Wostream << L"sub";
        break;
    case text_position::Super:
        _Wostream << L"super";
        break;
    case text_position::Percent:
        _Wostream << _Val.get_position();
        break;
    default:
        break;
    }

    if (_Val.has_font_size())
        _Wostream << L" "  << _Val.font_size();

    return _Wostream;
}

text_position text_position::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    boost::algorithm::trim(tmp);
    std::vector< std::wstring > splitted;
    boost::algorithm::split(splitted, tmp, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

    text_position::type type_position_ = text_position::Sub;
    percent position_;
    bool has_font_width_ = false;
    percent font_width_;
   
    if (splitted.size() > 0)
    {
        if (splitted[0] == L"sub")
            type_position_ = text_position::Sub;
        else if (splitted[0] == L"super")
            type_position_ = text_position::Super;
        else if (boost::algorithm::contains(splitted[0], L"%"))
        {
            position_ = percent::parse(splitted[0]);
            type_position_ = text_position::Percent;
        }
        else
        {
            position_ = 0;
            type_position_ = text_position::Percent;
        }
    }

    if (splitted.size() > 1)
    {
        font_width_ = percent::parse(splitted[1]);
        has_font_width_ = true;
    }

    if (type_position_ != text_position::Percent)
    {
        if (!has_font_width_)
            return text_position(type_position_);
        else
            return text_position(type_position_, font_width_.get_value());
    }
    else
    {
        if (!has_font_width_)
            return text_position(position_.get_value());
        else
            return text_position(position_.get_value(), font_width_.get_value());
    }
}

} }

