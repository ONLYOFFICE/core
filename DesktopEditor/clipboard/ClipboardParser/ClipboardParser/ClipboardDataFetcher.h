#ifndef _Clipboard_CClipboardDataFetcher_
#define _Clipboard_CClipboardDataFetcher_

#include "IClipboardFetcher.h"

// Windows MSDN
// For MingW on Windows
#if defined (_WIN32) || defined (__MINGW32__)
#include  "ClipboardFetcherWin.h"
#endif


// Macos X
#ifdef __APPLE__
#include  "ClipboardFetcherMacosx.h"
#endif

// GCC on Linux
#ifdef __GNUC__
#endif

class CClipboardDataFetcher
{
public:
	IClipboardFetcher* createClipboardFetcher ();
public:
	CClipboardDataFetcher(void);
	~CClipboardDataFetcher(void);
};
#endif // #ifndef _Clipboard_CClipboardDataFetcher_
