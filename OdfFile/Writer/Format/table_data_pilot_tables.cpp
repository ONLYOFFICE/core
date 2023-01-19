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
#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_tables::ns		= L"table";
const wchar_t * table_data_pilot_tables::name	= L"data-pilot-tables";

void table_data_pilot_tables::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void table_data_pilot_tables::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	content_.push_back(child_element);
}
void table_data_pilot_tables::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {    
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_table::ns		= L"table";
const wchar_t * table_data_pilot_table::name	= L"data-pilot-table";

void table_data_pilot_table::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableDataPilotField) 
	{
		fields_.push_back(child_element);
	}
    else if (type_ == typeTableDataPilotGrandTotal) 
	{
		grand_total_ = child_element;
	}
	else
		source_ = child_element;
}

void table_data_pilot_table::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	if (L"table" == Ns && L"data-pilot-field" == Name)
		CP_CREATE_ELEMENT (fields_);
	else if (L"tableooo" == Ns && L"data-pilot-grand-total" == Name)
		CP_CREATE_ELEMENT (grand_total_);
	else
		CP_CREATE_ELEMENT (source_);

}

void table_data_pilot_table::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name"		, table_name_);
			CP_XML_ATTR(L"table:application-data"			, table_application_data_);
			CP_XML_ATTR(L"table:buttons"					, table_buttons_);
			CP_XML_ATTR(L"table:drill-down-on-double-click"	, table_drill_down_ondouble_click_);
			CP_XML_ATTR(L"table:grand-total"				, table_grand_total_);
			CP_XML_ATTR(L"table:identify-categories"		, table_identify_categories_);
			CP_XML_ATTR(L"table:ignore-empty-rows"			, table_ignore_empty_rows_);
			CP_XML_ATTR(L"table:show-filter-button"			, table_show_filter_button_);
			CP_XML_ATTR(L"table:target-range-address"		, table_target_range_address_);

			if (source_)
				source_->serialize(CP_XML_STREAM());
			if (grand_total_)
				grand_total_->serialize(CP_XML_STREAM());

			for (size_t i = 0; i < fields_.size(); i++)
			{
				fields_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_field::ns		= L"table";
const wchar_t * table_data_pilot_field::name	= L"data-pilot-field";

void table_data_pilot_field::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	if (L"table" == Ns && L"data-pilot-field-reference" == Name)
		CP_CREATE_ELEMENT (reference_);  
	else if (L"table" == Ns && L"data-pilot-level" == Name)
		CP_CREATE_ELEMENT (level_);  
	else if (L"table" == Ns && L"data-pilot-groups" == Name)
		CP_CREATE_ELEMENT (groups_); 
}
void table_data_pilot_field::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;
	
	ElementType type_ = child_element->get_type();
    
	if (type_ == typeTableDataPilotFieldReference) 
	{
		reference_ = child_element;
	}
    else if (type_ == typeTableDataPilotLevel)
	{
		level_ = child_element;
	}
    else if (type_ == typeTableDataPilotGroups)
	{
		groups_ = child_element;
	}
}
void table_data_pilot_field::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR(L"table:source-field-name"		, table_source_field_name_);
			CP_XML_ATTR(L"table:orientation"			, table_orientation_);
			CP_XML_ATTR(L"table:used-hierarchy"			, table_used_hierarchy_);
			CP_XML_ATTR(L"table:function"				, table_function_);
			CP_XML_ATTR(L"loext:ignore-selected-page"	, loext_ignore_selected_page_);
			CP_XML_ATTR(L"table:selected-page"			, table_selected_page_);
			CP_XML_ATTR(L"table:is-data-layout-field"	, table_is_data_layout_field_);
			CP_XML_ATTR(L"tableooo:display-name"		, table_display_name_);

			if (level_)
				level_->serialize(CP_XML_STREAM());

			if (reference_)
				reference_->serialize(CP_XML_STREAM());

			if (groups_)
				groups_->serialize(CP_XML_STREAM());
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_field_reference::ns	= L"table";
const wchar_t * table_data_pilot_field_reference::name	= L"data-pilot-field-reference";

void table_data_pilot_field_reference::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR(L"table:field-name"		, table_field_name_);
			CP_XML_ATTR(L"table:member-name"	, table_member_name_);
			CP_XML_ATTR(L"table:member-type"	, table_member_type_);
			CP_XML_ATTR(L"table:type"			, table_type_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_table::ns		= L"table";
const wchar_t * table_database_source_table::name	= L"database-source-table";

void table_database_source_table::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR(L"table:database-name", table_database_name_);
			CP_XML_ATTR(L"table:table-name"	, table_table_name_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_query::ns		= L"table";
const wchar_t * table_database_source_query::name	= L"database-source-query";

void table_database_source_query::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR(L"table:database-name"	, table_database_name_);
			CP_XML_ATTR(L"table:query-name"		, table_query_name_);	
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_sql::ns	= L"table";
const wchar_t * table_database_source_sql::name	= L"database-source-sql";

void table_database_source_sql::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR(L"table:database-name"		, table_database_name_);
			CP_XML_ATTR(L"table:parse-sql-statement", table_parse_sql_statement_);
			CP_XML_ATTR(L"table:sql-statement"		, table_sql_statement_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_source_cell_range::ns		= L"table";
const wchar_t * table_source_cell_range::name	= L"source-cell-range";

void table_source_cell_range::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_source_cell_range::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;
	
	content_.push_back(child_element);
}
void table_source_cell_range::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:cell-range-address", table_cell_range_address_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_source_cell_ranges::ns	= L"table";
const wchar_t * table_source_cell_ranges::name	= L"source-cell-ranges";

void table_source_cell_ranges::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:cell-ranges-address", table_cell_ranges_address_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_source_service::ns	= L"table";
const wchar_t * table_source_service::name	= L"source-service";

void table_source_service::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
			CP_XML_ATTR(L"table:object-name", table_object_name_);
			CP_XML_ATTR(L"table:password",	table_password_);
			CP_XML_ATTR(L"table:source-name", table_source_name_);
			CP_XML_ATTR(L"table:user-name",	table_user_name_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_level::ns		= L"table";
const wchar_t * table_data_pilot_level::name	= L"data-pilot-level";

void table_data_pilot_level::create_child_element( const std::wstring & Ns, const std::wstring & Name)
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
void table_data_pilot_level::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;
	
	ElementType type_ = child_element->get_type();

	if (type_ == typeTableDataPilotMembers) 
	{
		members_ = child_element;
	}
    else if (type_ == typeTableDataPilotSubtotals)
	{
		subtotals_ = child_element;
	}
    else if (type_ == typeTableDataDisplayInfo)
	{
		display_info_ = child_element;
	}
    else if (type_ == typeTableDataLayoutInfo)
	{
		layout_info_ = child_element;
	}    else if (type_ == typeTableDataSortInfo)
	{
		sort_info_ = child_element;
	}
}
void table_data_pilot_level::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:show-empty", table_show_empty_);
			CP_XML_ATTR(L"calcext:repeat-item-labels", calcext_repeat_item_labels_);

			if (display_info_)
				display_info_->serialize(CP_XML_STREAM());

			if (layout_info_)
				layout_info_->serialize(CP_XML_STREAM());

			if (sort_info_)
				sort_info_->serialize(CP_XML_STREAM());

			if (display_info_)
				display_info_->serialize(CP_XML_STREAM());

			if (subtotals_)
				subtotals_->serialize(CP_XML_STREAM());
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_groups::ns		= L"table";
const wchar_t * table_data_pilot_groups::name	= L"data-pilot-groups";

void table_data_pilot_groups::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_groups::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;
	
	content_.push_back(child_element);
}
void table_data_pilot_groups::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:date-start",		table_date_start_);
			CP_XML_ATTR(L"table:date-end",			table_date_end_);
			CP_XML_ATTR(L"table:start",				table_start);
			CP_XML_ATTR(L"table:end",				table_end_);
			CP_XML_ATTR(L"table:grouped-by",		table_grouped_by_);
			CP_XML_ATTR(L"table:source-field-name",	table_source_field_name_);
			CP_XML_ATTR(L"table:step",				table_step);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_group::ns		= L"table";
const wchar_t * table_data_pilot_group::name	= L"data-pilot-group";

void table_data_pilot_group::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_group::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;
	
	content_.push_back(child_element);
}
void table_data_pilot_group::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_members::ns	= L"table";
const wchar_t * table_data_pilot_members::name	= L"data-pilot-members";

void table_data_pilot_members::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_members::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;
	
	content_.push_back(child_element);
}
void table_data_pilot_members::serialize(std::wostream & _Wostream)
{
     CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	 }
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_member::ns	= L"table";
const wchar_t * table_data_pilot_member::name	= L"data-pilot-member";

void table_data_pilot_member::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
			CP_XML_ATTR(L"table:display", table_display_);
			CP_XML_ATTR(L"table:show-details", table_show_details_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_group_member::ns	= L"table";
const wchar_t * table_data_pilot_group_member::name	= L"data-pilot-group-member";

void table_data_pilot_group_member::serialize(std::wostream & _Wostream)
{
     CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
		}
	 }
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_subtotals::ns		= L"table";
const wchar_t * table_data_pilot_subtotals::name	= L"data-pilot-subtotals";

void table_data_pilot_subtotals::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_subtotals::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;
	
	content_.push_back(child_element);
}
void table_data_pilot_subtotals::serialize(std::wostream & _Wostream)
{
     CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	 }
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_subtotal::ns	= L"table";
const wchar_t * table_data_pilot_subtotal::name	= L"data-pilot-subtotal";

void table_data_pilot_subtotal::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:function",	table_function_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_grand_total::ns	= L"tableooo";
const wchar_t * table_data_pilot_grand_total::name	= L"data-pilot-grand-total";

void table_data_pilot_grand_total::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:display"				, table_display_);
			CP_XML_ATTR(L"table:orientation"			, table_orientation_);
			//CP_XML_ATTR(L"table:display-name"			, table_display_name_);
			CP_XML_ATTR(L"tableooo:display-name"		, table_display_name_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_layout_info::ns	= L"table";
const wchar_t * table_data_pilot_layout_info::name	= L"data-pilot-layout-info";

void table_data_pilot_layout_info::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:add-empty-lines"	, table_add_empty_lines_);
			CP_XML_ATTR(L"table:layout-mode"		, table_layout_mode_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_sort_info::ns		= L"table";
const wchar_t * table_data_pilot_sort_info::name	= L"data-pilot-sort-info";

void table_data_pilot_sort_info::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:data-field"	, table_data_field_);
			CP_XML_ATTR(L"table:order"		, table_order_);
			CP_XML_ATTR(L"table:sort-mode"	, table_sort_mode_);
		}
	}
}
//----------------------------------1---------------------------------------------------------------
const wchar_t * table_data_pilot_display_info::ns	= L"table";
const wchar_t * table_data_pilot_display_info::name	= L"data-pilot-display-info";

void table_data_pilot_display_info::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"table:data-field"			, table_data_field_);
			CP_XML_ATTR(L"table:display-member-mode"	, table_display_member_mode_);
			CP_XML_ATTR(L"table:enabled"				, table_enabled_);
			CP_XML_ATTR(L"table:member-count"			, table_member_count_);
		}
	}
}
}
}
