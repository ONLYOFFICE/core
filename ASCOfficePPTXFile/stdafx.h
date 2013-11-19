// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1

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
#include <atldefine.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <wingdi.h>

#define NODOCX
#define PPTX_DEF

#define PPT_DEF
#define ENABLE_PPT_TO_PPTX_CONVERT
#define _AVS_PPT_SHAPE_INCLUDE_
#define AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

//#pragma comment(lib, "PPTXFormat.lib")

using namespace ATL;
#include "../../AVSImageStudio3/AVSGraphics/Interfaces/AVSRenderer.h"

#import "../../../Redist/AVSMediaCore3.dll"								named_guids raw_interfaces_only rename_namespace("MediaCore"), exclude("tagRECT")
#import "../../../Redist/AVSImageStudio3.dll"							named_guids raw_interfaces_only rename_namespace("AVSImageStudio"), exclude("IAVSRenderer")
#import "../../../Redist/AVSOfficeStudio/AVSHTMLRenderer.dll"			named_guids raw_interfaces_only rename_namespace("HtmlRenderer"), exclude("IAVSRenderer")

#import "../../../AVS/Redist/AVSVideoFile3.dll"							named_guids raw_interfaces_only rename_namespace("VideoFile")
#import "../../../AVS/Redist/AVSGraphics.dll"							named_guids raw_interfaces_only rename_namespace("AVSGraphics"), exclude("IAVSRenderer")
#import "../../../AVS/Redist/AVSOfficeStudio/AVSFontConverter.dll"		named_guids raw_interfaces_only rename_namespace("Fonts")
#import "../../../AVS/Redist/AVSOfficeStudio/AVSOfficeDocxFile2.dll"	named_guids raw_interfaces_only rename_namespace("DocxFile2")

#import "../../../AVS/Sources/TeamlabOffice/trunk/wwwroot/Bin/XlsxSerializerCom.dll"	named_guids raw_interfaces_only rename_namespace("XlsxCom"), exclude("_IAVSOfficeFileTemplateEvents"), exclude("_IAVSOfficeFileTemplateEvents2")