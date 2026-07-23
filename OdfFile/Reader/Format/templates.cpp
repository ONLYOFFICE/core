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


#include "office_elements.h"
#include "office_elements_create.h"

#include "templates.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

const wchar_t * table_template_element::ns = L"table";
const wchar_t * table_template_element::name = L"template_element";

void table_template_element::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	std::wstring text_style_name_;

	const std::wstring empty = L"";
	CP_APPLY_ATTR(L"table:style-name",	table_style_name_, empty);
	CP_APPLY_ATTR(L"text:style-name",	text_style_name_, empty);

	if (table_style_name_.empty())
		table_style_name_ = text_style_name_;
	//http://lists.freedesktop.org/archives/libreoffice-bugs/2011-March/007109.html
	// file 1 (7).odp
}

const wchar_t * table_body_template::ns = L"table";
const wchar_t * table_body_template::name = L"body";

const wchar_t * table_first_row_template::ns = L"table";
const wchar_t * table_first_row_template::name = L"first-row";

const wchar_t * table_last_row_template::ns = L"table";
const wchar_t * table_last_row_template::name = L"last-row";

const wchar_t * table_first_column_template::ns = L"table";
const wchar_t * table_first_column_template::name = L"first-column";

const wchar_t * table_last_column_template::ns = L"table";
const wchar_t * table_last_column_template::name = L"last-column";

const wchar_t * table_odd_columns_template::ns = L"table";
const wchar_t * table_odd_columns_template::name = L"odd-columns";

const wchar_t * table_odd_rows_template::ns = L"table";
const wchar_t * table_odd_rows_template::name = L"odd-rows";

//----------------------------------------------------------------------------------------
const wchar_t * table_table_template::ns = L"table";
const wchar_t * table_table_template::name = L"table-template";

void table_table_template::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name",	text_style_name_);
}

void table_table_template::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{   
    if CP_CHECK_NAME(L"table", L"body")
    {
        CP_CREATE_ELEMENT(table_body_);    
    }
    else if CP_CHECK_NAME(L"table", L"first-row")
    {
        CP_CREATE_ELEMENT(table_first_row_);    
    }
    else if CP_CHECK_NAME(L"table", L"last-row")
    {
        CP_CREATE_ELEMENT(table_last_row_);    
    }
    else if CP_CHECK_NAME(L"table", L"first-column")
    {
        CP_CREATE_ELEMENT(table_first_column_);    
    }
    else if CP_CHECK_NAME(L"table", L"last-column")
    {
        CP_CREATE_ELEMENT(table_last_column_);    
    }
    else if CP_CHECK_NAME(L"table", L"odd-rows")
    {
        CP_CREATE_ELEMENT(table_odd_rows_);    
    }
    else if CP_CHECK_NAME(L"table", L"odd-columns")
    {
        CP_CREATE_ELEMENT(table_odd_columns_);    
    }
}
}
}