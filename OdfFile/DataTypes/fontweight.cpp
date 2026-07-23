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

#include "fontweight.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const font_weight & _Val)
{
    switch(_Val.get_type())
    {
    case font_weight::WNormal:
        _Wostream << "normal";
        break;
    case font_weight::WBold:
        _Wostream << "bold";
        break;
    case font_weight::W100:
        _Wostream << "100";
        break;
    case font_weight::W200:
        _Wostream << "200";
        break;
    case font_weight::W300:
        _Wostream << "300";
        break;
    case font_weight::W400:
        _Wostream << "400";
        break;
    case font_weight::W500:
        _Wostream << "500";
        break;
    case font_weight::W600:
        _Wostream << "600";
        break;
    case font_weight::W700:
        _Wostream << "700";
        break;
    case font_weight::W800:
        _Wostream << "800";
        break;
    case font_weight::W900:
        _Wostream << "900";
        break;
    default:
        break;
    }
    return _Wostream;
}

font_weight font_weight::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    
    if (tmp == L"normal")
        return font_weight( WNormal );
    else if (tmp == L"bold")
        return font_weight( WBold );
    else if (tmp == L"100")
        return font_weight( W100 );
    else if (tmp == L"200")
        return font_weight( W200 );
    else if (tmp == L"300")
        return font_weight( W300 );
    else if (tmp == L"400")
        return font_weight( W400 );
    else if (tmp == L"500")
        return font_weight( W500 );
    else if (tmp == L"600")
        return font_weight( W600 );
    else if (tmp == L"700")
        return font_weight( W700 );
    else if (tmp == L"800")
        return font_weight( W800 );
    else if (tmp == L"900")
        return font_weight( W900 );
    else
    {
        return WNormal;
    }
}

} }

