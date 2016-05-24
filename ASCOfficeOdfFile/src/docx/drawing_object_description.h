#pragma once

//#include "oox_drawing_fills.h"

namespace cpdoccore {
namespace oox {

struct _rect
{
	_rect() : x(0), y(0), cx(0), cy(0) {}
	_rect(double cx_, double cy_, double x_, double y_) : x(x_), y(y_), cx(cx_), cy(cy_) {}

    double cx;
    double cy;
    double x;
    double y;
};

struct drawing_object_description
{
	oox::mediaitems::Type					type_;

	std::wstring							name_;	
	_CP_OPT(_rect)							svg_rect_;

	std::wstring							anchor_;
    double									anchor_x_;
    double									anchor_y_;

	std::vector<_hlink_desc>				hlinks_;
	std::vector<odf_reader::_property>		additional_;	//shape properties
	
	std::wstring							xlink_href_;	//ссылка на внешний объект
	std::wstring							clipping_string_;

	_oox_fill								fill_;

	bool									use_image_replace_;
	bool									in_group_;

	int										shape_type_;	//default - frame

	std::vector<drawing_object_description>	child_objects_;
};

}
}