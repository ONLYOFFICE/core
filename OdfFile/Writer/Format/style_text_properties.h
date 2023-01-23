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
#pragma once

#include <iosfwd>
#include <xml/attributes.h>
#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"

#include "../../DataTypes/fontvariant.h"
#include "../../DataTypes/texttransform.h"
#include "../../DataTypes/color.h"
#include "../../DataTypes/linetype.h"
#include "../../DataTypes/linestyle.h"
#include "../../DataTypes/linewidth.h"
#include "../../DataTypes/textposition.h"
#include "../../DataTypes/fontfamilygeneric.h"
#include "../../DataTypes/fontpitch.h"
#include "../../DataTypes/fontsize.h"
#include "../../DataTypes/scripttype.h"
#include "../../DataTypes/letterspacing.h"
#include "../../DataTypes/fontstyle.h"
#include "../../DataTypes/shadowtype.h"
#include "../../DataTypes/fontweight.h"
#include "../../DataTypes/linemode.h"
#include "../../DataTypes/backgroundcolor.h"
#include "../../DataTypes/underlinecolor.h"
#include "../../DataTypes/fontrelief.h"
#include "../../DataTypes/textcombine.h"
#include "../../DataTypes/textemphasize.h"
#include "../../DataTypes/textrotationscale.h"
#include "../../DataTypes/textdisplay.h"

namespace cpdoccore { 
namespace odf_writer {


class text_format_properties_content 
{
public:
	void serialize(std::wostream & strm,const wchar_t * ns, const wchar_t * name );
    void apply_from(const text_format_properties_content & Other);    
	void set_r_style(const std::wstring & rStyle);

	void clear();

    _CP_OPT(std::wstring)				r_style_;
    _CP_OPT(odf_types::font_variant)    fo_font_variant_;               
    _CP_OPT(odf_types::text_transform)  fo_text_transform_;             
    _CP_OPT(odf_types::color)           fo_color_;                      
    
	_CP_OPT(odf_types::Bool)            style_use_window_font_color_;
    _CP_OPT(odf_types::Bool)            style_text_outline_;            
   
	_CP_OPT(odf_types::line_type)       style_text_line_through_type_;  
    _CP_OPT(odf_types::line_style)      style_text_line_through_style_; 
    _CP_OPT(odf_types::line_width)      style_text_line_through_width_;
    _CP_OPT(odf_types::color)           style_text_line_through_color_;
    _CP_OPT(std::wstring)				style_text_line_through_text_;
    _CP_OPT(std::wstring)				style_text_line_through_text_style_;
    
	_CP_OPT(odf_types::text_position)   style_text_position_;           

    _CP_OPT(std::wstring)				style_font_name_;               
    _CP_OPT(std::wstring)				style_font_name_asian_;         
    _CP_OPT(std::wstring)				style_font_name_complex_;       
    
    _CP_OPT(std::wstring)				fo_font_family_;
    _CP_OPT(std::wstring)				style_font_family_asian_;
    _CP_OPT(std::wstring)				style_font_family_complex_;

    _CP_OPT(odf_types::font_family_generic)    style_font_family_generic_;
    _CP_OPT(odf_types::font_family_generic)    style_font_family_generic_asian_;
    _CP_OPT(odf_types::font_family_generic)    style_font_family_generic_complex_;

    _CP_OPT(std::wstring)					style_font_style_name_;
    _CP_OPT(std::wstring)					style_font_style_name_asian_;
    _CP_OPT(std::wstring)					style_font_style_name_complex_;

    _CP_OPT(odf_types::font_pitch)			 style_font_pitch_;
    _CP_OPT(odf_types::font_pitch)			style_font_pitch_asian_;
    _CP_OPT(odf_types::font_pitch)			style_font_pitch_complex_;

    _CP_OPT(std::wstring)					style_font_charset_;
    _CP_OPT(std::wstring)					style_font_charset_asian_;
    _CP_OPT(std::wstring)					style_font_charset_complex_;
    
    _CP_OPT(odf_types::font_size)			fo_font_size_;              
    _CP_OPT(odf_types::font_size)			style_font_size_asian_;
    _CP_OPT(odf_types::font_size)			style_font_size_complex_;

    _CP_OPT(odf_types::length)				style_font_size_rel_;
    _CP_OPT(odf_types::length)				style_font_size_rel_asian_;
    _CP_OPT(odf_types::length)				style_font_size_rel_complex_;

    _CP_OPT(odf_types::script_type)			style_script_type_;

    _CP_OPT(odf_types::letter_spacing)		fo_letter_spacing_;         

    _CP_OPT(std::wstring)					fo_language_;                 
    _CP_OPT(std::wstring)					style_language_asian_;
    _CP_OPT(std::wstring)					style_language_complex_;

    _CP_OPT(std::wstring)					fo_country_;                  
    _CP_OPT(std::wstring)					style_country_asian_;
    _CP_OPT(std::wstring)					style_country_complex_;
           
    _CP_OPT(odf_types::font_style)			fo_font_style_;                 
    _CP_OPT(odf_types::font_style)			style_font_style_asian_;
    _CP_OPT(odf_types::font_style)			style_font_style_complex_;

    _CP_OPT(odf_types::font_relief)			style_font_relief_;             
    
    _CP_OPT(odf_types::shadow_type)			fo_text_shadow_;               

    _CP_OPT(odf_types::line_type)			style_text_underline_type_;      
    _CP_OPT(odf_types::line_style)			style_text_underline_style_;    
    _CP_OPT(odf_types::line_width)			style_text_underline_width_;      
    _CP_OPT(odf_types::underline_color)		style_text_underline_color_; 
    
    _CP_OPT(odf_types::font_weight)			fo_font_weight_;                   
    _CP_OPT(odf_types::font_weight)			style_font_weight_asian_;
    _CP_OPT(odf_types::font_weight)			style_font_weight_complex_;
    
    _CP_OPT(odf_types::line_mode)			style_text_underline_mode_;
    _CP_OPT(odf_types::line_mode)			style_text_line_through_mode_;

	_CP_OPT(odf_types::Bool)				style_letter_kerning_;
    
    _CP_OPT(odf_types::Bool)				style_text_blinking_;

    _CP_OPT(odf_types::background_color)	fo_background_color_;

    _CP_OPT(odf_types::text_combine)		style_text_combine_;

    _CP_OPT(wchar_t)						style_text_combine_start_char_;
    _CP_OPT(wchar_t)						style_text_combine_end_char_;

    _CP_OPT(odf_types::text_emphasize)		style_text_emphasize_;   
    _CP_OPT(odf_types::percent)				style_text_scale_;          
    _CP_OPT(int)							style_text_rotation_angle_;

    _CP_OPT(odf_types::text_rotation_scale) style_text_rotation_scale_;

    _CP_OPT(odf_types::Bool)				fo_hyphenate_;
    _CP_OPT(unsigned int)					fo_hyphenation_remain_char_count_;
    _CP_OPT(unsigned int)					fo_hyphenation_push_char_count_;

    _CP_OPT(odf_types::text_display)		text_display_;                     

    _CP_OPT(std::wstring)					text_condition_;

    _CP_OPT(std::wstring)					style_text_overline_color_;
    _CP_OPT(std::wstring)					style_text_overline_mode_;
    _CP_OPT(std::wstring)					style_text_overline_style_;

	odf_types::common_border_attlist common_border_attlist_;

};

class style_text_properties;
typedef shared_ptr<style_text_properties>::Type style_text_properties_ptr;

class style_text_properties : public office_element_impl<style_text_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleTextProperties;

    
    
	void apply_from(const style_text_properties * Other);

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);

    style_text_properties(){};
    style_text_properties(const std::wstring & rStyle){ content_.set_r_style(rStyle); };
 
    text_format_properties_content content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_text_properties);

}
}
