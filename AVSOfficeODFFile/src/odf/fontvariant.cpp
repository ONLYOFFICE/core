#include "precompiled_cpodf.h"
#include "fontvariant.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const font_variant & _Val)
{
    switch(_Val.get_type())
    {
    case font_variant::Normal:
        _Wostream << L"normal";
        break;
    case font_variant::SmallCaps:
        _Wostream << L"small-caps";
        break;
    default:
        break;
    }
    return _Wostream;    
}

font_variant font_variant::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"normal")
        return font_variant( Normal );
    else if (tmp == L"small-caps")
        return font_variant( SmallCaps );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return font_variant( Normal );
    }
}

} }
