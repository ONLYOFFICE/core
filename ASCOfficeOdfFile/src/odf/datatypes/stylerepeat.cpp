
#include "stylerepeat.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const style_repeat & _Val)
{
    switch(_Val.get_type())
    {
    case style_repeat::NoRepeat:
        _Wostream << L"no-repeat";
        break;
    case style_repeat::Repeat:
        _Wostream << L"repeat";
        break;
    case style_repeat::Stretch:
        _Wostream << L"stretch";
        break;
    default:
        break;
    }
    return _Wostream;    
}

style_repeat style_repeat::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"no-repeat")
        return style_repeat( NoRepeat );
    else if (tmp == L"repeat")
        return style_repeat( Repeat );
    else if (tmp == L"stretch")
        return style_repeat( Stretch );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return style_repeat( NoRepeat );
    }
}

} }
