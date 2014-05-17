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
		void clear()
		{
			elements_.clear();
		}

		std::vector<odf_element_state> elements_;
	};
class odf_chart_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		styles_context_ = NULL;
		
		current_chart_properties = NULL;
	} 
	odf_chart_state				current_chart_state_;

	std::vector<office_element_ptr> current_level_;//постоянно меняющийся список уровней наследования
	std::vector<odf_chart_state> chart_list_; //все элементы .. для удобства разделение по "топам"

	odf_style_context			*styles_context_;
	odf_conversion_context		*odf_context_;

	style_chart_properties		*current_chart_properties;
	chart_chart					*current_chart_;
};

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

	impl_->current_chart_ = dynamic_cast<chart_chart*>(chart_elm.get());
	if (impl_->current_chart_ == NULL)return;

	root->add_child_element(chart_elm);
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		impl_->current_chart_properties = style_->style_content_.get_style_chart_properties();
		
		impl_->current_chart_->chart_chart_attlist_.common_attlist_.chart_style_name_ = style_name;
	}

	drawing_context()->start_element(chart_elm, style_elm);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int level = impl_->current_level_.size();

	//if (impl_->current_level_.size()>0)	impl_->current_level_.back()->add_child_element(chart_elm); не надо ... наследование через drawing

	impl_->current_level_.push_back(chart_elm);

	odf_element_state state={chart_elm, style_name, style_elm, level};

	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::set_type_chart(std::wstring type)
{
	if (!impl_->current_chart_)return;

	impl_->current_chart_->chart_chart_attlist_.chart_class_ = std::wstring(L"chart:") + type;
}
void odf_chart_context::set_3D(bool Val)
{
	if (!impl_->current_chart_properties) return;
	impl_->current_chart_properties->chart_three_dimensional_ = Val;
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
		impl_->current_chart_properties = style_->style_content_.get_style_chart_properties();
		
		series->chart_series_attlist_.common_attlist_.chart_style_name_ = style_name;
		series->chart_series_attlist_.chart_class_ = std::wstring(L"chart:") + type;
	}
	start_element(chart_elm, style_elm, style_name);
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
		impl_->current_chart_properties = style_->style_content_.get_style_chart_properties();
		
		axis->chart_axis_attlist_.common_attlist_.chart_style_name_ = style_name;
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
		impl_->current_chart_properties = style_->style_content_.get_style_chart_properties();
		
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
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state().get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		impl_->current_chart_properties = style_->style_content_.get_style_chart_properties();
		
		plot_area->chart_plot_area_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
}
void odf_chart_context::start_text()
{
	impl_->odf_context_->start_text_context();
	impl_->odf_context_->text_context()->set_styles_context(impl_->styles_context_);

	style_paragraph_properties *para_props = NULL;
	style_text_properties *text_props = NULL;

	style *style_ = dynamic_cast<style*>(impl_->current_chart_state_.elements_.back().style_elm.get());
	if (style_)
	{
		para_props = style_->style_content_.get_style_paragraph_properties();
		text_props = style_->style_content_.get_style_text_properties();
	}
	
	impl_->odf_context_->text_context()->set_single_object(true,para_props,text_props);
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
		impl_->current_chart_properties = style_->style_content_.get_style_chart_properties();
		
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
	if (mode == 0) return;
	
	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.common_draw_position_attlist_.svg_x_ = length(*val,length::pt);

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_position_attlist_.svg_x_ = length(*val,length::pt);
	
	chart_title *title = dynamic_cast<chart_title*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (title)title->chart_title_attlist_.common_draw_position_attlist_.svg_x_ = length(*val,length::pt);
}
void odf_chart_context::set_layout_y(double *val,int mode)
{
	if (!val)return;
	if (mode == 0) return;

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.common_draw_position_attlist_.svg_y_ = length(*val,length::pt);

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_position_attlist_.svg_y_ = length(*val,length::pt);
	
	chart_title *title = dynamic_cast<chart_title*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (title)title->chart_title_attlist_.common_draw_position_attlist_.svg_y_ = length(*val,length::pt);
}
void odf_chart_context::set_layout_w(double *val,int mode)
{
	if (!val)return;
	if (mode == 0) return;
	
	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.chartooo_width_ = length(*val,length::pt);

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_size_attlist_.svg_width_ = length(*val,length::pt);
}
void odf_chart_context::set_layout_h(double *val,int mode)
{
	if (!val)return;
	if (mode == 0) return;

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.chartooo_height_ = length(*val,length::pt);

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_size_attlist_.svg_height_ = length(*val,length::pt);
}

void odf_chart_context::start_element(office_element_ptr & elm, office_element_ptr & style_elm, std::wstring style_name)
{
	int level = impl_->current_level_.size();
	
	drawing_context()->start_element(elm, style_elm);
	//if (impl_->current_level_.size()>0) impl_->current_level_.back()->add_child_element(elm); не надо...наследование через start_element в drawing

	impl_->current_level_.push_back(elm);//стоит ли сюда перенести и current_chart_properties ????

	odf_element_state state={elm, style_name, style_elm, level};

	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::end_element()
{
	impl_->current_level_.pop_back();
	drawing_context()->end_element();

	impl_->current_chart_properties = NULL;
}

void odf_chart_context::end_chart()
{
	if (impl_->current_chart_state_.elements_.size() < 1) return;
	
	end_element();

///////////////
	impl_->chart_list_.push_back(impl_->current_chart_state_);

	impl_->current_chart_state_.clear();
	impl_->current_chart_properties = NULL;
	impl_->current_chart_ = NULL;

}
static formulasconvert::oox2odf_converter formulas_converter;

void odf_chart_context::set_series_formula(int category, std::wstring oox_formula)
{
	std::wstring odfFormula = formulas_converter.convert_chart_distance(oox_formula);

	chart_series *series = dynamic_cast<chart_series*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (series == NULL)return;
	
	if (category == 1)
	{
		series->chart_series_attlist_.chart_label_cell_address_ = odfFormula;
	}
	if (category == 2)
	{
		series->chart_series_attlist_.chart_values_cell_range_address_ = odfFormula;
	}
}

}
}