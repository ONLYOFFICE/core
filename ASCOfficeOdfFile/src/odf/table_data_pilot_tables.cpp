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

void table_data_pilot_tables::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

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
    CP_APPLY_ATTR(L"table:show-filterbutton"		, table_show_filterbutton_);
    CP_APPLY_ATTR(L"table:show-target-range-address", table_show_target_range_address_);

}

void table_data_pilot_table::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_table::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_data_pilot_field::ns		= L"table";
const wchar_t * table_data_pilot_field::name	= L"data-pilot-field";

void table_data_pilot_field::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_data_pilot_field::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_data_pilot_field::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_table::ns		= L"table";
const wchar_t * table_database_source_table::name	= L"database-source-table";

void table_database_source_table::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_database_source_table::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_database_source_table::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_query::ns		= L"table";
const wchar_t * table_database_source_query::name	= L"database-source-query";

void table_database_source_query::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_database_source_query::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_database_source_query::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_database_source_sql::ns	= L"table";
const wchar_t * table_database_source_sql::name	= L"database-source-sql";

void table_database_source_sql::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_database_source_sql::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_database_source_sql::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
//-------------------------------------------------------------------------------------------------
const wchar_t * table_source_cell_range::ns		= L"table";
const wchar_t * table_source_cell_range::name	= L"source-cell-range";

void table_source_cell_range::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
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
}

void table_source_service::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT (content_);
}
void table_source_service::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
	}
}
}
}
