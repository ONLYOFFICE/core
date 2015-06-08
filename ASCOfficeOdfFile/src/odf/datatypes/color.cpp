
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

color color::parse(const std::wstring & Str)
{
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
