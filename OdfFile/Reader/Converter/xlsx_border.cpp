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

#include "xlsx_border.h"
#include <boost/functional.hpp>
#include <xml/simple_xml_writer.h>
#include "../Format/style_text_properties.h"

namespace cpdoccore {
namespace oox {

bool xlsx_border::operator == (const xlsx_border & rVal) const
{
    const bool res = 
        diagonalUp.get_value_or(false) == rVal.diagonalUp.get_value_or(false) &&
        diagonalDown.get_value_or(false) == rVal.diagonalDown.get_value_or(false) && 
        outline.get_value_or(true) == rVal.outline.get_value_or(true) &&

        left.get_value_or(xlsx_border_edge(L"none")) == rVal.left.get_value_or(xlsx_border_edge(L"none")) &&
        right.get_value_or(xlsx_border_edge(L"none")) == rVal.right.get_value_or(xlsx_border_edge(L"none")) &&
        top.get_value_or(xlsx_border_edge(L"none")) == rVal.top.get_value_or(xlsx_border_edge(L"none")) &&
        bottom.get_value_or(xlsx_border_edge(L"none")) == rVal.bottom.get_value_or(xlsx_border_edge(L"none")) &&
        diagonal.get_value_or(xlsx_border_edge(L"none")) == rVal.diagonal.get_value_or(xlsx_border_edge(L"none")) &&
        vertical.get_value_or(xlsx_border_edge(L"none")) == rVal.vertical.get_value_or(xlsx_border_edge(L"none")) &&
        horizontal.get_value_or(xlsx_border_edge(L"none")) == rVal.horizontal.get_value_or(xlsx_border_edge(L"none"));

    return res;
}

bool xlsx_border::operator != (const xlsx_border & rVal) const
{
    return !(this->operator ==(rVal));
}

bool xlsx_border_edge::operator == (const xlsx_border_edge & rVal) const
{
    const bool res = 
        style.get_value_or(L"none") == rVal.style.get_value_or(L"none") && 
		//abs(width.get_value_or(0)/100 - rVal.width.get_value_or(0)/100)<10 && 
        color == rVal.color;
    return res;
}

bool xlsx_border_edge::operator != (const xlsx_border_edge & rVal) const
{
    return !(this->operator ==(rVal));    
}

std::size_t hash_value(xlsx_border_edge const& val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.style.get_value_or(L""));
    boost::hash_combine(seed, val.color.get_value_or(xlsx_color()));
    return seed;
}
std::size_t hash_value(const _CP_OPT(xlsx_border_edge) & val)
{
    std::size_t seed = 0;
    if (val)
    {
        boost::hash_combine(seed, val->style.get_value_or(L""));
        boost::hash_combine(seed, val->color.get_value_or(xlsx_color()));
    }
    return seed;
}
void xlsx_serialize(std::wostream & _Wostream, const _CP_OPT(xlsx_border_edge) & borderEdge, const std::wstring & name)
{
    if (borderEdge)
    {
        CP_XML_WRITER(_Wostream)
        {
            CP_XML_NODE(name)
            {
                if (borderEdge->style)
                    CP_XML_ATTR(L"style", borderEdge->style.get());

                if (borderEdge->color)
                    xlsx_serialize(CP_XML_STREAM(), borderEdge->color.get());
            }
        }
    }        
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_border & border)
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"border")
        {
            if (border.diagonalUp)
                CP_XML_ATTR(L"diagonalUp", border.diagonalUp.get());
            if (border.diagonalDown)
                CP_XML_ATTR(L"diagonalDown", border.diagonalDown.get());
            if (border.outline && border.outline.get() == false)
                CP_XML_ATTR(L"outline", border.outline.get());


            xlsx_serialize(CP_XML_STREAM(), border.left, L"left");
            xlsx_serialize(CP_XML_STREAM(), border.right, L"right");
            xlsx_serialize(CP_XML_STREAM(), border.top, L"top");
            xlsx_serialize(CP_XML_STREAM(), border.bottom, L"bottom");
            xlsx_serialize(CP_XML_STREAM(), border.diagonal, L"diagonal");
            xlsx_serialize(CP_XML_STREAM(), border.vertical, L"vertical");
            xlsx_serialize(CP_XML_STREAM(), border.horizontal, L"horizontal");
        }
    }
}

std::size_t hash_value(xlsx_border const& val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.diagonalUp.get_value_or(false));
    boost::hash_combine(seed, val.diagonalDown.get_value_or(false));
    boost::hash_combine(seed, val.outline.get_value_or(false));

    boost::hash_combine(seed, val.left);
    boost::hash_combine(seed, val.right);
    boost::hash_combine(seed, val.top);
    boost::hash_combine(seed, val.bottom);
    boost::hash_combine(seed, val.diagonal);
    boost::hash_combine(seed, val.vertical);
    boost::hash_combine(seed, val.horizontal);

    return seed;
}

bool is_default(const _CP_OPT(xlsx_border_edge) & borderEdge)
{
    if (!borderEdge)                    return true;
    if (is_default(borderEdge.get()))  return true;

    return false;
}

bool is_default(xlsx_border_edge * borderEdge)
{
    if (!borderEdge)                         return true;
    if (!borderEdge->style)                  return true;
    if (borderEdge->style.get() == L"none")  return true;

    return false;
}
bool is_default(xlsx_border_edge const& borderEdge)
{
    if (!borderEdge.style)                  return true;
    if (borderEdge.style.get() == L"none")  return true;

    return false;
}
bool is_default(xlsx_border const& border)
{
    if (border.diagonalUp.get_value_or(false) == false &&
        border.diagonalDown.get_value_or(false) == false &&
        border.outline.get_value_or(true) == true &&
        is_default(border.left) &&
        is_default(border.right) &&
        is_default(border.top) &&
        is_default(border.bottom) &&
        is_default(border.diagonal) &&
        is_default(border.vertical) &&
        is_default(border.horizontal)
        )
        return true;
    else
        return false;
}

}
}
