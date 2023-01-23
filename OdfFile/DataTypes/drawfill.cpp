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

#include "drawfill.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { 
namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const draw_fill & _Val)
{
	std::wstring fillType;
	switch(_Val.get_type())
	{
		case draw_fill::none:		_Wostream << L"none" ;break;
		case draw_fill::hatch:		_Wostream << L"hatch" ;break;
		case draw_fill::solid:		_Wostream << L"solid" ;break;
		case draw_fill::gradient:	_Wostream << L"gradient" ;break;
		case draw_fill::bitmap:		_Wostream << L"bitmap" ;break;
	}

    return _Wostream;
}
//std::wstring draw_fill::get_type_ms()
//{
//	std::wstring fillType;
//	switch(type_)
//	{
//		case draw_fill::none:		fillType = L"a:noFill";break;
//		case draw_fill::hatch:		fillType = L"a:pattFill";break;
//		case draw_fill::solid:		fillType = L"a:solidFill";break;
//		case draw_fill::gradient:	fillType = L"a:gradFill";break;
//		case draw_fill::bitmap:		fillType = L"a:imageFill";break;
//	}
//	return fillType;
//}
draw_fill draw_fill::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return draw_fill( none );
    else if (tmp == L"solid")
        return draw_fill( solid );
    else if (tmp == L"bitmap")
        return draw_fill( bitmap );
    else if (tmp == L"gradient")
        return draw_fill( gradient );
    else if (tmp == L"hatch")
        return draw_fill( hatch );
    else
    {
        return draw_fill( solid );
    }
}
}
}
