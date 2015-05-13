// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/PdfReader.lib")
#pragma comment(lib, "../../DesktopEditor/Qt_build/graphics/project/debug/graphics.lib")
#else
#pragma comment(lib, "../x64/Release/PdfReader.lib")
#pragma comment(lib, "../../DesktopEditor/Qt_build/graphics/project/release/graphics.lib")
#endif

// TODO: reference additional headers your program requires here
