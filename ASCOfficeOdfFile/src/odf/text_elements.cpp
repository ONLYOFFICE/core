
#include "text_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "paragraph_elements.h"
#include "serialize_elements.h"
#include "list.h"

#include <cpdoccore/odf/odf_document.h>
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

void process_page_break_after(const style_instance * styleInst, oox::docx_conversion_context & Context)
{
    if (styleInst)
    {
        const style_instance * inst = styleInst;
        while (inst)
        {
            if (inst->content() && inst->content()->get_style_paragraph_properties())
            {
                _CP_OPT(fo_break) fo_break_val = inst->content()->get_style_paragraph_properties()->content().fo_break_after_;
                if (fo_break_val)
                {
                    if (fo_break_val->get_type() == fo_break::Page)
                    {
                        Context.set_page_break_after(true);     
                        break;
                    }
                    else if (fo_break_val->get_type() == fo_break::Auto)
                    {
                        break;                    
                    }
                }
            }
            inst = inst->parent();
        }
    }
}
void process_paragraph_drop_cap_attr(const paragraph_attrs & Attr, oox::docx_conversion_context & Context)
{
    if (Attr.text_style_name_.empty())return;

	style_instance * styleInst 
            = Context.root()->odf_context().styleContainer().style_by_name(Attr.text_style_name_.style_name(), style_family::Paragraph,Context.process_headers_footers_);
    if ((!styleInst) || (styleInst->is_automatic() == false))return;

	style_content * styleContent = styleInst->content();
    if (!styleContent)return;

	style_paragraph_properties * paragraph_properties = styleContent->get_style_paragraph_properties();
	
	if (!paragraph_properties)return;

	const office_element_ptr & elm_style_drop_cap = paragraph_properties->content().style_drop_cap_;

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
	style_text_properties * text_properties = styleContent->get_style_text_properties();
	if ((text_properties) && (text_properties->content().fo_font_size_))
	{
		Context.get_drop_cap_context().FontSize = styleContent->get_style_text_properties()->content().process_font_size(
				text_properties->content().fo_font_size_, Context.get_styles_context().get_current_processed_style(),false,
					 7.25 * (Context.get_drop_cap_context().Scale + (Context.get_drop_cap_context().Scale-1) * 0.7));//формула ачуметь !! - подбор вручную
	}
	return;
}

int process_paragraph_attr(const paragraph_attrs & Attr, oox::docx_conversion_context & Context)
{
    if (!Attr.text_style_name_.empty())
    {
        if (style_instance * styleInst 
            = Context.root()->odf_context().styleContainer().style_by_name(Attr.text_style_name_.style_name(), style_family::Paragraph,Context.process_headers_footers_)
            )
        {
            process_page_break_after(styleInst, Context);
            if (styleInst->is_automatic())
            {
                if (style_content * styleContent = styleInst->content())
                {
                    std::wstring id;
                    if (const style_instance * parentStyleContent = styleInst->parent())
					{
                        id = Context.get_style_map().get( parentStyleContent->name(), parentStyleContent->type() );
					}

                    Context.start_automatic_style(id);
					
					{//вытаскивает rtl c цепочки стилей !! - просто прописать в наследуемом Ќ≈Ћ№«я !!
						paragraph_format_properties properties = calc_paragraph_properties_content(styleInst);

 						if (properties.style_writing_mode_)
						{
							writing_mode::type type = properties.style_writing_mode_->get_type();
							switch(type)
							{
							case writing_mode::RlTb:
							case writing_mode::TbRl:
							case writing_mode::Rl:
								Context.set_rtl(true);
								break;
							default:
								Context.set_rtl(false);
							}
						}
					}
                    
					styleContent->docx_convert(Context);                
                   
					Context.end_automatic_style();

                    Context.push_text_properties(styleContent->get_style_text_properties());
                    return 1;
                }            
            }
            else
            {
                const std::wstring id = Context.get_style_map().get( styleInst->name(), styleInst->type() );
				std::wostream & _Wostream = Context.output_stream();
                _Wostream << L"<w:pPr>";
					Context.process_page_properties(_Wostream);
					_Wostream << L"<w:pStyle w:val=\"" << id << L"\" />";
					Context.docx_serialize_list_properties(_Wostream);
                _Wostream << L"</w:pPr>";
				return 2;
			}
        }
    }
    return 0;
}
}

::std::wostream & paragraph::text_to_stream(::std::wostream & _Wostream) const
{
    // TODO!!!!
    CP_SERIALIZE_TEXT(paragraph_content_);
    _Wostream << L"\n";
    return _Wostream;
}

void paragraph::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    paragraph_attrs_.add_attributes(Attributes);
}

void paragraph::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context)
{
    CP_CREATE_ELEMENT_SIMPLE(paragraph_content_);
}

void paragraph::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    paragraph_content_.push_back( elm );
}

void paragraph::afterCreate(document_context * Context)
{
    // вызываетс€ сразу после создани€ объекта
    if (Context)
    {
        // выставл€ем у предыдущего параграфа указатель на следующий (т.е. на вновь созданный)
        if (paragraph * prevPar = Context->get_last_paragraph())
            prevPar->set_next(this);
        
        // запоминаем в контексте вновь созданный параграф
        Context->set_last_paragraph(this);
    }
}
const wchar_t * emptyParagraphContent = L"<w:pPr></w:pPr><w:r><w:rPr></w:rPr></w:r>";

const wchar_t * emptyParagraphDrawing = L"<w:p><w:pPr></w:pPr></w:p>";

void paragraph::drop_cap_text_docx_convert(office_element_ptr first_text_element_paragraph,oox::docx_conversion_context & Context)
{
	if (first_text_element_paragraph == NULL)return;
	text* first_text_paragraph = dynamic_cast<text*>(first_text_element_paragraph.get());
	if (first_text_paragraph == NULL)return;
	
	std::wstring & str = first_text_paragraph->attr_text();
	std::wstring store_str = str;

	if (Context.get_drop_cap_context().Length == -1)Context.get_drop_cap_context().Length = store_str.find(L" ");//find length word

	str = store_str.substr(0,Context.get_drop_cap_context().Length);
	
	int textStyle = process_paragraph_attr(paragraph_attrs_, Context);
	first_text_paragraph->docx_convert(Context); 

	int str_start = Context.get_drop_cap_context().Length;
	int str_size = store_str.length()-Context.get_drop_cap_context().Length;

	if (str_size <0) str_size = 0;										// это если на буквы в буквице разные стили
	if (str_start > store_str.length()) str_start = store_str.length(); // это если на буквы в буквице разные стили

	str=store_str.substr(str_start, str_size);
}
void paragraph::drop_cap_docx_convert(oox::docx_conversion_context & Context)
{
	if ( paragraph_content_.size()<1)return;

	//в рассчет берутс€ только первые элементы !!! разные там break-и отмен€ют реэжим drop_cap!!- todooo сделать возможным множественным span
	if ( paragraph_content_[0]->get_type() == typeTextText)
	{
		drop_cap_text_docx_convert(paragraph_content_[0],Context);
	}
	else if (paragraph_content_[0]->get_type() == typeTextSpan)
	{
		span* first_span_in_paragraph = dynamic_cast<span*>(paragraph_content_[0].get());
		if (Context.get_drop_cap_context().FontSize<1)
		{
			style_instance * styleInst = Context.root()->odf_context().styleContainer().style_by_name(first_span_in_paragraph->text_style_name_.style_name(), style_family::Text,Context.process_headers_footers_);
			if ((styleInst) && (styleInst->is_automatic()))
			{
				style_content * styleContent = styleInst->content();
				if (styleContent)
				{
					style_text_properties * text_properties = styleContent->get_style_text_properties();
					if ((text_properties) && (text_properties->content().fo_font_size_))
					{
						Context.get_drop_cap_context().FontSize = styleContent->get_style_text_properties()->content().process_font_size(
							text_properties->content().fo_font_size_, Context.get_styles_context().get_current_processed_style(),false,
									 7.25 * (Context.get_drop_cap_context().Scale + (Context.get_drop_cap_context().Scale-1) * 0.7));
					}
				}
			}
		}
		//в рассчет берутс€ только первые элементы !!! разные там break-и отмен€ют реэжим drop_cap!!
		if ((first_span_in_paragraph->paragraph_content_.size()>0) &&
			 (first_span_in_paragraph->paragraph_content_[0]->get_type() == typeTextText))
			drop_cap_text_docx_convert(first_span_in_paragraph->paragraph_content_[0],Context);
	}
}
void paragraph::docx_convert(oox::docx_conversion_context & Context)
{
	bool drawing	= false;

 	if (Context.get_drawing_context().get_current_shape() || Context.get_drawing_context().get_current_frame())
	{
		drawing = true;
	}
				
	if (Context.get_paragraph_state() && (Context.get_process_note() == oox::docx_conversion_context::noNote) && !drawing )
    {//вложеннные элементы
		if (paragraph_content_.size()==0)
		{
			Context.output_stream() << L"<w:p>";
			Context.output_stream() << emptyParagraphContent;
			Context.output_stream() << L"</w:p>";
		}

        BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
        {
            elm->docx_convert(Context); 
        }
        return;
    }
	bool is_empty= paragraph_content_.size()==0;

 //   BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
 //   {
 //		if (elm->get_type() == typeTextSoftPageBreak && !Context.get_page_break_after() && !Context.get_page_break_before() && !(next_section_ || next_end_section_))
	//	{//1 (206).odt - понатыканы soft-break в различных ситуаци€х
	//		// 1 (130).odt конфликт (
	//		Context.set_page_break_before(true);
	//	}
	//}

    Context.start_paragraph();
	
	std::wostream & _Wostream = Context.output_stream();

    if (next_par_)
    {
        // провер€ем не сменит ли следующий параграф свойства страницы. 
        // если да Ч устанавливаем контексту флаг на то что необходимо в конце текущего параграфа 
        // распечатать свойства секции
		//проверить ... не она ли основна€ - может быть прописан дубл€ж - и тогда разрыв нарисуетс€ ненужный
        const std::wstring & styleName = next_par_->paragraph_attrs_.text_style_name_.style_name();
        const std::wstring masterPageName = Context.root()->odf_context().styleContainer().master_page_name_by_name(styleName);

        if (!masterPageName.empty()  && Context.get_master_page_name() != masterPageName)
        {
            const std::wstring pageLayoutName = Context.root()->odf_context().pageLayoutContainer().page_layout_name_by_style(masterPageName);
            Context.next_dump_page_properties();
            Context.get_section_context().set_after_section(false);

			is_empty = false;
        }
    }
    
    if (next_section_ || next_end_section_)
	{
        Context.next_dump_page_properties();
		is_empty = false;
	}

    const std::wstring & styleName = paragraph_attrs_.text_style_name_.style_name();
    const std::wstring masterPageName = Context.root()->odf_context().styleContainer().master_page_name_by_name(styleName);
    if (!masterPageName.empty())
    {
        const std::wstring masterPageNameLayout = Context.root()->odf_context().pageLayoutContainer().page_layout_name_by_style(masterPageName);
        Context.set_page_properties(masterPageNameLayout);
        Context.set_master_page_name(masterPageName);

		is_empty = false;
    }    
	process_paragraph_drop_cap_attr(paragraph_attrs_, Context);

	if (Context.get_drop_cap_context().state() == 2)//active
	{
		drop_cap_docx_convert(Context); 
		
		Context.finish_run();
		Context.finish_paragraph();		
		
		Context.get_drop_cap_context().state(1);//after 
		Context.start_paragraph();

	}

    int textStyle = process_paragraph_attr(paragraph_attrs_, Context);

    Context.add_note_reference();

    BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
    {
        elm->docx_convert(Context); 
 		
		if (Context.get_drop_cap_context().state() >0)		
			Context.get_drop_cap_context().state(0);//disable

		if (elm->get_type() == typeTextSoftPageBreak && !Context.get_page_break_after() && !Context.get_page_break_before())
		{
			_Wostream << L"<w:lastRenderedPageBreak/>";

			if (next_section_ || next_end_section_)
				Context.set_page_break_before(true);
		}

   }

    if (textStyle>0)
	{
		is_empty = false;
        if (textStyle==1) Context.pop_text_properties();
	}

    Context.finish_run();

    // конвертируем в контексте параграфа элементы, которые были помещены в очередь
	if (!Context.delayed_converting_)//иначе возможно зацикливание
	{
		Context.docx_convert_delayed();
	}

    if (Context.get_page_break_after())
    {
		Context.set_page_break_after(false);

		is_empty = false;
        Context.add_new_run(_T(""));
        _Wostream << L"<w:br w:type=\"page\" />";        
        Context.finish_run();
    }

	if (is_empty)
		Context.output_stream() << emptyParagraphContent;


	Context.finish_paragraph();
}

void paragraph::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_paragraph(paragraph_attrs_.text_style_name_.style_name());
    BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
    {
        elm->xlsx_convert(Context); 
    }
    Context.end_paragraph();    
}
void paragraph::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_paragraph(paragraph_attrs_.text_style_name_.style_name());
    BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
    {
        elm->pptx_convert(Context); 
    }
    Context.get_text_context().end_paragraph();    
}
//////////////////////////////////////////////

::std::wostream & h::text_to_stream(::std::wostream & _Wostream) const
{
    return paragraph_.text_to_stream(_Wostream);
}

h::h() : text_outline_level_(1), text_restart_numbering_(false), text_is_list_header_(false)
{}

void h::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:outline-level", text_outline_level_, (unsigned int)1);
    CP_APPLY_ATTR(L"text:restart-numbering", text_restart_numbering_, false);
    CP_APPLY_ATTR(L"text:start-value", text_start_value_);
    CP_APPLY_ATTR(L"text:is-list-header", text_is_list_header_, false);

    paragraph_.add_attributes(Attributes);
}

void h::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    paragraph_.add_child_element(Reader, Ns, Name, getContext());
}

void h::add_text(const std::wstring & Text)
{
    paragraph_.add_text(Text);
}

void h::afterCreate()
{
    paragraph_.afterCreate( getContext() );
}

void h::docx_convert(oox::docx_conversion_context & Context) 
{
    paragraph_.docx_convert(Context);
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

void p::afterCreate()
{
    paragraph_.afterCreate( getContext() );
}

::std::wostream & p::text_to_stream(::std::wostream & _Wostream) const
{
    return paragraph_.text_to_stream(_Wostream);
}

void p::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    paragraph_.add_attributes(Attributes);
}

void p::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    paragraph_.add_child_element(Reader, Ns, Name, getContext());
}

void p::add_text(const std::wstring & Text)
{
    paragraph_.add_text(Text);
}

void p::docx_convert(oox::docx_conversion_context & Context)
{
    paragraph_.docx_convert(Context);
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

::std::wostream & list::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & listItem, text_list_items_)
    {
        listItem->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void list::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_style_name_ = style_ref( Attributes->get_val< ::std::wstring >(L"text:style-name").get_value_or(L"") );
    text_continue_numbering_ = Attributes->get_val< bool >(L"text:continue-numbering");
    // TODO
}

void list::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"list-header")
    {
        CP_CREATE_ELEMENT(text_list_header_);
    }
    else
    {
        CP_CREATE_ELEMENT(text_list_items_);
    }
}

void list::add_text(const std::wstring & Text)
{
    // TODO : false
}

void list::docx_convert(oox::docx_conversion_context & Context)
{
    bool continue_ = text_continue_numbering_.get_value_or(false);
    Context.start_list(text_style_name_.style_name(), continue_);

    if (text_list_header_)
        text_list_header_->docx_convert(Context);

    BOOST_FOREACH(const office_element_ptr & elm, text_list_items_)
    {
        elm->docx_convert(Context);
    }

    Context.end_list();
}
void list::pptx_convert(oox::pptx_conversion_context & Context)
{
    bool continue_ = text_continue_numbering_.get_value_or(false);
    Context.get_text_context().start_list(text_style_name_.style_name(), continue_);

    if (text_list_header_)
        text_list_header_->pptx_convert(Context);

    BOOST_FOREACH(const office_element_ptr & elm, text_list_items_)
    {
        elm->pptx_convert(Context);
    }

    Context.get_text_context().end_list();
}
// text:soft-page-break
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * soft_page_break::ns = L"text";
const wchar_t * soft_page_break::name = L"soft-page-break";

::std::wostream & soft_page_break::text_to_stream(::std::wostream & _Wostream) const
{
    _Wostream << L"\n";
    return _Wostream;
}

void soft_page_break::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void soft_page_break::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
}

// text-section-attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_section_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:style-name", text_style_name_);
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"text:protected", text_protected_);
    CP_APPLY_ATTR(L"text:protection-key", text_protection_key_);
    CP_APPLY_ATTR(L"text:display", text_display_);
    CP_APPLY_ATTR(L"text:condition", text_condition_);
}

// text:section
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_section::ns = L"text";
const wchar_t * text_section::name = L"section";

::std::wostream & text_section::text_to_stream(::std::wostream & _Wostream) const
{
    return serialize_elements_text(_Wostream, text_content_);
}

void text_section::afterCreate()
{
    if (document_context * context = getContext())
    {
        if (paragraph * lastPar = context->get_last_paragraph())
        {
            lastPar->set_next_section(true);        
        }
    }
}

void text_section::afterReadContent()
{
    if (document_context * context = getContext())
    {
        if (paragraph * lastPar = context->get_last_paragraph())
        {
            lastPar->set_next_end_section(true);        
        }
    }
}

void text_section::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_section_attr_.add_attributes(Attributes);
}

void text_section::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"text" == Ns && L"section-source" == Name)
    {
        CP_CREATE_ELEMENT(text_section_source_);    
    }
    else
    {
        CP_CREATE_ELEMENT(text_content_);
    }
}

void text_section::docx_convert(oox::docx_conversion_context & Context)
{
    Context.get_section_context().start_section(
        text_section_attr_.text_name_, 
        text_section_attr_.text_style_name_.get_value_or(style_ref()).style_name()
        );

    BOOST_FOREACH(const office_element_ptr & elm, text_content_)
    {
        elm->docx_convert(Context);
    }

    Context.get_section_context().end_section();
}

// text-section-source-attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_section_source_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_xlink_attlist_.add_attributes(Attributes);

    CP_APPLY_ATTR(L"text:section-name", text_section_name_);
    CP_APPLY_ATTR(L"text:filter-name", text_filter_name_);
}

// text:section-source
// text-section-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_section_source::ns = L"text";
const wchar_t * text_section_source::name = L"section-source";

void text_section_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_section_source_attr_.add_attributes(Attributes);
}

void text_section_source::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

// text:index-body
// text-index-body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_index_body::ns = L"text";
const wchar_t * text_index_body::name = L"index-body";

::std::wostream & text_index_body::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(index_content_main_);
    return _Wostream;
}

void text_index_body::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void text_index_body::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(index_content_main_);
}

void text_index_body::docx_convert(oox::docx_conversion_context & Context) 
{
    BOOST_FOREACH(const office_element_ptr & elm, index_content_main_)
    {
        elm->docx_convert(Context);
    }
}
void text_index_body::pptx_convert(oox::pptx_conversion_context & Context) 
{
    BOOST_FOREACH(const office_element_ptr & elm, index_content_main_)
    {
        elm->pptx_convert(Context);
    }
}
// text:index-title
// text-index-title
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_index_title::ns = L"text";
const wchar_t * text_index_title::name = L"index-title";

void text_index_title::docx_convert(oox::docx_conversion_context & Context) 
{
    BOOST_FOREACH(office_element_ptr & elm, index_content_main_)
    {
        elm->docx_convert(Context);
    }
}
void text_index_title::pptx_convert(oox::pptx_conversion_context & Context) 
{
    BOOST_FOREACH(office_element_ptr & elm, index_content_main_)
    {
        elm->pptx_convert(Context);
    }
}
::std::wostream & text_index_title::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(index_content_main_);
    return _Wostream;
}

void text_index_title::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_section_attr_.add_attributes(Attributes);
}

void text_index_title::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
     CP_CREATE_ELEMENT(index_content_main_);
}

// text:table-of-content
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_table_of_content::ns = L"text";
const wchar_t * text_table_of_content::name = L"table-of-content";

void text_table_of_content::docx_convert(oox::docx_conversion_context & Context)
{
    if (text_index_body_)
        text_index_body_->docx_convert(Context);
}

void text_table_of_content::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (text_index_body_)
        text_index_body_->pptx_convert(Context);
}


::std::wostream & text_table_of_content::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_index_body_);
    return _Wostream;
}

void text_table_of_content::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_section_attr_.add_attributes( Attributes );
}

void text_table_of_content::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(text_index_body_);
    }
    // TODO text-table-of-content-source
}

// text:table-index
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_table_index::ns = L"text";
const wchar_t * text_table_index::name = L"table-index";

void text_table_index::docx_convert(oox::docx_conversion_context & Context)
{
    if (text_index_body_)
        text_index_body_->docx_convert(Context);
}

void text_table_index::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (text_index_body_)
        text_index_body_->pptx_convert(Context);
}


::std::wostream & text_table_index::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_index_body_);
    return _Wostream;
}

void text_table_index::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_section_attr_.add_attributes( Attributes );
}

void text_table_index::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(text_index_body_);
    }
    // TODO text-table-index-source
}
// text:illustration-index
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_illustration_index::ns = L"text";
const wchar_t * text_illustration_index::name = L"illustration-index";

void text_illustration_index::docx_convert(oox::docx_conversion_context & Context)
{
    if (text_index_body_)
        text_index_body_->docx_convert(Context);
}

void text_illustration_index::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (text_index_body_)
        text_index_body_->pptx_convert(Context);
}


::std::wostream & text_illustration_index::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_index_body_);
    return _Wostream;
}

void text_illustration_index::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_section_attr_.add_attributes( Attributes );
}

void text_illustration_index::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(text_index_body_);
    }
    // TODO text-illustration-index-source
}

}
}
}
