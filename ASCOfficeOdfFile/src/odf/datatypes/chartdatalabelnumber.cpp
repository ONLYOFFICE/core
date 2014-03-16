#include "precompiled_cpodf.h"
#include "chartdatalabelnumber.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const chart_data_label_number & _Val)
{
	switch(_Val.get_type())
	{
	case   chart_data_label_number::none: _Wostream <<  L"none"; break;
	case   chart_data_label_number::value: _Wostream <<  L"value"; break;
	case   chart_data_label_number::percentage : _Wostream <<  L"percentage"; break;
	}
    return _Wostream;    
}

chart_data_label_number chart_data_label_number::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return chart_data_label_number( none );
    else if (tmp == L"value")
        return chart_data_label_number( value );
    else if (tmp == L"percentage")
        return chart_data_label_number( percentage );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_data_label_number( none );
    }
}

} }
