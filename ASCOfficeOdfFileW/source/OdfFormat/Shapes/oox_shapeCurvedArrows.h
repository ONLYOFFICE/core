//	shapetypeCurvedConnector2,
//	shapetypeCurvedConnector3,
//	shapetypeCurvedConnector4,
//	shapetypeCurvedConnector5,
//+	shapetypeCurvedDownArrow,
//+	shapetypeCurvedLeftArrow,
//	shapetypeCurvedRightArrow,
//	shapetypeCurvedUpArrow,

#include "..\oox_shape_defines.h"

namespace cpdoccore 
{
class oox_shape_curvedLeftArrow : public oox_shape
{
public:
	oox_shape_curvedLeftArrow()
	{
		odf_type_name	=L"ooxml-curvedLeftArrow";

		//modifiers		= L"23520";
		enhanced_path	= L"M ?f2 ?f8 A ?f65 ?f66 ?f67 ?f68 ?f2 ?f8 ?f62 ?f64  W ?f69 ?f70 ?f71 ?f72 ?f2 ?f8 ?f62 ?f64 Z N";
		text_areas		= L"?f22 ?f24 ?f23 ?f25";
		glue_points		= L"?f22 ?f24 ?f22 ?f25 ?f23 ?f25 ?f23 ?f24";
		view_box		= L"0 0 21600 21600";
		
		add(L"f0", L"10800000");
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
		add(L"f13", L"2700000 + ?f1");
		add(L"f14", L"?f13 * ?f12 / ?f0");
		add(L"f15", L"0 - ?f14");
		add(L"f16", L"sin(?f15)");
		add(L"f17", L"0 - ?f16");
		add(L"f18", L"?f17 * ?f10");
		add(L"f19", L"cos(?f15)");
		add(L"f20", L"0 - ?f19");
		add(L"f21", L"?f20 * ?f7");
		add(L"f22", L"?f11 - ?f18");
		add(L"f23", L"?f11 + ?f18");
		add(L"f24", L"?f8 - ?f21");
		add(L"f25", L"?f8 + ?f21");
		add(L"f26", L"21550000 - 21600000");
		add(L"f27", L"if(?f26, 21600000, 21550000)");
		add(L"f28", L"-21550000 - ?f27");
		add(L"f29", L"if(?f28, -21550000, ?f27)");
		add(L"f30", L"?f0 + ?f29");
		add(L"f31", L"?f0 + ?f1");
		add(L"f32", L"?f31 * ?f12 / ?f0");
		add(L"f33", L"0 - ?f32");
		add(L"f34", L"cos(?f33)");
		add(L"f35", L"0 - ?f34");
		add(L"f36", L"?f35 * ?f10");
		add(L"f37", L"sin(?f33)");
		add(L"f38", L"0 - ?f37");
		add(L"f39", L"?f38 * ?f7");
		add(L"f40", L"sqrt(?f36 * ?f36 + ?f39 * ?f39 + 0 * 0)");
		add(L"f41", L"?f10 * ?f7 / ?f40");
		add(L"f42", L"?f38 * ?f41");
		add(L"f43", L"?f2 - ?f42");
		add(L"f44", L"?f35 * ?f41");
		add(L"f45", L"?f8 - ?f44");
		add(L"f46", L"?f43 - ?f10");
		add(L"f47", L"?f45 - ?f7");
		add(L"f48", L"?f43 + ?f10");
		add(L"f49", L"?f45 + ?f7");
		add(L"f50", L"?f30 + ?f1");
		add(L"f51", L"?f50 * ?f12 / ?f0");
		add(L"f52", L"0 - ?f51");
		add(L"f53", L"cos(?f52)");
		add(L"f54", L"0 - ?f53");
		add(L"f55", L"?f54 * ?f10");
		add(L"f56", L"sin(?f52)");
		add(L"f57", L"0 - ?f56");
		add(L"f58", L"?f57 * ?f7");
		add(L"f59", L"sqrt(?f55 * ?f55 + ?f58 * ?f58 + 0 * 0)");
		add(L"f60", L"?f10 * ?f7 / ?f59");
		add(L"f61", L"?f57 * ?f60");
		add(L"f62", L"?f43 + ?f61");
		add(L"f63", L"?f54 * ?f60");
		add(L"f64", L"?f45 + ?f63");
		add(L"f65", L"if(?f29, ?f2, ?f46)");
		add(L"f66", L"if(?f29, ?f8, ?f47)");
		add(L"f67", L"if(?f29, ?f2, ?f48)");
		add(L"f68", L"if(?f29, ?f8, ?f49)");
		add(L"f69", L"if(?f29, ?f46, ?f62)");
		add(L"f70", L"if(?f29, ?f47, ?f64)");
		add(L"f71", L"if(?f29, ?f48, ?f62)");
		add(L"f72", L"if(?f29, ?f49, ?f64)");
/////////////////////////////////////////////////////////
		_handle h;
		h.position = L"0 ?f2";
		h.y_maximum= L"51965";
		h.y_minimum= L"0";

		handles.push_back(h);
	}
};
class oox_shape_curvedDownArrow : public oox_shape
{
public:
	oox_shape_curvedDownArrow()
	{
		odf_type_name	=L"ooxml-curvedDownArrow";

		//modifiers		= L"23520";
		enhanced_path	= L"M ?f2 ?f8 A ?f65 ?f66 ?f67 ?f68 ?f2 ?f8 ?f62 ?f64  W ?f69 ?f70 ?f71 ?f72 ?f2 ?f8 ?f62 ?f64 Z N";
		text_areas		= L"?f22 ?f24 ?f23 ?f25";
		glue_points		= L"?f22 ?f24 ?f22 ?f25 ?f23 ?f25 ?f23 ?f24";
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
		add(L"f13", L"2700000 + ?f1");
		add(L"f14", L"?f13 * ?f12 / ?f0");
		add(L"f15", L"0 - ?f14");
		add(L"f16", L"sin(?f15)");
		add(L"f17", L"0 - ?f16");
		add(L"f18", L"?f17 * ?f10");
		add(L"f19", L"cos(?f15)");
		add(L"f20", L"0 - ?f19");
		add(L"f21", L"?f20 * ?f7");
		add(L"f22", L"?f11 - ?f18");
		add(L"f23", L"?f11 + ?f18");
		add(L"f24", L"?f8 - ?f21");
		add(L"f25", L"?f8 + ?f21");
		add(L"f26", L"21550000 - 21600000");
		add(L"f27", L"if(?f26, 21600000, 21550000)");
		add(L"f28", L"-21550000 - ?f27");
		add(L"f29", L"if(?f28, -21550000, ?f27)");
		add(L"f30", L"?f0 + ?f29");
		add(L"f31", L"?f0 + ?f1");
		add(L"f32", L"?f31 * ?f12 / ?f0");
		add(L"f33", L"0 - ?f32");
		add(L"f34", L"cos(?f33)");
		add(L"f35", L"0 - ?f34");
		add(L"f36", L"?f35 * ?f10");
		add(L"f37", L"sin(?f33)");
		add(L"f38", L"0 - ?f37");
		add(L"f39", L"?f38 * ?f7");
		add(L"f40", L"sqrt(?f36 * ?f36 + ?f39 * ?f39 + 0 * 0)");
		add(L"f41", L"?f10 * ?f7 / ?f40");
		add(L"f42", L"?f38 * ?f41");
		add(L"f43", L"?f2 - ?f42");
		add(L"f44", L"?f35 * ?f41");
		add(L"f45", L"?f8 - ?f44");
		add(L"f46", L"?f43 - ?f10");
		add(L"f47", L"?f45 - ?f7");
		add(L"f48", L"?f43 + ?f10");
		add(L"f49", L"?f45 + ?f7");
		add(L"f50", L"?f30 + ?f1");
		add(L"f51", L"?f50 * ?f12 / ?f0");
		add(L"f52", L"0 - ?f51");
		add(L"f53", L"cos(?f52)");
		add(L"f54", L"0 - ?f53");
		add(L"f55", L"?f54 * ?f10");
		add(L"f56", L"sin(?f52)");
		add(L"f57", L"0 - ?f56");
		add(L"f58", L"?f57 * ?f7");
		add(L"f59", L"sqrt(?f55 * ?f55 + ?f58 * ?f58 + 0 * 0)");
		add(L"f60", L"?f10 * ?f7 / ?f59");
		add(L"f61", L"?f57 * ?f60");
		add(L"f62", L"?f43 + ?f61");
		add(L"f63", L"?f54 * ?f60");
		add(L"f64", L"?f45 + ?f63");
		add(L"f65", L"if(?f29, ?f2, ?f46)");
		add(L"f66", L"if(?f29, ?f8, ?f47)");
		add(L"f67", L"if(?f29, ?f2, ?f48)");
		add(L"f68", L"if(?f29, ?f8, ?f49)");
		add(L"f69", L"if(?f29, ?f46, ?f62)");
		add(L"f70", L"if(?f29, ?f47, ?f64)");
		add(L"f71", L"if(?f29, ?f48, ?f62)");
		add(L"f72", L"if(?f29, ?f49, ?f64)");
/////////////////////////////////////////////////////////
		_handle h;
		h.position = L"0 ?f2";
		h.y_maximum= L"51965";
		h.y_minimum= L"0";

		handles.push_back(h);
	}
};
}