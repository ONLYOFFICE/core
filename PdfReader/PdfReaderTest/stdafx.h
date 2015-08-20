// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/PdfReader.lib")
#pragma comment(lib, "../../SDK/lib/win_64/DEBUG/graphics.lib")
#pragma comment(lib, "../../PdfWriter/x64/Debug/PdfWriter.lib")
#else
#pragma comment(lib, "../x64/Release/PdfReader.lib")
#pragma comment(lib, "../../SDK/lib/win_64/graphics.lib")
#pragma comment(lib, "../../PdfWriter/x64/Release/PdfWriter.lib")
#endif

// TODO: reference additional headers your program requires here
