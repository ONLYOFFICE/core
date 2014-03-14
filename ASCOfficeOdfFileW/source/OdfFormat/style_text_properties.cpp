#include "precompiled_cpodf.h"

#include <iostream>
#include "style_text_properties.h"
#include <cpdoccore/xml/serialize.h>

#include "fontvariant.h"
#include "logging.h"

#include "common_attlists.h"

#include <cpdoccore/xml/simple_xml_writer.h>

//#include "../docx/oox_drawing_fills.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

std::wstring delete_apostroph_in_name(std::wstring value)
{
	if (value.length()<1)return value;

	if (value[0] == 0x27 && value[value.length()-1] == 0x27)
	{
		return value.substr(1,value.length()-2);
	}
	return value;
}

void text_format_properties_content::apply_from(const text_format_properties_content & Other)
{
    _CP_APPLY_PROP(r_style_, Other.r_style_);

    _CP_APPLY_PROP(fo_font_variant_, Other.fo_font_variant_);
    _CP_APPLY_PROP(fo_text_transform_, Other.fo_text_transform_);
    _CP_APPLY_PROP(fo_color_, Other.fo_color_);
    _CP_APPLY_PROP(style_use_window_font_color_, Other.style_use_window_font_color_);
    _CP_APPLY_PROP(style_text_outline_, Other.style_text_outline_);
    _CP_APPLY_PROP(style_text_line_through_type_, Other.style_text_line_through_type_); 
    _CP_APPLY_PROP(style_text_line_through_style_, Other.style_text_line_through_style_);
    
    apply_line_width(style_text_line_through_width_, Other.style_text_line_through_width_);
    
    _CP_APPLY_PROP(style_text_line_through_color_, Other.style_text_line_through_color_);
    _CP_APPLY_PROP(style_text_line_through_text_, Other.style_text_line_through_text_);
    _CP_APPLY_PROP(style_text_line_through_text_style_, Other.style_text_line_through_text_style_);
    _CP_APPLY_PROP(style_text_position_, Other.style_text_position_);
    _CP_APPLY_PROP(style_font_name_, Other.style_font_name_);
    _CP_APPLY_PROP(style_font_name_asian_, Other.style_font_name_asian_);
    _CP_APPLY_PROP(style_font_name_complex_, Other.style_font_name_complex_);
    _CP_APPLY_PROP(fo_font_family_, Other.fo_font_family_);
    _CP_APPLY_PROP(style_font_family_asian_, Other.style_font_family_asian_);
    _CP_APPLY_PROP(style_font_family_complex_, Other.style_font_family_complex_);

    _CP_APPLY_PROP(style_font_family_generic_, Other.style_font_family_generic_);
    _CP_APPLY_PROP(style_font_family_generic_asian_, Other.style_font_family_generic_asian_);
    _CP_APPLY_PROP(style_font_family_generic_complex_, Other.style_font_family_generic_complex_);

    _CP_APPLY_PROP(style_font_style_name_, Other.style_font_style_name_);
    _CP_APPLY_PROP(style_font_style_name_asian_, Other.style_font_style_name_asian_);
    _CP_APPLY_PROP(style_font_style_name_complex_, Other.style_font_style_name_complex_);

    _CP_APPLY_PROP(style_font_pitch_, Other.style_font_pitch_);
    _CP_APPLY_PROP(style_font_pitch_asian_, Other.style_font_pitch_asian_);
    _CP_APPLY_PROP(style_font_pitch_complex_, Other.style_font_pitch_complex_);

    _CP_APPLY_PROP(style_font_charset_, Other.style_font_charset_);
    _CP_APPLY_PROP(style_font_charset_asian_, Other.style_font_charset_asian_);
    _CP_APPLY_PROP(style_font_charset_complex_, Other.style_font_charset_complex_);
    
    // TODO check
    _CP_APPLY_PROP(style_font_size_rel_, Other.style_font_size_rel_);
    _CP_APPLY_PROP(style_font_size_rel_asian_, Other.style_font_size_rel_asian_);
    _CP_APPLY_PROP(style_font_size_rel_complex_, Other.style_font_size_rel_complex_);

    _CP_APPLY_PROP(style_script_type_, Other.style_script_type_);
    _CP_APPLY_PROP(fo_letter_spacing_, Other.fo_letter_spacing_);
    _CP_APPLY_PROP(fo_language_, Other.fo_language_);
    _CP_APPLY_PROP(style_language_asian_, Other.style_language_asian_);
    _CP_APPLY_PROP(style_language_complex_, Other.style_language_complex_);

    _CP_APPLY_PROP(fo_country_, Other.fo_country_);
    _CP_APPLY_PROP(style_country_asian_, Other.style_country_asian_);
    _CP_APPLY_PROP(style_country_complex_, Other.style_country_complex_);
           
    _CP_APPLY_PROP(fo_font_style_, Other.fo_font_style_);
    _CP_APPLY_PROP(style_font_style_asian_, Other.style_font_style_asian_);
    _CP_APPLY_PROP(style_font_style_complex_, Other.style_font_style_complex_);

    _CP_APPLY_PROP(style_font_relief_, Other.style_font_relief_);
    _CP_APPLY_PROP(fo_text_shadow_, Other.fo_text_shadow_);
    _CP_APPLY_PROP(style_text_underline_type_, Other.style_text_underline_type_);
    _CP_APPLY_PROP(style_text_underline_style_, Other.style_text_underline_style_);
    
    apply_line_width(style_text_underline_width_, Other.style_text_underline_width_);

    _CP_APPLY_PROP(style_text_underline_color_, Other.style_text_underline_color_);
    
    _CP_APPLY_PROP(fo_font_weight_, Other.fo_font_weight_);
    _CP_APPLY_PROP(style_font_weight_asian_, Other.style_font_weight_asian_);
    _CP_APPLY_PROP(style_font_weight_complex_, Other.style_font_weight_complex_);
    _CP_APPLY_PROP(style_text_underline_mode_, Other.style_text_underline_mode_);
    _CP_APPLY_PROP(style_text_line_through_mode_, Other.style_text_line_through_mode_);
    _CP_APPLY_PROP(style_letter_kerning_, Other.style_letter_kerning_);
    _CP_APPLY_PROP(style_text_blinking_, Other.style_text_blinking_);
    _CP_APPLY_PROP(fo_background_color_, Other.fo_background_color_);
    _CP_APPLY_PROP(style_text_combine_, Other.style_text_combine_);
    _CP_APPLY_PROP(style_text_combine_start_char_, Other.style_text_combine_start_char_);
    _CP_APPLY_PROP(style_text_combine_end_char_, Other.style_text_combine_end_char_);
    _CP_APPLY_PROP(style_text_emphasize_, Other.style_text_emphasize_);
    _CP_APPLY_PROP(style_text_scale_, Other.style_text_scale_);
    _CP_APPLY_PROP(style_text_rotation_angle_, Other.style_text_rotation_angle_);
    _CP_APPLY_PROP(style_text_rotation_scale_, Other.style_text_rotation_scale_);
    _CP_APPLY_PROP(fo_hyphenate_, Other.fo_hyphenate_);
    _CP_APPLY_PROP(fo_hyphenation_remain_char_count_, Other.fo_hyphenation_remain_char_count_);
    _CP_APPLY_PROP(fo_hyphenation_push_char_count_, Other.fo_hyphenation_push_char_count_);
    _CP_APPLY_PROP(text_display_, Other.text_display_);
    _CP_APPLY_PROP(text_condition_, Other.text_condition_);
    _CP_APPLY_PROP(style_text_overline_color_, Other.style_text_overline_color_);
    _CP_APPLY_PROP(style_text_overline_mode_, Other.style_text_overline_mode_);
    _CP_APPLY_PROP(style_text_overline_style_, Other.style_text_overline_style_);

}


// style:text-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_text_properties::ns = L"style";
const wchar_t * style_text_properties::name = L"text-properties";


void style_text_properties::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}


}
}

