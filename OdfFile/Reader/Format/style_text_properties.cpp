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

#include "../../Common/logging.h"
#include <xml/simple_xml_writer.h>

#include "../Format/odf_document.h"
#include "calcs_styles.h"

#include "odfcontext.h"
#include "../../DataTypes/fontvariant.h"

#include "draw_shapes.h"

#include "../../Reader/Converter/oox_drawing_fills.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

void removeCharsFromString( std::wstring &str, std::wstring charsToRemove ) 
{
   for ( size_t i = 0; i < charsToRemove.length(); ++i ) 
   {
	   str.erase( std::remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
   }
}

std::wstring delete_apostroph_in_name(std::wstring value)
{
	if (value.length()<1)return value;

	if (value[0] == 0x27 && value[value.length()-1] == 0x27)
	{
		return value.substr(1,value.length()-2);
	}
	return value;
}

void text_format_properties_content::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"fo:font-variant",					fo_font_variant_);
    CP_APPLY_ATTR(L"fo:text-transform",					fo_text_transform_);
    CP_APPLY_ATTR(L"fo:color",							fo_color_);
    CP_APPLY_ATTR(L"fo:font-family",					fo_font_family_);
    CP_APPLY_ATTR(L"fo:font-size",						fo_font_size_);
    CP_APPLY_ATTR(L"fo:letter-spacing",					fo_letter_spacing_);
    CP_APPLY_ATTR(L"fo:language",						fo_language_);
    CP_APPLY_ATTR(L"fo:country",						fo_country_);
    CP_APPLY_ATTR(L"fo:font-style",						fo_font_style_);
    CP_APPLY_ATTR(L"fo:text-shadow",					fo_text_shadow_);
    CP_APPLY_ATTR(L"fo:font-weight",					fo_font_weight_);
    CP_APPLY_ATTR(L"fo:background-color",				fo_background_color_);
    CP_APPLY_ATTR(L"fo:hyphenate",						fo_hyphenate_);
    CP_APPLY_ATTR(L"fo:hyphenation-remain-char-count",	fo_hyphenation_remain_char_count_);
    CP_APPLY_ATTR(L"fo:hyphenation-push-char-count",	fo_hyphenation_push_char_count_);

    CP_APPLY_ATTR(L"style:use-window-font-color",		style_use_window_font_color_);
    CP_APPLY_ATTR(L"style:text-outline",				style_text_outline_);
    CP_APPLY_ATTR(L"style:text-line-through-type",		style_text_line_through_type_);
    CP_APPLY_ATTR(L"style:text-line-through-style",		style_text_line_through_style_);
    CP_APPLY_ATTR(L"style:text-line-through-width",		style_text_line_through_width_);
    CP_APPLY_ATTR(L"style:text-line-through-color",		style_text_line_through_color_);
    CP_APPLY_ATTR(L"style:text-line-through-text",		style_text_line_through_text_);
    CP_APPLY_ATTR(L"style:text-line-through-text-style", style_text_line_through_text_style_);
    CP_APPLY_ATTR(L"style:text-position",				style_text_position_);
    CP_APPLY_ATTR(L"style:font-name",					style_font_name_);
    CP_APPLY_ATTR(L"style:font-name-asian",				style_font_name_asian_);
    CP_APPLY_ATTR(L"style:font-name-complex",			style_font_name_complex_);
    CP_APPLY_ATTR(L"style:font-family-asian",			style_font_family_asian_);
    CP_APPLY_ATTR(L"style:font-family-complex",			style_font_family_complex_);
    CP_APPLY_ATTR(L"style:font-family-generic",			style_font_family_generic_);
    CP_APPLY_ATTR(L"style:font-family-generic-asian",	style_font_family_generic_asian_);
    CP_APPLY_ATTR(L"style:font-family-generic-complex", style_font_family_generic_complex_);
    CP_APPLY_ATTR(L"style:font-style-name",				style_font_style_name_);
    CP_APPLY_ATTR(L"style:font-style-name-asian",		style_font_style_name_asian_);
    CP_APPLY_ATTR(L"style:font-style-name-complex",		style_font_style_name_complex_);
    CP_APPLY_ATTR(L"style:font-pitch",					style_font_pitch_);
    CP_APPLY_ATTR(L"style:font-pitch",					style_font_pitch_asian_);
    CP_APPLY_ATTR(L"style:font-pitch-complex",			style_font_pitch_complex_);
    CP_APPLY_ATTR(L"style:font-charset",				style_font_charset_);
    CP_APPLY_ATTR(L"style:font-charset-asian",			style_font_charset_asian_);
    CP_APPLY_ATTR(L"style:font-charset-complex",		style_font_charset_complex_);
    CP_APPLY_ATTR(L"style:font-size-asian",				style_font_size_asian_);
    CP_APPLY_ATTR(L"style:font-size-complex",			style_font_size_complex_);
    CP_APPLY_ATTR(L"style:font-size-rel",				style_font_size_rel_);
    CP_APPLY_ATTR(L"style:font-size-rel-asian",			style_font_size_rel_asian_);
    CP_APPLY_ATTR(L"style:font-size-rel-complex",		style_font_size_rel_complex_);
    CP_APPLY_ATTR(L"style:script-type",					style_script_type_);
    CP_APPLY_ATTR(L"style:language-asian",				style_language_asian_);
    CP_APPLY_ATTR(L"style:language-complex",			style_language_complex_);
    CP_APPLY_ATTR(L"style:country-asian",				style_country_asian_);
    CP_APPLY_ATTR(L"style:country-complex",				style_country_complex_);
    CP_APPLY_ATTR(L"style:font-style-asian",			style_font_style_asian_);
    CP_APPLY_ATTR(L"style:font-style-complex",			style_font_style_complex_);
    CP_APPLY_ATTR(L"style:font-relief",					style_font_relief_);
    CP_APPLY_ATTR(L"style:text-underline-type",			style_text_underline_type_);
    CP_APPLY_ATTR(L"style:text-underline-style",		style_text_underline_style_);
    CP_APPLY_ATTR(L"style:text-underline-width",		style_text_underline_width_);
    CP_APPLY_ATTR(L"style:text-underline-color",		style_text_underline_color_);
    CP_APPLY_ATTR(L"style:font-weight-asian",			style_font_weight_asian_);
    CP_APPLY_ATTR(L"style:font-weight-complex",			style_font_weight_complex_);
    CP_APPLY_ATTR(L"style:text-underline-mode",			style_text_underline_mode_);
    CP_APPLY_ATTR(L"style:text-line-through-mode",		style_text_line_through_mode_);
    CP_APPLY_ATTR(L"style:letter-kerning",				style_letter_kerning_);
    CP_APPLY_ATTR(L"style:text-blinking",				style_text_blinking_);
    CP_APPLY_ATTR(L"style:text-combine",				style_text_combine_);
    CP_APPLY_ATTR(L"style:text-combine-start-char",		style_text_combine_start_char_);
    CP_APPLY_ATTR(L"style:text-combine-end-char",		style_text_combine_end_char_);
    CP_APPLY_ATTR(L"style:text-emphasize",				style_text_emphasize_);
    CP_APPLY_ATTR(L"style:text-scale",					style_text_scale_);
    CP_APPLY_ATTR(L"style:text-rotation-angle",			style_text_rotation_angle_);
    CP_APPLY_ATTR(L"style:text-rotation-scale",			style_text_rotation_scale_);
    CP_APPLY_ATTR(L"style:text-overline-color",			style_text_overline_color_);
    CP_APPLY_ATTR(L"style:text-overline-mode",			style_text_overline_mode_);
    CP_APPLY_ATTR(L"style:text-overline-style",			style_text_overline_style_);

    CP_APPLY_ATTR(L"text:display",						text_display_);
    CP_APPLY_ATTR(L"text:condition",					text_condition_);
}



int text_format_properties_content::process_font_size(const _CP_OPT(font_size) & FontSize, const style_instance * currnetStyle, bool Complex, double Mul)
{
    if (FontSize)
    {
        double val = process_font_size_impl(FontSize, currnetStyle, Complex, Mul);
        if (val > 0)
            return (int)(val + 0.5);
    }
    return 0;
}

double text_format_properties_content::process_font_size_impl(const _CP_OPT(font_size) & FontSize, const style_instance * currnetStyle, bool Complex, double Mul)
{
    font_size usedFontSize = (!FontSize) ? font_size(percent(100)) : *FontSize;

    if (usedFontSize.get_type() == font_size::Length)
    {
		const double w_val = 2.0 * Mul * usedFontSize.get_length().get_value_unit(length::pt);
        return w_val;
    } 
    else if (usedFontSize.get_type() == font_size::Percent)
    {
        if (currnetStyle)
        {
            const style_instance * parentStyle = currnetStyle->parent();
            if (!parentStyle)
            {
                if (currnetStyle->container())
                    parentStyle = currnetStyle->container()->style_default_by_type(style_family::Paragraph);
            }
            else
            {
                if (const style_text_properties * textProp = parentStyle->content()->get_style_text_properties())
                {
                    return process_font_size_impl(textProp->content_.fo_font_size_, parentStyle, Complex, Mul * usedFontSize.get_percent().get_value() / 100.0);
                }
            }
        }
    }
    else
    {
        //_CP_LOG << L"[docx_convert] convert font_size error (" << FontSize << ")\n";
    }

    return -1.0;
}

int text_format_properties_content::process_font_weight(const _CP_OPT(font_weight) & FontWeight)
{
    if (FontWeight)
    {
		if (FontWeight->get_type() == font_weight::WNormal)
            return -1;
        else
            return +1; //bold
    }
    return 0; //not set
}

int text_format_properties_content::process_font_style(const _CP_OPT(font_style) & FontStyle)
{
    if (FontStyle)
    {
        if (FontStyle->get_type() == font_style::Italic || FontStyle->get_type() == font_style::Oblique) // TODO ???
            return 1;
        else
            return -1;
    }
    return 0;   
}
void text_format_properties_content::pptx_convert_as_list(oox::pptx_conversion_context & Context)
{
	oox::styles_context & styles_context_ = Context.get_text_context().get_styles_context();
	CP_XML_WRITER(styles_context_.text_style())
	{  
		if (fo_color_)
		{
			CP_XML_NODE(L"a:buClr")
			{
                _CP_OPT(double) opacity;
                oox::oox_serialize_srgb(CP_XML_STREAM(),fo_color_->get_hex_value(), opacity);
			}
		}
		if (fo_font_size_)
		{
			if(fo_font_size_->get_type() == font_size::Percent)
			{
				double fontSizePct = fo_font_size_->get_percent().get_value();
				CP_XML_NODE(L"a:buSzPct")
				{
					CP_XML_ATTR(L"val", int(fontSizePct * 1000 + 0.5));//% * 1000
				}		
			}
			else
			{
				double fontSizePt = fo_font_size_->get_length().get_value();
				CP_XML_NODE(L"a:buSzPts")
				{
					CP_XML_ATTR(L"val", (int)(fontSizePt * 100 + 0.5));//pt * 100
				}
			}
		}
			
		//const int fontStyle = process_font_style(fo_font_style_);
		//if (fontStyle>0)CP_XML_ATTR(L"i",true);
		
		//if (fo_language_ || style_language_asian_ || style_language_complex_)
		//{
		//	std::wstring w_val;
		//	if (fo_language_)		w_val = *fo_language_;
		//	else if (fo_country_)	w_val = *fo_country_;
		//	else if (style_country_asian_)w_val = *style_country_asian_;
		//	else if (style_language_asian_)w_val = *style_language_asian_;
		//	else if (style_language_complex_)w_val = *style_language_complex_;
		//	else if (style_country_complex_)w_val = *style_country_complex_;

		//	CP_XML_ATTR(L"lang",  w_val);
		//}
		if (style_font_name_ || style_font_name_asian_ || style_font_name_complex_ || fo_font_family_)
		{
			std::wstring w_font	= (fo_font_family_ ? *fo_font_family_: L""); 

			if (w_font.empty())
			{
				std::wstring w_ascii	= (style_font_name_			? *style_font_name_			: L"");
				std::wstring w_eastAsia = (style_font_name_asian_	? *style_font_name_asian_	: L"");
				std::wstring w_cs		= (style_font_name_complex_ ? *style_font_name_complex_	: L"");
				
				fonts_container & fonts = Context.root()->odf_context().fontContainer();	        
		
				font_instance *		font = fonts.font_by_style_name(w_ascii);
				if (font == NULL)	font = fonts.font_by_style_name(w_eastAsia);
				if (font == NULL)	font = fonts.font_by_style_name(w_cs);
				
				if (font)		
					w_font = font->name();

				//'Arial' глючит
				removeCharsFromString(w_font, L"'");
			}

			if (w_font.length()>0)
			{				
				CP_XML_NODE(L"a:buFont")
				{			
					removeCharsFromString(w_font, L"'");
					CP_XML_ATTR(L"typeface", w_font);
					if ((style_font_charset_))
					{		
						if ((*style_font_charset_!=L"x-symbol"))	CP_XML_ATTR(L"charset",(*style_font_charset_));
						//else CP_XML_ATTR(L"charset",0x02);
					}				
				}
			}
		}
	}
}
void text_format_properties_content::drawing_serialize(std::wostream & strm, std::wstring node, fonts_container & fonts, const odf_reader::style_instance *current_style, std::wstring hlink)
{
	CP_XML_WRITER(strm)
	{  
		CP_XML_NODE(node)//a:rPr & a:defRPr
		{
	//attr
			if (fo_language_ || style_language_asian_ || style_language_complex_)
			{
				std::wstring w_val;
				if (fo_language_)	
				{
					w_val = *fo_language_;
					if (fo_country_) w_val += L"-" + *fo_country_;
				}
				else if (style_language_asian_)	
				{
					w_val = *style_language_asian_;
					if (style_country_asian_) w_val += L"-" + *style_country_asian_;
				}
				else if (style_language_complex_)
				{
					w_val = *style_language_complex_;
					if (style_country_complex_)w_val += L"-" + *style_country_complex_;
				}

				if (w_val.empty() == false)
				{
					CP_XML_ATTR(L"lang",  w_val);
				}
			}
			
			const int fontStyle = process_font_style(fo_font_style_);
			if (fontStyle > 0) CP_XML_ATTR(L"i", true);
			
			double fontSizeVal = (fo_font_size_) ?	process_font_size_impl(fo_font_size_, current_style) : 
													process_font_size_impl(font_size(percent(100.0)), current_style);		
			double mul = 1;
			
			if ((style_text_position_) && (style_text_position_->has_font_size()))
			{
				mul = style_text_position_->font_size().get_value() / 100.0;
			}
			if (fontSizeVal > 0)
			{
				CP_XML_ATTR(L"sz", (int)(fontSizeVal/2. * mul *100 + 0.5));//in pt *100 
			}
			if (fo_font_variant_)
			{
				if (fo_font_variant_->get_type() == font_variant::SmallCaps)
				{
					CP_XML_ATTR(L"cap", "small");
				}
			}			
			std::wstring underline = L"";
			if (fo_text_transform_)
			{
				if (fo_text_transform_->get_type() == text_transform::Uppercase)
				{
					CP_XML_ATTR(L"cap", "all");
				}
				else if (fo_text_transform_->get_type() == text_transform::Lowercase)
				{
					CP_XML_ATTR(L"cap", "small");
				}
				else if (fo_text_transform_->get_type() == text_transform::Underline)
				{
					underline = L"sng";
				}
			}
			const int W = process_font_weight(fo_font_weight_);
			if (W > 0) CP_XML_ATTR(L"b", true);

		// underline
			line_width under	=	style_text_underline_width_.get_value_or(line_width::Auto);
			bool underlineBold	=	under.get_type() == line_width::Bold	|| 
									under.get_type() == line_width::Thick;			

            if ((style_text_underline_type_ && style_text_underline_type_->get_type() == line_type::None) ||
				(style_text_underline_style_ && style_text_underline_style_->get_type() == line_style::None))
			{
				underline = L"none";
			}
			else if (style_text_underline_type_ && 
				(!style_text_underline_style_ || style_text_underline_style_ && style_text_underline_style_->get_type() == line_style::Solid) )
			{
				if (underlineBold) underline = L"thick"; 

				switch (style_text_underline_type_->get_type())
				{
				case line_type::Single:	underline = L"sng";		break;
				case line_type::Double:	underline = L"dbl";		break;
				}
			}
			else if (style_text_underline_style_)
			{
				switch (style_text_underline_style_->get_type())
				{
				case line_style::Solid:
					if (underlineBold)	underline = L"heavy"; 
					else				underline = L"sng";
					break;
				case line_style::Dotted:
					if (underlineBold)	underline = L"dottedHeavy"; 
					else				underline = L"dotted";
					break;
				case line_style::Dash:
					if (underlineBold)	underline = L"dashedHeavy"; 
					else				underline = L"dash";
					break;
				case line_style::LongDash:
					if (underlineBold)	underline = L"dashLongHeavy"; 
					else				underline = L"dashLong";
					break;
				case line_style::DotDash:
					if (underlineBold)	underline = L"dashDotHeavy"; 
					else				underline = L"dotDash";
					break;
				case line_style::DotDotDash:
					if (underlineBold)	underline = L"dashDotDotHeavy"; 
					else				underline = L"dotDotDash";
					break;
				case line_style::Wave:
					if (underlineBold)	underline = L"wavyHeavy"; 
					else if (style_text_underline_type_.get_value_or( line_type(line_type::Single) ).get_type() == line_type::Double)
										underline = L"wavyDbl"; 
					else				underline = L"wavy"; 
					break;
				}
			}
			if (!underline.empty())CP_XML_ATTR(L"u", underline);

			if (style_text_line_through_type_)
			{
				if (style_text_line_through_type_->get_type() == line_type::Single)
					CP_XML_ATTR(L"strike", L"sngStrike");           
				else if (style_text_line_through_type_->get_type() == line_type::Double)
					CP_XML_ATTR(L"strike", L"dblStrike");           
			}
			else if (style_text_line_through_style_ && style_text_line_through_style_->get_type() != line_style::None)
			{
				CP_XML_ATTR(L"strike", L"sngStrike");           
			}
			else CP_XML_ATTR(L"strike",L"noStrike");
			
			if ((fo_letter_spacing_) && (fo_letter_spacing_->get_type() != letter_spacing::Normal))
			{
				CP_XML_ATTR(L"spc",(int)(100. * fo_letter_spacing_->get_length().get_value_unit(length::pt)));
			}
		
			if (style_text_position_)
			{
				if (style_text_position_->get_type() == text_position::Percent)
				{
					CP_XML_ATTR(L"baseline",  style_text_position_->get_position().get_value() * 1000);		
				}
				else if (style_text_position_->get_type() == text_position::Sub)
				{
					CP_XML_ATTR(L"baseline",  -30000);		
				}
				else if (style_text_position_->get_type() == text_position::Super)
				{
					CP_XML_ATTR(L"baseline",  +30000);		
				}
			}
	//nodes
			if (fo_color_)
			{
				std::wstring strRGB = fo_color_->get_hex_value(); //auto ... не нужно
				if (!strRGB.empty())
				{
					CP_XML_NODE(L"a:solidFill")
					{
						CP_XML_NODE(L"a:srgbClr") {CP_XML_ATTR(L"val", strRGB);}
					}
				}
			}
			if (style_font_name_ || style_font_name_asian_ || style_font_name_complex_ || fo_font_family_)
			{
				std::wstring w_eastAsia;
				std::wstring w_hAnsi;
				std::wstring w_cs;
				std::wstring w_ascii = w_hAnsi = w_cs = (fo_font_family_ ? *fo_font_family_ : L"");

				if (style_font_name_complex_)
				{
					font_instance * font = fonts.font_by_style_name(*style_font_name_complex_);
					if (font)
						w_cs = font->name();
				}
				if (style_font_name_asian_)
				{
					font_instance * font = fonts.font_by_style_name(*style_font_name_asian_);
					if (font)
						w_eastAsia	= font->name();
				}
				if (style_font_name_)
				{
					font_instance * font = fonts.font_by_style_name(*style_font_name_);
					if (font) 
						w_ascii = w_hAnsi = font->name();
				}
			
				if (!w_ascii.empty())	
				{
					CP_XML_NODE(L"a:latin"){CP_XML_ATTR(L"typeface", delete_apostroph_in_name(w_ascii));}
				}

				if (!w_eastAsia.empty())
				{
					CP_XML_NODE(L"a:ea"){CP_XML_ATTR(L"typeface", delete_apostroph_in_name(w_eastAsia));}
				}

				if (!w_cs.empty())
				{
					CP_XML_NODE(L"a:cs"){CP_XML_ATTR(L"typeface", delete_apostroph_in_name(w_cs));}
				}
			}
			if ((fo_background_color_) && (fo_background_color_->get_type() == odf_types::background_color::Enabled ))
			{
				std::wstring strRGB = fo_background_color_->get_color().get_hex_value(); //auto ... не нужно
				if (!strRGB.empty())
				{
					CP_XML_NODE(L"a:highlight")
					{
						CP_XML_NODE(L"a:srgbClr") {CP_XML_ATTR(L"val", strRGB);}
					}
				}

			}
			if (!hlink.empty())
			{
				CP_XML_NODE(L"a:hlinkClick")
				{
					CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
					CP_XML_ATTR(L"r:id", hlink);
				}
			}
		}
	}
}
void text_format_properties_content::xlsx_serialize(std::wostream & strm, oox::xlsx_conversion_context & Context)
{
	double font_size = process_font_size_impl(fo_font_size_, NULL);

	bool bBold = false, bItalic = false;
	if (font_size > 0)
	{
		strm << L"&amp;" << font_size;
	}
	if (fo_font_style_ && fo_font_style_->get_type() == font_style::Italic)
	{
		bItalic = true;
		//strm << L"&amp;I";
	}
	if (fo_font_weight_ && fo_font_weight_->get_type() == font_weight::WNormal)
	{
		bBold = true;
		//strm << L"&amp;B";
	}
	std::wstring font_name;
	if (fo_font_family_)
	{
		font_name = *fo_font_family_;
	}
	else if (style_font_name_)
	{
        odf_read_context & context =  Context.root()->odf_context();
        fonts_container & fonts = context.fontContainer();

		font_instance * font = fonts.font_by_style_name(*style_font_name_);
		if (font)
			font_name = font->name();
	}
	//if (font_name.empty())
	{
		font_name = L"-";
	}
	font_name += L",";
	
	if (!bBold && !bItalic)		font_name += L"Regular";
	else if (bBold && bItalic)	font_name += L"Bold Italic";
	else if (bBold)				font_name += L"Bold";
	else if (bItalic)			font_name += L"Italic";
	
	strm << L"&amp;&quot;" << font_name << L"&quot;";

	if (style_text_underline_type_)
	{
		if (style_text_underline_type_->get_type() == line_type::Single) strm << L"&amp;U";
		if (style_text_underline_type_->get_type() == line_type::Double) strm << L"&amp;E";
	}
	if (style_text_line_through_type_ && style_text_line_through_type_->get_type() != line_type::None)
	{
		strm << L"&amp;S";
	}
	if (style_text_position_)
	{
		if (style_text_position_->get_position().get_value() < 0) strm << L"&amp;Y";
		if (style_text_position_->get_position().get_value() > 0) strm << L"&amp;X";
	}
	if (fo_color_)
	{
		strm << L"&amp;K" << fo_color_->get_hex_value();
	}
}
void text_format_properties_content::docx_serialize(std::wostream & _rPr, fonts_container & fonts)
{//упрощенный вариант
	
	_rPr << L"<w:rPr>";
	const int W			= process_font_weight	(fo_font_weight_);
	const int fontStyle = process_font_style	(fo_font_style_);
	const int WCs		= process_font_weight	(style_font_weight_complex_);
	
	if (r_style_)
	{
		_rPr << L"<w:rStyle w:val=\"" << *r_style_ << L"\" />";
	}
	if (W)
	{
		if (W > 0)
			_rPr << L"<w:b/>";
		else
			_rPr << L"<w:b w:val=\"false\" />";
	}
	if (WCs)
	{
		if (WCs > 0)
			_rPr << L"<w:bCs/>";
		else
			_rPr << L"<w:bCs w:val=\"false\" />";
	}
	if (fo_text_transform_)
	{
		_rPr << (fo_text_transform_->get_type() ==  text_transform::Uppercase ? L"<w:caps w:val=\"true\" />" : L"<w:caps w:val=\"false\" />");
	}
	if (fontStyle)
	{
		if (fontStyle > 0)
			_rPr << L"<w:i />";
		else
			_rPr << L"<w:i w:val=\"false\" />";
	}
	const int fontStyleComplex = process_font_style(style_font_style_complex_);
	if (fontStyleComplex)
	{
		if (fontStyleComplex > 0)
			_rPr << L"<w:iCs />";
		else
			_rPr << L"<w:iCs w:val=\"false\" />";
	}
	if (fo_font_variant_)
	{
		_rPr << (fo_font_variant_->get_type() == font_variant::SmallCaps ?	L"<w:smallCaps w:val=\"true\" />" : 
																			L"<w:smallCaps w:val=\"false\" />" );
	}
	if (style_font_relief_)
	{
		if (style_font_relief_->get_type() == font_relief::Embossed)
			_rPr << L"<w:emboss w:val=\"true\" />";
		else if (style_font_relief_->get_type() == font_relief::Engraved)
			_rPr << L"<w:imprint w:val=\"true\" />";
		else
		{
			_rPr << L"<w:emboss w:val=\"false\" />";
			_rPr << L"<w:imprint w:val=\"false\" />";
		}
	}
	if (style_text_outline_)
	{
	   _rPr << ((*style_text_outline_ == true) ? L"<w:outline w:val=\"true\" />" : L"<w:outline w:val=\"false\" />" );
	}
	if (fo_text_shadow_)
	{
		_rPr << ((fo_text_shadow_->get_type() == shadow_type::Enable) ? L"<w:shadow w:val=\"true\" />" : L"<w:shadow w:val=\"false\" />" );        
	}
	if (text_display_)
	{
		if (text_display_->get_type() == text_display::None)
			_rPr << L"<w:vanish />";        
	}    
	// underline
	{
		line_width under = style_text_underline_width_.get_value_or(line_width::Auto);
		bool underlineBold = under.get_type() == line_width::Bold || 
			under.get_type() == line_width::Thick;
		std::wstring underline = L"";

		if ((style_text_underline_type_ && style_text_underline_type_->get_type() == line_type::None) ||
			(style_text_underline_style_ && style_text_underline_style_->get_type() == line_style::None))
		{
			// подчеркивание выключено 
			underline = L"none";
		}
		else if (style_text_underline_type_ && 
			(!style_text_underline_style_ || style_text_underline_style_ && style_text_underline_style_->get_type() == line_style::Solid) )
		{
			if (underlineBold)
				underline = L"thick"; 

			switch (style_text_underline_type_->get_type())
			{
			case line_type::Single:
				underline = L"single";
				break;
			case line_type::Double:
				underline = L"dbl";
				break;
			}
		}
		else if (style_text_underline_style_)
		{
			switch (style_text_underline_style_->get_type())
			{
			case line_style::Solid:
				if (underlineBold)
					underline = L"thick"; 
				else
					underline = L"single";
				break;
			case line_style::Dotted:
				if (underlineBold)
					underline = L"dottedHeavy"; 
				else
					underline = L"dotted";
				break;
			case line_style::Dash:
				if (underlineBold)
					underline = L"dashedHeavy"; 
				else
					underline = L"dash";
				break;
			case line_style::LongDash:if (underlineBold)
					underline = L"dashLongHeavy"; 
				else
					underline = L"dashLong";
				break;
			case line_style::DotDash:
				if (underlineBold)
					underline = L"dashDotHeavy"; 
				else
					underline = L"dotDash";
				break;
			case line_style::DotDotDash:
				if (underlineBold)
					underline = L"dashDotDotHeavy"; 
				else
					underline = L"dotDotDash";
				break;
			case line_style::Wave:
				if (underlineBold)
					underline = L"wavyHeavy"; 
				else if (style_text_underline_type_.get_value_or( line_type(line_type::Single) ).get_type() == line_type::Double)
					underline = L"wavyDbl"; 
				else
					underline = L"wavy"; 
				break;
			}
		}

		std::wstring color = L"";
		if (style_text_underline_color_ &&
			style_text_underline_color_->get_type() == underline_color::Enabled)
		{
			color = style_text_underline_color_->get_color().get_color();
		}
        
		if (!underline.empty())
		{
			_rPr << L"<w:u w:val=\"" << underline << "\" ";
			if (!color.empty())
				_rPr << L"w:color=\"" << color <<  L"\" ";
			_rPr << L"/>";
		}
	}
	if ((style_text_line_through_type_	&& style_text_line_through_type_->get_type()	== line_type::None) ||
		(style_text_line_through_style_	&& style_text_line_through_style_->get_type()	== line_style::None))
	{
		_rPr << L"<w:dstrike w:val=\"false\" />";
		_rPr << L"<w:strike w:val=\"false\" />";
	}
	else if (style_text_line_through_type_)
	{
		if (style_text_line_through_type_->get_type() == line_type::Single)
			_rPr << L"<w:strike w:val=\"true\" />";            
		else if (style_text_line_through_type_->get_type() == line_type::Double)
			_rPr << L"<w:dstrike w:val=\"true\" />";
	}
	else if (style_text_line_through_style_ && style_text_line_through_style_->get_type() != line_style::None)
	{
		_rPr << L"<w:strike w:val=\"true\" />";
	}

	bool needProcessFontSize = true;

	if (style_text_position_)
	{
		bool noNeedSize = false;
		if (style_text_position_->get_type() == text_position::Sub)
		{
			_rPr << L"<w:vertAlign w:val=\"subscript\" />";
			noNeedSize = true;
		}
		if (style_text_position_->get_type() == text_position::Super)
		{
			_rPr << L"<w:vertAlign w:val=\"superscript\" />";
			noNeedSize = true;
		}

		double fontSizeVal = (fo_font_size_) ?	process_font_size_impl(fo_font_size_, NULL) :
												process_font_size_impl(font_size(percent(100.0)), NULL);

		if (style_text_position_->get_type() == text_position::Percent)
		{
			const double mul = style_text_position_->get_position().get_value() / 100.0;
			if (fontSizeVal > 0)
			{
				const std::wstring position = std::to_wstring( (int)(fontSizeVal * mul + 0.5));
				if (!position.empty())
				{
					_rPr << L"<w:position w:val=\"" << position << "\" />";
				}
			}
			else
			{
				if (mul < -0.3) _rPr << L"<w:vertAlign w:val=\"subscript\"/>";
				else if (mul > 0.3)  _rPr << L"<w:vertAlign w:val=\"superscript\"/>";
			}
		}

		if (style_text_position_->has_font_size() && !noNeedSize)
		{
			const double mul = style_text_position_->font_size().get_value() / 100.0;
			if (fontSizeVal > 0 && mul > 0)
			{
				const std::wstring fontSize = std::to_wstring((int)(fontSizeVal * mul + 0.5));
				if (!fontSize.empty())
				{
					needProcessFontSize = false;
					_rPr << L"<w:sz w:val=\"" << fontSize << "\" />";
				}
			}
		}
	}

	if (style_text_rotation_angle_)
	{
		unsigned int value = (std::abs)(*style_text_rotation_angle_);
		if (90 == value || 270 == value)
			_rPr << L"<w:eastAsianLayout w:vert=\"true\" />";
	}

	if (style_text_scale_)
	{
		_rPr << L"<w:w w:val=\"" << style_text_scale_->get_value() << "\" />";
	}

	if (needProcessFontSize)
	{
		 int fontSize = process_font_size(fo_font_size_, NULL);
       
		 if (fontSize > 0)
		{
			_rPr << L"<w:sz w:val=\"" << fontSize << "\" />";
		}
	}

	int fontSizeComplex = process_font_size(style_font_size_complex_, NULL, true);

	if (fontSizeComplex > 0)
		_rPr << L"<w:szCs w:val=\"" << fontSizeComplex << "\" />";

	if (fo_letter_spacing_)
	{
		if (fo_letter_spacing_->get_type() == letter_spacing::Normal)
			_rPr << L"<w:spacing w:val=\"0\" />";
		else
			_rPr << L"<w:spacing w:val=\"" << 
			(int)(20.0 * fo_letter_spacing_->get_length().get_value_unit(length::pt))
			<< "\" />";
	}

	if (style_text_emphasize_)
	{
		std::wstring em = L"";
		switch(style_text_emphasize_->get_type())
		{
		case text_emphasize::None:
			em = L"none";
			break;
		case text_emphasize::Accent:
			em = L"comma";
			break;
		case text_emphasize::Dot:
			if (style_text_emphasize_->get_type_2() == text_emphasize::Above)
				em = L"dot";
			else
				em = L"underDot";
			break;
		case text_emphasize::Circle:
			em = L"circle";
			break;
		case text_emphasize::Disc:
			em = L"dot"; // ?
			break;
		}

		_rPr << L"<w:em w:val=\"" << em << "\" />";        
	}

	if (style_font_name_ || style_font_name_asian_ || style_font_name_complex_ || fo_font_family_)
	{
		std::wstring w_eastAsia;
		std::wstring w_hAnsi;
		std::wstring w_cs;
		std::wstring w_ascii = w_hAnsi = w_cs = (fo_font_family_ ? *fo_font_family_ : L"");
				
		if (style_font_name_complex_)
		{
			font_instance * font = fonts.font_by_style_name(*style_font_name_complex_);
			if (font)
				w_cs = font->name();
		}
		if (style_font_name_asian_)
		{
			font_instance * font = fonts.font_by_style_name(*style_font_name_asian_);
			if (font)
				w_eastAsia	= font->name();
		}
		if (style_font_name_)
		{
			font_instance * font = fonts.font_by_style_name(*style_font_name_);
			if (font)
				w_ascii = w_hAnsi	= font->name();
		}

		_rPr << L"<w:rFonts";
		if (!w_ascii.empty())	{XmlUtils::replace_all(w_ascii, L"\"", L"");	_rPr << L" w:ascii=\""		<< w_ascii		<<"\"";}
		if (!w_hAnsi.empty())	{XmlUtils::replace_all(w_hAnsi, L"\"", L"");	_rPr << L" w:hAnsi=\""		<< w_hAnsi		<<"\"";}
		if (!w_eastAsia.empty()){XmlUtils::replace_all(w_eastAsia, L"\"", L"");	_rPr << L" w:eastAsia=\""	<< w_eastAsia	<<"\"";}
		if (!w_cs.empty())		{XmlUtils::replace_all(w_cs, L"\"", L"");		_rPr << L" w:cs=\""			<< w_cs			<<"\"";}
		_rPr << L"/>";
	}

	_CP_OPT(color) color_text = fo_color_;

	if (color_text)
	{
		_rPr << L"<w:color w:val=\"" << color_text->get_hex_value() << "\" />";
	}
	else if (style_use_window_font_color_ && *style_use_window_font_color_)
	{
		_rPr << L"<w:color w:val=\"auto\" />";
	}

	if (fo_background_color_)
	{
		std::wstring w_fill;
		if (fo_background_color_->get_type() == background_color::Transparent)
			w_fill = L"auto";        
		else
			w_fill = fo_background_color_->get_color().get_hex_value();

		_rPr << L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" << w_fill << "\" />";
	}

	if (fo_language_ || style_language_asian_ || style_language_complex_)
	{

		std::wstring w_val;
		if (fo_language_)
		{
			w_val = *fo_language_;
			if (fo_country_)
				w_val += L"-" + *fo_country_;
		}

		std::wstring w_eastAsia;
		if (style_language_asian_)
		{
			w_eastAsia = *style_language_asian_;
			if (style_country_asian_)
				w_eastAsia += L"-" + *style_country_asian_;
		}

		std::wstring w_bidi;
		if (style_language_complex_)
		{
			w_bidi = *style_language_complex_;
			if (style_country_complex_)
				w_bidi += L"-" + *style_country_complex_;
		}

		_rPr << L"<w:lang ";
		if (!w_val.empty())
			_rPr << L"w:val=\"" << w_val << "\" ";
        
		if (!w_eastAsia.empty())
			_rPr << L"w:eastAsia=\"" << w_eastAsia << "\" ";

		if (!w_bidi.empty())
			_rPr << L"w:bidi=\"" << w_bidi << "\" ";
		_rPr << L"/>";
	}
	if (style_text_blinking_)
	{
		std::wstring w_val = L"none";
		if (*style_text_blinking_)
			w_val = L"blinkBackground";

		_rPr << L"<w:effect w:val=\"" << w_val << "\" />";
	}
	_rPr << L"</w:rPr>";	
}

void text_format_properties_content::pptx_convert(oox::pptx_conversion_context & Context)
{
	oox::styles_context	& styles_context_	= Context.get_text_context().get_styles_context();
	fonts_container & fonts_			= Context.root()->odf_context().fontContainer();	  

	drawing_serialize(styles_context_.text_style(), styles_context_.extern_node(), fonts_, styles_context_.get_current_processed_style(), styles_context_.hlinkClick());
}

void text_format_properties_content::docx_convert(oox::docx_conversion_context & Context)
{//расширенный вариант
    std::wostream & _pPr = Context.get_styles_context().paragraph_nodes();

    // to paragraph properties
    if (fo_hyphenate_)
    {
        if (*fo_hyphenate_)
            _pPr << L"<w:suppressAutoHyphens w:val=\"false\" />";
        else
            _pPr << L"<w:suppressAutoHyphens w:val=\"true\" />";
    }
   
	std::wostream & _rPr	= Context.get_styles_context().text_style();
	fonts_container & fonts = Context.root()->odf_context().fontContainer();	        
    
	if (Context.get_rtl())
    {
        _rPr << L"<w:rtl/>";/* w:val=\"true\" */
    }
    if (r_style_)
    {
        _rPr << L"<w:rStyle w:val=\"" << *r_style_ << L"\" />";
    }
    {
        const int W = process_font_weight(fo_font_weight_);
        if (W)
        {
            if (W > 0)
                _rPr << L"<w:b/>";
            else
                _rPr << L"<w:b w:val=\"false\" />";
        }
    }

    {
        const int WCs = process_font_weight(style_font_weight_complex_);
        if (WCs)
        {
            if (WCs > 0)
                _rPr << L"<w:bCs/>";
            else
                _rPr << L"<w:bCs w:val=\"false\" />";
        }
    }
    if (fo_text_transform_)
    {
        _rPr << (fo_text_transform_->get_type() ==  text_transform::Uppercase ? L"<w:caps w:val=\"true\" />" : L"<w:caps w:val=\"false\" />");
    }
    {
        const int fontStyle = process_font_style(fo_font_style_);
        if (fontStyle)
        {
            if (fontStyle > 0)
                _rPr << L"<w:i />";
            else
                _rPr << L"<w:i w:val=\"false\" />";
        }
    }
    
    {
        const int fontStyleComplex = process_font_style(style_font_style_complex_);
        if (fontStyleComplex)
        {
            if (fontStyleComplex > 0)
                _rPr << L"<w:iCs />";
            else
                _rPr << L"<w:iCs w:val=\"false\" />";
        }
    }
    if (fo_font_variant_)
    {
        _rPr << (fo_font_variant_->get_type() == font_variant::SmallCaps ?	L"<w:smallCaps w:val=\"true\" />" : 
																			L"<w:smallCaps w:val=\"false\" />" );
    }
    if (style_font_relief_)
    {
        if (style_font_relief_->get_type() == font_relief::Embossed)
            _rPr << L"<w:emboss w:val=\"true\" />";
        else if (style_font_relief_->get_type() == font_relief::Engraved)
            _rPr << L"<w:imprint w:val=\"true\" />";
        else
        {
            _rPr << L"<w:emboss w:val=\"false\" />";
            _rPr << L"<w:imprint w:val=\"false\" />";
        }
    }
    if (style_text_outline_)
    {
       _rPr << ((*style_text_outline_ == true) ? L"<w:outline w:val=\"true\" />" : L"<w:outline w:val=\"false\" />" );
    }
    if (fo_text_shadow_)
    {
        _rPr << ((fo_text_shadow_->get_type() == shadow_type::Enable) ? L"<w:shadow w:val=\"true\" />" : L"<w:shadow w:val=\"false\" />" );        
    }

    if (text_display_)
    {
        if (text_display_->get_type() == text_display::None)
            _rPr << L"<w:vanish/>";        
    }    
    // underline
    {
        line_width under = style_text_underline_width_.get_value_or(line_width::Auto);
        bool underlineBold = under.get_type() == line_width::Bold || 
            under.get_type() == line_width::Thick;
        std::wstring underline = L"";

        if (( style_text_underline_type_ && style_text_underline_type_->get_type() == line_type::None) ||
            (style_text_underline_style_ && style_text_underline_style_->get_type() == line_style::None))      
        {
            // подчеркивание выключено 
            underline = L"none";
        }
        else if (style_text_underline_type_ && 
            (!style_text_underline_style_ || style_text_underline_style_ && style_text_underline_style_->get_type() == line_style::Solid) )
        {
            if (underlineBold)
                underline = L"thick"; 

            switch (style_text_underline_type_->get_type())
            {
            case line_type::Single:
                underline = L"single";
                break;
            case line_type::Double:
                underline = L"dbl";
                break;
            }
        }
        else if (style_text_underline_style_)
        {
            switch (style_text_underline_style_->get_type())
            {
            case line_style::Solid:
                if (underlineBold)
                    underline = L"thick"; 
                else
                    underline = L"single";
                break;
            case line_style::Dotted:
                if (underlineBold)
                    underline = L"dottedHeavy"; 
                else
                    underline = L"dotted";
                break;
            case line_style::Dash:
                if (underlineBold)
                    underline = L"dashedHeavy"; 
                else
                    underline = L"dash";
                break;
            case line_style::LongDash:if (underlineBold)
                    underline = L"dashLongHeavy"; 
                else
                    underline = L"dashLong";
                break;
            case line_style::DotDash:
                if (underlineBold)
                    underline = L"dashDotHeavy"; 
                else
                    underline = L"dotDash";
                break;
            case line_style::DotDotDash:
                if (underlineBold)
                    underline = L"dashDotDotHeavy"; 
                else
                    underline = L"dotDotDash";
                break;
            case line_style::Wave:
                if (underlineBold)
                    underline = L"wavyHeavy"; 
                else if (style_text_underline_type_.get_value_or( line_type(line_type::Single) ).get_type() == line_type::Double)
                    underline = L"wavyDbl"; 
                else
                    underline = L"wavy"; 
                break;
            }
        }

        std::wstring color = L"";
        if (style_text_underline_color_ &&
            style_text_underline_color_->get_type() == underline_color::Enabled)
        {
            color = style_text_underline_color_->get_color().get_color();
        }
        
		if (!underline.empty())
        {
            _rPr << L"<w:u w:val=\"" << underline << "\" ";
            if (!color.empty())
                _rPr << L"w:color=\"" << color <<  L"\" ";
            _rPr << L"/>";
        }
    }
    {
        if ((style_text_line_through_type_ && style_text_line_through_type_->get_type() == line_type::None) ||
            (style_text_line_through_style_ && style_text_line_through_style_->get_type() == line_style::None))
        {
            _rPr << L"<w:dstrike w:val=\"false\" />";
            _rPr << L"<w:strike w:val=\"false\" />";
        }
        else if (style_text_line_through_type_)
        {
            if (style_text_line_through_type_->get_type() == line_type::Single)
                _rPr << L"<w:strike w:val=\"true\" />";            
            else if (style_text_line_through_type_->get_type() == line_type::Double)
                _rPr << L"<w:dstrike w:val=\"true\"/>";
        }
        else if (style_text_line_through_style_ && style_text_line_through_style_->get_type() != line_style::None)
        {
            _rPr << L"<w:strike w:val=\"true\"/>";
        }
    }

    bool needProcessFontSize = true;

    // 17.3.2.42
	if (Context.get_drop_cap_context().state() == 2)
	{
		_rPr << L"<w:position w:val=\"-" << (2+Context.get_drop_cap_context().Scale*2-3)*2 << "\"/> ";//формула ачуметь !! - подбор вручную
	}
	else if (style_text_position_)
    {
        bool noNeedSize = false;
        if (style_text_position_->get_type() == text_position::Sub)
        {
            _rPr << L"<w:vertAlign w:val=\"subscript\"/>";
            noNeedSize = true;
        }
        if (style_text_position_->get_type() == text_position::Super)
        {
            _rPr << L"<w:vertAlign w:val=\"superscript\"/>";
            noNeedSize = true;
        }

        double fontSizeVal = (fo_font_size_) ? process_font_size_impl(fo_font_size_, Context.get_styles_context().get_current_processed_style()) :
            process_font_size_impl(font_size(percent(100.0)), Context.get_styles_context().get_current_processed_style());

		if (fontSizeVal <= 0)
			fontSizeVal = Context.get_current_fontSize();
      
		if (style_text_position_->get_type() == text_position::Percent)
        {
            const double mul = style_text_position_->get_position().get_value() / 100.0;
            if (fontSizeVal > 0)
            {
                const std::wstring position = std::to_wstring( (int)(fontSizeVal * mul + 0.5));
                if (!position.empty())
                {
                    _rPr << L"<w:position w:val=\"" << position << "\" />";
                }
            }
			else
			{
				if (mul < - 0.3) _rPr << L"<w:vertAlign w:val=\"subscript\"/>";
				else if (mul > 0.3)  _rPr << L"<w:vertAlign w:val=\"superscript\"/>";
			}
        }

        if (style_text_position_->has_font_size() && !noNeedSize)
        {
            const double mul = style_text_position_->font_size().get_value() / 100.0;
            if (fontSizeVal > 0 && mul > 0)
            {
                const std::wstring fontSize = std::to_wstring((int)(fontSizeVal * mul + 0.5));
                if (!fontSize.empty())
                {
                    needProcessFontSize = false;
                    _rPr << L"<w:sz w:val=\"" << fontSize << "\"/>";
                }
            }
        }
    }

    if (style_text_rotation_angle_)
    {
        unsigned int value = (std::abs)(*style_text_rotation_angle_);
        if (90 == value || 270 == value)
            _rPr << L"<w:eastAsianLayout w:vert=\"true\" />";
    }

    // 17.3.2.43
    if (style_text_scale_)
    {
        _rPr << L"<w:w w:val=\"" << style_text_scale_->get_value() << "\" />";
    }

    // 17.3.2.38
    if (needProcessFontSize)
    {
         int fontSize=0;
		 if (Context.get_drop_cap_context().state()==2)
		 {
			 fontSize = process_font_size(fo_font_size_, Context.get_styles_context().get_current_processed_style(),false,
				 Context.get_drop_cap_context().Scale + (Context.get_drop_cap_context().Scale-1) * 0.7);//вместо 1 ДОЛЖНОБЫТЬ коэфф. межстрочного интервала!!!

			 if (fontSize < 1)
				 fontSize = (int)(Context.get_drop_cap_context().FontSize / 7.52);
		 }
		 else
		 {
			 fontSize = process_font_size(fo_font_size_, Context.get_styles_context().get_current_processed_style());
		 }
       
		if (fontSize >  0)
		{
            _rPr << L"<w:sz w:val=\"" << fontSize << "\" />";
		}
    }

    int fontSizeComplex=0;
	if (Context.get_drop_cap_context().state()==2)
		fontSizeComplex = process_font_size(style_font_size_complex_, Context.get_styles_context().get_current_processed_style(), true,
			Context.get_drop_cap_context().Scale + (Context.get_drop_cap_context().Scale-1) * 0.7);
	else
		fontSizeComplex = process_font_size(style_font_size_complex_, Context.get_styles_context().get_current_processed_style(), true);

    if (fontSizeComplex>0)
        _rPr << L"<w:szCs w:val=\"" << fontSizeComplex << "\" />";

    // TODO: font size asian ??

    if (fo_letter_spacing_)
    {
        if (fo_letter_spacing_->get_type() == letter_spacing::Normal)
            _rPr << L"<w:spacing w:val=\"0\" />";
        else
            _rPr << L"<w:spacing w:val=\"" << 
            (int)(20.0 * fo_letter_spacing_->get_length().get_value_unit(length::pt))
            << "\" />";
    }

    if (style_text_emphasize_)
    {
        std::wstring em = L"";
        switch(style_text_emphasize_->get_type())
        {
        case text_emphasize::None:
            em = L"none";
            break;
        case text_emphasize::Accent:
            em = L"comma";
            break;
        case text_emphasize::Dot:
            if (style_text_emphasize_->get_type_2() == text_emphasize::Above)
                em = L"dot";
            else
                em = L"underDot";
            break;
        case text_emphasize::Circle:
            em = L"circle";
            break;
        case text_emphasize::Disc:
            em = L"dot"; // ?
            break;
        }

        _rPr << L"<w:em w:val=\"" << em << "\" />";        
    }

    if (style_font_name_ || style_font_name_asian_ || style_font_name_complex_ || fo_font_family_)
    {
		std::wstring w_eastAsia;
        std::wstring w_hAnsi;
        std::wstring w_cs;
        std::wstring w_ascii = w_hAnsi = w_cs = (fo_font_family_ ? *fo_font_family_ : L"");

		if (style_font_name_complex_)
		{
			font_instance * font = fonts.font_by_style_name(*style_font_name_complex_);
			if (font)
				w_cs = font->name();
		}
		if (style_font_name_asian_)
		{
			font_instance * font = fonts.font_by_style_name(*style_font_name_asian_);
			if (font)
				w_eastAsia	= font->name();
		}
		if (style_font_name_)
		{
			font_instance * font = fonts.font_by_style_name(*style_font_name_);
			if (font)
			{
				w_ascii = w_hAnsi = font->name();

				if (font->charset() == L"02")
				{
					if (w_cs.empty()) w_cs = font->name();
					if (w_eastAsia.empty()) w_eastAsia = font->name();
				}
			}
		}

		_rPr << L"<w:rFonts";
		if (!w_ascii.empty())	{XmlUtils::replace_all(w_ascii, L"\"", L"");	_rPr << L" w:ascii=\""		<< w_ascii		<<"\"";}
		if (!w_hAnsi.empty())	{XmlUtils::replace_all(w_hAnsi, L"\"", L"");	_rPr << L" w:hAnsi=\""		<< w_hAnsi		<<"\"";}
		if (!w_eastAsia.empty()){XmlUtils::replace_all(w_eastAsia, L"\"", L"");	_rPr << L" w:eastAsia=\""	<< w_eastAsia	<<"\"";}
		if (!w_cs.empty())		{XmlUtils::replace_all(w_cs, L"\"", L"");		_rPr << L" w:cs=\""			<< w_cs			<<"\"";}
        _rPr << L"/>";
    }

	_CP_OPT(color) color_text = fo_color_;

	if (Context.get_drawing_context().get_current_shape())
	{
		odf_reader::draw_shape *shape = Context.get_drawing_context().get_current_shape();

		if (shape->word_art_)
		{//взять из графических ствойст цвет текста .. 

			const std::wstring styleName = shape->common_draw_attlists_.shape_with_text_and_styles_.
				common_shape_draw_attlist_.draw_style_name_.get_value_or(L"");

			std::vector<const odf_reader::style_instance *> instances;
			odf_reader::style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(styleName, odf_types::style_family::Graphic,Context.process_headers_footers_);
			if (styleInst)
			{
				style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
				if (defaultStyle)instances.push_back(defaultStyle);
				instances.push_back(styleInst);
			}
			graphic_format_properties graphicProperties = calc_graphic_properties_content(instances);

			draw_fill fill = draw_fill::solid;
			if (graphicProperties.common_draw_fill_attlist_.draw_fill_)
				fill = *graphicProperties.common_draw_fill_attlist_.draw_fill_;

			if (graphicProperties.common_draw_fill_attlist_.draw_fill_color_ &&
				( fill.get_type() != draw_fill::bitmap &&  fill.get_type() != draw_fill::none ))
			{
				color_text = graphicProperties.common_draw_fill_attlist_.draw_fill_color_;
			}
		}
	}
    if (color_text)
    {
        _rPr << L"<w:color w:val=\"" << color_text->get_hex_value() << "\" />";
    }
    else if (style_use_window_font_color_ && *style_use_window_font_color_)
    {
        _rPr << L"<w:color w:val=\"auto\" />";
    }

    if (fo_background_color_)
    {
        std::wstring w_fill;
        if (fo_background_color_->get_type() == background_color::Transparent)
            w_fill = L"auto";        
        else
            w_fill = fo_background_color_->get_color().get_hex_value();

        _rPr << L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" << w_fill << "\" />";
    }

    if (fo_language_ || style_language_asian_ || style_language_complex_)
    {

        std::wstring w_val;
        if (fo_language_)
        {
            w_val = *fo_language_;
            if (fo_country_)
                w_val += L"-" + *fo_country_;
        }

        std::wstring w_eastAsia;
        if (style_language_asian_)
        {
            w_eastAsia = *style_language_asian_;
            if (style_country_asian_)
                w_eastAsia += L"-" + *style_country_asian_;
        }

        std::wstring w_bidi;
        if (style_language_complex_)
        {
            w_bidi = *style_language_complex_;
            if (style_country_complex_)
                w_bidi += L"-" + *style_country_complex_;
        }

        _rPr << L"<w:lang ";
        if (!w_val.empty())
            _rPr << L"w:val=\"" << w_val << "\" ";
        
        if (!w_eastAsia.empty())
            _rPr << L"w:eastAsia=\"" << w_eastAsia << "\" ";

        if (!w_bidi.empty())
            _rPr << L"w:bidi=\"" << w_bidi << "\" ";
        _rPr << L"/>";
    }

    if (style_text_blinking_)
    {
        std::wstring w_val = L"none";
        if (*style_text_blinking_)
            w_val = L"blinkBackground";

        _rPr << L"<w:effect w:val=\"" << w_val << "\" />";
    }
}


void text_format_properties_content::oox_serialize(std::wostream & strm, bool graphic, fonts_container & fonts, bool default_)
{
	if (graphic)
	{	
		if (default_)
			drawing_serialize(strm, L"a:defRPr", fonts);
		else
			drawing_serialize(strm, L"a:rPr", fonts);
	}
	else
	{
		docx_serialize(strm, fonts);
	}
}


void apply_font_size(optional<font_size>::Type & A, const optional<font_size>::Type & B)
{
    if (B && 
        B->get_type() == font_size::Percent &&
        A && A->get_type() == font_size::Length)
    {
        A = font_size( length( A->get_length().get_value() * B->get_percent().get_value(),  A->get_length().get_unit()) );
    }
    else
        _CP_APPLY_PROP(A, B);
}



void text_format_properties_content::apply_from(const text_format_properties_content & Other)
{
    _CP_APPLY_PROP(r_style_, Other.r_style_);

    _CP_APPLY_PROP(fo_font_variant_, Other.fo_font_variant_);
    _CP_APPLY_PROP(fo_text_transform_, Other.fo_text_transform_);
    _CP_APPLY_PROP(fo_color_, Other.fo_color_);
    _CP_APPLY_PROP(style_use_window_font_color_, Other.style_use_window_font_color_);
    _CP_APPLY_PROP(style_text_outline_, Other.style_text_outline_);
    _CP_APPLY_PROP(style_text_line_through_type_, Other.style_text_line_through_type_); 
    _CP_APPLY_PROP(style_text_line_through_style_, Other.style_text_line_through_style_);
    
    apply_line_width(style_text_line_through_width_, Other.style_text_line_through_width_);
    
    _CP_APPLY_PROP(style_text_line_through_color_, Other.style_text_line_through_color_);
    _CP_APPLY_PROP(style_text_line_through_text_, Other.style_text_line_through_text_);
    _CP_APPLY_PROP(style_text_line_through_text_style_, Other.style_text_line_through_text_style_);
    _CP_APPLY_PROP(style_text_position_, Other.style_text_position_);
    _CP_APPLY_PROP(style_font_name_, Other.style_font_name_);
    _CP_APPLY_PROP(style_font_name_asian_, Other.style_font_name_asian_);
    _CP_APPLY_PROP(style_font_name_complex_, Other.style_font_name_complex_);
    _CP_APPLY_PROP(fo_font_family_, Other.fo_font_family_);
    _CP_APPLY_PROP(style_font_family_asian_, Other.style_font_family_asian_);
    _CP_APPLY_PROP(style_font_family_complex_, Other.style_font_family_complex_);

    _CP_APPLY_PROP(style_font_family_generic_, Other.style_font_family_generic_);
    _CP_APPLY_PROP(style_font_family_generic_asian_, Other.style_font_family_generic_asian_);
    _CP_APPLY_PROP(style_font_family_generic_complex_, Other.style_font_family_generic_complex_);

    _CP_APPLY_PROP(style_font_style_name_, Other.style_font_style_name_);
    _CP_APPLY_PROP(style_font_style_name_asian_, Other.style_font_style_name_asian_);
    _CP_APPLY_PROP(style_font_style_name_complex_, Other.style_font_style_name_complex_);

    _CP_APPLY_PROP(style_font_pitch_,	Other.style_font_pitch_);
    _CP_APPLY_PROP(style_font_pitch_asian_,		Other.style_font_pitch_asian_);
    _CP_APPLY_PROP(style_font_pitch_complex_,	Other.style_font_pitch_complex_);

    _CP_APPLY_PROP(style_font_charset_,			Other.style_font_charset_);
    _CP_APPLY_PROP(style_font_charset_asian_,	Other.style_font_charset_asian_);
    _CP_APPLY_PROP(style_font_charset_complex_, Other.style_font_charset_complex_);
    
    apply_font_size(fo_font_size_, Other.fo_font_size_);
    apply_font_size(style_font_size_asian_, Other.style_font_size_asian_);
    apply_font_size(style_font_size_complex_, Other.style_font_size_complex_);

    // TODO check
    _CP_APPLY_PROP(style_font_size_rel_, Other.style_font_size_rel_);
    _CP_APPLY_PROP(style_font_size_rel_asian_, Other.style_font_size_rel_asian_);
    _CP_APPLY_PROP(style_font_size_rel_complex_, Other.style_font_size_rel_complex_);

    _CP_APPLY_PROP(style_script_type_, Other.style_script_type_);
    _CP_APPLY_PROP(fo_letter_spacing_, Other.fo_letter_spacing_);
    _CP_APPLY_PROP(fo_language_, Other.fo_language_);
    _CP_APPLY_PROP(style_language_asian_, Other.style_language_asian_);
    _CP_APPLY_PROP(style_language_complex_, Other.style_language_complex_);

    _CP_APPLY_PROP(fo_country_, Other.fo_country_);
    _CP_APPLY_PROP(style_country_asian_, Other.style_country_asian_);
    _CP_APPLY_PROP(style_country_complex_, Other.style_country_complex_);
           
    _CP_APPLY_PROP(fo_font_style_, Other.fo_font_style_);
    _CP_APPLY_PROP(style_font_style_asian_, Other.style_font_style_asian_);
    _CP_APPLY_PROP(style_font_style_complex_, Other.style_font_style_complex_);

    _CP_APPLY_PROP(style_font_relief_, Other.style_font_relief_);
    _CP_APPLY_PROP(fo_text_shadow_, Other.fo_text_shadow_);
    _CP_APPLY_PROP(style_text_underline_type_, Other.style_text_underline_type_);
    _CP_APPLY_PROP(style_text_underline_style_, Other.style_text_underline_style_);
    
    apply_line_width(style_text_underline_width_, Other.style_text_underline_width_);

    _CP_APPLY_PROP(style_text_underline_color_, Other.style_text_underline_color_);
    
    _CP_APPLY_PROP(fo_font_weight_, Other.fo_font_weight_);
    _CP_APPLY_PROP(style_font_weight_asian_, Other.style_font_weight_asian_);
    _CP_APPLY_PROP(style_font_weight_complex_, Other.style_font_weight_complex_);
    _CP_APPLY_PROP(style_text_underline_mode_, Other.style_text_underline_mode_);
    _CP_APPLY_PROP(style_text_line_through_mode_, Other.style_text_line_through_mode_);
    _CP_APPLY_PROP(style_letter_kerning_, Other.style_letter_kerning_);
    _CP_APPLY_PROP(style_text_blinking_, Other.style_text_blinking_);
    _CP_APPLY_PROP(fo_background_color_, Other.fo_background_color_);
    _CP_APPLY_PROP(style_text_combine_, Other.style_text_combine_);
    _CP_APPLY_PROP(style_text_combine_start_char_, Other.style_text_combine_start_char_);
    _CP_APPLY_PROP(style_text_combine_end_char_, Other.style_text_combine_end_char_);
    _CP_APPLY_PROP(style_text_emphasize_, Other.style_text_emphasize_);
    _CP_APPLY_PROP(style_text_scale_, Other.style_text_scale_);
    _CP_APPLY_PROP(style_text_rotation_angle_, Other.style_text_rotation_angle_);
    _CP_APPLY_PROP(style_text_rotation_scale_, Other.style_text_rotation_scale_);
    _CP_APPLY_PROP(fo_hyphenate_, Other.fo_hyphenate_);
    _CP_APPLY_PROP(fo_hyphenation_remain_char_count_, Other.fo_hyphenation_remain_char_count_);
    _CP_APPLY_PROP(fo_hyphenation_push_char_count_, Other.fo_hyphenation_push_char_count_);
    _CP_APPLY_PROP(text_display_, Other.text_display_);
    _CP_APPLY_PROP(text_condition_, Other.text_condition_);
    _CP_APPLY_PROP(style_text_overline_color_, Other.style_text_overline_color_);
    _CP_APPLY_PROP(style_text_overline_mode_, Other.style_text_overline_mode_);
    _CP_APPLY_PROP(style_text_overline_style_, Other.style_text_overline_style_);

}
void text_format_properties_content::set_r_style(const std::wstring & rStyle)
{ 
	r_style_ = rStyle; 
}

void text_format_properties_content::apply_to(std::vector<_property> & properties)
{
	if (fo_font_weight_)	properties.push_back(_property(L"font-weight",	fo_font_weight_.get().get_type()) );
	if (fo_font_family_)	properties.push_back(_property(L"font-family",	fo_font_family_.get()) );
	if (style_font_name_)	properties.push_back(_property(L"font-name",	style_font_name_.get()) );
	if (fo_font_size_)		properties.push_back(_property(L"font-size",	fo_font_size_.get().get_length().get_value_unit(length::pt)) );
	if (fo_font_style_)		properties.push_back(_property(L"font-style",	fo_font_style_.get().get_type()) );
	if (fo_color_)			properties.push_back(_property(L"font-color",	fo_color_.get().get_hex_value()) );

}

// style:text-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_text_properties::ns = L"style";
const wchar_t * style_text_properties::name = L"text-properties";

void style_text_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	content_.add_attributes(Attributes);
}

void style_text_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void style_text_properties::docx_convert(oox::docx_conversion_context & Context)
{
    content_.docx_convert(Context);
}

void style_text_properties::pptx_convert(oox::pptx_conversion_context & Context)
{
    content_.pptx_convert(Context);
}

}
}

