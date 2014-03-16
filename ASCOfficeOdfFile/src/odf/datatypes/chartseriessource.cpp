#include "precompiled_cpodf.h"
#include "chartseriessource.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const chart_series_source & _Val)
{
	switch(_Val.get_type())
	{
	case   chart_series_source::columns: _Wostream <<  L"columns";
	case   chart_series_source::rows: _Wostream <<  L"rows";
	}
    return _Wostream;    
}


chart_series_source chart_series_source::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"columns")
        return chart_series_source( columns );
    else if (tmp == L"rows")
        return chart_series_source( rows );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_series_source( columns );
    }
}

} }
