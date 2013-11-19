#ifndef _CPDOCCORE_XLSX_COLOR_H_INCLUDED_
#define _CPDOCCORE_XLSX_COLOR_H_INCLUDED_
#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <string>
#include <vector>

namespace cpdoccore {
namespace oox {

struct xlsx_color
{
    _CP_OPT(bool) autoVal;
    _CP_OPT(unsigned int) indexed;
    _CP_OPT(std::wstring) rgb;
    _CP_OPT(unsigned int) theme;
    _CP_OPT(double) tint;

    bool operator == (const xlsx_color & rVal) const;
    bool operator != (const xlsx_color & rVal) const;

    friend void xlsx_serialize(std::wostream & _Wostream, const xlsx_color & color);
    friend void xlsx_serialize(std::wostream & _Wostream, const xlsx_color & color, const std::wstring & nodeName);

    friend std::size_t hash_value(xlsx_color const & val);
};


}
}

#endif