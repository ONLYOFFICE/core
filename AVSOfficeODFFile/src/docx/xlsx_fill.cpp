#include "precompiled_cpodf.h"
#include "xlsx_fill.h"

#include <cpdoccore/xml/simple_xml_writer.h>

#include <boost/foreach.hpp>
#include <boost/functional.hpp>

namespace cpdoccore {
namespace oox {

bool xlsx_patternFill::operator == (const xlsx_patternFill & rVal) const
{
    return fgColor == rVal.fgColor &&
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

    //_Wostream << L"<patternFill ";
    //if (patternFill.patternType)
    //    _Wostream << L"patternType=\"" << patternFill.patternType.get() << L"\" ";
    //_Wostream << L">";

    //if (patternFill.fgColor)
    //    xlsx_serialize(_Wostream, patternFill.fgColor.get(), L"fgColor");

    //if (patternFill.bgColor)
    //    xlsx_serialize(_Wostream, patternFill.bgColor.get(), L"bgColor");

    //_Wostream << L"</patternFill>";
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
    // TODO    
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
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"fill")
        {
            if (fill.patternFill)
                xlsx_serialize(CP_XML_STREAM(), fill.patternFill.get());

            if (fill.gradientFill)
                xlsx_serialize(CP_XML_STREAM(), fill.gradientFill.get());
        }
    }

    //_Wostream << L"<fill>";
    //if (fill.patternFill)
    //    xlsx_serialize(_Wostream, fill.patternFill.get());

    //if (fill.gradientFill)
    //    xlsx_serialize(_Wostream, fill.gradientFill.get());
    //

    //_Wostream << L"</fill>";
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
    return patternFill == rVal.patternFill &&
        gradientFill == rVal.gradientFill;
}

bool xlsx_fill::operator != (const xlsx_fill & rVal) const
{
    return !(this->operator ==(rVal));
}

}
}
