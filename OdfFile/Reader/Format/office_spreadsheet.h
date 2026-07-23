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

#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

class office_spreadsheet : public office_element_impl<office_spreadsheet>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSpreadsheet;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
	virtual void add_attributes(const xml::attributes_wc_ptr & Attributes);
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
	_CP_OPT(bool)				table_structure_protected_;
	_CP_OPT(std::wstring)		table_protection_key_;
	_CP_OPT(std::wstring)		table_protection_key_digest_algorithm_;
	
	office_element_ptr			named_expressions_;
	office_element_ptr			database_ranges_; 
	office_element_ptr			data_pilot_tables_;
	office_element_ptr			tracked_changes_;
	office_element_ptr			content_validations_;
	
	office_element_ptr_array	content_;
 	
	office_element_ptr			user_fields_;
	office_element_ptr			variables_;
 	office_element_ptr			sequences_;
	// table:calculation-settings
	// table:consolidation
	// table:dde-links
	// table:label-ranges
	
	// text:alphabetical-index-auto-mark-file
	// text:ddeconnection-decls
};
CP_REGISTER_OFFICE_ELEMENT2(office_spreadsheet);

}
}
