#include "stdafx.h"
#include "LiteHTMLAttributes.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif	// _DEBUG

#pragma warning(push, 4)

const COLORREF CLiteHTMLElemAttr::_clrInvalid = (COLORREF)0xFFFFFFFF;
const unsigned short CLiteHTMLElemAttr::_percentMax = USHRT_MAX;

// the reason behind setting the block size of our collection 
// to 166 is that we have a total of 166 known named colors

CLiteHTMLElemAttr::CNamedColors CLiteHTMLElemAttr::_namedColors(166 /* block size */);

#pragma warning(pop)

