#pragma once

#include <cpdoccore/CPOptional.h>
#include "logging.h"

namespace cpdoccore { 
namespace common {

template <class T, class StringT>
typename optional<T>::Type read_string( const StringT & Value );


}
}
