// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>

#ifdef _DEBUG
#define _RAW_TRACE(...) do {AtlTrace(__VA_ARGS__);} while(0);
#else
#define _RAW_TRACE(...)
#endif

// TODO: reference additional headers your program requires here
