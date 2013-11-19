#ifndef _CPDOCCORE_XML_ELEMENT_H_
#define _CPDOCCORE_XML_ELEMENT_H_

#include <string>
#include <iosfwd>
#include <sstream>
#include <cpdoccore/xml/nodetype.h>

namespace cpdoccore {
namespace xml {

template <class StringT>
class attributes;

template <class CharT>
class element
{
public:
    virtual const CharT * get_ns() const = 0;
    virtual const CharT * get_name() const = 0;
    virtual NodeType get_xml_type() const = 0;

    virtual ::std::basic_ostream<CharT> & text_to_stream(::std::basic_ostream<CharT> & _Wostream) const
    {
        return _Wostream;
    }

    virtual ::std::basic_ostream<CharT> & xml_to_stream(::std::basic_ostream<CharT> & _Wostream) const = 0;
    
    std::basic_string<CharT> text_to_string() const;
    std::basic_string<CharT> xml_to_string() const;

    virtual ~element() = 0;

//    virtual const attributes< ::std::basic_string<CharT> > * get_attributes() const = 0;
};

template <class CharT>
inline element<CharT>::~element()
{}

typedef element<char> element_c;
typedef element<wchar_t> element_wc;

template <class Element, class CharT = wchar_t>
class element_impl : public element<CharT>
{
public:
    virtual const CharT * get_ns() const { return Element::ns; }
    virtual const CharT * get_name() const { return Element::name; }
    virtual NodeType get_xml_type() const { return Element::xml_type; };
//    virtual const attributes< ::std::basic_string<CharT> > * get_attributes() const = 0;
};

template <class CharT>
inline std::basic_string<CharT> element<CharT>::text_to_string() const
{
    ::std::basic_stringstream<CharT> strm;
    text_to_stream(strm);
    return strm.str();
}

template <class CharT>
inline std::basic_string<CharT> element<CharT>::xml_to_string() const
{
    ::std::basic_stringstream<CharT> strm;
    xml_to_stream(strm);
    return strm.str();
}

}
}

#endif
