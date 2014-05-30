#ifndef _Clipboard_CClipboardDataFetcherMacosx_
#define _Clipboard_CClipboardDataFetcherMacosx_

#ifdef __APPLE__

#include "IClipboardFetcher.h"

class CClipboardFetcherMacosx: public IClipboardFetcher
{
public:
	CClipboardFetcherMacosx(void);
	~CClipboardFetcherMacosx(void);
};

#endif // #ifdef __APPLE__
#endif // #ifndef _Clipboard_CClipboardDataFetcherMacosx_

