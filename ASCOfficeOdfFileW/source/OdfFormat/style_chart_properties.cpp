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

#include "style_chart_properties.h"

#include <iostream>

//#include "chart_objects.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;

// style:chart-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_chart_properties::ns = L"style";
const wchar_t * style_chart_properties::name = L"chart-properties";

void chart_format_properties::serialize(std::wostream & _Wostream ,const wchar_t * ns, const wchar_t * name )
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"chart:scale-text",			chart_scale_text_ );	
			CP_XML_ATTR_OPT(L"chart:three-dimensional",		chart_three_dimensional_ );
			CP_XML_ATTR_OPT(L"chart:vertical",				chart_vertical_ );
			CP_XML_ATTR_OPT(L"chart:stacked",				chart_stacked_ );
			CP_XML_ATTR_OPT(L"chart:visible",				chart_visible_ );
			CP_XML_ATTR_OPT(L"chart:logarithmic",			chart_logarithmic_ );	
			CP_XML_ATTR_OPT(L"chart:percentage",			chart_percentage_ );		
			CP_XML_ATTR_OPT(L"chart:connect-bars",			chart_connect_bars_ );		
			CP_XML_ATTR_OPT(L"chart:deep",					chart_deep_ );		
			CP_XML_ATTR_OPT(L"chart:tick-marks-major-inner",chart_tick_marks_major_inner_ );	
			CP_XML_ATTR_OPT(L"chart:tick-marks-major-outer",chart_tick_marks_major_outer_ );	
			CP_XML_ATTR_OPT(L"chart:tick-marks-minor-inner",chart_tick_marks_minor_inner_ );	
			CP_XML_ATTR_OPT(L"chart:tick-marks-minor-outer",chart_tick_marks_minor_outer_ );	
			CP_XML_ATTR_OPT(L"chart:lines",					chart_lines_ );		
			CP_XML_ATTR_OPT(L"chart:display-label",			chart_display_label_ );		
			CP_XML_ATTR_OPT(L"chart:reverse-direction",		chart_reverse_direction_ );		
			CP_XML_ATTR_OPT(L"text:line-break",				text_line_break_ );		
			CP_XML_ATTR_OPT(L"chart:text-overlap",			chart_text_overlap_ );		
			CP_XML_ATTR_OPT(L"chart:link-data-style-to-source",chart_link_data_style_to_source_ );
			CP_XML_ATTR_OPT(L"chart:data-label-symbol",		chart_data_label_symbol_ );	
			CP_XML_ATTR_OPT(L"chart:data-label-text",		chart_data_label_text_ );	
			CP_XML_ATTR_OPT(L"chart:mean-value",			chart_mean_value_ );	
			CP_XML_ATTR_OPT(L"chart:error-upper-indicator",	chart_error_upper_indicator_ );	
			CP_XML_ATTR_OPT(L"chart:error-lower-indicator",	chart_error_lower_indicator_ );
			CP_XML_ATTR_OPT(L"chart:treat-empty-cells",		chart_treat_empty_cells_);
			CP_XML_ATTR_OPT(L"chart:japanese-candle-stick",	chart_japanese_candle_stick_);

			CP_XML_ATTR_OPT(L"chart:symbol-type",			chart_symbol_type_);	
			CP_XML_ATTR_OPT(L"chart:symbol-name",			chart_symbol_name_);	
			
			CP_XML_ATTR_OPT(L"chart:gap-width",				chart_gap_width_);
			CP_XML_ATTR_OPT(L"chart:overlap",				chart_overlap_);
			CP_XML_ATTR_OPT(L"chart:spline-order",			chart_spline_order_);
			CP_XML_ATTR_OPT(L"chart:spline-resolution",		chart_spline_resolution_); 
			CP_XML_ATTR_OPT(L"chart:pie-offset",			chart_pie_offset_);	
			CP_XML_ATTR_OPT(L"chart:interval-minor-divisor",chart_interval_minor_divisor_); 
			
			CP_XML_ATTR_OPT(L"chart:maximum",				chart_maximum_);
			CP_XML_ATTR_OPT(L"chart:minimum",				chart_minimum_); 
			CP_XML_ATTR_OPT(L"chart:origin",				chart_origin_); 
			CP_XML_ATTR_OPT(L"chart:interval-major",		chart_interval_major_); 
			CP_XML_ATTR_OPT(L"chart:error-percentage",		chart_error_percentage_); 
			CP_XML_ATTR_OPT(L"chart:error-margin",			chart_error_margin_); 
			CP_XML_ATTR_OPT(L"chart:error-upper-limit",		chart_error_upper_limit_);   

			CP_XML_ATTR_OPT(L"chart:symbol-width",			chart_symbol_width_);
			CP_XML_ATTR_OPT(L"chart:symbol-height",			chart_symbol_height_); 

			CP_XML_ATTR_OPT(L"chart:interpolation",			chart_interpolation_);  
			CP_XML_ATTR_OPT(L"chart:solid-type",			chart_solid_type_);	    
			CP_XML_ATTR_OPT(L"chart:label-arrangement",		chart_label_arrangement_); 
			CP_XML_ATTR_OPT(L"style:direction",				style_direction_); 
			CP_XML_ATTR_OPT(L"chart:series-source",			chart_series_source_); 
			CP_XML_ATTR_OPT(L"chart:regression-type",		chart_regression_type_); 	
			CP_XML_ATTR_OPT(L"chart:data-label-number",		chart_data_label_number_); 
			CP_XML_ATTR_OPT(L"chart:error-category",		chart_error_category_); 
			CP_XML_ATTR_OPT(L"chart:axis-label-position",	chart_axis_label_position_);
			CP_XML_ATTR_OPT(L"chart:axis-position",			chart_axis_position_);

			common_rotation_angle_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
void style_chart_properties::serialize(std::wostream & _Wostream)
{
	content_.serialize(_Wostream,ns,name);
}

}
}
