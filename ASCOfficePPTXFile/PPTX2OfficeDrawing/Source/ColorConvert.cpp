#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/Colors/SrgbClr.h"
#include "Logic/Colors/PrstClr.h"
#include "Logic/Colors/SchemeClr.h"
#include "Logic/Colors/SysClr.h"

namespace PPTX2OfficeDrawing
{
	long Converter::GetColorIndexFromStr(const std::string& str)
	{
		if(str == "phClr")	return 0;
		if(str == "bg1")	return 1;
		if(str == "tx1")	return 2;
		if(str == "bg2")	return 3;
		if(str == "tx2")	return 4;
		if(str == "accent1")return 5;
		if(str == "accent2")return 6;
		if(str == "accent3")return 7;
		if(str == "accent4")return 8;
		if(str == "accent5")return 9;
		if(str == "accent6")return 10;
		if(str == "hlink")	return 11;
		if(str == "folHlink")return 12;
		if(str == "lt1")	return 13;
		if(str == "dk1")	return 14;
		if(str == "lt2")	return 15;
		if(str == "dk2")	return 16;
		return 0;
	}

	void Converter::ColorConvert(const PPTX::Logic::UniColor& Source, NSPresentationEditor::CColor& Dest, DWORD BGRA)
	{
		if(Source.is<PPTX::Logic::SchemeClr>())
		{
			Dest = 0;
			Dest.m_lSchemeIndex = GetColorIndexFromStr(*Source.as<PPTX::Logic::SchemeClr>().val);
		}
		else
			Dest = Source.GetBGRA(BGRA);
	}
}