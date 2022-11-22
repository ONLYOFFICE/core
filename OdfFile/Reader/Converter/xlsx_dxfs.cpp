/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "xlsx_dxfs.h"

#include "xlsx_fill.h"
#include "xlsx_font.h"

#include <xml/simple_xml_writer.h>

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

size_t xlsx_dxfs::dxfId(	const odf_reader::text_format_properties_content_ptr	textProp,
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
			
			for (size_t i = 0; i < impl_->dxf_array.size(); i++)
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
