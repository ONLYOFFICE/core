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
#pragma once   

#include <ostream>
#include <sstream>
#include <string>
#include <vector>


#include <CPOptional.h>

namespace cpdoccore 
{ 

namespace svg_path
{
	struct _point
	{
		_point(double _x,double _y){x=_x; y=_y;}
		
		_CP_OPT(double) x;
		_CP_OPT(double) y;
		
	};
	struct _polyline
	{
		std::wstring command;
		std::vector<_point> points; //будем бить строку пути по количеству точек в буковках

	};
	//m - 1 point
	//c - 3 point
	//s - 2 point
	//l - 1 point
	//z - finish
	//h - 0.5 point :)
	//v - 0.5 point
	//q - 2 point
	//t - 1 point
	//a - [[[[

	bool createSvgD(std::vector<_polyline> & Polyline, const std::wstring &  rSvgDStatement, bool bWrongPositionAfterZ);
	bool createPolygon(std::vector<_polyline> & Polyline, const std::wstring &  rPolygonStatement, bool bWrongPositionAfterZ);

}
}

