#pragma once

#include <string>
#include <vector>

namespace FileSystem {
#ifdef UNICODE
    typedef std::wstring String;
#else
    typedef std::wstring String;
#endif
    typedef std::vector<String> StringArray;
}
