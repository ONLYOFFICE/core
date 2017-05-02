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
//	shapetypeCurvedConnector2,
//	shapetypeCurvedConnector3,
//	shapetypeCurvedConnector4,
//	shapetypeCurvedConnector5,
//+	shapetypeCurvedDownArrow,
//+	shapetypeCurvedLeftArrow,
//+	shapetypeCurvedRightArrow,
//+	shapetypeCurvedUpArrow,

#include "../oox_shape_defines.h"

namespace cpdoccore 
{
class oox_shape_CurvedLeftArrow : public oox_shape
{
public:
	oox_shape_CurvedLeftArrow()
	{
		odf_type_name	=L"ooxml-curvedLeftArrow";

		enhanced_path	= L"M 0 ?f31 L ?f32 ?f28 ?f32 ?f24 G ?f44 ?f8 ?f45 ?f46 ?f44 ?f8 ?f47 ?f48 L ?f32 ?f29 Z S N M ?f42 ?f18 G ?f44 ?f8 ?f49 ?f50 L 0 0 G ?f44 ?f8 ?f51 ?f52 Z I S N M ?f42 ?f18 G ?f44 ?f8 ?f53 ?f54 L 0 0 G ?f44 ?f8 ?f55 ?f56 L ?f42 ?f18 G ?f44 ?f8 ?f57 ?f58 L ?f32 ?f29 0 ?f31 ?f32 ?f28 ?f32 ?f24 G ?f44 ?f8 ?f59 ?f60 F N";
		text_areas		= L"0 0 ?f42 ?f43";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 50000 25000";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logheight/?f0 ");
		add(L"f2", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f3", L"if(0-$0 ,0,if(?f2 -$0 ,$0 ,?f2 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"?f0 *?f2 /100000");
		add(L"f6", L"(?f4 +?f5 )/4");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f8 *2/1");
		add(L"f10", L"?f9 *?f9 /1");
		add(L"f11", L"?f4 *?f4 /1");
		add(L"f12", L"?f10 +0-?f11 ");
		add(L"f13", L"sqrt(?f12 )");
		add(L"f14", L"?f13 *logwidth/?f9 ");
		add(L"f15", L"100000*?f14 /?f0 ");
		add(L"f16", L"if(0-$2 ,0,if(?f15 -$2 ,$2 ,?f15 ))");
		add(L"f17", L"?f0 *?f16 /100000");
		add(L"f18", L"?f8 +?f4 -0");
		add(L"f19", L"logwidth*logwidth/1");
		add(L"f20", L"?f17 *?f17 /1");
		add(L"f21", L"?f19 +0-?f20 ");
		add(L"f22", L"sqrt(?f21 )");
		add(L"f23", L"?f22 *?f8 /logwidth");
		add(L"f24", L"?f8 +?f23 -0");
		add(L"f25", L"?f18 +?f23 -0");
		add(L"f26", L"?f5 +0-?f4 ");
		add(L"f27", L"?f26 *1/2");
		add(L"f28", L"?f24 +0-?f27 ");
		add(L"f29", L"?f25 +?f27 -0");
		add(L"f30", L"?f5 *1/2");
		add(L"f31", L"logheight+0-?f30 ");
		add(L"f32", L"0+?f17 -0");
		add(L"f33", L"(10800000*atan2(?f23 ,?f17 ))/pi");
		add(L"f34", L"0+0-?f33 ");
		add(L"f35", L"0+?f14 -0");
		add(L"f36", L"(?f8 +?f18 )/2");
		add(L"f37", L"?f4 *1/2");
		add(L"f38", L"(10800000*atan2(?f37 ,?f14 ))/pi");
		add(L"f39", L"?f38 +0-?f33 ");
		add(L"f40", L"?f33 +?f38 -0");
		add(L"f41", L"0+0-?f38 ");
		add(L"f42", L"logwidth");
		add(L"f43", L"logheight");
		add(L"f44", L"logwidth");
		add(L"f45", L"(?f33 )/60000.0");
		add(L"f46", L"(?f39 )/60000.0");
		add(L"f47", L"(?f41 )/60000.0");
		add(L"f48", L"(?f40 )/60000.0");
		add(L"f49", L"(0)/60000.0");
		add(L"f50", L"(-5400000)/60000.0");
		add(L"f51", L"(16200000)/60000.0");
		add(L"f52", L"(5400000)/60000.0");
		add(L"f53", L"(0)/60000.0");
		add(L"f54", L"(-5400000)/60000.0");
		add(L"f55", L"(16200000)/60000.0");
		add(L"f56", L"(5400000)/60000.0");
		add(L"f57", L"(0)/60000.0");
		add(L"f58", L"(?f33 )/60000.0");
		add(L"f59", L"(?f33 )/60000.0");
		add(L"f60", L"(?f39 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h1.position = L"?f32 ?f24";
		h1.y_maximum= L"?f2";
		h1.y_minimum= L"0";
		handles.push_back(h1);

		h1.position = L"?f42 ?f28";
		h1.y_maximum= L"?f1";
		h1.y_minimum= L"0";
		handles.push_back(h1);
		
		h2.position = L"?f32 ?f43";
		h2.x_maximum= L"?f15";
		h2.x_minimum= L"0";
		handles.push_back(h2);
	}
};
class oox_shape_CurvedRightArrow : public oox_shape
{
public:
	oox_shape_CurvedRightArrow()
	{
		odf_type_name	=L"ooxml-curvedRightArrow";

		enhanced_path	= L"M 0 ?f8 G ?f45 ?f8 ?f46 ?f47 L ?f32 ?f28 ?f43 ?f31 ?f32 ?f29 ?f32 ?f25 G ?f45 ?f8 ?f48 ?f49 Z S N M ?f43 ?f4 G ?f45 ?f8 ?f50 ?f51 ?f45 ?f8 ?f52 ?f53 Z I S N M 0 ?f8 G ?f45 ?f8 ?f54 ?f55 L ?f32 ?f28 ?f43 ?f31 ?f32 ?f29 ?f32 ?f25 G ?f45 ?f8 ?f56 ?f57 L 0 ?f8 G ?f45 ?f8 ?f58 ?f59 L ?f43 ?f4 G ?f45 ?f8 ?f60 ?f61 F N";
		text_areas		= L"0 0 ?f43 ?f44";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 50000 25000";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logheight/?f0 ");
		add(L"f2", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f3", L"if(0-$0 ,0,if(?f2 -$0 ,$0 ,?f2 ))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"?f0 *?f2 /100000");
		add(L"f6", L"(?f4 +?f5 )/4");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f8 *2/1");
		add(L"f10", L"?f9 *?f9 /1");
		add(L"f11", L"?f4 *?f4 /1");
		add(L"f12", L"?f10 +0-?f11 ");
		add(L"f13", L"sqrt(?f12 )");
		add(L"f14", L"?f13 *logwidth/?f9 ");
		add(L"f15", L"100000*?f14 /?f0 ");
		add(L"f16", L"if(0-$2 ,0,if(?f15 -$2 ,$2 ,?f15 ))");
		add(L"f17", L"?f0 *?f16 /100000");
		add(L"f18", L"?f8 +?f4 -0");
		add(L"f19", L"logwidth*logwidth/1");
		add(L"f20", L"?f17 *?f17 /1");
		add(L"f21", L"?f19 +0-?f20 ");
		add(L"f22", L"sqrt(?f21 )");
		add(L"f23", L"?f22 *?f8 /logwidth");
		add(L"f24", L"?f8 +?f23 -0");
		add(L"f25", L"?f18 +?f23 -0");
		add(L"f26", L"?f5 +0-?f4 ");
		add(L"f27", L"?f26 *1/2");
		add(L"f28", L"?f24 +0-?f27 ");
		add(L"f29", L"?f25 +?f27 -0");
		add(L"f30", L"?f5 *1/2");
		add(L"f31", L"logheight+0-?f30 ");
		add(L"f32", L"logwidth+0-?f17 ");
		add(L"f33", L"(10800000*atan2(?f23 ,?f17 ))/pi");
		add(L"f34", L"10800000+0-?f33 ");
		add(L"f35", L"0+0-?f33 ");
		add(L"f36", L"logwidth+0-?f14 ");
		add(L"f37", L"(?f8 +?f18 )/2");
		add(L"f38", L"?f4 *1/2");
		add(L"f39", L"(10800000*atan2(?f38 ,?f14 ))/pi");
		add(L"f40", L"?f39 +0-5400000");
		add(L"f41", L"5400000+?f39 -0");
		add(L"f42", L"10800000+0-?f39 ");
		add(L"f43", L"logwidth");
		add(L"f44", L"logheight");
		add(L"f45", L"logwidth");
		add(L"f46", L"(10800000)/60000.0");
		add(L"f47", L"(?f35 )/60000.0");
		add(L"f48", L"(?f34 )/60000.0");
		add(L"f49", L"(?f33 )/60000.0");
		add(L"f50", L"(16200000)/60000.0");
		add(L"f51", L"(?f40 )/60000.0");
		add(L"f52", L"(?f42 )/60000.0");
		add(L"f53", L"(?f41 )/60000.0");
		add(L"f54", L"(10800000)/60000.0");
		add(L"f55", L"(?f35 )/60000.0");
		add(L"f56", L"(?f34 )/60000.0");
		add(L"f57", L"(?f33 )/60000.0");
		add(L"f58", L"(10800000)/60000.0");
		add(L"f59", L"(5400000)/60000.0");
		add(L"f60", L"(16200000)/60000.0");
		add(L"f61", L"(?f40 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h1.position = L"?f32 ?f24";
		h1.y_maximum= L"?f2";
		h1.y_minimum= L"0";
		handles.push_back(h1);

		h1.position = L"?f43 ?f28";
		h1.y_maximum= L"?f1";
		h1.y_minimum= L"0";
		handles.push_back(h1);
		
		h2.position = L"?f32 ?f43";
		h2.x_maximum= L"?f15";
		h2.x_minimum= L"0";
		handles.push_back(h2);
	}
};
class oox_shape_CurvedDownArrow : public oox_shape
{
public:
	oox_shape_CurvedDownArrow()
	{
		odf_type_name	=L"ooxml-curvedDownArrow";

		enhanced_path	= L"M ?f31 ?f43 L ?f28 ?f32 ?f24 ?f32 G ?f8 ?f45 ?f46 ?f47 L ?f18 0 G ?f8 ?f45 ?f48 ?f49 L ?f29 ?f32 Z S N M ?f36 ?f35 G ?f8 ?f45 ?f50 ?f51 L 0 ?f43 G ?f8 ?f45 ?f52 ?f53 Z I S N M ?f36 ?f35 G ?f8 ?f45 ?f54 ?f55 L 0 ?f43 G ?f8 ?f45 ?f56 ?f57 L ?f18 0 G ?f8 ?f45 ?f58 ?f59 L ?f29 ?f32 ?f31 ?f43 ?f28 ?f32 ?f24 ?f32 G ?f8 ?f45 ?f60 ?f61 F N";
		text_areas		= L"0 0 ?f44 ?f43";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 50000 25000";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logwidth/?f0 ");
		add(L"f2", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f3", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"?f0 *?f2 /100000");
		add(L"f6", L"(?f4 +?f5 )/4");
		add(L"f7", L"logwidth/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f8 *2/1");
		add(L"f10", L"?f9 *?f9 /1");
		add(L"f11", L"?f4 *?f4 /1");
		add(L"f12", L"?f10 +0-?f11 ");
		add(L"f13", L"sqrt(?f12 )");
		add(L"f14", L"?f13 *logheight/?f9 ");
		add(L"f15", L"100000*?f14 /?f0 ");
		add(L"f16", L"if(0-$2 ,0,if(?f15 -$2 ,$2 ,?f15 ))");
		add(L"f17", L"?f0 *$2 /100000");
		add(L"f18", L"?f8 +?f4 -0");
		add(L"f19", L"logheight*logheight/1");
		add(L"f20", L"?f17 *?f17 /1");
		add(L"f21", L"?f19 +0-?f20 ");
		add(L"f22", L"sqrt(?f21 )");
		add(L"f23", L"?f22 *?f8 /logheight");
		add(L"f24", L"?f8 +?f23 -0");
		add(L"f25", L"?f18 +?f23 -0");
		add(L"f26", L"?f5 +0-?f4 ");
		add(L"f27", L"?f26 *1/2");
		add(L"f28", L"?f24 +0-?f27 ");
		add(L"f29", L"?f25 +?f27 -0");
		add(L"f30", L"?f5 *1/2");
		add(L"f31", L"logwidth+0-?f30 ");
		add(L"f32", L"logheight+0-?f17 ");
		add(L"f33", L"(10800000*atan2(?f23 ,?f17 ))/pi");
		add(L"f34", L"0+0-?f33 ");
		add(L"f35", L"logheight+0-?f14 ");
		add(L"f36", L"(?f8 +?f18 )/2");
		add(L"f37", L"?f4 *1/2");
		add(L"f38", L"(10800000*atan2(?f37 ,?f14 ))/pi");
		add(L"f39", L"16200000+?f33 -0");
		add(L"f40", L"16200000+0-?f38 ");
		add(L"f41", L"?f38 +0-5400000");
		add(L"f42", L"5400000+?f38 -0");
		add(L"f43", L"logheight");
		add(L"f44", L"logwidth");
		add(L"f45", L"logheight");
		add(L"f46", L"(?f39 )/60000.0");
		add(L"f47", L"(?f34 )/60000.0");
		add(L"f48", L"(16200000)/60000.0");
		add(L"f49", L"(?f33 )/60000.0");
		add(L"f50", L"(?f40 )/60000.0");
		add(L"f51", L"(?f41 )/60000.0");
		add(L"f52", L"(10800000)/60000.0");
		add(L"f53", L"(?f42 )/60000.0");
		add(L"f54", L"(?f40 )/60000.0");
		add(L"f55", L"(?f41 )/60000.0");
		add(L"f56", L"(10800000)/60000.0");
		add(L"f57", L"(5400000)/60000.0");
		add(L"f58", L"(16200000)/60000.0");
		add(L"f59", L"(?f33 )/60000.0");
		add(L"f60", L"(?f39 )/60000.0");
		add(L"f61", L"(?f34 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h1.position = L"?f25 ?f32";
		h1.x_maximum= L"$1";
		h1.x_minimum= L"0";
		handles.push_back(h1);

		h1.position = L"?f28 ?f43";
		h1.x_maximum= L"?f1";
		h1.x_minimum= L"0";
		handles.push_back(h1);
		
		h2.position = L"?f44 ?f32";
		h2.y_maximum= L"?f15";
		h2.y_minimum= L"0";
		handles.push_back(h2);

	}
};
class oox_shape_CurvedUpArrow : public oox_shape
{
public:
	oox_shape_CurvedUpArrow()
	{
		odf_type_name	=L"ooxml-curvedUpArrow";

		enhanced_path	= L"M ?f31 0 L ?f29 ?f32 ?f25 ?f32 G ?f8 ?f46 ?f47 ?f48 ?f8 ?f46 ?f49 ?f50 L ?f28 ?f32 Z S N M ?f8 ?f45 G ?f8 ?f46 ?f51 ?f52 L ?f4 0 G ?f8 ?f46 ?f53 ?f54 Z I S N M ?f36 ?f35 G ?f8 ?f46 ?f55 ?f56 L ?f28 ?f32 ?f31 0 ?f29 ?f32 ?f25 ?f32 G ?f8 ?f46 ?f57 ?f58 L ?f8 ?f45 G ?f8 ?f46 ?f59 ?f60 L ?f4 0 G ?f8 ?f46 ?f61 ?f62 F N";
		text_areas		= L"0 0 ?f44 ?f45";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000 50000 25000";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logwidth/?f0 ");
		add(L"f2", L"if(0-$1 ,0,if(?f1 -$1 ,$1 ,?f1 ))");
		add(L"f3", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f4", L"?f0 *?f3 /100000");
		add(L"f5", L"?f0 *?f2 /100000");
		add(L"f6", L"(?f4 +?f5 )/4");
		add(L"f7", L"logwidth/2");
		add(L"f8", L"?f7 +0-?f6 ");
		add(L"f9", L"?f8 *2/1");
		add(L"f10", L"?f9 *?f9 /1");
		add(L"f11", L"?f4 *?f4 /1");
		add(L"f12", L"?f10 +0-?f11 ");
		add(L"f13", L"sqrt(?f12 )");
		add(L"f14", L"?f13 *logheight/?f9 ");
		add(L"f15", L"100000*?f14 /?f0 ");
		add(L"f16", L"if(0-$2 ,0,if(?f15 -$2 ,$2 ,?f15 ))");
		add(L"f17", L"?f0 *$2 /100000");
		add(L"f18", L"?f8 +?f4 -0");
		add(L"f19", L"logheight*logheight/1");
		add(L"f20", L"?f17 *?f17 /1");
		add(L"f21", L"?f19 +0-?f20 ");
		add(L"f22", L"sqrt(?f21 )");
		add(L"f23", L"?f22 *?f8 /logheight");
		add(L"f24", L"?f8 +?f23 -0");
		add(L"f25", L"?f18 +?f23 -0");
		add(L"f26", L"?f5 +0-?f4 ");
		add(L"f27", L"?f26 *1/2");
		add(L"f28", L"?f24 +0-?f27 ");
		add(L"f29", L"?f25 +?f27 -0");
		add(L"f30", L"?f5 *1/2");
		add(L"f31", L"logwidth+0-?f30 ");
		add(L"f32", L"0+?f17 -0");
		add(L"f33", L"(10800000*atan2(?f23 ,?f17 ))/pi");
		add(L"f34", L"0+0-?f33 ");
		add(L"f35", L"0+?f14 -0");
		add(L"f36", L"(?f8 +?f18 )/2");
		add(L"f37", L"?f4 *1/2");
		add(L"f38", L"(10800000*atan2(?f37 ,?f14 ))/pi");
		add(L"f39", L"?f38 +0-?f33 ");
		add(L"f40", L"0+0-?f39 ");
		add(L"f41", L"5400000+0-?f33 ");
		add(L"f42", L"?f33 +?f38 -0");
		add(L"f43", L"5400000+0-?f38 ");
		add(L"f44", L"logwidth");
		add(L"f45", L"logheight");
		add(L"f46", L"logheight");
		add(L"f47", L"(?f41 )/60000.0");
		add(L"f48", L"(?f42 )/60000.0");
		add(L"f49", L"(?f43 )/60000.0");
		add(L"f50", L"(?f39 )/60000.0");
		add(L"f51", L"(5400000)/60000.0");
		add(L"f52", L"(5400000)/60000.0");
		add(L"f53", L"(10800000)/60000.0");
		add(L"f54", L"(-5400000)/60000.0");
		add(L"f55", L"(?f43 )/60000.0");
		add(L"f56", L"(?f39 )/60000.0");
		add(L"f57", L"(?f41 )/60000.0");
		add(L"f58", L"(?f33 )/60000.0");
		add(L"f59", L"(5400000)/60000.0");
		add(L"f60", L"(5400000)/60000.0");
		add(L"f61", L"(10800000)/60000.0");
		add(L"f62", L"(-5400000)/60000.0");
/////////////////////////////////////////////////////////
		_handle h1, h2;
		
		h1.position = L"?f25 ?f32";
		h1.x_maximum= L"?f2";
		h1.x_minimum= L"0";
		handles.push_back(h1);

		h1.position = L"?f28 0";
		h1.x_maximum= L"?f1";
		h1.x_minimum= L"0";
		handles.push_back(h1);
		
		h2.position = L"?f44 ?f32";
		h2.y_maximum= L"?f15";
		h2.y_minimum= L"0";
		handles.push_back(h2);

	}
};

}
