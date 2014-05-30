#include "ClipboardDataFetcher.h"


IClipboardFetcher* CClipboardDataFetcher::createClipboardFetcher ()
{
	IClipboardFetcher* pFetcher = NULL;

	// Windows
	#if defined (_WIN32) || defined (__MINGW32__)
	pFetcher = new CClipboardFetcherWin ();
	#endif 

	// Macos X
	#ifdef __APPLE__
	pFetcher = new CClipboardFetcherMacosx ();
	#endif

	// GCC on Linux
	#ifdef __GNUC__
	pFetcher = new CClipboardFetcherLinux ();
	#endif

	return pFetcher;
}

CClipboardDataFetcher::CClipboardDataFetcher(void)
{
}

CClipboardDataFetcher::~CClipboardDataFetcher(void)
{
}
