#include "precompiled_cpodf.h"
#include "presentationclass.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

presentation_class presentation_class::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"page")
        return presentation_class( page );
    else if (tmp == L"title")
        return presentation_class( title);
    else if (tmp == L"outline")
        return presentation_class( outline );
    else if (tmp == L"subtitle")
        return presentation_class( subtitle);
     else if (tmp == L"text")
        return presentation_class( text);
    else if (tmp == L"graphic")
        return presentation_class( graphic );
    else if (tmp == L"object")
        return presentation_class( object );
    else if (tmp == L"chart")
        return presentation_class( chart );
    else if (tmp == L"table")
        return presentation_class( table );
    else if (tmp == L"orgchart")
        return presentation_class( orgchart );
    else if (tmp == L"notes")
        return presentation_class( notes );
    else if (tmp == L"handout")
        return presentation_class( handout );
    else if (tmp == L"header")
        return presentation_class( header );
    else if (tmp == L"footer")
        return presentation_class( footer );
    else if (tmp == L"date-time")
        return presentation_class( date_time );
    else if (tmp == L"page-number")
        return presentation_class( page_number );
   else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return presentation_class( page );
    }
}

std::wstring presentation_class::get_type_ms()
{
	std::wstring res = L"";
	switch(type_)//str 2978
	{
        case title:
			res = L"title";
			break;
        case subtitle:
			res = L"subTitle";
			break;
		case graphic:
			res = L"dgm";
			break;
		case object:
			res = L"obj";
			break;
		case chart:
			res = L"chart";
			break;
		case table:
			res = L"tbl";
			break;
		case orgchart:
			res = L"chart";
			break;
		case header:
			res = L"hdr";
			break;
		case footer:
			res = L"ftr";
			break;
		case date_time:
			res = L"dt";
			break;
		case page_number:
			res = L"sldNum";
			break;
		case page:
		case notes:
		case handout:
        case outline:
		case text:
			res = L"body";
			break;
	}
	return res;
}



} }
