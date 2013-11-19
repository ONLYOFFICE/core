#pragma once
#include "../stdafx.h"

#include "Structures.h"

#include "../PPTXFormat/Logic/Colors/SrgbClr.h"
#include "../PPTXFormat/Logic/Colors/PrstClr.h"
#include "../PPTXFormat/Logic/Colors/SchemeClr.h"
#include "../PPTXFormat/Logic/Colors/SysClr.h"

#include "PathHelpers.h"

namespace PPTX2EditorAdvanced
{
	AVSINLINE LONG GetColorIndexFromStr(const CString& str)
	{
		if (str == _T("phClr"))		return 0;
		if (str == _T("bg1"))		return 1;
		if (str == _T("tx1"))		return 2;
		if (str == _T("bg2"))		return 3;
		if (str == _T("tx2"))		return 4;
		if (str == _T("accent1"))	return 5;
		if (str == _T("accent2"))	return 6;
		if (str == _T("accent3"))	return 7;
		if (str == _T("accent4"))	return 8;
		if (str == _T("accent5"))	return 9;
		if (str == _T("accent6"))	return 10;
		if (str == _T("hlink"))		return 11;
		if (str == _T("folHlink"))	return 12;
		if (str == _T("lt1"))		return 13;
		if (str == _T("dk1"))		return 14;
		if (str == _T("lt2"))		return 15;
		if (str == _T("dk2"))		return 16;
		return 0;
	}

	void ColorConvert(const PPTX::Logic::UniColor& Source, NSPresentationEditor::CColor& Dest, DWORD BGRA)
	{
		if(Source.is<PPTX::Logic::SchemeClr>())
		{
			Dest = Source.GetBGRA(BGRA);
			Dest.m_lSchemeIndex = GetColorIndexFromStr(Source.as<PPTX::Logic::SchemeClr>().val.get());

			if (0 == Dest.m_lSchemeIndex)
				Dest.m_lSchemeIndex = -1;

			return;
		}
		Dest = Source.GetBGRA(BGRA);
	}
}