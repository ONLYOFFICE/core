
#include <iostream>
#include "style_paragraph_properties.h"

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>

#include <boost/foreach.hpp>


#include "borderstyle.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// style:tab-stop
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_tab_stop::ns = L"style";
const wchar_t * style_tab_stop::name = L"tab-stop";

void style_tab_stop::serialize(std::wostream & strm)
{
 	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"style:position",	style_position_);
			CP_XML_ATTR_OPT(L"style:type",		style_type_);
			CP_XML_ATTR_OPT(L"style:char",		style_char_);
		    
			CP_XML_ATTR_OPT(L"style:leader-type",	style_leader_style_);
			CP_XML_ATTR_OPT(L"style:leader-style",	style_leader_style_);
			CP_XML_ATTR_OPT(L"style:leader-width",	style_leader_width_);
			CP_XML_ATTR_OPT(L"style:leader-color",	style_leader_color_);
			CP_XML_ATTR_OPT(L"style:leader-text",	style_leader_text_);
			CP_XML_ATTR_OPT(L"style:leader-text-style", style_leader_text_style_);  
		}
	}
}
// style:tab-stop
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_tab_stops::ns = L"style";
const wchar_t * style_tab_stops::name = L"tab-stops";

void style_tab_stops::serialize(std::wostream & strm)
{
 	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(office_element_ptr & elm, style_tab_stops_)
			{
				elm->serialize(CP_XML_STREAM());
			}	
		}
	}
}
void style_tab_stops::add_child_element( office_element_ptr & child_element)
{
	style_tab_stops_.push_back(child_element);
}
// style:drop-cap
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_drop_cap::ns = L"style";
const wchar_t * style_drop_cap::name = L"drop-cap";

void style_drop_cap::serialize(std::wostream & strm)
{
  	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:length",	style_length_);
			CP_XML_ATTR_OPT(L"style:lines",		style_lines_);
			CP_XML_ATTR_OPT(L"style:distance",	style_distance_);
			CP_XML_ATTR_OPT(L"style:style-name",style_style_name_);
		}
	}

}
// style:background-image
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_background_image::ns = L"style";
const wchar_t * style_background_image::name = L"background-image";

void style_background_image::serialize(std::wostream & strm)
{
 	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:repeat", style_repeat_);
			CP_XML_ATTR_OPT(L"style:position", style_position_);
			CP_XML_ATTR_OPT(L"filter:name", filter_name_);
			CP_XML_ATTR_OPT(L"draw:opacity", draw_opacity_);

			if (common_xlink_attlist_)	common_xlink_attlist_->serialize(CP_GET_XML_NODE());
			if (office_binary_data_)	office_binary_data_->serialize(CP_XML_STREAM());
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void paragraph_format_properties::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name,odf_conversion_context * Context)
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
		//else
		//	CP_NOT_APPLICABLE_ELM();
    }
}
void paragraph_format_properties::add_child_element( office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeStyleTabStops)
		style_tab_stops_ = child_element;
	else if (type == typeStyleDropCap)
		style_drop_cap_ = child_element;
	else if (type == typeStyleBackgroundImage)
		style_background_image_ = child_element;
}

void paragraph_format_properties::serialize(std::wostream & _Wostream ,const wchar_t * ns, const wchar_t * name )
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"fo:line-height", fo_line_height_);
			CP_XML_ATTR_OPT(L"style:line-height-at-least", style_line_height_at_least_);
			CP_XML_ATTR_OPT(L"style:line-spacing", style_line_spacing_);
			CP_XML_ATTR_OPT(L"style:font-independent-line-spacing", style_font_independent_line_spacing_);
			CP_XML_ATTR_OPT(L"fo:text-align", fo_text_align_);
			CP_XML_ATTR_OPT(L"fo:text-align-last", fo_text_align_last_);
			CP_XML_ATTR_OPT(L"style:justify-single-word", style_justify_single_word_);
			CP_XML_ATTR_OPT(L"fo:keep-together", fo_keep_together_);
			CP_XML_ATTR_OPT(L"fo:widows", fo_widows_);
			CP_XML_ATTR_OPT(L"fo:orphans", fo_orphans_);
			CP_XML_ATTR_OPT(L"style:tab-stop-distance", style_tab_stop_distance_);
			CP_XML_ATTR_OPT(L"fo:hyphenation-keep", fo_hyphenation_keep_);
			CP_XML_ATTR_OPT(L"fo:hyphenation-ladder-count", fo_hyphenation_ladder_count_);
			CP_XML_ATTR_OPT(L"style:register-true", style_register_true_);
			CP_XML_ATTR_OPT(L"fo:margin-left", fo_margin_left_);
			CP_XML_ATTR_OPT(L"fo:margin-right", fo_margin_right_);
			CP_XML_ATTR_OPT(L"fo:text-indent", fo_text_indent_);
			CP_XML_ATTR_OPT(L"style:auto-text-indent", style_auto_text_indent_);
			CP_XML_ATTR_OPT(L"fo:margin-top", fo_margin_top_);
			CP_XML_ATTR_OPT(L"fo:margin-bottom", fo_margin_bottom_);
			CP_XML_ATTR_OPT(L"fo:margin", fo_margin_);
			CP_XML_ATTR_OPT(L"fo:break-before", fo_break_before_);
			CP_XML_ATTR_OPT(L"fo:break-after", fo_break_after_);
			CP_XML_ATTR_OPT(L"fo:background-color", fo_background_color_);
			CP_XML_ATTR_OPT(L"style:border-line-width", style_border_line_width_);
			CP_XML_ATTR_OPT(L"style:border-line-width-top", style_border_line_width_top_);
			CP_XML_ATTR_OPT(L"style:border-line-width-bottom", style_border_line_width_bottom_);
			CP_XML_ATTR_OPT(L"style:border-line-width-left", style_border_line_width_left_);
			CP_XML_ATTR_OPT(L"style:border-line-width-right", style_border_line_width_right_);
			CP_XML_ATTR_OPT(L"fo:padding", fo_padding_);
			CP_XML_ATTR_OPT(L"fo:padding-top", fo_padding_top_);
			CP_XML_ATTR_OPT(L"fo:padding-bottom", fo_padding_bottom_);
			CP_XML_ATTR_OPT(L"fo:padding-left", fo_padding_left_);
			CP_XML_ATTR_OPT(L"fo:padding-right", fo_padding_right_);
			CP_XML_ATTR_OPT(L"style:shadow", style_shadow_);
			CP_XML_ATTR_OPT(L"fo:keep-with-next", fo_keep_with_next_);
			CP_XML_ATTR_OPT(L"text:number-lines", text_number_lines_);
			CP_XML_ATTR_OPT(L"text:line-number", text_line_number_);
			CP_XML_ATTR_OPT(L"style:text-autospace", style_text_autospace_);
			CP_XML_ATTR_OPT(L"style:punctuation-wrap", style_punctuation_wrap_);
			CP_XML_ATTR_OPT(L"style:line-break", style_line_break_);
			CP_XML_ATTR_OPT(L"style:vertical-align", style_vertical_align_);
			CP_XML_ATTR_OPT(L"style:writing-mode", style_writing_mode_);
			CP_XML_ATTR_OPT(L"style:writing-mode-automatic", style_writing_mode_automatic_);
			CP_XML_ATTR_OPT(L"style:snap-to-layout-grid", style_snap_to_layout_grid_);
			CP_XML_ATTR_OPT(L"style:contextual-spacing", style_contextual_spacing_);
			CP_XML_ATTR_OPT(L"style:background-transparency", style_background_transparency_);
			CP_XML_ATTR_OPT(L"style:join_border", style_join_border_);
		
			common_border_attlist_.serialize(CP_GET_XML_NODE());
			//_CP_OPT(std::wstring) style_page_number_str_;
			//CP_XML_ATTR_OPT(L"style:page-number", style_page_number_str_);

			//if (style_page_number_str_)
			//{
			//	int res =0;
			//	if ((res=style_page_number_str_.get().find(L"auto"))<0)
			//		style_page_number_=  boost::lexical_cast<int>(style_page_number_str_.get());
			//}


			if (style_tab_stops_)		style_tab_stops_->serialize(CP_XML_STREAM());
			if (style_drop_cap_)		style_drop_cap_->serialize(CP_XML_STREAM());
			if (style_background_image_)style_background_image_->serialize(CP_XML_STREAM());
		}
	}
}

// style:paragraph-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_paragraph_properties::ns = L"style";
const wchar_t * style_paragraph_properties::name = L"paragraph-properties";

void style_paragraph_properties::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    style_paragraph_properties_content_.create_child_element(Ns, Name, getContext());    
}
void style_paragraph_properties::add_child_element( office_element_ptr & child_element)
{
	style_paragraph_properties_content_.add_child_element(child_element);
}
void style_paragraph_properties::serialize(std::wostream & strm)
{
	 style_paragraph_properties_content_.serialize(strm,ns,name);
}
void style_paragraph_properties::apply_from(style_paragraph_properties * Other)
{
	if (Other == NULL)return;

	style_paragraph_properties_content_.apply_from(Other->content());
}

void paragraph_format_properties::clear()
{
	fo_line_height_				= boost::none;
	style_line_height_at_least_	= boost::none;
	style_line_spacing_			= boost::none;
	style_font_independent_line_spacing_= boost::none;
	fo_text_align_				= boost::none;
	fo_text_align_last_			= boost::none;
	style_justify_single_word_	= boost::none;
	fo_keep_together_			= boost::none;
	fo_widows_					= boost::none;
	fo_orphans_					= boost::none;
	style_tab_stop_distance_	= boost::none;
	fo_hyphenation_keep_		= boost::none;
	fo_hyphenation_ladder_count_= boost::none;
	style_register_true_		= boost::none;
	fo_margin_left_			= boost::none;
	fo_margin_right_		= boost::none; 
	//fo_text_indent_			= boost::none;//заточено под буквицу
	style_auto_text_indent_	= boost::none;
	fo_margin_top_			= boost::none;
	fo_margin_bottom_		= boost::none;
	fo_margin_				= boost::none;
	fo_break_before_		= boost::none;
	fo_break_after_			= boost::none;
	fo_background_color_	= boost::none;
	fo_padding_				= boost::none;
	fo_padding_top_			= boost::none;
	fo_padding_bottom_		= boost::none;
	fo_padding_left_		= boost::none;
	fo_padding_right_		= boost::none;
	outline_level_			= boost::none;
	style_page_number_		= boost::none;
	style_snap_to_layout_grid_= boost::none;
	style_join_border_		= boost::none;
	style_text_autospace_	= boost::none;
	fo_keep_with_next_		= boost::none;
	fo_background_color_	= boost::none;
	style_vertical_align_	= boost::none;
	style_writing_mode_		= boost::none;
	style_writing_mode_automatic_= boost::none;
	style_line_break_		= boost::none;
	style_background_transparency_= boost::none;
	style_text_autospace_= boost::none;
	text_line_number_		= boost::none;
	style_line_break_		= boost::none;
	text_number_lines_		= boost::none; 
	style_shadow_			= boost::none;


		//todo borders !!!
}

void paragraph_format_properties::apply_from(paragraph_format_properties & Other)
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
    _CP_APPLY_PROP(style_tab_stop_distance_, Other.style_tab_stop_distance_);
    _CP_APPLY_PROP(fo_hyphenation_keep_, Other.fo_hyphenation_keep_);
    _CP_APPLY_PROP(fo_hyphenation_ladder_count_, Other.fo_hyphenation_ladder_count_);
    
    _CP_APPLY_PROP(style_drop_cap_, Other.style_drop_cap_);
    _CP_APPLY_PROP(style_tab_stops_, Other.style_tab_stops_);
    
    _CP_APPLY_PROP(style_register_true_, Other.style_register_true_);
    apply_length_or_percent(fo_margin_left_, Other.fo_margin_left_);
    apply_length_or_percent(fo_margin_right_, Other.fo_margin_right_);
    apply_length_or_percent(fo_text_indent_, Other.fo_text_indent_);
    _CP_APPLY_PROP(style_auto_text_indent_, Other.style_auto_text_indent_);
    apply_length_or_percent(fo_margin_top_, Other.fo_margin_top_);
    apply_length_or_percent(fo_margin_bottom_, Other.fo_margin_bottom_);
    apply_length_or_percent(fo_margin_, Other.fo_margin_);    
    _CP_APPLY_PROP(fo_break_before_, Other.fo_break_before_);
    _CP_APPLY_PROP(fo_break_after_, Other.fo_break_after_)
    _CP_APPLY_PROP(fo_background_color_, Other.fo_background_color_);

    _CP_APPLY_PROP(style_background_image_, Other.style_background_image_);

	common_border_attlist_.apply_from(Other.common_border_attlist_);

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
    _CP_APPLY_PROP(style_background_transparency_, Other.style_background_transparency_);
    _CP_APPLY_PROP(style_join_border_, Other.style_join_border_);
    
	_CP_APPLY_PROP(outline_level_, Other.outline_level_);
}


}
}
