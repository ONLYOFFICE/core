#pragma once
#include "..\..\..\..\Common\OfficeRendererTemplate.h"

[object, uuid("BFD24172-C1B1-4de0-8CFF-378A4F9EC111"), dual, pointer_default(unique)]
__interface IAVSPageCommands : IAVSOfficeRendererTemplate
{
	[id(1000 + 1), propget]	HRESULT Width([out, retval] FLOAT* lWidth);
	[id(1000 + 2), propget]	HRESULT Height([out, retval] FLOAT* lHeight);

	[id(1000 + 3)]			HRESULT Draw([in] IUnknown* punkRenderer, [in] double dDPIX, [in] double dDPIY, [in] BOOL* pBreak);
	[id(1000 + 6)]			HRESULT Draw2([in] IUnknown* punkRenderer, [in] double dDPIX, [in] double dDPIY, [in] BOOL* pBreak);

	[id(1000 + 4)]			HRESULT InitAdvancedCommands();
	[id(1000 + 5)]			HRESULT DestroyAdvancedCommands();
};

[object, uuid("38BD7597-2053-41cf-AE1D-00A62609F580"), dual, pointer_default(unique)]
__interface IAVSCommandsRenderer : IAVSOfficeRendererTemplate
{
	[id(1000 + 1)] HRESULT DrawPage([in] LONG lPageNumber, [in] IUnknown* punkRenderer);

	[id(1000 + 2), propget] HRESULT PageCount([out, retval] LONG* lPageNumber);
	[id(1000 + 3)] HRESULT GetPageSize([in] LONG lPageNumber, [out] FLOAT* pfWidth, [out] FLOAT* pfHeight);

	[id(1000 + 4)] HRESULT LockPage([in] LONG lPageNumber, [out, retval] IUnknown** ppunkPage);
};

[dispinterface, uuid("71412D3A-845E-47e1-A3EE-0D3EF4A07A67")]
__interface _IAVSCommandsRendererEvents
{
	[id(100)] HRESULT OnNewPage(double dWidthMm, double dHeightMm);
	[id(101)] HRESULT OnCompletePage();
};
