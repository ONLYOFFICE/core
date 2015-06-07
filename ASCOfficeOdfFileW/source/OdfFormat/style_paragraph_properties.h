#pragma once

#include <iosfwd>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>

#include "office_elements_create.h"

#include "linewidth.h"
#include "length.h"
#include "textalign.h"
#include "keeptogether.h"
#include "hyphenationkeep.h"
#include "backgroundcolor.h"
#include "shadowtype.h"
#include "percent.h"
#include "writingmode.h"
#include "verticalalign.h"
#include "linebreak.h"
#include "punctuationwrap.h"
#include "textautospace.h"
#include "hyphenationladdercount.h"
#include "lengthorpercent.h"
#include "styletype.h"
#include "linestyle.h"
#include "linetype.h"
#include "styleleadercolor.h"
#include "style_ref.h"
#include "dropcaplength.h"
#include "fobreak.h"
#include "styleposition.h"
#include "borderwidths.h"

//#include "common_draw_data.h"
//#include "office_binary_data.h"
#include "common_attlists.h"

namespace cpdoccore{

namespace oox{

	class docx_conversion_context;
	class pptx_conversion_context;
};

namespace odf {

/// \class  style_tab_stop
class style_tab_stop : public office_element_impl<style_tab_stop>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTabStop;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);

    _CP_OPT(length)				style_position_;
    _CP_OPT(style_type)			style_type_;
    _CP_OPT(wchar_t)			style_char_;

    _CP_OPT(line_type)			style_leader_type_;
    _CP_OPT(line_style)			style_leader_style_;
    _CP_OPT(line_width)			style_leader_width_;
    _CP_OPT(style_leader_color) style_leader_color_;

    _CP_OPT( std::wstring )	style_leader_text_;
    _CP_OPT( style_ref )		style_leader_text_style_;            

};

CP_REGISTER_OFFICE_ELEMENT2(style_tab_stop)

/// \class  style_tab_stops
class style_tab_stops : public office_element_impl<style_tab_stops>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTabStops;

    CPDOCCORE_DEFINE_VISITABLE()
    
    size_t size() const { return style_tab_stops_.size(); }

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    office_element_ptr_array style_tab_stops_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_tab_stops)

/// \class  style_drop_cap
class style_drop_cap : public office_element_impl<style_drop_cap>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDropCap;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
    
	_CP_OPT(drop_cap_length)	style_length_;
    _CP_OPT(unsigned int)		style_lines_;
    _CP_OPT(length)				style_distance_;
    _CP_OPT(style_ref)			style_style_name_;
    _CP_OPT(common_xlink_attlist) common_xlink_attlist_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_drop_cap)

/// \class  style_background_image
///         style:background-image
class style_background_image : public office_element_impl<style_background_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleBackgroundImage;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
    
    _CP_OPT(style_repeat) style_repeat_;
    _CP_OPT(style_position) style_position_;
    _CP_OPT(std::wstring) filter_name_;
    _CP_OPT(percent) draw_opacity_;

    _CP_OPT(common_xlink_attlist) common_xlink_attlist_;
    office_element_ptr office_binary_data_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_background_image)

/// \class  paragraph_format_properties
class paragraph_format_properties 
{
public:
	void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name,odf_conversion_context * Context);
	void add_child_element( const office_element_ptr & child);

	void serialize(std::wostream & strm, const wchar_t * ns, const wchar_t * name );

    void apply_from( paragraph_format_properties & Other);

	void clear();

public:
    // 15.5.1 fo:line-height
    _CP_OPT(line_width) fo_line_height_;                 // +
    
    // 15.5.2 style:line-height-at-least
    _CP_OPT(length) style_line_height_at_least_;         // +

    // 15.5.3 style:line-spacing
    _CP_OPT(length_or_percent) style_line_spacing_;                 // +

    // 15.5.4 style:font-independent-line-spacing
    _CP_OPT(Bool) style_font_independent_line_spacing_; // ???

    // 15.5.5 fo:text-align
    _CP_OPT(text_align) fo_text_align_;                  // +

    // 15.5.6 fo:text-align-last
    _CP_OPT(text_align) fo_text_align_last_;

    // 15.5.7 style:justify-single-word
    _CP_OPT(Bool) style_justify_single_word_;

    // 15.5.8 fo:keep-together
    _CP_OPT(keep_together) fo_keep_together_;            // +
    
    // 15.5.9 fo:widows
    _CP_OPT(unsigned int) fo_widows_;                    // +

    // 15.5.10 fo:orphans
    _CP_OPT(unsigned int) fo_orphans_;

    // 15.5.11 <style:tab-stops>
    office_element_ptr  style_tab_stops_;                       // +
    
    // 15.5.12 style:tab-stop-distance
    _CP_OPT(length) style_tab_stop_distance_;

    // 15.5.13 fo:hyphenation-keep
    _CP_OPT(hyphenation_keep) fo_hyphenation_keep_;

    // 15.5.14 fo:hyphenation-ladder-count
    _CP_OPT(integer_or_nolimit) fo_hyphenation_ladder_count_;
    
    // 15.5.15  <style:drop-cap>
    office_element_ptr style_drop_cap_;
    
    // 15.5.16 style:register-true
    _CP_OPT(Bool) style_register_true_;

    // 15.5.17 fo:margin-left
    _CP_OPT(length_or_percent) fo_margin_left_;      // +

    // fo:margin-right
    _CP_OPT(length_or_percent) fo_margin_right_;     // +

    // 15.5.18 fo:text-indent
    _CP_OPT(length_or_percent) fo_text_indent_;      // +

    // 15.5.19 style:auto-text-indent
    _CP_OPT(Bool) style_auto_text_indent_;           // ???

    // 15.5.20 fo:margin-top
    _CP_OPT(length_or_percent) fo_margin_top_;       // +
    
    // fo:margin-bottom
    _CP_OPT(length_or_percent) fo_margin_bottom_;    // +

    // 15.5.21 fo:margin
    _CP_OPT(length_or_percent) fo_margin_;
    
    // 15.5.22 fo:break-before
    _CP_OPT(fo_break) fo_break_before_;              // +

    // fo:break-after
    _CP_OPT(fo_break) fo_break_after_;

    // 15.5.23 fo:background-color
    _CP_OPT(background_color) fo_background_color_;

    // 15.5.24 <style:background-image>
    office_element_ptr style_background_image_;

	common_border_attlist common_border_attlist_;
    // 15.5.26
    // style:border-line-width
    _CP_OPT( border_widths ) style_border_line_width_;       // +
    
    // style:border-line-width-top
    _CP_OPT( border_widths ) style_border_line_width_top_;   // +
    // style:border-line-width-bottom
    _CP_OPT( border_widths ) style_border_line_width_bottom_;// +
    // style:border-line-width-left
    _CP_OPT( border_widths ) style_border_line_width_left_;  // +
    // style:border-line-width-right
    _CP_OPT( border_widths ) style_border_line_width_right_; // +

    // 15.5.27
    // fo:padding
    _CP_OPT(length) fo_padding_;                 // +
    
    // fo:padding-top
    _CP_OPT(length) fo_padding_top_;             // +
    
    // fo:padding-bottom
    _CP_OPT(length) fo_padding_bottom_;          // +
    
    // fo:padding-left
    _CP_OPT(length) fo_padding_left_;            // +
    
    // fo:padding-right
    _CP_OPT(length) fo_padding_right_;           // +

    // 15.5.28 style:shadow
	_CP_OPT(std::wstring) style_shadow_;

    // 15.5.29 fo:keep-with-next
    _CP_OPT(keep_together) fo_keep_with_next_;   // +

    // 15.5.30 text:number-lines
    _CP_OPT(Bool) text_number_lines_;

    // 15.5.31 text:line-number
    _CP_OPT(unsigned int) text_line_number_;

    // 15.5.32 style:text-autospace
    _CP_OPT(text_autospace) style_text_autospace_;

    // 15.5.33 style:punctuation-wrap
    _CP_OPT(punctuation_wrap) style_punctuation_wrap_;

    // 15.5.34 style:line-break
    _CP_OPT(line_break) style_line_break_;

    // 15.5.35 style:vertical-align
    _CP_OPT(vertical_align) style_vertical_align_;   // +

    // 15.5.36 style:writing-mode
    _CP_OPT(writing_mode) style_writing_mode_;

    // 15.5.37 style:writing-mode-automatic
    _CP_OPT(Bool) style_writing_mode_automatic_;

    // 15.5.38 style:snap-to-layout-grid
    _CP_OPT(Bool) style_snap_to_layout_grid_;
    
    // 15.5.39 style:page-number
    _CP_OPT(unsigned int) style_page_number_;

    // 15.5.40 style:background-transparency
    _CP_OPT(percent) style_background_transparency_;

    // 
    _CP_OPT(Bool) style_join_border_;

	_CP_OPT(Bool) style_contextual_spacing_;

//-----------------------------------------------------------------------------
//not writing elements - used in other place
//-----------------------------------------------------------------------------

	_CP_OPT(unsigned int) outline_level_;

};

/// \class  style_paragraph_properties
class style_paragraph_properties : public office_element_impl<style_paragraph_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleParagraphProperties;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	void apply_from(style_paragraph_properties * Other);

	virtual void serialize(std::wostream & strm);

    paragraph_format_properties & content(){ return style_paragraph_properties_content_; }

private:
    paragraph_format_properties style_paragraph_properties_content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_paragraph_properties)

} }
