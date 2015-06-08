
#include "style_ref.h"

#include <iostream>
#include <sstream>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore { 
namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const style_ref & _styleRef)
{
    _Wostream << _styleRef.style_name();
    return _Wostream;
}

}
}