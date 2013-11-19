#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	WORD Converter::GetFontAlignFromStr(const CString& str)
	{
		if (str == _T("auto"))	return 0;
		if (str == _T("base"))	return 0;
		if (str == _T("t"))		return 1;
		if (str == _T("ctr"))	return 2;
		if (str == _T("b"))		return 3;
		return 0;
	}
}