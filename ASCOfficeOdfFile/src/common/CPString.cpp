#include "precompiled_cpcommon.h"

#include <boost/program_options/detail/convert.hpp>

#include <cpdoccore/CPString.h>
#include <cstdlib>

namespace cpdoccore {

std::string ansi_to_utf8(const std::string & _AnsiString)
{
    char * currentLocale = ::setlocale(LC_ALL, NULL);
    std::string currentLocaleStr = currentLocale;
    ::setlocale(LC_ALL, "");
    size_t size = mbstowcs(NULL, _AnsiString.c_str(), 0);
    std::wstring utf16;
    utf16.resize(size);
    size = mbstowcs(&utf16[0], _AnsiString.c_str(), utf16.length());
    ::setlocale(LC_ALL, currentLocaleStr.c_str());

    return boost::to_utf8(utf16);
}

std::wstring utf8_to_utf16(const std::string & _Utf8String)
{
    return boost::from_utf8(_Utf8String);
}

std::string utf16_to_utf8(const std::wstring & _Utf16String)
{
    return boost::to_utf8(_Utf16String);
}

}
