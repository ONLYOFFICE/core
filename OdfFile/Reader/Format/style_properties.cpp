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
}


}
}

