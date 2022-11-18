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
class oox_shape_star6 : public oox_shape
{
public:
	oox_shape_star6()
	{
		odf_type_name	=L"ooxml-star6";

		modifiers		= L"28868 0";
		enhanced_path	= L"M ?f22 ?f8 L ?f29 ?f38 ?f12 ?f4 ?f30 ?f38 ?f23 ?f8 ?f31 ?f9 ?f23 ?f24 ?f30 ?f39 ?f12 ?f5 ?f29 ?f39 ?f22 ?f24 ?f28 ?f9 Z N";
		text_areas		= L"?f28 ?f38 ?f31 ?f39";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f23 ?f8 ?f23 ?f24 ?f22 ?f24 ?f22 ?f8";
		
        add(L"f0", L"10800000");
        add(L"f1", L"5400000");
        add(L"f2", L"left");
        add(L"f3", L"right");
        add(L"f4", L"top");
        add(L"f5", L"bottom");
        add(L"f6", L"?f5 - ?f4");
        add(L"f7", L"?f6 / 2");
        add(L"f8", L"?f6 / 4");
        add(L"f9", L"?f4 + ?f7");
        add(L"f10", L"?f3 - ?f2");
        add(L"f11", L"?f10 / 2");
        add(L"f12", L"?f2 + ?f11");
        add(L"f13", L"5419351 / 1725033");
        add(L"f14", L"$0");
        add(L"f15", L"?f11 * ?f40 / 100000");
        add(L"f16", L"1800000 + ?f1");
        add(L"f17", L"?f16 * ?f13 / ?f0");
        add(L"f18", L"0 - ?f17");
        add(L"f19", L"sin(?f18)");
        add(L"f20", L"0 - ?f19");
        add(L"f21", L"?f20 * ?f15");
        add(L"f22", L"?f12 - ?f21");
        add(L"f23", L"?f12 + ?f21");
        add(L"f24", L"?f9 + ?f8");
        add(L"f25", L"?f15 * ?f14 / 50000");
        add(L"f26", L"?f7 * ?f14 / 50000");
        add(L"f27", L"?f25 / 2");
        add(L"f28", L"?f12 - ?f25");
        add(L"f29", L"?f12 - ?f27");
        add(L"f30", L"?f12 + ?f27");
        add(L"f31", L"?f12 + ?f25");
        add(L"f32", L"3600000 + ?f1");
        add(L"f33", L"?f32 * ?f13 / ?f0");
        add(L"f34", L"0 - ?f33");
        add(L"f35", L"cos(?f34)");
        add(L"f36", L"0 - ?f35");
        add(L"f37", L"?f36 * ?f26");
        add(L"f38", L"?f9 - ?f37");
        add(L"f39", L"?f9 + ?f37");
        add(L"f40", L"115470");

	}
};
class oox_shape_star7 : public oox_shape
{
public:
	oox_shape_star7()
	{
		odf_type_name	=L"ooxml-star7";

		modifiers		= L"34601 0 0";
		enhanced_path	= L"M ?f20 ?f27 L ?f34 ?f44 ?f21 ?f26 ?f36 ?f43 ?f9 ?f2 ?f37 ?f43 ?f24 ?f26 ?f39 ?f44 ?f25 ?f27 ?f38 ?f45 ?f23 ?f28 ?f9 ?f46 ?f22 ?f28 ?f35 ?f45 Z N";
		text_areas		= L"?f35 ?f43 ?f38 ?f45";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f24 ?f26 ?f25 ?f27 ?f23 ?f28 ?f22 ?f28 ?f20 ?f27 ?f21 ?f26";
		
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
        add(L"f10", L"$0");
        add(L"f11", L"?f8 * ?f47 / 100000");
        add(L"f12", L"?f5 * ?f48 / 100000");
        add(L"f13", L"?f6 * ?f48 / 100000");
        add(L"f14", L"?f11 * 97493 / 100000");
        add(L"f15", L"?f11 * 78183 / 100000");
        add(L"f16", L"?f11 * 43388 / 100000");
        add(L"f17", L"?f12 * 62349 / 100000");
        add(L"f18", L"?f12 * 22252 / 100000");
        add(L"f19", L"?f12 * 90097 / 100000");
        add(L"f20", L"?f9 - ?f14");
        add(L"f21", L"?f9 - ?f15");
        add(L"f22", L"?f9 - ?f16");
        add(L"f23", L"?f9 + ?f16");
        add(L"f24", L"?f9 + ?f15");
        add(L"f25", L"?f9 + ?f14");
        add(L"f26", L"?f13 - ?f17");
        add(L"f27", L"?f13 + ?f18");
        add(L"f28", L"?f13 + ?f19");
        add(L"f29", L"?f11 * ?f10 / 50000");
        add(L"f30", L"?f12 * ?f10 / 50000");
        add(L"f31", L"?f29 * 97493 / 100000");
        add(L"f32", L"?f29 * 78183 / 100000");
        add(L"f33", L"?f29 * 43388 / 100000");
        add(L"f34", L"?f9 - ?f31");
        add(L"f35", L"?f9 - ?f32");
        add(L"f36", L"?f9 - ?f33");
        add(L"f37", L"?f9 + ?f33");
        add(L"f38", L"?f9 + ?f32");
        add(L"f39", L"?f9 + ?f31");
        add(L"f40", L"?f30 * 90097 / 100000");
        add(L"f41", L"?f30 * 22252 / 100000");
        add(L"f42", L"?f30 * 62349 / 100000");
        add(L"f43", L"?f13 - ?f40");
        add(L"f44", L"?f13 - ?f41");
        add(L"f45", L"?f13 + ?f42");
        add(L"f46", L"?f13 + ?f30");
        add(L"f47", L"102572");
        add(L"f48", L"105210");

	}
};
class oox_shape_star10 : public oox_shape
{
public:
	oox_shape_star10()
	{
		odf_type_name	=L"ooxml-star10";

		modifiers		= L"42533 0";
		enhanced_path	= L"M ?f14 ?f21 L ?f31 ?f37 ?f15 ?f20 ?f32 ?f36 ?f9 ?f2 ?f33 ?f36 ?f16 ?f20 ?f34 ?f37 ?f17 ?f21 ?f35 ?f6 ?f17 ?f22 ?f34 ?f38 ?f16 ?f23 ?f33 ?f39 ?f9 ?f3 ?f32 ?f39 ?f15 ?f23 ?f31 ?f38 ?f14 ?f22 ?f30 ?f6 Z N";
		text_areas		= L"?f31 ?f37 ?f34 ?f38";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f17 ?f21 ?f17 ?f22 ?f16 ?f23 ?f15 ?f23 ?f14 ?f22 ?f14 ?f21 ?f15 ?f20 ?f16 ?f20";
		
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
        add(L"f10", L"$0");
        add(L"f11", L"?f8 * ?f40 / 100000");
        add(L"f12", L"?f11 * 95106 / 100000");
        add(L"f13", L"?f11 * 58779 / 100000");
        add(L"f14", L"?f9 - ?f12");
        add(L"f15", L"?f9 - ?f13");
        add(L"f16", L"?f9 + ?f13");
        add(L"f17", L"?f9 + ?f12");
        add(L"f18", L"?f5 * 80902 / 100000");
        add(L"f19", L"?f5 * 30902 / 100000");
        add(L"f20", L"?f6 - ?f18");
        add(L"f21", L"?f6 - ?f19");
        add(L"f22", L"?f6 + ?f19");
        add(L"f23", L"?f6 + ?f18");
        add(L"f24", L"?f11 * ?f10 / 50000");
        add(L"f25", L"?f5 * ?f10 / 50000");
        add(L"f26", L"?f24 * 80902 / 100000");
        add(L"f27", L"?f24 * 30902 / 100000");
        add(L"f28", L"?f25 * 95106 / 100000");
        add(L"f29", L"?f25 * 58779 / 100000");
        add(L"f30", L"?f9 - ?f24");
        add(L"f31", L"?f9 - ?f26");
        add(L"f32", L"?f9 - ?f27");
        add(L"f33", L"?f9 + ?f27");
        add(L"f34", L"?f9 + ?f26");
        add(L"f35", L"?f9 + ?f24");
        add(L"f36", L"?f6 - ?f28");
        add(L"f37", L"?f6 - ?f29");
        add(L"f38", L"?f6 + ?f29");
        add(L"f39", L"?f6 + ?f28");
        add(L"f40", L"105146");

	}
};

class oox_shape_star12 : public oox_shape
{
public:
	oox_shape_star12()
	{
		odf_type_name	=L"ooxml-star12";

		modifiers		= L"37500";
		enhanced_path	= L"M ?f2 ?f9 L ?f63 ?f71 ?f28 ?f8 ?f64 ?f70 ?f12 ?f31 ?f65 ?f69 ?f13 ?f4 ?f66 ?f69 ?f29 ?f31 ?f67 ?f70 ?f30 ?f8 ?f68 ?f71 ?f3 ?f9 ?f68 ?f72 ?f30 ?f32 ?f67 ?f73 ?f29 ?f33 ?f66 ?f74 ?f13 ?f5 ?f65 ?f74 ?f12 ?f33 ?f64 ?f73 ?f28 ?f32 ?f63 ?f72 Z N";
		text_areas		= L"?f64 ?f70 ?f67 ?f73";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f30 ?f8 ?f30 ?f32 ?f29 ?f33 ?f12 ?f33 ?f28 ?f32 ?f28 ?f8 ?f12 ?f31 ?f29 ?f31";
		
        add(L"f0", L"10800000");
        add(L"f1", L"5400000");
        add(L"f2", L"left");
        add(L"f3", L"right");
        add(L"f4", L"top");
        add(L"f5", L"bottom");
        add(L"f6", L"?f5 - ?f4");
        add(L"f7", L"?f6 / 2");
        add(L"f8", L"?f6 / 4");
        add(L"f9", L"?f4 + ?f7");
        add(L"f10", L"?f3 - ?f2");
        add(L"f11", L"?f10 / 2");
        add(L"f12", L"?f10 / 4");
        add(L"f13", L"?f2 + ?f11");
        add(L"f14", L"5419351 / 1725033");
        add(L"f15", L"$0");
        add(L"f16", L"1800000 + ?f1");
        add(L"f17", L"?f16 * ?f14 / ?f0");
        add(L"f18", L"0 - ?f17");
        add(L"f19", L"sin(?f18)");
        add(L"f20", L"0 - ?f19");
        add(L"f21", L"?f20 * ?f11");
        add(L"f22", L"3600000 + ?f1");
        add(L"f23", L"?f22 * ?f14 / ?f0");
        add(L"f24", L"0 - ?f23");
        add(L"f25", L"cos(?f24)");
        add(L"f26", L"0 - ?f25");
        add(L"f27", L"?f26 * ?f7");
        add(L"f28", L"?f13 - ?f21");
        add(L"f29", L"?f10 * 3 / 4");
        add(L"f30", L"?f13 + ?f21");
        add(L"f31", L"?f9 - ?f27");
        add(L"f32", L"?f6 * 3 / 4");
        add(L"f33", L"?f9 + ?f27");
        add(L"f34", L"?f11 * ?f15 / 50000");
        add(L"f35", L"?f7 * ?f15 / 50000");
        add(L"f36", L"900000 + ?f1");
        add(L"f37", L"?f36 * ?f14 / ?f0");
        add(L"f38", L"0 - ?f37");
        add(L"f39", L"sin(?f38)");
        add(L"f40", L"0 - ?f39");
        add(L"f41", L"?f40 * ?f34");
        add(L"f42", L"2700000 + ?f1");
        add(L"f43", L"?f42 * ?f14 / ?f0");
        add(L"f44", L"0 - ?f43");
        add(L"f45", L"sin(?f44)");
        add(L"f46", L"0 - ?f45");
        add(L"f47", L"?f46 * ?f34");
        add(L"f48", L"4500000 + ?f1");
        add(L"f49", L"?f48 * ?f14 / ?f0");
        add(L"f50", L"0 - ?f49");
        add(L"f51", L"sin(?f50)");
        add(L"f52", L"0 - ?f51");
        add(L"f53", L"?f52 * ?f34");
        add(L"f54", L"cos(?f50)");
        add(L"f55", L"0 - ?f54");
        add(L"f56", L"?f55 * ?f35");
        add(L"f57", L"cos(?f44)");
        add(L"f58", L"0 - ?f57");
        add(L"f59", L"?f58 * ?f35");
        add(L"f60", L"cos(?f38)");
        add(L"f61", L"0 - ?f60");
        add(L"f62", L"?f61 * ?f35");
        add(L"f63", L"?f13 - ?f41");
        add(L"f64", L"?f13 - ?f47");
        add(L"f65", L"?f13 - ?f53");
        add(L"f66", L"?f13 + ?f53");
        add(L"f67", L"?f13 + ?f47");
        add(L"f68", L"?f13 + ?f41");
        add(L"f69", L"?f9 - ?f56");
        add(L"f70", L"?f9 - ?f59");
        add(L"f71", L"?f9 - ?f62");
        add(L"f72", L"?f9 + ?f62");
        add(L"f73", L"?f9 + ?f59");
        add(L"f74", L"?f9 + ?f56");

	}
};
class oox_shape_star16 : public oox_shape
{
public:
	oox_shape_star16()
	{
		odf_type_name	=L"ooxml-star16";

		modifiers		= L"37500";
		enhanced_path	= L"M ?f2 ?f8 L ?f42 ?f53 ?f20 ?f28 ?f43 ?f52 ?f21 ?f27 ?f44 ?f51 ?f22 ?f26 ?f45 ?f50 ?f11 ?f4 ?f46 ?f50 ?f23 ?f26 ?f47 ?f51 ?f24 ?f27 ?f48 ?f52 ?f25 ?f28 ?f49 ?f53 ?f3 ?f8 ?f49 ?f54 ?f25 ?f29 ?f48 ?f55 ?f24 ?f30 ?f47 ?f56 ?f23 ?f31 ?f46 ?f57 ?f11 ?f5 ?f45 ?f57 ?f22 ?f31 ?f44 ?f56 ?f21 ?f30 ?f43 ?f55 ?f20 ?f29 ?f42 ?f54 Z N";
		text_areas		= L"?f67 ?f68 ?f69 ?f70";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f24 ?f27 ?f25 ?f28 ?f25 ?f29 ?f24 ?f30 ?f23 ?f31 ?f22 ?f31 ?f21 ?f30 ?f20 ?f29 ?f20 ?f28 ?f21 ?f27 ?f22 ?f26 ?f23 ?f26";
		
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
        add(L"f12", L"5419351 / 1725033");
        add(L"f13", L"$0");
        add(L"f14", L"?f10 * 92388 / 100000");
        add(L"f15", L"?f10 * 70711 / 100000");
        add(L"f16", L"?f10 * 38268 / 100000");
        add(L"f17", L"?f7 * 92388 / 100000");
        add(L"f18", L"?f7 * 70711 / 100000");
        add(L"f19", L"?f7 * 38268 / 100000");
        add(L"f20", L"?f11 - ?f14");
        add(L"f21", L"?f11 - ?f15");
        add(L"f22", L"?f11 - ?f16");
        add(L"f23", L"?f11 + ?f16");
        add(L"f24", L"?f11 + ?f15");
        add(L"f25", L"?f11 + ?f14");
        add(L"f26", L"?f8 - ?f17");
        add(L"f27", L"?f8 - ?f18");
        add(L"f28", L"?f8 - ?f19");
        add(L"f29", L"?f8 + ?f19");
        add(L"f30", L"?f8 + ?f18");
        add(L"f31", L"?f8 + ?f17");
        add(L"f32", L"?f10 * ?f13 / 50000");
        add(L"f33", L"?f7 * ?f13 / 50000");
        add(L"f34", L"?f32 * 98079 / 100000");
        add(L"f35", L"?f32 * 83147 / 100000");
        add(L"f36", L"?f32 * 55557 / 100000");
        add(L"f37", L"?f32 * 19509 / 100000");
        add(L"f38", L"?f33 * 98079 / 100000");
        add(L"f39", L"?f33 * 83147 / 100000");
        add(L"f40", L"?f33 * 55557 / 100000");
        add(L"f41", L"?f33 * 19509 / 100000");
        add(L"f42", L"?f11 - ?f34");
        add(L"f43", L"?f11 - ?f35");
        add(L"f44", L"?f11 - ?f36");
        add(L"f45", L"?f11 - ?f37");
        add(L"f46", L"?f11 + ?f37");
        add(L"f47", L"?f11 + ?f36");
        add(L"f48", L"?f11 + ?f35");
        add(L"f49", L"?f11 + ?f34");
        add(L"f50", L"?f8 - ?f38");
        add(L"f51", L"?f8 - ?f39");
        add(L"f52", L"?f8 - ?f40");
        add(L"f53", L"?f8 - ?f41");
        add(L"f54", L"?f8 + ?f41");
        add(L"f55", L"?f8 + ?f40");
        add(L"f56", L"?f8 + ?f39");
        add(L"f57", L"?f8 + ?f38");
        add(L"f58", L"2700000 + ?f1");
        add(L"f59", L"?f58 * ?f12 / ?f0");
        add(L"f60", L"0 - ?f59");
        add(L"f61", L"sin(?f60)");
        add(L"f62", L"0 - ?f61");
        add(L"f63", L"?f62 * ?f32");
        add(L"f64", L"cos(?f60)");
        add(L"f65", L"0 - ?f64");
        add(L"f66", L"?f65 * ?f33");
        add(L"f67", L"?f11 - ?f63");
        add(L"f68", L"?f8 - ?f66");
        add(L"f69", L"?f11 + ?f63");
        add(L"f70", L"?f8 + ?f66");

	}
};
class oox_shape_star32 : public oox_shape
{
public:
	oox_shape_star32()
	{
		odf_type_name	=L"ooxml-star32";

		modifiers		= L"37500";
		enhanced_path	= L"M ?f2 ?f8 L ?f81 ?f104 ?f35 ?f55 ?f82 ?f103 ?f36 ?f54 ?f83 ?f102 ?f37 ?f53 ?f84 ?f101 ?f38 ?f52 ?f85 ?f100 ?f39 ?f51 ?f86 ?f99 ?f40 ?f50 ?f87 ?f98 ?f41 ?f49 ?f88 ?f97 ?f11 ?f4 ?f89 ?f97 ?f42 ?f49 ?f90 ?f98 ?f43 ?f50 ?f91 ?f99 ?f44 ?f51 ?f92 ?f100 ?f45 ?f52 ?f93 ?f101 ?f46 ?f53 ?f94 ?f102 ?f47 ?f54 ?f95 ?f103 ?f48 ?f55 ?f96 ?f104 ?f3 ?f8 ?f96 ?f105 ?f48 ?f56 ?f95 ?f106 ?f47 ?f57 ?f94 ?f107 ?f46 ?f58 ?f93 ?f108 ?f45 ?f59 ?f92 ?f109 ?f44 ?f60 ?f91 ?f110 ?f43 ?f61 ?f90 ?f111 ?f42 ?f62 ?f89 ?f112 ?f11 ?f5 ?f88 ?f112 ?f41 ?f62 ?f87 ?f111 ?f40 ?f61 ?f86 ?f110 ?f39 ?f60 ?f85 ?f109 ?f38 ?f59 ?f84 ?f108 ?f37 ?f58 ?f83 ?f107 ?f36 ?f57 ?f82 ?f106 ?f35 ?f56 ?f81 ?f105 Z N";
		text_areas		= L"?f115 ?f116 ?f117 ?f118";
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
        add(L"f12", L"5419351 / 1725033");
        add(L"f13", L"$0");
        add(L"f14", L"?f10 * 98079 / 100000");
        add(L"f15", L"?f10 * 92388 / 100000");
        add(L"f16", L"?f10 * 83147 / 100000");
        add(L"f17", L"2700000 + ?f1");
        add(L"f18", L"?f17 * ?f12 / ?f0");
        add(L"f19", L"0 - ?f18");
        add(L"f20", L"sin(?f19)");
        add(L"f21", L"0 - ?f20");
        add(L"f22", L"?f21 * ?f10");
        add(L"f23", L"?f10 * 55557 / 100000");
        add(L"f24", L"?f10 * 38268 / 100000");
        add(L"f25", L"?f10 * 19509 / 100000");
        add(L"f26", L"?f7 * 98079 / 100000");
        add(L"f27", L"?f7 * 92388 / 100000");
        add(L"f28", L"?f7 * 83147 / 100000");
        add(L"f29", L"cos(?f19)");
        add(L"f30", L"0 - ?f29");
        add(L"f31", L"?f30 * ?f7");
        add(L"f32", L"?f7 * 55557 / 100000");
        add(L"f33", L"?f7 * 38268 / 100000");
        add(L"f34", L"?f7 * 19509 / 100000");
        add(L"f35", L"?f11 - ?f14");
        add(L"f36", L"?f11 - ?f15");
        add(L"f37", L"?f11 - ?f16");
        add(L"f38", L"?f11 - ?f22");
        add(L"f39", L"?f11 - ?f23");
        add(L"f40", L"?f11 - ?f24");
        add(L"f41", L"?f11 - ?f25");
        add(L"f42", L"?f11 + ?f25");
        add(L"f43", L"?f11 + ?f24");
        add(L"f44", L"?f11 + ?f23");
        add(L"f45", L"?f11 + ?f22");
        add(L"f46", L"?f11 + ?f16");
        add(L"f47", L"?f11 + ?f15");
        add(L"f48", L"?f11 + ?f14");
        add(L"f49", L"?f8 - ?f26");
        add(L"f50", L"?f8 - ?f27");
        add(L"f51", L"?f8 - ?f28");
        add(L"f52", L"?f8 - ?f31");
        add(L"f53", L"?f8 - ?f32");
        add(L"f54", L"?f8 - ?f33");
        add(L"f55", L"?f8 - ?f34");
        add(L"f56", L"?f8 + ?f34");
        add(L"f57", L"?f8 + ?f33");
        add(L"f58", L"?f8 + ?f32");
        add(L"f59", L"?f8 + ?f31");
        add(L"f60", L"?f8 + ?f28");
        add(L"f61", L"?f8 + ?f27");
        add(L"f62", L"?f8 + ?f26");
        add(L"f63", L"?f10 * ?f13 / 50000");
        add(L"f64", L"?f7 * ?f13 / 50000");
        add(L"f65", L"?f63 * 99518 / 100000");
        add(L"f66", L"?f63 * 95694 / 100000");
        add(L"f67", L"?f63 * 88192 / 100000");
        add(L"f68", L"?f63 * 77301 / 100000");
        add(L"f69", L"?f63 * 63439 / 100000");
        add(L"f70", L"?f63 * 47140 / 100000");
        add(L"f71", L"?f63 * 29028 / 100000");
        add(L"f72", L"?f63 * 9802 / 100000");
        add(L"f73", L"?f64 * 99518 / 100000");
        add(L"f74", L"?f64 * 95694 / 100000");
        add(L"f75", L"?f64 * 88192 / 100000");
        add(L"f76", L"?f64 * 77301 / 100000");
        add(L"f77", L"?f64 * 63439 / 100000");
        add(L"f78", L"?f64 * 47140 / 100000");
        add(L"f79", L"?f64 * 29028 / 100000");
        add(L"f80", L"?f64 * 9802 / 100000");
        add(L"f81", L"?f11 - ?f65");
        add(L"f82", L"?f11 - ?f66");
        add(L"f83", L"?f11 - ?f67");
        add(L"f84", L"?f11 - ?f68");
        add(L"f85", L"?f11 - ?f69");
        add(L"f86", L"?f11 - ?f70");
        add(L"f87", L"?f11 - ?f71");
        add(L"f88", L"?f11 - ?f72");
        add(L"f89", L"?f11 + ?f72");
        add(L"f90", L"?f11 + ?f71");
        add(L"f91", L"?f11 + ?f70");
        add(L"f92", L"?f11 + ?f69");
        add(L"f93", L"?f11 + ?f68");
        add(L"f94", L"?f11 + ?f67");
        add(L"f95", L"?f11 + ?f66");
        add(L"f96", L"?f11 + ?f65");
        add(L"f97", L"?f8 - ?f73");
        add(L"f98", L"?f8 - ?f74");
        add(L"f99", L"?f8 - ?f75");
        add(L"f100", L"?f8 - ?f76");
        add(L"f101", L"?f8 - ?f77");
        add(L"f102", L"?f8 - ?f78");
        add(L"f103", L"?f8 - ?f79");
        add(L"f104", L"?f8 - ?f80");
        add(L"f105", L"?f8 + ?f80");
        add(L"f106", L"?f8 + ?f79");
        add(L"f107", L"?f8 + ?f78");
        add(L"f108", L"?f8 + ?f77");
        add(L"f109", L"?f8 + ?f76");
        add(L"f110", L"?f8 + ?f75");
        add(L"f111", L"?f8 + ?f74");
        add(L"f112", L"?f8 + ?f73");
        add(L"f113", L"?f21 * ?f63");
        add(L"f114", L"?f30 * ?f64");
        add(L"f115", L"?f11 - ?f113");
        add(L"f116", L"?f8 - ?f114");
        add(L"f117", L"?f11 + ?f113");
        add(L"f118", L"?f8 + ?f114");

	}
};
}
