#ifndef _CPDOCCORE_ODF_STYLEPOSITION_H_
#define _CPDOCCORE_ODF_STYLEPOSITION_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class style_position
{
public:
    enum type
    {
        Left,
        Center,
        Right,
        Top,
        Bottom
    };

    style_position() : typeVertical_(Center), typeHorisontal_(Center)  {}

    style_position(type _TypeVertical, type _TypeHorisontal) : typeVertical_(_TypeVertical), typeHorisontal_(_TypeHorisontal)
    {}

    type get_vertical() const
    {
        return typeVertical_;
    };
    
    type get_horisontal() const
    {
        return typeHorisontal_;
    };

    static style_position parse(const std::wstring & Str);

private:
    type typeVertical_;
    type typeHorisontal_;

};

std::wostream & operator << (std::wostream & _Wostream, const style_position & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::style_position);

}

#endif
