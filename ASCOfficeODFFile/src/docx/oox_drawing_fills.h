#pragma once

#include <string>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPSharedPtr.h>

#include "../odf/lengthorpercent.h"


namespace cpdoccore {
namespace oox {


	class oox_gradient_fill;
	typedef _CP_PTR(oox_gradient_fill) oox_gradient_fill_ptr;		
	class oox_gradient_fill
	{
	public:
		oox_gradient_fill(){}
		static oox_gradient_fill_ptr create();
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
	};
////////////////////////////////////////////////////////////
	class oox_solid_fill;
	typedef _CP_PTR(oox_solid_fill) oox_solid_fill_ptr;		
	class oox_solid_fill
	{
	public:
		oox_solid_fill(){}
		std::wstring	color;
		static oox_solid_fill_ptr create();
	};

	struct _oox_fill
	{

		oox_gradient_fill_ptr	gradient;
		oox_hatch_fill_ptr		hatch;
		oox_bitmap_fill_ptr		bitmap;
		oox_solid_fill_ptr		solid;

		_CP_OPT(double) opacity;
		int type;

		friend void oox_serialize_fill(std::wostream & strm, const _oox_fill & val);
		friend void oox_serialize_srgb(std::wostream & strm,std::wstring color,_CP_OPT(double)  opacity);
		friend void oox_serialize_srgb(std::wostream & strm,std::wstring color,_CP_OPT(odf::length_or_percent)  opacity);
	};

	
}
}