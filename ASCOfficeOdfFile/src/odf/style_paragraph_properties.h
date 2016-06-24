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

#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/linewidth.h"
#include "datatypes/length.h"
#include "datatypes/textalign.h"
#include "datatypes/keeptogether.h"
#include "datatypes/hyphenationkeep.h"
#include "datatypes/backgroundcolor.h"
#include "datatypes/shadowtype.h"
#include "datatypes/percent.h"
#include "datatypes/writingmode.h"
#include "datatypes/verticalalign.h"
#include "datatypes/linebreak.h"
#include "datatypes/punctuationwrap.h"
#include "datatypes/textautospace.h"
#include "datatypes/hyphenationladdercount.h"
#include "datatypes/lengthorpercent.h"
#include "datatypes/styletype.h"
#include "datatypes/linestyle.h"
#include "datatypes/linetype.h"
#include "datatypes/styleleadercolor.h"
#include "datatypes/style_ref.h"
#include "datatypes/dropcaplength.h"
#include "datatypes/fobreak.h"
#include "datatypes/styleposition.h"
#include "datatypes/borderwidths.h"

#include "office_binary_data.h"
#include "datatypes/common_attlists.h"

namespace cpdoccore{

namespace oox{

	class docx_conversion_context;
	class pptx_conversion_context;
};

namespace odf_reader {

//  style_tab_stop
class style_tab_stop : public office_element_impl<style_tab_stop>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTabStop;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    odf_types::length style_position_;
    _CP_OPT(odf_types::style_type) style_type_;
    _CP_OPT(wchar_t) style_char_;

	_CP_OPT(odf_types::line_type) style_leader_type_;
    _CP_OPT(odf_types::line_style) style_leader_style_;
    _CP_OPT(odf_types::line_width) style_leader_width_;
    _CP_OPT(odf_types::style_leader_color) style_leader_color_;

    _CP_OPT( std::wstring ) style_leader_text_;
    _CP_OPT( odf_types::style_ref ) style_leader_text_style_;            

};

CP_REGISTER_OFFICE_ELEMENT2(style_tab_stop);

//  style_tab_stops
class style_tab_stops : public office_element_impl<style_tab_stops>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTabStops;

    CPDOCCORE_DEFINE_VISITABLE();
    
    size_t size() const { return style_tab_stops_.size(); }

    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array style_tab_stops_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_tab_stops);

//  style_drop_cap
class style_drop_cap : public office_element_impl<style_drop_cap>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDropCap;

    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
    
	odf_types::drop_cap_length style_length_;
    unsigned int style_lines_;
    _CP_OPT(odf_types::length) style_distance_;
    _CP_OPT(odf_types::style_ref) style_style_name_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

    _CP_OPT(odf_types::common_xlink_attlist) common_xlink_attlist_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_drop_cap);

///         style:background-image
class style_background_image : public office_element_impl<style_background_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleBackgroundImage;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
    
    _CP_OPT(odf_types::style_repeat)			style_repeat_;
    _CP_OPT(odf_types::style_position)			style_position_;
    _CP_OPT(std::wstring)						filter_name_;
    _CP_OPT(odf_types::percent)					draw_opacity_;

    _CP_OPT(odf_types::common_xlink_attlist)	common_xlink_attlist_;
    office_element_ptr							office_binary_data_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_background_image);

//  paragraph_format_properties
class paragraph_format_properties : public oox::conversion_element
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    bool add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);

    void apply_from(const paragraph_format_properties & Other);

    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);
    
	void xlsx_convert(std::wostream & strm, bool in_draw);

    // 15.5.1 fo:line-height
    _CP_OPT(odf_types::line_width) fo_line_height_;                 // +
    
    // 15.5.2 style:line-height-at-least
    _CP_OPT(odf_types::length) style_line_height_at_least_;         // +

    // 15.5.3 style:line-spacing
    _CP_OPT(odf_types::length) style_line_spacing_;                 // +

    // 15.5.4 style:font-independent-line-spacing
    _CP_OPT(bool) style_font_independent_line_spacing_; // ???

    // 15.5.5 fo:text-align
    _CP_OPT(odf_types::text_align) fo_text_align_;                  // +

    // 15.5.6 fo:text-align-last
    _CP_OPT(odf_types::text_align) fo_text_align_last_;

    // 15.5.7 style:justify-single-word
    _CP_OPT(bool) style_justify_single_word_;

    // 15.5.8 fo:keep-together
    _CP_OPT(odf_types::keep_together) fo_keep_together_;            // +
    
    // 15.5.9 fo:widows
    _CP_OPT(unsigned int) fo_widows_;                    // +

    // 15.5.10 fo:orphans
    _CP_OPT(unsigned int) fo_orphans_;

    // 15.5.11 <style:tab-stops>
    office_element_ptr  style_tab_stops_;                       // +
    
    // 15.5.12 style:tab-stop-distance
    _CP_OPT(odf_types::length) style_tab_stop_distance_;

    // 15.5.13 fo:hyphenation-keep
    _CP_OPT(odf_types::hyphenation_keep) fo_hyphenation_keep_;

    // 15.5.14 fo:hyphenation-ladder-count
    _CP_OPT(odf_types::integer_or_nolimit) fo_hyphenation_ladder_count_;
    
    // 15.5.15  <style:drop-cap>
    office_element_ptr style_drop_cap_;
    
    // 15.5.16 style:register-true
    _CP_OPT(bool) style_register_true_;

    // 15.5.17 fo:margin-left
    _CP_OPT(odf_types::length_or_percent) fo_margin_left_;      // +

    // fo:margin-right
    _CP_OPT(odf_types::length_or_percent) fo_margin_right_;     // +

    // 15.5.18 fo:text-indent
    _CP_OPT(odf_types::length_or_percent) fo_text_indent_;      // +

    // 15.5.19 style:auto-text-indent
    _CP_OPT(bool) style_auto_text_indent_;           // ???

    // 15.5.20 fo:margin-top
    _CP_OPT(odf_types::length_or_percent) fo_margin_top_;       // +
    
    // fo:margin-bottom
    _CP_OPT(odf_types::length_or_percent) fo_margin_bottom_;    // +

    // 15.5.21 fo:margin
    _CP_OPT(odf_types::length_or_percent) fo_margin_;
    
    // 15.5.22 fo:break-before
    _CP_OPT(odf_types::fo_break) fo_break_before_;              // +

    // fo:break-after
    _CP_OPT(odf_types::fo_break) fo_break_after_;

    // 15.5.23 fo:background-color
    _CP_OPT(odf_types::background_color) fo_background_color_;

    // 15.5.24 <style:background-image>
    office_element_ptr style_background_image_;

	_CP_OPT(odf_types::border_style) fo_border_;                // +
	_CP_OPT(odf_types::border_style) fo_border_top_;            // +
    _CP_OPT(odf_types::border_style) fo_border_bottom_;         // +
    _CP_OPT(odf_types::border_style) fo_border_left_;           // +
    _CP_OPT(odf_types::border_style) fo_border_right_;          // +

    // 15.5.26
    _CP_OPT( odf_types::border_widths ) style_border_line_width_;       // +
    
    _CP_OPT( odf_types::border_widths ) style_border_line_width_top_;   // +
    _CP_OPT( odf_types::border_widths ) style_border_line_width_bottom_;// +
    _CP_OPT( odf_types::border_widths ) style_border_line_width_left_;  // +
    _CP_OPT( odf_types::border_widths ) style_border_line_width_right_; // +

    _CP_OPT(odf_types::length)			fo_padding_;                 // +
    _CP_OPT(odf_types::length)			fo_padding_top_;             // +
    _CP_OPT(odf_types::length)			fo_padding_bottom_;          // +
    _CP_OPT(odf_types::length)			fo_padding_left_;            // +
    _CP_OPT(odf_types::length)			fo_padding_right_;           // +
    _CP_OPT(odf_types::shadow_type)		style_shadow_;
    _CP_OPT(odf_types::keep_together)	fo_keep_with_next_;  
    _CP_OPT(bool)						text_number_lines_;
    _CP_OPT(unsigned int)				text_line_number_;
    _CP_OPT(odf_types::text_autospace)	style_text_autospace_;
    _CP_OPT(odf_types::punctuation_wrap) style_punctuation_wrap_;
    _CP_OPT(odf_types::line_break)		style_line_break_;
    _CP_OPT(odf_types::vertical_align)	style_vertical_align_; 
    _CP_OPT(odf_types::writing_mode)	style_writing_mode_;
    _CP_OPT(bool)						style_writing_mode_automatic_;
    _CP_OPT(bool)						style_snap_to_layout_grid_;
    _CP_OPT(unsigned int)				style_page_number_;

    // 15.5.40 style:background-transparency
    _CP_OPT(odf_types::percent) style_background_transparency_;

    // 
    _CP_OPT(bool) style_join_border_;
};

//  style_paragraph_properties
class style_paragraph_properties : public office_element_impl<style_paragraph_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleParagraphProperties;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    const paragraph_format_properties & content() const { return style_paragraph_properties_content_; }

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);   


private:
    paragraph_format_properties style_paragraph_properties_content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_paragraph_properties);

} }
