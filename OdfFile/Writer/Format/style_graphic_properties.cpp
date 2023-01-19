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
#include "style_graphic_properties.h"

#include <xml/attributes.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;

graphic_format_properties::graphic_format_properties()
{
	//draw_stroke_ = line_style::None;
	//style_mirror_ = L"none";
	//common_draw_fill_attlist_.draw_fill_ = draw_fill::none;
}

void graphic_format_properties::apply_from(const graphic_format_properties & Other)
{
	_CP_APPLY_PROP2(draw_stroke_); 
	_CP_APPLY_PROP2(draw_stroke_dash_); 
	_CP_APPLY_PROP2(draw_marker_start_); 
	_CP_APPLY_PROP2(draw_marker_end_); 
	_CP_APPLY_PROP2(draw_textarea_horizontal_align_); 
	_CP_APPLY_PROP2(draw_textarea_vertical_align_); 
	_CP_APPLY_PROP2(draw_auto_grow_height_);
	_CP_APPLY_PROP2(draw_auto_grow_width_);
	_CP_APPLY_PROP2(draw_fit_to_size_);
	_CP_APPLY_PROP2(draw_fit_to_contour_);
    _CP_APPLY_PROP2(draw_wrap_influence_on_position_);
    _CP_APPLY_PROP2(draw_ole_draw_aspect_);

	_CP_APPLY_PROP2(svg_stroke_color_); 
	_CP_APPLY_PROP2(svg_stroke_width_);	
	_CP_APPLY_PROP2(svg_stroke_opacity_); 
	
	_CP_APPLY_PROP2(fo_min_width_);
    _CP_APPLY_PROP2(fo_min_height_);
    _CP_APPLY_PROP2(fo_max_width_);
    _CP_APPLY_PROP2(fo_max_height_);
	_CP_APPLY_PROP2(fo_wrap_option_);

    _CP_APPLY_PROP2(style_print_content_);
    _CP_APPLY_PROP2(style_protect_);
	_CP_APPLY_PROP2(style_editable_);
    _CP_APPLY_PROP2(style_wrap_);
    _CP_APPLY_PROP2(style_wrap_dynamic_treshold_);
    _CP_APPLY_PROP2(style_number_wrapped_paragraphs_);
    _CP_APPLY_PROP2(style_wrap_contour_);
    _CP_APPLY_PROP2(style_wrap_contour_mode_);
    _CP_APPLY_PROP2(style_run_through_); 
    _CP_APPLY_PROP2(style_flow_with_text_);
    _CP_APPLY_PROP2(style_overflow_behavior_);
    _CP_APPLY_PROP2(style_mirror_);
    _CP_APPLY_PROP2(fo_clip_);

    common_draw_fill_attlist_.apply_from(Other.common_draw_fill_attlist_);
    common_draw_rel_size_attlist_.apply_from(Other.common_draw_rel_size_attlist_);
    common_horizontal_margin_attlist_.apply_from(Other.common_horizontal_margin_attlist_);
    common_vertical_margin_attlist_.apply_from(Other.common_vertical_margin_attlist_);
    common_margin_attlist_.apply_from(Other.common_margin_attlist_);    
    common_horizontal_pos_attlist_.apply_from(Other.common_horizontal_pos_attlist_);
    common_horizontal_rel_attlist_.apply_from(Other.common_horizontal_rel_attlist_);
    common_vertical_pos_attlist_.apply_from(Other.common_vertical_pos_attlist_);
    common_vertical_rel_attlist_.apply_from(Other.common_vertical_rel_attlist_);
    common_text_anchor_attlist_.apply_from(Other.common_text_anchor_attlist_);
    common_border_attlist_.apply_from(Other.common_border_attlist_);
    common_border_line_width_attlist_.apply_from(Other.common_border_line_width_attlist_);
    common_padding_attlist_.apply_from(Other.common_padding_attlist_);
    common_shadow_attlist_.apply_from(Other.common_shadow_attlist_);
	common_background_color_attlist_.apply_from(Other.common_background_color_attlist_);
	
	_CP_APPLY_PROP(style_background_image_, Other.style_background_image_);
}

void graphic_format_properties::serialize(std::wostream & _Wostream ,const wchar_t * ns, const wchar_t * name )
{
	if (common_draw_fill_attlist_.draw_fill_gradient_name_ || 
		common_draw_fill_attlist_.draw_fill_image_name_)
	{
		common_draw_fill_attlist_.draw_opacity_ = boost::none;
	}

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_draw_fill_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"draw:wrap-influence-on-position", draw_wrap_influence_on_position_);
			CP_XML_ATTR_OPT(L"draw:textarea-horizontal-align",	draw_textarea_horizontal_align_);
			CP_XML_ATTR_OPT(L"draw:textarea-vertical-align",	draw_textarea_vertical_align_);
			CP_XML_ATTR_OPT(L"draw:auto-grow-height",			draw_auto_grow_height_);
			CP_XML_ATTR_OPT(L"draw:auto-grow-width",			draw_auto_grow_width_);			
			CP_XML_ATTR_OPT(L"draw:fit-to-size",				draw_fit_to_size_);
			CP_XML_ATTR_OPT(L"draw:fit-to-contour",				draw_fit_to_contour_);
			CP_XML_ATTR_OPT(L"draw:ole-draw-aspect",			draw_ole_draw_aspect_);
			CP_XML_ATTR_OPT(L"style:shrink-to-fit",				style_shrink_to_fit_);
			
			CP_XML_ATTR_OPT(L"draw:stroke",				draw_stroke_); 
			CP_XML_ATTR_OPT(L"draw:stroke-dash",		draw_stroke_dash_); 
			CP_XML_ATTR_OPT(L"draw:marker-start",		draw_marker_start_); 
			CP_XML_ATTR_OPT(L"draw:marker-end",			draw_marker_end_); 
			CP_XML_ATTR_OPT(L"draw:marker-start-width",	draw_marker_start_width_);
			CP_XML_ATTR_OPT(L"draw:marker-end-width",	draw_marker_end_width_);
			CP_XML_ATTR_OPT(L"svg:stroke-color",		svg_stroke_color_); 
			CP_XML_ATTR_OPT(L"svg:stroke-width",		svg_stroke_width_);	
			CP_XML_ATTR_OPT(L"svg:stroke-opacity",		svg_stroke_opacity_); 

			CP_XML_ATTR_OPT(L"fo:min-width",			fo_min_width_);
			CP_XML_ATTR_OPT(L"fo:min-height",			fo_min_height_);
			CP_XML_ATTR_OPT(L"fo:max-width",			fo_max_width_);
			CP_XML_ATTR_OPT(L"fo:max-height",			fo_max_height_);
			CP_XML_ATTR_OPT(L"fo:clip",					fo_clip_);
			CP_XML_ATTR_OPT(L"fo:wrap-option",			fo_wrap_option_);			
		   
			CP_XML_ATTR_OPT(L"style:print-content",		style_print_content_);
			CP_XML_ATTR_OPT(L"style:protect",			style_protect_);
 			CP_XML_ATTR_OPT(L"style:editable",			style_editable_);
			CP_XML_ATTR_OPT(L"style:wrap",				style_wrap_);
			CP_XML_ATTR_OPT(L"style:wrap-dynamic-treshold",		style_wrap_dynamic_treshold_);
			CP_XML_ATTR_OPT(L"style:number-wrapped-paragraphs", style_number_wrapped_paragraphs_);
			CP_XML_ATTR_OPT(L"style:wrap-contour",		style_wrap_contour_);
			CP_XML_ATTR_OPT(L"style:wrap-contour-mode", style_wrap_contour_mode_);
			CP_XML_ATTR_OPT(L"style:run-through",		style_run_through_); 
			CP_XML_ATTR_OPT(L"style:flow-with-text",	style_flow_with_text_);
			CP_XML_ATTR_OPT(L"style:overflow-behavior", style_overflow_behavior_);
			CP_XML_ATTR_OPT(L"style:mirror",			style_mirror_);
			
			common_shadow_attlist_.serialize(CP_GET_XML_NODE());
	
			common_draw_rel_size_attlist_.serialize(CP_GET_XML_NODE());
			common_horizontal_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_vertical_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_horizontal_pos_attlist_.serialize(CP_GET_XML_NODE());
			common_horizontal_rel_attlist_.serialize(CP_GET_XML_NODE());    
			common_vertical_pos_attlist_.serialize(CP_GET_XML_NODE());
			common_vertical_rel_attlist_.serialize(CP_GET_XML_NODE());
			common_text_anchor_attlist_.serialize(CP_GET_XML_NODE());
			common_border_attlist_.serialize(CP_GET_XML_NODE());
			common_border_line_width_attlist_.serialize(CP_GET_XML_NODE());
			common_padding_attlist_.serialize(CP_GET_XML_NODE());
			common_background_color_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

//------------------------------------
const wchar_t * style_graphic_properties::ns = L"style";
const wchar_t * style_graphic_properties::name = L"graphic-properties";

void style_graphic_properties::serialize(std::wostream & strm)
{
	content_.serialize(strm,ns,name);
}

//------------------------------------
const wchar_t * loext_graphic_properties::ns = L"loext";
const wchar_t * loext_graphic_properties::name = L"graphic-properties";

void loext_graphic_properties::serialize(std::wostream & strm)
{
	content_.serialize(strm,ns,name);
}

}
}
