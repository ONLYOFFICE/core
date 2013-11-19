#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	DWORD Converter::GetTLCalcModeFromStr(const CString& str)
	{
		if (str == _T("discrete"))	return 0;
		if (str == _T("lin"))		return 1;
		if (str == _T("fmla"))		return 2;
		return 0;
	}
}