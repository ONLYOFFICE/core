#include "stdafx.h"
#include "./../Converter.h"
#include "Styles/FillImage.h"
#include "Styles/Gradient.h"

namespace Odp2DrawingXML
{
	void Converter::FillPenBrush(const Odt::Logic::Properties& pProps, CShapeElement& pShape)const
	{
		if(pProps.Graphic.is_init())
		{
			std::string fill = pProps.Graphic->Fill.get_value_or("none");
			long opacity = 255;
			if(pProps.Graphic->Opacity.is_init())
				opacity = (long)(255 * (100 - pProps.Graphic->Opacity->value())/100.0);

			if(fill == "none")
			{
				pShape.m_oShape.m_oBrush.Alpha1 = 0;
				pShape.m_oShape.m_oBrush.Alpha2 = 0;
			}
			else if(fill == "solid")
			{
				pShape.m_oShape.m_oBrush.Type = (int)c_BrushTypeSolid;
				if(pProps.Graphic->FillColor.is_init())
				{
					pShape.m_oShape.m_oBrush.Color1.R = pProps.Graphic->FillColor->Red.get();
					pShape.m_oShape.m_oBrush.Color1.G = pProps.Graphic->FillColor->Green.get();
					pShape.m_oShape.m_oBrush.Color1.B = pProps.Graphic->FillColor->Blue.get();
					pShape.m_oShape.m_oBrush.Color2.R = pShape.m_oShape.m_oBrush.Color1.R;
					pShape.m_oShape.m_oBrush.Color2.G = pShape.m_oShape.m_oBrush.Color1.G;
					pShape.m_oShape.m_oBrush.Color2.B = pShape.m_oShape.m_oBrush.Color1.B;
				}
				else
				{
					pShape.m_oShape.m_oBrush.Color1.R = 255;
					pShape.m_oShape.m_oBrush.Color1.G = 255;
					pShape.m_oShape.m_oBrush.Color1.B = 255;
					pShape.m_oShape.m_oBrush.Color2.R = 255;
					pShape.m_oShape.m_oBrush.Color2.G = 255;
					pShape.m_oShape.m_oBrush.Color2.B = 255;
				}
				pShape.m_oShape.m_oBrush.Alpha1 = pShape.m_oShape.m_oBrush.Alpha2 = opacity;
			}
			else if((fill == "gradient") && (pProps.Graphic->FillGradientName.is_init()))
			{
				pShape.m_oShape.m_oBrush.Type = (int)c_BrushTypeCenter;//TODO : изменить в заивсимости от типа
				std::string gradient = pProps.Graphic->FillGradientName.get();
				Odp::Styles::Gradient Gradient = m_Folder->Styles->Gradients->find(gradient);
				pShape.m_oShape.m_oBrush.Color1.R = Gradient.StartColor->Red.get();
				pShape.m_oShape.m_oBrush.Color1.G = Gradient.StartColor->Green.get();
				pShape.m_oShape.m_oBrush.Color1.B = Gradient.StartColor->Blue.get();
				pShape.m_oShape.m_oBrush.Color2.R = Gradient.EndColor->Red.get();
				pShape.m_oShape.m_oBrush.Color2.G = Gradient.EndColor->Green.get();
				pShape.m_oShape.m_oBrush.Color2.B = Gradient.EndColor->Blue.get();

				pShape.m_oShape.m_oBrush.Alpha1 = pShape.m_oShape.m_oBrush.Alpha2 = opacity;
			}
			else if((fill == "bitmap") && (pProps.Graphic->FillImageName.is_init()))
			{
				pShape.m_oShape.m_oBrush.Type = (int)c_BrushTypeTexture;
				std::string image = pProps.Graphic->FillImageName.get();
				Odp::Styles::FillImage FillImage = m_Folder->Styles->FillImages->find(image);
				pShape.m_oShape.m_oBrush.TexturePath = CStringW(m_Folder->GetPath().c_str()) + CStringW(FillImage.Href.get().c_str());
				pShape.m_oShape.m_oBrush.TextureMode = (int)c_BrushTextureModeTile;//:(int)BrushTextureModeStretch;
			}

			std::string stroke = pProps.Graphic->Stroke.get_value_or("none");
			if(stroke == "none")
			{
				pShape.m_oShape.m_oPen.Alpha = 0;
			}
			else if(stroke == "solid")
			{
				if(pProps.Graphic->StrokeColor.is_init())
				{
					pShape.m_oShape.m_oPen.Color.R = pProps.Graphic->StrokeColor->Red.get();
					pShape.m_oShape.m_oPen.Color.G = pProps.Graphic->StrokeColor->Green.get();
					pShape.m_oShape.m_oPen.Color.B = pProps.Graphic->StrokeColor->Blue.get();
				}
				else
				{
					pShape.m_oShape.m_oPen.Color.R = 0;
					pShape.m_oShape.m_oPen.Color.G = 0;
					pShape.m_oShape.m_oPen.Color.B = 0;
				}
			}
		}
	}

	void Converter::FillBackground(const Odt::Logic::Properties& pProps, CBrush& pBrush)const
	{
		std::string fill = pProps.DrawingPage->Fill.get_value_or("none");
		if(fill == "none")
		{
			pBrush.Alpha1 = 0;
			pBrush.Alpha2 = 0;
		}
		else if((fill == "bitmap") && (pProps.DrawingPage->FillImageName.is_init()))
		{
			std::string image = pProps.DrawingPage->FillImageName.get();
			Odp::Styles::FillImage FillImage = m_Folder->Styles->FillImages->find(image);
			pBrush.Type			= (int)c_BrushTypeTexture;
			pBrush.TexturePath	= CStringW(m_Folder->GetPath().c_str()) + CStringW(FillImage.Href.get().c_str());
		}
		else if(fill == "solid")
		{
			pBrush.Type = (int)c_BrushTypeSolid;
			if(pProps.DrawingPage->FillColor.is_init())
			{
				pBrush.Color1.R = pProps.DrawingPage->FillColor->Red.get();
				pBrush.Color1.G = pProps.DrawingPage->FillColor->Green.get();
				pBrush.Color1.B = pProps.DrawingPage->FillColor->Blue.get();
				pBrush.Color2.R = pBrush.Color1.R;
				pBrush.Color2.G = pBrush.Color1.G;
				pBrush.Color2.B = pBrush.Color1.B;
			}
			else
			{
				pBrush.Color1.R = 255;
				pBrush.Color1.G = 255;
				pBrush.Color1.B = 255;
				pBrush.Color2.R = 255;
				pBrush.Color2.G = 255;
				pBrush.Color2.B = 255;
			}
		}
	}
}