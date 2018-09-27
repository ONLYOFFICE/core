// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")

#ifdef _DEBUG
#pragma comment(lib, "CryptoPPLib.lib")
#pragma comment(lib, "UnicodeConverter.lib")
#pragma comment(lib, "graphics.lib")
#pragma comment(lib, "kernel.lib")
#pragma comment(lib, "Ws2_32.lib")
#else
#pragma comment(lib, "../../build/lib/win_64/CryptoPPLib.lib")
#pragma comment(lib, "../../build/lib/win_64/UnicodeConverter.lib")
#pragma comment(lib, "../../build/lib/win_64/graphics.lib")
#pragma comment(lib, "../../build/lib/win_64/kernel.lib")
#pragma comment(lib, "Ws2_32.lib")
#endif

// TODO: reference additional headers your program requires here
