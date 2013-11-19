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

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _CRT_SECURE_NO_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlcoll.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <gdiplus.h>

using namespace ATL;
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#define PPT_DEF
#define PPTX_DEF
#define ODP_DEF

#define _PRESENTATION_WRITER_

#include "../Common/OfficeFileTemplate.h"
#include "../../AVSImageStudio3/AVSGraphics/Interfaces/AVSRenderer.h"

#import "../../../../AVS/Redist/AVSMediaCore3.dll"				named_guids rename_namespace("MediaCore"), exclude("tagRECT")
#import "../../../../AVS/Redist/AVSImageFile3.dll"				named_guids rename_namespace("ImageFile")
#import "../../../../AVS/Redist/AVSVideoFile3.dll"				named_guids rename_namespace("VideoFile"), raw_interfaces_only
#import "../../../../AVS/Redist/AVSGraphics.dll"				named_guids rename_namespace("AVSGraphics"), raw_interfaces_only, exclude("IAVSRenderer")
#import "../../../../AVS/Redist/AVSOfficeStudio/AVSHTMLRenderer.dll"			named_guids rename_namespace("HTMLRenderer"), raw_interfaces_only, exclude("IAVSRenderer")

#import "../../../../AVS/Redist/AVSOfficeStudio\AVSOfficePPTFile.dll"	named_guids rename_namespace("PPTFile"),  raw_interfaces_only, exclude("IAVSOfficeFileTemplate", "_IAVSOfficeFileTemplateEvents", "_IAVSOfficeFileTemplateEvents2", "IAVSOfficeFile") 
#import "../../../../AVS/Redist/AVSOfficeStudio\AVSOfficePPTXFile.dll"	named_guids rename_namespace("PPTXFile"), raw_interfaces_only, exclude("IAVSOfficeFileTemplate", "_IAVSOfficeFileTemplateEvents", "_IAVSOfficeFileTemplateEvents2", "IAVSOfficeFile")
#import "../../../../AVS/Redist/AVSOfficeStudio\AVSOfficeOdpFile.dll"	named_guids rename_namespace("ODPFile"),  raw_interfaces_only, exclude("IAVSOfficeFileTemplate", "_IAVSOfficeFileTemplateEvents", "_IAVSOfficeFileTemplateEvents2", "IAVSOfficeFile") 
#import "../../../../AVS/Redist/AVSOfficeStudio\AVSOfficePDFWriter.dll"	named_guids rename_namespace("PDFFile"),  raw_interfaces_only, exclude("IAVSRenderer")

