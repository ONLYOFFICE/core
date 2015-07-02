#include <boost/algorithm/string.hpp>

#include "borderstyle.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const border_style & borderStyle)
{
    if (borderStyle.is_none())
    {
        _Wostream << std::wstring(L"none");
        return _Wostream;
    }

    _Wostream << borderStyle.get_length();

    switch (borderStyle.get_style())
    {
        case border_style::none:        _Wostream << L" none "; break;
        case border_style::double_:     _Wostream << L" double "; break;
        case border_style::dotted:      _Wostream << L" dotted "; break;
        case border_style::dashed:      _Wostream << L" dashed "; break;
        case border_style::dot_dashed:  _Wostream << L" dot-dashed "; break;
        case border_style::solid:
        default:
            _Wostream <<  L" solid "; break;
    }
    _Wostream << borderStyle.get_color();
    return _Wostream;
}

border_style::border_style(const border_style & Value)
{
    color_ = Value.get_color();
    length_ = Value.get_length();
    style_ = Value.get_style();

	none_ = Value.is_none();

    initialized_ = true;
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
		none_ = false;

        std::vector< std::wstring > splitted;
        boost::algorithm::split(splitted, Value, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

        try 
        {
            if (splitted.size() > 0)
                length_ = length::parse(splitted[0]);

            if (splitted.size() > 1)
            {
                if (splitted[1] == L"solid")        style_ = solid;
                if (splitted[1] == L"double")       style_ = double_;
                if (splitted[1] == L"dotted")       style_ = dotted;
                if (splitted[1] == L"dashed")       style_ = dashed;
                if (splitted[1] == L"dot-dashed")   style_ = dot_dashed;
            }

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

border_style::border_style(const color & color_,  const type & style_, const length & length_)
{
	this->color_	= color_;
	this->style_	= style_;
	this->length_	= length_;

    if (this->style_ == none)
        none_ = true;
	else 
		none_ = false;

    initialized_ = true;
}


}
}
