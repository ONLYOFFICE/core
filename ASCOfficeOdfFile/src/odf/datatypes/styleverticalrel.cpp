
#include "styleverticalrel.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const vertical_rel & _Val)
{
    switch(_Val.get_type())
    {
    case vertical_rel::Page:
        _Wostream << L"page";
        break;
    case vertical_rel::PageContent:
        _Wostream << L"page-content";
        break;
    case vertical_rel::Frame:
        _Wostream << L"frame";
        break;
    case vertical_rel::FrameContent:
        _Wostream << L"frame-content";
        break;
    case vertical_rel::Paragraph:
        _Wostream << L"paragraph";
        break;
    case vertical_rel::ParagraphContent:
        _Wostream << L"paragraph-content";
        break;
    case vertical_rel::Char:
        _Wostream << L"char";
        break;
    case vertical_rel::Line:
        _Wostream << L"line";
        break;
    case vertical_rel::Baseline:
        _Wostream << L"baseline";
        break;
    case vertical_rel::Text:
        _Wostream << L"text";
        break;
    default:
        break;
    }
    return _Wostream;    
}

vertical_rel vertical_rel::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"page")
        return vertical_rel( Page );
    else if (tmp == L"page-content")
        return vertical_rel( PageContent );
    else if (tmp == L"frame")
        return vertical_rel( Frame );
    else if (tmp == L"frame-content")
        return vertical_rel( FrameContent );
     else if (tmp == L"paragraph")
        return vertical_rel( Paragraph );
     else if (tmp == L"paragraph-content")
        return vertical_rel( ParagraphContent );
     else if (tmp == L"char")
        return vertical_rel( Char );
     else if (tmp == L"line")
        return vertical_rel( Line );
     else if (tmp == L"baseline")
        return vertical_rel( Baseline );
     else if (tmp == L"text")
        return vertical_rel( Text );

    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return vertical_rel( Page );
    }
}

} }
