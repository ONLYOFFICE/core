/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "calcext_elements.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/regex.hpp>

#include <xml/xmlchar.h>

#include "../Format/odf_document.h"
#include "../../Formulas/formulasconvert.h"

#include "serialize_elements.h"
#include "calcs_styles.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


void calcext_data_bar_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:axis-color",		axis_color_);
	CP_APPLY_ATTR(L"calcext:positive-color",	positive_color_);
	CP_APPLY_ATTR(L"calcext:negative-color",	negative_color_);
	CP_APPLY_ATTR(L"calcext:min-length",		min_length_);
	CP_APPLY_ATTR(L"calcext:max-length",		max_length_);
	CP_APPLY_ATTR(L"calcext:axis-position",		axis_position_);
	CP_APPLY_ATTR(L"calcext:gradient",			gradient_);
}

void calcext_icon_set_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:icon-set-type",	icon_set_type_); 

}
void calcext_condition_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:base-cell-address",	base_cell_address_);
	CP_APPLY_ATTR(L"calcext:apply-style-name",	apply_style_name_);
	CP_APPLY_ATTR(L"calcext:value",				value_);
	CP_APPLY_ATTR(L"loext:stdDev", loext_stdDev_);
}
void calcext_date_is_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:style",	style_);
	CP_APPLY_ATTR(L"calcext:date",	date_);
}
void calcext_sparkline_group_attr::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"calcext:id", id_);
	CP_APPLY_ATTR(L"calcext:type", type_);
	CP_APPLY_ATTR(L"calcext:line-width", line_width_);
	CP_APPLY_ATTR(L"calcext:first", first_);
	CP_APPLY_ATTR(L"calcext:last", last_);
	CP_APPLY_ATTR(L"calcext:markers", markers_);
	CP_APPLY_ATTR(L"calcext:display-hidden", display_hidden_);
	CP_APPLY_ATTR(L"calcext:right-to-left", right_to_left_);
	CP_APPLY_ATTR(L"calcext:display-x-axis", display_x_axis_);
	CP_APPLY_ATTR(L"calcext:display-empty-cells-as", display_empty_cells_as_);
	CP_APPLY_ATTR(L"calcext:min-axis-type", min_axis_type_);
	CP_APPLY_ATTR(L"calcext:max-axis-type", max_axis_type_);
	CP_APPLY_ATTR(L"calcext:manual-min", manual_min_);
	CP_APPLY_ATTR(L"calcext:color-series", color_series_);
	CP_APPLY_ATTR(L"calcext:color-negative", color_negative_);
	CP_APPLY_ATTR(L"calcext:color-axis", color_axis_);
	CP_APPLY_ATTR(L"calcext:color-markers", color_markers_);
	CP_APPLY_ATTR(L"calcext:color-first", color_first_);
	CP_APPLY_ATTR(L"calcext:color-last", color_last_);
	CP_APPLY_ATTR(L"calcext:color-high", color_high_);
	CP_APPLY_ATTR(L"calcext:color-low", color_low_);
	CP_APPLY_ATTR(L"calcext:date-axis", date_axis_);
	CP_APPLY_ATTR(L"calcext:high", high_);
	CP_APPLY_ATTR(L"calcext:low", low_);
}
// calcext_conditional_formats
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_conditional_formats::ns		= L"calcext";
const wchar_t * calcext_conditional_formats::name	= L"conditional-formats";

void calcext_conditional_formats::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void calcext_conditional_formats::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_conditional_formats::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
}
	
// calcext_conditional_format
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_conditional_format::ns		= L"calcext";
const wchar_t * calcext_conditional_format::name	= L"conditional-format";

void calcext_conditional_format::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:target-range-address",	target_range_address_);
}
void calcext_conditional_format::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_conditional_format::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (!target_range_address_) return;

	Context.get_conditionalFormatting_context().start(*target_range_address_);

	for (size_t i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
	Context.get_conditionalFormatting_context().end();
}
	
// calcext_data_bar
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_data_bar::ns	= L"calcext";
const wchar_t * calcext_data_bar::name	= L"data-bar";

void calcext_data_bar::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:show-value", show_value_);
	attr_.add_attributes(Attributes);

}
void calcext_data_bar::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_data_bar::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_conditionalFormatting_context().add_rule(2);
	if (show_value_)
		Context.get_conditionalFormatting_context().set_showVal(*show_value_);

	if (attr_.positive_color_)
		Context.get_conditionalFormatting_context().add_color(L"ff" + attr_.positive_color_->get_hex_value());
	if (attr_.negative_color_)
		Context.get_conditionalFormatting_context().set_negative_color(L"ff" + attr_.negative_color_->get_hex_value());

	if (attr_.axis_position_)
		Context.get_conditionalFormatting_context().set_axis_position(*attr_.axis_position_);

	if (attr_.gradient_)
		Context.get_conditionalFormatting_context().set_gradient(*attr_.gradient_);

	Context.get_conditionalFormatting_context().set_dataBar(attr_.min_length_, attr_.max_length_);

	for (size_t i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
}
// calcext_color_scale
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_color_scale::ns		= L"calcext";
const wchar_t * calcext_color_scale::name	= L"color-scale";

void calcext_color_scale::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void calcext_color_scale::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_color_scale::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_conditionalFormatting_context().add_rule(3);
	for (size_t i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
}
// calcext_icon_set
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_icon_set::ns	= L"calcext";
const wchar_t * calcext_icon_set::name	= L"icon-set";

void calcext_icon_set::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	attr_.add_attributes(Attributes);
}
void calcext_icon_set::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_icon_set::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_conditionalFormatting_context().add_rule(4);
	
	if (attr_.icon_set_type_)
		Context.get_conditionalFormatting_context().set_icon_set_type(attr_.icon_set_type_->get_type());
	
	for (size_t i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
	}
}
	
///  calcext:formatting-entry
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_formatting_entry::ns	= L"calcext";
const wchar_t * calcext_formatting_entry::name	= L"formatting-entry";

void calcext_formatting_entry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:value", value_);
	CP_APPLY_ATTR(L"calcext:type", type_);
	CP_APPLY_ATTR(L"calcext:show-value", show_value_);
}
void calcext_formatting_entry::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_formatting_entry::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	calcext_type::type t = type_.get_value_or(calcext_type::Number).get_type();
	Context.get_conditionalFormatting_context().add_sfv((int)t, value_.get_value_or(L""));
	
	if (show_value_)
		Context.get_conditionalFormatting_context().set_showVal(*show_value_);
}

// calcext_color_scale_entry
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_color_scale_entry::ns	= L"calcext";
const wchar_t * calcext_color_scale_entry::name = L"color-scale-entry";

void calcext_color_scale_entry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"calcext:value",	value_);
	CP_APPLY_ATTR(L"calcext:type",	type_);
	CP_APPLY_ATTR(L"calcext:color",	color_);
}
void calcext_color_scale_entry::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_color_scale_entry::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (color_)
		Context.get_conditionalFormatting_context().add_color(L"ff" + color_->get_hex_value());
	else 
		Context.get_conditionalFormatting_context().add_color(L"ffffffff");

	calcext_type::type t = type_.get_value_or(calcext_type::Number).get_type();
	Context.get_conditionalFormatting_context().add_sfv((int)t, value_.get_value_or(L""));
}
// calcext_condition
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_condition::ns	= L"calcext";
const wchar_t * calcext_condition::name = L"condition";

void calcext_condition::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	attr_.add_attributes(Attributes);
}
void calcext_condition::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_condition::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_conditionalFormatting_context().add_rule(1);

	if (attr_.value_)
	{
		Context.get_conditionalFormatting_context().set_formula(*attr_.value_);
	}
	std::wstring style_name = attr_.apply_style_name_.get_value_or(L"");
	
	int dxfId = Context.get_dxfId_style(style_name);

	if (dxfId >= 0)
		Context.get_conditionalFormatting_context().set_dxf(dxfId);

	if (attr_.loext_stdDev_)
		Context.get_conditionalFormatting_context().set_stdDev(*attr_.loext_stdDev_);
}
// calcext_condition
//---------------------------------------------------------------------------------------------------------
const wchar_t * calcext_date_is::ns		= L"calcext";
const wchar_t * calcext_date_is::name	= L"date-is";

void calcext_date_is::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	attr_.add_attributes(Attributes);
}
void calcext_date_is::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}
void calcext_date_is::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_conditionalFormatting_context().add_rule(5);
	
	Context.get_conditionalFormatting_context().set_time_period(attr_.date_.get_value_or(odf_types::time_period::today).get_type());
	
	std::wstring style_name = attr_.style_.get_value_or(L"");

	int dxfId = Context.get_dxfId_style(style_name);

	if (dxfId >= 0)
		Context.get_conditionalFormatting_context().set_dxf(dxfId);
}
//---------------------------------------------------------------------------------------------------------
const wchar_t* calcext_sparkline_groups::ns = L"calcext";
const wchar_t* calcext_sparkline_groups::name = L"sparkline-groups";

void calcext_sparkline_groups::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
}
void calcext_sparkline_groups::add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_sparkline_groups::xlsx_convert(oox::xlsx_conversion_context& Context)
{
	CP_XML_WRITER(Context.current_sheet().sparklines())
	{
		CP_XML_NODE(L"x14:sparklineGroups")
		{
			CP_XML_ATTR(L"xmlns:xm", L"http://schemas.microsoft.com/office/excel/2006/main");

			for (size_t i = 0; i < content_.size(); i++)
			{
				calcext_sparkline_group *group = dynamic_cast<calcext_sparkline_group*>(content_[i].get());
				group->serialize(CP_XML_STREAM());
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
const wchar_t* calcext_sparkline_group::ns = L"calcext";
const wchar_t* calcext_sparkline_group::name = L"sparkline-group";

void calcext_sparkline_group::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	attr_.add_attributes(Attributes);
}
void calcext_sparkline_group::add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_sparkline_group::serialize(std::wostream& strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"x14:sparklineGroup")
		{
			CP_XML_ATTR_OPT(L"xr2:uid", attr_.id_);
			CP_XML_ATTR_OPT(L"type", attr_.type_);
			CP_XML_ATTR_OPT(L"ref", attr_.ref_);
			CP_XML_ATTR_OPT(L"lineWeight", attr_.line_width_);
			CP_XML_ATTR_OPT(L"markers", attr_.markers_);
			CP_XML_ATTR_OPT(L"first", attr_.first_);
			CP_XML_ATTR_OPT(L"last", attr_.last_);
			CP_XML_ATTR_OPT(L"high", attr_.high_);
			CP_XML_ATTR_OPT(L"low", attr_.low_);
			CP_XML_ATTR_OPT(L"displayXAxis", attr_.display_x_axis_);
			CP_XML_ATTR_OPT(L"displayHidden", attr_.display_hidden_);
			CP_XML_ATTR_OPT(L"dateAxis", attr_.date_axis_);
			CP_XML_ATTR_OPT(L"negative", attr_.negative_);
			CP_XML_ATTR_OPT(L"displayEmptyCellsAs", attr_.display_empty_cells_as_);
			CP_XML_ATTR_OPT(L"minAxisType", attr_.min_axis_type_);
			CP_XML_ATTR_OPT(L"maxAxisType", attr_.max_axis_type_);
			CP_XML_ATTR_OPT(L"rightToLeft", attr_.right_to_left_);
			CP_XML_ATTR_OPT(L"manualMax", attr_.manual_max_);
			CP_XML_ATTR_OPT(L"manualMin", attr_.manual_min_);


			if (attr_.color_series_)
			{
				CP_XML_NODE(L"x14:colorSeries")
				{
					CP_XML_ATTR(L"rgb", attr_.color_series_->get_hex_value(true));
				}
			}
			if (attr_.color_negative_)
			{
				CP_XML_NODE(L"x14:colorNegative")
				{
					CP_XML_ATTR(L"rgb", attr_.color_negative_->get_hex_value(true));
				}
			}
			if (attr_.color_axis_)
			{
				CP_XML_NODE(L"x14:colorAxis")
				{
					CP_XML_ATTR(L"rgb", attr_.color_axis_->get_hex_value(true));
				}
			}
			if (attr_.color_markers_)
			{
				CP_XML_NODE(L"x14:colorMarkers")
				{
					CP_XML_ATTR(L"rgb", attr_.color_markers_->get_hex_value(true));
				}
			}
			if (attr_.color_first_)
			{
				CP_XML_NODE(L"x14:colorFirst")
				{
					CP_XML_ATTR(L"rgb", attr_.color_first_->get_hex_value(true));
				}
			}
			if (attr_.color_last_)
			{
				CP_XML_NODE(L"x14:colorLast")
				{
					CP_XML_ATTR(L"rgb", attr_.color_last_->get_hex_value(true));
				}
			}
			if (attr_.color_high_)
			{
				CP_XML_NODE(L"x14:colorHigh")
				{
					CP_XML_ATTR(L"rgb", attr_.color_high_->get_hex_value(true));
				}
			}
			if (attr_.color_low_)
			{
				CP_XML_NODE(L"x14:colorLow")
				{
					CP_XML_ATTR(L"rgb", attr_.color_low_->get_hex_value(true));
				}
			}
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				calcext_sparklines* sparklines = dynamic_cast<calcext_sparklines*>(content_[i].get());
				sparklines->serialize(CP_XML_STREAM());
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
const wchar_t* calcext_sparklines::ns = L"calcext";
const wchar_t* calcext_sparklines::name = L"sparklines";

void calcext_sparklines::add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_sparklines::serialize(std::wostream& strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"x14:sparklines")
		{
			for (size_t i = 0; i < content_.size(); i++)
			{
				calcext_sparkline* sparkline = dynamic_cast<calcext_sparkline*>(content_[i].get());
				sparkline->serialize(CP_XML_STREAM());
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
const wchar_t* calcext_sparkline::ns = L"calcext";
const wchar_t* calcext_sparkline::name = L"sparkline";

void calcext_sparkline::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"calcext:data-range", data_range_);
	CP_APPLY_ATTR(L"calcext:cell-address", cell_address_);
}
void calcext_sparkline::serialize(std::wostream& strm)
{
	CP_XML_WRITER(strm)
	{		
		CP_XML_NODE(L"x14:sparkline")
		{
			if (data_range_)
			{
				formulasconvert::odf2oox_converter converter;
				std::wstring f = converter.convert_named_ref(*data_range_);

				CP_XML_NODE(L"xm:f")
				{
					CP_XML_STREAM() << XmlUtils::EncodeXmlString(f);
				}
			}
			if (cell_address_)
			{
				formulasconvert::odf2oox_converter converter;
				std::wstring ref = converter.convert_named_ref(*cell_address_, false);

				CP_XML_NODE(L"xm:sqref")
				{
					CP_XML_STREAM() << XmlUtils::EncodeXmlString(ref);
				}
			}
		}
	}
}
}
}
