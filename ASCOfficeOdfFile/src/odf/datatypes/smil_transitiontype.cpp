/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "smil_transitiontype.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const smil_transition_type & _Val)
{
	switch(_Val.get_type())
	{
	case smil_transition_type::barWipe			:	_Wostream << L"barWipe"			; break;
	case smil_transition_type::boxWipe			:	_Wostream << L"boxWipe"			; break;
	case smil_transition_type::fourBoxWipe		:	_Wostream << L"fourBoxWipe"		; break;
	case smil_transition_type::barnDoorWipe		:	_Wostream << L"barnDoorWipe"	; break;
	case smil_transition_type::diagonalWipe		:	_Wostream << L"diagonalWipe"	; break;
	case smil_transition_type::bowTieWipe		:	_Wostream << L"bowTieWipe"		; break;
	case smil_transition_type::miscDiagonalWipe	:	_Wostream << L"miscDiagonalWipe"; break;
	case smil_transition_type::veeWipe			:	_Wostream << L"veeWipe"			; break;
	case smil_transition_type::barnVeeWipe		:	_Wostream << L"barnVeeWipe"		; break;
	case smil_transition_type::zigZagWipe		:	_Wostream << L"zigZagWipe"		; break;
	case smil_transition_type::barnZigZagWipe	:	_Wostream << L"barnZigZagWipe"	; break;
	case smil_transition_type::irisWipe			:	_Wostream << L"irisWipe"		; break;
	case smil_transition_type::triangleWipe		:	_Wostream << L"triangleWipe"	; break;
	case smil_transition_type::arrowHeadWipe	:	_Wostream << L"arrowHeadWipe"	; break;
	case smil_transition_type::pentagonWipe		:	_Wostream << L"pentagonWipe"	; break;
	case smil_transition_type::hexagonWipe		:	_Wostream << L"hexagonWipe"		; break;
	case smil_transition_type::ellipseWipe		:	_Wostream << L"ellipseWipe"		; break;
	case smil_transition_type::eyeWipe			:	_Wostream << L"eyeWipe"			; break;
	case smil_transition_type::roundRectWipe	:	_Wostream << L"roundRectWipe"	; break;
	case smil_transition_type::starWipe			:	_Wostream << L"starWipe"		; break;
	case smil_transition_type::miscShapeWipe	:	_Wostream << L"miscShapeWipe"	; break;
	case smil_transition_type::clockWipe		:	_Wostream << L"clockWipe"		; break;
	case smil_transition_type::pinWheelWipe		:	_Wostream << L"pinWheelWipe"	; break;
	case smil_transition_type::singleSweepWipe	:	_Wostream << L"singleSweepWipe"	; break;
	case smil_transition_type::fanWipe			:	_Wostream << L"fanWipe"			; break;
	case smil_transition_type::doubleFanWipe	:	_Wostream << L"doubleFanWipe"	; break;
	case smil_transition_type::doubleSweepWipe	:	_Wostream << L"doubleSweepWipe"	; break;
	case smil_transition_type::saloonDoorWipe	:	_Wostream << L"saloonDoorWipe"	; break;
	case smil_transition_type::windshieldWipe	:	_Wostream << L"windshieldWipe"	; break;
	case smil_transition_type::snakeWipe		:	_Wostream << L"snakeWipe"		; break;
	case smil_transition_type::spiralWipe		:	_Wostream << L"spiralWipe"		; break;
	case smil_transition_type::parallelSnakesWipe:	_Wostream << L"parallelSnakesWipe"; break;
	case smil_transition_type::boxSnakesWipe	:	_Wostream << L"boxSnakesWipe"	; break;
	case smil_transition_type::waterfallWipe	:	_Wostream << L"waterfallWipe"	; break;
	case smil_transition_type::pushWipe			:	_Wostream << L"pushWipe"		; break;
	case smil_transition_type::slideWipe		:	_Wostream << L"slideWipe"		; break;
	case smil_transition_type::fade				:	_Wostream << L"fade"			; break;
	case smil_transition_type::checkerBoardWipe	:	_Wostream << L"checkerboardWipe"; break;
	case smil_transition_type::blindsWipe		:	_Wostream << L"blindsWipe"		; break;
	case smil_transition_type::dissolve			:	_Wostream << L"dissolve"		; break;
	case smil_transition_type::randomBarWipe	:	_Wostream << L"randomBarWipe"	; break;
	}
    return _Wostream;
}

smil_transition_type smil_transition_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

		 if(tmp == L"barWipe")			return smil_transition_type( barWipe );
	else if(tmp == L"boxWipe")			return smil_transition_type( boxWipe );
	else if(tmp == L"fourboxWipe")		return smil_transition_type( fourBoxWipe );
	else if(tmp == L"barndoorWipe")		return smil_transition_type( barnDoorWipe );
	else if(tmp == L"diagonalWipe")		return smil_transition_type( diagonalWipe );
	else if(tmp == L"bowtieWipe")		return smil_transition_type( bowTieWipe );
	else if(tmp == L"miscdiagonalWipe")	return smil_transition_type( miscDiagonalWipe );
	else if(tmp == L"veeWipe")			return smil_transition_type( veeWipe );
	else if(tmp == L"barnveeWipe")		return smil_transition_type( barnVeeWipe );
	else if(tmp == L"zigzagWipe")		return smil_transition_type( zigZagWipe );
	else if(tmp == L"barnzigzagWipe")	return smil_transition_type( barnZigZagWipe );
	else if(tmp == L"irisWipe")			return smil_transition_type( irisWipe);
	else if(tmp == L"triangleWipe")		return smil_transition_type( triangleWipe);
	else if(tmp == L"arrowheadWipe")	return smil_transition_type( arrowHeadWipe );
	else if(tmp == L"pentagonWipe")		return smil_transition_type( pentagonWipe );
	else if(tmp == L"hexagonWipe")		return smil_transition_type( hexagonWipe );
	else if(tmp == L"ellipseWipe")		return smil_transition_type( ellipseWipe );
	else if(tmp == L"eyeWipe")			return smil_transition_type( eyeWipe );
	else if(tmp == L"roundrectWipe")	return smil_transition_type( roundRectWipe );
	else if(tmp == L"starWipe")			return smil_transition_type( starWipe );
	else if(tmp == L"miscshapeWipe")	return smil_transition_type( miscShapeWipe );
	else if(tmp == L"clockWipe")		return smil_transition_type( clockWipe );
	else if(tmp == L"pinwheelWipe")		return smil_transition_type( pinWheelWipe );
	else if(tmp == L"singlesweepWipe")	return smil_transition_type( singleSweepWipe);
	else if(tmp == L"fanWipe")			return smil_transition_type( fanWipe );
	else if(tmp == L"doublefanWipe")	return smil_transition_type( doubleFanWipe );
	else if(tmp == L"doublesweepWipe")	return smil_transition_type( doubleSweepWipe );
	else if(tmp == L"saloondoorWipe")	return smil_transition_type( saloonDoorWipe );
	else if(tmp == L"windshieldWipe")	return smil_transition_type( windshieldWipe );
	else if(tmp == L"snakeWipe")		return smil_transition_type( snakeWipe );
	else if(tmp == L"spiralWipe")		return smil_transition_type( spiralWipe );
	else if(tmp == L"parallelsnakesWipe")return smil_transition_type( parallelSnakesWipe );
	else if(tmp == L"boxsnakesWipe")	return smil_transition_type( boxSnakesWipe );
	else if(tmp == L"waterfallWipe")	return smil_transition_type( waterfallWipe );
	else if(tmp == L"pushWipe")			return smil_transition_type( pushWipe );
	else if(tmp == L"slideWipe")		return smil_transition_type( slideWipe );
	else if(tmp == L"fade")				return smil_transition_type( fade );
	else if(tmp == L"checkerboardWipe")	return smil_transition_type( checkerBoardWipe);
	else if(tmp == L"blindsWipe")		return smil_transition_type( blindsWipe);
	else if(tmp == L"dissolve")			return smil_transition_type( dissolve);
	else if(tmp == L"randombarWipe")	return smil_transition_type( randomBarWipe);
	else
    {
        return smil_transition_type( barWipe );
    }
}

} }
