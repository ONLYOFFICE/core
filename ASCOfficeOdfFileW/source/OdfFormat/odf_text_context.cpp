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

	current_outline_ = 0;
	in_field_ = false;
	
	keep_next_paragraph_ = false;
	
	list_state_.started_list = false;
}
odf_text_context::~odf_text_context()
{

}
void odf_text_context::clear_params()
{
	single_paragraph_ = false;
	paragraph_properties_ = NULL;
	text_properties_ = NULL;

	current_outline_ = 0;
	in_field_ = false;
	
	keep_next_paragraph_ = false;
	
	list_state_.levels.clear();
	list_state_.started_list = false;
	list_state_.style_name = L"";
}
void odf_text_context::set_styles_context(odf_style_context*  styles_context)
{
	styles_context_ = styles_context;
}
odf_style_context* odf_text_context::get_styles_context()
{
	return styles_context_;
}
void odf_text_context::set_single_object(bool val, style_paragraph_properties *para_props, style_text_properties *text_props)
{
	single_paragraph_ = val;
	paragraph_properties_ = para_props;
	text_properties_ = text_props;

	//if (paragraph_properties_)//??? а могут ли быть разрывы после-до диаграммы??? 
	//{
	//	paragraph_properties_->content().fo_break_before_ = need_break_;
	//	need_break_ = boost::none;
	//}

}

void odf_text_context::add_text_content(std::wstring & text)
{
	if (current_level_.size() < 1) return;

	//boost::wregex re(L"?:([ ]{2,})|(.+)");//(L"(\\w+)");
	//std::list<std::wstring> result;
	////boost::match_results<std::wstring::const_iterator> result;
	////bool b = boost::regex_match(text, result, re);
	//std::wstring t = text;
	//bool b = boost::regex_split(std::back_inserter(result),t, re);

	//int res;
	//int sz=0;
	//for (std::list<std::wstring>::iterator i=result.begin(); i!=result.end(); ++i)
	//{
	//	office_element_ptr elm;
	//	text = *i;
	//	sz = text.length();

	//	if (res = text.find(L" "))
	//	{
	//		add_text_space(sz);
	//	}
	//	else
	//		current_level_.back().elm->add_text(text);
	//}

	//std::string expression = "1a234bc43";
	boost::wregex re(L"[ ]{2,}");
	boost::wsregex_iterator i(
	  text.begin (),     
	  text.end (),     
	  re);
	boost::wsregex_iterator j;
	size_t pos=0;
	for(; i!=j;++i) 
	{
		std::wstring s = text.substr(pos, (*i).position()-pos); 
		if (s.length() > 0)current_level_.back().elm->add_text(s);

		std::wstringstream s1;
		s1 << (*i);
		s= s1.str();
		if (s.length() >0)
			add_text_space(s.length());
	  
		pos = (*i).position() + s.length();
	}
	std::wstring last = text.substr(pos);
	if (last.length() > 0)current_level_.back().elm->add_text(last);
}
void odf_text_context::add_text_space(int count)
{
	office_element_ptr s_elm;
	create_element(L"text", L"s", s_elm, odf_context_);

	text_s* s = dynamic_cast<text_s*>(s_elm.get());
	if (s)s->text_c_ = count;

	//необязательно хранить..
	//int level = current_level_.size();
	//odf_element_state state={	s_elm, L"", office_element_ptr(), level};
	//text_elements_list_.push_back(state);
	
	if (current_level_.size()>0)
		current_level_.back().elm->add_child_element(s_elm);
}
void odf_text_context::set_symbol_font(std::wstring & font)
{
	if (text_properties_ == NULL) return;

	text_properties_->text_format_properties_content_.fo_font_family_ = font;
	text_properties_->text_format_properties_content_.style_font_family_complex_ = font;
	text_properties_->text_format_properties_content_.style_font_family_asian_ = font;
}
void odf_text_context::set_symbol_text(int sym)
{
	char ch  = char( sym ^ 0xF000);
	std::wstring s;s.push_back(ch);
	add_text_content(s);
}
void odf_text_context::start_paragraph(bool styled)
{
	office_element_ptr paragr_elm;
	if (current_outline_ > 0)
	{
		create_element(L"text", L"h",paragr_elm,odf_context_);
		
		text_h* h = dynamic_cast<text_h*>(paragr_elm.get());
		if (h)h->text_outline_level_ = current_outline_ + 1;
	}
	else
	{
		create_element(L"text", L"p",paragr_elm,odf_context_);
	}
	current_outline_ = 0;

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
		odf_style_state_ptr style_state = styles_context_->last_state(style_family::Paragraph);

		if (style_state)
		{
			style_name = style_state->get_name();
			style_elm = style_state->get_office_element();
		}
	
		text_p* p = dynamic_cast<text_p*>(elm.get());
		if (p)p->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(style_name);	
		
		text_h* h = dynamic_cast<text_h*>(elm.get());
		if (h)h->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(style_name);	

		style *style_ = dynamic_cast<style*>(style_elm.get());
		if (style_)
		{
			if (parent_paragraph_style_.length() >0)style_->style_parent_style_name_ = parent_paragraph_style_;
			
			paragraph_properties_ = style_->style_content_.get_style_paragraph_properties();
		}
	}
	else if (parent_paragraph_style_.length() >0)
	{
		text_p* p = dynamic_cast<text_p*>(elm.get());
		if (p)p->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(parent_paragraph_style_);	
		
		text_h* h = dynamic_cast<text_h*>(elm.get());
		if (h)p->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(parent_paragraph_style_);	
	}
	if (paragraph_properties_ && need_break_)
	{
		paragraph_properties_->content().fo_break_before_ = need_break_;
		need_break_ = boost::none;
	}
	
	odf_element_state state={elm,  style_name, style_elm,level};
	text_elements_list_.push_back(state);
	if (current_level_.size()>0)
		current_level_.back().elm->add_child_element(elm);

	current_level_.push_back(state);
	
}

void odf_text_context::end_paragraph()
{
	if (single_paragraph_ == false && current_level_.size() > 0)
	{
		current_level_.pop_back();
	}
	paragraph_properties_ = NULL;
	text_properties_ = NULL;
}

void odf_text_context::start_element(office_element_ptr & elm, office_element_ptr style_elm ,std::wstring style_name)
{
	int level = current_level_.size();

	odf_element_state state={elm, style_name, style_elm, level};

	text_elements_list_.push_back(state);
	if (current_level_.size()>0)
		current_level_.back().elm->add_child_element(elm);

	current_level_.push_back(state);
}
void odf_text_context::end_element()
{
	if (single_paragraph_ == false && current_level_.size() > 0)
	{
		current_level_.pop_back();
	}
	else
	{
		int t=0;
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

	text_properties_ = NULL;
	if (styled)
	{		
		odf_style_state_ptr style_state = styles_context_->last_state(style_family::Text);

		if (style_state)
		{
			style_name = style_state->get_name();
			style_elm = style_state->get_office_element();
		}
		style *style_ = dynamic_cast<style*>(style_elm.get());
		
		text_span* span = dynamic_cast<text_span*>(span_elm.get());
		if (span) span->text_style_name_ = style_ref(style_name);

		if (style_)
		{
			if (parent_span_style_.length() >0)
			{
				style_->style_parent_style_name_ = parent_span_style_;
			}
			text_properties_ = style_->style_content_.get_style_text_properties();//для  буквиц на поле
		}
	}

	odf_element_state state={	span_elm, style_name, style_elm, level};

	text_elements_list_.push_back(state);
	
	if (current_level_.size()>0)
		current_level_.back().elm->add_child_element(span_elm);

	current_level_.push_back(state);
}

void odf_text_context::end_span()
{
	if (styles_context_ == NULL || single_paragraph_)return;
	
	if (current_level_.size() > 0)	
		current_level_.pop_back();

	text_properties_ = NULL;
}
/////////////////////////////////////////////////////////////////////////////////   LIST 
void odf_text_context::start_list_item()
{
	if (styles_context_ == NULL || single_paragraph_)return;
	if (list_state_.levels.size() < 1) return;
	
	if (list_state_.levels.back()) end_list_item();	
	
	office_element_ptr list_elm;
	create_element(L"text", L"list-item", list_elm, odf_context_);

	int level = current_level_.size();
	
	std::wstring style_name;
	office_element_ptr style_elm;

	odf_element_state state={list_elm, style_name, style_elm, level};

	text_elements_list_.push_back(state);
	
	if (current_level_.size()>0)
		current_level_.back().elm->add_child_element(list_elm);

	current_level_.push_back(state);
	
	list_state_.levels.back() = true;
}
void odf_text_context::end_list_item()
{
	if (styles_context_ == NULL || single_paragraph_) return;
	if (list_state_.levels.size() < 1) return;

	if (list_state_.levels.back() == false) return;

	if (current_level_.size() > 0)	
		current_level_.pop_back();

	list_state_.levels.back() = false;
}
void odf_text_context::start_list(std::wstring style_name) //todoooo add new_numbering ???
{
	if (styles_context_ == NULL || single_paragraph_)return;

	office_element_ptr list_elm;
	create_element(L"text", L"list", list_elm, odf_context_);

	int level = current_level_.size();
	
	office_element_ptr style_elm;
	odf_element_state state = {	list_elm, style_name, style_elm, level};
	
	if (style_name.length() > 0)
	{		
		text_list* list = dynamic_cast<text_list*>(list_elm.get());
		if (list)
		{
			list->text_style_name_ = style_ref(style_name);
			list->text_continue_numbering_ = true;
		}
		list_state_.style_name = style_name;
	}
	else
	{
		level = level;
	}
	text_elements_list_.push_back(state);
	
	if (current_level_.size()>0)
		current_level_.back().elm->add_child_element(list_elm);

	current_level_.push_back(state);

	list_state_.levels.push_back(false);
	list_state_.started_list = true;
}
void odf_text_context::end_list()
{
	if (styles_context_ == NULL || single_paragraph_)return;
	if (list_state_.levels.size() < 1) return;

	if (list_state_.levels.back()) end_list_item();	
	
	if (current_level_.size() > 0)	current_level_.pop_back();

	list_state_.levels.pop_back();
}
///////////////////////////////////////////////////////////////////////////////////////////  LIST
void odf_text_context::start_field(int type)
{
	if (single_paragraph_ == true) return;

	office_element_ptr elm;
	if (type == 2)
	{
		create_element(L"text", L"page-number", elm, odf_context_);
		text_page_number *page_numb = dynamic_cast<text_page_number*>(elm.get());
		if (page_numb)
		{
			page_numb->text_select_page_ = L"current";

			if ((odf_context_->page_layout_context()) && (odf_context_->page_layout_context()->last_layout().page_number_format))
			{
			
				page_numb->common_num_format_attlist_.style_num_format_ = odf_context_->page_layout_context()->last_layout().page_number_format;
			}
		}	
	}
	if (type == 3)
	{
		create_element(L"text", L"page-count", elm, odf_context_);
	}
	if (type == 4)
	{
		create_element(L"text", L"date", elm, odf_context_);
	}
	if (elm)
	{
		in_field_ = true;
		start_element(elm);
	}
}

void odf_text_context::end_field()
{
	if (single_paragraph_ == true) return;
	if (in_field_ == false) return;
	
	end_element();
	in_field_ = false;
}

void odf_text_context::set_outline_level(int level)
{
	current_outline_ = level;
}

void odf_text_context::add_text_style(office_element_ptr & style_elm, std::wstring style_name)
{
	if (style_name.size() < 1 || !style_elm)return;

	if (current_level_.size() < 1 )return;
	
	if (text_span* span = dynamic_cast<text_span*>(current_level_.back().elm.get()))
	{
		span->text_style_name_ = style_ref(style_name);
	}
	
	if (text_p* p = dynamic_cast<text_p*>(current_level_.back().elm.get()))
	{
		p->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(style_name);	
	}

	if (text_h* h = dynamic_cast<text_h*>(current_level_.back().elm.get()))
	{
		h->paragraph_.paragraph_attrs_.text_style_name_ = style_ref(style_name);	
	}
}
void odf_text_context::add_tab()
{
 	office_element_ptr elm;
	create_element(L"text", L"tab", elm, odf_context_);
	
	if (current_level_.size()>0)
		current_level_.back().elm->add_child_element(elm);
}
void odf_text_context::save_property_break()
{
	if (!need_break_) return;

	if (paragraph_properties_ == NULL)
	{
		for (unsigned int i = current_level_.size()-1; i>=0; i--)
		{
			office_element_ptr & elm = current_level_[i].elm;

			text_p* p = dynamic_cast<text_p*>(elm.get());
			
			text_h* h = dynamic_cast<text_h*>(elm.get());

			if ((p || h) && !current_level_[i].style_elm)
			{
				styles_context_->create_style(L"",odf::style_family::Paragraph, true, false, -1);					
				odf_style_state_ptr style_ = styles_context_->last_state();
				if (style_)
				{
					paragraph_properties_			= style_->get_paragraph_properties();
					current_level_[i].style_elm		= style_->get_office_element();
					current_level_[i].style_name	= style_->get_name();
					
					if (p) p->paragraph_.paragraph_attrs_.text_style_name_ = current_level_[i].style_name;
					if (h) h->paragraph_.paragraph_attrs_.text_style_name_ = current_level_[i].style_name;
				}
				break;
			}
		}
	}
	if (paragraph_properties_ == NULL) return;
	paragraph_properties_->content().fo_break_before_ = need_break_;
	need_break_ = boost::none;

}
void odf_text_context::set_type_break(int type, int clear)
{
		//brclearAll   = 0,
		//brclearLeft  = 1,
		//brclearNone  = 2,
		//brclearRight = 3
	if (type == 0)//brtypeColumn 
	{
		need_break_= fo_break(fo_break::Column);
	}
	else if (type == 1)//brtypePage
	{
		need_break_ = fo_break(fo_break::Page);
	}
	else //brtypeTextWrapping
	{
		office_element_ptr elm;
		create_element(L"text", L"line-break", elm, odf_context_);

		if (current_level_.size()>0)
			current_level_.back().elm->add_child_element(elm);	
	}

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