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

#include "writingmode.h"
#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const writing_mode & _Val)
{
    switch(_Val.get_type())
    {
    case writing_mode::LrTb:
        _Wostream << L"lr-tb";
        break;
    case writing_mode::RlTb:
        _Wostream << L"rl-tb";
        break;
    case writing_mode::TbRl:
        _Wostream << L"tb-rl";
        break;
    case writing_mode::TbLr:
        _Wostream << L"tb-lr";
        break;
    case writing_mode::Lr:
        _Wostream << L"lr";
        break;
    case writing_mode::Rl:
        _Wostream << L"rl";
        break;
    case writing_mode::Tb:
        _Wostream << L"tb";
        break;
    case writing_mode::Page:
        _Wostream << L"page";
        break;
    default:
        break;
    }
    return _Wostream;    
}

writing_mode writing_mode::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"lr-tb")
        return writing_mode( LrTb );
    else if (tmp == L"rl-tb")
        return writing_mode( RlTb );
    else if (tmp == L"tb-rl")
        return writing_mode( TbRl );
    else if (tmp == L"tb-lr")
        return writing_mode( TbLr );
    else if (tmp == L"lr")
        return writing_mode( Lr );
    else if (tmp == L"rl")
        return writing_mode( Rl );
    else if (tmp == L"tb")
        return writing_mode( Tb );
    else if (tmp == L"page")
        return writing_mode( Page );
    else
    {
        return writing_mode( LrTb );
    }
}

} }
