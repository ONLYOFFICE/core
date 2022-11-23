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

namespace cpdoccore 
{
class oox_shape_textPlain : public oox_shape
{
public:
	oox_shape_textPlain()
	{
		odf_type_name	=L"fontwork-plain-text";

		modifiers		= L"10800";
		enhanced_path	= L"M ?f3 0 L ?f5 0 N M ?f6 21600 L ?f7 21600 N";
		text_areas		= L"0 0 21600 21600";
		view_box		= L"0 0 21600 21600";
		
        add(L"f0", L"$0 -10800");
        add(L"f1", L"?f0 *2");
        add(L"f2", L"abs(?f1 )");
        add(L"f3", L"if(?f1 ,0,?f2 )");
        add(L"f4", L"21600-?f2 ");
        add(L"f5", L"if(?f1 ,?f4 ,21600)");
        add(L"f6", L"if(?f1 ,?f2 ,0)");
        add(L"f7", L"if(?f1 ,21600,?f4 )");
	}
};
class oox_shape_textCirclePour : public oox_shape
{
public:
	oox_shape_textCirclePour ()
	{
		odf_type_name	=L"fontwork-circle-pour";

		modifiers		= L"-179 5400";
		enhanced_path	= L"W 0 0 21600 21600 ?f2 ?f3 ?f2 ?f4 N W ?f5 ?f5 ?f11 ?f11 ?f8 ?f9 ?f8 ?f10 N";
		text_areas		= L"0 0 21600 21600";
		view_box		= L"0 0 21600 21600";
		
        add(L"f0", L"10800*cos($0 *(pi/180))");
        add(L"f1", L"10800*sin($0 *(pi/180))");
        add(L"f2", L"?f0 +10800");
        add(L"f3", L"?f1 +10800");
        add(L"f4", L"21600-?f3 ");
        add(L"f5", L"10800-$1 ");
        add(L"f6", L"?f5 *cos($0 *(pi/180))");
        add(L"f7", L"?f5 *sin($0 *(pi/180))");
        add(L"f8", L"?f6 +10800");
        add(L"f9", L"?f7 +10800");
        add(L"f10", L"21600-?f9 ");
        add(L"f11", L"21600-?f5 ");
        add(L"f12", L"21600");
//<draw:handle draw:handle-position="$1 $0" draw:handle-polar="10800 10800" draw:handle-radius-range-minimum="0" draw:handle-radius-range-maximum="10800");

	}
};
class oox_shape_textSlantUp : public oox_shape
{
public:
	oox_shape_textSlantUp()
	{
		odf_type_name	=L"fontwork-slant-up";

		modifiers		= L"50000";
		enhanced_path	= L"M ?f20 ?f2 L ?f21 ?f2 N M ?f22 ?f3 L ?f23 ?f3 N";
		text_areas		= L"?f24 ?f26 ?f25 ?f27";
		view_box		= L"0 0 21600 21600";
		
		add(L"f0", L"left");
		add(L"f1", L"right");
		add(L"f2", L"top");
		add(L"f3", L"bottom");
		add(L"f4", L"?f3 - ?f2");
		add(L"f5", L"?f1 - ?f0");
		add(L"f6", L"?f5 / 2");
		add(L"f7", L"?f0 + ?f6");
		add(L"f8", L"?f5 / 21600");
		add(L"f9", L"?f4 / 21600");
		add(L"f10", L"$0");
		add(L"f11", L"?f10 * ?f5 / 100000");
		add(L"f12", L"?f11 - ?f7");
		add(L"f13", L"?f11 - ?f0");
		add(L"f14", L"?f1 - ?f11");
		add(L"f15", L"?f13 * 2");
		add(L"f16", L"?f14 * 2");
		add(L"f17", L"if(?f12, ?f16, ?f15)");
		add(L"f18", L"?f0 + ?f17");
		add(L"f19", L"?f1 - ?f17");
		add(L"f20", L"if(?f12, ?f0, ?f19)");
		add(L"f21", L"if(?f12, ?f18, ?f1)");
		add(L"f22", L"if(?f12, ?f19, ?f0)");
		add(L"f23", L"if(?f12, ?f1, ?f18)");
		add(L"f24", L"0 / ?f8");
		add(L"f25", L"21600 / ?f8");
		add(L"f26", L"0 / ?f9");
		add(L"f27", L"21600 / ?f9");
	}
};
}
