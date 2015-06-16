#include <boost/algorithm/string.hpp>

#include "borderstyle.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const border_style & borderStyle)
{
    std::wstring w_sz;
    std::wstring w_color;
    std::wstring w_val = L"single";
    std::wstring w_space;

    if (!borderStyle.initialized() || borderStyle.is_none())
    {
        w_val = L"none";
    }
    else if (borderStyle.initialized())
    {
        double width = borderStyle.get_length().get_value_unit(length::pt);
        //borderLineWidths ? borderLineWidths->get_summ_unit(length::pt) : borderStyle.get_length().get_value_unit(length::pt);
        int szInt = (int)(0.5 + 8.0 * width);
        if (szInt <= 0)
            szInt = 1;
        w_sz = boost::lexical_cast<std::wstring>( szInt );
        w_color = boost::lexical_cast<std::wstring>( borderStyle.get_color().get_hex_value() );

        const std::wstring borderStyleStr = borderStyle.get_style();
        if (szInt == 0)
            w_val = L"none";
        else if (borderStyleStr == L"solid" 
            || borderStyleStr == L"single")
            w_val = L"single";
        else if (borderStyleStr == L"double")
            w_val = L"double";
        else if (borderStyleStr == L"dotted")
            w_val = borderStyleStr;
        else if (borderStyleStr == L"dashed")
            w_val = borderStyleStr;
        else if (borderStyleStr == L"groove")
            w_val = L"thinThickMediumGap";
        else if (borderStyleStr == L"ridge")
            w_val = L"thickThinMediumGap";
        else if (borderStyleStr == L"inset")
            w_val = L"inset";
        else if (borderStyleStr == L"outset")
            w_val = L"outset";
        else if (borderStyleStr == L"hidden")
            w_val = L"nil";

    }
    std::wstring res;
    if (!w_val.empty())
        res += L" w:val=\"" + w_val + L"\" ";
    if (!w_sz.empty())
        res += L"w:sz=\"" + w_sz + L"\" ";
    if (!w_color.empty())
        res += L"w:color=\"" + w_color + L"\" ";
    if (!w_space.empty())
        res += L"w:space=\"" + w_space + L"\" ";

	_Wostream << res;

    return _Wostream;    
}

border_style::border_style(const std::wstring & Value) : initialized_(false), none_(false)
{
	std::wstring tmp =  boost::algorithm::trim_copy(Value);
    boost::algorithm::to_lower(tmp);
    
    if (L"none" == tmp || tmp.length() < 1)
    {
        none_ = true;
    }
    else
    {

        std::vector< std::wstring > splitted;
        boost::algorithm::split(splitted, Value, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

        try 
        {
            if (splitted.size() > 0)
                length_ = length::parse(splitted[0]);

            if (splitted.size() > 1)
                style_ = splitted[1];

            if (splitted.size() > 2)
                color_ = color::parse(splitted[2]);
        }
        catch(...)
        {
        }
    }
    initialized_ = true;
}

border_style border_style::parse( const std::wstring & Value)
{
	return border_style(Value);
}

}
}
