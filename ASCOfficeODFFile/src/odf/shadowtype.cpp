#include "precompiled_cpodf.h"
#include "shadowtype.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const shadow_type & _Val)
{
    switch(_Val.get_type())
    {
    case shadow_type::None:
        _Wostream << L"none";
        break;
    case shadow_type::Enable:
        _Wostream << _Val.get_name();
        break;
    default:
        break;
    }
    return _Wostream;    
}

shadow_type shadow_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return shadow_type( None );
    else 
        return shadow_type( Str );

}

} }
