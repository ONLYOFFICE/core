//
// Created by danya on 27.06.2021.
//

#ifndef CORE_STRINGADAPTOR_H
#define CORE_STRINGADAPTOR_H
#include "../lib/goo/GString.h"
#include <string>
#include <cstring>
#include <codecvt>
#include <locale>

class StringAdaptor {
private:
    char *str;
    wchar_t *w_str;

public:
    explicit StringAdaptor(const std::string &s);
    explicit StringAdaptor(const std::wstring &s);
    static StringAdaptor FromGString(GString &s);
    ~StringAdaptor();

    char* get_char_string();
    wchar_t* get_wchar_string();
    std::string get_string();
    std::wstring  get_wstring();
    GString get_gstring();

    void update_wstr();
    void update_str();

};


#endif //CORE_STRINGADAPTOR_H
