#pragma once
#include "..\stdafx.h"

#include "IASCGraphicsBase.h"

[object, uuid("166BB233-E93A-4159-8BC7-A2B2B5394B23"), dual, pointer_default(unique)]
__interface IAVSPen : IAVSGraphicsBase
{
	[id(100), propget]	HRESULT Xml([out, retval] BSTR* bstrXML);
	[id(100), propput]  HRESULT Xml([in] BSTR bstrXML);
	
	// general ------------------------------------------------------------
	[id(101), propget]	HRESULT Color([out, retval] LONG* lColor);
	[id(101), propput]	HRESULT Color([in] LONG lColor);

	[id(102), propget]	HRESULT Alpha([out, retval] LONG* Alpha);
	[id(102), propput]	HRESULT Alpha([in] LONG Alpha);

	[id(103), propget]	HRESULT Size([out, retval] double* dSize);
	[id(103), propput]	HRESULT Size([in] double dSize);

	// dash-style ---------------------------------------------------------
	[id(201), propget]	HRESULT DashStyle([out, retval] LONG* lColor);
	[id(201), propput]	HRESULT DashStyle([in] LONG lColor);

	[id(202), propget]	HRESULT DashOffset([out, retval] double* dOffset);
	[id(202), propput]	HRESULT DashOffset([in] double dOffset);

	[id(203), propget]	HRESULT DashPattern([out, retval] LPSAFEARRAY* lpArray);
	[id(203), propput]	HRESULT DashPattern([in] LPSAFEARRAY lpArray);

	[id(204), propget]	HRESULT DashCap([out, retval] LONG* lDashCap);
	[id(204), propput]	HRESULT DashCap([in] LONG lDashCap);

	// pen-style ----------------------------------------------------------
	[id(301), propget]	HRESULT LineStartCap([out, retval] LONG* lStartCap);
	[id(301), propput]	HRESULT LineStartCap([in] LONG lStartCap);

	[id(302), propget]	HRESULT LineEndCap([out, retval] LONG* lEndCap);
	[id(302), propput]	HRESULT LineEndCap([in] LONG lEndCap);

	[id(303), propget]	HRESULT LineJoin([out, retval] LONG* lJoin);
	[id(303), propput]	HRESULT LineJoin([in] LONG lJoin);

	[id(304), propget]	HRESULT LineMiterLimit([out, retval] LONG* lMiterLimit);
	[id(304), propput]	HRESULT LineMiterLimit([in] LONG lMiterLimit);

	[id(305), propget]	HRESULT Alignment([out, retval] LONG* lAlignment);
	[id(305), propput]	HRESULT Alignment([in] LONG lAlignment);

	// --------------------------------------------------------------------
	[id(400)]			HRESULT SetLineCap([in] LONG lStartCap, [in] LONG lEndCap, [in] LONG lDashCap);
};