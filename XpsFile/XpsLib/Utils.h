#ifndef _XPS_XPSLIB_UTILS_H
#define _XPS_XPSLIB_UTILS_H

#include <string>
#include <vector>

namespace XmlUtils
{
	class CXmlLiteReader;
}

class IRenderer;

namespace XPS
{
	class CWStringBuffer;
	class CWString
	{
	public:
		CWString();
		CWString(const wchar_t* wsString);
		CWString(const CWString& wsString);
		CWString(wchar_t* wsString, bool bCopy, int nLen = -1);
		~CWString();
		void create(const wchar_t*, bool bCopy, int nLen = -1);
		void operator=(const wchar_t* wsString);
		void operator=(const CWString& wsString);
		bool operator<(const CWString& wsString) const;
		bool operator>(const CWString& wsString) const;
		bool operator==(const CWString& wsString) const;
		bool operator==(const wchar_t* wsString) const;
		unsigned int size() const;
		bool empty() const;
		wchar_t operator[](const unsigned int& unIndex) const;
		const wchar_t* c_str() const;
		void clear();

	private:

		void*        m_pBuffer;
		unsigned int m_unLen;
		bool         m_bOwnBuffer;
	};


	bool   IsAlpha(wchar_t wChar);
	double GetDouble(const std::wstring& wsString);
	int    GetInteger(const std::wstring& wsString);
	bool   GetBool(const std::wstring& wsString);
	void   GetBgra(const std::wstring& wsString, int& nBgr, int& nAlpha);

	unsigned char GetCapStyle(const wchar_t* wsCapStyle);

	std::wstring NormalizePath(const std::wstring& wsPath);
	std::wstring GetPath(const std::wstring& wsPath);
	std::wstring GetFileName(const std::wstring& wsPath);
	std::wstring GetFileExtension(const std::wstring& wsPath);
	std::wstring RemoveNamespace(const std::wstring& wsString);

	std::vector<std::vector<std::wstring>> Split(const std::wstring& wsString, wchar_t wDelim1, wchar_t wDelim2);

	void ReadAttribute(XmlUtils::CXmlLiteReader& oReader, const wchar_t* wsAttrName, std::wstring& wsAttr);
	bool VmlToRenderer(const wchar_t* wsString, IRenderer* pRenderer);
}

#endif // _XPS_XPSLIB_UTILS_H