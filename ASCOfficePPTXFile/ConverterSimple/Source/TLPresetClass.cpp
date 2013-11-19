#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	DWORD Converter::GetTLPresetClassFromStr(const CString& str)
	{
		if (str == _T("entr"))		return 1;
		if (str == _T("exit"))		return 2;
		if (str == _T("emph"))		return 3;
		if (str == _T("path"))		return 4;
		if (str == _T("verb"))		return 5;
		if (str == _T("mediacall")) return 6;
		return 1;
	}
}