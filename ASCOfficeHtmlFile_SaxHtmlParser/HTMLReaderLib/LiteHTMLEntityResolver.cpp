#include "stdafx.h"
#include "LiteHTMLEntityResolver.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif	// _DEBUG

#pragma warning(push, 4)
CLiteHTMLEntityResolver::CCharEntityRefs CLiteHTMLEntityResolver::m_CharEntityRefs;
#pragma warning(pop)
