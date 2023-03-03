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


#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "../../Common/utils.h"

#include "odt_conversion_context.h"

#include "styles.h"

#include "style_paragraph_properties.h"
#include "style_table_properties.h"
#include "style_section_properties.h"
#include "style_text_properties.h"

#include "office_text.h"
#include "office_annotation.h"

#include "paragraph_elements.h"
#include "text_elements.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

namespace utils
{

double calculate_size_font_symbols(std::wstring str_test, std::wstring font_name, double font_size, NSFonts::IApplicationFonts *appFonts)
{
    std::pair<float,float> appr = _graphics_utils_::calculate_size_symbol_asc(font_name, font_size, false, false, appFonts);
	
	if (appr.first < 0.01 || appr.second < 0.01)
	{
        appr.first = _graphics_utils_::calculate_size_symbol_win(font_name, font_size, false, false, str_test);
		//appr_px = ((int)(appr_px+0.5) + 2*(int)appr_px)/3.;
	}

	return appr.first * 0.55;
}
}
odt_conversion_context::odt_conversion_context(package::odf_document * outputDocument) 
	:	odf_conversion_context (TextDocument, outputDocument),
		comment_context_(this), notes_context_(this), main_text_context_(NULL), table_context_(this), controls_context_(this)
{

	is_hyperlink_				= false;

	is_header_					= false;
	is_footer_					= false;

	is_paragraph_in_current_section_		= false;

	text_changes_state_.main_text_context	= NULL; //header, footer, drawing, main, ..
	
	drop_cap_state_.clear();
}

odt_conversion_context::~odt_conversion_context()
{
	if (main_text_context_)	delete main_text_context_;
}

void odt_conversion_context::start_document()
{
	start_text();
	
	root_document_		= get_current_object_element();
	root_text_			= dynamic_cast<office_text*>(root_document_.get());
	
	main_text_context_	= new odf_text_context(this, styles_context()); 

	page_layout_context()->set_styles_context(styles_context());

/////////////////настройки дефолтовые

	root_text_->office_text_attlist_.text_use_soft_page_breaks_ = true;
}

void odt_conversion_context::end_document()
{
	if (false == mapSequenceDecls.empty())
	{
		office_element_ptr seq_decls;		
		create_element(L"text", L"sequence-decls", seq_decls, this);	

		for (std::map<std::wstring, int>::iterator it = mapSequenceDecls.begin(); it != mapSequenceDecls.end(); ++it)
		{
			office_element_ptr elm;		
			create_element(L"text", L"sequence-decl", elm, this);	

			text_sequence_decl* decl = dynamic_cast<text_sequence_decl*>(elm.get());
			if (decl)
			{
				decl->name_ = it->first;
				decl->display_outline_level_ = 0;
			}		
			seq_decls->add_child_element(elm);
		}
		root_document_->add_child_element(seq_decls);
	}
	if (controls_context()->is_exist_content())
	{
		office_element_ptr forms_root_elm;
		create_element(L"office", L"forms", forms_root_elm, this);

		controls_context()->finalize(forms_root_elm);
		
		root_document_->add_child_element(forms_root_elm);
	}
	//add sections to root
	for (size_t i = 0; i < sections_.size(); i++)
	{
		root_document_->add_child_element(sections_[i].elm);
	}
	sections_.clear();
	
	//add last elements to root 
	for (size_t i = 0; i< current_root_elements_.size(); i++)
	{
		root_document_->add_child_element(current_root_elements_[i].elm);
	}
	current_root_elements_.clear();
	
	odf_conversion_context::end_document();
} 
odf_controls_context* odt_conversion_context::controls_context()
{
	return  &controls_context_;
}

odf_drawing_context* odt_conversion_context::drawing_context()	
{
	if (drawing_context_.size() > 0)
	{
		return drawing_context_.back().get();
	}
	else
		return NULL;
}
odf_comment_context* odt_conversion_context::comment_context()	
{
	return &comment_context_;
}

odf_style_context* odt_conversion_context::styles_context()	
{
	if (text_context_.size() > 0)
	{
		return text_context_.back()->get_styles_context();
	}
	else
	{
		return odf_conversion_context::styles_context();
	}
}

odf_table_context* odt_conversion_context::table_context()
{
	return &table_context_;
}
	
odf_notes_context* odt_conversion_context::notes_context()
{
	return &notes_context_;
}

odf_text_context* odt_conversion_context::text_context()	
{
	if (text_context_.size() > 0)
	{
		return text_context_.back().get();
	}
	else
	{
		return main_text_context_;
	}
} 

void odt_conversion_context::add_text_content(const std::wstring & text)
{
	if (drop_cap_state_.enabled)
	{
        int count = (int)text.length();
		drop_cap_state_.characters += count;
		
		style_text_properties * props = text_context()->get_text_properties();
		if (props)
		{
			if (drop_cap_state_.inline_style == false)
			{
				std::wstring f_name = props->content_.fo_font_family_.get_value_or(L"Arial");
				double f_size = props->content_.fo_font_size_.get_value_or(font_size(length(12, length::pt))).get_length().get_value_unit(length::pt);
					
				drop_cap_state_.characters_size_pt += utils::calculate_size_font_symbols(text, f_name, f_size, applicationFonts_);
			}
		}
	}

	text_context()->add_text_content(text);
}
void odt_conversion_context::add_to_root()
{
	if (!text_context_.empty()) return; // не root element (embedded) ????

	if (comment_context_.is_started()) return;

	if (text_context()->current_level_.size() > 1) return;

	odf_element_state & state = text_context()->current_level_.back();
	current_root_elements_.push_back(state);
}
void odt_conversion_context::start_drawing_context()
{
	odf_conversion_context::start_drawing_context();

	drawing_context()->set_footer_state(is_footer_);
	drawing_context()->set_header_state(is_header_);
}
bool odt_conversion_context::start_math()
{
	if (false == math_context()->isEmpty()) return false;

	start_drawing_context();
	return odf_conversion_context::start_math();
}
void odt_conversion_context::end_math()
{
	odf_conversion_context::end_math();
	end_drawing_context();
}
void odt_conversion_context::end_drawing_context()
{
	if (drawing_context_.empty()) return;

	office_element_ptr & elm = drawing_context()->get_root_element();

	if (elm && text_context()->current_level_.size() > 0)//add to 'p' or 'h' !!!!!
	{
		anchor_type::type anchor = drawing_context()->get_anchor();

		bool bSet = false;
		if (( anchor == anchor_type::Page || anchor == anchor_type::Paragraph) || (is_header_ || is_footer_))
		{
			for (long i = (long)text_context()->current_level_.size() - 1 ; i>=0; i--)
			{
				text_p *p = dynamic_cast<text_p*>(text_context()->current_level_[i].elm.get());
				text_h *h = dynamic_cast<text_h*>(text_context()->current_level_[i].elm.get());

				if (p || h)
				{
					bSet = true;
					text_context()->current_level_[i].elm->add_child_element(elm);
					break;
				}
			}
		}
		if (!bSet)
		{
			if (text_context()->current_level_.size() > 1 && dynamic_cast<text_span*>(text_context()->current_level_.back().elm.get()))
			{
				text_context()->end_span();
			}
			text_context()->current_level_.back().elm->add_child_element(elm);
		}
		
		drawing_context()->clear();
		drawing_context_.pop_back();
	}
	else
	{		
		if (elm)
		{
			text_context()->start_element(elm);
			text_context()->end_element();
		}

		drawing_context()->clear();
		drawing_context_.pop_back();
	}
}
void odt_conversion_context::start_paragraph(bool styled)
{
	if (false == current_fields.empty() && current_fields.back().status == 1 && false == current_fields.back().in_span)
	{
		current_fields.back().status = 2;
		//if (!current_fields.empty() && !current_fields.back().result) return;	//Стандартное_составное_письмо.docx
		switch (current_fields.back().type)
		{
		case fieldBibliography:	start_bibliography();		break;
		case fieldIndex:		start_alphabetical_index();	break;
		case fieldIllustration:	start_illustration_index();	break;
		case fieldTable:		start_table_index();		break;
		case fieldToc:			start_table_of_content();	break;
		}	
	}
	if (is_paragraph_in_current_section_ && false == styled) 
	{
		styles_context()->create_style(L"", odf_types::style_family::Paragraph, true, false, -1);					
		styled = true;
	}

	text_context()->start_paragraph(styled);	
	
	add_to_root();
}
void odt_conversion_context::add_paragraph_break(int type)
{
	if (current_root_elements_.size() > 0)			
	{ 	
		//text_p* para	= NULL;
		//style * style_	= NULL;
		//http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#__RefHeading__1415190_253892949
		//нихере не работает !! в span, ... приходится генерить разрыв вручную !!

		//if (para = dynamic_cast<text_p*>(current_root_elements_.back().elm.get()))
		//{
		//	style_ = dynamic_cast<style*>(current_root_elements_.back().style_elm.get());//для переноса свойств параграфа
		//	end_paragraph();
		//}	
		//else/* if (header = dynamic_cast<text_h*>(current_root_elements_.back().elm.get()))*/
		//{
			//тут получается что разрыв будет прописан внутри элемента (не параграфа) - так что вручную свойство запишем
			//в случае разрыва параграфов оно запишется при старте после-разрывного параграфа
			//text_context()->set_type_break(type, 0);
		text_context()->save_property_break();
		//}
		//if (type == 1)	//page
		//{
		//	text_context()->start_element(elm);
		//	//add_to_root();
		//	text_context()->end_element();
		//}
		//if (para)
		//{
		//	styles_context()->create_style(L"",	odf_types::style_family::Paragraph, true, false, -1);	
		//	//styles_context()->last_state().apply_from(style_);
		//	if (style_ )
		//	{
		//		style_paragraph_properties * new_props = NULL;
		//		if (styles_context()->last_state())
		//		{// нужна именно копия св-в так как будет добавочные свойства
		//			new_props = styles_context()->last_state()->get_paragraph_properties();				
		//			if (new_props)new_props->apply_from(style_->content_.get_style_paragraph_properties());
		//			
		//			if (style_->style_parent_style_name_)
		//				styles_context()->last_state()->set_parent_style_name(*style_->style_parent_style_name_);
		//		}
		//	}	
		//	start_paragraph(true);
		//}
	}
}
void odt_conversion_context::start_table_of_content()
{
	office_element_ptr elm;
	
	create_element(L"text", L"table-of-content", elm, this);	
	text_context()->start_element(elm);

	add_to_root();
	
	start_index_field();
}

void odt_conversion_context::end_table_of_content()
{
	text_context()->end_element();

	office_element_ptr elm;
	create_element(L"text", L"table-of-content-source", elm, this);
	text_table_of_content_source* source = dynamic_cast<text_table_of_content_source*>(elm.get());
	
	if (source)
	{
		if (false == current_fields.back().arStyleLevels.empty())
		{
			source->outline_level_ = (int) current_fields.back().arStyleLevels.size(); 
			source->use_index_source_styles_ = true;
		}
		else if (current_fields.back().outline_levels > 0)
		{
			source->outline_level_ = current_fields.back().outline_levels;
		}
	}

	text_context()->start_element(elm);
	//template
	text_context()->end_element();
	text_context()->end_element();
}

void odt_conversion_context::start_alphabetical_index()
{
	office_element_ptr elm;
	create_element(L"text", L"alphabetical-index", elm, this);	
	text_context()->start_element(elm);	

	add_to_root();

	start_index_field();
}
void odt_conversion_context::end_alphabetical_index()
{
	text_context()->end_element();

	office_element_ptr elm;
	create_element(L"text", L"alphabetical-index-source", elm, this);
	
	text_context()->start_element(elm);
	//tempalte
	text_context()->end_element();
	text_context()->end_element();
}
void odt_conversion_context::start_illustration_index()
{
	office_element_ptr elm;
	create_element(L"text", L"illustration-index", elm, this);	
	text_context()->start_element(elm);	
	
	add_to_root();
	
	start_index_field();
}
void odt_conversion_context::end_illustration_index()
{
	text_context()->end_element();
	
	office_element_ptr elm;
	create_element(L"text", L"illustration-index-source", elm, this);
	text_illustration_index_source* index_source = dynamic_cast<text_illustration_index_source*>(elm.get());
	if (index_source)
	{
		if (false == current_fields.back().captionSEQ.empty())
		{
			index_source->caption_sequence_name_ = current_fields.back().captionSEQ;
			index_source->caption_sequence_format_ = L"text";
		}
	}
	
	text_context()->start_element(elm);
	//template
	text_context()->end_element();
	text_context()->end_element();
}

void odt_conversion_context::start_bibliography()
{
	office_element_ptr elm;
	create_element(L"text", L"bibliography", elm, this);	
	text_context()->start_element(elm);	

	add_to_root();

	start_index_field();
}
void odt_conversion_context::end_bibliography()
{
	text_context()->end_element();
	
	office_element_ptr elm;
	create_element(L"text", L"bibliography-source", elm, this);
	
	text_context()->start_element(elm);
	//template
	text_context()->end_element();
	text_context()->end_element();
}
void odt_conversion_context::start_table_index()
{
	office_element_ptr elm;
	create_element(L"text", L"table-index", elm, this);	
	text_context()->start_element(elm);	

	add_to_root();

	start_index_field();
}
void odt_conversion_context::end_table_index()
{
	text_context()->end_element();

	office_element_ptr elm;
	create_element(L"text", L"table-index-source", elm, this);
	
	text_context()->start_element(elm);
	//template
	text_context()->end_element();
	text_context()->end_element();
}
void odt_conversion_context::start_index_field()
{
	if (current_fields.empty()) return;

	office_element_ptr elm1, elm2;
	create_element(L"text", L"index-body", elm1, this);
	text_context()->start_element(elm1);

	if (false == current_fields.back().title.empty())
	{
		create_element(L"text", L"index-title", elm2, this);
		text_context()->start_element(elm2);
		text_context()->end_element();
	}
}
void odt_conversion_context::end_index_field()
{
	text_context()->end_element();
	text_context()->end_element();
}
void odt_conversion_context::start_bookmark (int id, const std::wstring& name)
{
	office_element_ptr bookmark_elm;
	create_element(L"text", L"bookmark-start", bookmark_elm, this);

	text_bookmark_start* bookmark = dynamic_cast<text_bookmark_start*>(bookmark_elm.get());
	if (bookmark)
	{
		bookmark->text_name_ = name;

		std::map<int, std::wstring>::iterator pFind = mapBookmarks.find(id);
		if (pFind == mapBookmarks.end())
		{
			mapBookmarks.insert(std::make_pair(id, name));
		}
		text_context()->add_element_in_span_or_par(bookmark_elm);
	}
}
void odt_conversion_context::end_bookmark (int id)
{
	std::map<int, std::wstring>::iterator pFind = mapBookmarks.find(id);
	if (pFind == mapBookmarks.end()) return;

	office_element_ptr bookmark_elm;
	create_element(L"text", L"bookmark-end", bookmark_elm, this);

	text_bookmark_end* bookmark = dynamic_cast<text_bookmark_end*>(bookmark_elm.get());
	if (bookmark)
	{
		bookmark->text_name_ = pFind->second;

		text_context()->add_element_in_span_or_par(bookmark_elm);
	}
}
void odt_conversion_context::start_hyperlink(const std::wstring& link, const std::wstring& location)
{
	if (false == current_fields.empty() && current_fields.back().status == 1 && false == current_fields.back().in_span)
	{
		end_paragraph();
		start_paragraph(true);
	}
	office_element_ptr hyperlink_elm;
	create_element(L"text", L"a", hyperlink_elm, this);

	text_a* hyperlink = dynamic_cast<text_a*>(hyperlink_elm.get());
	if (hyperlink)
	{
		hyperlink->common_xlink_attlist_.href_	= link + (location.empty() ? L"" : (L"#" + location));
		hyperlink->common_xlink_attlist_.type_	= xlink_type::Simple;
		
		text_context()->start_element(hyperlink_elm);

		is_hyperlink_ = true;
	}
}
void odt_conversion_context::end_hyperlink()
{
	if (!is_hyperlink_) return;

	if (false == text_context()->current_level_.empty() && dynamic_cast<text_span*>(text_context()->current_level_.back().elm.get()))	//CARA SETTING WARNET.docx
		text_context()->end_element();

	text_context()->end_element();

	is_hyperlink_ = false; //метка .. для гиперлинков в объектах - там не будет span
}
void odt_conversion_context::start_drop_down()
{
	office_element_ptr elm_drop_down;
	create_element(L"text", L"drop-down", elm_drop_down, this);

	text_drop_down* drop_down = dynamic_cast<text_drop_down*>(elm_drop_down.get());
	if (drop_down)
		drop_down->text_name_ = current_fields.back().name;

	for (size_t i = 0; i < current_fields.back().items.size(); i++)
	{
		office_element_ptr elm;
		create_element(L"text", L"label", elm, this);

		text_label* label = dynamic_cast<text_label*>(elm.get());
		if (label)
		{
			label->text_value_ = current_fields.back().items[i].first;
		}

		elm_drop_down->add_child_element(elm);
	}
	text_context()->start_element(elm_drop_down);
}
void odt_conversion_context::end_drop_down()
{
	text_context()->end_element();
}
void odt_conversion_context::start_sequence()
{
	std::map<std::wstring, int>::iterator pFind = mapSequenceDecls.find(current_fields.back().value);
	
	int index = 0;
	if (pFind == mapSequenceDecls.end())
	{
		mapSequenceDecls.insert(std::make_pair(current_fields.back().value, index));
	}
	else
	{
		index = ++pFind->second;
	}
	office_element_ptr seq_elm;
	create_element(L"text", L"sequence", seq_elm, this);

	text_sequence* sequence = dynamic_cast<text_sequence*>(seq_elm.get());
	if (sequence)
	{
		sequence->name_	= current_fields.back().value;
		sequence->ref_name_	= L"ref" + current_fields.back().value + std::to_wstring(index);
		sequence->formula_ = L"ooow:" + current_fields.back().value + L"+1";
		sequence->style_num_format_	= style_numformat(style_numformat::arabic);
		
		text_context()->start_element(seq_elm);
	}
}	
void odt_conversion_context::end_sequence()
{
	text_context()->end_span();
	text_context()->end_element();
}
std::map<std::wstring, std::wstring> odt_conversion_context::parse_instr_options(const std::wstring& value)
{
	std::map<std::wstring, std::wstring> result;

	std::vector<std::wstring> arOptions;
	boost::algorithm::split(arOptions, value, boost::algorithm::is_any_of(L"\\"), boost::algorithm::token_compress_on);

	for (size_t i = 0; i < arOptions.size(); i++)
	{
		std::wstring key = arOptions[i].substr(0, 1);
		std::wstring value;
		if (arOptions[i].length() > 2)
		{
			size_t pos = arOptions[i].find(L"\"");
			if (std::wstring::npos != pos)
			{
				value = arOptions[i].substr(pos + 1, arOptions[i].length() - pos - 1);
				
				pos = value.rfind(L"\"");
				if (std::wstring::npos != pos)
				{
					value = value.substr(0, pos);
				}
			}
			else
			{
				value = arOptions[i].substr(1);		
			}
		}
		result.insert(std::make_pair(key, value));
	}

	return result;
}

void odt_conversion_context::add_field_instr(const std::wstring &instr)
{
	if (current_fields.empty()) return;
	current_fields.back().instrText += instr;
}
void odt_conversion_context::set_field_instr()
{
	if (current_fields.empty()) return;
	
	std::wstring instr = current_fields.back().instrText;

	current_fields.back().status = 1; //prepare

	size_t res1 = instr.find(L"HYPERLINK");
	if (std::wstring::npos != res1)	
	{
		current_fields.back().type = fieldHyperlink;
		current_fields.back().in_span = false;
		
		std::wstring ref;
		boost::wregex r2 (L"([\"'])(.+?)\\1");			

		std::list<std::wstring> result1;
		bool b1 = boost::regex_split(std::back_inserter(result1), instr, r2);

		if (b1 && !result1.empty())
        {
			std::list<std::wstring>::iterator it = result1.begin();
			it++;

			if (it != result1.end())
			{
				ref = *it;
				if (ref.size() > 2)
					current_fields.back().value = ref;

				it++;
				if (it != result1.end())
				{
					it++;
					if (it != result1.end())
					{
						std::wstring ref2 = *it;

						if (ref2.size() > 2)
							current_fields.back().value += L"#" + ref2;
					}
				}
			}
        }
	}
	res1 = instr.find(L"NUMPAGES");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldNumPages;
	}	
	res1 = instr.find(L"PAGEREF");
	if (std::wstring::npos != res1 && current_fields.back().type == 0 )
	{
		current_fields.back().type = fieldPageRef;
		if (instr.length() > 9)
			current_fields.back().value = instr.substr(9, instr.length() - 5);
	}
	res1 = instr.find(L"PAGE");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldPage;
	}
	res1 = instr.find(L"TIME");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldTime;
	}
	res1 = instr.find(L"DATE");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldDate;
	}
	res1 = instr.find(L"FORMTEXT");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldTextInput;
	}
	res1 = instr.find(L"SEQ");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldSeq;
		std::map<std::wstring, std::wstring> options = parse_instr_options(instr.substr(4));
		
		for (std::map<std::wstring, std::wstring>::iterator it = options.begin(); it != options.end(); ++it)
		{
			if (it->first == L" ")//field-argument
			{
				current_fields.back().value = it->second.substr(0, it->second.length() - 1);	
			}
			else if (it->first == L"*")
			{
				current_fields.back().format = it->second.substr(0, it->second.length() - 1);	
			}
		}
	}
	res1 = instr.find(L"XE");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldXE;
		std::map<std::wstring, std::wstring> options = parse_instr_options(instr.substr(3));
		
		for (std::map<std::wstring, std::wstring>::iterator it = options.begin(); it != options.end(); ++it)
		{
			if (it->first == L" ")//field-argument
			{
				current_fields.back().value = it->second.substr(0, it->second.length() - 1);	
			}
		}
	}
	res1 = instr.find(L"BIBLIOGRAPHY");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldBibliography;
		current_fields.back().in_span = false;
	}
	res1 = instr.find(L"INDEX");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldIndex;
		current_fields.back().in_span = false;
	}
	res1 = instr.find(L"TOC");
	if (std::wstring::npos != res1 && current_fields.back().type == 0)
	{
		current_fields.back().type = fieldIllustration;
		current_fields.back().in_span = false;
		std::map<std::wstring, std::wstring> options = parse_instr_options(instr.substr(res1 + 3));
		
		std::map<std::wstring, std::wstring>::iterator pFind = options.find(L"c");
		if ( pFind != options.end())
		{
			current_fields.back().captionSEQ = pFind->second;
		}
		pFind = options.find(L"h");
		if ( pFind != options.end())
		{
			current_fields.back().bHyperlinks = true; 
		}
		pFind = options.find(L"u"); //paragraph outline level
		if ( pFind != options.end())
		{
			current_fields.back().type = fieldToc;  //table of content
		}
		pFind = options.find(L"o");
		if ( pFind != options.end())//table of content outline levels style
		{
			std::vector<std::wstring> arLevels;
			boost::algorithm::split(arLevels, pFind->second, boost::algorithm::is_any_of(L"-"), boost::algorithm::token_compress_on);
			if (arLevels.size() > 1)
			{
				if (current_fields.back().captionSEQ.empty()) 
					current_fields.back().type = fieldToc;
				current_fields.back().outline_levels = XmlUtils::GetInteger(arLevels[1]);
			}
		}
		pFind = options.find(L"t");
		if ( pFind != options.end())//content styles name
		{
			std::vector<std::wstring> arStyles;
			boost::algorithm::split(arStyles, pFind->second, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

			for (size_t i = 0; i < arStyles.size() - 1; i += 2)
			{
				int level = XmlUtils::GetInteger(arStyles[i + 1]);
				if (level < 1) continue;

				while(current_fields.back().arStyleLevels.size() + 1 < level )
				{
					current_fields.back().arStyleLevels.push_back(L"");
				}
				current_fields.back().arStyleLevels.push_back(arStyles[i]);
			}
		}
		pFind = options.find(L"z");
		if ( pFind != options.end())
		{
			current_fields.back().bHidePageNumbers = true; 
		}
	}
	////////////////////////////////////////// 
	res1 = instr.find(L" ");
	if (std::wstring::npos != res1)
	{
		if (current_fields.back().format.empty())
		{
			std::map<std::wstring, std::wstring> options = parse_instr_options(instr.substr(res1 + 1));

			std::map<std::wstring, std::wstring>::iterator pFind = options.find(L"@");
			if (pFind != options.end())
			{
				current_fields.back().format = pFind->second;
			}
		}

		if (current_fields.back().type == 0)
		{
			current_fields.back().name = instr.substr(0, res1);
		}
	}
}
void odt_conversion_context::set_field_date_time(const std::wstring &date_time)
{
	if (current_fields.empty()) return;

	current_fields.back().status = 1;

	current_fields.back().type = fieldDateTime;
	current_fields.back().value = date_time;
}
void odt_conversion_context::set_field_color(_CP_OPT(color) &clr)
{
	if (current_fields.empty()) return;

	current_fields.back().color_ = clr;
}
void odt_conversion_context::set_field_format(const std::wstring &format)
{
	if (current_fields.empty()) return;

	current_fields.back().format = format;
}
void odt_conversion_context::set_field_name(const std::wstring &name)
{
	if (current_fields.empty()) return;

	current_fields.back().name = name;
}
void odt_conversion_context::set_field_drop_down()
{
	if (current_fields.empty()) return;
	
	current_fields.back().status = 1;

	current_fields.back().type = fieldDropDown;
}
void odt_conversion_context::set_field_item	(const std::wstring &value, const std::wstring &display)
{
	if (current_fields.empty()) return;

	current_fields.back().items.push_back(std::make_pair(value, display));
}
void odt_conversion_context::start_field(bool in_span)
{
	if (false == current_fields.empty() && current_fields.back().status == 0 && current_fields.back().instrText.empty() )
		return; //start_field из sdt 

	_field_state field;
	current_fields.push_back(field);
	current_fields.back().in_span = in_span;
}
void odt_conversion_context::separate_field()
{
	if (current_fields.empty()) return;

	set_field_instr();
	current_fields.back().result = true;
}
void odt_conversion_context::set_master_page_name(std::wstring master_name)
{
	if (current_root_elements_.empty())// return; - эффект_штурмовика.docx - 1 страница !! (и ваще - 
	{
		is_paragraph_in_current_section_ = true;
		return;
	}

	if (current_master_page_ == master_name) return; // Newslette.docx

	style *style_ = dynamic_cast<style*>(current_root_elements_.back().style_elm.get());

	if (style_)
	{
		is_paragraph_in_current_section_	= false;
		style_->style_master_page_name_		= master_name;
	}
	else
	{
		if (text_context()->set_master_page_name(master_name))
			is_paragraph_in_current_section_ = false;		
	}
	current_master_page_ = master_name;
}
int odt_conversion_context::get_current_section_columns()
{
	if (sections_.size() > 0)
		return sections_.back().count_columns;
	else return 1;
}
void odt_conversion_context::add_section(bool continuous)
{
//--dump first elements to root------------------------------------------------
	for (size_t i = 0; i< current_root_elements_.size(); i++)
	{
		root_document_->add_child_element(current_root_elements_[i].elm);
	}
	current_root_elements_.clear();

//----------------------------------------------------------------------------
	odt_section_state state;
	
	state.empty			= true;
	state.continuous	= continuous;
//----------------------------------------------------------------
	styles_context()->create_style(L"", odf_types::style_family::Section, true, false, -1);		

	create_element(L"text", L"section", state.elm, this);
	state.style_elm		= styles_context()->last_state()->get_office_element();
	state.style_name	= styles_context()->last_state()->get_name();
	
	text_section* section = dynamic_cast<text_section*>(state.elm.get());
	if (section)section->text_section_attr_.text_style_name_ = state.style_name;	

	sections_.push_back(state);
}
void odt_conversion_context::add_section_columns(int count, double space_pt, bool separator)
{
	if (sections_.empty() || count < 1) return;

	style* style_ = dynamic_cast<style*>(sections_.back().style_elm.get());
	if (!style_)return;

	style_section_properties	* section_properties	= style_->content_.get_style_section_properties();
	
	create_element(L"style", L"columns",section_properties->style_columns_,this);	
	
	style_columns* columns = dynamic_cast<style_columns*>(section_properties->style_columns_.get());
	if (!columns)return;

	sections_.back().count_columns	= count;

						columns->fo_column_count_	= count;
	if (space_pt >= 0)	columns->fo_column_gap_		= length(length(space_pt,length::pt).get_value_unit(length::cm),length::cm);

	if (separator)
	{
		create_element(L"style", L"column-sep",columns->style_column_sep_,this);
		style_column_sep* sep = dynamic_cast<style_column_sep*>(columns->style_column_sep_.get());
		if (sep)//default set
		{
			sep->style_width_			= length(0.035, length::cm);
			sep->style_height_			= percent(100);
			sep->style_vertical_align_	= vertical_align(vertical_align::Middle);
			sep->style_color_			= color(L"#000000");	
		}
	}
}
void odt_conversion_context::add_section_column(std::vector<std::pair<double, double>> width_space)
{
	if (sections_.empty() || width_space.empty()) return;

	style* style_ = dynamic_cast<style*>(sections_.back().style_elm.get());
	if (!style_)return;

	style_section_properties	* section_properties		= style_->content_.get_style_section_properties();
	//section_properties->text_dont_balance_text_columns_	= true;
	
	style_columns* columns = dynamic_cast<style_columns*>(section_properties->style_columns_.get());
	if (!columns)return;
	
	double width_all = 0;
	for (size_t i = 0; i < width_space.size() ; i++)
	{
		if (width_space[i].first >= 0) 
		
			width_all += width_space[i].first + width_space[i].second;
	}

	if (width_all < 1)	return;

	section_properties->style_editable_ = false;

	double last_space = 0;

	for (size_t i = 0; i < width_space.size() && width_all > 0 ; i++)
	{
		office_element_ptr col_elm;
		
		create_element(L"style", L"column", col_elm, this);
		style_column* col = dynamic_cast<style_column*>(col_elm.get());
		if (!col) continue;

		int val =  (width_space[i].first)* 65535. / width_all ;
		col->style_rel_width_ = odf_types::percent_rel(val);
		
		col->fo_start_indent_ = odf_types::length(last_space / 2, odf_types::length::pt);
		
		col->fo_end_indent_ = odf_types::length(width_space[i].second / 2, odf_types::length::pt);
		
		columns->add_child_element(col_elm);

		last_space = width_space[i].second;

	}
}
void odt_conversion_context::end_field()
{
	if (current_fields.empty()) return;

	if (current_fields.back().status == 0 && current_fields.back().instrText.empty() == false)
	{
		set_field_instr();
	}
	if (current_fields.back().status == 1 && false == current_fields.back().in_span && current_fields.back().type < 0xff)
	{
		current_fields.back().status = 2;

		if (current_fields.back().type == fieldHyperlink)
		{
			std::wstring location;
			start_hyperlink(current_fields.back().value, location);
		}
		else if (current_fields.back().type == fieldSeq)		start_sequence();
		else if (current_fields.back().type == fieldDropDown)	start_drop_down();
		else
			text_context()->start_field(current_fields.back().type, current_fields.back().value, current_fields.back().format);
	}
	if (current_fields.back().status == 2)
	{
		current_fields.back().status = 3;//prepare for delete
		
		if (current_fields.back().type < 0xff)
		{
			if (current_fields.back().type == fieldHyperlink)		end_hyperlink();
			else if (current_fields.back().type == fieldSeq)		end_sequence();
			else if (current_fields.back().type == fieldDropDown)	end_drop_down();
			else text_context()->end_field();
		
			current_fields.pop_back();
		}
	}
	else if (current_fields.back().status == 0)	
	{
		current_fields.pop_back();
	}
	if (false == current_fields.empty())
	{
		if (current_fields.back().type < 0xff	&& current_fields.back().type != fieldHyperlink 
												&& current_fields.back().type != fieldSeq
												&& current_fields.back().type != fieldDropDown)
			text_context()->in_field_ = true;
	}

}
void odt_conversion_context::end_paragraph()
{
	//if (!current_fields.empty() && !current_fields.back().result) return;	//Стандартное_составное_письмо.docx
	text_context()->end_paragraph();
	
	if (false == current_fields.empty() && current_fields.back().status == 3)	//prepare for delete
	{
		switch(current_fields.back().type)
		{
			case fieldBibliography:	end_bibliography();			break;
			case fieldIndex:		end_alphabetical_index();	break;
			case fieldIllustration:	end_illustration_index();	break;
			case fieldTable:		end_table_index();			break;
			case fieldToc:			end_table_of_content();		break;
		}
		current_fields.pop_back();
	}
	
	flush_section();
}

void odt_conversion_context::start_list_item(int level, std::wstring style_name )
{
	//if (text_context()->list_state_.started_list == false)
	//{
	//	text_context()->start_list(style_name);
	//	add_to_root();
	//	text_context()->start_list_item();
	//}
	level = level + 1; //отсчет от 1 (а не 0)

	if (text_context()->list_state_.style_name != style_name && text_context()->list_state_.started_list)
	{
		set_no_list();
	}

	while (text_context()->list_state_.levels.size() > level)
	{
		text_context()->end_list();
	}

	if (text_context()->list_state_.levels.empty())
	{
		text_context()->list_state_.started_list = false;
		text_context()->list_state_.style_name = L"";
	}

	if (text_context()->list_state_.started_list == false)
	{
		if (text_context()->start_list(style_name))
		{
			//text_context()->set_list_continue(true); //??? держать в памяти все списки????
			add_to_root();
		}
		
	}
	text_context()->start_list_item(); 

	if (text_context()->list_state_.style_name == style_name)
		style_name = L"";

	while (text_context()->list_state_.levels.size() < level)
	{
		if (false == text_context()->start_list(style_name))
			break;
		text_context()->start_list_item();
	}	
}
void odt_conversion_context::end_list_item()
{
	text_context()->end_list_item();
}
void odt_conversion_context::set_no_list()
{
	if (text_context()->list_state_.started_list == false) return;

	while (false == text_context()->list_state_.levels.empty())
	{
		text_context()->end_list_item();
		text_context()->end_list();	
	}
	text_context()->list_state_.started_list = false;
	text_context()->list_state_.style_name = L"";
}
void odt_conversion_context::flush_section()
{
	if (!sections_.empty() && sections_.back().empty)
	{
		for (size_t i = 0; i < current_root_elements_.size(); i++)
		{
			if ((sections_.back().continuous && i < 2) || !sections_.back().continuous)
				// при вставлении параграфа возможен искусственный разрыв в параграфах - см add_page_break
			{
				text_soft_page_break * break_ = dynamic_cast<text_soft_page_break*>(current_root_elements_[i].elm.get());
				if (break_)
				{
					//необходимо добавить в свойства предыдущего параграфа	(если он есть, если нет - следующего), что брейк не нужен
					//int paragraph_break = i-1;
					//if (paragraph_break < 0) paragraph_break = i+1;
					//style *style_ = dynamic_cast<style*>(current_root_elements_[paragraph_break].style_elm.get());
					//if (style_)
					//{
					//	style_paragraph_properties *props = style_->content_.get_style_paragraph_properties();
					//	props->content_.fo_break_before_ = fo_break(fo_break::Auto);
					//	props->content_.fo_break_after_  = fo_break(fo_break::Auto);
					//}
					continue;
				}
			}
			sections_.back().elm->add_child_element(current_root_elements_[i].elm);
		}
		current_root_elements_.clear();

		//sections_.back().empty = false;
	}
}
void odt_conversion_context::start_run(bool styled)
{
	if (is_hyperlink_ && text_context_.size() > 0) return;
	
	if (!current_fields.empty() && current_fields.back().status == 1 && false == current_fields.back().in_span && current_fields.back().type < 0xff)
	{
		current_fields.back().status = 2;

		if (current_fields.back().type == fieldHyperlink)
		{
			std::wstring location;
			start_hyperlink(current_fields.back().value, location);
		}
		else if (current_fields.back().type == fieldSeq)		start_sequence();
		else if (current_fields.back().type == fieldDropDown)	start_drop_down();
		else												
			text_context()->start_field(current_fields.back().type, current_fields.back().value, current_fields.back().format);
	}	
	
	text_context()->start_span(styled);

	if (drop_cap_state_.enabled)
	{
		style_text_properties *props = text_context()->get_text_properties();
		if (props)
			props->apply_from(dynamic_cast<style_text_properties*>(drop_cap_state_.text_properties.get()));

	}
	if (!current_fields.empty() && current_fields.back().status == 1 && current_fields.back().in_span)//поле стартуется в span - нужно для сохранения стиля
	{
		current_fields.back().status = 2;
		text_context()->start_field(current_fields.back().type, current_fields.back().value, current_fields.back().format);
	}	
}
void odt_conversion_context::end_run()
{
	if (is_hyperlink_ && text_context_.size() > 0) return;

	if (!current_fields.empty() && current_fields.back().status == 1 && current_fields.back().in_span)
	{
		end_field();
	}
	
	text_context()->end_span();

	//if (current_field_.enabled == true && current_field_.started== false && current_field_.type == 1)
	//{
	//	start_hyperlink(current_field_.value);
	//	current_field_.started = true;
	//}
}
//--------------------------------------------------------------------------------------------------------
int odt_conversion_context::start_comment(int oox_comm_id)
{
	int comm_state = comment_context_.find_by_id(oox_comm_id);

	if (comm_state == 0)
	{
		office_element_ptr comm_elm;
		create_element(L"office", L"annotation", comm_elm, this);

		comment_context_.start_comment(comm_elm, oox_comm_id);
	
		comm_state = 1;
		if (text_context()->current_level_.empty())
		{
			comm_state = 2;
			
			start_paragraph();
			text_context()->set_KeepNextParagraph(true);

			//аннотацию нельзя добавить на верхний уровень - нет поддержки в open_format
		}
		text_context()->current_level_.back().elm->add_child_element(comm_elm);

		odf_element_state state(comm_elm, L"", office_element_ptr(), text_context()->current_level_.size());
		text_context()->current_level_.push_back(state);
	
		return comm_state; 
	}
	else return 0;
}
void odt_conversion_context::start_comment_content()
{
	comment_context_.start_comment_content();
	office_element_ptr & root_comm_element = text_context()->current_level_.back().elm;
	start_text_context();

	text_context()->start_element(root_comm_element);
}
void odt_conversion_context::end_comment_content()
{
	if (comment_context_.is_started())
	{
		comment_context_.end_comment_content();

		text_context()->end_element();
		end_text_context();

		text_context()->current_level_.pop_back();
	}
}
void odt_conversion_context::end_comment(int oox_comm_id)
{
	int index_added = comment_context_.find_by_id(oox_comm_id);

	if (index_added > 0 )
	{
		office_element_ptr comm_elm;
		create_element(L"office", L"annotation-end", comm_elm, this);

		comment_context_.end_comment(comm_elm, oox_comm_id);
		
		if (false == text_context()->current_level_.empty())
			text_context()->current_level_.back().elm->add_child_element(comm_elm);
	}
}
//--------------------------------------------------------------------------------------------------------
void odt_conversion_context::start_note(int oox_ref_id, int type)
{
	office_element_ptr note_elm;
	create_element(L"text", L"note", note_elm, this);

	notes_context_.start_note(note_elm, oox_ref_id, type);
	
	if (text_context()->current_level_.size() > 0)
		text_context()->current_level_.back().elm->add_child_element(note_elm);

	odf_element_state state(note_elm, L"", office_element_ptr(), text_context()->current_level_.size());
	text_context()->current_level_.push_back(state);
}
void odt_conversion_context::start_note_content()
{
	notes_context_.start_note_content();

    office_element_ptr note_content_element = notes_context_.get_note_content();

	odf_element_state state(note_content_element, L"", office_element_ptr(), text_context()->current_level_.size());
	text_context()->current_level_.push_back(state);

	start_text_context();

	text_context()->start_element(note_content_element);
}
void odt_conversion_context::end_note_content()
{
	notes_context_.end_note_content();
	
	text_context()->end_element();
	end_text_context();

	text_context()->current_level_.pop_back();
}
void odt_conversion_context::end_note()
{
	notes_context_.end_note();
	text_context()->current_level_.pop_back();
}
//--------------------------------------------------------------------------------------------------------
bool odt_conversion_context::start_change (int id, int type, std::wstring &author, std::wstring &userId, std::wstring &date, std::wstring style_name)
{
	if (id < 0) return false;

	if (!text_changes_state_.current_types.empty() && text_changes_state_.current_types.back() == 2) return false;

	//if (!text_changes_state_.main_text_context)
	//{
	//	text_changes_state_.main_text_context = text_context();
	//}
	//if (!text_changes_state_.main_text_context) return;
	
	text_changes_state_.current_types.push_back(type);

	std::wstring strId = L"ct" + boost::lexical_cast<std::wstring>(id);
//---------------------------------------------------------------------------------
	office_element_ptr start_elm;

	if (type == 2) create_element(L"text", L"change", start_elm, this); //место удаления ..
	else			
		create_element(L"text", L"change-start", start_elm, this);	//место изменения .... 

	text_add_change* change = dynamic_cast<text_add_change*>(start_elm.get());
	if (change) change->text_change_id_ = strId;

	text_context()->start_element(start_elm);
		add_to_root();//добавление/удаление параграфов и т д
	text_context()->end_element();	

	//text_changes_state_.main_text_context->start_element(start_elm);
	//text_changes_state_.main_text_context->end_element();	
//---------------------------------------------------------------------------------
	office_element_ptr region_elm;
	create_element(L"text", L"changed-region", region_elm, this);

	text_changed_region* region = dynamic_cast<text_changed_region*>(region_elm.get());
	if (region)region->text_id_ = strId;

	office_element_ptr child_elm;
	if (type == 1) create_element(L"text", L"insertion",		child_elm, this);
	if (type == 2) create_element(L"text", L"deletion",			child_elm, this);
	if (type == 3) create_element(L"text", L"format-change",	child_elm, this);

	office_element_ptr info_elm;
	create_element(L"office", L"change-info", info_elm, this);

	child_elm->add_child_element(info_elm);
	if (info_elm)
	{
		office_element_ptr creator_elm;
		create_element(L"dc", L"creator", creator_elm, this);
		info_elm->add_child_element(creator_elm);
		
		dc_creator* creator = dynamic_cast<dc_creator*>(creator_elm.get());
		if (creator)
			creator->content_ = author;

		office_element_ptr date_elm;
		create_element(L"dc", L"date", date_elm, this);
		info_elm->add_child_element(date_elm);
		
		dc_date* date_ = dynamic_cast<dc_date*>(date_elm.get());
		if (date_)
			date_->content_ = date;
	}

	if (is_header_ || is_footer_)	page_layout_context()->root_header_footer_->add_child_element(region_elm);
	else							get_current_object_element()->add_child_element(region_elm);

	region_elm->add_child_element(child_elm);

	if (type == 3 && !style_name.empty())
	{
		//не по спецификации !!!
		text_format_change * format_change = dynamic_cast<text_format_change*> (child_elm.get());
		if (format_change)
			format_change->text_style_name_ = style_name;

	}

	if (type == 2)//delete
	{
		start_text_context();
		text_context()->start_element(child_elm);
		text_context()->start_paragraph();//ваще то не по стандарту .. может мы уже в параграфе (ради Libra! ... гы)
	}
	return true;
}
void odt_conversion_context::end_change (int id, int type)
{
	//if (!text_changes_state_.main_text_context) return;
	
	std::wstring strId = L"ct" + boost::lexical_cast<std::wstring>(id);

	if (type == 2)//delete
	{
		text_context()->end_paragraph();
		text_context()->end_element();
		end_text_context();
	}
	else
	{
		office_element_ptr end_elm;
		create_element(L"text", L"change-end", end_elm, this);	

		text_add_change* change = dynamic_cast<text_add_change*>(end_elm.get());
		if (change) change->text_change_id_ = strId;

		text_context()->start_element(end_elm);
			add_to_root();
		text_context()->end_element();	
	}
	text_changes_state_.current_types.pop_back();//todooo map?? удаление без проверки чего удаляешь
}
//bool odt_conversion_context::is_delete_changes()
//{
//	if (text_changes_state_.current_types.empty()) return false;
//
//	return (text_changes_state_.current_types.back() == 2);
//}
//--------------------------------------------------------------------------------------------------------
style_text_properties* odt_conversion_context::get_drop_cap_properties() 
{
	if (!drop_cap_state_.text_properties)
	{
		create_element(L"style", L"text-properties", drop_cap_state_.text_properties, this);
	}
	return dynamic_cast<style_text_properties *>(drop_cap_state_.text_properties.get());
}
void odt_conversion_context::start_drop_cap(style_paragraph_properties *paragraph_properties)
{
	if (drop_cap_state_.enabled) 
		end_drop_cap(); // 2 подряд ваще возможны ???

	if (paragraph_properties == NULL) return;

	drop_cap_state_.enabled = true;
	drop_cap_state_.paragraph_properties = paragraph_properties;

	office_element_ptr comm_elm;
	create_element(L"style", L"drop-cap", drop_cap_state_.paragraph_properties->content_.style_drop_cap_, this);
}

void odt_conversion_context::set_drop_cap_lines(int lines)
{
	if (!drop_cap_state_.enabled) return;
	if (!drop_cap_state_.paragraph_properties) return;

	style_drop_cap *drop_cap = dynamic_cast<style_drop_cap*>(drop_cap_state_.paragraph_properties->content_.style_drop_cap_.get());
	if (drop_cap)drop_cap->style_lines_ = lines;

	drop_cap_state_.lines = lines;
}
void odt_conversion_context::set_drop_cap_margin(bool val)
{
	if (!drop_cap_state_.enabled) return;
	drop_cap_state_.inline_style = !val;
}
void odt_conversion_context::end_drop_cap()
{
	if (!drop_cap_state_.enabled) return;

	if (drop_cap_state_.characters > 0 && drop_cap_state_.paragraph_properties)
	{
		style_drop_cap *drop_cap = dynamic_cast<style_drop_cap*>(drop_cap_state_.paragraph_properties->content_.style_drop_cap_.get());
		if (drop_cap)
		{
			drop_cap->style_length_ = drop_cap_length(drop_cap_state_.characters);
		}
		if (drop_cap_state_.inline_style == false)
		{
			//сдвинуть первую строку - так чтоб буквица вся (что поместится) была на поле
			//double indent_pt = 0;
			//double indent_percent = 0;
			//if (drop_cap_state_.paragraph_properties->content_.fo_text_indent_)
			//{
			//	if ( drop_cap_state_.paragraph_properties->content_.fo_text_indent_->get_type() == length_or_percent::Length)
			//		indent_pt = drop_cap_state_.paragraph_properties->content_.fo_text_indent_->get_length().get_value_unit(length::pt);
			//	else
			//		indent_percent = drop_cap_state_.paragraph_properties->content_.fo_text_indent_->get_percent()->get_value();
			//}
			
			drop_cap_state_.paragraph_properties->content_.fo_text_indent_ = length(length(-drop_cap_state_.characters_size_pt,length::pt).get_value_unit(length::cm),length::cm);
			//drop_cap_state_.characters * size_char;
		}
	}

	drop_cap_state_.clear();
}
///////////////////////////////////////
void odt_conversion_context::start_table(bool styled)
{
	office_element_ptr elm;
	create_element(L"table", L"table",elm,this);

	table_context()->start_table(elm, styled);

	office_element_ptr	style_elm;
	std::wstring		style_name;
	if (styled)
	{
		odf_style_state_ptr style_state = styles_context()->last_state(style_family::Table);
		if (style_state)
		{
			style_elm = style_state->get_office_element();
			style_name = style_state->get_name();
		}
	}
	text_context()->start_element(elm, style_elm, style_name);

	add_to_root();
}
void odt_conversion_context::start_table_columns()
{
	office_element_ptr elm;
	create_element(L"table", L"table-columns", elm, this);

	text_context()->start_element(elm);
}
void odt_conversion_context::add_table_column(double width)
{
	office_element_ptr elm;
	create_element(L"table", L"table-column", elm, this);

	styles_context()->create_style(L"", style_family::TableColumn, true, false, -1);

	//не срабатывает ..
	//std::wstring parent_name = table_context()->get_default_cell_properties();

	//if (parent_name.length() > 0) 
	//{
	//	odf_writer::style_table_cell_properties * props = styles_context()->last_state().get_table_cell_properties();
	//	style * style_ = NULL;
	//	
	//	if (styles_context()->find_odf_style(parent_name,style_family::TableCell,style_))
	//	{
	//		style_table_cell_properties * parent = style_->content_.get_style_table_cell_properties();
	//		props->apply_from(parent);
	//	}
	//}

	table_context()->add_column(elm, true);
		table_context()->set_column_width(width);

	text_context()->start_element(elm); // для связи элментов
	text_context()->end_element();
}
void odt_conversion_context::end_table_columns()
{
	text_context()->end_element();
}
void odt_conversion_context::start_table_header_rows()
{
	office_element_ptr elm;
	create_element(L"table", L"table-header-rows", elm, this);

	text_context()->start_element(elm);
}
void odt_conversion_context::end_table_header_rows()
{
	text_context()->end_element();
}
void odt_conversion_context::start_table_row(bool styled, bool header)
{
	office_element_ptr elm;
	create_element(L"table", L"table-row", elm, this);

	if (header && !table_row_header_state_)
	{
		start_table_header_rows();
		table_row_header_state_ = true;
	}

	if (!header && table_row_header_state_)
	{
		end_table_header_rows();
		table_row_header_state_ = false;
	}

	if (styled)
	{
		styles_context()->create_style(L"", odf_types::style_family::TableRow, true, false, -1);
	}
	text_context()->start_element(elm);

	table_context()->start_row(elm, styled);

}
void odt_conversion_context::start_table_cell(int col, bool covered, bool styled)
{
	for (int i = table_context()->current_column(); i < col;  i++)
	{
		add_default_cell();
	}
//-------------------------------------------------------
	office_element_ptr elm;
	if (covered)
		create_element(L"table", L"covered-table-cell",elm,this);
	else
		create_element(L"table", L"table-cell",elm,this);


	table_context()->start_cell(elm, styled);
	text_context()->start_element(elm);
}
void odt_conversion_context::end_table_cell()
{
	set_no_list();
	table_context()->end_cell();
	text_context()->end_element();
}
void odt_conversion_context::end_table_row()
{
	for (int i = table_context()->current_column() ; i < table_context()->count_columns(); i++)
	{
		add_default_cell();
	}
//---------------------------------------------
	table_context()->end_row();
	text_context()->end_element();
}

void odt_conversion_context::add_default_cell()
{
	office_element_ptr elm;
	create_element(L"table", L"covered-table-cell",elm,this);

	table_context()->start_cell(elm, false);
	text_context()->start_element(elm);

	table_context()->end_cell();
	text_context()->end_element();
}
void odt_conversion_context::end_table()
{
	table_context()->end_table();
	text_context()->end_element();
}


bool odt_conversion_context::start_header(int type)
{
    if (page_layout_context()->add_header(type) == false)   return false;
    if (page_layout_context()->last_master() == NULL)       return false;

	start_text_context();
	text_context()->set_styles_context(page_layout_context()->get_local_styles_context());

    text_context()->start_element(page_layout_context()->last_master()->get_last_element());

	is_header_ = true;

	return true;
}

void odt_conversion_context::end_header_footer()
{
	text_context()->end_element();
	end_text_context();

	is_header_ = false;
	is_footer_ = false;
}

void odt_conversion_context::add_empty_header(int type)
{
	if (start_header(type))
	{
		start_paragraph(false);
		end_paragraph();
	}
	end_header_footer();
}
void odt_conversion_context::add_empty_footer(int type)
{
	if (start_footer(type))
	{
		start_paragraph(false);
		end_paragraph();
	}
	end_header_footer();
}


void odt_conversion_context::set_background(_CP_OPT(color) & color, int type)
{
	if (!color) return;

	page_layout_context()->set_background(color, type);
}

bool odt_conversion_context::start_footer(int type)
{
    if (page_layout_context()->add_footer(type) == false)   return false;
    if (page_layout_context()->last_master() == NULL)       return false;

	start_text_context();
	text_context()->set_styles_context(page_layout_context()->get_local_styles_context());
	
    text_context()->start_element(page_layout_context()->last_master()->get_last_element());

 	is_footer_ = true;

	return true;
}


}
}
