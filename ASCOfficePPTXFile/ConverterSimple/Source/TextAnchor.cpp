#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	WORD Converter::GetTextAnchorFromStr(const CString& str)
	{
		if (str == _T("t"))		return 0;
		if (str == _T("ctr"))	return 1;
		if (str == _T("b"))		return 2;
		return 0;
	}
}