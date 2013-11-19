#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/Fills/SolidFill.h"
#include "Logic/Fills/NoFill.h"
#include "Logic/Fills/BlipFill.h"
#include "Logic/Fills/GradFill.h"
#include "Logic/Fills/PattFill.h"
#include "Parse.h"

//0		- абсолютная прозрачность
//255	- абсолютная непрозрачность

namespace PPTX2OfficeDrawing
{
	long Converter::GetPattFillValFromStr(const std::string& str)
	{
		long result = 4009;
		if(str == "")
			return result;
		switch(str[0])
		{
		case 'c':
			if(str == "cross")		{ result += 4; break;} //(Cross)
			break;
		case 'd':
			if(str == "dashDnDiag")	{ result += 30; break;} //(Dashed Downward Diagonal)
			if(str == "dashHorz")	{ result += 32; break;} //(Dashed Horizontal)
			if(str == "dashUpDiag")	{ result += 31; break;} //(Dashed Upward DIagonal)
			if(str == "dashVert")	{ result += 33; break;} //(Dashed Vertical)
			if(str == "diagBrick")	{ result += 38; break;} //(Diagonal Brick)
			if(str == "diagCross")	{ result += 5; break;} //(Diagonal Cross)
			if(str == "divot")		{ result += 42; break;} //(Divot)
			if(str == "dkDnDiag")	{ result += 20; break;} //(Dark Downward Diagonal)
			if(str == "dkHorz")		{ result += 29; break;} //(Dark Horizontal)
			if(str == "dkUpDiag")	{ result += 21; break;} //(Dark Upward Diagonal)
			if(str == "dkVert")		{ result += 28; break;} //(Dark Vertical)
			if(str == "dnDiag")		{ result += 3; break;} //(Downward Diagonal)
			if(str == "dotDmnd")	{ result += 44; break;} //(Dotted Diamond)
			if(str == "dotGrid")	{ result += 43; break;} //(Dotted Grid)
			break;
		case 'h':
			if(str == "horz")		{ result += 0; break;} //(Horizontal)
			if(str == "horzBrick")	{ result += 39; break;} //(Horizontal Brick)
			break;
		case 'l':
			if(str == "lgCheck")	{ result += 50; break;} //(Large Checker Board)
			if(str == "lgConfetti")	{ result += 35; break;} //(Large Confetti)
			if(str == "lgGrid")		{ result += 4; break;} //(Large Grid)
			if(str == "ltDnDiag")	{ result += 18; break;} //(Light Downward Diagonal)
			if(str == "ltHorz")		{ result += 25; break;} //(Light Horizontal)
			if(str == "ltUpDiag")	{ result += 19; break;} //(Light Upward Diagonal)
			if(str == "ltVert")		{ result += 24; break;} //(Light Vertical)
			break;
		case 'n':
			if(str == "narHorz")	{ result += 27; break;} //(Narrow Horizontal)
			if(str == "narVert")	{ result += 26; break;} //(Narrow Vertical)
			break;
		case 'o':
			if(str == "openDmnd")	{ result += 51; break;} //(Open Diamond)
			break;
		case 'p':
			if(str == "pct10")		{ result += 7; break;} //(10%)
			if(str == "pct20")		{ result += 8; break;} //(20%)
			if(str == "pct25")		{ result += 9; break;} //(25%)
			if(str == "pct30")		{ result += 10; break;} //(30%)
			if(str == "pct40")		{ result += 11; break;} //(40%)
			if(str == "pct5")		{ result += 6; break;} //(5%)
			if(str == "pct50")		{ result += 12; break;} //(50%)
			if(str == "pct60")		{ result += 13; break;} //(60%)
			if(str == "pct70")		{ result += 14; break;} //(70%)
			if(str == "pct75")		{ result += 15; break;} //(75%)
			if(str == "pct80")		{ result += 16; break;} //(80%)
			if(str == "pct90")		{ result += 17; break;} //(90%)
			if(str == "plaid")		{ result += 41; break;} //(Plaid)
			break;
		case 's':
			if(str == "shingle")	{ result += 45; break;} //(Shingle)
			if(str == "smCheck")	{ result += 49; break;} //(Small Checker Board)
			if(str == "smConfetti")	{ result += 34; break;} //(Small Confetti)
			if(str == "smGrid")		{ result += 48; break;} //(Small Grid)
			if(str == "solidDmnd")	{ result += 52; break;} //(Solid Diamond)
			if(str == "sphere")		{ result += 47; break;} //(Sphere)
			break;
		case 't':
			if(str == "trellis")	{ result += 46; break;} //(Trellis)
			break;
		case 'u':
			if(str == "upDiag")		{ result += 2; break;} //(Upward Diagonal)
			break;
		case 'v':
			if(str == "vert")		{ result += 1; break;} //(Vertical)
			break;
		case 'w':
			if(str == "wave")		{ result += 37; break;} //(Wave)
			if(str == "wdDnDiag")	{ result += 22; break;} //(Wide Downward Diagonal)
			if(str == "wdUpDiag")	{ result += 23; break;} //(Wide Upward Diagonal)
			if(str == "weave")		{ result += 40; break;} //(Weave)
			break;
		case 'z':
			if(str == "zigZag")		{ result += 36; break;} //(Zig Zag)
			break;
		}
		return result;
	}

	void Converter::FillToBrush(const PPTX::Logic::UniFill& Fill, NSPresentationEditor::CBrush& Brush, DWORD BGRA)
	{
/*
		int		m_nBrushType;
		CColor	m_oColor1;
		CColor	m_oColor2;
		BYTE	m_Alpha1;
		BYTE	m_Alpha2;
		CStringW m_sTexturePath;
		int		m_nTextureMode;
		int		m_nRectable;
		BYTE	m_nTextureAlpha;
		CDoubleRect m_rcBounds;
		CEffects m_oEffects;
		bool m_bApplyBounds;
*/
		if(Fill.is<PPTX::Logic::NoFill>())
		{
			Brush.m_Alpha1 = 0;
			Brush.m_Alpha2 = 0;
		}
		else if(Fill.is<PPTX::Logic::SolidFill>())
		{
			Brush.m_nBrushType = (int)BrushTypeSolid;
			ColorConvert(Fill.as<PPTX::Logic::SolidFill>().Color, Brush.m_oColor1, BGRA);
			Brush.m_oColor2 = Brush.m_oColor1;
			//Brush.m_oColor1 = Fill.as<PPTX::Logic::SolidFill>().Color.GetBGRA(BGRA);
			//Brush.m_oColor2 = Fill.as<PPTX::Logic::SolidFill>().Color.GetBGRA(BGRA);
			Brush.m_Alpha1 = Brush.m_Alpha2 = Brush.m_oColor1.A;
		}
		else if(Fill.is<PPTX::Logic::BlipFill>())
		{
			Brush.m_nBrushType = (int)BrushTypeTexture;
			Brush.m_sTexturePath = CStringW(Fill.as<PPTX::Logic::BlipFill>().blip->GetFullPicName().c_str());
			Brush.m_nTextureMode = Fill.as<PPTX::Logic::BlipFill>().stretch.is_init()?(int)BrushTextureModeStretch:(int)BrushTextureModeTile;
			if(Fill.as<PPTX::Logic::BlipFill>().srcRect.is_init())
			{
				Brush.m_bApplyBounds = true;//Rectable - ???
				Brush.m_rcBounds.top = Parse<int>(Fill.as<PPTX::Logic::BlipFill>().srcRect->t.get_value_or("0")) / 1000.0;
				Brush.m_rcBounds.left = Parse<int>(Fill.as<PPTX::Logic::BlipFill>().srcRect->l.get_value_or("0")) / 1000.0;
				Brush.m_rcBounds.right = Parse<int>(Fill.as<PPTX::Logic::BlipFill>().srcRect->r.get_value_or("0")) / 1000.0;
				Brush.m_rcBounds.bottom = Parse<int>(Fill.as<PPTX::Logic::BlipFill>().srcRect->b.get_value_or("0")) / 1000.0;
			}
		}
		else if(Fill.is<PPTX::Logic::PattFill>())
		{
			Brush.m_nBrushType = GetPattFillValFromStr(Fill.as<PPTX::Logic::PattFill>().prst.get_value_or("horz"));
			ColorConvert(Fill.as<PPTX::Logic::PattFill>().fgClr, Brush.m_oColor1, BGRA);
			ColorConvert(Fill.as<PPTX::Logic::PattFill>().bgClr, Brush.m_oColor2, BGRA);
			//Brush.m_oColor1 = Fill.as<PPTX::Logic::PattFill>().fgClr.GetBGRA(BGRA);
			//Brush.m_oColor2 = Fill.as<PPTX::Logic::PattFill>().bgClr.GetBGRA(BGRA);
			Brush.m_Alpha1 = Brush.m_oColor1.A;
			Brush.m_Alpha2 = Brush.m_oColor2.A;
		}
		else if(Fill.is<PPTX::Logic::GradFill>())
		{
			if(Fill.as<PPTX::Logic::GradFill>().path.is_init())
			{
				if(Fill.as<PPTX::Logic::GradFill>().path->path.get() == "rect")
					//Brush.m_nBrushType = (int)BrushTypePathGradient2;
					Brush.m_nBrushType = (int)BrushTypeCenter;
				else if(Fill.as<PPTX::Logic::GradFill>().path->path.get() == "circle")
					//Brush.m_nBrushType = (int)BrushTypePathGradient1;
					//Brush.m_nBrushType = (int)BrushTypePathGradient2;
					//Brush.m_nBrushType = (int)BrushTypeCenter;
					Brush.m_nBrushType = (int)BrushTypeCylinderHor;
/*
    BrushTypeCylinderHor =	 2008,
    BrushTypeCylinderVer =	 2009,

*/
				else Brush.m_nBrushType = (int)BrushTypeCenter;

				ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor2, BGRA);
				ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor1, BGRA);
				//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
				//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
			}
			else
			{
				int ang = Fill.as<PPTX::Logic::GradFill>().lin->ang.get_value_or(5400000);
				if(ang <= 1800000)
				{
					Brush.m_nBrushType = (int)BrushTypeHorizontal;
					//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor1, BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor2, BGRA);
				}
				else if(ang < 3600000)
				{
					Brush.m_nBrushType = (int)BrushTypeDiagonal1;
					//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor1, BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor2, BGRA);
				}
				else if(ang < 7200000)
				{
					Brush.m_nBrushType = (int)BrushTypeVertical;
					//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor1, BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor2, BGRA);
				}
				else if(ang < 9000000)
				{
					Brush.m_nBrushType = (int)BrushTypeDiagonal2;
					//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor1, BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor2, BGRA);
				}
				else if(ang < 12600000)
				{
					Brush.m_nBrushType = (int)BrushTypeHorizontal;
					//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor2, BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor1, BGRA);
				}
				else if(ang < 14400000)
				{
					Brush.m_nBrushType = (int)BrushTypeDiagonal1;
					//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor2, BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor1, BGRA);
				}
				else if(ang < 18000000)
				{
					Brush.m_nBrushType = (int)BrushTypeVertical;
					//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor2, BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor1, BGRA);
				}
				else
				{
					Brush.m_nBrushType = (int)BrushTypeDiagonal2;
					//Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					//Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->front().color, Brush.m_oColor2, BGRA);
					ColorConvert(Fill.as<PPTX::Logic::GradFill>().GsLst->back().color, Brush.m_oColor1, BGRA);
				}
			}
			Brush.m_Alpha1 = Brush.m_oColor1.A;
			Brush.m_Alpha2 = Brush.m_oColor2.A;
		}
		else
		{
			Brush.m_Alpha1 = 0;
			Brush.m_Alpha2 = 0;
		}
	}
}