#include "precompiled_cpodf.h"
#include "charterrorcategory.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const chart_error_category & _Val)
{
	switch(_Val.get_type())
	{
	case   chart_error_category::none:			_Wostream <<  L"none"; break;
	case   chart_error_category::variance:		 _Wostream <<  L"variance"; break;
	case   chart_error_category::standard_deviation: _Wostream <<  L"standard-deviation"; break;
	case   chart_error_category::percentage:	 _Wostream <<  L"percentage"; break;
	case   chart_error_category::error_margin:	 _Wostream <<  L"error-margin"; break;
	case   chart_error_category::constant:		 _Wostream <<  L"constant"; break;
	}
    return _Wostream;    
}

chart_error_category chart_error_category::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return chart_error_category( none );
    else if (tmp == L"variance")
        return chart_error_category( variance );
    else if (tmp == L"standard-deviation")
        return chart_error_category( standard_deviation );
    else if (tmp == L"percentage")
        return chart_error_category( percentage );
    else if (tmp == L"error-margin")
        return chart_error_category( error_margin );
    else if (tmp == L"constant")
        return chart_error_category( constant );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_error_category( none );
    }
}

} }
