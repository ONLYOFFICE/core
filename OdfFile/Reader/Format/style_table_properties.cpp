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
#include "style_table_properties.h"

#include <xml/simple_xml_writer.h>

#include "serialize_elements.h"
#include "../../DataTypes/borderstyle.h"
#include "odfcontext.h"
#include "odf_document_impl.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



void table_format_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_horizontal_margin_attlist_.add_attributes(Attributes);
    common_vertical_margin_attlist_.add_attributes(Attributes);
    common_margin_attlist_.add_attributes(Attributes);
    common_page_number_attlist_.add_attributes(Attributes);
    common_break_attlist_.add_attributes(Attributes);
    common_background_color_attlist_.add_attributes(Attributes);
    common_shadow_attlist_.add_attributes(Attributes);
    common_keep_with_next_attlist_.add_attributes(Attributes);
    common_writing_mode_attlist_.add_attributes(Attributes);
   
	CP_APPLY_ATTR(L"style:width",					style_width_);
    CP_APPLY_ATTR(L"style:rel-width",				style_rel_width_);
    CP_APPLY_ATTR(L"style:may-break-between-rows",	style_may_break_between_rows_);
    
	CP_APPLY_ATTR(L"table:align",					table_align_);
    CP_APPLY_ATTR(L"table:border-model",			table_border_model_);
    CP_APPLY_ATTR(L"table:display",					table_display_);
	CP_APPLY_ATTR(L"tableooo:tab-color",			tableooo_tab_color_);			
	CP_APPLY_ATTR(L"style:use-optimal-column-width",style_use_optimal_column_width_);
}

bool table_format_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_background_image_);
        return true;
    }
    else
    {
        not_applicable_element(L"style-table-properties", Reader, Ns, Name);
        return false;
    }
}

void table_format_properties::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & _tblPr = Context.get_styles_context().table_style();

    if (style_rel_width_)
    {
        int w_w = (int)(0.5 + 50.0 * style_rel_width_->get_value());
        _tblPr << L"<w:tblW w:type=\"pct\" w:w=\"" <<  w_w << "\"/>";
    }
	else if (style_use_optimal_column_width_)
	{
       _tblPr << L"<w:tblW w:type=\"auto\" w:w=\"0\"/>";
	}    
	else if (style_width_)
    {
        int w_w = (int)(0.5 + 20.0 * style_width_->get_value_unit(length::pt));
 		if (w_w > 31680)w_w = 31680;
       _tblPr << L"<w:tblW w:type=\"dxa\" w:w=\"" <<  w_w << "\"/>";
    }

    else
    {
        _tblPr << L"<w:tblW w:type=\"pct\" w:w=\"5000\"/>";
    }

    if (common_break_attlist_.fo_break_before_)
    {
		Context.set_page_break_before(common_break_attlist_.fo_break_before_->get_type());
	}

    if (table_align_)
    {
        std::wstring w_val = L"left";
        if (table_align_->get_type() == table_align::Margins || table_align_->get_type() == table_align::Left)
		{
			if (common_horizontal_margin_attlist_.fo_margin_left_ && common_horizontal_margin_attlist_.fo_margin_right_)
			{
				if (common_horizontal_margin_attlist_.fo_margin_left_->get_length() == common_horizontal_margin_attlist_.fo_margin_right_->get_length())
					w_val = L"center";
			}
			if (w_val != L"center" && common_horizontal_margin_attlist_.fo_margin_left_ )
			{
				odf_types::length indent = common_horizontal_margin_attlist_.fo_margin_left_->get_length();
				
				_tblPr << L"<w:tblInd w:w=\"" << indent.get_value_unit(odf_types::length::pt) * 20 << "\" w:type=\"dxa\"/>";
			}
		}
        else //if (table_align_->get_type() == table_align::Center)
			w_val = boost::lexical_cast<std::wstring>(*table_align_);

        _tblPr << L"<w:jc w:val=\"" << w_val << "\"/>";
    }

	if (!style_use_optimal_column_width_)
	{
		_tblPr << "<w:tblLayout w:type=\"fixed\"/>";
	}

    if (common_background_color_attlist_.fo_background_color_)
    {
        const std::wstring w_fill = (common_background_color_attlist_.fo_background_color_->get_type() == background_color::Enabled 
            ? common_background_color_attlist_.fo_background_color_->get_color().get_hex_value() : L"auto");

        _tblPr << L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" << w_fill << "\"/>";
    }

}

// style:table-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_properties::ns = L"style";
const wchar_t * style_table_properties::name = L"table-properties";

void style_table_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    content_.add_attributes(Attributes);
}

void style_table_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content_.add_child_element(Reader, Ns, Name, getContext());
}

void style_table_properties::docx_convert(oox::docx_conversion_context & Context)
{
    content_.docx_convert(Context);
}

// style-table-column-properties-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void style_table_column_properties_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
   CP_APPLY_ATTR(L"style:column-width", style_column_width_);
   CP_APPLY_ATTR(L"style:rel-column-width", style_rel_column_width_);
   CP_APPLY_ATTR(L"style:use-optimal-column-width", style_use_optimal_column_width_);
   common_break_attlist_.add_attributes(Attributes);
}
void style_table_column_properties_attlist::apply_from(const style_table_column_properties_attlist & Other)
{
	_CP_APPLY_PROP(style_column_width_, Other.style_column_width_);
	_CP_APPLY_PROP(style_rel_column_width_, Other.style_rel_column_width_);
	_CP_APPLY_PROP(style_use_optimal_column_width_, Other.style_use_optimal_column_width_);

	common_break_attlist_.apply_from(Other.common_break_attlist_);
}
// style:table-column-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_column_properties::ns = L"style";
const wchar_t * style_table_column_properties::name = L"table-column-properties";

void style_table_column_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void style_table_column_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void style_table_column_properties::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();

	double page_width = 0;
	
	const page_layout_instance * pp = Context.root()->odf_context().pageLayoutContainer().page_layout_first();
	if ((pp) && (pp->properties()))
	{
		style_page_layout_properties_attlist & attr_page = pp->properties()->attlist_;
		if (attr_page.fo_page_width_)
		{
			page_width = attr_page.fo_page_width_->get_value_unit(odf_types::length::pt);
		}
		if (attr_page.common_horizontal_margin_attlist_.fo_margin_left_)
		{
			page_width -= attr_page.common_horizontal_margin_attlist_.fo_margin_left_->get_length().get_value_unit(odf_types::length::pt);
		}
		if (attr_page.common_horizontal_margin_attlist_.fo_margin_right_)
		{
			page_width -= attr_page.common_horizontal_margin_attlist_.fo_margin_right_->get_length().get_value_unit(odf_types::length::pt);
		}
	}	
    if (attlist_.style_column_width_)
    {
		double kf_max_width_ms = 1.;

		int val =  (int)(0.5 + 20.0 * page_width);
		if (val > 31680.)
			kf_max_width_ms = 31680./val;

		double width = 0.5 + 20.0 * attlist_.style_column_width_->get_value_unit(length::pt) * kf_max_width_ms;

		Context.get_table_context().add_column_width(width);
		if (width > 0)
			strm << L"<w:gridCol w:w=\"" << (int)(width) << "\"/>";
		else
			strm << L"<w:gridCol/>";
    }
	else if ((attlist_.style_rel_column_width_) && (attlist_.style_rel_column_width_->get_unit() == length::rel))
	{
		double width = 0.5 + 20.0 * page_width * attlist_.style_rel_column_width_->get_value() / 65534.;
		
		Context.get_table_context().add_column_width(width);
		if (width > 0)
			strm << L"<w:gridCol w:w=\"" << (int)(width) << "\"/>";
		else
			strm << L"<w:gridCol/>";
	}
	else
	{
		Context.get_table_context().add_column_width(0);
		strm << L"<w:gridCol/>";
	}
}


void style_table_column_properties::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wostream & strm = Context.get_table_context().tableData();

    if (attlist_.style_column_width_)
    {
		int val = (int)attlist_.style_column_width_->get_value_unit(length::emu);

        strm << L"<a:gridCol w=\"" << val << "\"/>";
    }
}


void style_table_column_properties::xlsx_convert(oox::xlsx_conversion_context & Context)
{
                    
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void style_table_cell_properties_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:vertical-align"				, style_vertical_align_);
    CP_APPLY_ATTR(L"style:text-align-source"			, style_text_align_source_);
    CP_APPLY_ATTR(L"style:direction"					, style_direction_);
    CP_APPLY_ATTR(L"style:glyph-orientation-vertical"	, style_glyph_orientation_vertical_);
    
	common_shadow_attlist_.add_attributes(Attributes);
    common_background_color_attlist_.add_attributes(Attributes);
    common_border_attlist_.add_attributes(Attributes);
    common_border_line_width_attlist_.add_attributes(Attributes);
    
	CP_APPLY_ATTR(L"style:diagonal-tl-br"			, style_diagonal_tl_br_);
    CP_APPLY_ATTR(L"style:diagonal-tl-br-widths"	, style_diagonal_tl_br_widths_);
    CP_APPLY_ATTR(L"style:diagonal-bl-tr"			, style_diagonal_bl_tr_);
    CP_APPLY_ATTR(L"style:diagonal-bl-tr-widths"	, style_diagonal_bl_tr_widths_);
    
	common_padding_attlist_.add_attributes(Attributes);
    common_rotation_angle_attlist_.add_attributes(Attributes);
   
	CP_APPLY_ATTR(L"fo:wrap-option"			, fo_wrap_option_);
    CP_APPLY_ATTR(L"style:rotation-align"	, style_rotation_align_);    
    CP_APPLY_ATTR(L"style:cell-protect"		, style_cell_protect_);
    CP_APPLY_ATTR(L"style:print-content"	, style_print_content_);
    CP_APPLY_ATTR(L"style:decimal-places"	, style_decimal_places_);
    CP_APPLY_ATTR(L"style:repeat-content"	, style_repeat_content_);
    CP_APPLY_ATTR(L"style:shrink-to-fit"	, style_shrink_to_fit_);
}


/// style:table-cell-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_cell_properties::ns = L"style";
const wchar_t * style_table_cell_properties::name = L"table-cell-properties";

void style_table_cell_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void style_table_cell_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);        
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();        
    }
}

// style-table-row-properties-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void style_table_row_properties_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:row-height", style_row_height_);
    CP_APPLY_ATTR(L"style:min-row-height", style_min_row_height_);
    CP_APPLY_ATTR(L"style:use-optimal-row-height", style_use_optimal_row_height_);
    common_background_color_attlist_.add_attributes(Attributes);
    common_break_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"fo:keep-together", fo_keep_together_);     
}
void style_table_row_properties_attlist::apply_from(const style_table_row_properties_attlist & Other)
{
	_CP_APPLY_PROP(style_row_height_, Other.style_row_height_);
	_CP_APPLY_PROP(style_min_row_height_, Other.style_min_row_height_);
	_CP_APPLY_PROP(style_use_optimal_row_height_, Other.style_use_optimal_row_height_);
	_CP_APPLY_PROP(fo_keep_together_, Other.fo_keep_together_);

	common_background_color_attlist_.apply_from(Other.common_background_color_attlist_);
	common_break_attlist_.apply_from(Other.common_break_attlist_);
}
void style_table_row_properties_attlist::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    
    if (style_row_height_)
    {
        int val = (int)( 0.5 + 20.0 * style_row_height_->get_value_unit(length::pt) );
        if (val < 0)
            val = 0;
        strm << L"<w:trHeight w:hRule=\"exact\" w:val=\"" << val << L"\" />";
    }
    else if (style_min_row_height_)
    {
        int val = (int)( 0.5 + 20.0 * style_min_row_height_->get_value_unit(length::pt) );
        if (val < 0)
            val = 0;
        strm << L"<w:trHeight w:hRule=\"atLeast\" w:val=\"" << val << L"\" />";
    }

    if (fo_keep_together_ && fo_keep_together_->get_type() == keep_together::Always)
    {
        strm << L"<w:cantSplit w:val=\"true\" />";
    }
}



//  style:table-row-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_row_properties::ns = L"style";
const wchar_t * style_table_row_properties::name = L"table-row-properties";

void style_table_row_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void style_table_row_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);        
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();        
    }
}

void style_table_row_properties::docx_convert(oox::docx_conversion_context & Context) 
{
    attlist_.docx_convert(Context);
}

/////

void insert_cell_margin(oox::docx_conversion_context & Context,
                        const std::wstring & Side,
                        const length & Length)
{
    std::wostream & strm = Context.output_stream();
    strm<< L"<w:" << Side << L" w:type=\"dxa\" w:w=\"" <<  (int)(0.5 + 20.0 * Length.get_value_unit(length::pt)) << L"\" />";
}

void insert_cell_border(oox::docx_conversion_context & Context, 
                        const std::wstring & Side,
                        const _CP_OPT(border_style) & BorderStyle,
                        const _CP_OPT(border_widths) & BorderWidths
                        )
{
    std::wostream & strm = Context.output_stream();
    
    std::wstring w_color;
    std::wstring w_sz;
    std::wstring w_val;
    bool none = false;
    try 
    {
        if (BorderStyle)
        {
            if (BorderStyle->is_none())
            {
				strm << L"<w:" << Side << " ";
				strm << L"w:val=\"none\" ";
				strm << L"/>";
                none = true;
            }else
			{
				w_color = BorderStyle->get_color().get_hex_value();

                if (BorderStyle->get_style() == border_style::double_)
				{
					w_val = L"double";
					if (BorderWidths)
					{
						if (BorderWidths->get_len1().get_value_unit(length::pt) > BorderWidths->get_len3().get_value_unit(length::pt))
							w_val = L"thickThinMediumGap";
						else if (BorderWidths->get_len1().get_value_unit(length::pt) < BorderWidths->get_len3().get_value_unit(length::pt))
							w_val = L"thinThickMediumGap";
					}
				}
				else
					w_val = L"single";

				double w_sz_ = BorderStyle->get_length().get_value_unit(length::pt);

				//if (BorderStyle.get_style() == L"double")
				//  w_sz_ /= 2;            

				w_sz_ = (int)(0.5 + 8.0 * w_sz_);
				if (w_sz_ < 2.0)
					w_sz_ = 2.0;
				else if (w_sz_ > 96.0)
					w_sz_ = 96.0;

				w_sz = std::to_wstring( w_sz_ );
			}
        } 
    }
    catch(...)
    {
    }


    if (!none)
    {
        strm << L"<w:" << Side << " ";
        if (!w_color.empty())
            strm << L"w:color=\"" << w_color << "\" ";
		strm << L"w:space=\"0\" ";
        if (!w_sz.empty())
            strm << L"w:sz=\"" << w_sz << "\" ";
        if (!w_val.empty())
            strm << L"w:val=\"" << w_val << "\" ";

        strm << L"/>";
    }
}


void insert_cell_border(oox::pptx_conversion_context & Context, 
                        const std::wstring & Side,
                        const std::wstring & BorderStyle,
                        const _CP_OPT(border_widths) & BorderWidths
                        )
{
    std::wostream & strm = Context.get_table_context().tableData();
    
    std::wstring w_color;
    std::wstring w_sz;
    std::wstring w_val;
    bool none = false;
    try 
    {
        do 
        {
            border_style borderStyle(BorderStyle);
            if (borderStyle.is_none())
            {
				strm << L"<a:" << Side << " ";
				strm << L"val=\"none\" ";
				strm << L"/>";
                none = true;
                break;
            }

            w_color = borderStyle.get_color().get_hex_value();

            if (borderStyle.get_style() == border_style::double_)
            {
                w_val = L"double";
                if (BorderWidths)
                {
                    if (BorderWidths->get_len1().get_value_unit(length::pt) > BorderWidths->get_len3().get_value_unit(length::pt))
                        w_val = L"thickThinMediumGap";
                    else if (BorderWidths->get_len1().get_value_unit(length::pt) < BorderWidths->get_len3().get_value_unit(length::pt))
                        w_val = L"thinThickMediumGap";
                }
            }
            else
                w_val = L"single";

            double w_sz_ = borderStyle.get_length().get_value_unit(length::pt);

            //if (borderStyle.get_style() == L"double")
            //  w_sz_ /= 2;            

            w_sz_ = (int)(0.5 + 8.0 * w_sz_);
            if (w_sz_ < 2.0)
                w_sz_ = 2.0;
            else if (w_sz_ > 96.0)
                w_sz_ = 96.0;

            w_sz = std::to_wstring( w_sz_ );
            
        } 
        while (0);
    }
    catch(...)
    {
    }


    if (!none)
    {
        strm << L"<a:" << Side << " ";
        if (!w_color.empty())
            strm << L"color=\"" << w_color << "\" ";
		strm << L"space=\"0\" ";
        if (!w_sz.empty())
            strm << L"sz=\"" << w_sz << "\" ";
        if (!w_val.empty())
            strm << L"val=\"" << w_val << "\" ";

        strm << L"/>";
    }
}


void style_table_cell_properties_attlist::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();

    if (style_vertical_align_)
    {
        std::wstring vAlign;
        switch(style_vertical_align_->get_type())
        {
        case vertical_align::Baseline: vAlign = L"top";  break;
        case vertical_align::Top:      vAlign = L"top"; break;
        case vertical_align::Middle:   vAlign = L"center"; break;
        case vertical_align::Bottom:   vAlign = L"bottom"; break;
        case vertical_align::Auto:  break;
        }
        if (!vAlign.empty())
            strm << L"<w:vAlign w:val=\"" << vAlign << "\" />";        
    }

    strm << L"<w:tcBorders>";

    if (common_border_attlist_.fo_border_)
    {
        insert_cell_border(Context, L"top",		*common_border_attlist_.fo_border_, common_border_line_width_attlist_.style_border_line_width_);
        insert_cell_border(Context, L"left",	*common_border_attlist_.fo_border_, common_border_line_width_attlist_.style_border_line_width_);
        insert_cell_border(Context, L"bottom",	*common_border_attlist_.fo_border_, common_border_line_width_attlist_.style_border_line_width_);
        insert_cell_border(Context, L"right",	*common_border_attlist_.fo_border_, common_border_line_width_attlist_.style_border_line_width_);
    }
    else
    {
        if (common_border_attlist_.fo_border_top_)
            insert_cell_border(Context, L"top", *common_border_attlist_.fo_border_top_, common_border_line_width_attlist_.style_border_line_width_top_);
        if (common_border_attlist_.fo_border_left_)
            insert_cell_border(Context, L"left", *common_border_attlist_.fo_border_left_, common_border_line_width_attlist_.style_border_line_width_left_);
        if (common_border_attlist_.fo_border_bottom_)
            insert_cell_border(Context, L"bottom", *common_border_attlist_.fo_border_bottom_, common_border_line_width_attlist_.style_border_line_width_bottom_);
        if (common_border_attlist_.fo_border_right_)
            insert_cell_border(Context, L"right", *common_border_attlist_.fo_border_right_, common_border_line_width_attlist_.style_border_line_width_right_);
    }
    
    strm << L"</w:tcBorders>";

    if (common_background_color_attlist_.fo_background_color_)
    {
        const std::wstring w_fill = (common_background_color_attlist_.fo_background_color_->get_type() == background_color::Enabled 
            ? common_background_color_attlist_.fo_background_color_->get_color().get_hex_value() : L"auto");

        strm << L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" << w_fill << "\" />";        
    }

    if (common_padding_attlist_.fo_padding_ || 
        common_padding_attlist_.fo_padding_top_ || 
        common_padding_attlist_.fo_padding_bottom_ || 
        common_padding_attlist_.fo_padding_left_ || 
        common_padding_attlist_.fo_padding_right_)
    {
        strm << L"<w:tcMar>";
        if (common_padding_attlist_.fo_padding_)
        {
            insert_cell_margin(Context, L"top", *common_padding_attlist_.fo_padding_);
            insert_cell_margin(Context, L"bottom", *common_padding_attlist_.fo_padding_);
            insert_cell_margin(Context, L"left", *common_padding_attlist_.fo_padding_);
            insert_cell_margin(Context, L"right", *common_padding_attlist_.fo_padding_);
        }
        else
        {
            if (common_padding_attlist_.fo_padding_top_)
                insert_cell_margin(Context, L"top", *common_padding_attlist_.fo_padding_top_);            
            
            if (common_padding_attlist_.fo_padding_bottom_)
                insert_cell_margin(Context, L"bottom", *common_padding_attlist_.fo_padding_bottom_);                        
            
            if (common_padding_attlist_.fo_padding_left_)
                insert_cell_margin(Context, L"left", *common_padding_attlist_.fo_padding_left_);                        

            if (common_padding_attlist_.fo_padding_right_)
                insert_cell_margin(Context, L"right", *common_padding_attlist_.fo_padding_right_);            
        }
        strm << L"</w:tcMar>";
    }
}

void style_table_cell_properties_attlist::pptx_serialize(oox::pptx_conversion_context & Context, std::wostream & strm)
{
    //strm << L"<a:tcBorders>";

    //if (common_border_attlist_.fo_border_)
    //{
    //    insert_cell_border(Context, L"top", *common_border_attlist_.fo_border_, common_border_line_width_attlist_.style_border_line_width_);
    //    insert_cell_border(Context, L"left", *common_border_attlist_.fo_border_, common_border_line_width_attlist_.style_border_line_width_);
    //    insert_cell_border(Context, L"bottom", *common_border_attlist_.fo_border_, common_border_line_width_attlist_.style_border_line_width_);
    //    insert_cell_border(Context, L"right", *common_border_attlist_.fo_border_, common_border_line_width_attlist_.style_border_line_width_);
    //}
    //else
    //{
    //    if (common_border_attlist_.fo_border_top_)
    //        insert_cell_border(Context, L"top", *common_border_attlist_.fo_border_top_, common_border_line_width_attlist_.style_border_line_width_top_);
    //    if (common_border_attlist_.fo_border_left_)
    //        insert_cell_border(Context, L"left", *common_border_attlist_.fo_border_left_, common_border_line_width_attlist_.style_border_line_width_left_);
    //    if (common_border_attlist_.fo_border_bottom_)
    //        insert_cell_border(Context, L"bottom", *common_border_attlist_.fo_border_bottom_, common_border_line_width_attlist_.style_border_line_width_bottom_);
    //    if (common_border_attlist_.fo_border_right_)
    //        insert_cell_border(Context, L"right", *common_border_attlist_.fo_border_right_, common_border_line_width_attlist_.style_border_line_width_right_);
    //}
    //
    //strm << L"</a:tcBorders>";

    //if (common_background_color_attlist_.fo_background_color_)
    //{
    //    const std::wstring w_fill = (common_background_color_attlist_.fo_background_color_->get_type() == background_color::Enabled 
    //        ? common_background_color_attlist_.fo_background_color_->get_color().get_hex_value() : L"auto");

    //    strm << L"<a:shd val=\"clear\" color=\"auto\" fill=\"" << w_fill << "\" />";        
    //}
}

void style_table_cell_properties::docx_convert(oox::docx_conversion_context & Context) 
{
    attlist_.docx_convert(Context);    
}

void style_table_cell_properties_attlist::apply_from(const style_table_cell_properties_attlist & Other)
{
     _CP_APPLY_PROP(style_vertical_align_				, Other.style_vertical_align_);
     _CP_APPLY_PROP(style_text_align_source_			, Other.style_text_align_source_);
     _CP_APPLY_PROP(style_direction_					, Other.style_direction_);
     _CP_APPLY_PROP(style_glyph_orientation_vertical_	, Other.style_glyph_orientation_vertical_);
    
    common_shadow_attlist_.apply_from			(Other.common_shadow_attlist_);
    common_background_color_attlist_.apply_from	(Other.common_background_color_attlist_);
    common_border_attlist_.apply_from			(Other.common_border_attlist_);

    _CP_APPLY_PROP(style_diagonal_tl_br_			, Other.style_diagonal_tl_br_);
    _CP_APPLY_PROP(style_diagonal_tl_br_widths_		, Other.style_diagonal_tl_br_widths_);
    _CP_APPLY_PROP(style_diagonal_bl_tr_			, Other.style_diagonal_bl_tr_);
    _CP_APPLY_PROP(style_diagonal_bl_tr_widths_		, Other.style_diagonal_bl_tr_widths_);

    common_border_line_width_attlist_.apply_from(Other.common_border_line_width_attlist_);
    common_padding_attlist_.apply_from			(Other.common_padding_attlist_);
    common_rotation_angle_attlist_.apply_from	(Other.common_rotation_angle_attlist_);
    
	_CP_APPLY_PROP(fo_wrap_option_			, Other.fo_wrap_option_);
	_CP_APPLY_PROP(style_rotation_align_	, Other.style_rotation_align_);    
    _CP_APPLY_PROP(style_cell_protect_		, Other.style_cell_protect_);
    _CP_APPLY_PROP(style_print_content_		, Other.style_print_content_);
    _CP_APPLY_PROP(style_decimal_places_	, Other.style_decimal_places_);
    _CP_APPLY_PROP(style_repeat_content_	, Other.style_repeat_content_);
    _CP_APPLY_PROP(style_shrink_to_fit_		, Other.style_shrink_to_fit_);    
}

}
}
