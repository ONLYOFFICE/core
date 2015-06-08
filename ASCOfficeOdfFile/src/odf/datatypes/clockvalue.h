#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf_types { 

class clockvalue
{
public:

public:
    clockvalue(int ms = 0) : value_(ms)
    {}

    int get_value() const 
    { 
        return value_; 
    }
    

    static clockvalue parse(const std::wstring & Str);

private:
    int value_;//in  ms
};

}

APPLY_PARSE_XML_ATTRIBUTES(odf_types::clockvalue);

}

