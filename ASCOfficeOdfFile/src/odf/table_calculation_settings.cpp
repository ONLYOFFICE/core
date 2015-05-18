
#include "table_calculation_settings.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf {

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

void table_calculation_settings::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"null-date")
    {
        CP_CREATE_ELEMENT(table_null_date_);
    }
    else if CP_CHECK_NAME(L"table", L"null-date")
    {
    }
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

void table_null_date::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
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

void table_iteration::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void table_iteration::add_text(const std::wstring & Text)
{
}

}
}
