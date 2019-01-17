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

#include "chartlabelposition.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types {

std::wostream & operator << (std::wostream & _Wostream, const chart_label_position & _Val)
{
	switch(_Val.get_type())
	{
		case   chart_label_position::avoid_overlap:	_Wostream <<  L"avoid-overlap"; break;
		case   chart_label_position::bottom:		_Wostream <<  L"bottom";		break;
		case   chart_label_position::bottom_left:	_Wostream <<  L"bottom-left";	break;
		case   chart_label_position::bottom_right:	_Wostream <<  L"bottom_right";	break;
		case   chart_label_position::center:		_Wostream <<  L"center";		break;
		case   chart_label_position::inside:		_Wostream <<  L"insidev";		break;
		case   chart_label_position::left:			_Wostream <<  L"left";			break;
		case   chart_label_position::near_origin:	_Wostream <<  L"near-origin";	break;
		case   chart_label_position::outside:		_Wostream <<  L"outside";		break;
		case   chart_label_position::right:			_Wostream <<  L"right";			break;
		case   chart_label_position::top:			_Wostream <<  L"top";			break;
		case   chart_label_position::top_left:		_Wostream <<  L"top-left";		break;
		case   chart_label_position::top_right:		_Wostream <<  L"top-right";		break;
	}
    return _Wostream;    
}
chart_label_position chart_label_position::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	if (tmp == L"avoid-overlap")	return chart_label_position( avoid_overlap );
	else if (tmp == L"bottom")		return chart_label_position( bottom );
	else if (tmp == L"bottom-left")	return chart_label_position( bottom_left );
	else if (tmp == L"bottom-right")return chart_label_position( bottom_right );
	else if (tmp == L"center")		return chart_label_position( center );
	else if (tmp == L"inside")		return chart_label_position( inside );
	else if (tmp == L"left")		return chart_label_position( left );
	else if (tmp == L"near-origin")	return chart_label_position( near_origin );
	else if (tmp == L"outside")		return chart_label_position( outside );
	else if (tmp == L"right")       return chart_label_position( right );
	else if (tmp == L"top")			return chart_label_position( top );
	else if (tmp == L"top-left")	return chart_label_position( top_left );
	else if (tmp == L"top-right")	return chart_label_position( top_right );    
	else
    {
        return chart_label_position( near_origin );
    }
}



} }
