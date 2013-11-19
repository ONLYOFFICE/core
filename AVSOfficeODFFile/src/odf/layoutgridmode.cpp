#include "precompiled_cpodf.h"
#include "layoutgridmode.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const layout_grid_mode & _Val)
{
    switch(_Val.get_type())
    {
    case layout_grid_mode::None:
        _Wostream << L"none";
        break;
    case layout_grid_mode::Line:
        _Wostream << L"line";
        break;
    case layout_grid_mode::Both:
        _Wostream << L"both";
        break;
    default:
        break;
    }
    return _Wostream;    
}

layout_grid_mode layout_grid_mode::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return layout_grid_mode( None );
    else if (tmp == L"line")
        return layout_grid_mode( Line );
    else if (tmp == L"both")
        return layout_grid_mode( Both );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return layout_grid_mode( None );
    }
}

} }
