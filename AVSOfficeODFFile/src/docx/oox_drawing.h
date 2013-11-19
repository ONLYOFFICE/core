#pragma once

#include <iosfwd>
#include <iostream>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/attributes.h>

#include "mediaitems.h"

static const int _odf_to_oox_ShapeType[]=
{ 4,4,4,34,};

static const std::wstring _ooxShapeType[]=
{
	L"rect", //frame
	L"rect", //text box
	L"rect", //shape
	L"ellipse",
	L"ellipse", 
	L"line", 
	L"path",
	L"custGeom",//uses sub-sub type,
	L"polygon", 
};

namespace cpdoccore {
namespace oox {

struct _hlink_desc
{
	std::wstring hId;
	std::wstring hRef;
	bool object;
};
struct _oox_drawing
{
	_oox_drawing():type(mediaitems::typeUnknown),id(0),x(0),y(0),cx(0),cy(0),sub_type(0),clipping_enabled(false),name(L"object")
	{
	}
	mediaitems::Type type;

	size_t id;
    std::wstring name;

    std::wstring rId;
	//std::wstring fill_Id;
	//std::wstring fill_Ref_Out;
	//std::wstring fill_Ref_Inp;

    size_t x, y;
    size_t cx, cy;

	bool clipping_enabled;
	double clipping_rect[4];//0-left, 1 -top, 2- right, 3 - bottom

	int sub_type;
 
	std::vector<_hlink_desc> hlinks;
  
	std::vector<odf::_property> additional;

	friend void oox_serialize_xfrm(	std::wostream & _Wostream, _oox_drawing const & val); 
	friend void oox_serialize_shape(std::wostream & strm, _oox_drawing const & val);
	friend void oox_serialize_ln	(std::wostream & _Wostream, const std::vector<odf::_property> & val);
	friend void oox_serialize_hlink	(std::wostream & _Wostream, const std::vector<_hlink_desc> & val);
	friend void oox_serialize_clipping(std::wostream & strm, _oox_drawing const & val);


};
}
}
