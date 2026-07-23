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

#include "office_spreadsheet.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>

#include "office_elements_create.h"

#include <xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf_writer {

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_spreadsheet::ns = L"office";
const wchar_t * office_spreadsheet::name = L"spreadsheet";

office_spreadsheet::~office_spreadsheet()
{
	named_expressions_.reset();
	database_ranges_.reset();
	data_pilot_tables_.reset();
	content_validations_.reset();
	forms_.reset();

	content_.clear();
}
void office_spreadsheet::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
 	if CP_CHECK_NAME(L"office", L"forms") 
	{
		CP_CREATE_ELEMENT(forms_);
	}
	else if CP_CHECK_NAME(L"table", L"database-ranges")
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
	else if CP_CHECK_NAME(L"table", L"content-validations")
	{
		CP_CREATE_ELEMENT(content_validations_);
	}
	else
		CP_CREATE_ELEMENT(content_);}

void office_spreadsheet::add_child_element( const office_element_ptr & child_element)
{
	ElementType type_ = child_element->get_type();

	if ( type_ == typeOfficeForms)
    {
		forms_ = child_element;
    }
	else if ( type_ == typeTableContentValidations)
    {
		content_validations_ = child_element;
    }
	else if ( type_ == typeTableDataPilotTables)
    {
		data_pilot_tables_ = child_element;
    }
	else if ( type_ == typeTableDatabaseRanges)
    {
		database_ranges_ = child_element;
    }
	else if ( type_ == typeTableNamedExpressions)
    {
		named_expressions_ = child_element;
    }
	else
	{
		content_.push_back(child_element);
	}
}
void office_spreadsheet::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"table:structure-protected", table_structure_protected_);

			if (forms_)
				forms_->serialize(CP_XML_STREAM());

			if (content_validations_)
				content_validations_->serialize(CP_XML_STREAM());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
			
			if (named_expressions_)
				named_expressions_->serialize(CP_XML_STREAM());

			if (database_ranges_)
				database_ranges_->serialize(CP_XML_STREAM());		

			if (data_pilot_tables_)
				data_pilot_tables_->serialize(CP_XML_STREAM());
		}
	}
}

}
}
