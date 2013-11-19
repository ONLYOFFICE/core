#include "precompiled_cpodf.h"
#include "textposition.h"
#include "errors.h"

#include <boost_string.h>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const text_position & _Val)
{
    switch(_Val.get_type())
    {
    case text_position::Sub:
        _Wostream << L"sub";
        break;
    case text_position::Super:
        _Wostream << L"super";
        break;
    case text_position::Percent:
        _Wostream << _Val.get_position();
        break;
    default:
        break;
    }

    if (_Val.has_font_size())
        _Wostream << L" "  << _Val.font_size();

    return _Wostream;
}

text_position text_position::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    boost::algorithm::trim(tmp);
    std::vector< std::wstring > splitted;
    boost::algorithm::split(splitted, tmp, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

    text_position::type type_= text_position::Sub;
    percent position_;
    bool has_font_width_ = false;
    percent font_width_;
   
    if (splitted.size() > 0)
    {
        if (splitted[0] == L"sub")
            type_ = text_position::Sub;
        else if (splitted[0] == L"super")
            type_ = text_position::Super;
        else if (boost::algorithm::contains(splitted[0], L"%"))
        {
            position_ = percent::parse(splitted[0]);
            type_ = text_position::Percent;
        }
    }

    if (splitted.size() > 1)
    {
        font_width_ = percent::parse(splitted[1]);
        has_font_width_ = true;
    }

    if (type_ != text_position::Percent)
    {
        if (!has_font_width_)
            return text_position(type_);
        else
            return text_position(type_, font_width_.get_value());
    }
    else
    {
        if (!has_font_width_)
            return text_position(position_.get_value());
        else
            return text_position(position_.get_value(), font_width_.get_value());
    }
}

} }

