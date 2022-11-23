/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

    text_position::type type_= text_position::Sub;
    percent position_;
    bool has_font_width_ = false;
    percent font_width_;
   
    if (splitted.size() > 0)
    {
        if (splitted[0] == L"sub")
            type_ = text_position::Sub;
        else if (splitted[0] == L"super")
            type_ = text_position::Super;
        else if (boost::algorithm::contains(splitted[0], L"%"))
        {
            position_ = percent::parse(splitted[0]);
            type_ = text_position::Percent;
        }
    }

    if (splitted.size() > 1)
    {
        font_width_ = percent::parse(splitted[1]);
        has_font_width_ = true;
    }

    if (type_ != text_position::Percent)
    {
        if (!has_font_width_)
            return text_position(type_);
        else
            return text_position(type_, font_width_.get_value());
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

