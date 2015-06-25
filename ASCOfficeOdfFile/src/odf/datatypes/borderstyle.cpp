#include <boost/algorithm/string.hpp>

#include "borderstyle.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const border_style & borderStyle)
{
	_Wostream << borderStyle.set_border_style_;

    return _Wostream;    
}

border_style::border_style(const std::wstring & Value) : initialized_(false), none_(false)
{
	set_border_style_ = Value;

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

border_style::border_style(const color & color_,  const std::wstring & style_, const length & length_)
{
	this->color_	= color_;
	this->style_	= style_;
	this->length_	= length_;

	std::wstringstream s;
	
	s << length_;
	s << std::wstring(L" ");
	s << style_;
	s << std::wstring(L" ");
	s << color_;
	
	set_border_style_ = s.str();
}


}
}
