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

#include "table_calculation_settings.h"

#include <xml/xmlchar.h>

namespace cpdoccore { 
namespace odf_reader {

// office:calculation-settings
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_calculation_settings::ns = L"table";
const wchar_t * table_calculation_settings::name = L"calculation-settings";

void table_calculation_settings::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:case-sensitive", table_case_sensitive_);
    CP_APPLY_ATTR(L"table:precision-as-shown", table_precision_as_shown_);
    CP_APPLY_ATTR(L"table:search-criteria-must-apply-to-whole-cell", table_search_criteria_must_apply_to_whole_cell_);
    CP_APPLY_ATTR(L"table:automatic-find-labels", table_automatic_find_labels_);
    CP_APPLY_ATTR(L"table:use-regular-expressions", table_use_regular_expressions_);
    CP_APPLY_ATTR(L"table:null-year", table_null_year_);
}

void table_calculation_settings::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"null-date")
    {
        CP_CREATE_ELEMENT(table_null_date_);
    }
    //else if CP_CHECK_NAME(L"table", L"iteration")
    //{
    //}
    else
        CP_NOT_APPLICABLE_ELM();
}

void table_calculation_settings::add_text(const std::wstring & Text)
{}

// table:null-date
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_null_date::ns = L"table";
const wchar_t * table_null_date::name = L"null-date";

void table_null_date::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:value-type", table_value_type_);
    CP_APPLY_ATTR(L"table:date-value-type", table_date_value_type_);
}

void table_null_date::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void table_null_date::add_text(const std::wstring & Text)
{
}

// office:iteration
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_iteration::ns = L"table";
const wchar_t * table_iteration::name = L"iteration";

void table_iteration::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:status", table_status_);
    CP_APPLY_ATTR(L"table:steps", table_steps_);
    CP_APPLY_ATTR(L"table:maximum-difference", table_maximum_difference_);
}

void table_iteration::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void table_iteration::add_text(const std::wstring & Text)
{
}

}
}
