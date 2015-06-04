#pragma once

#include <string>
#include <cpdoccore/CPOptional.h>
#include "xlsx_color.h"

namespace cpdoccore {
namespace oox {

    struct xlsx_patternFill
    {
        _CP_OPT(xlsx_color) fgColor;
        _CP_OPT(xlsx_color) bgColor;
        _CP_OPT(std::wstring) patternType;

        bool operator == (const xlsx_patternFill & rVal) const;
        bool operator != (const xlsx_patternFill & rVal) const;
        friend std::size_t hash_value(xlsx_patternFill const & val);
    };

    struct xlsx_gradientFill
    {
        bool operator == (const xlsx_gradientFill & rVal) const;
        bool operator != (const xlsx_gradientFill & rVal) const;
        friend std::size_t hash_value(xlsx_gradientFill const & val);
    };

    struct xlsx_fill
    {
        _CP_OPT(xlsx_patternFill)	patternFill;
        _CP_OPT(xlsx_gradientFill)	gradientFill;

        std::size_t index;
        bool bDefault;

        bool operator == (const xlsx_fill & rVal) const;
        bool operator != (const xlsx_fill & rVal) const;
        friend std::size_t hash_value(xlsx_fill const & val);
    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_gradientFill  & gradientFill);
    void xlsx_serialize(std::wostream & _Wostream, const xlsx_patternFill   & patternFill);
    void xlsx_serialize(std::wostream & _Wostream, const xlsx_fill          & fill);

}
}
