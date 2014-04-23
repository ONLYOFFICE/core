#include "..\oox_shape_defines.h"
		//	shapetypeIrregularSeal1,
		//	shapetypeIrregularSeal2,
//	+	//	shapetypeSun,
		//	shapetypeWave,
namespace cpdoccore 
{
class oox_shape_sun : public oox_shape
{
public:
	oox_shape_sun()
	{
		odf_type_name	=L"ooxml-sun";

		modifiers		= L"23520";
		enhanced_path	= L"M ?f3 ?f8 L ?f41 ?f57 ?f41 ?f53 Z M ?f31 ?f32 L ?f42 ?f52 ?f43 ?f51 Z M ?f11 ?f4 L ?f44 ?f50 ?f40 ?f50 Z M ?f33 ?f32 L ?f39 ?f51 ?f38 ?f52 Z M ?f2 ?f8 L ?f37 ?f53 ?f37 ?f57 Z M ?f33 ?f34 L ?f38 ?f56 ?f39 ?f55 Z M ?f11 ?f5 L ?f40 ?f54 ?f44 ?f54 Z M ?f31 ?f34 L ?f43 ?f55 ?f42 ?f56 Z M ?f45 ?f8 A ?f97 ?f98 ?f99 ?f100 ?f45 ?f8 ?f94 ?f96  W ?f101 ?f102 ?f103 ?f104 ?f45 ?f8 ?f94 ?f96 Z N";
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
		add(L"f12", L"5419351 / 1725033");
		add(L"f13", L"$0");
		add(L"f14", L"50000 - ?f13");
		add(L"f15", L"?f14 * 30274 / 32768");
		add(L"f16", L"?f14 * 12540 / 32768");
		add(L"f17", L"50000 - ?f15");
		add(L"f18", L"50000 - ?f16");
		add(L"f19", L"?f14 * 23170 / 32768");
		add(L"f20", L"50000 + ?f19");
		add(L"f21", L"50000 - ?f19");
		add(L"f22", L"?f17 * 3 / 4");
		add(L"f23", L"?f18 * 3 / 4");
		add(L"f24", L"?f22 + 3662");
		add(L"f25", L"?f23 + 3662");
		add(L"f26", L"?f23 + 12500");
		add(L"f27", L"100000 - ?f22");
		add(L"f28", L"100000 - ?f24");
		add(L"f29", L"100000 - ?f25");
		add(L"f30", L"100000 - ?f26");
		add(L"f31", L"?f9 * 18436 / 21600");
		add(L"f32", L"?f6 * 3163 / 21600");
		add(L"f33", L"?f9 * 3163 / 21600");
		add(L"f34", L"?f6 * 18436 / 21600");
		add(L"f35", L"?f9 * ?f20 / 100000");
		add(L"f36", L"?f9 * ?f21 / 100000");
		add(L"f37", L"?f9 * ?f22 / 100000");
		add(L"f38", L"?f9 * ?f24 / 100000");
		add(L"f39", L"?f9 * ?f25 / 100000");
		add(L"f40", L"?f9 * ?f26 / 100000");
		add(L"f41", L"?f9 * ?f27 / 100000");
		add(L"f42", L"?f9 * ?f28 / 100000");
		add(L"f43", L"?f9 * ?f29 / 100000");
		add(L"f44", L"?f9 * ?f30 / 100000");
		add(L"f45", L"?f9 * ?f13 / 100000");
		add(L"f46", L"?f9 * ?f14 / 100000");
		add(L"f47", L"?f6 * ?f14 / 100000");
		add(L"f48", L"?f6 * ?f20 / 100000");
		add(L"f49", L"?f6 * ?f21 / 100000");
		add(L"f50", L"?f6 * ?f22 / 100000");
		add(L"f51", L"?f6 * ?f24 / 100000");
		add(L"f52", L"?f6 * ?f25 / 100000");
		add(L"f53", L"?f6 * ?f26 / 100000");
		add(L"f54", L"?f6 * ?f27 / 100000");
		add(L"f55", L"?f6 * ?f28 / 100000");
		add(L"f56", L"?f6 * ?f29 / 100000");
		add(L"f57", L"?f6 * ?f30 / 100000");
		add(L"f58", L"21550000 - 21600000");
		add(L"f59", L"if(?f58, 21600000, 21550000)");
		add(L"f60", L"-21550000 - ?f59");
		add(L"f61", L"if(?f60, -21550000, ?f59)");
		add(L"f62", L"?f0 + ?f61");
		add(L"f63", L"?f0 + ?f1");
		add(L"f64", L"?f63 * ?f12 / ?f0");
		add(L"f65", L"0 - ?f64");
		add(L"f66", L"cos(?f65)");
		add(L"f67", L"0 - ?f66");
		add(L"f68", L"?f67 * ?f46");
		add(L"f69", L"sin(?f65)");
		add(L"f70", L"0 - ?f69");
		add(L"f71", L"?f70 * ?f47");
		add(L"f72", L"sqrt(?f68 * ?f68 + ?f71 * ?f71 + 0 * 0)");
		add(L"f73", L"?f46 * ?f47 / ?f72");
		add(L"f74", L"?f70 * ?f73");
		add(L"f75", L"?f45 - ?f74");
		add(L"f76", L"?f67 * ?f73");
		add(L"f77", L"?f8 - ?f76");
		add(L"f78", L"?f75 - ?f46");
		add(L"f79", L"?f77 - ?f47");
		add(L"f80", L"?f75 + ?f46");
		add(L"f81", L"?f77 + ?f47");
		add(L"f82", L"?f62 + ?f1");
		add(L"f83", L"?f82 * ?f12 / ?f0");
		add(L"f84", L"0 - ?f83");
		add(L"f85", L"cos(?f84)");
		add(L"f86", L"0 - ?f85");
		add(L"f87", L"?f86 * ?f46");
		add(L"f88", L"sin(?f84)");
		add(L"f89", L"0 - ?f88");
		add(L"f90", L"?f89 * ?f47");
		add(L"f91", L"sqrt(?f87 * ?f87 + ?f90 * ?f90 + 0 * 0)");
		add(L"f92", L"?f46 * ?f47 / ?f91");
		add(L"f93", L"?f89 * ?f92");
		add(L"f94", L"?f75 + ?f93");
		add(L"f95", L"?f86 * ?f92");
		add(L"f96", L"?f77 + ?f95");
		add(L"f97", L"if(?f61, ?f45, ?f78)");
		add(L"f98", L"if(?f61, ?f8, ?f79)");
		add(L"f99", L"if(?f61, ?f45, ?f80)");
		add(L"f100", L"if(?f61, ?f8, ?f81)");
		add(L"f101", L"if(?f61, ?f78, ?f94)");
		add(L"f102", L"if(?f61, ?f79, ?f96)");
		add(L"f103", L"if(?f61, ?f80, ?f94)");
		add(L"f104", L"if(?f61, ?f81, ?f96)");
	}
};

}