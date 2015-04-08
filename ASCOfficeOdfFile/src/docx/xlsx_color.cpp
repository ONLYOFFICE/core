#include "../odf/precompiled_cpodf.h"
#include "xlsx_font.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/functional.hpp>
#include <sstream>

namespace cpdoccore {
namespace oox {

void xlsx_serialize(std::wostream & _Wostream, const xlsx_color & color)
{
    return xlsx_serialize(_Wostream, color, L"color");
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_color & color, const std::wstring & nodeName)
{
    std::wstringstream strm;

    if (color.autoVal)
        strm << L"auto=\"" << (bool)(*color.autoVal) << L"\" ";

    if (color.indexed)
        strm << L"indexed=\"" << (*color.indexed) << L"\" ";

    if (color.rgb)
    {
        std::wstring rgb = *color.rgb;
        boost::algorithm::to_upper(rgb);
        strm << L"rgb=\"" << rgb << L"\" ";
    }

    if (color.theme)
        strm << L"theme=\"" << (*color.theme) << L"\" ";

    if (color.tint)
        strm << L"tint=\"" << (*color.tint) << L"\" ";

    if (!strm.str().empty())
    {
        _Wostream << L"<" << nodeName <<L" ";
        _Wostream << strm.str();
        _Wostream << L"/>";
    }
}


bool xlsx_color::operator == (const xlsx_color & rVal) const
{
    const bool res = 
        autoVal == rVal.autoVal &&
        indexed == rVal.indexed &&
        rgb == rVal.rgb &&
        theme == rVal.theme &&
        tint == rVal.tint;
    return res;
}

bool xlsx_color::operator != (const xlsx_color & rVal) const
{
    return !(this->operator ==(rVal));
}

std::size_t hash_value(xlsx_color const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.autoVal.get_value_or(false));
    boost::hash_combine(seed, val.indexed.get_value_or(0));
    boost::hash_combine(seed, val.rgb.get_value_or(L""));
    boost::hash_combine(seed, val.theme.get_value_or(0));
    boost::hash_combine(seed, val.tint.get_value_or(0.0));
    return seed;    
}

}
}
