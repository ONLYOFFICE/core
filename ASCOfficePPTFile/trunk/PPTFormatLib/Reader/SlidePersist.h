#pragma once

#if defined(_WIN32) || defined (_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../../../DesktopEditor/common/ASCVariant.h"
    #include "../../../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include <vector>
/**************************************************************
теперь все структуры...
которые участвуют в записях
**************************************************************/
class CTextFullSettings;
struct SSlidePersist
{
	DWORD m_nPsrRef;
	DWORD m_nSlideID;

	std::vector<CTextFullSettings> m_arTextAttrs;

	SSlidePersist();
	SSlidePersist(const SSlidePersist& oSrc);
	CString ToString();
	SSlidePersist& operator =(const SSlidePersist& oSrc);
}; 
