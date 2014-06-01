#include "precompiled_cpodf.h"
#include "paragraph_elements.h"

#include <ostream>
#include <string>

#include <boost/foreach.hpp>
#include <boost_string.h>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>
#include <cpdoccore/common/readstring.h>

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPWeakPtr.h>

#include "targetframename.h"
#include "styles.h"
//#include "note.h"
//#include "ruby.h"

#include "office_elements_create.h"

#include "odf_conversion_context.h"

#include "style_text_properties.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// simple text
//////////////////////////////////////////////////////////////////////////////////////////////////

const wchar_t * text_text::ns = L"";
const wchar_t * text_text::name = L"";

void text_text::serialize(std::wostream & _Wostream)
{
    _Wostream << xml::utils::replace_text_to_xml( text_ );
}

void text_text::add_text(const std::wstring & Text) 
{
    text_ = Text;
};

office_element_ptr text_text::create(const std::wstring & Text)
{
    return boost::make_shared<text_text>(Text);
}

// text:s
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_s::ns = L"text";
const wchar_t * text_s::name = L"s";

void text_s::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 		
			CP_XML_ATTR_OPT(L"text:c", text_c_);
		}
	}

}


// text:tab
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_tab::ns = L"text";
const wchar_t * text_tab::name = L"tab";

void text_tab::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT(L"text:tab-ref", text_tab_ref_);
		}
	}
}

// text:line-break
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_line_break::ns = L"text";
const wchar_t * text_line_break::name = L"line-break";

void text_line_break::serialize(std::wostream & _Wostream) 
{
}


// text:bookmark
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_bookmark::ns = L"text";
const wchar_t * text_bookmark::name = L"bookmark";

void text_bookmark::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:name", text_name_);
		}
	}
}

// text:bookmark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_bookmark_start::ns = L"text";
const wchar_t * text_bookmark_start::name = L"bookmark-start";

void text_bookmark_start::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:name", text_name_);
		}
	}
}

// text:bookmark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_bookmark_end::ns = L"text";
const wchar_t * text_bookmark_end::name = L"bookmark-end";

void text_bookmark_end::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:name", text_name_);
		}
	}
}

// text:reference-mark
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_reference_mark::ns = L"text";
const wchar_t * text_reference_mark::name = L"reference-mark";

void text_reference_mark::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:name", text_name_);
		}
	}
}


// text:reference-mark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_reference_mark_start::ns = L"text";
const wchar_t * text_reference_mark_start::name = L"reference-mark-start";

void text_reference_mark_start::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:name", text_name_);
		}
	}
}


// text:reference-mark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_reference_mark_end::ns = L"text";
const wchar_t * text_reference_mark_end::name = L"reference-mark-end";

void text_reference_mark_end::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:name", text_name_);
		}
	}
}


// text:span
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_span::ns = L"text";
const wchar_t * text_span::name = L"span";

void text_span::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT(L"text:style-name", text_style_name_);
			
			BOOST_FOREACH(const office_element_ptr & parElement, paragraph_content_)
			{
				parElement->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_span::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(paragraph_content_);
}
void text_span::add_child_element( office_element_ptr & child_element)
{
	paragraph_content_.push_back(child_element);
}
void text_span::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text);
    paragraph_content_.push_back( elm );
}
// text:a
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_a::ns = L"text";
const wchar_t * text_a::name = L"a";

void text_a::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			common_xlink_attlist_.serialize(CP_GET_XML_NODE());
		
		    CP_XML_ATTR_OPT(L"office:name", office_name_);
		    CP_XML_ATTR_OPT(L"office:target-frame-name", office_target_frame_name_);
		    CP_XML_ATTR_OPT(L"text:style-name", text_style_name_);
		    CP_XML_ATTR_OPT(L"text:visited-style-name", text_visited_style_name_);   
			
			BOOST_FOREACH(const office_element_ptr & element, paragraph_content_)
			{
				element->serialize(CP_XML_STREAM());
			}
		}
	}
    
}

void text_a::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(paragraph_content_);
}

void text_a::add_child_element( office_element_ptr & child_element)
{
	paragraph_content_.push_back(child_element);
}

void text_a::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    paragraph_content_.push_back( elm );
}

// text:note
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_note::ns = L"text";
const wchar_t * text_note::name = L"note";

text_note::text_note()
{}

void text_note::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
		    CP_XML_ATTR(L"text:id", text_id_);
		    CP_XML_ATTR(L"text:note-class", text_note_class_);
			
			if (text_note_citation_)
				text_note_citation_->serialize(CP_XML_STREAM());

			if (text_note_body_)
				text_note_body_->serialize(CP_XML_STREAM());
		}
	}    
}

void text_note::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
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
void text_note::add_child_element( office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTextNoteCitation)
		text_note_citation_ = child_element;
	else if (type == typeTextNoteBody)
		text_note_body_ = child_element;
	else
	{
	}
}

void text_note::add_text(const std::wstring & Text)
{
}

// text:ruby
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_ruby::ns = L"text";
const wchar_t * text_ruby::name = L"ruby";

void text_ruby::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
		    CP_XML_ATTR(L"text:style-name", text_style_name_.style_name());

			if (text_ruby_base_)
				text_ruby_base_->serialize(CP_XML_STREAM());

			if (text_ruby_text_)
				text_ruby_text_->serialize(CP_XML_STREAM());
		}
	}
}

void text_ruby::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
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
void text_ruby::add_child_element( office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTextRubyBase)
		text_ruby_base_ = child_element;
	else if (type == typeTextRubyText)
		text_ruby_text_ = child_element;
}

void text_ruby::add_text(const std::wstring & Text)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////

//void common_field_fixed_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
//{
//    CP_XML_ATTR_OPT(L"text:fixed", text_fixed_);
//}

// text:title
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_title::ns = L"text";
const wchar_t * text_title::name = L"title";

void text_title::serialize(std::wostream & _Wostream) 
{

	//common_field_fixed_attlist_.add_attributes(Attributes);
    
}

void text_title::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
}

void text_title::add_child_element( office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void text_title::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    content_.push_back( elm );
}

// text:placeholder
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_placeholder::ns = L"text";
const wchar_t * text_placeholder::name = L"placeholder";

void text_placeholder::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}    
}

void text_placeholder::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
}

void text_placeholder::add_child_element( office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void text_placeholder::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    content_.push_back( elm );
}


// text:page-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_page_number::ns = L"text";
const wchar_t * text_page_number::name = L"page-number";

void text_page_number::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			//    //text:select-page
			//	//common-field-num-format-attlist"/> ----	//style:num-format="1"/>
			//	//common-field-fixed-attlist"/>
			//	//text:page-adjust="1"    
			BOOST_FOREACH(const office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_page_number::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_page_number::add_child_element( office_element_ptr & child_element)
{
	text_.push_back(child_element);
}

void text_page_number::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}

// text:page-count
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_page_count::ns = L"text";
const wchar_t * text_page_count::name = L"page-count";

void text_page_count::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			BOOST_FOREACH(const office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
void text_page_count::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_page_count::add_child_element( office_element_ptr & child_element)
{
	text_.push_back(child_element);
}

void text_page_count::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}


// text:date
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_date::ns = L"text";
const wchar_t * text_date::name = L"date";

void text_date::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			//	CP_XML_ATTR_OPT(L"style:data-style-name", style_data_style_name_);
			//    CP_XML_ATTR_OPT(L"text:fixed", text_fixed_);
			//    CP_XML_ATTR_OPT(L"text:date-value", text_date_value_);   
			BOOST_FOREACH(const office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
void text_date::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}
void text_date::add_child_element( office_element_ptr & child_element)
{
	text_.push_back(child_element);
}

void text_date::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}


// text:time
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_time::ns = L"text";
const wchar_t * text_time::name = L"time";

void text_time::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT(L"style:data-style-name", style_data_style_name_);

			CP_XML_ATTR_OPT(L"text:fixed", text_fixed_);
			CP_XML_ATTR_OPT(L"text:time-value", text_time_value_);
			
			BOOST_FOREACH(const office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}


void text_time::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_time::add_child_element( office_element_ptr & child_element)
{
	text_.push_back(child_element);
}

void text_time::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}


// text:time
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_file_name::ns = L"text";
const wchar_t * text_file_name::name = L"file-name";

void text_file_name::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			BOOST_FOREACH(const office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}


void text_file_name::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_file_name::add_child_element( office_element_ptr & child_element)
{
	text_.push_back(child_element);
}
void text_file_name::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}


// text:sequence
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_sequence::ns = L"text";
const wchar_t * text_sequence::name = L"sequence";

void text_sequence::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			BOOST_FOREACH(const office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_sequence::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}
void text_sequence::add_child_element( office_element_ptr & child_element)
{
	text_.push_back(child_element);
}
void text_sequence::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}


// text:sequesheet-namence
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_sheet_name::ns = L"text";
const wchar_t * text_sheet_name::name = L"sheet-name";

void text_sheet_name::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void text_sheet_name::add_child_element( office_element_ptr & child_element)
{
	text_.push_back(child_element);
}
void text_sheet_name::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}
void text_sheet_name::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			BOOST_FOREACH(const office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// presentation:footer
const wchar_t * presentation_footer::ns = L"presentation";
const wchar_t * presentation_footer::name = L"footer";

void presentation_footer::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}

const wchar_t * presentation_date_time::ns = L"presentation";
const wchar_t * presentation_date_time::name = L"date-time";

void presentation_date_time::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}

}
}
