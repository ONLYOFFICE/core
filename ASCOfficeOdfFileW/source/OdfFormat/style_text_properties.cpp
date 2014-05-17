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
void text_format_properties_content::serialize(std::wostream & _Wostream,const wchar_t * ns, const wchar_t * name  )
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
		// 15.4.1
			CP_XML_ATTR_OPT(L"fo:font-variant", fo_font_variant_);
		    
			// 15.4.2
			CP_XML_ATTR_OPT(L"fo:text-transform", fo_text_transform_);
		    
			// 15.4.3 
			CP_XML_ATTR_OPT(L"fo:color", fo_color_);
		    
			// 15.4.4 
			CP_XML_ATTR_OPT(L"style:use-window-font-color", style_use_window_font_color_);
		    
			// 15.4.5 
			CP_XML_ATTR_OPT(L"style:text-outline", style_text_outline_);
		    
			// 15.4.6 
			CP_XML_ATTR_OPT(L"style:text-line-through-type", style_text_line_through_type_);
		    
			// 15.4.7 
			CP_XML_ATTR_OPT(L"style:text-line-through-style", style_text_line_through_style_);
		    
			// 15.4.8 
			CP_XML_ATTR_OPT(L"style:text-line-through-width", style_text_line_through_width_);
		     
			// 15.4.9 
			CP_XML_ATTR_OPT(L"style:text-line-through-color", style_text_line_through_color_);
		    
			// 15.4.10
			CP_XML_ATTR_OPT(L"style:text-line-through-text", style_text_line_through_text_);
		    
			// 15.4.11 
			CP_XML_ATTR_OPT(L"style:text-line-through-text-style", style_text_line_through_text_style_);

			// 15.4.12 
			CP_XML_ATTR_OPT(L"style:text-position", style_text_position_);
		    
			// 15.4.13 
			CP_XML_ATTR_OPT(L"style:font-name", style_font_name_);
			CP_XML_ATTR_OPT(L"style:font-name-asian", style_font_name_asian_);
			CP_XML_ATTR_OPT(L"style:font-name-complex", style_font_name_complex_);
		    
			// 15.4.14 
			CP_XML_ATTR_OPT(L"fo:font-family", fo_font_family_);
			CP_XML_ATTR_OPT(L"style:font-family-asian", style_font_family_asian_);
			CP_XML_ATTR_OPT(L"style:font-family-complex", style_font_family_complex_);

			// 15.4.15 
			CP_XML_ATTR_OPT(L"style:font-family-generic", style_font_family_generic_);
			CP_XML_ATTR_OPT(L"style:font-family-generic-asian", style_font_family_generic_asian_);
			CP_XML_ATTR_OPT(L"style:font-family-generic-complex", style_font_family_generic_complex_);

			// 15.4.16 
			CP_XML_ATTR_OPT(L"style:font-style-name", style_font_style_name_);
			CP_XML_ATTR_OPT(L"style:font-style-name-asian", style_font_style_name_asian_);
			CP_XML_ATTR_OPT(L"style:font-style-name-complex", style_font_style_name_complex_);

			// 15.4.17 
			CP_XML_ATTR_OPT(L"style:font-pitch", style_font_pitch_);
			CP_XML_ATTR_OPT(L"style:font-pitch", style_font_pitch_asian_);
			CP_XML_ATTR_OPT(L"style:font-pitch-complex", style_font_pitch_complex_);

			// 15.4.18 
			CP_XML_ATTR_OPT(L"style:font-charset", style_font_charset_);
			CP_XML_ATTR_OPT(L"style:font-charset-asian", style_font_charset_asian_);
			CP_XML_ATTR_OPT(L"style:font-charset-complex", style_font_charset_complex_);
		    
			// 15.4.19 
			CP_XML_ATTR_OPT(L"fo:font-size", fo_font_size_);
			CP_XML_ATTR_OPT(L"style:font-size-asian", style_font_size_asian_);
			CP_XML_ATTR_OPT(L"style:font-size-complex", style_font_size_complex_);

			// 15.4.20 
			CP_XML_ATTR_OPT(L"style:font-size-rel", style_font_size_rel_);
			CP_XML_ATTR_OPT(L"style:font-size-rel-asian", style_font_size_rel_asian_);
			CP_XML_ATTR_OPT(L"style:font-size-rel-complex", style_font_size_rel_complex_);

			// 15.4.21 
			CP_XML_ATTR_OPT(L"style:script-type", style_script_type_);

			// 15.4.22 
			CP_XML_ATTR_OPT(L"fo:letter-spacing", fo_letter_spacing_);

			// 15.4.23 
			CP_XML_ATTR_OPT(L"fo:language", fo_language_);
			CP_XML_ATTR_OPT(L"style:language-asian", style_language_asian_);
			CP_XML_ATTR_OPT(L"style:language-complex", style_language_complex_);

			// 15.4.24 
			CP_XML_ATTR_OPT(L"fo:country", fo_country_);
			CP_XML_ATTR_OPT(L"style:country-asian", style_country_asian_);
			CP_XML_ATTR_OPT(L"style:country-complex", style_country_complex_);
		           
			// 15.4.25 
			CP_XML_ATTR_OPT(L"fo:font-style", fo_font_style_);
			CP_XML_ATTR_OPT(L"style:font-style-asian", style_font_style_asian_);
			CP_XML_ATTR_OPT(L"style:font-style-complex", style_font_style_complex_);

			// 15.4.26 
			CP_XML_ATTR_OPT(L"style:font-relief", style_font_relief_);
		    
			// 15.4.27 
			CP_XML_ATTR_OPT(L"fo:text-shadow", fo_text_shadow_);

			// 15.4.28 
			CP_XML_ATTR_OPT(L"style:text-underline-type", style_text_underline_type_);
		    
			// 15.4.29 
			CP_XML_ATTR_OPT(L"style:text-underline-style", style_text_underline_style_);
		    
			// 15.4.30 
			CP_XML_ATTR_OPT(L"style:text-underline-width", style_text_underline_width_);

			// 15.4.31 
			CP_XML_ATTR_OPT(L"style:text-underline-color", style_text_underline_color_);
		    
			// 15.4.32 
			CP_XML_ATTR_OPT(L"fo:font-weight", fo_font_weight_);
			CP_XML_ATTR_OPT(L"style:font-weight-asian", style_font_weight_asian_);
			CP_XML_ATTR_OPT(L"style:font-weight-complex", style_font_weight_complex_);
		   
			// 15.4.33 
			CP_XML_ATTR_OPT(L"style:text-underline-mode", style_text_underline_mode_);
		    
			// 15.4.34 
			CP_XML_ATTR_OPT(L"style:text-line-through-mode", style_text_line_through_mode_);
		    
			// 15.4.35 
			CP_XML_ATTR_OPT(L"style:letter-kerning", style_letter_kerning_);
		    
			// 15.4.36 
			CP_XML_ATTR_OPT(L"style:text-blinking", style_text_blinking_);

			// 15.4.37 
			CP_XML_ATTR_OPT(L"fo:background-color", fo_background_color_);

			// 15.4.38 
			CP_XML_ATTR_OPT(L"style:text-combine", style_text_combine_);

			// 15.4.39 
			CP_XML_ATTR_OPT(L"style:text-combine-start-char", style_text_combine_start_char_);
			CP_XML_ATTR_OPT(L"style:text-combine-end-char", style_text_combine_end_char_);

			// 15.4.40 
			CP_XML_ATTR_OPT(L"style:text-emphasize", style_text_emphasize_);

			// 15.4.41 
			CP_XML_ATTR_OPT(L"style:text-scale", style_text_scale_);

			// 15.4.42 
			CP_XML_ATTR_OPT(L"style:text-rotation-angle", style_text_rotation_angle_);

			// 15.4.43 
			CP_XML_ATTR_OPT(L"style:text-rotation-scale", style_text_rotation_scale_);

			// 15.4.44 
			CP_XML_ATTR_OPT(L"fo:hyphenate", fo_hyphenate_);

			// 15.4.45 
			CP_XML_ATTR_OPT(L"fo:hyphenation-remain-char-count", fo_hyphenation_remain_char_count_);

			// 15.4.46
			CP_XML_ATTR_OPT(L"fo:hyphenation-push-char-count", fo_hyphenation_push_char_count_);

			// 15.4.47 
			CP_XML_ATTR_OPT(L"text:display", text_display_);
			CP_XML_ATTR_OPT(L"text:condition", text_condition_);

			CP_XML_ATTR_OPT(L"style:text-overline-color", style_text_overline_color_);
			CP_XML_ATTR_OPT(L"style:text-overline-mode", style_text_overline_mode_);
			CP_XML_ATTR_OPT(L"style:text-overline-style", style_text_overline_style_);
		}
	}
}

// style:text-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_text_properties::ns = L"style";
const wchar_t * style_text_properties::name = L"text-properties";

void style_text_properties::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void style_text_properties::serialize(std::wostream & strm)
{
	text_format_properties_content_.serialize(strm,ns,name);
}

}
}

