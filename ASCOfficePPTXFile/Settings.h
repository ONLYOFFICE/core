#pragma once

#include <string>
#include <vector>

#ifdef _WIN32
#include <atlbase.h>
#include <atlstr.h>
#endif

namespace FileSystem {
#ifdef UNICODE
    typedef std::wstring String;
#else
    typedef std::wstring String;
#endif
    typedef std::vector<String> StringArray;
}
