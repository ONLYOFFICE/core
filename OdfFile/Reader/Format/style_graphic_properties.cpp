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

#include "serialize_elements.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


void graphic_format_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_draw_fill_attlist_.add_attributes(Attributes);

	CP_APPLY_ATTR(L"draw:wrap-influence-on-position",	draw_wrap_influence_on_position_);
	CP_APPLY_ATTR(L"draw:textarea-horizontal-align",	draw_textarea_horizontal_align_);
	CP_APPLY_ATTR(L"draw:textarea-vertical-align",		draw_textarea_vertical_align_);
	CP_APPLY_ATTR(L"draw:auto-grow-height",				draw_auto_grow_height_);
	CP_APPLY_ATTR(L"draw:auto-grow-width",				draw_auto_grow_width_);
	CP_APPLY_ATTR(L"draw:fit-to-size",					draw_fit_to_size_);
	CP_APPLY_ATTR(L"draw:fit-to-contour",				draw_fit_to_contour_);
	CP_APPLY_ATTR(L"style:shrink-to-fit",				style_shrink_to_fit_);
	CP_APPLY_ATTR(L"draw:fit-to-size",					draw_fit_to_size_str_);

	CP_APPLY_ATTR(L"draw:stroke",						draw_stroke_); 
	CP_APPLY_ATTR(L"draw:stroke-dash",					draw_stroke_dash_); 
	CP_APPLY_ATTR(L"draw:marker-start",					draw_marker_start_); 
	CP_APPLY_ATTR(L"draw:marker-end",					draw_marker_end_); 

	CP_APPLY_ATTR(L"svg:stroke-color",					svg_stroke_color_); 
	CP_APPLY_ATTR(L"svg:stroke-width",					svg_stroke_width_);	
	CP_APPLY_ATTR(L"svg:stroke-opacity",				svg_stroke_opacity_); 

	CP_APPLY_ATTR(L"fo:min-width",						fo_min_width_);
    CP_APPLY_ATTR(L"fo:min-height",						fo_min_height_);
    CP_APPLY_ATTR(L"fo:max-width",						fo_max_width_);
    CP_APPLY_ATTR(L"fo:max-height",						fo_max_height_);
    CP_APPLY_ATTR(L"fo:clip",							fo_clip_);
	 CP_APPLY_ATTR(L"fo:wrap-option",					fo_wrap_option_);
   
	CP_APPLY_ATTR(L"style:print-content",				style_print_content_);
    CP_APPLY_ATTR(L"style:protect",						style_protect_);
 	CP_APPLY_ATTR(L"style:editable",					style_editable_);
    CP_APPLY_ATTR(L"style:wrap",						style_wrap_);
    CP_APPLY_ATTR(L"style:wrap-dynamic-treshold",		style_wrap_dynamic_treshold_);
    CP_APPLY_ATTR(L"style:number-wrapped-paragraphs",	style_number_wrapped_paragraphs_);
    CP_APPLY_ATTR(L"style:wrap-contour",				style_wrap_contour_);
    CP_APPLY_ATTR(L"style:wrap-contour-mode",			style_wrap_contour_mode_);
    CP_APPLY_ATTR(L"style:run-through",					style_run_through_); 
    CP_APPLY_ATTR(L"style:flow-with-text",				style_flow_with_text_);
    CP_APPLY_ATTR(L"style:overflow-behavior",			style_overflow_behavior_);
    CP_APPLY_ATTR(L"style:mirror",						style_mirror_);
	
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

	if (draw_fit_to_size_str_ && !style_shrink_to_fit_)
	{//https://bugs.documentfoundation.org/show_bug.cgi?id=97630
		if (*draw_fit_to_size_str_ == L"shrink-to-fit")
			style_shrink_to_fit_ = true;
	}    
}

void graphic_format_properties::apply_to(std::vector<_property> & properties)
{
	if (common_draw_fill_attlist_.draw_fill_color_)	properties.push_back(_property(L"fill-color",	common_draw_fill_attlist_.draw_fill_color_->get_hex_value() ));

	if (draw_stroke_)		properties.push_back(_property(L"stroke",		draw_stroke_->get_type() ));
	if (svg_stroke_color_)	properties.push_back(_property(L"stroke-color",	svg_stroke_color_->get_hex_value() ));
	if (draw_stroke_dash_)
	{
		if (!draw_stroke_dash_->empty())
			properties.push_back(_property(L"stroke-dash",	draw_stroke_dash_.get()));
	}
	if (svg_stroke_width_)	properties.push_back(_property(L"stroke-width",	svg_stroke_width_->get_length().get_value_unit(odf_types::length::pt) ));
	if (svg_stroke_opacity_)properties.push_back(_property(L"stroke-opacity",svg_stroke_opacity_->get_percent().get_value()));	
	if (draw_marker_start_)
	{
		properties.push_back(_property(L"marker-start",	draw_marker_start_->get() ));
	}
	if (draw_marker_end_)
	{
		//const std::wstring style_name = draw_marker_end_->get();
		//if (!style_name.empty())
		////if (office_element_ptr style = styles.find_by_style_name(style_name))
		//{
			properties.push_back(_property(L"marker-end",	draw_marker_end_->get() ));
		//}
	}
	
	if (draw_textarea_horizontal_align_)properties.push_back(_property(L"textarea-horizontal_align",draw_textarea_horizontal_align_->get_type() ));
	if (draw_textarea_vertical_align_)	properties.push_back(_property(L"textarea-vertical_align",	draw_textarea_vertical_align_->get_type() ));

	if (draw_auto_grow_height_)	properties.push_back(_property(L"auto-grow-height", *draw_auto_grow_height_));
	if (draw_auto_grow_width_)	properties.push_back(_property(L"auto-grow-width",	*draw_auto_grow_width_));
	if (draw_fit_to_contour_)	properties.push_back(_property(L"fit-to-contour",	*draw_fit_to_contour_));
	
	if (style_shrink_to_fit_)   properties.push_back(_property(L"fit-to-size",		true));
	else if (draw_fit_to_size_)	properties.push_back(_property(L"fit-to-size",		*draw_fit_to_size_));

	if (common_draw_fill_attlist_.draw_color_mode_)
	{
		properties.push_back(_property(L"color-mode", *common_draw_fill_attlist_.draw_color_mode_));
	}
	if (common_draw_fill_attlist_.draw_luminance_)
	{
		properties.push_back(_property(L"luminance", common_draw_fill_attlist_.draw_luminance_->get_value()));
	}
	if (common_draw_fill_attlist_.draw_contrast_)
	{
		properties.push_back(_property(L"contrast", common_draw_fill_attlist_.draw_contrast_->get_value()));
	}
	if (common_shadow_attlist_.draw_shadow_.get_value_or(shadow_type1::Hidden).get_type() == shadow_type1::Visible)
	{
		properties.push_back(_property(L"shadow", true));
		if (common_shadow_attlist_.draw_shadow_color_)	properties.push_back(_property(L"shadow-color",		common_shadow_attlist_.draw_shadow_color_->get_hex_value()));
		if (common_shadow_attlist_.draw_shadow_opacity_) properties.push_back(_property(L"shadow-opacity",	common_shadow_attlist_.draw_shadow_opacity_->get_value()));
		if (common_shadow_attlist_.draw_shadow_offset_x_) properties.push_back(_property(L"shadow-offset-x",	common_shadow_attlist_.draw_shadow_offset_x_->get_value_unit(length::emu)));
		if (common_shadow_attlist_.draw_shadow_offset_y_) properties.push_back(_property(L"shadow-offset-y",	common_shadow_attlist_.draw_shadow_offset_y_->get_value_unit(length::emu)));
	}
	if (common_padding_attlist_.fo_padding_)
	{
		properties.push_back(_property(L"text-padding-left",	common_padding_attlist_.fo_padding_->get_value_unit(length::emu)));
		properties.push_back(_property(L"text-padding-right",	common_padding_attlist_.fo_padding_->get_value_unit(length::emu)));
		properties.push_back(_property(L"text-padding-top",		common_padding_attlist_.fo_padding_->get_value_unit(length::emu)));
		properties.push_back(_property(L"text-padding-bottom",	common_padding_attlist_.fo_padding_->get_value_unit(length::emu)));
	}
	else
	{
		if (common_padding_attlist_.fo_padding_left_) 
			properties.push_back(_property(L"text-padding-left",	common_padding_attlist_.fo_padding_left_->get_value_unit(length::emu)));
		if (common_padding_attlist_.fo_padding_right_) 
			properties.push_back(_property(L"text-padding-right",	common_padding_attlist_.fo_padding_right_->get_value_unit(length::emu)));
		if (common_padding_attlist_.fo_padding_top_) 
			properties.push_back(_property(L"text-padding-top",		common_padding_attlist_.fo_padding_top_->get_value_unit(length::emu)));
		if (common_padding_attlist_.fo_padding_bottom_) 
			properties.push_back(_property(L"text-padding-bottom",	common_padding_attlist_.fo_padding_bottom_->get_value_unit(length::emu)));
	}
	if (fo_wrap_option_)	
		properties.push_back(_property(L"text-wrap", (int)fo_wrap_option_->get_type()));
}
void graphic_format_properties::apply_from(const graphic_format_properties * Other)
{
	if (Other == NULL) return;

	_CP_APPLY_PROP3(draw_stroke_); 
	_CP_APPLY_PROP3(draw_stroke_dash_); 
	_CP_APPLY_PROP3(draw_marker_start_); 
	_CP_APPLY_PROP3(draw_marker_end_); 
	_CP_APPLY_PROP3(draw_textarea_horizontal_align_); 
	_CP_APPLY_PROP3(draw_textarea_vertical_align_); 
	_CP_APPLY_PROP3(draw_auto_grow_height_);
	_CP_APPLY_PROP3(draw_auto_grow_width_);
	_CP_APPLY_PROP3(draw_fit_to_size_);
	_CP_APPLY_PROP3(draw_fit_to_contour_);
	_CP_APPLY_PROP3(style_shrink_to_fit_);
	
	_CP_APPLY_PROP3(svg_stroke_color_); 
	_CP_APPLY_PROP3(svg_stroke_width_);	
	_CP_APPLY_PROP3(svg_stroke_opacity_); 
	
	_CP_APPLY_PROP3(fo_min_width_);
    _CP_APPLY_PROP3(fo_min_height_);
    _CP_APPLY_PROP3(fo_max_width_);
    _CP_APPLY_PROP3(fo_max_height_);
	_CP_APPLY_PROP3(fo_wrap_option_);

    _CP_APPLY_PROP3(style_print_content_);
    _CP_APPLY_PROP3(style_protect_);
	_CP_APPLY_PROP3(style_editable_);
    _CP_APPLY_PROP3(style_wrap_);
    _CP_APPLY_PROP3(style_wrap_dynamic_treshold_);
    _CP_APPLY_PROP3(style_number_wrapped_paragraphs_);
    _CP_APPLY_PROP3(style_wrap_contour_);
    _CP_APPLY_PROP3(style_wrap_contour_mode_);
    _CP_APPLY_PROP3(style_run_through_); 
    _CP_APPLY_PROP3(style_flow_with_text_);
    _CP_APPLY_PROP3(style_overflow_behavior_);
    _CP_APPLY_PROP3(style_mirror_);
    _CP_APPLY_PROP3(fo_clip_);
    _CP_APPLY_PROP3(draw_wrap_influence_on_position_);

    common_draw_fill_attlist_.apply_from		(Other->common_draw_fill_attlist_);
    common_draw_rel_size_attlist_.apply_from	(Other->common_draw_rel_size_attlist_);
    common_horizontal_margin_attlist_.apply_from(Other->common_horizontal_margin_attlist_);
    common_vertical_margin_attlist_.apply_from	(Other->common_vertical_margin_attlist_);
    common_margin_attlist_.apply_from			(Other->common_margin_attlist_);    
    common_horizontal_pos_attlist_.apply_from	(Other->common_horizontal_pos_attlist_);
    common_horizontal_rel_attlist_.apply_from	(Other->common_horizontal_rel_attlist_);
    common_vertical_pos_attlist_.apply_from		(Other->common_vertical_pos_attlist_);
    common_vertical_rel_attlist_.apply_from		(Other->common_vertical_rel_attlist_);
    common_text_anchor_attlist_.apply_from		(Other->common_text_anchor_attlist_);
    common_border_attlist_.apply_from			(Other->common_border_attlist_);
    common_border_line_width_attlist_.apply_from(Other->common_border_line_width_attlist_);
    common_padding_attlist_.apply_from			(Other->common_padding_attlist_);
    common_shadow_attlist_.apply_from			(Other->common_shadow_attlist_);
    common_background_color_attlist_.apply_from	(Other->common_background_color_attlist_);
     

    _CP_APPLY_PROP(style_background_image_,	Other->style_background_image_);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_graphic_properties::ns = L"style";
const wchar_t * style_graphic_properties::name = L"graphic-properties";

void style_graphic_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	content_.add_attributes(Attributes);
}

void style_graphic_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
     if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(content_.style_background_image_);
    }

	//if (CP_CHECK_NAME(L"text", L"list-style") 	
	//	styles_.add_child_element(Reader, Ns, Name, getContext()); он тут и не нужен по сути... описание есть и в другом сместе
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * loext_graphic_properties::ns	= L"loext";
const wchar_t * loext_graphic_properties::name	= L"graphic-properties";

void loext_graphic_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	content_.add_attributes(Attributes);
}

void loext_graphic_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
     if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(content_.style_background_image_);
    }

	//if (CP_CHECK_NAME(L"text", L"list-style") 	
	//	styles_.add_child_element(Reader, Ns, Name, getContext()); он тут и не нужен по сути... описание есть и в другом сместе
}
}
}
