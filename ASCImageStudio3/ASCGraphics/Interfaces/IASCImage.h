#pragma once
#include "..\stdafx.h"

const long c_ag_ImageTypeBitmap			= 0;
const long c_ag_ImageTypeMetafile		= 1;

#include "ASCRenderer.h"
#include "IASCFontManager.h"

[object, uuid("73F78025-69A1-4094-8C53-6066930D49E3"), dual, pointer_default(unique)]
__interface IAVSImage : IASCGraphicsBase
{
	[id(100), propget] HRESULT Type([out, retval] LONG* lType);

	[id(101), propget] HRESULT Width([out, retval] LONG* lWidth);
	[id(101), propput] HRESULT Width([in] LONG lWidth);

	[id(102), propget] HRESULT Height([out, retval] LONG* lHeight);
	[id(102), propput] HRESULT Height([in] LONG lHeight);

	[id(201), propget] HRESULT DpiX([out, retval] double* dDpiX);
	[id(201), propput] HRESULT DpiX([in] double dDpiX);

	[id(202), propget] HRESULT DpiY([out, retval] double* dDpiY);
	[id(202), propput] HRESULT DpiY([in] double dDpiY);

	[id(300)]		   HRESULT Draw([in] IASCGraphicsBase* pGraphics);
	
	[id(301), propget] HRESULT FontManager([out, retval] IASCFontManager** ppManager);
	[id(301), propput] HRESULT FontManager([in] IASCFontManager* pManager);

	[id(400)]          HRESULT LoadFromFile([in] BSTR bsFilePath);
	[id(401)]          HRESULT DrawOnRenderer([in]IASCRenderer* pRenderer, [in] double dX, [in] double dY, [in] double dWidth, [in] double dHeight);
};