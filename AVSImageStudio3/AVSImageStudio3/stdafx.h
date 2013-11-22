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
#define _CRT_SECURE_NO_DEPRECATE

#define GDIPVER 0x0100

// include libraries
#pragma comment(lib, "ippcorel.lib")
#pragma comment(lib, "ippiemerged.lib")
#pragma comment(lib, "ippimerged.lib")
#pragma comment(lib, "ippsemerged.lib")
#pragma comment(lib, "ippsmerged.lib")
#pragma comment(lib, "ippcvemerged.lib")
#pragma comment(lib, "ippcvmerged.lib")
// #pragma comment(lib, "ippi20.lib")

// include headers
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlfile.h>
#include <atlhost.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <gdiplus.h>
#include <ippcore.h>
#include <ippdefs.h>
#include <ippi.h>
#include <ippcv.h>
#include <shlwapi.h>

// include namespaces
using namespace ATL;
using namespace Gdiplus;

#import "../../../../AVS/Redist/AVSImageFile3.dll"				named_guids rename_namespace("ImageFile")
//#import "../../../../AVS/Redist/AVSImageTransform3.dll"			named_guids rename_namespace("ImageTransform")
#import "../../../../AVS/Redist/AVSImagePaint3.dll"				named_guids rename_namespace("ImagePaint")
//#import "../../../../AVS/Redist/AVSImageCompose3.dll"			named_guids rename_namespace("ImageCompose")
#import "../../../../AVS/Redist/AVSMediaCore3.dll"				named_guids rename_namespace("MediaCore"), exclude("tagRECT")
#import "../../../../AVS/Redist/AVSMediaFormatSettings3.dll"	named_guids rename_namespace("MediaFormat"), exclude("tagRECT")
#import "../../../../AVS/Redist/AVSUniversalVideoConverter.dll" named_guids rename_namespace("FileCache")
#import "../../../../AVS/Redist/AVSImageRaw3.dll"	            named_guids rename_namespace("ImageRaw")
#import "../../../../AVS/Redist/AVSImageJpeg2000.dll"           named_guids rename_namespace("Jpeg2000")
#import "../../../../AVS/Redist/AVSGraphics.dll"				named_guids rename_namespace("AVSGraphics")
#import "../../../../AVS/Redist/AVSSWFFile3.dll"				named_guids rename_namespace("SWF")
