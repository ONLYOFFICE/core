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

		enhanced_path	= L"M ?f63 ?f64 L ?f69 ?f70 G ?f13 ?f14 ?f210 ?f211 L ?f185 ?f186 ?f57 ?f58 ?f183 ?f184 ?f126 ?f127 G ?f10 ?f12 ?f212 ?f213 Z N";
		text_areas		= L"?f206 ?f208 ?f207 ?f209";
		modifiers		= L"10980 1142322 6300000 18900000 12500";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(0-$4 ,0,if(25000-$4 ,$4 ,25000))");
		add(L"f1", L"?f0 *2/1");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"if(1-$2 ,1,if(21599999-$2 ,$2 ,21599999))");
		add(L"f4", L"if(0-$3 ,0,if(21599999-$3 ,$3 ,21599999))");
		add(L"f5", L"min(logwidth,logheight)");
		add(L"f6", L"?f5 *?f2 /100000");
		add(L"f7", L"?f5 *?f0 /100000");
		add(L"f8", L"?f6 *1/2");
		add(L"f9", L"logwidth/2");
		add(L"f10", L"?f9 +?f8 -?f7 ");
		add(L"f11", L"logheight/2");
		add(L"f12", L"?f11 +?f8 -?f7 ");
		add(L"f13", L"?f10 +0-?f6 ");
		add(L"f14", L"?f12 +0-?f6 ");
		add(L"f15", L"?f13 +?f8 -0");
		add(L"f16", L"?f14 +?f8 -0");
		add(L"f17", L"?f15 *sin(pi*(?f3 )/10800000)");
		add(L"f18", L"?f16 *cos(pi*(?f3 )/10800000)");
		add(L"f19", L"?f15 *(cos(atan2(?f17 ,?f18 )))");
		add(L"f20", L"?f16 *(sin(atan2(?f17 ,?f18 )))");
		add(L"f21", L"logwidth/2");
		add(L"f22", L"?f21 +?f19 -0");
		add(L"f23", L"logheight/2");
		add(L"f24", L"?f23 +?f20 -0");
		add(L"f25", L"min(?f13 ,?f14 )");
		add(L"f26", L"?f19 *?f19 /1");
		add(L"f27", L"?f20 *?f20 /1");
		add(L"f28", L"?f25 *?f25 /1");
		add(L"f29", L"?f26 +0-?f28 ");
		add(L"f30", L"?f27 +0-?f28 ");
		add(L"f31", L"?f29 *?f30 /?f26 ");
		add(L"f32", L"?f31 *1/?f27 ");
		add(L"f33", L"1+0-?f32 ");
		add(L"f34", L"sqrt(?f33 )");
		add(L"f35", L"?f29 *1/?f19 ");
		add(L"f36", L"?f35 *1/?f20 ");
		add(L"f37", L"(1+?f34 )/?f36 ");
		add(L"f38", L"(10800000*atan2(?f37 ,1))/pi");
		add(L"f39", L"?f38 +21600000-0");
		add(L"f40", L"if(?f38 ,?f38 ,?f39 )");
		add(L"f41", L"?f40 +0-?f3 ");
		add(L"f42", L"?f41 +21600000-0");
		add(L"f43", L"if(?f41 ,?f41 ,?f42 )");
		add(L"f44", L"?f43 +0-10800000");
		add(L"f45", L"?f43 +0-21600000");
		add(L"f46", L"if(?f44 ,?f45 ,?f43 )");
		add(L"f47", L"abs(?f46 )");
		add(L"f48", L"?f47 *-1/1");
		add(L"f49", L"abs($1 )");
		add(L"f50", L"?f49 *-1/1");
		add(L"f51", L"if(?f48 -?f50 ,?f48 ,if(0-?f50 ,?f50 ,0))");
		add(L"f52", L"?f3 +?f51 -0");
		add(L"f53", L"?f15 *sin(pi*(?f52 )/10800000)");
		add(L"f54", L"?f16 *cos(pi*(?f52 )/10800000)");
		add(L"f55", L"?f15 *(cos(atan2(?f53 ,?f54 )))");
		add(L"f56", L"?f16 *(sin(atan2(?f53 ,?f54 )))");
		add(L"f57", L"?f21 +?f55 -0");
		add(L"f58", L"?f23 +?f56 -0");
		add(L"f59", L"?f10 *sin(pi*(?f4 )/10800000)");
		add(L"f60", L"?f12 *cos(pi*(?f4 )/10800000)");
		add(L"f61", L"?f10 *(cos(atan2(?f59 ,?f60 )))");
		add(L"f62", L"?f12 *(sin(atan2(?f59 ,?f60 )))");
		add(L"f63", L"?f21 +?f61 -0");
		add(L"f64", L"?f23 +?f62 -0");
		add(L"f65", L"?f13 *sin(pi*(?f4 )/10800000)");
		add(L"f66", L"?f14 *cos(pi*(?f4 )/10800000)");
		add(L"f67", L"?f13 *(cos(atan2(?f65 ,?f66 )))");
		add(L"f68", L"?f14 *(sin(atan2(?f65 ,?f66 )))");
		add(L"f69", L"?f21 +?f67 -0");
		add(L"f70", L"?f23 +?f68 -0");
		add(L"f71", L"?f7 *cos(pi*(?f52 )/10800000)");
		add(L"f72", L"?f7 *sin(pi*(?f52 )/10800000)");
		add(L"f73", L"?f22 +?f71 -0");
		add(L"f74", L"?f24 +?f72 -0");
		add(L"f75", L"?f7 *cos(pi*(?f52 )/10800000)");
		add(L"f76", L"?f7 *sin(pi*(?f52 )/10800000)");
		add(L"f77", L"?f22 +0-?f75 ");
		add(L"f78", L"?f24 +0-?f76 ");
		add(L"f79", L"?f77 +0-?f21 ");
		add(L"f80", L"?f78 +0-?f23 ");
		add(L"f81", L"?f73 +0-?f21 ");
		add(L"f82", L"?f74 +0-?f23 ");
		add(L"f83", L"min(?f10 ,?f12 )");
		add(L"f84", L"?f79 *?f83 /?f10 ");
		add(L"f85", L"?f80 *?f83 /?f12 ");
		add(L"f86", L"?f81 *?f83 /?f10 ");
		add(L"f87", L"?f82 *?f83 /?f12 ");
		add(L"f88", L"?f86 +0-?f84 ");
		add(L"f89", L"?f87 +0-?f85 ");
		add(L"f90", L"sqrt(?f88 *?f88 +?f89 *?f89 +0*0)");
		add(L"f91", L"?f84 *?f87 /1");
		add(L"f92", L"?f86 *?f85 /1");
		add(L"f93", L"?f91 +0-?f92 ");
		add(L"f94", L"?f83 *?f83 /1");
		add(L"f95", L"?f90 *?f90 /1");
		add(L"f96", L"?f94 *?f95 /1");
		add(L"f97", L"?f93 *?f93 /1");
		add(L"f98", L"?f96 +0-?f97 ");
		add(L"f99", L"max(?f98 ,0)");
		add(L"f100", L"sqrt(?f99 )");
		add(L"f101", L"?f89 *-1/1");
		add(L"f102", L"if(?f101 ,-1,1)");
		add(L"f103", L"?f102 *?f88 /1");
		add(L"f104", L"?f103 *?f100 /1");
		add(L"f105", L"?f93 *?f89 /1");
		add(L"f106", L"(?f105 +?f104 )/?f95 ");
		add(L"f107", L"?f105 +0-?f104 ");
		add(L"f108", L"?f107 *1/?f95 ");
		add(L"f109", L"abs(?f89 )");
		add(L"f110", L"?f109 *?f100 /1");
		add(L"f111", L"?f93 *?f88 /-1");
		add(L"f112", L"(?f111 +?f110 )/?f95 ");
		add(L"f113", L"?f111 +0-?f110 ");
		add(L"f114", L"?f113 *1/?f95 ");
		add(L"f115", L"?f86 +0-?f106 ");
		add(L"f116", L"?f86 +0-?f108 ");
		add(L"f117", L"?f87 +0-?f112 ");
		add(L"f118", L"?f87 +0-?f114 ");
		add(L"f119", L"sqrt(?f115 *?f115 +?f117 *?f117 +0*0)");
		add(L"f120", L"sqrt(?f116 *?f116 +?f118 *?f118 +0*0)");
		add(L"f121", L"?f120 +0-?f119 ");
		add(L"f122", L"if(?f121 ,?f106 ,?f108 )");
		add(L"f123", L"if(?f121 ,?f112 ,?f114 )");
		add(L"f124", L"?f122 *?f10 /?f83 ");
		add(L"f125", L"?f123 *?f12 /?f83 ");
		add(L"f126", L"?f21 +?f124 -0");
		add(L"f127", L"?f23 +?f125 -0");
		add(L"f128", L"?f79 *?f25 /?f13 ");
		add(L"f129", L"?f80 *?f25 /?f14 ");
		add(L"f130", L"?f81 *?f25 /?f13 ");
		add(L"f131", L"?f82 *?f25 /?f14 ");
		add(L"f132", L"?f130 +0-?f128 ");
		add(L"f133", L"?f131 +0-?f129 ");
		add(L"f134", L"sqrt(?f132 *?f132 +?f133 *?f133 +0*0)");
		add(L"f135", L"?f128 *?f131 /1");
		add(L"f136", L"?f130 *?f129 /1");
		add(L"f137", L"?f135 +0-?f136 ");
		add(L"f138", L"?f25 *?f25 /1");
		add(L"f139", L"?f134 *?f134 /1");
		add(L"f140", L"?f138 *?f139 /1");
		add(L"f141", L"?f137 *?f137 /1");
		add(L"f142", L"?f140 +0-?f141 ");
		add(L"f143", L"max(?f142 ,0)");
		add(L"f144", L"sqrt(?f143 )");
		add(L"f145", L"?f102 *?f132 /1");
		add(L"f146", L"?f145 *?f144 /1");
		add(L"f147", L"?f137 *?f133 /1");
		add(L"f148", L"(?f147 +?f146 )/?f139 ");
		add(L"f149", L"?f147 +0-?f146 ");
		add(L"f150", L"?f149 *1/?f139 ");
		add(L"f151", L"abs(?f133 )");
		add(L"f152", L"?f151 *?f144 /1");
		add(L"f153", L"?f137 *?f132 /-1");
		add(L"f154", L"(?f153 +?f152 )/?f139 ");
		add(L"f155", L"?f153 +0-?f152 ");
		add(L"f156", L"?f155 *1/?f139 ");
		add(L"f157", L"?f128 +0-?f148 ");
		add(L"f158", L"?f128 +0-?f150 ");
		add(L"f159", L"?f129 +0-?f154 ");
		add(L"f160", L"?f129 +0-?f156 ");
		add(L"f161", L"sqrt(?f157 *?f157 +?f159 *?f159 +0*0)");
		add(L"f162", L"sqrt(?f158 *?f158 +?f160 *?f160 +0*0)");
		add(L"f163", L"?f162 +0-?f161 ");
		add(L"f164", L"if(?f163 ,?f148 ,?f150 )");
		add(L"f165", L"if(?f163 ,?f154 ,?f156 )");
		add(L"f166", L"?f164 *?f13 /?f25 ");
		add(L"f167", L"?f165 *?f14 /?f25 ");
		add(L"f168", L"?f21 +?f166 -0");
		add(L"f169", L"?f23 +?f167 -0");
		add(L"f170", L"(10800000*atan2(?f167 ,?f166 ))/pi");
		add(L"f171", L"?f170 +21600000-0");
		add(L"f172", L"if(?f170 ,?f170 ,?f171 )");
		add(L"f173", L"?f4 +0-?f172 ");
		add(L"f174", L"?f173 +21600000-0");
		add(L"f175", L"if(?f173 ,?f173 ,?f174 )");
		add(L"f176", L"?f172 +?f175 -0");
		add(L"f177", L"0+0-?f175 ");
		add(L"f178", L"?f126 +0-?f168 ");
		add(L"f179", L"?f127 +0-?f169 ");
		add(L"f180", L"sqrt(?f178 *?f178 +?f179 *?f179 +0*0)");
		add(L"f181", L"?f180 *1/2");
		add(L"f182", L"?f181 +0-?f7 ");
		add(L"f183", L"if(?f182 ,?f126 ,?f73 )");
		add(L"f184", L"if(?f182 ,?f127 ,?f74 )");
		add(L"f185", L"if(?f182 ,?f168 ,?f77 )");
		add(L"f186", L"if(?f182 ,?f169 ,?f78 )");
		add(L"f187", L"(10800000*atan2(?f125 ,?f124 ))/pi");
		add(L"f188", L"?f187 +21600000-0");
		add(L"f189", L"if(?f187 ,?f187 ,?f188 )");
		add(L"f190", L"?f189 +0-?f4 ");
		add(L"f191", L"?f190 +0-21600000");
		add(L"f192", L"if(?f190 ,?f191 ,?f190 )");
		add(L"f193", L"?f4 +?f192 -0");
		add(L"f194", L"0+0-?f192 ");
		add(L"f195", L"?f15 *sin(pi*(?f4 )/10800000)");
		add(L"f196", L"?f16 *cos(pi*(?f4 )/10800000)");
		add(L"f197", L"?f15 *(cos(atan2(?f195 ,?f196 )))");
		add(L"f198", L"?f16 *(sin(atan2(?f195 ,?f196 )))");
		add(L"f199", L"?f21 +?f197 -0");
		add(L"f200", L"?f23 +?f198 -0");
		add(L"f201", L"?f4 +5400000-0");
		add(L"f202", L"?f52 +0-5400000");
		add(L"f203", L"?f52 +10800000-0");
		add(L"f204", L"?f10 *cos(pi*(2700000)/10800000)");
		add(L"f205", L"?f12 *sin(pi*(2700000)/10800000)");
		add(L"f206", L"?f21 +0-?f204 ");
		add(L"f207", L"?f21 +?f204 -0");
		add(L"f208", L"?f23 +0-?f205 ");
		add(L"f209", L"?f23 +?f205 -0");
		add(L"f210", L"(?f176 )/60000.0");
		add(L"f211", L"(?f177 )/60000.0");
		add(L"f212", L"(?f193 )/60000.0");
		add(L"f213", L"(?f194 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f57 ?f58";
		handles.push_back(h);

 		h.position = L"?f63 ?f64";
		handles.push_back(h);

 		h.position = L"?f126 ?f127";
		h.r_minimum = L"0";
		h.r_maximum = L"?f1";
		handles.push_back(h);

 		h.position = L"?f77 ?f78";
		h.r_minimum = L"0";
		h.r_maximum = L"25000";
		handles.push_back(h);
	}
};
class oox_shape_CircularArrow : public oox_shape
{
public:
	oox_shape_CircularArrow()
	{
		odf_type_name	= L"ooxml-circularArrow";

		enhanced_path	= L"M ?f60 ?f61 G ?f10 ?f12 ?f197 ?f198 L ?f172 ?f173 ?f54 ?f55 ?f174 ?f175 ?f159 ?f160 G ?f13 ?f14 ?f199 ?f200 Z N";
		text_areas		= L"?f193 ?f195 ?f194 ?f196";
		modifiers		= L"12500 515062 20457681 17744048 18489";
		view_box		= L"0 0 0 0";
		
		add(L"f0", L"if(0-$4 ,0,if(25000-$4 ,$4 ,25000))");
		add(L"f1", L"?f0 *2/1");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"if(1-$2 ,1,if(21599999-$2 ,$2 ,21599999))");
		add(L"f4", L"if(0-$3 ,0,if(21599999-$3 ,$3 ,21599999))");
		add(L"f5", L"min(logwidth,logheight)");
		add(L"f6", L"?f5 *?f2 /100000");
		add(L"f7", L"?f5 *?f0 /100000");
		add(L"f8", L"?f6 *1/2");
		add(L"f9", L"logwidth/2");
		add(L"f10", L"?f9 +?f8 -?f7 ");
		add(L"f11", L"logheight/2");
		add(L"f12", L"?f11 +?f8 -?f7 ");
		add(L"f13", L"?f10 +0-?f6 ");
		add(L"f14", L"?f12 +0-?f6 ");
		add(L"f15", L"?f13 +?f8 -0");
		add(L"f16", L"?f14 +?f8 -0");
		add(L"f17", L"?f15 *sin(pi*(?f3 )/10800000)");
		add(L"f18", L"?f16 *cos(pi*(?f3 )/10800000)");
		add(L"f19", L"?f15 *(cos(atan2(?f17 ,?f18 )))");
		add(L"f20", L"?f16 *(sin(atan2(?f17 ,?f18 )))");
		add(L"f21", L"logwidth/2");
		add(L"f22", L"?f21 +?f19 -0");
		add(L"f23", L"logheight/2");
		add(L"f24", L"?f23 +?f20 -0");
		add(L"f25", L"min(?f13 ,?f14 )");
		add(L"f26", L"?f19 *?f19 /1");
		add(L"f27", L"?f20 *?f20 /1");
		add(L"f28", L"?f25 *?f25 /1");
		add(L"f29", L"?f26 +0-?f28 ");
		add(L"f30", L"?f27 +0-?f28 ");
		add(L"f31", L"?f29 *?f30 /?f26 ");
		add(L"f32", L"?f31 *1/?f27 ");
		add(L"f33", L"1+0-?f32 ");
		add(L"f34", L"sqrt(?f33 )");
		add(L"f35", L"?f29 *1/?f19 ");
		add(L"f36", L"?f35 *1/?f20 ");
		add(L"f37", L"(1+?f34 )/?f36 ");
		add(L"f38", L"(10800000*atan2(?f37 ,1))/pi");
		add(L"f39", L"?f38 +21600000-0");
		add(L"f40", L"if(?f38 ,?f38 ,?f39 )");
		add(L"f41", L"?f40 +0-?f3 ");
		add(L"f42", L"?f41 +21600000-0");
		add(L"f43", L"if(?f41 ,?f41 ,?f42 )");
		add(L"f44", L"?f43 +0-10800000");
		add(L"f45", L"?f43 +0-21600000");
		add(L"f46", L"if(?f44 ,?f45 ,?f43 )");
		add(L"f47", L"abs(?f46 )");
		add(L"f48", L"if(0-$1 ,0,if(?f47 -$1 ,$1 ,?f47 ))");
		add(L"f49", L"?f3 +?f48 -0");
		add(L"f50", L"?f15 *sin(pi*(?f49 )/10800000)");
		add(L"f51", L"?f16 *cos(pi*(?f49 )/10800000)");
		add(L"f52", L"?f15 *(cos(atan2(?f50 ,?f51 )))");
		add(L"f53", L"?f16 *(sin(atan2(?f50 ,?f51 )))");
		add(L"f54", L"?f21 +?f52 -0");
		add(L"f55", L"?f23 +?f53 -0");
		add(L"f56", L"?f10 *sin(pi*(?f4 )/10800000)");
		add(L"f57", L"?f12 *cos(pi*(?f4 )/10800000)");
		add(L"f58", L"?f10 *(cos(atan2(?f56 ,?f57 )))");
		add(L"f59", L"?f12 *(sin(atan2(?f56 ,?f57 )))");
		add(L"f60", L"?f21 +?f58 -0");
		add(L"f61", L"?f23 +?f59 -0");
		add(L"f62", L"?f7 *cos(pi*(?f49 )/10800000)");
		add(L"f63", L"?f7 *sin(pi*(?f49 )/10800000)");
		add(L"f64", L"?f22 +?f62 -0");
		add(L"f65", L"?f24 +?f63 -0");
		add(L"f66", L"?f7 *cos(pi*(?f49 )/10800000)");
		add(L"f67", L"?f7 *sin(pi*(?f49 )/10800000)");
		add(L"f68", L"?f22 +0-?f66 ");
		add(L"f69", L"?f24 +0-?f67 ");
		add(L"f70", L"?f68 +0-?f21 ");
		add(L"f71", L"?f69 +0-?f23 ");
		add(L"f72", L"?f64 +0-?f21 ");
		add(L"f73", L"?f65 +0-?f23 ");
		add(L"f74", L"min(?f10 ,?f12 )");
		add(L"f75", L"?f70 *?f74 /?f10 ");
		add(L"f76", L"?f71 *?f74 /?f12 ");
		add(L"f77", L"?f72 *?f74 /?f10 ");
		add(L"f78", L"?f73 *?f74 /?f12 ");
		add(L"f79", L"?f77 +0-?f75 ");
		add(L"f80", L"?f78 +0-?f76 ");
		add(L"f81", L"sqrt(?f79 *?f79 +?f80 *?f80 +0*0)");
		add(L"f82", L"?f75 *?f78 /1");
		add(L"f83", L"?f77 *?f76 /1");
		add(L"f84", L"?f82 +0-?f83 ");
		add(L"f85", L"?f74 *?f74 /1");
		add(L"f86", L"?f81 *?f81 /1");
		add(L"f87", L"?f85 *?f86 /1");
		add(L"f88", L"?f84 *?f84 /1");
		add(L"f89", L"?f87 +0-?f88 ");
		add(L"f90", L"max(?f89 ,0)");
		add(L"f91", L"sqrt(?f90 )");
		add(L"f92", L"?f80 *-1/1");
		add(L"f93", L"if(?f92 ,-1,1)");
		add(L"f94", L"?f93 *?f79 /1");
		add(L"f95", L"?f94 *?f91 /1");
		add(L"f96", L"?f84 *?f80 /1");
		add(L"f97", L"(?f96 +?f95 )/?f86 ");
		add(L"f98", L"?f96 +0-?f95 ");
		add(L"f99", L"?f98 *1/?f86 ");
		add(L"f100", L"abs(?f80 )");
		add(L"f101", L"?f100 *?f91 /1");
		add(L"f102", L"?f84 *?f79 /-1");
		add(L"f103", L"(?f102 +?f101 )/?f86 ");
		add(L"f104", L"?f102 +0-?f101 ");
		add(L"f105", L"?f104 *1/?f86 ");
		add(L"f106", L"?f77 +0-?f97 ");
		add(L"f107", L"?f77 +0-?f99 ");
		add(L"f108", L"?f78 +0-?f103 ");
		add(L"f109", L"?f78 +0-?f105 ");
		add(L"f110", L"sqrt(?f106 *?f106 +?f108 *?f108 +0*0)");
		add(L"f111", L"sqrt(?f107 *?f107 +?f109 *?f109 +0*0)");
		add(L"f112", L"?f111 +0-?f110 ");
		add(L"f113", L"if(?f112 ,?f97 ,?f99 )");
		add(L"f114", L"if(?f112 ,?f103 ,?f105 )");
		add(L"f115", L"?f113 *?f10 /?f74 ");
		add(L"f116", L"?f114 *?f12 /?f74 ");
		add(L"f117", L"?f21 +?f115 -0");
		add(L"f118", L"?f23 +?f116 -0");
		add(L"f119", L"?f70 *?f25 /?f13 ");
		add(L"f120", L"?f71 *?f25 /?f14 ");
		add(L"f121", L"?f72 *?f25 /?f13 ");
		add(L"f122", L"?f73 *?f25 /?f14 ");
		add(L"f123", L"?f121 +0-?f119 ");
		add(L"f124", L"?f122 +0-?f120 ");
		add(L"f125", L"sqrt(?f123 *?f123 +?f124 *?f124 +0*0)");
		add(L"f126", L"?f119 *?f122 /1");
		add(L"f127", L"?f121 *?f120 /1");
		add(L"f128", L"?f126 +0-?f127 ");
		add(L"f129", L"?f25 *?f25 /1");
		add(L"f130", L"?f125 *?f125 /1");
		add(L"f131", L"?f129 *?f130 /1");
		add(L"f132", L"?f128 *?f128 /1");
		add(L"f133", L"?f131 +0-?f132 ");
		add(L"f134", L"max(?f133 ,0)");
		add(L"f135", L"sqrt(?f134 )");
		add(L"f136", L"?f93 *?f123 /1");
		add(L"f137", L"?f136 *?f135 /1");
		add(L"f138", L"?f128 *?f124 /1");
		add(L"f139", L"(?f138 +?f137 )/?f130 ");
		add(L"f140", L"?f138 +0-?f137 ");
		add(L"f141", L"?f140 *1/?f130 ");
		add(L"f142", L"abs(?f124 )");
		add(L"f143", L"?f142 *?f135 /1");
		add(L"f144", L"?f128 *?f123 /-1");
		add(L"f145", L"(?f144 +?f143 )/?f130 ");
		add(L"f146", L"?f144 +0-?f143 ");
		add(L"f147", L"?f146 *1/?f130 ");
		add(L"f148", L"?f119 +0-?f139 ");
		add(L"f149", L"?f119 +0-?f141 ");
		add(L"f150", L"?f120 +0-?f145 ");
		add(L"f151", L"?f120 +0-?f147 ");
		add(L"f152", L"sqrt(?f148 *?f148 +?f150 *?f150 +0*0)");
		add(L"f153", L"sqrt(?f149 *?f149 +?f151 *?f151 +0*0)");
		add(L"f154", L"?f153 +0-?f152 ");
		add(L"f155", L"if(?f154 ,?f139 ,?f141 )");
		add(L"f156", L"if(?f154 ,?f145 ,?f147 )");
		add(L"f157", L"?f155 *?f13 /?f25 ");
		add(L"f158", L"?f156 *?f14 /?f25 ");
		add(L"f159", L"?f21 +?f157 -0");
		add(L"f160", L"?f23 +?f158 -0");
		add(L"f161", L"(10800000*atan2(?f158 ,?f157 ))/pi");
		add(L"f162", L"?f161 +21600000-0");
		add(L"f163", L"if(?f161 ,?f161 ,?f162 )");
		add(L"f164", L"?f4 +0-?f163 ");
		add(L"f165", L"?f164 +0-21600000");
		add(L"f166", L"if(?f164 ,?f165 ,?f164 )");
		add(L"f167", L"?f117 +0-?f159 ");
		add(L"f168", L"?f118 +0-?f160 ");
		add(L"f169", L"sqrt(?f167 *?f167 +?f168 *?f168 +0*0)");
		add(L"f170", L"?f169 *1/2");
		add(L"f171", L"?f170 +0-?f7 ");
		add(L"f172", L"if(?f171 ,?f117 ,?f64 )");
		add(L"f173", L"if(?f171 ,?f118 ,?f65 )");
		add(L"f174", L"if(?f171 ,?f159 ,?f68 )");
		add(L"f175", L"if(?f171 ,?f160 ,?f69 )");
		add(L"f176", L"(10800000*atan2(?f116 ,?f115 ))/pi");
		add(L"f177", L"?f176 +21600000-0");
		add(L"f178", L"if(?f176 ,?f176 ,?f177 )");
		add(L"f179", L"?f178 +0-?f4 ");
		add(L"f180", L"?f179 +21600000-0");
		add(L"f181", L"if(?f179 ,?f179 ,?f180 )");
		add(L"f182", L"?f15 *sin(pi*(?f4 )/10800000)");
		add(L"f183", L"?f16 *cos(pi*(?f4 )/10800000)");
		add(L"f184", L"?f15 *(cos(atan2(?f182 ,?f183 )))");
		add(L"f185", L"?f16 *(sin(atan2(?f182 ,?f183 )))");
		add(L"f186", L"?f21 +?f184 -0");
		add(L"f187", L"?f23 +?f185 -0");
		add(L"f188", L"?f4 +0-5400000");
		add(L"f189", L"?f49 +5400000-0");
		add(L"f190", L"?f49 +10800000-0");
		add(L"f191", L"?f10 *cos(pi*(2700000)/10800000)");
		add(L"f192", L"?f12 *sin(pi*(2700000)/10800000)");
		add(L"f193", L"?f21 +0-?f191 ");
		add(L"f194", L"?f21 +?f191 -0");
		add(L"f195", L"?f23 +0-?f192 ");
		add(L"f196", L"?f23 +?f192 -0");
		add(L"f197", L"(?f4 )/60000.0");
		add(L"f198", L"(?f181 )/60000.0");
		add(L"f199", L"(?f163 )/60000.0");
		add(L"f200", L"(?f166 )/60000.0");
/////////////////////////////////////////////////////////
		_handle h;

		h.position = L"?f54 ?f55";
		handles.push_back(h);

 		h.position = L"?f60 ?f61";
		handles.push_back(h);

 		h.position = L"?f117 ?f118";
		h.r_minimum = L"0";
		h.r_maximum = L"?f1";
		handles.push_back(h);

 		h.position = L"?f68 ?f69";
		h.r_minimum = L"0";
		h.r_maximum = L"25000";
		handles.push_back(h);
	}
};
}