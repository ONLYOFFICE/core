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

#include "iconset_type.h"

#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const iconset_type & _Val)
{
    switch(_Val.get_type())
    {
    case iconset_type::Arrows3:
        _Wostream << L"3Arrows";
        break;
    case iconset_type::Arrows3Gray:
        _Wostream << L"3ArrowsGray";
        break;
    case iconset_type::Flags3:
        _Wostream << L"3Flags";
        break;
 	case iconset_type::Signs3:
        _Wostream << L"3Signs";
        break;
	case iconset_type::Symbols3:
        _Wostream << L"3Symbols";
        break;
	case iconset_type::Symbols3_2:
        _Wostream << L"3Symbols2";
        break;    
	case iconset_type::Traffic3Lights1:
        _Wostream << L"3TrafficLights1";
        break;
	case iconset_type::Traffic3Lights2:
        _Wostream << L"3TrafficLights2";
        break;
    case iconset_type::Arrows4:
        _Wostream << L"4Arrows";
        break;
    case iconset_type::Arrows4Gray:
        _Wostream << L"4ArrowsGray";
        break;
	case iconset_type::Rating4:
        _Wostream << L"4Rating";
        break;
	case iconset_type::RedToBlack4:
        _Wostream << L"4RedToBlack";
        break;
	case iconset_type::Traffic4Lights:
        _Wostream << L"4TrafficLights";
		break;
    case iconset_type::Arrows5:
        _Wostream << L"5Arrows";
        break;
    case iconset_type::Arrows5Gray:
        _Wostream << L"5ArrowsGray";
        break;
	case iconset_type::Quarters5:
        _Wostream << L"5Quarters";
        break;
	case iconset_type::Rating5:
        _Wostream << L"5Rating";
        break;
	 default:
        break;
    }
    return _Wostream;    
}

iconset_type iconset_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	if (tmp == L"3arrows")
        return iconset_type( Arrows3 );
	else     if (tmp == L"3arrowsgray")
        return iconset_type( Arrows3Gray );
	else     if (tmp == L"3flags")
        return iconset_type( Flags3 );
	else     if (tmp == L"3signs")
        return iconset_type( Signs3 );
	else     if (tmp == L"3symbols")
        return iconset_type( Symbols3 );
	else	if (tmp == L"3symbols2")
        return iconset_type( Symbols3_2 );
	else     if (tmp == L"3trafficlights1")
        return iconset_type( Traffic3Lights1 );
    else	if (tmp == L"3trafficlights2")
        return iconset_type( Traffic3Lights2 );
    else	if (tmp == L"4arrows")
        return iconset_type( Arrows4 );
	else	if (tmp == L"4arrowsgray")
        return iconset_type( Arrows4Gray );
	else	if (tmp == L"4Rating")
        return iconset_type( Rating4 );
	else	if (tmp == L"4redtoblack")
        return iconset_type( RedToBlack4 );
	else	if (tmp == L"4trafficlights")
        return iconset_type( Traffic4Lights );
	else	if (tmp == L"5arrows")
        return iconset_type( Arrows5 );
	else    if (tmp == L"5arrowsgray")
        return iconset_type( Arrows5Gray );
	else	if (tmp == L"5quarters")
        return iconset_type( Quarters5 );
	else	if (tmp == L"5rating")
        return iconset_type( Rating5 );
	else
    {
        return iconset_type( Arrows3 );
    }
}

} }
