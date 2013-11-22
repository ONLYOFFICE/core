#pragma once
#include "..\stdafx.h"

#include "IAVSGraphicsBase.h"

[object, uuid("280F9CFE-D5F6-4594-B6C5-A7F3D6984CF2"), dual, pointer_default(unique)]
__interface IAVSGlyphImage : IAVSGraphicsBase
{
	[id(101), propget]	HRESULT Width([out, retval] LONG* width);

	[id(102), propget]	HRESULT Height([out, retval] LONG* height);

	[id(201), propget]	HRESULT Antialiased([out, retval] VARIANT_BOOL* val);

	[id(301), propget]	HRESULT Data([out, retval] BYTE** ppBuffer);
};

[object, uuid("1F21202B-9983-4164-B6B8-1BF82D66BF90"), dual, pointer_default(unique)]
__interface IAVSGlyphImage2 : IAVSGlyphImage
{
	[id(401)]			HRESULT Create([in] BYTE* pBuffer, [in] LONG lX, [in] LONG lY, [in] LONG lWidth, [in] LONG lHeight, [in] BOOL bIsFreeData);
};