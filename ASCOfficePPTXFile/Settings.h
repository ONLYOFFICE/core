#pragma once

#include <string>
#include <vector>

#if defined(_WIN32) || defined (_WIN64)
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
