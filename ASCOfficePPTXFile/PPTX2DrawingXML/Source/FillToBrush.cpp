#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/Fills/SolidFill.h"
#include "Logic/Fills/NoFill.h"
#include "Logic/Fills/BlipFill.h"
#include "Logic/Fills/GradFill.h"
#include "Logic/Fills/PattFill.h"
//#include "Parse.h"

//0		- абсолютная прозрачность
//255	- абсолютная непрозрачность

namespace PPTX2DrawingXML
{
	void Converter::FillToBrush(const PPTX::Logic::UniFill& Fill, DWORD BGRA, NSAttributes::CBrush_& Brush)
	{
//		int m_nBrushType;
//		CColor_ m_oColor1;
//		CColor_ m_oColor2;
//		BYTE m_Alpha1;
//		BYTE m_Alpha2;
//		CStringW m_sTexturePath;
//		int m_nTextureMode;
//		int m_nRectable;
//		BYTE m_nTextureAlpha;
//		RECT m_rcBounds;

		if(Fill.is_init())
		{
			if(Fill.is<PPTX::Logic::NoFill>())
			{
				Brush.m_Alpha1 = 0;
				Brush.m_Alpha2 = 0;
			}
			else if(Fill.is<PPTX::Logic::SolidFill>())
			{
				Brush.m_nBrushType = (int)BrushTypeSolid;
				Brush.m_oColor1 = Fill.as<PPTX::Logic::SolidFill>().Color.GetBGRA(BGRA);
				Brush.m_oColor2 = Fill.as<PPTX::Logic::SolidFill>().Color.GetBGRA(BGRA);
				Brush.m_Alpha1 = Brush.m_Alpha2 = Brush.m_oColor1.A;
			}
			else if(Fill.is<PPTX::Logic::BlipFill>())
			{
				Brush.m_nBrushType = (int)BrushTypeTexture;
				Brush.m_sTexturePath = CStringW(Fill.as<PPTX::Logic::BlipFill>().blip->GetFullPicName().c_str());
				Brush.m_nTextureMode = Fill.as<PPTX::Logic::BlipFill>().tile.is_init()?(int)BrushTextureModeTile:(int)BrushTextureModeStretch;
				//if(Fill.as<PPTX::Logic::BlipFill>().srcRect.is_init())
				//{
				//	Brush.m_rcBounds.top = 
				//	Brush.m_rcBounds.left = 
				//	Brush.m_rcBounds.rigth = 
				//	Brush.m_rcBounds.bottom = 
				//}
			}
			else if(Fill.is<PPTX::Logic::PattFill>())
			{
				Brush.m_nBrushType = GetPattFillValFromStr(Fill.as<PPTX::Logic::PattFill>().prst.get_value_or("horz"));
				Brush.m_oColor1 = Fill.as<PPTX::Logic::PattFill>().fgClr.GetBGRA(BGRA);
				Brush.m_oColor2 = Fill.as<PPTX::Logic::PattFill>().bgClr.GetBGRA(BGRA);
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

					Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
					Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
				}
				else
				{
					int ang = 5400000;
					PPTX::Logic::GradFill _fill = Fill.as<PPTX::Logic::GradFill>();
					if (_fill.lin.is_init())
					{
						ang = Fill.as<PPTX::Logic::GradFill>().lin->ang.get_value_or(5400000);
					}
					if(ang <= 1800000)
					{
						Brush.m_nBrushType = (int)BrushTypeHorizontal;
						Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
						Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					}
					else if(ang < 3600000)
					{
						Brush.m_nBrushType = (int)BrushTypeDiagonal1;
						Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
						Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					}
					else if(ang < 7200000)
					{
						Brush.m_nBrushType = (int)BrushTypeVertical;
						Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
						Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					}
					else if(ang < 9000000)
					{
						Brush.m_nBrushType = (int)BrushTypeDiagonal2;
						Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
						Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					}
					else if(ang < 12600000)
					{
						Brush.m_nBrushType = (int)BrushTypeHorizontal;
						Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
						Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					}
					else if(ang < 14400000)
					{
						Brush.m_nBrushType = (int)BrushTypeDiagonal1;
						Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
						Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					}
					else if(ang < 18000000)
					{
						Brush.m_nBrushType = (int)BrushTypeVertical;
						Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
						Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					}
					else
					{
						Brush.m_nBrushType = (int)BrushTypeDiagonal2;
						Brush.m_oColor2 = Fill.as<PPTX::Logic::GradFill>().GsLst->front().color.GetBGRA(BGRA);
						Brush.m_oColor1 = Fill.as<PPTX::Logic::GradFill>().GsLst->back().color.GetBGRA(BGRA);
					}
				}
				Brush.m_Alpha1 = Brush.m_oColor1.A;
				Brush.m_Alpha2 = Brush.m_oColor2.A;
			}
		}
		else
		{
			Brush.m_Alpha1 = 0;
			Brush.m_Alpha2 = 0;
		}
	}
}