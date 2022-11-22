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

#include "table_named_expressions.h"

#include <boost/make_shared.hpp>
#include <xml/xmlchar.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include "odf_document.h"


namespace cpdoccore { 
namespace odf_reader {



// table:table-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_expressions::ns = L"table";
const wchar_t * table_named_expressions::name = L"named-expressions";

void table_named_expressions::add_attributes(const xml::attributes_wc_ptr & Attributes )
{

}

void table_named_expressions::add_child_element(xml::sax * Reader,
                                                const std::wstring & Ns,
                                                const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void table_named_expressions::docx_convert(oox::docx_conversion_context & Context)
{
}

void table_named_expressions::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }
}

// table:named-range
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_range::ns = L"table";
const wchar_t * table_named_range::name = L"named-range";

void table_named_range::docx_convert(oox::docx_conversion_context & Context)
{

}

void table_named_range::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    if (table_cell_range_address_ && table_name_)
    {
		int tableId = -1;
		if (Context.get_table_context().state())
		{
			tableId = Context.get_table_context().state()->get_table_id();
		}
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
		
		Context.get_xlsx_defined_names().add(name, table_cell_range_address_.get(), false, tableId);
    }
}

void table_named_range::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
    CP_APPLY_ATTR(L"table:name",				table_name_);
    CP_APPLY_ATTR(L"table:cell-range-address",	table_cell_range_address_);
    CP_APPLY_ATTR(L"table:base-cell-address",	table_base_cell_address_);
}

void table_named_range::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

// table:named-expression
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_expression::ns = L"table";
const wchar_t * table_named_expression::name = L"named-expression";

void table_named_expression::docx_convert(oox::docx_conversion_context & Context)
{

}

void table_named_expression::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    if (table_expression_ && table_name_)
    {
 		int tableId = -1;
		if (Context.get_table_context().state())
		{
			tableId = Context.get_table_context().state()->get_table_id();
		}
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
        Context.get_xlsx_defined_names().add(name, table_expression_.get(), true, tableId);
    }
}

void table_named_expression::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
    CP_APPLY_ATTR(L"table:name", table_name_);
    CP_APPLY_ATTR(L"table:expression", table_expression_);
    CP_APPLY_ATTR(L"table:base-cell-address", table_base_cell_address_);
}

void table_named_expression::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

}
}
