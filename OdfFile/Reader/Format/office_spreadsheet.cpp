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

#include "office_spreadsheet.h"

#include <xml/xmlchar.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf_reader {

// office:spreadsheet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_spreadsheet::ns = L"office";
const wchar_t * office_spreadsheet::name = L"spreadsheet";

void office_spreadsheet::add_attributes(const xml::attributes_wc_ptr & Attributes)
{
	CP_APPLY_ATTR(L"table:structure-protected", table_structure_protected_);
	CP_APPLY_ATTR(L"table:protection-key", table_protection_key_);
	CP_APPLY_ATTR(L"table:protection-key-digest-algorithm", table_protection_key_digest_algorithm_);
}

void office_spreadsheet::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"table", L"database-ranges")
	{
		CP_CREATE_ELEMENT(database_ranges_);
	}
	else if CP_CHECK_NAME(L"table", L"named-expressions")
    {
        CP_CREATE_ELEMENT(named_expressions_);    
    }
	else if CP_CHECK_NAME(L"table", L"data-pilot-tables")
	{
		CP_CREATE_ELEMENT(data_pilot_tables_);
	}
	else if CP_CHECK_NAME(L"table", L"tracked-changes")
	{
		CP_CREATE_ELEMENT(tracked_changes_);
	}
	else if CP_CHECK_NAME(L"table", L"content-validations")
	{
		CP_CREATE_ELEMENT(content_validations_);
	}
	else if CP_CHECK_NAME(L"text", L"user-field-decls")
	{
		CP_CREATE_ELEMENT(user_fields_);
	}
	else if CP_CHECK_NAME(L"text", L"sequence-decls")
	{
		CP_CREATE_ELEMENT(sequences_);
	}
	else if CP_CHECK_NAME(L"text", L"variable-decls")
	{
		CP_CREATE_ELEMENT(variables_);
	}
	else
		CP_CREATE_ELEMENT(content_);
}

void office_spreadsheet::docx_convert(oox::docx_conversion_context & Context)
{
    Context.start_office_text();
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
    Context.end_office_text();
}

void office_spreadsheet::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_office_spreadsheet(this);
    _CP_LOG << L"[info][xlsx] process spreadsheet (" << content_.size() << L" tables)" << std::endl;
   
	if (table_structure_protected_)
	{
		Context.set_table_structure_protected(*table_structure_protected_);
	}
	if (user_fields_)
		user_fields_->xlsx_convert(Context);

	if (variables_)
		variables_->xlsx_convert(Context);

	if (sequences_)
		sequences_->xlsx_convert(Context);

	if (named_expressions_)
		named_expressions_->xlsx_convert(Context);
       
	if (database_ranges_)
		database_ranges_->xlsx_convert(Context);

	if (content_validations_)
		content_validations_->xlsx_convert(Context);

	if (data_pilot_tables_)
		data_pilot_tables_->xlsx_convert(Context);
	
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }        

	Context.end_office_spreadsheet();
}

}
}
