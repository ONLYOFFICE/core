#ifndef _CPDOCCORE_XLSX_FONTS_H_30b72711
#define _CPDOCCORE_XLSX_FONTS_H_30b72711

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore { 
namespace odf {

class text_format_properties_content;
class paragraph_format_properties;
class style_table_cell_properties_attlist;

}
}

namespace cpdoccore {
namespace oox {

struct xlsx_font;

/// \class xlsx_fonts
class xlsx_fonts
{
public:
    xlsx_fonts();
    ~xlsx_fonts();

public:
    size_t size() const;
    size_t fontId(const odf::text_format_properties_content * textProp,
        const odf::paragraph_format_properties * parProp,
        const odf::style_table_cell_properties_attlist * cellProp);
    const xlsx_font & getFont(size_t id) const;

    void xlsx_serialize(std::wostream & _Wostream) const;

    friend void xlsx_serialize(std::wostream & _Wostream, xlsx_fonts const & fonts);
    
private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};



}
}

#endif
