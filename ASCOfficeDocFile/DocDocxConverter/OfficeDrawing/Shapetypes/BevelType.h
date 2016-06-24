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
	class BevelType : public ShapeType
	{
	public:
		BevelType () : ShapeType(msosptBevel)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
			Path				=	_T("m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0e");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum width 0 #0"));
			Formulas.push_back(_T("sum height 0 #0"));
			Formulas.push_back(_T("prod width 1 2"));
			Formulas.push_back(_T("prod height 1 2"));
			Formulas.push_back(_T("prod #0 1 2"));
			Formulas.push_back(_T("prod #0 3 2"));
			Formulas.push_back(_T("sum @1 @5 0"));
			Formulas.push_back(_T("sum @2 @5 0"));

			AdjustmentValues	=	_T("2700");
			ConnectorLocations	=	_T("0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0");
			TextBoxRectangle	=	_T("@0,@0,@1,@2");

			Handle one;
			one.position		=	_T("#0,topLeft");
			one.xrange			=	_T("0,10800"); 
			Handles.push_back (one);
       
			Limo				=	_T("10800,10800");
		}
	};
}