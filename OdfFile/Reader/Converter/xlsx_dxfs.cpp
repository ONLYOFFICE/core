/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "xlsx_dxfs.h"

#include "xlsx_fill.h"
#include "xlsx_font.h"

#include "../Format/odf_document.h"
#include "../Format/odfcontext.h"

#include <xml/simple_xml_writer.h>

#include <ostream>
#include <vector>

namespace cpdoccore {
namespace oox {

class xlsx_dxfs::Impl
{
public:
	Impl(){}

    std::vector<std::pair<xlsx_fill, xlsx_font>> dxf_array;
};

xlsx_dxfs::xlsx_dxfs(odf_reader::fonts_container & fonts) : fonts_container(fonts), impl_( new xlsx_dxfs::Impl() )
{
}

xlsx_dxfs::~xlsx_dxfs()
{
}

size_t xlsx_dxfs::dxfId(	const odf_reader::text_format_properties_ptr &textProp,
							const odf_reader::graphic_format_properties_ptr &graphProp,
							const odf_reader::style_table_cell_properties_attlist	*cellProp)
{
	xlsx_fill fill( graphProp.get(), cellProp);
	xlsx_font font( textProp, NULL, cellProp, false, fonts_container);

	impl_->dxf_array.push_back(std::make_pair(fill, font));

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
					xlsx_serialize(CP_XML_STREAM(), impl_->dxf_array[i].second); //font
					xlsx_serialize(CP_XML_STREAM(), impl_->dxf_array[i].first); //fill
				}
            }
        }
    }
}

}
}
