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
#pragma once

#include <iosfwd>
#include <string>
#include "../../Common/CPScopedPtr.h"

namespace cpdoccore {
namespace odf_reader {

	class graphic_format_properties;
	typedef boost::shared_ptr<graphic_format_properties> graphic_format_properties_ptr;

	class paragraph_format_properties;
	class style_table_cell_properties_attlist;
	
	class text_format_properties;
	typedef boost::shared_ptr<text_format_properties> text_format_properties_ptr;
}

namespace oox {

	class xlsx_cell_format;
	class xlsx_conversion_context;

class xlsx_style_manager
{
public:
    xlsx_style_manager(xlsx_conversion_context *context);
    size_t size() const;
    
	size_t xfId(const odf_reader::text_format_properties_ptr &textProp,
				const odf_reader::paragraph_format_properties			*parProp,
				const odf_reader::style_table_cell_properties_attlist	*cellProp,
				const xlsx_cell_format * xlxsCellFormat,
				const std::wstring &num_format, char num_format_type, bool  default_set, bool & is_visible);

    size_t xfId(const odf_reader::text_format_properties_ptr &textProp,
				const odf_reader::paragraph_format_properties			*parProp,
				const odf_reader::style_table_cell_properties_attlist	*cellProp,
				const xlsx_cell_format * xlxsCellFormat,
				const std::wstring &num_format, char num_format_type, bool  default_set);

    size_t dxfId(const odf_reader::text_format_properties_ptr &textProp,
				 const odf_reader::graphic_format_properties_ptr &graphProp,
				 const odf_reader::style_table_cell_properties_attlist	* cellProp);

    void xlsx_serialize(std::wostream & _Wostream);
    
    ~xlsx_style_manager();

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}
