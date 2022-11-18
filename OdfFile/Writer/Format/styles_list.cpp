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

#include "styles_list.h"
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {
    

///////////////////////////////////////
void text_list_style_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:name",					style_name_);
	CP_XML_ATTR_OPT_ENCODE_STRING(L"style:display-name", style_display_name_);
    CP_XML_ATTR_OPT(L"text:consecutive-numbering",	text_consecutive_numbering_);
 }

void text_list_level_style_number_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:style-name"		,	text_style_name_);
    CP_XML_ATTR_OPT(L"text:display-levels"	,	text_display_levels_);
    CP_XML_ATTR_OPT(L"text:start-value"		,	text_start_value_);

	common_num_format_attlist_.serialize(CP_GET_XML_NODE());
    common_num_format_prefix_suffix_attlist_.serialize(CP_GET_XML_NODE());
 }
void text_list_level_style_bullet_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:style-name",			text_style_name_);
    CP_XML_ATTR_OPT(L"text:bullet-char",		text_bullet_char_);
    CP_XML_ATTR_OPT(L"text:bullet-relative-size",	text_bullet_relative_size_);

    prefix_suffix_attlist_.serialize(CP_GET_XML_NODE());
 }
void text_list_level_style_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR(L"text:level",	text_level_);
 }
void text_list_level_style_image_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:style-name",			text_style_name_);

	common_xlink_attlist_.serialize(CP_GET_XML_NODE());
 }
// text:list-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_style::ns = L"text";
const wchar_t * text_list_style::name = L"list-style";


void text_list_style::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"list-level-style-number" == Name)
        CP_CREATE_ELEMENT(text_list_style_content_);
    else if (L"text" == Ns && L"list-level-style-bullet" == Name)
        CP_CREATE_ELEMENT(text_list_style_content_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
void text_list_style::add_child_element( const office_element_ptr & child)
{
 	ElementType type_ = child->get_type();
	
	text_list_style_content_.push_back(child);
}
void text_list_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			text_list_style_attr_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < text_list_style_content_.size(); i++)
			{
				if (text_list_style_content_[i])
					text_list_style_content_[i]->serialize(CP_XML_STREAM());
			}	
		}
	}
}

// text:list-level-style-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_number::ns = L"text";
const wchar_t * text_list_level_style_number::name = L"list-level-style-number";


void text_list_level_style_number::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if	(L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);    
	else if (L"style" == Ns && L"text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_); 
	else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
void text_list_level_style_number::add_child_element( const office_element_ptr & child)
{
 	ElementType type_ = child->get_type();
	
	if (type_ == typeStyleListLevelProperties)
		style_list_level_properties_ = child;
    else if (type_ == typeStyleTextProperties)
       style_text_properties_ = child;    
    else
    {
    }
}
void text_list_level_style_number::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			text_list_level_style_attr_.serialize(CP_GET_XML_NODE());
			text_list_level_style_number_attr_.serialize(CP_GET_XML_NODE());	

			if (style_list_level_properties_)	style_list_level_properties_->serialize(CP_XML_STREAM());
			if (style_text_properties_)			style_text_properties_->serialize(CP_XML_STREAM());
		}
	}
}
// style:list-level-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_properties::ns = L"style";
const wchar_t * style_list_level_properties::name = L"list-level-properties";

void style_list_level_properties::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"style", L"list-level-label-alignment")
    {
        CP_CREATE_ELEMENT(style_list_level_label_alignment_);
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void style_list_level_properties::add_child_element( const office_element_ptr & child)
{
 	ElementType type_ = child->get_type();
	
	if (type_ == typeStyleListLevelProperties)
		style_list_level_label_alignment_ = child;
    else
    {
    }
}
void style_list_level_properties::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"text:space-before",		text_space_before_);
			CP_XML_ATTR_OPT(L"text:min-label-width",	text_min_label_width_);
			CP_XML_ATTR_OPT(L"text:min-label-distance",	text_min_label_distance_);
			CP_XML_ATTR_OPT(L"style:font-name",			style_font_name_);
			CP_XML_ATTR_OPT(L"fo:width",				fo_width_);
			CP_XML_ATTR_OPT(L"fo:height",				fo_height_);
		  
			common_text_align_.serialize(CP_GET_XML_NODE());
			common_vertical_rel_attlist_.serialize(CP_GET_XML_NODE());
			common_vertical_pos_attlist_.serialize(CP_GET_XML_NODE());

			if (style_list_level_label_alignment_)
			{
				CP_XML_ATTR(L"text:list-level-position-and-space-mode", L"label-alignment");
				style_list_level_label_alignment_->serialize(CP_XML_STREAM());
			}
		}
	}
}
// style:list-level-label-alignment
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_label_alignment::ns = L"style";
const wchar_t * style_list_level_label_alignment::name = L"list-level-label-alignment";

void style_list_level_label_alignment::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
     CP_NOT_APPLICABLE_ELM();
}
void style_list_level_label_alignment::add_child_element( const office_element_ptr & child)
{
//   CP_NOT_APPLICABLE_ELM();
}

void style_list_level_label_alignment::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"text:label-followed-by",		text_label_followed_by_);
			CP_XML_ATTR_OPT(L"text:list-tab-stop-position",	text_list_tab_stop_position_);
			CP_XML_ATTR_OPT(L"fo:text-indent",				fo_text_indent_);
			CP_XML_ATTR_OPT(L"fo:margin-left",				fo_margin_left_);
			CP_XML_ATTR_OPT(L"fo:margin-right",				fo_margin_right_);
		}
	}
}
// text:list-level-style-bullet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_bullet::ns = L"text";
const wchar_t * text_list_level_style_bullet::name = L"list-level-style-bullet";


void text_list_level_style_bullet::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);
    else if (L"style" == Ns && L"text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
void text_list_level_style_bullet::add_child_element( const office_element_ptr & child)
{
 	ElementType type_ = child->get_type();
	
	if (type_ == typeStyleListLevelProperties)
		style_list_level_properties_ = child;
    else if (type_ == typeStyleTextProperties)
       style_text_properties_ = child;    
    else
    {
    }
}
void text_list_level_style_bullet::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			text_list_level_style_attr_.serialize(CP_GET_XML_NODE());
			text_list_level_style_bullet_attr_.serialize(CP_GET_XML_NODE());	

			if (style_list_level_properties_)	style_list_level_properties_->serialize(CP_XML_STREAM());
			if (style_text_properties_)			style_text_properties_->serialize(CP_XML_STREAM());
		}
	}
}

// text:list-level-style-image
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_image::ns = L"text";
const wchar_t * text_list_level_style_image::name = L"list-level-style-image";


void text_list_level_style_image::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);
    else if (L"style" == Ns && L"text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
void text_list_level_style_image::add_child_element( const office_element_ptr & child)
{
 	ElementType type_ = child->get_type();
	
	if (type_ == typeStyleListLevelProperties)
		style_list_level_properties_ = child;
    else if (type_ == typeStyleTextProperties)
       style_text_properties_ = child;    
    else
    {
    }
}
void text_list_level_style_image::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			text_list_level_style_attr_.serialize(CP_GET_XML_NODE());
			text_list_level_style_image_attr_.serialize(CP_GET_XML_NODE());	

			if (style_list_level_properties_)	style_list_level_properties_->serialize(CP_XML_STREAM());
			if (style_text_properties_)			style_text_properties_->serialize(CP_XML_STREAM());
		}
	}
}
// text:outline-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_outline_style::ns = L"text";
const wchar_t * text_outline_style::name = L"outline-style";

void text_outline_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_ATTR(L"style:name", style_name_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (content_[i])
					content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void text_outline_style::add_child_element(const office_element_ptr & child)
{
	ElementType type_ = child->get_type();

	content_.push_back(child);
}
void text_outline_style::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	if (L"text" == Ns && L"outline-level-style" == Name)
		CP_CREATE_ELEMENT(content_);
	else
	{
		CP_NOT_APPLICABLE_ELM();
	}
}
// text:outline-level-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_outline_level_style::ns = L"text";
const wchar_t * text_outline_level_style::name = L"outline-level-style";

void text_outline_level_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE_SIMPLE()
		{
			common_num_format_attlist_.serialize(CP_GET_XML_NODE());
			common_num_format_prefix_suffix_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR(L"text:style-name", text_style_name_);
			CP_XML_ATTR(L"text:display-levels", text_display_levels_);
			CP_XML_ATTR(L"text:start-value", text_start_value_);
			CP_XML_ATTR(L"text:level", text_level_);

			if (list_level_properties_)
				list_level_properties_->serialize(CP_XML_STREAM());

			if (text_properties_)
				text_properties_->serialize(CP_XML_STREAM());
		}
	}
}

void text_outline_level_style::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	if (L"style" == Ns && L"list-level-properties" == Name)
		CP_CREATE_ELEMENT(list_level_properties_);
	else if (L"style" == Ns && L"text-properties" == Name)
		CP_CREATE_ELEMENT(text_properties_);
	else
	{
		CP_NOT_APPLICABLE_ELM();
	}
}
void text_outline_level_style::add_child_element(const office_element_ptr & child)
{
	ElementType type_ = child->get_type();

	if (type_ == typeStyleListLevelProperties)
		list_level_properties_ = child;
	else if (type_ == typeStyleTextProperties)
		text_properties_ = child;
	else
	{
	}
}
}
}
