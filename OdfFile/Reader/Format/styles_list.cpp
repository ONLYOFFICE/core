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

#include "styles_list.h"

#include <xml/simple_xml_writer.h>

#include "serialize_elements.h"
#include "style_text_properties.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {
    


//  text_list_style_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_style_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:name", style_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"style:display-name", style_display_name_);
    CP_APPLY_ATTR(L"text:consecutive-numbering", text_consecutive_numbering_);
}

// text:list-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_style::ns = L"text";
const wchar_t * text_list_style::name = L"list-style";

void text_list_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attr_.add_attributes(Attributes);
}

void text_list_style::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"text" == Ns && L"list-level-style-number" == Name)
	{
		CP_CREATE_ELEMENT(content_);
	}
	else if (L"text" == Ns && L"list-level-style-bullet" == Name)
	{
		CP_CREATE_ELEMENT(content_);
	}
	else if (L"text" == Ns && L"list-level-style-image" == Name)
	{
		CP_CREATE_ELEMENT(content_);
	}
   else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

//  text_list_level_style_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_level_style_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:level", text_level_, (unsigned int)0);
}

//  text_list_level_style_number_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_level_style_number_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:style-name", text_style_name_);  
    common_num_format_attlist_.add_attributes(Attributes);
    common_num_format_prefix_suffix_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"text:display-levels", text_display_levels_, (unsigned int)1);    
    CP_APPLY_ATTR(L"text:start-value", text_start_value_, (unsigned int)1);    
}

//  text_list_level_style_bullet_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_level_style_bullet_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:style-name", text_style_name_);
    CP_APPLY_ATTR(L"text:bullet-char", text_bullet_char_);
    common_num_format_prefix_suffix_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"text:bullet-relative-size", text_bullet_relative_size_);
}

//  text_list_level_style_image_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_level_style_image_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    xlink_attlist_.add_attributes(Attributes);
}
 

// text:list-level-style-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_number::ns = L"text";
const wchar_t * text_list_level_style_number::name = L"list-level-style-number";

void text_list_level_style_number::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_list_level_style_attr_.add_attributes(Attributes);
    text_list_level_style_number_attr_.add_attributes(Attributes);
}

void text_list_level_style_number::add_child_element(xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(list_level_properties_);    
	else if (L"style" == Ns && L"text-properties" == Name)
	{
		CP_CREATE_ELEMENT(style_text_properties_);
	}
	else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

//text:list-level-style-image
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_image::ns = L"text";
const wchar_t * text_list_level_style_image::name = L"list-level-style-image";

void text_list_level_style_image::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_list_level_style_attr_.add_attributes(Attributes);
    text_list_level_style_image_attr_.add_attributes(Attributes);
}

void text_list_level_style_image::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(list_level_properties_);    
	else if (L"style" == Ns && L"text-properties" == Name)
	{
        CP_CREATE_ELEMENT(style_text_properties_);
	}
	else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

// style:list-level-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_properties::ns = L"style";
const wchar_t * style_list_level_properties::name = L"list-level-properties";

void style_list_level_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_text_align_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"text:space-before", text_space_before_);
    CP_APPLY_ATTR(L"text:min-label-width", text_min_label_width_);
    CP_APPLY_ATTR(L"text:min-label-distance", text_min_label_distance_);
    CP_APPLY_ATTR(L"style:font-name", style_font_name_);
    CP_APPLY_ATTR(L"fo:width", fo_width_);
    CP_APPLY_ATTR(L"fo:height", fo_height_);

    common_vertical_rel_attlist_.add_attributes(Attributes);
    common_vertical_pos_attlist_.add_attributes(Attributes);
}

void style_list_level_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"style", L"list-level-label-alignment")
    {
        CP_CREATE_ELEMENT(style_list_level_label_alignment_);
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

// style:list-level-label-alignment
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_label_alignment::ns = L"style";
const wchar_t * style_list_level_label_alignment::name = L"list-level-label-alignment";

void style_list_level_label_alignment::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:label-followed-by", text_label_followed_by_);
    CP_APPLY_ATTR(L"text:list-tab-stop-position", text_list_tab_stop_position_);
    CP_APPLY_ATTR(L"fo:text-indent", fo_text_indent_);
    CP_APPLY_ATTR(L"fo:margin-left", fo_margin_left_);

	if ((text_label_followed_by_) && (text_label_followed_by_.get() == L"listtab")) text_label_followed_by_ = L"tab";
}

void style_list_level_label_alignment::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
}

// text:list-level-style-bullet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_bullet::ns = L"text";
const wchar_t * text_list_level_style_bullet::name = L"list-level-style-bullet";

void text_list_level_style_bullet::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_list_level_style_attr_.add_attributes(Attributes);
    text_list_level_style_bullet_attr_.add_attributes(Attributes);
}

void text_list_level_style_bullet::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"style" == Ns && L"list-level-properties" == Name)
	{
		CP_CREATE_ELEMENT(list_level_properties_);
	}
	else if (L"style" == Ns && L"properties" == Name)
	{
		create_element_and_read(Reader, L"style", L"list-level-properties", list_level_properties_, getContext());
	}
	else if (L"style" == Ns && L"text-properties" == Name)
	{
		CP_CREATE_ELEMENT(style_text_properties_);
	}
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {

std::wstring GetLevelText(unsigned int displayLevels,
                          unsigned int textLevel,
                          oox::docx_conversion_context & Context)
{
    std::wstring res;
    if (displayLevels > 1)
        res = GetLevelText(displayLevels - 1, textLevel - 1, Context);

    if (displayLevels > 1)
        res += L".";
    res += L"%";
    res += std::to_wstring(textLevel);
    return res;
}

void docx_serialize_label_alignment_props(std::wostream & strm, style_list_level_label_alignment * labelAlignment, bool bTabs = true)
{
	double position = labelAlignment->text_list_tab_stop_position_ ? labelAlignment->text_list_tab_stop_position_->get_value_unit(length::pt) : 0;
	CP_XML_WRITER(strm)
	{
		if (position > 0 && bTabs)
		{
			CP_XML_NODE(L"w:tabs")
			{
				CP_XML_NODE(L"w:tab")
				{
					CP_XML_ATTR(L"w:pos", (int)( 0.5 + 20.0 * position ));
					CP_XML_ATTR(L"w:val", L"num");
				}
			}
		}
		int w_hanging = (int)( 0.5 - 20.0 * labelAlignment->fo_text_indent_.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) );
		int w_left = (int)( 0.5 + 20.0 * labelAlignment->fo_margin_left_.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) );

		CP_XML_NODE(L"w:ind")
		{
			CP_XML_ATTR(L"w:left" , w_left);
			CP_XML_ATTR(L"w:hanging" , w_hanging);
		}
	}
}

void docx_serialize_level_justification(std::wostream & strm, style_list_level_properties * listLevelProperties)
{
    std::wstring w_lvlJc;

    text_align textAlign(text_align::Left);
	
	if ((listLevelProperties)  && (listLevelProperties->common_text_align_.fo_text_align_)) 
	{
		textAlign = *listLevelProperties->common_text_align_.fo_text_align_;
	}

    if (textAlign.get_type() == text_align::Right || textAlign.get_type() == text_align::End)
        w_lvlJc = L"right";
    else if (textAlign.get_type() == text_align::Center)
        w_lvlJc = L"center";
    else 
        w_lvlJc = L"left";

    if (!w_lvlJc.empty())
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"w:lvlJc")
			{      
				CP_XML_ATTR(L"w:val",w_lvlJc);   
			}
		}
	}
}

}
void text_list_level_style_number::docx_convert(oox::docx_conversion_context & Context)
{
	if (text_list_level_style_attr_.get_text_level() - 1 > 10)
        return;

    std::wostream & strm = Context.output_stream();
    style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( list_level_properties_.get() );
  
    style_list_level_label_alignment * labelAlignment = listLevelProperties ?
        dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"w:lvl")
		{
			CP_XML_ATTR(L"w:ilvl",(text_list_level_style_attr_.get_text_level() - 1));
		    
			CP_XML_NODE(L"w:start")
			{
				CP_XML_ATTR(L"w:val",text_list_level_style_number_attr_.text_start_value_);
			}
			if ((text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_) && 
				(text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_->get_type() != style_numformat::none))
			{
				CP_XML_NODE(L"w:numFmt")
				{
					std::wstring num_format = L"arabic";
					
					{
						switch(text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_->get_type())
						{
							case style_numformat::romanUc:		num_format= L"upperRoman"; break;
							case style_numformat::romanLc:		num_format= L"lowerRoman"; break;
							
							case style_numformat::alphaUc:		num_format= L"upperLetter"; break;
							case style_numformat::alphaLc:		num_format= L"lowerLetter"; break;
							
							case style_numformat::russianUp:	num_format = L"russianUpper"; break;
							case style_numformat::russianLo:	num_format = L"russianLower"; break;
							
							case style_numformat::aiueo:		num_format = L"russianLower"; break;
							
							case style_numformat::chineseCounting:	num_format = L"chineseCounting"; break;
							case style_numformat::chineseLegal:		num_format = L"chineseLegalSimplified"; break;
							
							case style_numformat::ideographLegal:		num_format = L"ideographLegalTraditional"; break;
							case style_numformat::ideographTraditional:	num_format = L"ideographTraditional"; break;
							case style_numformat::ideographZodiac:		num_format = L"ideographZodiac"; break;
							case style_numformat::ideographZodiacTraditional:	num_format = L"ideographZodiacTraditional"; break;
							
							case style_numformat::iroha:			num_format = L"iroha"; break;
							
							case style_numformat::koreanDigital:	num_format = L"koreanDigital"; break;
							
							case style_numformat::arabic:
							default:
									num_format= L"decimal"; break;
						}
					}
					CP_XML_ATTR(L"w:val", num_format);
				}
			}
			CP_XML_NODE(L"w:suff")
			{
				if ((labelAlignment) && (labelAlignment->text_label_followed_by_))
				{
					CP_XML_ATTR(L"w:val",labelAlignment->text_label_followed_by_.get());
				}
				else 
					CP_XML_ATTR(L"w:val",L"tab");
			}

			std::wstring w_lvlText;
			w_lvlText += text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_prefix_.get_value_or(L"");

//////////////////////////////////////////////////// 
			const unsigned int displayLevels = text_list_level_style_number_attr_.text_display_levels_;
			const unsigned int textLevel = text_list_level_style_attr_.get_text_level();
	    
			w_lvlText += GetLevelText(displayLevels, textLevel, Context);

			w_lvlText += text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_suffix_.get_value_or(L"");

			if (!w_lvlText.empty())
			{
				CP_XML_NODE(L"w:lvlText")
				{
					CP_XML_ATTR(L"w:val", w_lvlText);
				}
			}
/////////////////////////////////////////////////    
			docx_serialize_level_justification(CP_XML_STREAM(), listLevelProperties);

			double spaceBeforeTwip = 0.0;
			if (listLevelProperties && listLevelProperties->text_space_before_)
			{
				spaceBeforeTwip = 20.0 * listLevelProperties->text_space_before_->get_value_unit(length::pt);
			}
		    
			double minLabelWidthTwip = 0.0;
			if (listLevelProperties && listLevelProperties->text_min_label_width_)
			{
				minLabelWidthTwip = 20.0 * listLevelProperties->text_min_label_width_->get_value_unit(length::pt);
			}

			double minLabelDistanceTwip = 0.0;
			if (listLevelProperties && 
				text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_ &&
				listLevelProperties->text_min_label_distance_)
			{
				minLabelDistanceTwip = 20.0 * listLevelProperties->text_min_label_distance_->get_value_unit(length::pt);
			}
			CP_XML_NODE(L"w:pPr")
			{        
				if (Context.get_rtl())	CP_XML_NODE(L"w:bidi");
	
				if (labelAlignment)
				{
					docx_serialize_label_alignment_props(CP_XML_STREAM(), labelAlignment);                                 
				}
				else
				{    
					CP_XML_NODE(L"w:ind")
					{
						CP_XML_ATTR(L"w:left",((int)(minLabelWidthTwip + spaceBeforeTwip + 0.5)));
			            
						if (spaceBeforeTwip < 0.0)
						{
							CP_XML_ATTR(L"w:firstLine", ((int)(minLabelWidthTwip + 0.5)));
						}
						else
						{
							double hanging = 0.0;
							if ( (int)minLabelWidthTwip == 0)
							{
								if (spaceBeforeTwip < 0)
									hanging = spaceBeforeTwip;
								else
									hanging = -spaceBeforeTwip;
							}
							else
							{
								hanging = minLabelWidthTwip;
							}

							CP_XML_ATTR(L"w:hanging",((int)( hanging  + 0.5)));
						}
					}
				}
			}

			if (style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get()))
			{
				Context.get_styles_context().start();
		//to style_context
				textProperties->content_.docx_convert(Context);
		//serialize style_context
				Context.get_styles_context().docx_serialize_text_style( CP_XML_STREAM(), L"", Context.get_text_tracked_context().dumpRPr_);
			}		
		}
	}
}

void text_list_level_style_number::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (text_list_level_style_attr_.get_text_level() - 1 > 10)
        return;

	std::wostream & strm = Context.get_text_context().get_styles_context().list_style();

   style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( list_level_properties_.get() );
    
	style_list_level_label_alignment * labelAlignment = listLevelProperties ?
        dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;

	//int level = text_list_level_style_attr_.get_text_level();
	
	std::wstring num_format;

	if ((text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_ ) && 
		(text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_->get_type() != style_numformat::none))
	{
		switch(text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_->get_type())
		{
			case style_numformat::romanUc:	num_format= L"romanUc"; break;
			case style_numformat::romanLc:	num_format= L"romanLc"; break;
			case style_numformat::alphaUc:	num_format= L"alphaUc"; break;
			case style_numformat::alphaLc:	num_format= L"alphaLc"; break;
			case style_numformat::arabic:
			default:
														num_format= L"arabic"; break;
		}
		if (text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_prefix_)
		{
			num_format += L"ParenBoth";
		}
		else 
		{
			if (text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_suffix_)
			{
				if (*text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_suffix_ == L".")
					num_format += L"Period";
				else
					num_format += L"ParenR";
			}
			else
				num_format += L"Period";
		}
	}		
	
	CP_XML_WRITER(strm)
	{ 	
		if (style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get()))///эти свойства относятся 
			// к отрисовки значков !!! а не самого текста
	    {
	        textProperties->content_.pptx_convert_as_list(Context);
			strm << Context.get_text_context().get_styles_context().text_style().str();
	    }
		if (false == num_format.empty())
		{
			CP_XML_NODE(L"a:buAutoNum")//ms козлы !! для них оказыается ВАЖЕН порядок .. если записать это поле первым, а потом свойства - нихера в мс2010 не отображается верно !!!
			{
				CP_XML_ATTR(L"startAt",text_list_level_style_number_attr_.text_start_value_);
				CP_XML_ATTR(L"type", num_format);
			}
		}
	} 
}

namespace 
{

std::wstring convert_bullet_char(std::wstring c)
{
    //return c;
	std::wstring res;

	for(size_t i = 0; i < c.size(); ++i)
	{
		switch (c[i])
		{
		case L'\xE00C': res += L'\xF075';/*L'\xF0A8'*/; break;
		case L'\xE00A': res += L'\xF0A7'; break;
		case L'\x2611': res += L'\xF0FE'; break;
		case L'\x2022': res += L'\x2022'; res += L'\x20'; break;
		case L'\x25CF': res += L'\xF0B7'; break;
		case L'\x27A2': res += L'\xF0D8'; break;
		case L'\x2714': res += L'\xF0FC'; break;
			/*case L'\x25A0': res = L'\xF0A7'; break; */
		case L'\x25CB': res += L'\x006F'; break;
		case L'\x2794': res += L'\xF0DA'; break;
		case L'\x2717': res += L'\xF04F'; break;
		case L'\x2013': res += L'\x2013'; break;
		default:
			res += c[i];
			break;
		}
    }
    return res;
}

}

void text_list_level_style_bullet::docx_convert(oox::docx_conversion_context & Context) 
{    
	if (text_list_level_style_attr_.get_text_level() - 1 > 10)
        return;

    std::wostream & strm = Context.output_stream();
    style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( list_level_properties_.get() );
    
	style_list_level_label_alignment * labelAlignment = listLevelProperties ?
        dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"w:lvl")
		{
			CP_XML_ATTR(L"w:ilvl",(text_list_level_style_attr_.get_text_level() - 1));
			CP_XML_NODE(L"w:numFmt"){CP_XML_ATTR(L"w:val",L"bullet");}

			if ((labelAlignment) && (labelAlignment->text_label_followed_by_))
			{
				CP_XML_NODE(L"w:suff")
				{
					CP_XML_ATTR(L"w:val", labelAlignment->text_label_followed_by_.get());
				}
			}

			std::wstring bullet = text_list_level_style_bullet_attr_.text_bullet_char_.get_value_or(L"\x2022");
			CP_XML_NODE(L"w:lvlText")
			{
				std::wstring out = convert_bullet_char(bullet);
				CP_XML_ATTR(L"w:val", out);
			}

			docx_serialize_level_justification(CP_XML_STREAM(), listLevelProperties);

			double spaceBeforeTwip = 0.0;
			if (listLevelProperties && listLevelProperties->text_space_before_)
			{
				spaceBeforeTwip = 20.0 * listLevelProperties->text_space_before_->get_value_unit(length::pt);
			}
		    
			double minLabelWidthTwip = 0.0;
			if (listLevelProperties && listLevelProperties->text_min_label_width_)
			{
				minLabelWidthTwip = 20.0 * listLevelProperties->text_min_label_width_->get_value_unit(length::pt);
			}

			double minLabelDistanceTwip = 0.0;

			CP_XML_NODE(L"w:pPr")
			{
				if (Context.get_rtl())	CP_XML_NODE(L"w:bidi");

				if (labelAlignment)
				{
					docx_serialize_label_alignment_props(CP_XML_STREAM(), labelAlignment, false);                                 
				}
				else
				{    
					CP_XML_NODE(L"w:ind")
					{
						CP_XML_ATTR(L"w:left",((int)(minLabelWidthTwip + spaceBeforeTwip + 0.5)));
			            
						if (spaceBeforeTwip < 0.0)
						{
							CP_XML_ATTR(L"w:firstLine",((int)(minLabelWidthTwip + 0.5)));
						}
						else
						{
							double hanging = 0.0;
							if ( (int)minLabelWidthTwip == 0)
							{
								if (spaceBeforeTwip < 0)
									hanging = spaceBeforeTwip;
								else
									hanging = -spaceBeforeTwip;
							}
							else
							{
								hanging = minLabelWidthTwip;
							}
							CP_XML_ATTR(L"w:hanging" ,((int)( hanging  + 0.5)));
						}
					}
				}		    
			}
			
			if (style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get()))
			{
				Context.get_styles_context().start();
				textProperties->content_.docx_convert(Context);
				Context.get_styles_context().docx_serialize_text_style(CP_XML_STREAM(), L"", Context.get_text_tracked_context().dumpRPr_);
			}    
		}
	}
}


void text_list_level_style_bullet::pptx_convert(oox::pptx_conversion_context & Context) 
{    
	if (text_list_level_style_attr_.get_text_level() - 1 > 10)
        return;

	std::wostream & strm = Context.get_text_context().get_styles_context().list_style();

 //   style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( list_level_properties_.get() );
 //   
	//style_list_level_label_alignment * labelAlignment = listLevelProperties ?
 //       dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;

	//int level = text_list_level_style_attr_.get_text_level();
	
	CP_XML_WRITER(strm)
	{ 	
		style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get());
		std::wstring bullet = text_list_level_style_bullet_attr_.text_bullet_char_.get_value_or(L"\x2022");
	    
		if (textProperties)///эти свойства относятся 
			// к отрисовки значков !!! а не самого текста
	    {
	        textProperties->content_.pptx_convert_as_list(Context);
			strm << Context.get_text_context().get_styles_context().text_style().str();
	    }
		
		CP_XML_NODE(L"a:buChar")
		{
			//if ((textProperties) && (textProperties->content().style_font_charset_))
			//{
			//	if (textProperties->content().style_font_charset_.get() == L"x-xsymbol")bullet = bullet + 0xf000;
			//}
			CP_XML_ATTR(L"char", bullet/*convert_bullet_char(bullet)*/);
		}
	}
}



void text_list_level_style_image::docx_convert(oox::docx_conversion_context & Context) 
{    
	if (text_list_level_style_attr_.get_text_level() - 1 > 10)
        return;

    std::wostream & strm = Context.output_stream();
    style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( list_level_properties_.get() );
    
	style_list_level_label_alignment * labelAlignment = listLevelProperties ?
        dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"w:lvl")
		{
			CP_XML_ATTR(L"w:ilvl",(text_list_level_style_attr_.get_text_level() - 1));
			CP_XML_NODE(L"w:numFmt"){CP_XML_ATTR(L"w:val",L"bullet");}
   
			if ((labelAlignment) && (labelAlignment->text_label_followed_by_))
			{
				CP_XML_NODE(L"w:suff")
				{
					CP_XML_ATTR(L"w:val",labelAlignment->text_label_followed_by_.get() );
				}
			}

			std::wstring bullet = L"\x2022";
			CP_XML_NODE(L"w:lvlText")
			{
				CP_XML_ATTR(L"w:val", bullet/*convert_bullet_char(bullet)*/);
			}

			docx_serialize_level_justification(CP_XML_STREAM(), listLevelProperties);

			double spaceBeforeTwip = 0.0;
			if (listLevelProperties && listLevelProperties->text_space_before_)
			{
				spaceBeforeTwip = 20.0 * listLevelProperties->text_space_before_->get_value_unit(length::pt);
			}
		    
			double minLabelWidthTwip = 0.0;
			if (listLevelProperties && listLevelProperties->text_min_label_width_)
			{
				minLabelWidthTwip = 20.0 * listLevelProperties->text_min_label_width_->get_value_unit(length::pt);
			}

			double minLabelDistanceTwip = 0.0;

			CP_XML_NODE(L"w:pPr")
			{
				if (Context.get_rtl())	CP_XML_NODE(L"w:bidi");

				if (labelAlignment)
				{
					docx_serialize_label_alignment_props(CP_XML_STREAM(), labelAlignment, false);                                 
				}
				else
				{    
					CP_XML_NODE(L"w:ind")
					{
						CP_XML_ATTR(L"w:left",((int)(minLabelWidthTwip + spaceBeforeTwip + 0.5)));
			            
						if (spaceBeforeTwip < 0.0)
						{
							CP_XML_ATTR(L"w:firstLine",((int)(minLabelWidthTwip + 0.5)));
						}
						else
						{
							double hanging = 0.0;
							if ( (int)minLabelWidthTwip == 0)
							{
								if (spaceBeforeTwip < 0)
									hanging = spaceBeforeTwip;
								else
									hanging = -spaceBeforeTwip;
							}
							else
							{
								hanging = minLabelWidthTwip;
							}
							CP_XML_ATTR(L"w:hanging" ,((int)( hanging  + 0.5)));
						}
					}
				}		    
			}
			
			if (style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get()))
			{
				Context.get_styles_context().start();
				textProperties->content_.docx_convert(Context);
				Context.get_styles_context().docx_serialize_text_style(CP_XML_STREAM(), L"", Context.get_text_tracked_context().dumpRPr_);
			}    
		}
	}
}


void text_list_level_style_image::pptx_convert(oox::pptx_conversion_context & Context) 
{    
	if (text_list_level_style_attr_.get_text_level() - 1 > 10)
        return;

	std::wostream & strm = Context.get_text_context().get_styles_context().list_style();

 //   style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( list_level_properties_.get() );
 //   
	//style_list_level_label_alignment * labelAlignment = listLevelProperties ?
 //       dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;

	//int level = text_list_level_style_attr_.get_text_level();
	
	CP_XML_WRITER(strm)
	{ 	
		style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get());
		wchar_t bullet = L'\x2022';
	    
		if (textProperties)///эти свойства относятся 
			// к отрисовки значков !!! а не самого текста
	    {
	        textProperties->content_.pptx_convert_as_list(Context);
			strm << Context.get_text_context().get_styles_context().text_style().str();
	    }
		
		CP_XML_NODE(L"a:buChar")
		{
			//if ((textProperties) && (textProperties->content().style_font_charset_))
			//{
			//	if (textProperties->content().style_font_charset_.get() == L"x-xsymbol")bullet = bullet + 0xf000;
			//}
			CP_XML_ATTR(L"char",bullet/*convert_bullet_char(bullet)*/);
		}
	}
}
// text:outline-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_outline_style::ns = L"text";
const wchar_t * text_outline_style::name = L"outline-style";

void text_outline_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:name", style_name_, std::wstring(L""));
}

void text_outline_style::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"outline-level-style" == Name)
        CP_CREATE_ELEMENT(content_);
   else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
// text:outline-level-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_outline_level_style::ns = L"text";
const wchar_t * text_outline_level_style::name = L"outline-level-style";

void text_outline_level_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_num_format_attlist_.add_attributes(Attributes);
    common_num_format_prefix_suffix_attlist_.add_attributes(Attributes);
   
	CP_APPLY_ATTR(L"text:style-name", text_style_name_, std::wstring(L""));  
    CP_APPLY_ATTR(L"text:display-levels", text_display_levels_, (unsigned int)1);    
    CP_APPLY_ATTR(L"text:start-value", text_start_value_, (unsigned int)1);    
    CP_APPLY_ATTR(L"text:level", text_level_, (unsigned int)1);    
}

void text_outline_level_style::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"style" == Ns && L"list-level-properties" == Name)
	{
		CP_CREATE_ELEMENT(list_level_properties_);
	}
	else if (L"style" == Ns && L"properties" == Name)
	{
		create_element_and_read(Reader, L"style", L"list-level-properties", list_level_properties_, getContext());
	}
	else if (L"style" == Ns && L"text-properties" == Name)
	{
		CP_CREATE_ELEMENT(text_properties_);
	}
	else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
void text_outline_level_style::docx_convert(oox::docx_conversion_context & Context)
{
	if (text_level_ - 1 > 10)
        return;

    std::wostream & strm = Context.output_stream();
    style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( list_level_properties_.get() );
  
    style_list_level_label_alignment * labelAlignment = listLevelProperties ?
        dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"w:lvl")
		{
			CP_XML_ATTR(L"w:ilvl",  text_level_ - 1);
		    
			CP_XML_NODE(L"w:start")
			{
				CP_XML_ATTR(L"w:val", text_start_value_);
			}
			if ((common_num_format_attlist_.style_num_format_) && (common_num_format_attlist_.style_num_format_->get_type() != style_numformat::none))
			{
				CP_XML_NODE(L"w:numFmt")
				{
					std::wstring num_format = L"arabic";
					switch(common_num_format_attlist_.style_num_format_->get_type())
					{
						case style_numformat::romanUc:	num_format= L"upperRoman"; break;
						case style_numformat::romanLc:	num_format= L"lowerRoman"; break;
						case style_numformat::alphaUc:	num_format= L"upperLetter"; break;
						case style_numformat::alphaLc:	num_format= L"lowerLetter"; break;
						case style_numformat::arabic:
						default:
																	num_format= L"decimal"; break;
					}
					CP_XML_ATTR(L"w:val", num_format);
				}
				if ((labelAlignment) && (labelAlignment->text_label_followed_by_))
				{	
					CP_XML_NODE(L"w:suff")
					{
						CP_XML_ATTR(L"w:val", labelAlignment->text_label_followed_by_.get());
					}
				}

				std::wstring w_lvlText;
				w_lvlText += common_num_format_prefix_suffix_attlist_.style_num_prefix_.get_value_or(L"");

	//////////////////////////////////////////////////// 
	   
				w_lvlText += GetLevelText(text_display_levels_, text_level_, Context);

				w_lvlText += common_num_format_prefix_suffix_attlist_.style_num_suffix_.get_value_or(L"");

				if (!w_lvlText.empty())
				{
					CP_XML_NODE(L"w:lvlText")
					{
						CP_XML_ATTR(L"w:val", w_lvlText);
					}
				}
			}
/////////////////////////////////////////////////    
			docx_serialize_level_justification(CP_XML_STREAM(), listLevelProperties);

			double spaceBeforeTwip = 0.0;
			if (listLevelProperties && listLevelProperties->text_space_before_)
			{
				spaceBeforeTwip = 20.0 * listLevelProperties->text_space_before_->get_value_unit(length::pt);
			}
		    
			double minLabelWidthTwip = 0.0;
			if (listLevelProperties && listLevelProperties->text_min_label_width_)
			{
				minLabelWidthTwip = 20.0 * listLevelProperties->text_min_label_width_->get_value_unit(length::pt);
			}

			double minLabelDistanceTwip = 0.0;
			if (listLevelProperties && 
				common_num_format_attlist_.style_num_format_ &&
				listLevelProperties->text_min_label_distance_)
			{
				minLabelDistanceTwip = 20.0 * listLevelProperties->text_min_label_distance_->get_value_unit(length::pt);
			}
			CP_XML_NODE(L"w:pPr")
			{        
				if (Context.get_rtl())	CP_XML_NODE(L"w:bidi");
	
				if (labelAlignment)
				{
					docx_serialize_label_alignment_props(CP_XML_STREAM(), labelAlignment);                                 
				}
				else
				{    
					CP_XML_NODE(L"w:ind")
					{
						CP_XML_ATTR(L"w:left",((int)(minLabelWidthTwip + spaceBeforeTwip + 0.5)));
			            
						if (spaceBeforeTwip < 0.0)
						{
							CP_XML_ATTR(L"w:firstLine", ((int)(minLabelWidthTwip + 0.5)));
						}
						else
						{
							double hanging = 0.0;
							if ( (int)minLabelWidthTwip == 0)
							{
								if (spaceBeforeTwip < 0)
									hanging = spaceBeforeTwip;
								else
									hanging = -spaceBeforeTwip;
							}
							else
							{
								hanging = minLabelWidthTwip;
							}

							CP_XML_ATTR(L"w:hanging",((int)( hanging  + 0.5)));
						}
					}
				}
			}

			if (style_text_properties * textProperties = dynamic_cast<style_text_properties *>(text_properties_.get()))
			{
				Context.get_styles_context().start();
		//to style_context
				textProperties->content_.docx_convert(Context);
		//serialize style_context
				Context.get_styles_context().docx_serialize_text_style( CP_XML_STREAM(), L"", Context.get_text_tracked_context().dumpRPr_);
			}		
		}
	}
}


}
}
