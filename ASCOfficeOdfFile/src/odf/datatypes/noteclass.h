#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf_types { 

class noteclass
{
public:
    enum type
    {
        Footnote,
        Endnote
    };

    noteclass() : type_(Footnote) {}

    noteclass(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static noteclass parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const noteclass & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::noteclass);

}
