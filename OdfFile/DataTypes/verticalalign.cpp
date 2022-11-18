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

#include "verticalalign.h"
#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const vertical_align & _Val)
{
    switch(_Val.get_type())
    {
    case vertical_align::Top:
        _Wostream << L"top";
        break;
    case vertical_align::Middle:
        _Wostream << L"middle";
        break;
    case vertical_align::Bottom:
        _Wostream << L"bottom";
        break;
    case vertical_align::Auto:
        _Wostream << L"auto";
        break;
    case vertical_align::Baseline:
        _Wostream << L"baseline";
        break;
    case vertical_align::Justify:
        _Wostream << L"justify";
        break;
    default:
        break;
    }
    return _Wostream;    
}

vertical_align vertical_align::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"top")
        return vertical_align( Top );
    else if (tmp == L"middle" || tmp == L"center")
        return vertical_align( Middle );
    else if (tmp == L"bottom")
        return vertical_align( Bottom );
    else if (tmp == L"auto" || tmp == L"automatic")
        return vertical_align( Auto );
    else if (tmp == L"baseline")
        return vertical_align( Baseline );
    else if (tmp == L"justify")
        return vertical_align( Justify );
    else
    {
        return vertical_align( Baseline );
    }
}

} }
