#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_conversion_context.h"

#include "text_elements.h"
#include "paragraph_elements.h"

namespace cpdoccore {
namespace odf
{

odf_text_context::odf_text_context(odf_style_context * styles_context,odf_conversion_context *odf_context)
{
	styles_context_ = styles_context;
	odf_context_ = odf_context;

	last_paragraph_ = NULL;
}
odf_text_context::~odf_text_context()
{
}
void odf_text_context::set_styles_context(odf_style_context*  styles_context)
{
	styles_context_ = styles_context;
}

void odf_text_context::add_text_content(const std::wstring & text)
{
	if (current_level_.size() >=0 )
		current_level_.back()->add_text(text);
	else
	{
	}
}

void odf_text_context::start_paragraph(office_element_ptr & elm)
{
	office_element_ptr style;

	int level = current_level_.size();

	odf_text_state state={elm,L"", style,level};
	text_elements_list_.push_back(state);
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(elm);

	current_level_.push_back(elm);
	
	//text_p *text_p_ = dynamic_cast<text_p*>(elm.get());

	//if (text_p_ ==  NULL)return;
	//
	//paragraph *new_paragraph = &(text_p_->paragraph_);

	//if (last_paragraph_)
	//	last_paragraph_->set_next(new_paragraph);

	//new_paragraph->set_prev(last_paragraph_);

	//last_paragraph_ =  new_paragraph;

}

void odf_text_context::end_paragraph()
{
	current_level_.pop_back();
}

void odf_text_context::start_element(office_element_ptr & elm)
{
	int level = current_level_.size();

	odf_text_state state={elm, L"", office_element_ptr(), level};

	text_elements_list_.push_back(state);
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(elm);

	current_level_.push_back(elm);
}
void odf_text_context::end_element()
{
	current_level_.pop_back();
}

void odf_text_context::start_span()//текст с особым стилем - который определяется по контексту
{
	if (styles_context_ == NULL)return;

	office_element_ptr span_elm;
	create_element(L"text", L"span", span_elm, odf_context_);

	text_span* span = dynamic_cast<text_span*>(span_elm.get());
	if (span == NULL)return;

	span->text_style_name_ = style_ref(styles_context_->last_state().get_name());

	int level = current_level_.size();

	odf_text_state state={	span_elm, 
							styles_context_->last_state().get_name(), 
							styles_context_->last_state().get_office_element(), 
							level};

	text_elements_list_.push_back(state);
	
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(span_elm);

	current_level_.push_back(span_elm);
}

void odf_text_context::end_span()
{
	current_level_.pop_back();
}

}
}