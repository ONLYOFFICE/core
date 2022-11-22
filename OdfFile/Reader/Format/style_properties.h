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

#include "office_elements_create.h"

#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_table_properties.h"
#include "style_graphic_properties.h"
#include "style_chart_properties.h"
#include "style_presentation.h"
#include "style_regions.h"

namespace cpdoccore { 
namespace odf_reader {

	// style:properties
class style_properties : public office_element_impl<style_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleProperties;

    CPDOCCORE_DEFINE_VISITABLE();
	 
    text_format_properties_content text_properties_;
	paragraph_format_properties paragraph_properties_;
	graphic_format_properties graphic_properties_;
	
	table_format_properties table_properties_;
	style_table_row_properties_attlist table_row_properties_;
	style_table_column_properties_attlist table_column_properties_;
	style_table_cell_properties_attlist	table_cell_properties_;

	chart_format_properties chart_properties_;
private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element(xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(style_properties);
}
}
