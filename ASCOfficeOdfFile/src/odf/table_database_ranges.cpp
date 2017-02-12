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

#include "table_database_ranges.h"

#include <boost/make_shared.hpp>
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>

namespace cpdoccore { 
namespace odf_reader {


// table:table-source
//////////////////////////////////////////////////////////////////////////////////////////////////
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
// table:named-range
//////////////////////////////////////////////////////////////////////////////////////////////////
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

	Context.get_table_context().start_database_range(table_name_.get_value_or(L""), *table_target_range_address_);
	
	if (table_display_filter_buttons_)
		Context.get_table_context().set_database_filter(table_display_filter_buttons_->get());

	if (table_orientation_)
		Context.get_table_context().set_database_orientation(*table_orientation_ == L"row" ? true : false);

	if (table_contains_header_)
		Context.get_table_context().set_database_header(table_contains_header_->get());

	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}

	Context.get_table_context().end_database_range();
}

// table:sort
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_sort::ns		= L"table";
const wchar_t * table_sort::name	= L"sort";

void table_sort::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"table" == Ns && L"sort-by" == Name)
    {
        CP_CREATE_ELEMENT(table_sort_by_);
    } 
}

void table_sort::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < table_sort_by_.size(); i++)
	{
		table_sort_by * sort_by = dynamic_cast<table_sort_by*>(table_sort_by_[i].get());

		int descending = 1;
		if ((sort_by->table_order_) && (sort_by->table_order_.get() == L"descending"))
			descending = 2;

		Context.get_table_context().add_database_sort(sort_by->table_field_number_, descending); 
	}
}
// table:sort-by
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_sort_by::ns	= L"table";
const wchar_t * table_sort_by::name	= L"sort-by";

void table_sort_by::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
	CP_APPLY_ATTR(L"table:field-number"	, table_field_number_, 0);
	CP_APPLY_ATTR(L"table:data-type"	, table_data_type_);
	CP_APPLY_ATTR(L"table:order"		, table_order_);

}

}
}
