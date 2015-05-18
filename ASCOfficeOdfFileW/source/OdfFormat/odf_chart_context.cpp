
#include "logging.h"

#include <boost/foreach.hpp>

#include "../../../ASCOfficeOdfFile/formulasconvert/include/cpdoccore/formulasconvert.h"

#include <iostream>

#include "odf_chart_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_drawing_context.h"
 
#include "odf_conversion_context.h"

#include "ods_table_state.h"//???

#include "office_chart.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"
#include "style_chart_properties.h"

namespace cpdoccore 
{
namespace odf
{
	static const std::wstring default_MS_series_colors[] = 
	{
		L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934"
	};

	struct 	odf_axis_state
	{
		unsigned int		oox_id;
		int					dimension;//1 -x, 2, -y, 3 -z
		std::wstring		name;
		office_element_ptr	elm;
	};
	struct 	odf_cash_state
	{
		std::wstring ref;
		std::wstring format;

		bool label;
		//std::vector<double>		data_double;
		std::vector<std::wstring>	data_str;
	};
	struct 	odf_chart_state
	{
		odf_chart_state() {clear();}
		void clear()
		{
			elements_.clear();
			chart_width_pt = chart_height_pt = 0;
		}
		double chart_width_pt;
		double chart_height_pt;

		std::vector<odf_element_state>	elements_;
	};

	struct odf_chart_level_state
	{
		style_text_properties		*text_properties_;
		style_graphic_properties	*graphic_properties_;
		style_paragraph_properties	*paragraph_properties_;
		style_chart_properties		*chart_properties_;
		
		office_element_ptr	elm;
	};	
	struct _cell_cash
	{
		int col;
		int row;
		bool label;
		std::wstring val;
	};
class odf_chart_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		styles_context_ = NULL;
		current_series_count_= 0;
		local_table_enabled_ = false;
		
	} 

	odf_chart_state							current_chart_state_;
	int										current_series_count_;
	int										current_data_points_series_count_;
	
	std::vector<std::pair<std::wstring,int>>categories_;
	std::vector<odf_axis_state>				axis_;
	std::vector<office_element_ptr>			group_series_;
	std::vector<unsigned int>				axis_group_series_;

	struct _range
	{
		std::wstring	ref;
		bool			label;
	};
	std::vector<_range>						data_cell_ranges_;
	
	std::vector<odf_cash_state>				cash_;
	bool									local_table_enabled_;

	std::vector<odf_chart_level_state>	current_level_;	//постоянно меняющийся список уровней наследования
	std::vector<odf_chart_state>		chart_list_;		//все элементы .. для удобства разделение по "топам"
////////
	void set_default_series_color();
	void clear_current();
	odf_style_context			*styles_context_;
	odf_conversion_context		*odf_context_;

	chart_chart					*get_current_chart();
	chart_axis					*get_current_axis();
	chart_series				*get_current_series();

	void create_local_table();
	void create_local_table_rows(ods_table_state * table_state,std::vector<_cell_cash> & cells, bool header );
};
chart_chart* odf_chart_context::Impl::get_current_chart()
{
	for (long i=current_level_.size()-1; i>=0; i--)
	{
		chart_chart * chart = dynamic_cast<chart_chart*>(current_level_[i].elm.get());
		if (chart) return chart;
	}
	return NULL;
}
chart_series* odf_chart_context::Impl::get_current_series()
{
	for (long i=current_level_.size()-1; i>=0; i--)
	{
		chart_series * chart = dynamic_cast<chart_series*>(current_level_[i].elm.get());
		if (chart) return chart;
	}
	return NULL;
}
chart_axis * odf_chart_context::Impl::get_current_axis()
{
	if (axis_.size() > 0) return dynamic_cast<chart_axis*>(axis_.back().elm.get());

	return NULL;
}
void odf_chart_context::Impl::clear_current()
{
	current_chart_state_.clear();
	categories_.clear();
	axis_.clear();
	group_series_.clear();
	data_cell_ranges_.clear();
	cash_.clear();

	current_series_count_ = 0;
	local_table_enabled_ = false;
}
void odf_chart_context::Impl::set_default_series_color()
{
	if (!current_level_.back().graphic_properties_)return;

	color col = color(default_MS_series_colors[current_series_count_]);
	
	current_level_.back().graphic_properties_->content().common_draw_fill_attlist_.draw_fill_color_ = col;
	current_level_.back().graphic_properties_->content().svg_stroke_color_ = col;
}
////////////////////////////////////////////////////////////////////////////

odf_chart_context::odf_chart_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_chart_context::Impl(odf_context))
{
}

odf_chart_context::~odf_chart_context()
{
} 
void odf_chart_context::set_styles_context(odf_style_context * style_context)
{
	impl_->styles_context_ = style_context;
	
	impl_->odf_context_->drawing_context()->set_styles_context(style_context);
}

odf_drawing_context * odf_chart_context::drawing_context()
{
	return  impl_->odf_context_->drawing_context();
}
odf_text_context	* odf_chart_context::text_context()
{
	return impl_->odf_context_->text_context();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void odf_chart_context::start_chart(office_element_ptr & root)
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"chart", chart_elm, impl_->odf_context_);

	chart_chart *chart = dynamic_cast<chart_chart*>(chart_elm.get());
	if (chart == NULL)return;

	root->add_child_element(chart_elm);
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	int level = impl_->current_level_.size();
	std::wstring style_name;
	
	odf_element_state		state={chart_elm, style_name, style_elm, level};
	odf_chart_level_state	level_state = {NULL,NULL,NULL,NULL,chart_elm};

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		level_state.chart_properties_ = style_->style_content_.get_style_chart_properties();
		
		chart->chart_chart_attlist_.common_attlist_.chart_style_name_ = style_name;
	}

	drawing_context()->start_element(chart_elm, style_elm);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//if (impl_->current_level_.size()>0)	impl_->current_level_.back()->add_child_element(chart_elm); не надо ... наследование через drawing

	impl_->current_level_.push_back(level_state);


	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::set_chart_size(double width_pt, double height_pt)
{
	if (width_pt <0.01 || height_pt < 0.01)return;

	impl_->current_chart_state_.chart_height_pt = height_pt;
	impl_->current_chart_state_.chart_width_pt = width_pt;
	
	chart_chart *chart = impl_->get_current_chart();
	if (!chart)return;

	chart->chart_chart_attlist_.common_draw_size_attlist_.svg_width_ = length(length(width_pt,length::pt).get_value_unit(length::cm),length::cm);
	chart->chart_chart_attlist_.common_draw_size_attlist_.svg_height_ = length(length(height_pt,length::pt).get_value_unit(length::cm),length::cm);
}
void odf_chart_context::set_chart_type(std::wstring type)
{
	chart_chart *chart = impl_->get_current_chart();
	if (!chart)return;

	chart->chart_chart_attlist_.chart_class_ = std::wstring(L"chart:") + type;
}

void odf_chart_context::set_chart_bar_type(int type)
{	
	if (!impl_->current_level_.back().chart_properties_) return;
	switch(type)
	{
		case 0:	//	st_shapeCONE = 0,
			impl_->current_level_.back().chart_properties_->content().chart_solid_type_ = chart_solid_type(chart_solid_type::cone); break;
		case 1:	//	st_shapeCONETOMAX = 1,
			impl_->current_level_.back().chart_properties_->content().chart_solid_type_ = chart_solid_type(chart_solid_type::cone); break;
		case 2:	//	st_shapeBOX = 2,
			impl_->current_level_.back().chart_properties_->content().chart_solid_type_ = chart_solid_type(chart_solid_type::cuboid); break;
		case 3:	//	st_shapeCYLINDER = 3,
			impl_->current_level_.back().chart_properties_->content().chart_solid_type_ = chart_solid_type(chart_solid_type::cylinder); break;
		case 4:	//	st_shapePYRAMID = 4,
			impl_->current_level_.back().chart_properties_->content().chart_solid_type_ = chart_solid_type(chart_solid_type::pyramid); break;
		case 5:	//	st_shapePYRAMIDTOMAX = 5
			impl_->current_level_.back().chart_properties_->content().chart_solid_type_ = chart_solid_type(chart_solid_type::pyramid); break;
	}
	if (type == -1)
	{
		//нужно вытащить свойство с уровня выше.
		int sz = impl_->current_level_.size();
		if (sz > 1)
			impl_->current_level_.back().chart_properties_->content().chart_solid_type_ = 
					impl_->current_level_[sz-2].chart_properties_->content().chart_solid_type_;
	}
}

void odf_chart_context::set_chart_bar_direction(int type)
{	
	if (!impl_->current_level_.back().chart_properties_) return;
	switch(type)
	{
		case 0:	//	st_bardirBAR = 0,
			impl_->current_level_.back().chart_properties_->content().chart_vertical_ = true; break;
		case 1:	//	st_bardirCOL = 1
			break;
	}
}
void odf_chart_context::set_chart_bar_gap_width(std::wstring val)
{
	if (!impl_->current_level_.back().chart_properties_) return;
	
	int res = val.find(L"%");

	bool percent=false;
	if (res > 0)
	{
		val = val.substr(0,res);
		percent=true;
	}
	double dVal = boost::lexical_cast<double>(val);
	impl_->current_level_.back().chart_properties_->content().chart_gap_width_ = (int)dVal;
}
void odf_chart_context::set_chart_bar_overlap(std::wstring val)
{
	if (!impl_->current_level_.back().chart_properties_) return;
	int res = val.find(L"%");

	bool percent=false;
	if (res > 0)
	{
		val = val.substr(0,res);
		percent=true;
	}
	double dVal = boost::lexical_cast<double>(val);
	impl_->current_level_.back().chart_properties_->content().chart_overlap_ = (int)dVal;
}

void odf_chart_context::set_chart_stock_candle_stick(bool val)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	impl_->current_level_.back().chart_properties_->content().chart_japanese_candle_stick_ = val;

}

void odf_chart_context::set_chart_radar_type(int type)
{
	switch(type)
	{
		case 0:	//	st_radarstyleSTANDARD = 0,
			break;
		case 1:	//	st_radarstyleMARKER = 1,
			set_marker_type(12);//automatic
			break;
		case 2:	//	st_radarstyleFILLED = 2
			chart_chart *chart = impl_->get_current_chart();
			if (chart)chart->chart_chart_attlist_.chart_class_ = std::wstring(L"chart:filled-radar");
			break;
	}

}
void odf_chart_context::set_chart_bar_grouping(int type)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	switch(type)
	{
		case 0:	//	st_groupingPERCENTSTACKED = 0,
			impl_->current_level_.back().chart_properties_->content().chart_percentage_ = true; break;
		case 1:	//	st_bargroupingCLUSTERED = 1,
		case 2:	//	st_bargroupingSTANDARD = 2,
			break;
		case 3:	//	st_bargroupingSTACKED = 3
			impl_->current_level_.back().chart_properties_->content().chart_stacked_ = true; break;
	}
}
void odf_chart_context::set_chart_grouping(int type)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	switch(type)
	{
		case 0:	//	st_groupingPERCENTSTACKED = 0,
			impl_->current_level_.back().chart_properties_->content().chart_percentage_ = true; break;
		case 1:	//	st_groupingSTANDARD = 1,
			break;
		case 2:	//	st_groupingSTACKED = 2
			impl_->current_level_.back().chart_properties_->content().chart_stacked_ = true; break;
	}
}
void odf_chart_context::set_chart_3D(bool val)
{
	if (!impl_->current_level_.back().chart_properties_) return;
	impl_->current_level_.back().chart_properties_->content().chart_three_dimensional_ = val;

	impl_->current_level_.back().chart_properties_->content().chart_treat_empty_cells_ = boost::none;
	//impl_->current_level_.back().chart_properties_->content().chart_series_source_ = chart_series_source(chart_series_source::rows);

	//impl_->current_level_.back().chart_properties_->content().
	//chart:treat-empty-cells="leave-gap" 
	//chart:series-source="rows"
		  //impl_->current_level_.back().chart_properties_->content().chart_deep_ = true;
	//chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(current_level_.back().elm.get());
	//if (!plot_area)return;

	//plot_area->chart_plot_area_attlist_.dr3d_shade_mode=L"gouraud";
}
void odf_chart_context::set_chart_colored(bool val)
{
	if (!impl_->current_level_.back().chart_properties_) return;
	impl_->current_level_.back().chart_properties_->content().chart_three_dimensional_ = val;
}
void odf_chart_context::set_marker_size(int size)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	impl_->current_level_.back().chart_properties_->content().chart_symbol_width_ = length(size,length::pt);
	impl_->current_level_.back().chart_properties_->content().chart_symbol_height_ = length(size,length::pt);
}
void odf_chart_context::set_marker_type(int type)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	impl_->current_level_.back().chart_properties_->content().chart_symbol_type_ = chart_symbol_type(chart_symbol_type::namedSymbol);

	switch(type)
	{
	case 0://st_markerstyleCIRCLE = 0,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::circleSymbol);	break;
	case 1://st_markerstyleDASH = 1,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::horizontal_barSymbol);	break;
	case 2://st_markerstyleDIAMOND = 2,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::diamondSymbol);	break;
	case 3://st_markerstyleDOT = 3,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::circleSymbol);	break;
	case 4://st_markerstyleNONE = 4,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_type_ = chart_symbol_type(chart_symbol_type::noneSymbol);	break;
	case 5://st_markerstylePICTURE = 5,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::asteriskSymbol);	break;
	case 6://st_markerstylePLUS = 6,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::plusSymbol);	break;
	case 7://st_markerstyleSQUARE = 7,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::squareSymbol);	break;
	case 8://st_markerstyleSTAR = 8,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::starSymbol);	break;
	case 9://st_markerstyleTRIANGLE = 9,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::arrow_upSymbol);	break;
	case 10://st_markerstyleX = 10,
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::xSymbol);	break;
	case 11://st_markerstyleAUTO = 11
		impl_->current_level_.back().chart_properties_->content().chart_symbol_name_ = chart_symbol_name(chart_symbol_name::autoSymbol);	break;
	default:
		impl_->current_level_.back().chart_properties_->content().chart_symbol_type_ = chart_symbol_type(chart_symbol_type::autoSymbol);
	}

}
void odf_chart_context::set_chart_scatter_type(int type)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	switch(type)
	{
	case 0://st_scatterstyleNONE
		break;
	case 1://st_scatterstyleLINE
		impl_->current_level_.back().chart_properties_->content().chart_symbol_type_ = chart_symbol_type(chart_symbol_type::noneSymbol);
		break;
	case 2://st_scatterstyleLINEMARKER
		impl_->current_level_.back().chart_properties_->content().chart_symbol_type_ = chart_symbol_type(chart_symbol_type::autoSymbol);
		break;
	case 3://st_scatterstyleMARKER
		impl_->current_level_.back().chart_properties_->content().chart_symbol_type_ = chart_symbol_type(chart_symbol_type::autoSymbol);
		break;	
	case 4://st_scatterstyleSMOOTH
		impl_->current_level_.back().chart_properties_->content().chart_symbol_type_ = chart_symbol_type(chart_symbol_type::noneSymbol);
		impl_->current_level_.back().chart_properties_->content().chart_interpolation_ = chart_interpolation(chart_interpolation::cubicSpline);
		break;	
	case 5://st_scatterstyleSMOOTHMARKER
		impl_->current_level_.back().chart_properties_->content().chart_interpolation_ = chart_interpolation(chart_interpolation::cubicSpline);
		impl_->current_level_.back().chart_properties_->content().chart_symbol_type_ = chart_symbol_type(chart_symbol_type::autoSymbol);
		break;	
	}
}
void odf_chart_context::start_group_series()
{
}
void odf_chart_context::start_series(std::wstring type)
{
	office_element_ptr elm;
	create_element(L"chart", L"series", elm, impl_->odf_context_);
	
	chart_series *series = dynamic_cast<chart_series*>(elm.get());
	if (series == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		series->chart_series_attlist_.common_attlist_.chart_style_name_ = style_name;
		series->chart_series_attlist_.chart_class_ = std::wstring(L"chart:") + type;

	}
	start_element(elm, style_elm, style_name);

	impl_->group_series_.push_back(elm);
//////////////////////////////////////////////////////////////
	chart_chart * chart = impl_->get_current_chart();
	if (chart) 
	{
		//может хранить отдельно общий класс чарта??
		if (type == L"radar" || (*chart->chart_chart_attlist_.chart_class_ == L"chart:stock" && type == L"line"))
			series->chart_series_attlist_.chart_class_ = chart->chart_chart_attlist_.chart_class_;	
	}

	if (style_)
	{
		impl_->current_level_.back().graphic_properties_ = style_->style_content_.get_style_graphic_properties();
		impl_->set_default_series_color();
	}

	impl_->current_series_count_ ++;
}
void odf_chart_context::end_series()
{
	if ((impl_->categories_.size() > 0) && impl_->categories_.back().second == 2)
	{
		add_domain(impl_->categories_.back().first);
	}
	end_element();
}
void odf_chart_context::set_label_delete(bool val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content().chart_display_label_ = !val;
}
void odf_chart_context::set_label_show_bubble_size(bool val)
{
}
void odf_chart_context::set_label_show_cat_name(bool val)
{
}
void odf_chart_context::set_label_show_leader_line(bool val)
{
}
void odf_chart_context::set_label_show_legend_key(bool val)
{
}
void odf_chart_context::set_label_show_percent(bool val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	//impl_->current_level_.back().chart_properties_->content().chart_percentage_ = val;
}
void odf_chart_context::set_label_show_ser_name(bool val)
{
}
void odf_chart_context::set_label_show_values(bool val)
{
	if (!impl_->current_level_.back().chart_properties_ || !val)return;
	impl_->current_level_.back().chart_properties_->content().chart_data_label_number_=chart_data_label_number(chart_data_label_number::value);
}
void odf_chart_context::add_axis_group_series(unsigned int id)
{
	impl_->axis_group_series_.push_back(id);
}
void odf_chart_context::end_group_series()
{
	if (impl_->axis_.size() < 1 && impl_->categories_.size() > 0)
	{//без осей нихера не понимает MS Office !!! - причем оси для MS должны идти обязательно перед сериями
		start_axis();
			set_axis_dimension(1);
		end_element();
		start_axis();
			set_axis_dimension(2);
		end_element();
	}

	std::wstring axis_name;

	bool presentZ = false;
	long countX = 0;
	long countY = 0;
	for (long j = 0; j < impl_->axis_.size(); j++)
	{
		if (impl_->axis_[j].dimension ==1)		countX++;
		else if (impl_->axis_[j].dimension ==3)	presentZ = true;
		else countY++;
	}
	if (countX < 1 && countY > 1)
	{
		impl_->axis_[0].dimension == 1;
		chart_axis *axis = dynamic_cast<chart_axis*>(impl_->axis_[0].elm.get());
		axis->chart_axis_attlist_.chart_dimension_ = L"x";
		countY--;
	}
	if (presentZ == false && impl_->axis_group_series_.size() == 3 && (countY > 1 || countX > 1))
	{
		impl_->axis_.back().dimension == 3;
		chart_axis *axis = dynamic_cast<chart_axis*>(impl_->axis_.back().elm.get());
		axis->chart_axis_attlist_.chart_dimension_ = L"z";
		countY--;
	}

	for (long i=0; i < impl_->axis_group_series_.size(); i++)
	{
		for (long j = 0; j < impl_->axis_.size(); j++)
		{
			if (impl_->axis_[j].oox_id == impl_->axis_group_series_[i] && impl_->axis_[j].dimension ==2)
			{
				axis_name = impl_->axis_[j].name;//привязка оси Y
				break;
			}
		}
		if (axis_name.length() > 0)break;
	}


	for (long i =0; i < impl_->group_series_.size() && axis_name.length() > 0; i++)
	{
		chart_series *series= dynamic_cast<chart_series*>(impl_->group_series_[i].get());
		if (series)
		{
			series->chart_series_attlist_.chart_attached_axis_ = axis_name;
		}		
	}
	impl_->group_series_.clear();
	impl_->axis_group_series_.clear();
}
void odf_chart_context::add_domain(std::wstring formula)
{
	int level = impl_->current_level_.size();
	if (level < 1)return;

	office_element_ptr elm;
	create_element(L"chart", L"domain", elm, impl_->odf_context_);
		
	chart_domain *domain = dynamic_cast<chart_domain*>(elm.get());
	if (domain == NULL)return;

	domain->table_cell_range_address_ = formula;
	
	if (impl_->current_level_.back().elm)impl_->current_level_.back().elm->add_child_element(elm);

	odf_element_state state={elm, L"",office_element_ptr(), level};
	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::add_categories(std::wstring formula, office_element_ptr & axis)
{
	office_element_ptr elm;
	create_element(L"chart", L"categories", elm, impl_->odf_context_);
		
	chart_categories *categories = dynamic_cast<chart_categories*>(elm.get());
	if (categories== NULL)return;

	categories->table_cell_range_address_ = formula;
	int level = impl_->current_level_.size();
	
	if (axis)axis->add_child_element(elm);

	odf_element_state state={elm, L"",office_element_ptr(), level};
	impl_->current_chart_state_.elements_.push_back(state);
}

void odf_chart_context::start_axis()
{
	office_element_ptr elm;
	create_element(L"chart", L"axis", elm, impl_->odf_context_);
	
	chart_axis *axis = dynamic_cast<chart_axis*>(elm.get());
	if (axis == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		axis->chart_axis_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);

	odf_axis_state axis_state={0,0,L"",elm};
	impl_->axis_.push_back(axis_state);
/////////////////////defaults
	impl_->current_level_.back().chart_properties_->content().chart_reverse_direction_ = false;
}
void odf_chart_context::start_grid(int type)
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"grid", chart_elm, impl_->odf_context_);
	
	chart_grid *grid = dynamic_cast<chart_grid*>(chart_elm.get());
	if (grid == NULL)return;
//////////	
	if (type == 1) grid->chart_grid_attlist_.chart_class_ = L"major";
	if (type == 2) grid->chart_grid_attlist_.chart_class_ = L"minor";

	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;

		grid->chart_grid_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
}

void odf_chart_context::start_title()
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"title", chart_elm, impl_->odf_context_);
	
	chart_title *title = dynamic_cast<chart_title*>(chart_elm.get());
	if (title == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		title->chart_title_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
}
void odf_chart_context::start_plot_area()
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"plot-area", chart_elm, impl_->odf_context_);
	
	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(chart_elm.get());
	if (plot_area == NULL)return;

	plot_area->chart_plot_area_attlist_.chart_data_source_has_labels_ = L"both";
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		plot_area->chart_plot_area_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
	
	if (!impl_->current_level_.back().chart_properties_) return;
	
	impl_->current_level_.back().chart_properties_->content().chart_treat_empty_cells_ = true;
}
void odf_chart_context::end_plot_area()
{
	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(impl_->current_level_.back().elm.get());
	if (plot_area)
	{
		std::wstring cell_range;
		for (long i=0; i< impl_->data_cell_ranges_.size();i++)
		{
			cell_range = cell_range + impl_->data_cell_ranges_[i].ref + L" ";
		}
		//plot_area->chart_plot_area_attlist_.table_cell_range_address_ = cell_range; - точно для локальной диагр это лишнее!!!
	}
	end_element();
}
void odf_chart_context::start_text()
{
	impl_->odf_context_->start_text_context();
	impl_->odf_context_->text_context()->set_styles_context(impl_->styles_context_);

	style_text_properties *text_props = NULL;

	style *style_ = dynamic_cast<style*>(impl_->current_chart_state_.elements_.back().style_elm.get());
	if (style_)
	{
		impl_->current_level_.back().paragraph_properties_ = style_->style_content_.get_style_paragraph_properties();
		impl_->current_level_.back().text_properties_ = style_->style_content_.get_style_text_properties();
	}
	
	impl_->odf_context_->text_context()->set_single_object(true,impl_->current_level_.back().paragraph_properties_,impl_->current_level_.back().text_properties_);
}
void odf_chart_context::end_text()
{
	odf_text_context * text_context_ = text_context();
	if (text_context_ == NULL || impl_->current_level_.size() <1 )return;

	for (long i=0; i< text_context_->text_elements_list_.size(); i++)
	{
		if (text_context_->text_elements_list_[i].level ==0)
		{
			impl_->current_level_.back().elm->add_child_element(text_context_->text_elements_list_[i].elm);
		}
		int level_root = impl_->current_level_.size() + 1;
		
		odf_element_state state={text_context_->text_elements_list_[i].elm, 
								text_context_->text_elements_list_[i].style_name, 
								text_context_->text_elements_list_[i].style_elm, 
								text_context_->text_elements_list_[i].level + level_root};

		impl_->current_chart_state_.elements_.push_back(state);
	}

	impl_->odf_context_->end_text_context();
}
void odf_chart_context::start_floor()
{
	office_element_ptr elm;
	create_element(L"chart", L"floor", elm, impl_->odf_context_);
	
	chart_floor *floor = dynamic_cast<chart_floor*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && floor)
	{
		style_name = style_->style_name_;
		floor->common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);

	if (style_)
	{
		impl_->current_level_.back().graphic_properties_ = style_->style_content_.get_style_graphic_properties();
	}
}
void odf_chart_context::start_wall()
{
	office_element_ptr elm;
	create_element(L"chart", L"wall", elm, impl_->odf_context_);
	
	chart_wall *wall = dynamic_cast<chart_wall*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && wall)
	{
		style_name = style_->style_name_;
		wall->chart_wall_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);

	if (style_)
	{
		impl_->current_level_.back().graphic_properties_ = style_->style_content_.get_style_graphic_properties();
		if (impl_->current_level_.back().graphic_properties_)
			impl_->current_level_.back().graphic_properties_->content().common_draw_fill_attlist_.draw_fill_color_ = color(L"#ffffff");
	}
}
void odf_chart_context::start_legend()
{
	office_element_ptr elm;
	create_element(L"chart", L"legend", elm, impl_->odf_context_);
	
	chart_legend *legend = dynamic_cast<chart_legend*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && legend)
	{
		style_name = style_->style_name_;
		legend->chart_legend_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);
}
void odf_chart_context::start_stock_range_line()
{
	office_element_ptr elm;
	create_element(L"chart", L"stock-range-line", elm, impl_->odf_context_);
	
	chart_stock_range_line *line = dynamic_cast<chart_stock_range_line*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && line)
	{
		style_name = style_->style_name_;
		line->common_attlist_.chart_style_name_ = style_name;
		
		impl_->current_level_.back().graphic_properties_ = style_->style_content_.get_style_graphic_properties();
		impl_->set_default_series_color();
	}
	start_element(elm, style_elm, style_name);
}
void odf_chart_context::start_stock_gain_marker()
{
	office_element_ptr elm;
	create_element(L"chart", L"stock-gain-marker", elm, impl_->odf_context_);
	
	chart_stock_gain_marker *marker = dynamic_cast<chart_stock_gain_marker*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && marker)
	{
		style_name = style_->style_name_;
		marker->common_attlist_.chart_style_name_ = style_name;
		
		impl_->current_level_.back().graphic_properties_ = style_->style_content_.get_style_graphic_properties();
		impl_->set_default_series_color();
	}
	start_element(elm, style_elm, style_name);
}
void odf_chart_context::start_stock_loss_marker()
{
	office_element_ptr elm;
	create_element(L"chart", L"stock-loss-marker", elm, impl_->odf_context_);
	
	chart_stock_loss_marker *marker = dynamic_cast<chart_stock_loss_marker*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && marker)
	{
		style_name = style_->style_name_;
		marker->common_attlist_.chart_style_name_ = style_name;
	
		impl_->current_level_.back().graphic_properties_ = style_->style_content_.get_style_graphic_properties();
		impl_->set_default_series_color();	
	}
	start_element(elm, style_elm, style_name);
}
void odf_chart_context::set_stock_gain_marker_width(std::wstring val)
{
	int res = val.find(L"%");

	bool percent=false;
	if (res > 0)
	{
		val = val.substr(0,res);
		percent=true;
	}
	double dVal = boost::lexical_cast<double>(val);
}
void odf_chart_context::set_stock_loss_marker_width(std::wstring val)
{
	int res = val.find(L"%");

	bool percent=false;
	if (res > 0)
	{
		val = val.substr(0,res);
		percent=true;
	}
	double dVal = boost::lexical_cast<double>(val);
}
long odf_chart_context::get_count_data_points_series()
{
	return impl_->current_data_points_series_count_;
}
void odf_chart_context::start_data_point_series(int count)
{
	office_element_ptr elm;
	create_element(L"chart", L"data-point", elm, impl_->odf_context_);
	
	chart_data_point *data_point = dynamic_cast<chart_data_point*>(elm.get());
	if (data_point == NULL)return;

	if (count < 1) count = get_count_data_points_series();
	data_point->chart_data_point_attlist_.chart_repeated_ = count;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		data_point->chart_data_point_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);
//defaults
	chart_series * series = impl_->get_current_series();
}
void odf_chart_context::set_legend_position(int val)
{
	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend == NULL)return;
	switch(val)
	{
		case 0: legend->chart_legend_attlist_.chart_legend_position_ = L"bottom";	break;//st_legendposB
		case 1: legend->chart_legend_attlist_.chart_legend_position_ = L"top-end";	break;//st_legendposTR 
		case 2: legend->chart_legend_attlist_.chart_legend_position_ = L"start";	break;//st_legendposL 
		case 3: legend->chart_legend_attlist_.chart_legend_position_ = L"end";		break;//st_legendposR
		case 4:	legend->chart_legend_attlist_.chart_legend_position_ = L"top";		break;//st_legendposT
	}
}
void odf_chart_context::set_layout_x(double *val,int mode)//edge, factor
{
	if (!val)return;
	if (mode == 0) *val = impl_->current_chart_state_.chart_width_pt * (*val);

	length x_cm = length(length(*val,length::pt).get_value_unit(length::cm),length::cm);

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.common_draw_position_attlist_.svg_x_ = x_cm;

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_position_attlist_.svg_x_ = x_cm;
	
	chart_title *title = dynamic_cast<chart_title*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (title)title->chart_title_attlist_.common_draw_position_attlist_.svg_x_ = x_cm;
}
void odf_chart_context::set_display_label(bool Val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content().chart_display_label_ =  Val;
}
void odf_chart_context::set_display_label_position(int type)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	//impl_->current_level_.back().chart_properties_->content().chart:label-arrangement_ =  Val;
}
void odf_chart_context::set_axis_orientation(int type)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	if (type == 0) impl_->current_level_.back().chart_properties_->content().chart_reverse_direction_ =  true;
}
void odf_chart_context::set_axis_max(double val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content().chart_maximum_ =  val;
}
void odf_chart_context::set_axis_min(double val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content().chart_minimum_ =  val;
}
void odf_chart_context::set_axis_tick_minor(int type)
{
	if (!impl_->current_level_.back().chart_properties_)return;

	switch (type)
	{
		case 0: break;//		st_tickmarkCROSS = 0,		
		case 1: //		st_tickmarkIN = 1,
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_minor_inner_ = true;	
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_minor_outer_ = false; break;
		case 2: //		st_tickmarkNONE = 2,
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_minor_inner_ = false;	
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_minor_outer_ = false; break;
		case 3: //		st_tickmarkOUT = 3
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_minor_inner_ = false;	
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_minor_outer_ = true; break;
	}

}
void odf_chart_context::set_no_fill(bool Val)
{
	if (Val == false)return;
	if (!impl_->current_level_.back().graphic_properties_)return;

	impl_->current_level_.back().graphic_properties_->content().common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::none);

}
void odf_chart_context::set_axis_tick_major(int type)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	switch (type)
	{
		case 0: break;//		st_tickmarkCROSS = 0,		
		case 1: //		st_tickmarkIN = 1,
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_major_inner_ = true;	
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_major_outer_ = false; break;
		case 2: //		st_tickmarkNONE = 2,
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_major_inner_ = false;	
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_major_outer_ = false; break;
		case 3: //		st_tickmarkOUT = 3
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_major_inner_ = false;	
		impl_->current_level_.back().chart_properties_->content().chart_tick_marks_major_outer_ = true; break;
	}
}
void odf_chart_context::set_axis_logarithmic(bool val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content().chart_logarithmic_ =  val;
}
void odf_chart_context::set_axis_id(unsigned int id)
{
	if (impl_->axis_.size()>0)impl_->axis_.back().oox_id = id;
}
void odf_chart_context::set_axis_dimension(int type)
{
	chart_axis *axis = impl_->get_current_axis();
	if (!axis)return;

	std::wstring val;
	if (type == 1) val = L"x";
	if (type == 2) val = L"y";
	if (type == 3) val = L"z";
	
	axis->chart_axis_attlist_.chart_dimension_ = val;
	//axis->chart_axis_attlist_.chart_name_ = std::wstring(L"axis-") + boost::lexical_cast<std::wstring>(impl_->axis_.size()+1); 

	if (impl_->axis_.size()>0)
	{
		impl_->axis_.back().dimension = type;
		//impl_->axis_.back().name = *axis->chart_axis_attlist_.chart_name_;
	}
}
void odf_chart_context::set_axis_position(int type)
{	
	if (!impl_->current_level_.back().chart_properties_)return;
			//st_axposB = 0,
			//st_axposL = 1,
			//st_axposR = 2,
			//st_axposT = 3
	//if (type ==0 || type ==2)
	//	impl_->current_level_.back().chart_properties_->content().chart_axis_position_ = L"end";
	//if (type ==1 || type ==3)
	//	impl_->current_level_.back().chart_properties_->content().chart_axis_position_ = L"start";
//a value of type double – the axis line is placed at the given value on the crossing axis. 
//If the crossing axis is an axis displaying categories rather than values, a value of 1 indicates that the axis should be placed at the first category, a value of 2 indicates that the axis should be placed at the second category and so forth.
}
void odf_chart_context::set_axis_label_position(int type)
{	
	if (!impl_->current_level_.back().chart_properties_)return;
			//st_ticklblposHIGH = 0,
			//st_ticklblposLOW = 1,
			//st_ticklblposNEXTTO = 2,
			//st_ticklblposNONE = 3
	if (type ==1)impl_->current_level_.back().chart_properties_->content().chart_axis_label_position_ = L"outside-start";
	if (type ==0)impl_->current_level_.back().chart_properties_->content().chart_axis_label_position_ = L"outside-end";
	if (type ==2)impl_->current_level_.back().chart_properties_->content().chart_axis_label_position_ = L"near-axis";

	//near-axis-other-side
	//near-axis //default

}
void odf_chart_context::set_layout_y(double *val,int mode)
{
	if (!val)return;
	if (mode == 0)*val = impl_->current_chart_state_.chart_height_pt * (*val);

	length y_cm = length(length(*val,length::pt).get_value_unit(length::cm),length::cm);

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.common_draw_position_attlist_.svg_y_ = y_cm;

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_position_attlist_.svg_y_ = y_cm;
	
	chart_title *title = dynamic_cast<chart_title*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (title)title->chart_title_attlist_.common_draw_position_attlist_.svg_y_ = y_cm;
}
void odf_chart_context::set_layout_w(double *val,int mode)
{
	if (!val)return;
	if (mode == 0) *val = impl_->current_chart_state_.chart_width_pt * (*val);
	
	length width_cm = length(length(*val,length::pt).get_value_unit(length::cm),length::cm);

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.chartooo_width_ = width_cm;

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_size_attlist_.svg_width_ = width_cm;
}
void odf_chart_context::set_layout_h(double *val,int mode)
{
	if (!val)return;
	if (mode == 0) *val = impl_->current_chart_state_.chart_height_pt * (*val);

	length height_cm = length(length(*val,length::pt).get_value_unit(length::cm),length::cm);

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.chartooo_height_ = height_cm;

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_size_attlist_.svg_height_ = height_cm;
}

void odf_chart_context::start_element(office_element_ptr & elm, office_element_ptr & style_elm, std::wstring style_name)
{
	int level = impl_->current_level_.size();
	
	drawing_context()->start_element(elm, style_elm);
	//if (impl_->current_level_.size()>0) impl_->current_level_.back()->add_child_element(elm); не надо...наследование через start_element в drawing
	
	odf_element_state		state={elm, style_name, style_elm, level};
	odf_chart_level_state	level_state = {NULL,NULL,NULL,NULL,elm};
	
	impl_->current_chart_state_.elements_.push_back(state);
	
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		level_state.chart_properties_ = style_->style_content_.get_style_chart_properties();
	}
	impl_->current_level_.push_back(level_state);//стоит ли сюда перенести и current_chart_properties ????

}

void odf_chart_context::end_element()
{
	//допричесываение элемента
	//if (impl_->current_level_.size()>0)
	{
		if (impl_->current_level_.back().paragraph_properties_)
		{
			if (impl_->current_level_.back().paragraph_properties_->content().style_writing_mode_)
			{
				switch(impl_->current_level_.back().paragraph_properties_->content().style_writing_mode_->get_type())
				{
					case writing_mode::LrTb: 
					case writing_mode::RlTb:
					case writing_mode::Lr:
						impl_->current_level_.back().chart_properties_->content().style_direction_ = direction(direction::Ltr); break;
					case writing_mode::TbRl:
					case writing_mode::TbLr:
					case writing_mode::Tb:
						impl_->current_level_.back().chart_properties_->content().style_direction_ = direction(direction::Ttb); break;
				}
				impl_->current_level_.back().chart_properties_->content().common_rotation_angle_attlist_.style_rotation_angle_ =0;
			}
		}
	}
	impl_->current_level_.pop_back();
	drawing_context()->end_element();
}

void odf_chart_context::end_chart()
{
	if (impl_->current_chart_state_.elements_.size() < 1) return;

	impl_->create_local_table();
	
	end_element();
///////////////////


	for (long i=0; i< impl_->axis_.size() && impl_->categories_.size() > 0; i++)
	{
		if (impl_->axis_[i].elm == NULL)continue;
		if (impl_->categories_[0].second == 1) 
		{
			add_categories(impl_->categories_[0].first,impl_->axis_[i].elm);
		}
		else
		{
			if (i==0) 
			{
				chart_axis *axis = dynamic_cast<chart_axis*>(impl_->axis_[i].elm.get());
				axis->chart_axis_attlist_.chart_dimension_ = L"x";
			}
		}
	}
///////////////
	impl_->chart_list_.push_back(impl_->current_chart_state_);

	impl_->clear_current();
}
static formulasconvert::oox2odf_converter formulas_converter;

void odf_chart_context::set_series_value_formula(std::wstring oox_formula)
{
	std::wstring odfFormula;
	
	if (oox_formula.length() > 0)
		odfFormula = formulas_converter.convert_chart_distance(oox_formula);

	chart_series *series = dynamic_cast<chart_series*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (series == NULL)return;

	Impl::_range r = {odfFormula, false};
	impl_->data_cell_ranges_.push_back(r);

	if (oox_formula.length() > 0)
	{
		series->chart_series_attlist_.chart_values_cell_range_address_ = odfFormula;
		impl_->current_data_points_series_count_ = formulas_converter.get_count_value_points(oox_formula);
	}
}
void odf_chart_context::set_series_label_formula(std::wstring oox_formula)
{
	std::wstring odfFormula;

	if (oox_formula.length() > 0)
		odfFormula = formulas_converter.convert_chart_distance(oox_formula);

	Impl::_range r = {odfFormula, true};
	impl_->data_cell_ranges_.push_back(r);
	
	if (odfFormula.length() > 0)
	{
		chart_series *series = dynamic_cast<chart_series*>(impl_->current_chart_state_.elements_.back().elm.get());
		if (series == NULL)return;
		series->chart_series_attlist_.chart_label_cell_address_ = odfFormula;
	}
}

void odf_chart_context::set_category_axis_formula(std::wstring oox_formula,int type)
{
	std::wstring odfFormula;
	
	if (oox_formula.length() > 0)
		odfFormula = formulas_converter.convert_chart_distance(oox_formula);

	Impl::_range r = {odfFormula, true};
	impl_->data_cell_ranges_.push_back(r);
	
	impl_->categories_.push_back(std::pair<std::wstring,int>(odfFormula,type));
}

void odf_chart_context::set_series_pie_explosion(int val)//или точка серии
{
	if (!impl_->current_level_.back().chart_properties_)return;

	impl_->current_level_.back().chart_properties_->content().chart_pie_offset_ = val;	
}
//void odf_chart_context::set_cash(std::wstring format, std::vector<double> &data_double)
//{
//	if (data_double.size() <1 || impl_->data_cell_ranges_.size() < 1) return;
//
//	std::wstring ref = impl_->data_cell_ranges_.back();
//	std::vector<std::wstring> data_str;
//	
//	odf_cash_state state = {ref, format,/*data_double,*/data_str};
//	impl_->cash_.push_back(state);
//}

void odf_chart_context::set_cash(std::wstring format, std::vector<std::wstring> & data_str, bool label)
{
	if (data_str.size() <1) return;
	if (impl_->data_cell_ranges_.size()<1) return;

	std::wstring ref = impl_->data_cell_ranges_.back().ref;

	if (ref.length() < 1)
	{
		std::wstring col;
		if (label) col = L"A";
		else
		{
			int curr_col = 0;
			for (long i=0; i<impl_->data_cell_ranges_.size(); i++)
			{
				if (impl_->data_cell_ranges_[i].label)continue;
				curr_col++;
			}
			col = utils::getColAddress(curr_col);
		}
		ref = std::wstring(L"local-table.$") + col + std::wstring(L"$") + boost::lexical_cast<std::wstring>(2) + L":.$" + col + std::wstring(L"$") + boost::lexical_cast<std::wstring>(data_str.size()+1);

		impl_->data_cell_ranges_.back().ref = ref;

		if (label == false)
		{
			chart_series *series = dynamic_cast<chart_series*>(impl_->current_chart_state_.elements_.back().elm.get());
			if (series == NULL)return;
			series->chart_series_attlist_.chart_values_cell_range_address_ = ref;
		}
		else
			impl_->categories_.back().first = ref;
	}
	std::vector<double> data_double;

	odf_cash_state state = {ref, format/*,data_double*/,label,data_str};
	
	impl_->cash_.push_back(state);
}

void odf_chart_context::set_local_table (bool Val)
{
	if (Val == false) return;

	impl_->local_table_enabled_ = Val;
}

struct _sort_cells
{
	bool operator() (_cell_cash i, _cell_cash j)
	{ 
		if (i.row == j.row)
			return (i.col<j.col);
		else
			return (i.row<j.row);
	}
} sort_cells;

void odf_chart_context::Impl::create_local_table_rows(ods_table_state * table_state,std::vector<_cell_cash> & cells, bool header )
{
	int curr_row = 0;
	int curr_cell = 0;

	for (long i = 0; i< cells.size(); i++)
	{
		if (cells[i].row  > curr_row+1)
		{	
			office_element_ptr row_elm;
			create_element(L"table", L"table-row",row_elm, odf_context_);
			table_state->add_row(row_elm,cells[i].row - curr_row -2,office_element_ptr());
			curr_row =  cells[i].row-1;
		}
		if (cells[i].row == curr_row+1)
		{
			if (cells[i].label == header)
			{
				office_element_ptr row_elm;

				create_element(L"table", L"table-row",row_elm, odf_context_);
				table_state->add_row(row_elm,1 ,office_element_ptr());
			}
			curr_row++;

			curr_cell=0;
		}
		if (cells[i].label == !header)continue;

		if (curr_cell+1 < cells[i].col)
			table_state->add_default_cell(cells[i].col - curr_cell-1);
		
		office_element_ptr cell_elm;
		create_element(L"table", L"table-cell",cell_elm, odf_context_);
		
		table_state->start_cell(cell_elm,office_element_ptr());
			table_state->set_cell_value(cells[i].val, true);
			//add type ???
		table_state->end_cell();

		curr_cell = cells[i].col;
	}
}
void odf_chart_context::Impl::create_local_table()
{
	if (local_table_enabled_ == false)return;

	std::vector<_cell_cash> cells_cash;
	std::vector<_cell_cash> cells_cash_label;

	std::wstring table_name = L"local-table";
	int max_columns=0;

	bool col_header = false;
	bool row_header = false;
	
	//выкинем дублирующие ref
	for (long i=0; i < cash_.size(); i++)
	{
		for (long j=i+1; j < cash_.size(); j++)
		{
			if (cash_[j].ref == cash_[i].ref && cash_[j].ref.length() > 1)
			{
				cash_.erase(cash_.begin()+j);
			}
		}
	}

	for (long i=0; i < cash_.size(); i++)
	{
		std::vector<std::wstring> refs;
		boost::algorithm::split(refs,cash_[i].ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

		__int32 col1,col2,row1,row2;

		if (refs.size()<1) continue;
		int r = refs[0].find(L".");
		if (r>0)
		{
			table_name = refs[0].substr (0,r);
			refs[0] = refs[0].substr(r+1,refs[0].size()-r);
		}

		utils::parsing_ref( refs[0],col1,row1);
		
		if (refs.size()>1) 
		{
			r = refs[1].find(L".");
			if (r>=0)refs[1] = refs[1].substr(r+1,refs[1].size()-r);
			utils::parsing_ref( refs[1],col2,row2);
		}
		else
		{
			col2=col1;row2=row1;
		}
		for (long j=0;j<cash_[i].data_str.size(); j++)
		{
			_cell_cash c;
			c.col = (col2==col1) ? col1 : col1+j;
			c.row = (row2==row1) ? row1 : row1+j;
			c.val = cash_[i].data_str[j];
			c.label = false;

			if (cash_[i].label && c.row == 1)
			{
				c.label = cash_[i].label;
				row_header = true;
			}
			if (cash_[i].label && c.col == 1)
			{
				col_header = true;
			}
			cells_cash.push_back(c);

			if (c.label) cells_cash_label.push_back(c);
			//else cells_cash.push_back(c);

			if (c.col > max_columns) max_columns = c.col;
		}
	}

	std::sort(cells_cash.begin(), cells_cash.end(),sort_cells);
	std::sort(cells_cash_label.begin(), cells_cash_label.end(),sort_cells);

/////////////////////////
	//create tables

	office_element_ptr table_elm;
	create_element(L"table", L"table",table_elm, odf_context_);
	ods_table_state * table_state = new ods_table_state(odf_context_, table_elm);

	if (table_state)
	{
		current_level_[0].elm->add_child_element(table_elm);
		int level = current_level_.size();
		odf_element_state		state={table_elm, L"", office_element_ptr(), level+1};		
		current_chart_state_.elements_.push_back(state);

		table_state->set_table_name(table_name);

		/////////////////////////////////////////////////
		office_element_ptr col_elm;
		create_element(L"table", L"table-column",col_elm, odf_context_);

		if (col_header)
		{
			office_element_ptr cols_header_elm;
			create_element(L"table", L"table-header-columns",cols_header_elm, odf_context_);
			table_elm->add_child_element(cols_header_elm);	
				cols_header_elm->add_child_element(col_elm);
		}

		office_element_ptr cols_elm;
		create_element(L"table", L"table-columns",cols_elm, odf_context_);
		table_elm->add_child_element(cols_elm);


		for (long i=0; i < max_columns - (col_header ? 1 : 0); i++)
			cols_elm->add_child_element(col_elm);

		office_element_ptr row_headers_elm;
		office_element_ptr row_elm;

		if (cells_cash_label.size() > 0 || cells_cash.size() > 0)
		{
			if (cells_cash_label.size() > 0 && row_header)
			{
				create_element(L"table", L"table-header-rows",row_headers_elm, odf_context_);
				
				table_state->start_headers(row_headers_elm);
					create_local_table_rows(table_state, cells_cash_label ,true);
				table_state->end_headers();
			}
			if (cells_cash.size() > 0) create_local_table_rows(table_state, cells_cash ,false);
			else 
			{
				create_element(L"table", L"table-rows",row_elm, odf_context_);
				table_state->add_row(row_elm,1,office_element_ptr());
			}
		}
		else
		{
			create_element(L"table", L"table-header-rows",row_headers_elm, odf_context_);
			
			table_state->start_headers(row_headers_elm);
			{
				create_element(L"table", L"table-row",row_elm, odf_context_);
				table_state->add_row(row_elm,1,office_element_ptr());
				{
					office_element_ptr cell_elm;
					create_element(L"table", L"table-cell",cell_elm, odf_context_);
					
					table_state->start_cell(cell_elm,office_element_ptr());
					table_state->end_cell();
				}
			}
			table_state->end_headers();

			create_element(L"table", L"table-rows",row_elm, odf_context_);
			table_state->add_row(row_elm,1,office_element_ptr());
		}
	}

	delete table_state;

}

}
}