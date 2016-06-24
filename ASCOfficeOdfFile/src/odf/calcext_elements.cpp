/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#include "calcs_styles.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


void calcext_data_bar_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:axis-color",		calcext_axis_color_);
	CP_APPLY_ATTR(L"calcext:positive-color",	calcext_positive_color_);
	CP_APPLY_ATTR(L"calcext:negative-color",	calcext_negative_color_);
	CP_APPLY_ATTR(L"calcext:min-length",		calcext_min_length_);
	CP_APPLY_ATTR(L"calcext:max-length",		calcext_max_length_);
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
	for (int i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
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
	if (!calcext_target_range_address_) return;

	Context.start_conditional_format(*calcext_target_range_address_);

	for (int i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
	Context.end_conditional_format();
}
	
// calcext_data_bar
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_data_bar::ns = L"calcext";
const wchar_t * calcext_data_bar::name = L"data-bar";

void calcext_data_bar::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:show-value", calcext_show_value_);
	calcext_data_bar_attr_.add_attributes(Attributes);

}
void calcext_data_bar::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_data_bar::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.start_conditional_format_rule(2);
	if (calcext_show_value_)
		Context.set_conditional_format_showval(*calcext_show_value_);

	if (calcext_data_bar_attr_.calcext_positive_color_)
		Context.add_conditional_format_color(L"ff" + calcext_data_bar_attr_.calcext_positive_color_->get_hex_value());

	Context.set_conditional_format_dataBar(calcext_data_bar_attr_.calcext_min_length_, calcext_data_bar_attr_.calcext_max_length_);

	for (int i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
	Context.end_conditional_format_rule();	
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
	Context.start_conditional_format_rule(3);
	for (int i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
	Context.end_conditional_format_rule();
}
// calcext_icon_set
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_icon_set::ns = L"calcext";
const wchar_t * calcext_icon_set::name = L"icon-set";

void calcext_icon_set::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:show-value", calcext_show_value_);
	calcext_icon_set_attr_.add_attributes(Attributes);
}
void calcext_icon_set::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_icon_set::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.start_conditional_format_rule(4);
	if (calcext_show_value_)
		Context.set_conditional_format_showval(*calcext_show_value_);
	
	for (int i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
	Context.end_conditional_format_rule();
}
	
// calcext_formatting_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_formatting_entry::ns = L"calcext";
const wchar_t * calcext_formatting_entry::name = L"formatting-entry";

void calcext_formatting_entry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:value",				calcext_value_);
	CP_APPLY_ATTR(L"calcext:type",				calcext_type_);
}
void calcext_formatting_entry::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_formatting_entry::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	calcext_type::type type_ = calcext_type_.get_value_or(calcext_type::Number).get_type();
	Context.add_conditional_format_entry((int)type_, calcext_value_.get_value_or(L""));
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
	if (calcext_color_)
		Context.add_conditional_format_color(L"ff" + calcext_color_->get_hex_value());
	else 
		Context.add_conditional_format_color(L"ffffffff");

	calcext_type::type type_ = calcext_type_.get_value_or(calcext_type::Number).get_type();
	Context.add_conditional_format_entry((int)type_, calcext_value_.get_value_or(L""));
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
	Context.start_conditional_format_rule(1);

	if (calcext_condition_attr_.calcext_value_)
	{
		Context.set_conditional_format_formula(*calcext_condition_attr_.calcext_value_);
	}
	std::wstring style_name = calcext_condition_attr_.calcext_apply_style_name_.get_value_or(L"");
	
	style_instance * instStyle = 
				Context.root()->odf_context().styleContainer().style_by_name(style_name, style_family::TableCell, false);
	if (!instStyle)
		instStyle = Context.root()->odf_context().styleContainer().style_by_display_name(style_name, style_family::TableCell, false);
	if (instStyle)
	{
		text_format_properties_content		textFormats		= calc_text_properties_content		(instStyle);
		graphic_format_properties			graphicFormats	= calc_graphic_properties_content	(instStyle);
		style_table_cell_properties_attlist	cellFormats		= calc_table_cell_properties		(instStyle);
	
		int dxfId = Context.get_style_manager().dxfId(&textFormats, &graphicFormats, &cellFormats);
		Context.set_conditional_format_dxf(dxfId);
	}
	Context.end_conditional_format_rule();
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
	//Context.start_conditional_format_rule(5);
	//Context.end_conditional_format_rule();
}

}
}