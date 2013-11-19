#pragma once 

#include "ColorConvert.h"

#include "../PPTXFormat/Logic/Fills/SolidFill.h"
#include "../PPTXFormat/Logic/Fills/NoFill.h"
#include "../PPTXFormat/Logic/Fills/BlipFill.h"
#include "../PPTXFormat/Logic/Fills/GradFill.h"
#include "../PPTXFormat/Logic/Fills/PattFill.h"

//0		- абсолютная прозрачность
//255	- абсолютная непрозрачность

namespace PPTX2EditorAdvanced
{
	void FillToBrush(const PPTX::Logic::UniFill& Fill, DWORD BGRA, NSPresentationEditor::CBrush& Brush)
	{
		if(Fill.is_init())
		{
			if(Fill.is<PPTX::Logic::NoFill>())
			{
				Brush.Alpha1 = 0;
				Brush.Alpha2 = 0;
			}
			else if(Fill.is<PPTX::Logic::SolidFill>())
			{
				const PPTX::Logic::SolidFill& oSolid = Fill.as<PPTX::Logic::SolidFill>();
				
				Brush.Type = (int)NSPresentationEditor::c_BrushTypeSolid;
				
				ColorConvert(oSolid.Color, Brush.Color1, BGRA);
				Brush.Color2   = Brush.Color1;
				//Brush.Color1 = oSolid.Color.GetBGRA(BGRA);
				//Brush.Color2 = oSolid.Color.GetBGRA(BGRA);
				Brush.Alpha1 = Brush.Alpha2 = Brush.Color1.A;
			}
			else if(Fill.is<PPTX::Logic::BlipFill>())
			{
				const PPTX::Logic::BlipFill& oTexture = Fill.as<PPTX::Logic::BlipFill>();
				
				Brush.Type = (int)NSPresentationEditor::c_BrushTypeTexture;		
				Brush.TexturePath = oTexture.blip->GetFullPicName();
				Brush.TextureMode = oTexture.tile.is_init()?(int)NSPresentationEditor::c_BrushTextureModeTile:(int)NSPresentationEditor::c_BrushTextureModeStretch;
				//if(oTexture.srcRect.is_init())
				//{
				//	Brush.m_rcBounds.top = 
				//	Brush.m_rcBounds.left = 
				//	Brush.m_rcBounds.rigth = 
				//	Brush.m_rcBounds.bottom = 
				//}
			}
			else if(Fill.is<PPTX::Logic::PattFill>())
			{
				const PPTX::Logic::PattFill& oPatt = Fill.as<PPTX::Logic::PattFill>();
				
				Brush.Type = GetPattFillValFromStr(Fill.as<PPTX::Logic::PattFill>().prst.get_value_or(_T("horz")));
				Brush.Color1 = oPatt.fgClr.GetBGRA(BGRA);
				Brush.Color2 = oPatt.bgClr.GetBGRA(BGRA);
				Brush.Alpha1 = Brush.Color1.A;
				Brush.Alpha2 = Brush.Color2.A;
			}
			else if(Fill.is<PPTX::Logic::GradFill>())
			{
				const PPTX::Logic::GradFill& GradFill = Fill.as<PPTX::Logic::GradFill>();

				if (GradFill.path.is_init())
				{
					if (GradFill.path->path.get_value_or(_T("")) == _T("rect"))
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeCenter;
					}
					else if (GradFill.path->path.get_value_or(_T("")) == _T("circle"))
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeCylinderHor;
					}

					else Brush.Type = (int)NSPresentationEditor::c_BrushTypeCenter;

					size_t nCount = GradFill.GsLst.GetCount();
					if (0 == nCount)
					{
						Brush.Color1 = 0xFF;
						Brush.Color2 = 0xFF;
					}
					else
					{
						Brush.Color2 = GradFill.GsLst[0].color.GetBGRA(BGRA);
						Brush.Color1 = GradFill.GsLst[nCount - 1].color.GetBGRA(BGRA);
					}
				}
				else
				{
					int ang = 5400000;
					if (GradFill.lin.is_init())
					{
						ang = GradFill.lin->ang.get_value_or(5400000);
					}
					if(ang <= 1800000)
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeHorizontal;
					}
					else if(ang < 3600000)
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeDiagonal1;
					}
					else if(ang < 7200000)
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeVertical;
					}
					else if(ang < 9000000)
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeDiagonal2;
					}
					else if(ang < 12600000)
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeHorizontal;
					}
					else if(ang < 14400000)
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeDiagonal1;
					}
					else if(ang < 18000000)
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeVertical;
					}
					else
					{
						Brush.Type = (int)NSPresentationEditor::c_BrushTypeDiagonal2;
					}

					size_t nCount = GradFill.GsLst.GetCount();
					if (0 == nCount)
					{
						Brush.Color1 = 0xFF;
						Brush.Color2 = 0xFF;
					}
					else
					{
						Brush.Color1 = GradFill.GsLst[0].color.GetBGRA(BGRA);
						Brush.Color2 = GradFill.GsLst[nCount - 1].color.GetBGRA(BGRA);
					}
					
				}
				Brush.Alpha1 = Brush.Color1.A;
				Brush.Alpha2 = Brush.Color2.A;
			}
		}
		else
		{
			Brush.Alpha1 = 0;
			Brush.Alpha2 = 0;
		}
	}
}