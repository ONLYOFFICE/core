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
#include "../oox_shape_defines.h"

//+ shapetypeBentUpArrow
//	shapetypeBentArrow,
//	shapetypeBentConnector2,
//	shapetypeBentConnector3,
//	shapetypeBentConnector4,
//	shapetypeBentConnector5,

namespace cpdoccore 
{
class oox_shape_BentUpArrow : public oox_shape
{
public:
	oox_shape_BentUpArrow()
	{
		odf_type_name	=L"ooxml-bentUpArrow";

		enhanced_path	= L"M 0 ?f13 L ?f10 ?f13 ?f10 ?f4 ?f6 ?f4 ?f8 0 ?f18 ?f4 ?f11 ?f4 ?f11 ?f17 0 ?f17 Z N";
		text_areas		= L"0 ?f13 ?f11 ?f17";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 25000 25000";
		
		add(L"f0", L"if(0-$0 ,0,if(50000-$0 ,$0 ,50000))");
		add(L"f1", L"if(0-$1 ,0,if(50000-$1 ,$1 ,50000))");
		add(L"f2", L"if(0-$2 ,0,if(50000-$2 ,$2 ,50000))");
		add(L"f3", L"min(logwidth,logheight)");
		add(L"f4", L"?f3 *?f2 /100000");
		add(L"f5", L"?f3 *?f1 /50000");
		add(L"f6", L"logwidth+0-?f5 ");
		add(L"f7", L"?f3 *?f1 /100000");
		add(L"f8", L"logwidth+0-?f7 ");
		add(L"f9", L"?f3 *?f0 /200000");
		add(L"f10", L"?f8 +0-?f9 ");
		add(L"f11", L"?f8 +?f9 -0");
		add(L"f12", L"?f3 *?f0 /100000");
		add(L"f13", L"logheight+0-?f12 ");
		add(L"f14", L"?f11 *1/2");
		add(L"f15", L"(?f13 +logheight)/2");
		add(L"f16", L"(?f4 +logheight)/2");
		add(L"f17", L"logheight");
		add(L"f18", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h1.position = L"0 ?f13";
		h1.y_maximum= L"50000";
		h1.y_minimum= L"0";
		handles.push_back(h1);
		
		h2.position = L"?f6 0";
		h2.x_maximum= L"50000";
		h2.x_minimum= L"0";
		handles.push_back(h2);

		h1.position = L"?f10 ?f4";
		h1.y_maximum= L"50000";
		h1.y_minimum= L"0";
		handles.push_back(h1);
	}
};


}