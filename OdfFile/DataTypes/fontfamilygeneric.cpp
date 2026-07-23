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

#include "fontfamilygeneric.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const font_family_generic & _Val)
{
    switch(_Val.get_type())
    {
    case font_family_generic::Roman:
        _Wostream << L"roman";
        break;
    case font_family_generic::Swiss:
        _Wostream << L"swiss";
        break;
    case font_family_generic::Modern:
        _Wostream << L"modern";
        break;
    case font_family_generic::Decorative:
        _Wostream << L"decorative";
        break;
    case font_family_generic::Script:
        _Wostream << L"script";
        break;
    case font_family_generic::System:
        _Wostream << L"system";
        break;
    default:
        break;
    }
    return _Wostream;    
}

font_family_generic font_family_generic::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"roman")
        return font_family_generic( Roman );
    if (tmp == L"swiss")
        return font_family_generic( Swiss );
    if (tmp == L"modern")
        return font_family_generic( Modern );
    if (tmp == L"decorative")
        return font_family_generic( Decorative );
    if (tmp == L"script")
        return font_family_generic( Script );
    if (tmp == L"system")
        return font_family_generic( System );
    else
    {
        return font_family_generic( Roman );
    }
}

} }
