#include "ClipboardFetcherWin.h"
#include <vector>
#include <map>

#if defined (_WIN32) || defined (__MINGW32__)

#pragma comment(lib, "User32.lib") 


int CClipboardFetcherWin::m_nClipboardFormatID = 0;

CClipboardFetcherWin::CClipboardFetcherWin(void)
{
	if (0 == m_nClipboardFormatID) 
		m_nClipboardFormatID = RegisterClipboardFormatW (L"HTML Format");
}

CClipboardFetcherWin::~CClipboardFetcherWin(void)
{
}

bool CClipboardFetcherWin::getUnicodeTextContent (std::wstring &aContent)
{
	bool bResult = false;

	if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) 
		return bResult;

	BOOL bClipboardOpened = OpenClipboard (NULL);

	if (0 == bClipboardOpened)
		return bResult;


	HANDLE hglb = GetClipboardData(CF_UNICODETEXT); 
	if (hglb != NULL)
	{
		LPWSTR pText = (LPWSTR) GlobalLock(hglb);
		std::wstring sText = pText;
		aContent.assign (sText.begin(), sText.end());
		GlobalUnlock(hglb);
		
		bResult = true;
	}
	CloseClipboard ();

	return bResult;
}
bool CClipboardFetcherWin::getTextContent (std::wstring &aContent)
{
	bool bResult = false;

	if (!IsClipboardFormatAvailable(CF_TEXT)) 
		return bResult;

	BOOL bClipboardOpened = OpenClipboard (NULL);

	if (0 == bClipboardOpened)
		return bResult;


	HANDLE hglb = GetClipboardData(CF_TEXT); 
	if (hglb != NULL)
	{
		LPSTR pText = (LPSTR) GlobalLock(hglb);
		std::string sText = pText;
		aContent.assign (sText.begin(), sText.end());
		GlobalUnlock(hglb);
		
		bResult = true;
	}
	CloseClipboard ();

	return bResult;
}

bool CClipboardFetcherWin::getHtmlContent (std::wstring &aContent)
{
	bool bResult = false;

	if (0 == m_nClipboardFormatID || !IsClipboardFormatAvailable(m_nClipboardFormatID)) 
		return bResult;

	BOOL bClipboardOpened = OpenClipboard (NULL);

	if (0 == bClipboardOpened)
		return bResult;

	/*
	std::vector<UINT> vFormats;
	UINT nFormats = EnumClipboardFormats (0);
	while (0 != nFormats)
	{
		vFormats.push_back(nFormats);
		nFormats = EnumClipboardFormats (nFormats);
		//unsigned int dwError = GetLastError();
	}
	CloseClipboard ();
	


	bClipboardOpened = OpenClipboard (NULL);

	if (0 == bClipboardOpened)
		return bResult;
		*/

	std::string sText;
	HANDLE hglb = GetClipboardData(m_nClipboardFormatID); 
    if (hglb != NULL) 
    { 
		LPSTR pText = (LPSTR) GlobalLock(hglb);
		sText = pText;
		GlobalUnlock(hglb);

		
		bResult = true;
	}

	CloseClipboard ();

	if (bResult)
	{
		bResult &= extractHtmlClipboard (sText, aContent);
	}

	return bResult;
}

bool CClipboardFetcherWin::extractHtmlClipboard (const std::string &sText, std::wstring &aContent)
{
	bool bResult = true;
	try 
	{
		//std::map<std::string, std::string> aParameters;

		int nStartFragmentOffset = 0;
		int nEndFragmentOffset = 0;

		std::string::size_type nStartFragmentPos = sText.find ("StartFragment", 0);
		if (nStartFragmentPos >= 0)
		{
			std::string::size_type nStartFragmentNewLinePos = sText.find ("\n", nStartFragmentPos);
			std::string sStartFragmentText = sText.substr (nStartFragmentPos, nStartFragmentNewLinePos - nStartFragmentPos);
			std::string::size_type nSeparatorPos = sStartFragmentText.find (":", 0);
			if (nSeparatorPos >= 0)
			{
				std::string sStartFragmentOffset = sStartFragmentText.substr (nSeparatorPos + 1);
				nStartFragmentOffset = atoi (sStartFragmentOffset.c_str());
			}
			if (0 == nStartFragmentOffset)
				return false;
		}

		std::string::size_type nEndFragmentPos = sText.find ("EndFragment", nStartFragmentPos);
		if (nEndFragmentPos >= 0)
		{
			std::string::size_type nEndFragmentNewLinePos = sText.find ("\n", nEndFragmentPos);
			std::string sEndFragmentText = sText.substr (nEndFragmentPos, nEndFragmentNewLinePos - nEndFragmentPos);
			std::string::size_type nSeparatorPos = sEndFragmentText.find (":", 0);
			if (nSeparatorPos >= 0)
			{
				std::string sEndFragmentOffset = sEndFragmentText.substr (nSeparatorPos + 1);
				nEndFragmentOffset = atoi (sEndFragmentOffset.c_str());
			}
			if (0 == nEndFragmentOffset)
				return false;
		}
		std::string sHtmlContent = sText.substr (nStartFragmentOffset, nEndFragmentOffset - nStartFragmentOffset);
		// convert UTF-8 to UTF-16
		int nWideStringLength = MultiByteToWideChar (CP_UTF8, 0, sHtmlContent.c_str(), -1, NULL, 0);
		
		WCHAR *pWideStringBuffer = new WCHAR [nWideStringLength + 1];
		pWideStringBuffer [nWideStringLength] = 0x00;

		int nConversionResult = MultiByteToWideChar (CP_UTF8, 0, sHtmlContent.c_str(), -1, pWideStringBuffer, nWideStringLength);
		if (0 == nConversionResult || ERROR_NO_UNICODE_TRANSLATION == nConversionResult)
			return false;

		aContent = L"<body>";
		aContent.append (pWideStringBuffer);
		aContent += L"</body>";
		delete [] pWideStringBuffer;
	}
	catch (...)
	{
		bResult = false;
	}
	return bResult;
}

bool CClipboardFetcherWin::setHtmlContent (const std::wstring &aContent)
{
	bool bResult = false;
	return bResult;
}

#endif // #if defined (_WIN32) || defined (__MINGW32__)