// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _CRT_SECURE_NO_DEPRECATE
#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define USE_ATL_CSTRING

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

using namespace ATL;

#import "../../../Redist/AVSOfficeStudio/AVSOfficeDocxRenderer.dll"			raw_interfaces_only rename_namespace("DocxRenderer")
#import "../../../Redist/AVSOfficeStudio/AVSOfficeUtils.dll"				raw_interfaces_only rename_namespace("OfficeUtils")
#import "../../../Redist/AVSOfficeStudio/AVSOfficeHtmlFile.dll"				raw_interfaces_only rename_namespace("OfficeHtmlFile")
#import "../../../Redist/AVSGraphics.dll"									raw_interfaces_only rename_namespace("AVSGraphics")
#import "../../../Redist/AVSImageFile3.dll"									raw_interfaces_only rename_namespace("ImageFile")
#import "../../../Redist/AVSMediaCore3.dll"			rename_namespace("AVSMediaCore3"), exclude("tagRECT")
