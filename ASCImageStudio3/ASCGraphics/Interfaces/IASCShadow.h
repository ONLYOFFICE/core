#pragma once
#include "stdafx.h"

#include "IASCGraphics.h"

[object, uuid("E7F899B8-B190-431c-AB24-BC3BC630084C"), dual, pointer_default(unique)]
__interface IAVSShadow : IAVSGraphicsBase
{
	[id(100), propget]	HRESULT Xml([out, retval] BSTR* bstrXML);
	[id(100), propput]	HRESULT Xml([in] BSTR bstrXML);
	
	// general ------------------------------------------------------------
	[id(101), propget]	HRESULT Visible([out, retval] VARIANT_BOOL* vbVisible);
	[id(101), propput]	HRESULT Visible([in] VARIANT_BOOL vbVisible);

	[id(102), propget]	HRESULT Color([out, retval] LONG* lColor);
	[id(102), propput]	HRESULT Color([in] LONG lColor);

	[id(103), propget]	HRESULT Alpha([out, retval] LONG* Alpha);
	[id(103), propput]	HRESULT Alpha([in] LONG Alpha);

	[id(104), propget]	HRESULT DistanceX([out, retval] double* dDist);
	[id(104), propput]	HRESULT DistanceX([in] double dDist);

	[id(105), propget]	HRESULT DistanceY([out, retval] double* dDist);
	[id(105), propput]	HRESULT DistanceY([in] double dDist);

	[id(106), propget]	HRESULT BlurSize([out, retval] double* dSize);
	[id(106), propput]	HRESULT BlurSize([in] double dSize);

	[id(201)]			HRESULT DrawText([in] IAVSGraphics* pGraphics, [in] IAVSFontManager* pFont, [in] BSTR bsText, [in] double dLeft, [in] double dTop, [in] double  dRight, [in] double  dBottom);
	[id(202)]			HRESULT DrawPath([in] IAVSGraphics* pGraphics, [in] IAVSGraphicsPath* pPath);
};