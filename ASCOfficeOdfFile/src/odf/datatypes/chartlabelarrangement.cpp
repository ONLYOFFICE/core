
#include "chartlabelarrangement.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf {

std::wostream & operator << (std::wostream & _Wostream, const chart_label_arrangement & _Val)
{
	switch(_Val.get_type())
	{
		case   chart_label_arrangement::side_by_side: _Wostream <<  L"side_by_side"; break;
		case   chart_label_arrangement::stagger_even: _Wostream <<  L"stagger_even"; break;
		case   chart_label_arrangement::stagger_odd : _Wostream <<  L"stagger_odd"; break;
	}
    return _Wostream;    
}

chart_label_arrangement chart_label_arrangement::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"side-by-side")
        return chart_label_arrangement( side_by_side );
    else if (tmp == L"stagger-even")
        return chart_label_arrangement( stagger_even);
    else if (tmp == L"stagger-odd")
        return chart_label_arrangement( stagger_odd );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_label_arrangement( side_by_side );
    }
}



} }
