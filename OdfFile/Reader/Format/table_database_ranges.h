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

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/tableorientation.h"
#include "../../DataTypes/tableorder.h"
#include "../../DataTypes/tabledatatype.h"
#include "../../DataTypes/tableoperator.h"

#include "../../Reader/Converter/xlsxconversioncontext.h"

namespace cpdoccore { 
namespace odf_reader {

class table_database_ranges : public office_element_impl<table_database_ranges>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDatabaseRanges;

    CPDOCCORE_DEFINE_VISITABLE()

    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_database_ranges);
//-------------------------------------------------------------------------------------------------------------

class table_database_range: public office_element_impl<table_database_range>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDatabaseRange;

    CPDOCCORE_DEFINE_VISITABLE()

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

    _CP_OPT(std::wstring)					table_name_;
    _CP_OPT(std::wstring)					table_target_range_address_;
	_CP_OPT(odf_types::Bool)				table_display_filter_buttons_;
	_CP_OPT(odf_types::Bool)				table_contains_header_;	
	_CP_OPT(odf_types::table_orientation)	table_orientation_;
//table:refresh-delay

	office_element_ptr_array content_;
//"table-database-source-sql"
//"table-database-source-table"
//"table-database-source-query"
};

CP_REGISTER_OFFICE_ELEMENT2(table_database_range);
//-------------------------------------------------------------------------------------------------------------

class table_sort: public office_element_impl<table_sort>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableSort;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	office_element_ptr_array	content_;
//attr
	//table:algorithm
	//table:country
	//table:language
	//table:case-sensitive
};

CP_REGISTER_OFFICE_ELEMENT2(table_sort);
//-------------------------------------------------------------------------------------------------------------

class table_sort_by: public office_element_impl<table_sort_by>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableSortBy;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	int								    table_field_number_;
	_CP_OPT(odf_types::table_data_type) table_data_type_;
	_CP_OPT(odf_types::table_order)	    table_order_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_sort_by);
//-------------------------------------------------------------------------------------------------------------

class table_filter: public office_element_impl<table_filter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableFilter;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	_CP_OPT(std::wstring)		table_condition_source_;	//self or cell-range.
	_CP_OPT(std::wstring)		table_condition_source_range_address_;	//cellRangeAddress
	_CP_OPT(odf_types::Bool)	table_display_duplicates_;
	_CP_OPT(std::wstring)		table_target_range_address_;//cellRangeAddress

	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_filter);
//------------------------------------------------------------------------------

class table_filter_and: public office_element_impl<table_filter_and>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableFilterAnd;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_filter_and);
//-------------------------------------------------------------------------------------------------------------

class table_filter_or: public office_element_impl<table_filter_or>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableFilterOr;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_filter_or);
//-------------------------------------------------------------------------------------------------------------

class table_filter_condition: public office_element_impl<table_filter_condition>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableFilterCondition;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	_CP_OPT(std::wstring)	            table_case_sensitive_;
	_CP_OPT(odf_types::table_data_type)	table_data_type_;
	_CP_OPT(unsigned int)	            table_field_number_;
	_CP_OPT(odf_types::table_operator)  table_operator_;
	_CP_OPT(std::wstring)               table_value_;
	
	office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_filter_condition);
//-------------------------------------------------------------------------------------------------------------

class table_filter_set_item: public office_element_impl<table_filter_set_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableFilterSetItem;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	_CP_OPT(std::wstring)	table_value_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_filter_set_item);
//-------------------------------------------------------------------------------------------------------------

}
}
