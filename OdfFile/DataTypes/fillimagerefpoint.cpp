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

#include "fillimagerefpoint.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { 
namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const fill_image_ref_point & _Val)
{
	std::wstring fillType;
	switch(_Val.get_type())
	{
		case fill_image_ref_point::top_left:	_Wostream << L"top-left" ;break;
		case fill_image_ref_point::top:			_Wostream << L"top" ;break;
		case fill_image_ref_point::top_right:	_Wostream << L"top-right" ;break;
		case fill_image_ref_point::left:		_Wostream << L"left" ;break;
		case fill_image_ref_point::center:		_Wostream << L"center" ;break;
		case fill_image_ref_point::right:		_Wostream << L"right" ;break;
		case fill_image_ref_point::bottom_left:	_Wostream << L"bottom-left" ;break;
		case fill_image_ref_point::bottom:		_Wostream << L"bottom" ;break;
		case fill_image_ref_point::bottom_right:_Wostream << L"bottom-right" ;break;
	}

    return _Wostream;
}

fill_image_ref_point fill_image_ref_point::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"left")
        return fill_image_ref_point( left );
    else if (tmp == L"top")
        return fill_image_ref_point( top );
     else if (tmp == L"top-left")
        return fill_image_ref_point( top_left );
    else if (tmp == L"top-right")
        return fill_image_ref_point( top_right );
	else if (tmp == L"center")
        return fill_image_ref_point( center );
    else if (tmp == L"right")
        return fill_image_ref_point(right );
    else if (tmp == L"bottom-left")
        return fill_image_ref_point( bottom_left );
    else if (tmp == L"bottom")
        return fill_image_ref_point( bottom );
    else if (tmp == L"bottom-right")
        return fill_image_ref_point( bottom_right );
    else
    {
        return fill_image_ref_point( center );
    }
}
}
}
