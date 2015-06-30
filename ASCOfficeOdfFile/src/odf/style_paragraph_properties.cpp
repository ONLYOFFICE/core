
#include <iostream>
#include "style_paragraph_properties.h"

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>

#include <boost/foreach.hpp>

#include "office_elements_create.h"
#include "datatypes/borderstyle.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



// style:tab-stop
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_tab_stop::ns = L"style";
const wchar_t * style_tab_stop::name = L"tab-stop";

::std::wostream & style_tab_stop::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void style_tab_stop::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:position", style_position_, length());
    CP_APPLY_ATTR(L"style:type", style_type_);
    CP_APPLY_ATTR(L"style:char", style_char_);
    
    CP_APPLY_ATTR(L"style:leader-type", style_leader_style_);
    CP_APPLY_ATTR(L"style:leader-style", style_leader_style_);
    CP_APPLY_ATTR(L"style:leader-width", style_leader_width_);
    CP_APPLY_ATTR(L"style:leader-color", style_leader_color_);
    CP_APPLY_ATTR(L"style:leader-text", style_leader_text_);
    CP_APPLY_ATTR(L"style:leader-text-style", style_leader_text_style_);  

}

void style_tab_stop::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{}

void style_tab_stop::add_text(const std::wstring & Text)
{}

// style:tab-stop
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_tab_stops::ns = L"style";
const wchar_t * style_tab_stops::name = L"tab-stops";

::std::wostream & style_tab_stops::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void style_tab_stops::add_attributes( const xml::attributes_wc_ptr & Attributes )
{}

void style_tab_stops::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"tab-stop" == Name)
        CP_CREATE_ELEMENT(style_tab_stops_);
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

void style_tab_stops::add_text(const std::wstring & Text)
{}

// style:drop-cap
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_drop_cap::ns = L"style";
const wchar_t * style_drop_cap::name = L"drop-cap";

::std::wostream & style_drop_cap::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void style_drop_cap::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:length", style_length_, drop_cap_length());
    CP_APPLY_ATTR(L"style:lines", style_lines_, (unsigned int)1);
    CP_APPLY_ATTR(L"style:distance", style_distance_);
    CP_APPLY_ATTR(L"style:style-name", style_style_name_);
}

void style_drop_cap::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{}

void style_drop_cap::add_text(const std::wstring & Text)
{}

// style-background-image
// style:background-image
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_background_image::ns = L"style";
const wchar_t * style_background_image::name = L"background-image";

::std::wostream & style_background_image::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void style_background_image::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:repeat", style_repeat_);
    CP_APPLY_ATTR(L"style:position", style_position_);
    CP_APPLY_ATTR(L"filter:name", filter_name_);
    CP_APPLY_ATTR(L"draw:opacity", draw_opacity_);

    common_xlink_attlist tmp;
    if (tmp.add_attributes(Attributes))
    {
        common_xlink_attlist_ = _CP_OPT(common_xlink_attlist)(tmp);
    }
}

void style_background_image::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"office" == Ns && L"binary-data" == Name)
        CP_CREATE_ELEMENT(office_binary_data_);
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

void style_background_image::add_text(const std::wstring & Text)
{}

//////////////////////////////////////////////////////////////////////////////////////////////////

void paragraph_format_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    // 15.5.1 
    CP_APPLY_ATTR(L"fo:line-height", fo_line_height_);
    
    // 15.5.2 
    CP_APPLY_ATTR(L"style:line-height-at-least", style_line_height_at_least_);

    // 15.5.3 
    CP_APPLY_ATTR(L"style:line-spacing", style_line_spacing_);

    // 15.5.4 
    CP_APPLY_ATTR(L"style:font-independent-line-spacing", style_font_independent_line_spacing_);

    // 15.5.5 
    CP_APPLY_ATTR(L"fo:text-align", fo_text_align_);

    // 15.5.6 
    CP_APPLY_ATTR(L"fo:text-align-last", fo_text_align_last_);

    // 15.5.7 
    CP_APPLY_ATTR(L"style:justify-single-word", style_justify_single_word_);

    // 15.5.8 
    CP_APPLY_ATTR(L"fo:keep-together", fo_keep_together_);
    
    // 15.5.9 
    CP_APPLY_ATTR(L"fo:widows", fo_widows_);

    // 15.5.10 
    CP_APPLY_ATTR(L"fo:orphans", fo_orphans_);

    // 15.5.11 - Element
   
    // 15.5.12 
    CP_APPLY_ATTR(L"style:tab-stop-distance", style_tab_stop_distance_);

    // 15.5.13 
    CP_APPLY_ATTR(L"fo:hyphenation-keep", fo_hyphenation_keep_);

    // 15.5.14 
    CP_APPLY_ATTR(L"fo:hyphenation-ladder-count", fo_hyphenation_ladder_count_);
    
    // 15.5.15  - Element
      
    // 15.5.16 
    CP_APPLY_ATTR(L"style:register-true", style_register_true_);

    // 15.5.17 
    CP_APPLY_ATTR(L"fo:margin-left", fo_margin_left_);

    // 
    CP_APPLY_ATTR(L"fo:margin-right", fo_margin_right_);

    // 15.5.18 
    CP_APPLY_ATTR(L"fo:text-indent", fo_text_indent_);

    // 15.5.19 
    CP_APPLY_ATTR(L"style:auto-text-indent", style_auto_text_indent_);

    // 15.5.20 
    CP_APPLY_ATTR(L"fo:margin-top", fo_margin_top_);
    
    // 
    CP_APPLY_ATTR(L"fo:margin-bottom", fo_margin_bottom_);

    // 15.5.21 
    CP_APPLY_ATTR(L"fo:margin", fo_margin_);
    
    // 15.5.22 
    CP_APPLY_ATTR(L"fo:break-before", fo_break_before_);

    // 
    CP_APPLY_ATTR(L"fo:break-after", fo_break_after_);

    // 15.5.23 
    CP_APPLY_ATTR(L"fo:background-color", fo_background_color_);

    // 15.5.24 - Element

    // 15.5.25 
    CP_APPLY_ATTR(L"fo:border", fo_border_);

    // 
    CP_APPLY_ATTR(L"fo:border-top", fo_border_top_);

    // 
    CP_APPLY_ATTR(L"fo:border-bottom", fo_border_bottom_);

    // 
    CP_APPLY_ATTR(L"fo:border-left", fo_border_left_);

    // 
    CP_APPLY_ATTR(L"fo:border-right", fo_border_right_);

    // 15.5.26
    // 
    CP_APPLY_ATTR(L"style:border-line-width", style_border_line_width_);
    
    // 
    CP_APPLY_ATTR(L"style:border-line-width-top", style_border_line_width_top_);
    // 
    CP_APPLY_ATTR(L"style:border-line-width-bottom", style_border_line_width_bottom_);
    // 
    CP_APPLY_ATTR(L"style:border-line-width-left", style_border_line_width_left_);
    //
    CP_APPLY_ATTR(L"style:border-line-width-right", style_border_line_width_right_);

    // 15.5.27
    // 
    CP_APPLY_ATTR(L"fo:padding", fo_padding_);
    
    // 
    CP_APPLY_ATTR(L"fo:padding-top", fo_padding_top_);
    
    // 
    CP_APPLY_ATTR(L"fo:padding-bottom", fo_padding_bottom_);
    
    // 
    CP_APPLY_ATTR(L"fo:padding-left", fo_padding_left_);
    
    // 
    CP_APPLY_ATTR(L"fo:padding-right", fo_padding_right_);

    // 15.5.28 
    CP_APPLY_ATTR(L"style:shadow", style_shadow_);

    // 15.5.29 
    CP_APPLY_ATTR(L"fo:keep-with-next", fo_keep_with_next_);

    // 15.5.30 
    CP_APPLY_ATTR(L"text:number-lines", text_number_lines_);

    // 15.5.31 
    CP_APPLY_ATTR(L"text:line-number", text_line_number_);

    // 15.5.32 
    CP_APPLY_ATTR(L"style:text-autospace", style_text_autospace_);

    // 15.5.33 
    CP_APPLY_ATTR(L"style:punctuation-wrap", style_punctuation_wrap_);

    // 15.5.34 
    CP_APPLY_ATTR(L"style:line-break", style_line_break_);

    // 15.5.35 
    CP_APPLY_ATTR(L"style:vertical-align", style_vertical_align_);

    // 15.5.36 
    CP_APPLY_ATTR(L"style:writing-mode", style_writing_mode_);

    // 15.5.37 
    CP_APPLY_ATTR(L"style:writing-mode-automatic", style_writing_mode_automatic_);

    // 15.5.38 
    CP_APPLY_ATTR(L"style:snap-to-layout-grid", style_snap_to_layout_grid_);
    
    // 15.5.39 
	_CP_OPT(std::wstring) style_page_number_str_;
    CP_APPLY_ATTR(L"style:page-number", style_page_number_str_);

	if (style_page_number_str_)
	{
		int res =0;
		if ((res=style_page_number_str_.get().find(L"auto"))<0)
			style_page_number_=  boost::lexical_cast<int>(style_page_number_str_.get());
	}

    // 15.5.40 
    CP_APPLY_ATTR(L"style:background-transparency", style_background_transparency_);

    CP_APPLY_ATTR(L"style:join_border", style_join_border_);
}

bool paragraph_format_properties::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context)
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
        else
            return false;
        return true;
    }
    return false;
}


// style:paragraph-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_paragraph_properties::ns = L"style";
const wchar_t * style_paragraph_properties::name = L"paragraph-properties";

::std::wostream & style_paragraph_properties::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void style_paragraph_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
     style_paragraph_properties_content_.add_attributes(Attributes);
}

void style_paragraph_properties::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (!style_paragraph_properties_content_.add_child_element(Reader, Ns, Name, getContext()))    
         CP_NOT_APPLICABLE_ELM();
}

void style_paragraph_properties::add_text(const std::wstring & Text)
{}


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
