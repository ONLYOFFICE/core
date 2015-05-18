
#include "xlsx_border.h"
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/style_text_properties.h"

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

std::size_t hash_value(xlsx_border_edge const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.style.get_value_or(L""));
    boost::hash_combine(seed, val.color.get_value_or(xlsx_color()));
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

std::size_t hash_value(xlsx_border const & val)
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
    if (!borderEdge || is_default(borderEdge.get()))
        return true;
    else
        return false;
}

bool is_default(const xlsx_border_edge & borderEdge)
{
    if (!borderEdge.style || borderEdge.style.get() == L"none")
        return true;
    else
        return false;
}

bool is_default(const xlsx_border & border)
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
