//
// Created by danya on 27.06.2021.
//

#include "StringAdaptor.h"
#include <codecvt>

StringAdaptor::StringAdaptor(const std::string &s) {
    str = new char[s.length() + 1];
    std::strcpy(str, s.c_str());
    update_wstr();
}

StringAdaptor::StringAdaptor(const std::wstring &s) {
    w_str = new wchar_t[s.length() + 1];
    std::wcscpy(w_str, s.c_str());
    update_str();
}

StringAdaptor StringAdaptor::FromGString(GString &s) {
    return StringAdaptor(s.getCString());
}

StringAdaptor StringAdaptor::FromGString(GString *s) {
    return StringAdaptor(s->getCString());
}

GString StringAdaptor::get_gstring() {
    return GString(str);
}

char* StringAdaptor::get_char_string() {
    return str;
}

wchar_t * StringAdaptor::get_wchar_string() {
    return w_str;
}

std::wstring StringAdaptor::get_wstring() {
    return std::wstring (w_str);
}

void StringAdaptor::update_wstr() {
    delete[] w_str;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring tmp = converter.from_bytes(str);
    w_str = new wchar_t[tmp.length() + 1];
    std::wcscpy(w_str, tmp.c_str());

}

void StringAdaptor::update_str() {
    delete[] str;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string tmp = converter.to_bytes(w_str);
    str = new char[tmp.length() + 1];
    std::strcpy(str, tmp.c_str());
}

StringAdaptor::~StringAdaptor() {
    delete[] str;
    delete[] w_str;
}