#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/Fills/SolidFill.h"
#include "Logic/Fills/NoFill.h"
#include "Logic/Fills/GradFill.h"
#include "Logic/Fills/PattFill.h"

//0		- абсолютная прозрачность
//255	- абсолютная непрозрачность

namespace PPTX2DrawingXML
{
	BYTE Converter::GetLineEndType(const std::string& str)
	{
		if(str == "none") return 0x10;
		if(str == "diamond") return 0x13;
		if(str == "oval") return 0x12;
		if(str == "triangle") return 0x14;
		if(str == "arrow") return 0x14;
		if(str == "stealth") return 0x14;
		return 0x10;
	}

	BYTE Converter::GetLineJoinType(const PPTX::Logic::eJoin join)
	{
		if(join == PPTX::Logic::JoinEmpty) return 0;
		if(join == PPTX::Logic::JoinMiter) return 0;
		if(join == PPTX::Logic::JoinBevel) return 1;
		if(join == PPTX::Logic::JoinRound) return 2;
		return 0;
	}

	BYTE Converter::GetDashStyle(const std::string& str)
	{
		if(str == "solid") return 0;

		if(str == "dash") return 1;
		if(str == "dot") return 2;
		if(str == "dashDot") return 3;
		if(str == "lgDash") return 1;
		if(str == "lgDashDot") return 3;
		if(str == "lgDashDotDot") return 4;
		if(str == "sysDash") return 1;
		if(str == "sysDot") return 2;
		if(str == "sysDashDot") return 3;
		if(str == "sysDashDotDot") return 4;
		return 0;
	}

	void Converter::LnToPen(const PPTX::Logic::Ln& Line, DWORD BGRA, NSAttributes::CPen_& Pen)
	{
		if(!Line.Fill.is_init())
			Pen.m_nAlpha = 0;
		else if(Line.Fill.is<PPTX::Logic::NoFill>())
			Pen.m_nAlpha = 0;
		else if(Line.Fill.is<PPTX::Logic::SolidFill>())
		{
			Pen.m_oColor = Line.Fill.as<PPTX::Logic::SolidFill>().Color.GetBGRA(BGRA);
			Pen.m_nAlpha = Pen.m_oColor.A;
		}
		Pen.Size = Line.w.get_value_or(12700)/36000.0;

		if(Line.prstDash.is_init())
			Pen.DashStyle = GetDashStyle(Line.prstDash->val.get_value_or("solid"));
		if(Line.headEnd.is_init())
			Pen.LineStartCap = GetLineEndType(Line.headEnd->type.get_value_or("none"));
		if(Line.tailEnd.is_init())
			Pen.LineEndCap = GetLineEndType(Line.tailEnd->type.get_value_or("none"));
		Pen.LineJoin = GetLineJoinType(Line.Join.type.get());
	}
}