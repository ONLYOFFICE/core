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
#include "style_properties.h"

#include <xml/xmlchar.h>
#include <xml/simple_xml_writer.h>

#include "odf_document.h"

#include <iostream>

#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore { 
namespace odf_reader {

// style:properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_properties::ns = L"style";
const wchar_t * style_properties::name = L"properties";

void style_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	text_properties_.add_attributes(Attributes);
	paragraph_properties_.add_attributes(Attributes);
	graphic_properties_.add_attributes(Attributes);
	drawing_page_properties_.add_attributes(Attributes);

	table_properties_.add_attributes(Attributes);
	table_row_properties_.add_attributes(Attributes);
	table_column_properties_.add_attributes(Attributes);
	table_cell_properties_.add_attributes(Attributes);
}

void style_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"style" == Ns)
	{
		if (L"tab-stops" == Name)
		{
			CP_CREATE_ELEMENT(paragraph_properties_.style_tab_stops_);
		}
		else if (L"drop-cap" == Name)
		{
			CP_CREATE_ELEMENT(paragraph_properties_.style_drop_cap_);
		}
		else if (L"background-image" == Name)
		{
			CP_CREATE_ELEMENT(paragraph_properties_.style_background_image_);
		}
	}
	else if CP_CHECK_NAME(L"text", L"list-style")
	{
		CP_CREATE_ELEMENT(text_list_style_);
	}
}


}
}

