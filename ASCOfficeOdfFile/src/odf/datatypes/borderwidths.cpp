#include <boost/algorithm/string.hpp>

#include "borderwidths.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const border_widths & _Val)
{
    _Wostream << _Val.get_len1() << " " << _Val.get_len2() << " " << _Val.get_len3();
    return _Wostream;    
}

border_widths border_widths::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    boost::algorithm::trim(tmp);
    std::vector< std::wstring > splitted;
    boost::algorithm::split(splitted, tmp, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

    if (splitted.size() != 3)
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );

    length len1 = length::parse(splitted[0]);
    length len2 = length::parse(splitted[1]);
    length len3 = length::parse(splitted[2]);

    return border_widths(len1, len2, len3);
}

} }
