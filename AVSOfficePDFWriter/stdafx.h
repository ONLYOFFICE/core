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
#define WINVER 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0500 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0500	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#pragma warning(disable: 4018 4996 4101 4305 4244)
// 4101 надо будет убрать из списка, временно поставил

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

//#ifdef _VS_2005
#pragma comment (lib, "..\\..\\AVSImageStudio3\\AVSGraphics\\Objects\\Font\\FreeType\\freetype242_vs2005.lib")
//#endif
//
//#ifdef _VS_2008
//#pragma comment (lib, "FreeType\\freetype2312.lib")
//#endif

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlsafe.h>
#include <atlcoll.h>
#include <atlstr.h>
#include <atldefine.h>

#import "../../../../AVS/Redist/AVSMediaCore3.dll"				named_guids rename_namespace("MediaCore"), exclude("tagRECT")
#import "../../../../AVS/Redist/AVSMediaFormatSettings3.dll"	named_guids rename_namespace("MediaFormat"), exclude("tagRECT")
#import "../../../../AVS/Redist/AVSImageStudio3.dll"            named_guids rename_namespace("ImageStudio")
#import "../../../../AVS/Redist/AVSImageJbig2.dll"              named_guids rename_namespace("JBig2")
#import "../../../../AVS/Redist/AVSImageJpeg2000.dll"           named_guids rename_namespace("Jpx")
#import "../../../../AVS/Redist/AVSImageFile3.dll"              named_guids rename_namespace("ImageFile")
#import "../../../../AVS/Redist/AVSGraphics.dll"                named_guids rename_namespace("AVSGraphics")

using namespace ATL;