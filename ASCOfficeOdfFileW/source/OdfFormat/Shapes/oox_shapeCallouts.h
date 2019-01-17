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

//+ shapetypeLeftArrowCallout
//+ shapetypeRightArrowCallout
//+ shapetypeUpArrowCallout
//+ shapetypeDownArrowCallout
//+ shapetypeCallout1
//+ shapetypeCallout2
//- shapetypeCallout3
//+ shapetypeWedgeRectCallout
//+ shapetypeWedgeRoundRectCallout
//+ shapetypeCloudCallout
//+ shapetypeWedgeEllipseCallout
//+ shapetypeQuadArrowCallout

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
class oox_shape_Callout1 : public oox_shape
{
public:
	oox_shape_Callout1()
	{
		odf_type_name	=L"ooxml-callout1";

		enhanced_path	= L"S M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 112500 -38333";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f13 / 100000");
		add(L"f9", L"?f5 * ?f12 / 100000");
		add(L"f10", L"?f4 * ?f15 / 100000");
		add(L"f11", L"?f5 * ?f14 / 100000");
		add(L"f12", L"$1 / ?f7");
		add(L"f13", L"$0 / ?f7");
		add(L"f14", L"$3 / ?f7");
		add(L"f15", L"$2 / ?f7");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Callout2 : public oox_shape
{
public:
	oox_shape_Callout2()
	{
		odf_type_name	=L"ooxml-callout2";

		enhanced_path	= L"S M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 ?f13 ?f12 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 18750 -16667 112500 -46667";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f15 / 100000");
		add(L"f9", L"?f5 * ?f14 / 100000");
		add(L"f10", L"?f4 * ?f17 / 100000");
		add(L"f11", L"?f5 * ?f16 / 100000");
		add(L"f12", L"?f4 * ?f19 / 100000");
		add(L"f13", L"?f5 * ?f18 / 100000");
		add(L"f14", L"$1 / ?f7");
		add(L"f15", L"$0 / ?f7");
		add(L"f16", L"$3 / ?f7");
		add(L"f17", L"$2 / ?f7");
		add(L"f18", L"$5 / ?f7");
		add(L"f19", L"$4 / ?f7");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_CloudCallout : public oox_shape
{
public:
	oox_shape_CloudCallout()
	{
		odf_type_name	=L"ooxml-cloudCallout";

		enhanced_path	= L"M 1930 7160 C 1530 4490 3400 1970 5270 1970 5860 1950 6470 2210 6970 2600 7450 1390 8340 650 9340 650 10004 690 10710 1050 11210 1700 11570 630 12330 0 13150 0 13840 0 14470 460 14870 1160 15330 440 16020 0 16740 0 17910 0 18900 1130 19110 2710 20240 3150 21060 4580 21060 6220 21060 6720 21000 7200 20830 7660 21310 8460 21600 9450 21600 10460 21600 12750 20310 14680 18650 15010 18650 17200 17370 18920 15770 18920 15220 18920 14700 18710 14240 18310 13820 20240 12490 21600 11000 21600 9890 21600 8840 20790 8210 19510 7620 20000 7930 20290 6240 20290 4850 20290 3570 19280 2900 17640 1300 17600 480 16300 480 14660 480 13900 690 13210 1070 12640 380 12160 0 11210 0 10120 0 8590 840 7330 1930 7160 Z N M 1930 7160 C 1950 7410 2040 7690 2090 7920 F N M 6970 2600 C 7200 2790 7480 3050 7670 3310 F N M 11210 1700 C 11130 1910 11080 2160 11030 2400 F N M 14870 1160 C 14720 1400 14640 1720 14540 2010 F N M 19110 2710 C 19130 2890 19230 3290 19190 3380 F N M 20830 7660 C 20660 8170 20430 8620 20110 8990 F N M 18660 15010 C 18740 14200 18280 12200 17000 11450 F N M 14240 18310 C 14320 17980 14350 17680 14370 17360 F N M 8220 19510 C 8060 19250 7960 18950 7860 18640 F N M 2900 17640 C 3090 17600 3280 17540 3460 17450 F N M 1070 12640 C 1400 12900 1780 13130 2330 13040 F N U ?f17 ?f18 1800 1800 0 23592960 Z N U ?f19 ?f20 1200 1200 0 23592960 Z N U ?f13 ?f14 700 700 0 23592960 Z N";
		text_areas		= L"3000 3320 17110 17330";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"6300 24300";
		glue_points		= L"0 10800 10800 21600 21600 10800 10800 0 ?f13 ?f14";
		
		add(L"f0", L"$0 -10800");
		add(L"f1", L"$1 -10800");
		add(L"f2", L"atan2(?f1 ,?f0 )/(pi/180)");
		add(L"f3", L"10800*cos(?f2 *(pi/180))");
		add(L"f4", L"10800*sin(?f2 *(pi/180))");
		add(L"f5", L"?f3 +10800");
		add(L"f6", L"?f4 +10800");
		add(L"f7", L"$0 -?f5 ");
		add(L"f8", L"$1 -?f6 ");
		add(L"f9", L"?f7 /3");
		add(L"f10", L"?f8 /3");
		add(L"f11", L"?f7 *2/3");
		add(L"f12", L"?f8 *2/3");
		add(L"f13", L"$0 ");
		add(L"f14", L"$1 ");
		add(L"f15", L"?f3 /12");
		add(L"f16", L"?f4 /12");
		add(L"f17", L"?f9 +?f5 -?f15 ");
		add(L"f18", L"?f10 +?f6 -?f16 ");
		add(L"f19", L"?f11 +?f5 ");
		add(L"f20", L"?f12 +?f6 ");
/////////////////////////////////////////////////////////
		_handle h1;
		
		h1.position = L"$0 $1";
		handles.push_back(h1);
	}
};
class oox_shape_WedgeRectCallout : public oox_shape
{
public:
	oox_shape_WedgeRectCallout()
	{
		odf_type_name	=L"ooxml-wedgeRectCallout";

		enhanced_path	= L"M 0 0 L 0 3590 ?f2 ?f3 0 8970 0 12630 ?f4 ?f5 0 18010 0 21600 3590 21600 ?f6 ?f7 8970 21600 12630 21600 ?f8 ?f9 18010 21600 21600 21600 21600 18010 ?f10 ?f11 21600 12630 21600 8970 ?f12 ?f13 21600 3590 21600 0 18010 0 ?f14 ?f15 12630 0 8970 0 ?f16 ?f17 3590 0 0 0 Z N";
		text_areas		= L"0 0 21600 21600";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"6300 24300";
		glue_points		= L"?f40 ?f41";
		
		add(L"f0", L"$0 -10800");
		add(L"f1", L"$1 -10800");
		add(L"f2", L"if(?f18 ,$0 ,0)");
		add(L"f3", L"if(?f18 ,$1 ,6280)");
		add(L"f4", L"if(?f23 ,$0 ,0)");
		add(L"f5", L"if(?f23 ,$1 ,15320)");
		add(L"f6", L"if(?f26 ,$0 ,6280)");
		add(L"f7", L"if(?f26 ,$1 ,21600)");
		add(L"f8", L"if(?f29 ,$0 ,15320)");
		add(L"f9", L"if(?f29 ,$1 ,21600)");
		add(L"f10", L"if(?f32 ,$0 ,21600)");
		add(L"f11", L"if(?f32 ,$1 ,15320)");
		add(L"f12", L"if(?f34 ,$0 ,21600)");
		add(L"f13", L"if(?f34 ,$1 ,6280)");
		add(L"f14", L"if(?f36 ,$0 ,15320)");
		add(L"f15", L"if(?f36 ,$1 ,0)");
		add(L"f16", L"if(?f38 ,$0 ,6280)");
		add(L"f17", L"if(?f38 ,$1 ,0)");
		add(L"f18", L"if($0 ,-1,?f19 )");
		add(L"f19", L"if(?f1 ,-1,?f22 )");
		add(L"f20", L"abs(?f0 )");
		add(L"f21", L"abs(?f1 )");
		add(L"f22", L"?f20 -?f21 ");
		add(L"f23", L"if($0 ,-1,?f24 )");
		add(L"f24", L"if(?f1 ,?f22 ,-1)");
		add(L"f25", L"$1 -21600");
		add(L"f26", L"if(?f25 ,?f27 ,-1)");
		add(L"f27", L"if(?f0 ,-1,?f28 )");
		add(L"f28", L"?f21 -?f20 ");
		add(L"f29", L"if(?f25 ,?f30 ,-1)");
		add(L"f30", L"if(?f0 ,?f28 ,-1)");
		add(L"f31", L"$0 -21600");
		add(L"f32", L"if(?f31 ,?f33 ,-1)");
		add(L"f33", L"if(?f1 ,?f22 ,-1)");
		add(L"f34", L"if(?f31 ,?f35 ,-1)");
		add(L"f35", L"if(?f1 ,-1,?f22 )");
		add(L"f36", L"if($1 ,-1,?f37 )");
		add(L"f37", L"if(?f0 ,?f28 ,-1)");
		add(L"f38", L"if($1 ,-1,?f39 )");
		add(L"f39", L"if(?f0 ,-1,?f28 )");
		add(L"f40", L"$0 ");
		add(L"f41", L"$1 ");
/////////////////////////////////////////////////////////
		_handle h1;
		
		h1.position = L"$0 $1";
		handles.push_back(h1);
	}
};

class oox_shape_WedgeRoundRectCallout : public oox_shape
{
public:
	oox_shape_WedgeRoundRectCallout()
	{
		odf_type_name	=L"ooxml-wedgeRoundRectCallout";

		enhanced_path	= L"M 3590 0 X 0 3590 L ?f2 ?f3 0 8970 0 12630 ?f4 ?f5 0 18010 Y 3590 21600 L ?f6 ?f7 8970 21600 12630 21600 ?f8 ?f9 18010 21600 X 21600 18010 L ?f10 ?f11 21600 12630 21600 8970 ?f12 ?f13 21600 3590 Y 18010 0 L ?f14 ?f15 12630 0 8970 0 ?f16 ?f17 Z N";
		text_areas		= L"800 800 20800 20800";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"6300 24300";
		glue_points		= L"?f40 ?f41";
		
		add(L"f0", L"$0 -10800");
		add(L"f1", L"$1 -10800");
		add(L"f2", L"if(?f18 ,$0 ,0)");
		add(L"f3", L"if(?f18 ,$1 ,6280)");
		add(L"f4", L"if(?f23 ,$0 ,0)");
		add(L"f5", L"if(?f23 ,$1 ,15320)");
		add(L"f6", L"if(?f26 ,$0 ,6280)");
		add(L"f7", L"if(?f26 ,$1 ,21600)");
		add(L"f8", L"if(?f29 ,$0 ,15320)");
		add(L"f9", L"if(?f29 ,$1 ,21600)");
		add(L"f10", L"if(?f32 ,$0 ,21600)");
		add(L"f11", L"if(?f32 ,$1 ,15320)");
		add(L"f12", L"if(?f34 ,$0 ,21600)");
		add(L"f13", L"if(?f34 ,$1 ,6280)");
		add(L"f14", L"if(?f36 ,$0 ,15320)");
		add(L"f15", L"if(?f36 ,$1 ,0)");
		add(L"f16", L"if(?f38 ,$0 ,6280)");
		add(L"f17", L"if(?f38 ,$1 ,0)");
		add(L"f18", L"if($0 ,-1,?f19 )");
		add(L"f19", L"if(?f1 ,-1,?f22 )");
		add(L"f20", L"abs(?f0 )");
		add(L"f21", L"abs(?f1 )");
		add(L"f22", L"?f20 -?f21 ");
		add(L"f23", L"if($0 ,-1,?f24 )");
		add(L"f24", L"if(?f1 ,?f22 ,-1)");
		add(L"f25", L"$1 -21600");
		add(L"f26", L"if(?f25 ,?f27 ,-1)");
		add(L"f27", L"if(?f0 ,-1,?f28 )");
		add(L"f28", L"?f21 -?f20 ");
		add(L"f29", L"if(?f25 ,?f30 ,-1)");
		add(L"f30", L"if(?f0 ,?f28 ,-1)");
		add(L"f31", L"$0 -21600");
		add(L"f32", L"if(?f31 ,?f33 ,-1)");
		add(L"f33", L"if(?f1 ,?f22 ,-1)");
		add(L"f34", L"if(?f31 ,?f35 ,-1)");
		add(L"f35", L"if(?f1 ,-1,?f22 )");
		add(L"f36", L"if($1 ,-1,?f37 )");
		add(L"f37", L"if(?f0 ,?f28 ,-1)");
		add(L"f38", L"if($1 ,-1,?f39 )");
		add(L"f39", L"if(?f0 ,-1,?f28 )");
		add(L"f40", L"$0 ");
		add(L"f41", L"$1 ");
/////////////////////////////////////////////////////////
		_handle h1;
		
		h1.position = L"$0 $1";
		handles.push_back(h1);
	}
};
class oox_shape_WedgeEllipseCallout : public oox_shape
{
public:
	oox_shape_WedgeEllipseCallout()
	{
		odf_type_name	=L"ooxml-wedgeEllipseCallout";

		enhanced_path	= L"W 0 0 21600 21600 ?f22 ?f23 ?f18 ?f19 L ?f14 ?f15 Z N";
		text_areas		= L"3200 3200 18400 18400";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"6300 24300";
		glue_points		= L"3160 3160 3160 18440 18440 18440 18440 3160 ?f14 ?f15";
		
		add(L"f0", L"$0 -10800");
		add(L"f1", L"$1 -10800");
		add(L"f2", L"?f0 *?f0 ");
		add(L"f3", L"?f1 *?f1 ");
		add(L"f4", L"?f2 +?f3 ");
		add(L"f5", L"sqrt(?f4 )");
		add(L"f6", L"?f5 -10800");
		add(L"f7", L"atan2(?f1 ,?f0 )/(pi/180)");
		add(L"f8", L"?f7 -10");
		add(L"f9", L"?f7 +10");
		add(L"f10", L"10800*cos(?f7 *(pi/180))");
		add(L"f11", L"10800*sin(?f7 *(pi/180))");
		add(L"f12", L"?f10 +10800");
		add(L"f13", L"?f11 +10800");
		add(L"f14", L"if(?f6 ,$0 ,?f12 )");
		add(L"f15", L"if(?f6 ,$1 ,?f13 )");
		add(L"f16", L"10800*cos(?f8 *(pi/180))");
		add(L"f17", L"10800*sin(?f8 *(pi/180))");
		add(L"f18", L"?f16 +10800");
		add(L"f19", L"?f17 +10800");
		add(L"f20", L"10800*cos(?f9 *(pi/180))");
		add(L"f21", L"10800*sin(?f9 *(pi/180))");
		add(L"f22", L"?f20 +10800");
		add(L"f23", L"?f21 +10800");
/////////////////////////////////////////////////////////
		_handle h1;
		
		h1.position = L"$0 $1";
		handles.push_back(h1);
	}
};
class oox_shape_QuadArrowCallout : public oox_shape
{
public:
	oox_shape_QuadArrowCallout()
	{
		odf_type_name	=L"ooxml-quadArrowCallout";

		enhanced_path	= L"M ?f0 ?f6 L ?f17 ?f30 ?f17 ?f32 ?f21 ?f32 ?f21 ?f28 ?f25 ?f28 ?f25 ?f17 ?f23 ?f17 ?f9 ?f2 ?f24 ?f17 ?f26 ?f17 ?f26 ?f28 ?f22 ?f28 ?f22 ?f32 ?f20 ?f32 ?f20 ?f30 ?f1 ?f6 ?f20 ?f31 ?f20 ?f33 ?f22 ?f33 ?f22 ?f29 ?f26 ?f29 ?f26 ?f27 ?f24 ?f27 ?f9 ?f3 ?f23 ?f27 ?f25 ?f27 ?f25 ?f29 ?f21 ?f29 ?f21 ?f33 ?f17 ?f33 ?f17 ?f31 Z N";
		text_areas		= L"?f21 ?f28 ?f22 ?f29";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18515 18515 18515 48123";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f4 / 2");
		add(L"f6", L"?f2 + ?f5");
		add(L"f7", L"?f1 - ?f0");
		add(L"f8", L"?f7 / 2");
		add(L"f9", L"?f0 + ?f8");
		add(L"f10", L"min(?f7, ?f4)");
		add(L"f11", L"$1");
		add(L"f12", L"$0");
		add(L"f13", L"$2");
		add(L"f14", L"$3");
		add(L"f15", L"?f10 * ?f11 / 100000");
		add(L"f16", L"?f10 * ?f12 / 200000");
		add(L"f17", L"?f10 * ?f13 / 100000");
		add(L"f18", L"?f7 * ?f14 / 200000");
		add(L"f19", L"?f4 * ?f14 / 200000");
		add(L"f20", L"?f1 - ?f17");
		add(L"f21", L"?f9 - ?f18");
		add(L"f22", L"?f9 + ?f18");
		add(L"f23", L"?f9 - ?f15");
		add(L"f24", L"?f9 + ?f15");
		add(L"f25", L"?f9 - ?f16");
		add(L"f26", L"?f9 + ?f16");
		add(L"f27", L"?f3 - ?f17");
		add(L"f28", L"?f6 - ?f19");
		add(L"f29", L"?f6 + ?f19");
		add(L"f30", L"?f6 - ?f15");
		add(L"f31", L"?f6 + ?f15");
		add(L"f32", L"?f6 - ?f16");
		add(L"f33", L"?f6 + ?f16");
/////////////////////////////////////////////////////////
	}
};
}