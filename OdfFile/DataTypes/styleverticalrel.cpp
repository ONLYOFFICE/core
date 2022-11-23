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
