#pragma once

#include <iosfwd>
#include <string>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {

namespace odf {

class text_format_properties_content;
class paragraph_format_properties;
class style_table_cell_properties_attlist;

}

namespace oox {

class xlsx_cell_format;

/// \class  class xlsx_style_manager
class xlsx_style_manager
{
public:
    xlsx_style_manager();
    size_t size() const;
    size_t xfId(const odf::text_format_properties_content * textProp,
        const odf::paragraph_format_properties * parProp,
        const odf::style_table_cell_properties_attlist * cellProp,
        const xlsx_cell_format * xlxsCellFormat,
        const std::wstring &num_format,  bool  default_set,bool & is_visible);

    size_t xfId(const odf::text_format_properties_content * textProp,
        const odf::paragraph_format_properties * parProp,
        const odf::style_table_cell_properties_attlist * cellProp,
        const xlsx_cell_format * xlxsCellFormat,
        const std::wstring &num_format, bool  default_set);


    void xlsx_serialize(std::wostream & _Wostream);
    
    ~xlsx_style_manager();

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}
