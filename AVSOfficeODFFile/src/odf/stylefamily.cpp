#include "precompiled_cpodf.h"
#include "stylefamily.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const style_family & _Val)
{
    switch(_Val.get_type())
    {
    case style_family::Paragraph:
        _Wostream << L"paragraph";
        break;
    case style_family::Text:
        _Wostream << L"text";
        break;
    case style_family::Section:
        _Wostream << L"section";
        break;
    case style_family::Table:
        _Wostream << L"table";
        break;
    case style_family::TableColumn:
        _Wostream << L"table-column";
        break;
    case style_family::TableRow:
        _Wostream << L"table-row";
        break;
    case style_family::TableCell:
        _Wostream << L"table-cell";
        break;
    case style_family::TablePage:
        _Wostream << L"table-page";
        break;
    case style_family::Chart:
        _Wostream << L"chart";
        break;
    default:
    case style_family::Default:
        _Wostream << L"default";
        break;
    case style_family::DrawingPage:
        _Wostream << L"drawing-page";
        break;
    case style_family::Graphic:
        _Wostream << L"graphic";
        break;
    case style_family::Presentation:
        _Wostream << L"presentation";
        break;
    case style_family::Control:
        _Wostream << L"control";
        break;
    case style_family::Ruby:
        _Wostream << L"ruby";
        break;
    }
    return _Wostream;    
}

style_family style_family::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"paragraph")
        return style_family( Paragraph );
    else if (tmp == L"text")
        return style_family( Text );
    else if (tmp == L"section")
        return style_family( Section );
    else if (tmp == L"table")
        return style_family( Table );
    else if (tmp == L"table-column")
        return style_family( TableColumn );
    else if (tmp == L"table-row")
        return style_family( TableRow );
    else if (tmp == L"table-cell")
        return style_family( TableCell );
    else if (tmp == L"table-page")
        return style_family( TablePage );
    else if (tmp == L"chart")
        return style_family( Chart );
    else if (tmp == L"default")
        return style_family( Default );
    else if (tmp == L"drawing-page")
        return style_family( DrawingPage );
    else if (tmp == L"graphic")
        return style_family( Graphic );
    else if (tmp == L"presentation")
        return style_family( Presentation );
    else if (tmp == L"control")
        return style_family( Control );
    else if (tmp == L"ruby")
        return style_family( Ruby );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return style_family( Default );
    }
}

} }
