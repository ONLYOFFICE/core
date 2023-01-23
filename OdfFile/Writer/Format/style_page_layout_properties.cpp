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

#include "style_page_layout_properties.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {
    
//////////////////////////////////////////////////////////////////////////////////////////////////
void style_page_layout_properties_attlist::serialize(CP_ATTR_NODE)
{
	if (offset_page_border_ == 2)
	{
		//from page to from text
		if (common_padding_attlist_.fo_padding_)
		{
			if (common_margin_attlist_.fo_margin_)
			{
				length new_margin	= *common_padding_attlist_.fo_padding_ ;
				length new_padding	= common_margin_attlist_.fo_margin_->get_length() - *common_padding_attlist_.fo_padding_;
				
				common_margin_attlist_.fo_margin_	= new_margin;
				common_padding_attlist_.fo_padding_	= new_padding;
			}
			else
			{
				common_padding_attlist_.fo_padding_top_ = common_padding_attlist_.fo_padding_bottom_ = 
					common_padding_attlist_.fo_padding_left_ = common_padding_attlist_.fo_padding_right_ = common_padding_attlist_.fo_padding_;
				
				common_padding_attlist_.fo_padding_ = boost::none;
			}
		}

		if (common_padding_attlist_.fo_padding_top_  && (common_vertical_margin_attlist_.fo_margin_top_ || common_margin_attlist_.fo_margin_))
		{
			if (!common_vertical_margin_attlist_.fo_margin_top_ )
				common_vertical_margin_attlist_.fo_margin_top_ = common_margin_attlist_.fo_margin_;

			length new_margin	= *common_padding_attlist_.fo_padding_top_ ;
			length new_padding	= common_vertical_margin_attlist_.fo_margin_top_->get_length() - *common_padding_attlist_.fo_padding_top_ ;
			
			common_vertical_margin_attlist_.fo_margin_top_	= new_margin;
			common_padding_attlist_.fo_padding_top_			= new_padding;
		}

		if (common_padding_attlist_.fo_padding_bottom_  && (common_vertical_margin_attlist_.fo_margin_bottom_ || common_margin_attlist_.fo_margin_))
		{
			if (!common_vertical_margin_attlist_.fo_margin_bottom_ )
				common_vertical_margin_attlist_.fo_margin_bottom_ = common_margin_attlist_.fo_margin_;

			length new_margin	= *common_padding_attlist_.fo_padding_bottom_ ;
			length new_padding	= common_vertical_margin_attlist_.fo_margin_bottom_->get_length() - *common_padding_attlist_.fo_padding_bottom_ ;
			
			common_vertical_margin_attlist_.fo_margin_bottom_	= new_margin;
			common_padding_attlist_.fo_padding_bottom_			= new_padding;
		}
		
		if (common_padding_attlist_.fo_padding_left_  && (common_horizontal_margin_attlist_.fo_margin_left_ || common_margin_attlist_.fo_margin_))
		{
			if (!common_horizontal_margin_attlist_.fo_margin_left_)
				common_horizontal_margin_attlist_.fo_margin_left_ = common_margin_attlist_.fo_margin_;

			length new_margin	= *common_padding_attlist_.fo_padding_left_ ;
			length new_padding	= common_horizontal_margin_attlist_.fo_margin_left_->get_length() - *common_padding_attlist_.fo_padding_top_ ;
			
			common_horizontal_margin_attlist_.fo_margin_left_	= new_margin;
			common_padding_attlist_.fo_padding_left_			= new_padding;
		}	
		
		if (common_padding_attlist_.fo_padding_right_  && (common_horizontal_margin_attlist_.fo_margin_right_ || common_margin_attlist_.fo_margin_))
		{
			if (!common_horizontal_margin_attlist_.fo_margin_right_)
				common_horizontal_margin_attlist_.fo_margin_right_ = common_margin_attlist_.fo_margin_;

			length new_margin	= *common_padding_attlist_.fo_padding_right_ ;
			length new_padding	= common_horizontal_margin_attlist_.fo_margin_right_->get_length() - *common_padding_attlist_.fo_padding_right_ ;
			
			common_horizontal_margin_attlist_.fo_margin_right_	= new_margin;
			common_padding_attlist_.fo_padding_right_			= new_padding;
		}
	}
    
	CP_XML_ATTR_OPT	(L"fo:page-width",		fo_page_width_);
    CP_XML_ATTR_OPT	(L"fo:page-height",		fo_page_height_);
    
	common_num_format_attlist_.serialize				(CP_GET_XML_NODE());
    common_num_format_prefix_suffix_attlist_.serialize	(CP_GET_XML_NODE());
    
	CP_XML_ATTR_OPT	(L"style:paper-tray-name",		style_paper_tray_name_);
    CP_XML_ATTR_OPT	(L"style:print-orientation",	style_print_orientation_);
    
    common_vertical_margin_attlist_.serialize	(CP_GET_XML_NODE());
	common_horizontal_margin_attlist_.serialize	(CP_GET_XML_NODE());
    common_margin_attlist_.serialize			(CP_GET_XML_NODE());
    common_border_attlist_.serialize			(CP_GET_XML_NODE());
    common_border_line_width_attlist_.serialize	(CP_GET_XML_NODE());
    common_padding_attlist_.serialize			(CP_GET_XML_NODE());
    common_shadow_attlist_.serialize			(CP_GET_XML_NODE());
    common_background_color_attlist_.serialize	(CP_GET_XML_NODE());
	common_draw_fill_attlist_.serialize			(CP_GET_XML_NODE());
	common_page_number_attlist_.serialize		(CP_GET_XML_NODE());
    
	CP_XML_ATTR_OPT	(L"style:register-truth-ref-style-name",	style_register_truth_ref_style_name_);
    CP_XML_ATTR_OPT	(L"style:print",							style_print_);
    CP_XML_ATTR_OPT	(L"style:print-page-order",					style_print_page_order_);
    CP_XML_ATTR_OPT	(L"style:scale-to",							style_scale_to_);
    CP_XML_ATTR_OPT	(L"style:scale-to_pages",					style_scale_to_pages_);
    CP_XML_ATTR_OPT	(L"style:table-centering",					style_table_centering_);
    CP_XML_ATTR_OPT	(L"style:footnote-max-height",				style_footnote_max_height_);
	CP_XML_ATTR_OPT	(L"loext:scale-to-X",						loext_scale_to_X_);
	CP_XML_ATTR_OPT	(L"loext:scale-to-Y",						loext_scale_to_Y_);

	common_writing_mode_attlist_.serialize(CP_GET_XML_NODE());
    
	CP_XML_ATTR_OPT	(L"style:layout-grid-mode",			style_layout_grid_mode_);
    CP_XML_ATTR_OPT	(L"style:layout-grid-base-height",	style_layout_grid_base_height_);
    CP_XML_ATTR_OPT	(L"style:layout-grid-ruby-height",	style_layout_grid_ruby_height_);
    CP_XML_ATTR_OPT	(L"style:layout-grid-lines",		style_layout_grid_lines_);
    CP_XML_ATTR_OPT	(L"style:layout-grid-color",		style_layout_grid_color_);
    CP_XML_ATTR_OPT	(L"style:layout-grid-ruby-below",	style_layout_grid_ruby_below_);  
    CP_XML_ATTR_OPT	(L"style:layout-grid-print",		style_layout_grid_print_);
    CP_XML_ATTR_OPT	(L"style:layout-grid-display",		style_layout_grid_display_);
}
const wchar_t * style_page_layout_properties::ns	= L"style";
const wchar_t * style_page_layout_properties::name	= L"page-layout-properties";

void style_page_layout_properties::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    elements_.create_child_element( Ns, Name, getContext());
}

void style_page_layout_properties::add_child_element( const office_element_ptr & child)
{
	elements_.add_child_element(child);
}

void style_page_layout_properties::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize( CP_GET_XML_NODE());
			elements_.serialize(CP_XML_STREAM());
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void style_page_layout_properties_elements::create_child_element( const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context )
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_background_image_);            
    }
    else if (L"style" == Ns && L"columns" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_columns_);            
    }
    else if (L"style" == Ns && L"footnote-sep" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_footnote_sep_);            
    }
    else
    {
        not_applicable_element(L"style-page-layout-properties-elements", Ns, Name);
    }
}
void style_page_layout_properties_elements::add_child_element( const office_element_ptr & child )
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
	case typeStyleFootnoteSep:
        style_footnote_sep_ = child;   
		break;
    }
}
void style_page_layout_properties_elements::serialize(std::wostream & strm)
{
    if (style_background_image_)	
		style_background_image_->serialize(strm);

    if (style_columns_)				
		style_columns_->serialize(strm);

    if (style_footnote_sep_)		
		style_footnote_sep_->serialize(strm);
}

}
}