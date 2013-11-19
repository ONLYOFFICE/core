#ifndef _CPDOCCORE_XLSX_BORDER_H_INCLUDED_
#define _CPDOCCORE_XLSX_BORDER_H_INCLUDED_
#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <string>
#include <vector>

#include "xlsx_color.h"

namespace cpdoccore { 
namespace odf {

class text_format_properties_content;
class paragraph_format_properties;
class style_table_cell_properties;

}
}

namespace cpdoccore {
namespace oox {

struct xlsx_border_edge
{
    xlsx_border_edge()
    {}

    xlsx_border_edge(std::wstring _style, xlsx_color _color = xlsx_color()) : style(_style), color(_color)
    {}

    _CP_OPT(std::wstring)	style; // attribute
    _CP_OPT(xlsx_color)		color; // element
    _CP_OPT(int)			width; // 

    bool operator == (const xlsx_border_edge & rVal) const;
    bool operator != (const xlsx_border_edge & rVal) const;
    friend std::size_t hash_value(xlsx_border_edge const & val);
};

struct xlsx_border
{
    _CP_OPT(bool) diagonalUp;
    _CP_OPT(bool) diagonalDown;
    _CP_OPT(bool) outline; // default true

    _CP_OPT(xlsx_border_edge) left;
    _CP_OPT(xlsx_border_edge) right;
    _CP_OPT(xlsx_border_edge) top;
    _CP_OPT(xlsx_border_edge) bottom;
    _CP_OPT(xlsx_border_edge) diagonal;
    _CP_OPT(xlsx_border_edge) vertical;
    _CP_OPT(xlsx_border_edge) horizontal;

    std::size_t index;

    bool operator == (const xlsx_border & rVal) const;
    bool operator != (const xlsx_border & rVal) const;

    friend void xlsx_serialize(std::wostream & _Wostream, xlsx_border const & border);
    friend bool is_default(xlsx_border_edge const & borderEdge);
    friend bool is_default(_CP_OPT(xlsx_border_edge) const & borderEdge);
    friend bool is_default(xlsx_border const & border);
    
	friend std::size_t hash_value(xlsx_border const & val);
};


}
}

#endif

