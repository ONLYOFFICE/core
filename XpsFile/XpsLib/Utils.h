#ifndef _XPS_XPSLIB_UTILS_H
#define _XPS_XPSLIB_UTILS_H

#include <string>
#include <vector>

namespace XPS
{
	bool   IsAlpha(wchar_t wChar);
	double GetDouble(const std::wstring& wsString);
	int    GetInteger(const std::wstring& wsString);
	bool   GetBool(const std::wstring& wsString);
	void   GetBgra(const std::wstring& wsString, int& nBgr, int& nAlpha);

	std::wstring NormalizePath(const std::wstring& wsPath);
	std::wstring GetPath(const std::wstring& wsPath);
	std::wstring GetFileName(const std::wstring& wsPath);
	std::wstring GetFileExtension(const std::wstring& wsPath);
	std::wstring RemoveNamespace(const std::wstring& wsString);

	std::vector<std::vector<std::wstring>> Split(const std::wstring& wsString, wchar_t wDelim1, wchar_t wDelim2);
}

#endif // _XPS_XPSLIB_UTILS_H