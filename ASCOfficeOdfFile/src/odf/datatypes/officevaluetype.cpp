#include "../precompiled_cpodf.h"
#include "officevaluetype.h"
#include "errors.h"

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const office_value_type & _Val)
{
    switch(_Val.get_type())
    {
    case office_value_type::Custom:
        //_Wostream << "none";
        break;
    case office_value_type::Float:
        _Wostream << "float";
        break;
    case office_value_type::Currency:
        _Wostream << "currency";
        break;
    case office_value_type::Percentage:
        _Wostream << "percentage";
        break;
	case office_value_type::Date:
        _Wostream << "date";
        break;	
 	case office_value_type::Time:
        _Wostream << "time";
        break;	
	case office_value_type::Boolean:
        _Wostream << "boolean";
        break;	
	case office_value_type::String:
        _Wostream << "string";
        break;	
    default:
        break;
    }
    return _Wostream;
}

office_value_type office_value_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    
    if (tmp == L"float")
        return office_value_type( Float);
    else if (tmp == L"currency")
        return office_value_type( Currency );
    else if (tmp == L"percentage")
        return office_value_type( Percentage );
	else if (tmp == L"boolean")
        return office_value_type( Boolean );
	else if (tmp == L"date")
        return office_value_type( Date );
	else if (tmp == L"time")
        return office_value_type( Time );
	else if (tmp == L"string")
        return office_value_type( String );
	else
		return office_value_type(Custom);
}

} }

