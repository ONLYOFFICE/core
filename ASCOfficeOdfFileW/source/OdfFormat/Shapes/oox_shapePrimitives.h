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
//	shapetypeArc,
//	shapetypeChord,
//	shapetypeCorner,
//	shapetypeCornerTabs,
//	shapetypeDecagon,
//	shapetypeDiagStripe,
//	shapetypeDodecagon,
//+	shapetypeDoubleWave,
//+	shapetypeFunnel,
//+	shapetypeGear6,
//+	shapetypeGear9,
//	shapetypeHalfFrame,
//	shapetypeHeptagon,
//	shapetypeNonIsoscelesTrapezoid,
//	shapetypePie,
//	shapetypePieWedge,
//+	shapetypePlaque,
//	shapetypePlaqueTabs,
//	shapetypeSquareTabs,
//	shapetypeTeardrop,

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
class oox_shape_DoubleWave : public oox_shape
{
public:
	oox_shape_DoubleWave()
	{
		odf_type_name	=L"ooxml-DoubleWave";

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

class  oox_shape_Gear6 : public oox_shape
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
class  oox_shape_Gear9 : public oox_shape
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
}
