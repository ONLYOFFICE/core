#include "precompiled_cpxml.h"

#include <cpdoccore/xml/xmlchar.h>

namespace cpdoccore {
namespace xml {

const char xml_char<char>::eq = '=';
const char xml_char<char>::quote = '\"';
const char xml_char<char>::space = ' ';
const char xml_char<char>::left_brocket = '<';
const char xml_char<char>::right_brocket = '>';
const char xml_char<char>::slash = '/';
const char xml_char<char>::colon = ':';
const char * xml_char<char>::openTag = "<";
const char * xml_char<char>::closeTag = " />";
const char * xml_char<char>::endTag = "</";
const char * xml_char<char>::trueVal = "true";
const char * xml_char<char>::falseVal = "false";

const wchar_t xml_char<wchar_t>::eq = L'=';
const wchar_t xml_char<wchar_t>::quote = L'\"';
const wchar_t xml_char<wchar_t>::space = L' ';
const wchar_t xml_char<wchar_t>::left_brocket = L'<';
const wchar_t xml_char<wchar_t>::right_brocket = L'>';
const wchar_t xml_char<wchar_t>::slash = L'/';
const wchar_t xml_char<wchar_t>::colon = L':';
const wchar_t * xml_char<wchar_t>::openTag = L"<";
const wchar_t * xml_char<wchar_t>::closeTag = L" />";
const wchar_t * xml_char<wchar_t>::endTag = L"</";
const wchar_t * xml_char<wchar_t>::trueVal = L"true";
const wchar_t * xml_char<wchar_t>::falseVal = L"false";

}
}