
#pragma once

namespace cpdoccore {
namespace xml {

template <class ElemT>
class xml_char
{
public:
    //static const ElemT eq;
    //static const ElemT quote;
    //static const ElemT space;
    //static const ElemT left_brocket;
    //static const ElemT right_brocket;
    //static const ElemT slash;
    //static const ElemT colon;
    //static const ElemT * openTag;
    //static const ElemT * closeTag;
    //static const ElemT * endTag;
    //static const ElemT * trueVal;
    //static const ElemT * falseVal;
};

template <>
class xml_char<char>
{
public:
    static const char eq;
    static const char quote;
    static const char space;
    static const char left_brocket;
    static const char right_brocket;
    static const char slash;
    static const char colon;
    static const char * openTag;
    static const char * closeTag;
    static const char * endTag;
    static const char * trueVal;
    static const char * falseVal;

};



template <>
class xml_char<wchar_t>
{
public:
    static const wchar_t eq;
    static const wchar_t quote;
    static const wchar_t space;
    static const wchar_t left_brocket;
    static const wchar_t right_brocket;
    static const wchar_t slash;
    static const wchar_t colon;
    static const wchar_t * openTag;
    static const wchar_t * closeTag;
    static const wchar_t * endTag;
    static const wchar_t * trueVal;
    static const wchar_t * falseVal;
};



typedef xml_char<char> xml_char_c;
typedef xml_char<wchar_t> xml_char_wc;

}
}
