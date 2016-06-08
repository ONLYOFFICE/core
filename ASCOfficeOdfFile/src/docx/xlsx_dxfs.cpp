#include "xlsx_dxfs.h"

#include "xlsx_fill.h"
#include "xlsx_font.h"

#include <cpdoccore/xml/simple_xml_writer.h>

#include <ostream>
#include <vector>

namespace cpdoccore {
namespace oox {

struct xlsx_dxf
{
	xlsx_fill fill;
	xlsx_font font;

};

class xlsx_dxfs::Impl
{
public:
	Impl(){}

    std::vector<xlsx_dxf> dxf_array;
};

xlsx_dxfs::xlsx_dxfs(): impl_( new xlsx_dxfs::Impl() )
{
}

xlsx_dxfs::~xlsx_dxfs()
{
}

size_t xlsx_dxfs::dxfId(	const odf_reader::text_format_properties_content		* textProp,
							const odf_reader::graphic_format_properties				* graphProp,
							const odf_reader::style_table_cell_properties_attlist	* cellProp)
{
	xlsx_dxf dxf;

	dxf.fill = xlsx_fill( graphProp, cellProp);
	dxf.font = xlsx_font( textProp, NULL, cellProp);

	impl_->dxf_array.push_back(dxf);

	return impl_->dxf_array.size() - 1;

}

void xlsx_dxfs::serialize(std::wostream & _Wostream)  const
{
	if (impl_->dxf_array.size() < 1) return;

    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"dxfs")
        {
            CP_XML_ATTR(L"count", impl_->dxf_array.size());
			
			for (int i = 0; i < impl_->dxf_array.size(); i++)
            {
				CP_XML_NODE(L"dxf")
				{
					xlsx_serialize(CP_XML_STREAM(), impl_->dxf_array[i].font);
					xlsx_serialize(CP_XML_STREAM(), impl_->dxf_array[i].fill);
				}
            }
        }
    }
}

}
}
