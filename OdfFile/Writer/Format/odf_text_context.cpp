/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "logging.h"

#include <boost/regex.hpp>

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

	using namespace odf_types;

namespace odf_writer
{

odf_text_context::odf_text_context(odf_conversion_context *odf_context, odf_style_context *styles_context)
{
	odf_context_				= odf_context;
	styles_context_				= styles_context;
	
	single_paragraph_			= false;
	paragraph_properties_		= NULL;
	text_properties_			= NULL;

	current_outline_			= -1;
	in_field_					= false;	
	keep_next_paragraph_		= false;	
	list_state_.started_list	= false;
}
odf_text_context::~odf_text_context()
{

}
void odf_text_context::clear_params()
{
	single_paragraph_			= false;
	paragraph_properties_		= NULL;
	text_properties_			= NULL;

	current_outline_			= -1;
	in_field_					= false;
	
	keep_next_paragraph_		= false;
	
	list_state_.levels.clear();
	list_state_.started_list	= false;
	list_state_.style_name		= L"";
}
void odf_text_context::set_styles_context(odf_style_context*  styles_context)
{
	styles_context_ = styles_context;
}
odf_style_context* odf_text_context::get_styles_context()
{
	return styles_context_;
}
void odf_text_context::set_single_object(bool bSingle, style_paragraph_properties *para_props, style_text_properties *text_props)
{
	single_paragraph_		= bSingle;
	paragraph_properties_	= para_props;
	text_properties_		= text_props;
}

void odf_text_context::add_text_content(const std::wstring & text)
{
	if (current_level_.empty()) return;

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
		if (s.length() > 0)
			add_text_space((int)s.length());
	  
		pos = (*i).position() + s.length();
	}
	std::wstring last = text.substr(pos);
	if (last.length() > 0)current_level_.back().elm->add_text(last);
}
void odf_text_context::add_text_date(const std::wstring & text)
{
	office_element_ptr s_elm;
	create_element(L"text", L"date", s_elm, odf_context_);
	
	text_date* s = dynamic_cast<text_date*>(s_elm.get());
	if (s) s->add_text(text);

	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(s_elm);
}
void odf_text_context::add_text_time(const std::wstring & text)
{
	office_element_ptr s_elm;
	create_element(L"text", L"time", s_elm, odf_context_);
	
	text_time* s = dynamic_cast<text_time*>(s_elm.get());
	if (s) s->add_text(text);

	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(s_elm);
}
void odf_text_context::add_text_page_number(const std::wstring & text)
{
	office_element_ptr s_elm;
	create_element(L"text", L"page-number", s_elm, odf_context_);
	
	text_page_number* s = dynamic_cast<text_page_number*>(s_elm.get());
	if (s) s->add_text(text);

	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(s_elm);
}
void odf_text_context::add_text_page_count(const std::wstring & text)
{
	office_element_ptr s_elm;
	create_element(L"text", L"page-count", s_elm, odf_context_);
	
	text_page_count* s = dynamic_cast<text_page_count*>(s_elm.get());
	if (s) s->add_text(text);

	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(s_elm);
}
void odf_text_context::add_text_file_name(const std::wstring &text)
{
	office_element_ptr s_elm;
	create_element(L"text", L"file-name", s_elm, odf_context_);
	
	text_file_name* s = dynamic_cast<text_file_name*>(s_elm.get());
	if (s) s->add_text(text);

	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(s_elm);
}
void odf_text_context::add_text_sheet_name(const std::wstring &text)
{
	office_element_ptr s_elm;
	create_element(L"text", L"sheet-name", s_elm, odf_context_);
	
	text_sheet_name* s = dynamic_cast<text_sheet_name*>(s_elm.get());
	if (s) s->add_text(text);

	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(s_elm);
}

void odf_text_context::add_text_space(int count)
{
	office_element_ptr s_elm;
	create_element(L"text", L"s", s_elm, odf_context_);

	text_s* s = dynamic_cast<text_s*>(s_elm.get());
	if (s) s->text_c_ = count;

	//необязательно хранить..
	//int level = current_level_.size();
	//odf_element_state state={	s_elm, L"", office_element_ptr(), level};
	//text_elements_list_.push_back(state);
	
	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(s_elm);
}
void odf_text_context::set_symbol_font(const std::wstring & font)
{
	if (text_properties_ == NULL) return;

	text_properties_->content_.fo_font_family_ = font;
	text_properties_->content_.style_font_family_complex_ = font;
	text_properties_->content_.style_font_family_asian_ = font;
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
	if (current_outline_ >= 0)
	{
		create_element(L"text", L"h", paragr_elm, odf_context_);
		
		text_h* h = dynamic_cast<text_h*>(paragr_elm.get());
		if (h)	h->text_outline_level_ = current_outline_ + 1;
	}
	else
	{
		create_element(L"text", L"p", paragr_elm, odf_context_);
	}
	current_outline_ = -1;

	start_paragraph(paragr_elm, styled);

}
void odf_text_context::start_paragraph(office_element_ptr & elm, bool styled)
{
	size_t level = current_level_.size();
	
	if (single_paragraph_ && level > 0)
	{
		std::wstring str_enter(L"\n");
		add_text_content(str_enter);
		return;
	}
	
	std::wstring		style_name;
	office_element_ptr	style_elm;

	if (styled)
	{		
		odf_style_state_ptr style_state = styles_context_->last_state(style_family::Paragraph);

		if (style_state)
		{
			style_name	= style_state->get_name();
			style_elm	= style_state->get_office_element();
		}
	
		text_p* p = dynamic_cast<text_p*>(elm.get());
		if (p)	p->paragraph_attrs_.text_style_name_ = style_name;	
		
		text_h* h = dynamic_cast<text_h*>(elm.get());
		if (h)	h->paragraph_attrs_.text_style_name_ = style_name;	

		style *style_ = dynamic_cast<style*>(style_elm.get());
		if (style_)
		{
			if (parent_paragraph_style_.length() >0)style_->style_parent_style_name_ = parent_paragraph_style_;
			
			paragraph_properties_ = style_->content_.get_style_paragraph_properties();
		}
	}
	else if (false == parent_paragraph_style_.empty())
	{
		text_p* p = dynamic_cast<text_p*>(elm.get());
		if (p)p->paragraph_attrs_.text_style_name_ = parent_paragraph_style_;	
		
		text_h* h = dynamic_cast<text_h*>(elm.get());
		if (h)p->paragraph_attrs_.text_style_name_ = parent_paragraph_style_;	
	}
	if (paragraph_properties_ && need_break_)
	{
		paragraph_properties_->content_.fo_break_before_ = need_break_;
		need_break_ = boost::none;
	}
	
	odf_element_state state(elm,  style_name, style_elm, level);
	text_elements_list_.push_back(state);
	
	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(elm);

	current_level_.push_back(state);
	
}

void odf_text_context::end_paragraph()
{
	if (false == single_paragraph_ && false == current_level_.empty())
	{
		current_level_.pop_back();
	}
	paragraph_properties_ = NULL;
	text_properties_ = NULL;
}
void odf_text_context::add_element_in_span_or_par(office_element_ptr & elm)
{
	for (int i = (int)current_level_.size() - 1; i >= 0; i--)
	{
		ElementType type_ = current_level_[i].elm->get_type();
		if (type_ == typeTextSpan || type_ == typeTextA || type_ == typeTextH || type_ == typeTextP)
		{
			current_level_[i].elm->add_child_element(elm);
			break;
		}
	}
}
void odf_text_context::start_element(office_element_ptr & elm, office_element_ptr style_elm, std::wstring style_name)
{
	size_t level = current_level_.size();

	odf_element_state state(elm, style_name, style_elm, level);

	text_elements_list_.push_back(state);
	
	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(elm);

	current_level_.push_back(state);
}
void odf_text_context::end_element()
{
	if (single_paragraph_ == false && false == current_level_.empty())
	{
		current_level_.pop_back();
	}
	else
	{
		int t = 0;
	}
}
void odf_text_context::start_span(bool styled)
{
	if (styles_context_ == NULL || single_paragraph_)return;

	office_element_ptr span_elm;
	create_element(L"text", L"span", span_elm, odf_context_);

	size_t level = current_level_.size();
	
	std::wstring		style_name;
	office_element_ptr	style_elm;

	text_properties_ = NULL;
	if (styled)
	{		
		odf_style_state_ptr style_state = styles_context_->last_state(style_family::Text);

		if (style_state)
		{
			style_name	= style_state->get_name();
			style_elm	= style_state->get_office_element();
		}
		style *style_ = dynamic_cast<style*>(style_elm.get());
		
		text_span* span = dynamic_cast<text_span*>(span_elm.get());
		if (span) span->text_style_name_ = style_name;

		if (style_)
		{
			if (false == parent_span_style_.empty())
			{
				style_->style_parent_style_name_ = parent_span_style_;
			}
			text_properties_ = style_->content_.get_style_text_properties();//для  буквиц на поле
		}
	}

	odf_element_state state(span_elm, style_name, style_elm, level);

	text_elements_list_.push_back(state);
	
	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(span_elm);

	current_level_.push_back(state);
}

void odf_text_context::end_span()
{
	if (styles_context_ == NULL || single_paragraph_)return;
	
	if (false == current_level_.empty() && dynamic_cast<text_span*>(current_level_.back().elm.get()))	
		current_level_.pop_back();

	text_properties_ = NULL;
}
/////////////////////////////////////////////////////////////////////////////////   LIST 
void odf_text_context::start_list_item()
{
	if (styles_context_ == NULL || single_paragraph_)return;
	if (list_state_.levels.empty()) return;
	
	if (list_state_.levels.back()) end_list_item();	
	
	office_element_ptr list_elm;
	create_element(L"text", L"list-item", list_elm, odf_context_);

	size_t level = current_level_.size();
	
	std::wstring style_name;
	office_element_ptr style_elm;

	odf_element_state state(list_elm, style_name, style_elm, level);

	text_elements_list_.push_back(state);
	
	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(list_elm);

	current_level_.push_back(state);
	
	list_state_.levels.back() = true;
}
void odf_text_context::end_list_item()
{
	if (styles_context_ == NULL || single_paragraph_) return;
	if (list_state_.levels.empty()) return;

	if (list_state_.levels.back() == false) return;

	if (false == current_level_.empty())	
		current_level_.pop_back();

	list_state_.levels.back() = false;
}
bool odf_text_context::start_list(std::wstring style_name) //todoooo add new_numbering ???
{
	if (styles_context_ == NULL || single_paragraph_)return false;

	office_element_ptr list_elm;
	create_element(L"text", L"list", list_elm, odf_context_);

	size_t level = current_level_.size();
	
	office_element_ptr style_elm;
	odf_element_state state(list_elm, style_name, style_elm, level);
	
	if (false == style_name.empty())
	{		
		text_list* list = dynamic_cast<text_list*>(list_elm.get());
		if (list)
		{
			list->text_style_name_ = style_name;
			list->text_continue_numbering_ = true;
		}
		list_state_.style_name = style_name;
	}
	else
	{
		level = level;
	}
	text_elements_list_.push_back(state);
	
	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(list_elm);

	current_level_.push_back(state);

	list_state_.levels.push_back(false);
	list_state_.started_list = true;

	return true;
}
void odf_text_context::end_list()
{
	if (styles_context_ == NULL || single_paragraph_)return;
	if (list_state_.levels.empty()) return;

	if (list_state_.levels.back()) end_list_item();	
	
	if (current_level_.size() > 0)	current_level_.pop_back();

	list_state_.levels.pop_back();
}
//------------------------------------------------------------------------------------------  LIST

bool odf_text_context::start_field(int type, const std::wstring& value, const std::wstring& format)
{
	if (single_paragraph_ == true) return false;

	office_element_ptr elm;

	switch(type)
	{
		case fieldXE:
		{
			create_element(L"text", L"alphabetical-index-mark", elm, odf_context_);
			text_alphabetical_index_mark *index = dynamic_cast<text_alphabetical_index_mark*>(elm.get());
			if (index)
			{
				index->key1_ = value;
				index->string_value_ = value;
			}
		}break;
		case fieldPage:
		{
			create_element(L"text", L"page-number", elm, odf_context_);
			text_page_number *page_numb = dynamic_cast<text_page_number*>(elm.get());
			if (page_numb)
			{
				page_numb->text_select_page_ = L"current";

				if (    (odf_context_->page_layout_context()) &&
						(odf_context_->page_layout_context()->last_layout()) &&
						(odf_context_->page_layout_context()->last_layout()->page_number_format))
				{

					page_numb->common_num_format_attlist_.style_num_format_ = odf_context_->page_layout_context()->last_layout()->page_number_format;
				}
			}	
		}break;
		case fieldNumPages:
		{
			create_element(L"text", L"page-count", elm, odf_context_);
		}break;
		case fieldTime:
		{
			create_element(L"text", L"time", elm, odf_context_);

			text_time *time = dynamic_cast<text_time*>(elm.get());
			if (time)
			{			
				if (false == value.empty()) time->text_time_value_ = value;
				if (false == format.empty() && styles_context_)
				{
					number_format_state state = styles_context_->numbers_styles().add_or_find(-1, format);
					time->style_data_style_name_ = state.style_name;
				}

			}
		}break;
		case fieldDateTime:
		{
			create_element(L"text", L"date", elm, odf_context_);

			text_date *date = dynamic_cast<text_date*>(elm.get());
			if (date)
			{		
				if (false == value.empty()) date->text_date_value_ = value;
				if (false == format.empty() && styles_context_)
				{
					number_format_state state = styles_context_->numbers_styles().add_or_find(-1, format);
					date->style_data_style_name_ = state.style_name;
				}
			}
		}break;
		case fieldTextInput:
		{
			create_element(L"text", L"text-input", elm, odf_context_);
		}break;
	}

	if (elm)
	{
		in_field_ = true;
		start_element(elm);

		current_level_.back().type = 3;
	
		return true;
	}
	return false;
}

void odf_text_context::end_field()
{
	if (single_paragraph_ == true) return;
	if (in_field_ == false) return;
	
	if (current_level_.empty() )return;

	while ((false == current_level_.empty()) && (current_level_.back().type != 3))
	{
		end_element();
	}
	end_element();
	in_field_ = false;
}

void odf_text_context::set_outline_level(int level)
{
	current_outline_ = level;
}

void odf_text_context::add_text_style(office_element_ptr & style_elm, std::wstring style_name)
{
	if (style_name.empty() || !style_elm)return;

	if (current_level_.empty() )return;
	
	if (text_span* span = dynamic_cast<text_span*>(current_level_.back().elm.get()))
	{
		span->text_style_name_ = style_name;
	}
	
	if (text_p* p = dynamic_cast<text_p*>(current_level_.back().elm.get()))
	{
		p->paragraph_attrs_.text_style_name_ = style_name;	
	}

	if (text_h* h = dynamic_cast<text_h*>(current_level_.back().elm.get()))
	{
		h->paragraph_attrs_.text_style_name_ = style_name;	
	}
}

void odf_text_context::add_hyperlink (const std::wstring & link, const std::wstring & display, const std::wstring & location)
{
	office_element_ptr elm;
	create_element(L"text", L"a", elm, odf_context_);

	text_a* hyperlink = dynamic_cast<text_a*>(elm.get());
	if (!hyperlink)return;

	if (!display.empty())
		hyperlink->add_text(display);
////////////////////////////

	hyperlink->common_xlink_attlist_.href_	= link + (location.empty() ? L"" : (L"#" + location));
	hyperlink->common_xlink_attlist_.type_	= xlink_type::Simple;
	
	if (false == current_level_.empty())
		current_level_.back().elm->add_child_element(elm);
}

void odf_text_context::add_tab(_CP_OPT(int) ref)
{
 	office_element_ptr elm;
	create_element(L"text", L"tab", elm, odf_context_);
	
	if (current_level_.size() > 0)
		current_level_.back().elm->add_child_element(elm);

	text_tab *tab = dynamic_cast<text_tab *>(elm.get());
	if (tab)
	{
		tab->text_tab_ref_ = ref;
	}
}
void odf_text_context::save_property_break()
{
	if (!need_break_) return;

	if (paragraph_properties_ == NULL)
	{
		for (int i = (int)current_level_.size() - 1; i >= 0; i--)
		{
			office_element_ptr & elm = current_level_[i].elm;

			text_p* p = dynamic_cast<text_p*>(elm.get());
			
			text_h* h = dynamic_cast<text_h*>(elm.get());

			if ((p || h) && !current_level_[i].style_elm)
			{
				styles_context_->create_style(L"", odf_types::style_family::Paragraph, true, false, -1);					
				odf_style_state_ptr style_ = styles_context_->last_state();
				if (style_)
				{
					paragraph_properties_			= style_->get_paragraph_properties();
					current_level_[i].style_elm		= style_->get_office_element();
					current_level_[i].style_name	= style_->get_name();
					
					if (p) p->paragraph_attrs_.text_style_name_ = current_level_[i].style_name;
					if (h) h->paragraph_attrs_.text_style_name_ = current_level_[i].style_name;
				}
				break;
			}
		}
	}
	if (paragraph_properties_ == NULL) return;
	paragraph_properties_->content_.fo_break_before_ = need_break_;
	need_break_ = boost::none;

}
bool odf_text_context::set_type_break(int type, int clear)//todooo clear ???
{
	bool need_restart = false;

	if (type == -1)
	{
		need_break_		= boost::none;
	}
	else if (type == 0)//brtypeColumn 
	{
		need_break_		= fo_break(fo_break::Column);
		need_restart	= true;
	}
	else if (type == 1)//brtypePage
	{
		office_element_ptr elm;
		create_element(L"text", L"soft-page-break", elm, odf_context_);	
		
		start_element(elm);
		end_element();
		
		need_break_		= fo_break(fo_break::Page);
		need_restart	= true;
	}
	else //brtypeTextWrapping
	{
		office_element_ptr elm;
		create_element(L"text", L"line-break", elm, odf_context_);

		if (current_level_.size() > 0)
			current_level_.back().elm->add_child_element(elm);	
	}
	return need_restart;
}

bool odf_text_context::set_master_page_name (std::wstring & master_page_name)
{
	if (current_level_.empty()) return false;

	style *style_ = dynamic_cast<style*>(current_level_.back().style_elm.get());
	
	if (!style_) 
		return false;

	style_->style_master_page_name_	 = master_page_name;
	return true;
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
