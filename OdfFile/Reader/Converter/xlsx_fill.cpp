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
#include "xlsx_fill.h"

#include "../Format/style_table_properties.h"
#include "../Format/style_graphic_properties.h"

#include <xml/simple_xml_writer.h>

#include <boost/functional/hash/hash.hpp>

namespace cpdoccore {
namespace oox {

bool xlsx_patternFill::operator == (const xlsx_patternFill & rVal) const
{
    return	fgColor == rVal.fgColor &&
			bgColor == rVal.bgColor && 
		patternType == rVal.patternType;
}

bool xlsx_patternFill::operator != (const xlsx_patternFill & rVal) const
{
    return !(this->operator ==(rVal));
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_patternFill & patternFill)
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"patternFill")
        {
            CP_XML_ATTR(L"patternType", patternFill.patternType.get());            
        
            if (patternFill.fgColor)
                xlsx_serialize(CP_XML_STREAM(), patternFill.fgColor.get(), L"fgColor");

            if (patternFill.bgColor)
                xlsx_serialize(CP_XML_STREAM(), patternFill.bgColor.get(), L"bgColor");

        }
    }
}

std::size_t hash_value(xlsx_patternFill const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.patternType.get_value_or(L""));
    boost::hash_combine(seed, val.fgColor.get_value_or(xlsx_color()));
    boost::hash_combine(seed, val.bgColor.get_value_or(xlsx_color()));
    return seed;
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_gradientFill & gradientFill)
{
    // todooo  
}

std::size_t hash_value(xlsx_gradientFill const & val)
{
    std::size_t seed = 0;
    return seed;
}
bool xlsx_gradientFill::operator == (const xlsx_gradientFill & rVal) const
{
    return true;
}

bool xlsx_gradientFill::operator != (const xlsx_gradientFill & rVal) const
{
    return !(this->operator ==(rVal));
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_fill & fill)
{
	if (!fill.patternFill && !fill.gradientFill) return;

    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"fill")
        {
            if (fill.patternFill)
                xlsx_serialize(CP_XML_STREAM(), *fill.patternFill);

            if (fill.gradientFill)
                xlsx_serialize(CP_XML_STREAM(), *fill.gradientFill);
        }
    }
}

std::size_t hash_value(xlsx_fill const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.patternFill.get_value_or(xlsx_patternFill()));
    boost::hash_combine(seed, val.gradientFill.get_value_or(xlsx_gradientFill()));
    return seed;

}
bool xlsx_fill::operator == (const xlsx_fill & rVal) const
{
    return	patternFill	== rVal.patternFill &&
			gradientFill == rVal.gradientFill;
}

bool xlsx_fill::operator != (const xlsx_fill & rVal) const
{
    return !(this->operator ==(rVal));
}

xlsx_fill::xlsx_fill(	const odf_reader::graphic_format_properties				* graphProp,
						const odf_reader::style_table_cell_properties_attlist	* cellProp)
{
	bEnabled = false;
	bDefault = false;
	if (cellProp)
	{
		if (_CP_OPT(odf_types::background_color) bgClr = cellProp->common_background_color_attlist_.fo_background_color_)
		{
			if (bgClr->get_type() != odf_types::background_color::Transparent)
			{
				bEnabled = true;

				xlsx_color color;
				// alfa + rgb
				color.rgb = L"ff" + bgClr->get_color().get_hex_value();
				
				patternFill = xlsx_patternFill();
				patternFill->bgColor		= color;
				patternFill->fgColor		= color;
				patternFill->patternType = L"solid";
			}
		}
	}
}

}
}
