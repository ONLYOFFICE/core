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
#include <boost/algorithm/string.hpp>

#include "chartclass.h"

#include <ostream>

namespace cpdoccore { namespace odf_types { 
std::wostream & operator << (std::wostream & _Wostream, const chart_class & _Val)
{
	std::wstring res = L"";
	switch(_Val.get_type())
	{
        case chart_class::area:			_Wostream << L"chart:area";		break;
        case chart_class::bubble:		_Wostream << L"chart:bubble";	break;
		case chart_class::circle:		_Wostream << L"chart:circle";	break;
		case chart_class::filled_radar:	_Wostream << L"chart:filled-radar";		break;
		case chart_class::gantt:		_Wostream << L"chart:gantt";	break;
		case chart_class::line:			_Wostream << L"chart:line";		break;
		case chart_class::radar:		_Wostream << L"chart:radar";	break;
		case chart_class::ring:			_Wostream << L"chart:ring";		break;
		case chart_class::scatter:		_Wostream << L"chart:scatter";	break;
		case chart_class::stock:		_Wostream << L"chart:stock";	break;
		case chart_class::surface:		_Wostream << L"chart:surface";	break;
		case chart_class::bar:
		default:						_Wostream << L"chart:bar";
	}
    return _Wostream;
}
chart_class chart_class::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

		 if (tmp == L"chart:area")			return chart_class(area);
    else if (tmp == L"chart:bar")			return chart_class(bar);
    else if (tmp == L"chart:bubble")		return chart_class(bubble);
    else if (tmp == L"chart:circle")		return chart_class(circle);
	else if (tmp == L"chart:filled-radar")	return chart_class(filled_radar);
    else if (tmp == L"chart:gantt")			return chart_class(gantt);
    else if (tmp == L"chart:line")			return chart_class(line);
    else if (tmp == L"chart:radar")			return chart_class(radar);
    else if (tmp == L"chart:ring")			return chart_class(ring);
    else if (tmp == L"chart:scatter")		return chart_class(scatter);
    else if (tmp == L"chart:stock")			return chart_class(stock);
    else if (tmp == L"chart:surface")		return chart_class(surface);
	else
    {
        return chart_class(bar);
    }
}




} }
