#include "precompiled_cpodf.h"
#include "chartsolidtype.h"

#include <ostream>

namespace cpdoccore { namespace odf {

std::wostream & operator << (std::wostream & _Wostream, const chart_solid_type & _Val)
{
	switch(_Val.get_type())
	{
	case   chart_solid_type::cuboid:	_Wostream <<  L"cuboid"; break;
	case   chart_solid_type::cylinder:  _Wostream <<  L"cylinder"; break;
	case   chart_solid_type::cone:		_Wostream <<  L"cone"; break;
	case   chart_solid_type::pyramid:	_Wostream <<  L"pyramid"; break;
	}
    return _Wostream;    
}
chart_solid_type chart_solid_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"cuboid")
        return chart_solid_type( cuboid );
    else if (tmp == L"cylinder")
        return chart_solid_type( cylinder);
    else if (tmp == L"cone")
        return chart_solid_type( cone );
    else if (tmp == L"pyramid")
        return chart_solid_type( pyramid );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_solid_type( cuboid );
    }
}



} }
