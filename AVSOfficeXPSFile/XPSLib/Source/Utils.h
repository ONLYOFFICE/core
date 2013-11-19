#pragma once
#ifndef XPS_UTILS_INCLUDE_H_
#define XPS_UTILS_INCLUDE_H_

#include <vector>
#include <atlstr.h>

namespace XPS
{
	void PrepareVML(CString& str);
	void DelimString(const CString& source, std::vector<CString>& dest, TCHAR delim);
	void DelimString(const CString& source, std::vector<CString>& dest, const CString& delim);
	void DelimString(const CString& source, std::vector<std::vector<CString> >& dest, TCHAR delim1, TCHAR delim2);
	void GetFontKey(const CString& str, unsigned char* keys);

	void ARGB2BGRA(const CString& source, int& bgr, int& alpha);

	bool IsAlpha(TCHAR sym);
	double GetDouble(const CString& string);
	bool GetBool(const CString& string);

	CString GetPath(const CString& path);
	CString GetExtension(const CString& path);

	CString RemoveNamespace(const CString& str);

} // namespace XPS

#endif //XPS_UTILS_INCLUDE_H_