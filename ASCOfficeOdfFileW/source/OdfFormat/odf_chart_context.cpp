#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>

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
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context), drawing_context_(odf_context), text_context_(odf_context)
    {	
		styles_context_ = NULL;
		
		current_chart_properties = NULL;
	} 
	odf_chart_state				current_chart_state_;

	std::vector<office_element_ptr> current_level_;//постоянно меняющийся список уровней наследования
	std::vector<odf_chart_state> chart_list_; //все элементы .. для удобства разделение по "топам"

	odf_style_context			*styles_context_;
	odf_conversion_context		*odf_context_;

	odf_drawing_context			drawing_context_; //для заполения графических стилей объектов чарта
	odf_text_context			text_context_;

	style_chart_properties		*current_chart_properties;

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
	impl_->drawing_context_.set_styles_context(style_context);
	impl_->text_context_.set_styles_context(style_context);
}

odf_drawing_context * odf_chart_context::drawing_context()
{
	return  &impl_->drawing_context_;
}
odf_text_context	* odf_chart_context::text_context()
{
	return &impl_->text_context_;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void odf_chart_context::start_chart(office_element_ptr & root)
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"chart", chart_elm, impl_->odf_context_);

	chart_chart* chart = dynamic_cast<chart_chart*>(chart_elm.get());
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
		impl_->current_chart_properties = style_->style_content_.get_style_chart_properties();
		
		chart->chart_chart_attlist_.common_attlist_.chart_style_name_ = style_name;
	}

	drawing_context()->start_element(chart_elm, style_elm);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int level = impl_->current_level_.size();
	
	if (impl_->current_level_.size()>0)
		impl_->current_level_.back()->add_child_element(chart_elm);

	impl_->current_level_.push_back(chart_elm);

	odf_element_state state={chart_elm, style_name, style_elm, level};

	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::set_type_chart(int type)
{
}
void odf_chart_context::start_series()
{
}
void odf_chart_context::end_series()
{
}
void odf_chart_context::start_axis()
{
}
void odf_chart_context::end_axis()
{
}
void odf_chart_context::end_chart()
{
	if (impl_->current_chart_state_.elements_.size() < 1) return;

///////////////
	impl_->chart_list_.push_back(impl_->current_chart_state_);

	impl_->current_chart_state_.clear();
	impl_->current_chart_properties = NULL;

}

}
}