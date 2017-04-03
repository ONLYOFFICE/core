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
#include "../oox_shape_defines.h"
	//	+shapetypeLeftCircularArrow,
	//	shapetypeLeftRightCircularArrow,
	//	shapetypeSwooshArrow,
	//	+shapetypeCircularArrow
namespace cpdoccore 
{
class oox_shape_LeftCircularArrow : public oox_shape
{
public:
	oox_shape_LeftCircularArrow()
	{
		odf_type_name	=L"ooxml-leftCircularArrow";

		enhanced_path	= L"M 529769 128373 L 570534 225808 A ?f91 ?f92 ?f93 ?f94 570534 225808 ?f88 ?f90  W ?f95 ?f96 ?f97 ?f98 570534 225808 ?f88 ?f90 L 246425 854941 139301 967187 5691 907641 74093 892667 A ?f138 ?f139 ?f140 ?f141 74093 892667 ?f135 ?f137  W ?f142 ?f143 ?f144 ?f145 74093 892667 ?f135 ?f137 Z N";
		text_areas		= L"?f48 ?f50 ?f49 ?f51";
		glue_points		= L"?f40 ?f41 ?f42 ?f43 ?f44 ?f45 ?f46 ?f47";
		view_box		= L"0 0 1637004 1637004";
		
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
		add(L"f10", L"?f7 / 1637004");
		add(L"f11", L"?f6 / 1637004");
		add(L"f12", L"139301 - 818502");
		add(L"f13", L"967187 - 818502");
		add(L"f14", L"sqrt(?f12 * ?f12 + ?f13 * ?f13 + 0 * 0)");
		add(L"f15", L"atan2(?f12, ?f13)");
		add(L"f16", L"?f15 + ?f1");
		add(L"f17", L"?f16 * ?f9 / ?f0");
		add(L"f18", L"0 - ?f17");
		add(L"f19", L"529769 - 818502");
		add(L"f20", L"128373 - 818502");
		add(L"f21", L"sqrt(?f19 * ?f19 + ?f20 * ?f20 + 0 * 0)");
		add(L"f22", L"atan2(?f19, ?f20)");
		add(L"f23", L"?f22 + ?f1");
		add(L"f24", L"?f23 * ?f9 / ?f0");
		add(L"f25", L"0 - ?f24");
		add(L"f26", L"74093 - 818502");
		add(L"f27", L"892667 - 818502");
		add(L"f28", L"sqrt(?f26 * ?f26 + ?f27 * ?f27 + 0 * 0)");
		add(L"f29", L"atan2(?f26, ?f27)");
		add(L"f30", L"?f29 + ?f1");
		add(L"f31", L"?f30 * ?f9 / ?f0");
		add(L"f32", L"0 - ?f31");
		add(L"f33", L"246425 - 818502");
		add(L"f34", L"854941 - 818502");
		add(L"f35", L"sqrt(?f33 * ?f33 + ?f34 * ?f34 + 0 * 0)");
		add(L"f36", L"atan2(?f33, ?f34)");
		add(L"f37", L"?f36 + ?f1");
		add(L"f38", L"?f37 * ?f9 / ?f0");
		add(L"f39", L"0 - ?f38");
		add(L"f40", L"550151 / ?f10");
		add(L"f41", L"177091 / ?f11");
		add(L"f42", L"5691 / ?f10");
		add(L"f43", L"907641 / ?f11");
		add(L"f44", L"139301 / ?f10");
		add(L"f45", L"967187 / ?f11");
		add(L"f46", L"246425 / ?f10");
		add(L"f47", L"854941 / ?f11");
		add(L"f48", L"289519 / ?f10");
		add(L"f49", L"1347485 / ?f10");
		add(L"f50", L"289519 / ?f11");
		add(L"f51", L"1347485 / ?f11");
		add(L"f52", L"21550000 - -4313128");
		add(L"f53", L"if(?f52, -4313128, 21550000)");
		add(L"f54", L"-21550000 - ?f53");
		add(L"f55", L"if(?f54, -21550000, ?f53)");
		add(L"f56", L"14837806 + ?f55");
		add(L"f57", L"14837806 + ?f1");
		add(L"f58", L"?f57 * ?f8 / ?f0");
		add(L"f59", L"0 - ?f58");
		add(L"f60", L"cos(?f59)");
		add(L"f61", L"0 - ?f60");
		add(L"f62", L"?f61 * 642475");
		add(L"f63", L"sin(?f59)");
		add(L"f64", L"0 - ?f63");
		add(L"f65", L"?f64 * 642475");
		add(L"f66", L"sqrt(?f62 * ?f62 + ?f65 * ?f65 + 0 * 0)");
		add(L"f67", L"642475 * 642475 / ?f66");
		add(L"f68", L"?f64 * ?f67");
		add(L"f69", L"570534 - ?f68");
		add(L"f70", L"?f61 * ?f67");
		add(L"f71", L"225808 - ?f70");
		add(L"f72", L"?f69 - 642475");
		add(L"f73", L"?f71 - 642475");
		add(L"f74", L"?f69 + 642475");
		add(L"f75", L"?f71 + 642475");
		add(L"f76", L"?f56 + ?f1");
		add(L"f77", L"?f76 * ?f8 / ?f0");
		add(L"f78", L"0 - ?f77");
		add(L"f79", L"cos(?f78)");
		add(L"f80", L"0 - ?f79");
		add(L"f81", L"?f80 * 642475");
		add(L"f82", L"sin(?f78)");
		add(L"f83", L"0 - ?f82");
		add(L"f84", L"?f83 * 642475");
		add(L"f85", L"sqrt(?f81 * ?f81 + ?f84 * ?f84 + 0 * 0)");
		add(L"f86", L"642475 * 642475 / ?f85");
		add(L"f87", L"?f83 * ?f86");
		add(L"f88", L"?f69 + ?f87");
		add(L"f89", L"?f80 * ?f86");
		add(L"f90", L"?f71 + ?f89");
		add(L"f91", L"if(?f55, 570534, ?f72)");
		add(L"f92", L"if(?f55, 225808, ?f73)");
		add(L"f93", L"if(?f55, 570534, ?f74)");
		add(L"f94", L"if(?f55, 225808, ?f75)");
		add(L"f95", L"if(?f55, ?f72, ?f88)");
		add(L"f96", L"if(?f55, ?f73, ?f90)");
		add(L"f97", L"if(?f55, ?f74, ?f88)");
		add(L"f98", L"if(?f55, ?f75, ?f90)");
		add(L"f99", L"21550000 - 4379179");
		add(L"f100", L"if(?f99, 4379179, 21550000)");
		add(L"f101", L"-21550000 - ?f100");
		add(L"f102", L"if(?f101, -21550000, ?f100)");
		add(L"f103", L"10458627 + ?f102");
		add(L"f104", L"10458627 + ?f1");
		add(L"f105", L"?f104 * ?f8 / ?f0");
		add(L"f106", L"0 - ?f105");
		add(L"f107", L"cos(?f106)");
		add(L"f108", L"0 - ?f107");
		add(L"f109", L"?f108 * 748094");
		add(L"f110", L"sin(?f106)");
		add(L"f111", L"0 - ?f110");
		add(L"f112", L"?f111 * 748094");
		add(L"f113", L"sqrt(?f109 * ?f109 + ?f112 * ?f112 + 0 * 0)");
		add(L"f114", L"748094 * 748094 / ?f113");
		add(L"f115", L"?f111 * ?f114");
		add(L"f116", L"74093 - ?f115");
		add(L"f117", L"?f108 * ?f114");
		add(L"f118", L"892667 - ?f117");
		add(L"f119", L"?f116 - 748094");
		add(L"f120", L"?f118 - 748094");
		add(L"f121", L"?f116 + 748094");
		add(L"f122", L"?f118 + 748094");
		add(L"f123", L"?f103 + ?f1");
		add(L"f124", L"?f123 * ?f8 / ?f0");
		add(L"f125", L"0 - ?f124");
		add(L"f126", L"cos(?f125)");
		add(L"f127", L"0 - ?f126");
		add(L"f128", L"?f127 * 748094");
		add(L"f129", L"sin(?f125)");
		add(L"f130", L"0 - ?f129");
		add(L"f131", L"?f130 * 748094");
		add(L"f132", L"sqrt(?f128 * ?f128 + ?f131 * ?f131 + 0 * 0)");
		add(L"f133", L"748094 * 748094 / ?f132");
		add(L"f134", L"?f130 * ?f133");
		add(L"f135", L"?f116 + ?f134");
		add(L"f136", L"?f127 * ?f133");
		add(L"f137", L"?f118 + ?f136");
		add(L"f138", L"if(?f102, 74093, ?f119)");
		add(L"f139", L"if(?f102, 892667, ?f120)");
		add(L"f140", L"if(?f102, 74093, ?f121)");
		add(L"f141", L"if(?f102, 892667, ?f122)");
		add(L"f142", L"if(?f102, ?f119, ?f135)");
		add(L"f143", L"if(?f102, ?f120, ?f137)");
		add(L"f144", L"if(?f102, ?f121, ?f135)");
		add(L"f145", L"if(?f102, ?f122, ?f137)");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_CircularArrow : public oox_shape
{
public:
	oox_shape_CircularArrow()
	{
		odf_type_name	=L"ooxml-circularArrow";

		enhanced_path	= L"M 832838 101692 A ?f91 ?f92 ?f93 ?f94 832838 101692 ?f88 ?f90  W ?f95 ?f96 ?f97 ?f98 832838 101692 ?f88 ?f90 L 1755675 1929758 1616897 1879250 1618409 1725100 1657499 1783381 A ?f138 ?f139 ?f140 ?f141 1657499 1783381 ?f135 ?f137  W ?f142 ?f143 ?f144 ?f145 1657499 1783381 ?f135 ?f137 Z N";
		text_areas		= L"?f48 ?f50 ?f49 ?f51";
		glue_points		= L"?f40 ?f41 ?f42 ?f43 ?f44 ?f45 ?f46 ?f47";
		view_box		= L"0 0 2165070 2165070";
		
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
		add(L"f10", L"?f7 / 2165070");
		add(L"f11", L"?f6 / 2165070");
		add(L"f12", L"1616897 - 1082535");
		add(L"f13", L"1879250 - 1082535");
		add(L"f14", L"sqrt(?f12 * ?f12 + ?f13 * ?f13 + 0 * 0)");
		add(L"f15", L"atan2(?f12, ?f13)");
		add(L"f16", L"?f15 + ?f1");
		add(L"f17", L"?f16 * ?f9 / ?f0");
		add(L"f18", L"0 - ?f17");
		add(L"f19", L"832838 - 1082535");
		add(L"f20", L"101692 - 1082535");
		add(L"f21", L"sqrt(?f19 * ?f19 + ?f20 * ?f20 + 0 * 0)");
		add(L"f22", L"atan2(?f19, ?f20)");
		add(L"f23", L"?f22 + ?f1");
		add(L"f24", L"?f23 * ?f9 / ?f0");
		add(L"f25", L"0 - ?f24");
		add(L"f26", L"1716572 - 1082535");
		add(L"f27", L"1871457 - 1082535");
		add(L"f28", L"sqrt(?f26 * ?f26 + ?f27 * ?f27 + 0 * 0)");
		add(L"f29", L"atan2(?f26, ?f27)");
		add(L"f30", L"?f29 + ?f1");
		add(L"f31", L"?f30 * ?f9 / ?f0");
		add(L"f32", L"0 - ?f31");
		add(L"f33", L"1618409 - 1082535");
		add(L"f34", L"1725100 - 1082535");
		add(L"f35", L"sqrt(?f33 * ?f33 + ?f34 * ?f34 + 0 * 0)");
		add(L"f36", L"atan2(?f33, ?f34)");
		add(L"f37", L"?f36 + ?f1");
		add(L"f38", L"?f37 * ?f9 / ?f0");
		add(L"f39", L"0 - ?f38");
		add(L"f40", L"845866 / ?f10");
		add(L"f41", L"152866 / ?f11");
		add(L"f42", L"1755675 / ?f10");
		add(L"f43", L"1929758 / ?f11");
		add(L"f44", L"1616897 / ?f10");
		add(L"f45", L"1879250 / ?f11");
		add(L"f46", L"1618409 / ?f10");
		add(L"f47", L"1725100 / ?f11");
		add(L"f48", L"366853 / ?f10");
		add(L"f49", L"1798217 / ?f10");
		add(L"f50", L"366853 / ?f11");
		add(L"f51", L"1798217 / ?f11");
		add(L"f52", L"21550000 - 9329679");
		add(L"f53", L"if(?f52, 9329679, 21550000)");
		add(L"f54", L"-21550000 - ?f53");
		add(L"f55", L"if(?f54, -21550000, ?f53)");
		add(L"f56", L"15343043 + ?f55");
		add(L"f57", L"15343043 + ?f1");
		add(L"f58", L"?f57 * ?f8 / ?f0");
		add(L"f59", L"0 - ?f58");
		add(L"f60", L"cos(?f59)");
		add(L"f61", L"0 - ?f60");
		add(L"f62", L"?f61 * 1012127");
		add(L"f63", L"sin(?f59)");
		add(L"f64", L"0 - ?f63");
		add(L"f65", L"?f64 * 1012127");
		add(L"f66", L"sqrt(?f62 * ?f62 + ?f65 * ?f65 + 0 * 0)");
		add(L"f67", L"1012127 * 1012127 / ?f66");
		add(L"f68", L"?f64 * ?f67");
		add(L"f69", L"832838 - ?f68");
		add(L"f70", L"?f61 * ?f67");
		add(L"f71", L"101692 - ?f70");
		add(L"f72", L"?f69 - 1012127");
		add(L"f73", L"?f71 - 1012127");
		add(L"f74", L"?f69 + 1012127");
		add(L"f75", L"?f71 + 1012127");
		add(L"f76", L"?f56 + ?f1");
		add(L"f77", L"?f76 * ?f8 / ?f0");
		add(L"f78", L"0 - ?f77");
		add(L"f79", L"cos(?f78)");
		add(L"f80", L"0 - ?f79");
		add(L"f81", L"?f80 * 1012127");
		add(L"f82", L"sin(?f78)");
		add(L"f83", L"0 - ?f82");
		add(L"f84", L"?f83 * 1012127");
		add(L"f85", L"sqrt(?f81 * ?f81 + ?f84 * ?f84 + 0 * 0)");
		add(L"f86", L"1012127 * 1012127 / ?f85");
		add(L"f87", L"?f83 * ?f86");
		add(L"f88", L"?f69 + ?f87");
		add(L"f89", L"?f80 * ?f86");
		add(L"f90", L"?f71 + ?f89");
		add(L"f91", L"if(?f55, 832838, ?f72)");
		add(L"f92", L"if(?f55, 101692, ?f73)");
		add(L"f93", L"if(?f55, 832838, ?f74)");
		add(L"f94", L"if(?f55, 101692, ?f75)");
		add(L"f95", L"if(?f55, ?f72, ?f88)");
		add(L"f96", L"if(?f55, ?f73, ?f90)");
		add(L"f97", L"if(?f55, ?f74, ?f88)");
		add(L"f98", L"if(?f55, ?f75, ?f90)");
		add(L"f99", L"21550000 - -9295060");
		add(L"f100", L"if(?f99, -9295060, 21550000)");
		add(L"f101", L"-21550000 - ?f100");
		add(L"f102", L"if(?f101, -21550000, ?f100)");
		add(L"f103", L"3038103 + ?f102");
		add(L"f104", L"3038103 + ?f1");
		add(L"f105", L"?f104 * ?f8 / ?f0");
		add(L"f106", L"0 - ?f105");
		add(L"f107", L"cos(?f106)");
		add(L"f108", L"0 - ?f107");
		add(L"f109", L"?f108 * 906515");
		add(L"f110", L"sin(?f106)");
		add(L"f111", L"0 - ?f110");
		add(L"f112", L"?f111 * 906515");
		add(L"f113", L"sqrt(?f109 * ?f109 + ?f112 * ?f112 + 0 * 0)");
		add(L"f114", L"906515 * 906515 / ?f113");
		add(L"f115", L"?f111 * ?f114");
		add(L"f116", L"1657499 - ?f115");
		add(L"f117", L"?f108 * ?f114");
		add(L"f118", L"1783381 - ?f117");
		add(L"f119", L"?f116 - 906515");
		add(L"f120", L"?f118 - 906515");
		add(L"f121", L"?f116 + 906515");
		add(L"f122", L"?f118 + 906515");
		add(L"f123", L"?f103 + ?f1");
		add(L"f124", L"?f123 * ?f8 / ?f0");
		add(L"f125", L"0 - ?f124");
		add(L"f126", L"cos(?f125)");
		add(L"f127", L"0 - ?f126");
		add(L"f128", L"?f127 * 906515");
		add(L"f129", L"sin(?f125)");
		add(L"f130", L"0 - ?f129");
		add(L"f131", L"?f130 * 906515");
		add(L"f132", L"sqrt(?f128 * ?f128 + ?f131 * ?f131 + 0 * 0)");
		add(L"f133", L"906515 * 906515 / ?f132");
		add(L"f134", L"?f130 * ?f133");
		add(L"f135", L"?f116 + ?f134");
		add(L"f136", L"?f127 * ?f133");
		add(L"f137", L"?f118 + ?f136");
		add(L"f138", L"if(?f102, 1657499, ?f119)");
		add(L"f139", L"if(?f102, 1783381, ?f120)");
		add(L"f140", L"if(?f102, 1657499, ?f121)");
		add(L"f141", L"if(?f102, 1783381, ?f122)");
		add(L"f142", L"if(?f102, ?f119, ?f135)");
		add(L"f143", L"if(?f102, ?f120, ?f137)");
		add(L"f144", L"if(?f102, ?f121, ?f135)");
		add(L"f145", L"if(?f102, ?f122, ?f137)");
/////////////////////////////////////////////////////////
	}
};
}