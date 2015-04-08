#include "../precompiled_cpodf.h"
#include "hyphenationkeep.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const hyphenation_keep & _Val)
{
    switch(_Val.get_type())
    {
    case hyphenation_keep::Auto:
        _Wostream << L"auto";
        break;
    case hyphenation_keep::Page:
        _Wostream << L"page";
        break;
    default:
        break;
    }
    return _Wostream;    
}

hyphenation_keep hyphenation_keep::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"auto")
        return hyphenation_keep( Auto );
    else if (tmp == L"page")
        return hyphenation_keep( Page );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return hyphenation_keep( Auto );
    }
}

} }
