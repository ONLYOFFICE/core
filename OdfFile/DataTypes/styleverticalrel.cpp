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

#include "styleverticalrel.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const vertical_rel & _Val)
{
    switch(_Val.get_type())
    {
    case vertical_rel::Page:
        _Wostream << L"page";
        break;
    case vertical_rel::PageContent:
        _Wostream << L"page-content";
        break;
    case vertical_rel::Frame:
        _Wostream << L"frame";
        break;
    case vertical_rel::FrameContent:
        _Wostream << L"frame-content";
        break;
    case vertical_rel::Paragraph:
        _Wostream << L"paragraph";
        break;
    case vertical_rel::ParagraphContent:
        _Wostream << L"paragraph-content";
        break;
    case vertical_rel::Char:
        _Wostream << L"char";
        break;
    case vertical_rel::Line:
        _Wostream << L"line";
        break;
    case vertical_rel::Baseline:
        _Wostream << L"baseline";
        break;
    case vertical_rel::Text:
        _Wostream << L"text";
        break;
    default:
        break;
    }
    return _Wostream;    
}

vertical_rel vertical_rel::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"page")
        return vertical_rel( Page );
    else if (tmp == L"page-content")
        return vertical_rel( PageContent );
    else if (tmp == L"frame")
        return vertical_rel( Frame );
    else if (tmp == L"frame-content")
        return vertical_rel( FrameContent );
     else if (tmp == L"paragraph")
        return vertical_rel( Paragraph );
     else if (tmp == L"paragraph-content")
        return vertical_rel( ParagraphContent );
     else if (tmp == L"char")
        return vertical_rel( Char );
     else if (tmp == L"line")
        return vertical_rel( Line );
     else if (tmp == L"baseline")
        return vertical_rel( Baseline );
     else if (tmp == L"text")
        return vertical_rel( Text );

    else
    {
        return vertical_rel( Page );
    }
}

} }
