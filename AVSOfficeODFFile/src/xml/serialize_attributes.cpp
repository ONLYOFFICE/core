/// \file   serialize_attributes.cpp
/// \author nikolay.pogorskiy@gmail.com 

#include "precompiled_cpxml.h"

#include <ostream>

#include <cpdoccore/xml/serialize_attributes.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/xmlchar.h>


namespace cpdoccore {

namespace xml {

template <class ElemT>
::std::basic_ostream<ElemT> & serialize_attributes_impl( ::std::basic_ostream<ElemT> & _Ostream, const attributes< ::std::basic_string< ElemT > > * Attributes )
{
    const size_t count = Attributes->size();
    for (size_t i = 0; i < count; ++i)
    {
        const attributes< ::std::basic_string<ElemT> >::key_value & val = Attributes->at(i);
        _Ostream << val.first << xml_char<ElemT>::eq << xml_char<ElemT>::quote << val.second 
            << xml_char<ElemT>::quote << xml_char<ElemT>::space;
    }
    return _Ostream;
}

::std::wostream & serialize_attributes( ::std::wostream & _Wostream, const attributes< ::std::wstring > * Attributes )
{
    return serialize_attributes_impl<wchar_t>(_Wostream, Attributes);
}

::std::ostream & serialize_attributes( ::std::ostream & _Wostream, const attributes< ::std::string > * Attributes )
{
    return serialize_attributes_impl<char>(_Wostream, Attributes);    
}

}
}
