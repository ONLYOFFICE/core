#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost_string.h>
#include <regex.h>
//
#include <cpdoccore/CPString.h>
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "visitor.h"

#include "chart_objects.h"

#include "office_document.h"
#include "office_body.h"
#include "office_chart.h"
#include "office_text.h"
#include "table.h"

namespace cpdoccore { 

namespace odf {

namespace {

struct class_type_pair
{
    chart::class_type class_type_;
    std::wstring class_type_str_;
};

static const class_type_pair class_type_str[] = 
{
    {chart::chart_line, L"chart:line"},
    {chart::chart_area, L"chart:area"},
    {chart::chart_circle, L"chart:circle"},
    {chart::chart_ring, L"chart:ring"},
    {chart::chart_scatter, L"chart:scatter"},
    {chart::chart_radar, L"chart:radar"},
    {chart::chart_bar, L"chart:bar"},
    {chart::chart_stock, L"chart:stock"},
    {chart::chart_bubble, L"chart:bubble"},
    {chart::chart_surface, L"chart:surface"},
    {chart::chart_gantt, L"chart:gantt"},
	{chart::chart_filled_radar, L"chart:filled-radar"}
};

chart::class_type static get_series_class_type(std::wstring const & str)
{
    BOOST_FOREACH(class_type_pair const & p, class_type_str)
    {
        if (p.class_type_str_ == str)
            return p.class_type_;
    }
    return chart::chart_bar; //лучше хоть какой назначить чем никакой !!
}

}

class chart_build 
{
public:
	struct _cell
	{
		int col;
		int row;
		std::wstring val;
	};

    chart_build() : width_pt_(0), height_pt_(0), in_axis_(false),
        current_table_column_(0),
        current_table_row_(0),
        columns_spanned_num_(0),
		//target_table_(0/*targetTable*/),
		columns_count_(0),
		object_type_(0),
		office_text_(NULL)
    {
	}
	
	void set_width(double valPt);

    void set_height(double valPt);

    void set_class(std::wstring const & val);

    void set_style_name(std::wstring const & val);

    void start_axis(std::wstring const & dimensionName, std::wstring const & name, std::wstring const & styleName);

    void end_axis();

	void add_categories(std::wstring const & cellRangeAddress);

    void add_grid(std::wstring const & className, std::wstring const & styleName);
    void add_series(std::wstring const & cellRangeAddress,
            std::wstring const & labelCell,
			chart::class_type classType,
            std::wstring const & attachedAxis,
            std::wstring const & styleName);

    void add_point(unsigned int rep, std::wstring const & styleName);
	
	void chart_build::calc_cash_series(std::wstring adress, std::vector<std::wstring> & cash);
	
	void xlsx_convert(oox::xlsx_conversion_context & Context);
	void docx_convert(oox::docx_conversion_context & Context);
	void oox_convert(oox::oox_chart_context & chart);
	void pptx_convert(oox::pptx_conversion_context & Context);

    double width_pt_;
    double height_pt_;

	int object_type_;
	office_text *office_text_;
  
	std::wstring str_class_;  
	chart::class_type class_;  
	std::wstring style_name_;
 	std::wstring name_;
  
	bool in_axis_;
    std::vector<chart::axis>	axises_;
    std::vector<chart::series>	series_;
	std::vector<std::wstring>	categories_;

	std::wstring domain_cell_range_adress_;

	chart::title title_;
	office_element_ptr_array title_odf_context_;

	chart::title sub_title_;
	chart::simple legend_;

	chart::plot_area plot_area_;

	chart::simple wall_;
	chart::simple floor_;
	
	chart::simple footer_;

	std::vector<_property>	chart_properties_;
	std::vector<_property>	chart_graphic_properties_;
	oox::_oox_fill			chart_fill_;

	std::vector<_cell> cash_values;
public:
	std::wstring target_table_;

	std::wstring table_name_;
   
	int current_table_column_;
    int current_table_row_;

	unsigned int columns_spanned_num_;
    std::vector<oox::xlsx_row_spanned> rows_spanned_;
   
	std::vector<unsigned int> columns_;
    unsigned int columns_count_;

///////////////////////////////////////////

};
// Класс для обхода всех элеменов office:object для построения диаграммы

class process_build_chart : public base_visitor,
	public const_visitor<office_document_content>,
    
	public visitor<office_body>,
    public visitor<office_chart>,
    
	public visitor<office_text>,
    
    public const_visitor<chart_chart>,

    public const_visitor<chart_title>,
    public const_visitor<chart_subtitle>,
    public const_visitor<chart_footer>,
    public const_visitor<chart_legend>,

    public const_visitor<chart_plot_area>,
    
	public const_visitor<chart_axis>,
    public const_visitor<chart_categories>,
    public const_visitor<chart_grid>,

    public const_visitor<chart_series>,
	public const_visitor<chart_domain>,
    public const_visitor<chart_data_point>,
	public const_visitor<chart_mean_value>,
	public const_visitor<chart_regression_curve>,
	public const_visitor<chart_equation>,
	public const_visitor<chart_error_indicator>,
    public const_visitor<chart_wall>,
    public const_visitor<chart_floor>,

    public const_visitor<table_table>,
    
    public const_visitor<table_table_row_group>,
    public const_visitor<table_rows_no_group>,
    public const_visitor<table_table_header_rows>,
    public const_visitor<table_table_rows>,
    public const_visitor<table_table_row>,

    public visitor<table_table_rows>,
    public visitor<table_table_header_rows>,

    public const_visitor<table_table_cell>,
    public const_visitor<table_covered_table_cell>,    

    public const_visitor<table_table_column_group>,
    public visitor<table_table_header_columns>,
    public visitor<table_table_columns>,
    public const_visitor<table_table_column>,
    public const_visitor<table_columns_no_group>
{
public:

    process_build_chart(chart_build & chartBuild, styles_container & styles, styles_lite_container & draw_styles):	
	 stop_(false)
	,chart_build_(chartBuild)
	,styles_(styles)
	,draw_styles_(draw_styles)
    {
        _CP_LOG(warning) << L"[process_draw_object] \"" << L"\"" << std::endl;

	}

private:
	void ApplyChartProperties(std::wstring style,std::vector<_property> & propertiesOut);
	void ApplyTextProperties(std::wstring style,std::vector<_property> & propertiesOut);
	void ApplyGraphicProperties(std::wstring style,std::vector<_property> & propertiesOut, oox::_oox_fill & fill);

	bool visit_table(std::wstring const & name);
    void visit_column(unsigned int repeated);
    bool visit_rows(unsigned int repeated);
	virtual void on_not_impl(std::string const & message);

public:

	virtual void visit(const office_document_content& val);
	virtual void visit(office_body& val);
    virtual void visit(office_chart& val);
    virtual void visit(office_text& val);
    virtual void visit(const chart_chart& val);
	virtual void visit(const chart_title& val);
    virtual void visit(const chart_subtitle& val);
    virtual void visit(const chart_footer& val);
    virtual void visit(const chart_legend& val);
    virtual void visit(const chart_plot_area& val);
    virtual void visit(const chart_axis& val);
	virtual void visit(const chart_series& val);
    virtual void visit(const chart_domain& val);
	virtual void visit(const chart_data_point & val);
	virtual void visit(const chart_mean_value & val);
	virtual void visit(const chart_error_indicator & val);
	virtual void visit(const chart_regression_curve & val);
 	virtual void visit(const chart_equation & val);
	virtual void visit(const chart_categories& val);
	virtual void visit(const chart_grid& val);
    virtual void visit(const chart_wall& val);
    virtual void visit(const chart_floor& val);   
	virtual void visit(const table_table& val);

	virtual void visit(const table_table_rows& val);
	virtual void visit(const table_rows_no_group& val);
 	virtual void visit(const table_table_row_group& val);
	virtual void visit(const table_table_header_rows& val);
	virtual void visit(table_table_header_rows& val);

	virtual void visit(table_table_rows& val);
	virtual void visit(const table_table_row& val);

	virtual void visit(const table_columns_no_group& val);
	virtual void visit(table_table_header_columns& val);
	virtual void visit(table_table_columns& val);
	virtual void visit(const table_table_column_group& val);
    virtual void visit(const table_table_column& val);   
	
 
	virtual void visit(const table_table_cell& val);
 	virtual void visit(const table_covered_table_cell& val);

private:
    bool stop_;
    
	chart_build				& chart_build_;
	styles_container		& styles_;
	styles_lite_container	& draw_styles_;


};



}
}