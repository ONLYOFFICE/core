//	shapetypeArc,
//	shapetypeChord,
//	shapetypeCorner,
//	shapetypeCornerTabs,
//	shapetypeDecagon,
//	shapetypeDiagStripe,
//	shapetypeDodecagon,
//+	shapetypeDoubleWave,
//	shapetypeFunnel,
//	shapetypeGear6,
//	shapetypeGear9,
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
}
