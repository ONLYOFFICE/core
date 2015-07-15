#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "xlsx_color.h"

#include "common.h"


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

    };

    std::size_t hash_value(const _CP_OPT(xlsx_border_edge) & val);
    std::size_t hash_value(xlsx_border_edge const& val);
    std::size_t hash_value(xlsx_border const& val);

    bool is_default(xlsx_border_edge * borderEdge);
    bool is_default(xlsx_border_edge const& borderEdge);
    bool is_default(const _CP_OPT(xlsx_border_edge) & borderEdge);

     bool is_default(xlsx_border const& border);

    void xlsx_serialize(std::wostream & _Wostream, xlsx_border const & border);

}

