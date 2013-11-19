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

#include "atldefine.h"

using namespace ATL;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#import "../../../Redist/AVSOfficeStudio/AVSOfficeDocFile.dll"						rename_namespace("AVSOfficeDocFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeDocxFile.dll"						rename_namespace("AVSOfficeDocxFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeHtmlFile.dll"						rename_namespace("AVSOfficeHtmlFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeOdtFile.dll"						rename_namespace("AVSOfficeOdtFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeRtfFile.dll"						rename_namespace("AVSOfficeRtfFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeTxtFile.dll"						rename_namespace("AVSOfficeTxtFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeImageFile.dll"					rename_namespace("AVSOfficeImageFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficePDFFile.dll"						rename_namespace("AVSOfficePDFFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeXlsFile.dll"						rename_namespace("AVSOfficeXlsFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeEpubFile.dll"						rename_namespace("OfficeEpub"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeDjVuFile.dll"						rename_namespace("OfficeDjVu"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeFB2File.dll"						rename_namespace("OfficeFB2"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSSWFRenderer.dll"						rename_namespace("FlashStudio"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeWSHtmlFile.dll"					rename_namespace("WSHtmlFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeDocxRenderer.dll"					rename_namespace("DocxRenderer"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSHTMLRenderer.dll"						rename_namespace("HTMLRenderer"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeXPSFile.dll"						rename_namespace("OfficeXPS"), raw_interfaces_only 

#import "../../../Redist/AVSGraphics.dll"											rename_namespace("AVSGraphics"), raw_interfaces_only 
//#import "../../../Redist/AVSOfficeStudio/AVSOfficeEditor.dll"						rename_namespace("AVSOfficeEditor"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficePPTXFile.dll"						rename_namespace("AVSOfficePPTXFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficePDFWriter.dll"					rename_namespace("AVSOfficePDFWriter"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeFOFile.dll"						rename_namespace("AVSOfficeFOFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficePPTFile.dll"						rename_namespace("AVSOfficePPTFile"), raw_interfaces_only 
#import "../../../Redist/AVSOfficeStudio/AVSOfficeOdpFile.dll"						rename_namespace("AVSOfficeOdpFile"), raw_interfaces_only 
#import "../../../Redist/AVSMediaCore3.dll"											named_guids rename_namespace("MediaCore"), exclude("tagRECT")
#import "../../../Redist/AVSMediaFormatSettings3.dll"								named_guids rename_namespace("MediaFormat"), exclude("tagRECT")
#import "../../../Redist/AVSImageStudio3.dll"										rename_namespace("ImageStudio")
#import "../../../Redist/AVSOfficeStudio/AVSEBOOKWriter.dll"						rename_namespace("EBOOKWriter"), raw_interfaces_only 

#define _USE_MATH_DEFINES
