#include "precompiled_cpodf.h"

#include "../docx/xlsx_textcontext.h"

#include "serialize_elements.h"
#include <cpdoccore/odf/odf_document.h>

#include "../formulasconvert/include/cpdoccore/formulasconvert.h"

#include "style_graphic_properties.h"
#include "style_chart_properties.h"
#include "style_text_properties.h"

#include "odfcontext.h"

#include "draw_common.h"

#include "calcs_styles.h"
#include "chart_build_oox.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"

namespace cpdoccore { 
namespace odf {


using xml::xml_char_wc;

class office_element;
typedef shared_ptr<const office_element>::Type office_element_ptr_const;

#define ACCEPT_ALL_CONTENT(VAL) \
        BOOST_FOREACH(office_element_ptr & elm, (VAL)) \
        { \
            if (elm) \
                elm->accept(*this); \
        }
        
#define ACCEPT_ALL_CONTENT_CONST(VAL) \
        BOOST_FOREACH(const office_element_ptr_const & elm, (VAL)) \
        { \
            if (elm) \
                elm->accept(*this); \
        }


//  ласс дл€ конструировани€ чартов
using namespace chart;

void chart_build::set_width(double valPt)
{
    width_pt_ = valPt;
}

void chart_build::set_height(double valPt)
{
    height_pt_ = valPt;
}

void chart_build::set_class(std::wstring const & val)
{
    str_class_ = val;

	class_= get_series_class_type(val);
}

void chart_build::set_style_name(std::wstring const & val)
{
    style_name_ = val;        
}
void chart_build::start_axis(std::wstring const & dimensionName, std::wstring const & name, std::wstring const & styleName)
{
    in_axis_ = true;
	axis ax;
	ax.dimension_=dimensionName;
	ax.chart_name_= name;
	ax.style_name_=styleName;

	axises_.push_back(ax);
}

void chart_build::end_axis()
{
    in_axis_ = false;
}

void chart_build::add_categories(std::wstring const & cellRange)
{
	categories_.push_back(cellRange);
}

void chart_build::add_grid(std::wstring const & className, std::wstring const & styleName)
{
    if (!axises_.empty())
    {
		axis::grid gr;
		gr.type_= (className == L"major" ? axis::grid::minor : axis::grid::major); // default: major
		gr.style_name_ = styleName;         
		axises_.back().grids_.push_back(gr);


    }
    else
    {
        _CP_LOG(warning) << "unexpected chart:grid" << std::endl;
    }
}
void chart_build::add_series(std::wstring const & cellRangeAddress,
        std::wstring const & labelCell,
        class_type classType,
        std::wstring const & attachedAxis,
        std::wstring const & styleName)
{
	if (class_ == chart_ring) classType = chart_ring; 
	if (class_ == chart_stock) classType = chart_stock; 

    series_.push_back(series(cellRangeAddress,labelCell, classType, attachedAxis, styleName));
}

void chart_build::add_point(unsigned int rep, std::wstring const & styleName)
{
    if (!series_.empty())
    {
		series_.back().points_.push_back(series::point(rep, styleName));                    
    }
    else
    {
        _CP_LOG(warning) << "unexpected chart:data-point" << std::endl;
    }        
}


void chart_build::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.start_chart(L"");
	oox::oox_chart_context & chart = Context.current_chart();
	
	oox_convert(chart);

	Context.end_chart();
}
void chart_build::docx_convert(oox::docx_conversion_context & Context)
{
	Context.start_chart(L"");
	oox::oox_chart_context & chart = Context.current_chart();

	oox_convert(chart);

	Context.end_chart();
}
void chart_build::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.start_chart(L"");
	oox::oox_chart_context & chart = Context.current_chart();
	
	oox_convert(chart);

	Context.end_chart();
}
void chart_build::calc_cash_series(std::wstring adress, std::vector<std::wstring> & cash)
{
    formulasconvert::odf2oox_converter converter;

	std::wstring ref_1,ref_2,table;
	size_t col_1=0,row_1=0,col_2=0,row_2=0;
	int res;
   
	if (!converter.find_first_last_ref(adress,table,ref_1,ref_2))return;
	//if ((res = table.find(L"local-table"))<0)return;

	oox::getCellAddressInv(ref_1,col_1,row_1);
	oox::getCellAddressInv(ref_2,col_2,row_2);

	BOOST_FOREACH(_cell & val,cash_values)
	{
		if (val.col>=col_1 && val.col<=col_2 &&
			val.row>=row_1 && val.row<=row_2)
		{
			cash.push_back(val.val);
		}
	}
}
void chart_build::oox_convert(oox::oox_chart_context & chart)
{
	chart.set_title(title_);
	chart.set_wall(wall_);
	chart.set_floor(floor_);
	chart.set_legend(legend_);
	chart.set_plot_area_properties(plot_area_.properties_, plot_area_.fill_);
	//chart.set_footer(footer_);
	chart.set_chart_graphic_properties(chart_graphic_properties_, chart_fill_);
	//chart.set_chart_properties(chart_graphic_properties_);

	class_type last_set_type=chart_unknown; 

	int series_id =0;

	if (series_.empty())
	{
		chart.add_chart(class_);
	}

	BOOST_FOREACH(series & s, series_)
	{
		if (s.class_!=last_set_type)
		{//разные типы серий в диаграмме - например бар и лини€.
			chart.add_chart(s.class_);
			last_set_type = s.class_;
		}
		oox::oox_chart_ptr current = chart.get_current_chart();

		if (!current) continue;
		
		current->set_properties(plot_area_.properties_);
		current->set_additional_properties(chart_graphic_properties_);
	
		current->add_series(series_id++);
		
		if (s.cell_range_address_.length()<1) s.cell_range_address_ = plot_area_.cell_range_address_;//SplitByColumn(ind_ser,range);
																									//SplitByRow(ind_ser,range)
		//тут данные нужно поделить по столбцам или строкам - так как в плот-ареа общий диапазон
		//первый столбец-строка ћќ∆≈т использоватьс€ дл€ подписей
		//кажда€ сери€ берет каждый последующий диапазрн
		//такое определение - редкость = todooo
		
		std::vector<std::wstring>				cell_cash;
		calc_cash_series(s.cell_range_address_,	cell_cash);

		if (domain_cell_range_adress_.length()>0) 
		{
			std::vector<std::wstring> domain_cash;
		
			calc_cash_series(domain_cell_range_adress_,domain_cash);
			
			if (last_set_type == chart_bubble)
			{
				current->set_formula_series(2, domain_cell_range_adress_);//bubble
				current->set_formula_series(3, s.cell_range_address_);//y		

				current->set_values_series (2, domain_cash);//x
				current->set_values_series (3, cell_cash);
			}
			else
			{
				current->set_formula_series(1, domain_cell_range_adress_);//x
				current->set_formula_series(2, s.cell_range_address_);//y		
			
				current->set_values_series (1, domain_cash);//x
				current->set_values_series (2, cell_cash);
			}
		}
		else
		{
			current->set_formula_series(0, s.cell_range_address_);//common
			current->set_values_series(0, cell_cash);//common
		}

		if (categories_.size() > 0)//названи€ 
		{			
			std::vector<std::wstring>			cat_cash;
			calc_cash_series(categories_[0],	cat_cash);
			
			current->set_formula_series(4,categories_[0]);
			current->set_values_series(4,cat_cash);
		}
		current->set_name(s.name_);
		chart.set_content_series(s);
	}
	bool x_enabled = false;
	bool y_enabled = false;
	bool z_enabled = false;
	BOOST_FOREACH(axis & a, axises_)
	{
		if	(a.dimension_ == L"x" && x_enabled)continue;
		if	(a.dimension_ == L"y" && y_enabled)continue;
		if	(a.dimension_ == L"z" && z_enabled)continue;

		int type =3;
		if		(a.dimension_ == L"x")
		{
			//могут быть типы 1, 2, 3, 4
			type=1;
			if (last_set_type == chart_scatter ||
				last_set_type == chart_bubble)type = 2;

			//if (last_set_type == chart_stock)type = 4; //шкала дат.
			x_enabled = true;
		}
		else if (a.dimension_ == L"y")
		{
			type=2;
			if (last_set_type ==  chart_bar)
			{
				//вот нахрена свойства относ€щиес€ к серии и самому чарту воткнули в оси ???? (ооо писали идиеты???)
				//или это банальна€ ошибка которую так никогда и не исправили???
				//overlap & gap-width
				oox::oox_chart_ptr current = chart.get_current_chart();
				current->set_additional_properties(a.properties_);
			}
			y_enabled = true;
		}
		else if (a.dimension_ == L"z")
		{
			type=2;
			z_enabled = true;
			continue;
		}

		chart.add_axis(type);
		chart.set_content_axis(a);
	}
}
void process_build_chart::ApplyChartProperties(std::wstring style,std::vector<_property> & propertiesOut)
{
	style_instance* styleInst = styles_.style_by_name(style, odf::style_family::Chart,false);
    if(styleInst)
	{
		const style_content * Content = styleInst->content();
		const style_chart_properties *properties = Content->get_style_chart_properties();

		if (!properties)return;

		BOOST_FOREACH(_property const & p, properties->content_)
		{
			propertiesOut.push_back(p);
		}
    }
}
void process_build_chart::ApplyTextProperties(std::wstring style,std::vector<_property> & propertiesOut)
{
	style_instance* styleInst = styles_.style_by_name(style, odf::style_family::Chart,false/*Context.process_headers_footers_*/);
    if(styleInst)
	{
		text_format_properties_content properties = calc_text_properties_content(styleInst);
		properties.apply_to(propertiesOut);
    }
}
void process_build_chart::ApplyGraphicProperties(std::wstring style,std::vector<_property> & propertiesOut,oox::_oox_fill & fill)
{
	style_instance* styleInst = styles_.style_by_name(style, odf::style_family::Chart,false/*Context.process_headers_footers_*/);
    if(styleInst)
	{
		graphic_format_properties properties = calc_graphic_properties_content(styleInst);

		Compute_GraphicFill(properties.common_draw_fill_attlist_, draw_styles_ , fill);	
		properties.apply_to(propertiesOut);
    }
}	

///////////////////////////////////////
bool process_build_chart::visit_table(std::wstring const & name)
{
    chart_build_.table_name_ = name;
    return true;
}

void process_build_chart::visit_column(unsigned int repeated)
{
    chart_build_.columns_count_ += repeated;
    chart_build_.columns_.push_back(repeated);    
}
bool process_build_chart::visit_rows(unsigned int repeated)
{
    chart_build_.current_table_column_ = 0;
    chart_build_.current_table_row_ += repeated;

    return true;
}
//////////////////////////////////////////////////
void process_build_chart::on_not_impl(std::string const & message)
{
    _CP_LOG(warning) << L"[process_draw_chart visitor] : not impliment for \"" << utf8_to_utf16(message) << L"\"" << std::endl;
}

//////////////////////////////////////////////////
void process_build_chart::visit(const office_document_content& val)
{
    if (val.office_body_)
        val.office_body_->accept(*this);
}
void process_build_chart::visit(office_body& val)
{
    if (val.content_)
        val.content_->accept(*this);
}
void process_build_chart::visit(office_chart& val)
{
	ACCEPT_ALL_CONTENT_CONST(val.content_);
}
void process_build_chart::visit(office_text& val)
{
	chart_build_.object_type_ = 2;
	chart_build_.office_text_ = &val;//конвертаци€ будет уровнем выше
}
void process_build_chart::visit(const chart_chart& val)
{
	chart_build_.object_type_ = 1;
	
	if (val.chart_chart_attlist_.common_draw_size_attlist_.svg_width_)
    {
        chart_build_.set_width(val.chart_chart_attlist_.common_draw_size_attlist_.svg_width_->get_value_unit(length::pt));
    }
           
    if (val.chart_chart_attlist_.common_draw_size_attlist_.svg_height_)
    {
        chart_build_.set_height(val.chart_chart_attlist_.common_draw_size_attlist_.svg_height_->get_value_unit(length::pt));
    }
	ApplyGraphicProperties	(val.chart_chart_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.chart_graphic_properties_, chart_build_.chart_fill_);

	chart_build_.set_class(val.chart_chart_attlist_.chart_class_);

	ACCEPT_ALL_CONTENT_CONST(val.content_);

}

void process_build_chart::visit(const chart_title& val)
{
	title t;

	if (val.text_p_)
	{
		std::wstringstream v;
		val.text_p_->text_to_stream(v);
		t.content_ = v.str();
	}
	ApplyTextProperties(val.chart_title_attlist_.common_attlist_.chart_style_name_.get_value_or(L""), t.text_properties_);

///////////////////////////////////////////////////////////////////////////////////////
	if (val.chart_title_attlist_.common_draw_position_attlist_.svg_x_)
    {
        t.pos_x = val.chart_title_attlist_.common_draw_position_attlist_.svg_x_->get_value_unit(length::pt);
		t.pos_y = val.chart_title_attlist_.common_draw_position_attlist_.svg_y_->get_value_unit(length::pt);
	}
	
	if (chart_build_.in_axis_)
		chart_build_.axises_.back().title_ = t; 
	else			
		chart_build_.title_ = t;	

}

void process_build_chart::visit(const chart_subtitle & val)
{
	title t;
	std::wstringstream v;
	val.text_p_->text_to_stream(v);
	t.content_ = v.str();

	if (val.chart_title_attlist_.common_draw_position_attlist_.svg_x_)
    {
        t.pos_x = val.chart_title_attlist_.common_draw_position_attlist_.svg_x_->get_value_unit(length::pt);
		t.pos_y = val.chart_title_attlist_.common_draw_position_attlist_.svg_y_->get_value_unit(length::pt);
	}
	chart_build_.sub_title_ = t;
}

void process_build_chart::visit(const chart_footer& val)
{
	ApplyChartProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.footer_.properties_);
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.footer_.graphic_properties_, chart_build_.footer_.fill_);
	ApplyTextProperties		(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.footer_.text_properties_);
}

void process_build_chart::visit(const chart_legend& val)
{
	ApplyChartProperties	(val.chart_legend_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.legend_.properties_);
	ApplyGraphicProperties	(val.chart_legend_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.legend_.graphic_properties_,chart_build_.legend_.fill_);
	ApplyTextProperties		(val.chart_legend_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.legend_.text_properties_);
}

void process_build_chart::visit(const chart_plot_area& val)
{
	ACCEPT_ALL_CONTENT_CONST(val.content_);

	chart_build_.plot_area_.cell_range_address_ = val.chart_plot_area_attlist_.table_cell_range_address_.get_value_or(L"");

	ApplyChartProperties	(val.chart_plot_area_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),chart_build_.plot_area_.properties_);
	ApplyGraphicProperties	(val.chart_plot_area_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),chart_build_.plot_area_.graphic_properties_, chart_build_.plot_area_.fill_);
	ApplyTextProperties		(val.chart_plot_area_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),chart_build_.plot_area_.text_properties_);
}


void process_build_chart::visit(const chart_axis& val)
{
    chart_build_.start_axis(val.chart_axis_attlist_.chart_dimension_.get_value_or(L""),
        val.chart_axis_attlist_.chart_name_.get_value_or(L""),
        val.chart_axis_attlist_.common_attlist_.chart_style_name_.get_value_or(L""));

    ACCEPT_ALL_CONTENT_CONST(val.content_);

	ApplyChartProperties	(val.chart_axis_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),chart_build_.axises_.back().properties_);
	ApplyGraphicProperties	(val.chart_axis_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),chart_build_.axises_.back().graphic_properties_,chart_build_.axises_.back().fill_);
	ApplyTextProperties		(val.chart_axis_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),chart_build_.axises_.back().text_properties_);

    chart_build_.end_axis();        
}

void process_build_chart::visit(const chart_series& val)
{
    const chart_series_attlist & att = val.chart_series_attlist_;
   
	chart::class_type chartClass = get_series_class_type(att.chart_class_.get_value_or(chart_build_.str_class_));

    chart_build_.add_series(
							att.chart_values_cell_range_address_.get_value_or(L""),
							att.chart_label_cell_address_.get_value_or(L""),
							chartClass,
							att.chart_attached_axis_.get_value_or(L""),
							att.common_attlist_.chart_style_name_.get_value_or(L"")
        );
	
	ACCEPT_ALL_CONTENT_CONST(val.content_);

	ApplyChartProperties	(att.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().properties_);
	ApplyGraphicProperties	(att.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().graphic_properties_,chart_build_.series_.back().fill_);
	ApplyTextProperties		(att.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().text_properties_);

}

void process_build_chart::visit(const chart_domain& val)
{
	chart_build_.domain_cell_range_adress_ = val.table_cell_range_address_.get_value_or(L"");
}
void process_build_chart::visit(const chart_grid& val)
{
    chart_build_.add_grid(val.chart_grid_attlist_.chart_class_.get_value_or(L""),
        val.chart_grid_attlist_.common_attlist_.chart_style_name_.get_value_or(L"") );
	
	oox::_oox_fill fill;
	
	ApplyGraphicProperties	(val.chart_grid_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	(chart_build_.axises_.back()).grids_.back().graphic_properties_, fill);

 }
void process_build_chart::visit(const chart_wall& val)
{      
	ApplyChartProperties	(val.chart_wall_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.wall_.properties_);
	ApplyGraphicProperties	(val.chart_wall_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.wall_.graphic_properties_,chart_build_.wall_.fill_);
	ApplyTextProperties		(val.chart_wall_attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.wall_.text_properties_);
} 

void process_build_chart::visit(const chart_floor& val)
{   
	ApplyChartProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.floor_.properties_);
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.floor_.graphic_properties_,chart_build_.floor_.fill_);
	ApplyTextProperties		(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.floor_.text_properties_);
} 

void process_build_chart::visit(const chart_data_point & val)
{
	chart_build_.add_point(	val.chart_data_point_attlist_.chart_repeated_.get_value_or(0/*1*/),
							val.chart_data_point_attlist_.common_attlist_.chart_style_name_.get_value_or(L"")
        );
}
void process_build_chart::visit(const chart_mean_value & val)
{
	ApplyChartProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().mean_value_.properties_);
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().mean_value_.graphic_properties_,chart_build_.series_.back().mean_value_.fill_);
}
void process_build_chart::visit(const chart_error_indicator & val)
{
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().error_indicator_.graphic_properties_,chart_build_.series_.back().error_indicator_.fill_ );
}	
void process_build_chart::visit(const chart_regression_curve & val)
{
	oox::_oox_fill fill;
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().regression_curve_.line_properties_, fill);

	if (val.chart_equation_)
	{
		chart_build_.series_.back().regression_curve_.bEquation = true;

		val.chart_equation_->accept(*this);
	}
}
void process_build_chart::visit(const chart_equation & val)
{
	if (chart_build_.series_.back().regression_curve_.bEquation == false)return;
	
	
	if (val.display_r_square_)
		chart_build_.series_.back().regression_curve_.bREquation = val.display_r_square_.get();

	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().regression_curve_.equation_properties_.graphic_properties_,chart_build_.series_.back().regression_curve_.equation_properties_.fill_);
	ApplyTextProperties		(val.common_attlist_.chart_style_name_.get_value_or(L""),	chart_build_.series_.back().regression_curve_.equation_properties_.text_properties_);

}
void process_build_chart::visit(const chart_categories& val)
{     
	if (val.table_cell_range_address_)  chart_build_.add_categories(*val.table_cell_range_address_);
}    
void process_build_chart::visit(const table_table& val)
{        
	ACCEPT_ALL_CONTENT_CONST(val.table_columns_and_groups_.content_);
	ACCEPT_ALL_CONTENT_CONST(val.table_rows_and_groups_.content_);
}   
void process_build_chart::visit(const table_table_rows& val)
{        
    ACCEPT_ALL_CONTENT_CONST(val.table_table_row_);
}
void process_build_chart::visit(table_table_rows& val)
{        
    ACCEPT_ALL_CONTENT_CONST(val.table_table_row_);
}
void process_build_chart::visit(const table_table_row & val)
{       
    unsigned int repeated = val.table_table_row_attlist_.table_number_rows_repeated_;
    ACCEPT_ALL_CONTENT_CONST(val.content_);
	visit_rows(repeated);
}
void process_build_chart::visit(const table_table_column& val)
{
    const unsigned int columnsRepeated = val.table_table_column_attlist_.table_number_columns_repeated_;
  
	visit_column(columnsRepeated);
}
void process_build_chart::visit(const table_table_row_group& val)
{
    ACCEPT_ALL_CONTENT_CONST(val.table_rows_and_groups_.content_);
}
void process_build_chart::visit(const table_table_column_group& val)
{
    ACCEPT_ALL_CONTENT_CONST(val.table_columns_and_groups_.content_);
}
void process_build_chart::visit(table_table_columns& val)
{
    ACCEPT_ALL_CONTENT_CONST(val.table_table_column_);
}
void process_build_chart::visit(const table_columns_no_group& val)
{
    if (val.table_columns_1_.table_table_columns_)
        val.table_columns_1_.table_table_columns_->accept(*this);

    ACCEPT_ALL_CONTENT_CONST(val.table_columns_1_.table_table_column_);

    if (val.table_table_header_columns_)
        val.table_table_header_columns_->accept(*this);

    if (val.table_columns_2_.table_table_columns_)
        val.table_columns_2_.table_table_columns_->accept(*this);

    ACCEPT_ALL_CONTENT_CONST(val.table_columns_2_.table_table_column_);
}
void process_build_chart::visit(const table_rows_no_group& val)
{
 	if (val.table_rows_1_.table_table_rows_)
        val.table_rows_1_.table_table_rows_->accept(*this);
	else
		ACCEPT_ALL_CONTENT_CONST(val.table_rows_1_.table_table_row_);
  
	if (val.table_table_header_rows_)
        val.table_table_header_rows_->accept(*this);
   
 	if (val.table_rows_2_.table_table_rows_)
        val.table_rows_2_.table_table_rows_->accept(*this);
	else
		ACCEPT_ALL_CONTENT_CONST(val.table_rows_2_.table_table_row_);
}
void process_build_chart::visit(const table_table_cell& val)
{
    unsigned int repeated = val.table_table_cell_attlist_.table_number_columns_repeated_;
  
	std::wstringstream  wstream_temp;	
   
	val.table_table_cell_content_.text_to_stream(wstream_temp);

	chart_build::_cell cell_={chart_build_.current_table_column_,chart_build_.current_table_row_,wstream_temp.str()};

	chart_build_.cash_values.push_back(cell_);
	
	chart_build_.current_table_column_+=repeated;
}
void process_build_chart::visit(const table_covered_table_cell& val)
{
    unsigned int repeated = val.table_table_cell_attlist_.table_number_columns_repeated_;

	if ( repeated <2) 
		return;
}
void process_build_chart::visit(table_table_header_columns& val)
{
    ACCEPT_ALL_CONTENT_CONST(val.table_table_column_);
}
void process_build_chart::visit(const table_table_header_rows& val)
{        
    ACCEPT_ALL_CONTENT_CONST(val.table_table_row_);
}
void process_build_chart::visit(table_table_header_rows& val)
{        
    ACCEPT_ALL_CONTENT_CONST(val.table_table_row_);
}
}
}
