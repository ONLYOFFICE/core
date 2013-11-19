// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

#define HAS_WCTYPE			1
#define THREADMODEL			0
#define DO_CHANGELOCALE		0
#define WIN32_MONITOR
#define NEED_JPEG_DECODER
#define WIN32_JPEG
#define LIBDJVU_STATIC

#pragma comment(lib, "gdiplus.lib")

#ifdef _DEBUG
#pragma comment(lib, "Debug\\libdjvu.lib")
#else
#pragma comment(lib, "Release\\libdjvu.lib")
#endif

#import "..\..\..\Redist\AVSMediaCore3.dll"									named_guids raw_interfaces_only rename_namespace("MediaCore"), exclude("tagRECT")
#import "..\..\..\Redist\AVSMediaFormatSettings3.dll"						named_guids raw_interfaces_only rename_namespace("MediaFormat"), exclude("tagRECT")
#import "..\..\..\Redist\AVSImageStudio3.dll"								named_guids raw_interfaces_only rename_namespace("ImageStudio")
//#import "..\..\..\Redist\AVSOfficeStudio\AVSOfficeEditor.dll"				named_guids raw_interfaces_only rename_namespace("OfficeEditor")
#import "..\..\..\Redist\AVSGraphics.dll"									named_guids raw_interfaces_only rename_namespace("AVSGraphics")


using namespace ATL;