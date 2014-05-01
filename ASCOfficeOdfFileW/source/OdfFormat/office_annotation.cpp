#include "precompiled_cpodf.h"
#include "office_annotation.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>

namespace cpdoccore { 
namespace odf {

// dc:date
/////////////////////////////////////////////
const wchar_t * dc_date::ns = L"dc";
const wchar_t * dc_date::name = L"date";

void dc_date::add_text(const std::wstring & Text)
{
	content_ = Text;
}
void dc_date::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_STREAM() << content_;
		}
	}
}
// dc:creator
///////////////////////////////////////////
const wchar_t * dc_creator::ns = L"dc";
const wchar_t * dc_creator::name = L"creator";

void dc_creator::add_text(const std::wstring & Text)
{
	content_ = Text;
}
void dc_creator::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 	
			CP_XML_STREAM() << content_;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void office_annotation_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"draw:style-name",		draw_style_name_); 
	CP_XML_ATTR_OPT(L"draw:text-style-name",	draw_text_style_name_); 
	
	CP_XML_ATTR_OPT(L"draw:caption-point-x",	caption_point_x_); 
	CP_XML_ATTR_OPT(L"draw:caption-point-x",	caption_point_y_); 
	
	CP_XML_ATTR_OPT(L"svg:y", svg_y_);
	CP_XML_ATTR_OPT(L"svg:x", svg_x_);
	CP_XML_ATTR_OPT(L"svg:width",		svg_width_); 
	CP_XML_ATTR_OPT(L"svg:height",	svg_height_); 
	
	CP_XML_ATTR_OPT(L"office:display",display_);
}

// office:annotation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_annotation::ns = L"office";
const wchar_t * office_annotation::name = L"annotation";

void office_annotation::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if (Ns==L"dc" && Name == L"date")
	{
		CP_CREATE_ELEMENT(dc_date_);
	}
	else if (Ns==L"dc" && Name == L"creator")
	{
		CP_CREATE_ELEMENT(dc_creator_);
	}
	else
	{
		CP_CREATE_ELEMENT(content_);
	}
}
void office_annotation::add_child_element( office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeDcCreator)
		dc_creator_ = child_element;
	else if (type == typeDcDate)
		dc_date_ = child_element;
	else
		content_.push_back(child_element);
}
void office_annotation::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			office_annotation_attr_.serialize(CP_GET_XML_NODE());

			if (dc_creator_)dc_creator_->serialize(CP_XML_STREAM());
			if (dc_date_)	dc_date_->serialize(CP_XML_STREAM());
			
			BOOST_FOREACH(const office_element_ptr & item, content_)
			{
				item->serialize(CP_XML_STREAM());
			}
		}
	}
}
// officeooo:annotation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * officeooo_annotation::ns = L"officeooo";
const wchar_t * officeooo_annotation::name = L"annotation";

void officeooo_annotation::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if (Ns==L"dc" && Name == L"date")
	{
		CP_CREATE_ELEMENT(dc_date_);
	}
	else if (Ns==L"dc" && Name == L"creator")
	{
		CP_CREATE_ELEMENT(dc_creator_);
	}
	else
	{
		CP_CREATE_ELEMENT(content_);
	}
}
void officeooo_annotation::add_child_element( office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeDcCreator)
		dc_creator_ = child_element;
	else if (type == typeDcDate)
		dc_date_ = child_element;
	else
		content_.push_back(child_element);
}
void officeooo_annotation::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			office_annotation_attr_.serialize(CP_GET_XML_NODE());

			if (dc_creator_)dc_creator_->serialize(CP_XML_STREAM());
			if (dc_date_)	dc_date_->serialize(CP_XML_STREAM());
			
			BOOST_FOREACH(const office_element_ptr & item, content_)
			{
				item->serialize(CP_XML_STREAM());
			}
		}
	}
}
}
}
