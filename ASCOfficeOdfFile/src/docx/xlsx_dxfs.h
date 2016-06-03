#pragma once

#include <iosfwd>
#include <string>

#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
	namespace odf_reader {

	class text_format_properties_content;
	class graphic_format_properties;
	class paragraph_format_properties;
	class style_table_cell_properties_attlist;
}

namespace oox {

    class xlsx_dxfs
    {
    public:
		xlsx_dxfs();
		~xlsx_dxfs();

		size_t dxfId(	const odf_reader::text_format_properties_content		* textProp,
						const odf_reader::graphic_format_properties				* graphProp,
						const odf_reader::style_table_cell_properties_attlist	* cellProp);
		
        void serialize(std::wostream & _Wostream) const;

    private:
        class Impl;
        _CP_SCOPED_PTR(Impl) impl_;
    };

}
}
