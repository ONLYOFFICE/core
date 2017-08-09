/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>

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
    CP_APPLY_ATTR(L"table:drill-down-ondouble-click", table_drill_down_ondouble_click_);
    CP_APPLY_ATTR(L"table:grand-total"				, table_grand_total_);
    CP_APPLY_ATTR(L"table:identify-categories"		, table_identify_categories_);
    CP_APPLY_ATTR(L"table:ignore-empty-rows"		, table_ignore_empty_rows_);
    CP_APPLY_ATTR(L"table:show-filter-button"		, table_show_filter_button_);
    CP_APPLY_ATTR(L"table:show-target-range-address", table_show_target_range_address_);

}

void table_data_pilot_table::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"table" == Ns && L"data-pilot-field" == Name)
		CP_CREATE_ELEMENT (fields_);
	else
		CP_CREATE_ELEMENT (source_);

}
void table_data_pilot_table::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (!source_) return;

	Context.get_pivots_context().start_table();

	source_->xlsx_convert(Context);

	for (size_t i = 0; i < fields_.size(); i++)
	{
		fields_[i]->xlsx_convert(Context);
	}	  

	int index_view = Context.get_pivots_context().end_table();
	
	if (index_view > 0)
	{
		Context.current_sheet().sheet_rels().add(oox::relationship(L"pvId" + std::to_wstring(index_view),
			L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotTable",
			L"../pivotTables/pivotTable" + std::to_wstring(index_view) + L".xml"));
	} 
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
}

void table_data_pilot_field::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_field::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_pivots_context().start_field();

	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}			

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

}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_table::ns		= L"table";
const wchar_t * table_database_source_table::name	= L"database-source-table";

void table_database_source_table::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:database-name"		, table_database_name_);
	CP_APPLY_ATTR(L"table:database-table-name"	, table_database_table_name_);
}

void table_database_source_table::xlsx_convert(oox::xlsx_conversion_context & Context)
{

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

}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_source_cell_range::ns		= L"table";
const wchar_t * table_source_cell_range::name	= L"source-cell-range";

void table_source_cell_range::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:cellrange-address", table_cellrange_address_);
}

void table_source_cell_range::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_source_cell_range::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
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
	CP_APPLY_ATTR(L"table:show-empty-address", table_show_empty_);
}
void table_data_pilot_level::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_level::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
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
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
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

}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_group_member::ns	= L"table";
const wchar_t * table_data_pilot_group_member::name	= L"data-pilot-group-member";

void table_data_pilot_group_member::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:name",	table_name_);
}

void table_data_pilot_group_member::xlsx_convert(oox::xlsx_conversion_context & Context)
{

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

}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_layout_info::ns	= L"table";
const wchar_t * table_data_pilot_layout_info::name	= L"data-pilot-layout-info";

void table_data_pilot_layout_info::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:add-empty-lines"	, table_add_empty_lines_);
	CP_APPLY_ATTR(L"table:layout-mode"		, table_layout_mode_);
}
void table_data_pilot_layout_info::xlsx_convert(oox::xlsx_conversion_context & Context)
{

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
void table_data_pilot_sort_info::xlsx_convert(oox::xlsx_conversion_context & Context)
{

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
void table_data_pilot_display_info::xlsx_convert(oox::xlsx_conversion_context & Context)
{

}
}
}