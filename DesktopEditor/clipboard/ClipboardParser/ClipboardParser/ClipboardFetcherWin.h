#ifndef _Clipboard_CClipboardDataFetcherWin_
#define _Clipboard_CClipboardDataFetcherWin_

#if defined (_WIN32) || defined (__MINGW32__)

#include <string>
#include "IClipboardFetcher.h"

#include "Windows.h"

class CClipboardFetcherWin: public IClipboardFetcher
{
public:
	bool getTextContent (std::wstring &aContent);
	bool getUnicodeTextContent (std::wstring &aContent);
	bool getHtmlContent (std::wstring &aContent);
	bool setHtmlContent (const std::wstring &aContent);

public:
	CClipboardFetcherWin(void);
	~CClipboardFetcherWin(void);
private:
	bool extractHtmlClipboard (const std::string &sText, std::wstring &aContent);
private:
	static int m_nClipboardFormatID;
};

#endif // #if defined (_WIN32) || defined (__MINGW32__)
#endif // #ifndef _Clipboard_CClipboardDataFetcherWin_
