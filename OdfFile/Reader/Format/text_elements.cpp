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

#include "text_elements.h"

#include <xml/xmlchar.h>
#include <xml/utils.h>

#include "paragraph_elements.h"
#include "serialize_elements.h"
#include "list.h"

#include "odf_document.h"
#include "odfcontext.h"
#include "style_paragraph_properties.h"
#include "style_text_properties.h"
#include "documentcontext.h"

#include "calcs_styles.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {
namespace text {

// text:h
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * h::ns = L"text";
const wchar_t * h::name = L"h";

namespace {

typedef std::map<std::wstring, oox::text_tracked_context::_state>::iterator map_changes_iterator;
void process_paragraph_drop_cap_attr(const paragraph_attrs & Attr, oox::docx_conversion_context & Context)
{
    if (Attr.text_style_name_.empty())return;

	style_instance * styleInst 
            = Context.root()->odf_context().styleContainer().style_by_name(Attr.text_style_name_, style_family::Paragraph, Context.process_headers_footers_);
    if ((!styleInst) || (styleInst->is_automatic() == false))return;

	style_content * styleContent = styleInst->content();
    if (!styleContent)return;

	style_paragraph_properties * paragraph_properties = styleContent->get_style_paragraph_properties();	
	if (!paragraph_properties)return;

	const office_element_ptr & elm_style_drop_cap = paragraph_properties->content_.style_drop_cap_;

	if (!elm_style_drop_cap)return;
			
	Context.get_drop_cap_context().state(2);//active (enable)

	style_drop_cap* style_drop_cap_ = dynamic_cast<style_drop_cap*>(elm_style_drop_cap.get());

	if (style_drop_cap_->style_length_.get_type() == drop_cap_length::Integer)
		Context.get_drop_cap_context().Length = style_drop_cap_->style_length_.get_value();
	else
		Context.get_drop_cap_context().Length = -1;//word
	Context.get_drop_cap_context().Scale = style_drop_cap_->style_lines_;
	
	if (style_drop_cap_->style_distance_)
		Context.get_drop_cap_context().Space = (int)(20.0 * (style_drop_cap_->style_distance_->get_value_unit(length::pt) + 5)+ 0.5);//формула ачуметь !! - подбор вручную

	//font size пощитаем здесь .. так как его значение нужо в стиле параграфа (межстрочный интервал) - в (pt*20)
	
	text_format_properties_content_ptr text_properties = calc_text_properties_content (styleInst);

	if ((text_properties) && (!text_properties->fo_font_size_))
	{		//default
		styleInst = Context.root()->odf_context().styleContainer().style_default_by_type(style_family::Paragraph);
		text_properties = calc_text_properties_content (styleInst);
	}
	if ((text_properties) && (text_properties->fo_font_size_))
	{
		Context.get_drop_cap_context().FontSize = text_properties->process_font_size(
				text_properties->fo_font_size_, Context.get_styles_context().get_current_processed_style(), false, //1.);
		7.25 * (Context.get_drop_cap_context().Scale + (Context.get_drop_cap_context().Scale-1) * 0.7));//формула ачуметь !! - подбор вручную
	}
}

}

std::wostream & paragraph::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(content_, bXmlEncode);
    //_Wostream << L"\n";
    return _Wostream;
}

void paragraph::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attrs_.add_attributes(Attributes);
}

void paragraph::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if CP_CHECK_NAME(L"text", L"sequence")
    {
        CP_CREATE_ELEMENT_SIMPLE(sequence_);

		if ((false == content_.empty()) && (content_.back()->get_type() == typeTextText))
		{
			sequence* q = dynamic_cast<sequence*>(sequence_.get());
			text* t = dynamic_cast<text*>(content_.back().get());
			if (q && t)
			{
				q->template_ = t->text_;
				content_.pop_back();
			}
		}
		content_.push_back(sequence_);
    }
	else		
	{
		if CP_CHECK_NAME(L"text", L"a")
			is_present_hyperlink_ = true;

		CP_CREATE_ELEMENT_SIMPLE(content_);
	}
}

void paragraph::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
	content_.push_back( elm );
}
void paragraph::add_space(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
	content_.push_back( elm );
}
const wchar_t * emptyParagraphContent = L"<w:pPr></w:pPr><w:r><w:rPr></w:rPr></w:r>";

const wchar_t * emptyParagraphDrawing = L"<w:p><w:pPr></w:pPr></w:p>";

void paragraph::drop_cap_text_docx_convert(office_element_ptr first_text_element_paragraph,oox::docx_conversion_context & Context)
{
	if (first_text_element_paragraph == NULL)return;
	
	text* first_text_paragraph = dynamic_cast<text*>(first_text_element_paragraph.get());
	if (first_text_paragraph == NULL)return;
	
	std::wstring & str = first_text_paragraph->text_;
	std::wstring store_str = str;

	if (Context.get_drop_cap_context().Length == -1)
		Context.get_drop_cap_context().Length = store_str.find(L" ");//find length word

	str = store_str.substr(0,Context.get_drop_cap_context().Length);
	
	int textStyle = Context.process_paragraph_attr(&attrs_);
	first_text_paragraph->docx_convert(Context); 

	size_t str_start = Context.get_drop_cap_context().Length;
	size_t str_size	= store_str.length() - Context.get_drop_cap_context().Length;

	if (str_size < 0) str_size = 0;										// это если на буквы в буквице разные стили
	if (str_start > store_str.length()) str_start = store_str.length(); // это если на буквы в буквице разные стили

	str = store_str.substr(str_start, str_size);

	if (textStyle == 1) Context.pop_text_properties();
}

size_t paragraph::drop_cap_docx_convert(oox::docx_conversion_context & Context)
{
	if ( content_.empty()) return 0;

	size_t index = 0;

	while(index < content_.size()) // могут быть track-change, ...
	{
		if (content_[index]->get_type() == typeTextText || 
			content_[index]->get_type() == typeTextSpan)
			break;
		content_[index++]->docx_convert(Context); 
	}

	if (index >= content_.size()) return index;

	if ( content_[index]->get_type() == typeTextText)
	{
		drop_cap_text_docx_convert(content_[index], Context);
	}
	else if (content_[index]->get_type() == typeTextSpan)
	{
		span* first_span_in_paragraph = dynamic_cast<span*>(content_[index].get());
		
		if (Context.get_drop_cap_context().FontSize < 1)
		{
			style_instance * styleInst = NULL;
			if (first_span_in_paragraph)
			{
				styleInst = Context.root()->odf_context().styleContainer().style_by_name(first_span_in_paragraph->text_style_name_, style_family::Text, Context.process_headers_footers_);
			}		
			else
			{
				styleInst = Context.root()->odf_context().styleContainer().style_by_name(attrs_.text_style_name_, style_family::Paragraph, Context.process_headers_footers_);
			}
			text_format_properties_content_ptr text_properties = calc_text_properties_content (styleInst);

			if ((text_properties) && (!text_properties->fo_font_size_))
			{		//default
				styleInst = Context.root()->odf_context().styleContainer().style_default_by_type(style_family::Text);
				text_properties = calc_text_properties_content (styleInst);
			}
			if ((text_properties) && (text_properties->fo_font_size_))
			{
				Context.get_drop_cap_context().FontSize = text_properties->process_font_size(
					text_properties->fo_font_size_, Context.get_styles_context().get_current_processed_style(),  false, //1);
							 7.25 * (Context.get_drop_cap_context().Scale + (Context.get_drop_cap_context().Scale-1) * 0.7));
			}
		}
		//в рассчет берутся только первые элементы !!! разные там break-и отменяют реэжим drop_cap!!
		if ((!first_span_in_paragraph->content_.empty()) &&
			 (first_span_in_paragraph->content_[0]->get_type() == typeTextText))
		{
			drop_cap_text_docx_convert(first_span_in_paragraph->content_[0], Context);
		}
	}
	return index;
}
void paragraph::docx_convert(oox::docx_conversion_context & Context, _CP_OPT(std::wstring) next_element_style_name)
{
    std::wstring styleName = attrs_.text_style_name_;
	
	bool in_drawing	= false;

 	if (Context.get_drawing_context().get_current_shape() || Context.get_drawing_context().get_current_frame())
	{
		in_drawing = true;

		if (sequence_)
		{
			std::wstringstream _Wostream;
			CP_SERIALIZE_TEXT(content_, true);///todooo

			Context.get_drawing_context().set_next_object_caption(_Wostream.str());
		}
	}
		
	bool bIsNewParagraph = true;
	
	bool is_empty = content_.empty();

	if (Context.get_paragraph_state() && (Context.get_process_note() == oox::docx_conversion_context::noNote) && !in_drawing)
    {//вложеннные элементы ... или после графики embedded_linux_kernel_and_drivers_labs_zh_TW.odt
		bIsNewParagraph = false;
		
		if (!Context.get_paragraph_keep())// например Appendix I_IPP.odt - tracked elements (
		{
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->docx_convert(Context); 
			}
			if (!Context.get_delete_text_state())
				Context.set_paragraph_state(false);// например Appendix I_IPP.odt - tracked elements (вложенные списки из 2 элементов)
			return;
		}
    }

    if (bIsNewParagraph)
		Context.start_paragraph(is_header_);
	
    const _CP_OPT(std::wstring) masterPageName	= Context.root()->odf_context().styleContainer().master_page_name_by_name(styleName);
   
	if (masterPageName)
    {
		is_empty = false;
    }    
	_CP_OPT(std::wstring) next_masterPageName;
    if (next_element_style_name)
    {
        // проверяем не сменит ли следующий параграф свойства страницы.
        // если да — устанавливаем контексту флаг на то что необходимо в текущем параграфе
        // распечатать свойства раздела/секции
		// проверить ... не она ли текущая - может быть прописан дубляж - и тогда разрыв нарисуется ненужный
		// dump был выше уровнем
        next_masterPageName	= Context.root()->odf_context().styleContainer().master_page_name_by_name(*next_element_style_name);

        if ((next_masterPageName)  && (Context.get_master_page_name() != *next_masterPageName))
        {
			if (false == Context.root()->odf_context().pageLayoutContainer().compare_page_properties(Context.get_master_page_name(), *next_masterPageName))
			{
				is_empty = false;
			}
			else
				next_masterPageName = boost::none;
        }
		else next_masterPageName = boost::none;
    } 
	if (next_section_ || next_end_section_) // remove in text::section  - GreekSynopsis.odt
	{
		Context.get_section_context().get_last().is_dump_ = true;
		is_empty = false;
	}
	std::wstringstream strm;
	if (Context.process_page_properties(strm))
	{
		Context.get_section_context().dump_ = strm.str();
	}
	process_paragraph_drop_cap_attr(attrs_, Context);

	size_t index = 0;
	if (Context.get_drop_cap_context().state() == 2)//active
	{
		index = drop_cap_docx_convert(Context); 
		
		Context.finish_run();
		Context.finish_paragraph();		
		
		Context.get_drop_cap_context().state(1);//after 
		Context.start_paragraph();
		Context.process_paragraph_style(Context.get_current_paragraph_style());

	}
	Context.start_paragraph_style(styleName);

    int textStyle = Context.process_paragraph_attr(&attrs_);

    Context.add_note_reference();
	
	int break_ = Context.get_page_break_before();
	if (break_ > 0)
	{
		Context.set_page_break_before(0);

		is_empty = false;
		Context.add_new_run(_T(""));
			 if (break_ == 1) Context.output_stream() << L"<w:br w:type=\"column\"/>";
		else if (break_ == 2) Context.output_stream() << L"<w:br w:type=\"page\"/>";
		Context.finish_run();
	}
  	for (size_t i = index; i < content_.size(); i++)
	{
		if (content_[i]->get_type() == typeTextP)
		{//параграф в параграфе .... оО
			p *para_inside = dynamic_cast<p*>(content_[i].get());
			
			for (size_t j = 0; (para_inside) && (j < para_inside->paragraph_.content_.size()); ++j)
			{
				para_inside->paragraph_.content_[j]->docx_convert(Context);
			}
			continue;
		}
		if (Context.get_page_break())
		{
			if (Context.process_headers_footers_ == false) 
				//Context.output_stream() << L"<w:lastRenderedPageBreak/>";
				Context.output_stream() << L"<w:br w:type=\"page\"/>";  
			Context.set_page_break(false);
		}
		if (Context.is_alphabetical_index() && 
			(content_[i]->get_type() != typeTextAlphabeticalIndexMarkStart && 
			 content_[i]->get_type() != typeTextAlphabeticalIndexMarkEnd && 
			 content_[i]->get_type() != typeTextAlphabeticalIndexMark))
		{
			Context.add_alphabetical_index_text(content_[i]);
		}
		else
		{
			content_[i]->docx_convert(Context); 
			
			if (Context.get_drop_cap_context().state() > 0)		
				Context.get_drop_cap_context().state(0);//disable
		
		}
		if (Context.is_table_content())
		{
			Context.get_table_content_context().next_level_index();
		}
	}

    if (textStyle > 0)
	{
		is_empty = false;
        if (textStyle == 1) Context.pop_text_properties();
		
		Context.pop_current_fontSize();
	}

    Context.finish_run();

    // конвертируем в контексте параграфа элементы, которые были помещены в очередь
	if (!Context.delayed_converting_)//иначе возможно зацикливание
	{
		Context.docx_convert_delayed();
	}

	break_ = Context.get_page_break_after();
    if (break_ > 0)
    {
		Context.set_page_break_after(0);

		is_empty = false;
        Context.add_new_run(_T(""));
				 if (break_ == 1) Context.output_stream() << L"<w:br w:type=\"column\"/>";
			else if (break_ == 2) Context.output_stream() << L"<w:br w:type=\"page\"/>";
		Context.finish_run();		
    }

	if (is_empty)
		Context.output_stream() << emptyParagraphContent;

	Context.end_paragraph_style();
	Context.finish_paragraph();

	if (next_masterPageName)
	{
		Context.set_master_page_name(*next_masterPageName);
		std::wstring masterPageNameLayout = Context.root()->odf_context().pageLayoutContainer().page_layout_name_by_style(*next_masterPageName);

		if (false == masterPageNameLayout.empty())
		{
			Context.remove_page_properties();
			Context.add_page_properties(masterPageNameLayout);
		}
	}
}

void paragraph::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_paragraph(attrs_.text_style_name_);
  	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context); 
    }
    Context.end_paragraph();    
}
void paragraph::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_paragraph(attrs_.text_style_name_);
    
  	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context); 
    }
    
	Context.get_text_context().end_paragraph();    
}
///////////////////////////////////////////
void soft_page_break::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.process_headers_footers_) 
		return;
	
	if (0 == Context.get_page_break_after() && 0 == Context.get_page_break_before())
	{
		Context.output_stream() << L"<w:lastRenderedPageBreak/>";
	}
}

//////////////////////////////////////////////

std::wostream & h::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return paragraph_.text_to_stream(_Wostream, bXmlEncode);
}

void h::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name",	element_style_name);

	CP_APPLY_ATTR(L"text:outline-level"		, outline_level_);
    CP_APPLY_ATTR(L"text:restart-numbering"	, restart_numbering_);
    CP_APPLY_ATTR(L"text:start-value"		, start_value_);
    CP_APPLY_ATTR(L"text:is-list-header"	, is_list_header_);
	CP_APPLY_ATTR(L"text:level"				, outline_level_); // openoffice xml 1.0

    paragraph_.add_attributes(Attributes);

	paragraph_.is_header_				= true;
	paragraph_.attrs_.outline_level_	= outline_level_;
}

void h::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    paragraph_.add_child_element(Reader, Ns, Name, getContext());
}

void h::add_text(const std::wstring & Text)
{
    paragraph_.add_text(Text);
}

void h::docx_convert(oox::docx_conversion_context & Context) 
{
	std::wstring bookmark;
	if (false == Context.get_table_content_context().mapReferences.empty())
	{//когда заголовки находятся выше таблицы контента - херово
		std::wstringstream strm;
		text_to_stream(strm, false);
		std::wstring outline = strm.str();
		
		std::map<std::wstring, std::wstring>::iterator pFind = Context.get_table_content_context().mapReferences.find(outline);

		if (pFind != Context.get_table_content_context().mapReferences.end())
		{
			bookmark = pFind->second;
			Context.start_bookmark(bookmark);
		}
	}
    paragraph_.docx_convert(Context, next_element_style_name);

	if (false == bookmark.empty())
	{
		Context.end_bookmark(bookmark);
	}
}

void h::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    paragraph_.xlsx_convert(Context);
}
void h::pptx_convert(oox::pptx_conversion_context & Context) 
{
    paragraph_.pptx_convert(Context);
}
// text:p
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * p::ns = L"text";
const wchar_t * p::name = L"p";

std::wostream & p::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return paragraph_.text_to_stream(_Wostream, bXmlEncode);
}

void p::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name",	element_style_name);

    paragraph_.add_attributes(Attributes);
}

void p::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    paragraph_.add_child_element(Reader, Ns, Name, getContext());
}

void p::add_text(const std::wstring & Text)
{
    paragraph_.add_text(Text);
}
void p::add_space(const std::wstring & Text)
{
    paragraph_.add_space(Text);
}

void p::docx_convert(oox::docx_conversion_context & Context)
{
    paragraph_.docx_convert(Context, next_element_style_name);
}

void p::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    paragraph_.xlsx_convert(Context);
}

void p::pptx_convert(oox::pptx_conversion_context & Context)
{
    paragraph_.pptx_convert(Context);
}

// text:list
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * list::ns = L"text";
const wchar_t * list::name = L"list";

std::wostream & list::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
  	for (size_t i = 0; i < list_items_.size(); i++)
    {
        list_items_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}

void list::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    style_name_			= Attributes->get_val< std::wstring >(L"text:style-name").get_value_or(L"");
    continue_numbering_	= Attributes->get_val< bool >(L"text:continue-numbering");
    // TODO
}

void list::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"list-header")
    {
        CP_CREATE_ELEMENT(list_header_);
    }
    else
    {
        CP_CREATE_ELEMENT(list_items_);
    }
}

void list::docx_convert(oox::docx_conversion_context & Context)
{
    bool continue_ = continue_numbering_.get_value_or(false);
    Context.start_list(style_name_, continue_);

    if (list_header_)
        list_header_->docx_convert(Context);

	for (size_t i = 0; i < list_items_.size(); i++)
    {
        list_items_[i]->docx_convert(Context);
    }

    Context.end_list();
}
void list::pptx_convert(oox::pptx_conversion_context & Context)
{
    bool continue_ = continue_numbering_.get_value_or(false);
    Context.get_text_context().start_list(style_name_, continue_);

    if (list_header_)
        list_header_->pptx_convert(Context);

	for (size_t i = 0; i < list_items_.size(); i++)
	{
        list_items_[i]->pptx_convert(Context);
    }

    Context.get_text_context().end_list();
}
// text:soft-page-break
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * soft_page_break::ns = L"text";
const wchar_t * soft_page_break::name = L"soft-page-break";

std::wostream & soft_page_break::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    _Wostream << L"\n";
    return _Wostream;
}

void soft_page_break::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void soft_page_break::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
}

// text:section
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * section::ns = L"text";
const wchar_t * section::name = L"section";

std::wostream & section::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return serialize_elements_text(_Wostream, content_, bXmlEncode);
}

void section::afterCreate()
{
	if (document_context * context = getContext())
    {
        if (p *lastPar = dynamic_cast<p*>(dynamic_cast<p*>(context->get_last_paragraph())))
        {
            lastPar->paragraph_.set_next_section(true);        
        }
		else if (h *lastPar = dynamic_cast<h*>(dynamic_cast<p*>(context->get_last_paragraph())))
        {
            lastPar->paragraph_.set_next_section(true);        
        }
	}
	office_element::afterCreate();
}

void section::afterReadContent()
{
	if (document_context * context = getContext())
	{
		if (p *lastPar = dynamic_cast<p*>(dynamic_cast<p*>(context->get_last_paragraph())))
		{
			lastPar->paragraph_.set_next_end_section(true);
		}
		else if (h *lastPar = dynamic_cast<h*>(dynamic_cast<p*>(context->get_last_paragraph())))
		{
			lastPar->paragraph_.set_next_end_section(true);
		}
	}
	office_element::afterReadContent();
}
void section::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes(Attributes);
}

void section::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"section-source" == Name)
    {
        CP_CREATE_ELEMENT(section_source_);    
    }
    else
    {
        CP_CREATE_ELEMENT(content_);
    }
}

void section::docx_convert(oox::docx_conversion_context & Context)
{
	bool bAddSection = false;
	if ( false == Context.get_drawing_state_content())
	{
		std::wstring current_page_properties = Context.get_page_properties();
	   
		Context.get_section_context().add_section (section_attr_.name_, section_attr_.style_name_.get_value_or(L""), current_page_properties);
		
		Context.add_page_properties(current_page_properties);

		bAddSection = true;
	}
	else
	{
		//колонки для текста в объектах todooo
	}

   	for (size_t i = 0; i < content_.size(); i++)
	{
 		if (content_[i]->element_style_name)
		{
			std::wstring text___ = *content_[i]->element_style_name;

			const _CP_OPT(std::wstring) masterPageName	= Context.root()->odf_context().styleContainer().master_page_name_by_name(*content_[i]->element_style_name);
		   
			if (masterPageName)
			{				
				std::wstring masterPageNameLayout = Context.root()->odf_context().pageLayoutContainer().page_layout_name_by_style(*masterPageName);

				if (false == masterPageNameLayout.empty())
				{
					Context.set_master_page_name(*masterPageName); //проверка на то что тема действительно существует????
					
					Context.remove_page_properties();
					Context.add_page_properties(masterPageNameLayout);
				}
			}  
		}
		if (content_[i]->next_element_style_name)
		{
			std::wstring text___ = *content_[i]->next_element_style_name;
			// проверяем не сменится ли свойства страницы.
			// если да — устанавливаем контексту флаг на то что необходимо в текущем параграфе
			// распечатать свойства раздела/секции
			//проверить ... не она ли текущая - может быть прописан дубляж - и тогда разрыв нарисуется ненужный
			const _CP_OPT(std::wstring) next_masterPageName	= Context.root()->odf_context().styleContainer().master_page_name_by_name(*content_[i]->next_element_style_name);

			if ((next_masterPageName)  && (Context.get_master_page_name() != *next_masterPageName))
			{
				if (false == Context.root()->odf_context().pageLayoutContainer().compare_page_properties(Context.get_master_page_name(), *next_masterPageName))
				{
					Context.next_dump_page_properties(true);
					//is_empty = false;
				}
			}
		} 
		content_[i]->docx_convert(Context);
    }
	if (bAddSection)
	{
		Context.get_section_context().get_last().is_dump_ = true;
		Context.last_dump_page_properties(false);
	}
}

// text-section-source-attr
//////////////////////////////////////////////////////////////////////////////////////////////////
void section_source_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    xlink_attlist_.add_attributes(Attributes);

    CP_APPLY_ATTR(L"text:section-name", section_name_);
    CP_APPLY_ATTR(L"text:filter-name", filter_name_);
}

// text:section-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * section_source::ns = L"text";
const wchar_t * section_source::name = L"section-source";

void section_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_source_attr_.add_attributes(Attributes);
}

void section_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
// text:index-body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * index_body::ns = L"text";
const wchar_t * index_body::name = L"index-body";

std::wostream & index_body::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(content_, bXmlEncode);
    return _Wostream;
}

void index_body::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void index_body::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"index-title" == Name)
    {
        CP_CREATE_ELEMENT(index_title_);    
    }
    else
		CP_CREATE_ELEMENT(content_);
}

void index_body::docx_convert(oox::docx_conversion_context & Context) 
{
	if (index_title_)
	{
		index_title_->docx_convert(Context);
	}
	Context.start_index_content();
   	for (size_t i = 0; i < content_.size(); i++)
	{
		Context.start_index_element();
			content_[i]->docx_convert(Context);
		Context.end_index_element();
    }
	Context.end_index_content();
}
void index_body::pptx_convert(oox::pptx_conversion_context & Context) 
{
	for (size_t i = 0; i < content_.size(); i++)
	{
        content_[i]->pptx_convert(Context);
    }
}
// text:index-title
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * index_title::ns = L"text";
const wchar_t * index_title::name = L"index-title";

void index_title::docx_convert(oox::docx_conversion_context & Context) 
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
}
void index_title::pptx_convert(oox::pptx_conversion_context & Context) 
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
}
std::wostream & index_title::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(content_, bXmlEncode);
    return _Wostream;
}

void index_title::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes( Attributes );
}

void index_title::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
     CP_CREATE_ELEMENT(content_);
}
//-------------------------------------------------------------------------------------------------------
// text:index-title-template
//-------------------------------------------------------------------------------------------------------
const wchar_t * index_title_template::ns = L"text";
const wchar_t * index_title_template::name = L"index-title-template";

void index_title_template::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name", style_name_);
}
void index_title_template::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
	content_ = elm;
}
void index_title_template::docx_convert(oox::docx_conversion_context & Context) 
{
	if (content_)
    {
        content_->docx_convert(Context);
    }
}
//-------------------------------------------------------------------------------------------------------
// text:table-of-content
//-------------------------------------------------------------------------------------------------------
const wchar_t * table_of_content::ns = L"text";
const wchar_t * table_of_content::name = L"table-of-content";

void table_of_content::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes( Attributes );
}

void table_of_content::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(index_body_);
    }
	else  if CP_CHECK_NAME(L"text", L"table-of-content-source")
    {
        CP_CREATE_ELEMENT(table_of_content_source_);
    }
}
void table_of_content::docx_convert(oox::docx_conversion_context & Context)
{
	if (section_attr_.style_name_ && false == section_attr_.name_.empty()) // ??? ||
	{
		std::wstring current_page_properties = Context.get_page_properties();
		Context.get_section_context().add_section(section_attr_.name_, section_attr_.style_name_.get_value_or(L""), current_page_properties);
		Context.add_page_properties(current_page_properties);
	}

	if (index_body_)
	{
		Context.start_sdt(1);
		if (table_of_content_source_)
		{
			table_of_content_source_->docx_convert(Context);
		}
		index_body_->docx_convert(Context);
		Context.end_sdt();
	}
}

void table_of_content::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (index_body_)
        index_body_->pptx_convert(Context);
}


std::wostream & table_of_content::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(index_body_, bXmlEncode);
    return _Wostream;
}
// text:table-of-content-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_of_content_source::ns = L"text";
const wchar_t * table_of_content_source::name = L"table-of-content-source";

void table_of_content_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:outline-level",			outline_level_);
	CP_APPLY_ATTR(L"text:use-index-marks",			use_index_marks_);
	CP_APPLY_ATTR(L"text:use-index_source-styles",	use_index_source_styles_);
	CP_APPLY_ATTR(L"text:use-outline-level",		use_outline_level_);
}

void table_of_content_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"table-of-content-entry-template")
        CP_CREATE_ELEMENT(entry_templates_);
	else if CP_CHECK_NAME(L"text", L"index-title-template")
		CP_CREATE_ELEMENT(index_title_template_);
	else if CP_CHECK_NAME(L"text", L"index-source-styles")
		CP_CREATE_ELEMENT(index_source_styles_);
}
void table_of_content_source::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_table_content_context().start_template(1);
	for (size_t i = 0; i < entry_templates_.size(); i++)
	{
		entry_templates_[i]->docx_convert(Context);
	}
	Context.get_table_content_context().end_template();
}
//------------------------------------------------------------------------------------------------------
// common-entry-template
//------------------------------------------------------------------------------------------------------
const wchar_t * common_entry_template::ns = L"text";
const wchar_t * common_entry_template::name = L"common-entry-template";

void common_entry_template::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name", style_name_);
}
void common_entry_template::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void common_entry_template::docx_convert(oox::docx_conversion_context & Context)
{
	if (!style_name_)return;

	Context.get_table_content_context().start_level(*style_name_);

	if (outline_level_ && style_name_)
	{
		Context.get_table_content_context().add_outline_level_style(*style_name_, *outline_level_);
	}
	for (size_t i = 0; i < content_.size(); i++)
	{
		switch(content_[i]->get_type())
		{
		case typeTextIndexEntrySpan:		Context.get_table_content_context().add_level_content(1); break;
		case typeTextIndexEntryText:		Context.get_table_content_context().add_level_content(2); break;
		case typeTextIndexEntryLinkStart:	Context.get_table_content_context().add_level_content(3); break;
		case typeTextIndexEntryLinkEnd:		Context.get_table_content_context().add_level_content(4); break;
		case typeTextIndexEntryTabStop:		Context.get_table_content_context().add_level_content(5); break;
		case typeTextIndexEntryPageNumber:	Context.get_table_content_context().add_level_content(6); break;
		case typeTextIndexEntryChapter:		Context.get_table_content_context().add_level_content(7); break;
		case typeTextIndexEntryBibliography:Context.get_table_content_context().add_level_content(8); break;
		default:
			break;

		}
	}
	Context.get_table_content_context().end_level();
}
//------------------------------------------------------------------------------------------------------
// text:table-of-content-entry-template
//------------------------------------------------------------------------------------------------------
const wchar_t * table_of_content_entry_template::ns = L"text";
const wchar_t * table_of_content_entry_template::name = L"table-of-content-entry-template";

void table_of_content_entry_template::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:outline-level",	outline_level_);
	common_entry_template::add_attributes(Attributes);
}
void table_of_content_entry_template::docx_convert(oox::docx_conversion_context & Context)
{
	common_entry_template::docx_convert(Context);
}
//-----------------------------------------------------------------------------------------------
// text:table-index
//-----------------------------------------------------------------------------------------------
const wchar_t * table_index::ns = L"text";
const wchar_t * table_index::name = L"table-index";

void table_index::docx_convert(oox::docx_conversion_context & Context)
{
    if (index_body_)
	{
		Context.start_sdt(4);
		if (table_index_source_)
		{
			table_index_source_->docx_convert(Context);
		}
		index_body_->docx_convert(Context);
		Context.end_sdt();
	}
}
void table_index::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (index_body_)
        index_body_->pptx_convert(Context);
}
std::wostream & table_index::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(index_body_, bXmlEncode);
    return _Wostream;
}
void table_index::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes( Attributes );
}

void table_index::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(index_body_);
    }
    else if CP_CHECK_NAME(L"text", L"table-index-source")
    {
        CP_CREATE_ELEMENT(table_index_source_);
    }
}
//----------------------------------------------------------------------------------------
// text:table-index-source
//----------------------------------------------------------------------------------------
const wchar_t * table_index_source::ns = L"text";
const wchar_t * table_index_source::name = L"table-index-source";

void table_index_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:caption-sequence-name",	caption_sequence_name_);
	CP_APPLY_ATTR(L"text:caption-sequence-format",	caption_sequence_format_);

	CP_APPLY_ATTR(L"text:relative-tab-stop-position",	relative_tab_stop_position_);
	CP_APPLY_ATTR(L"text:use-caption",	use_caption_);
	CP_APPLY_ATTR(L"text:index-scope",	index_scope_); // chapter or document
}
void table_index_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"table-index-entry-template")
        CP_CREATE_ELEMENT(entry_templates_);
	else if CP_CHECK_NAME(L"text", L"index-title-template")
		CP_CREATE_ELEMENT(index_title_template_);
}
void table_index_source::docx_convert(oox::docx_conversion_context & Context)
{
	if (caption_sequence_name_)
	{
		Context.get_table_content_context().caption_sequence_name = *caption_sequence_name_;
	}
	Context.get_table_content_context().start_template(4);
	for (size_t i = 0; i < entry_templates_.size(); i++)
	{
		entry_templates_[i]->docx_convert(Context);
	}
	Context.get_table_content_context().end_template();
}
//---------------------------------------------------------------------------------
// text:table-index-entry-template
//---------------------------------------------------------------------------------
const wchar_t * table_index_entry_template::ns = L"text";
const wchar_t * table_index_entry_template::name = L"table-index-entry-template";
//----------------------------------------------------------------------------------------
// text:illustration-index
//----------------------------------------------------------------------------------------
const wchar_t * illustration_index::ns = L"text";
const wchar_t * illustration_index::name = L"illustration-index";

void illustration_index::afterCreate()
{
 	office_element::afterCreate();

//	if (document_context * context = getContext())
//    {
//        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
//        {
//            lastPar->paragraph_.set_next_section(true);        
//        }
//		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
//        {
//            lastPar->paragraph_.set_next_section(true);        
//        }
//    }
}

void illustration_index::afterReadContent()
{
    if (document_context * context = getContext())
    {
        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
    }
	office_element::afterReadContent();
}
void illustration_index::docx_convert(oox::docx_conversion_context & Context)
{
	if (section_attr_.style_name_ && false == section_attr_.name_.empty()) // ??? ||
	{
		std::wstring current_page_properties = Context.get_page_properties();
		Context.get_section_context().add_section(section_attr_.name_, section_attr_.style_name_.get_value_or(L""), current_page_properties);
		Context.add_page_properties(current_page_properties);
	}

	if (index_body_)
	{
		Context.start_sdt(2);
		if (illustration_index_source_)
		{
			illustration_index_source_->docx_convert(Context);
		}		
		index_body_->docx_convert(Context);
		Context.end_sdt();
	}
}

void illustration_index::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (index_body_)
        index_body_->pptx_convert(Context);
}

std::wostream & illustration_index::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(index_body_, bXmlEncode);
    return _Wostream;
}

void illustration_index::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes( Attributes );
}

void illustration_index::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(index_body_);
    }
	else if CP_CHECK_NAME(L"text", L"illustration-index-source")
	{
        CP_CREATE_ELEMENT(illustration_index_source_);
	}
}
//----------------------------------------------------------------------------------------
// text:illustration-index-source
//----------------------------------------------------------------------------------------
const wchar_t * illustration_index_source::ns = L"text";
const wchar_t * illustration_index_source::name = L"illustration-index-source";


void illustration_index_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:caption-sequence-name",	caption_sequence_name_);
	CP_APPLY_ATTR(L"text:caption-sequence-format",	caption_sequence_format_);

	CP_APPLY_ATTR(L"text:relative-tab-stop-position",	relative_tab_stop_position_);
	CP_APPLY_ATTR(L"text:use-caption",	use_caption_);
	CP_APPLY_ATTR(L"text:index-scope",	index_scope_); // chapter or document

}

void illustration_index_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"illustration-index-entry-template")
        CP_CREATE_ELEMENT(entry_templates_);
	else if CP_CHECK_NAME(L"text", L"index-title-template")
		CP_CREATE_ELEMENT(index_title_template_);

}
void illustration_index_source::docx_convert(oox::docx_conversion_context & Context)
{
	if (caption_sequence_name_)
	{
		Context.get_table_content_context().caption_sequence_name = *caption_sequence_name_;
	}
	Context.get_table_content_context().start_template(2);
	for (size_t i = 0; i < entry_templates_.size(); i++)
	{
		entry_templates_[i]->docx_convert(Context);
	}
	Context.get_table_content_context().end_template();
}
//---------------------------------------------------------------------------------
// text:illustration-index-entry-template
//---------------------------------------------------------------------------------
const wchar_t * illustration_index_entry_template::ns = L"text";
const wchar_t * illustration_index_entry_template::name = L"illustration-index-entry-template";

void illustration_index_entry_template::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:outline-level",	outline_level_);
	common_entry_template::add_attributes(Attributes);
}
void illustration_index_entry_template::docx_convert(oox::docx_conversion_context & Context)
{
	common_entry_template::docx_convert(Context);
}
//------------------------------------------------------------------------------------------------
// text:alphabetical-index
//------------------------------------------------------------------------------------------------
const wchar_t * alphabetical_index::ns = L"text";
const wchar_t * alphabetical_index::name = L"alphabetical-index";

void alphabetical_index::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes( Attributes );
}

void alphabetical_index::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(index_body_);
    }
	else if CP_CHECK_NAME(L"text", L"alphabetical-index-source")
	{
        CP_CREATE_ELEMENT(alphabetical_index_source_);
	}
}
void alphabetical_index::afterCreate()
{
	office_element::afterCreate();
	
//	if (document_context * context = getContext())
//    {
//        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
//        {
//            lastPar->paragraph_.set_next_section(true);        
//        }
//		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
//        {
//            lastPar->paragraph_.set_next_section(true);        
//        }
//    }
}
void alphabetical_index::afterReadContent()
{
    if (document_context * context = getContext())
    {
        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
    }
	office_element::afterReadContent();
}
void alphabetical_index::docx_convert(oox::docx_conversion_context & Context)
{
	if (section_attr_.style_name_ && false == section_attr_.name_.empty()) // ??? ||
	{
		std::wstring current_page_properties = Context.get_page_properties();
		Context.get_section_context().add_section(section_attr_.name_, section_attr_.style_name_.get_value_or(L""), current_page_properties);
		Context.add_page_properties(current_page_properties);
	}

	if (index_body_)
	{
		Context.start_sdt(5);
		if (alphabetical_index_source_)
		{
			alphabetical_index_source_->docx_convert(Context);
		}
		index_body_->docx_convert(Context);
		Context.end_sdt();
	}
}

void alphabetical_index::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (index_body_)
        index_body_->pptx_convert(Context);
}
std::wostream & alphabetical_index::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(index_body_, bXmlEncode);
    return _Wostream;
}
//----------------------------------------------------------------------------------------
// text:alphabetical-index-source
//----------------------------------------------------------------------------------------
const wchar_t * alphabetical_index_source::ns = L"text";
const wchar_t * alphabetical_index_source::name = L"alphabetical-index-source";

void alphabetical_index_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:index-scope",				index_scope_); // chapter or document
	CP_APPLY_ATTR(L"text:alphabetical-separators",	alphabetical_separators_); 
	CP_APPLY_ATTR(L"text:ignore-case",				ignore_case_); 

}
void alphabetical_index_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"alphabetical-index-entry-template")
        CP_CREATE_ELEMENT(entry_templates_);
	else if CP_CHECK_NAME(L"text", L"index-title-template")
		CP_CREATE_ELEMENT(index_title_template_);

}
void alphabetical_index_source::docx_convert(oox::docx_conversion_context & Context)
{
	if (alphabetical_separators_)
	{
		Context.get_table_content_context().bSeparators = alphabetical_separators_->get();
	}	
	
	Context.get_table_content_context().start_template(5);
	for (size_t i = 0; i < entry_templates_.size(); i++)
	{
		entry_templates_[i]->docx_convert(Context);
	}
	Context.get_table_content_context().end_template();
}
//---------------------------------------------------------------------------------
// text:alphabetical_index_entry_template
//---------------------------------------------------------------------------------
const wchar_t * alphabetical_index_entry_template::ns = L"text";
const wchar_t * alphabetical_index_entry_template::name = L"alphabetical-index-entry-template";

void alphabetical_index_entry_template::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:outline-level",	outline_level_);
	common_entry_template::add_attributes(Attributes);
}

void alphabetical_index_entry_template::docx_convert(oox::docx_conversion_context & Context)
{
	common_entry_template::docx_convert(Context);
}
//-----------------------------------------------------------------------------------------------
// text:object-index
//-----------------------------------------------------------------------------------------------
const wchar_t * object_index::ns = L"text";
const wchar_t * object_index::name = L"object-index";

void object_index::afterCreate()
{
	office_element::afterCreate();

//	if (document_context * context = getContext())
//    {
//        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
//        {
//            lastPar->paragraph_.set_next_section(true);        
//        }
//		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
//        {
//            lastPar->paragraph_.set_next_section(true);        
//        }
//    }
}
void object_index::afterReadContent()
{
    if (document_context * context = getContext())
    {
        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
    }
	office_element::afterReadContent();
}
void object_index::docx_convert(oox::docx_conversion_context & Context)
{
    if (index_body_)
	{
		Context.start_sdt(7);
		if (object_index_source_)
		{
			object_index_source_->docx_convert(Context);
		}
		index_body_->docx_convert(Context);
		Context.end_sdt();
	}
}
void object_index::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (index_body_)
        index_body_->pptx_convert(Context);
}
std::wostream & object_index::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(index_body_, bXmlEncode);
    return _Wostream;
}
void object_index::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes( Attributes );
}

void object_index::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(index_body_);
    }
    else if CP_CHECK_NAME(L"text", L"object-index-source")
    {
        CP_CREATE_ELEMENT(object_index_source_);
    }
}
//----------------------------------------------------------------------------------------
// text:object-index-source
//----------------------------------------------------------------------------------------
const wchar_t * object_index_source::ns = L"text";
const wchar_t * object_index_source::name = L"object-index-source";

void object_index_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:relative-tab-stop-position",	relative_tab_stop_position_);
	CP_APPLY_ATTR(L"text:index-scope",	index_scope_); // chapter or document
	CP_APPLY_ATTR(L"text:use-chart-objects",	use_chart_objects_);
	CP_APPLY_ATTR(L"text:use-use-draw-objects",	use_draw_objects_);
	CP_APPLY_ATTR(L"text:use-math-objects",		use_math_objects_);
	CP_APPLY_ATTR(L"text:use-other-objects",	use_other_objects_);
	CP_APPLY_ATTR(L"text:use-spreadsheet-objects",	use_spreadsheet_objects_);
}
void object_index_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"object-index-entry-template")
        CP_CREATE_ELEMENT(entry_templates_);
	else if CP_CHECK_NAME(L"text", L"index-title-template")
		CP_CREATE_ELEMENT(index_title_template_);
}
void object_index_source::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_table_content_context().start_template(7);
	for (size_t i = 0; i < entry_templates_.size(); i++)
	{
		entry_templates_[i]->docx_convert(Context);
	}
	Context.get_table_content_context().end_template();
}
//---------------------------------------------------------------------------------
// text:object-index-entry-template
//---------------------------------------------------------------------------------
const wchar_t * object_index_entry_template::ns = L"text";
const wchar_t * object_index_entry_template::name = L"object-index-entry-template";
//-----------------------------------------------------------------------------------------------
// text:user-index
//-----------------------------------------------------------------------------------------------
const wchar_t * user_index::ns = L"text";
const wchar_t * user_index::name = L"user-index";

void user_index::afterCreate()
{
	office_element::afterCreate();

	if (document_context * context = getContext())
    {
        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_section(true);        
        }
		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_section(true);        
        }
    }
}
void user_index::afterReadContent()
{
    if (document_context * context = getContext())
    {
        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
    }
	office_element::afterReadContent();
}
void user_index::docx_convert(oox::docx_conversion_context & Context)
{
    if (index_body_)
	{
		Context.start_sdt(6);
		if (user_index_source_)
		{
			user_index_source_->docx_convert(Context);
		}
		index_body_->docx_convert(Context);
		Context.end_sdt();
	}
}
void user_index::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (index_body_)
        index_body_->pptx_convert(Context);
}
std::wostream & user_index::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(index_body_, bXmlEncode);
    return _Wostream;
}
void user_index::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes( Attributes );
}

void user_index::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(index_body_);
    }
    else if CP_CHECK_NAME(L"text", L"user-index-source")
    {
        CP_CREATE_ELEMENT(user_index_source_);
    }
}
//----------------------------------------------------------------------------------------
// text:user-index-source
//----------------------------------------------------------------------------------------
const wchar_t * user_index_source::ns = L"text";
const wchar_t * user_index_source::name = L"user-index-source";

void user_index_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:index-name",	index_name_);

	CP_APPLY_ATTR(L"text:relative-tab-stop-position",	relative_tab_stop_position_);
	CP_APPLY_ATTR(L"text:index-scope",	index_scope_); // chapter or document

	CP_APPLY_ATTR(L"text:copy-outline-levels",		copy_outline_levels_);
	CP_APPLY_ATTR(L"text:use-chart-objects",		use_chart_objects_);
	CP_APPLY_ATTR(L"text:use-draw-objects",			use_draw_objects_);
	CP_APPLY_ATTR(L"text:use-graphics",				use_graphics_);
	CP_APPLY_ATTR(L"text:use-floating-frame",		use_floating_frames_);
	CP_APPLY_ATTR(L"text:use-index-marks",			use_index_marks_);
	CP_APPLY_ATTR(L"text:use-index-source-styles",	use_index_source_styles_);
	CP_APPLY_ATTR(L"text:use-objects",				use_objects_);
	CP_APPLY_ATTR(L"text:use-tables",				use_tables_);
}
void user_index_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"user-index-entry-template")
        CP_CREATE_ELEMENT(entry_templates_);
	else if CP_CHECK_NAME(L"text", L"index-title-template")
		CP_CREATE_ELEMENT(index_title_template_);
}
void user_index_source::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_table_content_context().start_template(6);
	for (size_t i = 0; i < entry_templates_.size(); i++)
	{
		entry_templates_[i]->docx_convert(Context);
	}
	Context.get_table_content_context().end_template();
}
//---------------------------------------------------------------------------------
// text:user-index-entry-template
//---------------------------------------------------------------------------------
const wchar_t * user_index_entry_template::ns = L"text";
const wchar_t * user_index_entry_template::name = L"user-index-entry-template";
//-----------------------------------------------------------------------------------------------
const wchar_t * index_entry_bibliography::ns = L"text";
const wchar_t * index_entry_bibliography::name = L"index-entry-bibliography";

void index_entry_bibliography::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style_name", style_name_);
	CP_APPLY_ATTR(L"text:bibliography-data-field", bibliography_data_field_);
}
//-----------------------------------------------------------------------------------------------
const wchar_t * index_entry_chapter::ns = L"text";
const wchar_t * index_entry_chapter::name = L"index-entry-chapter";
//-----------------------------------------------------------------------------------------------
const wchar_t * index_entry_link_end::ns = L"text";
const wchar_t * index_entry_link_end::name = L"index-entry-link-end";
//-----------------------------------------------------------------------------------------------
const wchar_t * index_entry_link_start::ns = L"text";
const wchar_t * index_entry_link_start::name = L"index-entry-link-start";
//-----------------------------------------------------------------------------------------------
const wchar_t * index_entry_page_number::ns = L"text";
const wchar_t * index_entry_page_number::name = L"index-entry-page-number";
//-----------------------------------------------------------------------------------------------
const wchar_t * index_entry_span::ns = L"text";
const wchar_t * index_entry_span::name = L"index-entry-span";
//-----------------------------------------------------------------------------------------------
const wchar_t * index_entry_tab_stop::ns = L"text";
const wchar_t * index_entry_tab_stop::name = L"index-entry-tab-stop";
//-----------------------------------------------------------------------------------------------
const wchar_t * index_entry_text::ns = L"text";
const wchar_t * index_entry_text::name = L"index-entry-text";
//-----------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// text:bibliography
//---------------------------------------------------------------------------------
const wchar_t * bibliography::ns = L"text";
const wchar_t * bibliography::name = L"bibliography";

void bibliography::afterCreate()
{
	office_element::afterCreate();

	if (document_context * context = getContext())
    {
        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_section(true);        
        }
		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_section(true);        
        }
    }
}

void bibliography::afterReadContent()
{
    if (document_context * context = getContext())
    {
        if (p *lastPar = dynamic_cast<p*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
		else if (h *lastPar = dynamic_cast<h*>(context->get_last_paragraph()))
        {
            lastPar->paragraph_.set_next_end_section(true);        
        }
    }
	office_element::afterReadContent();
}
void bibliography::docx_convert(oox::docx_conversion_context & Context)
{
	if (section_attr_.style_name_ && false == section_attr_.name_.empty()) // ??? ||
	{
		std::wstring current_page_properties = Context.get_page_properties();
		Context.get_section_context().add_section(section_attr_.name_, section_attr_.style_name_.get_value_or(L""), current_page_properties);
		Context.add_page_properties(current_page_properties);
	}

	if (index_body_)
	{
		Context.start_sdt(3);
		if (bibliography_source_)
		{
			bibliography_source_->docx_convert(Context);
		}
		index_body_->docx_convert(Context);
		Context.end_sdt();
	}
}

void bibliography::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (index_body_)
	{
        index_body_->pptx_convert(Context);
	}
}

std::wostream & bibliography::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(index_body_, bXmlEncode);
    return _Wostream;
}

void bibliography::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    section_attr_.add_attributes( Attributes );
}

void bibliography::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(index_body_);
    }
    else if CP_CHECK_NAME(L"text", L"bibliography-source")
    {
        CP_CREATE_ELEMENT(bibliography_source_);
    }
}
// text:bibliography-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * bibliography_source::ns = L"text";
const wchar_t * bibliography_source::name = L"bibliography-source";

void bibliography_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"bibliography-entry-template")
        CP_CREATE_ELEMENT(entry_templates_);
	else if CP_CHECK_NAME(L"text", L"index-title-template")
		CP_CREATE_ELEMENT(index_title_template_);
}
void bibliography_source::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_table_content_context().start_template(3);
	for (size_t i = 0; i < entry_templates_.size(); i++)
	{
		entry_templates_[i]->docx_convert(Context);
	}
	Context.get_table_content_context().end_template();
}
// text:bibliography-entry-template
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * bibliography_entry_template::ns = L"text";
const wchar_t * bibliography_entry_template::name = L"bibliography-entry-template";

void bibliography_entry_template::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:bibliography-type", bibliography_type_);
	common_entry_template::add_attributes( Attributes );
}
void bibliography_entry_template::docx_convert(oox::docx_conversion_context & Context)
{
	common_entry_template::docx_convert( Context );
}
//--------------------------------------------------------------------------------------------------------
// text:tracked-changes
const wchar_t * tracked_changes::ns		= L"text";
const wchar_t * tracked_changes::name	= L"tracked-changes";

void tracked_changes::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:track-changes", track_changes_);
}

void tracked_changes::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void tracked_changes::docx_convert(oox::docx_conversion_context & Context)
{
    for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->docx_convert(Context);
	}
}
//--------------------------------------------------------------------------------------------------------
// text:tracked-changes
const wchar_t * changed_region::ns		= L"text";
const wchar_t * changed_region::name	= L"changed-region";

void changed_region::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:id", id_);
}

void changed_region::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void changed_region::docx_convert(oox::docx_conversion_context & Context)
{
	if (content_.empty() || !id_) return;

	Context.get_text_tracked_context().start_change (*id_);

	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->docx_convert(Context);
	}
	
	Context.get_text_tracked_context().end_change ();
}

//----------------------------------------------------------------------------------------------------------
const wchar_t * unknown_base_change::ns		= L"text";
const wchar_t * unknown_base_change::name	= L"unknown-base-change"; 

void unknown_base_change::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"office", L"change-info")
	{
		CP_CREATE_ELEMENT(office_change_info_);
	}
	else
		CP_CREATE_ELEMENT(content_);
}

void unknown_base_change::docx_convert(oox::docx_conversion_context & Context)
{
	ElementType type = get_type();

	if (office_change_info_)
		office_change_info_->docx_convert(Context);

	if (content_.empty()) return;

//тут удаленный текст. не по стандарту сделать бы и форматы - стилями чтоли ....

	for (size_t i = 0; i < content_.size(); i++)
	{
		Context.get_text_tracked_context().start_changes_content();
		{
			content_[i]->docx_convert(Context);
			//h *h_ = dynamic_cast<h*>(content_[i].get());
			//p *p_ = dynamic_cast<p*>(content_[i].get());

			//paragraph *para = NULL;

			//if (h_) para = &h_->paragraph_;
			//if (p_) para = &p_->paragraph_;

			//if (para)
			//{
			//	for (size_t j = 0; j < para->content_.size(); j++)
			//	{
			//		para->content_[j]->docx_convert(Context);
			//	}
			//}
		}
		Context.get_text_tracked_context().end_changes_content();
	}
}
//---------------------------------------------------------------------------------------------------
const wchar_t * insertion::ns			= L"text";
const wchar_t * insertion::name		= L"insertion";

void insertion::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_text_tracked_context().set_type(1);

	unknown_base_change::docx_convert(Context);
}
//---------------------------------------------------------------------------------------------------
const wchar_t * deletion::ns			= L"text";
const wchar_t * deletion::name			= L"deletion";

void deletion::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_text_tracked_context().set_type(2);

	unknown_base_change::docx_convert(Context);
}
//---------------------------------------------------------------------------------------------------

const wchar_t * format_change::ns		= L"text";
const wchar_t * format_change::name	= L"format-change";

void format_change::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name", style_name_);
}

void format_change::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_text_tracked_context().set_type(3);

	if (style_name_)
		Context.get_text_tracked_context().set_style_name(*style_name_);

	unknown_base_change::docx_convert(Context);
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * unknown_change::ns		= L"text";
const wchar_t * unknown_change::name	= L"UnknownChange"; //?? libra пишет

void unknown_change::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_text_tracked_context().set_type(3);

	unknown_base_change::docx_convert(Context);
}
//---------------------------------------------------------------------------------------------------
const wchar_t * add_change::ns		= L"text";
const wchar_t * add_change::name	= L"unknown-change-mark";

void add_change::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:change-id", change_id_);
}

//---------------------------------------------------------------------------------------------------
const wchar_t * change::ns			= L"text";
const wchar_t * change::name		= L"change";

void change::docx_convert(oox::docx_conversion_context & Context)
{
	if (!change_id_) return;

	Context.start_text_changes	(*change_id_);
	Context.end_text_changes	(*change_id_);
}
//------------------------------------------------------------------------------------------
const wchar_t * change_start::ns	= L"text";
const wchar_t * change_start::name	= L"change-start";

void change_start::docx_convert(oox::docx_conversion_context & Context)
{
	Context.start_text_changes (*change_id_);
}
//------------------------------------------------------------------------------------------
const wchar_t * change_end::ns		= L"text";
const wchar_t * change_end::name	= L"change-end";

void change_end::docx_convert(oox::docx_conversion_context & Context)
{
	Context.end_text_changes (*change_id_);
}
//---------------------------------------------------------------------------------------------------
const wchar_t * variable_input::ns		= L"text";
const wchar_t * variable_input::name	= L"variable-input";

void variable_input::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"office:value-type",		office_value_type_);
	CP_APPLY_ATTR(L"style:data-style-name",	style_data_style_name_);
	CP_APPLY_ATTR(L"text:description",		description_);
	CP_APPLY_ATTR(L"text:display",			display_);
	CP_APPLY_ATTR(L"text:name",				name_);
}
void variable_input::add_text(const std::wstring & Text) 
{
    text_ = Text;
}
std::wostream & variable_input::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    _Wostream << XmlUtils::EncodeXmlString( text_, bXmlEncode );
    return _Wostream;
}

void variable_input::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
	Context.finish_run();
	
	strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>";
	strm << L"<w:r><w:instrText>ASK &quot;" << name_.get_value_or(L"") << L"&quot; " << description_.get_value_or(L"") << L" \\d ";
	strm << text_;
	strm << L"</w:instrText></w:r>";
	strm << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
    strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";

	strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>";
	strm << L"<w:r><w:instrText>REF &quot;" << name_.get_value_or(L"") << L"&quot; </w:instrText></w:r>";
	strm << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	strm << L"<w:r><w:t>" << text_ << L"</w:t></w:r>";
	strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";

}
//---------------------------------------------------------------------------------------------------
const wchar_t * variable_get::ns	= L"text";
const wchar_t * variable_get::name	= L"variable-get";

void variable_get::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name",	style_data_style_name_);
	CP_APPLY_ATTR(L"text:display",			display_);
	CP_APPLY_ATTR(L"text:name",				name_);
}
void variable_get::docx_convert(oox::docx_conversion_context & Context)
{

}
//---------------------------------------------------------------------------------------------------
const wchar_t * variable_set::ns	= L"text";
const wchar_t * variable_set::name	= L"variable-set";

void variable_set::add_text(const std::wstring & Text)
{
	office_element_ptr elm = text::create(Text);
	content_.push_back(elm);
}
void variable_set::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name",	style_data_style_name_);
	CP_APPLY_ATTR(L"text:display",			display_);
	CP_APPLY_ATTR(L"text:name",				name_);

	CP_APPLY_ATTR(L"office:formula",		office_formula_);
	
	office_value_.add_attributes(Attributes);
}
void variable_set::docx_convert(oox::docx_conversion_context & Context)
{
	Context.finish_run();

	Context.output_stream() << L"<w:sdt>";
	Context.output_stream() << L"<w:sdtPr>";
	{
		Context.output_stream() << L"<w:id w:val=\"" + std::to_wstring(Context.get_drawing_context().get_current_shape_id()) + L"\"/>";
		if (name_)
		{
			Context.output_stream() << L"<w:placeholder/>";
			Context.output_stream() << L"<w:docPart w:val=\"" + XmlUtils::EncodeXmlString(*name_) + L"\"/>";
		}
		Context.output_stream() << L"<w:showingPlcHdr/>";
		Context.output_stream() << L"<w:text/>";
	}
	Context.output_stream() << L"</w:sdtPr>";
	Context.output_stream() << L"<w:sdtContent>";
	{
		for (size_t i = 0; i < content_.size(); i++)
		{
			content_[i]->docx_convert(Context);
		}
	}
	Context.finish_run();
	Context.output_stream() << L"</w:sdtContent>";
	Context.output_stream() << L"</w:sdt>";
}
//---------------------------------------------------------------------------------------------------
const wchar_t * variable_decl::ns		= L"text";
const wchar_t * variable_decl::name	= L"variable-decl";

void variable_decl::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"office:value-type",		office_value_type_);
	CP_APPLY_ATTR(L"text:display",			display_);
	CP_APPLY_ATTR(L"text:name",				name_);
}
void variable_decl::docx_convert(oox::docx_conversion_context & Context)
{

}
//---------------------------------------------------------------------------------------------------
const wchar_t * variable_decls::ns		= L"text";
const wchar_t * variable_decls::name	= L"variable-decls";

void variable_decls::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"text", L"variable-decl")
	{
		CP_CREATE_ELEMENT(content_);
	}
	else
	{
	}
}
void variable_decls::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->docx_convert(Context);
	}
}
//---------------------------------------------------------------------------------------------------
const wchar_t * user_field_decl::ns	= L"text";
const wchar_t * user_field_decl::name	= L"user-field-decl";

void user_field_decl::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:name",				name_);
	CP_APPLY_ATTR(L"office:formula",		office_formula_);

	office_value_.add_attributes(Attributes);
}
void user_field_decl::docx_convert(oox::docx_conversion_context & Context)
{
	if (!name_) return;

	std::wstring value;

	if (office_value_.office_string_value_)		value = *office_value_.office_string_value_;
	else if (office_value_.office_value_)		value = *office_value_.office_value_;
	else if (office_value_.office_date_value_)	value = *office_value_.office_date_value_;
	else if (office_value_.office_time_value_)	value = *office_value_.office_time_value_;
	else if (office_value_.office_currency_)	value = *office_value_.office_currency_;
	else if (office_value_.office_boolean_value_)value = *office_value_.office_boolean_value_;

	Context.add_user_field(*name_, value);
}
//---------------------------------------------------------------------------------------------------
const wchar_t * user_field_decls::ns	= L"text";
const wchar_t * user_field_decls::name	= L"user-field-decls";

void user_field_decls::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"text", L"user-field-decl")
	{
		CP_CREATE_ELEMENT(content_);
	}
	else
	{
	}
}
void user_field_decls::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->docx_convert(Context);
	}
}
//---------------------------------------------------------------------------------------------------
const wchar_t * sequence_decl::ns	= L"text";
const wchar_t * sequence_decl::name	= L"sequence-decl";

void sequence_decl::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:name",						name_);
	CP_APPLY_ATTR(L"text:display-outline-level",	display_outline_level_);
	CP_APPLY_ATTR(L"text:separation-character",		separation_character_);
}
void sequence_decl::docx_convert(oox::docx_conversion_context & Context)
{
	if (!name_) return;

	Context.get_table_content_context().add_sequence(*name_, display_outline_level_ ? *display_outline_level_ : -1);
}
//---------------------------------------------------------------------------------------------------
const wchar_t * sequence_decls::ns		= L"text";
const wchar_t * sequence_decls::name	= L"sequence-decls";

void sequence_decls::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"text", L"sequence-decl")
	{
		CP_CREATE_ELEMENT(content_);
	}
	else
	{
	}
}
void sequence_decls::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->docx_convert(Context);
	}
}
//---------------------------------------------------------------------------------------------------
const wchar_t * index_source_styles::ns = L"text";
const wchar_t * index_source_styles::name = L"index-source-styles";

void index_source_styles::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:outline-level", outline_level_);
}
void index_source_styles::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void index_source_styles::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->docx_convert(Context);
	}
}
//---------------------------------------------------------------------------------------------------
const wchar_t * index_source_style::ns = L"text";
const wchar_t * index_source_style::name = L"index-source-style";

void index_source_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name", style_name_);
}
void index_source_style::docx_convert(oox::docx_conversion_context & Context)
{

}
}
}
}
