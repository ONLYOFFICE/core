#pragma once
#include "..\stdafx.h"

#include "IAVSGraphics.h"

[object, uuid("20B29BAC-2ECF-4c29-B590-D32CC278EDAF"), dual, pointer_default(unique)]
__interface IAVSEdgeText : IAVSGraphicsBase
{
	[id(100), propget]	HRESULT Xml([out, retval] BSTR* bstrXML);
	[id(100), propput]	HRESULT Xml([in] BSTR bstrXML);
	
	// general ------------------------------------------------------------
	[id(101), propget]	HRESULT Visible([out, retval] LONG* lVisible);
	[id(101), propput]	HRESULT Visible([in] LONG lVisible);

	[id(102), propget]	HRESULT Color([out, retval] LONG* lColor);
	[id(102), propput]	HRESULT Color([in] LONG lColor);

	[id(103), propget]	HRESULT Alpha([out, retval] LONG* Alpha);
	[id(103), propput]	HRESULT Alpha([in] LONG Alpha);

	[id(104), propget]	HRESULT Distance([out, retval] double* dDist);
	[id(104), propput]	HRESULT Distance([in] double dDist);

	[id(201)]			HRESULT DrawText([in] IAVSGraphics* pGraphics, [in] IAVSFontManager* pFont, [in] BSTR bsText, [in] double dLeft, [in] double  dTop, [in] double  dRight, [in] double  dBottom);
};