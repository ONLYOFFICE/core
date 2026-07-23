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
