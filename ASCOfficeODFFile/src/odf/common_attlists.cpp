#include "precompiled_cpodf.h"
#include <iostream>
#include <cpdoccore/xml/serialize.h>
#include "common_attlists.h"
#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 
namespace odf {

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
        const double mul = B->get_percent().get_value();
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

void common_horizontal_margin_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:margin-left", fo_margin_left_);
    CP_APPLY_ATTR(L"fo:margin-right", fo_margin_right_);
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

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_break_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:break-before", fo_break_before_);
    CP_APPLY_ATTR(L"fo:break-after", fo_break_after_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_page_number_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:page-number", style_page_number_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_background_color_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:background-color", fo_background_color_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_shadow_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:shadow", style_shadow_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_keep_with_next_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"keep:together", keep_together_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_writing_mode_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:writing-mode", style_writing_mode_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool common_xlink_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    bool b = CP_APPLY_ATTR(L"xlink:href", xlink_href_);
    CP_APPLY_ATTR(L"xlink:type",		xlink_type_);
    CP_APPLY_ATTR(L"xlink:show",		xlink_show_);    
    CP_APPLY_ATTR(L"xlink:actuate",		xlink_actuate_);    
    return b;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool common_value_and_type_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    if (!CP_APPLY_ATTR(L"office:value-type", office_value_type_, std::wstring(L"") ))
        return false;
    
    CP_APPLY_ATTR(L"office:value", office_value_);
    CP_APPLY_ATTR(L"office:currency", office_currency_);
    CP_APPLY_ATTR(L"office:date-value", office_date_value_);
    CP_APPLY_ATTR(L"office:time-value", office_time_value_);
    CP_APPLY_ATTR(L"office:boolean-value", office_boolean_value_);
    CP_APPLY_ATTR(L"office:string-value", office_string_value_);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_border_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:border", fo_border_);
    CP_APPLY_ATTR(L"fo:border-top", fo_border_top_);
    CP_APPLY_ATTR(L"fo:border-bottom", fo_border_bottom_);
    CP_APPLY_ATTR(L"fo:border-left", fo_border_left_);
    CP_APPLY_ATTR(L"fo:border-right", fo_border_right_);    
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_border_line_width_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:border-line-width", style_border_line_width_);
    CP_APPLY_ATTR(L"style:border-line-width-top", style_border_line_width_top_);
    CP_APPLY_ATTR(L"style:border-line-width-bottom", style_border_line_width_bottom_);
    CP_APPLY_ATTR(L"style:border-line-width-left", style_border_line_width_left_);
    CP_APPLY_ATTR(L"style:border-line-width-right", style_border_line_width_right_);

}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_padding_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:padding", fo_padding_);
    CP_APPLY_ATTR(L"fo:padding-top", fo_padding_top_);
    CP_APPLY_ATTR(L"fo:padding-bottom", fo_padding_bottom_);
    CP_APPLY_ATTR(L"fo:padding-left", fo_padding_left_);
    CP_APPLY_ATTR(L"fo:padding-right", fo_padding_right_);

}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_rotation_angle_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:rotation-angle", style_rotation_angle_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_text_align::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:text-align", fo_text_align_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_size_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:width", svg_width_);
    CP_APPLY_ATTR(L"svg:height", svg_height_);
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

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_text_anchor_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:anchor-type", text_anchor_type_);
    CP_APPLY_ATTR(L"text:anchor-page-number", text_anchor_page_number_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_vertical_rel_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:vertical-rel", style_vertical_rel_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_horizontal_rel_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:horizontal-rel", style_horizontal_rel_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_vertical_pos_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:vertical-pos", style_vertical_pos_);
    CP_APPLY_ATTR(L"svg:y", svg_y_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_horizontal_pos_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:horizontal-pos", style_horizontal_pos_);
    CP_APPLY_ATTR(L"svg:x", svg_x_);
}

//  common_num_format_attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_num_format_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:num-format", style_num_format_, std::wstring(L""));
    CP_APPLY_ATTR(L"style:num-letter-sync", style_num_letter_sync_);    
}

//  common_num_format_prefix_suffix_attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_num_format_prefix_suffix_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:num-prefix", style_num_prefix_);
    CP_APPLY_ATTR(L"style:num-suffix", style_num_suffix_);
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

/// common-draw-z-index-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_z_index_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:z-index", draw_z_index_);
}

/// common-draw-id-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_id_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:id", draw_id_);
}

// common-draw-layer-name-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_layer_name_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:layer", draw_layer_);
}

// common-draw-transform-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_transform_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:transform", draw_transform_);
}

// common-draw-name-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_name_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:name", draw_name_);
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

/// common-draw-text-style-name-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_text_style_name_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:text-style-name", draw_text_style_name_);
}

/// common-draw-shape-with-text-and-styles-attlist

void common_draw_shape_with_text_and_styles_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_draw_shape_with_styles_attlist_.add_attributes(Attributes);
    common_draw_text_style_name_attlist_.add_attributes(Attributes);
}

/// common-draw-position-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_draw_position_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:x", svg_x_);
    CP_APPLY_ATTR(L"svg:y", svg_y_);
}

/// presentation-shape-attlist

void presentation_shape_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"presentation:class", presentation_class_);
    CP_APPLY_ATTR(L"presentation:placeholder", presentation_placeholder_);
    CP_APPLY_ATTR(L"presentation:user-transformed", presentation_user_transformed_);
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

//////////////////////////////////////////////////////////////////////////////////////////////////

void common_presentation_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"presentation:class", presentation_class_);
    CP_APPLY_ATTR(L"presentation:style-name", style_name_);
	CP_APPLY_ATTR(L"presentation:placeholder", presentation_placeholder_);
}


}
}
