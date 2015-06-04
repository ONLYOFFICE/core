#ifndef _CPDOCCORE_XLSX_FONT_H_INCLUDED
#define _CPDOCCORE_XLSX_FONT_H_INCLUDED
#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <string>
#include <vector>

#include "xlsx_color.h"

namespace cpdoccore {
namespace oox {

    enum xlsx_vertAlign
    {
        vertAlignBaseline,
        vertAlignSubscript,
        vertAlignSuperscript
    };

    enum XlsxFontFamily
    {
        XFAMILY_EMPTY = -1,
        XFAMILY_NOTAPPLICABLE = 0,
        XFAMILY_ROMAN = 1,
        XFAMILY_SWISS = 2,
        XFAMILY_MODERN = 3,
        XFAMILY_SCRIPT = 4,
        XFAMILY_DECORATIVE = 5
    };

    enum XlsxUnderline
    {
        XUNDERLINE_NONE,
        XUNDERLINE_SINGLE,
        XUNDERLINE_DOUBLE,
        XUNDERLINE_SINGLE_ACCOUNTING,
        XUNDERLINE_DOUBLE_ACCOUNTING
    };

    enum XlsxFontCharset
    {
        XCHARSET_EMPTY = -1,
        XCHARSET_CHARSET_ANSI = 0,
        XCHARSET_DEFAULT = 1,
        XCHARSET_SYMBOL = 2,
        XCHARSET_MAC = 77,
        XCHARSET_SHIFTJIS = 128,
        XCHARSET_HANGEUL = 129,
        XCHARSET_JOHAB = 130,
        XCHARSET_GB2312 = 134,
        XCHARSET_CHINESEBIG5 = 136,
        XCHARSET_GREEK = 161,
        XCHARSET_TURKISH = 162,
        XCHARSET_VIETNAMESE = 163,
        XCHARSET_HEBREW = 177,
        XCHARSET_ARABIC = 178,
        XCHARSET_BALTIC = 186,
        XCHARSET_RUSSIAN = 204,
        XCHARSET_THAI = 222,
        XCHARSET_EASTEUROPE = 238,
        XCHARSET_OEM = 255
    };

    struct xlsx_font
    {
        _CP_OPT(bool) bold;
        _CP_OPT(unsigned int) charset;
        _CP_OPT(xlsx_color) color;
        _CP_OPT(bool) condense;
        _CP_OPT(bool) extend;
        _CP_OPT(XlsxFontFamily) family;
        _CP_OPT(bool) i; ///< italic
        _CP_OPT(std::wstring) name;
        _CP_OPT(bool) outline;
        _CP_OPT(std::wstring) scheme;
        _CP_OPT(bool) shadow;
        _CP_OPT(bool) strike;
        _CP_OPT(double) sz;
        _CP_OPT(XlsxUnderline) u;
        _CP_OPT(xlsx_vertAlign) vertAlign;

        mutable std::size_t index;

        bool operator == (const xlsx_font & rVal) const;
        bool operator != (const xlsx_font & rVal) const;

        friend std::size_t hash_value(xlsx_font const & val);
    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_font & font);

}
}

#endif
