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
			CP_XML_ATTR_OPT(L"chart:scale-text",			scale_text_ );	
			CP_XML_ATTR_OPT(L"chart:three-dimensional",		three_dimensional_ );
			CP_XML_ATTR_OPT(L"chart:vertical",				vertical_ );
			CP_XML_ATTR_OPT(L"chart:stacked",				stacked_ );
			CP_XML_ATTR_OPT(L"chart:visible",				visible_ );
			CP_XML_ATTR_OPT(L"chart:logarithmic",			logarithmic_ );	
			CP_XML_ATTR_OPT(L"chart:percentage",			percentage_ );		
			CP_XML_ATTR_OPT(L"chart:connect-bars",			connect_bars_ );		
			CP_XML_ATTR_OPT(L"chart:deep",					deep_ );		
			CP_XML_ATTR_OPT(L"chart:tick-marks-major-inner",tick_marks_major_inner_ );	
			CP_XML_ATTR_OPT(L"chart:tick-marks-major-outer",tick_marks_major_outer_ );	
			CP_XML_ATTR_OPT(L"chart:tick-marks-minor-inner",tick_marks_minor_inner_ );	
			CP_XML_ATTR_OPT(L"chart:tick-marks-minor-outer",tick_marks_minor_outer_ );	
			CP_XML_ATTR_OPT(L"chart:lines",					lines_ );		
			CP_XML_ATTR_OPT(L"chart:display-label",			display_label_ );		
			CP_XML_ATTR_OPT(L"chart:reverse-direction",		reverse_direction_ );		
			CP_XML_ATTR_OPT(L"text:line-break",				text_line_break_ );		
			CP_XML_ATTR_OPT(L"chart:text-overlap",			text_overlap_ );		
			CP_XML_ATTR_OPT(L"chart:link-data-style-to-source",link_data_style_to_source_ );
			CP_XML_ATTR_OPT(L"chart:data-label-symbol",		data_label_symbol_ );	
			CP_XML_ATTR_OPT(L"chart:data-label-text",		data_label_text_ );	
			CP_XML_ATTR_OPT(L"chart:mean-value",			mean_value_ );	
			CP_XML_ATTR_OPT(L"chart:error-upper-indicator",	error_upper_indicator_ );	
			CP_XML_ATTR_OPT(L"chart:error-lower-indicator",	error_lower_indicator_ );
			CP_XML_ATTR_OPT(L"chart:treat-empty-cells",		treat_empty_cells_);
			CP_XML_ATTR_OPT(L"chart:japanese-candle-stick",	japanese_candle_stick_);

			CP_XML_ATTR_OPT(L"chart:symbol-type",			symbol_type_);	
			CP_XML_ATTR_OPT(L"chart:symbol-name",			symbol_name_);	
			
			CP_XML_ATTR_OPT(L"chart:gap-width",				gap_width_);
			CP_XML_ATTR_OPT(L"chart:overlap",				overlap_);
			CP_XML_ATTR_OPT(L"chart:spline-order",			spline_order_);
			CP_XML_ATTR_OPT(L"chart:spline-resolution",		spline_resolution_); 
			CP_XML_ATTR_OPT(L"chart:pie-offset",			pie_offset_);	
			CP_XML_ATTR_OPT(L"chart:interval-minor-divisor",interval_minor_divisor_); 
			
			CP_XML_ATTR_OPT(L"chart:maximum",				maximum_);
			CP_XML_ATTR_OPT(L"chart:minimum",				minimum_); 
			CP_XML_ATTR_OPT(L"chart:origin",				origin_); 
			CP_XML_ATTR_OPT(L"chart:interval-major",		interval_major_); 
			CP_XML_ATTR_OPT(L"chart:error-percentage",		error_percentage_); 
			CP_XML_ATTR_OPT(L"chart:error-margin",			error_margin_); 
			CP_XML_ATTR_OPT(L"chart:error-upper-limit",		error_upper_limit_);   

			CP_XML_ATTR_OPT(L"chart:symbol-width",			symbol_width_);
			CP_XML_ATTR_OPT(L"chart:symbol-height",			symbol_height_); 

			CP_XML_ATTR_OPT(L"chart:interpolation",			interpolation_);  
			CP_XML_ATTR_OPT(L"chart:solid-type",			solid_type_);	    
			CP_XML_ATTR_OPT(L"chart:label-arrangement",		label_arrangement_); 
			CP_XML_ATTR_OPT(L"style:direction",				style_direction_); 
			CP_XML_ATTR_OPT(L"chart:series-source",			series_source_); 
			CP_XML_ATTR_OPT(L"chart:regression-type",		regression_type_); 	
			CP_XML_ATTR_OPT(L"chart:data-label-number",		data_label_number_); 
			CP_XML_ATTR_OPT(L"chart:error-category",		error_category_); 
			CP_XML_ATTR_OPT(L"chart:axis-label-position",	axis_label_position_);
			CP_XML_ATTR_OPT(L"chart:axis-position",			axis_position_);

			CP_XML_ATTR_OPT(L"chart:right-angled-axes",		right_angled_axes_);

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
