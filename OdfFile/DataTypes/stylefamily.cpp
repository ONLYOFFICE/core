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
