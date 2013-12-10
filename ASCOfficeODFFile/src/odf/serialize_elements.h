#pragma once

#include "office_elements.h"
#include <boost/foreach.hpp>

namespace cpdoccore {
namespace odf {

inline ::std::wostream & serialize_elements(::std::wostream & _Wostream, const office_element_ptr & elm)
{
    if (elm)
        elm->xml_to_stream(_Wostream);
    return _Wostream;
}

inline ::std::wostream & serialize_elements(::std::wostream & _Wostream, const office_element_ptr_array & elms)
{
    BOOST_FOREACH(const office_element_ptr & elm, elms)
    {
        serialize_elements(_Wostream, elm);
    }
    return _Wostream;
}

inline ::std::wostream & serialize_elements_text(::std::wostream & _Wostream, const office_element_ptr & elm)
{
    if (elm)
        elm->text_to_stream(_Wostream);
    return _Wostream;
}

inline ::std::wostream & serialize_elements_text(::std::wostream & _Wostream, const office_element_ptr_array & elms)
{
    BOOST_FOREACH(const office_element_ptr & elm, elms)
    {
        serialize_elements_text(_Wostream, elm);
    }
    return _Wostream;
}

#define CP_SERIALIZE_XML(ELEMENT) serialize_elements(_Wostream, (ELEMENT))
#define CP_SERIALIZE_TEXT(ELEMENT) serialize_elements_text(_Wostream, (ELEMENT))


}
}
