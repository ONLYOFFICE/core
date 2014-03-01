#include "precompiled_cpodf.h"
#include "style_ref.h"

#include <iostream>
#include <sstream>
#include <lexical_cast.h>
#include <cpdoccore/CPAssert.h>

namespace cpdoccore { 
namespace odf { 


std::wostream & operator << (std::wostream & _Wostream, const style_ref & _styleRef)
{
    _Wostream << _styleRef.style_name();
    return _Wostream;
}

}
}