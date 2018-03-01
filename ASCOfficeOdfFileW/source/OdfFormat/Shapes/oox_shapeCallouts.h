/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include "../oox_shape_defines.h"

//+ shapetypeLeftArrowCallout
//+ shapetypeRightArrowCallout
//+ shapetypeUpArrowCallout
//+ shapetypeDownArrowCallout

namespace cpdoccore 
{
class oox_shape_LeftArrowCallout : public oox_shape
{
public:
	oox_shape_LeftArrowCallout()
	{
		odf_type_name	=L"ooxml-leftArrowCallout";

		enhanced_path	= L"M 0 ?f12 L ?f17 ?f13 ?f17 ?f14 ?f19 ?f14 ?f19 0 ?f22 0 ?f22 ?f21 ?f19 ?f21 ?f19 ?f15 ?f17 ?f15 ?f17 ?f16 Z N";
		text_areas		= L"?f19 0 ?f22 ?f21";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 25000 25000 64977";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logheight/?f0 ");
		add(L"f2", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f3", L"?f2 *2/1");
		add(L"f4", L"if(0-$0 ,0,if(?f3 -$0 ,$0 ,?f3 ))");
		add(L"f5", L"100000*logwidth/?f0 ");
		add(L"f6", L"if(0-$2 ,0,if(?f5 -$2 ,$2 ,?f5 ))");
		add(L"f7", L"?f6 *?f0 /logwidth");
		add(L"f8", L"100000+0-?f7 ");
		add(L"f9", L"if(0-$3 ,0,if(?f8 -$3 ,$3 ,?f8 ))");
		add(L"f10", L"?f0 *?f2 /100000");
		add(L"f11", L"?f0 *?f4 /200000");
		add(L"f12", L"logheight/2");
		add(L"f13", L"?f12 +0-?f10 ");
		add(L"f14", L"?f12 +0-?f11 ");
		add(L"f15", L"?f12 +?f11 -0");
		add(L"f16", L"?f12 +?f10 -0");
		add(L"f17", L"?f0 *?f6 /100000");
		add(L"f18", L"logwidth*?f9 /100000");
		add(L"f19", L"logwidth+0-?f18 ");
		add(L"f20", L"(?f19 +logwidth)/2");
		add(L"f21", L"logheight");
		add(L"f22", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h2.position = L"?f17 ?f14";
		h2.y_maximum= L"?f3";
		h2.y_minimum= L"0";
		handles.push_back(h2);
		
		h2.position = L"0 ?f13";
		h2.y_maximum= L"?f1";
		h2.y_minimum= L"0";
		handles.push_back(h2);

		h1.position = L"?f17 0";
		h1.x_maximum= L"?f5";
		h1.x_minimum= L"0";
		handles.push_back(h1);
		
		h1.position = L"?f19 ?f21";
		h1.x_maximum= L"?f8";
		h1.x_minimum= L"0";
		handles.push_back(h1);
	}
};

class oox_shape_RightArrowCallout : public oox_shape
{
public:
	oox_shape_RightArrowCallout()
	{
		odf_type_name	= L"ooxml-rightArrowCallout";

		enhanced_path	= L"M 0 0 L ?f19 0 ?f19 ?f14 ?f18 ?f14 ?f18 ?f13 ?f21 ?f12 ?f18 ?f16 ?f18 ?f15 ?f19 ?f15 ?f19 ?f22 0 ?f22 Z N";
		text_areas		= L"0 0 ?f19 ?f22";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 25000 25000 64977";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logheight/?f0 ");
		add(L"f2", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f3", L"?f2 *2/1");
		add(L"f4", L"if(0-$0 ,0,if(?f3 -$0 ,$0 ,?f3 ))");
		add(L"f5", L"100000*logwidth/?f0 ");
		add(L"f6", L"if(0-$2 ,0,if(?f5 -$2 ,$2 ,?f5 ))");
		add(L"f7", L"?f6 *?f0 /logwidth");
		add(L"f8", L"100000+0-?f7 ");
		add(L"f9", L"if(0-$3 ,0,if(?f8 -$3 ,$3 ,?f8 ))");
		add(L"f10", L"?f0 *?f2 /100000");
		add(L"f11", L"?f0 *?f4 /200000");
		add(L"f12", L"logheight/2");
		add(L"f13", L"?f12 +0-?f10 ");
		add(L"f14", L"?f12 +0-?f11 ");
		add(L"f15", L"?f12 +?f11 -0");
		add(L"f16", L"?f12 +?f10 -0");
		add(L"f17", L"?f0 *?f6 /100000");
		add(L"f18", L"logwidth+0-?f17 ");
		add(L"f19", L"logwidth*?f9 /100000");
		add(L"f20", L"?f19 *1/2");
		add(L"f21", L"logwidth");
		add(L"f22", L"logheight");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h2.position = L"?f18 ?f14";
		h2.y_maximum= L"?f3";
		h2.y_minimum= L"0";
		handles.push_back(h2);
		
		h2.position = L"?f21 ?f13";
		h2.y_maximum= L"?f1";
		h2.y_minimum= L"0";
		handles.push_back(h2);

		h1.position = L"?f18 0";
		h1.x_maximum= L"?f5";
		h1.x_minimum= L"0";
		handles.push_back(h1);
		
		h1.position = L"?f19 ?f22";
		h1.x_maximum= L"?f8";
		h1.x_minimum= L"0";
		handles.push_back(h1);
	}
};

class oox_shape_UpArrowCallout : public oox_shape
{
public:
	oox_shape_UpArrowCallout()
	{
		odf_type_name	=L"ooxml-upArrowCallout";

		enhanced_path	= L"M 0 ?f19 L ?f14 ?f19 ?f14 ?f17 ?f13 ?f17 ?f12 0 ?f16 ?f17 ?f15 ?f17 ?f15 ?f19 ?f21 ?f19 ?f21 ?f22 0 ?f22 Z N";
		text_areas		= L"0 ?f19 ?f21 ?f22";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 25000 25000 64977";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logwidth/?f0 ");
		add(L"f2", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f3", L"?f2 *2/1");
		add(L"f4", L"if(0-$0 ,0,if(?f3 -$0 ,$0 ,?f3 ))");
		add(L"f5", L"100000*logheight/?f0 ");
		add(L"f6", L"if(0-$2 ,0,if(?f5 -$2 ,$2 ,?f5 ))");
		add(L"f7", L"?f6 *?f0 /logheight");
		add(L"f8", L"100000+0-?f7 ");
		add(L"f9", L"if(0-$3 ,0,if(?f8 -$3 ,$3 ,?f8 ))");
		add(L"f10", L"?f0 *?f2 /100000");
		add(L"f11", L"?f0 *?f4 /200000");
		add(L"f12", L"logwidth/2");
		add(L"f13", L"?f12 +0-?f10 ");
		add(L"f14", L"?f12 +0-?f11 ");
		add(L"f15", L"?f12 +?f11 -0");
		add(L"f16", L"?f12 +?f10 -0");
		add(L"f17", L"?f0 *?f6 /100000");
		add(L"f18", L"logheight*?f9 /100000");
		add(L"f19", L"logheight+0-?f18 ");
		add(L"f20", L"(?f19 +logheight)/2");
		add(L"f21", L"logwidth");
		add(L"f22", L"logheight");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h1.position = L"?f14 ?f17";
		h1.x_maximum= L"?f3";
		h1.x_minimum= L"0";
		handles.push_back(h1);
		
		h1.position = L"?f13 0";
		h1.x_maximum= L"?f1";
		h1.x_minimum= L"0";
		handles.push_back(h1);

		h2.position = L"?f21 ?f17";
		h2.y_maximum= L"?f5";
		h2.y_minimum= L"0";
		handles.push_back(h2);
		
		h2.position = L"0 ?f19";
		h2.y_maximum= L"?f8";
		h2.y_minimum= L"0";
		handles.push_back(h2);
	}
};

class oox_shape_DownArrowCallout : public oox_shape
{
public:
	oox_shape_DownArrowCallout()
	{
		odf_type_name	=L"ooxml-downArrowCallout";

		enhanced_path	= L"M 0 0 L ?f22 0 ?f22 ?f19 ?f15 ?f19 ?f15 ?f18 ?f16 ?f18 ?f12 ?f21 ?f13 ?f18 ?f14 ?f18 ?f14 ?f19 0 ?f19 Z N";
		text_areas		= L"0 0 ?f22 ?f19";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 25000 25000 64977";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logwidth/?f0 ");
		add(L"f2", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f3", L"?f2 *2/1");
		add(L"f4", L"if(0-$0 ,0,if(?f3 -$0 ,$0 ,?f3 ))");
		add(L"f5", L"100000*logheight/?f0 ");
		add(L"f6", L"if(0-$2 ,0,if(?f5 -$2 ,$2 ,?f5 ))");
		add(L"f7", L"?f6 *?f0 /logheight");
		add(L"f8", L"100000+0-?f7 ");
		add(L"f9", L"if(0-$3 ,0,if(?f8 -$3 ,$3 ,?f8 ))");
		add(L"f10", L"?f0 *?f2 /100000");
		add(L"f11", L"?f0 *?f4 /200000");
		add(L"f12", L"logwidth/2");
		add(L"f13", L"?f12 +0-?f10 ");
		add(L"f14", L"?f12 +0-?f11 ");
		add(L"f15", L"?f12 +?f11 -0");
		add(L"f16", L"?f12 +?f10 -0");
		add(L"f17", L"?f0 *?f6 /100000");
		add(L"f18", L"logheight+0-?f17 ");
		add(L"f19", L"logheight*?f9 /100000");
		add(L"f20", L"?f19 *1/2");
		add(L"f21", L"logheight");
		add(L"f22", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h1.position = L"?f14 ?f18";
		h1.x_maximum= L"?f3";
		h1.x_minimum= L"0";
		handles.push_back(h1);
		
		h1.position = L"?f13 ?f21";
		h1.x_maximum= L"?f1";
		h1.x_minimum= L"0";
		handles.push_back(h1);

		h2.position = L"?f22 ?f18";
		h2.y_maximum= L"?f5";
		h2.y_minimum= L"0";
		handles.push_back(h2);
		
		h2.position = L"0 ?f19";
		h2.y_maximum= L"?f8";
		h2.y_minimum= L"0";
		handles.push_back(h2);
	}
};

}