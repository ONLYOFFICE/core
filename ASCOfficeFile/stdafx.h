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
#pragma warning(disable: 4244 4267 4683)
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

#include "../Common/atldefine.h"

using namespace ATL;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#import "../Redist/ASCOfficeDocFile.dll"						rename_namespace("AVSOfficeDocFile"), raw_interfaces_only 
#import "../Redist/ASCOfficeDocxFile.dll"						rename_namespace("AVSOfficeDocxFile"), raw_interfaces_only 
#import "../Redist/ASCOfficeHtmlFile.dll"						rename_namespace("AVSOfficeHtmlFile"), raw_interfaces_only 
#import "../Redist/ASCOfficeOdfFileW.dll"						rename_namespace("AVSOfficeOdfFileW"), raw_interfaces_only 
#import "../Redist/ASCOfficeRtfFile.dll"						rename_namespace("AVSOfficeRtfFile"), raw_interfaces_only 
#import "../Redist/ASCOfficeTxtFile.dll"						rename_namespace("AVSOfficeTxtFile"), raw_interfaces_only 
#import "../Redist/ASCOfficePDFFile.dll"						rename_namespace("AVSOfficePDFFile"), raw_interfaces_only 
#import "../Redist/ASCOfficeXlsFile.dll"						rename_namespace("AVSOfficeXlsFile"), raw_interfaces_only 
#import "../Redist/ASCOfficeEpubFile.dll"						rename_namespace("OfficeEpub"), raw_interfaces_only 
#import "../Redist/ASCOfficeDjVuFile.dll"						rename_namespace("OfficeDjVu"), raw_interfaces_only 
#import "../Redist/ASCOfficeFB2File.dll"						rename_namespace("OfficeFB2"), raw_interfaces_only 
#import "../Redist/ASCOfficeWSHtmlFile.dll"						rename_namespace("WSHtmlFile"), raw_interfaces_only 
#import "../Redist/ASCOfficeDocxRenderer.dll"					rename_namespace("DocxRenderer"), raw_interfaces_only 
#import "../Redist/ASCHTMLRenderer.dll"							rename_namespace("HTMLRenderer"), raw_interfaces_only 
#import "../Redist/ASCOfficeXPSFile.dll"						rename_namespace("OfficeXPS"), raw_interfaces_only 

#import "../Redist/ASCGraphics.dll"								rename_namespace("AVSGraphics"), raw_interfaces_only 
#import "../Redist/ASCOfficePPTXFile.dll"						rename_namespace("AVSOfficePPTXFile"), raw_interfaces_only 
#import "../Redist/ASCOfficePDFWriter.dll"						rename_namespace("AVSOfficePDFWriter"), raw_interfaces_only 
#import "../Redist/ASCOfficePPTFile.dll"						rename_namespace("AVSOfficePPTFile"), raw_interfaces_only 
#import "../Redist/ASCOfficeOdfFile.dll"						rename_namespace("AVSOfficeOdfFile"), raw_interfaces_only 
#import "../Redist/ASCMediaCore3.dll"							named_guids rename_namespace("MediaCore"), exclude("tagRECT")
#import "../Redist/ASCMediaFormatSettings3.dll"					named_guids rename_namespace("MediaFormat"), exclude("tagRECT")
#import "../Redist/ASCImageStudio3.dll"							rename_namespace("ImageStudio")
#import "../Redist/ASCEBOOKWriter.dll"							rename_namespace("EBOOKWriter"), raw_interfaces_only 

#define _USE_MATH_DEFINES
