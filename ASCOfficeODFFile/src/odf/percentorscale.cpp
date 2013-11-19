#include "precompiled_cpodf.h"
#include "percentorscale.h"
#include "errors.h"
#include <boost_string.h>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const percent_or_scale & _Val)
{
    switch(_Val.get_type())
    {
    case percent_or_scale::Percent:
        _Wostream << _Val.get_percent();
        break;
    case percent_or_scale::Scale:
        _Wostream << L"scale";
        break;        
    case percent_or_scale::ScaleMin:
        _Wostream << L"scale-min";
        break;
    }
    return _Wostream;
}

percent_or_scale percent_or_scale::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (boost::algorithm::contains(tmp, L"%"))
    {
        try 
        {
            return percent_or_scale( percent::parse(tmp) );
        }
        catch(errors::invalid_attribute &) {}
    }

    if (tmp == L"scale")
        return percent_or_scale( Scale );
    else if (tmp == L"scale-min")
        return percent_or_scale( ScaleMin );    
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return percent_or_scale( Scale );
    }
}

} 
}

