#ifndef _CPDOCCORE_ODF_STYLEVERTICALPOS_H_
#define _CPDOCCORE_ODF_STYLEVERTICALPOS_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf { 

class vertical_pos
{
public:
    enum type
    {
        Top,
        Middle,
        Bottom,
        FromTop,
        Below,
    };

    vertical_pos() {}

    vertical_pos(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
	std::wstring get_type_str() const
	{
		switch(type_)
		{
		case vertical_pos::Top:       return L"top";    
		case vertical_pos::Bottom:    return L"bottom"; 
		case vertical_pos::Middle:    return L"center"; 
		}
		return L"top";
	}
    static vertical_pos parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const vertical_pos & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::vertical_pos);

}

#endif
