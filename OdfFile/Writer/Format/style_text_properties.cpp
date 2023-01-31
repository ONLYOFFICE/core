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
#include <xml/simple_xml_writer.h>
#include <logging.h>

#include "style_text_properties.h"


#include "../../DataTypes/fontvariant.h"
#include "../../DataTypes/common_attlists.h"



//#include "../docx/oox_drawing_fills.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

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
void text_format_properties_content::set_r_style(const std::wstring & rStyle) 
{ 
	r_style_ = rStyle; 
}
void text_format_properties_content::apply_from(const text_format_properties_content & Other)
{
    _CP_APPLY_PROP( r_style_,							Other.r_style_);

    _CP_APPLY_PROP( fo_font_variant_,					Other.fo_font_variant_);
    _CP_APPLY_PROP( fo_text_transform_,					Other.fo_text_transform_);
    _CP_APPLY_PROP( fo_color_,							Other.fo_color_);
    _CP_APPLY_PROP( style_use_window_font_color_,		Other.style_use_window_font_color_);
    _CP_APPLY_PROP( style_text_outline_,				Other.style_text_outline_);
    _CP_APPLY_PROP( style_text_line_through_type_,		Other.style_text_line_through_type_); 
    _CP_APPLY_PROP( style_text_line_through_style_,		Other.style_text_line_through_style_);
    
    apply_line_width(style_text_line_through_width_,	Other.style_text_line_through_width_);
    
    _CP_APPLY_PROP( style_text_line_through_color_,		Other.style_text_line_through_color_);
    _CP_APPLY_PROP( style_text_line_through_text_,		Other.style_text_line_through_text_);
    _CP_APPLY_PROP( style_text_line_through_text_style_, Other.style_text_line_through_text_style_);
    _CP_APPLY_PROP( style_text_position_,				Other.style_text_position_);
    _CP_APPLY_PROP( style_font_name_,					Other.style_font_name_);
    _CP_APPLY_PROP( style_font_name_asian_,				Other.style_font_name_asian_);
    _CP_APPLY_PROP( style_font_name_complex_,			Other.style_font_name_complex_);
    _CP_APPLY_PROP( fo_font_family_,					Other.fo_font_family_);
    _CP_APPLY_PROP( style_font_family_asian_,			Other.style_font_family_asian_);
    _CP_APPLY_PROP( style_font_family_complex_,			Other.style_font_family_complex_);

    _CP_APPLY_PROP( style_font_family_generic_,			Other.style_font_family_generic_);
    _CP_APPLY_PROP( style_font_family_generic_asian_,	Other.style_font_family_generic_asian_);
    _CP_APPLY_PROP( style_font_family_generic_complex_,	Other.style_font_family_generic_complex_);

    _CP_APPLY_PROP( style_font_style_name_,				Other.style_font_style_name_);
    _CP_APPLY_PROP( style_font_style_name_asian_,		Other.style_font_style_name_asian_);
    _CP_APPLY_PROP( style_font_style_name_complex_,		Other.style_font_style_name_complex_);

    _CP_APPLY_PROP( style_font_pitch_,					Other.style_font_pitch_);
    _CP_APPLY_PROP( style_font_pitch_asian_,			Other.style_font_pitch_asian_);
    _CP_APPLY_PROP( style_font_pitch_complex_,			Other.style_font_pitch_complex_);

    _CP_APPLY_PROP( style_font_charset_,				Other.style_font_charset_);
    _CP_APPLY_PROP( style_font_charset_asian_,			Other.style_font_charset_asian_);
    _CP_APPLY_PROP( style_font_charset_complex_,		Other.style_font_charset_complex_);
    
	_CP_APPLY_PROP( fo_font_size_,						Other.fo_font_size_);
	_CP_APPLY_PROP( style_font_size_asian_,				Other.style_font_size_asian_);
	_CP_APPLY_PROP( style_font_size_complex_,			Other.style_font_size_complex_);
	
	_CP_APPLY_PROP( style_font_size_rel_,				Other.style_font_size_rel_);
    _CP_APPLY_PROP( style_font_size_rel_asian_,			Other.style_font_size_rel_asian_);
    _CP_APPLY_PROP( style_font_size_rel_complex_,		Other.style_font_size_rel_complex_);

    _CP_APPLY_PROP( style_script_type_,					Other.style_script_type_);
    _CP_APPLY_PROP( fo_letter_spacing_,					Other.fo_letter_spacing_);
    _CP_APPLY_PROP( fo_language_,						Other.fo_language_);
    _CP_APPLY_PROP( style_language_asian_,				Other.style_language_asian_);
    _CP_APPLY_PROP( style_language_complex_,			Other.style_language_complex_);

    _CP_APPLY_PROP( fo_country_,						Other.fo_country_);
    _CP_APPLY_PROP( style_country_asian_,				Other.style_country_asian_);
    _CP_APPLY_PROP( style_country_complex_,				Other.style_country_complex_);
           
    _CP_APPLY_PROP( fo_font_style_,						Other.fo_font_style_);
    _CP_APPLY_PROP( style_font_style_asian_,			Other.style_font_style_asian_);
    _CP_APPLY_PROP( style_font_style_complex_,			Other.style_font_style_complex_);

    _CP_APPLY_PROP( style_font_relief_,					Other.style_font_relief_);
    _CP_APPLY_PROP( fo_text_shadow_,					Other.fo_text_shadow_);
    _CP_APPLY_PROP( style_text_underline_type_,			Other.style_text_underline_type_);
    _CP_APPLY_PROP( style_text_underline_style_,		Other.style_text_underline_style_);
    
    apply_line_width(style_text_underline_width_,		Other.style_text_underline_width_);

    _CP_APPLY_PROP( style_text_underline_color_,		Other.style_text_underline_color_);
    
    _CP_APPLY_PROP( fo_font_weight_,					Other.fo_font_weight_);
    _CP_APPLY_PROP( style_font_weight_asian_,			Other.style_font_weight_asian_);
    _CP_APPLY_PROP( style_font_weight_complex_,			Other.style_font_weight_complex_);
    _CP_APPLY_PROP( style_text_underline_mode_,			Other.style_text_underline_mode_);
    _CP_APPLY_PROP( style_text_line_through_mode_,		Other.style_text_line_through_mode_);
    _CP_APPLY_PROP( style_letter_kerning_,				Other.style_letter_kerning_);
    _CP_APPLY_PROP( style_text_blinking_,				Other.style_text_blinking_);
    _CP_APPLY_PROP( fo_background_color_,				Other.fo_background_color_);
    _CP_APPLY_PROP( style_text_combine_,				Other.style_text_combine_);
    _CP_APPLY_PROP( style_text_combine_start_char_,		Other.style_text_combine_start_char_);
    _CP_APPLY_PROP( style_text_combine_end_char_,		Other.style_text_combine_end_char_);
    _CP_APPLY_PROP( style_text_emphasize_,				Other.style_text_emphasize_);
    _CP_APPLY_PROP( style_text_scale_,					Other.style_text_scale_);
    _CP_APPLY_PROP( style_text_rotation_angle_,			Other.style_text_rotation_angle_);
    _CP_APPLY_PROP( style_text_rotation_scale_,			Other.style_text_rotation_scale_);
    _CP_APPLY_PROP( fo_hyphenate_,						Other.fo_hyphenate_);
    _CP_APPLY_PROP( fo_hyphenation_remain_char_count_,	Other.fo_hyphenation_remain_char_count_);
    _CP_APPLY_PROP( fo_hyphenation_push_char_count_,	Other.fo_hyphenation_push_char_count_);
    _CP_APPLY_PROP( text_display_,						Other.text_display_);
    _CP_APPLY_PROP( text_condition_,					Other.text_condition_);
    _CP_APPLY_PROP( style_text_overline_color_,			Other.style_text_overline_color_);
    _CP_APPLY_PROP( style_text_overline_mode_,			Other.style_text_overline_mode_);
    _CP_APPLY_PROP( style_text_overline_style_,			Other.style_text_overline_style_);

	common_border_attlist_.apply_from(Other.common_border_attlist_);
}

void text_format_properties_content::clear()
{
    r_style_							= boost::none;
    fo_font_variant_					= boost::none;               
    fo_text_transform_					= boost::none;             
    fo_color_							= boost::none;                      
    
	style_use_window_font_color_		= boost::none;
    style_text_outline_					= boost::none;            
   
	style_text_line_through_type_		= boost::none;  
    style_text_line_through_style_		= boost::none; 
    style_text_line_through_width_		= boost::none;
    style_text_line_through_color_		= boost::none;
    style_text_line_through_text_		= boost::none;
    style_text_line_through_text_style_	= boost::none;
    
	style_text_position_				= boost::none;           

    style_font_name_					= boost::none;               
    style_font_name_asian_				= boost::none;         
    style_font_name_complex_			= boost::none;       
    
    fo_font_family_						= boost::none;
    style_font_family_asian_			= boost::none;
    style_font_family_complex_			= boost::none;

    style_font_family_generic_			= boost::none;
    style_font_family_generic_asian_	= boost::none;
    style_font_family_generic_complex_	= boost::none;

    style_font_style_name_				= boost::none;
    style_font_style_name_asian_		= boost::none;
    style_font_style_name_complex_		= boost::none;

    style_font_pitch_					= boost::none;
    style_font_pitch_asian_				= boost::none;
    style_font_pitch_complex_			= boost::none;

    style_font_charset_					= boost::none;
    style_font_charset_asian_			= boost::none;
    style_font_charset_complex_			= boost::none;
    
    fo_font_size_						= boost::none;              
    style_font_size_asian_				= boost::none;
    style_font_size_complex_			= boost::none;

    style_font_size_rel_				= boost::none;
    style_font_size_rel_asian_			= boost::none;
    style_font_size_rel_complex_		= boost::none;

    style_script_type_					= boost::none;

    fo_letter_spacing_					= boost::none;         

    fo_language_						= boost::none;                 
    style_language_asian_				= boost::none;
    style_language_complex_				= boost::none;

    fo_country_							= boost::none;                  
    style_country_asian_				= boost::none;
    style_country_complex_				= boost::none;
           
    fo_font_style_						= boost::none;                 
    style_font_style_asian_				= boost::none;
    style_font_style_complex_			= boost::none;

    style_font_relief_					= boost::none;             
    
    fo_text_shadow_						= boost::none;               

    style_text_underline_type_			= boost::none;      
    style_text_underline_style_			= boost::none;    
    style_text_underline_width_			= boost::none;      
    style_text_underline_color_			= boost::none; 
    
    fo_font_weight_						= boost::none;                   
    style_font_weight_asian_			= boost::none;
    style_font_weight_complex_			= boost::none;    
    style_text_underline_mode_			= boost::none;
    style_text_line_through_mode_		= boost::none;
	style_letter_kerning_				= boost::none;
    style_text_blinking_				= boost::none;
    fo_background_color_				= boost::none;

    style_text_combine_					= boost::none;

    style_text_combine_start_char_		= boost::none;
    style_text_combine_end_char_		= boost::none;

    style_text_emphasize_				= boost::none;   
    style_text_scale_					= boost::none;          
    style_text_rotation_angle_			= boost::none;

    style_text_rotation_scale_			= boost::none;

    fo_hyphenate_						= boost::none;
    fo_hyphenation_remain_char_count_	= boost::none;
    fo_hyphenation_push_char_count_		= boost::none;
    text_display_						= boost::none;                     

    text_condition_						= boost::none;

    style_text_overline_color_			= boost::none;
    style_text_overline_mode_			= boost::none;
    style_text_overline_style_			= boost::none;

	//todooo borders
}

void text_format_properties_content::serialize(std::wostream & _Wostream,const wchar_t * ns, const wchar_t * name  )
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT( L"fo:font-variant",				fo_font_variant_);
			CP_XML_ATTR_OPT( L"fo:text-transform",				fo_text_transform_);
			CP_XML_ATTR_OPT( L"fo:color",						fo_color_);

			CP_XML_ATTR_OPT( L"style:use-window-font-color",	style_use_window_font_color_);
			CP_XML_ATTR_OPT( L"style:text-outline",				style_text_outline_);
			CP_XML_ATTR_OPT( L"style:text-line-through-type",	style_text_line_through_type_);
			CP_XML_ATTR_OPT( L"style:text-line-through-style",	style_text_line_through_style_);
			CP_XML_ATTR_OPT( L"style:text-line-through-width",	style_text_line_through_width_);
			CP_XML_ATTR_OPT( L"style:text-line-through-color",	style_text_line_through_color_);
			CP_XML_ATTR_OPT( L"style:text-line-through-text",	style_text_line_through_text_);
			CP_XML_ATTR_OPT( L"style:text-line-through-text-style", style_text_line_through_text_style_);
			CP_XML_ATTR_OPT( L"style:text-position",			style_text_position_);
		    
			CP_XML_ATTR_OPT( L"style:font-name",				style_font_name_);
			CP_XML_ATTR_OPT( L"style:font-name-asian",			style_font_name_asian_);
			CP_XML_ATTR_OPT( L"style:font-name-complex",		style_font_name_complex_);
		    
			CP_XML_ATTR_OPT( L"fo:font-family",					fo_font_family_);
			CP_XML_ATTR_OPT( L"style:font-family-asian",		style_font_family_asian_);
			CP_XML_ATTR_OPT( L"style:font-family-complex",		style_font_family_complex_);

			CP_XML_ATTR_OPT( L"style:font-family-generic",			style_font_family_generic_);
			CP_XML_ATTR_OPT( L"style:font-family-generic-asian",	style_font_family_generic_asian_);
			CP_XML_ATTR_OPT( L"style:font-family-generic-complex",	style_font_family_generic_complex_);

			CP_XML_ATTR_OPT( L"style:font-style-name",			style_font_style_name_);
			CP_XML_ATTR_OPT( L"style:font-style-name-asian",	style_font_style_name_asian_);
			CP_XML_ATTR_OPT( L"style:font-style-name-complex",	style_font_style_name_complex_);

			CP_XML_ATTR_OPT( L"style:font-pitch",				style_font_pitch_);
			CP_XML_ATTR_OPT( L"style:font-pitch",				style_font_pitch_asian_);
			CP_XML_ATTR_OPT( L"style:font-pitch-complex",		style_font_pitch_complex_);

			CP_XML_ATTR_OPT( L"style:font-charset",				style_font_charset_);
			CP_XML_ATTR_OPT( L"style:font-charset-asian",		style_font_charset_asian_);
			CP_XML_ATTR_OPT( L"style:font-charset-complex",		style_font_charset_complex_);
		    
			CP_XML_ATTR_OPT( L"fo:font-size",					fo_font_size_);
			CP_XML_ATTR_OPT( L"style:font-size-asian",			style_font_size_asian_);
			CP_XML_ATTR_OPT( L"style:font-size-complex",		style_font_size_complex_);

			CP_XML_ATTR_OPT( L"style:font-size-rel",			style_font_size_rel_);
			CP_XML_ATTR_OPT( L"style:font-size-rel-asian",		style_font_size_rel_asian_);
			CP_XML_ATTR_OPT( L"style:font-size-rel-complex",	style_font_size_rel_complex_);

			CP_XML_ATTR_OPT( L"style:script-type",				style_script_type_);

			CP_XML_ATTR_OPT( L"fo:letter-spacing",				fo_letter_spacing_);

			CP_XML_ATTR_OPT( L"fo:language",					fo_language_);
			CP_XML_ATTR_OPT( L"style:language-asian",			style_language_asian_);
			CP_XML_ATTR_OPT( L"style:language-complex",			style_language_complex_);

			CP_XML_ATTR_OPT( L"fo:country",						fo_country_);
			CP_XML_ATTR_OPT( L"style:country-asian",			style_country_asian_);
			CP_XML_ATTR_OPT( L"style:country-complex",			style_country_complex_);
		           
			CP_XML_ATTR_OPT( L"fo:font-style",					fo_font_style_);
			CP_XML_ATTR_OPT( L"style:font-style-asian",			style_font_style_asian_);
			CP_XML_ATTR_OPT( L"style:font-style-complex",		style_font_style_complex_);

			CP_XML_ATTR_OPT( L"style:font-relief",				style_font_relief_);
		    
			CP_XML_ATTR_OPT( L"fo:text-shadow",					fo_text_shadow_);
			CP_XML_ATTR_OPT( L"style:text-underline-type",		style_text_underline_type_);		    
			CP_XML_ATTR_OPT( L"style:text-underline-style",		style_text_underline_style_);		    
			CP_XML_ATTR_OPT( L"style:text-underline-width",		style_text_underline_width_);
			CP_XML_ATTR_OPT( L"style:text-underline-color",		style_text_underline_color_);
		    
			CP_XML_ATTR_OPT( L"fo:font-weight",					fo_font_weight_);
			CP_XML_ATTR_OPT( L"style:font-weight-asian",		style_font_weight_asian_);
			CP_XML_ATTR_OPT( L"style:font-weight-complex",		style_font_weight_complex_);
		   
			CP_XML_ATTR_OPT( L"style:text-underline-mode",		style_text_underline_mode_);
		    
			CP_XML_ATTR_OPT( L"style:text-line-through-mode",	style_text_line_through_mode_);
			CP_XML_ATTR_OPT( L"style:letter-kerning",			style_letter_kerning_);
			CP_XML_ATTR_OPT( L"style:text-blinking",			style_text_blinking_);
			CP_XML_ATTR_OPT( L"fo:background-color",			fo_background_color_);
			CP_XML_ATTR_OPT( L"style:text-combine",				style_text_combine_);
			CP_XML_ATTR_OPT( L"style:text-combine-start-char",	style_text_combine_start_char_);
			CP_XML_ATTR_OPT( L"style:text-combine-end-char",	style_text_combine_end_char_);
			CP_XML_ATTR_OPT( L"style:text-emphasize",			style_text_emphasize_);
			CP_XML_ATTR_OPT( L"style:text-scale",				style_text_scale_);
			CP_XML_ATTR_OPT( L"style:text-rotation-angle",		style_text_rotation_angle_);
			CP_XML_ATTR_OPT( L"style:text-rotation-scale",		style_text_rotation_scale_);
			CP_XML_ATTR_OPT( L"fo:hyphenate",					fo_hyphenate_);
			CP_XML_ATTR_OPT( L"fo:hyphenation-remain-char-count", fo_hyphenation_remain_char_count_);

			CP_XML_ATTR_OPT( L"fo:hyphenation-push-char-count", fo_hyphenation_push_char_count_);

			CP_XML_ATTR_OPT( L"text:display",					text_display_);
			CP_XML_ATTR_OPT( L"text:condition",					text_condition_);

			CP_XML_ATTR_OPT( L"style:text-overline-color",		style_text_overline_color_);
			CP_XML_ATTR_OPT( L"style:text-overline-mode",		style_text_overline_mode_);
			CP_XML_ATTR_OPT( L"style:text-overline-style",		style_text_overline_style_);

			common_border_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_text_properties::ns = L"style";
const wchar_t * style_text_properties::name = L"text-properties";

void style_text_properties::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void style_text_properties::serialize(std::wostream & strm)
{
	content_.serialize(strm,ns,name);
}

void style_text_properties::apply_from(const style_text_properties * Other)
{
	if (Other == NULL) return;
	content_.apply_from(Other->content_);
}

}
}

