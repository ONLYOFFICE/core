﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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


#include "styles.h"
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include <cpdoccore/odf/odf_document.h>

#include <iostream>

#include "office_elements.h"
#include "office_elements_create.h"

#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_table_properties.h"
#include "style_graphic_properties.h"
#include "style_chart_properties.h"
#include "style_presentation.h"

#include "serialize_elements.h"
#include "odfcontext.h"
#include "draw_common.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

	namespace {

std::wstring process_border(const border_style	& borderStyle,
			const	_CP_OPT(border_widths)		& borderLineWidths,
					_CP_OPT(length)				& borderPadding, 
					_CP_OPT(length_or_percent)	& margin, 
			const	std::wstring				& Shadow = L"")
{
    std::wstring w_sz;
    std::wstring w_color;
    std::wstring w_val = L"single";
    std::wstring w_space;

    if (!borderStyle.initialized() || borderStyle.is_none())
    {
        w_val = L"none";
    }
    else if (borderStyle.initialized())
    {
        double width = borderStyle.get_length().get_value_unit(length::pt);
	
		if (borderLineWidths)
		{
			width = borderLineWidths->get_len1().get_value_unit(length::pt);
		}

        int szInt = (int)(0.5 + 8.0 * width); //eighths of a point (ST_EighthPointMeasure)
        if (szInt <= 0)      szInt = 1;

		w_sz = std::to_wstring( szInt );
		w_color = borderStyle.get_color().get_hex_value() ;

        if (borderPadding)
            w_space = std::to_wstring((int)(borderPadding->get_value_unit(length::pt) + 0.5) );

        switch(borderStyle.get_style())
        {
        case border_style::none:        w_val = L"none";    break;
        case border_style::solid:
        case border_style::single:
                                        w_val = L"single";  break;
        case border_style::double_:     w_val = L"double";  break;
        case border_style::dotted:      w_val = L"dotted";  break;
        case border_style::dashed:      w_val = L"dashed";  break;
        case border_style::groove:      w_val = L"thinThickMediumGap";  break;
        case border_style::ridge:       w_val = L"thickThinMediumGap";  break;
        case border_style::inset:       w_val = L"inset";  break;
        case border_style::outset:      w_val = L"outset";  break;
        case border_style::hidden:      w_val = L"nil";     break;
        }
    }
    std::wstring res;
    if (!w_val.empty())
        res += L" w:val=\"" + w_val + L"\" ";
    if (!w_sz.empty())
        res += L"w:sz=\"" + w_sz + L"\" ";
    if (!w_color.empty())
        res += L"w:color=\"" + w_color + L"\" ";
    if (!w_space.empty())
        res += L"w:space=\"" + w_space + L"\" ";
    if (!Shadow.empty())
        res += L"w:shadow=\"" + Shadow + L"\" ";

    return res;
}

std::wstring process_margin(const _CP_OPT(length_or_percent) & margin, double Mul)
{
    if (margin)
    {
        if (margin->get_type() == length_or_percent::Length)
        {
            int val = (int)(0.5 + Mul * margin->get_length().get_value_unit(length::pt));
            return std::to_wstring( val );
        }
        else
        {
            _CP_LOG << L"[docx_convert] convert margin warning: invalid type (percent)\n";
        }       
    }
    return L"";
}

}


style_text_properties * style_content::get_style_text_properties() const
{
    return dynamic_cast<style_text_properties *>(style_text_properties_.get());    
}

style_paragraph_properties * style_content::get_style_paragraph_properties() const
{
    return dynamic_cast<style_paragraph_properties *>(style_paragraph_properties_.get());    
}

graphic_format_properties * style_content::get_graphic_properties() const
{
    style_graphic_properties *style_ =  dynamic_cast<style_graphic_properties *>(style_graphic_properties_.get());    
    loext_graphic_properties *loext_ =  dynamic_cast<loext_graphic_properties *>(style_graphic_properties_.get());   

	if (style_) return &style_->content_;
	if (loext_) return &loext_->content_;

	return NULL;
}
style_table_properties * style_content::get_style_table_properties() const
{
    return dynamic_cast<style_table_properties *>(style_table_properties_.get());    
}

style_section_properties * style_content::get_style_section_properties() const
{
    return dynamic_cast<style_section_properties *>(style_section_properties_.get());
}

style_table_cell_properties * style_content::get_style_table_cell_properties() const
{
    return dynamic_cast<style_table_cell_properties *>(style_table_cell_properties_.get());
}

style_table_row_properties * style_content::get_style_table_row_properties() const
{
    return dynamic_cast<style_table_row_properties *>(style_table_row_properties_.get());
}

style_table_column_properties * style_content::get_style_table_column_properties() const
{
    return dynamic_cast<style_table_column_properties *>(style_table_column_properties_.get());
}

style_chart_properties * style_content::get_style_chart_properties() const
{
    return dynamic_cast<style_chart_properties *>(style_chart_properties_.get());    
}
style_drawing_page_properties * style_content::get_style_drawing_page_properties() const
{
    return dynamic_cast<style_drawing_page_properties *>(style_drawing_page_properties_.get());
}

void style_content::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    if (style_table_column_properties_)
        style_table_column_properties_->xlsx_convert(Context);

	if (style_table_cell_properties_)
		style_table_cell_properties_->xlsx_convert(Context);

    if (style_chart_properties_)
        style_chart_properties_->xlsx_convert(Context);

}

void style_content::docx_convert(oox::docx_conversion_context & Context, bool in_styles) 
{
    Context.start_process_style_content();

    if (style_text_properties_)
        style_text_properties_->docx_convert(Context);

    if (style_paragraph_properties_)
        style_paragraph_properties_->docx_convert(Context);

    if (style_table_properties_)
        style_table_properties_->docx_convert(Context);

    if (style_table_column_properties_)
        style_table_column_properties_->docx_convert(Context);

    if (style_table_row_properties_)
        style_table_row_properties_->docx_convert(Context);

    if (style_table_cell_properties_)
        style_table_cell_properties_->docx_convert(Context);        

    Context.end_process_style_content(in_styles);
}

void style_content::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{   
    if CP_CHECK_NAME(L"style", L"text-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_text_properties_);    
    }
    else if CP_CHECK_NAME(L"style", L"paragraph-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_paragraph_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"table-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_table_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"table-column-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_table_column_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"table-cell-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_table_cell_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"table-row-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_table_row_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"section-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_section_properties_);
    } 
    else if (CP_CHECK_NAME(L"style", L"graphic-properties") || CP_CHECK_NAME(L"loext", L"graphic-properties"))
    {
        CP_CREATE_ELEMENT_SIMPLE(style_graphic_properties_);    
    }
    else if CP_CHECK_NAME(L"style", L"chart-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_chart_properties_);    
    }
	else if CP_CHECK_NAME(L"style", L"drawing-page-properties")
	{
		CP_CREATE_ELEMENT_SIMPLE(style_drawing_page_properties_);
	}
    else
    {
        not_applicable_element(L"style-content", Reader, Ns, Name);
    }
}

// style:default-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * default_style::ns = L"style";
const wchar_t * default_style::name = L"default-style";

std::wostream & default_style::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void default_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:family", style_family_, style_family());
}

void default_style::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    style_content_.add_child_element(Reader, Ns, Name, getContext());
}

void default_style::add_text(const std::wstring & Text)
{    
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_fill_image::ns = L"draw";
const wchar_t * draw_fill_image::name = L"fill-image";

void draw_fill_image::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:name",	draw_name_);
	xlink_attlist_.add_attributes(Attributes);
}

void draw_fill_image::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_gradient::ns = L"draw";
const wchar_t * draw_gradient::name = L"gradient";

void draw_gradient::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:name"				, draw_name_);
	CP_APPLY_ATTR(L"draw:display-name"		, draw_display_name_);

	CP_APPLY_ATTR(L"draw:start-color"		, draw_start_color_);
	CP_APPLY_ATTR(L"draw:end-color"			, draw_end_color_);

	CP_APPLY_ATTR(L"draw:end-intensity"		, draw_end_intensity_); 
	CP_APPLY_ATTR(L"draw:start-intensity"	, draw_start_intensity_);

	CP_APPLY_ATTR(L"draw:cy"				, draw_cy_);//%
	CP_APPLY_ATTR(L"draw:cx"				, draw_cx_);
	
	CP_APPLY_ATTR(L"draw:border"			, draw_border_);
	CP_APPLY_ATTR(L"draw:angle"				, draw_angle_);
	CP_APPLY_ATTR(L"draw:style"				, draw_style_);//"square" 

}

void draw_gradient::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_hatch::ns = L"draw";
const wchar_t * draw_hatch::name = L"hatch";

void draw_hatch::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:name",			draw_name_);
	CP_APPLY_ATTR(L"draw:display-name",	draw_display_name_);
	CP_APPLY_ATTR(L"draw:style",		draw_style_);
	CP_APPLY_ATTR(L"draw:color",		draw_color_);
	CP_APPLY_ATTR(L"draw:distance",		draw_distance_);
	CP_APPLY_ATTR(L"draw:rotation",		draw_rotation_);
}

void draw_hatch::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_opacity::ns = L"draw";
const wchar_t * draw_opacity::name = L"opacity";

void draw_opacity::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:name",	draw_name_);
	CP_APPLY_ATTR(L"draw:display-name",	draw_display_name_);

	CP_APPLY_ATTR(L"draw:start",	draw_start_);
	CP_APPLY_ATTR(L"draw:end",		draw_end_);

	
	CP_APPLY_ATTR(L"draw:border",	draw_border_);
	CP_APPLY_ATTR(L"draw:angle",	draw_angle_);
	CP_APPLY_ATTR(L"draw:style",	draw_style_); 

}

void draw_opacity::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
// style:style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style::ns = L"style";
const wchar_t * style::name = L"style";

std::wostream & style::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:name"					, style_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"style:display-name"			, style_display_name_);
    CP_APPLY_ATTR(L"style:family"				, style_family_, style_family());
    CP_APPLY_ATTR(L"style:parent-style-name"	, style_parent_style_name_);
    CP_APPLY_ATTR(L"style:next-style-name"		, style_next_style_name_);
    CP_APPLY_ATTR(L"style:list-style-name"		, style_list_style_name_);
    CP_APPLY_ATTR(L"style:master-page-name"		, style_master_page_name_);
    CP_APPLY_ATTR(L"style:auto-update"			, style_auto_update_, false);
    CP_APPLY_ATTR(L"style:data-style-name"		, style_data_style_name_);
    CP_APPLY_ATTR(L"style:class"				, style_class_);
    CP_APPLY_ATTR(L"style:default-outline-level", style_default_outline_level_);//было int .. error

	if (style_master_page_name_)
	{
		if (style_master_page_name_->empty())
		{
			style_master_page_name_.reset();
		}
	}
}

void style::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"style", L"map")
    {
        CP_CREATE_ELEMENT(style_map_);
    }
    else
        style_content_.add_child_element(Reader, Ns, Name, getContext());
}

void style::add_text(const std::wstring & Text)
{
}

// styles & draw_styles
//////////////////////////////////////////////////////////////////////////////////////////////////

void styles::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if CP_CHECK_NAME(L"style", L"style")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_style_);
    } 
    else if CP_CHECK_NAME(L"text", L"list-style")
    {
        CP_CREATE_ELEMENT_SIMPLE(text_list_style_);
    }
    else if (
        CP_CHECK_NAME(L"number", L"number-style")	||
        CP_CHECK_NAME(L"number", L"currency-style") ||
        CP_CHECK_NAME(L"number", L"percentage-style") ||
        CP_CHECK_NAME(L"number", L"date-style")		||
        CP_CHECK_NAME(L"number", L"time-style")		||
        CP_CHECK_NAME(L"number", L"text-style")		||
        CP_CHECK_NAME(L"number", L"number-boolean-style")
        )
    {
        CP_CREATE_ELEMENT_SIMPLE(number_styles_);
    }   
    else
    {
        CP_NOT_APPLICABLE_ELM_SIMPLE(L"styles");
    }
}

void templates::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-template")
    {
        CP_CREATE_ELEMENT_SIMPLE(table_templates_);
    } 
}

void draw_styles::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if CP_CHECK_NAME(L"draw", L"gradient")
    {
        CP_CREATE_ELEMENT_SIMPLE(draw_gradient_);
    } 
    else if CP_CHECK_NAME(L"draw", L"hatch")
    {
        CP_CREATE_ELEMENT_SIMPLE(draw_hatch_);
    }
    else if CP_CHECK_NAME(L"draw", L"fill-image")
    {
        CP_CREATE_ELEMENT_SIMPLE(draw_fill_image_);
    }
    else if CP_CHECK_NAME(L"draw", L"opacity")
	{
         CP_CREATE_ELEMENT_SIMPLE(draw_opacity_);  
	}
	else if CP_CHECK_NAME(L"draw", L"marker")
	{
         CP_CREATE_ELEMENT_SIMPLE(draw_marker_);
	}
    else if CP_CHECK_NAME(L"draw", L"stroke-dash")
	{
         CP_CREATE_ELEMENT_SIMPLE(draw_stroke_dash_);
	}
    else
    {
        CP_NOT_APPLICABLE_ELM_SIMPLE(L"draw_styles");
    }
}


// office:automatic-styles
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_automatic_styles::ns = L"office";
const wchar_t * office_automatic_styles::name = L"automatic-styles";

std::wostream & office_automatic_styles::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void office_automatic_styles::add_attributes( const xml::attributes_wc_ptr & Attributes )
{}

void office_automatic_styles::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"page-layout" == Name)
    {
        CP_CREATE_ELEMENT(style_page_layout_);
    }
    else
    {
        styles_.add_child_element(Reader, Ns, Name, getContext());            
    }
}

void office_automatic_styles::add_text(const std::wstring & Text)
{}

// office:master-styles
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_master_styles::ns = L"office";
const wchar_t * office_master_styles::name = L"master-styles";

std::wostream & office_master_styles::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void office_master_styles::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_master_styles::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"master-page" == Name)
        CP_CREATE_ELEMENT(style_master_page_);
    else if (L"style" == Ns && L"handout-master" == Name)
        CP_CREATE_ELEMENT(style_handout_master_);
    else if (L"style" == Ns && L"draw-layer-set" == Name)
        CP_CREATE_ELEMENT(draw_layer_set_);
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

void office_master_styles::add_text(const std::wstring & Text)
{
}


// office:styles
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_styles::ns = L"office";
const wchar_t * office_styles::name = L"styles";

void office_styles::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_styles::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (CP_CHECK_NAME(L"style",		L"style") ||
        CP_CHECK_NAME(L"text",		L"list-style") ||
        
		CP_CHECK_NAME(L"number",	L"number-style") ||
        CP_CHECK_NAME(L"number",	L"currency-style") || 
        CP_CHECK_NAME(L"number",	L"text-style") ||
        CP_CHECK_NAME(L"number",	L"percentage-style") ||
        CP_CHECK_NAME(L"number",	L"date-style") ||
		CP_CHECK_NAME(L"number",	L"time-style") ||
		CP_CHECK_NAME(L"number",	L"boolean-style")
        )
    {
        styles_.add_child_element(Reader, Ns, Name, getContext());
    }
    else if (L"style" == Ns && L"default-style" == Name)
        CP_CREATE_ELEMENT(style_default_style_);

    else if (L"style" == Ns && L"presentation-page-layout" == Name)
        CP_CREATE_ELEMENT(style_presentation_page_layout_);

    else if(CP_CHECK_NAME(L"draw",		L"gradient") ||
			CP_CHECK_NAME(L"draw",		L"fill-image") ||
			CP_CHECK_NAME(L"draw",		L"hatch") ||
			CP_CHECK_NAME(L"draw",		L"marker") ||
			CP_CHECK_NAME(L"draw",		L"opacity") ||
			CP_CHECK_NAME(L"draw",		L"stroke-dash") ||
			CP_CHECK_NAME(L"svg",		L"linearGradient") ||
			CP_CHECK_NAME(L"svg",		L"radialGradient") 
			)
	{
        draw_styles_.add_child_element(Reader, Ns, Name, getContext());
	}
     else if(CP_CHECK_NAME(L"table",	L"table-template"))
	{
        templates_.add_child_element(Reader, Ns, Name, getContext());
	}
	 else if (L"text" == Ns && L"outline-style" == Name)
        CP_CREATE_ELEMENT(text_outline_style_);
    else if (L"text" == Ns && L"notes-configuration" == Name)
        CP_CREATE_ELEMENT(text_notes_configuration_);
    else if (L"text" == Ns && L"bibliography-configuration" == Name)
        CP_CREATE_ELEMENT(text_bibliography_configuration_);
    else if (L"text" == Ns && L"linenumbering-configuration" == Name)
        CP_CREATE_ELEMENT(text_linenumbering_configuration_);
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}


// style:header
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_header::ns = L"style";
const wchar_t * style_header::name = L"header";

void style_header::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    content().common_style_header_footer_attlist_.add_attributes(Attributes);
}

void style_header::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content().header_footer_content_.add_child_element(Reader, Ns, Name, getContext());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footer::ns = L"style";
const wchar_t * style_footer::name = L"footer";

void style_footer::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    content().common_style_header_footer_attlist_.add_attributes(Attributes);
}

void style_footer::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content().header_footer_content_.add_child_element(Reader, Ns, Name, getContext());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_header_first::ns = L"style";
const wchar_t * style_header_first::name = L"header-first";

void style_header_first::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    content().common_style_header_footer_attlist_.add_attributes(Attributes);
}

void style_header_first::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content().header_footer_content_.add_child_element(Reader, Ns, Name, getContext());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footer_first::ns = L"style";
const wchar_t * style_footer_first::name = L"footer-first";

void style_footer_first::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    content().common_style_header_footer_attlist_.add_attributes(Attributes);
}

void style_footer_first::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content().header_footer_content_.add_child_element(Reader, Ns, Name, getContext());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_header_left::ns = L"style";
const wchar_t * style_header_left::name = L"header-left";

void style_header_left::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    content().common_style_header_footer_attlist_.add_attributes(Attributes);
}

void style_header_left::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content().header_footer_content_.add_child_element(Reader, Ns, Name, getContext());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footer_left::ns = L"style";
const wchar_t * style_footer_left::name = L"footer-left";

void style_footer_left::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    content().common_style_header_footer_attlist_.add_attributes(Attributes);
}

void style_footer_left::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content().header_footer_content_.add_child_element(Reader, Ns, Name, getContext());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_columns::ns = L"style";
const wchar_t * style_columns::name = L"columns";

void style_columns::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:column-count", fo_column_count_);
    CP_APPLY_ATTR(L"fo:column-gap", fo_column_gap_);
}

void style_columns::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"column" == Name)
        CP_CREATE_ELEMENT(style_columns_);
    else if (L"style" == Ns && L"column-sep" == Name)
        CP_CREATE_ELEMENT(style_column_sep_);
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_column::ns = L"style";
const wchar_t * style_column::name = L"column";

void style_column::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:rel-width",	style_rel_width_);
    CP_APPLY_ATTR(L"fo:start-indent",	fo_start_indent_);
    CP_APPLY_ATTR(L"fo:end-indent",		fo_end_indent_);
    CP_APPLY_ATTR(L"fo:space-before",	fo_space_before_);
    CP_APPLY_ATTR(L"fo:space-after",	fo_space_after_);    
}

void style_column::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_column_sep::ns = L"style";
const wchar_t * style_column_sep::name = L"column-sep";

void style_column_sep::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:style", style_style_, std::wstring(L"solid")); // default solid
    CP_APPLY_ATTR(L"style:width", style_width_);
    CP_APPLY_ATTR(L"style:height", style_height_, percent(100.0)); // default 100
    CP_APPLY_ATTR(L"style:vertical-align", style_vertical_align_, vertical_align(vertical_align::Top)); 
    CP_APPLY_ATTR(L"style:color", style_color_, color(L"#000000"));  
}

void style_column_sep::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_section_properties::ns = L"style";
const wchar_t * style_section_properties::name = L"section-properties";

void style_section_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_background_color_attlist_.add_attributes(Attributes);
    common_horizontal_margin_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"style:protect", style_protect_, false); // default false
    CP_APPLY_ATTR(L"text:dont-balance-text-columns", text_dont_balance_text_columns_);
    common_writing_mode_attlist_.add_attributes(Attributes);
}

void style_section_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
        CP_CREATE_ELEMENT(style_background_image_);    
    else if (L"style" == Ns && L"columns" == Name)
        CP_CREATE_ELEMENT(style_columns_);        
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_header_style::ns = L"style";
const wchar_t * style_header_style::name = L"header-style";

std::wostream & style_header_style::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void style_header_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void style_header_style::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"header-footer-properties" == Name)
    {
        CP_CREATE_ELEMENT(style_header_footer_properties_);        
    } 
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footer_style::ns = L"style";
const wchar_t * style_footer_style::name = L"footer-style";

std::wostream & style_footer_style::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void style_footer_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void style_footer_style::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"header-footer-properties" == Name)
    {
        CP_CREATE_ELEMENT(style_header_footer_properties_);        
    } 
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

void style_page_layout_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:name", style_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"style:page-usage", style_page_usage_, page_usage(page_usage::All));    
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_page_layout::ns = L"style";
const wchar_t * style_page_layout::name = L"page-layout";

std::wostream & style_page_layout::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;    
}

void style_page_layout::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    style_page_layout_attlist_.add_attributes(Attributes);
}

void style_page_layout::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"header-style" == Name)
    {
        CP_CREATE_ELEMENT(style_header_style_);        
    } 
    else if (L"style" == Ns && L"footer-style" == Name)
    {
        CP_CREATE_ELEMENT(style_footer_style_);
    }
    else if (L"style" == Ns && L"page-layout-properties" == Name)
    {
        CP_CREATE_ELEMENT(style_page_layout_properties_);
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

void style_page_layout_properties_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:page-width",		fo_page_width_);
    CP_APPLY_ATTR(L"fo:page-height",	fo_page_height_);
    CP_APPLY_ATTR(L"style:paper-tray-name",		style_paper_tray_name_);
    CP_APPLY_ATTR(L"style:print-orientation",	style_print_orientation_);
   
    common_num_format_attlist_.add_attributes(Attributes);
    common_num_format_prefix_suffix_attlist_.add_attributes(Attributes);
	common_horizontal_margin_attlist_.add_attributes(Attributes);
    common_vertical_margin_attlist_.add_attributes(Attributes);
    common_margin_attlist_.add_attributes(Attributes);
    common_border_attlist_.add_attributes(Attributes);
    common_border_line_width_attlist_.add_attributes(Attributes);
    common_padding_attlist_.add_attributes(Attributes);
    common_shadow_attlist_.add_attributes(Attributes);
    common_background_color_attlist_.add_attributes(Attributes);
	common_draw_fill_attlist_.add_attributes(Attributes);

	CP_APPLY_ATTR(L"style:register-truth-ref-style-name", style_register_truth_ref_style_name_);
    CP_APPLY_ATTR(L"style:print",					style_print_);
    CP_APPLY_ATTR(L"style:print-page-order",		style_print_page_order_);
    CP_APPLY_ATTR(L"style:first-page-number",		style_first_page_number_);
    CP_APPLY_ATTR(L"style:scale-to",				style_scale_to_);
    CP_APPLY_ATTR(L"style:scale-to_pages",			style_scale_to_pages_);
    CP_APPLY_ATTR(L"style:table-centering",			style_table_centering_);
    CP_APPLY_ATTR(L"style:footnote-max-height",		style_footnote_max_height_);
    
	common_writing_mode_attlist_.add_attributes(Attributes);
    
	CP_APPLY_ATTR(L"style:layout-grid-mode",		style_layout_grid_mode_);
    CP_APPLY_ATTR(L"style:layout-grid-base-height", style_layout_grid_base_height_);
    CP_APPLY_ATTR(L"style:layout-grid-ruby-height", style_layout_grid_ruby_height_);
    CP_APPLY_ATTR(L"style:layout-grid-lines",		style_layout_grid_lines_);
    CP_APPLY_ATTR(L"style:layout-grid-color",		style_layout_grid_color_);
    CP_APPLY_ATTR(L"style:layout-grid-ruby-below",	style_layout_grid_ruby_below_);  
    CP_APPLY_ATTR(L"style:layout-grid-print",		style_layout_grid_print_);
    CP_APPLY_ATTR(L"style:layout-grid-display",		style_layout_grid_display_);
}

namespace {

std::wstring process_page_margin(const _CP_OPT(length_or_percent)	& Val, 
								 const _CP_OPT(length)				& AddVal = _CP_OPT(length)())
{
    //if ((!Val) || (Val->get_type() == length_or_percent::Percent))
    //    return L"0";

    double v = (!Val || Val->get_type() == length_or_percent::Percent) ? 0 :
						(20.0 * Val->get_length().get_value_unit(length::pt));

    double dAddVal = 20.0 * AddVal.get_value_or(length(0, length::pt)).get_value_unit(length::pt) + 0.5;

	if (dAddVal < 0 ) dAddVal = 0;
    return std::to_wstring( (int)( v + dAddVal));
}

}

void style_page_layout_properties_attlist::docx_convert_serialize(std::wostream & strm, oox::docx_conversion_context & Context, _CP_OPT(length_or_percent) margin_left, 
																								_CP_OPT(length_or_percent) margin_right) 
{
	double height_page = 0;
	
	if (fo_page_width_ || fo_page_height_ || style_print_orientation_)
	{
		std::wstring w_w = L"";
		std::wstring w_h = L"";
		
		if (fo_page_width_)
		{
			int val =  0.5 + 20.0 * fo_page_width_->get_value_unit(length::pt);
			if (val > 31680)
			{
				//Context.set_settings_property(odf_reader::_property(L"UnormalWidthPage",val));
				val =31680;//22"
			}
			w_w = std::to_wstring(val);
		}
		if (fo_page_height_)
		{
			int val = 0.5 + 20.0 * fo_page_height_->get_value_unit(length::pt);
			if (val > 31680) val =31680;//22"
			w_h = std::to_wstring(val);

			height_page = val;
		}
                
		std::wstring w_orient;
		if (style_print_orientation_)
		{
			if (L"landscape" == *style_print_orientation_)
				w_orient = L"landscape";
			else if (L"portrait" == *style_print_orientation_)
				w_orient = L"portrait";
		}
        
		strm << L"<w:pgSz ";
		if (!w_h.empty())
			strm << L"w:h=\"" << w_h << L"\" ";

		if (!w_w.empty())
			strm << L"w:w=\"" << w_w << L"\" ";

		if (!w_orient.empty())
			strm << L"w:orient=\"" << w_orient << L"\" ";

		strm << L"/>";
	}
	if (!common_padding_attlist_.fo_padding_top_)
		common_padding_attlist_.fo_padding_top_		= common_padding_attlist_.fo_padding_;
	if (!common_padding_attlist_.fo_padding_bottom_)
		common_padding_attlist_.fo_padding_bottom_	= common_padding_attlist_.fo_padding_;
	if (!common_padding_attlist_.fo_padding_left_)
		common_padding_attlist_.fo_padding_left_	= common_padding_attlist_.fo_padding_;
	if (!common_padding_attlist_.fo_padding_right_)
		common_padding_attlist_.fo_padding_right_	= common_padding_attlist_.fo_padding_;

	if (!common_horizontal_margin_attlist_.fo_margin_left_)
		common_horizontal_margin_attlist_.fo_margin_left_	= common_margin_attlist_.fo_margin_;
	if (!common_horizontal_margin_attlist_.fo_margin_right_)
		common_horizontal_margin_attlist_.fo_margin_right_	= common_margin_attlist_.fo_margin_;
	if (!common_vertical_margin_attlist_.fo_margin_top_)
		common_vertical_margin_attlist_.fo_margin_top_		= common_margin_attlist_.fo_margin_;
	if (!common_vertical_margin_attlist_.fo_margin_bottom_)
		common_vertical_margin_attlist_.fo_margin_bottom_	= common_margin_attlist_.fo_margin_;

//hange offset border from text to from page
	if (common_padding_attlist_.fo_padding_left_ && common_horizontal_margin_attlist_.fo_margin_left_)
	{
	}
	if (common_padding_attlist_.fo_padding_right_ && common_horizontal_margin_attlist_.fo_margin_right_)
	{
	}	
	if (common_padding_attlist_.fo_padding_top_ && common_vertical_margin_attlist_.fo_margin_top_)
	{
	}
	if (common_padding_attlist_.fo_padding_bottom_ && common_vertical_margin_attlist_.fo_margin_bottom_)
	{
	}
			
	if (common_border_attlist_.fo_border_ || common_border_attlist_.fo_border_top_ || common_border_attlist_.fo_border_bottom_ ||
			common_border_attlist_.fo_border_left_ || common_border_attlist_.fo_border_right_ )
	{
		std::wstring w_top, w_left, w_right, w_bottom, w_shadow;
	
		if (common_shadow_attlist_.style_shadow_)
			w_shadow = common_shadow_attlist_.style_shadow_->get_type() != shadow_type::None ? L"1" : L"0";

		if (common_border_attlist_.fo_border_ || common_border_attlist_.fo_border_top_)
		{
			w_top = process_border (	common_border_attlist_.fo_border_top_ ? *common_border_attlist_.fo_border_top_ : *common_border_attlist_.fo_border_ 
										, common_border_line_width_attlist_.style_border_line_width_top_
										, common_padding_attlist_.fo_padding_top_
										, common_vertical_margin_attlist_.fo_margin_top_
										, w_shadow);
		}
		if (common_border_attlist_.fo_border_ || common_border_attlist_.fo_border_bottom_)
		{
			w_bottom = process_border (	common_border_attlist_.fo_border_bottom_ ? *common_border_attlist_.fo_border_bottom_ : *common_border_attlist_.fo_border_
										, common_border_line_width_attlist_.style_border_line_width_bottom_ 
										, common_padding_attlist_.fo_padding_bottom_
										, common_vertical_margin_attlist_.fo_margin_bottom_
										, w_shadow);
		}
		if (common_border_attlist_.fo_border_ || common_border_attlist_.fo_border_left_)
		{
			w_left = process_border (	common_border_attlist_.fo_border_left_ ? *common_border_attlist_.fo_border_left_ : *common_border_attlist_.fo_border_ 
										, common_border_line_width_attlist_.style_border_line_width_left_
										, common_padding_attlist_.fo_padding_left_
										, common_horizontal_margin_attlist_.fo_margin_left_
										, w_shadow);
		}
		if (common_border_attlist_.fo_border_ || common_border_attlist_.fo_border_right_)
		{
			w_right = process_border (	common_border_attlist_.fo_border_right_ ? *common_border_attlist_.fo_border_right_ : *common_border_attlist_.fo_border_ 
										, common_border_line_width_attlist_.style_border_line_width_right_
										, common_padding_attlist_.fo_padding_right_
										, common_horizontal_margin_attlist_.fo_margin_right_
										, w_shadow);
		}
		 strm << L"<w:pgBorders w:offsetFrom=\"text\">";
			if (!w_top.empty())			strm << L"<w:top "		<< w_top	<< L" />";
			if (!w_left.empty())		strm << L"<w:left "		<< w_left	<< L" />";
			if (!w_right.empty())		strm << L"<w:right "	<< w_right	<< L" />";
			if (!w_bottom.empty())		strm << L"<w:bottom "	<< w_bottom << L" />";
		 strm << L"</w:pgBorders>";
	}
   
	CP_XML_WRITER(strm)
	{
		if (common_horizontal_margin_attlist_.fo_margin_left_	||
			common_horizontal_margin_attlist_.fo_margin_right_	||
			common_vertical_margin_attlist_.fo_margin_top_		||
			common_vertical_margin_attlist_.fo_margin_bottom_	||
			margin_right || margin_left
			)
		{
			_CP_OPT(odf_types::length)  margin_left_length, margin_right_length;

			if (margin_left)	margin_left_length	= margin_left->get_length();
			if (margin_right)	margin_right_length = margin_right->get_length();

			CP_XML_NODE(L"w:pgMar")
			{
				CP_XML_ATTR(L"w:left"	, process_page_margin(common_horizontal_margin_attlist_.fo_margin_left_	, margin_left_length) );
				CP_XML_ATTR(L"w:right"	, process_page_margin(common_horizontal_margin_attlist_.fo_margin_right_, margin_right_length) );
				CP_XML_ATTR(L"w:top"	, process_page_margin(common_vertical_margin_attlist_.fo_margin_top_	, Context.get_header_footer_context().header()) );
				CP_XML_ATTR(L"w:bottom"	, process_page_margin(common_vertical_margin_attlist_.fo_margin_bottom_	, Context.get_header_footer_context().footer()) );
				
				CP_XML_ATTR(L"w:header"	, process_page_margin(common_vertical_margin_attlist_.fo_margin_top_	) );
				CP_XML_ATTR(L"w:footer"	, process_page_margin(common_vertical_margin_attlist_.fo_margin_bottom_ ) );
				CP_XML_ATTR(L"w:gutter"	, 0 );
			}
		}

		if ((style_layout_grid_mode_) && (style_layout_grid_mode_->get_type() != odf_types::layout_grid_mode::None))
		{
			CP_XML_NODE(L"w:docGrid")
			{
				if (style_layout_grid_mode_->get_type() == odf_types::layout_grid_mode::Line)
					CP_XML_ATTR(L"w:type", L"lines");
				if (style_layout_grid_mode_->get_type() == odf_types::layout_grid_mode::Both)	
					CP_XML_ATTR(L"w:type", L"linesAndChars");

				int val_step = 0;
				if (style_layout_grid_base_height_ && style_layout_grid_ruby_height_)
				{
					val_step = 0.5	+ 20. * style_layout_grid_ruby_height_->get_value_unit(odf_types::length::pt) 
									+ 20. * style_layout_grid_base_height_->get_value_unit(odf_types::length::pt);
				}
				else  if (style_layout_grid_lines_ && height_page > 0)
				{
					val_step = height_page / (*style_layout_grid_lines_);
				}
				
				if (val_step > 0)CP_XML_ATTR(L"w:linePitch", val_step);
			}			
		}
	}
}

void style_page_layout_properties_attlist::pptx_convert(oox::pptx_conversion_context & Context) 
{
    std::wostream & strm = Context.current_presentation().slidesProperties();

    if (fo_page_width_ || fo_page_height_ || style_print_orientation_)
    {
        std::wstring w_w, w_h;

		_INT64 h = 0, w = 0;
		
		if (fo_page_width_)
		{
			w =  fo_page_width_->get_value_unit(length::emu);
			if (w < 914400) w = 914400;

			w_w = std::to_wstring(w);
		}
        if (fo_page_height_)
		{
			h = fo_page_height_->get_value_unit(length::emu);
			if (h < 914400) h = 914400;

			w_h = std::to_wstring(h);
		}
                
        std::wstring w_orient = L"custom";

		//if (w && h)
		//{
		//	double ratio = (double)w/(double)h;
		//	if (abs(ratio - 16./9.)<0.01)	w_orient = L"screen16x9";
		//	if (abs(ratio - 4./3.)<0.01)	w_orient = L"screen4x3";
		//}
        
        strm << L"<p:sldSz ";
        if (!w_h.empty())
            strm << L"cy=\"" << w_h << L"\" ";

        if (!w_w.empty())
            strm << L"cx=\"" << w_w << L"\" ";
	
		strm << L"type=\"" << w_orient << L"\" ";

        strm << L"/>";
    }

}

// style-footnote-sep-attlist
void style_footnote_sep_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:width",					style_width_);
    CP_APPLY_ATTR(L"style:rel-width",				style_rel_width_);
    CP_APPLY_ATTR(L"style:color",					style_color_);
    CP_APPLY_ATTR(L"style:line-style",				style_line_style_);
    CP_APPLY_ATTR(L"style:type",					style_adjustment_, style_type(style_type::Left)); // default Left
    CP_APPLY_ATTR(L"style:distance-before-sep",		style_distance_before_sep_);
    CP_APPLY_ATTR(L"style:distance-after-sep",		style_distance_after_sep_);
}

/// style:footnote-sep
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footnote_sep::ns = L"style";
const wchar_t * style_footnote_sep::name = L"footnote-sep";

void style_footnote_sep::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    style_footnote_sep_attlist_.add_attributes( Attributes );
}

void style_footnote_sep::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

/// style:page-layout-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_page_layout_properties::ns = L"style";
const wchar_t * style_page_layout_properties::name = L"page-layout-properties";

void style_page_layout_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void style_page_layout_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    elements_.add_child_element(Reader, Ns, Name, getContext());
}

bool style_page_layout_properties::docx_background_serialize(std::wostream & strm, oox::docx_conversion_context & Context, oox::_oox_fill & fill, int id)
{
	if (attlist_.common_background_color_attlist_.fo_background_color_ && 
		attlist_.common_background_color_attlist_.fo_background_color_->get_type() == background_color::Transparent)
		return true; //??
    //прозрачный фон
	
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"w:background")
		{
			std::wstring color = L"ffffff";
			
			if (attlist_.common_background_color_attlist_.fo_background_color_)
				color = attlist_.common_background_color_attlist_.fo_background_color_->get_color().get_hex_value();
			
			CP_XML_ATTR(L"w:color", color);

			oox::vml_serialize_background(CP_XML_STREAM(), fill, color, id);
		}
	}
	return true;
}

void style_page_layout_properties::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}
void style_page_layout_properties::xlsx_serialize(std::wostream & strm, oox::xlsx_conversion_context & Context)
{
	CP_XML_WRITER(strm)
	{
		odf_types::common_horizontal_margin_attlist		horizontal_margins	= attlist_.common_horizontal_margin_attlist_;
		odf_types::common_vertical_margin_attlist		vertical_margins	= attlist_.common_vertical_margin_attlist_;
		
		if (horizontal_margins.fo_margin_left_	|| horizontal_margins.fo_margin_right_	||
			vertical_margins.fo_margin_top_		|| vertical_margins.fo_margin_bottom_ )
		{
			//_CP_OPT(odf_types::length)  margin_top, margin_bottom;

			//margin_top	= Context.get_header_footer_context().header();
			//margin_bottom = Context.get_header_footer_context().footer();

			CP_XML_NODE(L"pageMargins")
			{
				if (horizontal_margins.fo_margin_left_ && horizontal_margins.fo_margin_left_->get_type() == odf_types::length_or_percent::Length)
					CP_XML_ATTR(L"left"		, horizontal_margins.fo_margin_left_->get_length().get_value_unit(odf_types::length::inch));
				else CP_XML_ATTR(L"left", 0);
				
				if (horizontal_margins.fo_margin_right_ && horizontal_margins.fo_margin_right_->get_type() == odf_types::length_or_percent::Length)
					CP_XML_ATTR(L"right"	, horizontal_margins.fo_margin_right_->get_length().get_value_unit(odf_types::length::inch));
				else CP_XML_ATTR(L"right", 0);
				
				if (vertical_margins.fo_margin_top_ && vertical_margins.fo_margin_top_->get_type() == odf_types::length_or_percent::Length)
					CP_XML_ATTR(L"top"		, vertical_margins.fo_margin_top_->get_length().get_value_unit(odf_types::length::inch));
				else CP_XML_ATTR(L"top", 1.025);

				if (vertical_margins.fo_margin_bottom_ && vertical_margins.fo_margin_bottom_->get_type() == odf_types::length_or_percent::Length)
					CP_XML_ATTR(L"bottom"	, vertical_margins.fo_margin_bottom_->get_length().get_value_unit(odf_types::length::inch));
				else CP_XML_ATTR(L"bottom", 1.025);
				
				if (vertical_margins.fo_margin_top_)
					CP_XML_ATTR(L"header"	, vertical_margins.fo_margin_top_->get_length().get_value_unit(odf_types::length::inch));
				else CP_XML_ATTR(L"header", 0.7875);

				if (vertical_margins.fo_margin_bottom_)
					CP_XML_ATTR(L"footer"	, vertical_margins.fo_margin_bottom_->get_length().get_value_unit(odf_types::length::inch));
				else CP_XML_ATTR(L"footer", 0.7875);
			}
		}
		if (attlist_.fo_page_width_ || attlist_.fo_page_height_ || attlist_.style_print_orientation_)
		{
			CP_XML_NODE(L"pageSetup")
			{
				double h = 0, w = 0;
				if (attlist_.fo_page_width_)
				{
					w =  attlist_.fo_page_width_->get_value_unit(length::mm);
					CP_XML_ATTR(L"paperWidth", (int)w);
				}
				if (attlist_.fo_page_height_)
				{
					h = attlist_.fo_page_height_->get_value_unit(length::mm);
					CP_XML_ATTR(L"paperHeight", (int)h);
				}
				CP_XML_ATTR(L"paperUnits", L"mm");
				if (attlist_.style_print_orientation_)
				{
					CP_XML_ATTR(L"orientation", *attlist_.style_print_orientation_);
				}
			}
		}
	}
	if (elements_.style_background_image_)
	{
		oox::_oox_fill fill;
			
		Compute_GraphicFill(attlist_.common_draw_fill_attlist_, elements_.style_background_image_, Context.root()->odf_context().drawStyles(), fill);
		if (fill.bitmap)
		{
			if ( fill.bitmap->rId.empty())
			{
				std::wstring href	= fill.bitmap->xlink_href_;
				fill.bitmap->rId	= Context.get_mediaitems().add_or_find(href, oox::typeImage, fill.bitmap->isInternal, href);

				Context.get_drawing_context().get_drawings()->add(fill.bitmap->isInternal, fill.bitmap->rId, href, oox::typeImage, true);
			}

			Context.get_table_context().state()->set_background(fill.bitmap->rId);
		}		
	}
}

void style_page_layout_properties::docx_serialize(std::wostream & strm, oox::docx_conversion_context & Context)
{
	style_columns * columns = dynamic_cast<style_columns *>( elements_.style_columns_.get());

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"w:sectPr")
		{
			Context.process_section( CP_XML_STREAM(), columns);
			
			bool change_page_layout = Context.is_next_dump_page_properties();
			
			CP_XML_NODE(L"w:type")
			{				
				if (change_page_layout)
				{
					CP_XML_ATTR(L"w:val", L"nextPage");
				}
				else			
					CP_XML_ATTR(L"w:val", L"continuous");
			}			

			std::wstring masterPageName = Context.get_master_page_name();
			bool res = Context.get_headers_footers().write_sectPr(masterPageName, change_page_layout, strm);
			
			if (res == false)
			{
				// default???
				masterPageName = L"Standard";
				const std::wstring masterPageNameLayout = Context.root()->odf_context().pageLayoutContainer().page_layout_name_by_style(masterPageName);
				
				Context.remove_page_properties();
				Context.add_page_properties(masterPageNameLayout);
				
				bool res = Context.get_headers_footers().write_sectPr(masterPageName, change_page_layout, strm);
			}
	
			oox::section_context::_section & section = Context.get_section_context().get();

			attlist_.docx_convert_serialize(strm, Context, section.margin_left_, section.margin_right_);
			//todooo при появлении еще накладок - переписать !!
		}
	}
}
void style_page_layout_properties::pptx_convert(oox::pptx_conversion_context & Context)
{
    attlist_.pptx_convert(Context);        
}

void style_page_layout_properties::pptx_serialize(std::wostream & strm, oox::pptx_conversion_context & Context)
{
    if (attlist_.fo_page_width_ || attlist_.fo_page_height_ || attlist_.style_print_orientation_)
    {
        std::wstring w_w, w_h;

		_INT64 h = 0, w = 0;
		
		if (attlist_.fo_page_width_)
		{
			w =  attlist_.fo_page_width_->get_value_unit(length::emu);
			if (w < 914400) w = 914400;

			w_w = std::to_wstring(w);
		}
        if (attlist_.fo_page_height_)
		{
			h = attlist_.fo_page_height_->get_value_unit(length::emu);
			if (h < 914400) h = 914400;

			w_h = std::to_wstring(h);
		}
                
        std::wstring w_orient = L"custom";

		//if (w && h)
		//{
		//	double ratio = (double)w/(double)h;
		//	if (abs(ratio - 16./9.)<0.01)	w_orient = L"screen16x9";
		//	if (abs(ratio - 4./3.)<0.01)	w_orient = L"screen4x3";
		//}
        
        strm << L"<p:sldSz ";
        if (!w_h.empty())
            strm << L"cy=\"" << w_h << L"\" ";

        if (!w_w.empty())
            strm << L"cx=\"" << w_w << L"\" ";
	
		strm << L"type=\"" << w_orient << L"\" ";

        strm << L"/>";
    }
}

// style-page-layout-properties-elements
//////////////////////////////////////////////////////////////////////////////////////////////////

void style_page_layout_properties_elements::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context )
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_background_image_);            
    }
    else if (L"style" == Ns && L"columns" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_columns_);            
    }
    else if (L"style" == Ns && L"footnote-sep" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_footnote_sep_);            
    }
    else
    {
        not_applicable_element(L"style-page-layout-properties-elements", Reader, Ns, Name);
    }
}

// style-master-page-attlist
////////////////////////////////////////////////////////////////////////////////////////////////// 
void style_master_page_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:name",			style_name_);
    CP_APPLY_ATTR(L"style:display-name",	style_display_name_);
    CP_APPLY_ATTR(L"style:page-layout-name",style_page_layout_name_);
    CP_APPLY_ATTR(L"draw:style-name",		draw_style_name_);
    CP_APPLY_ATTR(L"style:next-style-name", style_next_style_name_);
}

/// style:master-page
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_master_page::ns = L"style";
const wchar_t * style_master_page::name = L"master-page";

std::wostream & style_master_page::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void style_master_page::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void style_master_page::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"header" == Name)
        CP_CREATE_ELEMENT(style_header_);
	else if (L"style" == Ns && L"header-first" == Name)
        CP_CREATE_ELEMENT(style_header_first_);
	else if (L"style" == Ns && L"header-left" == Name)
        CP_CREATE_ELEMENT(style_header_left_);

	else if (L"style" == Ns && L"footer" == Name)
        CP_CREATE_ELEMENT(style_footer_);
	else if (L"style" == Ns && L"footer-first" == Name)
        CP_CREATE_ELEMENT(style_footer_first_);
	else if (L"style" == Ns && L"footer-left" == Name)
        CP_CREATE_ELEMENT(style_footer_left_);

    else if (L"office" == Ns && L"forms" == Name)
        CP_CREATE_ELEMENT(office_forms_);
    else if (L"style" == Ns && L"style" == Name)
        CP_CREATE_ELEMENT(style_style_);
    else if (L"presentation" == Ns && L"notes" == Name)
        CP_CREATE_ELEMENT(presentation_notes_); 
    else
    {
        CP_CREATE_ELEMENT(content_);   
    }
}

int style_master_page::find_placeHolderIndex(presentation_class::type placeHolder,int & last_idx)
{
	int idx = -1;

	for (size_t i = 0; i <  content_.size(); i++)
    {
		if (content_[i]->get_type() == odf_reader::typeDrawFrame)
		{
			draw_frame* frame = dynamic_cast<draw_frame *>(content_[i].get());
			if (frame)
			{
				odf_types::common_presentation_attlist	&common_presentation_attlist_ = frame->common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_;
				if (frame->idx_in_owner < 0)
					frame->idx_in_owner = last_idx++;

				if ((common_presentation_attlist_.presentation_class_) && 
							   (common_presentation_attlist_.presentation_class_->get_type()== placeHolder))
				{
					idx = frame->idx_in_owner;
					break;
				}
			}
			draw_shape* shape = dynamic_cast<draw_shape *>(content_[i].get());
			if (shape)
			{
				odf_types::common_presentation_attlist	&common_presentation_attlist_ = shape->common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_;
				if (shape->idx_in_owner < 0)
					shape->idx_in_owner = last_idx++;

				if ((common_presentation_attlist_.presentation_class_) && 
							   (common_presentation_attlist_.presentation_class_->get_type()== placeHolder))
				{
					idx = shape->idx_in_owner;
					break;
				}
			}

		}
    }
	return idx;
}

void style_master_page::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (attlist_.draw_style_name_)
	{
		std::wstring style_name = attlist_.draw_style_name_.get();
		style_instance * style_inst = Context.root()->odf_context().styleContainer().style_by_name(style_name,style_family::DrawingPage,true);
		
		if ((style_inst) && (style_inst->content()))
		{
			const style_drawing_page_properties * properties = style_inst->content()->get_style_drawing_page_properties();

			if (properties)
			{				
				oox::_oox_fill fill;
				Compute_GraphicFill(properties->content().common_draw_fill_attlist_, office_element_ptr(), 
																			Context.root()->odf_context().drawStyles() ,fill);
				Context.get_slide_context().add_background(fill);
			}
		}
	}
	
	for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->pptx_convert(Context);
	}

}
////////////////

const wchar_t * hdrHeader = L"<w:hdr \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
mc:Ignorable=\"w14 wp14\">";


const wchar_t * ftrHeader = L"<w:ftr \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
mc:Ignorable=\"w14 wp14\">";

void style_header::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();

    strm << hdrHeader;
    header_footer_impl::docx_convert(Context);
    
	strm << L"</w:hdr>";
}

void style_footer::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    strm << ftrHeader;
    header_footer_impl::docx_convert(Context);
    
	strm << L"</w:ftr>";
}

void style_header_first::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();

    strm << hdrHeader;
    header_footer_impl::docx_convert(Context);
    
	strm << L"</w:hdr>";
}

void style_footer_first::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    strm << ftrHeader;
    header_footer_impl::docx_convert(Context);
    
	strm << L"</w:ftr>";
}


void style_header_left::docx_convert(oox::docx_conversion_context & Context) 
{
    std::wostream & strm = Context.output_stream();
    strm << hdrHeader;
    header_footer_impl::docx_convert(Context);
    
	strm << L"</w:hdr>";
}

void style_footer_left::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    strm << ftrHeader;
    header_footer_impl::docx_convert(Context);
    
	strm << L"</w:ftr>";
}

void header_footer_impl::docx_convert(oox::docx_conversion_context & Context)
{
	if (content_.header_footer_content_.tracked_changes_)
		content_.header_footer_content_.tracked_changes_->docx_convert(Context);

    for (size_t i = 0; i < content_.header_footer_content_.content_.size(); i++)
    {
        content_.header_footer_content_.content_[i]->docx_convert(Context);
    }
}

/// text:notes-configuration
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_notes_configuration::ns = L"text";
const wchar_t * text_notes_configuration::name = L"notes-configuration";

void text_notes_configuration::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:note-class", noteclass_, noteclass(noteclass::Footnote));
    CP_APPLY_ATTR(L"text:citation-style-name", text_citation_style_name_);
    CP_APPLY_ATTR(L"text:citation-body-style-name", text_citation_body_style_name_);
    CP_APPLY_ATTR(L"text:default-style-name", text_default_style_name_);
    CP_APPLY_ATTR(L"text:master-page-name", text_master_page_name_);
    CP_APPLY_ATTR(L"text:start-value", text_start_value_);
    common_num_format_prefix_suffix_attlist_.add_attributes(Attributes);
    common_num_format_attlist_.add_attributes(Attributes);
    
    CP_APPLY_ATTR(L"text:start-numbering-at", text_start_numbering_at_);   
    CP_APPLY_ATTR(L"text:footnotes-position", text_footnotes_position_);

}

void text_notes_configuration::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    
    if CP_CHECK_NAME(L"text", L"note-continuation-notice-forward")
        CP_CREATE_ELEMENT(text_note_continuation_notice_forward_);
    else if CP_CHECK_NAME(L"text", L"note-continuation-notice-backward")
        CP_CREATE_ELEMENT(text_note_continuation_notice_backward_);
    else
        CP_NOT_APPLICABLE_ELM();    
}

void text_notes_configuration::add_text(const std::wstring & Text)
{
}

/// style:presentation-page-layout
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_presentation_page_layout::ns = L"style";
const wchar_t * style_presentation_page_layout::name = L"presentation-page-layout";

std::wostream & style_presentation_page_layout::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;    
} 

void style_presentation_page_layout::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:name", style_name_);
	//style:display-name
}

void style_presentation_page_layout::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"presentation" == Ns && L"placeholder" == Name)
    {
        CP_CREATE_ELEMENT(content_);        
    } 
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void style_presentation_page_layout::pptx_convert(oox::pptx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->pptx_convert(Context);
	}
}

}
}
