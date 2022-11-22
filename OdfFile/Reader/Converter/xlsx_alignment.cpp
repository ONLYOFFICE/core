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

#include "xlsxconversioncontext.h"
#include "xlsx_alignment.h"

#include "../Format/style_paragraph_properties.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_table_properties.h"

#include <ostream>
#include <boost/functional.hpp>
#include <xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

bool xlsx_alignment::operator == (const xlsx_alignment & rVal) const
{
    const bool res = 
        horizontal == rVal.horizontal &&
        indent == rVal.indent &&
        justifyLastLine.get_value_or(false) == rVal.justifyLastLine.get_value_or(false) &&
        readingOrder == rVal.readingOrder &&
        relativeIndent == rVal.relativeIndent &&
        shrinkToFit.get_value_or(false) == rVal.shrinkToFit.get_value_or(false) &&
        textRotation.get_value_or(0) == rVal.textRotation.get_value_or(0) &&
        vertical == rVal.vertical &&
        wrapText.get_value_or(false) == rVal.wrapText.get_value_or(false);   

    return res;
}

bool xlsx_alignment::operator != (const xlsx_alignment & rVal) const
{
    return !(this->operator ==(rVal));
}
std::size_t hash_value(xlsx_alignment const & val)
{
    std::size_t seed = 0;
	boost::hash_combine(seed, val.horizontal.get_value_or(L""));
    boost::hash_combine(seed, val.indent.get_value_or(0));
    boost::hash_combine(seed, val.justifyLastLine.get_value_or(false));
    boost::hash_combine(seed, val.readingOrder.get_value_or(0));
    boost::hash_combine(seed, val.relativeIndent.get_value_or(0));
    boost::hash_combine(seed, val.shrinkToFit.get_value_or(false));
    boost::hash_combine(seed, val.textRotation.get_value_or(0));
    boost::hash_combine(seed, val.vertical.get_value_or(L""));
    boost::hash_combine(seed, val.wrapText.get_value_or(false));
    return seed;

}

bool is_default(const xlsx_alignment & rVal)
{
    xlsx_alignment defaultAlignment;
    return rVal == defaultAlignment;
}

xlsx_alignment OdfProperties2XlsxAlignment(	xlsx_conversion_context * context,
											const odf_reader::text_format_properties_content_ptr	textProp, 
											const odf_reader::paragraph_format_properties			* parProp,
											const odf_reader::style_table_cell_properties_attlist	* cellProp)
{
    xlsx_alignment alignment;

    if (parProp)
	{
		if (parProp->fo_text_align_)
		{
			switch(parProp->fo_text_align_->get_type())
			{
				default:
				case odf_types::text_align::Start:
				case odf_types::text_align::Left:
					alignment.horizontal = L"left";
					break;
				case odf_types::text_align::Right:
				case odf_types::text_align::End:
					alignment.horizontal = L"right";
					break;
				case odf_types::text_align::Center:
					alignment.horizontal = L"center";
					break;
				case odf_types::text_align::Justify:
					alignment.horizontal = L"justify";
					break;        
			}
		}

		if (parProp->fo_text_align_last_ &&
			parProp->fo_text_align_last_->get_type() == odf_types::text_align::Justify)
		{
			alignment.justifyLastLine = true;            
		}
		if (parProp->fo_margin_left_)
		{
			if (parProp->fo_margin_left_->get_type() == odf_types::length_or_percent::Length)
			{
				 double indent_inch		= parProp->fo_margin_left_->get_length().get_value_unit(odf_types::length::inch);
				 double indent_symbol	= (int(( (indent_inch * 96.) - 5)/ context->getMaxDigitSize().first * 100. + 0.5)) /100.;

				  alignment.indent = (int)(indent_symbol / 1.5);
			}
			else //percent
			{
			}
		}
	}

	_CP_OPT(odf_types::vertical_align) v_align;
    
	if (parProp && parProp->style_vertical_align_)
		v_align = parProp->style_vertical_align_;
	else if (cellProp && cellProp->style_vertical_align_)
		v_align = cellProp->style_vertical_align_;
    
	if (v_align)
	{
		switch(v_align->get_type())
		{        
		case odf_types::vertical_align::Top:
			alignment.vertical = L"top";
			break;
		default:
		case odf_types::vertical_align::Auto:
		case odf_types::vertical_align::Middle:
			alignment.vertical = L"center";
			break;
		case odf_types::vertical_align::Baseline:
		case odf_types::vertical_align::Bottom:
			alignment.vertical = L"bottom";
			break;
		case odf_types::vertical_align::Justify:
			alignment.vertical = L"justify";
			break;
		}
	}

    if (textProp && textProp->style_text_rotation_angle_)
    {
        int angle = textProp->style_text_rotation_angle_.get();

		alignment.textRotation = angle;
    }

    if (cellProp)
	{
		if (cellProp->common_rotation_angle_attlist_.style_rotation_angle_)
		{
			 int angle = cellProp->common_rotation_angle_attlist_.style_rotation_angle_.get(); 

			if (angle > 90)		angle = angle - 90;
			if (angle < -90)	angle = 90 + angle;

			alignment.textRotation = angle;       //??? приоритет какой
		}		
		if ((cellProp->fo_wrap_option_) &&
			(cellProp->fo_wrap_option_->get_type() == odf_types::wrap_option::Wrap))		
		{
			alignment.wrapText = true;
		}
		if (cellProp->style_shrink_to_fit_)
		{
			alignment.shrinkToFit = *cellProp->style_shrink_to_fit_;
		}
	}

    return alignment;
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_alignment & alignment)
{
    if (is_default(alignment))
        return;
    
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"alignment")
        {
            if (alignment.horizontal)
                CP_XML_ATTR(L"horizontal", alignment.horizontal.get());
            
            if (alignment.indent)
                CP_XML_ATTR(L"indent", alignment.indent.get());

            if (alignment.justifyLastLine)
                CP_XML_ATTR(L"justifyLastLine", alignment.justifyLastLine.get());

            if (alignment.readingOrder)
                CP_XML_ATTR(L"readingOrder", alignment.readingOrder.get());

            if (alignment.relativeIndent)
                CP_XML_ATTR(L"relativeIndent", alignment.relativeIndent.get());

            if (alignment.shrinkToFit)
                CP_XML_ATTR(L"shrinkToFit", alignment.shrinkToFit.get());

            if (alignment.textRotation)
                CP_XML_ATTR(L"textRotation", alignment.textRotation.get());

            if (alignment.vertical)
                CP_XML_ATTR(L"vertical", alignment.vertical.get());

            if (alignment.wrapText)
                CP_XML_ATTR(L"wrapText", alignment.wrapText.get());
        }
    }

}

}
}
