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

namespace cpdoccore 
{
class oox_shape_snip1Rect : public oox_shape
{
public:
	oox_shape_snip1Rect()
	{
		odf_type_name	=L"ooxml-snip1Rect";

		modifiers		= L"16667";
		enhanced_path	= L"M ?f0 ?f2 L ?f9 ?f2 ?f1 ?f8 ?f1 ?f3 ?f0 ?f3 Z N";
		text_areas		= L"?f0 ?f10 ?f11 ?f3";
		view_box		= L"0 0 21600 21600";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"$0");
		add(L"f8", L"?f6 * ?f7 / 100000");
		add(L"f9", L"?f1 - ?f8");
		add(L"f10", L"?f8 / 2");
		add(L"f11", L"(?f9 + ?f1) / 2");
	}
};
class oox_shape_snip2DiagRect : public oox_shape
{
public:
	oox_shape_snip2DiagRect()
	{
		odf_type_name	=L"ooxml-snip2DiagRect";

		modifiers		= L"0 16667";
		enhanced_path	= L"M ?f3 0 L ?f7 0 ?f14 ?f6 ?f14 ?f5 ?f4 ?f17 ?f6 ?f17 0 ?f8 0 ?f3 Z N";
		text_areas		= L"?f11 ?f11 ?f12 ?f13";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(0-$0 ,0,if(50000-$0 ,$0 ,50000))");
		add(L"f1", L"if(0-$1 ,0,if(50000-$1 ,$1 ,50000))");
		add(L"f2", L"min(logwidth,logheight)");
		add(L"f3", L"?f2 *?f0 /100000");
		add(L"f4", L"logwidth+0-?f3 ");
		add(L"f5", L"logheight+0-?f3 ");
		add(L"f6", L"?f2 *?f1 /100000");
		add(L"f7", L"logwidth+0-?f6 ");
		add(L"f8", L"logheight+0-?f6 ");
		add(L"f9", L"?f3 +0-?f6 ");
		add(L"f10", L"if(?f9 ,?f3 ,?f6 )");
		add(L"f11", L"?f10 *1/2");
		add(L"f12", L"logwidth+0-?f11 ");
		add(L"f13", L"logheight+0-?f11 ");
		add(L"f14", L"logwidth");
		add(L"f15", L"logheight/2");
		add(L"f16", L"logwidth/2");
		add(L"f17", L"logheight");

		_handle h;

		h.position = L"?f3 0";
		h.x_maximum= L"50000";
		h.x_minimum= L"0";
		handles.push_back(h);

		h.position = L"?f7 0";
		h.x_maximum= L"50000";
		h.x_minimum= L"0";
		handles.push_back(h);

	}
};
class oox_shape_snip2SameRect : public oox_shape
{
public:
	oox_shape_snip2SameRect()
	{
		odf_type_name	=L"ooxml-snip2SameRect";

		modifiers		= L"16667 0";
		enhanced_path	= L"M ?f9 ?f2 L ?f10 ?f2 ?f1 ?f9 ?f1 ?f13 ?f12 ?f3 ?f11 ?f3 ?f0 ?f13 ?f0 ?f9 Z N";
		text_areas		= L"?f16 ?f18 ?f17 ?f19";
		view_box		= L"0 0 21600 21600";
		
        add(L"f0", L"left");
        add(L"f1", L"right");
        add(L"f2", L"top");
        add(L"f3", L"bottom");
        add(L"f4", L"?f3 - ?f2");
        add(L"f5", L"?f1 - ?f0");
        add(L"f6", L"min(?f5, ?f4)");
        add(L"f7", L"$0");
        add(L"f8", L"$1");
        add(L"f9", L"?f6 * ?f7 / 100000");
        add(L"f10", L"?f1 - ?f9");
        add(L"f11", L"?f6 * ?f8 / 100000");
        add(L"f12", L"?f1 - ?f11");
        add(L"f13", L"?f3 - ?f11");
        add(L"f14", L"?f9 - ?f11");
        add(L"f15", L"if(?f14, ?f9, ?f11)");
        add(L"f16", L"?f15 / 2");
        add(L"f17", L"?f1 - ?f16");
        add(L"f18", L"?f9 / 2");
        add(L"f19", L"(?f13 + ?f3) / 2");
	}
};
class oox_shape_snipRoundRect : public oox_shape
{
public:
	oox_shape_snipRoundRect()
	{
		odf_type_name	=L"ooxml-snipRoundRect";

		modifiers		= L"16667 16667";
		enhanced_path	= L"M ?f12 ?f4 L ?f14 ?f4 ?f3 ?f13 ?f3 ?f5 ?f2 ?f5 ?f2 ?f12 A ?f56 ?f57 ?f58 ?f59 ?f2 ?f12 ?f53 ?f55  W ?f60 ?f61 ?f62 ?f63 ?f2 ?f12 ?f53 ?f55 Z N";
		text_areas		= L"?f15 ?f15 ?f16 ?f5";
		view_box		= L"0 0 21600 21600";
		
        add(L"f0", L"10800000");
        add(L"f1", L"5400000");
        add(L"f2", L"left");
        add(L"f3", L"right");
        add(L"f4", L"top");
        add(L"f5", L"bottom");
        add(L"f6", L"?f5 - ?f4");
        add(L"f7", L"?f3 - ?f2");
        add(L"f8", L"min(?f7, ?f6)");
        add(L"f9", L"5419351 / 1725033");
        add(L"f10", L"$0");
        add(L"f11", L"$1");
        add(L"f12", L"?f8 * ?f10 / 100000");
        add(L"f13", L"?f8 * ?f11 / 100000");
        add(L"f14", L"?f3 - ?f13");
        add(L"f15", L"?f12 * 29289 / 100000");
        add(L"f16", L"(?f14 + ?f3) / 2");
        add(L"f17", L"21550000 - ?f1");
        add(L"f18", L"if(?f17, ?f1, 21550000)");
        add(L"f19", L"-21550000 - ?f18");
        add(L"f20", L"if(?f19, -21550000, ?f18)");
        add(L"f21", L"?f0 + ?f20");
        add(L"f22", L"?f0 + ?f1");
        add(L"f23", L"?f22 * ?f9 / ?f0");
        add(L"f24", L"0 - ?f23");
        add(L"f25", L"cos(?f24)");
        add(L"f26", L"0 - ?f25");
        add(L"f27", L"?f26 * ?f12");
        add(L"f28", L"sin(?f24)");
        add(L"f29", L"0 - ?f28");
        add(L"f30", L"?f29 * ?f12");
        add(L"f31", L"sqrt(?f27 * ?f27 + ?f30 * ?f30 + 0 * 0)");
        add(L"f32", L"?f12 * ?f12 / ?f31");
        add(L"f33", L"?f29 * ?f32");
        add(L"f34", L"?f2 - ?f33");
        add(L"f35", L"?f26 * ?f32");
        add(L"f36", L"?f12 - ?f35");
        add(L"f37", L"?f34 - ?f12");
        add(L"f38", L"?f36 - ?f12");
        add(L"f39", L"?f34 + ?f12");
        add(L"f40", L"?f36 + ?f12");
        add(L"f41", L"?f21 + ?f1");
        add(L"f42", L"?f41 * ?f9 / ?f0");
        add(L"f43", L"0 - ?f42");
        add(L"f44", L"cos(?f43)");
        add(L"f45", L"0 - ?f44");
        add(L"f46", L"?f45 * ?f12");
        add(L"f47", L"sin(?f43)");
        add(L"f48", L"0 - ?f47");
        add(L"f49", L"?f48 * ?f12");
        add(L"f50", L"sqrt(?f46 * ?f46 + ?f49 * ?f49 + 0 * 0)");
        add(L"f51", L"?f12 * ?f12 / ?f50");
        add(L"f52", L"?f48 * ?f51");
        add(L"f53", L"?f34 + ?f52");
        add(L"f54", L"?f45 * ?f51");
        add(L"f55", L"?f36 + ?f54");
        add(L"f56", L"if(?f20, ?f2, ?f37)");
        add(L"f57", L"if(?f20, ?f12, ?f38)");
        add(L"f58", L"if(?f20, ?f2, ?f39)");
        add(L"f59", L"if(?f20, ?f12, ?f40)");
        add(L"f60", L"if(?f20, ?f37, ?f53)");
        add(L"f61", L"if(?f20, ?f38, ?f55)");
        add(L"f62", L"if(?f20, ?f39, ?f53)");
        add(L"f63", L"if(?f20, ?f40, ?f55)");
	}
};
class oox_shape_round1Rect : public oox_shape
{
public:
	oox_shape_round1Rect()
	{
		odf_type_name	=L"ooxml-round1Rect";
		
		modifiers		= L"16667";
		enhanced_path	= L"M ?f3 ?f5 L ?f13 ?f5 A ?f55 ?f56 ?f57 ?f58 ?f13 ?f5 ?f52 ?f54  W ?f59 ?f60 ?f61 ?f62 ?f13 ?f5 ?f52 ?f54 L ?f4 ?f6 ?f3 ?f6 Z N";
		text_areas		= L"?f3 ?f5 ?f15 ?f6";
		view_box		= L"0 0 21600 21600";
		
        add(L"f0", L"16200000");
        add(L"f1", L"10800000");
        add(L"f2", L"5400000");
        add(L"f3", L"left");
        add(L"f4", L"right");
        add(L"f5", L"top");
        add(L"f6", L"bottom");
        add(L"f7", L"?f6 - ?f5");
        add(L"f8", L"?f4 - ?f3");
        add(L"f9", L"min(?f8, ?f7)");
        add(L"f10", L"5419351 / 1725033");
        add(L"f11", L"$0");
        add(L"f12", L"?f9 * ?f11 / 100000");
        add(L"f13", L"?f4 - ?f12");
        add(L"f14", L"?f12 * 29289 / 100000");
        add(L"f15", L"?f4 - ?f14");
        add(L"f16", L"21550000 - ?f2");
        add(L"f17", L"if(?f16, ?f2, 21550000)");
        add(L"f18", L"-21550000 - ?f17");
        add(L"f19", L"if(?f18, -21550000, ?f17)");
        add(L"f20", L"?f0 + ?f19");
        add(L"f21", L"?f0 + ?f2");
        add(L"f22", L"?f21 * ?f10 / ?f1");
        add(L"f23", L"0 - ?f22");
        add(L"f24", L"cos(?f23)");
        add(L"f25", L"0 - ?f24");
        add(L"f26", L"?f25 * ?f12");
        add(L"f27", L"sin(?f23)");
        add(L"f28", L"0 - ?f27");
        add(L"f29", L"?f28 * ?f12");
        add(L"f30", L"sqrt(?f26 * ?f26 + ?f29 * ?f29 + 0 * 0)");
        add(L"f31", L"?f12 * ?f12 / ?f30");
        add(L"f32", L"?f28 * ?f31");
        add(L"f33", L"?f13 - ?f32");
        add(L"f34", L"?f25 * ?f31");
        add(L"f35", L"?f5 - ?f34");
        add(L"f36", L"?f33 - ?f12");
        add(L"f37", L"?f35 - ?f12");
        add(L"f38", L"?f33 + ?f12");
        add(L"f39", L"?f35 + ?f12");
        add(L"f40", L"?f20 + ?f2");
        add(L"f41", L"?f40 * ?f10 / ?f1");
        add(L"f42", L"0 - ?f41");
        add(L"f43", L"cos(?f42)");
        add(L"f44", L"0 - ?f43");
        add(L"f45", L"?f44 * ?f12");
        add(L"f46", L"sin(?f42)");
        add(L"f47", L"0 - ?f46");
        add(L"f48", L"?f47 * ?f12");
        add(L"f49", L"sqrt(?f45 * ?f45 + ?f48 * ?f48 + 0 * 0)");
        add(L"f50", L"?f12 * ?f12 / ?f49");
        add(L"f51", L"?f47 * ?f50");
        add(L"f52", L"?f33 + ?f51");
        add(L"f53", L"?f44 * ?f50");
        add(L"f54", L"?f35 + ?f53");
        add(L"f55", L"if(?f19, ?f13, ?f36)");
        add(L"f56", L"if(?f19, ?f5, ?f37)");
        add(L"f57", L"if(?f19, ?f13, ?f38)");
        add(L"f58", L"if(?f19, ?f5, ?f39)");
        add(L"f59", L"if(?f19, ?f36, ?f52)");
        add(L"f60", L"if(?f19, ?f37, ?f54)");
        add(L"f61", L"if(?f19, ?f38, ?f52)");
        add(L"f62", L"if(?f19, ?f39, ?f54)");
	}
};
class oox_shape_round2DiagRect : public oox_shape
{
public:
	oox_shape_round2DiagRect()
	{
		odf_type_name	=L"ooxml-round2DiagRect";

		modifiers		= L"16667 0";
		enhanced_path	= L"M ?f13 ?f5 L ?f16 ?f5 A ?f62 ?f63 ?f64 ?f65 ?f16 ?f5 ?f59 ?f61  W ?f66 ?f67 ?f68 ?f69 ?f16 ?f5 ?f59 ?f61 L ?f4 ?f14 A ?f105 ?f106 ?f107 ?f108 ?f4 ?f14 ?f102 ?f104  W ?f109 ?f110 ?f111 ?f112 ?f4 ?f14 ?f102 ?f104 L ?f15 ?f6 A ?f148 ?f149 ?f150 ?f151 ?f15 ?f6 ?f145 ?f147  W ?f152 ?f153 ?f154 ?f155 ?f15 ?f6 ?f145 ?f147 L ?f3 ?f13 A ?f191 ?f192 ?f193 ?f194 ?f3 ?f13 ?f188 ?f190  W ?f195 ?f196 ?f197 ?f198 ?f3 ?f13 ?f188 ?f190 Z N";
		text_areas		= L"?f20 ?f20 ?f21 ?f22";
		view_box		= L"0 0 21600 21600";
		
         add(L"f0", L"16200000");
         add(L"f1", L"10800000");
         add(L"f2", L"5400000");
         add(L"f3", L"left");
         add(L"f4", L"right");
         add(L"f5", L"top");
         add(L"f6", L"bottom");
         add(L"f7", L"?f6 - ?f5");
         add(L"f8", L"?f4 - ?f3");
         add(L"f9", L"min(?f8, ?f7)");
         add(L"f10", L"5419351 / 1725033");
         add(L"f11", L"$0");
         add(L"f12", L"$1");
         add(L"f13", L"?f9 * ?f11 / 100000");
         add(L"f14", L"?f6 - ?f13");
         add(L"f15", L"?f9 * ?f12 / 100000");
         add(L"f16", L"?f4 - ?f15");
         add(L"f17", L"?f13 * 29289 / 100000");
         add(L"f18", L"?f15 * 29289 / 100000");
         add(L"f19", L"?f17 - ?f18");
         add(L"f20", L"if(?f19, ?f17, ?f18)");
         add(L"f21", L"?f4 - ?f20");
         add(L"f22", L"?f6 - ?f20");
         add(L"f23", L"21550000 - ?f2");
         add(L"f24", L"if(?f23, ?f2, 21550000)");
         add(L"f25", L"-21550000 - ?f24");
         add(L"f26", L"if(?f25, -21550000, ?f24)");
         add(L"f27", L"?f0 + ?f26");
         add(L"f28", L"?f0 + ?f2");
         add(L"f29", L"?f28 * ?f10 / ?f1");
         add(L"f30", L"0 - ?f29");
         add(L"f31", L"cos(?f30)");
         add(L"f32", L"0 - ?f31");
         add(L"f33", L"?f32 * ?f15");
         add(L"f34", L"sin(?f30)");
         add(L"f35", L"0 - ?f34");
         add(L"f36", L"?f35 * ?f15");
         add(L"f37", L"sqrt(?f33 * ?f33 + ?f36 * ?f36 + 0 * 0)");
         add(L"f38", L"?f15 * ?f15 / ?f37");
         add(L"f39", L"?f35 * ?f38");
         add(L"f40", L"?f16 - ?f39");
         add(L"f41", L"?f32 * ?f38");
         add(L"f42", L"?f5 - ?f41");
         add(L"f43", L"?f40 - ?f15");
         add(L"f44", L"?f42 - ?f15");
         add(L"f45", L"?f40 + ?f15");
         add(L"f46", L"?f42 + ?f15");
         add(L"f47", L"?f27 + ?f2");
         add(L"f48", L"?f47 * ?f10 / ?f1");
         add(L"f49", L"0 - ?f48");
         add(L"f50", L"cos(?f49)");
         add(L"f51", L"0 - ?f50");
         add(L"f52", L"?f51 * ?f15");
         add(L"f53", L"sin(?f49)");
         add(L"f54", L"0 - ?f53");
         add(L"f55", L"?f54 * ?f15");
         add(L"f56", L"sqrt(?f52 * ?f52 + ?f55 * ?f55 + 0 * 0)");
         add(L"f57", L"?f15 * ?f15 / ?f56");
         add(L"f58", L"?f54 * ?f57");
         add(L"f59", L"?f40 + ?f58");
         add(L"f60", L"?f51 * ?f57");
         add(L"f61", L"?f42 + ?f60");
         add(L"f62", L"if(?f26, ?f16, ?f43)");
         add(L"f63", L"if(?f26, ?f5, ?f44)");
         add(L"f64", L"if(?f26, ?f16, ?f45)");
         add(L"f65", L"if(?f26, ?f5, ?f46)");
         add(L"f66", L"if(?f26, ?f43, ?f59)");
         add(L"f67", L"if(?f26, ?f44, ?f61)");
         add(L"f68", L"if(?f26, ?f45, ?f59)");
         add(L"f69", L"if(?f26, ?f46, ?f61)");
         add(L"f70", L"0 + ?f26");
         add(L"f71", L"0 + ?f2");
         add(L"f72", L"?f71 * ?f10 / ?f1");
         add(L"f73", L"0 - ?f72");
         add(L"f74", L"cos(?f73)");
         add(L"f75", L"0 - ?f74");
         add(L"f76", L"?f75 * ?f13");
         add(L"f77", L"sin(?f73)");
         add(L"f78", L"0 - ?f77");
         add(L"f79", L"?f78 * ?f13");
         add(L"f80", L"sqrt(?f76 * ?f76 + ?f79 * ?f79 + 0 * 0)");
         add(L"f81", L"?f13 * ?f13 / ?f80");
         add(L"f82", L"?f78 * ?f81");
         add(L"f83", L"?f4 - ?f82");
         add(L"f84", L"?f75 * ?f81");
         add(L"f85", L"?f14 - ?f84");
         add(L"f86", L"?f83 - ?f13");
         add(L"f87", L"?f85 - ?f13");
         add(L"f88", L"?f83 + ?f13");
         add(L"f89", L"?f85 + ?f13");
         add(L"f90", L"?f70 + ?f2");
         add(L"f91", L"?f90 * ?f10 / ?f1");
         add(L"f92", L"0 - ?f91");
         add(L"f93", L"cos(?f92)");
         add(L"f94", L"0 - ?f93");
         add(L"f95", L"?f94 * ?f13");
         add(L"f96", L"sin(?f92)");
         add(L"f97", L"0 - ?f96");
         add(L"f98", L"?f97 * ?f13");
         add(L"f99", L"sqrt(?f95 * ?f95 + ?f98 * ?f98 + 0 * 0)");
         add(L"f100", L"?f13 * ?f13 / ?f99");
         add(L"f101", L"?f97 * ?f100");
         add(L"f102", L"?f83 + ?f101");
         add(L"f103", L"?f94 * ?f100");
         add(L"f104", L"?f85 + ?f103");
         add(L"f105", L"if(?f26, ?f4, ?f86)");
         add(L"f106", L"if(?f26, ?f14, ?f87)");
         add(L"f107", L"if(?f26, ?f4, ?f88)");
         add(L"f108", L"if(?f26, ?f14, ?f89)");
         add(L"f109", L"if(?f26, ?f86, ?f102)");
         add(L"f110", L"if(?f26, ?f87, ?f104)");
         add(L"f111", L"if(?f26, ?f88, ?f102)");
         add(L"f112", L"if(?f26, ?f89, ?f104)");
         add(L"f113", L"?f2 + ?f26");
         add(L"f114", L"?f2 + ?f2");
         add(L"f115", L"?f114 * ?f10 / ?f1");
         add(L"f116", L"0 - ?f115");
         add(L"f117", L"cos(?f116)");
         add(L"f118", L"0 - ?f117");
         add(L"f119", L"?f118 * ?f15");
         add(L"f120", L"sin(?f116)");
         add(L"f121", L"0 - ?f120");
         add(L"f122", L"?f121 * ?f15");
         add(L"f123", L"sqrt(?f119 * ?f119 + ?f122 * ?f122 + 0 * 0)");
         add(L"f124", L"?f15 * ?f15 / ?f123");
         add(L"f125", L"?f121 * ?f124");
         add(L"f126", L"?f15 - ?f125");
         add(L"f127", L"?f118 * ?f124");
         add(L"f128", L"?f6 - ?f127");
         add(L"f129", L"?f126 - ?f15");
         add(L"f130", L"?f128 - ?f15");
         add(L"f131", L"?f126 + ?f15");
         add(L"f132", L"?f128 + ?f15");
         add(L"f133", L"?f113 + ?f2");
         add(L"f134", L"?f133 * ?f10 / ?f1");
         add(L"f135", L"0 - ?f134");
         add(L"f136", L"cos(?f135)");
         add(L"f137", L"0 - ?f136");
         add(L"f138", L"?f137 * ?f15");
         add(L"f139", L"sin(?f135)");
         add(L"f140", L"0 - ?f139");
         add(L"f141", L"?f140 * ?f15");
         add(L"f142", L"sqrt(?f138 * ?f138 + ?f141 * ?f141 + 0 * 0)");
         add(L"f143", L"?f15 * ?f15 / ?f142");
         add(L"f144", L"?f140 * ?f143");
         add(L"f145", L"?f126 + ?f144");
         add(L"f146", L"?f137 * ?f143");
         add(L"f147", L"?f128 + ?f146");
         add(L"f148", L"if(?f26, ?f15, ?f129)");
         add(L"f149", L"if(?f26, ?f6, ?f130)");
         add(L"f150", L"if(?f26, ?f15, ?f131)");
         add(L"f151", L"if(?f26, ?f6, ?f132)");
         add(L"f152", L"if(?f26, ?f129, ?f145)");
         add(L"f153", L"if(?f26, ?f130, ?f147)");
         add(L"f154", L"if(?f26, ?f131, ?f145)");
         add(L"f155", L"if(?f26, ?f132, ?f147)");
         add(L"f156", L"?f1 + ?f26");
         add(L"f157", L"?f1 + ?f2");
         add(L"f158", L"?f157 * ?f10 / ?f1");
         add(L"f159", L"0 - ?f158");
         add(L"f160", L"cos(?f159)");
         add(L"f161", L"0 - ?f160");
         add(L"f162", L"?f161 * ?f13");
         add(L"f163", L"sin(?f159)");
         add(L"f164", L"0 - ?f163");
         add(L"f165", L"?f164 * ?f13");
         add(L"f166", L"sqrt(?f162 * ?f162 + ?f165 * ?f165 + 0 * 0)");
         add(L"f167", L"?f13 * ?f13 / ?f166");
         add(L"f168", L"?f164 * ?f167");
         add(L"f169", L"?f3 - ?f168");
         add(L"f170", L"?f161 * ?f167");
         add(L"f171", L"?f13 - ?f170");
         add(L"f172", L"?f169 - ?f13");
         add(L"f173", L"?f171 - ?f13");
         add(L"f174", L"?f169 + ?f13");
         add(L"f175", L"?f171 + ?f13");
         add(L"f176", L"?f156 + ?f2");
         add(L"f177", L"?f176 * ?f10 / ?f1");
         add(L"f178", L"0 - ?f177");
         add(L"f179", L"cos(?f178)");
         add(L"f180", L"0 - ?f179");
         add(L"f181", L"?f180 * ?f13");
         add(L"f182", L"sin(?f178)");
         add(L"f183", L"0 - ?f182");
         add(L"f184", L"?f183 * ?f13");
         add(L"f185", L"sqrt(?f181 * ?f181 + ?f184 * ?f184 + 0 * 0)");
         add(L"f186", L"?f13 * ?f13 / ?f185");
         add(L"f187", L"?f183 * ?f186");
         add(L"f188", L"?f169 + ?f187");
         add(L"f189", L"?f180 * ?f186");
         add(L"f190", L"?f171 + ?f189");
         add(L"f191", L"if(?f26, ?f3, ?f172)");
         add(L"f192", L"if(?f26, ?f13, ?f173)");
         add(L"f193", L"if(?f26, ?f3, ?f174)");
         add(L"f194", L"if(?f26, ?f13, ?f175)");
         add(L"f195", L"if(?f26, ?f172, ?f188)");
         add(L"f196", L"if(?f26, ?f173, ?f190)");
         add(L"f197", L"if(?f26, ?f174, ?f188)");
         add(L"f198", L"if(?f26, ?f175, ?f190)");
	}
};
class oox_shape_round2SameRect : public oox_shape
{
public:
	oox_shape_round2SameRect()
	{
		odf_type_name	=L"ooxml-round2SameRect";

		modifiers		= L"16667 0";
		enhanced_path	= L"M ?f13 ?f5 L ?f14 ?f5 A ?f62 ?f63 ?f64 ?f65 ?f14 ?f5 ?f59 ?f61  W ?f66 ?f67 ?f68 ?f69 ?f14 ?f5 ?f59 ?f61 L ?f4 ?f16 A ?f105 ?f106 ?f107 ?f108 ?f4 ?f16 ?f102 ?f104  W ?f109 ?f110 ?f111 ?f112 ?f4 ?f16 ?f102 ?f104 L ?f15 ?f6 A ?f148 ?f149 ?f150 ?f151 ?f15 ?f6 ?f145 ?f147  W ?f152 ?f153 ?f154 ?f155 ?f15 ?f6 ?f145 ?f147 L ?f3 ?f13 A ?f191 ?f192 ?f193 ?f194 ?f3 ?f13 ?f188 ?f190  W ?f195 ?f196 ?f197 ?f198 ?f3 ?f13 ?f188 ?f190 Z N";
		text_areas		= L"?f20 ?f18 ?f21 ?f22";
		view_box		= L"0 0 21600 21600";
		
        add(L"f0", L"16200000");
        add(L"f1", L"10800000");
        add(L"f2", L"5400000");
        add(L"f3", L"left");
        add(L"f4", L"right");
        add(L"f5", L"top");
        add(L"f6", L"bottom");
        add(L"f7", L"?f6 - ?f5");
        add(L"f8", L"?f4 - ?f3");
        add(L"f9", L"min(?f8, ?f7)");
        add(L"f10", L"5419351 / 1725033");
        add(L"f11", L"$0");
        add(L"f12", L"$1");
        add(L"f13", L"?f9 * ?f11 / 100000");
        add(L"f14", L"?f4 - ?f13");
        add(L"f15", L"?f9 * ?f12 / 100000");
        add(L"f16", L"?f6 - ?f15");
        add(L"f17", L"?f13 - ?f15");
        add(L"f18", L"?f13 * 29289 / 100000");
        add(L"f19", L"?f15 * 29289 / 100000");
        add(L"f20", L"if(?f17, ?f18, ?f19)");
        add(L"f21", L"?f4 - ?f20");
        add(L"f22", L"?f6 - ?f19");
        add(L"f23", L"21550000 - ?f2");
        add(L"f24", L"if(?f23, ?f2, 21550000)");
        add(L"f25", L"-21550000 - ?f24");
        add(L"f26", L"if(?f25, -21550000, ?f24)");
        add(L"f27", L"?f0 + ?f26");
        add(L"f28", L"?f0 + ?f2");
        add(L"f29", L"?f28 * ?f10 / ?f1");
        add(L"f30", L"0 - ?f29");
        add(L"f31", L"cos(?f30)");
        add(L"f32", L"0 - ?f31");
        add(L"f33", L"?f32 * ?f13");
        add(L"f34", L"sin(?f30)");
        add(L"f35", L"0 - ?f34");
        add(L"f36", L"?f35 * ?f13");
        add(L"f37", L"sqrt(?f33 * ?f33 + ?f36 * ?f36 + 0 * 0)");
        add(L"f38", L"?f13 * ?f13 / ?f37");
        add(L"f39", L"?f35 * ?f38");
        add(L"f40", L"?f14 - ?f39");
        add(L"f41", L"?f32 * ?f38");
        add(L"f42", L"?f5 - ?f41");
        add(L"f43", L"?f40 - ?f13");
        add(L"f44", L"?f42 - ?f13");
        add(L"f45", L"?f40 + ?f13");
        add(L"f46", L"?f42 + ?f13");
        add(L"f47", L"?f27 + ?f2");
        add(L"f48", L"?f47 * ?f10 / ?f1");
        add(L"f49", L"0 - ?f48");
        add(L"f50", L"cos(?f49)");
        add(L"f51", L"0 - ?f50");
        add(L"f52", L"?f51 * ?f13");
        add(L"f53", L"sin(?f49)");
        add(L"f54", L"0 - ?f53");
        add(L"f55", L"?f54 * ?f13");
        add(L"f56", L"sqrt(?f52 * ?f52 + ?f55 * ?f55 + 0 * 0)");
        add(L"f57", L"?f13 * ?f13 / ?f56");
        add(L"f58", L"?f54 * ?f57");
        add(L"f59", L"?f40 + ?f58");
        add(L"f60", L"?f51 * ?f57");
        add(L"f61", L"?f42 + ?f60");
        add(L"f62", L"if(?f26, ?f14, ?f43)");
        add(L"f63", L"if(?f26, ?f5, ?f44)");
        add(L"f64", L"if(?f26, ?f14, ?f45)");
        add(L"f65", L"if(?f26, ?f5, ?f46)");
        add(L"f66", L"if(?f26, ?f43, ?f59)");
        add(L"f67", L"if(?f26, ?f44, ?f61)");
        add(L"f68", L"if(?f26, ?f45, ?f59)");
        add(L"f69", L"if(?f26, ?f46, ?f61)");
        add(L"f70", L"0 + ?f26");
        add(L"f71", L"0 + ?f2");
        add(L"f72", L"?f71 * ?f10 / ?f1");
        add(L"f73", L"0 - ?f72");
        add(L"f74", L"cos(?f73)");
        add(L"f75", L"0 - ?f74");
        add(L"f76", L"?f75 * ?f15");
        add(L"f77", L"sin(?f73)");
        add(L"f78", L"0 - ?f77");
        add(L"f79", L"?f78 * ?f15");
        add(L"f80", L"sqrt(?f76 * ?f76 + ?f79 * ?f79 + 0 * 0)");
        add(L"f81", L"?f15 * ?f15 / ?f80");
        add(L"f82", L"?f78 * ?f81");
        add(L"f83", L"?f4 - ?f82");
        add(L"f84", L"?f75 * ?f81");
        add(L"f85", L"?f16 - ?f84");
        add(L"f86", L"?f83 - ?f15");
        add(L"f87", L"?f85 - ?f15");
        add(L"f88", L"?f83 + ?f15");
        add(L"f89", L"?f85 + ?f15");
        add(L"f90", L"?f70 + ?f2");
        add(L"f91", L"?f90 * ?f10 / ?f1");
        add(L"f92", L"0 - ?f91");
        add(L"f93", L"cos(?f92)");
        add(L"f94", L"0 - ?f93");
        add(L"f95", L"?f94 * ?f15");
        add(L"f96", L"sin(?f92)");
        add(L"f97", L"0 - ?f96");
        add(L"f98", L"?f97 * ?f15");
        add(L"f99", L"sqrt(?f95 * ?f95 + ?f98 * ?f98 + 0 * 0)");
        add(L"f100", L"?f15 * ?f15 / ?f99");
        add(L"f101", L"?f97 * ?f100");
        add(L"f102", L"?f83 + ?f101");
        add(L"f103", L"?f94 * ?f100");
        add(L"f104", L"?f85 + ?f103");
        add(L"f105", L"if(?f26, ?f4, ?f86)");
        add(L"f106", L"if(?f26, ?f16, ?f87)");
        add(L"f107", L"if(?f26, ?f4, ?f88)");
        add(L"f108", L"if(?f26, ?f16, ?f89)");
        add(L"f109", L"if(?f26, ?f86, ?f102)");
        add(L"f110", L"if(?f26, ?f87, ?f104)");
        add(L"f111", L"if(?f26, ?f88, ?f102)");
        add(L"f112", L"if(?f26, ?f89, ?f104)");
        add(L"f113", L"?f2 + ?f26");
        add(L"f114", L"?f2 + ?f2");
        add(L"f115", L"?f114 * ?f10 / ?f1");
        add(L"f116", L"0 - ?f115");
        add(L"f117", L"cos(?f116)");
        add(L"f118", L"0 - ?f117");
        add(L"f119", L"?f118 * ?f15");
        add(L"f120", L"sin(?f116)");
        add(L"f121", L"0 - ?f120");
        add(L"f122", L"?f121 * ?f15");
        add(L"f123", L"sqrt(?f119 * ?f119 + ?f122 * ?f122 + 0 * 0)");
        add(L"f124", L"?f15 * ?f15 / ?f123");
        add(L"f125", L"?f121 * ?f124");
        add(L"f126", L"?f15 - ?f125");
        add(L"f127", L"?f118 * ?f124");
        add(L"f128", L"?f6 - ?f127");
        add(L"f129", L"?f126 - ?f15");
        add(L"f130", L"?f128 - ?f15");
        add(L"f131", L"?f126 + ?f15");
        add(L"f132", L"?f128 + ?f15");
        add(L"f133", L"?f113 + ?f2");
        add(L"f134", L"?f133 * ?f10 / ?f1");
        add(L"f135", L"0 - ?f134");
        add(L"f136", L"cos(?f135)");
        add(L"f137", L"0 - ?f136");
        add(L"f138", L"?f137 * ?f15");
        add(L"f139", L"sin(?f135)");
        add(L"f140", L"0 - ?f139");
        add(L"f141", L"?f140 * ?f15");
        add(L"f142", L"sqrt(?f138 * ?f138 + ?f141 * ?f141 + 0 * 0)");
        add(L"f143", L"?f15 * ?f15 / ?f142");
        add(L"f144", L"?f140 * ?f143");
        add(L"f145", L"?f126 + ?f144");
        add(L"f146", L"?f137 * ?f143");
        add(L"f147", L"?f128 + ?f146");
        add(L"f148", L"if(?f26, ?f15, ?f129)");
        add(L"f149", L"if(?f26, ?f6, ?f130)");
        add(L"f150", L"if(?f26, ?f15, ?f131)");
        add(L"f151", L"if(?f26, ?f6, ?f132)");
        add(L"f152", L"if(?f26, ?f129, ?f145)");
        add(L"f153", L"if(?f26, ?f130, ?f147)");
        add(L"f154", L"if(?f26, ?f131, ?f145)");
        add(L"f155", L"if(?f26, ?f132, ?f147)");
        add(L"f156", L"?f1 + ?f26");
        add(L"f157", L"?f1 + ?f2");
        add(L"f158", L"?f157 * ?f10 / ?f1");
        add(L"f159", L"0 - ?f158");
        add(L"f160", L"cos(?f159)");
        add(L"f161", L"0 - ?f160");
        add(L"f162", L"?f161 * ?f13");
        add(L"f163", L"sin(?f159)");
        add(L"f164", L"0 - ?f163");
        add(L"f165", L"?f164 * ?f13");
        add(L"f166", L"sqrt(?f162 * ?f162 + ?f165 * ?f165 + 0 * 0)");
        add(L"f167", L"?f13 * ?f13 / ?f166");
        add(L"f168", L"?f164 * ?f167");
        add(L"f169", L"?f3 - ?f168");
        add(L"f170", L"?f161 * ?f167");
        add(L"f171", L"?f13 - ?f170");
        add(L"f172", L"?f169 - ?f13");
        add(L"f173", L"?f171 - ?f13");
        add(L"f174", L"?f169 + ?f13");
        add(L"f175", L"?f171 + ?f13");
        add(L"f176", L"?f156 + ?f2");
        add(L"f177", L"?f176 * ?f10 / ?f1");
        add(L"f178", L"0 - ?f177");
        add(L"f179", L"cos(?f178)");
        add(L"f180", L"0 - ?f179");
        add(L"f181", L"?f180 * ?f13");
        add(L"f182", L"sin(?f178)");
        add(L"f183", L"0 - ?f182");
        add(L"f184", L"?f183 * ?f13");
        add(L"f185", L"sqrt(?f181 * ?f181 + ?f184 * ?f184 + 0 * 0)");
        add(L"f186", L"?f13 * ?f13 / ?f185");
        add(L"f187", L"?f183 * ?f186");
        add(L"f188", L"?f169 + ?f187");
        add(L"f189", L"?f180 * ?f186");
        add(L"f190", L"?f171 + ?f189");
        add(L"f191", L"if(?f26, ?f3, ?f172)");
        add(L"f192", L"if(?f26, ?f13, ?f173)");
        add(L"f193", L"if(?f26, ?f3, ?f174)");
        add(L"f194", L"if(?f26, ?f13, ?f175)");
        add(L"f195", L"if(?f26, ?f172, ?f188)");
        add(L"f196", L"if(?f26, ?f173, ?f190)");
        add(L"f197", L"if(?f26, ?f174, ?f188)");
        add(L"f198", L"if(?f26, ?f175, ?f190)");
	}
};
class oox_shape_roundRect : public oox_shape
{
public:
	oox_shape_roundRect()
	{
		odf_type_name	=L"ooxml-roundRect";
		
		enhanced_path	= L"M 0 ?f2 G ?f2 ?f2 ?f12 ?f13 L ?f3 0 G ?f2 ?f2 ?f14 ?f15 L ?f11 ?f4 G ?f2 ?f2 ?f16 ?f17 L ?f2 ?f10 G ?f2 ?f2 ?f18 ?f19 Z N";
		text_areas		= L"?f5 ?f5 ?f6 ?f7";
		view_box		= L"0 0 0 0";
		modifiers		= L"16667";
		
		add(L"f0", L"if(0-$0 ,0,if(50000-$0 ,$0 ,50000))");
		add(L"f1", L"min(logwidth,logheight)");
		add(L"f2", L"?f1 *?f0 /100000");
		add(L"f3", L"logwidth+0-?f2 ");
		add(L"f4", L"logheight+0-?f2 ");
		add(L"f5", L"?f2 *29289/100000");
		add(L"f6", L"logwidth+0-?f5 ");
		add(L"f7", L"logheight+0-?f5 ");
		add(L"f8", L"logwidth/2");
		add(L"f9", L"logheight/2");
		add(L"f10", L"logheight");
		add(L"f11", L"logwidth");
		add(L"f12", L"(10800000)/60000.0");
		add(L"f13", L"(5400000)/60000.0");
		add(L"f14", L"(16200000)/60000.0");
		add(L"f15", L"(5400000)/60000.0");
		add(L"f16", L"(0)/60000.0");
		add(L"f17", L"(5400000)/60000.0");
		add(L"f18", L"(5400000)/60000.0");
		add(L"f19", L"(5400000)/60000.0");
//-----------------------------------------------------------------		
		_handle h1;
		
		h1.position = L"?f2 0";
		h1.x_maximum= L"50000";
		h1.x_minimum= L"0";
		handles.push_back(h1);
	}
};
}
