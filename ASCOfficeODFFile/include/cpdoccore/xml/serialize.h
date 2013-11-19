#ifndef _CPDOCCORE_XML_SERIALIZE_H_
#define _CPDOCCORE_XML_SERIALIZE_H_

#include <ostream>
#include <cpdoccore/xml/xmlchar.h>

namespace cpdoccore {
namespace xml {

template <class CharT, class Element>
::std::basic_ostream<CharT> & serialize_name(::std::basic_ostream<CharT> & _Ostream, const Element * elm)
{
    _Ostream << elm->get_ns() << xml_char<CharT>::colon << elm->get_name();
    return _Ostream;
}

template <class CharT, class Element>
::std::basic_ostream<CharT> & serialize_simple_open_tag(::std::basic_ostream<CharT> & _Ostream, const Element * elm)
{
    _Ostream << xml_char<CharT>::openTag;
    serialize_name(_Ostream, elm);
    _Ostream << xml_char<CharT>::right_brocket;
    return _Ostream ;
}

template <class CharT, class Element>
::std::basic_ostream<CharT> & serialize_simple_close_tag(::std::basic_ostream<CharT> & _Ostream, const Element * elm)
{
    _Ostream << xml_char<CharT>::endTag;
    serialize_name(_Ostream, elm);
    _Ostream << xml_char<CharT>::right_brocket;
    return _Ostream ;
}

template <class CharT, class StringT, class AttrType>
::std::basic_ostream<CharT> & serialize_attribute(::std::basic_ostream<CharT> & _Ostream, const StringT & Name, const AttrType & Value)
{
    ::std::basic_string<CharT> name = Name;
    _Ostream << xml_char<CharT>::space << name << xml_char<CharT>::eq << xml_char<CharT>::quote << Value << xml_char<CharT>::quote;
    return _Ostream;
}

//template <class CharT, class StringT, class T>
//::std::basic_ostream<CharT> & serialize_attribute(::std::basic_ostream<CharT> & _Ostream, const StringT & Name, typename const optional<T>::Type & Value)
//{
//    if (Value)
//    {
//        ::std::basic_string<CharT> name = Name;
//        _Ostream << xml_char<CharT>::space << name << xml_char<CharT>::eq << xml_char<CharT>::quote << (*Value) << xml_char<CharT>::quote;
//    }
//    return _Ostream;
//}


template <class CharT, class StringT>
::std::basic_ostream<CharT> & serialize_attribute(::std::basic_ostream<CharT> & _Ostream, const StringT & Name, bool Value)
{
    ::std::basic_string<CharT> name = Name;
    _Ostream << xml_char<CharT>::space << name << xml_char<CharT>::eq << xml_char<CharT>::quote << 
        (Value ? xml_char<CharT>::trueVal : xml_char<CharT>::falseVal) << xml_char<CharT>::quote;
    return _Ostream;
}


#define CP_XML_SERIALIZE_ATTR_OPT(NAME, VAL)\
    if (VAL)\
        xml::serialize_attribute(_Wostream, (NAME), *(VAL));

#define CP_XML_SERIALIZE_ATTR(NAME, VAL)\
    xml::serialize_attribute(_Wostream, (NAME), (VAL));



} // namespace xml
} // namespace cpdoccore

#endif // #ifndef _CPDOCCORE_XML_SERIALIZE_H_
