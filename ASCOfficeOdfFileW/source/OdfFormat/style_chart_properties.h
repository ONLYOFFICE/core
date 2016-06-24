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
#pragma once

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "chartsymbol.h"
#include "length.h"
#include "chartinterpolation.h"
#include "chartsolidtype.h"
#include "chartlabelarrangement.h"
#include "common_attlists.h"
#include "chartdatalabelnumber.h"
#include "charterrorcategory.h"
#include "chartseriessource.h"
#include "chartregressiontype.h"
#include "direction.h"

namespace cpdoccore { 
namespace odf_writer {


class chart_format_properties
{
public:

	void serialize(std::wostream & strm, const wchar_t * ns, const wchar_t * name );

	_CP_OPT(odf_types::Bool) chart_scale_text_;	
    _CP_OPT(odf_types::Bool) chart_three_dimensional_;
	_CP_OPT(odf_types::Bool) chart_vertical_;
	_CP_OPT(odf_types::Bool) chart_stacked_;
	_CP_OPT(odf_types::Bool) chart_visible_;
	_CP_OPT(odf_types::Bool) chart_logarithmic_;	
	_CP_OPT(odf_types::Bool) chart_percentage_;		
	_CP_OPT(odf_types::Bool) chart_connect_bars_;		
	_CP_OPT(odf_types::Bool) chart_deep_;		
	_CP_OPT(odf_types::Bool) chart_tick_marks_major_outer_;	
	_CP_OPT(odf_types::Bool) chart_tick_marks_major_inner_;	
	_CP_OPT(odf_types::Bool) chart_tick_marks_minor_inner_;	
	_CP_OPT(odf_types::Bool) chart_tick_marks_minor_outer_;	
	_CP_OPT(odf_types::Bool) chart_lines_;		
	_CP_OPT(odf_types::Bool) chart_display_label_;		
	_CP_OPT(odf_types::Bool) chart_reverse_direction_;		
	_CP_OPT(odf_types::Bool) text_line_break_;		
	_CP_OPT(odf_types::Bool) chart_text_overlap_;		
	_CP_OPT(odf_types::Bool) chart_link_data_style_to_source_;
	_CP_OPT(odf_types::Bool) chart_data_label_symbol_;	
	_CP_OPT(odf_types::Bool) chart_data_label_text_;	
	_CP_OPT(odf_types::Bool) chart_mean_value_;	
	_CP_OPT(odf_types::Bool) chart_error_upper_indicator_;	
	_CP_OPT(odf_types::Bool) chart_error_lower_indicator_;
	_CP_OPT(odf_types::Bool) chart_treat_empty_cells_;
	_CP_OPT(odf_types::Bool) chart_japanese_candle_stick_;
	
	_CP_OPT(odf_types::chart_symbol_type) chart_symbol_type_;
	_CP_OPT(odf_types::chart_symbol_name) chart_symbol_name_;
	
	_CP_OPT(int) chart_gap_width_;
	_CP_OPT(int) chart_overlap_;
	_CP_OPT(int) chart_spline_order_;
	_CP_OPT(int) chart_spline_resolution_;
	_CP_OPT(int) chart_pie_offset_;	
	_CP_OPT(int) chart_interval_minor_divisor_;

    _CP_OPT(double) chart_maximum_;
	_CP_OPT(double) chart_minimum_; 
	_CP_OPT(double) chart_origin_; 
	_CP_OPT(double) chart_interval_major_;
	_CP_OPT(double) chart_error_percentage_;
	_CP_OPT(double) chart_error_margin_;
	_CP_OPT(double) chart_error_upper_limit_;

	_CP_OPT(odf_types::chart_interpolation)		chart_interpolation_;
	_CP_OPT(odf_types::chart_solid_type)			chart_solid_type_;
	_CP_OPT(odf_types::chart_label_arrangement)	chart_label_arrangement_;
	_CP_OPT(odf_types::direction)					style_direction_;
	_CP_OPT(odf_types::chart_series_source)		chart_series_source_;
	_CP_OPT(odf_types::length)						chart_symbol_width_;
	_CP_OPT(odf_types::length)						chart_symbol_height_;
	_CP_OPT(odf_types::chart_regression_type)		chart_regression_type_;
	_CP_OPT(odf_types::chart_data_label_number)	chart_data_label_number_;
	_CP_OPT(odf_types::chart_error_category)		chart_error_category_;

	_CP_OPT(std::wstring)				chart_axis_label_position_;
	_CP_OPT(std::wstring)				chart_axis_position_;									

	odf_types::common_rotation_angle_attlist       common_rotation_angle_attlist_;
};
/// \class  style_chart_properties
///         style:chart-properties
class style_chart_properties : public office_element_impl<style_chart_properties>//стили чарта не наследуются
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleChartProperties;

    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
	chart_format_properties & content() { return chart_format_properties_; }

private:	
	chart_format_properties chart_format_properties_;


};

CP_REGISTER_OFFICE_ELEMENT2(style_chart_properties);

}
}
