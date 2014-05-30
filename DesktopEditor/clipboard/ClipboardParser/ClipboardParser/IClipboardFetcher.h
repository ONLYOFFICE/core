#ifndef _Clipboard_IClipboardDataFetcher_
#define _Clipboard_IClipboardDataFetcher_

#define CLIPBOARD_FORMAT_TEXT	0x000001
#define CLIPBOARD_FORMAT_HTML	0x000002

// interface for clipboard fetcher. it's a parent for platform-dependent classes
#include <string>

class IClipboardFetcher
{
public:
	virtual bool getTextContent (std::wstring &aContent) = 0;
	virtual bool getUnicodeTextContent (std::wstring &aContent) = 0;
	virtual bool getHtmlContent (std::wstring &aContent) = 0;
	virtual bool setHtmlContent (const std::wstring &aContent) = 0;
};

#endif // #ifndef _Clipboard_IClipboardDataFetcher_