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
#pragma once

#include <string>

#include "../../DataTypes/lengthorpercent.h"

namespace cpdoccore {
namespace oox {


	class oox_solid_fill;
	typedef _CP_PTR(oox_solid_fill) oox_solid_fill_ptr;		
	
	class oox_solid_fill
	{
	public:
		std::wstring	color;
		static oox_solid_fill_ptr create();
	};
	
///////////////////////////////////	
	class oox_bitmap_fill;
	typedef _CP_PTR(oox_bitmap_fill) oox_bitmap_fill_ptr;		
	
	class oox_bitmap_fill
	{
	public:
		oox_bitmap_fill();
		
		static oox_bitmap_fill_ptr create();
		
		std::wstring	xlink_href_;
		bool			isInternal;
		std::wstring	name_space;
		std::wstring	rId;
		bool			bStretch;
		bool			bCrop;
		bool			bTile;

		double			cropRect[4];//0-left, 1 -top, 2- right, 3 - bottom
		
		_CP_OPT(int)	dpi;
		_CP_OPT(bool)	rotate;

		bool			bGrayscale;
		_CP_OPT(double)	luminance;
		_CP_OPT(double)	contrast;
	};
/////////////////////////////////////////////////////////
	class oox_hatch_fill;
	typedef _CP_PTR(oox_hatch_fill) oox_hatch_fill_ptr;	
	
	class oox_hatch_fill
	{
	public:
		oox_hatch_fill(){}
		static oox_hatch_fill_ptr create();

		std::wstring color_ref;
		_CP_OPT(std::wstring) color_back_ref;

		std::wstring preset;
	};
////////////////////////////////////////////////////////////
	class oox_gradient_fill;
	typedef _CP_PTR(oox_gradient_fill) oox_gradient_fill_ptr;		
	
	class oox_gradient_fill
	{
	public:
		struct _color_position
		{
			double			pos;
			std::wstring	color_ref;
			_CP_OPT(double) opacity;
		};
		static oox_gradient_fill_ptr create();

		oox_gradient_fill() : style(0), angle(90)//from top to bottom
		{
			memset(rect, 0, sizeof(double) * 4);
		}

		int		style;
		double	rect[4];
		double	angle;


		std::vector<_color_position> colors;
	};
//////////////////////////////////////////////////////////////////////////////
	struct _oox_fill
	{
		int type = -1;

		oox_gradient_fill_ptr	gradient;
		oox_hatch_fill_ptr		hatch;
		oox_bitmap_fill_ptr		bitmap;
		oox_solid_fill_ptr		solid;

		_CP_OPT(double) opacity;

		void clear()
		{
			type = -1;
			opacity = boost::none;

			gradient.reset();
			hatch.reset();
			bitmap.reset();
			solid.reset();
		}
	};

    void vml_serialize_background	(std::wostream & strm, const _oox_fill & val, const std::wstring &color, int id);
   
	void oox_serialize_srgb			(std::wostream & strm, const std::wstring &color, _CP_OPT(double)  opacity, const std::wstring &ns = L"a");
    void oox_serialize_srgb			(std::wostream & strm, const std::wstring &color, _CP_OPT(odf_types::percent)  opacity, const std::wstring &ns = L"a");
	void oox_serialize_bitmap_fill	(std::wostream & strm, const _oox_fill & val, const std::wstring &ns = L"a");
	
    void oox_serialize_fill			(std::wostream & strm, const _oox_fill & val, const std::wstring &ns = L"a");
    void vml_serialize_fill			(std::wostream & strm, const _oox_fill & val);
}
}
