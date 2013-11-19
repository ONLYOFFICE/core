#include "stdafx.h"
#include "./../Converter.h"
#include "../../PPTXFormat/Logic/Fills/SolidFill.h"
#include "../../PPTXFormat/Logic/Fills/NoFill.h"
#include "../../PPTXFormat/Logic/Fills/GradFill.h"
#include "../../PPTXFormat/Logic/Fills/PattFill.h"

//0		- абсолютная прозрачность
//255	- абсолютная непрозрачность

namespace PPTX2EditorSimple
{
	BYTE Converter::GetLineEndType(const CString& str)
	{
		if (str == _T("none"))		return 0x10;
		if (str == _T("diamond"))	return 0x13;
		if (str == _T("oval"))		return 0x12;
		if (str == _T("triangle"))	return 0x14;
		if (str == _T("arrow"))		return 0x14;
		if (str == _T("stealth"))	return 0x14;
		return 0x10;
	}

	BYTE Converter::GetLineJoinType(const PPTX::Logic::eJoin join)
	{
		if (join == PPTX::Logic::JoinEmpty) return 0;
		if (join == PPTX::Logic::JoinMiter) return 0;
		if (join == PPTX::Logic::JoinBevel) return 1;
		if (join == PPTX::Logic::JoinRound) return 2;
		return 0;
	}

	BYTE Converter::GetDashStyle(const CString& str)
	{
		if (str == _T("solid"))			return 0;
		if (str == _T("dash"))			return 1;
		if (str == _T("dot"))			return 2;
		if (str == _T("dashDot"))		return 3;
		if (str == _T("lgDash"))		return 1;
		if (str == _T("lgDashDot"))		return 3;
		if (str == _T("lgDashDotDot"))	return 4;
		if (str == _T("sysDash"))		return 1;
		if (str == _T("sysDot"))		return 2;
		if (str == _T("sysDashDot"))	return 3;
		if (str == _T("sysDashDotDot"))	return 4;
		return 0;
	}

	void Converter::LnToPen(const PPTX::Logic::Ln& Line, DWORD BGRA, NSPresentationEditor::CPen& Pen)
	{
		if(!Line.Fill.is_init())
			Pen.Alpha = 0;
		else if(Line.Fill.is<PPTX::Logic::NoFill>())
			Pen.Alpha = 0;
		else if(Line.Fill.is<PPTX::Logic::SolidFill>())
		{
			Pen.Color = Line.Fill.as<PPTX::Logic::SolidFill>().Color.GetBGRA(BGRA);
			Pen.Alpha = Pen.Color.A;
		}
		Pen.Size = Line.w.get_value_or(12700)/36000.0;

		if(Line.prstDash.is_init())
			Pen.DashStyle = GetDashStyle(Line.prstDash->val.get_value_or(_T("solid")));
		if(Line.headEnd.is_init())
			Pen.LineStartCap = GetLineEndType(Line.headEnd->type.get_value_or(_T("none")));
		if(Line.tailEnd.is_init())
			Pen.LineEndCap = GetLineEndType(Line.tailEnd->type.get_value_or(_T("none")));
		Pen.LineJoin = GetLineJoinType(Line.Join.type);
	}
}