
#include "tablemode.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const table_mode & _Val)
{
    switch(_Val.get_type())
    {
    case table_mode::CopyAll:
        _Wostream << L"copy-all";
        break;
    case table_mode::CopyResultsOnly:
        _Wostream << L"copy-results-only";
        break;
    default:
        break;
    }
    return _Wostream;    
}

table_mode table_mode::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"copy-all")
        return table_mode( CopyAll );
    else if (tmp == L"copy-results-only")
        return table_mode( CopyResultsOnly );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return table_mode( CopyAll );
    }
}

} }
