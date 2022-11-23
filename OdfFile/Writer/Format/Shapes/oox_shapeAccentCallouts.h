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

//+	shapetypeAccentBorderCallout1,
//+	shapetypeAccentBorderCallout2,
//+	shapetypeAccentBorderCallout3,
//+ shapetypeAccentCallout1
//+ shapetypeAccentCallout2
//+ shapetypeAccentCallout3
//+ shapetypeBorderCallout1
//+ shapetypeBorderCallout2
//+ shapetypeBorderCallout3

namespace cpdoccore 
{
class oox_shape_AccentCallout1 : public oox_shape
{
public:
	oox_shape_AccentCallout1()
	{
		odf_type_name	=L"ooxml-accentCallout1";

		enhanced_path	= L"S M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f2 L ?f9 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 112500 -38333";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f13 / 100000");
		add(L"f9", L"?f5 * ?f12 / 100000");
		add(L"f10", L"?f4 * ?f15 / 100000");
		add(L"f11", L"?f5 * ?f14 / 100000");
		add(L"f12", L"$1 / ?f7");
		add(L"f13", L"$0 / ?f7");
		add(L"f14", L"$3 / ?f7");
		add(L"f15", L"$2 / ?f7");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_AccentCallout2 : public oox_shape
{
public:
	oox_shape_AccentCallout2()
	{
		odf_type_name	=L"ooxml-accentCallout2";

		enhanced_path	= L"S M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f2 L ?f9 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 ?f13 ?f12 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 18750 -16667 112500 -46667";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f15 / 100000");
		add(L"f9", L"?f5 * ?f14 / 100000");
		add(L"f10", L"?f4 * ?f17 / 100000");
		add(L"f11", L"?f5 * ?f16 / 100000");
		add(L"f12", L"?f4 * ?f19 / 100000");
		add(L"f13", L"?f5 * ?f18 / 100000");
		add(L"f14", L"$1 / ?f7");
		add(L"f15", L"$0 / ?f7");
		add(L"f16", L"$3 / ?f7");
		add(L"f17", L"$2 / ?f7");
		add(L"f18", L"$5 / ?f7");
		add(L"f19", L"$4 / ?f7");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_AccentCallout3 : public oox_shape
{
public:
	oox_shape_AccentCallout3()
	{
		odf_type_name	=L"ooxml-accentCallout3";

		enhanced_path	= L"S M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f2 L ?f9 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 ?f13 ?f12 ?f15 ?f14 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 18750 -16667 100000 -16667 112963 -8333";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f17 / 100000");
		add(L"f9", L"?f5 * ?f16 / 100000");
		add(L"f10", L"?f4 * ?f19 / 100000");
		add(L"f11", L"?f5 * ?f18 / 100000");
		add(L"f12", L"?f4 * ?f21 / 100000");
		add(L"f13", L"?f5 * ?f20 / 100000");
		add(L"f14", L"?f4 * ?f23 / 100000");
		add(L"f15", L"?f5 * ?f22 / 100000");
		add(L"f16", L"$1 / ?f7");
		add(L"f17", L"$0 / ?f7");
		add(L"f18", L"$3 / ?f7");
		add(L"f19", L"$2 / ?f7");
		add(L"f20", L"$5 / ?f7");
		add(L"f21", L"$4 / ?f7");
		add(L"f22", L"$7 / ?f7");
		add(L"f23", L"$6 / ?f7");
/////////////////////////////////////////////////////////
	}
};

class oox_shape_BorderCallout1 : public oox_shape
{
public:
	oox_shape_BorderCallout1()
	{
		odf_type_name	=L"ooxml-borderCallout1";

		enhanced_path	= L"M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 112500 -38333";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f13 / 100000");
		add(L"f9", L"?f5 * ?f12 / 100000");
		add(L"f10", L"?f4 * ?f15 / 100000");
		add(L"f11", L"?f5 * ?f14 / 100000");
		add(L"f12", L"$1 / ?f7");
		add(L"f13", L"$0 / ?f7");
		add(L"f14", L"$3 / ?f7");
		add(L"f15", L"$2 / ?f7");
/////////////////////////////////////////////////////////
	}
};

class oox_shape_BorderCallout2 : public oox_shape
{
public:
	oox_shape_BorderCallout2()
	{
		odf_type_name	=L"ooxml-borderCallout2";

		enhanced_path	= L"M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 ?f13 ?f12 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 18750 -16667 112500 -46667";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f15 / 100000");
		add(L"f9", L"?f5 * ?f14 / 100000");
		add(L"f10", L"?f4 * ?f17 / 100000");
		add(L"f11", L"?f5 * ?f16 / 100000");
		add(L"f12", L"?f4 * ?f19 / 100000");
		add(L"f13", L"?f5 * ?f18 / 100000");
		add(L"f14", L"$1 / ?f7");
		add(L"f15", L"$0 / ?f7");
		add(L"f16", L"$3 / ?f7");
		add(L"f17", L"$2 / ?f7");
		add(L"f18", L"$5 / ?f7");
		add(L"f19", L"$4 / ?f7");
/////////////////////////////////////////////////////////
	}
};

class oox_shape_BorderCallout3 : public oox_shape
{
public:
	oox_shape_BorderCallout3()
	{
		odf_type_name	=L"ooxml-borderCallout3";

		enhanced_path	= L"M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 ?f13 ?f12 ?f15 ?f14 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 18750 -16667 100000 -16667 112963 -8333";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f17 / 100000");
		add(L"f9", L"?f5 * ?f16 / 100000");
		add(L"f10", L"?f4 * ?f19 / 100000");
		add(L"f11", L"?f5 * ?f18 / 100000");
		add(L"f12", L"?f4 * ?f21 / 100000");
		add(L"f13", L"?f5 * ?f20 / 100000");
		add(L"f14", L"?f4 * ?f23 / 100000");
		add(L"f15", L"?f5 * ?f22 / 100000");
		add(L"f16", L"$1 / ?f7");
		add(L"f17", L"$0 / ?f7");
		add(L"f18", L"$3 / ?f7");
		add(L"f19", L"$2 / ?f7");
		add(L"f20", L"$5 / ?f7");
		add(L"f21", L"$4 / ?f7");
		add(L"f22", L"$7 / ?f7");
		add(L"f23", L"$6 / ?f7");
/////////////////////////////////////////////////////////
	}
};

class oox_shape_AccentBorderCallout1 : public oox_shape
{
public:
	oox_shape_AccentBorderCallout1()
	{
		odf_type_name	=L"ooxml-accentBorderCallout1";

		enhanced_path	= L"M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f2 L ?f9 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 112500 -38333";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f13 / 100000");
		add(L"f9", L"?f5 * ?f12 / 100000");
		add(L"f10", L"?f4 * ?f15 / 100000");
		add(L"f11", L"?f5 * ?f14 / 100000");
		add(L"f12", L"$1 / ?f7");
		add(L"f13", L"$0 / ?f7");
		add(L"f14", L"$3 / ?f7");
		add(L"f15", L"$2 / ?f7");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_AccentBorderCallout2 : public oox_shape
{
public:
	oox_shape_AccentBorderCallout2()
	{
		odf_type_name	=L"ooxml-accentBorderCallout2";

		enhanced_path	= L"M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f2 L ?f9 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 ?f13 ?f12 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 18750 -16667 112500 -46667";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f15 / 100000");
		add(L"f9", L"?f5 * ?f14 / 100000");
		add(L"f10", L"?f4 * ?f17 / 100000");
		add(L"f11", L"?f5 * ?f16 / 100000");
		add(L"f12", L"?f4 * ?f19 / 100000");
		add(L"f13", L"?f5 * ?f18 / 100000");
		add(L"f14", L"$1 / ?f7");
		add(L"f15", L"$0 / ?f7");
		add(L"f16", L"$3 / ?f7");
		add(L"f17", L"$2 / ?f7");
		add(L"f18", L"$5 / ?f7");
		add(L"f19", L"$4 / ?f7");
/////////////////////////////////////////////////////////
	}
};
class oox_shape_AccentBorderCallout3 : public oox_shape
{
public:
	oox_shape_AccentBorderCallout3()
	{
		odf_type_name	=L"ooxml-accentBorderCallout3";

		enhanced_path	= L"M ?f0 ?f2 L ?f1 ?f2 ?f1 ?f3 ?f0 ?f3 Z N F M ?f9 ?f2 L ?f9 ?f3 Z N F M ?f9 ?f8 L ?f11 ?f10 ?f13 ?f12 ?f15 ?f14 N";
		text_areas		= L"?f0 ?f2 ?f1 ?f3";
		view_box		= L"0 0 21600 21600";
		modifiers		= L"18750 -8333 18750 -16667 100000 -16667 112963 -8333";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"min(?f5, ?f4)");
		add(L"f7", L"?f6 / 21600");
		add(L"f8", L"?f4 * ?f17 / 100000");
		add(L"f9", L"?f5 * ?f16 / 100000");
		add(L"f10", L"?f4 * ?f19 / 100000");
		add(L"f11", L"?f5 * ?f18 / 100000");
		add(L"f12", L"?f4 * ?f21 / 100000");
		add(L"f13", L"?f5 * ?f20 / 100000");
		add(L"f14", L"?f4 * ?f23 / 100000");
		add(L"f15", L"?f5 * ?f22 / 100000");
		add(L"f16", L"$1 / ?f7");
		add(L"f17", L"$0 / ?f7");
		add(L"f18", L"$3 / ?f7");
		add(L"f19", L"$2 / ?f7");
		add(L"f20", L"$5 / ?f7");
		add(L"f21", L"$4 / ?f7");
		add(L"f22", L"$7 / ?f7");
		add(L"f23", L"$6 / ?f7");
/////////////////////////////////////////////////////////
	}
};

}