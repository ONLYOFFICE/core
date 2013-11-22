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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlcoll.h>
#include <atlfile.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <gdiplus.h>
#include <ippcore.h>
#include <ippdefs.h>
#include <ippi.h>
#include <ippcv.h>
#include <shlwapi.h>


using namespace ATL;


#import "../../../../AVS/Redist/AVSImageStudio3.dll"				named_guids rename_namespace("ImageStudio")
#import "../../../../AVS/Redist/AVSMediaCore3.dll"					named_guids rename_namespace("MediaCore"), exclude("tagRECT")
#import "../../../../AVS/Redist/AVSImageFile3.dll"					named_guids rename_namespace("ImageFile")
#import "../../../Redist/AVSOfficeStudio/AVSOfficePDFFile.dll"		named_guids rename_namespace("OfficePDFFile")
#import "../../../../AVS/Redist/AVSImageVideoFile3.dll"				named_guids rename_namespace("ImageVideoFile"), raw_interfaces_only
#import "../../../Redist/AVSMediaFormatSettings3.dll"				named_guids rename_namespace("MediaFormatSettings"),raw_interfaces_only

