#pragma once


#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "common_attlists.h"
#include "text_elements.h"

namespace cpdoccore {
namespace odf {

/// \class  office_chart
/// \brief  office:chart
/// office-chart
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(office_chart);

///common-chart-attlis
class common_chart_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring)		chart_style_name_;
};

/// chart-chart-attlist
class chart_chart_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    common_draw_size_attlist	common_draw_size_attlist_;
    common_chart_attlist		common_attlist_;
    std::wstring				chart_class_;
    _CP_OPT(std::wstring)		chart_column_mapping_;
    _CP_OPT(std::wstring)		chart_row_mapping_;
};

/// \class  chart_chart
/// \brief  chart:chart
///         chart-chart
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_chart_attlist chart_chart_attlist_;
	office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_chart);

/// chart-title-attlist
class chart_title_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring)		table_cell_range_;
    common_draw_position_attlist common_draw_position_attlist_;
    common_chart_attlist		common_attlist_;

};

/// \class  chart_title
/// \brief  chart:title
///         chart-title
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_title_attlist chart_title_attlist_;
    office_element_ptr	text_p_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_title);

/// \class  chart_subtitle
/// \brief  chart:subtitle
///         chart-subtitle
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_title_attlist chart_title_attlist_;
    office_element_ptr text_p_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_subtitle);

/// \class  chart_footer
/// \brief  chart:footer
///         chart-footer
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;
	office_element_ptr text_p_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_footer);

class chart_legend_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring) chart_legend_position_;
    _CP_OPT(std::wstring) chart_legend_align_;
    common_draw_position_attlist common_draw_position_attlist_;
    _CP_OPT(std::wstring) style_legend_expansion_;
    _CP_OPT(double) style_legend_expansion_aspect_ratio_;
    common_chart_attlist		common_attlist_;

};

/// \class  chart_legend
/// \brief  chart:legend
///         chart-legend
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_legend_attlist chart_legend_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_legend);


class chart_plot_area_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    common_draw_position_attlist common_draw_position_attlist_;
    common_draw_size_attlist	common_draw_size_attlist_;
    common_chart_attlist		common_attlist_;
    
	_CP_OPT(std::wstring) table_cell_range_address_;
    _CP_OPT(std::wstring) chart_data_source_has_labels_;
    // dr3d-scene-attlist
    // common-dr3d-transform-attlist
};

/// \class  chart_plot_area
/// \brief  chart:plot-area
///         chart-plot-area
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_plot_area_attlist chart_plot_area_attlist_;

	office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_plot_area);

class chart_wall_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(length)				svg_width_;
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_wall_attlist	chart_wall_attlist_;

};
CP_REGISTER_OFFICE_ELEMENT2(chart_wall);


/// \class  chart_floor
/// \brief  chart:floor
///         chart-floor
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_floor);

class chart_axis_attlist 
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring) chart_dimension_;
    _CP_OPT(std::wstring) chart_name_;
    common_chart_attlist		common_attlist_;
};

/// \class  chart_axis
/// \brief  chart:axis
///         chart-axis
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_axis_attlist chart_axis_attlist_;

    office_element_ptr_array content_; 

};

CP_REGISTER_OFFICE_ELEMENT2(chart_axis);

class chart_grid_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring)		chart_class_;
    common_chart_attlist		common_attlist_;
    
};

/// \class  chart_grid
/// \brief  chart:grid
///         chart-grid
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_grid_attlist chart_grid_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_grid);

/// \class  chart_categories
/// \brief  chart:categories
///         chart-categories
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    _CP_OPT(std::wstring) table_cell_range_address_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_categories);

class chart_series_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring) chart_values_cell_range_address_;
    _CP_OPT(std::wstring) chart_label_cell_address_;
    _CP_OPT(std::wstring) chart_class_;
    _CP_OPT(std::wstring) chart_attached_axis_;
    common_chart_attlist  common_attlist_;

};

/// \class  chart_series
/// \brief  chart:series
///         chart-series
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_series_attlist chart_series_attlist_;

	office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_series);

/// \class  chart_domain
/// \brief  chart:domain
///         chart-domain
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

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

/// \class  chart_data_point
/// \brief  chart:data-point
///         chart-data-point
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    chart_data_point_attlist chart_data_point_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_data_point);

/// \class  chart_mean_value
/// \brief  chart:mean-value
///         chart-mean-value
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_mean_value);

/// \class  chart_error_indicator
/// \brief  chart:error-indicator
///         chart-error-indicator
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;

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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;
	_CP_OPT(bool) display_r_square_;

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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;
	office_element_ptr			chart_equation_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_regression_curve);


/// \class  chart_stock_gain_marker
/// \brief  chart:stock-gain-marker
///         chart-stock-gain-marker
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_stock_gain_marker);

/// \class  chart_stock_loss_marker
/// \brief  chart:stock-loss-marker
///         chart-stock-loss-marker
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_stock_loss_marker);

/// \class  chart_stock_range_line
/// \brief  chart:stock-range-line
///         chart-stock-range-line
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    common_chart_attlist		common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_stock_range_line);


}
}
