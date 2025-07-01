#include "CWmfParserBase.h"

#include "../WmfInterpretator/CWmfInterpretatorRender.h"
#include "../WmfInterpretator/CWmfInterpretatorSvg.h"

#include "../../Emf/EmfParser/CEmfParser.h"
#include "../../Emf/EmfInterpretator/CEmfInterpretatorSvg.h"

namespace MetaFile
{
	CWmfParserBase::CWmfParserBase()
		: m_oPlayer(this), m_oBoundingBox(0, 0, 1, 1), m_pInterpretator(NULL), m_bEof(false)
	{
		m_pDC = m_oPlayer.GetDC();
	}

	CWmfParserBase::~CWmfParserBase()
	{}

	void CWmfParserBase::PlayMetaFile()
	{

	}

	void CWmfParserBase::ClearFile()
	{
		m_oPlayer.Clear();
		m_pDC = m_oPlayer.GetDC();
		m_bEof = false;
	}

	const TRectL& CWmfParserBase::GetDCBounds() const
	{
		return m_oDCRect;
	}
	
	const CClip *CWmfParserBase::GetClip() const
	{
		return m_pDC->GetClip();
	}

	double CWmfParserBase::GetPixelHeight() const
	{
		return m_pDC->GetPixelHeight();
	}

	double CWmfParserBase::GetPixelWidth() const
	{
		return m_pDC->GetPixelWidth();
	}

	int CWmfParserBase::GetTextColor() const
	{
		return m_pDC->GetTextColor().ToInt();
	}

	const IFont *CWmfParserBase::GetFont() const
	{
		return m_pDC->GetFont();
	}

	const IBrush *CWmfParserBase::GetBrush() const
	{
		return m_pDC->GetBrush();
	}

	const IPen *CWmfParserBase::GetPen() const
	{
		return m_pDC->GetPen();
	}

	unsigned int CWmfParserBase::GetTextAlign() const
	{
		return (unsigned int)m_pDC->GetTextAlign();
	}

	unsigned int CWmfParserBase::GetTextBgMode() const
	{
		return (unsigned int)m_pDC->GetTextBgMode();
	}

	int CWmfParserBase::GetTextBgColor() const
	{
		return m_pDC->GetTextBgColor().ToInt();
	}

	unsigned int CWmfParserBase::GetFillMode() const
	{
		return (unsigned int)m_pDC->GetPolyFillMode();
	}

	TPointD CWmfParserBase::GetCurPos() const
	{
		TPointL oPoint = m_pDC->GetCurPos();
		double dX, dY;
		TranslatePoint(oPoint.X, oPoint.Y, dX, dY);
		return TPointD(dX, dY);
	}

	const TXForm& CWmfParserBase::GetInverseTransform() const
	{
		return m_pDC->GetInverseTransform();
	}

	const TXForm& CWmfParserBase::GetTransform(int iGraphicsMode)
	{
		const TRectL& oBounds{GetDCBounds()};
		double dT = oBounds.Top;
		double dL = oBounds.Left;

		TXForm oShiftXForm(1, 0, 0, 1, -dL, -dT);
		m_oTransform.Copy(m_pDC->GetFinalTransform(iGraphicsMode));
		m_oTransform.Multiply(oShiftXForm, MWT_RIGHTMULTIPLY);
		return m_oTransform;
	}

	unsigned int CWmfParserBase::GetMiterLimit() const
	{
		return m_pDC->GetMiterLimit();
	}

	unsigned int CWmfParserBase::GetRop2Mode() const
	{
		return (unsigned int)m_pDC->GetRop2Mode();
	}

	int CWmfParserBase::GetCharSpace() const
	{
		return m_pDC->GetCharSpacing();
	}

	bool CWmfParserBase::IsWindowFlippedY() const
	{
		return m_pDC->GetWindow().h < 0;
	}

	bool CWmfParserBase::IsWindowFlippedX() const
	{
		return m_pDC->GetWindow().w < 0;
	}

	unsigned int CWmfParserBase::GetMapMode() const
	{
		return m_pDC->GetMapMode();
	}

	USHORT CWmfParserBase::GetDpi() const
	{
		return (0 != m_oPlaceable.ushInch) ? m_oPlaceable.ushInch : 96;
	}

	const IRegion *CWmfParserBase::GetRegion() const
	{
		return m_pDC->GetRegion();
	}

	unsigned int CWmfParserBase::GetArcDirection() const
	{
		return AD_CLOCKWISE;
	}
	
	const CPath *CWmfParserBase::GetPath() const
	{
		return NULL;
	}

	void CWmfParserBase::SetInterpretator(IOutputDevice *pOutput)
	{
		if (NULL != m_pInterpretator)
			delete m_pInterpretator;

		m_pInterpretator = new CWmfInterpretatorRender(pOutput);
	}

	void CWmfParserBase::SetInterpretator(const wchar_t *wsFilePath, InterpretatorType oInterpretatorType, unsigned int unWidth, unsigned int unHeight)
	{
		RELEASEOBJECT(m_pInterpretator);

		//TODO:: добавить как появится реализация интерпретаторов
	}

	CWmfInterpretatorBase* CWmfParserBase::GetInterpretator()
	{
		return m_pInterpretator;
	}

	CWmfDC *CWmfParserBase::GetDC()
	{
		return m_pDC;
	}

	void CWmfParserBase::SetInterpretator(InterpretatorType oInterpretatorType, unsigned int unWidth, unsigned int unHeight)
	{
		RELEASEOBJECT(m_pInterpretator);

		if (InterpretatorType::Svg == oInterpretatorType)
		{
			CWmfInterpretatorSvg *pWmfInterpretatorSvg = new CWmfInterpretatorSvg(this, unWidth, unHeight);
			pWmfInterpretatorSvg->SetShapeRendering(EShapeRendering::OptimizeSpeed);
			m_pInterpretator = pWmfInterpretatorSvg;
		}
	}

	void CWmfParserBase::SetInterpretator(IOutputDevice *pOutput, const wchar_t *wsFilePath)
	{
		RELEASEOBJECT(m_pInterpretator);
	}

	TRectD CWmfParserBase::GetBounds()
	{
		TRectD oBounds(m_oDCRect);
		return oBounds;
	}

	void CWmfParserBase::SkipVoid()
	{
		do
		{
		} while (m_oStream.ReadChar() == 0);

		m_oStream.SeekBack(1);
	}

	void CWmfParserBase::TranslatePoint(short shX, short shY, double &dX, double &dY) const
	{
		dX = (double)shX;
		dY = (double)shY;
	}

	void CWmfParserBase::UpdateDCRect()
	{
		if (!IsPlaceable())
		{
			m_oDCRect = m_oBoundingBox;
			return;
		}

		m_oDCRect = m_oPlaceable.oBoundingBox;

		if (96 != m_oPlaceable.ushInch)
		{
			const double dKoef = 96. / (double)m_oPlaceable.ushInch;

			m_oDCRect.Left   = static_cast<int>(std::round(m_oDCRect.Left   * dKoef));
			m_oDCRect.Top    = static_cast<int>(std::round(m_oDCRect.Top    * dKoef));
			m_oDCRect.Right  = static_cast<int>(std::round(m_oDCRect.Right  * dKoef));
			m_oDCRect.Bottom = static_cast<int>(std::round(m_oDCRect.Bottom * dKoef));
		}

		// Иногда m_oPlaceable.BoundingBox задается нулевой ширины и высоты
		if (abs(m_oDCRect.Right - m_oDCRect.Left) <= 1)
		{
			m_oDCRect.Right = m_oBoundingBox.Right;
			m_oDCRect.Left  = m_oBoundingBox.Left;
		}
		if (abs(m_oDCRect.Bottom - m_oDCRect.Top) <= 1)
		{
			m_oDCRect.Top    = m_oBoundingBox.Top;
			m_oDCRect.Bottom = m_oBoundingBox.Bottom;
		}
	}

	bool CWmfParserBase::IsPlaceable() const
	{
		return (0x9AC6CDD7 == m_oPlaceable.unKey);
	}

	int CWmfParserBase::GetRecordRemainingBytesCount() const
	{
		const unsigned int unReadedSize = m_oStream.Tell() - m_unRecordPos;
		return (m_unRecordSize - unReadedSize);
	}

	double CWmfParserBase::GetSweepAngle(const double &dStartAngle, const double &dEndAngle) const
	{
		return (dEndAngle - dStartAngle);
	}

	void CWmfParserBase::MoveTo(short shX, short shY)
	{
		if (NULL != m_pInterpretator)
		{
			double dX, dY;
			TranslatePoint(shX, shY, dX, dY);
			m_pInterpretator->MoveTo(dX, dY);
		}
		else
		{
			RegisterPoint(shX, shY);
		}

		m_pDC->SetCurPos(shX, shY);
	}

	void CWmfParserBase::MoveToD(double dX, double dY)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->MoveTo(dX, dY);
		else
			RegisterPoint(static_cast<short>(dX), static_cast<short>(dY));

		m_pDC->SetCurPos(static_cast<short>(dX), static_cast<short>(dY));
	}

	void CWmfParserBase::LineTo(short shX, short shY)
	{
		if (NULL != m_pInterpretator)
		{
			double dX, dY;
			TranslatePoint(shX, shY, dX, dY);
			m_pInterpretator->LineTo(dX, dY);
		}
		else
		{
			RegisterPoint(shX, shY);
		}
		m_pDC->SetCurPos(shX, shY);
	}

	void CWmfParserBase::ArcTo(short shL, short shT, short shR, short shB, double dStart, double dSweep)
	{
		// Тут не делаем пересчет текущей точки, т.к. при вызове данной функции не всегда он нужен (например эллипс).
		// Текущая точка обновляется на том уровне, на котором вызывалась данная функция.
		if (NULL != m_pInterpretator)
		{
			double dL, dT, dR, dB;
			TranslatePoint(shL, shT, dL, dT);
			TranslatePoint(shR, shB, dR, dB);
			m_pInterpretator->ArcTo(dL, dT, dR, dB, dStart, dSweep);
		}
		else
		{
			// TODO: Возможно нужно регистрировать более точно
			RegisterPoint(shL, shT);
			RegisterPoint(shR, shB);
		}
	}

	void CWmfParserBase::ClosePath()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->ClosePath();
	}

	void CWmfParserBase::DrawPath(bool bStroke, bool bFill)
	{
		if (NULL != m_pInterpretator)
		{
			int lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
			m_pInterpretator->DrawPath(lType);
			m_pInterpretator->EndPath();
		}
	}

	void CWmfParserBase::DrawText(const unsigned char *pString, unsigned int unCharsCount, short _shX, short _shY, short *pDx)
	{
		int nX = _shX;
		int nY = _shY;

		if (m_pDC->GetTextAlign() & TA_UPDATECP && (0 == _shX && 0 == _shY))
		{
			nX = m_pDC->GetCurPos().X;
			nY = m_pDC->GetCurPos().Y;
		}

		const IFont* pFont = GetFont();
		std::wstring wsText = ConvertToUnicode(pString, (long)unCharsCount, (NULL != pFont) ? pFont->GetCharSet() : DEFAULT_CHARSET);

		if (NULL != m_pInterpretator)
		{
			double dX, dY;
			TranslatePoint(nX, nY, dX, dY);

			double* pdDx = NULL;
			if (NULL != pDx && unCharsCount == wsText.length())
			{
				pdDx = new double[unCharsCount];
				if (pdDx)
				{
					int nCurX = nX;
					double dCurX = dX;
					for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount; unCharIndex++)
					{
						int nX1 = nCurX + pDx[unCharIndex];
						double dX1, dY1;
						TranslatePoint(nX1, nY, dX1, dY1);

						pdDx[unCharIndex] = dX1 - dCurX;

						nCurX = nX1;
						dCurX = dX1;
					}
				}
			}

			m_pInterpretator->DrawString(wsText, unCharsCount, dX, dY, pdDx, 1, GetTransform().M11, GetTransform().M22);

			RELEASEARRAYOBJECTS(pdDx);
		}
		else
		{
			if (pFont)
			{
				// TODO: Здесь идет точное повторение кода из CMetaFileRenderer->DrawString
				//       неплохо бы перенести этот пересчет в базовый класс IMetaFileBase.
				NSFonts::IFontManager* pFontManager = GetFontManager();
				if (pFontManager)
				{
					int lLogicalFontHeight = static_cast<int>(pFont->GetHeight());
					if (lLogicalFontHeight < 0)
						lLogicalFontHeight = -lLogicalFontHeight;
					if (lLogicalFontHeight < 0.01)
						lLogicalFontHeight = 18;

					double dFontHeight = lLogicalFontHeight;

					std::wstring wsFaceName = pFont->GetFaceName();

					int lStyle = 0;
					if (pFont->GetWeight() > 550)
						lStyle |= 0x01;
					if (pFont->IsItalic())
						lStyle |= 0x02;

					float fL = 0, fT = 0, fW = 0, fH = 0;
					pFontManager->LoadFontByName(wsFaceName, dFontHeight, lStyle, 72, 72);
					pFontManager->SetCharSpacing(GetCharSpace());

					NSFonts::IFontFile* pFontFile = pFontManager->GetFile();
					double dFHeight  = pFontFile ? (dFontHeight * pFontFile->GetHeight() / pFontFile->Units_Per_Em()) : 0;
					double dFDescent = pFontFile ? (dFontHeight * pFontFile->GetDescender() / pFontFile->Units_Per_Em()) : 0;
					double dFAscent  = dFHeight - std::abs(dFDescent);

					if (NULL != pDx && unCharsCount > 1)
					{
						// Тогда мы складываем все pDx кроме последнего символа, последний считаем отдельно
						double dTempTextW = 0;
						for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount - 1; unCharIndex++)
						{
							dTempTextW += pDx[unCharIndex];
						}

						std::wstring wsTempText;
						wsTempText += wsText.at(wsText.length() - 1);
						//wsTempText += wsText.at(unCharsCount - 1);

						pFontManager->LoadString1(wsTempText, 0, 0);
						TBBox oBox = pFontManager->MeasureString2();
						dTempTextW += (oBox.fMaxX - oBox.fMinX);

						fW = (float)dTempTextW;
					}
					else
					{
						pFontManager->LoadString1(wsText, 0, 0);
						TBBox oBox = pFontManager->MeasureString2();
						fL = static_cast<float>(oBox.fMinX);
						fW = static_cast<float>(oBox.fMaxX - oBox.fMinX);
					}

					pFontManager->LoadString1(wsText, 0, 0);
					TBBox oBox = pFontManager->MeasureString2();
					fL = (float)(oBox.fMinX);
					fW = (float)(oBox.fMaxX - oBox.fMinX);

					fT = (float)-dFAscent;
					fH = (float)dFHeight;

					double dTheta = -((((double)pFont->GetEscapement()) / 10) * 3.14159265358979323846 / 180);
					double dCosTheta = (float)cos(dTheta);
					double dSinTheta = (float)sin(dTheta);

					double dX = (double)nX;
					double dY = (double)nY;

					// Найдем начальную точку текста
					unsigned int ulTextAlign = GetTextAlign();
					if (ulTextAlign & TA_BASELINE)
					{
						// Ничего не делаем
					}
					else if (ulTextAlign & TA_BOTTOM)
					{
						float fTemp = -(-fT + fH);

						dX += -fTemp * dSinTheta;
						dY +=  fTemp * dCosTheta;
					}
					else // if (ulTextAlign & TA_TOP)
					{
						float fTemp = -fT;

						dX += -fTemp * dSinTheta;
						dY +=  fTemp * dCosTheta;
					}

					if (ulTextAlign & TA_CENTER)
					{
						dX += -fW / 2 * dCosTheta;
						dY += -fW / 2 * dSinTheta;
					}
					else if (ulTextAlign & TA_RIGHT)
					{
						dX += -fW * dCosTheta;
						dY += -fW * dSinTheta;
					}
					else //if (ulTextAlign & TA_LEFT)
					{
						// Ничего не делаем
					}

					double dX0 = dX + fL, dY0 = dY + fT;
					double dX1 = dX + fL + fW, dY1 = dY + fT;
					double dX2 = dX + fL + fW, dY2 = dY + fT + fH;
					double dX3 = dX + fL, dY3 = dY + fT + fH;
					if (0 != pFont->GetEscapement())
					{
						TXForm oForm(dCosTheta, dSinTheta, -dSinTheta, dCosTheta, dX - dX * dCosTheta + dY * dSinTheta, dY - dX * dSinTheta - dY * dCosTheta);

						oForm.Apply(dX0, dY0);
						oForm.Apply(dX1, dY1);
						oForm.Apply(dX2, dY2);
						oForm.Apply(dX3, dY3);
					}

					RegisterPoint((short)dX0, (short)dY0);
					RegisterPoint((short)dX1, (short)dY1);
					RegisterPoint((short)dX2, (short)dY2);
					RegisterPoint((short)dX3, (short)dY3);
				}
				else
				{
					int lLogicalFontHeight = static_cast<int>(pFont->GetHeight());
					if (lLogicalFontHeight < 0)
						lLogicalFontHeight = -lLogicalFontHeight;
					if (lLogicalFontHeight < 0.01)
						lLogicalFontHeight = 18;

					double dFontHeight = lLogicalFontHeight;

					float fL = 0, fT = 0, fW = 0, fH = 0;

					if (NULL != pDx && unCharsCount > 1)
					{
						// Тогда мы складываем все pDx кроме последнего символа, последний считаем отдельно
						double dTempTextW = 0;
						for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount - 1; unCharIndex++)
						{
							dTempTextW += pDx[unCharIndex];
						}

						dTempTextW += dFontHeight * wsText.length();

						fW = (float)dTempTextW;
					}
					else
					{
						fW = (float)(dFontHeight * wsText.length());
					}

					fH = (float)dFontHeight * 1.2f;

					double dTheta = -((((double)pFont->GetEscapement()) / 10) * 3.14159265358979323846 / 180);
					double dCosTheta = (float)cos(dTheta);
					double dSinTheta = (float)sin(dTheta);

					double dX = (double)nX;
					double dY = (double)nY;

					// Найдем начальную точку текста
					unsigned int ulTextAlign = GetTextAlign();
					if (ulTextAlign & TA_BASELINE)
					{
						// Ничего не делаем
					}
					else if (ulTextAlign & TA_BOTTOM)
					{
						float fTemp = -(-fT + fH);

						dX += -fTemp * dSinTheta;
						dY +=  fTemp * dCosTheta;
					}
					else // if (ulTextAlign & TA_TOP)
					{
						float fTemp = -fT;

						dX += -fTemp * dSinTheta;
						dY +=  fTemp * dCosTheta;
					}

					if (ulTextAlign & TA_CENTER)
					{
						dX += -fW / 2 * dCosTheta;
						dY += -fW / 2 * dSinTheta;
					}
					else if (ulTextAlign & TA_RIGHT)
					{
						dX += -fW * dCosTheta;
						dY += -fW * dSinTheta;
					}
					else //if (ulTextAlign & TA_LEFT)
					{
						// Ничего не делаем
					}

					double dX0 = dX + fL, dY0 = dY + fT;
					double dX1 = dX + fL + fW, dY1 = dY + fT;
					double dX2 = dX + fL + fW, dY2 = dY + fT + fH;
					double dX3 = dX + fL, dY3 = dY + fT + fH;
					if (0 != pFont->GetEscapement())
					{
						TXForm oForm(dCosTheta, dSinTheta, -dSinTheta, dCosTheta, dX - dX * dCosTheta + dY * dSinTheta, dY - dX * dSinTheta - dY * dCosTheta);

						oForm.Apply(dX0, dY0);
						oForm.Apply(dX1, dY1);
						oForm.Apply(dX2, dY2);
						oForm.Apply(dX3, dY3);
					}

					RegisterPoint((short)dX0, (short)dY0);
					RegisterPoint((short)dX1, (short)dY1);
					RegisterPoint((short)dX2, (short)dY2);
					RegisterPoint((short)dX3, (short)dY3);
				}
			}
			else
			{
				RegisterPoint(nX, nY);
			}
		}

		if (NULL != pDx)
		{
			int nTextW = 0;
			for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount; unCharIndex++)
			{
				nTextW += pDx[unCharIndex];
			}
			m_pDC->SetCurPos(nX + nTextW, nY);
		}
		else
			m_pDC->SetCurPos(nX, nY);
	}

	void CWmfParserBase::RegisterPoint(short shX, short shY)
	{
		if (m_bFirstPoint)
		{
			m_oBoundingBox.Left   = shX;
			m_oBoundingBox.Right  = shX;
			m_oBoundingBox.Top    = shY;
			m_oBoundingBox.Bottom = shY;
			m_bFirstPoint = false;
		}
		else
		{
			if (shX < m_oBoundingBox.Left)
				m_oBoundingBox.Left = shX;
			else if (shX > m_oBoundingBox.Right)
				m_oBoundingBox.Right = shX;

			if (shY < m_oBoundingBox.Top)
				m_oBoundingBox.Top = shY;
			else if (shY > m_oBoundingBox.Bottom)
				m_oBoundingBox.Bottom = shY;
		}
	}

	bool CWmfParserBase::ReadImage(unsigned short ushColorUsage, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight, unsigned int& unColorUsed)
	{
		unsigned int unRemainBytes = GetRecordRemainingBytesCount();
		if (unRemainBytes <= 0)
			return false;

		BYTE* pBuffer = m_oStream.GetCurPtr();
		MetaFile::ReadImage(pBuffer, unRemainBytes, ushColorUsage, ppBgraBuffer, pulWidth, pulHeight, unColorUsed);
		return true;
	}

	void CWmfParserBase::DrawImage(const TRectL& oDestRect, const TRectL& oSrcRect, unsigned int unColorUsage, unsigned int unRasterOperation)
	{
		if (NULL != m_pInterpretator)
		{
			BYTE* pBgra = NULL;
			unsigned int unWidth, unHeight, unColorUsed;

			if (ReadImage(unColorUsage, &pBgra, &unWidth, &unHeight, unColorUsed))
			{
				ProcessRasterOperation(unRasterOperation, &pBgra, unWidth, unHeight);

				unsigned int unBlendMode{BLEND_MODE_DEFAULT};

				if (2 == unColorUsed && 0x00660046 == unRasterOperation)
					unBlendMode = BLEND_MODE_DRAW_ON_BLACK;

				double dX, dY, dX1, dY1;
				TranslatePoint(oDestRect.Left, oDestRect.Top, dX, dY);
				TranslatePoint(oDestRect.Right, oDestRect.Bottom, dX1, dY1);

				if (oDestRect != oSrcRect)
				{
					TRectL oClip = oSrcRect;

					oClip.Right  = oClip.Left + (std::min)(oClip.Right  - oClip.Left, (int)unWidth );
					oClip.Bottom = oClip.Top  + (std::min)(oClip.Bottom - oClip.Top,  (int)unHeight);

					BYTE* pNewBuffer = ClipBuffer(pBgra, unWidth, unHeight, oClip);

					if (NULL != pNewBuffer)
					{
						m_pInterpretator->DrawBitmap(dX, dY, dX1 - dX, dY1 - dY, pNewBuffer, std::abs(oClip.Right - oClip.Left), std::abs(oClip.Bottom - oClip.Top), unBlendMode);
						delete[] pNewBuffer;
					}
					else
						m_pInterpretator->DrawBitmap(dX, dY, dX1 - dX, dY1 - dY, pBgra, unWidth, unHeight, unBlendMode);
				}
				else
					m_pInterpretator->DrawBitmap(dX, dY, dX1 - dX, dY1 - dY, pBgra, unWidth, unHeight, unBlendMode);
			}

			if (pBgra)
				delete[] pBgra;

			int nRemainingBytes = GetRecordRemainingBytesCount();
			if (nRemainingBytes < 0)
				return SetError();
		}
		else
		{
			RegisterPoint(oDestRect.Left, oDestRect.Top);
			RegisterPoint(oDestRect.Right, oDestRect.Bottom);
		}
	}

	BYTE *CWmfParserBase::ClipBuffer(BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight, TRectL &oNewRect)
	{
		if (NULL == pBuffer ||
		    oNewRect.Left < 0 || oNewRect.Right  < 0 ||
		    oNewRect.Top  < 0 || oNewRect.Bottom < 0)
			return NULL;

		if (unHeight < static_cast<unsigned int>(std::abs(oNewRect.Bottom - oNewRect.Top)))
			oNewRect.Bottom = oNewRect.Top + unHeight;

		if (unWidth < static_cast<unsigned int>(std::abs(oNewRect.Right - oNewRect.Left)))
			oNewRect.Right = oNewRect.Left + unWidth;

		if (unHeight == (oNewRect.Bottom - oNewRect.Top) &&
		    unWidth  == (oNewRect.Right  - oNewRect.Left))
			return NULL;

		ULONG nBeginX, nBeginY, nEndX, nEndY;

		nBeginX = (std::min)(oNewRect.Left, oNewRect.Right);
		nBeginY = (std::min)(oNewRect.Top,  oNewRect.Bottom);

		nEndX   = (std::max)(oNewRect.Left, oNewRect.Right);
		nEndY   = (std::max)(oNewRect.Top,  oNewRect.Bottom);

		int nWidth = nEndX - nBeginX;
		int nHeight = nEndY - nBeginY;

		BYTE* pNewBuffer = new BYTE[nWidth * nHeight * 4];

		ULONG ulPos = 0;

		for (ULONG ulPosY = nBeginY * 4; ulPosY < nEndY * 4; ulPosY += 4)
		{
			for (ULONG ulPosX = nBeginX * 4; ulPosX < nEndX * 4; ulPosX += 4)
			{
				pNewBuffer[ulPos + 0]   = (BYTE)pBuffer[ulPosY * unWidth + ulPosX + 0];
				pNewBuffer[ulPos + 1]   = (BYTE)pBuffer[ulPosY * unWidth + ulPosX + 1];
				pNewBuffer[ulPos + 2]   = (BYTE)pBuffer[ulPosY * unWidth + ulPosX + 2];
				pNewBuffer[ulPos + 3]   = (BYTE)pBuffer[ulPosY * unWidth + ulPosX + 3];
				ulPos += 4;
			}
		}

		return pNewBuffer;
	}

	void CWmfParserBase::UpdateOutputDC()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->UpdateDC();
	}

	void CWmfParserBase::HANDLE_META_HEADER()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_HEADER(m_oPlaceable, m_oHeader);

		if (0x0001 != m_oHeader.ushType && 0x0002 != m_oHeader.ushType)
			return SetError();

		if (0x0009 != m_oHeader.ushHeaderSize)
			return SetError();

		if (0x0100 != m_oHeader.ushVersion && 0x0300 != m_oHeader.ushVersion)
			return SetError();

		m_pDC->SetMapMode(MM_ANISOTROPIC);

		if (NULL != m_pInterpretator)
		{
			UpdateDCRect();

			m_pDC->SetWindowOrg(m_oDCRect.Left, m_oDCRect.Top);
			m_pDC->SetWindowExt(m_oDCRect.Right - m_oDCRect.Left, m_oDCRect.Bottom - m_oDCRect.Top);
			m_pDC->SetViewportOrg(m_oDCRect.Left, m_oDCRect.Top);
			m_pDC->SetViewportExt(m_oDCRect.Right - m_oDCRect.Left, m_oDCRect.Bottom - m_oDCRect.Top);

			if (IsPlaceable() && 0 != m_oPlaceable.ushInch)
			{
				double dScale = 1440. / m_oPlaceable.ushInch;
				m_pDC->SetWindowScale(dScale, dScale);
			}
		}

		// Если у нас не задан Output, значит мы считаем, что идет сканирование метафайла.
		// Во время сканирования мы регистрируем все точки и вычисляем BoundingBox
		if (NULL == m_pInterpretator)
			m_bFirstPoint = true;

	}

	void CWmfParserBase::HANDLE_META_BITBLT(const TWmfBitBlt &oWmfBitBlt, CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_BITBLT(oWmfBitBlt, oDataStream);
	}

	void CWmfParserBase::HANDLE_META_DIBBITBLT(const TWmfBitBlt &oWmfBitBlt, CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_DIBBITBLT(oWmfBitBlt, oDataStream);

		unsigned int unRecordSizeDword = m_unRecordSize >> 1;
		unsigned int unValue = (META_DIBBITBLT >> 8) + 3;
		if (unRecordSizeDword == unValue) // похоже на META_PATBLT
		{
			if (0x00F00021 == oWmfBitBlt.unRasterOperation)
			{
				short shSkip;

				m_oStream >> shSkip;

				HANDLE_META_PATBLT(oWmfBitBlt.unRasterOperation, oWmfBitBlt.shWidth, oWmfBitBlt.shYDest, oWmfBitBlt.shXDest, oWmfBitBlt.shYSrc + shSkip);
			}
			else
				m_oStream.Skip(2);
		}
		else
		{
			TRectL oDestRect(oWmfBitBlt.shXDest, oWmfBitBlt.shYDest, oWmfBitBlt.shXDest + oWmfBitBlt.shWidth, oWmfBitBlt.shYDest + oWmfBitBlt.shHeight);

			DrawImage(oDestRect, oDestRect, 0, oWmfBitBlt.unRasterOperation);
		}
	}

	void CWmfParserBase::HANDLE_META_DIBSTRETCHBLT(const TWmfStretchBlt &oWmfStretchBlt, CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_DIBSTRETCHBLT(oWmfStretchBlt, oDataStream);

		unsigned int unRecordSizeDWORD = m_unRecordSize >> 1;
		unsigned int unValue = (META_DIBSTRETCHBLT >> 8) + 3;

		if (unRecordSizeDWORD == unValue)
		{
			m_oStream.Skip(2); // Reserved
		}
		else
		{
			TRectL oDestRect(oWmfStretchBlt.shXDest, oWmfStretchBlt.shYDest, oWmfStretchBlt.shXDest + oWmfStretchBlt.shDestWidth, oWmfStretchBlt.shYDest + oWmfStretchBlt.shDestHeight);

			DrawImage(oDestRect, oDestRect, 0, oWmfStretchBlt.unRasterOperation);
		}
	}

	void CWmfParserBase::HANDLE_META_SETDIBTODEV(const TWmfSetDibToDev &oWmfSetDibToDev, CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETDIBTODEV(oWmfSetDibToDev, oDataStream);

		TRectL oDestRect(oWmfSetDibToDev.ushXDest, oWmfSetDibToDev.ushYDest, oWmfSetDibToDev.ushXDest + oWmfSetDibToDev.ushWidth, oWmfSetDibToDev.ushYDest + oWmfSetDibToDev.ushHeight);

		// TODO: Тут надо делать обрезку в зависимости от ScanCount и StartScan. Как встретится файл сделать.
		DrawImage(oDestRect, oDestRect, oWmfSetDibToDev.ushColorUsage, 0);
	}

	void CWmfParserBase::HANDLE_META_STRETCHBLT(const TWmfStretchBlt &oWmfStretchBlt, CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_STRETCHBLT(oWmfStretchBlt, oDataStream);
	}

	void CWmfParserBase::HANDLE_META_STRETCHDIB(const TWmfStretchDib &oWmfStretchDib, CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_STRETCHDIB(oWmfStretchDib, oDataStream);

		TRectL oDestRect(oWmfStretchDib.shXDst, oWmfStretchDib.shYDst, oWmfStretchDib.shXDst + oWmfStretchDib.shDestWidth, oWmfStretchDib.shYDst + oWmfStretchDib.shDestHeight);
		TRectL oScrcRect(oWmfStretchDib.shXSrc, oWmfStretchDib.shYSrc, oWmfStretchDib.shXSrc + oWmfStretchDib.shSrcWidth, oWmfStretchDib.shYSrc + oWmfStretchDib.shSrcHeight);

		DrawImage(oDestRect, oScrcRect, oWmfStretchDib.ushColorUsage, oWmfStretchDib.unRasterOperation);
	}

	void CWmfParserBase::HANDLE_META_EOF()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_EOF();

		m_bEof = true;
	}

	void CWmfParserBase::HANDLE_META_ARC(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_ARC(shYEndArc, shXEndArc, shYStartArc, shXStartArc, shBottom, shRight, shTop, shLeft);

		double dXC = (shRight + shLeft) / 2.;
		double dYC = (shTop + shBottom) / 2.;

		double dXRadius = std::fabs(shRight - shLeft) / 2;
		double dYRadius = std::fabs(shBottom - shTop) / 2;

		double dStartAngle = agg::rad2deg(std::atan2(shYStartArc - dYC, shXStartArc - dXC));
		double dEndAngle   = agg::rad2deg(std::atan2(shYEndArc - dYC, shXEndArc - dXC));

		if ((shXStartArc - dXC) < 0 && (shYStartArc - dYC) < 0)
			dStartAngle += 360;

		if ((shXEndArc - dXC) < 0 && (shYEndArc - dYC) < 0)
			dEndAngle += 360;

		double dX1 = dXC + std::cos(agg::deg2rad(dStartAngle)) * dXRadius;
		double dY1 = dYC + std::sin(agg::deg2rad(dStartAngle)) * dYRadius;

		double dSweepAngle = GetSweepAngle(dStartAngle, dEndAngle);

		MoveToD(dX1, dY1);
		ArcTo(shLeft, shTop, shRight, shBottom, dStartAngle, dSweepAngle);
		DrawPath(true, false);
	}

	void CWmfParserBase::HANDLE_META_CHORD(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_CHORD(shYEndArc, shXEndArc, shYStartArc, shXStartArc, shBottom, shRight, shTop, shLeft);

		double dStartAngle = GetEllipseAngle((int)shLeft, (int)shTop, (int)shRight, (int)shBottom, (int)shXStartArc, (int)shYStartArc);
		double dEndAngle   = GetEllipseAngle((int)shLeft, (int)shTop, (int)shRight, (int)shBottom, (int)shXEndArc, (int)shYEndArc);
		double dSweepAngle = GetSweepAngle(dStartAngle, dEndAngle);

		MoveTo(shXStartArc, shYStartArc);
		ArcTo(shLeft, shTop, shRight, shBottom, dStartAngle, dSweepAngle);
		LineTo(shXStartArc, shYStartArc);
		DrawPath(true, true);
		m_pDC->SetCurPos(shXEndArc, shYEndArc);
	}

	void CWmfParserBase::HANDLE_META_ELLIPSE(short shBottom, short shRight, short shTop, short shLeft)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_ELLIPSE(shBottom, shRight, shTop, shLeft);

		ArcTo(shLeft, shTop, shRight, shBottom, 0, 360);
		DrawPath(true, true);
		m_pDC->SetCurPos((shLeft + shRight) / 2, (shTop + shBottom) / 2);
	}

	void CWmfParserBase::HANDLE_META_EXTTEXTOUT(short shY, short shX, short shStringLength, unsigned short ushFwOptions, const TRectS &oRectangle, unsigned char *pString, short *pDx)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_EXTTEXTOUT(shY, shX, shStringLength, ushFwOptions, oRectangle, pString, pDx);

		DrawText(pString, shStringLength, shX, shY, pDx);
	}

	void CWmfParserBase::HANDLE_META_FILLREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex)
	{
		m_oPlayer.SelectObject(ushRegionIndex);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_FILLREGION(ushRegionIndex, ushBrushIndex);

		const CWmfRegion *pRegion = m_pDC->GetRegion();

		if (NULL != pRegion)
		{
			for (unsigned int unScanIndex = 0; unScanIndex < pRegion->shScanCount; ++unScanIndex)
			{
				TWmfScanObject *pScanObject = &pRegion->pScans[unScanIndex];

				if (pScanObject->ushCount == 0) continue;

				for (unsigned int unIndex = 0; unIndex < pScanObject->ushCount >> 1; ++unIndex)
				{
					MoveTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushTop);
					LineTo(pScanObject->pScanLines[unIndex].ushRight, pScanObject->ushTop);
					LineTo(pScanObject->pScanLines[unIndex].ushRight, pScanObject->ushBottom);
					LineTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushBottom);
					LineTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushTop);
				}
			}
			DrawPath(false, true);
		}
	}

	void CWmfParserBase::HANDLE_META_FRAMEREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex, short shHeight, short shWidth)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_FRAMEREGION(ushRegionIndex, ushBrushIndex, shHeight, shWidth);

		m_oPlayer.SelectObject(ushRegionIndex);

		const CWmfRegion *pRegion = m_pDC->GetRegion();

		if (NULL != pRegion)
		{
			for (unsigned int unScanIndex = 0; unScanIndex < pRegion->shScanCount; ++unScanIndex)
			{
				TWmfScanObject *pScanObject = &pRegion->pScans[unScanIndex];

				if (pScanObject->ushCount == 0) continue;

				for (unsigned int unIndex = 0; unIndex < pScanObject->ushCount >> 1; ++unIndex)
				{
					MoveTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushTop);
					LineTo(pScanObject->pScanLines[unIndex].ushRight, pScanObject->ushTop);
					LineTo(pScanObject->pScanLines[unIndex].ushRight, pScanObject->ushBottom);
					LineTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushBottom);
					LineTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushTop);
				}
			}
			DrawPath(true, false);
		}
	}

	void CWmfParserBase::HANDLE_META_INVERTREGION(unsigned short ushRegionIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_INVERTREGION(ushRegionIndex);

		//TODO:: реализовать
	}

	void CWmfParserBase::HANDLE_META_LINETO(short shY, short shX)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_LINETO(shY, shX);

		LineTo(shX, shY);
		DrawPath(true, false);
	}

	void CWmfParserBase::HANDLE_META_PAINTREGION(unsigned short ushRegionIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_PAINTREGION(ushRegionIndex);

		m_oPlayer.SelectObject(ushRegionIndex);

		const CWmfRegion *pRegion = m_pDC->GetRegion();

		if (NULL != pRegion)
		{
			for (unsigned int unScanIndex = 0; unScanIndex < pRegion->shScanCount; ++unScanIndex)
			{
				TWmfScanObject *pScanObject = &pRegion->pScans[unScanIndex];

				if (pScanObject->ushCount == 0) continue;

				for (unsigned int unIndex = 0; unIndex < pScanObject->ushCount >> 1; ++unIndex)
				{
					MoveTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushTop);
					LineTo(pScanObject->pScanLines[unIndex].ushRight, pScanObject->ushTop);
					LineTo(pScanObject->pScanLines[unIndex].ushRight, pScanObject->ushBottom);
					LineTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushBottom);
					LineTo(pScanObject->pScanLines[unIndex].ushLeft,  pScanObject->ushTop);
				}
			}
			DrawPath(false, true);
		}
	}

	void CWmfParserBase::HANDLE_META_PATBLT(unsigned int unRasterOperation, short shH, short shW, short shY, short shX)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_PATBLT(unRasterOperation, shH, shW, shY, shX);

		// TODO: Нужно использовать растровую операцию unRasterOperation

		MoveTo(shX, shY);
		LineTo(shX + shW, shY);
		LineTo(shX + shW, shY + shH);
		LineTo(shX, shY + shH);
		ClosePath();
		DrawPath(false, true);
	}

	void CWmfParserBase::HANDLE_META_PIE(short shXRadial1, short shYRadial1, short shXRadial2, short shYRadial2, short shB, short shR, short shT, short shL)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_PIE(shXRadial1, shYRadial1, shXRadial2, shYRadial2, shB, shR, shT, shL);

		short shCenterX = (shL + shR) / 2;
		short shCenterY = (shT + shB) / 2;

		double dStartAngle = GetEllipseAngle(shL, shT, shR, shB, shXRadial1, shYRadial1);
		double dEndAngle   = GetEllipseAngle(shL, shT, shR, shB, shXRadial2, shYRadial2);

		//TODO: это получается заглушка, поэтому нужно научиться определять угол с учетом направления рисования
		if (shXRadial2 >= shCenterX && shYRadial2 <= shCenterY && shXRadial1 >= shCenterX && shYRadial1 >= shCenterY)
			dEndAngle -= 360;

		double dSweepAngle = GetSweepAngle(dStartAngle, dEndAngle);

		MoveTo(shCenterX, shCenterY);
		LineTo(shXRadial1, shYRadial1);
		ArcTo(shL, shT, shR, shB, dStartAngle, dSweepAngle);
		LineTo(shCenterX, shCenterY);
		DrawPath(true, true);
	}

	void CWmfParserBase::HANDLE_META_POLYLINE(const std::vector<TPointS> &arPoints)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_POLYLINE(arPoints);

		if (arPoints.empty())
			return;

		MoveTo(arPoints[0].X, arPoints[0].Y);

		for (short shIndex = 1; shIndex < arPoints.size(); shIndex++)
			LineTo(arPoints[shIndex].X, arPoints[shIndex].Y);

		DrawPath(true, false);
	}

	void CWmfParserBase::HANDLE_META_POLYGON(const std::vector<TPointS> &arPoints)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_POLYGON(arPoints);

		if (arPoints.empty())
			return;

		MoveTo(arPoints[0].X, arPoints[0].Y);

		for (short shIndex = 1; shIndex < arPoints.size(); shIndex++)
			LineTo(arPoints[shIndex].X, arPoints[shIndex].Y);

		ClosePath();
		DrawPath(true, true);
	}

	void CWmfParserBase::HANDLE_META_POLYPOLYGON(const std::vector<std::vector<TPointS> > &arPolygons)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_POLYPOLYGON(arPolygons);

		if (arPolygons.empty())
			return;

		for (unsigned int unPolygonIndex = 0; unPolygonIndex < arPolygons.size(); ++unPolygonIndex)
		{
			if (arPolygons[unPolygonIndex].empty())
				continue;

			MoveTo(arPolygons[unPolygonIndex][0].X, arPolygons[unPolygonIndex][0].Y);

			for (unsigned int unPointIndex = 1; unPointIndex < arPolygons[unPolygonIndex].size(); ++unPointIndex)
				LineTo(arPolygons[unPolygonIndex][unPointIndex].X, arPolygons[unPolygonIndex][unPointIndex].Y);

			ClosePath();
		}

		DrawPath(true, true);
	}

	void CWmfParserBase::HANDLE_META_RECTANGLE(short shB, short shR, short shT, short shL)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_RECTANGLE(shB, shR, shT, shL);

		MoveTo(shL, shT);
		LineTo(shR, shT);
		LineTo(shR, shB);
		LineTo(shL, shB);
		ClosePath();
		DrawPath(true, true);

		m_pDC->SetCurPos((shL + shR) / 2, (shT + shB) / 2);
	}

	void CWmfParserBase::HANDLE_META_ROUNDRECT(short shH, short shW, short shB, short shR, short shT, short shL)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_ROUNDRECT(shH, shW, shB, shR, shT, shL);

		MoveTo(shL + shW, shT);
		LineTo(shR - shW, shT);
		ArcTo(shR - shW, shT, shR, shT + shH, 270, 90);
		LineTo(shR, shB - shH);
		ArcTo(shR - shW, shB - shH, shR, shB, 0, 90);
		LineTo(shL + shW, shB);
		ArcTo(shL, shB - shH, shL + shW, shB, 90, 90);
		LineTo(shL, shT + shH);
		ArcTo(shL, shT, shL + shW, shT + shH, 180, 90);
		ClosePath();
		DrawPath(true, true);
	}

	void CWmfParserBase::HANDLE_META_SETPIXEL(const TRGBA &oColor, short shY, short shX)
	{
		BYTE pBgraBuffer[4];
		pBgraBuffer[0] = oColor.b;
		pBgraBuffer[1] = oColor.g;
		pBgraBuffer[2] = oColor.r;
		pBgraBuffer[3] = 0xff;

		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->HANDLE_META_SETPIXEL(oColor, shY, shX);
			m_pInterpretator->DrawBitmap(shX, shY, 1, 1, pBgraBuffer, 1, 1, BLEND_MODE_DEFAULT);
		}
	}

	void CWmfParserBase::HANDLE_META_TEXTOUT(short shStringLength, unsigned char *pString, short shY, short shX)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_TEXTOUT(shStringLength, pString, shX, shY);

		DrawText(pString, shStringLength, shX, shY, NULL);
	}

	void CWmfParserBase::HANDLE_META_CREATEBRUSHINDIRECT(TWmfLogBrush &oBrush)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_CREATEBRUSHINDIRECT(oBrush);

		CWmfBrush* pBrush = new CWmfBrush(oBrush);
		if (!pBrush)
			return SetError();

		if (BS_PATTERN == pBrush->ushBrushStyle || BS_DIBPATTERNPT == pBrush->ushBrushStyle)
		{
			pBrush->ushBrushStyle = BS_SOLID;
			pBrush->oColor.Set(0, 0, 0);
		}

		m_oPlayer.RegisterObject((CWmfObjectBase*)pBrush);
	}

	void CWmfParserBase::HANDLE_META_CREATEFONTINDIRECT(CWmfFont &oFont)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_CREATEFONTINDIRECT(oFont);

		m_oPlayer.RegisterObject((CWmfObjectBase*)&oFont);
	}

	void CWmfParserBase::HANDLE_META_CREATEPALETTE(CWmfPalette &oPalette)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_CREATEPALETTE(oPalette);

		m_oPlayer.RegisterObject((CWmfObjectBase*)&oPalette);
	}

	void CWmfParserBase::HANDLE_META_CREATEPATTERNBRUSH(CWmfBrush &oBrush, CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_CREATEPATTERNBRUSH(oBrush, oDataStream);

		m_oPlayer.RegisterObject((CWmfObjectBase*)&oBrush);
	}

	void CWmfParserBase::HANDLE_META_CREATEPENINDIRECT(CWmfPen &oPen)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_CREATEPENINDIRECT(oPen);

		m_oPlayer.RegisterObject((CWmfObjectBase*)&oPen);
	}

	void CWmfParserBase::HANDLE_META_CREATEREGION(CWmfRegion &oRegion)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_CREATEREGION(oRegion);

		m_oPlayer.RegisterObject((CWmfObjectBase*)&oRegion);
	}

	void CWmfParserBase::HANDLE_META_DELETEOBJECT(unsigned short ushIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_DELETEOBJECT(ushIndex);

		m_oPlayer.DeleteObject(ushIndex);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_DIBCREATEPATTERNBRUSH(unsigned short ushStyle, unsigned short ushColorUsage)
	{
		CWmfBrush* pBrush = new CWmfBrush();
		if (!pBrush)
			return SetError();

		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->HANDLE_META_DIBCREATEPATTERNBRUSH(ushStyle, ushColorUsage, *pBrush, m_oStream);

			BYTE* pBgra = NULL;
			unsigned int unWidth = 0, unHeight = 0, unColorUsed = 0;
			if (ReadImage(ushColorUsage, &pBgra, &unWidth, &unHeight, unColorUsed))
			{
				pBrush->SetDibPattern(pBgra, unWidth, unHeight);
			}
		}

		m_oPlayer.RegisterObject((CWmfObjectBase*)pBrush);
	}

	void CWmfParserBase::HANDLE_META_SELECTCLIPREGION(unsigned short ushIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SELECTCLIPREGION(ushIndex);

		// Тут просто сбрасываем текущий клип. Ничего не добавляем в клип, т.е. реализовать регионы с
		// текущим интерфейсом рендерера невозможно.
		m_pDC->GetClip()->Reset();
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SELECTOBJECT(unsigned short ushIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SELECTOBJECT(ushIndex);

		m_oPlayer.SelectObject(ushIndex);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SELECTPALETTE(unsigned short ushIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SELECTPALETTE(ushIndex);

		m_oPlayer.SelectPalette(ushIndex);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_EXCLUDECLIPRECT(short shBottom, short shRight, short shTop, short shLeft)
	{
		// Поскольку мы реализовываем данный тип клипа с помощью разницы внешнего ректа и заданного, и
		// пересечением с полученной областью, то нам надо вычесть границу заданного ректа.
		if (shLeft < shRight)
		{
			shLeft--;
			shRight++;
		}
		else
		{
			shLeft++;
			shRight--;
		}

		if (shTop < shBottom)
		{
			shTop--;
			shBottom++;
		}
		else
		{
			shTop++;
			shBottom--;
		}

		TRectD oClip;

		TranslatePoint(shLeft, shTop, oClip.Left, oClip.Top);
		TranslatePoint(shRight, shBottom, oClip.Right, oClip.Bottom);

		const TWmfWindow& oWindow{m_pDC->GetWindow()};
		TRectD oBB;

		TranslatePoint(oWindow.x, oWindow.y, oBB.Left, oBB.Top);
		TranslatePoint(oWindow.x + oWindow.w, oWindow.y + oWindow.h, oBB.Right, oBB.Bottom);

		m_pDC->GetClip()->Exclude(oClip, oBB);
		UpdateOutputDC();

		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->HANDLE_META_EXCLUDECLIPRECT(shBottom, shRight, shTop, shLeft);
			m_pInterpretator->ExcludeClip(oClip, oBB);
		}
	}

	void CWmfParserBase::HANDLE_META_INTERSECTCLIPRECT(short shLeft, short shTop, short shRight, short shBottom)
	{
		TRectD oClip;

		TranslatePoint(shLeft, shTop, oClip.Left, oClip.Top);
		TranslatePoint(shRight, shBottom, oClip.Right, oClip.Bottom);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_INTERSECTCLIPRECT(shLeft, shTop, shRight, shBottom);

		m_pDC->GetClip()->Intersect(oClip);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_MOVETO(short shX, short shY)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_MOVETO(shX, shY);

		MoveTo(shX, shY);
	}

	void CWmfParserBase::HANDLE_META_OFFSETCLIPRGN(short shOffsetX, short shOffsetY)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_MOVETO(shOffsetX, shOffsetY);

		// TODO: Реализовать
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_OFFSETVIEWPORTORG(short shXOffset, short shYOffset)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_OFFSETVIEWPORTORG(shXOffset, shYOffset);

		m_pDC->SetViewportOff(shXOffset, shYOffset);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_OFFSETWINDOWORG(short shXOffset, short shYOffset)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_OFFSETWINDOWORG(shXOffset, shYOffset);

		m_pDC->SetWindowOff(shXOffset, shYOffset);
		UpdateOutputDC();

		if (NULL == m_pInterpretator)
		{
			const TWmfWindow& oWindow{m_pDC->GetWindow()};
			RegisterPoint(oWindow.x            , oWindow.y            );
			RegisterPoint(oWindow.x + oWindow.w, oWindow.y + oWindow.h);
		}
	}

	void CWmfParserBase::HANDLE_META_RESTOREDC()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_RESTOREDC();

		m_pDC = m_oPlayer.RestoreDC();
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SAVEDC()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SAVEDC();

		m_pDC = m_oPlayer.SaveDC();
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SCALEVIEWPORTEXT(short yDenom, short yNum, short xDenom, short xNum)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SCALEVIEWPORTEXT(yDenom, yNum, xDenom, xNum);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->SetViewportScale((double)xNum / (double)xDenom, (double)yNum / (double)yDenom);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SCALEWINDOWEXT(short yDenom, short yNum, short xDenom, short xNum)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SCALEWINDOWEXT(yDenom, yNum, xDenom, xNum);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->SetWindowScale((double)xNum / (double)xDenom, (double)yNum / (double)yDenom);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETBKCOLOR(TRGBA &oColor)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETBKCOLOR(oColor);

		m_pDC->SetTextBgColor(oColor);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETBKMODE(unsigned short ushMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETBKMODE(ushMode);

		m_pDC->SetTextBgMode(ushMode);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETLAYOUT(unsigned short ushLayout)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETLAYOUT(ushLayout);

		m_pDC->SetLayout(ushLayout);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETMAPMODE(unsigned short ushMapMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETMAPMODE(ushMapMode);

		m_pDC->SetMapMode(ushMapMode);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETPOLYFILLMODE(unsigned short ushMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETPOLYFILLMODE(ushMode);

		m_pDC->SetPolyFillMode(ushMode);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETROP2(unsigned short ushMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETROP2(ushMode);

		m_pDC->SetRop2Mode(ushMode);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETSTRETCHBLTMODE(unsigned short ushMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETSTRETCHBLTMODE(ushMode);

		m_pDC->SetStretchBltMode(ushMode);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETTEXTALIGN(unsigned short ushTextAlign)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETTEXTALIGN(ushTextAlign);

		m_pDC->SetTextAlign(ushTextAlign);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETTEXTCHAREXTRA(unsigned short ushCharSpacing)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETTEXTCHAREXTRA(ushCharSpacing);

		m_pDC->SetCharSpacing(ushCharSpacing);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETTEXTCOLOR(TRGBA &oColor)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETTEXTCOLOR(oColor);

		m_pDC->SetTextColor(oColor);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETTEXTJUSTIFICATION(unsigned short ushBreakCount, unsigned short ushBreakExtra)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETTEXTJUSTIFICATION(ushBreakCount, ushBreakExtra);

		// TODO: Реализовать
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETVIEWPORTEXT(short shX, short shY)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETVIEWPORTEXT(shX, shY);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->SetViewportExt(shX, shY);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETVIEWPORTORG(short shX, short shY)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETVIEWPORTORG(shX, shY);

		m_pDC->SetViewportOrg(shX, shY);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETWINDOWEXT(short shX, short shY)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETWINDOWEXT(shX, shY);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->SetWindowExt(shX, shY);
		UpdateOutputDC();

		if (NULL == m_pInterpretator)
		{
			const TWmfWindow& oWindow{m_pDC->GetWindow()};
			RegisterPoint(oWindow.x + oWindow.w, oWindow.y + oWindow.h);
		}
	}

	void CWmfParserBase::HANDLE_META_SETWINDOWORG(short shX, short shY)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETWINDOWORG(shX, shY);

		m_pDC->SetWindowOrg(shX, shY);
		UpdateOutputDC();

		if (NULL == m_pInterpretator)
		{
			const TWmfWindow& oWindow{m_pDC->GetWindow()};
			RegisterPoint(oWindow.x            , oWindow.y            );
			RegisterPoint(oWindow.x + oWindow.w, oWindow.y + oWindow.h);
		}
	}

	void CWmfParserBase::HANDLE_META_ESCAPE(unsigned short ushEscapeFunction, unsigned short ushByteCount)
	{
		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->HANDLE_META_ESCAPE(ushEscapeFunction, ushByteCount);
			PRINT_WMF_LOG("Escape function type: " << ushEscapeFunction << " size: " << ushByteCount);
		}

		if (WMF_META_ESCAPE_ENHANCED_METAFILE == ushEscapeFunction)
		{
			if (ushByteCount < 34)
				return;

			unsigned int unCommentIdentifier, unCommentType, unVersion;

			m_oStream >> unCommentIdentifier;
			m_oStream >> unCommentType;
			m_oStream >> unVersion;

			if (0x43464D57 != unCommentIdentifier || 0x00000001 != unCommentType || 0x00010000 != unVersion)
				return;

			unsigned short ushChecksum;
			unsigned int unFlags;

			m_oStream >> ushChecksum;
			m_oStream >> unFlags;

			if (0x00000000 != unFlags)
				return;

			unsigned int unCommentRecordCount, unCurrentRecordSize, unRemainingBytes, unEnhancedMetafileDataSize;

			m_oStream >> unCommentRecordCount;
			m_oStream >> unCurrentRecordSize;
			m_oStream >> unRemainingBytes;
			m_oStream >> unEnhancedMetafileDataSize;

			if (m_oEscapeBuffer.Empty())
				m_oEscapeBuffer.SetSize(unEnhancedMetafileDataSize);

			unsigned int nEscapeRecordSize = m_oEscapeBuffer.GetTileSize();
			if (nEscapeRecordSize > unCurrentRecordSize)
				nEscapeRecordSize = unCurrentRecordSize;

			m_oStream.ReadBytes(m_oEscapeBuffer.GetCurPtr(), nEscapeRecordSize);
			m_oEscapeBuffer.IncreasePosition(nEscapeRecordSize);

			if (0 == unRemainingBytes)
			{
				CEmfParser oEmfParser;

				oEmfParser.SetFontManager(GetFontManager());
				oEmfParser.SetStream(m_oEscapeBuffer.GetBuffer(), m_oEscapeBuffer.GetSize());
				oEmfParser.Scan();

				if (oEmfParser.CheckError())
				{
					m_oEscapeBuffer.Clear();
					return;
				}

				if (NULL == m_pInterpretator)
				{
					if (!IsPlaceable())
						m_oBoundingBox = oEmfParser.GetDCBounds();

					HANDLE_META_EOF();
				}
				else if (InterpretatorType::Render == m_pInterpretator->GetType())
				{
					CMetaFileRenderer oEmfOut(&oEmfParser, ((CWmfInterpretatorRender*)m_pInterpretator)->GetRenderer());
					oEmfParser.SetInterpretator(&oEmfOut);

					oEmfParser.PlayFile();

					HANDLE_META_EOF();
				}
				else if (InterpretatorType::Svg == m_pInterpretator->GetType())
				{
					((CEmfParserBase*)&oEmfParser)->SetInterpretator(InterpretatorType::Svg);

					XmlUtils::CXmlWriter *pXmlWriter = ((CWmfInterpretatorSvg*)GetInterpretator())->GetXmlWriter();

					const TRectL& oEmfRect{oEmfParser.GetDCBounds()};
					const TRectL& oCurentRect{GetDCBounds()};

					const double dScaleX = std::abs((oCurentRect.Right  - oCurentRect.Left) / (oEmfRect.Right  - oEmfRect.Left));
					const double dScaleY = std::abs((oCurentRect.Bottom - oCurentRect.Top)  / (oEmfRect.Bottom - oEmfRect.Top));

					const bool bAddGElement = !Equals(1., dScaleX) || !Equals(1., dScaleY);

					if (bAddGElement)
					{
						pXmlWriter->WriteNodeBegin(L"g", true);
						pXmlWriter->WriteAttribute(L"transform", L"scale(" + ConvertToWString(dScaleX) + L',' + ConvertToWString(dScaleY) + L')');
						pXmlWriter->WriteNodeEnd(L"g", true, false);
					}

					((CEmfInterpretatorSvg*)oEmfParser.GetInterpretator())->SetXmlWriter(pXmlWriter);

					oEmfParser.PlayFile();

					if (bAddGElement)
						pXmlWriter->WriteNodeEnd(L"g", false, false);

					HANDLE_META_EOF();
				}

				m_oEscapeBuffer.Clear();
			}
		}
		// TODO: Реализовать
	}

	void CWmfParserBase::HANDLE_META_UNKNOWN(CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_UNKNOWN(oDataStream);
	}
}
