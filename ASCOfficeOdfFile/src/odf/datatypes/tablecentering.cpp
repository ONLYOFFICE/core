
#include "tablecentering.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const table_centering & _Val)
{
    switch(_Val.get_type())
    {
    case table_centering::Horizontal:
        _Wostream << L"horizontal";
        break;
    case table_centering::Vertical:
        _Wostream << L"vertical";
        break;
    case table_centering::Both:
        _Wostream << L"both";
        break;
    case table_centering::None:
        _Wostream << L"none";
        break;
    default:
        break;
    }
    return _Wostream;
}

table_centering table_centering::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"horizontal")
        return table_centering( Horizontal );
    else if (tmp == L"vertical")
        return table_centering( Vertical );
    else if (tmp == L"both")
        return table_centering( Both );
    else if (tmp == L"none")
        return table_centering( None );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return table_centering( Horizontal );
    }
}

} }
