#include <boost/algorithm/string.hpp>

#include "noteclass.h"

#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const noteclass & _Val)
{
    switch(_Val.get_type())
    {
    case noteclass::Footnote:
        _Wostream << L"footnote";
        break;
    case noteclass::Endnote:
        _Wostream << L"endnote";
        break;
    default:
        break;
    }
    return _Wostream;    
}

noteclass noteclass::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"footnote")
        return noteclass( Footnote );
    else if (tmp == L"endnote")
        return noteclass( Endnote );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return noteclass( Footnote );
    }
}

} }
