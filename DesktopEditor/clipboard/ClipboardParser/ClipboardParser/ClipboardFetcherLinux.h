#ifndef _Clipboard_CClipboardDataFetcherLinux_
#define _Clipboard_CClipboardDataFetcherLinux_

// GCC on Linux
#ifdef __GNUC__

#include "IClipboardFetcher.h"

class CClipboardFetcherLinux: public IClipboardFetcher
{
public:
	CClipboardFetcherLinux(void);
	~CClipboardFetcherLinux(void);
};

#endif // #ifdef __GNUC__
#endif // #ifndef _Clipboard_CClipboardDataFetcherLinux_
