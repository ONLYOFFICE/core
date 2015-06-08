
#include "draw_frame.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.h>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>

#include "style_graphic_properties.h"
#include "length.h"
#include "borderstyle.h"

#include "odf_conversion_context.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;

void draw_image_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:filter-name", draw_filter_name_);
}

void draw_frame_attlist::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"draw:copy-of", draw_copy_of_);
}

void draw_chart_attlist::serialize(CP_ATTR_NODE)
{
    //CP_XML_ATTR_OPT(L"draw:filter-name", draw_filter_name_);
}

// draw:image
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_image::ns = L"draw";
const wchar_t * draw_image::name = L"image";

void draw_image::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_image_attlist_.serialize(CP_GET_XML_NODE());
			common_xlink_attlist_.serialize(CP_GET_XML_NODE());

			if (office_binary_data_)office_binary_data_->serialize(CP_XML_STREAM());

			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

void draw_image::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"office", L"binary-data")
    {
        CP_CREATE_ELEMENT(office_binary_data_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);
        //CP_NOT_APPLICABLE_ELM();
    }
}
void draw_image::add_child_element( const office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeOfficeBinaryData)
    {
		office_binary_data_ = child_element;
    } 
	else
		content_.push_back(child_element);
}
// draw:chart
////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_chart::ns = L"draw";
const wchar_t * draw_chart::name = L"chart";


void draw_chart::serialize(std::wostream & _Wostream)
{
     CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_chart_attlist_.serialize(CP_GET_XML_NODE());
			common_xlink_attlist_.serialize(CP_GET_XML_NODE());

			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}


void draw_chart::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void draw_chart::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

// draw:g
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_g::ns = L"draw";
const wchar_t * draw_g::name = L"g";

void draw_g::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_draw_attlists_.serialize(CP_GET_XML_NODE());

			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
void draw_g::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void draw_g::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// draw:frame
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_frame::ns = L"draw";
const wchar_t * draw_frame::name = L"frame";

void draw_frame::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_base::serialize_attlist(CP_GET_XML_NODE());
			
			draw_frame_attlist_.serialize(CP_GET_XML_NODE());

			if (office_event_listeners_)office_event_listeners_->serialize(CP_XML_STREAM());

			draw_base::serialize(CP_XML_STREAM());
		}
	}
}

void draw_frame::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (CP_CHECK_NAME(L"draw", L"text-box") ||
        CP_CHECK_NAME(L"draw", L"image") ||//копия чарта в виде картинки ну.. или просто картинка
        CP_CHECK_NAME(L"draw", L"object") ||//месторасположение чарта
        CP_CHECK_NAME(L"draw", L"object-ole") ||
        CP_CHECK_NAME(L"draw", L"applet") ||
        CP_CHECK_NAME(L"draw", L"floating-frame") ||
        CP_CHECK_NAME(L"draw", L"plugin") ||
        CP_CHECK_NAME(L"table", L"table") 
        )
    {
        draw_base::create_child_element(Ns,Name);
    }
    else if CP_CHECK_NAME(L"office", L"event-listeners")
    {
        CP_CREATE_ELEMENT(office_event_listeners_);                    
    }
    else if CP_CHECK_NAME(L"draw", L"glue-point")
    {
        CP_CREATE_ELEMENT(draw_glue_point_);
    }
    else if CP_CHECK_NAME(L"draw", L"image-map")
    {
        CP_CREATE_ELEMENT(draw_image_map_);
    }
    else if (   CP_CHECK_NAME(L"draw", L"contour-polygon") ||
                CP_CHECK_NAME(L"draw", L"contour-path") )
    {
        CP_CREATE_ELEMENT(draw_contour_);        
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void draw_frame::add_child_element( const office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeDrawImage || type == typeDrawTextBox || type == typeDrawObject || type == typeDrawObjectOle || type == typeTableTable)
    {
		draw_base::add_child_element(child_element);
    } 
	else if (type == typeOfficeEventListeners)
	{
		office_event_listeners_ = child_element;
	}
	else
	{
        //CP_NOT_APPLICABLE_ELM();
	}

}

///////////////////////

void draw_text_box_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:chain-next-name", draw_chain_next_name_);
    CP_XML_ATTR_OPT(L"draw:corner-radius", draw_corner_radius_);
    CP_XML_ATTR_OPT(L"fo:min-width", fo_min_width_);
    CP_XML_ATTR_OPT(L"fo:min-height", fo_min_height_);
    CP_XML_ATTR_OPT(L"fo:max-width", fo_max_width_);
    CP_XML_ATTR_OPT(L"fo:max-height", fo_max_height_);
}

// draw:text-box
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_text_box::ns = L"draw";
const wchar_t * draw_text_box::name = L"text-box";


void draw_text_box::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_text_box_attlist_.serialize(CP_GET_XML_NODE());

			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

void draw_text_box::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_); 
}
void draw_text_box::add_child_element( const office_element_ptr & child_element)
{
   content_.push_back(child_element);
}

// draw:object
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_object::ns = L"draw";
const wchar_t * draw_object::name = L"object";

void draw_object::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_xlink_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

void draw_object::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM(); // TODO
}
void draw_object::add_child_element( const office_element_ptr & child_element)
{
	//CP_NOT_APPLICABLE_ELM();
}
// draw:object
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_object_ole::ns = L"draw";
const wchar_t * draw_object_ole::name = L"object-ole";

void draw_object_ole::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"draw:class-id", draw_class_id_);
			common_xlink_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

void draw_object_ole::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM(); 
}
void draw_object_ole::add_child_element( const office_element_ptr & child_element)
{
	//CP_NOT_APPLICABLE_ELM();
}
}
}
