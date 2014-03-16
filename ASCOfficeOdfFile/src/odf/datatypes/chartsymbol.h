#ifndef _CPDOCCORE_ODF_CHARTSYMBOL_H_
#define _CPDOCCORE_ODF_CHARTSYMBOL_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class chart_symbol_type
{
public:
    enum type
    {
        noneSymbol,
        autoSymbol,
        namedSymbol
    };

    chart_symbol_type() {}

    chart_symbol_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static chart_symbol_type parse(const std::wstring & Str);

private:
    type type_;

};




class chart_symbol_name
{
public:
    enum type
    {
		noneSymbol,
		autoSymbol,
		squareSymbol,
		diamondSymbol,
		arrow_downSymbol,
		arrow_upSymbol,
		arrow_rightSymbol,
		arrow_leftSymbol,
		bow_tieSymbol,
		hourglassSymbol,
		circleSymbol,
		starSymbol,
		xSymbol,
		plusSymbol,
		asteriskSymbol,
		horizontal_barSymbol,
		verticalSymbol
    };

    chart_symbol_name() {}

    chart_symbol_name(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static chart_symbol_name parse(const std::wstring & Str);

private:
    type type_;

};

	std::wostream & operator << (std::wostream & _Wostream, const chart_symbol_type & _Val);
	std::wostream & operator << (std::wostream & _Wostream, const chart_symbol_name & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::chart_symbol_type);
APPLY_PARSE_XML_ATTRIBUTES(odf::chart_symbol_name);


}

#endif
