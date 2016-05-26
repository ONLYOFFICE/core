#pragma once

#include "../../include/cpdoccore/xml/sax.h"
#include "../../include/cpdoccore/CPSharedPtr.h"

namespace cpdoccore {
namespace xml {

sax_ptr create_sax_xmllite(const wchar_t * FileName);

} 
}