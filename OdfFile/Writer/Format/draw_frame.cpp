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

#include "draw_frame.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/regex.h>

#include <xml/xmlchar.h>

#include <xml/attributes.h>

#include "style_graphic_properties.h"
#include "odf_conversion_context.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"

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

//-----------------------------------------------------------------------
const wchar_t * draw_image::ns		= L"draw";
const wchar_t * draw_image::name	= L"image";

void draw_image::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_image_attlist_.serialize(CP_GET_XML_NODE());
			common_xlink_attlist_.serialize(CP_GET_XML_NODE());

			if (office_binary_data_)office_binary_data_->serialize(CP_XML_STREAM());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void draw_image::create_child_element(const std::wstring & Ns, const std::wstring & Name)
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
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeOfficeBinaryData)
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

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}


void draw_chart::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void draw_chart::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

///-----------------------------------------------------------------------
const wchar_t * draw_g::ns = L"draw";
const wchar_t * draw_g::name = L"g";

void draw_g::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (!common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_name_)
				common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_name_ = L"";
			//брееед ... иначе либра не показывает группу!!!

			common_draw_attlists_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void draw_g::create_child_element( const std::wstring & Ns, const std::wstring & Name)
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

void draw_frame::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
    if (CP_CHECK_NAME(L"draw", L"text-box") ||
        CP_CHECK_NAME(L"draw", L"image")	||//копия чарта в виде картинки ну.. или просто картинка
        CP_CHECK_NAME(L"draw", L"object")	||//месторасположение чарта
        CP_CHECK_NAME(L"draw", L"object-ole") ||
        CP_CHECK_NAME(L"draw", L"applet")	||
        CP_CHECK_NAME(L"draw", L"floating-frame") ||
        CP_CHECK_NAME(L"draw", L"plugin")	||
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
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeDrawImage		|| 
		type_ == typeDrawTextBox	|| 
		type_ == typeDrawObject		|| 
		type_ == typeDrawObjectOle	|| 
		type_ == typeDrawPlugin		|| 
		type_ == typeTableTable)
    {
		draw_base::add_child_element(child_element);
    } 
	else if (type_ == typeOfficeEventListeners)
	{
		office_event_listeners_ = child_element;
	}
	else
	{
        //CP_NOT_APPLICABLE_ELM();
	}

}

void draw_text_box_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:chain-next-name", draw_chain_next_name_);
    CP_XML_ATTR_OPT(L"draw:corner-radius", draw_corner_radius_);
    CP_XML_ATTR_OPT(L"fo:min-width", fo_min_width_);
    CP_XML_ATTR_OPT(L"fo:min-height", fo_min_height_);
    CP_XML_ATTR_OPT(L"fo:max-width", fo_max_width_);
    CP_XML_ATTR_OPT(L"fo:max-height", fo_max_height_);
}

//-----------------------------------------------------------------------
const wchar_t * draw_text_box::ns	= L"draw";
const wchar_t * draw_text_box::name = L"text-box";


void draw_text_box::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void draw_text_box::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_); 
}
void draw_text_box::add_child_element( const office_element_ptr & child_element)
{
   content_.push_back(child_element);
}

//-----------------------------------------------------------------------
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

void draw_object::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM(); // TODO
}
void draw_object::add_child_element( const office_element_ptr & child_element)
{
	//CP_NOT_APPLICABLE_ELM();
}
//-----------------------------------------------------------------------
const wchar_t * draw_object_ole::ns		= L"draw";
const wchar_t * draw_object_ole::name	= L"object-ole";

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

void draw_object_ole::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM(); 
}
void draw_object_ole::add_child_element( const office_element_ptr & child_element)
{
	//CP_NOT_APPLICABLE_ELM();
}
//-----------------------------------------------------------------------
const wchar_t * draw_plugin::ns		= L"draw";
const wchar_t * draw_plugin::name	= L"plugin";

void draw_plugin::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_xlink_attlist_.serialize(CP_GET_XML_NODE());
			CP_XML_ATTR_OPT(L"draw:mime-type", draw_mime_type_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void draw_plugin::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"draw", L"param")
    {
        CP_CREATE_ELEMENT(content_);
    }
}
void draw_plugin::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
}
}
