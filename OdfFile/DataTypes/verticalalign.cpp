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
