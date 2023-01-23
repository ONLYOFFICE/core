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
#pragma once

#include <xml/attributes.h>
#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements_create.h"

#include "../../DataTypes/chartsymbol.h"
#include "../../DataTypes/length.h"
#include "../../DataTypes/chartinterpolation.h"
#include "../../DataTypes/chartsolidtype.h"
#include "../../DataTypes/chartlabelarrangement.h"
#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/chartdatalabelnumber.h"
#include "../../DataTypes/charterrorcategory.h"
#include "../../DataTypes/chartseriessource.h"
#include "../../DataTypes/chartregressiontype.h"
#include "../../DataTypes/direction.h"

namespace cpdoccore { 
namespace odf_writer {


class chart_format_properties
{
public:

	void serialize(std::wostream & strm, const wchar_t * ns, const wchar_t * name );

	_CP_OPT(odf_types::Bool) scale_text_;	
    _CP_OPT(odf_types::Bool) three_dimensional_;
	_CP_OPT(odf_types::Bool) vertical_;
	_CP_OPT(odf_types::Bool) stacked_;
	_CP_OPT(odf_types::Bool) visible_;
	_CP_OPT(odf_types::Bool) logarithmic_;	
	_CP_OPT(odf_types::Bool) percentage_;		
	_CP_OPT(odf_types::Bool) connect_bars_;		
	_CP_OPT(odf_types::Bool) deep_;		
	_CP_OPT(odf_types::Bool) tick_marks_major_outer_;	
	_CP_OPT(odf_types::Bool) tick_marks_major_inner_;	
	_CP_OPT(odf_types::Bool) tick_marks_minor_inner_;	
	_CP_OPT(odf_types::Bool) tick_marks_minor_outer_;	
	_CP_OPT(odf_types::Bool) lines_;		
	_CP_OPT(odf_types::Bool) display_label_;		
	_CP_OPT(odf_types::Bool) reverse_direction_;		
	_CP_OPT(odf_types::Bool) text_line_break_;		
	_CP_OPT(odf_types::Bool) text_overlap_;		
	_CP_OPT(odf_types::Bool) link_data_style_to_source_;
	_CP_OPT(odf_types::Bool) data_label_symbol_;	
	_CP_OPT(odf_types::Bool) data_label_text_;	
	_CP_OPT(odf_types::Bool) mean_value_;	
	_CP_OPT(odf_types::Bool) error_upper_indicator_;	
	_CP_OPT(odf_types::Bool) error_lower_indicator_;
	_CP_OPT(odf_types::Bool) treat_empty_cells_;
	_CP_OPT(odf_types::Bool) japanese_candle_stick_;
	
	_CP_OPT(odf_types::chart_symbol_type) symbol_type_;
	_CP_OPT(odf_types::chart_symbol_name) symbol_name_;
	
	_CP_OPT(int) gap_width_;
	_CP_OPT(int) overlap_;
	_CP_OPT(int) spline_order_;
	_CP_OPT(int) spline_resolution_;
	_CP_OPT(int) pie_offset_;	
	_CP_OPT(bool) pie_bubble_;
	_CP_OPT(int) interval_minor_divisor_;

    _CP_OPT(double) maximum_;
	_CP_OPT(double) minimum_; 
	_CP_OPT(double) origin_; 
	_CP_OPT(double) interval_major_;
	_CP_OPT(double) error_percentage_;
	_CP_OPT(double) error_margin_;
	_CP_OPT(double) error_upper_limit_;

	_CP_OPT(odf_types::chart_interpolation)			interpolation_;
	_CP_OPT(odf_types::chart_solid_type)			solid_type_;
	_CP_OPT(odf_types::chart_label_arrangement)		label_arrangement_;
	_CP_OPT(odf_types::direction)					style_direction_;
	_CP_OPT(odf_types::chart_series_source)			series_source_;
	_CP_OPT(odf_types::length)						symbol_width_;
	_CP_OPT(odf_types::length)						symbol_height_;
	_CP_OPT(odf_types::chart_regression_type)		regression_type_;
	_CP_OPT(odf_types::chart_data_label_number)		data_label_number_;
	_CP_OPT(odf_types::chart_error_category)		error_category_;
	_CP_OPT(odf_types::Bool)						right_angled_axes_;

	_CP_OPT(std::wstring)				axis_label_position_;
	_CP_OPT(std::wstring)				axis_position_;									

	odf_types::common_rotation_angle_attlist       common_rotation_angle_attlist_;
};

class style_chart_properties : public office_element_impl<style_chart_properties>//стили чарта не наследуются
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleChartProperties;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
	
	chart_format_properties content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_chart_properties);

}
}
