#pragma once

#include <iosfwd>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "common_attlists.h"

#include "fontvariant.h"
#include "texttransform.h"
#include "color.h"
#include "linetype.h"
#include "linestyle.h"
#include "linewidth.h"
#include "textposition.h"
#include "fontfamilygeneric.h"
#include "fontpitch.h"
#include "fontsize.h"
#include "scripttype.h"
#include "letterspacing.h"
#include "fontstyle.h"
#include "shadowtype.h"
#include "fontweight.h"
#include "linemode.h"
#include "backgroundcolor.h"
#include "underlinecolor.h"
#include "fontrelief.h"
#include "textcombine.h"
#include "textemphasize.h"
#include "textrotationscale.h"
#include "textdisplay.h"

namespace cpdoccore { 
namespace odf {


// 15.4
class text_format_properties_content 
{
public:
	void serialize(std::wostream & strm,const wchar_t * ns, const wchar_t * name );

    void apply_from(const text_format_properties_content & Other);
    
	void set_r_style(const std::wstring & rStyle) { r_style_ = rStyle; }

private:
    //static double process_font_size_impl(const _CP_OPT(font_size) & FontSize, const style_instance * currnetStyle, bool Complex = false, double Mul = 1.0);
    //static int process_font_weight(const optional<font_weight>::Type & FontWeight);
    //static int process_font_style(const optional<font_style>::Type & FontStyle);
public:

    _CP_OPT(std::wstring)    r_style_;
    _CP_OPT(font_variant)    fo_font_variant_;               
    _CP_OPT(text_transform)  fo_text_transform_;             
    _CP_OPT(color)           fo_color_;                      
    
	_CP_OPT(Bool)            style_use_window_font_color_;
    _CP_OPT(Bool)            style_text_outline_;            
   
	_CP_OPT(line_type)       style_text_line_through_type_;  
    _CP_OPT(line_style)      style_text_line_through_style_; 
    _CP_OPT(line_width)      style_text_line_through_width_;
    _CP_OPT(color)           style_text_line_through_color_;
    _CP_OPT(std::wstring)    style_text_line_through_text_;
    _CP_OPT(style_ref)       style_text_line_through_text_style_;
    
	_CP_OPT(text_position)   style_text_position_;           

    _CP_OPT(std::wstring)    style_font_name_;               
    _CP_OPT(std::wstring)    style_font_name_asian_;         
    _CP_OPT(std::wstring)    style_font_name_complex_;       
    
    _CP_OPT(std::wstring)    fo_font_family_;
    _CP_OPT(std::wstring)    style_font_family_asian_;
    _CP_OPT(std::wstring)    style_font_family_complex_;

    _CP_OPT(font_family_generic)    style_font_family_generic_;
    _CP_OPT(font_family_generic)    style_font_family_generic_asian_;
    _CP_OPT(font_family_generic)    style_font_family_generic_complex_;

    _CP_OPT(std::wstring)    style_font_style_name_;
    _CP_OPT(std::wstring)    style_font_style_name_asian_;
    _CP_OPT(std::wstring)    style_font_style_name_complex_;

    _CP_OPT(font_pitch) style_font_pitch_;
    _CP_OPT(font_pitch) style_font_pitch_asian_;
    _CP_OPT(font_pitch) style_font_pitch_complex_;

    _CP_OPT(std::wstring)    style_font_charset_;
    _CP_OPT(std::wstring)    style_font_charset_asian_;
    _CP_OPT(std::wstring)    style_font_charset_complex_;
    
    _CP_OPT(font_size)       fo_font_size_;              
    _CP_OPT(font_size)       style_font_size_asian_;
    _CP_OPT(font_size)       style_font_size_complex_;

    _CP_OPT(length)      style_font_size_rel_;
    _CP_OPT(length)      style_font_size_rel_asian_;
    _CP_OPT(length)      style_font_size_rel_complex_;

    _CP_OPT(script_type) style_script_type_;

    _CP_OPT(letter_spacing)  fo_letter_spacing_;         

    _CP_OPT(std::wstring)  fo_language_;                 
    _CP_OPT(std::wstring)  style_language_asian_;
    _CP_OPT(std::wstring)  style_language_complex_;

    _CP_OPT(std::wstring)	fo_country_;                  
    _CP_OPT(std::wstring)	style_country_asian_;
    _CP_OPT(std::wstring)	style_country_complex_;
           
    _CP_OPT(font_style)		fo_font_style_;                 
    _CP_OPT(font_style)		style_font_style_asian_;
    _CP_OPT(font_style)		style_font_style_complex_;

    _CP_OPT(font_relief)	style_font_relief_;             
    
    _CP_OPT(shadow_type)	fo_text_shadow_;               

    _CP_OPT(line_type)			style_text_underline_type_;      
    _CP_OPT(line_style)			style_text_underline_style_;    
    _CP_OPT(line_width)			style_text_underline_width_;      
    _CP_OPT(underline_color)	style_text_underline_color_; 
    
    _CP_OPT(font_weight)  fo_font_weight_;                   
    _CP_OPT(font_weight)  style_font_weight_asian_;
    _CP_OPT(font_weight)  style_font_weight_complex_;
    
    _CP_OPT(line_mode) style_text_underline_mode_;
    _CP_OPT(line_mode) style_text_line_through_mode_;

	_CP_OPT(Bool) style_letter_kerning_;
    
    _CP_OPT(Bool) style_text_blinking_;

    _CP_OPT(background_color) fo_background_color_;

    _CP_OPT(text_combine) style_text_combine_;

    _CP_OPT(wchar_t)		style_text_combine_start_char_;
    _CP_OPT(wchar_t)		style_text_combine_end_char_;

    _CP_OPT(text_emphasize) style_text_emphasize_;   
    _CP_OPT(percent)		style_text_scale_;          
    _CP_OPT(int)			style_text_rotation_angle_;

    _CP_OPT(text_rotation_scale) style_text_rotation_scale_;

    _CP_OPT(Bool)			fo_hyphenate_;
    _CP_OPT(unsigned int)	fo_hyphenation_remain_char_count_;
    _CP_OPT(unsigned int)	fo_hyphenation_push_char_count_;

    _CP_OPT(text_display) text_display_;                     

    _CP_OPT(std::wstring) text_condition_;

    _CP_OPT(std::wstring) style_text_overline_color_;
    _CP_OPT(std::wstring) style_text_overline_mode_;
    _CP_OPT(std::wstring) style_text_overline_style_;

	common_border_attlist common_border_attlist_;

};

class style_text_properties;
typedef shared_ptr<style_text_properties>::Type style_text_properties_ptr;

/// \class  style_text_properties
///         style:text-properties
class style_text_properties : public office_element_impl<style_text_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTextProperties;

    CPDOCCORE_DEFINE_VISITABLE();
    
	void apply_from(const style_text_properties * Other);

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element(office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);

    const text_format_properties_content & content() const { return text_format_properties_content_; } ;
    text_format_properties_content & content() { return text_format_properties_content_; } ;

    style_text_properties(){};
    style_text_properties(const std::wstring & rStyle){ text_format_properties_content_.set_r_style(rStyle); };
 
    text_format_properties_content text_format_properties_content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_text_properties);

}
}
