#include "precompiled_cpodf.h"
#include "writingmode.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const writing_mode & _Val)
{
    switch(_Val.get_type())
    {
    case writing_mode::LrTb:
        _Wostream << L"lr-tb";
        break;
    case writing_mode::RlTb:
        _Wostream << L"rl-tb";
        break;
    case writing_mode::TbRl:
        _Wostream << L"tb-rl";
        break;
    case writing_mode::TbLr:
        _Wostream << L"tb-lr";
        break;
    case writing_mode::Lr:
        _Wostream << L"lr";
        break;
    case writing_mode::Rl:
        _Wostream << L"rl";
        break;
    case writing_mode::Tb:
        _Wostream << L"tb";
        break;
    case writing_mode::Page:
        _Wostream << L"page";
        break;
    default:
        break;
    }
    return _Wostream;    
}

writing_mode writing_mode::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"lr-tb")
        return writing_mode( LrTb );
    else if (tmp == L"rl-tb")
        return writing_mode( RlTb );
    else if (tmp == L"tb-rl")
        return writing_mode( TbRl );
    else if (tmp == L"tb-lr")
        return writing_mode( TbLr );
    else if (tmp == L"lr")
        return writing_mode( Lr );
    else if (tmp == L"rl")
        return writing_mode( Rl );
    else if (tmp == L"tb")
        return writing_mode( Tb );
    else if (tmp == L"page")
        return writing_mode( Page );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return writing_mode( LrTb );
    }
}

} }
