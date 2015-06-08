#ifndef _CPDOCCORE_ODF_SCRIPTTYPE_H_
#define _CPDOCCORE_ODF_SCRIPTTYPE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class script_type
{
public:
    enum type
    {
        Latin,
        Asian,
        Complex,
        Ignore
    };

    script_type() {}

    script_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static script_type parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const script_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::script_type);

}

#endif // #ifndef _CPDOCCORE_ODF_SCRIPTTYPE_H_
