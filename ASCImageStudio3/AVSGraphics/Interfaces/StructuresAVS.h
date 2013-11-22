#pragma once

#include "..\Objects\Structures.h"
#include "..\agg\AVSWrapper\Graphics.h"

namespace NSStructuresAVS
{
	inline void DrawBlurString(int currentBlurSize, Aggplus::CGraphics* Gr, IAVSFontManager* font, 
		Aggplus::CStringFormat* format, Aggplus::CBrush* brush, BSTR Text, double x, double y, double width, double height)
	{
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			y += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x -= 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x -= 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			y -= 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			y -= 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			y += 1;
			Gr->DrawString(Text, font, brush, x, y);
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

		Aggplus::CBrush* GetBrush()
		{
			if ((0 == Type) || (c_BrushTypeSolid == Type) || (c_BrushTypeHatch1 <= Type && c_BrushTypeHatch53 >= Type))
			{
				Aggplus::CColor oColor((BYTE)Alpha1, Color1);
				Aggplus::CBrushSolid* pNew = new Aggplus::CBrushSolid(oColor);

				return pNew;
			}
			else if	   ((c_BrushTypeHorizontal		== Type) ||
						(c_BrushTypeVertical		== Type) ||
						(c_BrushTypeDiagonal1		== Type) ||
						(c_BrushTypeDiagonal2		== Type) ||
						(c_BrushTypeCenter			== Type) ||
						(c_BrushTypePathGradient1	== Type) ||
						(c_BrushTypePathGradient2	== Type) ||
						(c_BrushTypeCylinderHor		== Type) ||
						(c_BrushTypeCylinderVer		== Type))
			{
				Aggplus::CColor o1((BYTE)Alpha1, Color1);
				Aggplus::CColor o2((BYTE)Alpha2, Color2);

				Aggplus::CBrushLinearGradient* pNew = new Aggplus::CBrushLinearGradient( Aggplus::RectF(0, 0, 1, 1), o1, o2, LinearAngle, TRUE );
				if( pNew )
				{
					pNew->SetRelativeCoords( TRUE );

					int nCountSubColors = m_arrSubColors.GetSize();
					if( nCountSubColors > 0 )
					{
						Aggplus::CColor* pColors = new Aggplus::CColor[nCountSubColors];
						float* pBlends = new float[nCountSubColors];

						if( pColors && pBlends )
						{
							for( int i = 0; i < nCountSubColors; i++ )
							{
								pColors[i] = Aggplus::CColor(255, m_arrSubColors[i].color);
								pBlends[i] = (float)(m_arrSubColors[i].position / 65536.0);
							}

							pNew->SetInterpolationColors( pColors, pBlends, nCountSubColors );
						}

						delete [] pColors;
						delete [] pBlends;
					}
				}

				return pNew;
			}
			else
			{
				Aggplus::CBrushTexture* pNew = new Aggplus::CBrushTexture(TexturePath, Aggplus::WrapModeTile/*(Aggplus::WrapMode)TextureMode*/);
				if( pNew )
				{
					if( Type == c_BrushTypePattern )
					{
						pNew->m_bUsePattern = TRUE;
					}
				}

				return pNew;
			}
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

		void Draw(Aggplus::CGraphics* Gr, IAVSFontManager* font, BSTR Text, double x, double y, double width, double height, BYTE alpha = 0xFF)
		{
			if (!Visible || !Gr)
				return;

			x += float(DistanceX);
			y += float(DistanceY);

			Aggplus::CColor color((BYTE)(Alpha*alpha/255), GetRValue(Color), GetGValue(Color), GetBValue(Color));
			Aggplus::CBrushSolid solid(color);

			Aggplus::CStringFormat oFormat;

			if (BlurSize) 
			{	
				double _x = x;
				double _y = y;
				int nBlurSize = int(BlurSize);
				for(int i = 1; i <= nBlurSize + 1; i++) 
				{
					int iAlpha = (int)((double)((Alpha*alpha/255.)/8/(i)/1.1));
					solid.SetColor(Aggplus::CColor((BYTE)iAlpha, Color));
					DrawBlurString(i, Gr, font, &oFormat, &solid, Text, _x, _y, width, height);
					_x = x;
					_y = y;
				}
			}
			else
				Gr->DrawString(Text, font, &solid, x, y);
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

		void Draw(Aggplus::CGraphics* Gr, IAVSFontManager* font, BSTR Text, double x, double y, double width, double height, BYTE alpha = 0xFF)
		{
			if (!Visible || !Gr)
				return;

			Aggplus::CStringFormat format;
			
			if (Visible == -1)
			{
				Aggplus::CColor color((BYTE)((double)(Alpha*alpha/255.), Color));
				Aggplus::CBrushSolid solid(color);
				
				double _x = x;
				double _y = y;
				for(int i = 1; i <= Dist; i++) 
				{
					int iAlpha = (int)((double)(Alpha*alpha/255./8/(i)/1.1));
					solid.SetColor(Aggplus::CColor((BYTE)iAlpha, Color));
					DrawBlurString(i, Gr, font, &format, &solid, Text, _x, _y, width, height);
					_x = x;
					_y = y;
				}
			}
			if (Visible == 1)
			{
				// draw text
				int offset = int(Dist);
				Aggplus::CGraphicsPath path; path.AddString(Text, font, x, y);

				for (int i = 1; i <= offset; i++)
				{
					NSStructures::CPen oPen;
					oPen.Alpha = (LONG)(Alpha*i*alpha/(offset*255));
					oPen.Color = Color;
					oPen.Size = 2*(offset - i + 1);
					
					Gr->DrawPath(&oPen, &path);
				}
			}
		}
	};
}