#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_conversion_context.h"

#include "styles.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"

#include "text_elements.h"
#include "paragraph_elements.h"

namespace cpdoccore {
namespace odf
{

odf_text_context::odf_text_context(odf_conversion_context *odf_context)
{
	odf_context_ = odf_context;
	styles_context_ = odf_context->styles_context();
	
	single_paragraph_ = false;
	paragraph_properties_ = NULL;
	text_properties_ = NULL;

	last_paragraph_ = NULL;
}
odf_text_context::~odf_text_context()
{
}
void odf_text_context::set_styles_context(odf_style_context*  styles_context)
{
	styles_context_ = styles_context;
}

void odf_text_context::set_single_object(bool val, style_paragraph_properties *para_props, style_text_properties *text_props)
{
	single_paragraph_ = val;
	paragraph_properties_ = para_props;
	text_properties_ = text_props;
}

void odf_text_context::add_text_content(const std::wstring & text)
{
	//if (text == L" " && single_paragraph_ == false)
	//{
	//	office_element_ptr elm;
	//	create_element(L"text", L"s", elm, odf_context_);

	//	current_level_.back()->add_child_element(elm);

	//}
	//else
	{
		if (current_level_.size() > 0 )
			current_level_.back()->add_text(text);
	}
}
void odf_text_context::start_paragraph(bool styled)
{
	office_element_ptr paragr_elm;
	create_element(L"text", L"p",paragr_elm,odf_context_);

	start_paragraph(paragr_elm, styled);

}
void odf_text_context::start_paragraph(office_element_ptr & elm, bool styled)
{
	int level = current_level_.size();
	
	if (single_paragraph_ && level >0)
	{
		std::wstring str_enter(L"\n");
		add_text_content(str_enter);
		return;
	}
	
	std::wstring style_name;
	office_element_ptr style_elm;

	if (styled)
	{
		style_name = styles_context_->last_state().get_name();
		style_elm = styles_context_->last_state().get_office_element();
		
		text_p* p = dynamic_cast<text_p*>(elm.get());
		if (p)p->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(style_name);	
		
		if (parent_paragraph_style_.length() >0)
		{
			style *style_ = dynamic_cast<style*>(style_elm.get());
			if (style_)style_->style_parent_style_name_ = parent_paragraph_style_;
		}
	}
	else if (parent_paragraph_style_.length() >0)
	{
		text_p* p = dynamic_cast<text_p*>(elm.get());
		if (p)p->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(parent_paragraph_style_);	
	}

	odf_text_state state={elm,  style_name, style_elm,level};
	text_elements_list_.push_back(state);
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(elm);

	current_level_.push_back(elm);
	
}

void odf_text_context::end_paragraph()
{
	if (single_paragraph_ == false)
	{
		current_level_.pop_back();
	}
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
	if (single_paragraph_ == false)
	{
		current_level_.pop_back();
	}
}

void odf_text_context::start_span(bool styled)
{
	if (styles_context_ == NULL || single_paragraph_)return;

	office_element_ptr span_elm;
	create_element(L"text", L"span", span_elm, odf_context_);

	int level = current_level_.size();
	
	std::wstring style_name;
	office_element_ptr style_elm;

	if (styled)
	{		
		style_name = styles_context_->last_state().get_name();
		style_elm = styles_context_->last_state().get_office_element();
		style * style_ = dynamic_cast<style*>(style_elm.get());
		
		text_span* span = dynamic_cast<text_span*>(span_elm.get());
		if (span) span->text_style_name_ = style_ref(style_name);

		if (parent_span_style_.length() >0 && style_)
		{
			style_->style_parent_style_name_ = parent_span_style_;
		}
	}

	odf_text_state state={	span_elm, style_name, style_elm, level};

	text_elements_list_.push_back(state);
	
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(span_elm);

	current_level_.push_back(span_elm);
}

void odf_text_context::end_span()
{
	if (styles_context_ == NULL || single_paragraph_)return;
	
	current_level_.pop_back();
}
void odf_text_context::add_textline_break()
{
	office_element_ptr elm;
	create_element(L"text", L"line-break", elm, odf_context_);

	if (current_level_.size()>0)
		current_level_.back()->add_child_element(elm);
}
void odf_text_context::add_text_style(office_element_ptr & style_elm, std::wstring style_name)
{
	if (style_name.size() < 1 || !style_elm)return;

	if (text_elements_list_.size() < 1 )return;
	if (text_span* span = dynamic_cast<text_span*>(text_elements_list_.back().elm.get()))
	{
		span->text_style_name_ = style_ref(style_name);

		text_elements_list_.back().style_elm = style_elm;
		text_elements_list_.back().style_name = style_name;
	}
	
	if (text_p* p = dynamic_cast<text_p*>(text_elements_list_.back().elm.get()))
	{
		p->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(style_name);	
	
		text_elements_list_.back().style_elm = style_elm;
		text_elements_list_.back().style_name = style_name;
	}

}
void odf_text_context::add_tab()
{
 	office_element_ptr elm;
	create_element(L"text", L"tab", elm, odf_context_);
	
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(elm);
}
void odf_text_context::add_page_break()
{
 //	office_element_ptr elm;
	//create_element(L"text", L"soft-page-break", elm, odf_context_);

	//bool in_span = false;
	//if (text_span* span = dynamic_cast<text_span*>(current_level_.back().get()))
	//{
	//	in_span = true;
	//	end_span(); // todoo - перенос стиля
	//}

	//bool in_paragraph = false;
	//if (text_p* p = dynamic_cast<text_p*>(current_level_.back().get()))
	//{
	//	in_paragraph = true;
	////fo:break-before

	if (paragraph_properties_) paragraph_properties_->content().fo_break_before_ = fo_break(fo_break::Page);

		//if (current_level_.size()>0)
		//	current_level_.back()->add_child_element(elm);
//		end_paragraph(); // todoo - перенос стиля
	//}

	////end paragraph + style add after-break = page
	////start paragraph - continues style
	//if (in_paragraph)
	//{
	////	start_paragraph(false);
	//	
	//	//if (current_level_.size()>0)
	//	//	current_level_.back()->add_child_element(elm);
	//}
	//if (in_span)
	//{ 
	//	start_span(false);
	//}
}
void odf_text_context::set_parent_paragraph_style(std::wstring & style_name)
{
	parent_paragraph_style_ = style_name;
}
void odf_text_context::set_parent_span_style(std::wstring & style_name)
{
	parent_span_style_ = style_name;
}
}
}