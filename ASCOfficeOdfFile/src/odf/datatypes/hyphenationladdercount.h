#ifndef _CPDOCCORE_ODF_HYPHENATIONLADDERCOUNT_H_
#define _CPDOCCORE_ODF_HYPHENATIONLADDERCOUNT_H_    

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"

#include <limits>

namespace cpdoccore { namespace odf_types { 

class integer_or_nolimit
{
public:
    enum type
    {
        NoLimit,
        Integer
    };

    integer_or_nolimit() {}

    integer_or_nolimit(type _Type) : type_(_Type), _positive_integer( (std::numeric_limits<unsigned int>::max)() )
    {}

    integer_or_nolimit(unsigned int _Int) : type_(Integer), _positive_integer(_Int)
    {}

    type get_type() const
    {
        return type_;
    };

    unsigned int get_value() const
    {
        return _positive_integer;
    }
    
    static integer_or_nolimit parse(const std::wstring & Str);

private:
    type type_;
    unsigned int _positive_integer;

};

std::wostream & operator << (std::wostream & _Wostream, const integer_or_nolimit & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::integer_or_nolimit);

}

#endif
