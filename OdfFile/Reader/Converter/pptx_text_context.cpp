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


#include "pptx_conversion_context.h"
#include "pptx_text_context.h"

#include <iostream>
#include <list>

#include <xml/simple_xml_writer.h>
#include <xml/utils.h>

#include "../Format/odf_document.h"
#include "../Format/odfcontext.h"
#include "../Format/calcs_styles.h"

#include "../Format/style_text_properties.h"

namespace cpdoccore {
namespace oox {

class pptx_text_context::Impl: boost::noncopyable
{
public:
	Impl(odf_reader::odf_read_context & odf_context_,	pptx_conversion_context & pptx_context_);
public:
	styles_context & get_styles_context() { return styles_context_; }

	void add_text(const std::wstring & text);
	void add_paragraph(const std::wstring & para);
    
    void start_paragraph(const std::wstring & styleName);
    void end_paragraph	();

    void start_span			(const std::wstring & styleName);
    void end_span			();
    std::wstring end_span2	();

	void			start_object();
	std::wstring	end_object	();

	void start_base_style(const std::wstring baseStyleName, const odf_types::style_family::type baseStyleType);
	void end_base_style();

	void ApplyTextProperties		(std::wstring style, std::wstring para_style, odf_reader::text_format_properties_content & propertiesOut);
	void ApplyParagraphProperties	(std::wstring para_style, odf_reader::paragraph_format_properties & propertiesOut);
	void ApplyListProperties		(odf_reader::paragraph_format_properties & propertiesOut, int Level);

	void set_local_styles_container(odf_reader::styles_container*  local_styles_);//это если стили объектов содержатся в другом документе

	void end_hyperlink	(std::wstring hId);
	void start_hyperlink();

    void start_list		(const std::wstring & StyleName, bool Continue = false);
    void end_list		();
    void start_list_item(bool restart = false);
	void end_list_item	();

	void start_field	(field_type type, const std::wstring & styleName);
    void end_field		();

	void start_comment		();
    std::wstring end_comment();

	bool in_list_;
	bool process_layouts_;

private:
    styles_context styles_context_;

	odf_reader::odf_read_context & odf_context_ ;
	std::wstring hyperlink_hId;
	
	bool in_span;
	bool in_paragraph;
	bool in_comment;

	odf_reader::styles_container * local_styles_ptr_;

    void write_rPr	(std::wostream & strm);
	void write_pPr	(std::wostream & strm);
	void write_t	(std::wostream & strm);
   
    std::wstring	dump_paragraph();
	void			dump_run();
	void			dump_field();
  
	size_t paragraphs_cout_; //???? тока из за начала отсчета?
   
	std::wstringstream text_;		//приходящий текст
    std::wstringstream paragraph_;	//перманенто скидываемые параграфы
    std::wstringstream run_;		//перманенто скидываемые куски с быть может разными свойствами
   
	std::wstring		paragraph_style_name_;
    std::wstring		span_style_name_;

	std::wstring					base_style_name_;
	odf_types::style_family::type	base_style_family_;//Presentation Or SpreadSheet
//-------------------------------------------------------------------------------
    std::vector<std::wstring> list_style_stack_;
    bool first_element_list_item_;
    
    int new_list_style_number_;	// счетчик для нумерации имен созданных в процессе конвертации стилей
   
    boost::unordered_map<std::wstring, std::wstring> list_style_renames_; // цепочки переименований нумераций
   
	void write_list_styles(std::wostream & strm);
	//void write_list_properties(std::wostream & strm);
	std::wstring find_list_rename(const std::wstring & ListStyleName);
	std::wstring current_list_style();
///////////////////////////

	field_type field_type_;
	std::wstringstream field_value_;

	pptx_conversion_context & pptx_context_;

};

pptx_text_context::Impl::Impl(odf_reader::odf_read_context & odf_contxt_, pptx_conversion_context & pptx_contxt_): 
		odf_context_(odf_contxt_),	pptx_context_(pptx_contxt_),
		paragraphs_cout_(0), in_paragraph(false),in_span(false), in_comment(false), field_type_(none)
{
	new_list_style_number_=0;
	local_styles_ptr_ = NULL;
	process_layouts_ = false;
}

void pptx_text_context::Impl::add_text(const std::wstring & text)
{
	if (field_type_)
		field_value_ << text;
	else
		text_ << text;
}
void pptx_text_context::Impl::add_paragraph(const std::wstring & para)
{
	paragraph_ << para;
}
void pptx_text_context::Impl::set_local_styles_container(odf_reader::styles_container * local_styles_)
{
	local_styles_ptr_= local_styles_;
}

void pptx_text_context::Impl::start_paragraph(const std::wstring & styleName)
{
    if (paragraphs_cout_++ > 0)
    {	
		//if (in_list_ == false || in_comment == true)
		//{
		//// конец предыдущего абзаца и начало следующего
		////text_ << L"&#10;";
		//	text_ << L"\n"; 
		//}
		//else/* (paragraph_style_name_ != styleName)*/
		{
			dump_paragraph();
		}
	}else
	{
		text_.str(std::wstring());
		field_value_.str(std::wstring());
	}
	paragraph_style_name_	= styleName;
	in_paragraph			= true;
}

void pptx_text_context::Impl::end_paragraph()
{
	in_paragraph = false;
}

void pptx_text_context::Impl::start_span(const std::wstring & styleName)//кусок текста в абзаце(параграфе) со своими свойствами - этто может быть и 1 буква
{
	int text_size = text_.str().length();
	
	if ((span_style_name_ !=styleName && text_size > 0) || in_span)
	{
		dump_run();
	}

	span_style_name_ = styleName;

	in_span=true;
}

void pptx_text_context::Impl::end_span() 
{
    dump_run();
    span_style_name_ = L"";
	
	in_span = false;
}

std::wstring pptx_text_context::Impl::end_span2()
{
    dump_run();
    span_style_name_ = L"";
	
	in_span = false;
    return run_.str();
}
void pptx_text_context::Impl::start_hyperlink()
{
	dump_run();//проверить
}

void pptx_text_context::Impl::end_hyperlink(std::wstring hId)
{
	hyperlink_hId = hId;
	dump_run();
	hyperlink_hId = L"";
}
void pptx_text_context::Impl::ApplyTextProperties(std::wstring style_name, std::wstring para_style_name, odf_reader::text_format_properties_content & propertiesOut)
{
	std::vector<const odf_reader::style_instance *> instances;

	odf_reader::style_instance* defaultStyle	= NULL;
	odf_reader::style_instance* para_style		= NULL;
	odf_reader::style_instance* text_style		= NULL;
	odf_reader::style_instance* baseStyle		= NULL;
	
	if (local_styles_ptr_)
	{
		para_style		= local_styles_ptr_->style_by_name			(para_style_name, odf_types::style_family::Paragraph, false/*process_headers_footers_*/);
		text_style		= local_styles_ptr_->style_by_name			(style_name, odf_types::style_family::Text, false/*process_headers_footers_*/);
		defaultStyle	= local_styles_ptr_->style_default_by_type	(odf_types::style_family::Text);
		baseStyle		= local_styles_ptr_->style_by_name			(base_style_name_, base_style_family_, false/*process_headers_footers_*/);
	}
	else
	{
		para_style		= odf_context_.styleContainer().style_by_name			(para_style_name, odf_types::style_family::Paragraph, false/*process_headers_footers_*/);
		text_style		= odf_context_.styleContainer().style_by_name			(style_name, odf_types::style_family::Text, false/*process_headers_footers_*/);
		defaultStyle	= odf_context_.styleContainer().style_default_by_type	(odf_types::style_family::Text);
		baseStyle		= odf_context_.styleContainer().style_by_name			(base_style_name_, base_style_family_, false/*process_headers_footers_*/);
	}
	if	(defaultStyle)	instances.push_back(defaultStyle);
	if	(baseStyle)		instances.push_back(baseStyle);
	if	(para_style)	instances.push_back(para_style);
	if	(text_style)	instances.push_back(text_style);
	
	if		(text_style)	get_styles_context().start_process_style(text_style);
	else if (para_style)	get_styles_context().start_process_style(para_style);
	else					get_styles_context().start_process_style(baseStyle);

	odf_reader::text_format_properties_content_ptr text_props = calc_text_properties_content(instances);
	if (text_props)
	{
		propertiesOut.apply_from(*text_props.get());
	}
}

void pptx_text_context::Impl::ApplyListProperties(odf_reader::paragraph_format_properties & propertiesOut, int Level)
{
	if (Level < 0)return;
	if (list_style_stack_.empty())return;
	
	odf_reader::style_list_level_properties	*list_properties = NULL;

	odf_reader::text_list_style * text_list_style = odf_context_.listStyleContainer().list_style_by_name(list_style_stack_.back());
	
	if ((text_list_style) && (Level < (int)text_list_style->content_.size()))
	{
		odf_reader::office_element_ptr  elm = text_list_style->content_[Level];
		odf_reader::office_element_ptr  elm_list;

		if (elm->get_type() == typeTextListLevelStyleBullet)
		{
			odf_reader::text_list_level_style_bullet* list_bullet = dynamic_cast<odf_reader::text_list_level_style_bullet *>(elm.get());
			if (list_bullet)elm_list = list_bullet->list_level_properties_;
		}
		if (elm->get_type() == typeTextListLevelStyleNumber)
		{
			odf_reader::text_list_level_style_number* list_number = dynamic_cast<odf_reader::text_list_level_style_number *>(elm.get());
			if (list_number)elm_list = list_number->list_level_properties_;
		}
		if (elm->get_type() == typeTextListLevelStyleImage)
		{
			odf_reader::text_list_level_style_image* list_image = dynamic_cast<odf_reader::text_list_level_style_image *>(elm.get());
			if (list_image)elm_list = list_image->list_level_properties_;
		}
		////////////////////
		if (elm_list)
		{
			list_properties = dynamic_cast<odf_reader::style_list_level_properties	*>(elm_list.get());
		}
		
		elm->pptx_convert(pptx_context_);
	}
	if (list_properties)
	{
		propertiesOut.fo_text_indent_ = list_properties->text_min_label_width_;
		if (list_properties->text_space_before_)
		{
			double spaceBeforeTwip = list_properties->text_space_before_->get_value_unit(odf_types::length::pt);
			if (list_properties->text_min_label_width_)
			{
				spaceBeforeTwip += list_properties->text_min_label_width_->get_value_unit(odf_types::length::pt);
			}
			if (spaceBeforeTwip>0)
				propertiesOut.fo_margin_left_ = odf_types::length(spaceBeforeTwip,odf_types::length::pt);
		}
	}
	
}

void pptx_text_context::Impl::ApplyParagraphProperties(std::wstring style_name, odf_reader::paragraph_format_properties & propertiesOut)
{
	std::vector<const odf_reader::style_instance *> instances;

	odf_reader::style_instance* style			= NULL;
	odf_reader::style_instance* defaultStyle	= NULL;
	odf_reader::style_instance* baseStyle		= NULL;
	
	if (local_styles_ptr_)
	{
		style			= local_styles_ptr_->style_by_name			(style_name, odf_types::style_family::Paragraph, false/*process_headers_footers_*/);
		defaultStyle	= local_styles_ptr_->style_default_by_type	(odf_types::style_family::Paragraph);
		baseStyle		= local_styles_ptr_->style_by_name			(base_style_name_, base_style_family_, false/*process_headers_footers_*/);
	}
	else
	{
		style			= odf_context_.styleContainer().style_by_name			(style_name, odf_types::style_family::Paragraph, false/*process_headers_footers_*/);
		defaultStyle	= odf_context_.styleContainer().style_default_by_type	(odf_types::style_family::Paragraph);
		baseStyle		= odf_context_.styleContainer().style_by_name			(base_style_name_, base_style_family_,false/*process_headers_footers_*/);
	}

	if (defaultStyle)	instances.push_back(defaultStyle);
	if (baseStyle)		instances.push_back(baseStyle);
	if (style)			instances.push_back(style);

	if (style)	get_styles_context().start_process_style(style);
	else		get_styles_context().start_process_style(baseStyle);

	propertiesOut.apply_from(calc_paragraph_properties_content(instances));
}

void pptx_text_context::Impl::write_pPr(std::wostream & strm)
{
	get_styles_context().start();

	int level = list_style_stack_.size() - 1;		

	odf_reader::paragraph_format_properties paragraph_properties_;
	
	ApplyParagraphProperties	(paragraph_style_name_,	paragraph_properties_);
	ApplyListProperties			(paragraph_properties_, level);//выравнивания листа накатим на свойства параграфа

	paragraph_properties_.pptx_convert(pptx_context_);	
	
	const std::wstring & paragraphAttr  = get_styles_context().paragraph_attr().str();	
	const std::wstring & paragraphNodes = get_styles_context().paragraph_nodes().str();

	if (level < 0 && paragraphAttr.length() < 1 && !paragraphNodes.empty()) return;
	
	strm << L"<a:pPr ";

		if (level >= 0)
		{
			if (level > 8) level = 0;
			strm << L"lvl=\"" << level << L"\" ";
		}

		strm << paragraphAttr;

	strm << ">";
		strm << paragraphNodes;

		if (level >= 0 )
		 {
			

			strm << get_styles_context().list_style().str();
		}
	strm << L"</a:pPr>";
}

void pptx_text_context::Impl::write_t(std::wostream & strm)
{
}

void pptx_text_context::Impl::write_rPr(std::wostream & strm)
{
	if (paragraph_style_name_.empty() && span_style_name_.empty() && !(!hyperlink_hId.empty())  && base_style_name_.empty())
		return;

	odf_reader::text_format_properties_content text_properties_;
	
	ApplyTextProperties(span_style_name_, paragraph_style_name_, text_properties_);

	get_styles_context().start();

	get_styles_context().hlinkClick() = hyperlink_hId;
	
	text_properties_.pptx_convert(pptx_context_);

	strm << get_styles_context().text_style().str();

}
std::wstring pptx_text_context::Impl::dump_paragraph(/*bool last*/)
{				
	if (in_comment) return L""; 

	end_span();

    std::wstring str_run = run_.str();

	if (str_run.length() > 0 || paragraph_style_name_.length() > 0)
	{
		CP_XML_WRITER(paragraph_)
		{
			CP_XML_NODE(L"a:p")
			{
				write_pPr(CP_XML_STREAM());

				if (str_run.length() > 0)
				{
					CP_XML_STREAM() << run_.str();
				}
				else
				{
					CP_XML_NODE(L"a:endParaRPr");
				}
			}
		}
		run_.str(std::wstring());
	}
	return paragraph_.str();
}

#if defined (_WIN32) || defined (_WIN64)
    #include <Objbase.h>
#endif

void pptx_text_context::Impl::dump_field()
{
	if (field_type_ == none) return;
	
	CP_XML_WRITER(run_)
    {
		CP_XML_NODE(L"a:fld")
		{
			std::wstring content = XmlUtils::EncodeXmlString(field_value_.str());
			std::wstring string_id;
			std::wstring string_type;

#if defined (_WIN32) || defined (_WIN64)
            GUID new_id;
			CoCreateGuid(&new_id);
			wchar_t str[100]={};
			StringFromGUID2(new_id,str,100);
			string_id = str;
#endif
			switch (field_type_)
			{
			case page_number: 
				{
					string_type = L"slidenum";

#if !defined (_WIN32) &&  !defined (_WIN64)
                    string_id =  L"{5CC2A059-B141-45A7-B910-B096D6D06820}";
#endif
				//	content = L"‹#›";
				}
				break;
			case date:
				{
					string_type = L"datetime1";

#if !defined (_WIN32) &&  !defined (_WIN64)
                    string_id = L"{1D1B89AE-8D35-4BB5-B492-6D9BE4F23A39}";
#endif
                    if (content.length()<1)content = XmlUtils::EncodeXmlString(L"01.01.2000");
				}							
				break;
			case time:	
				{
					string_type = L"datetime11";

#if !defined (_WIN32) &&  !defined (_WIN64)
                    string_id = L"{03DA74A9-E3F2-4F30-AAF9-CC1A83980D5E}";
#endif
                    if (content.length()<1)content = XmlUtils::EncodeXmlString(L"00:00:00");
				}
				break;
			case datetime:
				{
					string_type =  L"datetime1";

#if !defined (_WIN32) &&  !defined (_WIN64)
                    string_id = L"{A9EA0FE8-FEF9-4B2F-BC9D-19DDCDB4AB9B}";
#endif
                 }break;
			}  
			if (string_type.length()>0)
			{
                CP_XML_ATTR(L"id",  string_id);
				CP_XML_ATTR(L"type", string_type);
				CP_XML_NODE(L"a:t")
				{
					CP_XML_STREAM() << content;
				}			
			}
			else
			{
				//запишем как обычный текст
				text_ << content;
			}
		}
	}
	field_value_.str(std::wstring());
	field_type_ = none;
}
void pptx_text_context::Impl::dump_run()
{
	if (in_comment) return;

	dump_field();
	
	if (process_layouts_) return; 
	
	const std::wstring content = XmlUtils::EncodeXmlString(text_.str());
	//if (content.length() <1 &&  span_style_name_.length()<1) return ;      ... провеить с пустыми строками нужны ли  ...

	if (content .length() > 0)
	{		
		CP_XML_WRITER(run_)
		{
			CP_XML_NODE(L"a:r")
			{
				write_rPr(CP_XML_STREAM());   

				CP_XML_NODE(L"a:t")
				{
					//CP_XML_ATTR(L"xml:space", L"preserve"); 
					CP_XML_STREAM() << content;
				}
			 }
			text_.str(std::wstring());			
		}
	}
	hyperlink_hId =L"";
}

void pptx_text_context::Impl::start_base_style(const std::wstring baseStyleName, const odf_types::style_family::type baseStyleType)
{
	base_style_name_ = baseStyleName;
	base_style_family_ = baseStyleType;
}

void pptx_text_context::Impl::end_base_style()
{
	base_style_name_ = L"";
	base_style_family_ = odf_types::style_family::None;
}

void pptx_text_context::Impl::start_object()
{
    paragraphs_cout_ = 0;
   
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
	field_value_.str(std::wstring());
    
	paragraph_style_name_ = L"";
	span_style_name_ = L"";

}

std::wstring pptx_text_context::Impl::end_object()
{
	dump_paragraph(/*true*/);

    std::wstring str_paragraph = paragraph_.str();

	std::wstringstream str_output;

	if (str_paragraph.length() > 0)
	{
		write_list_styles(str_output);
		str_output << str_paragraph;
	}
  
	paragraphs_cout_ = 0;
    
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
	field_value_.str(std::wstring());
	
	paragraph_style_name_ = L"";
    span_style_name_=L"";

	return str_output.str();
}
void pptx_text_context::Impl::start_list_item(bool restart)
{
    if (paragraphs_cout_ > 0 && restart)
    {	
		dump_paragraph();
	}

	in_list_ = true;
    first_element_list_item_ = true;

    if (restart && !list_style_stack_.empty())
    {
        const std::wstring curStyleName = current_list_style();
        const std::wstring newStyleName = curStyleName + std::to_wstring(new_list_style_number_++);
        list_style_renames_[curStyleName] = newStyleName;

        odf_reader::list_style_container & lists = odf_context_.listStyleContainer();
        
		odf_reader::text_list_style * curStyle = lists.list_style_by_name(curStyleName);
        lists.add_list_style(curStyle, newStyleName);
        end_list();
        start_list(newStyleName);
    }
}

void pptx_text_context::Impl::start_list(const std::wstring & StyleName, bool Continue)
{
    if (paragraphs_cout_ > 0 && ( in_paragraph || list_style_stack_.empty()))
    {	
		dump_paragraph();
	}
	 
	if (!StyleName.empty())
    {
        std::wstring name = StyleName;
        if (Continue)
            name = find_list_rename(name);

        list_style_stack_.push_back(name);
    }
    else if (!list_style_stack_.empty())
        list_style_stack_.push_back(list_style_stack_.back());
    else
        list_style_stack_.push_back(L"");
}

void pptx_text_context::Impl::end_list()
{
	in_list_ = false;
  
	list_style_stack_.pop_back();// пока не стираем .. как сохраним в lstStyles - очистим
}

std::wstring pptx_text_context::Impl::current_list_style()
{
    if (!list_style_stack_.empty())
        return list_style_stack_.back();    
    else
        return L"";
}
std::wstring pptx_text_context::Impl::find_list_rename(const std::wstring & ListStyleName)
{
    std::wstring name = ListStyleName;
    
	while (list_style_renames_.count(name) > 0)
        name = list_style_renames_.at(name);

    return name;
}

void pptx_text_context::Impl::end_list_item()
{
	dump_paragraph();
	
	paragraphs_cout_--;
	paragraph_style_name_ = L"";

	in_list_ = false;
}

void pptx_text_context::Impl::start_field(field_type type, const std::wstring & styleName)
{
	field_type_ = type;
}
void pptx_text_context::Impl::start_comment()
{
	in_comment = true;//отдельная ветка - так как без форматирования
}
std::wstring pptx_text_context::Impl::end_comment()
{
	std::wstring  str_comment = text_.str();
    text_.str(std::wstring());
	in_comment = false;

	paragraphs_cout_ = 0;    
	
	paragraph_style_name_ = L"";
    span_style_name_=L"";

	return str_comment;
}
void pptx_text_context::Impl::end_field()
{
	dump_run();
	field_type_ = none;
}

void pptx_text_context::Impl::write_list_styles(std::wostream & strm)//defaults style paragraph & lists
{
    odf_reader::list_style_container & list_styles = odf_context_.listStyleContainer();

    if (list_styles.empty())
        return;
 
	CP_XML_WRITER(strm)
	{ 	
		CP_XML_NODE(L"a:lstStyle");
		//{
		//	//defPPr
		//	//...

		//	//list levels 0 - 8
		//	BOOST_FOREACH(std::wstring & st_name, list_style_stack_ )
		//	{
		//		odf_reader::text_list_style * s = list_styles.list_style_by_name(st_name);
		//        
		//		BOOST_FOREACH(odf_reader::office_element_ptr & elm, s->get_content())
		//		{
		//			elm->pptx_convert(pptx_context_);
		//		}
		//	}

		//	CP_XML_STREAM() << get_styles_context().list_style().str();
		//}
	}

	list_style_stack_.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

pptx_text_context::pptx_text_context(odf_reader::odf_read_context & odf_context_, pptx_conversion_context & pptx_context_):
	impl_(new pptx_text_context::Impl(odf_context_,pptx_context_))
{}


pptx_text_context::~pptx_text_context()
{
}
void pptx_text_context::set_local_styles_container(odf_reader::styles_container*  local_styles_)
{
	return impl_->set_local_styles_container(local_styles_);
}
void pptx_text_context::add_paragraph(const std::wstring & para)
{
	return impl_->add_paragraph(para);
}
void pptx_text_context::add_text(const std::wstring & text)
{
    return impl_->add_text(text);
}
bool pptx_text_context::in_list()
{
    return impl_->in_list_;
}
void pptx_text_context::start_paragraph(const std::wstring & styleName)
{
    return impl_->start_paragraph(styleName);
}

void pptx_text_context::end_paragraph()
{
    return impl_->end_paragraph();
}

void pptx_text_context::start_span(const std::wstring & styleName)
{
    return impl_->start_span(styleName);
}

void pptx_text_context::end_span()
{
    return impl_->end_span();
}

std::wstring pptx_text_context::end_span2()
{
    return impl_->end_span2();
}

void pptx_text_context::start_list_item(bool restart)
{
	return impl_->start_list_item(restart);
}
void pptx_text_context::end_list_item()
{
	return impl_->end_list_item();
}
void pptx_text_context::start_list(const std::wstring & StyleName, bool Continue)
{
	return impl_->start_list(StyleName,Continue);
}
void pptx_text_context::end_list()
{
	return impl_->end_list();
}
void pptx_text_context::start_base_style(const std::wstring baseStyleName, const odf_types::style_family::type baseStyleType)
{
	return impl_->start_base_style(baseStyleName, baseStyleType);
}

void pptx_text_context::end_base_style()
{
	return impl_->end_base_style();
}

void pptx_text_context::start_object()
{
	return impl_->start_object();
}
void pptx_text_context::start_hyperlink()
{
	return impl_->start_hyperlink();
}
void pptx_text_context::end_hyperlink(std::wstring hId)
{
	return impl_->end_hyperlink(hId);
}
std::wstring pptx_text_context::end_object()
{
	return impl_->end_object();
}
styles_context & pptx_text_context::get_styles_context() 
{ 
	return  impl_->get_styles_context() ; 
}
void pptx_text_context::start_field(field_type type, const std::wstring & styleName)
{
	impl_->start_field(type, styleName);
}
void pptx_text_context::end_field()
{
	impl_->end_field();
}
void pptx_text_context::start_comment_content()
{
	impl_->start_comment();
}
std::wstring pptx_text_context::end_comment_content()
{
	return impl_->end_comment();
}

void pptx_text_context::set_process_layouts(bool val)
{
	impl_->process_layouts_ = val;
}

}
}
