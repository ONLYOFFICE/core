
#include "list.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf_writer {


using xml::xml_char_wc;

// text:number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_number::ns = L"text";
const wchar_t * text_number::name = L"number";

void text_number::serialize(::std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_STREAM() << string_;
		}
	}
}

void text_number::add_text(const std::wstring & Text)
{
    string_ = Text;
}

// text:list-item
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_item::ns = L"text";
const wchar_t * text_list_item::name = L"list-item";

void text_list_item::serialize(::std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (text_number_)text_number_->serialize(CP_XML_STREAM());

			BOOST_FOREACH(const office_element_ptr & parElement, content_)
			{
				parElement->serialize(CP_XML_STREAM());
			}
		}
	}
}


void text_list_item::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"number")
    {
        CP_CREATE_ELEMENT(text_number_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);        
    }
}
void text_list_item::add_child_element( const office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTextNumber)
    {
        text_number_ = child_element;
    }
    else
    {
        content_.push_back( child_element);       
    }
}
// text:list-header
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_header::ns = L"text";
const wchar_t * text_list_header::name = L"list-header";


void text_list_header::serialize(::std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (text_number_)text_number_->serialize(CP_XML_STREAM());
			
			BOOST_FOREACH(const office_element_ptr & parElement, content_)
			{
				parElement->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_list_header::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"number")
    {
        CP_CREATE_ELEMENT(text_number_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);        
    }
}
void text_list_header::add_child_element( const office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTextNumber)
    {
        text_number_ = child_element;
    }
    else
    {
        content_.push_back( child_element);       
    }
}

}
}