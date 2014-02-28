#pragma once

#include <string>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPSharedPtr.h>

#include "../odf/datatypes/lengthorpercent.h"


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
			double pos;
			std::wstring color_ref;
			_CP_OPT(double) opacity;
		};
		static oox_gradient_fill_ptr create();

		oox_gradient_fill() : style(0), angle(0)
		{
			memset(rect,0,sizeof(double)*4);
		}

		int style;
		double rect[4];
		double angle;


		std::vector<_color_position> colors;
	};
//////////////////////////////////////////////////////////////////////////////
	struct _oox_fill
	{
		_oox_fill() : type(-1){}
		oox_gradient_fill_ptr	gradient;
		oox_hatch_fill_ptr		hatch;
		oox_bitmap_fill_ptr		bitmap;
		oox_solid_fill_ptr		solid;

		_CP_OPT(double) opacity;
		int type;

		friend void oox_serialize_fill(std::wostream & strm, const _oox_fill & val);
		friend void oox_serialize_srgb(std::wostream & strm,std::wstring color,_CP_OPT(double)  opacity);
		friend void oox_serialize_srgb(std::wostream & strm,std::wstring color,_CP_OPT(odf::percent)  opacity);

		void clear()
		{
			type=-1;
			opacity = boost::none;

			gradient.reset();
			hatch.reset();
			bitmap.reset();
			solid.reset();
		}
	};

	
}
}