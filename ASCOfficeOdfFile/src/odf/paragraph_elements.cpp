/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "paragraph_elements.h"

#include <ostream>
#include <string>

#include <boost/algorithm/string.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>
#include <cpdoccore/common/readstring.h>
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPWeakPtr.h>

#include "styles.h"
#include "note.h"
#include "ruby.h"
#include "office_elements_create.h"

#include "serialize_elements.h"
#include "odfcontext.h"

#include "style_text_properties.h"

#include "datatypes/targetframename.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

namespace text {

template <class ElementT>
void paragraph_content_element<ElementT>::docx_serialize_field(const std::wstring & field_name, office_element_ptr & text, 
															   oox::docx_conversion_context & Context, bool bLock )
{
	std::wostream & strm = Context.output_stream();
	Context.finish_run();	
	
	if (false == field_name.empty())
	{
		strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"";
		if (bLock)
		{
			strm << L" w:fldLock=\"1\"";
		}
		strm << L"/></w:r>";
		strm << L"<w:r><w:instrText>" << field_name << L"</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	}

	docx_serialize_run(text, Context);
 	
	if (false == field_name.empty())
	{
	   strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
	}
}
template <class ElementT>
void paragraph_content_element<ElementT>::docx_serialize_sdt(const std::wstring & name, office_element_ptr & text, oox::docx_conversion_context & Context)
{
	std::wostream & strm = Context.output_stream();
	Context.finish_run();	

	strm << L"<w:sdt><w:sdtPr><w:alias w:val=\"";
	strm << name;
	strm << L"\"/><w:temporary/>";
	strm << L"<w:showingPlcHdr/><w:text/></w:sdtPr><w:sdtContent>";

	//if (!text)
	//{
	//	text = text::create(L"Enter your text here") ;
	//}

	docx_serialize_run(text, Context);

	strm << L"</w:sdtContent></w:sdt>";
}

template <class ElementT>
void paragraph_content_element<ElementT>::docx_serialize_run(office_element_ptr & text, oox::docx_conversion_context & Context)
{
	Context.add_new_run();
	if (text)
	{
		text->docx_convert(Context);
	}
	Context.finish_run();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * text::ns = L"";
const wchar_t * text::name = L"";

std::wostream & text::text_to_stream(std::wostream & _Wostream) const
{
    _Wostream << xml::utils::replace_text_to_xml( text_ );
    return _Wostream;
}

void text::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

void text::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_process_note() != oox::docx_conversion_context::noNote && 
		Context.get_delete_text_state()) 
			return; //в ms нет рецензирования notes

	bool add_del_run = false;
	if (Context.get_drawing_state_content() && Context.get_delete_text_state())
	{	//0503IG-AddingFormattingText.odt - удаленый текст в удаленом объекте

		oox::text_tracked_context::_state  &state = Context.get_text_tracked_context().get_tracked_change(L"");
		if (state.type == 2)
		{
			add_del_run = true;
			Context.output_stream() << L"<w:del>";
		}
	}  
	
	Context.add_element_to_run();
  
	std::wstring textNode = L"w:t";

	if (Context.get_delete_text_state()) textNode = L"w:delText";

	Context.output_stream() << L"<" << textNode;
	if (preserve_ && !Context.get_delete_text_state()) 
		Context.output_stream() << L" xml:space=\"preserve\"";
	Context.output_stream() << L">";

	Context.output_stream() << xml::utils::replace_text_to_xml( text_ );
    Context.output_stream() << L"</" << textNode << L">";

	if (add_del_run)
	{
		Context.finish_run();
		Context.output_stream() << L"</w:del>";
	}

}

void text::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.get_text_context().add_text(text_);
}
void text::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().add_text(text_);
}
office_element_ptr text::create(const std::wstring & Text)
{
    return boost::make_shared<text>(Text);
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * s::ns = L"text";
const wchar_t * s::name = L"s";

std::wostream & s::text_to_stream(std::wostream & _Wostream) const
{
    if (text_c_)
    {
        _Wostream << std::wstring(*text_c_, L' ');
    }
    else
        _Wostream << std::wstring(1, L' ');

	if (content_)
		_Wostream << *content_;

    return _Wostream;
}

void s::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:c", text_c_);
}
void s::add_text(const std::wstring & Text)
{
    content_ = Text;
}
void s::docx_convert(oox::docx_conversion_context & Context)
{
    Context.add_element_to_run();

	std::wostream & strm = Context.output_stream();

	std::wstring textNode = L"w:t";
	if (Context.get_delete_text_state()) textNode = L"w:delText";

	strm << L"<" << textNode;
	if (!Context.get_delete_text_state())	
		strm << L" xml:space=\"preserve\"";
	strm << L">";
		text_to_stream(strm);
    strm << L"</" << textNode << L">"; 
}

void s::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
void s::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * tab::ns = L"text";
const wchar_t * tab::name = L"tab";

std::wostream & tab::text_to_stream(std::wostream & _Wostream) const
{
    _Wostream << std::wstring(L"\t");
    return _Wostream;
}

void tab::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:tab-ref", text_tab_ref_);
}

void tab::docx_convert(oox::docx_conversion_context & Context)
{
    Context.add_element_to_run();
  
	std::wostream & _Wostream = Context.output_stream();
    _Wostream << L"<w:tab/>";
}

void tab::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    Context.get_text_context().add_text(L"\t");
}
void tab::pptx_convert(oox::pptx_conversion_context & Context) 
{
    Context.get_text_context().add_text(L"\t");
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * line_break::ns = L"text";
const wchar_t * line_break::name = L"line-break";

std::wostream & line_break::text_to_stream(std::wostream & _Wostream) const
{
    _Wostream << std::wstring(L"\r\n");
    return _Wostream;
}
void line_break::docx_convert(oox::docx_conversion_context & Context)
{
    Context.add_element_to_run();
    Context.output_stream() << L"<w:br/>";
}
void line_break::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.get_text_context().add_text(L"\n");
}
void line_break::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().add_text(L"\n");
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * bookmark::ns = L"text";
const wchar_t * bookmark::name = L"bookmark";

std::wostream & bookmark::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void bookmark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * bookmark_start::ns = L"text";
const wchar_t * bookmark_start::name = L"bookmark-start";

std::wostream & bookmark_start::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}
void bookmark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", name_, std::wstring(L""));
}
void bookmark_start::docx_convert(oox::docx_conversion_context & Context)
{
	Context.start_bookmark(name_);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * bookmark_end::ns = L"text";
const wchar_t * bookmark_end::name = L"bookmark-end";

std::wostream & bookmark_end::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}
void bookmark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", name_, std::wstring(L""));
}
void bookmark_end::docx_convert(oox::docx_conversion_context & Context)
{
	Context.end_bookmark(name_);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * bookmark_ref::ns = L"text";
const wchar_t * bookmark_ref::name = L"bookmark-ref";

std::wostream & bookmark_ref::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void bookmark_ref::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:ref-name", text_ref_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"text:reference-format", text_reference_format_);
}
void bookmark_ref::add_text(const std::wstring & Text)
{
    content_ = Text;
}
// text:reference-mark
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * reference_mark::ns = L"text";
const wchar_t * reference_mark::name = L"reference-mark";

std::wostream & reference_mark::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void reference_mark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

// text:reference-mark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * reference_mark_start::ns = L"text";
const wchar_t * reference_mark_start::name = L"reference-mark-start";

std::wostream & reference_mark_start::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void reference_mark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

// text:reference-mark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * reference_mark_end::ns = L"text";
const wchar_t * reference_mark_end::name = L"reference-mark-end";

std::wostream & reference_mark_end::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void reference_mark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * span::ns = L"text";
const wchar_t * span::name = L"span";

std::wostream & span::text_to_stream(std::wostream & _Wostream) const
{
    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void span::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name", text_style_name_, std::wstring(L""));
    
    const std::wstring classNames = Attributes->get_val< std::wstring >(L"text:class-names").get_value_or(L"");
    std::vector< std::wstring > classNamesArray;

    if (classNames.size())
    {
        boost::algorithm::split(classNamesArray, classNames, boost::algorithm::is_any_of(L" "));

        for (size_t i = 0; i < classNamesArray.size(); i++)
        {
            text_class_names_.push_back( classNamesArray[i] );        
        }
    }
}

void span::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void span::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text);
    content_.push_back( elm );
}

void span::docx_convert(oox::docx_conversion_context & Context)
{
	bool addNewRun = false;
    bool pushed = false;

    std::wostream & _Wostream = Context.output_stream();

    if (!text_style_name_.empty()/* && !drawing*/)
    {
        if (style_instance * styleInst 
            = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_, style_family::Text,Context.process_headers_footers_)
            )
        {
            if (styleInst->is_automatic())
            {
                if (const style_content * styleContent = styleInst->content())
                {
                    Context.push_text_properties(styleContent->get_style_text_properties());
                    pushed = true;
                    Context.get_styles_context().start_process_style(styleInst);
					Context.add_new_run();
                    Context.get_styles_context().end_process_style();
                    addNewRun = true;
                }                            
            }
            else
            {
                const std::wstring id = Context.styles_map_.get( styleInst->name(), styleInst->type() );
                Context.add_new_run(id);
                addNewRun = true;
            }                        
        }
                                     
    }


    if (!addNewRun)Context.add_new_run();

    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }

	Context.finish_run();
	
	if (pushed)
        Context.pop_text_properties();
}

void span::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_span(text_style_name_);
    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }
    Context.end_span();
}
void span::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (style_instance * styleInst = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_, style_family::Text,false))
		Context.get_text_context().get_styles_context().start_process_style(styleInst);
   
	Context.get_text_context().start_span(text_style_name_);
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
    Context.get_text_context().end_span();
	Context.get_text_context().get_styles_context().end_process_style();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * a::ns = L"text";
const wchar_t * a::name = L"a";

std::wostream & a::text_to_stream(std::wostream & _Wostream) const
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void a::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	xlink_attlist_.add_attributes(Attributes);

    CP_APPLY_ATTR(L"office:name",				office_name_,			std::wstring(L""));
    CP_APPLY_ATTR(L"office:target-frame-name",	office_target_frame_name_);
    CP_APPLY_ATTR(L"text:style-name",			text_style_name_,		std::wstring(L""));
    CP_APPLY_ATTR(L"text:visited-style-name",	text_visited_style_name_,std::wstring(L""));
}

void a::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void a::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}

void a::docx_convert(oox::docx_conversion_context & Context)
{
    bool pushed_style = false;
    
	bool addNewRun = false;
    Context.finish_run();
    
    std::wostream & _Wostream = Context.output_stream();

	std::wstring ref = xlink_attlist_.href_.get_value_or(L"");
	
	if (Context.is_table_content())
	{
		_Wostream << L"<w:hyperlink w:anchor=\"" << ref.substr(1) << L"\" w:history=\"1\">"; //без #
	}
	else
	{
		std::wstring rId;
		rId = Context.add_hyperlink(ref, false);

		_Wostream << L"<w:hyperlink r:id=\"" << rId << L"\">";
	}

    style_instance * styleInst = NULL;
    
    if (!text_style_name_.empty())
        styleInst = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_, style_family::Text, Context.process_headers_footers_);
    else if (false == Context.is_table_content())
        styleInst = Context.root()->odf_context().styleContainer().hyperlink_style();
        
    if (styleInst)
    {
        if (styleInst->is_automatic())
        {
            if (const style_content * styleContent = styleInst->content())
            {
                Context.push_text_properties(styleContent->get_style_text_properties());
                pushed_style = true;
                Context.get_styles_context().start_process_style(styleInst);
				Context.add_new_run();
                Context.get_styles_context().end_process_style();
                addNewRun = true;
            }                            
        }
        else
        {
			style_text_properties_ptr tempStyleTextProp;
			
			const std::wstring id = Context.styles_map_.get( styleInst->name(), styleInst->type() );
            tempStyleTextProp = style_text_properties_ptr( new style_text_properties(id) );
            Context.push_text_properties( tempStyleTextProp.get() );
            pushed_style = true;

            Context.add_new_run();
            addNewRun = true;
        }                        
    }   

    if (!addNewRun)
        Context.add_new_run();

    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }

	Context.finish_run();
	
	if (pushed_style)
        Context.pop_text_properties();
    _Wostream << L"</w:hyperlink>";
}

void a::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_hyperlink(text_style_name_);
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }
    Context.end_hyperlink(xlink_attlist_.href_.get_value_or(L""));
}
void a::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_text_context().start_hyperlink();
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
	
	std::wstring hId = Context.get_slide_context().add_hyperlink(xlink_attlist_.href_.get_value_or(L""));
	Context.get_text_context().end_hyperlink(hId);

}

//------------------------------------------------------------------------------------------------------------
const wchar_t * note::ns = L"text";
const wchar_t * note::name = L"note";

note::note()
{}

std::wostream & note::text_to_stream(std::wostream & _Wostream) const
{
    if (text_note_citation_)
        text_note_citation_->text_to_stream(_Wostream);

    if (text_note_body_)
        text_note_body_->text_to_stream(_Wostream);

    return _Wostream;
}

void note::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:id", text_id_, std::wstring(L""));
    CP_APPLY_ATTR(L"text:note-class", text_note_class_, noteclass(noteclass::Footnote));
}

void note::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"note-citation")
    {
        CP_CREATE_ELEMENT(text_note_citation_);
    }
    else if CP_CHECK_NAME(L"text", L"note-body")
    {
        CP_CREATE_ELEMENT(text_note_body_);        
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

void note::add_text(const std::wstring & Text)
{
}
void note::pptx_convert(oox::pptx_conversion_context & Context)
{
	//см presentation:notes
}
void note::docx_convert(oox::docx_conversion_context & Context)
{
    bool addNewRun = false;
    bool pushed = false;

    std::wostream & _Wostream = Context.output_stream();
    if (const text_notes_configuration * conf = Context.root()->odf_context().noteConfiguration().getConfiguration(text_note_class_.get_type()))
    {
        const std::wstring styleName = conf->text_citation_body_style_name_.get_value_or(L"");
        if (!styleName.empty())
        {
            if (style_instance * styleInst 
                = Context.root()->odf_context().styleContainer().style_by_name(styleName, style_family::Text,Context.process_headers_footers_)
                )
            {
                const std::wstring id = Context.styles_map_.get( styleInst->name(), styleInst->type() );
				Context.add_new_run(id);
                addNewRun = true;
            }
                                         
        }
    }

    if (!addNewRun)
        Context.add_new_run();

	Context.get_notes_context().set_current_note(text_note_class_.get_type(), dynamic_cast<const note_citation *>(text_note_citation_.get()));

    if (text_note_class_.get_type() == noteclass::Footnote)
    {
        _Wostream << "<w:footnoteReference w:customMarkFollows=\"1\" w:id=\"" << Context.get_notes_context().next_id() << "\" />";
    }
    else 
    {
        _Wostream << "<w:endnoteReference w:customMarkFollows=\"1\" w:id=\"" << Context.get_notes_context().next_id() << "\" />";
    }

    if (text_note_citation_)
        text_note_citation_->docx_convert(Context);

    Context.set_process_note( (text_note_class_.get_type() == noteclass::Footnote) ? oox::docx_conversion_context::footNote : oox::docx_conversion_context::endNote );
    if (text_note_body_)
        text_note_body_->docx_convert(Context);
    Context.set_process_note(oox::docx_conversion_context::noNote);

	Context.finish_run();
    
	if (pushed)
        Context.pop_text_properties();    

}

//------------------------------------------------------------------------------------------------------------
const wchar_t * ruby::ns = L"text";
const wchar_t * ruby::name = L"ruby";

std::wostream & ruby::text_to_stream(std::wostream & _Wostream) const
{
    if (text_ruby_base_)
        text_ruby_base_->text_to_stream(_Wostream);

    if (text_ruby_text_)
        text_ruby_text_->text_to_stream(_Wostream);

    return _Wostream;
}

void ruby::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_style_name_ = Attributes->get_val< std::wstring >(L"text:style-name").get_value_or(L"") ;
}

void ruby::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"ruby-base")
    {
        CP_CREATE_ELEMENT(text_ruby_base_);
    }
    else if CP_CHECK_NAME(L"text", L"ruby-text")
    {
        CP_CREATE_ELEMENT(text_ruby_text_);        
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

void ruby::add_text(const std::wstring & Text)
{
}


// text:title
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * title::ns = L"text";
const wchar_t * title::name = L"title";

std::wostream & title::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void title::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void title::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void title::docx_convert(oox::docx_conversion_context & Context)
{
 	docx_serialize_field(L"TITLE", text_, Context);
}

void title::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wstringstream val;
    text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
void title::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wstringstream val;
    text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}

// text:chapter
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * subject::ns = L"text";
const wchar_t * subject::name = L"subject";

std::wostream & subject::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void subject::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}

void subject::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void subject::docx_convert(oox::docx_conversion_context & Context)
{
 	docx_serialize_field(L"SUBJECT", text_, Context);
}

void subject::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
void subject::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * chapter::ns = L"text";
const wchar_t * chapter::name = L"chapter";

std::wostream & chapter::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void chapter::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void chapter::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void chapter::docx_convert(oox::docx_conversion_context & Context)
{
 	docx_serialize_field(L"BIBLIOGRAPHY", text_, Context);
}

void chapter::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
void chapter::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_placeholder::ns = L"text";
const wchar_t * text_placeholder::name = L"placeholder";

std::wostream & text_placeholder::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void text_placeholder::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void text_placeholder::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}

void text_placeholder::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Click placeholder and overwrite", text_, Context);
}

void text_placeholder::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (text_)
    {
        text_->pptx_convert(Context);
    }
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_page_number::ns = L"text";
const wchar_t * text_page_number::name = L"page-number";

std::wostream & text_page_number::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void text_page_number::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void text_page_number::add_text(const std::wstring & Text)
{
	text_ = text::create(Text) ;

	text *t = dynamic_cast<text*>(text_.get());
	if (t) t->preserve_ = false;
}

void text_page_number::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"PAGE", text_, Context);
}
void text_page_number::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_text_context().start_field(oox::page_number, L"");
	
    if (text_)
    {
        text_->pptx_convert(Context);
    }

    Context.get_text_context().end_field();
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_page_count::ns = L"text";
const wchar_t * text_page_count::name = L"page-count";

std::wostream & text_page_count::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void text_page_count::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    //CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void text_page_count::add_text(const std::wstring & Text)
{
	text_ = text::create(Text) ;

	text *t = dynamic_cast<text*>(text_.get());
	if (t) t->preserve_ = false;
}

void text_page_count::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"NUMPAGES", text_, Context);
}
void text_page_count::pptx_convert(oox::pptx_conversion_context & Context)
{
	//поскольку такого поля в ms нет - конвертим как обычный текст
	if (text_)
    {
        text_->pptx_convert(Context);
    }
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_date::ns = L"text";
const wchar_t * text_date::name = L"date";

std::wostream & text_date::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void text_date::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name", style_data_style_name_);
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
    CP_APPLY_ATTR(L"text:date-value", text_date_value_);
}

void text_date::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}

void text_date::docx_convert(oox::docx_conversion_context & Context)
{
	bool bLock = text_fixed_ ? text_fixed_->get() : false;

	docx_serialize_field(L"DATE", text_, Context, bLock);
}

void text_date::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_field(oox::date,style_data_style_name_.get_value_or(L""));
    if (text_)
    {
        text_->pptx_convert(Context);
    }
    Context.get_text_context().end_field();
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_modification_date::ns = L"text";
const wchar_t * text_modification_date::name = L"modification-date";

void text_modification_date::docx_convert(oox::docx_conversion_context & Context)
{
	bool asText = true;//text_fixed_.get_value_or(false);

	if (asText)
	{
		docx_serialize_run(text_, Context);
	}
	else
	{
		docx_serialize_field(L"SAVEDATE \\@ \"dd.MM.yy\"", text_, Context);
	}
}
void text_modification_date::pptx_convert(oox::pptx_conversion_context & Context)
{
	text_date::pptx_convert(Context);
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_time::ns = L"text";
const wchar_t * text_time::name = L"time";

std::wostream & text_time::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void text_time::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name", style_data_style_name_);

    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
    CP_APPLY_ATTR(L"text:time-value", text_time_value_);
}
void text_time::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}

void text_time::docx_convert(oox::docx_conversion_context & Context)
{
	bool bLock = text_fixed_ ? text_fixed_->get() : false;

	docx_serialize_field(L"TIME", text_, Context, bLock);
}
void text_time::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_field(oox::time, style_data_style_name_.get_value_or(L""));
	if (text_)
    {
        text_->pptx_convert(Context);
    }
    Context.get_text_context().end_field();
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_modification_time::ns = L"text";
const wchar_t * text_modification_time::name = L"modification-time";

void text_modification_time::docx_convert(oox::docx_conversion_context & Context)
{
	bool asText = true;//text_fixed_.get_value_or(false);

	if (asText)
	{
		docx_serialize_run(text_, Context);
	}
	else
	{
		docx_serialize_field(L"SAVEDATE  \\@ \"h:mm:ss am/pm\"", text_, Context);
	}
}
void text_modification_time::pptx_convert(oox::pptx_conversion_context & Context)
{
    text_time::pptx_convert(Context);
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_file_name::ns = L"text";
const wchar_t * text_file_name::name = L"file-name";

std::wostream & text_file_name::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}
void text_file_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void text_file_name::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}

void text_file_name::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"FILENAME", text_, Context);
}
void text_file_name::pptx_convert(oox::pptx_conversion_context & Context)
{
//	Context.get_text_context().start_field(oox::file_name, style_data_style_name_.get_value_or(L""));
	if (text_)
    {
        text_->pptx_convert(Context);
    }
  //  Context.get_text_context().end_field();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sequence::ns = L"text";
const wchar_t * sequence::name = L"sequence";

std::wostream & sequence::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void sequence::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name", style_num_format_);
	CP_APPLY_ATTR(L"style:data-style-name", style_num_letter_sync_);
	CP_APPLY_ATTR(L"text:formula", text_formula_);
	CP_APPLY_ATTR(L"text:ref-name", text_ref_name_);
	CP_APPLY_ATTR(L"text:name", text_name_);
}

void sequence::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void sequence::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    text_.push_back( elm );
}

void sequence::docx_convert(oox::docx_conversion_context & Context) 
{
	for (size_t i = 0; i < text_.size(); i++)
    {
        text_[i]->docx_convert(Context);
    }
}
void sequence::pptx_convert(oox::pptx_conversion_context & Context) 
{
	for (size_t i = 0; i < text_.size(); i++)
    {
        text_[i]->pptx_convert(Context);
    }
}
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * text_drop_down::ns		= L"text";
const wchar_t * text_drop_down::name	= L"drop-down";

void text_drop_down::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:name", text_name_);
}

void text_drop_down::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"text", L"label")
	{
		CP_CREATE_ELEMENT(content_);
	}
	else
	{
	}
}

void text_drop_down::add_text(const std::wstring & Text) 
{
    text_ = Text;
}
std::wostream & text_drop_down::text_to_stream(std::wostream & _Wostream) const
{
    _Wostream << xml::utils::replace_text_to_xml( text_ );
    return _Wostream;
}

void text_drop_down::docx_convert(oox::docx_conversion_context & Context) 
{
    std::wostream & strm = Context.output_stream();
	Context.finish_run();
	
	strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"><w:ffData><w:name w:val=\"" << text_name_.get_value_or(L"") << L"\"/><w:enabled/>";

	strm << L"<w:ddList><w:result w:val=\"0\"/>";
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
	strm << L"</w:ddList></w:ffData>";

	strm << L"</w:fldChar></w:r>";
	strm << L"<w:r><w:instrText>FORMDROPDOWN</w:instrText></w:r>";
	strm << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	strm << L"<w:r><w:t>" << text_ << L"</w:t></w:r>";
    strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
}
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * text_label::ns		= L"text";
const wchar_t * text_label::name	= L"label";

void text_label::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:value", text_value_);
}
void text_label::docx_convert(oox::docx_conversion_context & Context) 
{
    std::wostream & strm = Context.output_stream();
	
	strm << L"<w:listEntry w:val=\"" << text_value_.get_value_or(L"") << L"\"/>";
}
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * sheet_name::ns = L"text";
const wchar_t * sheet_name::name = L"sheet-name";

void sheet_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sheet_name::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sheet_name::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"sheet name", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * author_name::ns = L"text";
const wchar_t * author_name::name = L"author-name";

void author_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void author_name::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void author_name::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"AUTHOR", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * author_initials::ns = L"text";
const wchar_t * author_initials::name = L"author-initials";

void author_initials::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void author_initials::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void author_initials::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"USERINITIALS", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_city::ns = L"text";
const wchar_t * sender_city::name = L"sender-city";

void sender_city::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void sender_city::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_city::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Sender city", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_email::ns = L"text";
const wchar_t * sender_email::name = L"sender-email";

void sender_email::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_email::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_email::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Sender email", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_lastname::ns = L"text";
const wchar_t * sender_lastname::name = L"sender-lastname";

void sender_lastname::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_lastname::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_lastname::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Sender last name", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_firstname::ns = L"text";
const wchar_t * sender_firstname::name = L"sender-firstname";

void sender_firstname::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_firstname::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_firstname::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Sender first name", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_company::ns = L"text";
const wchar_t * sender_company::name = L"sender-company";

void sender_company::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_company::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_company::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Sender company", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_postal_code::ns = L"text";
const wchar_t * sender_postal_code::name = L"sender-postal-code";

std::wostream & sender_postal_code::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void sender_postal_code::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void sender_postal_code::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_postal_code::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Sender postal code", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_street::ns = L"text";
const wchar_t * sender_street::name = L"sender-street";

void sender_street::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_street::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_street::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Sender street", text_, Context);
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_state_or_province::ns = L"text";
const wchar_t * sender_state_or_province::name = L"sender-state-or-province";

void sender_state_or_province::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_state_or_province::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_state_or_province::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt(L"Sender state or province", text_, Context);
}
//---------------------------------------------------------------------------------------------------
const wchar_t * text_user_field_get::ns		= L"text";
const wchar_t * text_user_field_get::name	= L"user-field-get";

void text_user_field_get::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name",	style_data_style_name_);
	CP_APPLY_ATTR(L"text:display",			text_display_);
	CP_APPLY_ATTR(L"text:name",				text_name_);
}
void text_user_field_get::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void text_user_field_get::docx_convert(oox::docx_conversion_context & Context)
{
	if (!text_name_) return;

	if (!text_)
	{
		std::wstring value = Context.get_user_field(*text_name_);
		text_ = text::create(value) ;
	}
	
	docx_serialize_run(text_, Context);
}
}//namespace text

//------------------------------------------------------------------------------------------------------------
const wchar_t * presentation_footer::ns = L"presentation";
const wchar_t * presentation_footer::name = L"footer";

void presentation_footer::pptx_convert(oox::pptx_conversion_context & Context)
{
	//Context.get_slide_context.set_footer_enabled();
}

const wchar_t * presentation_date_time::ns = L"presentation";
const wchar_t * presentation_date_time::name = L"date-time";

void presentation_date_time::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_field(oox::date, L"");
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * field_fieldmark_start::ns = L"field";
const wchar_t * field_fieldmark_start::name = L"fieldmark-start";

void field_fieldmark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_);
    CP_APPLY_ATTR(L"field:type", field_type_);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * field_fieldmark_end::ns = L"field";
const wchar_t * field_fieldmark_end::name = L"fieldmark-end";
}
}
