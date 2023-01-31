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

#include "paragraph_elements.h"

#include <ostream>
#include <string>

#include <boost/algorithm/string.hpp>

#include <xml/xmlchar.h>

#include <xml/attributes.h>
#include <xml/utils.h>
#include <readstring.h>

#include <CPOptional.h>
#include <CPWeakPtr.h>

#include "styles.h"
//#include "note.h"
//#include "ruby.h"

#include "office_elements_create.h"
#include "odf_conversion_context.h"
#include "style_text_properties.h"

#include "../../DataTypes/targetframename.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;

//----------------------------------------------------------------------------------
// simple text
//----------------------------------------------------------------------------------
const wchar_t * text_text::ns = L"";
const wchar_t * text_text::name = L"";

std::wostream & text_text::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
	_Wostream << XmlUtils::EncodeXmlString( text_ );

	return _Wostream;
}

void text_text::serialize(std::wostream & _Wostream) 
{
    _Wostream << XmlUtils::EncodeXmlString( text_ , true);
}

void text_text::add_text(const std::wstring & Text) 
{
    text_ = Text;
};

office_element_ptr text_text::create(const std::wstring & Text)
{
    return boost::make_shared<text_text>(Text);
}
//----------------------------------------------------------------------------------
// text:s
//----------------------------------------------------------------------------------
const wchar_t * text_s::ns = L"text";
const wchar_t * text_s::name = L"s";

void text_s::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 		
			if ((text_c_) && (*text_c_ > 1))
			{
				CP_XML_ATTR_OPT(L"text:c", text_c_);
			}
		}
	}

}
//----------------------------------------------------------------------------------
// text:tab
//----------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------
// text:line-break
//----------------------------------------------------------------------------------
const wchar_t * text_line_break::ns = L"text";
const wchar_t * text_line_break::name = L"line-break";

void text_line_break::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------
// text:bookmark
//----------------------------------------------------------------------------------
const wchar_t * text_bookmark::ns = L"text";
const wchar_t * text_bookmark::name = L"bookmark";

void text_bookmark::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_ENCODE_STRING(L"text:name", text_name_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:bookmark-start
//----------------------------------------------------------------------------------
const wchar_t * text_bookmark_start::ns = L"text";
const wchar_t * text_bookmark_start::name = L"bookmark-start";

void text_bookmark_start::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_ENCODE_STRING(L"text:name", text_name_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:bookmark-end
//----------------------------------------------------------------------------------
const wchar_t * text_bookmark_end::ns = L"text";
const wchar_t * text_bookmark_end::name = L"bookmark-end";

void text_bookmark_end::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_ENCODE_STRING(L"text:name", text_name_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:reference-mark
//----------------------------------------------------------------------------------
const wchar_t * text_reference_mark::ns = L"text";
const wchar_t * text_reference_mark::name = L"reference-mark";

void text_reference_mark::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_ENCODE_STRING(L"text:name", text_name_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:reference-mark-start
//----------------------------------------------------------------------------------
const wchar_t * text_reference_mark_start::ns = L"text";
const wchar_t * text_reference_mark_start::name = L"reference-mark-start";

void text_reference_mark_start::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_ENCODE_STRING(L"text:name", text_name_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:reference-mark-end
//----------------------------------------------------------------------------------
const wchar_t * text_reference_mark_end::ns = L"text";
const wchar_t * text_reference_mark_end::name = L"reference-mark-end";

void text_reference_mark_end::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_ENCODE_STRING(L"text:name", text_name_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:span
//----------------------------------------------------------------------------------
const wchar_t * text_span::ns = L"text";
const wchar_t * text_span::name = L"span";

std::wostream & text_span::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->text_to_stream(_Wostream, bXmlEncode);
	}
	return _Wostream;
}
void text_span::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT(L"text:style-name", text_style_name_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_span::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void text_span::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void text_span::add_text(const std::wstring & Text)
{
	int bSpace = true;
	for (size_t i = 0 ; i < Text.size() ; i++)
	{
		if (Text[i] != 0x20)
		{
			bSpace = false;
			break;
		}
	}
	if (bSpace)
	{
		office_element_ptr elm = boost::make_shared<text_s>((unsigned int)Text.size());
		content_.push_back( elm );
	}
	else
	{
		office_element_ptr elm = text_text::create(Text);
		content_.push_back( elm );
	}
}
//----------------------------------------------------------------------------------
// text:a
//----------------------------------------------------------------------------------
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
			
			for (size_t i = 0; i < paragraph_content_.size(); i++)
			{
				paragraph_content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}    
}

void text_a::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(paragraph_content_);
}

void text_a::add_child_element( const office_element_ptr & child_element)
{
	paragraph_content_.push_back(child_element);
}

void text_a::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    paragraph_content_.push_back( elm );
}
//----------------------------------------------------------------------------------
// text:note-citation
//----------------------------------------------------------------------------------
const wchar_t * text_note_citation::ns = L"text";
const wchar_t * text_note_citation::name = L"note-citation";

void text_note_citation::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void text_note_citation::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void text_note_citation::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    content_.push_back( elm );
}

void text_note_citation::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT(L"text:label", text_label_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}    
}
//----------------------------------------------------------------------------------
// text:note-body
//----------------------------------------------------------------------------------
const wchar_t * text_note_body::ns	= L"text";
const wchar_t * text_note_body::name = L"note-body";

void text_note_body::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void text_note_body::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void text_note_body::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}    
}
//----------------------------------------------------------------------------------
// text:note
//----------------------------------------------------------------------------------
const wchar_t * text_note::ns	= L"text";
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

void text_note::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
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
void text_note::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextNoteCitation)
		text_note_citation_ = child_element;
	else if (type_ == typeTextNoteBody)
		text_note_body_ = child_element;
	else
	{
	}
}

void text_note::add_text(const std::wstring & Text)
{
}

//----------------------------------------------------------------------------------
// text:ruby
//----------------------------------------------------------------------------------
const wchar_t * text_ruby::ns = L"text";
const wchar_t * text_ruby::name = L"ruby";

void text_ruby::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
		    CP_XML_ATTR(L"text:style-name", text_style_name_);

			if (text_ruby_base_)
				text_ruby_base_->serialize(CP_XML_STREAM());

			if (text_ruby_text_)
				text_ruby_text_->serialize(CP_XML_STREAM());
		}
	}
}

void text_ruby::create_child_element( const std::wstring & Ns, const std::wstring & Name)
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
void text_ruby::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextRubyBase)
		text_ruby_base_ = child_element;
	else if (type_ == typeTextRubyText)
		text_ruby_text_ = child_element;
}

void text_ruby::add_text(const std::wstring & Text)
{
}

//----------------------------------------------------------------------------------
void common_field_fixed_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:fixed", text_fixed_);
}
//----------------------------------------------------------------------------------
// text:title
//----------------------------------------------------------------------------------
const wchar_t * text_title::ns = L"text";
const wchar_t * text_title::name = L"title";

void text_title::serialize(std::wostream & _Wostream) 
{

	//common_field_fixed_attlist_.add_attributes(Attributes);
    
}

void text_title::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
}

void text_title::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void text_title::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    content_.push_back( elm );
}
//----------------------------------------------------------------------------------
// text:placeholder
//----------------------------------------------------------------------------------
const wchar_t * text_placeholder::ns = L"text";
const wchar_t * text_placeholder::name = L"placeholder";

void text_placeholder::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}    
}

void text_placeholder::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
}

void text_placeholder::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void text_placeholder::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    content_.push_back( elm );
}
//----------------------------------------------------------------------------------
// text:page-number
//----------------------------------------------------------------------------------
const wchar_t * text_page_number::ns = L"text";
const wchar_t * text_page_number::name = L"page-number";

void text_page_number::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			common_num_format_attlist_.serialize(CP_GET_XML_NODE());
			common_field_fixed_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"text:page-adjust", text_page_adjust_);
			CP_XML_ATTR_OPT(L"text:select-page", text_select_page_);
			
			for (size_t i = 0; i < text_.size(); i++)
			{
				text_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_page_number::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_page_number::add_child_element( const office_element_ptr & child_element)
{
	text_.push_back(child_element);
}

void text_page_number::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}
//----------------------------------------------------------------------------------
// text:page-count
//----------------------------------------------------------------------------------
const wchar_t * text_page_count::ns = L"text";
const wchar_t * text_page_count::name = L"page-count";

void text_page_count::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			common_field_fixed_attlist_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < text_.size(); i++)
			{
				text_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void text_page_count::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_page_count::add_child_element( const office_element_ptr & child_element)
{
	text_.push_back(child_element);
}

void text_page_count::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}
//----------------------------------------------------------------------------------
// text:date
//----------------------------------------------------------------------------------
const wchar_t * text_date::ns = L"text";
const wchar_t * text_date::name = L"date";

void text_date::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			common_field_fixed_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"style:data-style-name", style_data_style_name_);
			CP_XML_ATTR_OPT(L"text:date-value", text_date_value_);

			for (size_t i = 0; i < text_.size(); i++)
			{
				text_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void text_date::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}
void text_date::add_child_element( const office_element_ptr & child_element)
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
			common_field_fixed_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"style:data-style-name", style_data_style_name_);
			CP_XML_ATTR_OPT(L"text:time-value", text_time_value_);
			
			for (size_t i = 0; i < text_.size(); i++)
			{
				text_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}


void text_time::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_time::add_child_element( const office_element_ptr & child_element)
{
	text_.push_back(child_element);
}

void text_time::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}

//----------------------------------------------------------------------------------
// text:time
//----------------------------------------------------------------------------------
const wchar_t * text_file_name::ns = L"text";
const wchar_t * text_file_name::name = L"file-name";

void text_file_name::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			common_field_fixed_attlist_.serialize(CP_GET_XML_NODE());
			
			for (size_t i = 0; i < text_.size(); i++)
			{
				text_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}


void text_file_name::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void text_file_name::add_child_element( const office_element_ptr & child_element)
{
	text_.push_back(child_element);
}
void text_file_name::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}

//----------------------------------------------------------------------------------
// text:sequence
//----------------------------------------------------------------------------------
const wchar_t * text_sequence::ns = L"text";
const wchar_t * text_sequence::name = L"sequence";

void text_sequence::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT(L"text:name", name_);
			CP_XML_ATTR_OPT(L"text:ref-name", ref_name_);	
			CP_XML_ATTR_OPT(L"style:num-format", style_num_format_);
			CP_XML_ATTR_OPT(L"style:num-letter-syn", style_num_letter_sync_);
			CP_XML_ATTR_OPT(L"text:formula", formula_);
	
			for (size_t i = 0; i < text_.size(); i++)
			{
				text_[i]->text_to_stream(CP_XML_STREAM());
			}
		}
	}
}

void text_sequence::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}
void text_sequence::add_child_element( const office_element_ptr & child_element)
{
	text_.push_back(child_element);
}
void text_sequence::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}
//----------------------------------------------------------------------------------
// text:text-input
//----------------------------------------------------------------------------------
const wchar_t * text_text_input::ns = L"text";
const wchar_t * text_text_input::name = L"text-input";

void text_text_input::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT(L"text:description", text_description_);
	
			for (size_t i = 0; i < text_.size(); i++)
			{
				text_[i]->text_to_stream(CP_XML_STREAM());
			}
		}
	}
}
void text_text_input::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}
void text_text_input::add_child_element( const office_element_ptr & child_element)
{
	text_.push_back(child_element);
}
void text_text_input::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}
//----------------------------------------------------------------------------------
// text:sequesheet-namence
//----------------------------------------------------------------------------------
const wchar_t * text_sheet_name::ns = L"text";
const wchar_t * text_sheet_name::name = L"sheet-name";

void text_sheet_name::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void text_sheet_name::add_child_element( const office_element_ptr & child_element)
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
			for (size_t i = 0; i < text_.size(); i++)
			{
				text_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//----------------------------------------------------------------------------------
// presentation:footer
//----------------------------------------------------------------------------------
const wchar_t * presentation_footer::ns = L"presentation";
const wchar_t * presentation_footer::name = L"footer";

void presentation_footer::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------
// presentation:date-time
//----------------------------------------------------------------------------------
const wchar_t * presentation_date_time::ns = L"presentation";
const wchar_t * presentation_date_time::name = L"date-time";

void presentation_date_time::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------
// text:toc-mark-start
//----------------------------------------------------------------------------------
const wchar_t * text_toc_mark_start::ns = L"text";
const wchar_t * text_toc_mark_start::name = L"toc-mark-start";

void text_toc_mark_start::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:id", id_);
			CP_XML_ATTR_OPT(L"text:outline-level", outline_level_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:toc-mark-end
//----------------------------------------------------------------------------------
const wchar_t * text_toc_mark_end::ns = L"text";
const wchar_t * text_toc_mark_end::name = L"toc-mark-end";

void text_toc_mark_end::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:id", id_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:toc-mark
//----------------------------------------------------------------------------------
const wchar_t * text_toc_mark::ns = L"text";
const wchar_t * text_toc_mark::name = L"toc-mark";

void text_toc_mark::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT_ENCODE_STRING(L"text:string-value", string_value_);
			CP_XML_ATTR_OPT(L"text:outline-level", outline_level_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:alphabetical-index-mark-start
//----------------------------------------------------------------------------------
const wchar_t * text_alphabetical_index_mark_start::ns = L"text";
const wchar_t * text_alphabetical_index_mark_start::name = L"alphabetical-index-mark-start";

void text_alphabetical_index_mark_start::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR		(L"text:id", id_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"text:string-value-phonetic", string_value_phonetic_);
			CP_XML_ATTR_OPT (L"text:key1", key1_);
			CP_XML_ATTR_OPT (L"text:key1-phonetic", key1_phonetic_);
			CP_XML_ATTR_OPT (L"text:key2", key2_);
			CP_XML_ATTR_OPT (L"text:key2-phonetic", key2_phonetic_);
			CP_XML_ATTR_OPT (L"text:main-entry", main_entry_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:alphabetical-index-mark-end
//----------------------------------------------------------------------------------
const wchar_t * text_alphabetical_index_mark_end::ns = L"text";
const wchar_t * text_alphabetical_index_mark_end::name = L"alphabetical-index-mark-end";

void text_alphabetical_index_mark_end::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR(L"text:id", id_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:alphabetical_index-mark
//----------------------------------------------------------------------------------
const wchar_t * text_alphabetical_index_mark::ns = L"text";
const wchar_t * text_alphabetical_index_mark::name = L"alphabetical-index-mark";

void text_alphabetical_index_mark::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT_ENCODE_STRING(L"text:string-value-phonetic", string_value_phonetic_);
			CP_XML_ATTR_OPT (L"text:key1", key1_);
			CP_XML_ATTR_OPT (L"text:key1-phonetic", key1_phonetic_);
			CP_XML_ATTR_OPT (L"text:key2", key2_);
			CP_XML_ATTR_OPT (L"text:key2-phonetic", key2_phonetic_);
			CP_XML_ATTR_OPT (L"text:main-entry", main_entry_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:bibliography-mark
//----------------------------------------------------------------------------------
const wchar_t * text_bibliography_mark::ns = L"text";
const wchar_t * text_bibliography_mark::name = L"bibliography-mark";

void text_bibliography_mark::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR		(L"text:identifier", identifier_);
			CP_XML_ATTR		(L"text:identifier", bibliography_type_);
			
			CP_XML_ATTR_OPT (L"text:url", url_);
			CP_XML_ATTR_OPT (L"text:author", author_);
			CP_XML_ATTR_OPT (L"text:title", title_);
			CP_XML_ATTR_OPT (L"text:year", year_);
			CP_XML_ATTR_OPT (L"text:isbn", isbn_);
			CP_XML_ATTR_OPT (L"text:chapter", chapter_);
			CP_XML_ATTR_OPT (L"text:address", address_);
			CP_XML_ATTR_OPT (L"text:annote", annote_);
			CP_XML_ATTR_OPT (L"text:booktitle", booktitle_);
			CP_XML_ATTR_OPT (L"text:edition", edition_);
			CP_XML_ATTR_OPT (L"text:editor", editor_);
			CP_XML_ATTR_OPT (L"text:howpublished", howpublished_);
			CP_XML_ATTR_OPT (L"text:institution", institution_);
			CP_XML_ATTR_OPT (L"text:issn", issn_);
			CP_XML_ATTR_OPT (L"text:journal", journal_);
			CP_XML_ATTR_OPT (L"text:month", month_);
			CP_XML_ATTR_OPT (L"text:note", note_);
			CP_XML_ATTR_OPT (L"text:number", number_);
			CP_XML_ATTR_OPT (L"text:organizations", organizations_);
			CP_XML_ATTR_OPT (L"text:pages", pages_);
			CP_XML_ATTR_OPT (L"text:publisher",publisher_);
			CP_XML_ATTR_OPT (L"text:report_type", report_type_);
			CP_XML_ATTR_OPT (L"text:school",school_);
			CP_XML_ATTR_OPT (L"text:series",series_);
			CP_XML_ATTR_OPT (L"text:volume",volume_);

			if (content_)
				CP_XML_CONTENT(*content_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:sequence-ref
//----------------------------------------------------------------------------------
const wchar_t * text_sequence_ref::ns = L"text";
const wchar_t * text_sequence_ref::name = L"sequence-ref";

void text_sequence_ref::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT (L"text:ref-name", ref_name_);
			CP_XML_ATTR_OPT (L"text:reference-format", reference_format_);
			
			if (false == content_.empty())
				CP_XML_CONTENT(content_);
		}
	}
}
//----------------------------------------------------------------------------------
// text:drop-down
//----------------------------------------------------------------------------------
const wchar_t * text_drop_down::ns = L"text";
const wchar_t * text_drop_down::name = L"drop-down";

void text_drop_down::add_text(const std::wstring & Text)
{
    text_ = text_text::create(Text) ;
}
void text_drop_down::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"label")
    {
		CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();

}
void text_drop_down::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextLabel)
		content_.push_back(child_element);
	else
	{
		text_ = child_element;
	}
}
void text_drop_down::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT (L"text:name", text_name_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
			if (text_)
				text_->text_to_stream(CP_XML_STREAM());
		}
	}
}
//----------------------------------------------------------------------------------
// text:label
//----------------------------------------------------------------------------------
const wchar_t * text_label::ns = L"text";
const wchar_t * text_label::name = L"label";

void text_label::add_text(const std::wstring & Text)
{
    text_ = Text;
}
void text_label::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_ATTR_OPT (L"text:value", text_value_);
			
			if (text_)
				CP_XML_CONTENT(*text_);
		}
	}
}
}
}
