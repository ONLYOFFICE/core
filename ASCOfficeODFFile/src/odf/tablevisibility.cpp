#include "precompiled_cpodf.h"
#include "tablevisibility.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const table_visibility & _Val)
{
    switch(_Val.get_type())
    {
    case table_visibility::Visible:
        _Wostream << L"visible";
        break;
    case table_visibility::Collapse:
        _Wostream << L"collapse";
        break;
    case table_visibility::Filter:
        _Wostream << L"filter";
        break;
    default:
        break;
    }
    return _Wostream;    
}

table_visibility table_visibility::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"visible")
        return table_visibility( Visible );
    else if (tmp == L"collapse")
        return table_visibility( Collapse );
    else if (tmp == L"filter")
        return table_visibility( Filter );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return table_visibility( Visible );
    }
}

} }
