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
#include "../oox_shape_defines.h"

//+ shapetypeLeftArrow,
//+	shapetypeRightArrow,
//+ shapetypeDownArrow,
//+ shapetypeUpArrow,
//+ shapetypeLeftRightArrow
//+	shapetypeUpDownArrow
//+	shapetypeLeftCircularArrow,
//	shapetypeLeftRightCircularArrow,
//+	shapetypeSwooshArrow,
//+	shapetypeCircularArrow
//+	shapetypeNotchedRightArrow

//...
namespace cpdoccore 
{
class oox_shape_LeftArrow : public oox_shape
{
public:
	oox_shape_LeftArrow()
	{
		odf_type_name	= L"ooxml-leftArrow";

		enhanced_path	= L"M 0 ?f7 L ?f5 0 ?f5 ?f8 ?f13 ?f8 ?f13 ?f9 ?f5 ?f9 ?f5 ?f14 Z N";
		text_areas		= L"?f12 ?f8 ?f13 ?f9";
		modifiers		= L"50000 50000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logwidth/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f3", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"0+?f4 -0");
		add(L"f6", L"logheight*?f2 /200000");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f7 +?f6 -0");
		add(L"f10", L"logheight/2");
		add(L"f11", L"?f8 *?f4 /?f10 ");
		add(L"f12", L"?f5 +0-?f11 ");
		add(L"f13", L"logwidth");
		add(L"f14", L"logheight");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"?f13 ?f8";
		h1.y_minimum = L"0";
		h1.y_maximum = L"100000";
		
		handles.push_back(h1);

 		h2.position = L"?f5 0";
		h2.x_minimum = L"0";
		h2.x_maximum = L"?f1";
		handles.push_back(h2);
	}
};
class oox_shape_RightArrow : public oox_shape
{
public:
	oox_shape_RightArrow()
	{
		odf_type_name	= L"ooxml-rightArrow";

		enhanced_path	= L"M 0 ?f8 L ?f5 ?f8 ?f5 0 ?f14 ?f7 ?f5 ?f13 ?f5 ?f9 0 ?f9 Z N";
		text_areas		= L"?f12 ?f8 ?f13 ?f9";
		modifiers		= L"50000 50000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logwidth/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f3", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"logwidth+0-?f4 ");
		add(L"f6", L"logheight*?f2 /200000");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f7 +?f6 -0");
		add(L"f10", L"logheight/2");
		add(L"f11", L"?f8 *?f4 /?f10 ");
		add(L"f12", L"?f5 +?f11 -0");
		add(L"f13", L"logheight");
		add(L"f14", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"0 ?f8";
		h1.y_minimum = L"0";
		h1.y_maximum = L"100000";
		
		handles.push_back(h1);

 		h2.position = L"?f5 0";
		h2.x_minimum = L"0";
		h2.x_maximum = L"?f1";
		handles.push_back(h2);
	}
};
class oox_shape_DownArrow : public oox_shape
{
public:
	oox_shape_DownArrow()
	{
		odf_type_name	= L"ooxml-downArrow";

		enhanced_path	= L"M 0 ?f5 L ?f8 ?f5 ?f8 0 ?f9 0 ?f9 ?f5 ?f14 ?f5 ?f7 ?f13 Z N";
		text_areas		= L"?f8 0 ?f9 ?f12";
		modifiers		= L"50000 50000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logheight/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f3", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"logheight+0-?f4 ");
		add(L"f6", L"logwidth*?f2 /200000");
		add(L"f7", L"logwidth/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f7 +?f6 -0");
		add(L"f10", L"logwidth/2");
		add(L"f11", L"?f8 *?f4 /?f10 ");
		add(L"f12", L"?f5 +?f11 -0");
		add(L"f13", L"logheight");
		add(L"f14", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"?f8 0";
		h1.y_minimum = L"0";
		h1.y_maximum = L"100000";
		
		handles.push_back(h1);

 		h2.position = L"0 ?f5";
		h2.x_minimum = L"0";
		h2.x_maximum = L"?f1";
		handles.push_back(h2);
	}
};
class oox_shape_UpArrow : public oox_shape
{
public:
	oox_shape_UpArrow()
	{
		odf_type_name	= L"ooxml-upArrow";

		enhanced_path	= L"M 0 ?f5 L ?f7 0 ?f14 ?f5 ?f9 ?f5 ?f9 ?f13 ?f8 ?f13 ?f8 ?f5 Z N";
		text_areas		= L"?f8 ?f12 ?f9 ?f13";
		modifiers		= L"50000 50000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logheight/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f3", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"0+?f4 -0");
		add(L"f6", L"logwidth*?f2 /200000");
		add(L"f7", L"logwidth/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f7 +?f6 -0");
		add(L"f10", L"logwidth/2");
		add(L"f11", L"?f8 *?f4 /?f10 ");
		add(L"f12", L"?f5 +0-?f11 ");
		add(L"f13", L"logheight");
		add(L"f14", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"?f8 ?f13";
		h1.x_minimum = L"0";
		h1.x_maximum = L"100000";
		
		handles.push_back(h1);

 		h2.position = L"0 ?f5";
		h2.y_minimum = L"0";
		h2.y_maximum = L"?f1";
		handles.push_back(h2);
	}
};
class oox_shape_LeftRightArrow : public oox_shape
{
public:
	oox_shape_LeftRightArrow()
	{
		odf_type_name	= L"ooxml-leftRightArrow";

		enhanced_path	= L"M 0 ?f7 L ?f4 0 ?f4 ?f8 ?f5 ?f8 ?f5 0 ?f14 ?f7 ?f5 ?f15 ?f5 ?f9 ?f4 ?f9 ?f4 ?f15 Z N";
		text_areas		= L"?f12 ?f8 ?f13 ?f9";
		modifiers		= L"50000 50000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logwidth/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f3", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"logwidth+0-?f4 ");
		add(L"f6", L"logheight*?f2 /200000");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f7 +?f6 -0");
		add(L"f10", L"logheight/2");
		add(L"f11", L"?f8 *?f4 /?f10 ");
		add(L"f12", L"?f4 +0-?f11 ");
		add(L"f13", L"?f5 +?f11 -0");
		add(L"f14", L"logwidth");
		add(L"f15", L"logheight");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"?f5 ?f8";
		h1.y_minimum = L"0";
		h1.y_maximum = L"100000";
		
		handles.push_back(h1);

 		h2.position = L"?f4 0";
		h2.x_minimum = L"0";
		h2.x_maximum = L"?f1";
		handles.push_back(h2);
	}
};
class oox_shape_LeftUpArrow : public oox_shape
{
public:
	oox_shape_LeftUpArrow()
	{
		odf_type_name	= L"ooxml-leftUpArrow";

		enhanced_path	= L"M 0 ?f12 L ?f6 ?f9 ?f6 ?f16 ?f14 ?f16 ?f14 ?f6 ?f8 ?f6 ?f11 0 ?f22 ?f6 ?f15 ?f6 ?f15 ?f17 ?f6 ?f17 ?f6 ?f21 Z N";
		text_areas		= L"?f18 ?f16 ?f11 ?f17";
		modifiers		= L"25000 25000 25000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(0-$1 ,0,if(50000-$1 ,$1 ,50000))");
		add(L"f1", L"?f0 *2/1");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"100000+0-?f1 ");
		add(L"f4", L"if(0-$2 ,0,if(?f3 -$2 ,$2 ,?f3 ))");
		add(L"f5", L"min(logwidth,logheight)");
		add(L"f6", L"?f5 *?f4 /100000");
		add(L"f7", L"?f5 *?f0 /50000");
		add(L"f8", L"logwidth+0-?f7 ");
		add(L"f9", L"logheight+0-?f7 ");
		add(L"f10", L"?f5 *?f0 /100000");
		add(L"f11", L"logwidth+0-?f10 ");
		add(L"f12", L"logheight+0-?f10 ");
		add(L"f13", L"?f5 *?f2 /200000");
		add(L"f14", L"?f11 +0-?f13 ");
		add(L"f15", L"?f11 +?f13 -0");
		add(L"f16", L"?f12 +0-?f13 ");
		add(L"f17", L"?f12 +?f13 -0");
		add(L"f18", L"?f13 *?f6 /?f10 ");
		add(L"f19", L"(?f6 +?f15 )/2");
		add(L"f20", L"(?f6 +?f17 )/2");
		add(L"f21", L"logheight");
		add(L"f22", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"?f14 ?f16";
		h1.y_minimum = L"0";
		h1.y_maximum = L"?f1";		
		handles.push_back(h1);

 		h2.position = L"?f8 0";
		h2.x_minimum = L"0";
		h2.x_maximum = L"50000";
		handles.push_back(h2);
		
		h1.position = L"?f14 ?f6";
		h1.y_minimum = L"0";
		h1.y_maximum = L"?f3";		
		handles.push_back(h1);
	}
};
class oox_shape_UpDownArrow : public oox_shape
{
public:
	oox_shape_UpDownArrow()
	{
		odf_type_name	= L"ooxml-upDownArrow";

		enhanced_path	= L"M 0 ?f4 L ?f7 0 ?f16 ?f4 ?f9 ?f4 ?f9 ?f5 ?f16 ?f5 ?f7 ?f15 0 ?f5 ?f8 ?f5 ?f8 ?f4 Z N";
		text_areas		= L"?f8 ?f12 ?f9 ?f13";
		modifiers		= L"50000 50000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logheight/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f3", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"logheight+0-?f4 ");
		add(L"f6", L"logwidth*?f2 /200000");
		add(L"f7", L"logwidth/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f7 +?f6 -0");
		add(L"f10", L"logwidth/2");
		add(L"f11", L"?f8 *?f4 /?f10 ");
		add(L"f12", L"?f4 +0-?f11 ");
		add(L"f13", L"?f5 +?f11 -0");
		add(L"f14", L"logheight/2");
		add(L"f15", L"logheight");
		add(L"f16", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;

 		h2.position = L"?f8 ?f5";
		h2.x_minimum = L"0";
		h2.x_maximum = L"100000";
		handles.push_back(h2);
		
		h1.position = L"0 ?f4";
		h1.y_minimum = L"0";
		h1.y_maximum = L"?f1";
		
		handles.push_back(h1);
	}
};
class oox_shape_LeftCircularArrow : public oox_shape
{
public:
	oox_shape_LeftCircularArrow()
	{
		odf_type_name	=L"ooxml-leftCircularArrow";

		enhanced_path	= L"M ?f63 ?f64 L ?f69 ?f70 G ?f13 ?f14 ?f210 ?f211 L ?f185 ?f186 ?f57 ?f58 ?f183 ?f184 ?f126 ?f127 G ?f10 ?f12 ?f212 ?f213 Z N";
		text_areas		= L"?f206 ?f208 ?f207 ?f209";
		modifiers		= L"10980 1142322 6300000 18900000 12500";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(0-$4 ,0,if(25000-$4 ,$4 ,25000))");
		add(L"f1", L"?f0 *2/1");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"if(1-$2 ,1,if(21599999-$2 ,$2 ,21599999))");
		add(L"f4", L"if(0-$3 ,0,if(21599999-$3 ,$3 ,21599999))");
		add(L"f5", L"min(logwidth,logheight)");
		add(L"f6", L"?f5 *?f2 /100000");
		add(L"f7", L"?f5 *?f0 /100000");
		add(L"f8", L"?f6 *1/2");
		add(L"f9", L"logwidth/2");
		add(L"f10", L"?f9 +?f8 -?f7 ");
		add(L"f11", L"logheight/2");
		add(L"f12", L"?f11 +?f8 -?f7 ");
		add(L"f13", L"?f10 +0-?f6 ");
		add(L"f14", L"?f12 +0-?f6 ");
		add(L"f15", L"?f13 +?f8 -0");
		add(L"f16", L"?f14 +?f8 -0");
		add(L"f17", L"?f15 *sin(pi*(?f3 )/10800000)");
		add(L"f18", L"?f16 *cos(pi*(?f3 )/10800000)");
		add(L"f19", L"?f15 *(cos(atan2(?f17 ,?f18 )))");
		add(L"f20", L"?f16 *(sin(atan2(?f17 ,?f18 )))");
		add(L"f21", L"logwidth/2");
		add(L"f22", L"?f21 +?f19 -0");
		add(L"f23", L"logheight/2");
		add(L"f24", L"?f23 +?f20 -0");
		add(L"f25", L"min(?f13 ,?f14 )");
		add(L"f26", L"?f19 *?f19 /1");
		add(L"f27", L"?f20 *?f20 /1");
		add(L"f28", L"?f25 *?f25 /1");
		add(L"f29", L"?f26 +0-?f28 ");
		add(L"f30", L"?f27 +0-?f28 ");
		add(L"f31", L"?f29 *?f30 /?f26 ");
		add(L"f32", L"?f31 *1/?f27 ");
		add(L"f33", L"1+0-?f32 ");
		add(L"f34", L"sqrt(?f33 )");
		add(L"f35", L"?f29 *1/?f19 ");
		add(L"f36", L"?f35 *1/?f20 ");
		add(L"f37", L"(1+?f34 )/?f36 ");
		add(L"f38", L"(10800000*atan2(?f37 ,1))/pi");
		add(L"f39", L"?f38 +21600000-0");
		add(L"f40", L"if(?f38 ,?f38 ,?f39 )");
		add(L"f41", L"?f40 +0-?f3 ");
		add(L"f42", L"?f41 +21600000-0");
		add(L"f43", L"if(?f41 ,?f41 ,?f42 )");
		add(L"f44", L"?f43 +0-10800000");
		add(L"f45", L"?f43 +0-21600000");
		add(L"f46", L"if(?f44 ,?f45 ,?f43 )");
		add(L"f47", L"abs(?f46 )");
		add(L"f48", L"?f47 *-1/1");
		add(L"f49", L"abs($1 )");
		add(L"f50", L"?f49 *-1/1");
		add(L"f51", L"if(?f48 -?f50 ,?f48 ,if(0-?f50 ,?f50 ,0))");
		add(L"f52", L"?f3 +?f51 -0");
		add(L"f53", L"?f15 *sin(pi*(?f52 )/10800000)");
		add(L"f54", L"?f16 *cos(pi*(?f52 )/10800000)");
		add(L"f55", L"?f15 *(cos(atan2(?f53 ,?f54 )))");
		add(L"f56", L"?f16 *(sin(atan2(?f53 ,?f54 )))");
		add(L"f57", L"?f21 +?f55 -0");
		add(L"f58", L"?f23 +?f56 -0");
		add(L"f59", L"?f10 *sin(pi*(?f4 )/10800000)");
		add(L"f60", L"?f12 *cos(pi*(?f4 )/10800000)");
		add(L"f61", L"?f10 *(cos(atan2(?f59 ,?f60 )))");
		add(L"f62", L"?f12 *(sin(atan2(?f59 ,?f60 )))");
		add(L"f63", L"?f21 +?f61 -0");
		add(L"f64", L"?f23 +?f62 -0");
		add(L"f65", L"?f13 *sin(pi*(?f4 )/10800000)");
		add(L"f66", L"?f14 *cos(pi*(?f4 )/10800000)");
		add(L"f67", L"?f13 *(cos(atan2(?f65 ,?f66 )))");
		add(L"f68", L"?f14 *(sin(atan2(?f65 ,?f66 )))");
		add(L"f69", L"?f21 +?f67 -0");
		add(L"f70", L"?f23 +?f68 -0");
		add(L"f71", L"?f7 *cos(pi*(?f52 )/10800000)");
		add(L"f72", L"?f7 *sin(pi*(?f52 )/10800000)");
		add(L"f73", L"?f22 +?f71 -0");
		add(L"f74", L"?f24 +?f72 -0");
		add(L"f75", L"?f7 *cos(pi*(?f52 )/10800000)");
		add(L"f76", L"?f7 *sin(pi*(?f52 )/10800000)");
		add(L"f77", L"?f22 +0-?f75 ");
		add(L"f78", L"?f24 +0-?f76 ");
		add(L"f79", L"?f77 +0-?f21 ");
		add(L"f80", L"?f78 +0-?f23 ");
		add(L"f81", L"?f73 +0-?f21 ");
		add(L"f82", L"?f74 +0-?f23 ");
		add(L"f83", L"min(?f10 ,?f12 )");
		add(L"f84", L"?f79 *?f83 /?f10 ");
		add(L"f85", L"?f80 *?f83 /?f12 ");
		add(L"f86", L"?f81 *?f83 /?f10 ");
		add(L"f87", L"?f82 *?f83 /?f12 ");
		add(L"f88", L"?f86 +0-?f84 ");
		add(L"f89", L"?f87 +0-?f85 ");
		add(L"f90", L"sqrt(?f88 *?f88 +?f89 *?f89 +0*0)");
		add(L"f91", L"?f84 *?f87 /1");
		add(L"f92", L"?f86 *?f85 /1");
		add(L"f93", L"?f91 +0-?f92 ");
		add(L"f94", L"?f83 *?f83 /1");
		add(L"f95", L"?f90 *?f90 /1");
		add(L"f96", L"?f94 *?f95 /1");
		add(L"f97", L"?f93 *?f93 /1");
		add(L"f98", L"?f96 +0-?f97 ");
		add(L"f99", L"max(?f98 ,0)");
		add(L"f100", L"sqrt(?f99 )");
		add(L"f101", L"?f89 *-1/1");
		add(L"f102", L"if(?f101 ,-1,1)");
		add(L"f103", L"?f102 *?f88 /1");
		add(L"f104", L"?f103 *?f100 /1");
		add(L"f105", L"?f93 *?f89 /1");
		add(L"f106", L"(?f105 +?f104 )/?f95 ");
		add(L"f107", L"?f105 +0-?f104 ");
		add(L"f108", L"?f107 *1/?f95 ");
		add(L"f109", L"abs(?f89 )");
		add(L"f110", L"?f109 *?f100 /1");
		add(L"f111", L"?f93 *?f88 /-1");
		add(L"f112", L"(?f111 +?f110 )/?f95 ");
		add(L"f113", L"?f111 +0-?f110 ");
		add(L"f114", L"?f113 *1/?f95 ");
		add(L"f115", L"?f86 +0-?f106 ");
		add(L"f116", L"?f86 +0-?f108 ");
		add(L"f117", L"?f87 +0-?f112 ");
		add(L"f118", L"?f87 +0-?f114 ");
		add(L"f119", L"sqrt(?f115 *?f115 +?f117 *?f117 +0*0)");
		add(L"f120", L"sqrt(?f116 *?f116 +?f118 *?f118 +0*0)");
		add(L"f121", L"?f120 +0-?f119 ");
		add(L"f122", L"if(?f121 ,?f106 ,?f108 )");
		add(L"f123", L"if(?f121 ,?f112 ,?f114 )");
		add(L"f124", L"?f122 *?f10 /?f83 ");
		add(L"f125", L"?f123 *?f12 /?f83 ");
		add(L"f126", L"?f21 +?f124 -0");
		add(L"f127", L"?f23 +?f125 -0");
		add(L"f128", L"?f79 *?f25 /?f13 ");
		add(L"f129", L"?f80 *?f25 /?f14 ");
		add(L"f130", L"?f81 *?f25 /?f13 ");
		add(L"f131", L"?f82 *?f25 /?f14 ");
		add(L"f132", L"?f130 +0-?f128 ");
		add(L"f133", L"?f131 +0-?f129 ");
		add(L"f134", L"sqrt(?f132 *?f132 +?f133 *?f133 +0*0)");
		add(L"f135", L"?f128 *?f131 /1");
		add(L"f136", L"?f130 *?f129 /1");
		add(L"f137", L"?f135 +0-?f136 ");
		add(L"f138", L"?f25 *?f25 /1");
		add(L"f139", L"?f134 *?f134 /1");
		add(L"f140", L"?f138 *?f139 /1");
		add(L"f141", L"?f137 *?f137 /1");
		add(L"f142", L"?f140 +0-?f141 ");
		add(L"f143", L"max(?f142 ,0)");
		add(L"f144", L"sqrt(?f143 )");
		add(L"f145", L"?f102 *?f132 /1");
		add(L"f146", L"?f145 *?f144 /1");
		add(L"f147", L"?f137 *?f133 /1");
		add(L"f148", L"(?f147 +?f146 )/?f139 ");
		add(L"f149", L"?f147 +0-?f146 ");
		add(L"f150", L"?f149 *1/?f139 ");
		add(L"f151", L"abs(?f133 )");
		add(L"f152", L"?f151 *?f144 /1");
		add(L"f153", L"?f137 *?f132 /-1");
		add(L"f154", L"(?f153 +?f152 )/?f139 ");
		add(L"f155", L"?f153 +0-?f152 ");
		add(L"f156", L"?f155 *1/?f139 ");
		add(L"f157", L"?f128 +0-?f148 ");
		add(L"f158", L"?f128 +0-?f150 ");
		add(L"f159", L"?f129 +0-?f154 ");
		add(L"f160", L"?f129 +0-?f156 ");
		add(L"f161", L"sqrt(?f157 *?f157 +?f159 *?f159 +0*0)");
		add(L"f162", L"sqrt(?f158 *?f158 +?f160 *?f160 +0*0)");
		add(L"f163", L"?f162 +0-?f161 ");
		add(L"f164", L"if(?f163 ,?f148 ,?f150 )");
		add(L"f165", L"if(?f163 ,?f154 ,?f156 )");
		add(L"f166", L"?f164 *?f13 /?f25 ");
		add(L"f167", L"?f165 *?f14 /?f25 ");
		add(L"f168", L"?f21 +?f166 -0");
		add(L"f169", L"?f23 +?f167 -0");
		add(L"f170", L"(10800000*atan2(?f167 ,?f166 ))/pi");
		add(L"f171", L"?f170 +21600000-0");
		add(L"f172", L"if(?f170 ,?f170 ,?f171 )");
		add(L"f173", L"?f4 +0-?f172 ");
		add(L"f174", L"?f173 +21600000-0");
		add(L"f175", L"if(?f173 ,?f173 ,?f174 )");
		add(L"f176", L"?f172 +?f175 -0");
		add(L"f177", L"0+0-?f175 ");
		add(L"f178", L"?f126 +0-?f168 ");
		add(L"f179", L"?f127 +0-?f169 ");
		add(L"f180", L"sqrt(?f178 *?f178 +?f179 *?f179 +0*0)");
		add(L"f181", L"?f180 *1/2");
		add(L"f182", L"?f181 +0-?f7 ");
		add(L"f183", L"if(?f182 ,?f126 ,?f73 )");
		add(L"f184", L"if(?f182 ,?f127 ,?f74 )");
		add(L"f185", L"if(?f182 ,?f168 ,?f77 )");
		add(L"f186", L"if(?f182 ,?f169 ,?f78 )");
		add(L"f187", L"(10800000*atan2(?f125 ,?f124 ))/pi");
		add(L"f188", L"?f187 +21600000-0");
		add(L"f189", L"if(?f187 ,?f187 ,?f188 )");
		add(L"f190", L"?f189 +0-?f4 ");
		add(L"f191", L"?f190 +0-21600000");
		add(L"f192", L"if(?f190 ,?f191 ,?f190 )");
		add(L"f193", L"?f4 +?f192 -0");
		add(L"f194", L"0+0-?f192 ");
		add(L"f195", L"?f15 *sin(pi*(?f4 )/10800000)");
		add(L"f196", L"?f16 *cos(pi*(?f4 )/10800000)");
		add(L"f197", L"?f15 *(cos(atan2(?f195 ,?f196 )))");
		add(L"f198", L"?f16 *(sin(atan2(?f195 ,?f196 )))");
		add(L"f199", L"?f21 +?f197 -0");
		add(L"f200", L"?f23 +?f198 -0");
		add(L"f201", L"?f4 +5400000-0");
		add(L"f202", L"?f52 +0-5400000");
		add(L"f203", L"?f52 +10800000-0");
		add(L"f204", L"?f10 *cos(pi*(2700000)/10800000)");
		add(L"f205", L"?f12 *sin(pi*(2700000)/10800000)");
		add(L"f206", L"?f21 +0-?f204 ");
		add(L"f207", L"?f21 +?f204 -0");
		add(L"f208", L"?f23 +0-?f205 ");
		add(L"f209", L"?f23 +?f205 -0");
		add(L"f210", L"(?f176 )/60000.0");
		add(L"f211", L"(?f177 )/60000.0");
		add(L"f212", L"(?f193 )/60000.0");
		add(L"f213", L"(?f194 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f57 ?f58";
		handles.push_back(h);

 		h.position = L"?f63 ?f64";
		handles.push_back(h);

 		h.position = L"?f126 ?f127";
		h.r_minimum = L"0";
		h.r_maximum = L"?f1";
		handles.push_back(h);

 		h.position = L"?f77 ?f78";
		h.r_minimum = L"0";
		h.r_maximum = L"25000";
		handles.push_back(h);
	}
};
class oox_shape_CircularArrow : public oox_shape
{
public:
	oox_shape_CircularArrow()
	{
		odf_type_name	= L"ooxml-circularArrow";

		enhanced_path	= L"M 534468 1430865 A ?f91 ?f92 ?f93 ?f94 534468 1430865 ?f88 ?f90  W ?f95 ?f96 ?f97 ?f98 534468 1430865 ?f88 ?f90 L 853493 790375 695381 939240 547047 613089 643004 668602 A ?f138 ?f139 ?f140 ?f141 643004 668602 ?f135 ?f137  W ?f142 ?f143 ?f144 ?f145 643004 668602 ?f135 ?f137 Z N";
		text_areas		= L"?f48 ?f50 ?f49 ?f51";
		view_box		= L"0 0 880110 1583055";
		glue_points		= L"?f40 ?f41 ?f42 ?f43 ?f44 ?f45 ?f46 ?f47";
		
		add(L"f0", L"10800000");
		add(L"f1", L"5400000");
		add(L"f2", L"left");
		add(L"f3", L"right");
		add(L"f4", L"top");
		add(L"f5", L"bottom");
		add(L"f6", L"?f5 - ?f4");
		add(L"f7", L"?f3 - ?f2");
		add(L"f8", L"5419351 / 1725033");
		add(L"f9", L"180");
		add(L"f10", L"?f7 / 880110");
		add(L"f11", L"?f6 / 1583055");
		add(L"f12", L"695381 - 440055");
		add(L"f13", L"939240 - 791527");
		add(L"f14", L"sqrt(?f12 * ?f12 + ?f13 * ?f13 + 0 * 0)");
		add(L"f15", L"atan2(?f12, ?f13)");
		add(L"f16", L"?f15 + ?f1");
		add(L"f17", L"?f16 * ?f9 / ?f0");
		add(L"f18", L"0 - ?f17");
		add(L"f19", L"534468 - 440055");
		add(L"f20", L"1430865 - 791527");
		add(L"f21", L"sqrt(?f19 * ?f19 + ?f20 * ?f20 + 0 * 0)");
		add(L"f22", L"atan2(?f19, ?f20)");
		add(L"f23", L"?f22 + ?f1");
		add(L"f24", L"?f23 * ?f9 / ?f0");
		add(L"f25", L"0 - ?f24");
		add(L"f26", L"756945 - 440055");
		add(L"f27", L"734520 - 791527");
		add(L"f28", L"sqrt(?f26 * ?f26 + ?f27 * ?f27 + 0 * 0)");
		add(L"f29", L"atan2(?f26, ?f27)");
		add(L"f30", L"?f29 + ?f1");
		add(L"f31", L"?f30 * ?f9 / ?f0");
		add(L"f32", L"0 - ?f31");
		add(L"f33", L"547047 - 440055");
		add(L"f34", L"613089 - 791527");
		add(L"f35", L"sqrt(?f33 * ?f33 + ?f34 * ?f34 + 0 * 0)");
		add(L"f36", L"atan2(?f33, ?f34)");
		add(L"f37", L"?f36 + ?f1");
		add(L"f38", L"?f37 * ?f9 / ?f0");
		add(L"f39", L"0 - ?f38");
		add(L"f40", L"525840 / ?f10");
		add(L"f41", L"1372440 / ?f11");
		add(L"f42", L"853493 / ?f10");
		add(L"f43", L"790375 / ?f11");
		add(L"f44", L"695381 / ?f10");
		add(L"f45", L"939240 / ?f11");
		add(L"f46", L"547047 / ?f10");
		add(L"f47", L"613089 / ?f11");
		add(L"f48", L"215163 / ?f10");
		add(L"f49", L"664947 / ?f10");
		add(L"f50", L"318107 / ?f11");
		add(L"f51", L"1264948 / ?f11");
		add(L"f52", L"21550000 - 16092125");
		add(L"f53", L"if(?f52, 16092125, 21550000)");
		add(L"f54", L"-21550000 - ?f53");
		add(L"f55", L"if(?f54, -21550000, ?f53)");
		add(L"f56", L"4895979 + ?f55");
		add(L"f57", L"4895979 + ?f1");
		add(L"f58", L"?f57 * ?f8 / ?f0");
		add(L"f59", L"0 - ?f58");
		add(L"f60", L"cos(?f59)");
		add(L"f61", L"0 - ?f60");
		add(L"f62", L"?f61 * 318045");
		add(L"f63", L"sin(?f59)");
		add(L"f64", L"0 - ?f63");
		add(L"f65", L"?f64 * 669518");
		add(L"f66", L"sqrt(?f62 * ?f62 + ?f65 * ?f65 + 0 * 0)");
		add(L"f67", L"318045 * 669518 / ?f66");
		add(L"f68", L"?f64 * ?f67");
		add(L"f69", L"534468 - ?f68");
		add(L"f70", L"?f61 * ?f67");
		add(L"f71", L"1430865 - ?f70");
		add(L"f72", L"?f69 - 318045");
		add(L"f73", L"?f71 - 669518");
		add(L"f74", L"?f69 + 318045");
		add(L"f75", L"?f71 + 669518");
		add(L"f76", L"?f56 + ?f1");
		add(L"f77", L"?f76 * ?f8 / ?f0");
		add(L"f78", L"0 - ?f77");
		add(L"f79", L"cos(?f78)");
		add(L"f80", L"0 - ?f79");
		add(L"f81", L"?f80 * 318045");
		add(L"f82", L"sin(?f78)");
		add(L"f83", L"0 - ?f82");
		add(L"f84", L"?f83 * 669518");
		add(L"f85", L"sqrt(?f81 * ?f81 + ?f84 * ?f84 + 0 * 0)");
		add(L"f86", L"318045 * 669518 / ?f85");
		add(L"f87", L"?f83 * ?f86");
		add(L"f88", L"?f69 + ?f87");
		add(L"f89", L"?f80 * ?f86");
		add(L"f90", L"?f71 + ?f89");
		add(L"f91", L"if(?f55, 534468, ?f72)");
		add(L"f92", L"if(?f55, 1430865, ?f73)");
		add(L"f93", L"if(?f55, 534468, ?f74)");
		add(L"f94", L"if(?f55, 1430865, ?f75)");
		add(L"f95", L"if(?f55, ?f72, ?f88)");
		add(L"f96", L"if(?f55, ?f73, ?f90)");
		add(L"f97", L"if(?f55, ?f74, ?f88)");
		add(L"f98", L"if(?f55, ?f75, ?f90)");
		add(L"f99", L"21550000 - -14831828");
		add(L"f100", L"if(?f99, -14831828, 21550000)");
		add(L"f101", L"-21550000 - ?f100");
		add(L"f102", L"if(?f101, -21550000, ?f100)");
		add(L"f103", L"19727807 + ?f102");
		add(L"f104", L"19727807 + ?f1");
		add(L"f105", L"?f104 * ?f8 / ?f0");
		add(L"f106", L"0 - ?f105");
		add(L"f107", L"cos(?f106)");
		add(L"f108", L"0 - ?f107");
		add(L"f109", L"?f108 * 208032");
		add(L"f110", L"sin(?f106)");
		add(L"f111", L"0 - ?f110");
		add(L"f112", L"?f111 * 559504");
		add(L"f113", L"sqrt(?f109 * ?f109 + ?f112 * ?f112 + 0 * 0)");
		add(L"f114", L"208032 * 559504 / ?f113");
		add(L"f115", L"?f111 * ?f114");
		add(L"f116", L"643004 - ?f115");
		add(L"f117", L"?f108 * ?f114");
		add(L"f118", L"668602 - ?f117");
		add(L"f119", L"?f116 - 208032");
		add(L"f120", L"?f118 - 559504");
		add(L"f121", L"?f116 + 208032");
		add(L"f122", L"?f118 + 559504");
		add(L"f123", L"?f103 + ?f1");
		add(L"f124", L"?f123 * ?f8 / ?f0");
		add(L"f125", L"0 - ?f124");
		add(L"f126", L"cos(?f125)");
		add(L"f127", L"0 - ?f126");
		add(L"f128", L"?f127 * 208032");
		add(L"f129", L"sin(?f125)");
		add(L"f130", L"0 - ?f129");
		add(L"f131", L"?f130 * 559504");
		add(L"f132", L"sqrt(?f128 * ?f128 + ?f131 * ?f131 + 0 * 0)");
		add(L"f133", L"208032 * 559504 / ?f132");
		add(L"f134", L"?f130 * ?f133");
		add(L"f135", L"?f116 + ?f134");
		add(L"f136", L"?f127 * ?f133");
		add(L"f137", L"?f118 + ?f136");
		add(L"f138", L"if(?f102, 643004, ?f119)");
		add(L"f139", L"if(?f102, 668602, ?f120)");
		add(L"f140", L"if(?f102, 643004, ?f121)");
		add(L"f141", L"if(?f102, 668602, ?f122)");
		add(L"f142", L"if(?f102, ?f119, ?f135)");
		add(L"f143", L"if(?f102, ?f120, ?f137)");
		add(L"f144", L"if(?f102, ?f121, ?f135)");
		add(L"f145", L"if(?f102, ?f122, ?f137)");
	}
};
class oox_shape_SwooshArrow : public oox_shape
{
public:
	oox_shape_SwooshArrow()
	{
		odf_type_name	= L"ooxml-swooshArrow";

		enhanced_path	= L"M 0 ?f30 Q ?f25 ?f23 ?f6 ?f8 L ?f11 0 ?f31 ?f20 ?f15 ?f16 ?f14 ?f13 Q ?f29 ?f27 0 ?f30 Z N";
		text_areas		= L"0 0 ?f31 ?f30";
		modifiers		= L"25000 25000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(1-$0 ,1,if(75000-$0 ,$0 ,75000))");
		add(L"f1", L"min(logwidth,logheight)");
		add(L"f2", L"70000*logwidth/?f1 ");
		add(L"f3", L"if(0-$1 ,0,if(?f2 -$1 ,$1 ,?f2 ))");
		add(L"f4", L"logheight*?f0 /100000");
		add(L"f5", L"?f1 *?f3 /100000");
		add(L"f6", L"logwidth+0-?f5 ");
		add(L"f7", L"min(logwidth,logheight)/8");
		add(L"f8", L"0+?f7 -0");
		add(L"f9", L"5400000*1/14");
		add(L"f10", L"?f7 *tan(pi*(?f9 )/10800000)");
		add(L"f11", L"?f6 +0-?f10 ");
		add(L"f12", L"?f4 *tan(pi*(?f9 )/10800000)");
		add(L"f13", L"?f8 +?f4 -0");
		add(L"f14", L"?f6 +?f12 -0");
		add(L"f15", L"?f14 +?f10 -0");
		add(L"f16", L"?f13 +?f7 -0");
		add(L"f17", L"?f16 +0-0");
		add(L"f18", L"?f17 *1/2");
		add(L"f19", L"logheight*1/20");
		add(L"f20", L"0+?f18 -?f19 ");
		add(L"f21", L"logheight/6");
		add(L"f22", L"?f21 *1/1");
		add(L"f23", L"?f21 +?f22 -0");
		add(L"f24", L"logwidth/6");
		add(L"f25", L"?f24 ");
		add(L"f26", L"?f21 *1/2");
		add(L"f27", L"?f13 +?f26 -0");
		add(L"f28", L"logwidth/4");
		add(L"f29", L"?f28 ");
		add(L"f30", L"logheight");
		add(L"f31", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"?f54 ?f55";
		h1.y_minimum = L"1";
		h1.y_maximum = L"75000";
		
		handles.push_back(h1);

 		h2.position = L"?f60 ?f61";
		h2.x_minimum = L"0";
		h2.x_maximum = L"?f2";
		handles.push_back(h2);
	}
};
class oox_shape_leftRightUpArrow : public oox_shape
{
public:
	oox_shape_leftRightUpArrow()
	{
		odf_type_name	= L"ooxml-leftRightUpArrow";

		enhanced_path	= L"M 0 ?f18 L ?f7 ?f17 ?f7 ?f19 ?f13 ?f19 ?f13 ?f7 ?f10 ?f7 ?f9 0 ?f11 ?f7 ?f14 ?f7 ?f14 ?f19 ?f15 ?f19 ?f15 ?f17 ?f23 ?f18 ?f15 ?f24 ?f15 ?f20 ?f7 ?f20 ?f7 ?f24 Z N";
		text_areas		= L"?f21 ?f19 ?f22 ?f20";
		modifiers		= L"9928 10359 14667";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(0-$1 ,0,if(50000-$1 ,$1 ,50000))"); 
		add(L"f1", L"?f0 *2/1"); 
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))"); 
		add(L"f3", L"100000+0-?f1 "); 
		add(L"f4", L"?f3 *1/2"); 
		add(L"f5", L"if(0-$2 ,0,if(?f4 -$2 ,$2 ,?f4 ))"); 
		add(L"f6", L"min(logwidth,logheight)"); 
		add(L"f7", L"?f6 *?f5 /100000"); 
		add(L"f8", L"?f6 *?f0 /100000"); 
		add(L"f9", L"logwidth/2"); 
		add(L"f10", L"?f9 +0-?f8 "); 
		add(L"f11", L"?f9 +?f8 -0"); 
		add(L"f12", L"?f6 *?f2 /200000"); 
		add(L"f13", L"?f9 +0-?f12 "); 
		add(L"f14", L"?f9 +?f12 -0"); 
		add(L"f15", L"logwidth+0-?f7 "); 
		add(L"f16", L"?f6 *?f0 /50000"); 
		add(L"f17", L"logheight+0-?f16 "); 
		add(L"f18", L"logheight+0-?f8 "); 
		add(L"f19", L"?f18 +0-?f12 "); 
		add(L"f20", L"?f18 +?f12 -0"); 
		add(L"f21", L"?f12 *?f7 /?f8 "); 
		add(L"f22", L"logwidth+0-?f21 "); 
		add(L"f23", L"logwidth"); 
		add(L"f24", L"logheight");
/////////////////////////////////////////////////////////
		_handle h1, h2, h3;
		h1.position = L"?f13 ?f7";
		h1.x_minimum = L"0";
		h1.x_maximum = L"?f1";		
		handles.push_back(h1);

 		h2.position = L"?f10 0";
		h2.x_minimum = L"0";
		h2.x_maximum = L"50000";
		handles.push_back(h2);

 		h3.position = L"?f23 ?f7";
		h3.y_minimum = L"0";
		h3.y_maximum = L"?f4";
		handles.push_back(h3);
	}
};
class oox_shape_NotchedRightArrow : public oox_shape
{
public:
	oox_shape_NotchedRightArrow()
	{
		odf_type_name	= L"ooxml-notchedRightArrow";

		enhanced_path	= L"M 0 ?f8 L ?f5 ?f8 ?f5 0 ?f13 ?f7 ?f5 ?f14 ?f5 ?f9 0 ?f9 ?f11 ?f7 Z N";
		text_areas		= L"?f11 ?f8 ?f12 ?f9";
		modifiers		= L"50000 50000";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logwidth/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f3", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"logwidth+0-?f4 ");
		add(L"f6", L"logheight*?f2 /200000");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f7 +?f6 -0");
		add(L"f10", L"logheight/2");
		add(L"f11", L"?f6 *?f4 /?f10 ");
		add(L"f12", L"logwidth+0-?f11 ");
		add(L"f13", L"logwidth");
		add(L"f14", L"logheight");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"?f13 ?f8";
		h1.y_minimum = L"0";
		h1.y_maximum = L"100000";
		
		handles.push_back(h1);

 		h2.position = L"?f5 0";
		h2.x_minimum = L"0";
		h2.x_maximum = L"?f1";
		handles.push_back(h2);
	}
};

}