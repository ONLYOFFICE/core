#include "../precompiled_cpodf.h"
#include "fontweight.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const font_weight & _Val)
{
    switch(_Val.get_type())
    {
    case font_weight::WNormal:
        _Wostream << "normal";
        break;
    case font_weight::WBold:
        _Wostream << "bold";
        break;
    case font_weight::W100:
        _Wostream << "100";
        break;
    case font_weight::W200:
        _Wostream << "200";
        break;
    case font_weight::W300:
        _Wostream << "300";
        break;
    case font_weight::W400:
        _Wostream << "400";
        break;
    case font_weight::W500:
        _Wostream << "500";
        break;
    case font_weight::W600:
        _Wostream << "600";
        break;
    case font_weight::W700:
        _Wostream << "700";
        break;
    case font_weight::W800:
        _Wostream << "800";
        break;
    case font_weight::W900:
        _Wostream << "900";
        break;
    default:
        break;
    }
    return _Wostream;
}

font_weight font_weight::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    
    if (tmp == L"normal")
        return font_weight( WNormal );
    else if (tmp == L"bold")
        return font_weight( WBold );
    else if (tmp == L"100")
        return font_weight( W100 );
    else if (tmp == L"200")
        return font_weight( W200 );
    else if (tmp == L"300")
        return font_weight( W300 );
    else if (tmp == L"400")
        return font_weight( W400 );
    else if (tmp == L"500")
        return font_weight( W500 );
    else if (tmp == L"600")
        return font_weight( W600 );
    else if (tmp == L"700")
        return font_weight( W700 );
    else if (tmp == L"800")
        return font_weight( W800 );
    else if (tmp == L"900")
        return font_weight( W900 );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
        return WNormal;
    }
}

} }

