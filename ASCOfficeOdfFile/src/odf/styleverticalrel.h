#ifndef _CPDOCCORE_ODF_STYLEVERTICALREL_H_
#define _CPDOCCORE_ODF_STYLEVERTICALREL_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf { 

class vertical_rel
{
public:
    enum type
    {
        Page,
        PageContent,
        Frame,
        FrameContent,
        Paragraph,
        ParagraphContent,
        Char,
        Line,
        Baseline,
        Text,
    };

    vertical_rel() {}

    vertical_rel(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
	std::wstring get_type_str() const
    {		
		switch(type_)
		{
		case vertical_rel::Page:        return L"page"; 
		case vertical_rel::PageContent: return L"margin"; 
		case vertical_rel::Paragraph:   return L"paragraph"; 
		} 
		return L"margin";
	}
    static vertical_rel parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const vertical_rel & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::vertical_rel);

}

#endif
