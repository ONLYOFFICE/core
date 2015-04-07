//#include "stdafx.h"
#include "stringcommon.h"

std::wstring string2std_string(const CString& val)
{
	return std::wstring(val.GetString(), val.GetLength());
}
std::string string2std_string(const CStringA& val)
{
	return std::string(val.GetString(), val.GetLength());
}
CString std_string2string(const std::wstring& val)
{
	return			CString(val.c_str(), val.length());
}
CStringA std_string2string(const std::string& val)
{
	return			CStringA(val.c_str(), val.length());
}