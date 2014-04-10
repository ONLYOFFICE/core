#include "precompiled_cpodf.h"
#include "text_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "paragraph_elements.h"
#include "list.h"

#include "odf_conversion_context.h"
#include "odf_text_context.h"

#include "style_paragraph_properties.h"
#include "style_text_properties.h"


namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// text:h
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_h::ns = L"text";
const wchar_t * text_h::name = L"h";

void paragraph::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    CP_CREATE_ELEMENT_SIMPLE(paragraph_content_);
}

void paragraph::add_child_element( office_element_ptr & child_element)
{
	paragraph_content_.push_back(child_element);
}

void paragraph::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    paragraph_content_.push_back( elm );
}
void paragraph::serialize(std::wostream & _Wostream)
{
	BOOST_FOREACH(const office_element_ptr & elm, paragraph_content_)
	{    
		elm->serialize(_Wostream);
	}
}
void paragraph::serialize_attr(CP_ATTR_NODE)
{
	paragraph_attrs_.serialize(CP_GET_XML_NODE());

}

void paragraph_attrs::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:style-name",		text_style_name_);
    CP_XML_ATTR_OPT(L"text:cond-style-name",text_cond_style_name_);
   
	std::wstring text_class_names_all;

	BOOST_FOREACH(const style_ref & elm, text_class_names_)
	{    
		text_class_names_all = elm.style_name() + std::wstring(L" ");
	}
	if (text_class_names_all.length()>0)
		CP_XML_ATTR(L"text:class-names",	text_class_names_all);
}
//////////////////////////////////////////////


text_h::text_h() : text_outline_level_(1), text_restart_numbering_(false), text_is_list_header_(false)
{}

void text_h::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"text:outline-level",		text_outline_level_);
			CP_XML_ATTR(L"text:restart-numbering",	text_restart_numbering_);
			CP_XML_ATTR_OPT(L"text:start-value",	text_start_value_);
			CP_XML_ATTR(L"text:is-list-header",		text_is_list_header_);

			 paragraph_.serialize_attr	(CP_GET_XML_NODE());
			 paragraph_.serialize		(CP_XML_STREAM());
		}
	}
}
//
void text_h::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    paragraph_.create_child_element(Ns, Name, getContext());
}

void text_h::add_text(const std::wstring & Text)
{
    paragraph_.add_text(Text);
}
void text_h::add_child_element( office_element_ptr & child_element)
{
    paragraph_.add_child_element(child_element);
}
// text:p
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_p::ns = L"text";
const wchar_t * text_p::name = L"p";

void text_p::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    paragraph_.create_child_element( Ns, Name, getContext());
}
void text_p::add_child_element( office_element_ptr & child_element)
{
    paragraph_.add_child_element(child_element);
}
void text_p::add_text(const std::wstring & Text)
{
    paragraph_.add_text(Text);
}

void text_p::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			 paragraph_.serialize_attr	(CP_GET_XML_NODE());
			 paragraph_.serialize		(CP_XML_STREAM());
		}
	}
}


// text:list
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list::ns = L"text";
const wchar_t * text_list::name = L"list";

void text_list::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR(L"text:style-name",text_style_name_);
			CP_XML_ATTR_OPT(L"text_continue_numbering_", text_continue_numbering_ );
			
			if (text_list_header_) text_list_header_->serialize(CP_XML_STREAM());
   
			BOOST_FOREACH(const office_element_ptr & listItem, text_list_items_)
			{
				listItem->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_list::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
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
void text_list::add_child_element( office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTextListHeader)
		text_list_header_ = child_element;
	else
		text_list_items_.push_back(child_element);
}
// text:soft-page-break
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_soft_page_break::ns = L"text";
const wchar_t * text_soft_page_break::name = L"soft-page-break";

void text_soft_page_break::serialize(::std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}

// text-section-attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_section_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:style-name", text_style_name_);
    CP_XML_ATTR(L"text:name",			text_name_);
    CP_XML_ATTR_OPT(L"text:protected",	text_protected_);
    CP_XML_ATTR_OPT(L"text:protection-key", text_protection_key_);
    CP_XML_ATTR_OPT(L"text:display",		text_display_);
    CP_XML_ATTR_OPT(L"text:condition",	text_condition_);
}

// text:section
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_section::ns = L"text";
const wchar_t * text_section::name = L"section";

void text_section::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
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

void text_section::add_child_element( office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTextSectionSource)
		text_section_source_ = child_element;
	else
		text_content_.push_back(child_element);
}

void text_section::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			text_section_attr_.serialize(CP_GET_XML_NODE());

			if (text_section_source_)text_section_source_->serialize(CP_XML_STREAM());
 			
			BOOST_FOREACH(const office_element_ptr & elm, text_content_)
			{
				elm->serialize(CP_XML_STREAM());
			}   
		}
	}
}

// text-section-source-attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_section_source_attr::serialize(CP_ATTR_NODE)
{
    common_xlink_attlist_.serialize(CP_GET_XML_NODE());

    CP_XML_ATTR_OPT(L"text:section-name", text_section_name_);
    CP_XML_ATTR_OPT(L"text:filter-name", text_filter_name_);
}

// text:section-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_section_source::ns = L"text";
const wchar_t * text_section_source::name = L"section-source";

void text_section_source::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void text_section_source::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			text_section_source_attr_.serialize(CP_GET_XML_NODE());
		}
	}
}
// text:index-body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_index_body::ns = L"text";
const wchar_t * text_index_body::name = L"index-body";

void text_index_body::serialize(::std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			BOOST_FOREACH(const office_element_ptr & listItem, index_content_main_)
			{
				listItem->serialize(CP_XML_STREAM());
			}
		}
	} 
}

void text_index_body::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(index_content_main_);
}

void text_index_body::add_child_element( office_element_ptr & child_element)
{
    index_content_main_.push_back(child_element);
}

// text:index-title
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_index_title::ns = L"text";
const wchar_t * text_index_title::name = L"index-title";

void text_index_title::serialize(::std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			BOOST_FOREACH(const office_element_ptr & listItem, index_content_main_)
			{
				listItem->serialize(CP_XML_STREAM());
			}
		}
	} 
}
void text_index_title::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
     CP_CREATE_ELEMENT(index_content_main_);
}
void text_index_title::add_child_element( office_element_ptr & child_element)
{
     index_content_main_.push_back(child_element);
}

// text:table-of-content
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_table_of_content::ns = L"text";
const wchar_t * text_table_of_content::name = L"table-of-content";

void text_table_of_content::serialize(::std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			text_section_attr_.serialize(CP_GET_XML_NODE());
			if (text_index_body_)text_index_body_->serialize(CP_XML_STREAM());
			if (text_table_of_content_source_)text_table_of_content_source_->serialize(CP_XML_STREAM());
		}
	} 
}
void text_table_of_content::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(text_index_body_);
    }
}

void text_table_of_content::add_child_element( office_element_ptr & child_element)
{
    //if CP_CHECK_NAME(L"text", L"index-body")
    {
        text_index_body_ = child_element;
    }
}

}
}
