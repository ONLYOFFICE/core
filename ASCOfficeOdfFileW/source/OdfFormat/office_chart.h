#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "common_attlists.h"

#include "text_elements.h"

namespace cpdoccore {
namespace odf {

/// \brief  office:chart
class office_chart : public office_element_impl<office_chart>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeChart;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(office_chart);

///common-chart-attlis
class common_chart_attlist
{
public:
    _CP_OPT(std::wstring)			chart_style_name_;

	void serialize(CP_ATTR_NODE)	{CP_XML_ATTR_OPT(L"chart:style-name", chart_style_name_);}
};

/// chart-chart-attlist
class chart_chart_attlist
{
public:
    void serialize(CP_ATTR_NODE);

    common_draw_size_attlist	common_draw_size_attlist_;
    common_chart_attlist		common_attlist_;
    _CP_OPT(std::wstring)		chart_class_;
    _CP_OPT(std::wstring)		chart_column_mapping_;
    _CP_OPT(std::wstring)		chart_row_mapping_;
};

/// \brief  chart:chart
class chart_chart : public office_element_impl<chart_chart>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartChart;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    chart_chart_attlist				chart_chart_attlist_;
	office_element_ptr_array		content_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_chart);

/// chart-title-attlist
class chart_title_attlist
{
public:
    void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring)			table_cell_range_;
    common_draw_position_attlist	common_draw_position_attlist_;
    common_chart_attlist			common_attlist_;

};

/// \brief  chart:title
class chart_title : public office_element_impl<chart_title>
{
public: 
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartTitle;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    chart_title_attlist chart_title_attlist_;
    office_element_ptr	text_p_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_title);

/// \brief  chart:subtitle
class chart_subtitle : public office_element_impl<chart_subtitle>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartSubtitle;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    chart_title_attlist chart_title_attlist_;
    office_element_ptr text_p_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_subtitle);

/// \brief  chart:footer
class chart_footer : public office_element_impl<chart_footer>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartFooter;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    common_chart_attlist		common_attlist_;
	office_element_ptr			text_p_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_footer);

class chart_legend_attlist
{
public:
    void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring)			chart_legend_position_;
    _CP_OPT(std::wstring)			chart_legend_align_;
    common_draw_position_attlist	common_draw_position_attlist_;
    _CP_OPT(std::wstring)			style_legend_expansion_;
    _CP_OPT(double)					style_legend_expansion_aspect_ratio_;
    common_chart_attlist			common_attlist_;

};

/// \brief  chart:legend
class chart_legend : public office_element_impl<chart_legend>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartLegend;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    chart_legend_attlist chart_legend_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_legend);


class chart_plot_area_attlist
{
public:
    void serialize(CP_ATTR_NODE);

    common_draw_position_attlist	common_draw_position_attlist_;
    common_draw_size_attlist		common_draw_size_attlist_;
    common_chart_attlist			common_attlist_;
    
	_CP_OPT(std::wstring)			table_cell_range_address_;
    _CP_OPT(std::wstring)			chart_data_source_has_labels_;
    // dr3d-scene-attlist
    // common-dr3d-transform-attlist
};

/// \brief  chart:plot-area
class chart_plot_area : public office_element_impl<chart_plot_area>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartPlotArea;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    chart_plot_area_attlist chart_plot_area_attlist_;

	office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_plot_area);

class chart_wall_attlist
{
public:
     void serialize(CP_ATTR_NODE);

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

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    chart_wall_attlist	chart_wall_attlist_;

};
CP_REGISTER_OFFICE_ELEMENT2(chart_wall);


/// \brief  chart:floor
class chart_floor : public office_element_impl<chart_floor>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartFloor;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    common_chart_attlist		common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_floor);

class chart_axis_attlist 
{
public:
    void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring) chart_dimension_;
    _CP_OPT(std::wstring) chart_name_;
    common_chart_attlist		common_attlist_;
};

/// \brief  chart:axis
class chart_axis : public office_element_impl<chart_axis>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartAxis;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    chart_axis_attlist chart_axis_attlist_;
    office_element_ptr_array content_; 

};

CP_REGISTER_OFFICE_ELEMENT2(chart_axis);

class chart_grid_attlist
{
public:
   void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring)		chart_class_;
    common_chart_attlist		common_attlist_;
    
};

/// \brief  chart:grid
class chart_grid : public office_element_impl<chart_grid>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartGrid;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    chart_grid_attlist chart_grid_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_grid);

/// \brief  chart:categories
class chart_categories : public office_element_impl<chart_categories>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartCategories;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    _CP_OPT(std::wstring) table_cell_range_address_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_categories);

class chart_series_attlist
{
public:
    void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring) chart_values_cell_range_address_;
    _CP_OPT(std::wstring) chart_label_cell_address_;
    _CP_OPT(std::wstring) chart_class_;
    _CP_OPT(std::wstring) chart_attached_axis_;
    common_chart_attlist  common_attlist_;

};

/// \brief  chart:series
class chart_series : public office_element_impl<chart_series>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartSeries;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    chart_series_attlist chart_series_attlist_;
	office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_series);

/// \brief  chart:domain
class chart_domain : public office_element_impl<chart_domain>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartDomain;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);
   
	_CP_OPT(std::wstring) table_cell_range_address_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_domain);

class chart_data_point_attlist
{
public:
    void serialize(CP_ATTR_NODE);

    _CP_OPT(unsigned int)		chart_repeated_;
    common_chart_attlist		common_attlist_;
};

/// \brief  chart:data-point
class chart_data_point : public office_element_impl<chart_data_point>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartDataPoint;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    chart_data_point_attlist chart_data_point_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_data_point);

/// \brief  chart:mean-value
class chart_mean_value : public office_element_impl<chart_mean_value>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartMeanValue;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    common_chart_attlist		common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_mean_value);

/// \brief  chart:error-indicator
class chart_error_indicator : public office_element_impl<chart_error_indicator>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartErrorIndicator;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

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

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}//???
	virtual void add_child_element( office_element_ptr & child_element){}//???

	virtual void serialize(std::wostream & _Wostream);

    common_chart_attlist		common_attlist_;
	_CP_OPT(Bool)				display_r_square_;

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

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    common_chart_attlist		common_attlist_;
	office_element_ptr			chart_equation_;
};

CP_REGISTER_OFFICE_ELEMENT2(chart_regression_curve);

/// \brief  chart:stock-gain-marker
class chart_stock_gain_marker : public office_element_impl<chart_stock_gain_marker>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartStockGainMarker;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    common_chart_attlist common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_stock_gain_marker);

/// \brief  chart:stock-loss-marker
class chart_stock_loss_marker : public office_element_impl<chart_stock_loss_marker>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartStockLossMarker;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    common_chart_attlist		common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_stock_loss_marker);

/// \brief  chart:stock-range-line
class chart_stock_range_line : public office_element_impl<chart_stock_range_line>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeChartStockRangeLine;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    common_chart_attlist		common_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(chart_stock_range_line);


}
}
