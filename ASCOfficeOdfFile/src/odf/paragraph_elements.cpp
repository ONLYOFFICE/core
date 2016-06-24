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

#include "paragraph_elements.h"

#include <ostream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
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



// simple text
//////////////////////////////////////////////////////////////////////////////////////////////////

const wchar_t * text::ns = L"";
const wchar_t * text::name = L"";

::std::wostream & text::text_to_stream(::std::wostream & _Wostream) const
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
    Context.add_element_to_run();
	std::wostream & _Wostream = Context.output_stream();
  
	if (preserve_)
		_Wostream << L"<w:t xml:space=\"preserve\">";
	else
		_Wostream << L"<w:t>";

	_Wostream << xml::utils::replace_text_to_xml( text_ );
    _Wostream << L"</w:t>";
}

void text::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    //Context.get_text_context().add_text(xml::utils::replace_text_to_xml(text_));
    Context.get_text_context().add_text(text_);
}
void text::pptx_convert(oox::pptx_conversion_context & Context)
{
    //Context.get_text_context().add_text(xml::utils::replace_text_to_xml(text_));
    Context.get_text_context().add_text(text_);
}


office_element_ptr text::create(const std::wstring & Text)
{
    return boost::make_shared<text>(Text);
}

// text:s
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * s::ns = L"text";
const wchar_t * s::name = L"s";

::std::wostream & s::text_to_stream(::std::wostream & _Wostream) const
{
    if (text_c_)
    {
        _Wostream << std::wstring(*text_c_, L' ');
    }
    else
        _Wostream << std::wstring(1, L' ');

    return _Wostream;
}

void s::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:c", text_c_);
}

void s::docx_convert(oox::docx_conversion_context & Context)
{
    Context.add_element_to_run();
	std::wostream & _Wostream = Context.output_stream();
    _Wostream << L"<w:t xml:space=\"preserve\">";
		text_to_stream(_Wostream);
    _Wostream << L"</w:t>"; 
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

// text:tab
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * tab::ns = L"text";
const wchar_t * tab::name = L"tab";

::std::wostream & tab::text_to_stream(::std::wostream & _Wostream) const
{
    _Wostream << ::std::wstring(L"\t");
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
    _Wostream << L"<w:tab />";
}

void tab::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    Context.get_text_context().add_text(L"\t");
}
void tab::pptx_convert(oox::pptx_conversion_context & Context) 
{
    Context.get_text_context().add_text(L"\t");
}
// text:line-break
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * line_break::ns = L"text";
const wchar_t * line_break::name = L"line-break";

::std::wostream & line_break::text_to_stream(::std::wostream & _Wostream) const
{
    _Wostream << ::std::wstring(L"\r\n");
    return _Wostream;
}

void line_break::docx_convert(oox::docx_conversion_context & Context)
{
    Context.add_element_to_run();
    Context.output_stream() << L"<w:br />";
}

void line_break::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.get_text_context().add_text(L"\n");
}
void line_break::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().add_text(L"\n");
}

// text:bookmark
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * bookmark::ns = L"text";
const wchar_t * bookmark::name = L"bookmark";

::std::wostream & bookmark::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void bookmark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

// text:bookmark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * bookmark_start::ns = L"text";
const wchar_t * bookmark_start::name = L"bookmark-start";

::std::wostream & bookmark_start::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void bookmark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

// text:bookmark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * bookmark_end::ns = L"text";
const wchar_t * bookmark_end::name = L"bookmark-end";

::std::wostream & bookmark_end::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void bookmark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

// text:reference-mark
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * reference_mark::ns = L"text";
const wchar_t * reference_mark::name = L"reference-mark";

::std::wostream & reference_mark::text_to_stream(::std::wostream & _Wostream) const
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

::std::wostream & reference_mark_start::text_to_stream(::std::wostream & _Wostream) const
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

::std::wostream & reference_mark_end::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void reference_mark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

// text:span
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * span::ns = L"text";
const wchar_t * span::name = L"span";

::std::wostream & span::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & parElement, paragraph_content_)
    {
        parElement->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void span::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name", text_style_name_, style_ref(L""));
    
    const ::std::wstring classNames = Attributes->get_val< ::std::wstring >(L"text:class-names").get_value_or(L"");
    ::std::vector< ::std::wstring > classNamesArray;

    if (classNames.size())
    {
        boost::algorithm::split(classNamesArray, classNames, boost::algorithm::is_any_of(L" "));

        BOOST_FOREACH(const ::std::wstring & name, classNamesArray)
        {
            text_class_names_.push_back( style_ref(name) );        
        }
    }
}

void span::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(paragraph_content_);
}

void span::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text);
    paragraph_content_.push_back( elm );
}

void span::docx_convert(oox::docx_conversion_context & Context)
{
	bool addNewRun = false;
    bool pushed = false;

    std::wostream & _Wostream = Context.output_stream();

    if (!text_style_name_.style_name().empty()/* && !drawing*/)
    {
        if (style_instance * styleInst 
            = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_.style_name(), style_family::Text,Context.process_headers_footers_)
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

    BOOST_FOREACH(const office_element_ptr & parElement, paragraph_content_)
    {
        parElement->docx_convert(Context);
    }

	Context.finish_run();
	
	if (pushed)
        Context.pop_text_properties();
}

void span::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_span(text_style_name_.style_name());
    BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
    {
        elm->xlsx_convert(Context);
    }
    Context.end_span();
}
void span::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (style_instance * styleInst = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_.style_name(), style_family::Text,false))
		Context.get_text_context().get_styles_context().start_process_style(styleInst);
   
	Context.get_text_context().start_span(text_style_name_.style_name());
    BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
    {
        elm->pptx_convert(Context);
    }
    Context.get_text_context().end_span();
	Context.get_text_context().get_styles_context().end_process_style();
}
// text:a
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * a::ns = L"text";
const wchar_t * a::name = L"a";

::std::wostream & a::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & element, paragraph_content_)
    {
        element->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void a::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_xlink_attlist_.add_attributes(Attributes);

    CP_APPLY_ATTR(L"office:name", office_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"office:target-frame-name", office_target_frame_name_);
    CP_APPLY_ATTR(L"text:style-name", text_style_name_, style_ref(L""));
    CP_APPLY_ATTR(L"text:visited-style-name", text_visited_style_name_, style_ref(L""));
}

void a::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(paragraph_content_);
}

void a::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    paragraph_content_.push_back( elm );
}

void a::docx_convert(oox::docx_conversion_context & Context)
{
    bool pushed = false;
    bool addNewRun = false;
    Context.finish_run();
    style_text_properties_ptr tempStyleTextProp;

    
    std::wostream & _Wostream = Context.output_stream();

    std::wstring rId;
    rId = Context.add_hyperlink(common_xlink_attlist_.href_.get_value_or(L""), false);

    _Wostream << L"<w:hyperlink r:id=\"" << rId << L"\">";


    style_instance * styleInst = NULL;
    
    if (!text_style_name_.style_name().empty())
        styleInst = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_.style_name(), style_family::Text,Context.process_headers_footers_);
    else
        styleInst = Context.root()->odf_context().styleContainer().hyperlink_style();
        
    if (styleInst)
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
            tempStyleTextProp = style_text_properties_ptr( new style_text_properties(id) );
            Context.push_text_properties( tempStyleTextProp.get() );
            pushed = true;

            Context.add_new_run();
            addNewRun = true;
        }                        
    }
   

    if (!addNewRun)
        Context.add_new_run();

    BOOST_FOREACH(const office_element_ptr & parElement, paragraph_content_)
    {
        parElement->docx_convert(Context);
    }

	Context.finish_run();
	
	if (pushed)
        Context.pop_text_properties();
    _Wostream << L"</w:hyperlink>";
}

void a::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_hyperlink(text_style_name_.style_name());
    BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
    {
        elm->xlsx_convert(Context);
    }
    Context.end_hyperlink(common_xlink_attlist_.href_.get_value_or(L""));
}
void a::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_text_context().start_hyperlink();
    BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
    {
        elm->pptx_convert(Context);
    }
	
	std::wstring hId = Context.get_slide_context().add_hyperlink(common_xlink_attlist_.href_.get_value_or(L""),false);
	Context.get_text_context().end_hyperlink(hId);

}
// text:note
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * note::ns = L"text";
const wchar_t * note::name = L"note";

note::note()
{}

::std::wostream & note::text_to_stream(::std::wostream & _Wostream) const
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

void note::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
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

// text:ruby
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * ruby::ns = L"text";
const wchar_t * ruby::name = L"ruby";

::std::wostream & ruby::text_to_stream(::std::wostream & _Wostream) const
{
    if (text_ruby_base_)
        text_ruby_base_->text_to_stream(_Wostream);

    if (text_ruby_text_)
        text_ruby_text_->text_to_stream(_Wostream);

    return _Wostream;
}

void ruby::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_style_name_ = style_ref( Attributes->get_val< ::std::wstring >(L"text:style-name").get_value_or(L"") );
}

void ruby::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
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

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_field_fixed_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}

// text:title
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * title::ns = L"text";
const wchar_t * title::name = L"title";

std::wostream & title::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & parElement, content_)
    {
        parElement->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void title::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_field_fixed_attlist_.add_attributes(Attributes);
}

void title::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void title::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}
void title::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    Context.finish_run();
    strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
    strm << L"<w:r><w:instrText>TITLE</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
    Context.add_new_run();
	
	std::wostream & _Wostream = Context.output_stream();
    _Wostream << L"<w:t xml:space=\"preserve\">";
		text_to_stream(_Wostream);
    _Wostream << L"</w:t>"; 
    
	Context.finish_run();
    strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
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

std::wostream & subject::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & parElement, content_)
    {
        parElement->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void subject::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_field_fixed_attlist_.add_attributes(Attributes);
}

void subject::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void subject::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}
void subject::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    Context.finish_run();
    strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
    strm << L"<w:r><w:instrText>SUBJECT</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
    Context.add_new_run();
	
	std::wostream & _Wostream = Context.output_stream();
    _Wostream << L"<w:t xml:space=\"preserve\">";
		text_to_stream(_Wostream);
    _Wostream << L"</w:t>"; 
    
	Context.finish_run();
    strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
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
// text:chapter
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * chapter::ns = L"text";
const wchar_t * chapter::name = L"chapter";

std::wostream & chapter::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & parElement, content_)
    {
        parElement->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void chapter::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_field_fixed_attlist_.add_attributes(Attributes);
}

void chapter::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void chapter::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}
void chapter::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    Context.finish_run();
    //strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
    //strm << L"<w:r><w:instrText>BIBLIOGRAPHY</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
    Context.add_new_run();
	
	std::wostream & _Wostream = Context.output_stream();
    _Wostream << L"<w:t xml:space=\"preserve\">";
		text_to_stream(_Wostream);
    _Wostream << L"</w:t>"; 
    
	Context.finish_run();
    //strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
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

// text:placeholder
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_placeholder::ns = L"text";
const wchar_t * text_placeholder::name = L"placeholder";

::std::wostream & text_placeholder::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void text_placeholder::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    // TODO
}

void text_placeholder::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
}

void text_placeholder::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}

void text_placeholder::docx_convert(oox::docx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }
}

void text_placeholder::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->pptx_convert(Context);
    }
}

// text:page-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_page_number::ns = L"text";
const wchar_t * text_page_number::name = L"page-number";

::std::wostream & text_page_number::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void text_page_number::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    //text:select-page
	//common-field-num-format-attlist"/> ----	//style:num-format="1"/>
	//common-field-fixed-attlist"/>
	//text:page-adjust="1"

}

void text_page_number::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_page_number::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;

	text *t = dynamic_cast<text*>(elm.get());
	if (t) t->preserve_ = false;
   
	text_.push_back( elm );
}

void text_page_number::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    Context.finish_run();
    strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
    strm << L"<w:r><w:instrText>PAGE</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
    Context.add_new_run();
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->docx_convert(Context);
    }
    Context.finish_run();
    strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
}
void text_page_number::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_text_context().start_field(oox::page_number, L"");
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->pptx_convert(Context);
    }
    Context.get_text_context().end_field();
}

// text:page-count
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_page_count::ns = L"text";
const wchar_t * text_page_count::name = L"page-count";

::std::wostream & text_page_count::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void text_page_count::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    // TODO
}

void text_page_count::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_page_count::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    text_.push_back( elm );
}

void text_page_count::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    Context.finish_run();
    strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
    strm << L"<w:r><w:instrText>NUMPAGES</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
    Context.add_new_run();
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->docx_convert(Context);
    }
    Context.finish_run();
    strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
}
void text_page_count::pptx_convert(oox::pptx_conversion_context & Context)
{
	//поскольку такого поля в ms нет - конвертим как обычный текст
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->pptx_convert(Context);
    }
}

// text:date
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_date::ns = L"text";
const wchar_t * text_date::name = L"date";

::std::wostream & text_date::text_to_stream(::std::wostream & _Wostream) const
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

void text_date::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_date::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    text_.push_back( elm );
}

void text_date::docx_convert(oox::docx_conversion_context & Context)
{
	bool asText = text_fixed_.get_value_or(false);

	if (asText)
	{
		Context.add_new_run();
		BOOST_FOREACH(const office_element_ptr & elm, text_)
		{
			elm->docx_convert(Context);
		}
		Context.finish_run();
	}
	else
	{
		
		std::wostream & strm = Context.output_stream();
		Context.finish_run();
		strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
		strm << L"<w:r><w:instrText>DATE</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
		Context.add_new_run();
		
		std::wostream & _Wostream = Context.output_stream();
		_Wostream << L"<w:t xml:space=\"preserve\">";
			this->text_to_stream(_Wostream);
		_Wostream << L"</w:t>"; 
	    
		Context.finish_run();
		strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
	}
}

void text_date::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_field(oox::date,style_data_style_name_.get_value_or(L""));
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->pptx_convert(Context);
    }
    Context.get_text_context().end_field();
}

const wchar_t * text_modification_date::ns = L"text";
const wchar_t * text_modification_date::name = L"modification-date";

void text_modification_date::docx_convert(oox::docx_conversion_context & Context)
{
	bool asText = true;//text_fixed_.get_value_or(false);

	if (asText)
	{
		Context.add_new_run();
		BOOST_FOREACH(const office_element_ptr & elm, text_)
		{
			elm->docx_convert(Context);
		}
		Context.finish_run();
	}
	else
	{
		std::wostream & strm = Context.output_stream();
		Context.finish_run();
		strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
		strm << L"<w:r><w:instrText xml:space=\"preserve\">SAVEDATE \\@ \"dd.MM.yy\"</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
		Context.add_new_run();
		BOOST_FOREACH(const office_element_ptr & elm, text_)
		{
			elm->docx_convert(Context);
		}
		Context.finish_run();
		strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
	}
}
void text_modification_date::pptx_convert(oox::pptx_conversion_context & Context)
{
	text_date::pptx_convert(Context);
}

// text:time
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_time::ns = L"text";
const wchar_t * text_time::name = L"time";

::std::wostream & text_time::text_to_stream(::std::wostream & _Wostream) const
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

void text_time::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_time::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    text_.push_back( elm );
}

void text_time::docx_convert(oox::docx_conversion_context & Context)
{
	bool asText = text_fixed_.get_value_or(false);

	if (asText)
	{
		Context.add_new_run();
		BOOST_FOREACH(const office_element_ptr & elm, text_)
		{
			elm->docx_convert(Context);
		}
		Context.finish_run();
	}
	else
	{
		std::wostream & strm = Context.output_stream();
		Context.finish_run();
		strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
		strm << L"<w:r><w:instrText>TIME</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
		Context.add_new_run();
		
		std::wostream & _Wostream = Context.output_stream();
		_Wostream << L"<w:t xml:space=\"preserve\">";
			this->text_to_stream(_Wostream);
		_Wostream << L"</w:t>"; 
	    
		Context.finish_run();
		strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
	}
}
void text_time::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_field(oox::time, style_data_style_name_.get_value_or(L""));
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->pptx_convert(Context);
    }
    Context.get_text_context().end_field();
}

const wchar_t * text_modification_time::ns = L"text";
const wchar_t * text_modification_time::name = L"modification-time";

void text_modification_time::docx_convert(oox::docx_conversion_context & Context)
{
	bool asText = true;//text_fixed_.get_value_or(false);

	if (asText)
	{
		Context.add_new_run();
		BOOST_FOREACH(const office_element_ptr & elm, text_)
		{
			elm->docx_convert(Context);
		}
		Context.finish_run();
	}
	else
	{
		std::wostream & strm = Context.output_stream();
		Context.finish_run();
		strm << L"<w:r><w:fldChar w:fldCharType=\"begin\" /></w:r>";
		strm << L"<w:r><w:instrText>SAVEDATE  \\@ \"h:mm:ss am/pm\"</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\" /></w:r>";
		Context.add_new_run();
		BOOST_FOREACH(const office_element_ptr & elm, text_)
		{
			elm->docx_convert(Context);
		}
		Context.finish_run();
		strm << L"<w:r><w:fldChar w:fldCharType=\"end\" /></w:r>";
	}
}
void text_modification_time::pptx_convert(oox::pptx_conversion_context & Context)
{
    text_time::pptx_convert(Context);
}

// text:file-name
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_file_name::ns = L"text";
const wchar_t * text_file_name::name = L"file-name";

::std::wostream & text_file_name::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void text_file_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    // TODO
}

void text_file_name::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_file_name::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    text_.push_back( elm );
}

void text_file_name::docx_convert(oox::docx_conversion_context & Context)
{
    Context.add_new_run();
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->docx_convert(Context);
    }
    Context.finish_run();
}
void text_file_name::pptx_convert(oox::pptx_conversion_context & Context)
{
	//Context.get_text_context().start_field(oox::file_name, style_data_style_name_.get_value_or(L""));
	BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->pptx_convert(Context);
    }
    Context.get_text_context().end_field();
}
// text:sequence
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * sequence::ns = L"text";
const wchar_t * sequence::name = L"sequence";

::std::wostream & sequence::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_);
    return _Wostream;
}

void sequence::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void sequence::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
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
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->docx_convert(Context);
    }
}
void sequence::pptx_convert(oox::pptx_conversion_context & Context) 
{
    BOOST_FOREACH(const office_element_ptr & elm, text_)
    {
        elm->pptx_convert(Context);
    }
}

// text:sequesheet-namence
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * sheet_name::ns = L"text";
const wchar_t * sheet_name::name = L"sheet-name";

void sheet_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void sheet_name::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void sheet_name::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    text_.push_back( elm );
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// presentation:footer
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
}
}
}
