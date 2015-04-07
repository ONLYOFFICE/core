#ifndef XML_COMMON
#define XML_COMMON

#include <string>
#if defined(_WIN32) || defined (_WIN64)
#include <atlbase.h>
#include <atlstr.h>
#else
#include "../Base/ASCString.h"
#endif

std::wstring	string2std_string(const CString& val);
std::string		string2std_string(const CStringA& val);
CString			std_string2string(const std::wstring& val);
CStringA		std_string2string(const std::string& val);

#endif //XML_COMMON