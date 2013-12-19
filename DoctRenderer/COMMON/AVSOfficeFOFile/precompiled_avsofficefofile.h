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
#define _WIN32_WINDOWS 0x0510 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0501	// Change this to the appropriate value to target IE 5.0 or later.
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

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "utils\stringutils.h"
//#include <boost/algorithm/string.hpp>

#include "config.h"

#include "xmllite.h"
#pragma comment(lib,"xmllite.lib")


#if (MS_XML_VERSION == 6)
#include "msxml6.h"
#define DOMDOCUMENT DOMDocument60
#define IXMLDOMDOCUMENT IXMLDOMDocument3
#elif (MS_XML_VERSION == 4)
#include "msxml2.h"
#define DOMDOCUMENT DOMDocument40
#define IXMLDOMDOCUMENT IXMLDOMDocument2
#else 
#error Invalid MS_XML_VERSION
#endif

#define CHECK_HR(HR) if FAILED(hr = (HR)) return hr;

#import "..\..\..\Redist\AVSMediaCore3.dll"									named_guids raw_interfaces_only rename_namespace("MediaCore"), exclude("tagRECT")

using namespace ATL;