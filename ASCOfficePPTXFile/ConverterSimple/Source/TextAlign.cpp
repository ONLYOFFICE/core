#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	WORD Converter::GetTextAlignFromStr(const CString& str)
	{
		if (str == _T("l"))			return 0;
		if (str == _T("ctr"))		return 1;
		if (str == _T("r"))			return 2;
		if (str == _T("just"))		return 3;
		if (str == _T("dist"))		return 4;
		if (str == _T("thaiDist"))	return 5;
		if (str == _T("justLow"))	return 6;
		return 0;
	}
}