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
#pragma once

#include <iosfwd>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "datatypes/style_ref.h"
#include "datatypes/fontvariant.h"
#include "datatypes/texttransform.h"
#include "datatypes/color.h"
#include "datatypes/linetype.h"
#include "datatypes/linestyle.h"
#include "datatypes/linewidth.h"
#include "datatypes/textposition.h"
#include "datatypes/fontfamilygeneric.h"
#include "datatypes/fontpitch.h"
#include "datatypes/fontsize.h"
#include "datatypes/scripttype.h"
#include "datatypes/letterspacing.h"
#include "datatypes/fontstyle.h"
#include "datatypes/shadowtype.h"
#include "datatypes/fontweight.h"
#include "datatypes/linemode.h"
#include "datatypes/backgroundcolor.h"
#include "datatypes/underlinecolor.h"
#include "datatypes/fontrelief.h"
#include "datatypes/textcombine.h"
#include "datatypes/textemphasize.h"
#include "datatypes/textrotationscale.h"
#include "datatypes/textdisplay.h"

namespace cpdoccore { 
namespace odf_reader {

class style_instance;

// 15.4
class text_format_properties_content : public oox::conversion_element
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);
	void pptx_convert_as_list(oox::pptx_conversion_context & Context);

    void apply_from(const text_format_properties_content & Other);
	void apply_to(std::vector<_property> & properties);
    void set_r_style(const std::wstring & rStyle) { r_style_ = rStyle; }
    int process_font_size(const optional<odf_types::font_size>::Type & FontSize, const style_instance * currnetStyle, bool Complex = false, double Mul = 1.0);

private:
    static double process_font_size_impl(const _CP_OPT(odf_types::font_size) & FontSize, const style_instance * currnetStyle, bool Complex = false, double Mul = 1.0);
    static int process_font_weight(const optional<odf_types::font_weight>::Type & FontWeight);
    static int process_font_style(const optional<odf_types::font_style>::Type & FontStyle);
public:

    _CP_OPT(std::wstring)    r_style_;

    // 15.4.1 fo:font-variant
    _CP_OPT(odf_types::font_variant)    fo_font_variant_;               // +
    
    // 15.4.2 fo:text-transform
    _CP_OPT(odf_types::text_transform)  fo_text_transform_;             // +
    
    // 15.4.3 fo:color
    _CP_OPT(odf_types::color)           fo_color_;                      // +
    
    // 15.4.4 style:use-window-font-color
    _CP_OPT(bool)            style_use_window_font_color_;
    
    // 15.4.5 style:text-outline
    _CP_OPT(bool)            style_text_outline_;            // +
    
    // 15.4.6 style:text-line-through-type
    _CP_OPT(odf_types::line_type)       style_text_line_through_type_;  // +
    
    // 15.4.7 style:text-line-through-style
    _CP_OPT(odf_types::line_style)      style_text_line_through_style_; // +
    
    // 15.4.8 style:text-line-through-width
    _CP_OPT(odf_types::line_width)      style_text_line_through_width_;
    
    // 15.4.9 style:text-line-through-color
    _CP_OPT(odf_types::color)           style_text_line_through_color_;
    
    // 15.4.10 style:text-line-through-text
    _CP_OPT(std::wstring)    style_text_line_through_text_;
    
    // 15.4.11 style:text-line-through-text-style
    _CP_OPT(odf_types::style_ref)       style_text_line_through_text_style_;

    // 15.4.12 style:text-position
    _CP_OPT(odf_types::text_position)   style_text_position_;           // +
    
    // 15.4.13 style:font-name
    _CP_OPT(std::wstring)    style_font_name_;               // +
    // style:font-name-asian
    _CP_OPT(std::wstring)    style_font_name_asian_;         // +
    // style:font-name-complex
    _CP_OPT(std::wstring)    style_font_name_complex_;       // +
    
    // 15.4.14 fo:font-family
    _CP_OPT(std::wstring)    fo_font_family_;
    // style:font-family-asian
    _CP_OPT(std::wstring)    style_font_family_asian_;
    // style:font-family-complex
    _CP_OPT(std::wstring)    style_font_family_complex_;

    // 15.4.15 style:font-family-generic
    _CP_OPT(odf_types::font_family_generic)    style_font_family_generic_;
    // style:font-family-generic-asian
    _CP_OPT(odf_types::font_family_generic)    style_font_family_generic_asian_;
    // style:font-family-generic-complex
    _CP_OPT(odf_types::font_family_generic)    style_font_family_generic_complex_;

    // 15.4.16 style:font-style-name
    _CP_OPT(std::wstring)    style_font_style_name_;
    // style:font-style-name-asian
    _CP_OPT(std::wstring)    style_font_style_name_asian_;
    // style:font-style-name-complex
    _CP_OPT(std::wstring)    style_font_style_name_complex_;

    // 15.4.17 style:font-pitch
    _CP_OPT(odf_types::font_pitch) style_font_pitch_;
    // style:font-pitch
    _CP_OPT(odf_types::font_pitch) style_font_pitch_asian_;
    // style:font-pitch-complex
    _CP_OPT(odf_types::font_pitch) style_font_pitch_complex_;

    // 15.4.18 style:font-charset
    _CP_OPT(std::wstring)    style_font_charset_;
    // style:font-charset-asian
    _CP_OPT(std::wstring)    style_font_charset_asian_;
    // style:font-charset-complex
    _CP_OPT(std::wstring)    style_font_charset_complex_;
    
    // 15.4.19 fo:font-size
    _CP_OPT(odf_types::font_size)       fo_font_size_;              // +
    // style:font-size-asian
    _CP_OPT(odf_types::font_size)       style_font_size_asian_;
    // style:font-size-complex
    _CP_OPT(odf_types::font_size)       style_font_size_complex_;

    // 15.4.20 style:font-size-rel
    _CP_OPT(odf_types::length)      style_font_size_rel_;
    // style:font-size-rel-asian
    _CP_OPT(odf_types::length)      style_font_size_rel_asian_;
    // style:font-size-rel-complex
    _CP_OPT(odf_types::length)      style_font_size_rel_complex_;

    // 15.4.21 style:script-type
    _CP_OPT(odf_types::script_type) style_script_type_;

    // 15.4.22 fo:letter-spacing
    _CP_OPT(odf_types::letter_spacing)  fo_letter_spacing_;         // +

    // 15.4.23 fo:language
    _CP_OPT(std::wstring)  fo_language_;                 // +
    // style:language-asian
    _CP_OPT(std::wstring)  style_language_asian_;
    // style:language-complex
    _CP_OPT(std::wstring)  style_language_complex_;

    // 15.4.24 fo:country
    _CP_OPT(std::wstring)  fo_country_;                  // +
    // style:country-asian
    _CP_OPT(std::wstring)  style_country_asian_;
    // style:country-complex
    _CP_OPT(std::wstring)  style_country_complex_;
           
    // 15.4.25 fo:font-style
    _CP_OPT(odf_types::font_style)  fo_font_style_;                 // +
    // style:font-style-asian
    _CP_OPT(odf_types::font_style)  style_font_style_asian_;
    // style:font-style-complex
    _CP_OPT(odf_types::font_style)  style_font_style_complex_;

    // 15.4.26 style:font-relief
    _CP_OPT(odf_types::font_relief) style_font_relief_;             // +
    
    // 15.4.27 fo:text-shadow
    _CP_OPT(odf_types::shadow_type)  fo_text_shadow_;               // +

    // 15.4.28 style:text-underline-type
    _CP_OPT(odf_types::line_type)  style_text_underline_type_;      // +
    
    // 15.4.29 style:text-underline-style
    _CP_OPT(odf_types::line_style)  style_text_underline_style_;    // +
    
    // 15.4.30 style:text-underline-width
    _CP_OPT(odf_types::line_width)  style_text_underline_width_;        // +

    // 15.4.31 style:text-underline-color
    _CP_OPT(odf_types::underline_color)  style_text_underline_color_; // +
    
    // 15.4.32 fo:font-weight
    _CP_OPT(odf_types::font_weight)  fo_font_weight_;                   // +
    // style:font-weight-asian
    _CP_OPT(odf_types::font_weight)  style_font_weight_asian_;
    // style:font-weight-complex
    _CP_OPT(odf_types::font_weight)  style_font_weight_complex_;
    
    // 15.4.33 style:text-underline-mode
    _CP_OPT(odf_types::line_mode) style_text_underline_mode_;
    
    // 15.4.34 style:text-line-through-mode
    _CP_OPT(odf_types::line_mode) style_text_line_through_mode_;
    
    // 15.4.35 style:letter-kerning
    _CP_OPT(bool) style_letter_kerning_;
    
    // 15.4.36 style:text-blinking
    _CP_OPT(bool) style_text_blinking_;

    // 15.4.37 fo:background-color
    _CP_OPT(odf_types::background_color) fo_background_color_;

    // 15.4.38 style:text-combine
    _CP_OPT(odf_types::text_combine) style_text_combine_;

    // 15.4.39 style:text-combine-start-char
    _CP_OPT(wchar_t) style_text_combine_start_char_;

    //style:text-combine-end-char
    _CP_OPT(wchar_t) style_text_combine_end_char_;

    // 15.4.40 style:text-emphasize
    _CP_OPT(odf_types::text_emphasize) style_text_emphasize_;   // +

    // 15.4.41 style:text-scale
    _CP_OPT(odf_types::percent) style_text_scale_;              // +

    // 15.4.42 style:text-rotation-angle
    _CP_OPT(int) style_text_rotation_angle_;

    // 15.4.43 style:text-rotation-scale
    _CP_OPT(odf_types::text_rotation_scale) style_text_rotation_scale_;

    // 15.4.44 fo:hyphenate
    _CP_OPT(bool) fo_hyphenate_;

    // 15.4.45 fo:hyphenation-remain-char-count
    _CP_OPT(unsigned int) fo_hyphenation_remain_char_count_;

    // 15.4.46 fo:hyphenation-push-char-count
    _CP_OPT(unsigned int) fo_hyphenation_push_char_count_;

    // 15.4.47 text:display 
    _CP_OPT(odf_types::text_display) text_display_;                     // +

    // text:condition
    _CP_OPT(std::wstring) text_condition_;

    // style:text-overline-color
    _CP_OPT(std::wstring) style_text_overline_color_;

    // style:text-overline-mode
    _CP_OPT(std::wstring) style_text_overline_mode_;

    // style:text-overline-style
    _CP_OPT(std::wstring) style_text_overline_style_;

};

class style_text_properties;
typedef shared_ptr<style_text_properties>::Type style_text_properties_ptr;


///         style:text-properties
class style_text_properties : public office_element_impl<style_text_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTextProperties;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

    const text_format_properties_content & content() const { return text_format_properties_content_; } ;
    text_format_properties_content & content() { return text_format_properties_content_; } ;

public:
    style_text_properties(){};
    style_text_properties(const std::wstring & rStyle){ text_format_properties_content_.set_r_style(rStyle); };
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
 
private:
    text_format_properties_content text_format_properties_content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_text_properties);

}
}
