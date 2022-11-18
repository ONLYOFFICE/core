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

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/tableorientation.h"
#include "../../DataTypes/tableorder.h"

#include "../../Reader/Converter/xlsxconversioncontext.h"

namespace cpdoccore { 
namespace odf_reader {

class table_database_ranges : public office_element_impl<table_database_ranges>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableDatabaseRanges;

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
    static const ElementType type		= typeTableDatabaseRange;

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

	office_element_ptr_array	content_;
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
    static const ElementType type		= typeTableSort;

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
    static const ElementType type		= typeTableSortBy;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	int								table_field_number_;
	_CP_OPT(std::wstring)			table_data_type_;
	_CP_OPT(odf_types::table_order)	table_order_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_sort_by);
//-------------------------------------------------------------------------------------------------------------

class table_filter: public office_element_impl<table_filter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableFilter;

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
    static const ElementType type		= typeTableFilterAnd;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_filter_and);
//-------------------------------------------------------------------------------------------------------------

class table_filter_or: public office_element_impl<table_filter_or>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableFilterOr;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_filter_or);
//-------------------------------------------------------------------------------------------------------------

class table_filter_condition: public office_element_impl<table_filter_condition>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableFilterCondition;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	_CP_OPT(std::wstring)	table_case_sensitive_;
	_CP_OPT(std::wstring)	table_data_type_;
	_CP_OPT(unsigned int)	table_field_number_;
	_CP_OPT(std::wstring)	table_operator_;
	_CP_OPT(std::wstring)	table_value_;
	
	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_filter_condition);
//-------------------------------------------------------------------------------------------------------------

class table_filter_set_item: public office_element_impl<table_filter_set_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableFilterSetItem;

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
