#pragma once
#include "..\stdafx.h"

#include "IAVSGraphicsBase.h"

[object, uuid("90445113-FEDD-4e88-AC18-06564C5399E6"), dual, pointer_default(unique)]
__interface IAVSPattern : IAVSGraphicsBase
{
	[id(100), propget]	HRESULT Type([out, retval] LONG* lType);
	
	[id(201), propget]	HRESULT Width([out, retval] LONG* lWidth);
	[id(202), propget]  HRESULT Height([out, retval] LONG* lHeight);

	[id(300)]			HRESULT GetPixel([in] LONG lX, [in] LONG lY, [out] LONG* pColor);
};