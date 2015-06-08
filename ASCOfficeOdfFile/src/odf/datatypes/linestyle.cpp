#include <boost/algorithm/string.hpp>

#include "linestyle.h"
#include "errors.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const line_style & _Val)
{
    switch(_Val.get_type())
    {
    case line_style::None:
        _Wostream << "none";
        break;
    case line_style::Solid:
        _Wostream << "solid";
        break;
    case line_style::Dotted:
        _Wostream << "dotted";
        break;
    case line_style::Dash:
        _Wostream << "dash";
        break;
    case line_style::LongDash:
        _Wostream << "long-dash";
        break;
    case line_style::DotDash:
        _Wostream << "dot-dash";
        break;
    case line_style::DotDotDash:
        _Wostream << "dot-dot-dash";
        break;
    case line_style::Wave:
        _Wostream << "wave";
        break;
    default:
        break;
    }
    return _Wostream;
}

line_style line_style::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return line_style( None );
    else if (tmp == L"solid")
        return line_style( Solid );
    else if (tmp == L"dotted")
        return line_style( Dotted );
    else if (tmp == L"dash")
        return line_style( Dash );
    else if (tmp == L"long-dash")
        return line_style( LongDash );
    else if (tmp == L"dot-dash")
        return line_style( DotDash );
    else if (tmp == L"dot-dot-dash")
        return line_style( DotDotDash );
    else if (tmp == L"wave")
        return line_style( Wave );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return None;
    }
}

} }

