#include "calcext_elements.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/odf/odf_document.h>

#include "serialize_elements.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


void calcext_data_bar_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:axis-color",		calcext_axis_color_);
	CP_APPLY_ATTR(L"calcext:positive-color",	calcext_positive_color_);
	CP_APPLY_ATTR(L"calcext:negative-color",	calcext_negative_color_);
}

void calcext_icon_set_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:icon-set-type",	calcext_icon_set_type_); 

}
void calcext_condition_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:base-cell-address",	calcext_base_cell_address_);
	CP_APPLY_ATTR(L"calcext:apply-style-name",	calcext_apply_style_name_);
	CP_APPLY_ATTR(L"calcext:value",				calcext_value_);
}
void calcext_date_is_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:style",	calcext_style_);
	CP_APPLY_ATTR(L"calcext:date",	calcext_date_);
}
// calcext_conditional_formats
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_conditional_formats::ns = L"calcext";
const wchar_t * calcext_conditional_formats::name = L"conditional-formats";

void calcext_conditional_formats::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void calcext_conditional_formats::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_conditional_formats::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
	
// calcext_conditional_format
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_conditional_format::ns = L"calcext";
const wchar_t * calcext_conditional_format::name = L"conditional-format";

void calcext_conditional_format::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:target-range-address",	calcext_target_range_address_);
}
void calcext_conditional_format::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_conditional_format::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
	
// calcext_data_bar
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_data_bar::ns = L"calcext";
const wchar_t * calcext_data_bar::name = L"data-bar";

void calcext_data_bar::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	calcext_data_bar_attr_.add_attributes(Attributes);
}
void calcext_data_bar::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_data_bar::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	
}
// calcext_color_scale
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_color_scale::ns = L"calcext";
const wchar_t * calcext_color_scale::name = L"color-scale";

void calcext_color_scale::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void calcext_color_scale::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_color_scale::xlsx_convert(oox::xlsx_conversion_context & Context)
{

}
// calcext_icon_set
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_icon_set::ns = L"calcext";
const wchar_t * calcext_icon_set::name = L"icon-set";

void calcext_icon_set::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	calcext_icon_set_attr_.add_attributes(Attributes);
}
void calcext_icon_set::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_icon_set::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
	
// calcext_formatting_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_formatting_entry::ns = L"calcext";
const wchar_t * calcext_formatting_entry::name = L"formatting-entry";

void calcext_formatting_entry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void calcext_formatting_entry::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_formatting_entry::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}

// calcext_color_scale_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_color_scale_entry::ns = L"calcext";
const wchar_t * calcext_color_scale_entry::name = L"color_scale_entry";

void calcext_color_scale_entry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void calcext_color_scale_entry::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_color_scale_entry::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
// calcext_condition
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_condition::ns = L"calcext";
const wchar_t * calcext_condition::name = L"condition";

void calcext_condition::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	calcext_condition_attr_.add_attributes(Attributes);
}
void calcext_condition::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_condition::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
// calcext_condition
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_date_is::ns = L"calcext";
const wchar_t * calcext_date_is::name = L"date-is";

void calcext_date_is::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	calcext_date_is_attr_.add_attributes(Attributes);
}
void calcext_date_is::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_date_is::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}

}
}