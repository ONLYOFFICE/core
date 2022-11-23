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

#include "style_chart_properties.h"

#include <iostream>

//#include "chart_objects.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



// style:chart-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_chart_properties::ns = L"style";
const wchar_t * style_chart_properties::name = L"chart-properties";
void style_chart_properties::add_attributes(const xml::attributes_wc_ptr & Attributes)
{
	content_.add_attributes(Attributes);
}
void chart_format_properties::apply_from(const chart_format_properties & Other)
{
	common_rotation_angle_attlist_.apply_from(Other.common_rotation_angle_attlist_);
	
	if (false == Other.empty())
	{
		std::copy(Other.begin(), Other.end(), begin());
	}
}
void chart_format_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	_CP_OPT(bool) bVal;//потом можно и это разделение убрать - заменить вариант

	CP_APPLY_ATTR(L"chart:scale-text",			bVal);		if (bVal) push_back(_property(L"scale-text",			bVal.get()));
    CP_APPLY_ATTR(L"chart:three-dimensional",	bVal);		if (bVal) push_back(_property(L"three-dimensional",		bVal.get()));
    CP_APPLY_ATTR(L"chart:vertical",			bVal);		if (bVal) push_back(_property(L"vertical",				bVal.get()));
    CP_APPLY_ATTR(L"chart:stacked",				bVal);		if (bVal) push_back(_property(L"stacked",				bVal.get()));
    CP_APPLY_ATTR(L"chart:visible",				bVal);		if (bVal) push_back(_property(L"visible",				bVal.get()));
    CP_APPLY_ATTR(L"chart:logarithmic",			bVal);		if (bVal) push_back(_property(L"logarithmic",			bVal.get()));
    CP_APPLY_ATTR(L"chart:percentage",			bVal);		if (bVal) push_back(_property(L"percentage",			bVal.get()));
    CP_APPLY_ATTR(L"chart:connect-bars",		bVal);		if (bVal) push_back(_property(L"connect-bars",			bVal.get()));
    CP_APPLY_ATTR(L"chart:deep",				bVal);		if (bVal) push_back(_property(L"deep",					bVal.get()));
    CP_APPLY_ATTR(L"chart:tick-marks-major-inner",bVal);	if (bVal) push_back(_property(L"tick-marks-major-inner",bVal.get()));
    CP_APPLY_ATTR(L"chart:tick-marks-major-outer",bVal);	if (bVal) push_back(_property(L"tick-marks-major-outer",bVal.get()));
    CP_APPLY_ATTR(L"chart:tick-marks-minor-inner",bVal);	if (bVal) push_back(_property(L"tick-marks-minor-inner",bVal.get()));
    CP_APPLY_ATTR(L"chart:tick-marks-minor-outer",bVal);	if (bVal) push_back(_property(L"tick-marks-minor-outer",bVal.get()));
    CP_APPLY_ATTR(L"chart:lines",				bVal);		if (bVal) push_back(_property(L"lines",					bVal.get()));; 
    CP_APPLY_ATTR(L"chart:display-label",		bVal);		if (bVal) push_back(_property(L"display-label",			bVal.get()));
	CP_APPLY_ATTR(L"chart:reverse-direction",	bVal);		if (bVal) push_back(_property(L"reverse-direction",		bVal.get()));
    CP_APPLY_ATTR(L"text:line-break",			bVal);		if (bVal) push_back(_property(L"line-break",			bVal.get()));
    CP_APPLY_ATTR(L"chart:text-overlap",		bVal);		if (bVal) push_back(_property(L"text-overlap",			bVal.get()));
    CP_APPLY_ATTR(L"chart:link-data-style-to-source",bVal);	if (bVal) push_back(_property(L"link-data-style-to-source",bVal.get()));
    CP_APPLY_ATTR(L"chart:data-label-symbol",	bVal);		if (bVal) push_back(_property(L"data-label-symbol",		bVal.get()));
    CP_APPLY_ATTR(L"chart:data-label-text",		bVal);		if (bVal) push_back(_property(L"data-label-text",		bVal.get()));
    CP_APPLY_ATTR(L"chart:mean-value",			bVal);		if (bVal) push_back(_property(L"mean-value",			bVal.get()));
    CP_APPLY_ATTR(L"chart:error-upper-indicator",bVal);		if (bVal) push_back(_property(L"error-upper-indicator",	bVal.get()));
    CP_APPLY_ATTR(L"chart:error-lower-indicator",bVal);		if (bVal) push_back(_property(L"error-lower-indicator",	bVal.get()));
	
	_CP_OPT(std::wstring) strVal;
	
	CP_APPLY_ATTR(L"chartooo:axis-type", strVal); if (strVal) push_back(_property(L"axis-type", strVal.get()));

	CP_APPLY_ATTR(L"chart:symbol-type",		strVal);	
	if (strVal) push_back(_property(L"symbol-type",chart_symbol_type(chart_symbol_type::parse(strVal.get())).get_type()));

	CP_APPLY_ATTR(L"chart:symbol-name",		strVal);	
	if (strVal) push_back(_property(L"symbol-name",chart_symbol_name(chart_symbol_name::parse(strVal.get())).get_type()));

	_CP_OPT(int) iVal;

 	//CP_APPLY_ATTR(L"chart:symbol-type",	iVal);			if (iVal)push_back(_property(L"symbol-type",			iVal.get()));
	CP_APPLY_ATTR(L"chart:gap-width",		iVal);			if (iVal)push_back(_property(L"gap-width",				iVal.get()));
    CP_APPLY_ATTR(L"chart:overlap",			iVal);			if (iVal)push_back(_property(L"overlap",				iVal.get()));
    CP_APPLY_ATTR(L"chart:spline-order",	iVal);			if (iVal)push_back(_property(L"spline-order",			iVal.get()));
    CP_APPLY_ATTR(L"chart:spline-resolution",iVal);			if (iVal)push_back(_property(L"spline-resolution",		iVal.get()));
    CP_APPLY_ATTR(L"chart:pie-offset",		iVal);			if (iVal)push_back(_property(L"pie-offset",			iVal.get()));
    CP_APPLY_ATTR(L"chart:interval-minor-divisor",iVal);	if (iVal)push_back(_property(L"interval-minor-divisor",iVal.get()));

	_CP_OPT(double) dVal;

    CP_APPLY_ATTR(L"chart:maximum",			dVal);	if (dVal) push_back(_property(L"maximum",			dVal.get()));
    CP_APPLY_ATTR(L"chart:minimum",			dVal);	if (dVal) push_back(_property(L"minimum",			dVal.get()));
    CP_APPLY_ATTR(L"chart:origin",			dVal);	if (dVal) push_back(_property(L"origin",			dVal.get()));
    CP_APPLY_ATTR(L"chart:interval-major",	dVal);	if (dVal) push_back(_property(L"interval-major",	dVal.get()));
    CP_APPLY_ATTR(L"chart:error-percentage",dVal);	if (dVal) push_back(_property(L"error-percentage",	dVal.get()));
    CP_APPLY_ATTR(L"chart:error-margin",	dVal);	if (dVal) push_back(_property(L"error-margin",		dVal.get()));
    CP_APPLY_ATTR(L"chart:error-lower-limit",dVal); if (dVal) push_back(_property(L"error-lower-limit",	dVal.get()));
    CP_APPLY_ATTR(L"chart:error-upper-limit",dVal); if (dVal) push_back(_property(L"error-upper-limit",	dVal.get()));
	
    CP_APPLY_ATTR(L"chart:axis-position",	strVal);	
	if (strVal) push_back(_property(L"axis-position",	*strVal));
	
	CP_APPLY_ATTR(L"chart:tick-mark-position",	strVal);	
 	if (strVal) push_back(_property(L"tick-mark-position",	*strVal));
 
    CP_APPLY_ATTR(L"chart:interpolation",	strVal);	
	if (strVal) push_back(_property(L"interpolation",	chart_interpolation(chart_interpolation::parse(strVal.get())).get_type()));
  
	CP_APPLY_ATTR(L"chart:solid-type",		strVal);	
	if (strVal) push_back(_property(L"solid-type",	chart_solid_type(chart_solid_type::parse(strVal.get())).get_type()));
    
	CP_APPLY_ATTR(L"chart:label-arrangement",		strVal); 
	if (strVal) push_back(_property(L"label-arrangement",chart_label_arrangement(chart_label_arrangement::parse(strVal.get())).get_type()));

	CP_APPLY_ATTR(L"style:direction",		strVal); 
	if (strVal) push_back(_property(L"direction",	direction(direction::parse(strVal.get())).get_type() ));

	CP_APPLY_ATTR(L"chart:series-source",		strVal); 
	if (strVal) push_back(_property(L"series-source",	chart_series_source(chart_series_source::parse(strVal.get())).get_type() ));

	CP_APPLY_ATTR(L"chart:symbol-width",		strVal); 
	if (strVal)
	{
		push_back(_property(L"symbol-width-value",	length(length::parse(strVal.get())).get_value() ));
		push_back(_property(L"symbol-width-unit",	length(length::parse(strVal.get())).get_unit() ));
	}

	CP_APPLY_ATTR(L"chart:symbol-height",		strVal); 
	if (strVal)
	{
		push_back(_property(L"symbol-height-value",	length(length::parse(strVal.get())).get_value() ));
		push_back(_property(L"symbol-height-unit",		length(length::parse(strVal.get())).get_unit() ));
	}

	CP_APPLY_ATTR(L"chart:regression-type",		strVal); 
	if (strVal) push_back(_property(L"regression-type",	chart_regression_type(chart_regression_type::parse(strVal.get())).get_type() ));
	
	CP_APPLY_ATTR(L"chart:data-label-number",		strVal); 
	if (strVal) push_back(_property(L"data-label-number",	chart_data_label_number(chart_data_label_number::parse(strVal.get())).get_type() ));

	CP_APPLY_ATTR(L"chart:error-category",		strVal); 
	if (strVal) push_back(_property(L"error-category",	chart_error_category(chart_error_category::parse(strVal.get())).get_type() ));

	CP_APPLY_ATTR(L"chart:label-position",		strVal); 
	if (strVal) push_back(_property(L"label-position",	chart_label_position(chart_label_position::parse(strVal.get())).get_type() ));

	common_rotation_angle_attlist_.add_attributes(Attributes);

	//CP_APPLY_ATTR(L"chart:scale-text",			chart_scale_text_ );	
 //   CP_APPLY_ATTR(L"chart:three-dimensional",	chart_three_dimensional_ );
	//CP_APPLY_ATTR(L"chart:vertical",			chart_vertical_ );
	//CP_APPLY_ATTR(L"chart:stacked",				chart_stacked_ );
	//CP_APPLY_ATTR(L"chart:visible",				chart_visible_ );
	//CP_APPLY_ATTR(L"chart:logarithmic",			chart_logarithmic_ );	
	//CP_APPLY_ATTR(L"chart:percentage",			chart_percentage_ );		
	//CP_APPLY_ATTR(L"chart:connect-bars",		chart_connect_bars_ );		
	//CP_APPLY_ATTR(L"chart:deep",				chart_deep_ );		
	//CP_APPLY_ATTR(L"chart:tick-marks-major-outer",chart_tick_marks_major_outer_ );	
	//CP_APPLY_ATTR(L"chart:tick-marks-minor-inner",chart_tick_marks_minor_inner_ );	
	//CP_APPLY_ATTR(L"chart:tick-marks-minor-outer",chart_marks_minor_outer_ );	
	//CP_APPLY_ATTR(L"chart:lines",				chart_lines_ );		
	//CP_APPLY_ATTR(L"chart:display-label",		chart_display_label_ );		
	//CP_APPLY_ATTR(L"chart:reverse-direction",	chart_reverse_direction_ );		
	//CP_APPLY_ATTR(L"text:line-break",			chart_line_break_ );		
	//CP_APPLY_ATTR(L"chart:text-overlap",		chart_text_overlap_ );		
	//CP_APPLY_ATTR(L"chart:link-data-style-to-source",chart_link-data-style-to-source_ );
	//CP_APPLY_ATTR(L"chart:data-label-symbol",	chart_data-label-symbol_ );	
	//CP_APPLY_ATTR(L"chart:data-label-text",		chart_data-label-text_ );	
	//CP_APPLY_ATTR(L"chart:mean-value",			chart_mean-value_ );	
	//CP_APPLY_ATTR(L"chart:error-upper-indicator",chart_error_upper_indicator_ );	
	//CP_APPLY_ATTR(L"chart:error-lower-indicator",chart_error_lower_indicator_ );

	//CP_APPLY_ATTR(L"chart:symbol-type",			chart_symbol_type_);	
	//CP_APPLY_ATTR(L"chart:symbol-name",			chart_symbol_name_);	
	//
	//CP_APPLY_ATTR(L"chart:gap-width",			chart_gap_width_);
	//CP_APPLY_ATTR(L"chart:overlap",				chart_overlap_);
	//CP_APPLY_ATTR(L"chart:spline-order",		chart_spline_order_);
	//CP_APPLY_ATTR(L"chart:spline-resolution",	chart_spline_resolution_); 
	//CP_APPLY_ATTR(L"chart:pie-offset",			chart_pie_offset_);	
	//CP_APPLY_ATTR(L"chart:interval-minor-divisor",chart_interval_minor_divisor_); 
	//
	//CP_APPLY_ATTR(L"chart:maximum",				chart_maximum_);
	//CP_APPLY_ATTR(L"chart:minimum",				chart_minimum_); 
	//CP_APPLY_ATTR(L"chart:origin",				chart_origin_); 
	//CP_APPLY_ATTR(L"chart:interval-major",		chart_interval_major_); 
	//CP_APPLY_ATTR(L"chart:error-percentage",	chart_error_percentage_); 
	//CP_APPLY_ATTR(L"chart:error-margin",		chart_error_margin_); 
	//CP_APPLY_ATTR(L"chart:error-upper-limit",	chart_error_upper_limit_);   

	//CP_APPLY_ATTR(L"chart:symbol-width",		chart_symbol_width_);
	//CP_APPLY_ATTR(L"chart:symbol-height",		chart_symbol_height_); 

 //   CP_APPLY_ATTR(L"chart:interpolation",		chart_interpolation_);  
	//CP_APPLY_ATTR(L"chart:solid-type",			chart_solid_type_);	    
	//CP_APPLY_ATTR(L"chart:label-arrangement",	chart_label_arrangement_); 
	//CP_APPLY_ATTR(L"style:direction",			style_direction_); 
	//CP_APPLY_ATTR(L"chart:series-source",		chart_series_source_); 
	//CP_APPLY_ATTR(L"chart:regression-type",		chart_regression_type_); 	
	//CP_APPLY_ATTR(L"chart:data-label-number",	chart_data_label_number_); 
	//CP_APPLY_ATTR(L"chart:error-category",		chart_error_category_); 

	//common_rotation_angle_attlist_.add_attributes(Attributes);
}

void style_chart_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

}
}
