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