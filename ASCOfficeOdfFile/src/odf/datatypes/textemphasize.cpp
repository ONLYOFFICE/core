#include "precompiled_cpodf.h"
#include "textemphasize.h"
#include "errors.h"

#include <boost_string.h>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const text_emphasize & _Val)
{
    switch(_Val.get_type())
    {
    case text_emphasize::None:
        _Wostream << L"none";
        break;
    case text_emphasize::Accent:
        _Wostream << L"accent";
        break;
    case text_emphasize::Dot:
        _Wostream << L"dot";
        break;
    case text_emphasize::Circle:
        _Wostream << L"circle";
        break;
    case text_emphasize::Disc:
        _Wostream << L"disc";
        break;
    default:
        break;
    }

    if (_Val.get_type() != text_emphasize::None)
    {
        switch (_Val.get_type_2())
        {
        case text_emphasize::Above:
            _Wostream << L" above";
            break;
        case text_emphasize::Below:
            _Wostream << L" below";
            break;
        }
    }

    return _Wostream;
}

text_emphasize text_emphasize::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    boost::algorithm::trim(tmp);
    std::vector< std::wstring > splitted;
    boost::algorithm::split(splitted, tmp, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

    text_emphasize::type type1_;
   
    if (splitted.size() > 0)
    {
        if (splitted[0] == L"none")
            type1_ = None;
        else if (splitted[0] == L"accent")
            type1_ = Accent;
        else if (splitted[0] == L"dot")
            type1_ = Dot;
        else if (splitted[0] == L"circle")
            type1_ = Circle;
        else if (splitted[0] == L"disc")
            type1_ = Disc;
        else
        {
            BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
            return text_emphasize(None, None);
        }
    }

    text_emphasize::type type2_ = None;
    if (splitted.size() > 1)
    {
        if (splitted[1] == L"above")
            type2_ = Above;
        else if (splitted[1] == L"below")
            type2_ = Below;
        else
        {
            BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
            return text_emphasize(None, None);
        }        

    }

    if (type1_ == None)
        return text_emphasize(None, None);
    else if (type2_ != None)
        return text_emphasize(type1_, type2_);
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
        return text_emphasize(None, None);
    }

}

} }

