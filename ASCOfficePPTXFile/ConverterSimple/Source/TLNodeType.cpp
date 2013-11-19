#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	DWORD Converter::GetTLNodeTypeFromStr(const CString& str)
	{
		if (str == _T("clickEffect"))		return 1;
		if (str == _T("withEffect"))		return 2;
		if (str == _T("afterEffect"))		return 3;
		if (str == _T("mainSeq"))			return 4;
		if (str == _T("interactiveSeq"))	return 5;
		if (str == _T("clickPar"))			return 6;
		if (str == _T("withGroup"))			return 7;
		if (str == _T("afterGroup"))		return 8;
		if (str == _T("tmRoot"))			return 9;
		return 9;
	}
}