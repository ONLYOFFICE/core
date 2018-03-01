/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
//+	shapetypeLeftRightRibbon
namespace cpdoccore 
{
class oox_shape_LeftRightRibbon : public oox_shape
{
public:
	oox_shape_LeftRightRibbon()
	{
		odf_type_name	=L"ooxml-leftRightRibbon";

		enhanced_path	= L"M 0 ?f16 L ?f9 0 ?f9 ?f14 ?f23 ?f14 G ?f4 ?f22 ?f30 ?f31 ?f4 ?f22 ?f32 ?f33 L ?f10 ?f21 ?f10 ?f19 ?f28 ?f17 ?f10 ?f29 ?f10 ?f15 ?f23 ?f15 G ?f4 ?f22 ?f34 ?f35 L ?f24 ?f20 ?f9 ?f20 ?f9 ?f18 Z S N M ?f25 ?f26 G ?f4 ?f22 ?f36 ?f37 ?f4 ?f22 ?f38 ?f39 L ?f25 ?f21 Z I S N M 0 ?f16 L ?f9 0 ?f9 ?f14 ?f23 ?f14 G ?f4 ?f22 ?f40 ?f41 ?f4 ?f22 ?f42 ?f43 L ?f10 ?f21 ?f10 ?f19 ?f28 ?f17 ?f10 ?f29 ?f10 ?f15 ?f23 ?f15 G ?f4 ?f22 ?f44 ?f45 L ?f24 ?f20 ?f9 ?f20 ?f9 ?f18 Z M ?f25 ?f26 L ?f25 ?f21 M ?f24 ?f27 L ?f24 ?f20 F N";
		text_areas		= L"?f9 ?f14 ?f10 ?f15";
		view_box		= L"0 0 0 0";
		modifiers		= L"50000 50000 16667";
		
		add(L"f0", L"if(0-$2 ,0,if(33333-$2 ,$2 ,33333))");
		add(L"f1", L"100000+0-?f0 ");
		add(L"f2", L"if(0-$0 ,0,if(?f1 -$0 ,$0 ,?f1 ))");
		add(L"f3", L"logwidth/2");
		add(L"f4", L"logwidth/32");
		add(L"f5", L"?f3 +0-?f4 ");
		add(L"f6", L"min(logwidth,logheight)");
		add(L"f7", L"100000*?f5 /?f6 ");
		add(L"f8", L"if(0-$1 ,0,if(?f7 -$1 ,$1 ,?f7 ))");
		add(L"f9", L"?f6 *?f8 /100000");
		add(L"f10", L"logwidth+0-?f9 ");
		add(L"f11", L"logheight*?f2 /200000");
		add(L"f12", L"logheight*?f0 /-200000");
		add(L"f13", L"logheight/2");
		add(L"f14", L"?f13 +?f12 -?f11 ");
		add(L"f15", L"?f13 +?f11 -?f12 ");
		add(L"f16", L"?f14 +?f11 -0");
		add(L"f17", L"logheight+0-?f16 ");
		add(L"f18", L"?f16 *2/1");
		add(L"f19", L"logheight+0-?f18 ");
		add(L"f20", L"?f18 +0-?f14 ");
		add(L"f21", L"logheight+0-?f20 ");
		add(L"f22", L"?f0 *?f6 /400000");
		add(L"f23", L"logwidth/2");
		add(L"f24", L"?f23 +0-?f4 ");
		add(L"f25", L"?f23 +?f4 -0");
		add(L"f26", L"?f14 +?f22 -0");
		add(L"f27", L"?f21 +0-?f22 ");
		add(L"f28", L"logwidth");
		add(L"f29", L"logheight");
		add(L"f30", L"(16200000)/60000.0");
		add(L"f31", L"(10800000)/60000.0");
		add(L"f32", L"(16200000)/60000.0");
		add(L"f33", L"(-10800000)/60000.0");
		add(L"f34", L"(5400000)/60000.0");
		add(L"f35", L"(5400000)/60000.0");
		add(L"f36", L"(0)/60000.0");
		add(L"f37", L"(5400000)/60000.0");
		add(L"f38", L"(16200000)/60000.0");
		add(L"f39", L"(-10800000)/60000.0");
		add(L"f40", L"(16200000)/60000.0");
		add(L"f41", L"(10800000)/60000.0");
		add(L"f42", L"(16200000)/60000.0");
		add(L"f43", L"(-10800000)/60000.0");
		add(L"f44", L"(5400000)/60000.0");
		add(L"f45", L"(5400000)/60000.0");
/////////////////////////////////////////////////////////
		_handle h1, h2, h3;
		h1.position = L"??f10 ?f21";
		h1.y_maximum= L"?f1";
		h1.y_minimum= L"0";
		handles.push_back(h1);

		h2.position = L"?f9 0";
		h2.x_maximum= L"?f7";
		h2.x_minimum= L"0";
		handles.push_back(h2);

		h3.position = L"?f25 ?f21";
		h3.y_maximum= L"33333";
		h3.y_minimum= L"0";
		handles.push_back(h3);
	}
};
}