
#include "fontfamilygeneric.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const font_family_generic & _Val)
{
    switch(_Val.get_type())
    {
    case font_family_generic::Roman:
        _Wostream << L"roman";
        break;
    case font_family_generic::Swiss:
        _Wostream << L"swiss";
        break;
    case font_family_generic::Modern:
        _Wostream << L"modern";
        break;
    case font_family_generic::Decorative:
        _Wostream << L"decorative";
        break;
    case font_family_generic::Script:
        _Wostream << L"script";
        break;
    case font_family_generic::System:
        _Wostream << L"system";
        break;
    default:
        break;
    }
    return _Wostream;    
}

font_family_generic font_family_generic::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"roman")
        return font_family_generic( Roman );
    if (tmp == L"swiss")
        return font_family_generic( Swiss );
    if (tmp == L"modern")
        return font_family_generic( Modern );
    if (tmp == L"decorative")
        return font_family_generic( Decorative );
    if (tmp == L"script")
        return font_family_generic( Script );
    if (tmp == L"system")
        return font_family_generic( System );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return font_family_generic( Roman );
    }
}

} }
