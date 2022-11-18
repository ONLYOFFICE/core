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
#include "xlsx_font.h"

#include "../Format/style_text_properties.h"
#include "../Format/style_table_properties.h"
#include "../Format/style_paragraph_properties.h"

#include <boost/functional.hpp>
#include <xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

namespace {

std::wostream & operator << (std::wostream & _Wostream, const xlsx_vertAlign & vertAlign)
{
    switch(vertAlign)
    {
    default:
    case vertAlignBaseline:
        _Wostream << L"baseline";
        break;
    case vertAlignSubscript:
        _Wostream << L"subscript";
        break;
    case vertAlignSuperscript:
        _Wostream << L"superscript";
        break;
    }
    return _Wostream;
}

std::wostream & operator << (std::wostream & _Wostream, const XlsxUnderline & u)
{
    switch(u)
    {
    default:
    case XUNDERLINE_NONE:
        _Wostream << L"none";
        break;
    case XUNDERLINE_SINGLE:
        _Wostream << L"single";
        break;
    case XUNDERLINE_DOUBLE:
        _Wostream << L"double";
        break;
    case XUNDERLINE_SINGLE_ACCOUNTING:
        _Wostream << L"singleAccounting";
        break;
    case XUNDERLINE_DOUBLE_ACCOUNTING:
        _Wostream << L"doubleAccounting";
        break;
    }
    return _Wostream;
}

}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_font & font)
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"font")
        {
            if (font.bold) 
            {
                CP_XML_NODE(L"b")
                {
                    CP_XML_ATTR(L"val", (int)(*font.bold));
                }            
            }

            if (font.charset)
            {
                CP_XML_NODE(L"charset")
                {
                    CP_XML_ATTR(L"val", *font.charset);
                }
            }

            if (font.color)
                xlsx_serialize(CP_XML_STREAM(), *font.color);

            if (font.condense)
            {
                CP_XML_NODE(L"condense")
                {
                    CP_XML_ATTR(L"val", (int)(*font.condense));
                }
            }

            if (font.extend)
            {
                CP_XML_NODE(L"extend")
                {
                    CP_XML_ATTR(L"val", (int)(*font.extend));
                }
            }

            if (font.family)
            {
                CP_XML_NODE(L"family")
                {
                    CP_XML_ATTR(L"val", (int)(*font.family));
                }
            }

            if (font.i)
            {
                CP_XML_NODE(L"i")
                {
                    CP_XML_ATTR(L"val", (int)(*font.i));
                }
            }

            if (font.name)
            {
                CP_XML_NODE(L"name")
                {
                    CP_XML_ATTR(L"val", *font.name);
                }
            }

            if (font.outline)
            {
                CP_XML_NODE(L"outline")
                {
                    CP_XML_ATTR(L"val", (int)(*font.outline));
                }
            }

            if (font.scheme)
            {
                CP_XML_NODE(L"scheme")
                {
                    CP_XML_ATTR(L"val", *font.scheme);
                }
            }

            if (font.shadow)
            {
                CP_XML_NODE(L"shadow")
                {
                    CP_XML_ATTR(L"val", (int)(*font.shadow));
                }
            }

            if (font.strike)
            {
                CP_XML_NODE(L"strike")
                {
                    CP_XML_ATTR(L"val", (int)(*font.strike));
                }
            }

            if (font.sz)
            {
                CP_XML_NODE(L"sz")
                {
                    CP_XML_ATTR(L"val", *font.sz);
                }
            }

            if (font.u)
            {
                CP_XML_NODE(L"u")
                {
                    CP_XML_ATTR(L"val", *font.u);
                }
            }

            if (font.vertAlign)
            {
                CP_XML_NODE(L"vertAlign")
                {
                    CP_XML_ATTR(L"val", *font.vertAlign);
                }
            }
            
        }
    }
}

bool xlsx_font::operator == (const xlsx_font & rVal) const
{
    const bool res =     
    bold.get_value_or(false) == rVal.bold.get_value_or(false) &&
    charset == rVal.charset &&
    color == rVal.color &&
    condense.get_value_or(false) == rVal.condense.get_value_or(false) &&
    extend.get_value_or(false) == rVal.extend.get_value_or(false) &&
    family == rVal.family &&
    i.get_value_or(false) == rVal.i.get_value_or(false) &&
    name == rVal.name &&
    outline.get_value_or(false) == rVal.outline.get_value_or(false) &&
    scheme == rVal.scheme &&
    shadow.get_value_or(false) == rVal.shadow.get_value_or(false) &&
    strike.get_value_or(false) == rVal.strike.get_value_or(false) &&
    sz == rVal.sz &&
    u.get_value_or(XUNDERLINE_NONE) == rVal.u.get_value_or(XUNDERLINE_NONE) &&
    vertAlign == rVal.vertAlign;

    return res;
}

bool xlsx_font::operator != (const xlsx_font & rVal) const
{
    return !(this->operator ==(rVal));
}

std::size_t hash_value(xlsx_font const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.bold.get_value_or(false));
    boost::hash_combine(seed, val.charset.get_value_or(0));
    boost::hash_combine(seed, val.color.get_value_or(xlsx_color()));
    boost::hash_combine(seed, val.condense.get_value_or(false));
    boost::hash_combine(seed, val.extend.get_value_or(false));
    boost::hash_combine(seed, val.family.get_value_or(XFAMILY_EMPTY));
    boost::hash_combine(seed, val.i.get_value_or(false));
    boost::hash_combine(seed, val.name.get_value_or(L""));
    boost::hash_combine(seed, val.outline.get_value_or(false));
    boost::hash_combine(seed, val.scheme.get_value_or(L""));
    boost::hash_combine(seed, val.shadow.get_value_or(false));
    boost::hash_combine(seed, val.strike.get_value_or(false));
    boost::hash_combine(seed, val.sz.get_value_or(0));
    boost::hash_combine(seed, val.u.get_value_or(XUNDERLINE_NONE));
    boost::hash_combine(seed, val.vertAlign.get_value_or(vertAlignBaseline));
    return seed;
}

//----------------------------------------------------------------------------------

XlsxFontCharset GetXlsxFontCharset(const odf_reader::text_format_properties_content_ptr textProp)
{
    // TODO
    return XCHARSET_EMPTY;    
}

XlsxFontFamily GetXlsxFontFamily(const odf_reader::text_format_properties_content_ptr textProp)
{
    // TODO
    return XFAMILY_EMPTY;
}

xlsx_font::xlsx_font (	const odf_reader::text_format_properties_content_ptr	textProp,
						const odf_reader::paragraph_format_properties			* parProp,
						const odf_reader::style_table_cell_properties_attlist	* cellProp)
{
	bEnabled = false;
	if (!textProp) return;

	bEnabled = true;
    if (textProp->fo_font_weight_)
    {
        if (textProp->fo_font_weight_.get().get_type() == odf_types::font_weight::WBold)
            bold = true;
        else
            bold = false;
    }

    if (textProp->fo_font_style_)
    {
        if (textProp->fo_font_style_.get().get_type() == odf_types::font_style::Italic)
            i = true;
        else
            i = false;
    }

    XlsxFontCharset charset_ = GetXlsxFontCharset(textProp);
    if (charset_ != XCHARSET_EMPTY)
    {
        charset = (unsigned int)charset_;
    }

    XlsxFontFamily family_ = GetXlsxFontFamily(textProp);
    if (family_ != XFAMILY_EMPTY)
    {
        family = family_;
    }

    if (textProp->style_font_name_)
    {
        name = textProp->style_font_name_.get();            
    }

    if (textProp->fo_font_size_)
    {
        sz = textProp->fo_font_size_->get_length().get_value_unit(odf_types::length::pt);
    }
    //else
    //{
    //    sz = 10.;//kDefaultFontSize;        //todooo ... вытащить как в math
    //}

    if ((textProp->style_text_underline_type_ &&
        textProp->style_text_underline_type_->get_type() != odf_types::line_type::None) ||
    
        (textProp->style_text_underline_style_ &&
        textProp->style_text_underline_style_->get_type() != odf_types::line_style::None))
    {
        if (textProp->style_text_underline_type_ &&
            textProp->style_text_underline_type_->get_type() == odf_types::line_type::Double)
            u = XUNDERLINE_DOUBLE;
        else
            u = XUNDERLINE_SINGLE;
    }
	
    if ((textProp->style_text_line_through_type_ &&
        textProp->style_text_line_through_type_->get_type() != odf_types::line_type::None) ||

        (textProp->style_text_line_through_style_ &&
        textProp->style_text_line_through_style_->get_type() != odf_types::line_style::None))
    {
        strike = true;
    }

    if (textProp->fo_text_shadow_)
    {
        if (textProp->fo_text_shadow_->get_type() == odf_types::shadow_type::Enable)
            shadow = true;
        else
            shadow = false;
    }

	bool default_color = false;
	if (textProp->style_use_window_font_color_)
		default_color = true;
   
	if (textProp->fo_color_ && !default_color)
    {
		color = xlsx_color();
        color->rgb = L"ff" + textProp->fo_color_->get_hex_value();

    }
}

}
}
