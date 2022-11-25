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

#include "table_database_ranges.h"

#include <boost/make_shared.hpp>
#include <xml/xmlchar.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include "odf_document.h"

namespace cpdoccore { 

	using namespace odf_types;
namespace odf_reader {

//--------------------------------------------------------------------------------------------------
const wchar_t * table_database_ranges::ns = L"table";
const wchar_t * table_database_ranges::name = L"database-ranges";

void table_database_ranges::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
}

void table_database_ranges::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void table_database_ranges::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}

}
//--------------------------------------------------------------------------------------------------
const wchar_t * table_database_range::ns = L"table";
const wchar_t * table_database_range::name = L"database-range";

void table_database_range::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
    CP_APPLY_ATTR(L"table:name"						, table_name_);
    CP_APPLY_ATTR(L"table:target-range-address"		, table_target_range_address_);
    CP_APPLY_ATTR(L"table:display-filter-buttons"	, table_display_filter_buttons_);
	CP_APPLY_ATTR(L"table:orientation"				, table_orientation_);				
	CP_APPLY_ATTR(L"table:contains-header"			, table_contains_header_);				
}

void table_database_range::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);

}

void table_database_range::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (!table_target_range_address_)	return;
	if (!table_name_)	return;

	std::wstring name = table_name_.get();

	std::map<std::wstring, int>::iterator pFind = Context.mapUsedNames_.find(name);
	if (pFind == Context.mapUsedNames_.end())
	{
		Context.mapUsedNames_.insert(std::make_pair(name, 1));
	}
	else
	{
		name += L"_" + std::to_wstring(pFind->second);
		pFind->second++;
	}	
	
	if (Context.get_table_context().start_database_range(name, *table_target_range_address_))
	{	
		if (table_display_filter_buttons_)
			Context.get_table_context().set_database_filter(table_display_filter_buttons_->get());

		if (table_orientation_)
			Context.get_table_context().set_database_orientation(table_orientation_->get_type() == table_orientation::row ? true : false);

		if (table_contains_header_)
			Context.get_table_context().set_database_header(table_contains_header_->get());

		for (size_t i = 0; i < content_.size(); i++)
		{
			content_[i]->xlsx_convert(Context);
		}

		Context.get_table_context().end_database_range();
	}
	else
	{
		Context.get_xlsx_defined_names().add(name, table_target_range_address_.get(), false, -1);
	}
}

//--------------------------------------------------------------------------------------------------
const wchar_t * table_sort::ns		= L"table";
const wchar_t * table_sort::name	= L"sort";

void table_sort::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"table" == Ns && L"sort-by" == Name)
    {
        CP_CREATE_ELEMENT(content_);
    } 
}

void table_sort::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		table_sort_by * sort_by = dynamic_cast<table_sort_by*>(content_[i].get());

		int descending = 1;
		if ((sort_by->table_order_) && (sort_by->table_order_->get_type() == table_order::descending))
			descending = 2;

		Context.get_table_context().add_database_sort(sort_by->table_field_number_, descending); 
	}
}
//--------------------------------------------------------------------------------------------------
const wchar_t * table_sort_by::ns	= L"table";
const wchar_t * table_sort_by::name	= L"sort-by";

void table_sort_by::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
	CP_APPLY_ATTR(L"table:field-number"	, table_field_number_, 0);
	CP_APPLY_ATTR(L"table:data-type"	, table_data_type_);
	CP_APPLY_ATTR(L"table:order"		, table_order_);

}
//--------------------------------------------------------------------------------------------------
const wchar_t * table_filter::ns	= L"table";
const wchar_t * table_filter::name	= L"filter";

void table_filter::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
	CP_APPLY_ATTR(L"table:condition-source",				table_condition_source_);
	CP_APPLY_ATTR(L"table:condition-source-range-address",	table_condition_source_range_address_);
	CP_APPLY_ATTR(L"table:display-duplicates",				table_display_duplicates_);
	CP_APPLY_ATTR(L"table:target-range-address",			table_target_range_address_);
}

void table_filter::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void table_filter::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
}
//--------------------------------------------------------------------------------------------------
const wchar_t * table_filter_or::ns		= L"table";
const wchar_t * table_filter_or::name	= L"filter-or";

void table_filter_or::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void table_filter_or::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
}
//--------------------------------------------------------------------------------------------------
const wchar_t * table_filter_and::ns	= L"table";
const wchar_t * table_filter_and::name	= L"filter-and";

void table_filter_and::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void table_filter_and::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
}
//--------------------------------------------------------------------------------------------------
const wchar_t * table_filter_condition::ns		= L"table";
const wchar_t * table_filter_condition::name	= L"filter-condition";

void table_filter_condition::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
	CP_APPLY_ATTR(L"table:case-sensitive",	table_case_sensitive_);
	CP_APPLY_ATTR(L"table:data-type",		table_data_type_);
	CP_APPLY_ATTR(L"table:field-number",	table_field_number_);
	CP_APPLY_ATTR(L"table:operator",		table_operator_);
	CP_APPLY_ATTR(L"table:value",			table_value_);
}

void table_filter_condition::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void table_filter_condition::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
}
//--------------------------------------------------------------------------------------------------
const wchar_t * table_filter_set_item::ns	= L"table";
const wchar_t * table_filter_set_item::name = L"filter-set-item";

void table_filter_set_item::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
	CP_APPLY_ATTR(L"table:value",	table_value_);
}

void table_filter_set_item::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
}
}
