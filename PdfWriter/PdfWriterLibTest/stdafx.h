// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#if defined(_WIN64)
#if defined(_DEBUG)
#pragma comment(lib, "../../build/lib/win_64/DEBUG/graphics.lib")
#pragma comment(lib, "../../build/lib/win_64/DEBUG/kernel.lib")
#pragma comment(lib, "../../build/lib/win_64/DEBUG/UnicodeConverter.lib")
#else
#pragma comment(lib, "../../build/lib/win_64/graphics.lib")
#pragma comment(lib, "../../build/lib/win_64/kernel.lib")
#pragma comment(lib, "../../build/lib/win_64/UnicodeConverter.lib")
#endif
#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)
#if defined(_DEBUG)
#pragma comment(lib, "../../build/lib/win_32/DEBUG/graphics.lib")
#pragma comment(lib, "../../build/lib/win_32/DEBUG/kernel.lib")
#pragma comment(lib, "../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
#else
#pragma comment(lib, "../../build/lib/win_32/graphics.lib")
#pragma comment(lib, "../../build/lib/win_32/kernel.lib")
#pragma comment(lib, "../../build/lib/win_32/UnicodeConverter.lib")
#endif
#pragma comment(lib, "../../build/bin/icu/win_32/icuuc.lib")
#endif
#pragma comment(lib, "Ws2_32.lib")

// TODO: reference additional headers your program requires here
