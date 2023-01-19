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
//+	shapetypeMathDivide,
//+	shapetypeMathEqual,
//+	shapetypeMathMinus,
//+	shapetypeMathMultiply,
//+	shapetypeMathPlus,
//+	shapetypeMathNotEqual,
namespace cpdoccore 
{
class oox_shape_mathMultiply : public oox_shape
{
public:
	oox_shape_mathMultiply()
	{
		odf_type_name	=L"ooxml-mathMultiply";

		modifiers		= L"23520";
		enhanced_path	= L"M ?f40 ?f41 L ?f42 ?f43 ?f11 ?f46 ?f47 ?f43 ?f48 ?f41 ?f51 ?f8 ?f48 ?f53 ?f47 ?f54 ?f11 ?f55 ?f42 ?f54 ?f40 ?f53 ?f52 ?f8 Z N";
		text_areas		= L"?f36 ?f49 ?f35 ?f48";
		glue_points		= L"?f36 ?f37 ?f56 ?f37 ?f56 ?f57 ?f36 ?f57";
		view_box		= L"0 0 21600 21600";
		
		add(L"f0", L"10800000");
		add(L"f1", L"5400000");
		add(L"f2", L"left");
		add(L"f3", L"right");
		add(L"f4", L"top");
		add(L"f5", L"bottom");
		add(L"f6", L"?f5 - ?f4");
		add(L"f7", L"?f6 / 2");
		add(L"f8", L"?f4 + ?f7");
		add(L"f9", L"?f3 - ?f2");
		add(L"f10", L"?f9 / 2");
		add(L"f11", L"?f2 + ?f10");
		add(L"f12", L"min(?f9, ?f6)");
		add(L"f13", L"5419351 / 1725033");
		add(L"f14", L"$0");
		add(L"f15", L"?f12 * ?f14 / 100000");
		add(L"f16", L"0 - ?f9");
		add(L"f17", L"0 - ?f6");
		add(L"f18", L"atan2(?f16, ?f17)");
		add(L"f19", L"0 - ?f18");
		add(L"f20", L"?f19 * ?f0 / ?f13");
		add(L"f21", L"?f20 - ?f1");
		add(L"f22", L"?f21 + ?f1");
		add(L"f23", L"?f22 * ?f13 / ?f0");
		add(L"f24", L"0 - ?f23");
		add(L"f25", L"cos(?f24)");
		add(L"f26", L"0 - ?f25");
		add(L"f27", L"?f26");
		add(L"f28", L"sin(?f24)");
		add(L"f29", L"0 - ?f28");
		add(L"f30", L"?f29");
		add(L"f31", L"tan(?f24)");
		add(L"f32", L"1 / ?f31");
		add(L"f33", L"sqrt(?f9 * ?f9 + ?f6 * ?f6 + 0 * 0)");
		add(L"f34", L"?f33 * 51965 / 100000");
		add(L"f35", L"?f33 - ?f34");
		add(L"f36", L"?f30 * ?f35 / 2");
		add(L"f37", L"?f27 * ?f35 / 2");
		add(L"f38", L"?f27 * ?f15 / 2");
		add(L"f39", L"?f30 * ?f15 / 2");
		add(L"f40", L"?f36 - ?f38");
		add(L"f41", L"?f37 + ?f39");
		add(L"f42", L"?f36 + ?f38");
		add(L"f43", L"?f37 - ?f39");
		add(L"f44", L"?f11 - ?f42");
		add(L"f45", L"?f44 * ?f32");
		add(L"f46", L"?f45 + ?f43");
		add(L"f47", L"?f3 - ?f42");
		add(L"f48", L"?f3 - ?f40");
		add(L"f49", L"?f8 - ?f41");
		add(L"f50", L"?f49 / ?f32");
		add(L"f51", L"?f48 - ?f50");
		add(L"f52", L"?f40 + ?f50");
		add(L"f53", L"?f5 - ?f41");
		add(L"f54", L"?f5 - ?f43");
		add(L"f55", L"?f5 - ?f46");
		add(L"f56", L"?f3 - ?f36");
		add(L"f57", L"?f5 - ?f37");
/////////////////////////////////////////////////////////
		_handle h;
		h.position = L"0 ?f2";
		h.y_maximum= L"51965";
		h.y_minimum= L"0";

		handles.push_back(h);
	}
};
class oox_shape_mathEqual : public oox_shape
{
public:
	oox_shape_mathEqual()
	{
		odf_type_name	=L"ooxml-mathEqual";

		enhanced_path	= L"M ?f13 ?f10 L ?f14 ?f10 ?f14 ?f8 ?f13 ?f8 Z M ?f13 ?f9 L ?f14 ?f9 ?f14 ?f11 ?f13 ?f11 Z N";
		text_areas		= L"?f13 ?f10 ?f14 ?f11";
		view_box		= L"0 0 0 0";
		modifiers		= L"23520 11760";
		
		add(L"f0", L"if(0-$0 ,0,if(36745-$0 ,$0 ,36745))");
		add(L"f1", L"?f0 *2/1");
		add(L"f2", L"100000+0-2");
		add(L"f3", L"if(0-$1 ,0,if(?f2 -$1 ,$1 ,?f2 ))");
		add(L"f4", L"logheight*?f0 /100000");
		add(L"f5", L"logheight*?f3 /200000");
		add(L"f6", L"logwidth*73490/200000");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 +0-?f5 ");
		add(L"f9", L"?f7 +?f5 -0");
		add(L"f10", L"?f8 +0-?f4 ");
		add(L"f11", L"?f9 +?f4 -0");
		add(L"f12", L"logwidth/2");
		add(L"f13", L"?f12 +0-?f6 ");
		add(L"f14", L"?f12 +?f6 -0");
		add(L"f15", L"(?f10 +?f8 )/2");
		add(L"f16", L"(?f9 +?f11 )/2");
		add(L"f17", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h;
		h.position = L"0 ?f10";
		h.y_maximum= L"36745";
		h.y_minimum= L"0";
		handles.push_back(h);

		h.position = L"?f17 ?f8";
		h.y_maximum= L"?f2";
		h.y_minimum= L"0";
		handles.push_back(h);
	}
};
class oox_shape_mathDivide : public oox_shape
{
public:
	oox_shape_mathDivide()
	{
		odf_type_name	=L"ooxml-mathDivide";

		enhanced_path	= L"M ?f20 ?f18 G ?f11 ?f11 ?f25 ?f26 Z M ?f20 ?f19 G ?f11 ?f11 ?f27 ?f28 Z M ?f21 ?f14 L ?f22 ?f14 ?f22 ?f15 ?f21 ?f15 Z N";
		text_areas		= L"?f21 ?f14 ?f22 ?f15";
		view_box		= L"0 0 0 0";
		modifiers		= L"23520 5880 11760";
		
		add(L"f0", L"if(1000-$0 ,1000,if(36745-$0 ,$0 ,36745))");
		add(L"f1", L"0+0-?f0 ");
		add(L"f2", L"(73490+?f1 )/4");
		add(L"f3", L"36745*logwidth/logheight");
		add(L"f4", L"min(?f2 ,?f3 )");
		add(L"f5", L"if(1000-$2 ,1000,if(?f4 -$2 ,$2 ,?f4 ))");
		add(L"f6", L"-4*?f5 /1");
		add(L"f7", L"73490+?f6 -?f0 ");
		add(L"f8", L"if(0-$1 ,0,if(?f7 -$1 ,$1 ,?f7 ))");
		add(L"f9", L"logheight*?f0 /200000");
		add(L"f10", L"logheight*?f8 /100000");
		add(L"f11", L"logheight*?f5 /100000");
		add(L"f12", L"logwidth*73490/200000");
		add(L"f13", L"logheight/2");
		add(L"f14", L"?f13 +0-?f9 ");
		add(L"f15", L"?f13 +?f9 -0");
		add(L"f16", L"?f10 +?f11 -0");
		add(L"f17", L"?f14 +0-?f16 ");
		add(L"f18", L"?f17 +0-?f11 ");
		add(L"f19", L"logheight+0-?f18 ");
		add(L"f20", L"logwidth/2");
		add(L"f21", L"?f20 +0-?f12 ");
		add(L"f22", L"?f20 +?f12 -0");
		add(L"f23", L"?f20 +0-?f11 ");
		add(L"f24", L"logwidth");
		add(L"f25", L"(16200000)/60000.0");
		add(L"f26", L"(21600000)/60000.0");
		add(L"f27", L"(5400000)/60000.0");
		add(L"f28", L"(21600000)/60000.0");
/////////////////////////////////////////////////////////
		_handle h, h1;
		h.position = L"0 ?f14";
		h.y_maximum= L"36745";
		h.y_minimum= L"1000";
		handles.push_back(h);

		h.position = L"?f24 ?f17";
		h.y_maximum= L"?f7";
		h.y_minimum= L"0";
		handles.push_back(h);
		
		h1.position = L"?f23 0";
		h1.x_maximum= L"?f4";
		h1.x_minimum= L"1000";
		handles.push_back(h1);
	}
};
class oox_shape_mathNotEqual : public oox_shape
{
public:
	oox_shape_mathNotEqual()
	{
		odf_type_name	=L"ooxml-mathNotEqual";

		enhanced_path	= L"M ?f9 ?f14 L ?f24 ?f14 ?f43 ?f47 ?f42 ?f46 ?f34 ?f14 ?f10 ?f14 ?f10 ?f12 ?f35 ?f12 ?f36 ?f13 ?f10 ?f13 ?f10 ?f15 ?f37 ?f15 ?f49 ?f51 ?f48 ?f50 ?f30 ?f15 ?f9 ?f15 ?f9 ?f13 ?f28 ?f13 ?f26 ?f12 ?f9 ?f12 Z N";
		text_areas		= L"?f9 ?f14 ?f10 ?f15";
		view_box		= L"0 0 0 0";
		modifiers		= L"23520 6600000 11760";
		
		add(L"f0", L"if(0-$0 ,0,if(50000-$0 ,$0 ,50000))");
		add(L"f1", L"if(4200000-$1 ,4200000,if(6600000-$1 ,$1 ,6600000))");
		add(L"f2", L"?f0 *2/1");
		add(L"f3", L"100000+0-2");
		add(L"f4", L"if(0-$2 ,0,if(?f3 -$2 ,$2 ,?f3 ))");
		add(L"f5", L"logheight*?f0 /100000");
		add(L"f6", L"logheight*?f4 /200000");
		add(L"f7", L"logwidth*73490/200000");
		add(L"f8", L"logwidth/2");
		add(L"f9", L"?f8 +0-?f7 ");
		add(L"f10", L"?f8 +?f7 -0");
		add(L"f11", L"logheight/2");
		add(L"f12", L"?f11 +0-?f6 ");
		add(L"f13", L"?f11 +?f6 -0");
		add(L"f14", L"?f12 +0-?f5 ");
		add(L"f15", L"?f13 +?f5 -0");
		add(L"f16", L"?f1 +0-5400000");
		add(L"f17", L"logheight/2");
		add(L"f18", L"?f17 *tan(pi*(?f16 )/10800000)");
		add(L"f19", L"sqrt(?f18 *?f18 +?f17 *?f17 +0*0)");
		add(L"f20", L"?f19 *?f5 /?f17 ");
		add(L"f21", L"?f20 *1/2");
		add(L"f22", L"?f8 +?f18 -?f21 ");
		add(L"f23", L"?f18 *?f14 /?f17 ");
		add(L"f24", L"?f22 +0-?f23 ");
		add(L"f25", L"?f18 *?f12 /?f17 ");
		add(L"f26", L"?f22 +0-?f25 ");
		add(L"f27", L"?f18 *?f13 /?f17 ");
		add(L"f28", L"?f22 +0-?f27 ");
		add(L"f29", L"?f18 *?f15 /?f17 ");
		add(L"f30", L"?f22 +0-?f29 ");
		add(L"f31", L"?f18 *2/1");
		add(L"f32", L"?f22 +0-?f31 ");
		add(L"f33", L"?f22 +?f20 -0");
		add(L"f34", L"?f24 +?f20 -0");
		add(L"f35", L"?f26 +?f20 -0");
		add(L"f36", L"?f28 +?f20 -0");
		add(L"f37", L"?f30 +?f20 -0");
		add(L"f38", L"?f32 +?f20 -0");
		add(L"f39", L"?f5 *?f17 /?f19 ");
		add(L"f40", L"?f22 +?f39 -0");
		add(L"f41", L"?f33 +0-?f39 ");
		add(L"f42", L"if(?f16 ,?f40 ,?f33 )");
		add(L"f43", L"if(?f16 ,?f22 ,?f41 )");
		add(L"f44", L"?f5 *?f18 /?f19 ");
		add(L"f45", L"0+0-?f44 ");
		add(L"f46", L"if(?f16 ,?f44 ,0)");
		add(L"f47", L"if(?f16 ,0,?f45 )");
		add(L"f48", L"logwidth+0-?f42 ");
		add(L"f49", L"logwidth+0-?f43 ");
		add(L"f50", L"logheight+0-?f46 ");
		add(L"f51", L"logheight+0-?f47 ");
		add(L"f52", L"(?f42 +?f43 )/2");
		add(L"f53", L"(?f49 +?f48 )/2");
		add(L"f54", L"(?f46 +?f47 )/2");
		add(L"f55", L"(?f14 +?f12 )/2");
		add(L"f56", L"(?f13 +?f15 )/2");
		add(L"f57", L"(?f51 +?f50 )/2");
		add(L"f58", L"logwidth");
/////////////////////////////////////////////////////////
		_handle h, h1;
		h.position = L"0 ?f14";
		h.y_maximum= L"50000";
		h.y_minimum= L"0";
		handles.push_back(h);
		
		h1.position = L"?f43 0";
		handles.push_back(h1);
		
		h.position = L"?f58 ?f12";
		h.y_maximum= L"?f3";
		h.y_minimum= L"0";
		handles.push_back(h);
	}
};
class oox_shape_mathPlus : public oox_shape
{
public:
	oox_shape_mathPlus()
	{
		odf_type_name	=L"ooxml-mathPlus";

		enhanced_path	= L"M ?f6 ?f12 L ?f7 ?f12 ?f7 ?f11 ?f8 ?f11 ?f8 ?f12 ?f9 ?f12 ?f9 ?f13 ?f8 ?f13 ?f8 ?f14 ?f7 ?f14 ?f7 ?f13 ?f6 ?f13 Z N";
		text_areas		= L"?f6 ?f12 ?f9 ?f13";
		view_box		= L"0 0 0 0";
		modifiers		= L"23520";
		
		add(L"f0", L"if(0-$0 ,0,if(73490-$0 ,$0 ,73490))");
		add(L"f1", L"logwidth*73490/200000");
		add(L"f2", L"logheight*73490/200000");
		add(L"f3", L"min(logwidth,logheight)");
		add(L"f4", L"?f3 *?f0 /200000");
		add(L"f5", L"logwidth/2");
		add(L"f6", L"?f5 +0-?f1 ");
		add(L"f7", L"?f5 +0-?f4 ");
		add(L"f8", L"?f5 +?f4 -0");
		add(L"f9", L"?f5 +?f1 -0");
		add(L"f10", L"logheight/2");
		add(L"f11", L"?f10 +0-?f2 ");
		add(L"f12", L"?f10 +0-?f4 ");
		add(L"f13", L"?f10 +?f4 -0");
		add(L"f14", L"?f10 +?f2 -0");

/////////////////////////////////////////////////////////
		_handle h;
		h.position = L"0 ?f12";
		h.y_maximum= L"73490";
		h.y_minimum= L"0";
		handles.push_back(h);
	}
};

class oox_shape_mathMinus : public oox_shape
{
public:
	oox_shape_mathMinus()
	{
		odf_type_name	=L"ooxml-mathMinus";

		enhanced_path	= L"M ?f7 ?f4 L ?f8 ?f4 ?f8 ?f5 ?f7 ?f5 Z N";
		text_areas		= L"?f7 ?f4 ?f8 ?f5";
		view_box		= L"0 0 0 0";
		modifiers		= L"23520";
		
		add(L"f0", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f1", L"logheight*?f0 /200000");
		add(L"f2", L"logwidth*73490/200000");
		add(L"f3", L"logheight/2");
		add(L"f4", L"?f3 +0-?f1 ");
		add(L"f5", L"?f3 +?f1 -0");
		add(L"f6", L"logwidth/2");
		add(L"f7", L"?f6 +0-?f2 ");
		add(L"f8", L"?f6 +?f2 -0");
/////////////////////////////////////////////////////////
		_handle h;
		h.position = L"0 ?f4";
		h.y_maximum= L"100000";
		h.y_minimum= L"0";
		handles.push_back(h);
	}
};
}
