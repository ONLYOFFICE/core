// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#pragma warning(disable: 4996)

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlcoll.h>

#define _USE_LIBXML2_READER_

#pragma comment(lib, "libxml2.lib")

using namespace ATL;

#include "../ASCImageStudio3/ASCGraphics/Interfaces/ASCRenderer.h"

#import "..\Redist\ASCGraphics.dll"				named_guids raw_interfaces_only rename_namespace("ASCGraphics"), exclude("IASCRenderer")
#import "..\Redist\ASCOfficePDFWriter.dll"		named_guids raw_interfaces_only rename_namespace("PDFWriter"), exclude("IASCRenderer")