#ifndef _CPDOCCORE_COMMON_READSTRING_H_
#define _CPDOCCORE_COMMON_READSTRING_H_

#include <cpdoccore/CPOptional.h>
#include <lexical_cast.h>
#include "logging.h"

namespace cpdoccore { 
namespace common {

template <class T, class StringT>
typename optional<T>::Type read_string( const StringT & Value )
{
    try 
    {
        return optional<T>::Type ( ::boost::lexical_cast<T>( Value ) );
    }
    catch(...)
    {
        _CP_LOG(warning) << L"[warning] : could't convert \'" << Value << L"\' to type \'" << typeid(T).name() << L"\'\n";
        throw;
        return optional<T>::Type();
    }
}


}
}

#endif // #ifndef _CPDOCCORE_COMMON_READSTRING_H_
