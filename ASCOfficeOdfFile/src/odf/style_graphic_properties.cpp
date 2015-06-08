
#include <iostream>
#include "style_graphic_properties.h"

#include <cpdoccore/xml/attributes.h>
#include "serialize_elements.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


void graphic_format_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_draw_fill_attlist_.add_attributes(Attributes);

	CP_APPLY_ATTR(L"draw:wrap-influence-on-position", draw_wrap_influence_on_position_);
	CP_APPLY_ATTR(L"draw:textarea-horizontal-align", draw_textarea_horizontal_align_);
	CP_APPLY_ATTR(L"draw:textarea-vertical-align", draw_textarea_vertical_align_);
	
	CP_APPLY_ATTR(L"draw:stroke",		draw_stroke_); 
	CP_APPLY_ATTR(L"draw:stroke-dash",	draw_stroke_dash_); 
	CP_APPLY_ATTR(L"draw:marker-start",	draw_marker_start_); 
	CP_APPLY_ATTR(L"draw:marker-end",	draw_marker_end_); 
	CP_APPLY_ATTR(L"svg:stroke-color",	svg_stroke_color_); 
	CP_APPLY_ATTR(L"svg:stroke-width",	svg_stroke_width_);	
	CP_APPLY_ATTR(L"svg:stroke-opacity",svg_stroke_opacity_); 

	CP_APPLY_ATTR(L"fo:min-width",	fo_min_width_);
    CP_APPLY_ATTR(L"fo:min-height", fo_min_height_);
    CP_APPLY_ATTR(L"fo:max-width",	fo_max_width_);
    CP_APPLY_ATTR(L"fo:max-height", fo_max_height_);
    CP_APPLY_ATTR(L"fo:clip",		fo_clip_);
   
	CP_APPLY_ATTR(L"style:print-content", style_print_content_);
    CP_APPLY_ATTR(L"style:protect", style_protect_);
 	CP_APPLY_ATTR(L"style:editable", style_editable_);
    CP_APPLY_ATTR(L"style:wrap", style_wrap_);
    CP_APPLY_ATTR(L"style:wrap-dynamic-treshold", style_wrap_dynamic_treshold_);
    CP_APPLY_ATTR(L"style:number-wrapped-paragraphs", style_number_wrapped_paragraphs_);
    CP_APPLY_ATTR(L"style:wrap-contour", style_wrap_contour_);
    CP_APPLY_ATTR(L"style:wrap-contour-mode", style_wrap_contour_mode_);
    CP_APPLY_ATTR(L"style:run-through", style_run_through_); 
    CP_APPLY_ATTR(L"style:flow-with-text", style_flow_with_text_);
    CP_APPLY_ATTR(L"style:overflow-behavior", style_overflow_behavior_);
    CP_APPLY_ATTR(L"style:mirror", style_mirror_);
	
	common_draw_rel_size_attlist_.add_attributes(Attributes);
	common_horizontal_margin_attlist_.add_attributes(Attributes);
    common_vertical_margin_attlist_.add_attributes(Attributes);
    common_margin_attlist_.add_attributes(Attributes);
    common_horizontal_pos_attlist_.add_attributes(Attributes);
    common_horizontal_rel_attlist_.add_attributes(Attributes);    
    common_vertical_pos_attlist_.add_attributes(Attributes);
    common_vertical_rel_attlist_.add_attributes(Attributes);
    common_text_anchor_attlist_.add_attributes(Attributes);
    common_border_attlist_.add_attributes(Attributes);
    common_border_line_width_attlist_.add_attributes(Attributes);
    common_padding_attlist_.add_attributes(Attributes);
    common_shadow_attlist_.add_attributes(Attributes);
    common_background_color_attlist_.add_attributes(Attributes);
    
}

void graphic_format_properties::apply_to(std::vector<_property> & properties)
{
	if (common_draw_fill_attlist_.draw_fill_color_)	properties.push_back(_property(L"fill-color",	common_draw_fill_attlist_.draw_fill_color_->get_hex_value() ));

	if (draw_stroke_)		properties.push_back(_property(L"stroke",		draw_stroke_->get_type() ));
	if (svg_stroke_color_)	properties.push_back(_property(L"stroke-color",	svg_stroke_color_->get_hex_value() ));
	if (draw_stroke_dash_)	properties.push_back(_property(L"stroke-dash",	draw_stroke_dash_.get()));
	if (svg_stroke_width_)	properties.push_back(_property(L"stroke-width",	svg_stroke_width_->get_length().get_value_unit(odf_types::length::pt) ));
	if (svg_stroke_opacity_)properties.push_back(_property(L"stroke-opacity",svg_stroke_opacity_->get_percent().get_value()));	
	if (draw_marker_start_)	properties.push_back(_property(L"marker-start",	draw_marker_start_->get() ));
	if (draw_marker_end_)	properties.push_back(_property(L"marker-end",	draw_marker_end_->get() ));
	
	if (draw_textarea_horizontal_align_)properties.push_back(_property(L"textalign-horizontal",	draw_textarea_horizontal_align_->get_type() ));
	if (draw_textarea_vertical_align_)properties.push_back(_property(L"textalign-vertical",	draw_textarea_vertical_align_->get_type() ));

}
void graphic_format_properties::apply_from(const graphic_format_properties & Other)
{
	_CP_APPLY_PROP2(draw_stroke_); 
	_CP_APPLY_PROP2(draw_stroke_dash_); 
	_CP_APPLY_PROP2(draw_marker_start_); 
	_CP_APPLY_PROP2(draw_marker_end_); 
	_CP_APPLY_PROP2(draw_textarea_horizontal_align_); 
	_CP_APPLY_PROP2(draw_textarea_vertical_align_); 
	
	_CP_APPLY_PROP2(svg_stroke_color_); 
	_CP_APPLY_PROP2(svg_stroke_width_);	
	_CP_APPLY_PROP2(svg_stroke_opacity_); 
	
	_CP_APPLY_PROP2(fo_min_width_);
    _CP_APPLY_PROP2(fo_min_height_);
    _CP_APPLY_PROP2(fo_max_width_);
    _CP_APPLY_PROP2(fo_max_height_);

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
    _CP_APPLY_PROP2(draw_wrap_influence_on_position_);

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
     
}


// style:graphic-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_graphic_properties::ns = L"style";
const wchar_t * style_graphic_properties::name = L"graphic-properties";

void style_graphic_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	graphic_format_properties_.add_attributes(Attributes);
}

void style_graphic_properties::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
   // CP_NOT_APPLICABLE_ELM();

	//if (CP_CHECK_NAME(L"text", L"list-style") 	
	//	styles_.add_child_element(Reader, Ns, Name, getContext()); он тут и не нужен по сути... описание есть и в другом сместе
}

}
}
