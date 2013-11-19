#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	DWORD Converter::GetTLRestartFromStr(const CString& str)
	{
		if (str == _T("never"))			return 0;
		if (str == _T("always"))		return 1;
		if (str == _T("whenNotActive")) return 2;
		return 0;
	}
}