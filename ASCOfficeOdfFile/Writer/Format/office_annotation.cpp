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

#include "office_annotation.h"

#include <xml/xmlchar.h>

#include <xml/attributes.h>
#include <xml/utils.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

// dc:date
//-------------------------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------------

void office_annotation_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"draw:style-name",			draw_style_name_); 
	CP_XML_ATTR_OPT(L"draw:text-style-name",	draw_text_style_name_); 
	
	CP_XML_ATTR_OPT(L"draw:caption-point-x",	caption_point_x_); 
	CP_XML_ATTR_OPT(L"draw:caption-point-x",	caption_point_y_); 
	
	CP_XML_ATTR_OPT(L"svg:y", svg_y_);
	CP_XML_ATTR_OPT(L"svg:x", svg_x_);
	CP_XML_ATTR_OPT(L"svg:width",		svg_width_); 
	CP_XML_ATTR_OPT(L"svg:height",		svg_height_); 
	
	CP_XML_ATTR_OPT(L"office:display",display_);
	CP_XML_ATTR_OPT(L"office:name",name_);
}

// office:annotation
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * office_annotation::ns	= L"office";
const wchar_t * office_annotation::name = L"annotation";

void office_annotation::create_child_element(const std::wstring & Ns, const std::wstring & Name)
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
void office_annotation::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeDcCreator)
		dc_creator_ = child_element;
	else if (type_ == typeDcDate)
		dc_date_ = child_element;
	else
		content_.push_back(child_element);
}
const wchar_t * office_annotation_end::ns = L"office";
const wchar_t * office_annotation_end::name = L"annotation-end";

void office_annotation_end::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			attr_.serialize(CP_GET_XML_NODE());
		}
	}
}
void office_annotation::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			attr_.serialize(CP_GET_XML_NODE());

			if (dc_creator_)dc_creator_->serialize(CP_XML_STREAM());
			if (dc_date_)	dc_date_->serialize(CP_XML_STREAM());
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
// officeooo:annotation
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * officeooo_annotation::ns	= L"officeooo";
const wchar_t * officeooo_annotation::name	= L"annotation";

void officeooo_annotation::create_child_element( const std::wstring & Ns, const std::wstring & Name)
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
void officeooo_annotation::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeDcCreator)
		dc_creator_ = child_element;
	else if (type_ == typeDcDate)
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
			attr_.serialize(CP_GET_XML_NODE());

			if (dc_creator_)dc_creator_->serialize(CP_XML_STREAM());
			if (dc_date_)	dc_date_->serialize(CP_XML_STREAM());
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
}
}
