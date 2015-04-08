#include "../odf/precompiled_cpodf.h"
#include "xlsx_font.h"
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/style_text_properties.h"

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

    /*
    _Wostream << L"<font>";
    if (font.bold)
        _Wostream << L"<b val=\"" << (int)(*font.bold) << "\" />";

    if (font.charset)
        _Wostream << L"<charset val=\"" << *font.charset << "\" />";

    if (font.color)
        xlsx_serialize(_Wostream, *font.color);

    if (font.condense)
        _Wostream << L"<condense val=\"" << (int)(*font.condense) << "\" />";

    if (font.extend)
        _Wostream << L"<extend val=\"" << (int)(*font.extend) << "\" />";

    if (font.family)
        _Wostream << L"<family val=\"" << (int)(*font.family) << "\" />";

    if (font.i)
        _Wostream << L"<i val=\"" << (int)(*font.i) << "\" />";

    if (font.name)
        _Wostream << L"<name val=\"" << *font.name << "\" />";

    if (font.outline)
        _Wostream << L"<outline val=\"" << (int)(*font.outline) << "\" />";

    if (font.scheme)
        _Wostream << L"<scheme val=\"" << *font.scheme << "\" />";

    if (font.shadow)
        _Wostream << L"<shadow val=\"" << (int)(*font.shadow) << "\" />";

    if (font.strike)
        _Wostream << L"<strike val=\"" << (int)(*font.strike) << "\" />";

    if (font.sz)
        _Wostream << L"<sz val=\"" << *font.sz << "\" />";

    if (font.u)
        _Wostream << L"<u val=\"" << *font.u << "\" />";

    if (font.vertAlign)
        _Wostream << L"<vertAlign val=\"" << *font.vertAlign << "\" />";

    _Wostream << L"</font>";
    */
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
}
}
