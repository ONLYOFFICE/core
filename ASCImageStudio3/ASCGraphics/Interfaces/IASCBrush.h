#pragma once
#include "..\stdafx.h"

#include "IASCGraphicsBase.h"

[object, uuid("64AE5000-6CF6-461a-A766-344F88DC8381"), dual, pointer_default(unique)]
__interface IAVSBrush : IAVSGraphicsBase
{
	[id(100), propget]	HRESULT Xml([out, retval] BSTR* bstrXML);
	[id(100), propput]	HRESULT Xml([in] BSTR bstrXML);
	
	// general ------------------------------------------------------------
	[id(101), propget]	HRESULT Color1([out, retval] LONG* lColor);
	[id(101), propput]	HRESULT Color1([in] LONG lColor);

	[id(102), propget]	HRESULT Color2([out, retval] LONG* lColor);
	[id(102), propput]	HRESULT Color2([in] LONG lColor);

	[id(103), propget]	HRESULT Alpha1([out, retval] LONG* Alpha);
	[id(103), propput]	HRESULT Alpha1([in] LONG Alpha);

	[id(104), propget]	HRESULT Alpha2([out, retval] LONG* Alpha);
	[id(104), propput]	HRESULT Alpha2([in] LONG Alpha);

	[id(105), propget]	HRESULT Type([out, retval] LONG* lType);
	[id(105), propput]	HRESULT Type([in] LONG lType);

	[id(106), propget]	HRESULT TextureAlpha([out, retval] LONG* lType);
	[id(106), propput]	HRESULT TextureAlpha([in] LONG lType);

	[id(107), propget]	HRESULT TextureMode([out, retval] LONG* lType);
	[id(107), propput]	HRESULT TextureMode([in] LONG lType);

	[id(108), propget]	HRESULT TexturePath([out, retval] BSTR* bstrPath);
	[id(108), propput]	HRESULT TexturePath([in] BSTR bstrPath);

	[id(109)]			HRESULT SetRect([in]  double  dLeft, [in]  double  dTop, [in]  double  dRight, [in]  double  dBottom);
	[id(110)]			HRESULT GetRect([out] double* dLeft, [out] double* dTop, [out] double* dRight, [out] double* dBottom);
};