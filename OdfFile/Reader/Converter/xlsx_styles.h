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
#pragma once

#include <iosfwd>
#include <string>
#include "../../Common/CPScopedPtr.h"

namespace cpdoccore {
namespace odf_reader {

	class graphic_format_properties;
	class paragraph_format_properties;
	class style_table_cell_properties_attlist;
	
	class text_format_properties_content;
	typedef boost::shared_ptr<text_format_properties_content> text_format_properties_content_ptr;
}

namespace oox {

	class xlsx_cell_format;
	class xlsx_conversion_context;

class xlsx_style_manager
{
public:
    xlsx_style_manager(xlsx_conversion_context *context);
    size_t size() const;
    
	size_t xfId(const odf_reader::text_format_properties_content_ptr	textProp,
				const odf_reader::paragraph_format_properties			* parProp,
				const odf_reader::style_table_cell_properties_attlist	* cellProp,
				const xlsx_cell_format * xlxsCellFormat,
				const std::wstring &num_format, char num_format_type, bool  default_set, bool & is_visible);

    size_t xfId(const odf_reader::text_format_properties_content_ptr	textProp,
				const odf_reader::paragraph_format_properties			* parProp,
				const odf_reader::style_table_cell_properties_attlist	* cellProp,
				const xlsx_cell_format * xlxsCellFormat,
				const std::wstring &num_format, char num_format_type, bool  default_set);

    size_t dxfId(const odf_reader::text_format_properties_content_ptr	textProp,
				 const odf_reader::graphic_format_properties			* graphProp,
				 const odf_reader::style_table_cell_properties_attlist	* cellProp);

    void xlsx_serialize(std::wostream & _Wostream);
    
    ~xlsx_style_manager();

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}
