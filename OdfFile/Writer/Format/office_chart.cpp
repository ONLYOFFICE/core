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


#include "office_chart.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

// office:chart
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_chart::ns = L"office";
const wchar_t * office_chart::name = L"chart";

void office_chart::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void office_chart::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void office_chart::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void chart_chart_attlist::serialize(CP_ATTR_NODE)
{
	common_draw_size_attlist_.serialize(CP_GET_XML_NODE());
    common_attlist_.serialize(CP_GET_XML_NODE());
    
    CP_XML_ATTR(L"chart:class", chart_class_);
	CP_XML_ATTR_OPT(L"chart:column-mapping", chart_column_mapping_);
    CP_XML_ATTR_OPT(L"chart:row-mapping", chart_row_mapping_);
}

// chart:chart
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_chart::ns = L"chart";
const wchar_t * chart_chart::name = L"chart";


void chart_chart::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void chart_chart::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_chart_attlist_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void chart_chart::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void chart_title_attlist::serialize(CP_ATTR_NODE)
{
    common_draw_position_attlist_.serialize(CP_GET_XML_NODE());
    common_attlist_.serialize(CP_GET_XML_NODE());
   
	CP_XML_ATTR_OPT(L"table:cell-range", table_cell_range_);
}

// chart:title
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_title::ns = L"chart";
const wchar_t * chart_title::name = L"title";


void chart_title::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"p")
    {
        CP_CREATE_ELEMENT(text_p_);
    }    
    else
        CP_NOT_APPLICABLE_ELM();

}
void chart_title::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_title_attlist_.serialize(CP_GET_XML_NODE());
			if (text_p_)text_p_->serialize(CP_XML_STREAM());
		}
	}
}
void chart_title::add_child_element( const office_element_ptr & child_element)
{
	text_p_ = child_element;
}
// chart:subtitle
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_subtitle::ns = L"chart";
const wchar_t * chart_subtitle::name = L"subtitle";

void chart_subtitle::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"p")
    {
        CP_CREATE_ELEMENT(text_p_);
    }    
    else
        CP_NOT_APPLICABLE_ELM();
}
void chart_subtitle::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_title_attlist_.serialize(CP_GET_XML_NODE());
			if (text_p_)text_p_->serialize(CP_XML_STREAM());
		}
	}
}
void chart_subtitle::add_child_element( const office_element_ptr & child_element)
{
	text_p_ = child_element;
}
// chart:footer
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_footer::ns = L"chart";
const wchar_t * chart_footer::name = L"footer";

void chart_footer::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"p")
    {
       CP_CREATE_ELEMENT(text_p_);
    }    
    else
        CP_NOT_APPLICABLE_ELM();
}
void chart_footer::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
			if (text_p_)text_p_->serialize(CP_XML_STREAM());
		}
	}
}
void chart_footer::add_child_element( const office_element_ptr & child_element)
{
	text_p_ = child_element;
}
// chart:legend
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_legend::ns = L"chart";
const wchar_t * chart_legend::name = L"legend";

void chart_legend_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"chart:legend-position",	chart_legend_position_);
    CP_XML_ATTR_OPT(L"chart:legend-align",		chart_legend_align_);

    CP_XML_ATTR_OPT(L"style:legend-expansion",				style_legend_expansion_);
    CP_XML_ATTR_OPT(L"style:legend-expansion-aspect-ratio", style_legend_expansion_aspect_ratio_);

	CP_XML_ATTR_OPT(L"chartooo:height", chartooo_height_);
	CP_XML_ATTR_OPT(L"chartooo:width",	chartooo_width_);
	
	common_draw_position_attlist_.serialize(CP_GET_XML_NODE());
   
	common_attlist_.serialize(CP_GET_XML_NODE());
}
void chart_legend::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_legend_attlist_.serialize(CP_GET_XML_NODE());

		}
	}
}
/////////////////////

void chart_plot_area_attlist::serialize(CP_ATTR_NODE)
{
    common_draw_position_attlist_.serialize(CP_GET_XML_NODE());
    common_draw_size_attlist_.serialize(CP_GET_XML_NODE());
	common_dr3d_attlist_.serialize(CP_GET_XML_NODE());
   
	common_attlist_.serialize(CP_GET_XML_NODE());
    
	CP_XML_ATTR_OPT(L"table:cell-range-address", table_cell_range_address_);
    CP_XML_ATTR_OPT(L"chart:data-source-has-labels", chart_data_source_has_labels_);
}

// chart:plot-area
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_plot_area::ns = L"chart";
const wchar_t * chart_plot_area::name = L"plot-area";

void chart_plot_area::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
 }
void chart_plot_area::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_plot_area_attlist_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void chart_plot_area::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
/////////////

void chart_wall_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"svg:width", svg_width_);
    common_attlist_.serialize(CP_GET_XML_NODE());
}

// chart:wall
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_wall::ns = L"chart";
const wchar_t * chart_wall::name = L"wall";

void chart_wall::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_wall_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

// chart:floor
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_floor::ns = L"chart";
const wchar_t * chart_floor::name = L"floor";

void chart_floor::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

///////////////

void chart_axis_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"chart:dimension", chart_dimension_);
    CP_XML_ATTR_OPT(L"chart:name", chart_name_);
    CP_XML_ATTR_OPT(L"chartooo:axis-type", axis_type_);
    
	common_attlist_.serialize(CP_GET_XML_NODE());
}

// chart:axis
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_axis::ns = L"chart";
const wchar_t * chart_axis::name = L"axis";

void chart_axis::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void chart_axis::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_axis_attlist_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void chart_axis::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;
		
	content_.push_back(child_element);
}
//////

void chart_grid_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"chart:class", chart_class_);
    common_attlist_.serialize(CP_GET_XML_NODE());
}

// chart:grid
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_grid::ns = L"chart";
const wchar_t * chart_grid::name = L"grid";

void chart_grid::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_grid_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

// chart:categories
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_categories::ns = L"chart";
const wchar_t * chart_categories::name = L"categories";

void chart_categories::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"table:cell-range-address", table_cell_range_address_);
		}
	}
}
///////

void chart_series_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"chart:values-cell-range-address", chart_values_cell_range_address_);
    CP_XML_ATTR_OPT(L"chart:label-cell-address", chart_label_cell_address_);
    CP_XML_ATTR_OPT(L"chart:class", chart_class_);
    CP_XML_ATTR_OPT(L"chart:attached-axis", chart_attached_axis_);
    common_attlist_.serialize(CP_GET_XML_NODE());
}

// chart:categories
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_series::ns = L"chart";
const wchar_t * chart_series::name = L"series";

void chart_series::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void chart_series::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_series_attlist_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void chart_series::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// chart:domain
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_domain::ns = L"chart";
const wchar_t * chart_domain::name = L"domain";

void chart_domain::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"table:cell-range-address", table_cell_range_address_);
		}
	}
}
///////////////////////////////////////
void chart_data_point_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"chart:repeated", chart_repeated_);
    common_attlist_.serialize(CP_GET_XML_NODE());
}

// chart:data-point
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_data_point::ns = L"chart";
const wchar_t * chart_data_point::name = L"data-point";

void chart_data_point::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			chart_data_point_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
// chart:mean-value
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_mean_value::ns = L"chart";
const wchar_t * chart_mean_value::name = L"mean-value";

void chart_mean_value::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

// chart:error-indicator
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_error_indicator::ns = L"chart";
const wchar_t * chart_error_indicator::name = L"error-indicator";

void chart_error_indicator::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

// chart:regression-curve
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_regression_curve::ns = L"chart";
const wchar_t * chart_regression_curve::name = L"regression-curve";

void chart_regression_curve::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"chart", L"equation") 
	{
        CP_CREATE_ELEMENT(chart_equation_);
	}
}
void chart_regression_curve::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
			if (chart_equation_)chart_equation_->serialize(CP_XML_STREAM());
		}
	}
}
void chart_regression_curve::add_child_element( const office_element_ptr & child_element)
{
	chart_equation_ = child_element;
}
// chart:equatione
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_equation::ns = L"chart";
const wchar_t * chart_equation::name = L"equation";

void chart_equation::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
			CP_XML_ATTR_OPT(L"chart:display-r-square", display_r_square_);
		}
	}
}

// chart:stock-gain-marker
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_stock_gain_marker::ns = L"chart";
const wchar_t * chart_stock_gain_marker::name = L"stock-gain-marker";

void chart_stock_gain_marker::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
// chart:stock-loss-marker
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_stock_loss_marker::ns = L"chart";
const wchar_t * chart_stock_loss_marker::name = L"stock-loss-marker";

void chart_stock_loss_marker::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
// chart:stock-range-line
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_stock_range_line::ns = L"chart";
const wchar_t * chart_stock_range_line::name = L"stock-range-line";

void chart_stock_range_line::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
// chartooo:date-scale
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chart_date_scale::ns = L"chartooo";
const wchar_t * chart_date_scale::name = L"date-scale";

void chart_date_scale::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"chart:base-time-unit",		base_time_unit_);
			CP_XML_ATTR_OPT(L"chart:major-interval-value",	major_interval_value_);
			CP_XML_ATTR_OPT(L"chart:major-interval-unit",	major_interval_unit_);
			CP_XML_ATTR_OPT(L"chart:minor-interval-value",	minor_interval_value_);
			CP_XML_ATTR_OPT(L"chart:minor-interval-unit",	minor_interval_unit_);
		}
	}
}
}
}
