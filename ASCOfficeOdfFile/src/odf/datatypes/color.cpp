
#include "color.h"
#include <logging.h>

#include <ostream>
#include <sstream>

#include <boost/algorithm/string.hpp>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const color & _Color)
{
    _Wostream << _Color.get_color();
    return _Wostream;
}
bool operator== (const color & c1, const color & c2)
{
    if (c1.get_hex_value() == c2.get_hex_value())return true;
    else return false;
}

std::wstring html4_colors[16][2] = 
{	{L"aqua"	, L"#00ffff"},
	{L"black"	, L"#000000"}, 
	{L"blue"	, L"#0000ff"}, 
	{L"fuchsia"	, L"#ff00ff"}, 
	{L"gray"	, L"#808080"}, 
	{L"green"	, L"#008000"}, 
	{L"lime"	, L"#00ff00"}, 
	{L"maroon"	, L"#800000"}, 
	{L"navy"	, L"#000080"},
	{L"olive"	, L"#808000"}, 
	{L"purple"	, L"#800080"}, 
	{L"red"		, L"#ff0000"}, 
	{L"silver"	, L"#c0c0c0"}, 
	{L"teal"	, L"#008080"}, 
	{L"white"	, L"#FFFFFF"}, 
	{L"yellow"	, L"#ffff00"}
};
color color::parse(const std::wstring & Str)
{
	for (int i = 0 ; i < 16; i++)
	{
		if (Str == html4_colors[i][0])
			return color(html4_colors[i][1]);
	}
    return color(Str);
}

const std::wstring color::get_hex_value() const
{
    std::wstring tmp = color_;
    if ( boost::algorithm::contains(tmp, L"#") )
    {
        boost::algorithm::trim(tmp);
        boost::algorithm::trim_left_if(tmp, boost::algorithm::is_any_of("#"));     
        return tmp;
    }
    else if (tmp.size() == 6)
    {
        try 
        {
            std::wstringstream s;
            unsigned int t = 0;
            if ((s << tmp) && (s >> std::hex >> t) && (s >> std::ws).eof())
            {
                return tmp;
            }
        }
        catch(...)
        {
        }
        
    }

    //_CP_LOG << "[warning] convert color error (" << color_ << L")\n";
    return L"000000";
}

} }
