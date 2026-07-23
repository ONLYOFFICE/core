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

#include "stylewrap.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const style_wrap & _Val)
{
    switch(_Val.get_type())
    {
    case style_wrap::None:
        _Wostream << L"none";
        break;
    case style_wrap::Left:
        _Wostream << L"left";
        break;
    case style_wrap::Right:
        _Wostream << L"right";
        break;
    case style_wrap::Parallel:
        _Wostream << L"parallel";
        break;
    case style_wrap::Dynamic:
        _Wostream << L"dynamic";
        break;
    case style_wrap::RunThrough:
        _Wostream << L"run-through";
        break;
    case style_wrap::Biggest:
        _Wostream << L"biggest";
        break;
    default:
        break;
    }
    return _Wostream;    
}

style_wrap style_wrap::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return style_wrap( None );
    else if (tmp == L"left")
        return style_wrap( Left );
    else if (tmp == L"right")
        return style_wrap( Right );
    else if (tmp == L"parallel")
        return style_wrap( Parallel );
    else if (tmp == L"dynamic")
        return style_wrap( Dynamic );
    else if (tmp == L"run-through")
        return style_wrap( RunThrough );
    else if (tmp == L"biggest")
        return style_wrap( Biggest );

    else
    {
        return style_wrap( None );
    }
}

} }
