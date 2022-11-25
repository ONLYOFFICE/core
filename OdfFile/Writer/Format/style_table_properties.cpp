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

#include <iostream>
#include "style_table_properties.h"
#include "../../DataTypes/borderstyle.h"

#include <xml/simple_xml_writer.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;

bool table_format_properties::create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_background_image_);
        return true;
    }
    else
    {
        not_applicable_element(L"style-table-properties", Ns, Name);
        return false;
    }
}

void table_format_properties::serialize(std::wostream & _Wostream,const wchar_t * ns, const wchar_t * name)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:width", style_width_);
			CP_XML_ATTR_OPT(L"style:rel-width", style_rel_width_);
			CP_XML_ATTR_OPT(L"style:may-break-between-rows", style_may_break_between_rows_);
			CP_XML_ATTR_OPT(L"table:border-model", table_border_model_);
			CP_XML_ATTR_OPT(L"table:display", table_display_);
			CP_XML_ATTR_OPT(L"tableooo:tab-color", tableooo_tab_color_);			

			common_writing_mode_attlist_.serialize(CP_GET_XML_NODE());
			common_horizontal_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_vertical_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_page_number_attlist_.serialize(CP_GET_XML_NODE());
			common_break_attlist_.serialize(CP_GET_XML_NODE());
			common_background_color_attlist_.serialize(CP_GET_XML_NODE());
			common_shadow_attlist_.serialize(CP_GET_XML_NODE());
			common_keep_with_next_attlist_.serialize(CP_GET_XML_NODE());
			common_border_attlist_.serialize(CP_GET_XML_NODE());
			
			CP_XML_ATTR_OPT(L"table:align", table_align_);

//not from specification !! 			
			CP_XML_ATTR_OPT(L"style:use-optimal-column-width",	style_use_optimal_column_width_);
//not from specification !! 			
			
			if (style_background_image_) style_background_image_->serialize(_Wostream);
		}
	}
}
void table_format_properties::apply_from(const table_format_properties & Other)
{
	_CP_APPLY_PROP(style_width_					, Other.style_width_);
	_CP_APPLY_PROP(style_rel_width_				, Other.style_rel_width_);
	_CP_APPLY_PROP(style_may_break_between_rows_, Other.style_may_break_between_rows_);
	_CP_APPLY_PROP(table_border_model_			, Other.table_border_model_);
	_CP_APPLY_PROP(table_display_				, Other.table_display_);
	_CP_APPLY_PROP(tableooo_tab_color_			, Other.tableooo_tab_color_);			
	_CP_APPLY_PROP(table_align_					, Other.table_align_);

	common_writing_mode_attlist_.apply_from(Other.common_writing_mode_attlist_);
	common_horizontal_margin_attlist_.apply_from(Other.common_horizontal_margin_attlist_);
	common_vertical_margin_attlist_.apply_from(Other.common_vertical_margin_attlist_);
	common_margin_attlist_.apply_from(Other.common_margin_attlist_);
	common_page_number_attlist_.apply_from(Other.common_page_number_attlist_);
	common_break_attlist_.apply_from(Other.common_break_attlist_);
	common_background_color_attlist_.apply_from(Other.common_background_color_attlist_);
	common_shadow_attlist_.apply_from(Other.common_shadow_attlist_);
	common_keep_with_next_attlist_.apply_from(Other.common_keep_with_next_attlist_);
	common_border_attlist_.apply_from(Other.common_border_attlist_);
	
	style_background_image_			= Other.style_background_image_;
    style_use_optimal_column_width_ = Other.style_use_optimal_column_width_;
}

// style:table-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_properties::ns = L"style";
const wchar_t * style_table_properties::name = L"table-properties";


void style_table_properties::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    content_.create_child_element(Ns, Name, getContext());
}

void style_table_properties::serialize(std::wostream & _Wostream)
{
	content_.serialize(_Wostream,ns,name);

}
   
void style_table_properties::apply_from(const style_table_properties * Other)
{
	if (Other == NULL) return;

	content_.apply_from(Other->content_);
}


// style-table-column-properties-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////
void style_table_column_properties_attlist::serialize(std::wostream & _Wostream,const wchar_t * ns, const wchar_t * name )
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:column-width",				style_column_width_);
			CP_XML_ATTR_OPT(L"style:rel-column-width",			style_rel_column_width_);
			CP_XML_ATTR_OPT(L"style:use-optimal-column-width",	style_use_optimal_column_width_);
			
			common_break_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
// style:table-column-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_column_properties::ns = L"style";
const wchar_t * style_table_column_properties::name = L"table-column-properties";

void style_table_column_properties::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void style_table_column_properties::serialize(std::wostream & strm)
{
    style_table_column_properties_attlist_.serialize(strm,ns,name);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

/// style:table-cell-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_cell_properties::ns = L"style";
const wchar_t * style_table_cell_properties::name = L"table-cell-properties";

void style_table_cell_properties::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);        
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();        
    }
}
void style_table_cell_properties::apply_from(const style_table_cell_properties * Other)
{
	if (Other == NULL)return;

	content_.apply_from(Other->content_);

}
void style_table_cell_properties::serialize(std::wostream & strm)
{
	content_.serialize(strm,ns,name);
	if (style_background_image_)style_background_image_->serialize(strm);
}

void style_table_cell_properties_attlist::serialize(std::wostream & _Wostream ,const wchar_t * ns, const wchar_t * name )
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:vertical-align", style_vertical_align_);
			CP_XML_ATTR_OPT(L"style:text-align-source", style_text_align_source_);
			CP_XML_ATTR_OPT(L"style:direction", style_direction_);
			CP_XML_ATTR_OPT(L"style:glyph-orientation-vertical", style_glyph_orientation_vertical_);
		    
			common_shadow_attlist_.serialize(CP_GET_XML_NODE());
			common_background_color_attlist_.serialize(CP_GET_XML_NODE());
			common_border_attlist_.serialize(CP_GET_XML_NODE());
			common_border_line_width_attlist_.serialize(CP_GET_XML_NODE());
		    
			CP_XML_ATTR_OPT(L"style:diagonal-tl-br", style_diagonal_tl_br_);
			CP_XML_ATTR_OPT(L"style:diagonal-tl-br-widths", style_diagonal_tl_br_widths_);
			CP_XML_ATTR_OPT(L"style:diagonal-bl-tr", style_diagonal_bl_tr_);
			CP_XML_ATTR_OPT(L"style:diagonal-bl-tr-widths", style_diagonal_bl_tr_widths_);
			common_padding_attlist_.serialize(CP_GET_XML_NODE());
			common_rotation_angle_attlist_.serialize(CP_GET_XML_NODE());
		   
			CP_XML_ATTR_OPT(L"fo:wrap-option", fo_wrap_option_);
			CP_XML_ATTR_OPT(L"style:rotation-align", style_rotation_align_);    
			CP_XML_ATTR_OPT(L"style:cell-protect", style_cell_protect_);
			CP_XML_ATTR_OPT(L"style:print-content", style_print_content_);
			CP_XML_ATTR_OPT(L"style:decimal-places", style_decimal_places_);
			CP_XML_ATTR_OPT(L"style:repeat-content", style_repeat_content_);
			CP_XML_ATTR_OPT(L"style:shrink-to-fit", style_shrink_to_fit_);
		}
	}
}
void style_table_cell_properties_attlist::apply_from(const style_table_cell_properties_attlist & Other)
{
     _CP_APPLY_PROP(style_vertical_align_, Other.style_vertical_align_);
     _CP_APPLY_PROP(style_text_align_source_, Other.style_text_align_source_);
     _CP_APPLY_PROP(style_direction_, Other.style_direction_);
     _CP_APPLY_PROP(style_glyph_orientation_vertical_, Other.style_glyph_orientation_vertical_);
    
    common_shadow_attlist_.apply_from(Other.common_shadow_attlist_);
    common_background_color_attlist_.apply_from(Other.common_background_color_attlist_);
    common_border_attlist_.apply_from(Other.common_border_attlist_);

    _CP_APPLY_PROP(style_diagonal_tl_br_, Other.style_diagonal_tl_br_);
    _CP_APPLY_PROP(style_diagonal_tl_br_widths_, Other.style_diagonal_tl_br_widths_);
    _CP_APPLY_PROP(style_diagonal_bl_tr_, Other.style_diagonal_bl_tr_);
    _CP_APPLY_PROP(style_diagonal_bl_tr_widths_, Other.style_diagonal_bl_tr_widths_);

    common_border_line_width_attlist_.apply_from(Other.common_border_line_width_attlist_);
    common_padding_attlist_.apply_from(Other.common_padding_attlist_);
    _CP_APPLY_PROP(fo_wrap_option_, Other.fo_wrap_option_);
    common_rotation_angle_attlist_.apply_from(Other.common_rotation_angle_attlist_);
    
    _CP_APPLY_PROP(style_rotation_align_, Other.style_rotation_align_);    
    _CP_APPLY_PROP(style_cell_protect_, Other.style_cell_protect_);
    _CP_APPLY_PROP(style_print_content_, Other.style_print_content_);
    _CP_APPLY_PROP(style_decimal_places_, Other.style_decimal_places_);
    _CP_APPLY_PROP(style_repeat_content_, Other.style_repeat_content_);
    _CP_APPLY_PROP(style_shrink_to_fit_, Other.style_shrink_to_fit_);    
}
// style-table-row-properties-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////
void style_table_row_properties_attlist::serialize(std::wostream & _Wostream ,const wchar_t * ns, const wchar_t * name )
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:row-height", style_row_height_);
			CP_XML_ATTR_OPT(L"style:min-row-height", style_min_row_height_);
			CP_XML_ATTR_OPT(L"style:use-optimal-row-height", style_use_optimal_row_height_);
			common_background_color_attlist_.serialize(CP_GET_XML_NODE());
			common_break_attlist_.serialize(CP_GET_XML_NODE());
			CP_XML_ATTR_OPT(L"fo:keep-together", fo_keep_together_);     
		}
	}
}
//  style:table-row-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_row_properties::ns = L"style";
const wchar_t * style_table_row_properties::name = L"table-row-properties";


void style_table_row_properties::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);        
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();        
    }
}
void style_table_row_properties::serialize(std::wostream & strm)
{
	style_table_row_properties_attlist_.serialize(strm,ns,name);
	if (style_background_image_)style_background_image_->serialize(strm);
}
/////

}
}
