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

#include "office_elements_create.h"

#include "../../DataTypes/linewidth.h"
#include "../../DataTypes/length.h"
#include "../../DataTypes/textalign.h"
#include "../../DataTypes/keeptogether.h"
#include "../../DataTypes/hyphenationkeep.h"
#include "../../DataTypes/backgroundcolor.h"
#include "../../DataTypes/shadowtype.h"
#include "../../DataTypes/percent.h"
#include "../../DataTypes/writingmode.h"
#include "../../DataTypes/verticalalign.h"
#include "../../DataTypes/linebreak.h"
#include "../../DataTypes/punctuationwrap.h"
#include "../../DataTypes/textautospace.h"
#include "../../DataTypes/hyphenationladdercount.h"
#include "../../DataTypes/lengthorpercent.h"
#include "../../DataTypes/styletype.h"
#include "../../DataTypes/linestyle.h"
#include "../../DataTypes/linetype.h"
#include "../../DataTypes/styleleadercolor.h"
#include "../../DataTypes/dropcaplength.h"
#include "../../DataTypes/fobreak.h"
#include "../../DataTypes/styleposition.h"
#include "../../DataTypes/borderwidths.h"

//#include "common_draw_data.h"
//#include "office_binary_data.h"
#include "../../DataTypes/common_attlists.h"

namespace cpdoccore{

namespace oox{

	class docx_conversion_context;
	class pptx_conversion_context;
};

namespace odf_writer {

class style_tab_stop : public office_element_impl<style_tab_stop>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleTabStop;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);

    _CP_OPT(odf_types::length)				style_position_;
    _CP_OPT(odf_types::style_type)			style_type_;
    _CP_OPT(wchar_t)						style_char_;

    _CP_OPT(odf_types::line_type)			style_leader_type_;
    _CP_OPT(odf_types::line_style)			style_leader_style_;
    _CP_OPT(odf_types::line_width)			style_leader_width_;
    _CP_OPT(odf_types::style_leader_color)	style_leader_color_;

    _CP_OPT( std::wstring )					style_leader_text_;
    _CP_OPT( std::wstring )			style_leader_text_style_;            

};

CP_REGISTER_OFFICE_ELEMENT2(style_tab_stop)

class style_tab_stops : public office_element_impl<style_tab_stops>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleTabStops;

    
    
    size_t size() const { return style_tab_stops_.size(); }

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    office_element_ptr_array style_tab_stops_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_tab_stops)


class style_drop_cap : public office_element_impl<style_drop_cap>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleDropCap;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
    
	_CP_OPT(odf_types::drop_cap_length)		style_length_;
    _CP_OPT(unsigned int)					style_lines_;
    _CP_OPT(odf_types::length)				style_distance_;
    _CP_OPT(std::wstring)					style_style_name_;
    _CP_OPT(odf_types::common_xlink_attlist) common_xlink_attlist_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_drop_cap)

class style_background_image : public office_element_impl<style_background_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleBackgroundImage;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
    
    _CP_OPT(odf_types::style_repeat)		style_repeat_;
    _CP_OPT(odf_types::style_position)		style_position_;
    _CP_OPT(std::wstring)					filter_name_;
    _CP_OPT(odf_types::percent)				draw_opacity_;

    _CP_OPT(odf_types::common_xlink_attlist) xlink_attlist_;
    
	office_element_ptr office_binary_data_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_background_image)

class paragraph_format_properties 
{
public:
	void create_child_element(const std::wstring & Ns, const std::wstring & Name,odf_conversion_context * Context);
	void add_child_element( const office_element_ptr & child);

	void serialize(std::wostream & strm, const wchar_t * ns, const wchar_t * name );

    void apply_from( paragraph_format_properties & Other);

	void clear(bool bEraseDropCap = true);

    _CP_OPT(odf_types::length)				style_line_height_at_least_;         
    _CP_OPT(odf_types::length_or_percent)	style_line_spacing_;                 
    _CP_OPT(odf_types::Bool)				style_font_independent_line_spacing_; 
    _CP_OPT(odf_types::Bool)				style_justify_single_word_;
    office_element_ptr						style_tab_stops_;                       // +
    _CP_OPT(odf_types::length)				style_tab_stop_distance_;
    office_element_ptr						style_drop_cap_;
     _CP_OPT(odf_types::Bool)				style_register_true_;
    _CP_OPT(odf_types::Bool)				style_auto_text_indent_;   
    office_element_ptr						style_background_image_;
    _CP_OPT( odf_types::border_widths )		style_border_line_width_;      
    _CP_OPT( odf_types::border_widths )		style_border_line_width_top_;   
    _CP_OPT( odf_types::border_widths )		style_border_line_width_bottom_;
    _CP_OPT( odf_types::border_widths )		style_border_line_width_left_;  
    _CP_OPT( odf_types::border_widths )		style_border_line_width_right_; 
	_CP_OPT(std::wstring)					style_shadow_;
    _CP_OPT(odf_types::text_autospace)		style_text_autospace_;
    _CP_OPT(odf_types::punctuation_wrap)	style_punctuation_wrap_;
    _CP_OPT(odf_types::line_break)			style_line_break_;
    _CP_OPT(odf_types::vertical_align)		style_vertical_align_; 
    _CP_OPT(odf_types::writing_mode)		style_writing_mode_;
    _CP_OPT(odf_types::Bool)				style_writing_mode_automatic_;
    _CP_OPT(odf_types::Bool)				style_snap_to_layout_grid_;
    _CP_OPT(unsigned int)					style_page_number_;
    _CP_OPT(odf_types::percent)				style_background_transparency_;
    _CP_OPT(odf_types::Bool)				style_join_border_;
	_CP_OPT(odf_types::Bool)				style_contextual_spacing_;
   
	_CP_OPT(odf_types::text_align)			fo_text_align_;    
    _CP_OPT(odf_types::line_width)			fo_line_height_; 
    _CP_OPT(odf_types::text_align)			fo_text_align_last_;
    _CP_OPT(odf_types::keep_together)		fo_keep_together_; 
    _CP_OPT(unsigned int)					fo_widows_; 
    _CP_OPT(unsigned int)					fo_orphans_;
    _CP_OPT(odf_types::hyphenation_keep)	fo_hyphenation_keep_;
    _CP_OPT(odf_types::integer_or_nolimit)	fo_hyphenation_ladder_count_;
    _CP_OPT(odf_types::length_or_percent)	fo_margin_left_;
    _CP_OPT(odf_types::length_or_percent)	fo_margin_right_;    
    _CP_OPT(odf_types::length_or_percent)	fo_margin_top_;    
    _CP_OPT(odf_types::length_or_percent)	fo_margin_bottom_; 
    _CP_OPT(odf_types::length_or_percent)	fo_text_indent_;   
    _CP_OPT(odf_types::length_or_percent)	fo_margin_    ;
    _CP_OPT(odf_types::fo_break)			fo_break_before_;  
	_CP_OPT(odf_types::fo_break)			fo_break_after_;
    _CP_OPT(odf_types::length)				fo_padding_;          
    _CP_OPT(odf_types::length)				fo_padding_top_;    
    _CP_OPT(odf_types::length)				fo_padding_bottom_;
    _CP_OPT(odf_types::length)				fo_padding_left_; 
    _CP_OPT(odf_types::length)				fo_padding_right_; 
    _CP_OPT(odf_types::background_color)	fo_background_color_;
    _CP_OPT(odf_types::keep_together)		fo_keep_with_next_;


	odf_types::common_border_attlist		common_border_attlist_;
    _CP_OPT(odf_types::Bool)				text_number_lines_;
    _CP_OPT(unsigned int)					text_line_number_;

//-----------------------------------------------------------------------------
//not writing elements - used in other place
//-----------------------------------------------------------------------------

	_CP_OPT(unsigned int)				outline_level_;

};

class style_paragraph_properties : public office_element_impl<style_paragraph_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleParagraphProperties;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	void apply_from(style_paragraph_properties * Other);

	virtual void serialize(std::wostream & strm);

    paragraph_format_properties content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_paragraph_properties)

} }
