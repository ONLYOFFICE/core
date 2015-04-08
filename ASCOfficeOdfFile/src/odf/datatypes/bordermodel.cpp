#include "../precompiled_cpodf.h"
#include "bordermodel.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const border_model & _Val)
{
    switch(_Val.get_type())
    {
    case border_model::Collapsing:
        _Wostream << L"collapsing";
        break;
    case border_model::Separating:
        _Wostream << L"separating";
        break;
    default:
        break;
    }
    return _Wostream;    
}

border_model border_model::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"collapsing")
        return border_model( Collapsing );
    else if (tmp == L"separating")
        return border_model( Separating );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return border_model( Collapsing );
    }
}

} }
