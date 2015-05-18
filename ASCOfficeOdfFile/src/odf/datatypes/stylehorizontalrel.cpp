
#include "stylehorizontalrel.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const horizontal_rel & _Val)
{
    switch(_Val.get_type())
    {
    case horizontal_rel::Page:                 _Wostream << L"page";                   break;
    case horizontal_rel::PageContent:          _Wostream << L"page-content";           break;
    case horizontal_rel::PageStartMargin:      _Wostream << L"page-start-margin";      break;
    case horizontal_rel::PageEndMargin:        _Wostream << L"page-end-margin";        break;
    case horizontal_rel::Frame:                _Wostream << L"frame";                  break;
    case horizontal_rel::FrameContent:         _Wostream << L"frame-content";          break;
    case horizontal_rel::FrameStartMargin:     _Wostream << L"frame-start-margin";     break;
    case horizontal_rel::FrameEndMargin:       _Wostream << L"frame-end-margin";       break;
    case horizontal_rel::Paragraph:            _Wostream << L"paragraph";              break;
    case horizontal_rel::ParagraphContent:     _Wostream << L"paragraph-content";      break;
    case horizontal_rel::ParagraphStartMargin: _Wostream << L"paragraph-start-margin"; break;
    case horizontal_rel::ParagraphEndMargin:   _Wostream << L"paragraph-end-margin";   break;
    case horizontal_rel::Char:                 _Wostream << L"char";                   break;
    default:
        break;
    }
    return _Wostream;
}

horizontal_rel horizontal_rel::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"page")
        return horizontal_rel( Page );
    else if (tmp == L"page-content")
        return horizontal_rel( PageContent );
    else if (tmp == L"page-start-margin")
        return horizontal_rel( PageStartMargin );
    else if (tmp == L"page-end-margin")
        return horizontal_rel( PageEndMargin );
    else if (tmp == L"frame")
        return horizontal_rel( Frame );
    else if (tmp == L"frame-content")
        return horizontal_rel( FrameContent );
    else if (tmp == L"frame-start-margin")
        return horizontal_rel( PageStartMargin );
    else if (tmp == L"frame-end-margin")
        return horizontal_rel( FrameEndMargin );
    else if (tmp == L"paragraph")
        return horizontal_rel( Paragraph );
    else if (tmp == L"paragraph-content")
        return horizontal_rel( ParagraphContent );
    else if (tmp == L"paragraph-start-margin")
        return horizontal_rel( ParagraphStartMargin );
    else if (tmp == L"paragraph-end-margin")
        return horizontal_rel( ParagraphEndMargin );
    else if (tmp == L"char")
        return horizontal_rel( Char );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return horizontal_rel( Page );
    }
}

} }
