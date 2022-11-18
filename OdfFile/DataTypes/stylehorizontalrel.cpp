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

#include "stylehorizontalrel.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const horizontal_rel & _Val)
{
    switch(_Val.get_type())
    {
    case horizontal_rel::Page:                 _Wostream << L"page";                   break;
    case horizontal_rel::PageContent:          _Wostream << L"page-content";           break;
    case horizontal_rel::PageStartMargin:      _Wostream << L"page-start-margin";      break;
    case horizontal_rel::PageEndMargin:        _Wostream << L"page-end-margin";        break;
    case horizontal_rel::Frame:                _Wostream << L"frame";                  break;
    case horizontal_rel::FrameContent:         _Wostream << L"frame-content";          break;
    case horizontal_rel::FrameStartMargin:     _Wostream << L"frame-start-margin";     break;
    case horizontal_rel::FrameEndMargin:       _Wostream << L"frame-end-margin";       break;
    case horizontal_rel::Paragraph:            _Wostream << L"paragraph";              break;
    case horizontal_rel::ParagraphContent:     _Wostream << L"paragraph-content";      break;
    case horizontal_rel::ParagraphStartMargin: _Wostream << L"paragraph-start-margin"; break;
    case horizontal_rel::ParagraphEndMargin:   _Wostream << L"paragraph-end-margin";   break;
    case horizontal_rel::Char:                 _Wostream << L"char";                   break;
    default:
        break;
    }
    return _Wostream;
}

horizontal_rel horizontal_rel::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"page")
        return horizontal_rel( Page );
    else if (tmp == L"page-content")
        return horizontal_rel( PageContent );
    else if (tmp == L"page-start-margin")
        return horizontal_rel( PageStartMargin );
    else if (tmp == L"page-end-margin")
        return horizontal_rel( PageEndMargin );
    else if (tmp == L"frame")
        return horizontal_rel( Frame );
    else if (tmp == L"frame-content")
        return horizontal_rel( FrameContent );
    else if (tmp == L"frame-start-margin")
        return horizontal_rel( PageStartMargin );
    else if (tmp == L"frame-end-margin")
        return horizontal_rel( FrameEndMargin );
    else if (tmp == L"paragraph")
        return horizontal_rel( Paragraph );
    else if (tmp == L"paragraph-content")
        return horizontal_rel( ParagraphContent );
    else if (tmp == L"paragraph-start-margin")
        return horizontal_rel( ParagraphStartMargin );
    else if (tmp == L"paragraph-end-margin")
        return horizontal_rel( ParagraphEndMargin );
    else if (tmp == L"char")
        return horizontal_rel( Char );
    else
    {
        return horizontal_rel( Page );
    }
}

} }
