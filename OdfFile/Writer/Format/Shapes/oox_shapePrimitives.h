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
//+	shapetypeArc,
//+	shapetypeBevel,
//+	shapetypeCan,
//+	shapetypeChevron,
//+	shapetypeChord,
//+	shapetypeCorner,
//	shapetypeCornerTabs,
//+	shapetypeCube,
//+	shapetypeDecagon,
//	shapetypeDiagStripe,
//+	shapetypeDiamond
//+	shapetypeDodecagon,
//+	shapetypeDoubleWave,
//+	shapetypeFunnel,
//+	shapetypeGear6,
//+	shapetypeGear9,
//	shapetypeHalfFrame,
//+	shapetypeHeptagon,
//+	shapetypeHexagon,
//+ shapetypeHomePlate,
//	shapetypeNonIsoscelesTrapezoid,
//+	shapetypeOctagon,
//+	shapetypeParallelogram
//+	shapetypePentagon,
//+	shapetypePie,
//+	shapetypePieWedge,
//+	shapetypePlaque,
//	shapetypePlaqueTabs,
//	shapetypeSquareTabs,
//	shapetypeTeardrop
//+ shapetypeTriangle
//+	shapetypeTrapezoid
//+ shapetypeBlockArc
//+ shapetypeDonut
//+ shapetypeFrame

#include "../oox_shape_defines.h"

namespace cpdoccore 
{
class oox_shape_Plaque : public oox_shape
{
public:
	oox_shape_Plaque()
	{
		odf_type_name	=L"ooxml-Plaque";

		modifiers		= L"16667";
		enhanced_path	= L"M ?f3 ?f12 A ?f57 ?f58 ?f59 ?f60 ?f3 ?f12 ?f54 ?f56  W ?f61 ?f62 ?f63 ?f64 ?f3 ?f12 ?f54 ?f56 L ?f13 ?f5 A ?f100 ?f101 ?f102 ?f103 ?f13 ?f5 ?f97 ?f99  W ?f104 ?f105 ?f106 ?f107 ?f13 ?f5 ?f97 ?f99 L ?f4 ?f14 A ?f143 ?f144 ?f145 ?f146 ?f4 ?f14 ?f140 ?f142  W ?f147 ?f148 ?f149 ?f150 ?f4 ?f14 ?f140 ?f142 L ?f12 ?f6 A ?f186 ?f187 ?f188 ?f189 ?f12 ?f6 ?f183 ?f185  W ?f190 ?f191 ?f192 ?f193 ?f12 ?f6 ?f183 ?f185 Z N";
		text_areas		= L"?f15 ?f15 ?f16 ?f17";
		glue_points		= L"?f36 ?f37 ?f56 ?f37 ?f56 ?f57 ?f36 ?f57";
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
		add(L"f14", L"?f6 - ?f12");
		add(L"f15", L"?f12 * 70711 / 100000");
		add(L"f16", L"?f4 - ?f15");
		add(L"f17", L"?f6 - ?f15");
		add(L"f18", L"21550000 - -5400000");
		add(L"f19", L"if(?f18, -5400000, 21550000)");
		add(L"f20", L"-21550000 - ?f19");
		add(L"f21", L"if(?f20, -21550000, ?f19)");
		add(L"f22", L"?f2 + ?f21");
		add(L"f23", L"?f2 + ?f2");
		add(L"f24", L"?f23 * ?f10 / ?f1");
		add(L"f25", L"0 - ?f24");
		add(L"f26", L"cos(?f25)");
		add(L"f27", L"0 - ?f26");
		add(L"f28", L"?f27 * ?f12");
		add(L"f29", L"sin(?f25)");
		add(L"f30", L"0 - ?f29");
		add(L"f31", L"?f30 * ?f12");
		add(L"f32", L"sqrt(?f28 * ?f28 + ?f31 * ?f31 + 0 * 0)");
		add(L"f33", L"?f12 * ?f12 / ?f32");
		add(L"f34", L"?f30 * ?f33");
		add(L"f35", L"?f3 - ?f34");
		add(L"f36", L"?f27 * ?f33");
		add(L"f37", L"?f12 - ?f36");
		add(L"f38", L"?f35 - ?f12");
		add(L"f39", L"?f37 - ?f12");
		add(L"f40", L"?f35 + ?f12");
		add(L"f41", L"?f37 + ?f12");
		add(L"f42", L"?f22 + ?f2");
		add(L"f43", L"?f42 * ?f10 / ?f1");
		add(L"f44", L"0 - ?f43");
		add(L"f45", L"cos(?f44)");
		add(L"f46", L"0 - ?f45");
		add(L"f47", L"?f46 * ?f12");
		add(L"f48", L"sin(?f44)");
		add(L"f49", L"0 - ?f48");
		add(L"f50", L"?f49 * ?f12");
		add(L"f51", L"sqrt(?f47 * ?f47 + ?f50 * ?f50 + 0 * 0)");
		add(L"f52", L"?f12 * ?f12 / ?f51");
		add(L"f53", L"?f49 * ?f52");
		add(L"f54", L"?f35 + ?f53");
		add(L"f55", L"?f46 * ?f52");
		add(L"f56", L"?f37 + ?f55");
		add(L"f57", L"if(?f21, ?f3, ?f38)");
		add(L"f58", L"if(?f21, ?f12, ?f39)");
		add(L"f59", L"if(?f21, ?f3, ?f40)");
		add(L"f60", L"if(?f21, ?f12, ?f41)");
		add(L"f61", L"if(?f21, ?f38, ?f54)");
		add(L"f62", L"if(?f21, ?f39, ?f56)");
		add(L"f63", L"if(?f21, ?f40, ?f54)");
		add(L"f64", L"if(?f21, ?f41, ?f56)");
		add(L"f65", L"?f1 + ?f21");
		add(L"f66", L"?f1 + ?f2");
		add(L"f67", L"?f66 * ?f10 / ?f1");
		add(L"f68", L"0 - ?f67");
		add(L"f69", L"cos(?f68)");
		add(L"f70", L"0 - ?f69");
		add(L"f71", L"?f70 * ?f12");
		add(L"f72", L"sin(?f68)");
		add(L"f73", L"0 - ?f72");
		add(L"f74", L"?f73 * ?f12");
		add(L"f75", L"sqrt(?f71 * ?f71 + ?f74 * ?f74 + 0 * 0)");
		add(L"f76", L"?f12 * ?f12 / ?f75");
		add(L"f77", L"?f73 * ?f76");
		add(L"f78", L"?f13 - ?f77");
		add(L"f79", L"?f70 * ?f76");
		add(L"f80", L"?f5 - ?f79");
		add(L"f81", L"?f78 - ?f12");
		add(L"f82", L"?f80 - ?f12");
		add(L"f83", L"?f78 + ?f12");
		add(L"f84", L"?f80 + ?f12");
		add(L"f85", L"?f65 + ?f2");
		add(L"f86", L"?f85 * ?f10 / ?f1");
		add(L"f87", L"0 - ?f86");
		add(L"f88", L"cos(?f87)");
		add(L"f89", L"0 - ?f88");
		add(L"f90", L"?f89 * ?f12");
		add(L"f91", L"sin(?f87)");
		add(L"f92", L"0 - ?f91");
		add(L"f93", L"?f92 * ?f12");
		add(L"f94", L"sqrt(?f90 * ?f90 + ?f93 * ?f93 + 0 * 0)");
		add(L"f95", L"?f12 * ?f12 / ?f94");
		add(L"f96", L"?f92 * ?f95");
		add(L"f97", L"?f78 + ?f96");
		add(L"f98", L"?f89 * ?f95");
		add(L"f99", L"?f80 + ?f98");
		add(L"f100", L"if(?f21, ?f13, ?f81)");
		add(L"f101", L"if(?f21, ?f5, ?f82)");
		add(L"f102", L"if(?f21, ?f13, ?f83)");
		add(L"f103", L"if(?f21, ?f5, ?f84)");
		add(L"f104", L"if(?f21, ?f81, ?f97)");
		add(L"f105", L"if(?f21, ?f82, ?f99)");
		add(L"f106", L"if(?f21, ?f83, ?f97)");
		add(L"f107", L"if(?f21, ?f84, ?f99)");
		add(L"f108", L"?f0 + ?f21");
		add(L"f109", L"?f0 + ?f2");
		add(L"f110", L"?f109 * ?f10 / ?f1");
		add(L"f111", L"0 - ?f110");
		add(L"f112", L"cos(?f111)");
		add(L"f113", L"0 - ?f112");
		add(L"f114", L"?f113 * ?f12");
		add(L"f115", L"sin(?f111)");
		add(L"f116", L"0 - ?f115");
		add(L"f117", L"?f116 * ?f12");
		add(L"f118", L"sqrt(?f114 * ?f114 + ?f117 * ?f117 + 0 * 0)");
		add(L"f119", L"?f12 * ?f12 / ?f118");
		add(L"f120", L"?f116 * ?f119");
		add(L"f121", L"?f4 - ?f120");
		add(L"f122", L"?f113 * ?f119");
		add(L"f123", L"?f14 - ?f122");
		add(L"f124", L"?f121 - ?f12");
		add(L"f125", L"?f123 - ?f12");
		add(L"f126", L"?f121 + ?f12");
		add(L"f127", L"?f123 + ?f12");
		add(L"f128", L"?f108 + ?f2");
		add(L"f129", L"?f128 * ?f10 / ?f1");
		add(L"f130", L"0 - ?f129");
		add(L"f131", L"cos(?f130)");
		add(L"f132", L"0 - ?f131");
		add(L"f133", L"?f132 * ?f12");
		add(L"f134", L"sin(?f130)");
		add(L"f135", L"0 - ?f134");
		add(L"f136", L"?f135 * ?f12");
		add(L"f137", L"sqrt(?f133 * ?f133 + ?f136 * ?f136 + 0 * 0)");
		add(L"f138", L"?f12 * ?f12 / ?f137");
		add(L"f139", L"?f135 * ?f138");
		add(L"f140", L"?f121 + ?f139");
		add(L"f141", L"?f132 * ?f138");
		add(L"f142", L"?f123 + ?f141");
		add(L"f143", L"if(?f21, ?f4, ?f124)");
		add(L"f144", L"if(?f21, ?f14, ?f125)");
		add(L"f145", L"if(?f21, ?f4, ?f126)");
		add(L"f146", L"if(?f21, ?f14, ?f127)");
		add(L"f147", L"if(?f21, ?f124, ?f140)");
		add(L"f148", L"if(?f21, ?f125, ?f142)");
		add(L"f149", L"if(?f21, ?f126, ?f140)");
		add(L"f150", L"if(?f21, ?f127, ?f142)");
		add(L"f151", L"0 + ?f21");
		add(L"f152", L"0 + ?f2");
		add(L"f153", L"?f152 * ?f10 / ?f1");
		add(L"f154", L"0 - ?f153");
		add(L"f155", L"cos(?f154)");
		add(L"f156", L"0 - ?f155");
		add(L"f157", L"?f156 * ?f12");
		add(L"f158", L"sin(?f154)");
		add(L"f159", L"0 - ?f158");
		add(L"f160", L"?f159 * ?f12");
		add(L"f161", L"sqrt(?f157 * ?f157 + ?f160 * ?f160 + 0 * 0)");
		add(L"f162", L"?f12 * ?f12 / ?f161");
		add(L"f163", L"?f159 * ?f162");
		add(L"f164", L"?f12 - ?f163");
		add(L"f165", L"?f156 * ?f162");
		add(L"f166", L"?f6 - ?f165");
		add(L"f167", L"?f164 - ?f12");
		add(L"f168", L"?f166 - ?f12");
		add(L"f169", L"?f164 + ?f12");
		add(L"f170", L"?f166 + ?f12");
		add(L"f171", L"?f151 + ?f2");
		add(L"f172", L"?f171 * ?f10 / ?f1");
		add(L"f173", L"0 - ?f172");
		add(L"f174", L"cos(?f173)");
		add(L"f175", L"0 - ?f174");
		add(L"f176", L"?f175 * ?f12");
		add(L"f177", L"sin(?f173)");
		add(L"f178", L"0 - ?f177");
		add(L"f179", L"?f178 * ?f12");
		add(L"f180", L"sqrt(?f176 * ?f176 + ?f179 * ?f179 + 0 * 0)");
		add(L"f181", L"?f12 * ?f12 / ?f180");
		add(L"f182", L"?f178 * ?f181");
		add(L"f183", L"?f164 + ?f182");
		add(L"f184", L"?f175 * ?f181");
		add(L"f185", L"?f166 + ?f184");
		add(L"f186", L"if(?f21, ?f12, ?f167)");
		add(L"f187", L"if(?f21, ?f6, ?f168)");
		add(L"f188", L"if(?f21, ?f12, ?f169)");
		add(L"f189", L"if(?f21, ?f6, ?f170)");
		add(L"f190", L"if(?f21, ?f167, ?f183)");
		add(L"f191", L"if(?f21, ?f168, ?f185)");
		add(L"f192", L"if(?f21, ?f169, ?f183)");
		add(L"f193", L"if(?f21, ?f170, ?f185)");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Wave : public oox_shape
{
public:
	oox_shape_Wave()
	{
		odf_type_name	=L"ooxml-wave";

		modifiers		= L"12500 0";
		enhanced_path	= L"M ?f23 ?f12 C ?f27 ?f14 ?f28 ?f15 ?f25 ?f12 L ?f30 ?f16 C ?f32 ?f18 ?f31 ?f17 ?f29 ?f16 Z N";
		text_areas		= L"?f36 ?f38 ?f37 ?f39";
		glue_points		= L"?f35 ?f12 ?f21 ?f6 ?f34 ?f16 ?f33 ?f6";
		view_box		= L"0 0 21600 21600";
		
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
		add(L"f11", L"$1");
		add(L"f12", L"?f4 * ?f10 / 100000");
		add(L"f13", L"?f12 * 10 / 3");
		add(L"f14", L"?f12 - ?f13");
		add(L"f15", L"?f12 + ?f13");
		add(L"f16", L"?f3 - ?f12");
		add(L"f17", L"?f16 - ?f13");
		add(L"f18", L"?f16 + ?f13");
		add(L"f19", L"?f7 * ?f11 / 100000");
		add(L"f20", L"?f7 * ?f11 / 50000");
		add(L"f21", L"abs(?f19)");
		add(L"f22", L"if(?f20, 0, ?f20)");
		add(L"f23", L"?f0 - ?f22");
		add(L"f24", L"if(?f20, ?f20, 0)");
		add(L"f25", L"?f1 - ?f24");
		add(L"f26", L"(?f22 + ?f25) / 3");
		add(L"f27", L"?f23 + ?f26");
		add(L"f28", L"(?f27 + ?f25) / 2");
		add(L"f29", L"?f0 + ?f24");
		add(L"f30", L"?f1 + ?f22");
		add(L"f31", L"?f29 + ?f26");
		add(L"f32", L"(?f31 + ?f30) / 2");
		add(L"f33", L"?f1 - ?f21");
		add(L"f34", L"?f9 + ?f19");
		add(L"f35", L"?f9 - ?f19");
		add(L"f36", L"max(?f23, ?f29)");
		add(L"f37", L"min(?f25, ?f30)");
		add(L"f38", L"?f4 * ?f10 / 50000");
		add(L"f39", L"?f3 - ?f38");
/////////////////////////////////////////////////////////	
	}
};
class oox_shape_DoubleWave : public oox_shape
{
public:
	oox_shape_DoubleWave()
	{
		odf_type_name	=L"ooxml-doubleWave";

		modifiers		= L"6500 0";
		enhanced_path	= L"M ?f21 ?f10 C ?f25 ?f12 ?f27 ?f13 ?f28 ?f10 ?f29 ?f12 ?f30 ?f13 ?f23 ?f10 L ?f32 ?f14 C ?f37 ?f16 ?f36 ?f15 ?f35 ?f14 ?f34 ?f16 ?f33 ?f15 ?f31 ?f14 Z N";
		text_areas		= L"?f39 ?f41 ?f40 ?f42";
		glue_points		= L"?f35 ?f10 ?f19 ?f6 ?f28 ?f14 ?f38 ?f6";
		view_box		= L"0 0 21600 21600";
		
        add(L"f0", L"left");
        add(L"f1", L"right");
        add(L"f2", L"top");
        add(L"f3", L"bottom");
        add(L"f4", L"?f3 - ?f2");
        add(L"f5", L"?f4 / 2");
        add(L"f6", L"?f2 + ?f5");
        add(L"f7", L"?f1 - ?f0");
        add(L"f8", L"$0");
        add(L"f9", L"$1");
        add(L"f10", L"?f4 * ?f8 / 100000");
        add(L"f11", L"?f10 * 10 / 3");
        add(L"f12", L"?f10 - ?f11");
        add(L"f13", L"?f10 + ?f11");
        add(L"f14", L"?f3 - ?f10");
        add(L"f15", L"?f14 - ?f11");
        add(L"f16", L"?f14 + ?f11");
        add(L"f17", L"?f7 * ?f9 / 100000");
        add(L"f18", L"?f7 * ?f9 / 50000");
        add(L"f19", L"abs(?f17)");
        add(L"f20", L"if(?f18, 0, ?f18)");
        add(L"f21", L"?f0 - ?f20");
        add(L"f22", L"if(?f18, ?f18, 0)");
        add(L"f23", L"?f1 - ?f22");
        add(L"f24", L"(?f20 + ?f23) / 6");
        add(L"f25", L"?f21 + ?f24");
        add(L"f26", L"(?f20 + ?f23) / 3");
        add(L"f27", L"?f21 + ?f26");
        add(L"f28", L"(?f21 + ?f23) / 2");
        add(L"f29", L"?f28 + ?f24");
        add(L"f30", L"(?f29 + ?f23) / 2");
        add(L"f31", L"?f0 + ?f22");
        add(L"f32", L"?f1 + ?f20");
        add(L"f33", L"?f31 + ?f24");
        add(L"f34", L"?f31 + ?f26");
        add(L"f35", L"(?f31 + ?f32) / 2");
        add(L"f36", L"?f35 + ?f24");
        add(L"f37", L"(?f36 + ?f32) / 2");
        add(L"f38", L"?f1 - ?f19");
        add(L"f39", L"max(?f21, ?f31)");
        add(L"f40", L"min(?f23, ?f32)");
        add(L"f41", L"?f4 * ?f8 / 50000");
        add(L"f42", L"?f3 - ?f41");
/////////////////////////////////////////////////////////
	}
};

class oox_shape_Gear6 : public oox_shape
{
public:
	oox_shape_Gear6()
	{
		odf_type_name	=L"ooxml-gear6";

		enhanced_path	= L"M 541172 212681 L 631103 156982 683808 209688 628111 299619 C 649564 336513 660802 378456 660671 421135 L 753871 471167 734580 543164 628849 539893 C 607623 576919 576919 607624 539893 628849 L 543164 734581 471167 753872 421134 660670 C 378456 660802 336513 649563 299618 628110 L 209688 683809 156983 631103 212680 541172 C 191227 504278 179989 462335 180120 419656 L 86920 369624 106211 297627 211942 300898 C 233168 263872 263872 233167 300898 211942 L 297627 106210 369624 86919 419657 180121 C 462335 179989 504278 191228 541173 212681 L 541172 212681 Z N";
		text_areas		= L"?f68 ?f70 ?f69 ?f71";
		glue_points		= L"?f38 ?f39 ?f40 ?f41 ?f42 ?f43 ?f44 ?f45 ?f44 ?f46 ?f42 ?f47 ?f40 ?f48 ?f38 ?f49 ?f50 ?f51 ?f52 ?f53 ?f54 ?f53 ?f55 ?f51 ?f56 ?f49 ?f57 ?f48 ?f58 ?f47 ?f59 ?f60 ?f59 ?f61 ?f58 ?f43 ?f57 ?f41 ?f62 ?f39 ?f63 ?f64 ?f54 ?f65 ?f52 ?f65 ?f66 ?f64 ?f67 ?f39 ?f38 ?f39";
		view_box		= L"0 0 840791 840791";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"?f5 / 840791");
		add(L"f7", L"?f4 / 840791");
		add(L"f8", L"629119 * ?f5 / 840791");
		add(L"f9", L"212951 * ?f4 / 840791");
		add(L"f10", L"753164 * ?f5 / 840791");
		add(L"f11", L"175566 * ?f4 / 840791");
		add(L"f12", L"798808 * ?f5 / 840791");
		add(L"f13", L"254624 * ?f4 / 840791");
		add(L"f14", L"704410 * ?f5 / 840791");
		add(L"f15", L"343358 * ?f4 / 840791");
		add(L"f16", L"497434 * ?f4 / 840791");
		add(L"f17", L"586167 * ?f4 / 840791");
		add(L"f18", L"665225 * ?f4 / 840791");
		add(L"f19", L"627840 * ?f4 / 840791");
		add(L"f20", L"495685 * ?f5 / 840791");
		add(L"f21", L"704878 * ?f4 / 840791");
		add(L"f22", L"466040 * ?f5 / 840791");
		add(L"f23", L"830997 * ?f4 / 840791");
		add(L"f24", L"374751 * ?f5 / 840791");
		add(L"f25", L"345105 * ?f5 / 840791");
		add(L"f26", L"211671 * ?f5 / 840791");
		add(L"f27", L"87627 * ?f5 / 840791");
		add(L"f28", L"41983 * ?f5 / 840791");
		add(L"f29", L"136381 * ?f5 / 840791");
		add(L"f30", L"497433 * ?f4 / 840791");
		add(L"f31", L"343357 * ?f4 / 840791");
		add(L"f32", L"211672 * ?f5 / 840791");
		add(L"f33", L"345106 * ?f5 / 840791");
		add(L"f34", L"135913 * ?f4 / 840791");
		add(L"f35", L"9794 * ?f4 / 840791");
		add(L"f36", L"495686 * ?f5 / 840791");
		add(L"f37", L"629120 * ?f5 / 840791");
		add(L"f38", L"?f8 / ?f6");
		add(L"f39", L"?f9 / ?f7");
		add(L"f40", L"?f10 / ?f6");
		add(L"f41", L"?f11 / ?f7");
		add(L"f42", L"?f12 / ?f6");
		add(L"f43", L"?f13 / ?f7");
		add(L"f44", L"?f14 / ?f6");
		add(L"f45", L"?f15 / ?f7");
		add(L"f46", L"?f16 / ?f7");
		add(L"f47", L"?f17 / ?f7");
		add(L"f48", L"?f18 / ?f7");
		add(L"f49", L"?f19 / ?f7");
		add(L"f50", L"?f20 / ?f6");
		add(L"f51", L"?f21 / ?f7");
		add(L"f52", L"?f22 / ?f6");
		add(L"f53", L"?f23 / ?f7");
		add(L"f54", L"?f24 / ?f6");
		add(L"f55", L"?f25 / ?f6");
		add(L"f56", L"?f26 / ?f6");
		add(L"f57", L"?f27 / ?f6");
		add(L"f58", L"?f28 / ?f6");
		add(L"f59", L"?f29 / ?f6");
		add(L"f60", L"?f30 / ?f7");
		add(L"f61", L"?f31 / ?f7");
		add(L"f62", L"?f32 / ?f6");
		add(L"f63", L"?f33 / ?f6");
		add(L"f64", L"?f34 / ?f7");
		add(L"f65", L"?f35 / ?f7");
		add(L"f66", L"?f36 / ?f6");
		add(L"f67", L"?f37 / ?f6");
		add(L"f68", L"?f0 / ?f6");
		add(L"f69", L"?f1 / ?f6");
		add(L"f70", L"?f2 / ?f7");
		add(L"f71", L"?f3 / ?f7");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Gear9 : public oox_shape
{
public:
	oox_shape_Gear9()
	{
		odf_type_name	=L"ooxml-gear9";

		enhanced_path	= L"M 837517 188126 L 929297 111109 1002618 172633 942709 276392 C 985308 324313 1017696 380410 1037897 441262 L 1157709 441259 1174330 535519 1061742 576495 C 1063572 640586 1052324 704378 1028684 763978 L 1120467 840989 1072610 923880 960025 882899 C 920230 933172 870608 974809 814189 1005270 L 834997 1123262 745055 1155998 685151 1052236 C 622351 1065167 557575 1065167 494775 1052236 L 434872 1155998 344930 1123262 365739 1005270 C 309319 974809 259698 933172 219903 882899 L 107317 923880 59460 840989 151243 763978 C 127603 704378 116355 640586 118185 576495 L 5597 535519 22218 441259 142030 441262 C 162231 380410 194619 324312 237218 276392 L 177309 172633 250630 111109 342410 188126 C 397000 154496 457869 132341 521305 123014 L 542106 5021 637821 5021 658623 123013 C 722058 132340 782928 154495 837518 188125 L 837517 188126 Z N";
		text_areas		= L"?f122 ?f124 ?f123 ?f125";
		glue_points		= L"?f65 ?f66 ?f67 ?f68 ?f69 ?f70 ?f71 ?f72 ?f73 ?f74 ?f75 ?f76 ?f77 ?f78 ?f79 ?f80 ?f81 ?f82 ?f83 ?f84 ?f85 ?f86 ?f87 ?f88 ?f89 ?f90 ?f91 ?f92 ?f93 ?f94 ?f95 ?f96 ?f97 ?f96 ?f98 ?f94 ?f99 ?f92 ?f100 ?f90 ?f101 ?f88 ?f102 ?f86 ?f103 ?f84 ?f104 ?f82 ?f105 ?f80 ?f106 ?f78 ?f107 ?f76 ?f108 ?f74 ?f109 ?f72 ?f110 ?f70 ?f111 ?f68 ?f112 ?f66 ?f113 ?f114 ?f115 ?f116 ?f117 ?f116 ?f118 ?f119 ?f120 ?f121 ?f65 ?f66";
		view_box		= L"0 0 1179927 1179927";
		
		add(L"f0", L"left");
 		add(L"f1", L"right");
 		add(L"f2", L"top");
 		add(L"f3", L"bottom");
 		add(L"f4", L"?f3 - ?f2");
 		add(L"f5", L"?f1 - ?f0");
 		add(L"f6", L"?f5 / 1179927");
 		add(L"f7", L"?f4 / 1179927");
 		add(L"f8", L"837517 * ?f5 / 1179927");
 		add(L"f9", L"188126 * ?f4 / 1179927");
 		add(L"f10", L"929297 * ?f5 / 1179927");
 		add(L"f11", L"111109 * ?f4 / 1179927");
 		add(L"f12", L"1002618 * ?f5 / 1179927");
 		add(L"f13", L"172633 * ?f4 / 1179927");
 		add(L"f14", L"942709 * ?f5 / 1179927");
 		add(L"f15", L"276392 * ?f4 / 1179927");
 		add(L"f16", L"1037897 * ?f5 / 1179927");
 		add(L"f17", L"441262 * ?f4 / 1179927");
 		add(L"f18", L"1157709 * ?f5 / 1179927");
 		add(L"f19", L"441259 * ?f4 / 1179927");
 		add(L"f20", L"1174330 * ?f5 / 1179927");
 		add(L"f21", L"535519 * ?f4 / 1179927");
 		add(L"f22", L"1061742 * ?f5 / 1179927");
 		add(L"f23", L"576495 * ?f4 / 1179927");
 		add(L"f24", L"1028684 * ?f5 / 1179927");
 		add(L"f25", L"763978 * ?f4 / 1179927");
 		add(L"f26", L"1120467 * ?f5 / 1179927");
 		add(L"f27", L"840989 * ?f4 / 1179927");
 		add(L"f28", L"1072610 * ?f5 / 1179927");
 		add(L"f29", L"923880 * ?f4 / 1179927");
 		add(L"f30", L"960025 * ?f5 / 1179927");
 		add(L"f31", L"882899 * ?f4 / 1179927");
 		add(L"f32", L"814189 * ?f5 / 1179927");
 		add(L"f33", L"1005270 * ?f4 / 1179927");
 		add(L"f34", L"834997 * ?f5 / 1179927");
 		add(L"f35", L"1123262 * ?f4 / 1179927");
 		add(L"f36", L"745055 * ?f5 / 1179927");
 		add(L"f37", L"1155998 * ?f4 / 1179927");
 		add(L"f38", L"685151 * ?f5 / 1179927");
 		add(L"f39", L"1052236 * ?f4 / 1179927");
 		add(L"f40", L"494775 * ?f5 / 1179927");
 		add(L"f41", L"434872 * ?f5 / 1179927");
 		add(L"f42", L"344930 * ?f5 / 1179927");
 		add(L"f43", L"365739 * ?f5 / 1179927");
 		add(L"f44", L"219903 * ?f5 / 1179927");
 		add(L"f45", L"107317 * ?f5 / 1179927");
 		add(L"f46", L"59460 * ?f5 / 1179927");
 		add(L"f47", L"151243 * ?f5 / 1179927");
 		add(L"f48", L"118185 * ?f5 / 1179927");
 		add(L"f49", L"5597 * ?f5 / 1179927");
 		add(L"f50", L"22218 * ?f5 / 1179927");
 		add(L"f51", L"142030 * ?f5 / 1179927");
 		add(L"f52", L"237218 * ?f5 / 1179927");
 		add(L"f53", L"177309 * ?f5 / 1179927");
 		add(L"f54", L"250630 * ?f5 / 1179927");
 		add(L"f55", L"342410 * ?f5 / 1179927");
 		add(L"f56", L"521305 * ?f5 / 1179927");
 		add(L"f57", L"123014 * ?f4 / 1179927");
 		add(L"f58", L"542106 * ?f5 / 1179927");
 		add(L"f59", L"5021 * ?f4 / 1179927");
 		add(L"f60", L"637821 * ?f5 / 1179927");
 		add(L"f61", L"658623 * ?f5 / 1179927");
 		add(L"f62", L"123013 * ?f4 / 1179927");
 		add(L"f63", L"837518 * ?f5 / 1179927");
 		add(L"f64", L"188125 * ?f4 / 1179927");
 		add(L"f65", L"?f8 / ?f6");
 		add(L"f66", L"?f9 / ?f7");
 		add(L"f67", L"?f10 / ?f6");
 		add(L"f68", L"?f11 / ?f7");
 		add(L"f69", L"?f12 / ?f6");
 		add(L"f70", L"?f13 / ?f7");
 		add(L"f71", L"?f14 / ?f6");
 		add(L"f72", L"?f15 / ?f7");
 		add(L"f73", L"?f16 / ?f6");
 		add(L"f74", L"?f17 / ?f7");
 		add(L"f75", L"?f18 / ?f6");
 		add(L"f76", L"?f19 / ?f7");
 		add(L"f77", L"?f20 / ?f6");
 		add(L"f78", L"?f21 / ?f7");
 		add(L"f79", L"?f22 / ?f6");
 		add(L"f80", L"?f23 / ?f7");
 		add(L"f81", L"?f24 / ?f6");
 		add(L"f82", L"?f25 / ?f7");
 		add(L"f83", L"?f26 / ?f6");
 		add(L"f84", L"?f27 / ?f7");
 		add(L"f85", L"?f28 / ?f6");
 		add(L"f86", L"?f29 / ?f7");
 		add(L"f87", L"?f30 / ?f6");
 		add(L"f88", L"?f31 / ?f7");
 		add(L"f89", L"?f32 / ?f6");
 		add(L"f90", L"?f33 / ?f7");
 		add(L"f91", L"?f34 / ?f6");
 		add(L"f92", L"?f35 / ?f7");
 		add(L"f93", L"?f36 / ?f6");
 		add(L"f94", L"?f37 / ?f7");
 		add(L"f95", L"?f38 / ?f6");
 		add(L"f96", L"?f39 / ?f7");
 		add(L"f97", L"?f40 / ?f6");
 		add(L"f98", L"?f41 / ?f6");
 		add(L"f99", L"?f42 / ?f6");
 		add(L"f100", L"?f43 / ?f6");
 		add(L"f101", L"?f44 / ?f6");
 		add(L"f102", L"?f45 / ?f6");
 		add(L"f103", L"?f46 / ?f6");
 		add(L"f104", L"?f47 / ?f6");
 		add(L"f105", L"?f48 / ?f6");
 		add(L"f106", L"?f49 / ?f6");
 		add(L"f107", L"?f50 / ?f6");
 		add(L"f108", L"?f51 / ?f6");
 		add(L"f109", L"?f52 / ?f6");
 		add(L"f110", L"?f53 / ?f6");
 		add(L"f111", L"?f54 / ?f6");
 		add(L"f112", L"?f55 / ?f6");
 		add(L"f113", L"?f56 / ?f6");
 		add(L"f114", L"?f57 / ?f7");
 		add(L"f115", L"?f58 / ?f6");
 		add(L"f116", L"?f59 / ?f7");
 		add(L"f117", L"?f60 / ?f6");
 		add(L"f118", L"?f61 / ?f6");
 		add(L"f119", L"?f62 / ?f7");
 		add(L"f120", L"?f63 / ?f6");
 		add(L"f121", L"?f64 / ?f7");
 		add(L"f122", L"?f0 / ?f6");
 		add(L"f123", L"?f1 / ?f6");
 		add(L"f124", L"?f2 / ?f7");
 		add(L"f125", L"?f3 / ?f7");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Funnel : public oox_shape
{
public:
	oox_shape_Funnel()
	{
		odf_type_name	=L"ooxml-funnel";

		enhanced_path	= L"M ?f50 ?f51 A ?f99 ?f100 ?f101 ?f102 ?f50 ?f51 ?f96 ?f98  W ?f103 ?f104 ?f105 ?f106 ?f50 ?f51 ?f96 ?f98 L ?f65 ?f67 A ?f137 ?f138 ?f139 ?f140 ?f65 ?f67 ?f134 ?f136  W ?f141 ?f142 ?f143 ?f144 ?f65 ?f67 ?f134 ?f136 Z M ?f68 ?f7 A ?f184 ?f185 ?f186 ?f187 ?f68 ?f7 ?f181 ?f183  W ?f188 ?f189 ?f190 ?f191 ?f68 ?f7 ?f181 ?f183 Z N";
		text_areas		= L"?f2 ?f4 ?f3 ?f5";
		view_box		= L"0 0 21600 21600";
		
 		add(L"f0", L"10800000");
 		add(L"f1", L"5400000");
 		add(L"f2", L"left");
 		add(L"f3", L"right");
 		add(L"f4", L"top");
 		add(L"f5", L"bottom");
 		add(L"f6", L"?f5 - ?f4");
 		add(L"f7", L"?f6 / 4");
 		add(L"f8", L"?f3 - ?f2");
 		add(L"f9", L"?f8 / 2");
 		add(L"f10", L"?f2 + ?f9");
 		add(L"f11", L"min(?f8, ?f6)");
 		add(L"f12", L"5419351 / 1725033");
 		add(L"f13", L"?f11 / 20");
 		add(L"f14", L"?f9 - ?f13");
 		add(L"f15", L"?f7 - ?f13");
 		add(L"f16", L"480000 + ?f1");
 		add(L"f17", L"?f16 * ?f12 / ?f0");
 		add(L"f18", L"0 - ?f17");
 		add(L"f19", L"sin(?f18)");
 		add(L"f20", L"0 - ?f19");
 		add(L"f21", L"?f20 * ?f9");
 		add(L"f22", L"cos(?f18)");
 		add(L"f23", L"0 - ?f22");
 		add(L"f24", L"?f23 * ?f7");
 		add(L"f25", L"0 - ?f21");
 		add(L"f26", L"0 - ?f24");
 		add(L"f27", L"atan2(?f25, ?f26)");
 		add(L"f28", L"0 - ?f27");
 		add(L"f29", L"?f28 * ?f0 / ?f12");
 		add(L"f30", L"?f29 - ?f1");
 		add(L"f31", L"?f30 * 2");
 		add(L"f32", L"?f0 - ?f30");
 		add(L"f33", L"?f0 + ?f31");
 		add(L"f34", L"?f0 - ?f31");
 		add(L"f35", L"?f9 / 4");
 		add(L"f36", L"?f7 / 4");
 		add(L"f37", L"?f32 + ?f1");
 		add(L"f38", L"?f37 * ?f12 / ?f0");
 		add(L"f39", L"0 - ?f38");
 		add(L"f40", L"sin(?f39)");
 		add(L"f41", L"0 - ?f40");
 		add(L"f42", L"?f41 * ?f7");
 		add(L"f43", L"cos(?f39)");
 		add(L"f44", L"0 - ?f43");
 		add(L"f45", L"?f44 * ?f9");
 		add(L"f46", L"sqrt(?f42 * ?f42 + ?f45 * ?f45 + 0 * 0)");
 		add(L"f47", L"?f9 * ?f7 / ?f46");
 		add(L"f48", L"?f41 * ?f47");
 		add(L"f49", L"?f44 * ?f47");
 		add(L"f50", L"?f10 + ?f48");
 		add(L"f51", L"?f7 + ?f49");
 		add(L"f52", L"?f30 + ?f1");
 		add(L"f53", L"?f52 * ?f12 / ?f0");
 		add(L"f54", L"0 - ?f53");
 		add(L"f55", L"sin(?f54)");
 		add(L"f56", L"0 - ?f55");
 		add(L"f57", L"?f56 * ?f36");
 		add(L"f58", L"cos(?f54)");
 		add(L"f59", L"0 - ?f58");
 		add(L"f60", L"?f59 * ?f35");
 		add(L"f61", L"sqrt(?f57 * ?f57 + ?f60 * ?f60 + 0 * 0)");
 		add(L"f62", L"?f35 * ?f36 / ?f61");
 		add(L"f63", L"?f56 * ?f62");
 		add(L"f64", L"?f59 * ?f62");
 		add(L"f65", L"?f10 + ?f63");
 		add(L"f66", L"?f5 - ?f36");
 		add(L"f67", L"?f66 + ?f64");
 		add(L"f68", L"?f9 - ?f14");
 		add(L"f69", L"21550000 - ?f33");
 		add(L"f70", L"if(?f69, ?f33, 21550000)");
 		add(L"f71", L"-21550000 - ?f70");
 		add(L"f72", L"if(?f71, -21550000, ?f70)");
 		add(L"f73", L"?f32 + ?f72");
 		add(L"f74", L"sqrt(?f45 * ?f45 + ?f42 * ?f42 + 0 * 0)");
 		add(L"f75", L"?f9 * ?f7 / ?f74");
 		add(L"f76", L"?f41 * ?f75");
 		add(L"f77", L"?f50 - ?f76");
 		add(L"f78", L"?f44 * ?f75");
 		add(L"f79", L"?f51 - ?f78");
 		add(L"f80", L"?f77 - ?f9");
 		add(L"f81", L"?f79 - ?f7");
 		add(L"f82", L"?f77 + ?f9");
 		add(L"f83", L"?f79 + ?f7");
 		add(L"f84", L"?f73 + ?f1");
 		add(L"f85", L"?f84 * ?f12 / ?f0");
 		add(L"f86", L"0 - ?f85");
 		add(L"f87", L"cos(?f86)");
 		add(L"f88", L"0 - ?f87");
 		add(L"f89", L"?f88 * ?f9");
 		add(L"f90", L"sin(?f86)");
 		add(L"f91", L"0 - ?f90");
 		add(L"f92", L"?f91 * ?f7");
 		add(L"f93", L"sqrt(?f89 * ?f89 + ?f92 * ?f92 + 0 * 0)");
 		add(L"f94", L"?f9 * ?f7 / ?f93");
 		add(L"f95", L"?f91 * ?f94");
 		add(L"f96", L"?f77 + ?f95");
 		add(L"f97", L"?f88 * ?f94");
 		add(L"f98", L"?f79 + ?f97");
 		add(L"f99", L"if(?f72, ?f50, ?f80)");
 		add(L"f100", L"if(?f72, ?f51, ?f81)");
 		add(L"f101", L"if(?f72, ?f50, ?f82)");
 		add(L"f102", L"if(?f72, ?f51, ?f83)");
 		add(L"f103", L"if(?f72, ?f80, ?f96)");
 		add(L"f104", L"if(?f72, ?f81, ?f98)");
 		add(L"f105", L"if(?f72, ?f82, ?f96)");
 		add(L"f106", L"if(?f72, ?f83, ?f98)");
 		add(L"f107", L"21550000 - ?f34");
 		add(L"f108", L"if(?f107, ?f34, 21550000)");
 		add(L"f109", L"-21550000 - ?f108");
 		add(L"f110", L"if(?f109, -21550000, ?f108)");
 		add(L"f111", L"?f30 + ?f110");
 		add(L"f112", L"sqrt(?f60 * ?f60 + ?f57 * ?f57 + 0 * 0)");
 		add(L"f113", L"?f35 * ?f36 / ?f112");
 		add(L"f114", L"?f56 * ?f113");
 		add(L"f115", L"?f65 - ?f114");
 		add(L"f116", L"?f59 * ?f113");
 		add(L"f117", L"?f67 - ?f116");
 		add(L"f118", L"?f115 - ?f35");
 		add(L"f119", L"?f117 - ?f36");
 		add(L"f120", L"?f115 + ?f35");
 		add(L"f121", L"?f117 + ?f36");
 		add(L"f122", L"?f111 + ?f1");
 		add(L"f123", L"?f122 * ?f12 / ?f0");
 		add(L"f124", L"0 - ?f123");
 		add(L"f125", L"cos(?f124)");
 		add(L"f126", L"0 - ?f125");
 		add(L"f127", L"?f126 * ?f35");
 		add(L"f128", L"sin(?f124)");
 		add(L"f129", L"0 - ?f128");
 		add(L"f130", L"?f129 * ?f36");
 		add(L"f131", L"sqrt(?f127 * ?f127 + ?f130 * ?f130 + 0 * 0)");
 		add(L"f132", L"?f35 * ?f36 / ?f131");
 		add(L"f133", L"?f129 * ?f132");
 		add(L"f134", L"?f115 + ?f133");
 		add(L"f135", L"?f126 * ?f132");
 		add(L"f136", L"?f117 + ?f135");
 		add(L"f137", L"if(?f110, ?f65, ?f118)");
 		add(L"f138", L"if(?f110, ?f67, ?f119)");
 		add(L"f139", L"if(?f110, ?f65, ?f120)");
 		add(L"f140", L"if(?f110, ?f67, ?f121)");
 		add(L"f141", L"if(?f110, ?f118, ?f134)");
 		add(L"f142", L"if(?f110, ?f119, ?f136)");
 		add(L"f143", L"if(?f110, ?f120, ?f134)");
 		add(L"f144", L"if(?f110, ?f121, ?f136)");
 		add(L"f145", L"21550000 - -21600000");
 		add(L"f146", L"if(?f145, -21600000, 21550000)");
 		add(L"f147", L"-21550000 - ?f146");
 		add(L"f148", L"if(?f147, -21550000, ?f146)");
 		add(L"f149", L"?f0 + ?f148");
 		add(L"f150", L"?f0 + ?f1");
 		add(L"f151", L"?f150 * ?f12 / ?f0");
 		add(L"f152", L"0 - ?f151");
 		add(L"f153", L"cos(?f152)");
 		add(L"f154", L"0 - ?f153");
 		add(L"f155", L"?f154 * ?f14");
 		add(L"f156", L"sin(?f152)");
 		add(L"f157", L"0 - ?f156");
 		add(L"f158", L"?f157 * ?f15");
 		add(L"f159", L"sqrt(?f155 * ?f155 + ?f158 * ?f158 + 0 * 0)");
 		add(L"f160", L"?f14 * ?f15 / ?f159");
 		add(L"f161", L"?f157 * ?f160");
 		add(L"f162", L"?f68 - ?f161");
 		add(L"f163", L"?f154 * ?f160");
 		add(L"f164", L"?f7 - ?f163");
 		add(L"f165", L"?f162 - ?f14");
 		add(L"f166", L"?f164 - ?f15");
 		add(L"f167", L"?f162 + ?f14");
 		add(L"f168", L"?f164 + ?f15");
 		add(L"f169", L"?f149 + ?f1");
 		add(L"f170", L"?f169 * ?f12 / ?f0");
 		add(L"f171", L"0 - ?f170");
 		add(L"f172", L"cos(?f171)");
 		add(L"f173", L"0 - ?f172");
 		add(L"f174", L"?f173 * ?f14");
 		add(L"f175", L"sin(?f171)");
 		add(L"f176", L"0 - ?f175");
 		add(L"f177", L"?f176 * ?f15");
 		add(L"f178", L"sqrt(?f174 * ?f174 + ?f177 * ?f177 + 0 * 0)");
 		add(L"f179", L"?f14 * ?f15 / ?f178");
 		add(L"f180", L"?f176 * ?f179");
 		add(L"f181", L"?f162 + ?f180");
 		add(L"f182", L"?f173 * ?f179");
 		add(L"f183", L"?f164 + ?f182");
 		add(L"f184", L"if(?f148, ?f68, ?f165)");
 		add(L"f185", L"if(?f148, ?f7, ?f166)");
 		add(L"f186", L"if(?f148, ?f68, ?f167)");
 		add(L"f187", L"if(?f148, ?f7, ?f168)");
 		add(L"f188", L"if(?f148, ?f165, ?f181)");
 		add(L"f189", L"if(?f148, ?f166, ?f183)");
 		add(L"f190", L"if(?f148, ?f167, ?f181)");
 		add(L"f191", L"if(?f148, ?f168, ?f183)");
/////////////////////////////////////////////////////////
	}
};

class oox_shape_PieWedge : public oox_shape
{
public:
	oox_shape_PieWedge()
	{
		odf_type_name	=L"ooxml-pieWedge";

		enhanced_path	= L"M 0 ?f7 G ?f8 ?f9 ?f10 ?f11 L ?f4 ?f7 Z N";
		text_areas		= L"?f2 ?f3 ?f4 ?f7";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"logwidth*cos(pi*(13500000)/10800000)");
		add(L"f1", L"logheight*sin(pi*(13500000)/10800000)");
		add(L"f2", L"logwidth+?f0 -0");
		add(L"f3", L"logheight+?f1 -0");
		add(L"f4", L"logwidth");
		add(L"f5", L"logheight/2");
		add(L"f6", L"logwidth/2");
		add(L"f7", L"logheight");
		add(L"f8", L"logwidth");
		add(L"f9", L"logheight");
		add(L"f10", L"(10800000)/60000.0");
		add(L"f11", L"(5400000)/60000.0");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Pie : public oox_shape
{
public:
	oox_shape_Pie()
	{
		odf_type_name	=L"ooxml-pie";

		enhanced_path	= L"M ?f12 ?f14 G ?f5 ?f7 ?f29 ?f30 L ?f11 ?f13 Z N";
		text_areas		= L"?f23 ?f24 ?f25 ?f26";
		view_box		= L"0 0 0 0";
		modifiers		= L"9000000 16200000";
		
		add(L"f0", L"if(0-$0 ,0,if(21599999-$0 ,$0 ,21599999))");
		add(L"f1", L"if(0-$1 ,0,if(21599999-$1 ,$1 ,21599999))");
		add(L"f2", L"?f1 +0-?f0 ");
		add(L"f3", L"?f2 +21600000-0");
		add(L"f4", L"if(?f2 ,?f2 ,?f3 )");
		add(L"f5", L"logwidth/2");
		add(L"f6", L"?f5 *sin(pi*(?f0 )/10800000)");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 *cos(pi*(?f0 )/10800000)");
		add(L"f9", L"?f5 *(cos(atan2(?f6 ,?f8 )))");
		add(L"f10", L"?f7 *(sin(atan2(?f6 ,?f8 )))");
		add(L"f11", L"logwidth/2");
		add(L"f12", L"?f11 +?f9 -0");
		add(L"f13", L"logheight/2");
		add(L"f14", L"?f13 +?f10 -0");
		add(L"f15", L"?f5 *sin(pi*(?f1 )/10800000)");
		add(L"f16", L"?f7 *cos(pi*(?f1 )/10800000)");
		add(L"f17", L"?f5 *(cos(atan2(?f15 ,?f16 )))");
		add(L"f18", L"?f7 *(sin(atan2(?f15 ,?f16 )))");
		add(L"f19", L"?f11 +?f17 -0");
		add(L"f20", L"?f13 +?f18 -0");
		add(L"f21", L"?f5 *cos(pi*(2700000)/10800000)");
		add(L"f22", L"?f7 *sin(pi*(2700000)/10800000)");
		add(L"f23", L"?f11 +0-?f21 ");
		add(L"f24", L"?f11 +?f21 -0");
		add(L"f25", L"?f13 +0-?f22 ");
		add(L"f26", L"?f13 +?f22 -0");
		add(L"f27", L"logwidth");
		add(L"f28", L"logheight");
		add(L"f29", L"(?f0 )/60000.0");
		add(L"f30", L"(?f4 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f12 ?f14";
		handles.push_back(h);

		h.position = L"?f19 ?f20";
		handles.push_back(h);
	}
};
class oox_shape_BlockArc : public oox_shape
{
public:
	oox_shape_BlockArc()
	{
		odf_type_name	=L"ooxml-blockArc";

		enhanced_path	= L"M ?f18 ?f20 G ?f7 ?f9 ?f76 ?f77 L ?f35 ?f36 G ?f25 ?f26 ?f78 ?f79 Z N";
		text_areas		= L"f60 ?f68 ?f44 ?f52";
		modifiers		= L"13500000 10800000 12740";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(0-$0 ,0,if(21599999-$0 ,$0 ,21599999))");
		add(L"f1", L"if(0-$1 ,0,if(21599999-$1 ,$1 ,21599999))");
		add(L"f2", L"if(0-$2 ,0,if(50000-$2 ,$2 ,50000))");
		add(L"f3", L"?f1 +0-?f0 ");
		add(L"f4", L"?f3 +21600000-0");
		add(L"f5", L"if(?f3 ,?f3 ,?f4 )");
		add(L"f6", L"0+0-?f5 ");
		add(L"f7", L"logwidth/2");
		add(L"f8", L"?f7 *sin(pi*(?f0 )/10800000)");
		add(L"f9", L"logheight/2");
		add(L"f10", L"?f9 *cos(pi*(?f0 )/10800000)");
		add(L"f11", L"?f7 *sin(pi*(?f1 )/10800000)");
		add(L"f12", L"?f9 *cos(pi*(?f1 )/10800000)");
		add(L"f13", L"?f7 *(cos(atan2(?f8 ,?f10 )))");
		add(L"f14", L"?f9 *(sin(atan2(?f8 ,?f10 )))");
		add(L"f15", L"?f7 *(cos(atan2(?f11 ,?f12 )))");
		add(L"f16", L"?f9 *(sin(atan2(?f11 ,?f12 )))");
		add(L"f17", L"logwidth/2");
		add(L"f18", L"?f17 +?f13 -0");
		add(L"f19", L"logheight/2");
		add(L"f20", L"?f19 +?f14 -0");
		add(L"f21", L"?f17 +?f15 -0");
		add(L"f22", L"?f19 +?f16 -0");
		add(L"f23", L"min(logwidth,logheight)");
		add(L"f24", L"?f23 *?f2 /100000");
		add(L"f25", L"?f7 +0-?f24 ");
		add(L"f26", L"?f9 +0-?f24 ");
		add(L"f27", L"?f25 *sin(pi*(?f1 )/10800000)");
		add(L"f28", L"?f26 *cos(pi*(?f1 )/10800000)");
		add(L"f29", L"?f25 *sin(pi*(?f0 )/10800000)");
		add(L"f30", L"?f26 *cos(pi*(?f0 )/10800000)");
		add(L"f31", L"?f25 *(cos(atan2(?f27 ,?f28 )))");
		add(L"f32", L"?f26 *(sin(atan2(?f27 ,?f28 )))");
		add(L"f33", L"?f25 *(cos(atan2(?f29 ,?f30 )))");
		add(L"f34", L"?f26 *(sin(atan2(?f29 ,?f30 )))");
		add(L"f35", L"?f17 +?f31 -0");
		add(L"f36", L"?f19 +?f32 -0");
		add(L"f37", L"?f17 +?f33 -0");
		add(L"f38", L"?f19 +?f34 -0");
		add(L"f39", L"21600000+0-?f0 ");
		add(L"f40", L"?f5 +0-?f39 ");
		add(L"f41", L"max(?f18 ,?f35 )");
		add(L"f42", L"max(?f21 ,?f37 )");
		add(L"f43", L"max(?f41 ,?f42 )");
		add(L"f44", L"if(?f40 ,logwidth,?f43 )");
		add(L"f45", L"5400000+0-?f0 ");
		add(L"f46", L"27000000+0-?f0 ");
		add(L"f47", L"if(?f45 ,?f45 ,?f46 )");
		add(L"f48", L"?f5 +0-?f47 ");
		add(L"f49", L"max(?f20 ,?f36 )");
		add(L"f50", L"max(?f22 ,?f38 )");
		add(L"f51", L"max(?f49 ,?f50 )");
		add(L"f52", L"if(?f48 ,logheight,?f51 )");
		add(L"f53", L"10800000+0-?f0 ");
		add(L"f54", L"32400000+0-?f0 ");
		add(L"f55", L"if(?f53 ,?f53 ,?f54 )");
		add(L"f56", L"?f5 +0-?f55 ");
		add(L"f57", L"min(?f18 ,?f35 )");
		add(L"f58", L"min(?f21 ,?f37 )");
		add(L"f59", L"min(?f57 ,?f58 )");
		add(L"f60", L"if(?f56 ,0,?f59 )");
		add(L"f61", L"16200000+0-?f0 ");
		add(L"f62", L"37800000+0-?f0 ");
		add(L"f63", L"if(?f61 ,?f61 ,?f62 )");
		add(L"f64", L"?f5 +0-?f63 ");
		add(L"f65", L"min(?f20 ,?f36 )");
		add(L"f66", L"min(?f22 ,?f38 )");
		add(L"f67", L"min(?f65 ,?f66 )");
		add(L"f68", L"if(?f64 ,0,?f67 )");
		add(L"f69", L"(?f18 +?f37 )/2");
		add(L"f70", L"(?f20 +?f38 )/2");
		add(L"f71", L"(?f21 +?f35 )/2");
		add(L"f72", L"(?f22 +?f36 )/2");
		add(L"f73", L"?f0 +0-5400000");
		add(L"f74", L"?f1 +5400000-0");
		add(L"f75", L"(?f73 +?f74 )/2");
		add(L"f76", L"(?f0 )/60000.0");
		add(L"f77", L"(?f5 )/60000.0");
		add(L"f78", L"(?f1 )/60000.0");
		add(L"f79", L"(?f6 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f18 ?f20";
		handles.push_back(h);

		h.position = L"?f35 ?f36";
		h.r_minimum = L"0";
		h.r_maximum = L"50000";

		handles.push_back(h);
	}
};
class oox_shape_Corner : public oox_shape
{
public:
	oox_shape_Corner()
	{
		odf_type_name	=L"ooxml-corner";

		enhanced_path	= L"M 0 0 L ?f5 0 ?f5 ?f7 ?f13 ?f7 ?f13 ?f15 0 ?f15 Z N";
		text_areas		= L"0 ?f11 ?f12 ?f15";
		view_box		= L"0 0 0 0";
		modifiers		= L"16120 16110";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logheight/?f0 ");
		add(L"f2", L"100000*logwidth/?f0 ");
		add(L"f3", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f4", L"if(0-$1 ,0,if(?f2 -$1 ,$1 ,?f2 ))");
		add(L"f5", L"?f0 *?f4 /100000");
		add(L"f6", L"?f0 *?f3 /100000");
		add(L"f7", L"logheight+0-?f6 ");
		add(L"f8", L"?f5 *1/2");
		add(L"f9", L"(?f7 +logheight)/2");
		add(L"f10", L"logwidth+0-logheight");
		add(L"f11", L"if(?f10 ,?f7 ,0)");
		add(L"f12", L"if(?f10 ,logwidth,?f5 )");
		add(L"f13", L"logwidth");
		add(L"f14", L"logwidth/2");
		add(L"f15", L"logheight");
		add(L"f16", L"logheight/2");
/////////////////////////////////////////////////////////
		_handle h1, h2;

		h1.position = L"0 ?f7";
		h1.y_minimum = L"0";
		h1.y_maximum = L"?f1";
		handles.push_back(h1);

		h2.position = L"?f5 0";
		h2.x_minimum = L"0";
		h2.x_maximum = L"?f2";
		handles.push_back(h2);
	}
};
class oox_shape_Chord : public oox_shape
{
public:
	oox_shape_Chord()
	{
		odf_type_name	=L"ooxml-chord";

		enhanced_path	= L"M ?f16 ?f18 G ?f5 ?f7 ?f31 ?f32 Z N";
		text_areas		= L"?f27 ?f29 ?f28 ?f30";
		view_box		= L"0 0 0 0";
		modifiers		= L"1168272 9631728";
		
		add(L"f0", L"if(0-$0 ,0,if(21599999-$0 ,$0 ,21599999))");
		add(L"f1", L"if(0-$1 ,0,if(21599999-$1 ,$1 ,21599999))");
		add(L"f2", L"?f1 +0-?f0 ");
		add(L"f3", L"?f2 +21600000-0");
		add(L"f4", L"if(?f2 ,?f2 ,?f3 )");
		add(L"f5", L"logwidth/2");
		add(L"f6", L"?f5 *sin(pi*(?f0 )/10800000)");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 *cos(pi*(?f0 )/10800000)");
		add(L"f9", L"?f5 *(cos(atan2(?f6 ,?f8 )))");
		add(L"f10", L"?f7 *(sin(atan2(?f6 ,?f8 )))");
		add(L"f11", L"?f5 *sin(pi*(?f1 )/10800000)");
		add(L"f12", L"?f7 *cos(pi*(?f1 )/10800000)");
		add(L"f13", L"?f5 *(cos(atan2(?f11 ,?f12 )))");
		add(L"f14", L"?f7 *(sin(atan2(?f11 ,?f12 )))");
		add(L"f15", L"logwidth/2");
		add(L"f16", L"?f15 +?f9 -0");
		add(L"f17", L"logheight/2");
		add(L"f18", L"?f17 +?f10 -0");
		add(L"f19", L"?f15 +?f13 -0");
		add(L"f20", L"?f17 +?f14 -0");
		add(L"f21", L"(?f16 +?f19 )/2");
		add(L"f22", L"(?f18 +?f20 )/2");
		add(L"f23", L"?f4 *1/2");
		add(L"f24", L"?f0 +?f23 -10800000");
		add(L"f25", L"?f5 *cos(pi*(2700000)/10800000)");
		add(L"f26", L"?f7 *sin(pi*(2700000)/10800000)");
		add(L"f27", L"?f15 +0-?f25 ");
		add(L"f28", L"?f15 +?f25 -0");
		add(L"f29", L"?f17 +0-?f26 ");
		add(L"f30", L"?f17 +?f26 -0");
		add(L"f31", L"(?f0 )/60000.0");
		add(L"f32", L"(?f4 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f16 ?f18";
		handles.push_back(h);
		
		h.position = L"?f19 ?f20";
		handles.push_back(h);
	}
};


class oox_shape_Donut : public oox_shape
{
public:
	oox_shape_Donut()
	{
		odf_type_name	=L"ooxml-donut";

		enhanced_path	= L"M 0 ?f12 G ?f3 ?f5 ?f17 ?f18 ?f3 ?f5 ?f19 ?f20 ?f3 ?f5 ?f21 ?f22 ?f3 ?f5 ?f23 ?f24 Z M ?f2 ?f12 G ?f4 ?f6 ?f25 ?f26 ?f4 ?f6 ?f27 ?f28 ?f4 ?f6 ?f29 ?f30 ?f4 ?f6 ?f31 ?f32 Z N";
		text_areas		= L"?f10 ?f13 ?f11 ?f14";
		view_box		= L"0 0 0 0";
		modifiers		= L"20000";
		
		add(L"f0", L"if(0-$0 ,0,if(50000-$0 ,$0 ,50000))");
		add(L"f1", L"min(logwidth,logheight)");
		add(L"f2", L"?f1 *?f0 /100000");
		add(L"f3", L"logwidth/2");
		add(L"f4", L"?f3 +0-?f2 ");
		add(L"f5", L"logheight/2");
		add(L"f6", L"?f5 +0-?f2 ");
		add(L"f7", L"?f3 *cos(pi*(2700000)/10800000)");
		add(L"f8", L"?f5 *sin(pi*(2700000)/10800000)");
		add(L"f9", L"logwidth/2");
		add(L"f10", L"?f9 +0-?f7 ");
		add(L"f11", L"?f9 +?f7 -0");
		add(L"f12", L"logheight/2");
		add(L"f13", L"?f12 +0-?f8 ");
		add(L"f14", L"?f12 +?f8 -0");
		add(L"f15", L"logheight");
		add(L"f16", L"logwidth");
		add(L"f17", L"(10800000)/60000.0");
		add(L"f18", L"(5400000)/60000.0");
		add(L"f19", L"(16200000)/60000.0");
		add(L"f20", L"(5400000)/60000.0");
		add(L"f21", L"(0)/60000.0");
		add(L"f22", L"(5400000)/60000.0");
		add(L"f23", L"(5400000)/60000.0");
		add(L"f24", L"(5400000)/60000.0");
		add(L"f25", L"(10800000)/60000.0");
		add(L"f26", L"(-5400000)/60000.0");
		add(L"f27", L"(5400000)/60000.0");
		add(L"f28", L"(-5400000)/60000.0");
		add(L"f29", L"(0)/60000.0");
		add(L"f30", L"(-5400000)/60000.0");
		add(L"f31", L"(16200000)/60000.0");
		add(L"f32", L"(-5400000)/60000.0");
/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f2 ?f12";
		h.r_minimum = L"0";
		h.r_maximum = L"50000";
		handles.push_back(h);
	}
};


class oox_shape_Chevron : public oox_shape
{
public:
	oox_shape_Chevron()
	{
		odf_type_name	=L"ooxml-chevron";

		enhanced_path	= L"M 0 0 L ?f4 0 ?f11 ?f9 ?f4 ?f10 0 ?f10 ?f3 ?f9 Z N";
		text_areas		= L"?f7 0 ?f8 ?f10";
		view_box		= L"0 0 0 0";
		modifiers		= L"50000";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logwidth/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"?f0 *?f2 /100000");
		add(L"f4", L"logwidth+0-?f3 ");
		add(L"f5", L"?f4 *1/2");
		add(L"f6", L"?f4 +0-?f3 ");
		add(L"f7", L"if(?f6 ,?f3 ,0)");
		add(L"f8", L"if(?f6 ,?f4 ,logwidth)");
		add(L"f9", L"logheight/2");
		add(L"f10", L"logheight");
		add(L"f11", L"logwidth");

/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f4 0";
		h.x_minimum = L"0";
		h.x_maximum = L"?f1";
		handles.push_back(h);
	}
};
class oox_shape_Arc : public oox_shape
{
public:
	oox_shape_Arc()
	{
		odf_type_name	=L"ooxml-arc";

		enhanced_path	= L"M ?f16 ?f18 G ?f5 ?f7 ?f46 ?f47 L ?f15 ?f17 Z S N M ?f16 ?f18 G ?f5 ?f7 ?f48 ?f49 F N";
		text_areas		= L"?f36 ?f42 ?f24 ?f30";
		view_box		= L"0 0 0 0";
		modifiers		= L"16200000 0";
		
		add(L"f0", L"if(0-$0 ,0,if(21599999-$0 ,$0 ,21599999))");
		add(L"f1", L"if(0-$1 ,0,if(21599999-$1 ,$1 ,21599999))");
		add(L"f2", L"?f1 +0-?f0 ");
		add(L"f3", L"?f2 +21600000-0");
		add(L"f4", L"if(?f2 ,?f2 ,?f3 )");
		add(L"f5", L"logwidth/2");
		add(L"f6", L"?f5 *sin(pi*(?f0 )/10800000)");
		add(L"f7", L"logheight/2");
		add(L"f8", L"?f7 *cos(pi*(?f0 )/10800000)");
		add(L"f9", L"?f5 *(cos(atan2(?f6 ,?f8 )))");
		add(L"f10", L"?f7 *(sin(atan2(?f6 ,?f8 )))");
		add(L"f11", L"?f5 *sin(pi*(?f1 )/10800000)");
		add(L"f12", L"?f7 *cos(pi*(?f1 )/10800000)");
		add(L"f13", L"?f5 *(cos(atan2(?f11 ,?f12 )))");
		add(L"f14", L"?f7 *(sin(atan2(?f11 ,?f12 )))");
		add(L"f15", L"logwidth/2");
		add(L"f16", L"?f15 +?f9 -0");
		add(L"f17", L"logheight/2");
		add(L"f18", L"?f17 +?f10 -0");
		add(L"f19", L"?f15 +?f13 -0");
		add(L"f20", L"?f17 +?f14 -0");
		add(L"f21", L"21600000+0-?f0 ");
		add(L"f22", L"?f4 +0-?f21 ");
		add(L"f23", L"max(?f16 ,?f19 )");
		add(L"f24", L"if(?f22 ,logwidth,?f23 )");
		add(L"f25", L"5400000+0-?f0 ");
		add(L"f26", L"27000000+0-?f0 ");
		add(L"f27", L"if(?f25 ,?f25 ,?f26 )");
		add(L"f28", L"?f4 +0-?f27 ");
		add(L"f29", L"max(?f18 ,?f20 )");
		add(L"f30", L"if(?f28 ,logheight,?f29 )");
		add(L"f31", L"10800000+0-?f0 ");
		add(L"f32", L"32400000+0-?f0 ");
		add(L"f33", L"if(?f31 ,?f31 ,?f32 )");
		add(L"f34", L"?f4 +0-?f33 ");
		add(L"f35", L"min(?f16 ,?f19 )");
		add(L"f36", L"if(?f34 ,0,?f35 )");
		add(L"f37", L"16200000+0-?f0 ");
		add(L"f38", L"37800000+0-?f0 ");
		add(L"f39", L"if(?f37 ,?f37 ,?f38 )");
		add(L"f40", L"?f4 +0-?f39 ");
		add(L"f41", L"min(?f18 ,?f20 )");
		add(L"f42", L"if(?f40 ,0,?f41 )");
		add(L"f43", L"?f0 +0-5400000");
		add(L"f44", L"?f1 +5400000-0");
		add(L"f45", L"(?f43 +?f44 )/2");
		add(L"f46", L"(?f0 )/60000.0");
		add(L"f47", L"(?f4 )/60000.0");
		add(L"f48", L"(?f0 )/60000.0");
		add(L"f49", L"(?f4 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f16 ?f18";
		handles.push_back(h);
		
		h.position = L"?f19 ?f20";
		handles.push_back(h);
	}
};


class oox_shape_HomePlate : public oox_shape
{
public:
	oox_shape_HomePlate()
	{
		odf_type_name	=L"ooxml-homePlate";

		enhanced_path	= L"M 0 0 L ?f4 0 ?f9 ?f7 ?f4 ?f8 0 ?f8 Z N";
		text_areas		= L"0 0 ?f5 ?f8";
		view_box		= L"0 0 0 0";
		modifiers		= L"50000";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logwidth/?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"?f0 *?f2 /100000");
		add(L"f4", L"logwidth+0-?f3 ");
		add(L"f5", L"(?f4 +logwidth)/2");
		add(L"f6", L"?f4 *1/2");
		add(L"f7", L"logheight/2");
		add(L"f8", L"logheight");
		add(L"f9", L"logwidth");

/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f4 0";
		h.x_minimum = L"0";
		h.x_maximum = L"?f1";
		handles.push_back(h);
	}
};

class oox_shape_Bevel : public oox_shape
{
public:
	oox_shape_Bevel()
	{
		odf_type_name	=L"ooxml-bevel";

		enhanced_path	= L"S M ?f12 ?f12 L ?f13 ?f12 ?f13 ?f14 ?f12 ?f14 Z N S M ?f0 ?f2 L ?f1 ?f2 ?f13 ?f12 ?f12 ?f12 Z N S M ?f0 ?f3 L ?f12 ?f14 ?f13 ?f14 ?f1 ?f3 Z N S M ?f0 ?f2 L ?f12 ?f12 ?f12 ?f14 ?f0 ?f3 Z N S M ?f1 ?f2 L ?f1 ?f3 ?f13 ?f14 ?f13 ?f12 Z N F M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z M ?f12 ?f12 L ?f13 ?f12 ?f13 ?f14 ?f12 ?f14 Z M ?f0 ?f2 L ?f12 ?f12 M ?f0 ?f3 L ?f12 ?f14 M ?f1 ?f2 L ?f13 ?f12 M ?f1 ?f3 L ?f13 ?f14 N";
		text_areas		= L"?f2 ?f2 ?f3 ?f4";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"12500";
		glue_points		= L"?f13 ?f6 ?f9 ?f14 ?f12 ?f6 ?f9 ?f12";
		
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
		add(L"f11", L"$0");
		add(L"f12", L"?f10 * ?f11 / 100000");
		add(L"f13", L"?f1 - ?f12");
		add(L"f14", L"?f3 - ?f12");

/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f2 0";
		h.x_minimum = L"0";
		h.x_maximum = L"50000";
		handles.push_back(h);
	}
};


class oox_shape_Trapezoid : public oox_shape
{
public:
	oox_shape_Trapezoid()
	{
		odf_type_name	=L"ooxml-trapezoid";

		enhanced_path	= L"M 0 ?f14 L ?f4 0 ?f5 0 ?f15 ?f14 Z N";
		text_areas		= L"?f8 ?f10 ?f11 ?f14";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"50000*logwidth/?f0");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"?f0 *?f2 /200000");
		add(L"f4", L"?f0 *?f2 /100000");
		add(L"f5", L"logwidth+0-?f4");
		add(L"f6", L"logwidth+0-?f3");
		add(L"f7", L"logwidth/3");
		add(L"f8", L"?f7 *?f2 /?f1");
		add(L"f9", L"logheight/3");
		add(L"f10", L"?f9 *?f2 /?f1");
		add(L"f11", L"logwidth+0-?f8");
		add(L"f12", L"logwidth/2");
		add(L"f13", L"logheight/2");
		add(L"f14", L"logheight");
		add(L"f15", L"logwidth");

/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f4 0";
		h.x_minimum = L"0";
		h.x_maximum = L"?f1";
		handles.push_back(h);
	}
};


class oox_shape_Triangle : public oox_shape
{
public:
	oox_shape_Triangle()
	{
		odf_type_name	=L"ooxml-triangle";

		enhanced_path	= L"M 0 ?f6 L ?f2 0 ?f7 ?f6 Z N";
		text_areas		= L"?f1 ?f5 ?f4 ?f6";
		view_box		= L"0 0 0 0";
		modifiers		= L"50000";
		
		add(L"f0", L"if(0-$0 ,0,if(100000-$0 ,$0 ,100000))");
		add(L"f1", L"logwidth*?f0 /200000");
		add(L"f2", L"logwidth*?f0 /100000");
		add(L"f3", L"logwidth/2");
		add(L"f4", L"?f1 +?f3 -0");
		add(L"f5", L"logheight/2");
		add(L"f6", L"logheight");
		add(L"f7", L"logwidth");

/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f2 0";
		h.x_minimum = L"0";
		h.x_maximum = L"100000";
		handles.push_back(h);
	}
};

class oox_shape_Diamond : public oox_shape
{
public:
	oox_shape_Diamond()
	{
		odf_type_name	=L"ooxml-diamond";

		enhanced_path	= L"M 0 ?f3 L ?f2 0 ?f5 ?f3 ?f2 ?f4 Z N";
		text_areas		= L"?f6 ?f7 ?f0 ?f1";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"logwidth*3/4");
		add(L"f1", L"logheight*3/4");
		add(L"f2", L"logwidth/2");
		add(L"f3", L"logheight/2");
		add(L"f4", L"logheight");
		add(L"f5", L"logwidth");
		add(L"f6", L"logwidth/4");
		add(L"f7", L"logheight/4");
	}
};

class oox_shape_Parallelogram : public oox_shape
{
public:
	oox_shape_Parallelogram()
	{
		odf_type_name	=L"ooxml-parallelogram";

		enhanced_path	= L"M 0 ?f22 L ?f4 0 ?f23 0 ?f6 ?f22 Z N";
		text_areas		= L"?f13 ?f14 ?f15 ?f16";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000";
		
		add(L"f0", L"min(logwidth,logheight)");
		add(L"f1", L"100000*logwidth/?f0");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"?f0 *?f2 /200000");
		add(L"f4", L"?f0 *?f2 /100000");
		add(L"f5", L"logwidth+0-?f3");
		add(L"f6", L"logwidth+0-?f4");
		add(L"f7", L"?f6 *1/2");
		add(L"f8", L"logwidth+0-?f7");
		add(L"f9", L"logwidth/2");
		add(L"f10", L"?f9 *?f2 /?f1");
		add(L"f11", L"5*?f2 /?f1 ");
		add(L"f12", L"(1+?f11 )/12");
		add(L"f13", L"?f12 *logwidth/1");
		add(L"f14", L"?f12 *logheight/1");
		add(L"f15", L"logwidth+0-?f13");
		add(L"f16", L"logheight+0-?f14");
		add(L"f17", L"logwidth/2");
		add(L"f18", L"logheight*?f17 /?f4");
		add(L"f19", L"if(0-?f18 ,0,if(logheight-?f18 ,?f18 ,logheight))");
		add(L"f20", L"logheight+0-?f19");
		add(L"f21", L"logheight/2");
		add(L"f22", L"logheight");
		add(L"f23", L"logwidth");

/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f4 0";
		h.x_minimum = L"0";
		h.x_maximum = L"?f1";
		handles.push_back(h);
	}
};

class oox_shape_RtTriangle : public oox_shape
{
public:
	oox_shape_RtTriangle()
	{
		odf_type_name	=L"ooxml-rtTriangle";

		enhanced_path	= L"M 0 ?f4 L 0 0 ?f6 ?f4 Z N";
		text_areas		= L"?f7 ?f0 ?f1 ?f2";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"logheight*7/12");
		add(L"f1", L"logwidth*7/12");
		add(L"f2", L"logheight*11/12");
		add(L"f3", L"logheight/2");
		add(L"f4", L"logheight");
		add(L"f5", L"logwidth/2");
		add(L"f6", L"logwidth");
		add(L"f7", L"logwidth/12");
	}
};


class oox_shape_Plus : public oox_shape
{
public:
	oox_shape_Plus()
	{
		odf_type_name	=L"ooxml-plus";

		enhanced_path	= L"M 0 ?f2 L ?f2 ?f2 ?f2 0 ?f3 0 ?f3 ?f2 ?f13 ?f2 ?f13 ?f4 ?f3 ?f4 ?f3 ?f12 ?f2 ?f12 ?f2 ?f4 0 ?f4 Z N";
		text_areas		= L"?f6 ?f8 ?f7 ?f9";
		view_box		= L"0 0 0 0";
		modifiers		= L"25000";
		
		add(L"f0", L"if(0-$0 ,0,if(50000-$0 ,$0 ,50000))");
		add(L"f1", L"min(logwidth,logheight)");
		add(L"f2", L"?f1 *?f0 /100000");
		add(L"f3", L"logwidth+0-?f2 ");
		add(L"f4", L"logheight+0-?f2 ");
		add(L"f5", L"logwidth+0-logheight");
		add(L"f6", L"if(?f5 ,0,?f2 )");
		add(L"f7", L"if(?f5 ,logwidth,?f3 )");
		add(L"f8", L"if(?f5 ,?f2 ,0)");
		add(L"f9", L"if(?f5 ,?f4 ,logheight)");
		add(L"f10", L"logwidth/2");
		add(L"f11", L"logheight/2");
		add(L"f12", L"logheight");
		add(L"f13", L"logwidth");

/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f2 0";
		h.x_minimum = L"0";
		h.x_maximum = L"50000";
		handles.push_back(h);
	}
};
class oox_shape_Frame : public oox_shape
{
public:
	oox_shape_Frame()
	{
		odf_type_name	=L"ooxml-frame";

		modifiers		= L"12500";
		enhanced_path	= L"M 0 0 L ?f8 0 ?f8 ?f7 0 ?f7 Z M ?f2 ?f2 L ?f2 ?f4 ?f3 ?f4 ?f3 ?f2 Z N";
		text_areas		= L"?f2 ?f2 ?f3 ?f4";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(0-$0 ,0,if(50000-$0 ,$0 ,50000))");
		add(L"f1", L"min(logwidth,logheight)");
		add(L"f2", L"?f1 *?f0 /100000");
		add(L"f3", L"logwidth+0-?f2 ");
		add(L"f4", L"logheight+0-?f2 ");
		add(L"f5", L"logwidth/2");
		add(L"f6", L"logheight/2");
		add(L"f7", L"logheight");
		add(L"f8", L"logwidth");

 		_handle h2;
		
		h2.position = L"?f2 0";
		h2.y_maximum= L"50000";
		handles.push_back(h2);

/////////////////////////////////////////////////////////
	}
};

class oox_shape_Pentagon : public oox_shape
{
public:
	oox_shape_Pentagon()
	{
		odf_type_name	=L"ooxml-pentagon";

		enhanced_path	= L"M ?f34 ?f38 L ?f11 ?f4 ?f37 ?f38 ?f36 ?f39 ?f35 ?f39 Z N";
		text_areas		= L"?f35 ?f40 ?f36 ?f39";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f34 ?f38 ?f35 ?f39 ?f36 ?f39 ?f37 ?f38";
		
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
		add(L"f13", L"?f10 * ?f41 / 100000");
		add(L"f14", L"?f7 * ?f42 / 100000");
		add(L"f15", L"?f8 * ?f42 / 100000");
		add(L"f16", L"1080000 + ?f1");
		add(L"f17", L"?f16 * ?f12 / ?f0");
		add(L"f18", L"0 - ?f17");
		add(L"f19", L"sin(?f18)");
		add(L"f20", L"0 - ?f19");
		add(L"f21", L"?f20 * ?f13");
		add(L"f22", L"18360000 + ?f1");
		add(L"f23", L"?f22 * ?f12 / ?f0");
		add(L"f24", L"0 - ?f23");
		add(L"f25", L"sin(?f24)");
		add(L"f26", L"0 - ?f25");
		add(L"f27", L"?f26 * ?f13");
		add(L"f28", L"cos(?f18)");
		add(L"f29", L"0 - ?f28");
		add(L"f30", L"?f29 * ?f14");
		add(L"f31", L"cos(?f24)");
		add(L"f32", L"0 - ?f31");
		add(L"f33", L"?f32 * ?f14");
		add(L"f34", L"?f11 - ?f21");
		add(L"f35", L"?f11 - ?f27");
		add(L"f36", L"?f11 + ?f27");
		add(L"f37", L"?f11 + ?f21");
		add(L"f38", L"?f15 - ?f30");
		add(L"f39", L"?f15 - ?f33");
		add(L"f40", L"?f38 * ?f27 / ?f21");
		add(L"f41", L"105146");
		add(L"f42", L"110557");
/////////////////////////////////////////////////////////
	}
};

class oox_shape_Octagon : public oox_shape
{
public:
	oox_shape_Octagon()
	{
		odf_type_name	=L"ooxml-octagon";

		modifiers		= L"29289";
		enhanced_path	= L"M ?f0 ?f8 L ?f8 ?f2 ?f9 ?f2 ?f1 ?f8 ?f1 ?f10 ?f9 ?f3 ?f8 ?f3 ?f0 ?f10 Z N";
		text_areas		= L"?f11 ?f11 ?f12 ?f13";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f1 ?f8 ?f1 ?f10 ?f9 ?f3 ?f8 ?f3 ?f0 ?f10 ?f0 ?f8 ?f8 ?f2 ?f9 ?f2";
		
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
		add(L"f10", L"?f3 - ?f8");
		add(L"f11", L"?f8 / 2");
		add(L"f12", L"?f1 - ?f11");
		add(L"f13", L"?f3 - ?f11");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Hexagon : public oox_shape
{
public:
	oox_shape_Hexagon()
	{
		odf_type_name	=L"ooxml-hexagon";

		modifiers		= L"28921 115470";
		enhanced_path	= L"M ?f2 ?f8 L ?f15 ?f23 ?f16 ?f23 ?f3 ?f8 ?f16 ?f24 ?f15 ?f24 Z N";
		text_areas		= L"?f33 ?f34 ?f35 ?f36";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f16 ?f24 ?f15 ?f24 ?f15 ?f23 ?f16 ?f23";
		
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
		add(L"f10", L"min(?f9, ?f6)");
		add(L"f11", L"5419351 / 1725033");
		add(L"f12", L"50000 * ?f9 / ?f10");
		add(L"f13", L"$0");
		add(L"f14", L"?f7 * ?f37 / 100000");
		add(L"f15", L"?f10 * ?f13 / 100000");
		add(L"f16", L"?f3 - ?f15");
		add(L"f17", L"3600000 + ?f1");
		add(L"f18", L"?f17 * ?f11 / ?f0");
		add(L"f19", L"0 - ?f18");
		add(L"f20", L"cos(?f19)");
		add(L"f21", L"0 - ?f20");
		add(L"f22", L"?f21 * ?f14");
		add(L"f23", L"?f8 - ?f22");
		add(L"f24", L"?f8 + ?f22");
		add(L"f25", L"?f12 * -1 / 2");
		add(L"f26", L"?f13 + ?f25");
		add(L"f27", L"if(?f26, 4, 2)");
		add(L"f28", L"if(?f26, 3, 2)");
		add(L"f29", L"if(?f26, ?f25, 0)");
		add(L"f30", L"(?f13 + ?f29) / ?f25");
		add(L"f31", L"?f30 * ?f28 / -1");
		add(L"f32", L"?f27 + ?f31");
		add(L"f33", L"?f9 * ?f32 / 24");
		add(L"f34", L"?f6 * ?f32 / 24");
		add(L"f35", L"?f3 - ?f33");
		add(L"f36", L"?f5 - ?f34");
		add(L"f37", L"115470");
/////////////////////////////////////////////////////////
	}
};

class oox_shape_Heptagon : public oox_shape
{
public:
	oox_shape_Heptagon()
	{
		odf_type_name	=L"ooxml-heptagon";

		enhanced_path	= L"M ?f19 ?f26 L ?f20 ?f25 ?f9 ?f2 ?f23 ?f25 ?f24 ?f26 ?f22 ?f27 ?f21 ?f27 Z N";
		text_areas		= L"?f20 ?f25 ?f23 ?f28";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f23 ?f25 ?f24 ?f26 ?f22 ?f27 ?f21 ?f27 ?f19 ?f26 ?f20 ?f25";
		
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
		add(L"f10", L"?f8 * ?f29 / 100000");
		add(L"f11", L"?f5 * ?f30 / 100000");
		add(L"f12", L"?f6 * ?f30 / 100000");
		add(L"f13", L"?f10 * 97493 / 100000");
		add(L"f14", L"?f10 * 78183 / 100000");
		add(L"f15", L"?f10 * 43388 / 100000");
		add(L"f16", L"?f11 * 62349 / 100000");
		add(L"f17", L"?f11 * 22252 / 100000");
		add(L"f18", L"?f11 * 90097 / 100000");
		add(L"f19", L"?f9 - ?f13");
		add(L"f20", L"?f9 - ?f14");
		add(L"f21", L"?f9 - ?f15");
		add(L"f22", L"?f9 + ?f15");
		add(L"f23", L"?f9 + ?f14");
		add(L"f24", L"?f9 + ?f13");
		add(L"f25", L"?f12 - ?f16");
		add(L"f26", L"?f12 + ?f17");
		add(L"f27", L"?f12 + ?f18");
		add(L"f28", L"?f3 - ?f25");
		add(L"f29", L"102572");
		add(L"f30", L"105210");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Decagon : public oox_shape
{
public:
	oox_shape_Decagon()
	{
		odf_type_name	=L"ooxml-decagon";

		enhanced_path	= L"M ?f2 ?f8 L ?f26 ?f37 ?f27 ?f36 ?f28 ?f36 ?f29 ?f37 ?f3 ?f8 ?f29 ?f38 ?f28 ?f39 ?f27 ?f39 ?f26 ?f38 Z N";
		text_areas		= L"?f6 ?f14 ?f9 ?f15";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f29 ?f37 ?f29 ?f38 ?f28 ?f39 ?f27 ?f39 ?f26 ?f38 ?f26 ?f37 ?f27 ?f36 ?f28 ?f36";
		
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
		add(L"f13", L"?f7 * ?f40 / 100000");
		add(L"f14", L"2160000 + ?f1");
		add(L"f15", L"?f14 * ?f12 / ?f0");
		add(L"f16", L"0 - ?f15");
		add(L"f17", L"sin(?f16)");
		add(L"f18", L"0 - ?f17");
		add(L"f19", L"?f18 * ?f10");
		add(L"f20", L"4320000 + ?f1");
		add(L"f21", L"?f20 * ?f12 / ?f0");
		add(L"f22", L"0 - ?f21");
		add(L"f23", L"sin(?f22)");
		add(L"f24", L"0 - ?f23");
		add(L"f25", L"?f24 * ?f10");
		add(L"f26", L"?f11 - ?f19");
		add(L"f27", L"?f11 - ?f25");
		add(L"f28", L"?f11 + ?f25");
		add(L"f29", L"?f11 + ?f19");
		add(L"f30", L"cos(?f22)");
		add(L"f31", L"0 - ?f30");
		add(L"f32", L"?f31 * ?f13");
		add(L"f33", L"cos(?f16)");
		add(L"f34", L"0 - ?f33");
		add(L"f35", L"?f34 * ?f13");
		add(L"f36", L"?f8 - ?f32");
		add(L"f37", L"?f8 - ?f35");
		add(L"f38", L"?f8 + ?f35");
		add(L"f39", L"?f8 + ?f32");
		add(L"f40", L"105146");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Dodecagon : public oox_shape
{
public:
	oox_shape_Dodecagon()
	{
		odf_type_name	=L"ooxml-dodecagon";

		enhanced_path	= L"M ?f0 ?f11 L ?f6 ?f10 ?f7 ?f2 ?f8 ?f2 ?f9 ?f10 ?f1 ?f11 ?f1 ?f12 ?f9 ?f13 ?f8 ?f3 ?f7 ?f3 ?f6 ?f13 ?f0 ?f12 Z N";
		text_areas		= L"?f6 ?f10 ?f9 ?f13";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f9 ?f10 ?f1 ?f11 ?f1 ?f12 ?f9 ?f13 ?f8 ?f3 ?f7 ?f3 ?f6 ?f13 ?f0 ?f12 ?f0 ?f11 ?f6 ?f10 ?f7 ?f2 ?f8 ?f2";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"?f5 * 2894 / 21600");
		add(L"f7", L"?f5 * 7906 / 21600");
		add(L"f8", L"?f5 * 13694 / 21600");
		add(L"f9", L"?f5 * 18706 / 21600");
		add(L"f10", L"?f4 * 2894 / 21600");
		add(L"f11", L"?f4 * 7906 / 21600");
		add(L"f12", L"?f4 * 13694 / 21600");
		add(L"f13", L"?f4 * 18706 / 21600");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Cube : public oox_shape
{
public:
	oox_shape_Cube()
	{
		odf_type_name	=L"ooxml-cube";

		modifiers		= L"25000";
		enhanced_path	= L"S M ?f0 ?f8 L ?f12 ?f8 ?f12 ?f3 ?f0 ?f3 Z N S M ?f12 ?f8 L ?f1 ?f2 ?f1 ?f9 ?f12 ?f3 Z N S M ?f0 ?f8 L ?f8 ?f2 ?f1 ?f2 ?f12 ?f8 Z N F M ?f0 ?f8 L ?f8 ?f2 ?f1 ?f2 ?f1 ?f9 ?f12 ?f3 ?f0 ?f3 Z M ?f0 ?f8 L ?f12 ?f8 ?f1 ?f2 M ?f12 ?f8 L ?f12 ?f3 N";
		text_areas		= L"?f0 ?f8 ?f12 ?f3";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f14 ?f2 ?f13 ?f8 ?f0 ?f11 ?f13 ?f3 ?f12 ?f11 ?f1 ?f10";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"$0");
		add(L"f8", L"?f6 * ?f7 / 100000");
		add(L"f9", L"?f3 - ?f8");
		add(L"f10", L"?f9 / 2");
		add(L"f11", L"(?f8 + ?f3) / 2");
		add(L"f12", L"?f1 - ?f8");
		add(L"f13", L"?f12 / 2");
		add(L"f14", L"(?f8 + ?f1) / 2");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_Can : public oox_shape
{
public:
	oox_shape_Can()
	{
		odf_type_name	=L"ooxml-can";

		modifiers		= L"25000";
		enhanced_path	= L"S M ?f2 ?f13 A ?f55 ?f56 ?f57 ?f58 ?f2 ?f13 ?f52 ?f54  W ?f59 ?f60 ?f61 ?f62 ?f2 ?f13 ?f52 ?f54 L ?f3 ?f15 A ?f102 ?f103 ?f104 ?f105 ?f3 ?f15 ?f99 ?f101  W ?f106 ?f107 ?f108 ?f109 ?f3 ?f15 ?f99 ?f101 Z N S M ?f2 ?f13 A ?f130 ?f131 ?f132 ?f133 ?f2 ?f13 ?f127 ?f129  W ?f134 ?f135 ?f136 ?f137 ?f2 ?f13 ?f127 ?f129 Z N F M ?f3 ?f13 A ?f157 ?f158 ?f159 ?f160 ?f3 ?f13 ?f154 ?f156  W ?f161 ?f162 ?f163 ?f164 ?f3 ?f13 ?f154 ?f156 L ?f3 ?f15 A ?f102 ?f103 ?f104 ?f105 ?f3 ?f15 ?f99 ?f101  W ?f106 ?f107 ?f108 ?f109 ?f3 ?f15 ?f99 ?f101 L ?f2 ?f13 N";
		text_areas		= L"?f0 ?f8 ?f12 ?f3";
		view_box		= L"0 0 21600 21600";
		glue_points		= L"?f9 ?f14";
		
		add(L"f0", L"10800000");
		add(L"f1", L"5400000");
		add(L"f2", L"left");
		add(L"f3", L"right");
		add(L"f4", L"top");
		add(L"f5", L"bottom");
		add(L"f6", L"?f5 - ?f4");
		add(L"f7", L"?f3 - ?f2");
		add(L"f8", L"?f7 / 2");
		add(L"f9", L"?f2 + ?f8");
		add(L"f10", L"min(?f7, ?f6)");
		add(L"f11", L"5419351 / 1725033");
		add(L"f12", L"$0");
		add(L"f13", L"?f10 * ?f12 / 200000");
		add(L"f14", L"?f13 + ?f13");
		add(L"f15", L"?f5 - ?f13");
		add(L"f16", L"21550000 - -10800000");
		add(L"f17", L"if(?f16, -10800000, 21550000)");
		add(L"f18", L"-21550000 - ?f17");
		add(L"f19", L"if(?f18, -21550000, ?f17)");
		add(L"f20", L"?f0 + ?f19");
		add(L"f21", L"?f0 + ?f1");
		add(L"f22", L"?f21 * ?f11 / ?f0");
		add(L"f23", L"0 - ?f22");
		add(L"f24", L"cos(?f23)");
		add(L"f25", L"0 - ?f24");
		add(L"f26", L"?f25 * ?f8");
		add(L"f27", L"sin(?f23)");
		add(L"f28", L"0 - ?f27");
		add(L"f29", L"?f28 * ?f13");
		add(L"f30", L"sqrt(?f26 * ?f26 + ?f29 * ?f29 + 0 * 0)");
		add(L"f31", L"?f8 * ?f13 / ?f30");
		add(L"f32", L"?f28 * ?f31");
		add(L"f33", L"?f2 - ?f32");
		add(L"f34", L"?f25 * ?f31");
		add(L"f35", L"?f13 - ?f34");
		add(L"f36", L"?f33 - ?f8");
		add(L"f37", L"?f35 - ?f13");
		add(L"f38", L"?f33 + ?f8");
		add(L"f39", L"?f35 + ?f13");
		add(L"f40", L"?f20 + ?f1");
		add(L"f41", L"?f40 * ?f11 / ?f0");
		add(L"f42", L"0 - ?f41");
		add(L"f43", L"cos(?f42)");
		add(L"f44", L"0 - ?f43");
		add(L"f45", L"?f44 * ?f8");
		add(L"f46", L"sin(?f42)");
		add(L"f47", L"0 - ?f46");
		add(L"f48", L"?f47 * ?f13");
		add(L"f49", L"sqrt(?f45 * ?f45 + ?f48 * ?f48 + 0 * 0)");
		add(L"f50", L"?f8 * ?f13 / ?f49");
		add(L"f51", L"?f47 * ?f50");
		add(L"f52", L"?f33 + ?f51");
		add(L"f53", L"?f44 * ?f50");
		add(L"f54", L"?f35 + ?f53");
		add(L"f55", L"if(?f19, ?f2, ?f36)");
		add(L"f56", L"if(?f19, ?f13, ?f37)");
		add(L"f57", L"if(?f19, ?f2, ?f38)");
		add(L"f58", L"if(?f19, ?f13, ?f39)");
		add(L"f59", L"if(?f19, ?f36, ?f52)");
		add(L"f60", L"if(?f19, ?f37, ?f54)");
		add(L"f61", L"if(?f19, ?f38, ?f52)");
		add(L"f62", L"if(?f19, ?f39, ?f54)");
		add(L"f63", L"21550000 - ?f0");
		add(L"f64", L"if(?f63, ?f0, 21550000)");
		add(L"f65", L"-21550000 - ?f64");
		add(L"f66", L"if(?f65, -21550000, ?f64)");
		add(L"f67", L"0 + ?f66");
		add(L"f68", L"0 + ?f1");
		add(L"f69", L"?f68 * ?f11 / ?f0");
		add(L"f70", L"0 - ?f69");
		add(L"f71", L"cos(?f70)");
		add(L"f72", L"0 - ?f71");
		add(L"f73", L"?f72 * ?f8");
		add(L"f74", L"sin(?f70)");
		add(L"f75", L"0 - ?f74");
		add(L"f76", L"?f75 * ?f13");
		add(L"f77", L"sqrt(?f73 * ?f73 + ?f76 * ?f76 + 0 * 0)");
		add(L"f78", L"?f8 * ?f13 / ?f77");
		add(L"f79", L"?f75 * ?f78");
		add(L"f80", L"?f3 - ?f79");
		add(L"f81", L"?f72 * ?f78");
		add(L"f82", L"?f15 - ?f81");
		add(L"f83", L"?f80 - ?f8");
		add(L"f84", L"?f82 - ?f13");
		add(L"f85", L"?f80 + ?f8");
		add(L"f86", L"?f82 + ?f13");
		add(L"f87", L"?f67 + ?f1");
		add(L"f88", L"?f87 * ?f11 / ?f0");
		add(L"f89", L"0 - ?f88");
		add(L"f90", L"cos(?f89)");
		add(L"f91", L"0 - ?f90");
		add(L"f92", L"?f91 * ?f8");
		add(L"f93", L"sin(?f89)");
		add(L"f94", L"0 - ?f93");
		add(L"f95", L"?f94 * ?f13");
		add(L"f96", L"sqrt(?f92 * ?f92 + ?f95 * ?f95 + 0 * 0)");
		add(L"f97", L"?f8 * ?f13 / ?f96");
		add(L"f98", L"?f94 * ?f97");
		add(L"f99", L"?f80 + ?f98");
		add(L"f100", L"?f91 * ?f97");
		add(L"f101", L"?f82 + ?f100");
		add(L"f102", L"if(?f66, ?f3, ?f83)");
		add(L"f103", L"if(?f66, ?f15, ?f84)");
		add(L"f104", L"if(?f66, ?f3, ?f85)");
		add(L"f105", L"if(?f66, ?f15, ?f86)");
		add(L"f106", L"if(?f66, ?f83, ?f99)");
		add(L"f107", L"if(?f66, ?f84, ?f101)");
		add(L"f108", L"if(?f66, ?f85, ?f99)");
		add(L"f109", L"if(?f66, ?f86, ?f101)");
		add(L"f110", L"21550000 - 21600000");
		add(L"f111", L"if(?f110, 21600000, 21550000)");
		add(L"f112", L"-21550000 - ?f111");
		add(L"f113", L"if(?f112, -21550000, ?f111)");
		add(L"f114", L"?f0 + ?f113");
		add(L"f115", L"?f114 + ?f1");
		add(L"f116", L"?f115 * ?f11 / ?f0");
		add(L"f117", L"0 - ?f116");
		add(L"f118", L"cos(?f117)");
		add(L"f119", L"0 - ?f118");
		add(L"f120", L"?f119 * ?f8");
		add(L"f121", L"sin(?f117)");
		add(L"f122", L"0 - ?f121");
		add(L"f123", L"?f122 * ?f13");
		add(L"f124", L"sqrt(?f120 * ?f120 + ?f123 * ?f123 + 0 * 0)");
		add(L"f125", L"?f8 * ?f13 / ?f124");
		add(L"f126", L"?f122 * ?f125");
		add(L"f127", L"?f33 + ?f126");
		add(L"f128", L"?f119 * ?f125");
		add(L"f129", L"?f35 + ?f128");
		add(L"f130", L"if(?f113, ?f2, ?f36)");
		add(L"f131", L"if(?f113, ?f13, ?f37)");
		add(L"f132", L"if(?f113, ?f2, ?f38)");
		add(L"f133", L"if(?f113, ?f13, ?f39)");
		add(L"f134", L"if(?f113, ?f36, ?f127)");
		add(L"f135", L"if(?f113, ?f37, ?f129)");
		add(L"f136", L"if(?f113, ?f38, ?f127)");
		add(L"f137", L"if(?f113, ?f39, ?f129)");
		add(L"f138", L"0 + ?f113");
		add(L"f139", L"?f13 - ?f81");
		add(L"f140", L"?f139 - ?f13");
		add(L"f141", L"?f139 + ?f13");
		add(L"f142", L"?f138 + ?f1");
		add(L"f143", L"?f142 * ?f11 / ?f0");
		add(L"f144", L"0 - ?f143");
		add(L"f145", L"cos(?f144)");
		add(L"f146", L"0 - ?f145");
		add(L"f147", L"?f146 * ?f8");
		add(L"f148", L"sin(?f144)");
		add(L"f149", L"0 - ?f148");
		add(L"f150", L"?f149 * ?f13");
		add(L"f151", L"sqrt(?f147 * ?f147 + ?f150 * ?f150 + 0 * 0)");
		add(L"f152", L"?f8 * ?f13 / ?f151");
		add(L"f153", L"?f149 * ?f152");
		add(L"f154", L"?f80 + ?f153");
		add(L"f155", L"?f146 * ?f152");
		add(L"f156", L"?f139 + ?f155");
		add(L"f157", L"if(?f113, ?f3, ?f83)");
		add(L"f158", L"if(?f113, ?f13, ?f140)");
		add(L"f159", L"if(?f113, ?f3, ?f85)");
		add(L"f160", L"if(?f113, ?f13, ?f141)");
		add(L"f161", L"if(?f113, ?f83, ?f154)");
		add(L"f162", L"if(?f113, ?f140, ?f156)");
		add(L"f163", L"if(?f113, ?f85, ?f154)");
		add(L"f164", L"if(?f113, ?f141, ?f156)");
/////////////////////////////////////////////////////////
	}
};
}
