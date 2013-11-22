#pragma once
#include "ASCRenderer.h"

// IASCGraphicsRenderer
[object, uuid("A278FE9F-622A-43a3-9F18-363311637073"), dual, pointer_default(unique)]
__interface IASCGraphicsRenderer : IASCRenderer
{
	[id(20000 + 1)]	HRESULT CreateFromMediaData([in] IUnknown* punkMediaData, [in] double dLeft, [in] double dTop, [in] double dWidth, [in] double dHeight);
	[id(20000 + 2)]	HRESULT CreateFromHDC([in] LONG* pHDC, [in] IUnknown* punkFontManager, [in] double dWidthMM, [in] double dHeightMM,
		[in] double dLeft, [in] double dTop, [in] double dWidth, [in] double dHeight, [in] double dAngle);
	[id(20000 + 3)] HRESULT BitBlt([in] LONG* pHDC);
};