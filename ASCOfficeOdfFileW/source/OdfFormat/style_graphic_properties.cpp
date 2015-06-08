
#include <iostream>
#include "style_graphic_properties.h"

#include <cpdoccore/xml/attributes.h>

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
void graphic_format_properties::serialize(std::wostream & _Wostream ,const wchar_t * ns, const wchar_t * name )
{
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
			
			CP_XML_ATTR_OPT(L"draw:stroke",			draw_stroke_); 
			CP_XML_ATTR_OPT(L"draw:stroke-dash",	draw_stroke_dash_); 
			CP_XML_ATTR_OPT(L"draw:marker-start",	draw_marker_start_); 
			CP_XML_ATTR_OPT(L"draw:marker-end",		draw_marker_end_); 
			CP_XML_ATTR_OPT(L"draw:marker-start-width",		draw_marker_start_width_);
			CP_XML_ATTR_OPT(L"draw:marker-end-width",		draw_marker_end_width_);
			CP_XML_ATTR_OPT(L"svg:stroke-color",	svg_stroke_color_); 
			CP_XML_ATTR_OPT(L"svg:stroke-width",	svg_stroke_width_);	
			CP_XML_ATTR_OPT(L"svg:stroke-opacity",	svg_stroke_opacity_); 

			CP_XML_ATTR_OPT(L"fo:min-width",	fo_min_width_);
			CP_XML_ATTR_OPT(L"fo:min-height",	fo_min_height_);
			CP_XML_ATTR_OPT(L"fo:max-width",	fo_max_width_);
			CP_XML_ATTR_OPT(L"fo:max-height",	fo_max_height_);
			CP_XML_ATTR_OPT(L"fo:clip",			fo_clip_);
			CP_XML_ATTR_OPT(L"fo:wrap-option",	fo_wrap_option_);			
		   
			CP_XML_ATTR_OPT(L"style:print-content",		style_print_content_);
			CP_XML_ATTR_OPT(L"style:protect",			style_protect_);
 			CP_XML_ATTR_OPT(L"style:editable",			style_editable_);
			CP_XML_ATTR_OPT(L"style:wrap",				style_wrap_);
			CP_XML_ATTR_OPT(L"style:wrap-dynamic-treshold", style_wrap_dynamic_treshold_);
			CP_XML_ATTR_OPT(L"style:number-wrapped-paragraphs", style_number_wrapped_paragraphs_);
			CP_XML_ATTR_OPT(L"style:wrap-contour",		style_wrap_contour_);
			CP_XML_ATTR_OPT(L"style:wrap-contour-mode", style_wrap_contour_mode_);
			CP_XML_ATTR_OPT(L"style:run-through",		style_run_through_); 
			CP_XML_ATTR_OPT(L"style:flow-with-text",	style_flow_with_text_);
			CP_XML_ATTR_OPT(L"style:overflow-behavior", style_overflow_behavior_);
			CP_XML_ATTR_OPT(L"style:mirror",			style_mirror_);
			CP_XML_ATTR_OPT(L"draw:fit-to-size", draw_fit_to_size_);
			
			//common_shadow_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"draw:shadow",				draw_shadow_);
			CP_XML_ATTR_OPT(L"draw:shadow-opacity",		draw_shadow_opacity_);
			CP_XML_ATTR_OPT(L"draw:shadow-color",		draw_shadow_color_);
			CP_XML_ATTR_OPT(L"draw:shadow-offset-y",	draw_shadow_offset_y_);
			CP_XML_ATTR_OPT(L"draw:shadow-offset-x",	draw_shadow_offset_x_);
	
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
			common_shadow_attlist_.serialize(CP_GET_XML_NODE());
			common_padding_attlist_.serialize(CP_GET_XML_NODE());
			common_background_color_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

// style:graphic-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_graphic_properties::ns = L"style";
const wchar_t * style_graphic_properties::name = L"graphic-properties";

void style_graphic_properties::serialize(std::wostream & strm)
{
	graphic_format_properties_.serialize(strm,ns,name);
}


}
}
