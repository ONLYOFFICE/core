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


#include <iosfwd>

#include "office_elements.h"
#include "office_elements_create.h"
#include "text_elements.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/charttimeunit.h"
#include "../../DataTypes/chartclass.h"

namespace cpdoccore {
namespace odf_reader {

//  office:chart
class office_chart : public office_element_impl<office_chart>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeChart;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
	office_element_ptr			tracked_changes_;
	office_element_ptr			content_validations_;
    
	office_element_ptr_array	content_;

};

CP_REGISTER_OFFICE_ELEMENT2(office_chart);

///common-chart-attlis
class common_chart_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    _CP_OPT(std::wstring)		chart_style_name_;
};

/// chart-chart-attlist
class chart_chart_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    odf_types::common_draw_size_attlist	common_draw_size_attlist_;
    common_chart_attlist				common_attlist_;
	odf_types::chart_class				chart_class_;
    _CP_OPT(std::wstring)				chart_column_mapping_;
    _CP_OPT(std::wstring)				chart_row_mapping_;
	_CP_OPT(std::wstring)				loext_data_pilot_source_;
};

//  chart:chart
class chart_chart : public office_element_impl<chart_chart>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartChart;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_chart_attlist			attlist_;
	
	office_element_ptr			tracked_changes_;
	office_element_ptr			content_validations_;
	
	office_element_ptr_array	content_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_chart);

/// chart-title-attlist
class chart_title_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    _CP_OPT(std::wstring)					table_cell_range_;
    odf_types::common_draw_position_attlist common_draw_position_attlist_;
    common_chart_attlist					common_attlist_;

};

//  chart:title
class chart_title : public office_element_impl<chart_title>
{
public: 
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartTitle;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_title_attlist attlist_;
    office_element_ptr	text_p_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_title);

//  chart:subtitle
class chart_subtitle : public office_element_impl<chart_subtitle>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartSubtitle;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_title_attlist	attlist_;
    office_element_ptr	text_p_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_subtitle);

//  chart:footer
class chart_footer : public office_element_impl<chart_footer>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartFooter;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;
	office_element_ptr			text_p_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_footer);

class chart_legend_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    _CP_OPT(std::wstring)		chart_legend_position_;
    _CP_OPT(std::wstring)		chart_legend_align_;
    _CP_OPT(std::wstring)		style_legend_expansion_;
    _CP_OPT(double)				style_legend_expansion_aspect_ratio_;
	
	odf_types::common_draw_position_attlist		common_draw_position_attlist_;
	common_chart_attlist						common_attlist_;

};

//  chart:legend
class chart_legend : public office_element_impl<chart_legend>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartLegend;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_legend_attlist attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_legend);


class chart_plot_area_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
   
	odf_types::common_draw_position_attlist common_draw_position_attlist_;
    odf_types::common_draw_size_attlist		common_draw_size_attlist_;
 	odf_types::common_dr3d_attlist			common_dr3d_attlist_;
    
    common_chart_attlist					common_attlist_;
	
	_CP_OPT(std::wstring) table_cell_range_address_;
    _CP_OPT(std::wstring) chart_data_source_has_labels_;
    
};

//  chart:plot-area
class chart_plot_area : public office_element_impl<chart_plot_area>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartPlotArea;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_plot_area_attlist		attlist_;
	office_element_ptr_array	content_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_plot_area);

class chart_wall_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(odf_types::length)	svg_width_;
    common_chart_attlist		common_attlist_;

};

class chart_wall : public office_element_impl<chart_wall>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartWall;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_wall_attlist	attlist_;

};
CP_REGISTER_OFFICE_ELEMENT2(chart_wall);


//  chart:floor
class chart_floor : public office_element_impl<chart_floor>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartFloor;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    common_chart_attlist	common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_floor);

class chart_axis_attlist 
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring)		chart_dimension_;
    _CP_OPT(std::wstring)		chart_name_;
    common_chart_attlist		common_attlist_;
};

//  chart:axis
class chart_axis : public office_element_impl<chart_axis>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartAxis;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_axis_attlist			attlist_;
    office_element_ptr_array	content_; 

};

CP_REGISTER_OFFICE_ELEMENT2(chart_axis);

class chart_grid_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring)		grid_class_;
    common_chart_attlist		common_attlist_;
    
};

//  chart:grid
class chart_grid : public office_element_impl<chart_grid>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartGrid;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_grid_attlist attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_grid);

//  chart:categories
class chart_categories : public office_element_impl<chart_categories>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartCategories;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    _CP_OPT(std::wstring) table_cell_range_address_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_categories);

class chart_series_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring)			chart_values_cell_range_address_;
    _CP_OPT(std::wstring)			chart_label_cell_address_;
    _CP_OPT(odf_types::chart_class)	chart_class_;
    _CP_OPT(std::wstring)			chart_attached_axis_;
    common_chart_attlist			common_attlist_;

};

//  chart:series
class chart_series : public office_element_impl<chart_series>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartSeries;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    chart_series_attlist		attlist_;
	office_element_ptr_array	content_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_series);

//  chart:domain
class chart_domain : public office_element_impl<chart_domain>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartDomain;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    _CP_OPT(std::wstring) table_cell_range_address_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_domain);

class chart_data_point_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(unsigned int)		chart_repeated_;
    common_chart_attlist		common_attlist_;
};

//  chart:data-point
class chart_data_point : public office_element_impl<chart_data_point>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartDataPoint;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    chart_data_point_attlist attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_data_point);

//  chart:mean-value
class chart_mean_value : public office_element_impl<chart_mean_value>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartMeanValue;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    common_chart_attlist common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_mean_value);

//  chart:error-indicator
class chart_error_indicator : public office_element_impl<chart_error_indicator>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartErrorIndicator;
    CPDOCCORE_DEFINE_VISITABLE();


private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    common_chart_attlist common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_error_indicator);

class chart_equation : public office_element_impl<chart_equation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartEquation;
    CPDOCCORE_DEFINE_VISITABLE();


private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    common_chart_attlist	common_attlist_;
	_CP_OPT(bool)			display_r_square_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_equation);


class chart_regression_curve : public office_element_impl<chart_regression_curve>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartRegressionCurve;
    CPDOCCORE_DEFINE_VISITABLE();


private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    common_chart_attlist	common_attlist_;
	office_element_ptr		chart_equation_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_regression_curve);


//  chart:stock-gain-marker
class chart_stock_gain_marker : public office_element_impl<chart_stock_gain_marker>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartStockGainMarker;
    CPDOCCORE_DEFINE_VISITABLE();


private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    common_chart_attlist common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_stock_gain_marker);

//  chart:stock-loss-marker
class chart_stock_loss_marker : public office_element_impl<chart_stock_loss_marker>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartStockLossMarker;
    CPDOCCORE_DEFINE_VISITABLE();


private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    common_chart_attlist		common_attlist_;

};
CP_REGISTER_OFFICE_ELEMENT2(chart_stock_loss_marker);

//  chart:stock-range-line
class chart_stock_range_line : public office_element_impl<chart_stock_range_line>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartStockRangeLine;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
    common_chart_attlist common_attlist_;

};
CP_REGISTER_OFFICE_ELEMENT2(chart_stock_range_line);

//chartooo:date-scale
class chart_date_scale : public office_element_impl<chart_date_scale>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartDateScale;
    CPDOCCORE_DEFINE_VISITABLE();

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
	{}

public:
	_CP_OPT(odf_types::chart_time_unit)	base_time_unit_;

	_CP_OPT(unsigned int)				major_interval_value_;
	_CP_OPT(odf_types::chart_time_unit)	major_interval_unit_;

	_CP_OPT(unsigned int)				minor_interval_value_;
 	_CP_OPT(odf_types::chart_time_unit)	minor_interval_unit_;
};
CP_REGISTER_OFFICE_ELEMENT2(chart_date_scale);
}
}
