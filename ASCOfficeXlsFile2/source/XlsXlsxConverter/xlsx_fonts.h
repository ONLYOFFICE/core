#pragma once

#include <iosfwd>
#include <boost/scoped_ptr.hpp>

namespace oox {

    struct xlsx_font;

    class xlsx_fonts
    {
    public:
        xlsx_fonts();
        ~xlsx_fonts();

    public:
        size_t size() const;
        size_t fontId(/*const odf_reader::text_format_properties_content * textProp,
            const odf_reader::paragraph_format_properties * parProp,
            const odf_reader::style_table_cell_properties_attlist * cellProp*/);
        const xlsx_font & getFont(size_t id) const;

        void xlsx_serialize(std::wostream & _Wostream) const;

    private:
        class Impl;
		boost::scoped_ptr<Impl> impl_;
    };

    void xlsx_serialize(std::wostream & _Wostream, xlsx_fonts const & fonts);


}

