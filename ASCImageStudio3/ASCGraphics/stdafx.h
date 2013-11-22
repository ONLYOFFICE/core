// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once
#define _CRT_SECURE_NO_DEPRECATE	1
#define _CRT_NONSTDC_NO_DEPRECATE	1

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
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
//#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlcoll.h>
#include <atldefine.h>

#include "Gdiplus.h"
#pragma comment(lib, "gdiplus.lib")

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

//#ifdef _VS_2005
#pragma comment (lib, "Objects\\Font\\FreeType\\freetype242_vs2005.lib")
//#pragma comment (lib, "Objects\\Font\\FreeType\\freetype242_vs2005.lib")
//#endif
//
//#ifdef _VS_2008
//#pragma comment (lib, "FreeType\\freetype2312.lib")
//#endif

//#define AVS_CXIMAGE_USE
#define CXIMAGE_SUPPORT_ALPHA

#ifdef AVS_CXIMAGE_USE
#pragma comment (lib, "Expat\\lib\\libexpat.lib")

#pragma comment (lib, "cximage\\zlib\\Release\\zlib.lib")
#pragma comment (lib, "cximage\\tiff\\Release\\Tiff.lib")
#pragma comment (lib, "cximage\\png\\Release\\png.lib")
#pragma comment (lib, "cximage\\mng\\Release\\mng.lib")
#pragma comment (lib, "cximage\\jpeg\\Release\\Jpeg.lib")
#pragma comment (lib, "cximage\\jbig\\Release\\jbig.lib")
#pragma comment (lib, "cximage\\jasper\\Release\\jasper.lib")
#pragma comment (lib, "cximage\\raw\\Release\\libdcr.lib")

#pragma comment (lib, "cximage\\CxImage\\Release\\cximage.lib")
#endif
#pragma comment (lib, "cximage\\zlib\\Release\\zlib.lib")

#ifdef _DEBUG
#pragma comment (lib, "Debug\\agg2d.lib")
#else
#pragma comment (lib, "Release\\agg2d.lib")
#endif

#define _AVS_GRAPHICS_
using namespace ATL;

#include "..\..\AVSVideoStudio3\Common\AVSUtils.h"

#import "..\..\..\Redist\AVSMediaCore3.dll"									named_guids raw_interfaces_only rename_namespace("MediaCore"), exclude("tagRECT")
#import "..\..\..\Redist\AVSMediaFormatSettings3.dll"						named_guids raw_interfaces_only rename_namespace("MediaFormat"), exclude("tagRECT")
#import "..\..\..\Redist\AVSImageStudio3.dll"								named_guids raw_interfaces_only rename_namespace("ImageStudio")
#import "..\..\..\Redist\AVSOfficeStudio\AVSOfficeUniversalConverter.dll"	named_guids raw_interfaces_only rename_namespace("DocConverter")
