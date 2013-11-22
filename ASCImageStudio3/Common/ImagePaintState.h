#pragma once

#include <PaintStruct.h>
#include <ImageSerializeObjects.h>

namespace ImageStudio
{
	namespace Paint
	{
		class CPaintState
		{
		public:

			CPaintState()
			{
				m_dTextAngle = 0.0;
			}
			virtual ~CPaintState()
			{
			}
			
			BOOL GetPenAntiAlias() const
			{
				return m_oFormat.GetAntiAliasPen();
			}
			BOOL GetBrushAntiAlias() const
			{
				return m_oFormat.GetAntiAliasBrush();
			}
			BOOL GetTextAntiAlias() const
			{
				return m_oFormat.GetAntiAliasText();
			}
			long GetStringAlignmentVertical() const
			{
				return m_oFormat.GetStringAlignmentVertical();
			}
			long GetStringAlignmentHorizontal() const
			{
				return m_oFormat.GetStringAlignmentHorizontal();
			}
			long GetImageAlphaMode() const
			{
				return m_oFormat.GetImageAlphaMode();
			}
			double GetTextAngle() const
			{
				return m_dTextAngle;
			}
			
			Painter::CPen& GetPen()
			{
				return m_oPen;
			}
			Painter::CBrush& GetBrush()
			{
				return m_oBrush;
			}
			Painter::CFont& GetFont()
			{
				return m_oFont;
			}
			Painter::CFormat& GetFormat()
			{
				return m_oFormat;
			}
			Painter::CShadow& GetShadow()
			{
				return m_oShadow;
			}
			Painter::CEdgeText& GetEdge()
			{
				return m_oEdge;
			}
			
			void SetPen(const Painter::CPen& oPen)
			{
				m_oPen.FromPen(oPen);
			}
			void SetPen(const ImageStudio::Serialize::Paint::Structures::Pen& oPen)
			{
				if (oPen.IsValidColor())		m_oPen.SetColor(oPen.GetColor());
				if (oPen.IsValidAlpha())		m_oPen.SetAlpha(oPen.GetAlpha());
				if (oPen.IsValidSize())			m_oPen.SetSize(oPen.GetSize());
				if (oPen.IsValidAntiAliasPen())	m_oFormat.SetAntiAliasPen(oPen.GetAntiAliasPen());
				
				m_oPen.SetAlignment( oPen.GetAlignment() );
			}
			virtual void SetBrush(const Painter::CBrush& oBrush)
			{
				m_oBrush.FromBrush(oBrush);
			}
			virtual void SetBrush(const ImageStudio::Serialize::Paint::Structures::Brush& oBrush)
			{
				if (oBrush.IsValidTexturePath())	m_oBrush.SetTexturePath(oBrush.GetTexturePath());
				if (oBrush.IsValidTextureAlpha())	m_oBrush.SetTextureAlpha(oBrush.GetTextureAlpha());
				if (oBrush.IsValidTextureMode()) 	m_oBrush.SetTextureMode(oBrush.GetTextureMode());
				if (oBrush.IsValidColor1())		 	m_oBrush.SetColor1(oBrush.GetColor1());
				if (oBrush.IsValidColor2())		 	m_oBrush.SetColor2(oBrush.GetColor2());
				if (oBrush.IsValidAlpha1())		 	m_oBrush.SetAlpha1(oBrush.GetAlpha1());
				if (oBrush.IsValidAlpha2())		 	m_oBrush.SetAlpha2(oBrush.GetAlpha2());
				if (oBrush.IsValidGradientAngle())	m_oBrush.SetGradientAngle(oBrush.GetGradientAngle());
				if (oBrush.IsValidType())		 	m_oBrush.SetType(oBrush.GetType());

				if (oBrush.IsValidRectable() && oBrush.IsValidRect())
				{
					if (oBrush.GetRectable())
						m_oBrush.SetRect(&oBrush.GetRect());
					else
						m_oBrush.SetRect(NULL);
				}
			}
			void SetFont(const Painter::CFont& oFont)
			{
				m_oFont.FromFont(oFont);
			}
			void SetFont(const ImageStudio::Serialize::Paint::Structures::Font& oFont)
			{
				if (oFont.IsValidName())						m_oFont.SetName(oFont.GetName());
				if (oFont.IsValidSize())						m_oFont.SetFontSize(oFont.GetSize());
				if (oFont.IsValidBold())						m_oFont.SetBold(oFont.GetBold());
				if (oFont.IsValidItalic())						m_oFont.SetItalic(oFont.GetItalic());
				if (oFont.IsValidUnderline())					m_oFont.SetUnderline(oFont.GetUnderline());
				if (oFont.IsValidStrikeout())					m_oFont.SetStrikeout(oFont.GetStrikeout());
				if (oFont.IsValidAntiAliasText())				m_oFormat.SetAntiAliasText(oFont.GetAntiAliasText());
				if (oFont.IsValidStringAlignmentVertical())		m_oFormat.SetStringAlignmentVertical(oFont.GetStringAlignmentVertical());
				if (oFont.IsValidStringAlignmentHorizontal())	m_oFormat.SetStringAlignmentHorizontal(oFont.GetStringAlignmentHorizontal());
				if (oFont.IsValidAngle())						m_dTextAngle = oFont.GetAngle();
			}
			void SetFormat(const Painter::CFormat& oFormat)
			{
				m_oFormat.FromFormat(oFormat);
			}
			void SetShadow(const Painter::CShadow& oShadow)
			{
				m_oShadow.FromShadow(oShadow);
			}
			void SetShadow(const ImageStudio::Serialize::Paint::Structures::Shadow& oShadow)
			{
				if (oShadow.IsValidVisible())		m_oShadow.SetVisible(oShadow.GetVisible());
				if (oShadow.IsValidDistanceX())		m_oShadow.SetDistanceX(oShadow.GetDistanceX());
				if (oShadow.IsValidDistanceY())		m_oShadow.SetDistanceY(oShadow.GetDistanceY());
				if (oShadow.IsValidBlurSize())		m_oShadow.SetBlurSize(oShadow.GetBlurSize());
				if (oShadow.IsValidColor())			m_oShadow.SetColor(oShadow.GetColor());
				if (oShadow.IsValidAlpha())			m_oShadow.SetAlpha(oShadow.GetAlpha());
			}
			void SetEdge(const Painter::CEdgeText& oEdge)
			{
				m_oEdge.FromEdge(oEdge);
			}
			void SetEdge(const ImageStudio::Serialize::Paint::Structures::Edge& oEdge)
			{
				if (oEdge.IsValidVisible())			m_oEdge.SetVisible(oEdge.GetVisible());
				if (oEdge.IsValidDist())			m_oEdge.SetDistance(oEdge.GetDist());
				if (oEdge.IsValidColor())			m_oEdge.SetColor(oEdge.GetColor());
				if (oEdge.IsValidAlpha())			m_oEdge.SetAlpha(oEdge.GetAlpha());
			}
			void SetTextAngle(double dTextAngle)
			{
				m_dTextAngle = dTextAngle;
			}
			
			void FromState(CPaintState& oState)
			{
				SetPen(oState.GetPen());
				SetBrush(oState.GetBrush());
				SetFont(oState.GetFont());
				SetFormat(oState.GetFormat());
				SetShadow(oState.GetShadow());
				SetEdge(oState.GetEdge());
				SetTextAngle(oState.GetTextAngle());
			}
						
		protected:

			// текущие переменные и объекты, которые могут быть использованы при отрисовке
			Painter::CPen m_oPen;
			Painter::CBrush m_oBrush;
			Painter::CFont m_oFont;
			Painter::CFormat m_oFormat;
			Painter::CShadow m_oShadow;
			Painter::CEdgeText m_oEdge;

			double m_dTextAngle; // угол вращения текста, TODO: а нужен ли он тут ?
		};
		class CPaintStateManager : public CPaintState
		{
		public:

			CPaintStateManager()
			{
			}
			virtual ~CPaintStateManager()
			{
			}

			Painter::CTextureManager& GetTextureManager()
			{
				return m_oTextures;
			}
			
			virtual void SetBrush(const Painter::CBrush& oBrush, BOOL bVerticalFlip)
			{
				CPaintState::SetBrush(oBrush);

				PreloadBrushTexture(bVerticalFlip);
			}
			virtual void SetBrush(const ImageStudio::Serialize::Paint::Structures::Brush& oBrush, BOOL bVerticalFlip)
			{
				CPaintState::SetBrush(oBrush);

				PreloadBrushTexture(bVerticalFlip);
			}
			
			void SaveState()
			{
				CPaintState oState;

				oState.FromState(*this);

				m_arrStates.Add(oState);
			}
			void RestoreState()
			{
				if (m_arrStates.GetCount() < 0)
					return;

				FromState(m_arrStates[m_arrStates.GetCount() - 1]);

				m_arrStates.RemoveAt(m_arrStates.GetCount() - 1);
			}

		protected:

			void PreloadBrushTexture(BOOL bFlip)
			{
				if (GetBrush().GetType() == Painter::c_BrushTypeTexture)
				{
					int nTextureIndex = m_oTextures.Find(GetBrush().GetTexturePath());

					if (nTextureIndex < 0)
						m_oTextures.Add(GetBrush().GetTexturePath(), bFlip);
					else
					{
						Painter::CTexture* pTexture = m_oTextures[nTextureIndex];
						if( pTexture )
						{
							if( pTexture->bFlipVertical != bFlip )
							{
								if( pTexture->Bitmap )
									pTexture->Bitmap->RotateFlip( Gdiplus::RotateNoneFlipY );

								pTexture->bFlipVertical = bFlip;
							}
						}
					}
				}
			}
			
		protected:
			
			// сохраненные состояния
			CAtlArray<CPaintState> m_arrStates;

			// менеджер текстур (желательно, чтобы он был один во все время работы программы)
			Painter::CTextureManager m_oTextures;
		};
	}
}