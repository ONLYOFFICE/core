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

#include "stylefamily.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const style_family & _Val)
{
    switch(_Val.get_type())
    {
    case style_family::Paragraph:
        _Wostream << L"paragraph";
        break;
    case style_family::Text:
        _Wostream << L"text";
        break;
    case style_family::Section:
        _Wostream << L"section";
        break;
    case style_family::Table:
        _Wostream << L"table";
        break;
    case style_family::TableColumn:
        _Wostream << L"table-column";
        break;
    case style_family::TableRow:
        _Wostream << L"table-row";
        break;
    case style_family::TableCell:
        _Wostream << L"table-cell";
        break;
    case style_family::TablePage:
        _Wostream << L"table-page";
        break;
    case style_family::Chart:
        _Wostream << L"chart";
        break;
    default:
    case style_family::Default:
        _Wostream << L"default";
        break;
    case style_family::DrawingPage:
        _Wostream << L"drawing-page";
        break;
    case style_family::Graphic:
        _Wostream << L"graphic";
        break;
    case style_family::Presentation:
        _Wostream << L"presentation";
        break;
    case style_family::Control:
        _Wostream << L"control";
        break;
    case style_family::Ruby:
        _Wostream << L"ruby";
        break;
    }
    return _Wostream;    
}

bool style_family::operator == (const style_family & rVal) const
{
    const bool res = 
        get_type() ==  rVal.get_type();   

    return res;
}

style_family style_family::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"paragraph")
        return style_family( Paragraph );
    else if (tmp == L"text")
        return style_family( Text );
    else if (tmp == L"section")
        return style_family( Section );
    else if (tmp == L"table")
        return style_family( Table );
    else if (tmp == L"table-column")
        return style_family( TableColumn );
    else if (tmp == L"table-row")
        return style_family( TableRow );
    else if (tmp == L"table-cell")
        return style_family( TableCell );
    else if (tmp == L"table-page")
        return style_family( TablePage );
    else if (tmp == L"chart")
        return style_family( Chart );
    else if (tmp == L"default")
        return style_family( Default );
    else if (tmp == L"drawing-page")
        return style_family( DrawingPage );
    else if (tmp == L"graphic" || tmp == L"graphics")
        return style_family( Graphic );
    else if (tmp == L"presentation")
        return style_family( Presentation );
    else if (tmp == L"control")
        return style_family( Control );
    else if (tmp == L"ruby")
        return style_family( Ruby );
    else
    {
        return style_family( Default );
    }
}

} }
