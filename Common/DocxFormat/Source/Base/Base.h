#pragma once

#ifdef _WIN32
#pragma warning( disable : 4996 )


#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE	1
#endif

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE	1
#endif

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#ifndef _ATL_APARTMENT_THREADED
#define _ATL_APARTMENT_THREADED
#endif

#ifndef _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#endif

#ifndef _ATL_ALL_WARNINGS
// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS
#endif

//#include <windows.h>
//#include <atlbase.h>
//#include <atlcoll.h>
//#include <atlstr.h>
//#include <atltypes.h>

#include "../../../ASCUtils.h"

using namespace ATL;

#else
#include "ASCString.h"
#include "stdint.h"

typedef int BOOL;
static const BOOL TRUE = 1;
static const BOOL FALSE = 0;


typedef int HRESULT;
static const HRESULT S_OK = 0;
static const HRESULT S_FALSE = -1;

typedef unsigned char BYTE;
typedef unsigned short USHORT;
typedef short SHORT;
typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned int DWORD;
typedef unsigned long long ULONG64;
typedef long long LONG64;

typedef wchar_t WCHAR;
#endif // #ifdef _WIN32

#ifndef AVSINLINE
#if defined(_MSC_VER)
#define AVSINLINE __forceinline
#else
#define AVSINLINE inline
#endif
#endif
