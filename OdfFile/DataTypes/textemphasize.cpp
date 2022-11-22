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

