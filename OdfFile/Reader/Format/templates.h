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

#include <string>
#include <vector>
#include <iosfwd>

#include "../../Common/CPNoncopyable.h"

#include "styles_list.h"
#include "style_map.h"


namespace cpdoccore { 
namespace odf_reader { 

//<define name="common-table-template-attlist">
//  <attribute name="table:style-name">
//         <ref name="styleNameRef"/>
//  </attribute>
//  <optional>
//         <attribute name="table:paragraph-style-name">
//               <ref name="styleNameRef"/>
//         </attribute>
//  </optional>
//</define> 
class table_template_element: public office_element_impl<table_template_element>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTemplateElement;

    CPDOCCORE_DEFINE_VISITABLE();

//common-table-template-attlist

 	std::wstring		table_style_name_;

private:

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

    friend class odf_document;

};

class table_body_template: public table_template_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableBodyTemplate;
};
CP_REGISTER_OFFICE_ELEMENT2(table_body_template);

class table_first_row_template: public table_template_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableFirstRowTemplate;
};
CP_REGISTER_OFFICE_ELEMENT2(table_first_row_template);

class table_last_row_template: public table_template_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableLastRowTemplate;
};
CP_REGISTER_OFFICE_ELEMENT2(table_last_row_template);

class table_first_column_template: public table_template_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableFirstColumnTemplate;
};
CP_REGISTER_OFFICE_ELEMENT2(table_first_column_template);

class table_last_column_template: public table_template_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableLastColumnTemplate;
};
CP_REGISTER_OFFICE_ELEMENT2(table_last_column_template);

class table_odd_columns_template: public table_template_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableOddColumnsTemplate;
};
CP_REGISTER_OFFICE_ELEMENT2(table_odd_columns_template);

class table_odd_rows_template: public table_template_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableOddRowsTemplate;
};
CP_REGISTER_OFFICE_ELEMENT2(table_odd_rows_template);

//--------------------------------------------------------------------

class table_table_template: public office_element_impl<table_table_template>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTemplate;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring get_text_style_name(){return text_style_name_.get_value_or(L"");}
	
	office_element_ptr			table_body_;
	office_element_ptr			table_first_row_;
	office_element_ptr			table_last_row_;
	office_element_ptr			table_first_column_;
	office_element_ptr			table_last_column_;
	office_element_ptr			table_odd_rows_;
	office_element_ptr			table_odd_columns_;

private:
 	_CP_OPT(std::wstring)		text_style_name_;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    friend class odf_document;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_template);

}
}