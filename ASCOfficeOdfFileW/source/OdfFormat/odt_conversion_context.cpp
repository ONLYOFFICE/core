/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../utils.h"

#include "odt_conversion_context.h"
#include "odf_text_context.h"

#include "styles.h"

#include "style_paragraph_properties.h"
#include "style_table_properties.h"
#include "style_section_properties.h"
#include "style_text_properties.h"

#include "office_text.h"
#include "paragraph_elements.h"
#include "text_elements.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

namespace utils
{

double calculate_size_font_symbols(std::wstring str_test, std::wstring font_name, double font_size, CApplicationFonts *appFonts)
{
    double appr_px = _graphics_utils_::calculate_size_symbol_asc(font_name, font_size, false, false, appFonts);
	
	if (appr_px <0.01)
	{
        appr_px = _graphics_utils_::calculate_size_symbol_win(font_name, font_size, false, false, str_test);
		//appr_px = ((int)(appr_px+0.5) + 2*(int)appr_px)/3.;
	}

	return appr_px*0.55;
}
}
odt_conversion_context::odt_conversion_context(package::odf_document * outputDocument) 
		: odf_conversion_context(outputDocument),comment_context_(this), main_text_context_(NULL),table_context_(this)	
{
	current_field_.enabled = false;
	current_field_.started = false;
	current_field_.in_span = false;
	
	is_hyperlink_		= false;

	is_header_	= false;
	is_footer_	= false;

	is_paragraph_in_current_section_ = false;

	drop_cap_state_.clear();
}

odt_conversion_context::~odt_conversion_context()
{
	if (main_text_context_)	delete main_text_context_;
}

void odt_conversion_context::start_document()
{
	start_text();
	
	root_document_ = get_current_object_element();
	root_text_ = dynamic_cast<office_text*>(root_document_.get());
	main_text_context_ = new odf_text_context(this); 

	page_layout_context()->set_styles_context(styles_context());

	//current_level_.push_back(get_current_object_element());

/////////////////настройки дефолтовые

	root_text_->office_text_attlist_.text_use_soft_page_breaks_ = true;
}

void odt_conversion_context::end_document()
{
	//add sections to root
	for (long i=0; i< sections_.size(); i++)
	{
		root_document_->add_child_element(sections_[i].elm);
	}
	sections_.clear();
	
	//add last elements to root 
	for (long i=0; i< current_root_elements_.size(); i++)
	{
		root_document_->add_child_element(current_root_elements_[i].elm);
	}
	current_root_elements_.clear();
	
	odf_conversion_context::end_document();
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
void odt_conversion_context::start_text_context()
{
	odf_text_context_ptr new_text_context_ = boost::shared_ptr<odf_text_context>(new odf_text_context(this));
	if (!new_text_context_)return;

	text_context_.push_back(new_text_context_);
}
void odt_conversion_context::end_text_context()
{
	if (text_context_.size() > 0)
	{
		text_context_.pop_back();
	}
}
void odt_conversion_context::add_text_content(const std::wstring & text)
{
	if (drop_cap_state_.enabled)
	{
		int count = text.size();
		drop_cap_state_.characters += count;
		if (drop_cap_state_.inline_style == false)
		{
			style_text_properties * props = text_context()->get_text_properties();
			if (props)
			{
				std::wstring f_name = props->content().fo_font_family_.get_value_or(L"Arial");
				double f_size = props->content().fo_font_size_.get_value_or(font_size(length(12,length::pt))).get_length().get_value_unit(length::pt);
				
                drop_cap_state_.characters_size_pt += utils::calculate_size_font_symbols(text, f_name, f_size, applicationFonts_);
			}
		}
	}

	text_context()->add_text_content(text);
}
void odt_conversion_context::add_to_root()
{
	if (text_context_.size() >0) return; // не root element (embedded) ????

	if (comment_context_.is_started()) return;

	if (text_context()->current_level_.size() > 1) return;

	odf_element_state & state = text_context()->current_level_.back();
	current_root_elements_.push_back(state);
}
void odt_conversion_context::start_drawings()
{
	odf_drawing_context_ptr new_drawing_context_ = boost::shared_ptr<odf_drawing_context>(new odf_drawing_context(this));
	if (!new_drawing_context_)return;
	
	new_drawing_context_->set_styles_context(styles_context());

	new_drawing_context_->set_footer_state(is_footer_);
	new_drawing_context_->set_header_state(is_header_);

	drawing_context_.push_back(new_drawing_context_);
}
void odt_conversion_context::end_drawings()
{
	if (drawing_context_.size() < 1) return;

	office_element_ptr & elm = drawing_context()->get_root_element();

	if (elm && text_context()->current_level_.size() > 0)//add to 'p' or 'h' !!!!!
	{
		anchor_type::type anchor = drawing_context()->get_anchor();

		bool bSet = false;
		if ( anchor == anchor_type::Page || anchor == anchor_type::Paragraph)
		{
			for (long i = text_context()->current_level_.size()-1; i>=0; i--)
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
		if (!bSet) text_context()->current_level_.back().elm->add_child_element(elm);
		
		drawing_context()->clear();
		drawing_context_.pop_back();
	}
	else
	{		
		text_context()->start_element(elm);
		text_context()->end_element();

		drawing_context()->clear();
		drawing_context_.pop_back();
	}
}
void odt_conversion_context::start_paragraph(bool styled)
{
	text_context()->start_paragraph(styled);	
	
	add_to_root();
}
void odt_conversion_context::add_page_break()
{
	office_element_ptr elm;
	create_element(L"text", L"soft-page-break", elm, this);	
	
	if (current_root_elements_.size()>0/* && text_context()->is_need_break()*/)			
	{ 	
		text_p* para = NULL;
		style * style_ = NULL;
		//http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#__RefHeading__1415190_253892949
		//нихере не работает !! в span, ... приходится генерить разрыв вручную !!

		//if (para = dynamic_cast<text_p*>(current_root_elements_.back().elm.get()))
		//{
		//	style_ = dynamic_cast<style*>(current_root_elements_.back().style_elm.get());//для переноса свойств параграфа
		//	end_paragraph();
		//}	
		//else/* if (header = dynamic_cast<text_h*>(current_root_elements_.back().elm.get()))*/
		{
			//тут получается что разрыв будет прописан внутри элемента (не параграфа) - так что вручную свойство запишем
			//в случае разрыва параграфов оно запишется при старте после-разрывного параграфа
			text_context()->save_property_break();
		}
		text_context()->start_element(elm);
		add_to_root();
		text_context()->end_element();

		if (para)
		{
			styles_context()->create_style(L"",odf_types::style_family::Paragraph, true, false, -1);	
			//styles_context()->last_state().apply_from(style_);
			if (style_ )
			{
				style_paragraph_properties * new_props = NULL;
				if (styles_context()->last_state())
				{// нужна именно копия св-в так как будет добавочные свойства
					new_props = styles_context()->last_state()->get_paragraph_properties();				
					if (new_props)new_props->apply_from(style_->style_content_.get_style_paragraph_properties());
					
					if (style_->style_parent_style_name_)
						styles_context()->last_state()->set_parent_style_name(*style_->style_parent_style_name_);
				}
			}	
			start_paragraph(true);
		}
	}
}
void odt_conversion_context::start_hyperlink(std::wstring ref)
{
	office_element_ptr hyperlink_elm;
	create_element(L"text", L"a",hyperlink_elm,this);

	text_a* hyperlink = dynamic_cast<text_a*>(hyperlink_elm.get());
	if (!hyperlink)return;

////////////////////////////

	hyperlink->common_xlink_attlist_.href_ = ref;
	hyperlink->common_xlink_attlist_.type_= xlink_type::Simple;
	
	//current_level_.back()->add_child_element(hyperlink_elm);
	//current_level_.push_back(hyperlink_elm);
	
	text_context()->start_element(hyperlink_elm);

	is_hyperlink_ = true;
}
void odt_conversion_context::end_hyperlink()
{
	if (!is_hyperlink_) return;

	//current_level_.pop_back();
	text_context()->end_element();

	is_hyperlink_ = false; //метка .. для гиперлинков в объектах - там не будет span
}

void odt_conversion_context::set_field_instr(std::wstring instr)
{
	if (current_field_.enabled == false) 	return;

	int res1 = instr.find(L"HYPERLINK");
	if (res1 >=0)							//это не поле - это hyperlink
	{
		current_field_.type = 1;
		
		std::wstring ref;
		boost::match_results<std::wstring::const_iterator> res;
		boost::wregex r2 (L"(\".*?\")+");	
        if (boost::regex_search(instr, res, r2))
        {
            ref = res[1].str();
			current_field_.value = ref.substr(1, ref.length()-2);

        }
	}
	res1 = instr.find(L"NUMPAGES");
	if (res1 >=0 && current_field_.type == 0)
	{
		current_field_.type = 3;
	}	
	res1 = instr.find(L"PAGEREF");
	if (res1 >=0 && current_field_.type == 0 )	//это не поле - это bookmark
	{
		current_field_.type = 5;
		if (instr.length() > 9)
			current_field_.value = instr.substr(9, instr.length()-5);
	}
	res1 = instr.find(L"PAGE");
	if (res1 >=0 && current_field_.type == 0)
	{
		current_field_.type = 2;
	}
	res1 = instr.find(L"TIME");
	if (res1 >=0 && current_field_.type == 0)
	{
		current_field_.type = 4;
	}
	res1 = instr.find(L"BIBLIOGRAPHY");
	if (res1 >=0 && current_field_.type == 0)
	{
		current_field_.type = 6;
	}
////////////////////////////////////////// 
	res1 = instr.find(L"@");
	if (res1 >=0)
	{
		current_field_.format = instr.substr(res1+1, instr.length());
	}
}
void odt_conversion_context::start_field(bool in_span)
{
	current_field_.enabled = true;

	current_field_.in_span = in_span;
	current_field_.value = L"";
	current_field_.type = 0;
}

void odt_conversion_context::set_master_page_name(std::wstring master_name)
{
	if (current_root_elements_.size() < 1)// return; - эффект_штурмовика.docx - 1 страница !! (и ваще - 
	{
		is_paragraph_in_current_section_ = true;
		return;
	}

	is_paragraph_in_current_section_ = false;
	style *style_ = dynamic_cast<style*>(current_root_elements_[0].style_elm.get());

	if (style_)style_->style_master_page_name_ = master_name;
}
int odt_conversion_context::get_current_section_columns()
{
	if (sections_.size() > 0)
		sections_.back().count_columns;
	else return 1;
}
void odt_conversion_context::add_section(bool continuous)
{
	odt_section_state state;
	state.empty = true;
	state.continuous = continuous;
//----------------------------------------------------------------
	styles_context()->create_style(L"",odf_types::style_family::Section, true, false, -1);		

	create_element(L"text", L"section",state.elm,this);
	state.style_elm		= styles_context()->last_state()->get_office_element();
	state.style_name	= styles_context()->last_state()->get_name();
	
	text_section* section = dynamic_cast<text_section*>(state.elm.get());
	if (section)section->text_section_attr_.text_style_name_ = style_ref(state.style_name);	

	sections_.push_back(state);
}
void odt_conversion_context::add_section_columns(int count, double space_pt, bool separator )
{
	if (sections_.size() < 1 || count < 1) return;

	style* style_ = dynamic_cast<style*>(sections_.back().style_elm.get());
	if (!style_)return;

	style_section_properties	* section_properties	= style_->style_content_.get_style_section_properties();
	
	create_element(L"style", L"columns",section_properties->style_columns_,this);	
	style_columns* columns = dynamic_cast<style_columns*>(section_properties->style_columns_.get());
	if (!columns)return;
	sections_.back().count_columns = count;

						columns->fo_column_count_	= count;
	if (space_pt >= 0)	columns->fo_column_gap_		= length(length(space_pt,length::pt).get_value_unit(length::cm),length::cm);

	if (separator)
	{
		create_element(L"style", L"column-sep",columns->style_column_sep_,this);
		style_column_sep* sep = dynamic_cast<style_column_sep*>(columns->style_column_sep_.get());
		if (sep)//default set
		{
			sep->style_width_			= length(0,length::cm);
			sep->style_height_			= percent(100);
			sep->style_vertical_align_	= vertical_align(vertical_align::Middle);
			sep->style_color_			= color(L"#000000");	
		}
	}
}
void odt_conversion_context::add_section_column(std::vector<std::pair<double,double>> width_space)
{
	if (sections_.size() < 1 || width_space.size() < 1) return;

	style* style_ = dynamic_cast<style*>(sections_.back().style_elm.get());
	if (!style_)return;

	style_section_properties	* section_properties	= style_->style_content_.get_style_section_properties();
	//section_properties->text_dont_balance_text_columns_ = true;
	
	style_columns* columns = dynamic_cast<style_columns*>(section_properties->style_columns_.get());
	if (!columns)return;
	
	double width_all = 0;
	for (int i = 0; i < width_space.size() ; i++)
	{
		if (width_space[i].first >= 0) 
		
			width_all += width_space[i].first/* + width_space[i].second*/;
	}

	double curr = 0;
	int width_absolute = 0;

	if (width_all < 1)	return;

	section_properties->style_editable_ = false;

	for (int i = 0; i < width_space.size() && width_all > 0 ; i++)
	{
		office_element_ptr col_elm;
		
		create_element(L"style", L"column", col_elm, this);
		style_column* col = dynamic_cast<style_column*>(col_elm.get());
		if (!col) continue;

		int val =  (width_space[i].first/* + width_space[i].second*/)* 65535. /width_all /*:65535 - width_absolute*/;
		col->style_rel_width_ = odf_types::percent_rel(val);
		width_absolute += val;
		
		//col->fo_start_indent_ = odf_types::length(curr,odf_types::length::pt);
		//curr += width_space[0].first;
		//
		//col->fo_end_indent_ = odf_types::length(curr,odf_types::length::pt);
		//curr += width_space[0].second;
		
		columns->add_child_element(col_elm);

	}
}
void odt_conversion_context::end_field()
{
	if (current_field_.enabled && current_field_.started)	
	{
		if (current_field_.type == 1) end_hyperlink();
		else text_context()->end_field();
	}
	current_field_.value	= L"";
	current_field_.format	= L"";

	current_field_.enabled = false;
	current_field_.started = false;
	current_field_.in_span = false;
}
void odt_conversion_context::end_paragraph()
{
	text_context()->end_paragraph();
	
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
	level = level +1; //отсчет от 1 (а не 0)

	if (text_context()->list_state_.style_name != style_name && text_context()->list_state_.started_list)
	{
		set_no_list();
	}

	while (text_context()->list_state_.levels.size() > level)
	{
		text_context()->end_list();
	}

	if (text_context()->list_state_.levels.size() < 1)
	{
		text_context()->list_state_.started_list = false;
		text_context()->list_state_.style_name = L"";
	}

	if (text_context()->list_state_.started_list == false)
	{
		text_context()->start_list(style_name);
		//text_context()->set_list_continue(true); //??? держать в памяти все списки????
		add_to_root();
		
	}
	text_context()->start_list_item();

	if (text_context()->list_state_.style_name == style_name)
		style_name = L"";

	while (text_context()->list_state_.levels.size() < level)
	{
		text_context()->start_list(style_name);
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

	while (text_context()->list_state_.levels.size()>0)
	{
		text_context()->end_list_item();
		text_context()->end_list();	
	}
	text_context()->list_state_.started_list = false;
	text_context()->list_state_.style_name = L"";
}
void odt_conversion_context::flush_section()
{
	if (sections_.size() > 0 && sections_.back().empty)
	{
		for (long i=0; i< current_root_elements_.size(); i++)
		{
			if ((sections_.back().continuous && i<2) || !sections_.back().continuous)
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
					//	style_paragraph_properties *props = style_->style_content_.get_style_paragraph_properties();
					//	props->content().fo_break_before_ = fo_break(fo_break::Auto);
					//	props->content().fo_break_after_  = fo_break(fo_break::Auto);
					//}
					continue;
				}
			}
			sections_.back().elm->add_child_element(current_root_elements_[i].elm);
		}
		current_root_elements_.clear();

		sections_.back().empty = false;
	}
}
void odt_conversion_context::start_run(bool styled)
{
	if (is_hyperlink_ && text_context_.size() > 0) return;
	

	if (current_field_.started== false && current_field_.type >1 && current_field_.enabled ==true && !current_field_.in_span)
	{
		text_context()->start_field(current_field_.type);
		current_field_.started = true;
	}	
	
	text_context()->start_span(styled);

	if (current_field_.started== false && current_field_.type >1 && current_field_.enabled ==true && current_field_.in_span)//поле стартуется в span - нужно для сохранения стиля
	{
		text_context()->start_field(current_field_.type);
		current_field_.started = true;
	}	
}
void odt_conversion_context::end_run()
{
	if (is_hyperlink_ && text_context_.size() > 0) return;

	if (current_field_.in_span && current_field_.started== true && current_field_.enabled ==true) end_field();
	
	text_context()->end_span();

	if (current_field_.started== false && current_field_.type == 1 && current_field_.enabled ==true)
	{
		start_hyperlink(current_field_.value);
		current_field_.started = true;
	}
}
bool odt_conversion_context::start_comment(int oox_comm_id)
{
	int comm_state = comment_context_.find_by_id(oox_comm_id);

	if (comm_state == 0)
	{
		office_element_ptr comm_elm;
		create_element(L"office", L"annotation",comm_elm,this);

		comment_context_.start_comment(comm_elm, oox_comm_id);
		
		if (text_context()->current_level_.size() > 0)
			text_context()->current_level_.back().elm->add_child_element(comm_elm);

		odf_element_state state={comm_elm, L"", office_element_ptr(),(int)text_context()->current_level_.size()};
		text_context()->current_level_.push_back(state);

		return false; //типо новый
	}
	return true;
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
	bool added = comment_context_.find_by_id(oox_comm_id);

	if (added == true)
	{
		office_element_ptr comm_elm;
		create_element(L"office", L"annotation-end",comm_elm,this);

		comment_context_.end_comment(comm_elm, oox_comm_id);
		
		if (text_context()->current_level_.size() > 0)
			text_context()->current_level_.back().elm->add_child_element(comm_elm);
	}
}
void odt_conversion_context::start_image(const std::wstring & image_file_name)
{
	std::wstring odf_ref_name ;
	
	mediaitems()->add_or_find(image_file_name,_mediaitems::typeImage,odf_ref_name);

	drawing_context()->start_image(odf_ref_name);
}

void odt_conversion_context::start_drop_cap(style_paragraph_properties *paragraph_properties)
{
	if (drop_cap_state_.enabled) end_drop_cap(); // 2 подряд ваще возможны ???

	if (paragraph_properties == NULL) return;

	drop_cap_state_.enabled = true;
	drop_cap_state_.paragraph_properties = paragraph_properties;

	office_element_ptr comm_elm;
	create_element(L"style", L"drop-cap",drop_cap_state_.paragraph_properties->content().style_drop_cap_,this);
}

void odt_conversion_context::set_drop_cap_lines(int lines)
{
	if (!drop_cap_state_.enabled) return;
	if (!drop_cap_state_.paragraph_properties) return;

	style_drop_cap *drop_cap = dynamic_cast<style_drop_cap*>(drop_cap_state_.paragraph_properties->content().style_drop_cap_.get());
	if (drop_cap)drop_cap->style_lines_ = lines;
}
void odt_conversion_context::set_drop_cap_margin(bool val)
{
	if (!drop_cap_state_.enabled) return;
	drop_cap_state_.inline_style = !val;
}
void odt_conversion_context::end_drop_cap()
{
	if (!drop_cap_state_.enabled) return;

	if (drop_cap_state_.characters >0 && drop_cap_state_.paragraph_properties)
	{
		style_drop_cap *drop_cap = dynamic_cast<style_drop_cap*>(drop_cap_state_.paragraph_properties->content().style_drop_cap_.get());
		if (drop_cap)
		{
			drop_cap->style_length_ = drop_cap_length(drop_cap_state_.characters);
		}
		if (drop_cap_state_.inline_style == false)
		{
			//сдвинуть первую строку - так чтоб буквица вся (что поместится) была на поле
			//double indent_pt = 0;
			//double indent_percent = 0;
			//if (drop_cap_state_.paragraph_properties->content().fo_text_indent_)
			//{
			//	if ( drop_cap_state_.paragraph_properties->content().fo_text_indent_->get_type() == length_or_percent::Length)
			//		indent_pt = drop_cap_state_.paragraph_properties->content().fo_text_indent_->get_length().get_value_unit(length::pt);
			//	else
			//		indent_percent = drop_cap_state_.paragraph_properties->content().fo_text_indent_->get_percent()->get_value();
			//}
			
			drop_cap_state_.paragraph_properties->content().fo_text_indent_ = length(length(-drop_cap_state_.characters_size_pt,length::pt).get_value_unit(length::cm),length::cm);
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
	text_context()->start_element(elm,style_elm,style_name);

	add_to_root();
}
void odt_conversion_context::start_table_columns()
{
	office_element_ptr elm;
	create_element(L"table", L"table-columns",elm,this);

	text_context()->start_element(elm);
}
void odt_conversion_context::add_table_column(double width)
{
	office_element_ptr elm;
	create_element(L"table", L"table-column",elm,this);

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
	//		style_table_cell_properties * parent = style_->style_content_.get_style_table_cell_properties();
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
	create_element(L"table", L"table-header-rows",elm,this);

	text_context()->start_element(elm);
}
void odt_conversion_context::end_table_header_rows()
{
	text_context()->end_element();
}
void odt_conversion_context::start_table_row(bool styled)
{
	office_element_ptr elm;
	create_element(L"table", L"table-row",elm,this);

	if (styled)
	{
		styles_context()->create_style(L"",odf_types::style_family::TableRow, true, false, -1);
	}
	text_context()->start_element(elm);

	table_context()->start_row(elm, styled);

}
void odt_conversion_context::start_table_cell(int col, bool covered, bool styled)
{
	for (int i=table_context()->current_column() ; i < col;  i++)
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
	for (int i=table_context()->current_column() ; i < table_context()->count_columns(); i++)
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
