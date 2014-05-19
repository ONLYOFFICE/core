#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>

#include <cpdoccore/formulasconvert.h>

#include <iostream>

#include "odf_chart_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_drawing_context.h"

#include "odf_conversion_context.h"

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
	struct 	odf_element_state
	{
		office_element_ptr	elm;
		std::wstring		style_name;

		office_element_ptr	style_elm;
		
		int level;
	};
	struct 	odf_chart_state
	{
		odf_chart_state() {clear();}
		void clear()
		{
			elements_.clear();
			chart_width_pt = chart_height_pt = 0;
			categories_.clear();

			axis_id=0;
		}

		double chart_width_pt;
		double chart_height_pt;

		std::vector<std::wstring> categories_;

		std::vector<odf_element_state> elements_;

		int axis_id;
	};
class odf_chart_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		styles_context_ = NULL;
		
		current_text_properties_ = NULL;
		current_paragraph_properties_ = NULL;
		current_graphic_properties_ = NULL;
		current_chart_properties_ = NULL;
	} 
	odf_chart_state				current_chart_state_;

	std::vector<office_element_ptr>		current_level_;	//постоянно меняющийся список уровней наследования
	std::vector<odf_chart_state>		chart_list_;		//все элементы .. для удобства разделение по "топам"

	odf_style_context			*styles_context_;
	odf_conversion_context		*odf_context_;

	style_text_properties		*current_text_properties_;
	style_graphic_properties	*current_graphic_properties_;
	style_paragraph_properties	*current_paragraph_properties_;
	style_chart_properties		*current_chart_properties_;

	chart_chart					*get_current_chart();
	chart_axis					*get_current_axis();
	//chart_seris					*get_current_series();
	//chart_plot_area				*get_current_plot_area();
};
chart_chart* odf_chart_context::Impl::get_current_chart()
{
	for (long i=current_chart_state_.elements_.size()-1; i>=0; i--)
	{
		chart_chart * chart = dynamic_cast<chart_chart*>(current_chart_state_.elements_[i].elm.get());
		if (chart) return chart;
	}
	return NULL;
}
//chart_seris* odf_chart_context::Impl::odf_chart_context::get_current_series()
//{
//}
chart_axis * odf_chart_context::Impl::get_current_axis()
{
	for (long i=current_chart_state_.elements_.size()-1; i>=0; i--)
	{
		chart_axis * axis = dynamic_cast<chart_axis*>(current_chart_state_.elements_[i].elm.get());
		if (axis) return axis;
	}
	return NULL;
}
//chart_plot_area* odf_chart_context::Impl::get_current_plot_area()
//{
//}
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
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		impl_->current_chart_properties_ = style_->style_content_.get_style_chart_properties();
		
		chart->chart_chart_attlist_.common_attlist_.chart_style_name_ = style_name;
	}

	drawing_context()->start_element(chart_elm, style_elm);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int level = impl_->current_level_.size();

	//if (impl_->current_level_.size()>0)	impl_->current_level_.back()->add_child_element(chart_elm); не надо ... наследование через drawing

	impl_->current_level_.push_back(chart_elm);

	odf_element_state state={chart_elm, style_name, style_elm, level};

	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::set_size_chart(double width_pt, double height_pt)
{
	impl_->current_chart_state_.chart_height_pt = height_pt;
	impl_->current_chart_state_.chart_width_pt = width_pt;
	
	chart_chart *chart = impl_->get_current_chart();
	if (!chart)return;

	chart->chart_chart_attlist_.common_draw_size_attlist_.svg_width_ = length(length(width_pt,length::pt).get_value_unit(length::cm),length::cm);
	chart->chart_chart_attlist_.common_draw_size_attlist_.svg_height_ = length(length(height_pt,length::pt).get_value_unit(length::cm),length::cm);
}
void odf_chart_context::set_type_chart(std::wstring type)
{
	chart_chart *chart = impl_->get_current_chart();
	if (!chart)return;

	chart->chart_chart_attlist_.chart_class_ = std::wstring(L"chart:") + type;
}
void odf_chart_context::set_3D(bool Val)
{
	if (!impl_->current_chart_properties_) return;
	impl_->current_chart_properties_->content().chart_three_dimensional_ = Val;
}
void odf_chart_context::start_series(std::wstring type)
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"series", chart_elm, impl_->odf_context_);
	
	chart_series *series = dynamic_cast<chart_series*>(chart_elm.get());
	if (series == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		series->chart_series_attlist_.common_attlist_.chart_style_name_ = style_name;
		series->chart_series_attlist_.chart_class_ = std::wstring(L"chart:") + type;
	}
	start_element(chart_elm, style_elm, style_name);
}
void odf_chart_context::add_categories(std::wstring formula)
{
	office_element_ptr elm;
	create_element(L"chart", L"categories", elm, impl_->odf_context_);
		
	chart_categories *categories = dynamic_cast<chart_categories*>(elm.get());
	if (categories== NULL)return;

	categories->table_cell_range_address_ = formula;

	int level = impl_->current_level_.size();
	
	if (impl_->current_level_.size()>0) 
		impl_->current_level_.back()->add_child_element(elm);

	odf_element_state state={elm, L"",office_element_ptr(), level};
	impl_->current_chart_state_.elements_.push_back(state);
}

void odf_chart_context::start_axis()
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"axis", chart_elm, impl_->odf_context_);
	
	chart_axis *axis = dynamic_cast<chart_axis*>(chart_elm.get());
	if (axis == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		axis->chart_axis_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);

	if (impl_->current_chart_state_.axis_id < impl_->current_chart_state_.categories_.size()	&& 
		impl_->current_chart_state_.categories_[impl_->current_chart_state_.axis_id].length() > 0)
	{
		add_categories(impl_->current_chart_state_.categories_[impl_->current_chart_state_.axis_id]);
	}
	impl_->current_chart_state_.axis_id++;
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
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
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
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
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
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		plot_area->chart_plot_area_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
}
void odf_chart_context::start_text()
{
	impl_->odf_context_->start_text_context();
	impl_->odf_context_->text_context()->set_styles_context(impl_->styles_context_);

	style_text_properties *text_props = NULL;

	style *style_ = dynamic_cast<style*>(impl_->current_chart_state_.elements_.back().style_elm.get());
	if (style_)
	{
		impl_->current_paragraph_properties_ = style_->style_content_.get_style_paragraph_properties();
		impl_->current_text_properties_ = style_->style_content_.get_style_text_properties();
	}
	
	impl_->odf_context_->text_context()->set_single_object(true,impl_->current_paragraph_properties_,impl_->current_text_properties_);
}
void odf_chart_context::end_text()
{
	odf_text_context * text_context_ = text_context();
	if (text_context_ == NULL || impl_->current_level_.size() <1 )return;

	for (long i=0; i< text_context_->text_elements_list_.size(); i++)
	{
		if (text_context_->text_elements_list_[i].level ==0)
		{
			impl_->current_level_.back()->add_child_element(text_context_->text_elements_list_[i].elm);
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

void odf_chart_context::start_legend()
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"legend", chart_elm, impl_->odf_context_);
	
	chart_legend *legend = dynamic_cast<chart_legend*>(chart_elm.get());
	if (legend == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		legend->chart_legend_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
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
void odf_chart_context::set_axis_label(int type)
{
	if (!impl_->current_chart_properties_)return;
	if (type == 1 || type == 2)
		impl_->current_chart_properties_->content().chart_display_label_ =  true;
}
void odf_chart_context::set_axis_orientation(int type)
{
	if (!impl_->current_chart_properties_)return;
	if (type == 0) impl_->current_chart_properties_->content().chart_reverse_direction_ =  true;
}
void odf_chart_context::set_axis_logarithmic(bool val)
{
	if (!impl_->current_chart_properties_)return;
	impl_->current_chart_properties_->content().chart_logarithmic_ =  val;
}
void odf_chart_context::set_axis_dimension(std::wstring val)
{
	chart_axis *axis = impl_->get_current_axis();
	if (!axis)return;

	axis->chart_axis_attlist_.chart_dimension_ = val;
	axis->chart_axis_attlist_.chart_name_ = std::wstring(L"primary-") + val; // в оох нет заданного имени - формируем сами

}
void odf_chart_context::set_axis_position(int type)
{	
	if (!impl_->current_chart_properties_)return;
			//st_axposB = 0,
			//st_axposL = 1,
			//st_axposR = 2,
			//st_axposT = 3
	//if (type ==0 || type ==2)
	//	impl_->current_chart_properties_->content().chart_axis_position_ = L"end";
	//if (type ==1 || type ==3)
	//	impl_->current_chart_properties_->content().chart_axis_position_ = L"start";
//a value of type double – the axis line is placed at the given value on the crossing axis. 
//If the crossing axis is an axis displaying categories rather than values, a value of 1 indicates that the axis should be placed at the first category, a value of 2 indicates that the axis should be placed at the second category and so forth.
}
void odf_chart_context::set_axis_label_position(int type)
{	
	if (!impl_->current_chart_properties_)return;
			//st_ticklblposHIGH = 0,
			//st_ticklblposLOW = 1,
			//st_ticklblposNEXTTO = 2,
			//st_ticklblposNONE = 3
	if (type ==1)impl_->current_chart_properties_->content().chart_axis_label_position_ = L"outside-start";
	if (type ==0)impl_->current_chart_properties_->content().chart_axis_label_position_ = L"outside-end";
	if (type ==2)impl_->current_chart_properties_->content().chart_axis_label_position_ = L"near-axis";

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

	impl_->current_level_.push_back(elm);//стоит ли сюда перенести и current_chart_properties ????
	
	impl_->current_text_properties_ = NULL;
	impl_->current_paragraph_properties_ = NULL;
	impl_->current_graphic_properties_ = NULL;
	impl_->current_chart_properties_ = NULL;
	
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		impl_->current_chart_properties_ = style_->style_content_.get_style_chart_properties();
	}
	odf_element_state state={elm, style_name, style_elm, level};

	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::end_element()
{
	//допричесываение элемента
	{
		if (impl_->current_paragraph_properties_)
		{
			if (impl_->current_paragraph_properties_->content().style_writing_mode_)
			{
				switch(impl_->current_paragraph_properties_->content().style_writing_mode_->get_type())
				{
					case writing_mode::LrTb: 
					case writing_mode::RlTb:
					case writing_mode::Lr:
						impl_->current_chart_properties_->content().style_direction_ = direction(direction::Ltr); break;
					case writing_mode::TbRl:
					case writing_mode::TbLr:
					case writing_mode::Tb:
						impl_->current_chart_properties_->content().style_direction_ = direction(direction::Ttb); break;
				}
			}
		}
	}

	impl_->current_level_.pop_back();
	drawing_context()->end_element();

	impl_->current_paragraph_properties_ = NULL;
	impl_->current_graphic_properties_ = NULL;
	impl_->current_chart_properties_ = NULL;
}

void odf_chart_context::end_chart()
{
	if (impl_->current_chart_state_.elements_.size() < 1) return;
	
	end_element();

///////////////
	impl_->chart_list_.push_back(impl_->current_chart_state_);
	impl_->current_chart_state_.clear();
}
static formulasconvert::oox2odf_converter formulas_converter;

void odf_chart_context::set_series_name(std::wstring name)
{
}
void odf_chart_context::set_series_value_formula(std::wstring oox_formula)
{
	std::wstring odfFormula = formulas_converter.convert_chart_distance(oox_formula);

	chart_series *series = dynamic_cast<chart_series*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (series == NULL)return;
	
	series->chart_series_attlist_.chart_values_cell_range_address_ = odfFormula;
}
void odf_chart_context::set_series_label_formula(std::wstring oox_formula)
{
	std::wstring odfFormula = formulas_converter.convert_chart_distance(oox_formula);

	chart_series *series = dynamic_cast<chart_series*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (series == NULL)return;
	
	series->chart_series_attlist_.chart_label_cell_address_ = odfFormula;
}
void odf_chart_context::set_category_axis_formula(std::wstring oox_formula)
{
	std::wstring odfFormula = formulas_converter.convert_chart_distance(oox_formula);

	impl_->current_chart_state_.categories_.push_back(odfFormula);
}
}
}