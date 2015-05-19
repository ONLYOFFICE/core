#pragma once

#include "Gdiplus.h"
#include "..\Objects\Structures.h"

namespace NSStructuresGDI
{
	inline void DrawBlurString(int currentBlurSize, Gdiplus::Graphics* Gr, Gdiplus::Font* font, 
		Gdiplus::StringFormat* format, Gdiplus::Brush* brush, BSTR Text, int len, Gdiplus::RectF& rect)
	{
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.X += 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.Y += 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.X -= 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.X -= 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.Y -= 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.Y -= 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.X += 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.X += 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			rect.Y += 1;
			Gr->DrawString(Text, len, font, rect, format, brush);
		}
	}
	
	class CPen : public NSStructures::CPen
	{
	public:
		CPen() : NSStructures::CPen()
		{
		}
		CPen( const CPen& other ) : NSStructures::CPen(other)
		{
		}
		CPen& operator=(const CPen& other)
		{
			NSStructures::CPen::operator=(other);
			return *this;
		}
		virtual ~CPen()
		{
		}

		Gdiplus::Pen* GetPen()
		{
			Gdiplus::Color color((BYTE)Alpha, GetRValue(Color), GetGValue(Color), GetBValue(Color));

			Gdiplus::Pen* pPen = new Gdiplus::Pen(color, (float)Size);
			pPen->SetDashStyle((Gdiplus::DashStyle)DashStyle);
			pPen->SetStartCap((Gdiplus::LineCap)LineStartCap);
			pPen->SetEndCap((Gdiplus::LineCap)LineEndCap);
			pPen->SetLineJoin((Gdiplus::LineJoin)LineJoin);

			if ((Count != 0) && (DashPattern != NULL))
			{
				float* pPattern = new Gdiplus::REAL[Count];
				for (long i = 0; i < Count; ++i)
				{
					pPattern[i] = (float)DashPattern[i];
				}
				
				pPen->SetDashPattern(pPattern, Count);
				RELEASEARRAYOBJECTS(pPattern);
			}

			pPen->SetDashOffset((float)DashOffset);
			pPen->SetAlignment( (Gdiplus::PenAlignment)Align );

			return pPen;
		}
	};

	class CBrush : public NSStructures::CBrush
	{
	public:
		CBrush() : NSStructures::CBrush()
		{
		}
		CBrush( const CBrush& other ) : NSStructures::CBrush(other)
		{
		}
		CBrush& operator=(const CBrush& other)
		{
			NSStructures::CBrush::operator=(other);
			return *this;
		}
		virtual ~CBrush()
		{
		}

		Gdiplus::Brush* GetBrush()
		{
			// create texture brush
			if (c_BrushTypeTexture == Type || c_BrushTypePattern == Type)
			{
				Gdiplus::TextureBrush* brush = GetTextureBrush();
				return brush;
			}
			// create hatch brush
			else if (Type >= c_BrushTypeHatch1 && Type <= c_BrushTypeHatch53)
			{
				Gdiplus::Color color1((BYTE)Alpha1, GetRValue(Color1), GetGValue(Color1), GetBValue(Color1));
				Gdiplus::Color color2((BYTE)Alpha2, GetRValue(Color2), GetGValue(Color2), GetBValue(Color2));
				Gdiplus::HatchBrush* brush = new Gdiplus::HatchBrush((Gdiplus::HatchStyle)(Type - c_BrushTypeHatch1), color1, color2);

				return brush;
			}
			// create gradient brush (Horizontal, Vertical, Diagonals)
			else if (Type >= c_BrushTypeHorizontal && Type <= c_BrushTypeDiagonal2)
			{
				Gdiplus::LinearGradientBrush* brush = GetLinearGradientBrushHVD(Type - c_BrushTypeHorizontal);
				return brush;
			}
			// create gradient brush (rect with center)
			else if ((c_BrushTypeCenter == Type) || (c_BrushTypePathGradient1 == Type))
			{
				Gdiplus::PathGradientBrush* brush = GetPathGradientBrushCenter();
				return brush;
			}
			// create path gradient brush 2 for letter
			if (c_BrushTypePathGradient2 == Type)
			{   
				Gdiplus::PathGradientBrush* brush = GetPathGradientBrush();
				return brush;
			}

			// create gradient brush (Cylinder Horizontal, Cylinder Vertical)
			else if (Type >= c_BrushTypeCylinderHor && Type <= c_BrushTypeCylinderVer)
			{
				Gdiplus::LinearGradientBrush* brush = GetLinearGradientBrushCHV(Type - c_BrushTypeCylinderHor);
				return brush;
			}

			// create solid brush
			// if (c_BrushTypeSolid == nBrushType)
			Gdiplus::Color color((BYTE)Alpha1, GetRValue(Color1), GetGValue(Color1), GetBValue(Color1));
			Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(color);
			return brush;
		}

		Gdiplus::ImageAttributes* GetImageAttributes( int nAlpha )
		{
			if( nAlpha < 0 || nAlpha >= 255 )
				return NULL;

			float alph = (float)(nAlpha / 255.0);
			Gdiplus::ColorMatrix colorMatrix = {	
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, alph, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f
			};

			Gdiplus::ImageAttributes* pImageAtt = new Gdiplus::ImageAttributes();
			if( pImageAtt )
				pImageAtt->SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

			return pImageAtt;
		}
		void SetTextureBrushTransform( Gdiplus::TextureBrush* pBrush )
		{
			if( !pBrush )
				return;

			Gdiplus::Image* pImage = pBrush->GetImage();
			Gdiplus::RectF  oImageRect;
			Gdiplus::Unit   oImageUnit;
			if( pImage )
			{
				pImage->GetBounds( &oImageRect, &oImageUnit );
				delete pImage;
			}

			pBrush->ResetTransform();

			Gdiplus::PointF oBrushOffset;
			oBrushOffset.X = Rect.X;
			oBrushOffset.Y = Rect.Y;

			switch(TextureMode)
			{
			case c_BrushTextureModeStretch:
				pBrush->ScaleTransform( Rect.Width / oImageRect.Width, Rect.Height / oImageRect.Height );
				break;
			
			case c_BrushTextureModeTileCenter:
				oBrushOffset.X += (Rect.Width - oImageRect.Width)/2;
				oBrushOffset.Y += (Rect.Height - oImageRect.Height)/2;
				pBrush->SetWrapMode( Gdiplus::WrapModeTile );
				break;

			default:
				pBrush->SetWrapMode( Gdiplus::WrapModeTile );
			}

			if( oBrushOffset.X != 0 || oBrushOffset.Y != 0)
			{
				pBrush->TranslateTransform( oBrushOffset.X, oBrushOffset.Y, Gdiplus::MatrixOrderAppend );
			}

		}
		// create texture brush
		Gdiplus::TextureBrush* GetTextureBrush()
		{
			// create texture brush with textureAlpha
			Gdiplus::TextureBrush* pBrush = NULL;

			BSTR bsPath = TexturePath.AllocSysString();
			Gdiplus::Bitmap oTextureBitmap(bsPath);
			SysFreeString(bsPath);
			
			// пытаемся создать текстуру с атрибутами
			Gdiplus::ImageAttributes* pImageAttr = GetImageAttributes( Alpha1 );
			pBrush = new Gdiplus::TextureBrush(&oTextureBitmap, Gdiplus::Rect(0, 0, oTextureBitmap.GetWidth(), oTextureBitmap.GetHeight()), pImageAttr);

			RELEASEOBJECT(pImageAttr);

			SetTextureBrushTransform( pBrush );

			return pBrush;
		}
		// create gradient brush (Horizontal, Vertical, Diagonals)
		Gdiplus::LinearGradientBrush* GetLinearGradientBrushHVD(int type)
		{
			Gdiplus::Color color1((BYTE)Alpha1, GetRValue(Color1), GetGValue(Color1), GetBValue(Color1));
			Gdiplus::Color color2((BYTE)Alpha2, GetRValue(Color2), GetGValue(Color2), GetBValue(Color2));
			Gdiplus::LinearGradientBrush* brush = new Gdiplus::LinearGradientBrush(Rect, color1, color2, (Gdiplus::LinearGradientMode)type);
			if( brush )
			{
				brush->SetWrapMode( Gdiplus::WrapModeTileFlipXY );
			}
			// TODO: brush->SetGammaCorrection(TRUE);
			return brush;
		}
		// create gradient brush (Cylinder Horizontal, Cylinder Vertical)
		Gdiplus::LinearGradientBrush* GetLinearGradientBrushCHV(int type)
		{
			Gdiplus::Color color1((BYTE)Alpha1, GetRValue(Color1), GetGValue(Color1), GetBValue(Color1));
			Gdiplus::Color color2((BYTE)Alpha2, GetRValue(Color2), GetGValue(Color2), GetBValue(Color2));
			
			float angle = 0;

			switch(type)
			{
			case 0: angle = -90; break;
			case 1: angle = -180; break;
			}

			Gdiplus::LinearGradientBrush* brush = new Gdiplus::LinearGradientBrush(Rect, color1, color2, angle);
			if( brush )
			{
				brush->SetBlendBellShape(0.70f);
				brush->SetWrapMode( Gdiplus::WrapModeTileFlipXY );
			}
			return brush;
		}
		// create gradient brush (rect with center)
		Gdiplus::PathGradientBrush* GetPathGradientBrushCenter()
		{
			Gdiplus::Color color1((BYTE)Alpha1, GetRValue(Color1), GetGValue(Color1), GetBValue(Color1));
			Gdiplus::Color color2((BYTE)Alpha2, GetRValue(Color2), GetGValue(Color2), GetBValue(Color2));
			Gdiplus::GraphicsPath path; path.AddRectangle(Rect);
			Gdiplus::PathGradientBrush* brush = new Gdiplus::PathGradientBrush(&path);
			if( brush )
			{
				Gdiplus::Color colors[] = {color1};
				int count = 1;
				brush->SetSurroundColors(colors, &count);
				brush->SetCenterColor(color2);
				brush->SetWrapMode( Gdiplus::WrapModeTileFlipXY );
				// TODO: need add Center coordinate 
			}
			return brush;
		}
		// create path gradient brush2 for letter
		Gdiplus::PathGradientBrush* GetPathGradientBrush(int index = 1)
		{
			Gdiplus::Color color1((BYTE)Alpha1, GetRValue(Color1), GetGValue(Color1), GetBValue(Color1));
			Gdiplus::Color color2((BYTE)Alpha2, GetRValue(Color2), GetGValue(Color2), GetBValue(Color2));
			Gdiplus::GraphicsPath path; path.AddRectangle(Rect);
			Gdiplus::PathGradientBrush* brush = new Gdiplus::PathGradientBrush(&path);

			if( brush )
			{
				int colorsCount = 4;
				Gdiplus::Color colors1[] = {color1, color2, color2, color1};
				Gdiplus::Color colors2[] = {color2, color1, color1, color2};
				Gdiplus::Color* pColors = (index % 2) ? colors2 : colors1;
				Gdiplus::Color& oColor = (index % 2) ? color2 : color1;

				brush->SetSurroundColors(pColors, &colorsCount);
				brush->SetCenterColor(oColor);
				brush->SetWrapMode( Gdiplus::WrapModeTileFlipXY );
			}
			return brush;
		}
	};

	class CFont : public NSStructures::CFont
	{
	public:
		CFont() : NSStructures::CFont()
		{
		}
		CFont( const CFont& other ) : NSStructures::CFont(other)
		{
		}
		CFont& operator=(const CFont& other)
		{
			NSStructures::CFont::operator=(other);
			return *this;
		}
		virtual ~CFont()
		{
		}

		Gdiplus::Font* GetFont(Gdiplus::Unit units = Gdiplus::UnitPixel)
		{
			BSTR family = Name.AllocSysString();

			int style = Gdiplus::FontStyleRegular;

			if (Bold)
				style |= Gdiplus::FontStyleBold;
			if (Italic)
				style |= Gdiplus::FontStyleItalic;
			if (Underline)
				style |= Gdiplus::FontStyleUnderline;
			if (Strikeout)
				style |= Gdiplus::FontStyleStrikeout;

			Gdiplus::Font* font = new Gdiplus::Font(family, (float)Size, style, units);

			SysFreeString(family);
			return font;
		}

		Gdiplus::Font* GetFontBySize(double dSize, Gdiplus::Unit units = Gdiplus::UnitPixel)
		{
			BSTR family = Name.AllocSysString();

			int style = Gdiplus::FontStyleRegular;

			if (Bold)
				style |= Gdiplus::FontStyleBold;
			if (Italic)
				style |= Gdiplus::FontStyleItalic;
			if (Underline)
				style |= Gdiplus::FontStyleUnderline;
			if (Strikeout)
				style |= Gdiplus::FontStyleStrikeout;

			Gdiplus::Font* font = new Gdiplus::Font(family, (float)dSize, style, units);

			SysFreeString(family);
			return font;
		}
	};

	class CShadow : public NSStructures::CShadow
	{
	public:
		CShadow() : NSStructures::CShadow()
		{
		}
		CShadow( const CShadow& other ) : NSStructures::CShadow(other)
		{
		}
		CShadow& operator=(const CShadow& other)
		{
			NSStructures::CShadow::operator=(other);
			return *this;
		}
		virtual ~CShadow()
		{
		}

		void Draw(Gdiplus::Graphics* Gr, Gdiplus::Font* font, Gdiplus::StringFormat* format, BSTR Text, int len, Gdiplus::RectF& rectDraw, BYTE alpha = 0xFF)
		{
			if (!Visible || !Gr)
				return;

			Gdiplus::RectF shadowRect = rectDraw;
			shadowRect.X += float(DistanceX);
			shadowRect.Y += float(DistanceY);

			Gdiplus::Color color((BYTE)(Alpha*alpha/255), GetRValue(Color), GetGValue(Color), GetBValue(Color));
			Gdiplus::SolidBrush solid(color);

			if (BlurSize) 
			{	
				Gdiplus::RectF temp = shadowRect;
				int nBlurSize = int(BlurSize);
				for(int i = 1; i <= nBlurSize + 1; i++) 
				{
					int iAlpha = (int)((double)((Alpha*alpha/255.)/8/(i)/1.1));
					solid.SetColor(Gdiplus::Color((BYTE)iAlpha, GetRValue(Color), GetGValue(Color), GetBValue(Color)));
					DrawBlurString(i, Gr, font, format, &solid, Text, len, temp);
					temp = shadowRect;
				}
			}
			else
				Gr->DrawString(Text, len, font, shadowRect, format, &solid);
		}
	};

	class CBackground : public NSStructures::CBackground
	{
	public:
		CBackground() : NSStructures::CBackground()
		{
		}
		CBackground( const CBackground& other ) : NSStructures::CBackground(other)
		{
		}
		CBackground& operator=(const CBackground& other)
		{
			NSStructures::CBackground::operator=(other);
			return *this;
		}
		virtual ~CBackground()
		{
		}

		void Draw(Gdiplus::Graphics* Gr, Gdiplus::Font* font, Gdiplus::StringFormat* format, BSTR Text, int len, Gdiplus::RectF& rectDraw, BYTE alpha = 0xFF)
		{			
		}
	};

	class CEdgeText : public NSStructures::CEdgeText
	{
	public:
		CEdgeText() : NSStructures::CEdgeText()
		{
		}
		CEdgeText( const CEdgeText& other ) : NSStructures::CEdgeText(other)
		{
		}
		CEdgeText& operator=(const CEdgeText& other)
		{
			NSStructures::CEdgeText::operator=(other);
			return *this;
		}
		virtual ~CEdgeText()
		{
		}

		void Draw(Gdiplus::Graphics* Gr, Gdiplus::Font* font, Gdiplus::StringFormat* format, BSTR Text, int len, Gdiplus::RectF& rectDraw, BYTE alpha = 0xFF)
		{
			if (!Visible || !Gr)
				return;

			if (Visible == -1)
			{
				Gdiplus::Color color((BYTE)((double)(Alpha*alpha/255.), GetRValue(Color), GetGValue(Color), GetBValue(Color)));
				Gdiplus::SolidBrush solid(color);
				Gdiplus::RectF temp = rectDraw;
				for(int i = 1; i <= Dist; i++) 
				{
					int iAlpha = (int)((double)(Alpha*alpha/255./8/(i)/1.1));
					solid.SetColor(Gdiplus::Color((BYTE)iAlpha, GetRValue(Color), GetGValue(Color), GetBValue(Color)));
					DrawBlurString(i, Gr, font, format, &solid, Text, len, temp);
					temp = rectDraw;
				}
			}
			if (Visible == 1)
			{
				// save old smoothing mode and set high quality mode
				Gdiplus::SmoothingMode oldSmoothingMode = Gr->GetSmoothingMode();
				Gr->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

				// get glyphs path 
				Gdiplus::FontFamily family; font->GetFamily(&family);
				Gdiplus::GraphicsPath path; path.AddString(Text, len, &family, font->GetStyle(), font->GetSize(), rectDraw, format);

				// draw text
				int offset = int(Dist);
				for (int i = 1; i <= offset; i++)
				{
					Gdiplus::Pen pen(Gdiplus::Color((BYTE)(Alpha*i*alpha/(offset*255)), GetRValue(Color), GetGValue(Color), GetBValue(Color)), (float) 2*(offset - i + 1));
					pen.SetLineCap(Gdiplus::LineCapRound, Gdiplus::LineCapRound, Gdiplus::DashCapRound);
					Gr->DrawPath(&pen, &path);
				}

				// restore old smoothing mode
				Gr->SetSmoothingMode(oldSmoothingMode);
			}
		}
	};
}