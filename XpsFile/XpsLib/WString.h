#ifndef _XPS_XPSLIB_WSTRING_H
#define _XPS_XPSLIB_WSTRING_H

#include <vector>

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

		std::vector<CWString> split(wchar_t wChar, bool bCopy = false);

	private:

		void*        m_pBuffer;
		unsigned int m_unLen;
		bool         m_bOwnBuffer;
	};
}

#endif //_XPS_XPSLIB_WSTRING_H