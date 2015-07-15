#pragma once

#include <iosfwd>
#include <string>

#include "common.h"


namespace oox {

/// \struct xlsx_alignment
struct xlsx_alignment
{
    _CP_OPT(std::wstring) horizontal;
    _CP_OPT(int) indent;
    _CP_OPT(bool) justifyLastLine;
    _CP_OPT(int) readingOrder;
    _CP_OPT(int) relativeIndent;
    _CP_OPT(bool) shrinkToFit;
    _CP_OPT(unsigned int) textRotation;
    _CP_OPT(std::wstring) vertical;
    _CP_OPT(bool) wrapText;   

    bool operator == (const xlsx_alignment & rVal) const;
    bool operator != (const xlsx_alignment & rVal) const;

    friend void xlsx_serialize(std::wostream & _Wostream, const xlsx_alignment & alignment);
    friend bool is_default(const xlsx_alignment & rVal);
    friend std::size_t hash_value(xlsx_alignment const & val);
};

xlsx_alignment OdfProperties2XlsxAlignment();

}

