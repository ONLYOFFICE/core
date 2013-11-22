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
#include "../../Common/atldefine.h"

#include "Gdiplus.h"
#pragma comment(lib, "gdiplus.lib")

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#pragma comment (lib, "Objects/Font/FreeType/freetype242_vs2005.lib")
//#pragma comment (lib, "cximage/zlib/Release/zlib.lib")

#ifdef _DEBUG
#pragma comment (lib, "Debug/agg2d.lib")
#else
#pragma comment (lib, "Release/agg2d.lib")
#endif

#define _AVS_GRAPHICS_
using namespace ATL;

#include "../../Common/ASCUtils.h"
#include "../../Common/Config.h"

#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
#import "../../Redist/OfficeCore.dll"					named_guids raw_interfaces_only rename_namespace("OfficeCore")

namespace MediaCore
{
	typedef OfficeCore::IUncompressedFrame IAVSUncompressedVideoFrame;
	const GUID CLSID_CUncompressedVideoFrame = OfficeCore::CLSID_CUncompressedFrame;
	const GUID IID_IUncompressedVideoFrame = OfficeCore::IID_IUncompressedFrame;
}

#else
#import "../../Redist/ASCMediaCore3.dll"				named_guids raw_interfaces_only rename_namespace("MediaCore")
#import "../../Redist/ASCImageStudio3.dll"				named_guids raw_interfaces_only rename_namespace("ImageStudio")
#endif // BUILD_CONFIG_OPENSOURCE_VERSION

namespace ImageStudio
{
	static IUnknown* ISLoadImage(BSTR filename)
	{
#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
		OfficeCore::IImageGdipFilePtr pImageFile;
		pImageFile.CreateInstance(OfficeCore::CLSID_CImageGdipFile);

		pImageFile->OpenFile(filename);

		IUnknown* punkFrame = NULL;
		pImageFile->get_Frame(&punkFrame);
		return punkFrame;
#else
		ImageStudio::IImageTransforms* pTransforms = NULL;
		CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)&pTransforms); 

		if (NULL == pTransforms)
			return NULL;

		IUnknown* pResult = NULL;

		CStringW strXml = L"<ImageFile-LoadImage sourcepath='";
		strXml += CStringW(filename);
		strXml += L"'/>";

		VARIANT_BOOL vbRes = VARIANT_FALSE;
		BSTR bsXml = strXml.AllocSysString();
		pTransforms->SetXml(bsXml, &vbRes);
		pTransforms->Transform(&vbRes);

		SysFreeString(bsXml);

		VARIANT var;
		pTransforms->GetResult(0, &var);

		pResult = var.punkVal;
		var.punkVal = NULL;

		RELEASEINTERFACE(pTransforms);

		return pResult;
#endif // BUILD_CONFIG_OPENSOURCE_VERSION
	}

	static BOOL SaveImageAsPNG(IUnknown* punkFrame, CString file)
	{
#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
		OfficeCore::IImageGdipFilePtr pImageFile;
		pImageFile.CreateInstance(OfficeCore::CLSID_CImageGdipFile);

		pImageFile->put_Frame(punkFrame);
		BSTR bsFile = file.AllocSysString();
		HRESULT hr = pImageFile->SaveFile(bsFile, 4); 
		
		return (hr == S_OK);		
#else
		ImageStudio::IImageTransforms* pTransform = NULL;
		CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

		VARIANT var;
		var.vt = VT_UNKNOWN;
		var.punkVal = punkFrame;
		pTransform->SetSource(0, var);

		CString strXml = _T("<transforms><ImageFile-SaveAsPng destinationpath=\"") + file + _T("\" format=\"888\"/></transforms>");

		VARIANT_BOOL vbSuccess = VARIANT_FALSE;
		BSTR bsXml = strXml.AllocSysString();
		pTransform->SetXml(bsXml, &vbSuccess);
		SysFreeString(bsXml);

		pTransform->Transform(&vbSuccess);

		RELEASEINTERFACE(pTransform);

		return TRUE;
#endif // BUILD_CONFIG_OPENSOURCE_VERSION		
	}
}
