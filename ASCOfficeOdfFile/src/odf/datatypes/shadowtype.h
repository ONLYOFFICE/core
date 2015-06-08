#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class shadow_type
{
public:
    enum type
    {
        None,
        Enable
    };

    shadow_type() {}

    shadow_type(type _Type) : type_(_Type)
    {}

    shadow_type(const std::wstring & _Name) : type_(Enable), name_(_Name)
    {}

    type get_type() const
    {
        return type_;
    };

    const std::wstring & get_name() const
    {
        return name_;
    }
    
    static shadow_type parse(const std::wstring & Str);

private:
    type type_;
    std::wstring name_;

};

std::wostream & operator << (std::wostream & _Wostream, const shadow_type & _Val);


//--------------------------------------------------------------------------------------------------------------------------------
class shadow_type1
{
public:
    enum type
    {
        Hidden,
        Visible
    };

    shadow_type1() {}

    shadow_type1(type _Type) : type_(_Type)
    {}

    shadow_type1(const std::wstring & _Name) : type_(Visible), name_(_Name)
    {}

    type get_type() const
    {
        return type_;
    };

    const std::wstring & get_name() const
    {
        return name_;
    }
    
    static shadow_type1 parse(const std::wstring & Str);

private:
    type type_;
    std::wstring name_;

};

std::wostream & operator << (std::wostream & _Wostream, const shadow_type1 & _Val);

} 
APPLY_PARSE_XML_ATTRIBUTES(odf_types::shadow_type);
APPLY_PARSE_XML_ATTRIBUTES(odf_types::shadow_type1);
}
