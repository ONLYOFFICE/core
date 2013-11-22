#pragma once
#include "../stdafx.h"

#include "IAVSGraphicsBase.h"

[object, uuid("A03E5FD2-2F5D-464a-9CFF-F9B09BC5FC72"), dual, pointer_default(unique)]
__interface IAVSStringFormat : IAVSGraphicsBase
{
	[id(101), propget]	HRESULT AlignmentH([out, retval] LONG* lVal);
	[id(101), propput]	HRESULT AlignmentH([in] LONG lVal);

	[id(102), propget]	HRESULT AlignmentV([out, retval] LONG* lVal);
	[id(102), propput]	HRESULT AlignmentV([in] LONG lVal);
};