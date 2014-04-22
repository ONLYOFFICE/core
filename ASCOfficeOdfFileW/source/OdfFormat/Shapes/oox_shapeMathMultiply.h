#include "..\oox_shape_defines.h"

namespace cpdoccore 
{
class oox_shape_mathMultiply : public oox_shape
{
public:
	oox_shape_mathMultiply()
	{
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
}