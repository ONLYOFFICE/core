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


#include <xml/xmlchar.h>

#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>
#include <iostream>

#include "office_elements_create.h"

#include "style_section_properties.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {
	
/// style:columns
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_columns::ns = L"style";
const wchar_t * style_columns::name = L"columns";

void style_columns::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"column" == Name)
        CP_CREATE_ELEMENT(style_column_);
    else if (L"style" == Ns && L"column-sep" == Name)
        CP_CREATE_ELEMENT(style_column_sep_);
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void style_columns::add_child_element( const office_element_ptr & child)
{
	if (!child)return;

	ElementType type_ = child->get_type();

	switch(type_)
	{
	case typeStyleColumn:	
		style_column_.push_back(child);
		break;
	case typeStyleColumnSep:
        style_column_sep_ = child;
		break;
	}
}
void style_columns::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"fo:column-count",	fo_column_count_);
			CP_XML_ATTR_OPT(L"fo:column-gap",	fo_column_gap_);

			if(style_column_sep_) style_column_sep_->serialize(CP_XML_STREAM());
			
			for (size_t i = 0; i < style_column_.size(); i++)
			{		
				style_column_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
/// style:column
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_column::ns = L"style";
const wchar_t * style_column::name = L"column";

void style_column::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void style_column::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:rel-width",	style_rel_width_);
			CP_XML_ATTR_OPT(L"fo:start-indent",	fo_start_indent_);
			CP_XML_ATTR_OPT(L"fo:end-indent",	fo_end_indent_);
			CP_XML_ATTR_OPT(L"fo:space-before",	fo_space_before_);
			CP_XML_ATTR_OPT(L"fo:space-after",	fo_space_after_);
		}
	}
}
/// style:column-sep
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_column_sep::ns = L"style";
const wchar_t * style_column_sep::name = L"column-sep";

void style_column_sep::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void style_column_sep::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:style",			style_style_);			// default solid
			CP_XML_ATTR_OPT(L"style:width",			style_width_);
			CP_XML_ATTR_OPT(L"style:height",		style_height_);			// default 100
			CP_XML_ATTR_OPT(L"style:vertical-align",style_vertical_align_); //default top
			CP_XML_ATTR_OPT(L"style:color",			style_color_);			// default #000000
		}
	}
}
/// style:section-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_section_properties::ns = L"style";
const wchar_t * style_section_properties::name = L"section-properties";

void style_section_properties::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
        CP_CREATE_ELEMENT(style_background_image_);    
    else if (L"style" == Ns && L"columns" == Name)
        CP_CREATE_ELEMENT(style_columns_);        
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void style_section_properties::add_child_element( const office_element_ptr & child)
{
 	if (!child)return;

	ElementType type_ = child->get_type();

	switch(type_)
	{
	case typeStyleBackgroundImage:	
		style_background_image_ = child;
		break;
	case typeStyleColumns:
        style_columns_ = child;
		break;
	}
}
void style_section_properties::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_background_color_attlist_.serialize(CP_GET_XML_NODE());
			common_horizontal_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_writing_mode_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"style:editable",					style_editable_);
			CP_XML_ATTR_OPT(L"style:protect",					style_protect_); // default false
			CP_XML_ATTR_OPT(L"text:dont-balance-text-columns",	text_dont_balance_text_columns_);
		 
			if (style_columns_)			style_columns_->serialize(CP_XML_STREAM());
			if (style_background_image_)style_background_image_->serialize(CP_XML_STREAM());
		}
	}
}

}
}