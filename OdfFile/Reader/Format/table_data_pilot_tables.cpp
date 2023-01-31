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

#include "table_data_pilot_tables.h"
#include "../../Formulas/formulasconvert.h"

#include <xml/xmlchar.h>
#include "serialize_elements.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_tables::ns		= L"table";
const wchar_t * table_data_pilot_tables::name	= L"data-pilot-tables";

void table_data_pilot_tables::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_tables::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_table::ns		= L"table";
const wchar_t * table_data_pilot_table::name	= L"data-pilot-table";

void table_data_pilot_table::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:name"						, table_name_);
	CP_APPLY_ATTR(L"table:application-data"			, table_application_data_);
    CP_APPLY_ATTR(L"table:buttons"					, table_buttons_);
    CP_APPLY_ATTR(L"table:drill-down-on-double-click", table_drill_down_ondouble_click_);
    CP_APPLY_ATTR(L"table:grand-total"				, table_grand_total_);
    CP_APPLY_ATTR(L"table:identify-categories"		, table_identify_categories_);
    CP_APPLY_ATTR(L"table:ignore-empty-rows"		, table_ignore_empty_rows_);
    CP_APPLY_ATTR(L"table:show-filter-button"		, table_show_filter_button_, true);
	CP_APPLY_ATTR(L"table:target-range-address"		, table_target_range_address_);

}

void table_data_pilot_table::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"table" == Ns && L"data-pilot-field" == Name)
		CP_CREATE_ELEMENT (fields_);
	else if (L"tableooo" == Ns && L"data-pilot-grand-total" == Name)
		CP_CREATE_ELEMENT (grand_total_);
	else
		CP_CREATE_ELEMENT (source_);

}

void table_data_pilot_table::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (!source_) return;

	formulasconvert::odf2oox_converter formulas_converter;

	Context.get_pivots_context().start_table();

	source_->xlsx_convert(Context);

	std::wstring sheet_name;

	if (table_name_) Context.get_pivots_context().set_view_name(*table_name_);
	if (table_target_range_address_)
	{
		std::wstring ref = formulas_converter.convert_named_ref(*table_target_range_address_, false);
		sheet_name = formulas_converter.get_table_name();

		Context.get_table_context().check_database_range_intersection(sheet_name, ref);

		Context.get_pivots_context().set_view_target_range(ref);
		Context.get_pivots_context().set_view_target_table_name(sheet_name);
	}
	if (table_grand_total_)
	{
		Context.get_pivots_context().set_grand_total(table_grand_total_->get_type());
	}

	if (table_buttons_)
	{
		std::vector<std::wstring> headers;
		formulas_converter.split_distance_by(*table_buttons_, L" ", headers);

		for (size_t i = table_show_filter_button_ ? 1 : 0; i < headers.size(); i++)
		{
			std::wstring ref = formulas_converter.convert_named_ref(headers[i], false);
			Context.get_pivots_context().add_button_header(ref);
		}
	}
	if (table_identify_categories_)
		Context.get_pivots_context().set_identify_categories(table_identify_categories_->get());

	if (table_ignore_empty_rows_)
		Context.get_pivots_context().set_ignore_empty_rows(table_ignore_empty_rows_->get());

	if (table_drill_down_ondouble_click_)
		Context.get_pivots_context().set_drill(table_drill_down_ondouble_click_->get());

	for (size_t i = 0; i < fields_.size(); i++)
	{
		fields_[i]->xlsx_convert(Context);
	}	  

	int index_table_view = Context.get_pivots_context().end_table();
	
	if (index_table_view < 0)
		return;

	if (0 == sheet_name.find(L"'"))
	{
		sheet_name = sheet_name.substr(1, sheet_name.length() - 2);
	}
	Context.add_pivot_sheet_source(sheet_name, index_table_view);
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_field::ns		= L"table";
const wchar_t * table_data_pilot_field::name	= L"data-pilot-field";

void table_data_pilot_field::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:source-field-name"	, table_source_field_name_);
	CP_APPLY_ATTR(L"table:orientation"			, table_orientation_);
	CP_APPLY_ATTR(L"table:used-hierarchy"		, table_used_hierarchy_);
	CP_APPLY_ATTR(L"table:function"				, table_function_);
	CP_APPLY_ATTR(L"loext:ignore-selected-page"	, loext_ignore_selected_page_);
	CP_APPLY_ATTR(L"table:selected-page"		, table_selected_page_);
	CP_APPLY_ATTR(L"table:is-data-layout-field"	, table_is_data_layout_field_);
	CP_APPLY_ATTR(L"tableooo:display-name"		, table_display_name_);
}

void table_data_pilot_field::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"table" == Ns && L"data-pilot-field-reference" == Name)
		CP_CREATE_ELEMENT (reference_);  
	else if (L"table" == Ns && L"data-pilot-level" == Name)
		CP_CREATE_ELEMENT (level_);  
	else if (L"table" == Ns && L"data-pilot-groups" == Name)
		CP_CREATE_ELEMENT (groups_); 
}
void table_data_pilot_field::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	int field_type		= table_orientation_.get_value_or(table_orientation::data).get_type(); //?? default type ????
	int field_hierarchy = table_used_hierarchy_.get_value_or(-1); 

	Context.get_pivots_context().start_field();
	
	Context.get_pivots_context().set_field_name(table_source_field_name_.get_value_or(L""));
	
	if (table_display_name_)
		Context.get_pivots_context().set_field_display(*table_display_name_);

	if (table_is_data_layout_field_)
		Context.get_pivots_context().set_field_data_layout(table_is_data_layout_field_->get());
	
	if (table_function_)
	{
		table_function::type type = table_function_->get_type();

		if (type == table_function::String)
		{
			std::wstring formula = table_function_->get_string();

			formulasconvert::odf2oox_converter formulas_converter;
			std::wstring oox_formula = formulas_converter.convert(formula);
			
			Context.get_pivots_context().set_field_user_function(oox_formula);
		}
		else
		{
			Context.get_pivots_context().set_field_function(type);
		}
	}

	if (reference_) reference_->xlsx_convert(Context);
	if (groups_)	groups_->xlsx_convert(Context);
	if (level_)		level_->xlsx_convert(Context);

	Context.get_pivots_context().set_field_type(field_type, field_hierarchy);
	
	Context.get_pivots_context().end_field();
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_field_reference::ns	= L"table";
const wchar_t * table_data_pilot_field_reference::name	= L"data-pilot-field-reference";

void table_data_pilot_field_reference::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:field-name"		, table_field_name_);
	CP_APPLY_ATTR(L"table:member-name"		, table_member_name_);
	CP_APPLY_ATTR(L"table:member-type"		, table_member_type_);
	CP_APPLY_ATTR(L"table:type"				, table_type_);
}

void table_data_pilot_field_reference::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_pivots_context().start_field_reference();
	if (table_field_name_)
	{
		Context.get_pivots_context().set_field_ref_name(*table_field_name_);
	}
	if (table_type_)
	{
		Context.get_pivots_context().set_field_ref_type(table_type_->get_type());
	}
	if (table_member_name_)
	{
		Context.get_pivots_context().set_field_ref_member_name(*table_member_name_);
	}		
	if (table_member_type_)
	{
		Context.get_pivots_context().set_field_ref_member_type(table_member_type_->get_type());
	}
	Context.get_pivots_context().end_field_reference();
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_table::ns		= L"table";
const wchar_t * table_database_source_table::name	= L"database-source-table";

void table_database_source_table::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:database-name", table_database_name_);
	CP_APPLY_ATTR(L"table:table-name"	, table_table_name_);
}

void table_database_source_table::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (table_database_name_)
	{
		Context.get_pivots_context().set_source_database(*table_database_name_, table_table_name_.get_value_or(L""));
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_query::ns		= L"table";
const wchar_t * table_database_source_query::name	= L"database-source-query";

void table_database_source_query::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:database-name"	, table_database_name_);
	CP_APPLY_ATTR(L"table:query-name"		, table_query_name_);	
}


void table_database_source_query::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (table_database_name_)
	{
		Context.get_pivots_context().set_source_database_query(*table_database_name_, table_query_name_.get_value_or(L""));
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_sql::ns	= L"table";
const wchar_t * table_database_source_sql::name	= L"database-source-sql";

void table_database_source_sql::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:database-name"		, table_database_name_);
	CP_APPLY_ATTR(L"table:parse-sql-statement"	, table_parse_sql_statement_);
	CP_APPLY_ATTR(L"table:sql-statement"		, table_sql_statement_);
}

void table_database_source_sql::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	//todoooo split data-base-ranges & pivots

	if (table_database_name_)
	{//table_parse_sql_statement_??
		Context.get_pivots_context().set_source_database_sql(*table_database_name_, table_sql_statement_.get_value_or(L""));
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_source_cell_range::ns		= L"table";
const wchar_t * table_source_cell_range::name	= L"source-cell-range";

void table_source_cell_range::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:cell-range-address", table_cell_range_address_);
}

void table_source_cell_range::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_source_cell_range::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (table_cell_range_address_)
	{
		formulasconvert::odf2oox_converter formulas_converter;
		
		std::wstring ref = formulas_converter.convert_named_ref(*table_cell_range_address_, false);
		std::wstring table_name = formulas_converter.get_table_name();

		Context.get_pivots_context().set_source_range(table_name, ref);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_source_cell_ranges::ns	= L"table";
const wchar_t * table_source_cell_ranges::name	= L"source-cell-ranges";

void table_source_cell_ranges::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:cell-ranges-address", table_cell_ranges_address_);
}
void table_source_cell_ranges::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (table_cell_ranges_address_)//??? split by ref
	{
		//formulasconvert::odf2oox_converter formulas_converter;
		//
		//std::wstring ref = formulas_converter.convert_named_ref(*table_cell_range_address_, false);
		//std::wstring table_name = formulas_converter.get_table_name();

		//Context.get_pivots_context().set_source_range(table_name, ref);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_source_service::ns	= L"table";
const wchar_t * table_source_service::name	= L"source-service";

void table_source_service::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:name",		table_name_);
	CP_APPLY_ATTR(L"table:object-name", table_object_name_);
	CP_APPLY_ATTR(L"table:password",	table_password_);
	CP_APPLY_ATTR(L"table:source-name", table_source_name_);
	CP_APPLY_ATTR(L"table:user-name",	table_user_name_);
}

void table_source_service::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_level::ns		= L"table";
const wchar_t * table_data_pilot_level::name	= L"data-pilot-level";

void table_data_pilot_level::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:show-empty", table_show_empty_);
	CP_APPLY_ATTR(L"calcext:repeat-item-labels", calcext_repeat_item_labels_);
}
void table_data_pilot_level::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"table" == Ns && L"data-pilot-members" == Name)
		CP_CREATE_ELEMENT (members_);  
	else if (L"table" == Ns && L"data-pilot-subtotals" == Name)
		CP_CREATE_ELEMENT (subtotals_);  
	else if (L"table" == Ns && L"data-pilot-display-info" == Name)
		CP_CREATE_ELEMENT (	display_info_);  
	else if (L"table" == Ns && L"data-pilot-layout-info" == Name)
		CP_CREATE_ELEMENT (	layout_info_);  
	else if (L"table" == Ns && L"data-pilot-sort-info" == Name)
		CP_CREATE_ELEMENT (sort_info_);
}
void table_data_pilot_level::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (table_show_empty_)
		Context.get_pivots_context().set_field_show_empty(table_show_empty_->get());
	
	if (calcext_repeat_item_labels_)
		Context.get_pivots_context().set_repeat_item_labels(calcext_repeat_item_labels_->get());

	if (members_)
		members_->xlsx_convert(Context);

	if (subtotals_)
		subtotals_->xlsx_convert(Context);


	table_data_pilot_sort_info *sort = dynamic_cast<table_data_pilot_sort_info*>(sort_info_.get());
	if (sort)
	{
		Context.get_pivots_context().set_field_sort(sort->table_order_.get_value_or(table_order::ascending).get_type());
	}
	
	table_data_pilot_layout_info *layout = dynamic_cast<table_data_pilot_layout_info*>(layout_info_.get());
	if (layout)
	{
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_groups::ns		= L"table";
const wchar_t * table_data_pilot_groups::name	= L"data-pilot-groups";

void table_data_pilot_groups::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:date-start",			table_date_start_);
	CP_APPLY_ATTR(L"table:date-end",			table_date_end_);
	CP_APPLY_ATTR(L"table:start",				table_start);
	CP_APPLY_ATTR(L"table:end",					table_end_);
	CP_APPLY_ATTR(L"table:grouped-by",			table_grouped_by_);
	CP_APPLY_ATTR(L"table:source-field-name",	table_source_field_name_);
	CP_APPLY_ATTR(L"table:step",				table_step);
}
void table_data_pilot_groups::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_groups::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (table_grouped_by_)
		Context.get_pivots_context().set_field_groups(table_grouped_by_->get_type());
	else
		Context.get_pivots_context().set_field_groups(7);

	if (table_source_field_name_)
		Context.get_pivots_context().set_field_groups_source(*table_source_field_name_);


	for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_group::ns		= L"table";
const wchar_t * table_data_pilot_group::name	= L"data-pilot-group";

void table_data_pilot_group::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:name",	table_name_);
}
void table_data_pilot_group::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_group::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_pivots_context().start_field_group();
	
	if (table_name_)
		Context.get_pivots_context().set_field_group_name (*table_name_);
    
	for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
	Context.get_pivots_context().end_field_group();
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_members::ns	= L"table";
const wchar_t * table_data_pilot_members::name	= L"data-pilot-members";

void table_data_pilot_members::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_members::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_member::ns	= L"table";
const wchar_t * table_data_pilot_member::name	= L"data-pilot-member";

void table_data_pilot_member::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:display",		table_display_);
	CP_APPLY_ATTR(L"table:name",		table_name_);
	CP_APPLY_ATTR(L"table:show-details",table_show_details_);
}

void table_data_pilot_member::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_pivots_context().add_field_cache(-1, table_name_.get_value_or(L""), table_show_details_ ? table_show_details_->get() : true);
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_group_member::ns	= L"table";
const wchar_t * table_data_pilot_group_member::name	= L"data-pilot-group-member";

void table_data_pilot_group_member::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:name", table_name_);
}

void table_data_pilot_group_member::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_pivots_context().add_field_cache(-1, table_name_.get_value_or(L""));
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_subtotals::ns		= L"table";
const wchar_t * table_data_pilot_subtotals::name	= L"data-pilot-subtotals";

void table_data_pilot_subtotals::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_subtotals::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_subtotal::ns	= L"table";
const wchar_t * table_data_pilot_subtotal::name	= L"data-pilot-subtotal";

void table_data_pilot_subtotal::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:function",	table_function_);
}

void table_data_pilot_subtotal::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_pivots_context().add_field_subtotal(table_function_.get_value_or(table_function::Auto).get_type());
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_grand_total::ns	= L"tableooo";
const wchar_t * table_data_pilot_grand_total::name	= L"data-pilot-grand-total";

void table_data_pilot_grand_total::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:display"				, table_display_);
	CP_APPLY_ATTR(L"table:orientation"			, table_orientation_);
	//CP_APPLY_ATTR(L"table:display-name"		, table_display_name_);
	CP_APPLY_ATTR(L"tableooo:display-name"		, table_display_name_);
}
void table_data_pilot_grand_total::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_layout_info::ns	= L"table";
const wchar_t * table_data_pilot_layout_info::name	= L"data-pilot-layout-info";

void table_data_pilot_layout_info::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:add-empty-lines"	, table_add_empty_lines_);
	CP_APPLY_ATTR(L"table:layout-mode"		, table_layout_mode_);
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_sort_info::ns		= L"table";
const wchar_t * table_data_pilot_sort_info::name	= L"data-pilot-sort-info";

void table_data_pilot_sort_info::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:data-field"	, table_data_field_);
	CP_APPLY_ATTR(L"table:order"		, table_order_);
	CP_APPLY_ATTR(L"table:sort-mode"	, table_sort_mode_);
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_display_info::ns	= L"table";
const wchar_t * table_data_pilot_display_info::name	= L"data-pilot-display-info";

void table_data_pilot_display_info::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:data-field"			, table_data_field_);
	CP_APPLY_ATTR(L"table:display-member-mode"	, table_display_member_mode_);
	CP_APPLY_ATTR(L"table:enabled"				, table_enabled_);
	CP_APPLY_ATTR(L"table:member-count"			, table_member_count_);
}
}
}