#pragma once

#include <iosfwd>
#include <iostream>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/attributes.h>

#include "oox_drawing.h"
#include "xlsx_table_position.h"
#include "mediaitems.h"

namespace cpdoccore {
namespace oox {

struct xlsx_drawing_position
{
    enum type_t {from, to};
    type_t type; 
    xlsx_table_position position;

    friend void xlsx_serialize(std::wostream & _Wostream, xlsx_drawing_position const & val);
};


struct _xlsx_drawing : _oox_drawing
{
	_xlsx_drawing() : _oox_drawing(), type_anchor(1) {}
	
	int type_anchor;
    
	xlsx_drawing_position	from_;
    xlsx_drawing_position	to_;

	std::wstring			content_group_;

    
    friend void xlsx_serialize(std::wostream & _Wostream, _xlsx_drawing & val);    
};
}
}
