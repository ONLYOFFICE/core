/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "common_attlists.h"
#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 
namespace odf_types {

void apply_line_width(_CP_OPT(line_width) & A, const _CP_OPT(line_width) & B)
{
    if (B 
        && B->get_type() == line_width::Percent
        && A 
        && (
        A->get_type() == line_width::Percent ||
        A->get_type() == line_width::PositiveInteger ||
        A->get_type() == line_width::PositiveLength
        )
        )
    {
        const double mul = B->get_percent().get_value()/100.;
        // TODO
        switch(A->get_type())
        {
        case line_width::Percent:
            // TODO???
            A = line_width( percent( A->get_percent().get_value() * mul ) );
            break;
        case line_width::PositiveInteger:
            A = line_width( (unsigned int)(1.0 * mul * A->get_positive_integer() + 0.5) );
            break;
        case line_width::PositiveLength:
            A = line_width( length(A->get_positive_length().get_value() * mul, A->get_positive_length().get_unit()) );
            break;
        default:
            break;
        }
    }
    else
        _CP_APPLY_PROP(A, B);    
}

void apply_length_or_percent(_CP_OPT(length_or_percent) & Value, const _CP_OPT(length_or_percent) & Other)
{
    if (Other)
    {
        if (Value && 
            Value->get_type() == length_or_percent::Length &&
            Other->get_type() == length_or_percent::Percent)
        {
            Value = length(Value->get_length().get_value() * Other->get_percent().get_value() / 100.0, Value->get_length().get_unit());            
        }
        else
        {
            Value = Other;
        }
    }
}
void common_draw_fill_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:fill-color",	draw_fill_color_); 
	CP_APPLY_ATTR(L"draw:fill",			draw_fill_); 
	
	CP_APPLY_ATTR(L"draw:opacity",		draw_opacity_); 
	CP_APPLY_ATTR(L"draw:image-opacity",draw_image_opacity_); 
	
 	CP_APPLY_ATTR(L"draw:fill-image-name",		draw_fill_image_name_);
	CP_APPLY_ATTR(L"draw:fill-gradient-name",	draw_fill_gradient_name_);
	CP_APPLY_ATTR(L"draw:fill-hatch-name",		draw_fill_hatch_name_);
	CP_APPLY_ATTR(L"draw:opacity-name",			draw_opacity_name_);

	CP_APPLY_ATTR(L"draw:fill-hatch-solid",		draw_fill_hatch_solid_);
    CP_APPLY_ATTR(L"style:repeat",				style_repeat_);

	CP_APPLY_ATTR(L"draw:fill-image-width",		draw_fill_image_width_);
	CP_APPLY_ATTR(L"draw:fill-image-height",	draw_fill_image_height_);

}
void common_draw_fill_attlist::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"draw:fill-color",		draw_fill_color_); 
	CP_XML_ATTR_OPT(L"draw:fill",			draw_fill_); 
	
	CP_XML_ATTR_OPT(L"draw:opacity",		draw_opacity_); 
	CP_XML_ATTR_OPT(L"draw:image-opacity",	draw_image_opacity_); 
	
 	CP_XML_ATTR_OPT(L"draw:fill-image-name",	draw_fill_image_name_);
	CP_XML_ATTR_OPT(L"draw:fill-gradient-name",	draw_fill_gradient_name_);
	CP_XML_ATTR_OPT(L"draw:fill-hatch-name",	draw_fill_hatch_name_);
	CP_XML_ATTR_OPT(L"draw:opacity-name",		draw_opacity_name_);

	CP_XML_ATTR_OPT(L"draw:fill-hatch-solid",	draw_fill_hatch_solid_);
    CP_XML_ATTR_OPT(L"style:repeat",			style_repeat_);

	CP_XML_ATTR_OPT(L"draw:fill-image-ref-point",	draw_fill_image_ref_point_);
	CP_XML_ATTR_OPT(L"draw:fill-image-ref-point-y",	draw_fill_image_ref_point_y_);
	CP_XML_ATTR_OPT(L"draw:fill-image-ref-point-x",	draw_fill_image_ref_point_x_);

	CP_XML_ATTR_OPT(L"draw:fill-image-width",		draw_fill_image_width_);
	CP_XML_ATTR_OPT(L"draw:fill-image-height",		draw_fill_image_height_);

}

void common_draw_fill_attlist::apply_from(const common_draw_fill_attlist & Other)
{
	_CP_APPLY_PROP2(draw_fill_); 
	
 	_CP_APPLY_PROP2(draw_fill_image_name_);
	_CP_APPLY_PROP2(draw_fill_gradient_name_);
	_CP_APPLY_PROP2(draw_fill_hatch_name_);
	_CP_APPLY_PROP2(draw_opacity_name_);

	_CP_APPLY_PROP2(draw_opacity_); 
	_CP_APPLY_PROP2(draw_image_opacity_); 
	_CP_APPLY_PROP2(draw_fill_hatch_solid_);

	_CP_APPLY_PROP2(style_repeat_);
	_CP_APPLY_PROP2(draw_fill_color_); 

	_CP_APPLY_PROP2(draw_fill_image_width_);
	_CP_APPLY_PROP2(draw_fill_image_height_);
}

void common_horizontal_margin_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:margin-left", fo_margin_left_);
    CP_APPLY_ATTR(L"fo:margin-right", fo_margin_right_);
}

void common_horizontal_margin_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:margin-left", fo_margin_left_);
    CP_XML_ATTR_OPT(L"fo:margin-right", fo_margin_right_);
}
void common_horizontal_margin_attlist::apply_from(const common_horizontal_margin_attlist & Other)
{
    // TODO
    apply_length_or_percent(fo_margin_left_, Other.fo_margin_left_);
    apply_length_or_percent(fo_margin_right_, Other.fo_margin_right_);        
}


//////////////////////////////////////////////////////////////////////////////////////////////////

void common_vertical_margin_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:margin-top", fo_margin_top_);
    CP_APPLY_ATTR(L"fo:margin-bottom", fo_margin_bottom_);
}
void common_vertical_margin_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:margin-top", fo_margin_top_);
    CP_XML_ATTR_OPT(L"fo:margin-bottom", fo_margin_bottom_);
}

void common_vertical_margin_attlist::apply_from(const common_vertical_margin_attlist & Other)
{
    // TODO
    apply_length_or_percent(fo_margin_top_, Other.fo_margin_top_);
    apply_length_or_percent(fo_margin_bottom_, Other.fo_margin_bottom_);        
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void common_margin_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:margin", fo_margin_);
}
void common_margin_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:margin", fo_margin_);
}
void common_margin_attlist::apply_from(const common_margin_attlist & Other)
{
    // TODO
    _CP_APPLY_PROP(fo_margin_, Other.fo_margin_);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void common_math_style_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"math:mathvariant", mathvariant_);
	
	if (!mathvariant_)	CP_APPLY_ATTR(L"mathvariant", mathvariant_);
}
void common_math_style_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"mathvariant", mathvariant_);
}
void common_math_style_attlist::apply_from(const common_math_style_attlist & Other)
{
    // TODO
    _CP_APPLY_PROP(mathvariant_, Other.mathvariant_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_break_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:break-before", fo_break_before_);
    CP_APPLY_ATTR(L"fo:break-after", fo_break_after_);
}
void common_break_attlist::apply_from(const common_break_attlist & Other)
{
    _CP_APPLY_PROP(fo_break_before_, Other.fo_break_before_);
    _CP_APPLY_PROP(fo_break_after_, Other.fo_break_after_);
}
void common_break_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:break-before", fo_break_before_);
    CP_XML_ATTR_OPT(L"fo:break-after", fo_break_after_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_page_number_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:page-number", style_page_number_);
}
void common_page_number_attlist::apply_from(const common_page_number_attlist & Other)
{
    _CP_APPLY_PROP(style_page_number_, Other.style_page_number_);
}
void common_page_number_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:page-number", style_page_number_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_background_color_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:background-color", fo_background_color_);
}
void common_background_color_attlist::apply_from(const common_background_color_attlist & Other)
{
    _CP_APPLY_PROP(fo_background_color_, Other.fo_background_color_);
}
void common_background_color_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:background-color", fo_background_color_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_shadow_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:shadow", style_shadow_);
}
void common_shadow_attlist::apply_from(const common_shadow_attlist & Other)
{
    _CP_APPLY_PROP(style_shadow_, Other.style_shadow_);
}
void common_shadow_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:shadow", style_shadow_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_keep_with_next_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"keep:together", keep_together_);
}
void common_keep_with_next_attlist::apply_from(const common_keep_with_next_attlist & Other)
{
    _CP_APPLY_PROP(keep_together_, Other.keep_together_);
}
void common_keep_with_next_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"keep:together", keep_together_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_writing_mode_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:writing-mode", style_writing_mode_);
}
void common_writing_mode_attlist::apply_from(const common_writing_mode_attlist & Other)
{
    _CP_APPLY_PROP(style_writing_mode_, Other.style_writing_mode_);
}
void common_writing_mode_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:writing-mode", style_writing_mode_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool common_xlink_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"xlink:href",		href_);
    CP_APPLY_ATTR(L"xlink:type",		type_);
    CP_APPLY_ATTR(L"xlink:show",		show_);    
    CP_APPLY_ATTR(L"xlink:actuate",		actuate_);    
    
	if (href_ || type_ || show_ || actuate_) return true;

	return false;
}
void common_xlink_attlist::apply_from(const common_xlink_attlist & Other)
{
    _CP_APPLY_PROP(href_, Other.href_);
    _CP_APPLY_PROP(type_, Other.type_);
    _CP_APPLY_PROP(show_, Other.show_);
    _CP_APPLY_PROP(actuate_, Other.actuate_);
}
void common_xlink_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"xlink:href",		href_);
    CP_XML_ATTR_OPT(L"xlink:type",		type_);
    CP_XML_ATTR_OPT(L"xlink:show",		show_);    
    CP_XML_ATTR_OPT(L"xlink:actuate",	actuate_);    
}
//////////////////////////////////////////////////////////////////////////////////////////////////

bool common_value_and_type_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"office:value-type"	, office_value_type_);
    CP_APPLY_ATTR(L"office:value"		, office_value_);
    CP_APPLY_ATTR(L"office:currency"	, office_currency_);
    CP_APPLY_ATTR(L"office:date-value"	, office_date_value_);
    CP_APPLY_ATTR(L"office:time-value"	, office_time_value_);
    CP_APPLY_ATTR(L"office:boolean-value", office_boolean_value_);
    CP_APPLY_ATTR(L"office:string-value", office_string_value_);
  
	if (office_value_type_ || office_value_ || office_currency_ || office_date_value_	|| office_time_value_ 
																|| office_boolean_value_ || office_string_value_) return true;
	return false;
}
void common_value_and_type_attlist::apply_from(const common_value_and_type_attlist & Other)
{
    _CP_APPLY_PROP(office_value_type_	, Other.office_value_type_);
    _CP_APPLY_PROP(office_value_		, Other.office_value_);
    _CP_APPLY_PROP(office_currency_		, Other.office_currency_);
    _CP_APPLY_PROP(office_date_value_	, Other.office_date_value_);
    _CP_APPLY_PROP(office_time_value_	, Other.office_time_value_);
    _CP_APPLY_PROP(office_boolean_value_, Other.office_boolean_value_);
    _CP_APPLY_PROP(office_string_value_	, Other.office_string_value_);
}
void common_value_and_type_attlist::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"office:value-type", office_value_type_);
    CP_XML_ATTR_OPT(L"office:value", office_value_);
	if (office_value_)
	{
		CP_XML_ATTR_OPT(L"office:currency", office_currency_);
	}
    CP_XML_ATTR_OPT(L"office:date-value", office_date_value_);
    CP_XML_ATTR_OPT(L"office:time-value", office_time_value_);
    CP_XML_ATTR_OPT(L"office:boolean-value", office_boolean_value_);
    CP_XML_ATTR_OPT(L"office:string-value", office_string_value_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_border_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:border",			fo_border_);
    CP_APPLY_ATTR(L"fo:border-top",		fo_border_top_);
    CP_APPLY_ATTR(L"fo:border-bottom",	fo_border_bottom_);
    CP_APPLY_ATTR(L"fo:border-left",	fo_border_left_);
    CP_APPLY_ATTR(L"fo:border-right",	fo_border_right_);    
}
void common_border_attlist::apply_from(const common_border_attlist & Other)
{
	if (fo_border_ && !Other.fo_border_)
	{
		fo_border_top_ = fo_border_bottom_ = fo_border_left_ = fo_border_right_ = fo_border_;
		fo_border_ = Other.fo_border_;
	}
	_CP_APPLY_PROP(fo_border_,			Other.fo_border_);
    _CP_APPLY_PROP(fo_border_top_,		Other.fo_border_top_);
    _CP_APPLY_PROP(fo_border_bottom_,	Other.fo_border_bottom_);
    _CP_APPLY_PROP(fo_border_left_,		Other.fo_border_left_);
    _CP_APPLY_PROP(fo_border_right_,	Other.fo_border_right_);
}
void common_border_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:border",			fo_border_);
    CP_XML_ATTR_OPT(L"fo:border-top",		fo_border_top_);
    CP_XML_ATTR_OPT(L"fo:border-bottom",	fo_border_bottom_);
    CP_XML_ATTR_OPT(L"fo:border-left",		fo_border_left_);
    CP_XML_ATTR_OPT(L"fo:border-right",		fo_border_right_);    
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_border_line_width_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:border-line-width",		style_border_line_width_);
    CP_APPLY_ATTR(L"style:border-line-width-top",	style_border_line_width_top_);
    CP_APPLY_ATTR(L"style:border-line-width-bottom", style_border_line_width_bottom_);
    CP_APPLY_ATTR(L"style:border-line-width-left",	style_border_line_width_left_);
    CP_APPLY_ATTR(L"style:border-line-width-right", style_border_line_width_right_);

}
void common_border_line_width_attlist::apply_from(const common_border_line_width_attlist & Other)
{
    _CP_APPLY_PROP2(style_border_line_width_);
    _CP_APPLY_PROP2(style_border_line_width_top_);
    _CP_APPLY_PROP2(style_border_line_width_bottom_);
    _CP_APPLY_PROP2(style_border_line_width_left_);
    _CP_APPLY_PROP2(style_border_line_width_right_);
}
void common_border_line_width_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:border-line-width",			style_border_line_width_);
    CP_XML_ATTR_OPT(L"style:border-line-width-top",		style_border_line_width_top_);
    CP_XML_ATTR_OPT(L"style:border-line-width-bottom",	style_border_line_width_bottom_);
    CP_XML_ATTR_OPT(L"style:border-line-width-left",	style_border_line_width_left_);
    CP_XML_ATTR_OPT(L"style:border-line-width-right",	style_border_line_width_right_);

}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_padding_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:padding"			, fo_padding_);
    CP_APPLY_ATTR(L"fo:padding-top"		, fo_padding_top_);
    CP_APPLY_ATTR(L"fo:padding-bottom"	, fo_padding_bottom_);
    CP_APPLY_ATTR(L"fo:padding-left"	, fo_padding_left_);
    CP_APPLY_ATTR(L"fo:padding-right"	, fo_padding_right_);
}
void common_padding_attlist::apply_from(const common_padding_attlist & Other)
{
    _CP_APPLY_PROP2(fo_padding_);
    _CP_APPLY_PROP2(fo_padding_top_);
    _CP_APPLY_PROP2(fo_padding_bottom_);
    _CP_APPLY_PROP2(fo_padding_left_);
    _CP_APPLY_PROP2(fo_padding_right_);
}
void common_padding_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:padding"		, fo_padding_);
    CP_XML_ATTR_OPT(L"fo:padding-top"	, fo_padding_top_);
    CP_XML_ATTR_OPT(L"fo:padding-bottom", fo_padding_bottom_);
    CP_XML_ATTR_OPT(L"fo:padding-left"	, fo_padding_left_);
    CP_XML_ATTR_OPT(L"fo:padding-right"	, fo_padding_right_);

}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_rotation_angle_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:rotation-angle", style_rotation_angle_);
}
void common_rotation_angle_attlist::apply_from(const common_rotation_angle_attlist & Other)
{
    _CP_APPLY_PROP2(style_rotation_angle_);
}
void common_rotation_angle_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:rotation-angle", style_rotation_angle_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_text_align::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:text-align", fo_text_align_);
}
void common_text_align::apply_from(const common_text_align & Other)
{
    _CP_APPLY_PROP2(fo_text_align_);
}
void common_text_align::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:text-align", fo_text_align_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_size_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:width", svg_width_);
    CP_APPLY_ATTR(L"svg:height", svg_height_);
}

void common_draw_size_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"svg:width", svg_width_);
    CP_XML_ATTR_OPT(L"svg:height", svg_height_);
}

void common_draw_size_attlist::apply_from(const common_draw_size_attlist & Other)
{
    _CP_APPLY_PROP(svg_width_, Other.svg_width_);
    _CP_APPLY_PROP(svg_height_, Other.svg_height_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_rel_size_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_draw_size_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"style:rel-width", style_rel_width_);
    CP_APPLY_ATTR(L"style:rel-height", style_rel_height_);
}

void common_draw_rel_size_attlist::apply_from(const common_draw_rel_size_attlist & Other)
{
    // TODO
    common_draw_size_attlist_.apply_from(Other.common_draw_size_attlist_);
    _CP_APPLY_PROP(style_rel_width_, Other.style_rel_width_);
    _CP_APPLY_PROP(style_rel_height_, Other.style_rel_height_);
}

void common_draw_rel_size_attlist::serialize(CP_ATTR_NODE)
{
    common_draw_size_attlist_.serialize(CP_GET_XML_NODE());
    CP_XML_ATTR_OPT(L"style:rel-width", style_rel_width_);
    CP_XML_ATTR_OPT(L"style:rel-height", style_rel_height_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_text_anchor_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:anchor-type", type_);
    CP_APPLY_ATTR(L"text:anchor-page-number", page_number_);
}
void common_text_anchor_attlist::apply_from(const common_text_anchor_attlist & Other)
{
    _CP_APPLY_PROP2(type_);
    _CP_APPLY_PROP2(page_number_);
}
void common_text_anchor_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:anchor-type", type_);
    CP_XML_ATTR_OPT(L"text:anchor-page-number", page_number_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_text_animation_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
//    CP_APPLY_ATTR(L"text:animation", type_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_vertical_rel_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:vertical-rel", style_vertical_rel_);
}
void common_vertical_rel_attlist::apply_from(const common_vertical_rel_attlist & Other)
{
    _CP_APPLY_PROP2(style_vertical_rel_);
}
void common_vertical_rel_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:vertical-rel", style_vertical_rel_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_horizontal_rel_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:horizontal-rel", style_horizontal_rel_);
}
void common_horizontal_rel_attlist::apply_from(const common_horizontal_rel_attlist & Other)
{
    _CP_APPLY_PROP2(style_horizontal_rel_);
}
void common_horizontal_rel_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:horizontal-rel", style_horizontal_rel_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_vertical_pos_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:vertical-pos", style_vertical_pos_);
    CP_APPLY_ATTR(L"svg:y", svg_y_);
}
void common_vertical_pos_attlist::apply_from(const common_vertical_pos_attlist & Other)
{
    _CP_APPLY_PROP2(style_vertical_pos_);
    _CP_APPLY_PROP2(svg_y_);
}
void common_vertical_pos_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:vertical-pos", style_vertical_pos_);
    CP_XML_ATTR_OPT(L"svg:y", svg_y_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_horizontal_pos_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:horizontal-pos", style_horizontal_pos_);
    CP_APPLY_ATTR(L"svg:x", svg_x_);
}
void common_horizontal_pos_attlist::apply_from(const common_horizontal_pos_attlist & Other)
{
    _CP_APPLY_PROP2(style_horizontal_pos_);
    _CP_APPLY_PROP2(svg_x_);
}
void common_horizontal_pos_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:horizontal-pos", style_horizontal_pos_);
    CP_XML_ATTR_OPT(L"svg:x", svg_x_);
}

//  common_num_format_attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_num_format_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:num-format",		style_num_format_);
    CP_APPLY_ATTR(L"style:num-letter-sync", style_num_letter_sync_);    
}
void common_num_format_attlist::apply_from(const common_num_format_attlist & Other)
{
    _CP_APPLY_PROP2(style_num_format_);
    _CP_APPLY_PROP2(style_num_letter_sync_);
}
void common_num_format_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:num-format",		style_num_format_);
    CP_XML_ATTR_OPT(L"style:num-letter-sync",	style_num_letter_sync_);    
}

//  common_num_format_prefix_suffix_attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_num_format_prefix_suffix_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:num-prefix", style_num_prefix_);
    CP_APPLY_ATTR(L"style:num-suffix", style_num_suffix_);
}
void common_num_format_prefix_suffix_attlist::apply_from(const common_num_format_prefix_suffix_attlist & Other)
{
    _CP_APPLY_PROP2(style_num_prefix_);
    _CP_APPLY_PROP2(style_num_suffix_);
}
void common_num_format_prefix_suffix_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:num-prefix", style_num_prefix_);
    CP_XML_ATTR_OPT(L"style:num-suffix", style_num_suffix_);
}

/// common-draw-style-name-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_style_name_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:style-name", draw_style_name_);
    CP_APPLY_ATTR(L"draw:class-names", draw_class_names_);
    CP_APPLY_ATTR(L"presentation:style-name", presentation_style_name_);
    CP_APPLY_ATTR(L"presentation:class-names", presentation_class_names_);
}
void common_draw_style_name_attlist::apply_from(const common_draw_style_name_attlist & Other)
{
    _CP_APPLY_PROP2(draw_style_name_);
    _CP_APPLY_PROP2(draw_class_names_);
    _CP_APPLY_PROP2(presentation_style_name_);
    _CP_APPLY_PROP2(presentation_class_names_);
}
void common_draw_style_name_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:style-name", draw_style_name_);
    CP_XML_ATTR_OPT(L"draw:class-names", draw_class_names_);
    CP_XML_ATTR_OPT(L"presentation:style-name", presentation_style_name_);
    CP_XML_ATTR_OPT(L"presentation:class-names", presentation_class_names_);
}
/// common-draw-z-index-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_z_index_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:z-index", draw_z_index_);
}
void common_draw_z_index_attlist::apply_from(const common_draw_z_index_attlist & Other)
{
    _CP_APPLY_PROP2(draw_z_index_);
}
void common_draw_z_index_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:z-index", draw_z_index_);
}

/// common-draw-id-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_id_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:id", draw_id_);
}
void common_draw_id_attlist::apply_from(const common_draw_id_attlist & Other)
{
    _CP_APPLY_PROP2(draw_id_);
}
void common_draw_id_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:id", draw_id_);
}
// common-draw-layer-name-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_layer_name_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:layer", draw_layer_);
}
void common_draw_layer_name_attlist::apply_from(const common_draw_layer_name_attlist & Other)
{
    _CP_APPLY_PROP2(draw_layer_);
}
void common_draw_layer_name_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:layer", draw_layer_);
}
// common-draw-transform-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_transform_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:transform", draw_transform_);
}
void common_draw_transform_attlist::apply_from(const common_draw_transform_attlist & Other)
{
    _CP_APPLY_PROP2(draw_transform_);
}
void common_draw_transform_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:transform", draw_transform_);
}

// common-draw-name-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_name_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:name", draw_name_);
}
void common_draw_name_attlist::apply_from(const common_draw_name_attlist & Other)
{
    _CP_APPLY_PROP2(draw_name_);
}
void common_draw_name_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:name", draw_name_);
}
/// common-text-spreadsheet-shape-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_text_spreadsheet_shape_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:end-cell-address", table_end_cell_address_);
    CP_APPLY_ATTR(L"table:end-x", table_end_x_);
    CP_APPLY_ATTR(L"table:end-y", table_end_y_);
    CP_APPLY_ATTR(L"table:table-background", table_table_background_);
    common_text_anchor_attlist_.add_attributes(Attributes);
}
void common_text_spreadsheet_shape_attlist::apply_from(const common_text_spreadsheet_shape_attlist & Other)
{
    _CP_APPLY_PROP2(table_end_cell_address_);
    _CP_APPLY_PROP2(table_end_x_);
    _CP_APPLY_PROP2(table_end_y_);
    _CP_APPLY_PROP2(table_table_background_);
    common_text_anchor_attlist_.apply_from(Other.common_text_anchor_attlist_);
}
void common_text_spreadsheet_shape_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"table:end-cell-address", table_end_cell_address_);
    CP_XML_ATTR_OPT(L"table:end-x", table_end_x_);
    CP_XML_ATTR_OPT(L"table:end-y", table_end_y_);
    CP_XML_ATTR_OPT(L"table:table-background", table_table_background_);
    common_text_anchor_attlist_.serialize(CP_GET_XML_NODE());
}

/// common-draw-shape-with-styles-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_shape_with_styles_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_draw_z_index_attlist_.add_attributes(Attributes);
    common_draw_id_attlist_.add_attributes(Attributes);
    common_draw_layer_name_attlist_.add_attributes(Attributes);
    common_draw_style_name_attlist_.add_attributes(Attributes);
    common_draw_transform_attlist_.add_attributes(Attributes);
    common_draw_name_attlist_.add_attributes(Attributes);
    common_text_spreadsheet_shape_attlist_.add_attributes(Attributes);
}
void common_draw_shape_with_styles_attlist::apply_from(const common_draw_shape_with_styles_attlist & Other)
{
    common_draw_z_index_attlist_.apply_from(            Other.common_draw_z_index_attlist_);
    common_draw_id_attlist_.apply_from(                 Other.common_draw_id_attlist_);
    common_draw_layer_name_attlist_.apply_from(         Other.common_draw_layer_name_attlist_);
    common_draw_style_name_attlist_.apply_from(         Other.common_draw_style_name_attlist_);
    common_draw_transform_attlist_.apply_from(          Other.common_draw_transform_attlist_);
    common_draw_name_attlist_.apply_from(               Other.common_draw_name_attlist_);
    common_text_spreadsheet_shape_attlist_.apply_from(  Other.common_text_spreadsheet_shape_attlist_);
}
void common_draw_shape_with_styles_attlist::serialize(CP_ATTR_NODE)
{
    common_draw_z_index_attlist_.serialize(CP_GET_XML_NODE());
    common_draw_id_attlist_.serialize(CP_GET_XML_NODE());
    common_draw_layer_name_attlist_.serialize(CP_GET_XML_NODE());
    common_draw_style_name_attlist_.serialize(CP_GET_XML_NODE());
    common_draw_transform_attlist_.serialize(CP_GET_XML_NODE());
    common_draw_name_attlist_.serialize(CP_GET_XML_NODE());
    common_text_spreadsheet_shape_attlist_.serialize(CP_GET_XML_NODE());
}
/// common-draw-text-style-name-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_text_style_name_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:text-style-name", draw_text_style_name_);
}
void common_draw_text_style_name_attlist::apply_from(const common_draw_text_style_name_attlist & Other)
{
    _CP_APPLY_PROP2(draw_text_style_name_);
}
void common_draw_text_style_name_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:text-style-name", draw_text_style_name_);
}
/// common-draw-shape-with-text-and-styles-attlist

void common_draw_shape_with_text_and_styles_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_draw_shape_with_styles_attlist_.add_attributes(Attributes);
    common_draw_text_style_name_attlist_.add_attributes(Attributes);
}
void common_draw_shape_with_text_and_styles_attlist::apply_from(const common_draw_shape_with_text_and_styles_attlist & Other)
{
    common_draw_shape_with_styles_attlist_.apply_from(common_draw_shape_with_styles_attlist_);
    common_draw_text_style_name_attlist_.apply_from(common_draw_text_style_name_attlist_);
}
void common_draw_shape_with_text_and_styles_attlist::serialize(CP_ATTR_NODE)
{
    common_draw_shape_with_styles_attlist_.serialize(CP_GET_XML_NODE());
    common_draw_text_style_name_attlist_.serialize(CP_GET_XML_NODE());
}

/// common-draw-position-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_position_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:x", svg_x_);
    CP_APPLY_ATTR(L"svg:y", svg_y_);
}
void common_draw_position_attlist::apply_from(const common_draw_position_attlist & Other)
{
    _CP_APPLY_PROP2(svg_x_);
    _CP_APPLY_PROP2(svg_y_);
}
void common_draw_position_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"svg:x", svg_x_);
    CP_XML_ATTR_OPT(L"svg:y", svg_y_);
}

/// presentation-shape-attlist

void presentation_shape_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"presentation:class", presentation_class_);
    CP_APPLY_ATTR(L"presentation:placeholder", presentation_placeholder_);
    CP_APPLY_ATTR(L"presentation:user-transformed", presentation_user_transformed_);
}
void presentation_shape_attlist::apply_from(const presentation_shape_attlist & Other)
{
    _CP_APPLY_PROP2(presentation_class_);
    _CP_APPLY_PROP2(presentation_placeholder_);
    _CP_APPLY_PROP2(presentation_user_transformed_);
}
void presentation_shape_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"presentation:class", presentation_class_);
    CP_XML_ATTR_OPT(L"presentation:placeholder", presentation_placeholder_);
    CP_XML_ATTR_OPT(L"presentation:user-transformed", presentation_user_transformed_);
}
///  common-data-style-attlist
void common_data_style_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:name", style_name_);
    CP_APPLY_ATTR(L"number:language", number_language_);
    CP_APPLY_ATTR(L"number:country", number_country_);
    CP_APPLY_ATTR(L"number:title", number_title_);
    CP_APPLY_ATTR(L"number:volatile", number_volatile_);
    CP_APPLY_ATTR(L"number:transliteration-format", number_transliteration_format_);
    CP_APPLY_ATTR(L"number:transliteration-language", number_transliteration_language_);
    CP_APPLY_ATTR(L"number:transliteration-country", number_transliteration_country_);
    CP_APPLY_ATTR(L"number:transliteration-style", number_transliteration_style_);
    
}
void common_data_style_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:name", style_name_);
    CP_XML_ATTR_OPT(L"number:language", number_language_);
    CP_XML_ATTR_OPT(L"number:country", number_country_);
    CP_XML_ATTR_OPT(L"number:title", number_title_);
    CP_XML_ATTR_OPT(L"number:volatile", number_volatile_);
    CP_XML_ATTR_OPT(L"number:transliteration-format", number_transliteration_format_);
    CP_XML_ATTR_OPT(L"number:transliteration-language", number_transliteration_language_);
    CP_XML_ATTR_OPT(L"number:transliteration-country", number_transliteration_country_);
    CP_XML_ATTR_OPT(L"number:transliteration-style", number_transliteration_style_);
    
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_presentation_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"presentation:class", presentation_class_);
    CP_APPLY_ATTR(L"presentation:style-name", style_name_);
	CP_APPLY_ATTR(L"presentation:placeholder", presentation_placeholder_);
}
void common_presentation_attlist::apply_from(const common_presentation_attlist & Other)
{
    _CP_APPLY_PROP(presentation_class_, Other.presentation_class_);
    _CP_APPLY_PROP(style_name_, Other.style_name_);
    _CP_APPLY_PROP(presentation_placeholder_, Other.presentation_placeholder_);
}
void common_presentation_attlist::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"presentation:class", presentation_class_);
    CP_XML_ATTR_OPT(L"presentation:style-name", style_name_);
	CP_XML_ATTR_OPT(L"presentation:placeholder", presentation_placeholder_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_anim_smil_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	//CP_APPLY_ATTR(L"smil:direction", smil_direction_);
 //   CP_APPLY_ATTR(L"smil:subtype", smil_subtype_);
 //   CP_APPLY_ATTR(L"smil:type", smil_type_);
	//CP_APPLY_ATTR(L"smil:dur", smil_dur_);

}
void common_anim_smil_attlist::apply_from(const common_anim_smil_attlist & Other)
{
    //_CP_APPLY_PROP(smil_direction_, Other.smil_direction_);
    //_CP_APPLY_PROP(smil_subtype_, Other.smil_subtype_);
    //_CP_APPLY_PROP(smil_type_, Other.smil_type_);
   // _CP_APPLY_PROP(smil_dur_, Other.smil_dur_);
}
void union_common_draw_attlists::serialize(CP_ATTR_NODE)
{
    shape_with_text_and_styles_.serialize(CP_GET_XML_NODE());
    rel_size_.serialize(CP_GET_XML_NODE());
    position_.serialize(CP_GET_XML_NODE());
}

}
}
