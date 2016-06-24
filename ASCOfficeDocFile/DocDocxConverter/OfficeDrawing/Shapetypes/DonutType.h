/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
	class DonutType: public ShapeType
	{
	public: 
		DonutType() : ShapeType(msosptDonut)
		{
			ShapeConcentricFill =	false;
			Joins				=	round;
			Path				=	_T("m,10800qy10800,,21600,10800,10800,21600,,10800xm@0,10800qy10800@2@1,10800,10800@0@0,10800xe");

			Formulas.push_back(_T("val #0" ) );
			Formulas.push_back(_T("sum width 0 #0" ) );
			Formulas.push_back(_T("sum height 0 #0" ) );
			Formulas.push_back(_T("prod @0 2929 10000" ) );
			Formulas.push_back(_T("sum width 0 @3" ) );
			Formulas.push_back(_T("sum height 0 @3" ) );
			
			AdjustmentValues	=	_T("5400");
			ConnectorLocations	=	_T("10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163");
			TextBoxRectangle	=	_T("3163,3163,18437,18437");

			Handle HandleOne;
			HandleOne.position	=	_T("#0,center");
			HandleOne.xrange	=	_T("0,10800");
			Handles.push_back(HandleOne);
		}
	};
}