#include "precompiled_cpodf.h"
#include <iostream>
#include "style_paragraph_properties.h"

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>

#include <boost/foreach.hpp>


#include "borderstyle.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// style:tab-stop
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_tab_stop::ns = L"style";
const wchar_t * style_tab_stop::name = L"tab-stop";


// style:tab-stop
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_tab_stops::ns = L"style";
const wchar_t * style_tab_stops::name = L"tab-stops";

// style:drop-cap
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_drop_cap::ns = L"style";
const wchar_t * style_drop_cap::name = L"drop-cap";


// style:background-image
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_background_image::ns = L"style";
const wchar_t * style_background_image::name = L"background-image";

//////////////////////////////////////////////////////////////////////////////////////////////////
void paragraph_format_properties::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name,odf_conversion_context * Context)
{
    if (L"style" == Ns)
    {
        // 15.5.11 <style:tab-stops>
        if (L"tab-stops" == Name)
        {
            CP_CREATE_ELEMENT_SIMPLE(style_tab_stops_);
        } 
        // 15.5.15  <style:drop-cap>
        else if (L"drop-cap" == Name)
        {
            CP_CREATE_ELEMENT_SIMPLE(style_drop_cap_);
        }
        // 15.5.24 <style:background-image>
        else if (L"background-image" == Name)
        {
            CP_CREATE_ELEMENT_SIMPLE(style_background_image_);
        }
		//else
		//	CP_NOT_APPLICABLE_ELM();
    }
}


// style:paragraph-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_paragraph_properties::ns = L"style";
const wchar_t * style_paragraph_properties::name = L"paragraph-properties";


void style_paragraph_properties::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    style_paragraph_properties_content_.create_child_element(Ns, Name, getContext());    
}

void paragraph_format_properties::apply_from(const paragraph_format_properties & Other)
{
    apply_line_width(fo_line_height_, Other.fo_line_height_);
    _CP_APPLY_PROP(style_line_height_at_least_, Other.style_line_height_at_least_);
    _CP_APPLY_PROP(style_line_spacing_, Other.style_line_spacing_);
    _CP_APPLY_PROP(style_font_independent_line_spacing_, Other.style_font_independent_line_spacing_);
    _CP_APPLY_PROP(fo_text_align_, Other.fo_text_align_);
    _CP_APPLY_PROP(fo_text_align_last_, Other.fo_text_align_last_);
    _CP_APPLY_PROP(style_justify_single_word_, Other.style_justify_single_word_);
    _CP_APPLY_PROP(fo_keep_together_, Other.fo_keep_together_);
    _CP_APPLY_PROP(fo_widows_, Other.fo_widows_);
    _CP_APPLY_PROP(fo_orphans_, Other.fo_orphans_);

    // TODO
    _CP_APPLY_PROP(style_tab_stops_, Other.style_tab_stops_);

    _CP_APPLY_PROP(style_tab_stop_distance_, Other.style_tab_stop_distance_);
    _CP_APPLY_PROP(fo_hyphenation_keep_, Other.fo_hyphenation_keep_);
    _CP_APPLY_PROP(fo_hyphenation_ladder_count_, Other.fo_hyphenation_ladder_count_);
    
    // TODO
    _CP_APPLY_PROP(style_drop_cap_, Other.style_drop_cap_);
    
    _CP_APPLY_PROP(style_register_true_, Other.style_register_true_);
    apply_length_or_percent(fo_margin_left_, Other.fo_margin_left_);
    apply_length_or_percent(fo_margin_right_, Other.fo_margin_right_);
    apply_length_or_percent(fo_text_indent_, Other.fo_text_indent_);

    _CP_APPLY_PROP(style_auto_text_indent_, Other.style_auto_text_indent_);

    apply_length_or_percent(fo_margin_top_, Other.fo_margin_top_);
    apply_length_or_percent(fo_margin_bottom_, Other.fo_margin_bottom_);
    apply_length_or_percent(fo_margin_, Other.fo_margin_);
    
    _CP_APPLY_PROP(fo_break_before_, Other.fo_break_before_);
    _CP_APPLY_PROP(fo_break_after_, Other.fo_break_after_);

    _CP_APPLY_PROP(fo_background_color_, Other.fo_background_color_);

    // TODO
    _CP_APPLY_PROP(style_background_image_, Other.style_background_image_);

    _CP_APPLY_PROP(fo_border_, Other.fo_border_);
    _CP_APPLY_PROP(fo_border_top_, Other.fo_border_top_);
    _CP_APPLY_PROP(fo_border_bottom_, Other.fo_border_bottom_);
    _CP_APPLY_PROP(fo_border_left_, Other.fo_border_left_);
    _CP_APPLY_PROP(fo_border_right_, Other.fo_border_right_);
    _CP_APPLY_PROP(style_border_line_width_, Other.style_border_line_width_);
    _CP_APPLY_PROP(style_border_line_width_top_, Other.style_border_line_width_top_);
    _CP_APPLY_PROP(style_border_line_width_bottom_, Other.style_border_line_width_bottom_);
    _CP_APPLY_PROP(style_border_line_width_left_, Other.style_border_line_width_left_);
    _CP_APPLY_PROP(style_border_line_width_right_, Other.style_border_line_width_right_);
    _CP_APPLY_PROP(fo_padding_, Other.fo_padding_);
    _CP_APPLY_PROP(fo_padding_top_, Other.fo_padding_top_);
    _CP_APPLY_PROP(fo_padding_bottom_, Other.fo_padding_bottom_);
    _CP_APPLY_PROP(fo_padding_left_, Other.fo_padding_left_);
    _CP_APPLY_PROP(fo_padding_right_, Other.fo_padding_right_);
    _CP_APPLY_PROP(style_shadow_, Other.style_shadow_);
    _CP_APPLY_PROP(fo_keep_with_next_, Other.fo_keep_with_next_);
    _CP_APPLY_PROP(text_number_lines_, Other.text_number_lines_);
    _CP_APPLY_PROP(text_line_number_, Other.text_line_number_);
    _CP_APPLY_PROP(style_text_autospace_, Other.style_text_autospace_);
    _CP_APPLY_PROP(style_punctuation_wrap_, Other.style_punctuation_wrap_);
    _CP_APPLY_PROP(style_line_break_, Other.style_line_break_);
    _CP_APPLY_PROP(style_vertical_align_, Other.style_vertical_align_);
    _CP_APPLY_PROP(style_writing_mode_, Other.style_writing_mode_);
    _CP_APPLY_PROP(style_writing_mode_automatic_, Other.style_writing_mode_automatic_);
    _CP_APPLY_PROP(style_snap_to_layout_grid_, Other.style_snap_to_layout_grid_);
    _CP_APPLY_PROP(style_page_number_, Other.style_page_number_);
    _CP_APPLY_PROP(style_background_transparency_, Other.style_join_border_);
    _CP_APPLY_PROP(style_join_border_, Other.style_join_border_);
}


}
}
