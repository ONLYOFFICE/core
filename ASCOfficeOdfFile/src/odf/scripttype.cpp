#include "precompiled_cpodf.h"
#include "scripttype.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const script_type & _Val)
{
    switch(_Val.get_type())
    {
    case script_type::Latin:
        _Wostream << L"latin";
        break;
    case script_type::Asian:
        _Wostream << L"asian";
        break;
    case script_type::Complex:
        _Wostream << L"complex";
        break;
    case script_type::Ignore:
        _Wostream << L"ignore";
        break;
    default:
        break;
    }
    return _Wostream;    
}

script_type script_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"latin")
        return script_type( Latin );
    if (tmp == L"asian")
        return script_type( Asian );
    if (tmp == L"complex")
        return script_type( Complex );
    if (tmp == L"ignore")
        return script_type( Ignore );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return script_type( Latin );
    }
}

} }
