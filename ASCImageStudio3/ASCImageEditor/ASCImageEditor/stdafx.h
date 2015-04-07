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

#define _CRT_SECURE_NO_WARNINGS
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

#include <atlcoll.h>

#include <gdiplus.h>
#include <gdiplustypes.h>


using namespace ATL;


#include "ATLDefine.h"
#include "../../ASCGraphics/Interfaces/BaseThread.h"


#import "../../../Redist/ASCMediaCore3.dll"					named_guids rename_namespace("MediaCore"),		raw_interfaces_only, exclude("tagRECT")
#import "../../../Redist/ASCImageStudio3.dll"					named_guids rename_namespace("ImageStudio3"),		raw_interfaces_only, exclude("tagRECT")
#import "../../../Redist/ASCVectorImage.dll"					named_guids rename_namespace("VectorImage"),		raw_interfaces_only, exclude("tagRECT")
#import "../../../Redist/ASCImageFile3.dll"					named_guids rename_namespace("ImageFile"),		raw_interfaces_only


#include <math.h>

#pragma comment(lib, "ippcorel.lib")
#pragma comment(lib, "ippiemerged.lib")
#pragma comment(lib, "ippimerged.lib")
#pragma comment(lib, "ippsmerged.lib")
#pragma comment(lib, "ippcvemerged.lib")
#pragma comment(lib, "ippcvmerged.lib")

#include <ippcore.h>
#include <ippdefs.h>
#include <ippi.h>
#include <ippcv.h>

#define IMAGE_EDITOR

#include "CommonHeader.h"
#include <xmlutils.h>

int BGRA_TransformResize_(const BYTE* pSrcPixels, int nWidth, int nHeight, BYTE* pDstPixels, long nNewWidth, long nNewHeight, long Color, long Type, double dAspect, double dCropZoom );

//#define  LOGGING
#include "Logger.h"