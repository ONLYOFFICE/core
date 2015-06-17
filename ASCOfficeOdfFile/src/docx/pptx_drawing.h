#pragma once

#include <iosfwd>
#include <iostream>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/attributes.h>

#include "oox_drawing.h"
#include "mediaitems.h"

namespace cpdoccore {
namespace oox {

struct _pptx_drawing : _oox_drawing
{
	_pptx_drawing(): place_holder_idx_(-1){}
	std::wstring place_holder_type_;
	int place_holder_idx_;

    friend void pptx_serialize(std::wostream & _Wostream, _pptx_drawing & val);    
};

}
}