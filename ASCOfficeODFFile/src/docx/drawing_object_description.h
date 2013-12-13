#pragma once

//#include "oox_drawing_fills.h"

namespace cpdoccore {
namespace oox {

struct _rect
{
    double width_;
    double height_;
    double x_;
    double y_;
};

//+ 3-х мерный объект

struct drawing_object_description
{
	std::wstring draw_name_;
	
	_CP_OPT(_rect) svg_rect_;

	std::wstring anchor_;
    double anchor_x_;
    double anchor_y_;

	std::vector<_hlink_desc> hlinks_;

	std::vector<odf::_property> additional_;//for shapes
	
	std::wstring xlink_href_;//ссылка на внешний объект
	std::wstring clipping_string_;

	_oox_fill fill_;

	int type_;	//default - frame
};

}
}