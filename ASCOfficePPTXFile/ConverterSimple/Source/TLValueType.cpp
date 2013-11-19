#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	DWORD Converter::GetTLValueTypeFromStr(const CString& str)
	{
		if (str == _T("str")) return 0;
		if (str == _T("num")) return 1;
		if (str == _T("clr")) return 2;
		return 1;
	}
}