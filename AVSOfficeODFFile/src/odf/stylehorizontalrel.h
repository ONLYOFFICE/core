#ifndef _CPDOCCORE_ODF_HORIZONTALREL_H_
#define _CPDOCCORE_ODF_HORIZONTALREL_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class horizontal_rel
{
public:
    enum type
    {
        Page,
        PageContent,
        PageStartMargin,
        PageEndMargin,
        Frame,
        FrameContent,
        FrameStartMargin,
        FrameEndMargin,
        Paragraph,
        ParagraphContent,
        ParagraphStartMargin,
        ParagraphEndMargin,
        Char
    };

    horizontal_rel() {}

    horizontal_rel(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
	std::wstring get_type_str() const
	{
		switch(type_)
		{
		case horizontal_rel::Page:
			return L"page";
		case horizontal_rel::Paragraph:
		case horizontal_rel::ParagraphContent:
			return L"column";
		case horizontal_rel::PageEndMargin:
			return L"rightMargin";
		case horizontal_rel::PageStartMargin:
			return L"leftMargin";
		}
		return L"margin";
	}
   
    static horizontal_rel parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const horizontal_rel & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::horizontal_rel);

}

#endif
