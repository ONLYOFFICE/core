#include "CWmfParserBase.h"

#include "../WmfInterpretator/CWmfInterpretatorRender.h"
#include "../WmfInterpretator/CWmfInterpretatorSvg.h"

#include "../../Emf/EmfParser/CEmfParser.h"
#include "../../Emf/EmfInterpretator/CEmfInterpretatorSvg.h"

namespace MetaFile
{
	CWmfParserBase::CWmfParserBase()
		: m_oPlayer(this), m_pInterpretator(NULL), m_bEof(false)
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

	TRect *CWmfParserBase::GetDCBounds()
	{
		m_oDCRect = GetBoundingBox();
		return &m_oDCRect;
	}

	double CWmfParserBase::GetPixelHeight()
	{
		return m_pDC->GetPixelHeight();
	}

	double CWmfParserBase::GetPixelWidth()
	{
		return m_pDC->GetPixelWidth();
	}

	int CWmfParserBase::GetTextColor()
	{
		TWmfColor& oColor = m_pDC->GetTextColor();
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
	}

	IFont *CWmfParserBase::GetFont()
	{
		CWmfFont* pFont = m_pDC->GetFont();
		if (!pFont)
			return NULL;

		return (IFont*)pFont;
	}

	IBrush *CWmfParserBase::GetBrush()
	{
		CWmfBrush* pBrush = m_pDC->GetBrush();
		if (!pBrush)
			return NULL;

		return (IBrush*)pBrush;
	}

	IPen *CWmfParserBase::GetPen()
	{
		CWmfPen* pPen = m_pDC->GetPen();
		if (!pPen)
			return NULL;

		return (IPen*)pPen;
	}

	unsigned int CWmfParserBase::GetTextAlign()
	{
		return (unsigned int)m_pDC->GetTextAlign();
	}

	unsigned int CWmfParserBase::GetTextBgMode()
	{
		return (unsigned int)m_pDC->GetTextBgMode();
	}

	int CWmfParserBase::GetTextBgColor()
	{
		TWmfColor& oColor = m_pDC->GetTextBgColor();
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
	}

	unsigned int CWmfParserBase::GetFillMode()
	{
		return (unsigned int)m_pDC->GetPolyFillMode();
	}

	TPointD CWmfParserBase::GetCurPos()
	{
		TPointL oPoint = m_pDC->GetCurPos();
		double dX, dY;
		TranslatePoint(oPoint.x, oPoint.y, dX, dY);
		return TPointD(dX, dY);
	}

	TXForm *CWmfParserBase::GetInverseTransform()
	{
		return m_pDC->GetInverseTransform();
	}

	TXForm *CWmfParserBase::GetTransform(int iGraphicsMode)
	{
		TRect* pBounds = GetDCBounds();
		double dT = pBounds->nTop;
		double dL = pBounds->nLeft;

		TXForm oShiftXForm(1, 0, 0, 1, -dL, -dT);
		m_oTransform.Copy(m_pDC->GetFinalTransform(iGraphicsMode));
		m_oTransform.Multiply(oShiftXForm, MWT_RIGHTMULTIPLY);
		return &m_oTransform;
	}

	unsigned int CWmfParserBase::GetMiterLimit()
	{
		return m_pDC->GetMiterLimit();
	}

	unsigned int CWmfParserBase::GetRop2Mode()
	{
		return (unsigned int)m_pDC->GetRop2Mode();
	}

	IClip *CWmfParserBase::GetClip()
	{
		CWmfClip* pClip = m_pDC->GetClip();
		if (!pClip)
			return NULL;

		return (IClip*)pClip;
	}

	int CWmfParserBase::GetCharSpace()
	{
		return m_pDC->GetCharSpacing();
	}

	bool CWmfParserBase::IsWindowFlippedY()
	{
		TWmfWindow* pWindow = m_pDC->GetWindow();
		return (pWindow->h < 0);
	}

	bool CWmfParserBase::IsWindowFlippedX()
	{
		TWmfWindow* pWindow = m_pDC->GetWindow();
		return (pWindow->w < 0);
	}

	unsigned int CWmfParserBase::GetMapMode()
	{
		return m_pDC->GetMapMode();
	}

	double CWmfParserBase::GetDpi()
	{
		return (0 != m_oPlaceable.Inch) ? m_oPlaceable.Inch : 96.;
	}

	IRegion *CWmfParserBase::GetRegion()
	{
		return m_pDC->GetRegion();
	}

	unsigned int CWmfParserBase::GetArcDirection()
	{
		return AD_CLOCKWISE;
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
			m_pInterpretator = new CWmfInterpretatorSvg(this, unWidth, unHeight);
	}

	void CWmfParserBase::SetInterpretator(IOutputDevice *pOutput, const wchar_t *wsFilePath)
	{
		RELEASEOBJECT(m_pInterpretator);

	}

	TRectD CWmfParserBase::GetBounds()
	{
		TRect  oBoundsBox = GetBoundingBox();
		TRectD oBounds(oBoundsBox);
		return oBounds;
	}

	void CWmfParserBase::SkipVoid()
	{
		do
		{
		} while (m_oStream.ReadChar() == 0);

		m_oStream.SeekBack(1);
	}

	void CWmfParserBase::TranslatePoint(short shX, short shY, double &dX, double &dY)
	{
		dX = (double)shX;
		dY = (double)shY;
	}

	TRect CWmfParserBase::GetBoundingBox()
	{
		if (IsPlaceable())
		{
			TRect oBB;

			oBB = m_oPlaceable.BoundingBox;

			// Иногда m_oPlaceable.BoundingBox задается нулевой ширины и высоты
			if (abs(oBB.nRight - oBB.nLeft) <= 1)
			{
				oBB.nRight = m_oBoundingBox.nRight;
				oBB.nLeft  = m_oBoundingBox.nLeft;
			}
			if (abs(oBB.nBottom - oBB.nTop) <= 1)
			{
				oBB.nTop    = m_oBoundingBox.nTop;
				oBB.nBottom = m_oBoundingBox.nBottom;
			}

			return oBB;
		}
		else
			return m_oBoundingBox;
	}

	bool CWmfParserBase::IsPlaceable()
	{
		return (0x9AC6CDD7 == m_oPlaceable.Key);
	}

	int CWmfParserBase::GetRecordRemainingBytesCount()
	{
		unsigned int unReadedSize = m_oStream.Tell() - m_unRecordPos;
		return (m_unRecordSize - unReadedSize);
	}

	double CWmfParserBase::GetSweepAngle(const double &dStartAngle, const double &dEndAngle)
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

		if (m_pDC->GetTextAlign() & TA_UPDATECP)
		{
			nX = m_pDC->GetCurPos().x;
			nY = m_pDC->GetCurPos().y;
		}

		IFont* pFont = GetFont();
		NSStringExt::CConverter::ESingleByteEncoding eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT;;
		if (pFont)
		{
			// Соответствие Charset -> Codepage: http://support.microsoft.com/kb/165478
			// http://msdn.microsoft.com/en-us/library/cc194829.aspx
			//  Charset Name       Charset Value(hex)  Codepage number
			//  ------------------------------------------------------
			//
			//  DEFAULT_CHARSET           1 (x01)
			//  SYMBOL_CHARSET            2 (x02)
			//  OEM_CHARSET             255 (xFF)
			//  ANSI_CHARSET              0 (x00)            1252
			//  RUSSIAN_CHARSET         204 (xCC)            1251
			//  EASTEUROPE_CHARSET      238 (xEE)            1250
			//  GREEK_CHARSET           161 (xA1)            1253
			//  TURKISH_CHARSET         162 (xA2)            1254
			//  BALTIC_CHARSET          186 (xBA)            1257
			//  HEBREW_CHARSET          177 (xB1)            1255
			//  ARABIC _CHARSET         178 (xB2)            1256
			//  SHIFTJIS_CHARSET        128 (x80)             932
			//  HANGEUL_CHARSET         129 (x81)             949
			//  GB2313_CHARSET          134 (x86)             936
			//  CHINESEBIG5_CHARSET     136 (x88)             950
			//  THAI_CHARSET            222 (xDE)             874
			//  JOHAB_CHARSET	        130 (x82)            1361
			//  VIETNAMESE_CHARSET      163 (xA3)            1258

			switch (pFont->GetCharSet())
			{
			default:
			case DEFAULT_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT; break;
			case SYMBOL_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT; break;
			case ANSI_CHARSET:          eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1252; break;
			case RUSSIAN_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1251; break;
			case EASTEUROPE_CHARSET:    eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1250; break;
			case GREEK_CHARSET:         eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1253; break;
			case TURKISH_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1254; break;
			case BALTIC_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1257; break;
			case HEBREW_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1255; break;
			case ARABIC_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1256; break;
			case SHIFTJIS_CHARSET:      eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP932; break;
			case HANGEUL_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP949; break;
			case 134/*GB2313_CHARSET*/: eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP936; break;
			case CHINESEBIG5_CHARSET:   eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP950; break;
			case THAI_CHARSET:          eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP874; break;
			case JOHAB_CHARSET:         eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1361; break;
			case VIETNAMESE_CHARSET:    eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1258; break;
			}
		}

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromSingleByteString((const unsigned char*)pString, (long)unCharsCount, eCharSet);

		if (NULL != m_pInterpretator)
		{
			double dX, dY;
			TranslatePoint(nX, nY, dX, dY);

			double* pdDx = NULL;
			if (NULL != pDx)
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

			m_pInterpretator->DrawString(wsText, unCharsCount, dX, dY, pdDx, 1, GetTransform()->M11, GetTransform()->M22);

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
					int lLogicalFontHeight = pFont->GetHeight();
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
						fL = (float)(oBox.fMinX);
						fW = (float)(oBox.fMaxX - oBox.fMinX);
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
					int lLogicalFontHeight = pFont->GetHeight();
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

					fH = dFontHeight * 1.2;

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
			m_oBoundingBox.nLeft   = shX;
			m_oBoundingBox.nRight  = shX;
			m_oBoundingBox.nTop    = shY;
			m_oBoundingBox.nBottom = shY;
			m_bFirstPoint = false;
		}
		else
		{
			if (shX < m_oBoundingBox.nLeft)
				m_oBoundingBox.nLeft = shX;
			else if (shX > m_oBoundingBox.nRight)
				m_oBoundingBox.nRight = shX;

			if (shY < m_oBoundingBox.nTop)
				m_oBoundingBox.nTop = shY;
			else if (shY > m_oBoundingBox.nBottom)
				m_oBoundingBox.nBottom = shY;
		}
	}

	bool CWmfParserBase::ReadImage(unsigned short ushColorUsage, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight)
	{
		unsigned int unRemainBytes = GetRecordRemainingBytesCount();
		if (unRemainBytes <= 0)
			return false;

		BYTE* pBuffer = m_oStream.GetCurPtr();
		MetaFile::ReadImage(pBuffer, unRemainBytes, ushColorUsage, ppBgraBuffer, pulWidth, pulHeight);
		return true;
	}

	void CWmfParserBase::DrawImage(const TRect& oDestRect, const TRect& oSrcRect, unsigned int unColorUsage, unsigned int unRasterOperation)
	{
		if (NULL != m_pInterpretator)
		{
			BYTE* pBgra = NULL;
			unsigned int unWidth, unHeight;

			if (ReadImage(unColorUsage, &pBgra, &unWidth, &unHeight))
			{
				ProcessRasterOperation(unRasterOperation, &pBgra, unWidth, unHeight);

				double dX, dY, dX1, dY1;
				TranslatePoint(oDestRect.nLeft, oDestRect.nTop, dX, dY);
				TranslatePoint(oDestRect.nRight, oDestRect.nBottom, dX1, dY1);

				if (oDestRect != oSrcRect)
				{
					TRect oClip = oSrcRect;
					BYTE* pNewBuffer = ClipBuffer(pBgra, unWidth, unHeight, oClip);

					if (NULL != pNewBuffer)
					{
						m_pInterpretator->DrawBitmap(dX, dY, fabs(dX1 - dX), fabs(dY1 - dY), pNewBuffer, std::abs(oClip.nRight - oClip.nLeft), std::abs(oClip.nBottom - oClip.nTop));
						delete[] pNewBuffer;
					}
					else
						m_pInterpretator->DrawBitmap(dX, dY, fabs(dX1 - dX), fabs(dY1 - dY), pBgra, unWidth, unHeight);
				}
				else
					m_pInterpretator->DrawBitmap(dX, dY, fabs(dX1 - dX), fabs(dY1 - dY), pBgra, unWidth, unHeight);
			}

			if (pBgra)
				delete[] pBgra;

			int nRemainingBytes = GetRecordRemainingBytesCount();
			if (nRemainingBytes < 0)
				return SetError();
		}
		else
		{
			RegisterPoint(oDestRect.nLeft, oDestRect.nTop);
			RegisterPoint(oDestRect.nRight, oDestRect.nBottom);
		}
	}

	BYTE *CWmfParserBase::ClipBuffer(BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight, TRect &oNewRect)
	{
		if (NULL == pBuffer ||
		    oNewRect.nLeft < 0 || oNewRect.nRight  < 0 ||
		    oNewRect.nTop  < 0 || oNewRect.nBottom < 0)
			return NULL;

		if (unHeight < (oNewRect.nBottom - oNewRect.nTop))
			oNewRect.nBottom = oNewRect.nTop + unWidth;

		if (unWidth < (oNewRect.nRight - oNewRect.nLeft))
			oNewRect.nRight = oNewRect.nLeft + unWidth;

		if (unHeight == (oNewRect.nBottom - oNewRect.nTop) &&
		    unWidth  == (oNewRect.nRight  - oNewRect.nLeft))
			return NULL;

		int nBeginX, nBeginY, nEndX, nEndY;

		nBeginX = (std::min)(oNewRect.nLeft, oNewRect.nRight);
		nBeginY = (std::min)(oNewRect.nTop,  oNewRect.nBottom);

		nEndX   = (std::max)(oNewRect.nLeft, oNewRect.nRight);
		nEndY   = (std::max)(oNewRect.nTop,  oNewRect.nBottom);

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

		if (0x0001 != m_oHeader.Type && 0x0002 != m_oHeader.Type)
			return SetError();

		if (0x0009 != m_oHeader.HeaderSize)
			return SetError();

		if (0x0100 != m_oHeader.Version && 0x0300 != m_oHeader.Version)
			return SetError();

		m_pDC->SetMapMode(MM_ANISOTROPIC);

		if (NULL != m_pInterpretator)
		{
			m_oRect = GetBoundingBox();

			m_pDC->SetWindowOrg(m_oRect.nLeft, m_oRect.nTop);
			m_pDC->SetWindowExt(m_oRect.nRight - m_oRect.nLeft, m_oRect.nBottom - m_oRect.nTop);
			m_pDC->SetViewportOrg(m_oRect.nLeft, m_oRect.nTop);
			m_pDC->SetViewportExt(m_oRect.nRight - m_oRect.nLeft, m_oRect.nBottom - m_oRect.nTop);

			if (0 != m_oPlaceable.Inch)
			{
				double dScale = 1440. / m_oPlaceable.Inch;
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
			if (0x00F00021 == oWmfBitBlt.RasterOperation)
			{
				short shSkip;

				m_oStream >> shSkip;

				HANDLE_META_PATBLT(oWmfBitBlt.RasterOperation, oWmfBitBlt.Width, oWmfBitBlt.YDest, oWmfBitBlt.XDest, oWmfBitBlt.YSrc + shSkip);
			}
			else
				m_oStream.Skip(2);
		}
		else
		{
			TRect oDestRect(oWmfBitBlt.XDest, oWmfBitBlt.YDest, oWmfBitBlt.XDest + oWmfBitBlt.Width, oWmfBitBlt.YDest + oWmfBitBlt.Height);

			DrawImage(oDestRect, oDestRect, 0, oWmfBitBlt.RasterOperation);
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
			TRect oDestRect(oWmfStretchBlt.XDest, oWmfStretchBlt.YDest, oWmfStretchBlt.XDest + oWmfStretchBlt.DestWidth, oWmfStretchBlt.YDest + oWmfStretchBlt.DestHeight);

			DrawImage(oDestRect, oDestRect, 0, oWmfStretchBlt.RasterOperation);
		}
	}

	void CWmfParserBase::HANDLE_META_SETDIBTODEV(const TWmfSetDibToDev &oWmfSetDibToDev, CDataStream &oDataStream)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETDIBTODEV(oWmfSetDibToDev, oDataStream);

		TRect oDestRect(oWmfSetDibToDev.xDest, oWmfSetDibToDev.yDest, oWmfSetDibToDev.xDest + oWmfSetDibToDev.Width, oWmfSetDibToDev.yDest + oWmfSetDibToDev.Height);

		// TODO: Тут надо делать обрезку в зависимости от ScanCount и StartScan. Как встретится файл сделать.
		DrawImage(oDestRect, oDestRect, oWmfSetDibToDev.ColorUsage, 0);
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

		TRect oDestRect(oWmfStretchDib.xDst, oWmfStretchDib.yDst, oWmfStretchDib.xDst + oWmfStretchDib.DestWidth, oWmfStretchDib.yDst + oWmfStretchDib.DestHeight);
		TRect oScrcRect(oWmfStretchDib.XSrc, oWmfStretchDib.YSrc, oWmfStretchDib.XSrc + oWmfStretchDib.SrcWidth, oWmfStretchDib.YSrc + oWmfStretchDib.SrcHeight);

		DrawImage(oDestRect, oScrcRect, oWmfStretchDib.ColorUsage, oWmfStretchDib.RasterOperation);
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

		double dStartAngle = GetEllipseAngle((int)shLeft, (int)shTop, (int)shRight, (int)shBottom, (int)shXStartArc, (int)shYStartArc);
		double dEndAngle   = GetEllipseAngle((int)shLeft, (int)shTop, (int)shRight, (int)shBottom, (int)shXEndArc, (int)shYEndArc);
		double dSweepAngle = GetSweepAngle(dStartAngle, dEndAngle);

		m_pDC->SetCurPos(shXStartArc, shYStartArc);
		ArcTo(shLeft, shTop, shRight, shBottom, dStartAngle, dSweepAngle);
		DrawPath(true, false);
		m_pDC->SetCurPos(shXEndArc, shYEndArc);
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

	void CWmfParserBase::HANDLE_META_EXTTEXTOUT(short shY, short shX, short shStringLength, unsigned short ushFwOptions, const TWmfRect &oRectangle, unsigned char *pString, short *pDx)
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

		CWmfRegion *pRegion = m_pDC->GetRegion();

		if (NULL != pRegion)
		{
			for (unsigned int unScanIndex = 0; unScanIndex < pRegion->ScanCount; ++unScanIndex)
			{
				TWmfScanObject *pScanObject = &pRegion->aScans[unScanIndex];

				if (pScanObject->Count == 0) continue;

				for (unsigned int unIndex = 0; unIndex < pScanObject->Count >> 1; ++unIndex)
				{
					MoveTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Top);
					LineTo(pScanObject->ScanLines[unIndex].Right, pScanObject->Top);
					LineTo(pScanObject->ScanLines[unIndex].Right, pScanObject->Bottom);
					LineTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Bottom);
					LineTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Top);
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

		CWmfRegion *pRegion = m_pDC->GetRegion();

		if (NULL != pRegion)
		{
			for (unsigned int unScanIndex = 0; unScanIndex < pRegion->ScanCount; ++unScanIndex)
			{
				TWmfScanObject *pScanObject = &pRegion->aScans[unScanIndex];

				if (pScanObject->Count == 0) continue;

				for (unsigned int unIndex = 0; unIndex < pScanObject->Count >> 1; ++unIndex)
				{
					MoveTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Top);
					LineTo(pScanObject->ScanLines[unIndex].Right, pScanObject->Top);
					LineTo(pScanObject->ScanLines[unIndex].Right, pScanObject->Bottom);
					LineTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Bottom);
					LineTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Top);
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

		CWmfRegion *pRegion = m_pDC->GetRegion();

		if (NULL != pRegion)
		{
			for (unsigned int unScanIndex = 0; unScanIndex < pRegion->ScanCount; ++unScanIndex)
			{
				TWmfScanObject *pScanObject = &pRegion->aScans[unScanIndex];

				if (pScanObject->Count == 0) continue;

				for (unsigned int unIndex = 0; unIndex < pScanObject->Count >> 1; ++unIndex)
				{
					MoveTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Top);
					LineTo(pScanObject->ScanLines[unIndex].Right, pScanObject->Top);
					LineTo(pScanObject->ScanLines[unIndex].Right, pScanObject->Bottom);
					LineTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Bottom);
					LineTo(pScanObject->ScanLines[unIndex].Left,  pScanObject->Top);
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

	void CWmfParserBase::HANDLE_META_POLYLINE(const std::vector<TWmfPointS> &arPoints)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_POLYLINE(arPoints);

		if (arPoints.empty())
			return;

		MoveTo(arPoints[0].x, arPoints[0].y);

		for (short shIndex = 1; shIndex < arPoints.size(); shIndex++)
			LineTo(arPoints[shIndex].x, arPoints[shIndex].y);

		DrawPath(true, false);
	}

	void CWmfParserBase::HANDLE_META_POLYGON(const std::vector<TWmfPointS> &arPoints)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_POLYGON(arPoints);

		if (arPoints.empty())
			return;

		MoveTo(arPoints[0].x, arPoints[0].y);

		for (short shIndex = 1; shIndex < arPoints.size(); shIndex++)
			LineTo(arPoints[shIndex].x, arPoints[shIndex].y);

		ClosePath();
		DrawPath(true, true);
	}

	void CWmfParserBase::HANDLE_META_POLYPOLYGON(const std::vector<std::vector<TWmfPointS> > &arPolygons)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_POLYPOLYGON(arPolygons);

		if (arPolygons.empty())
			return;

		for (unsigned int unPolygonIndex = 0; unPolygonIndex < arPolygons.size(); ++unPolygonIndex)
		{
			if (arPolygons[unPolygonIndex].empty())
				continue;

			MoveTo(arPolygons[unPolygonIndex][0].x, arPolygons[unPolygonIndex][0].y);

			for (unsigned int unPointIndex = 1; unPointIndex < arPolygons[unPolygonIndex].size(); ++unPointIndex)
				LineTo(arPolygons[unPolygonIndex][unPointIndex].x, arPolygons[unPolygonIndex][unPointIndex].y);

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

	void CWmfParserBase::HANDLE_META_SETPIXEL(const TWmfColor &oColor, short shY, short shX)
	{
		BYTE pBgraBuffer[4];
		pBgraBuffer[0] = oColor.b;
		pBgraBuffer[1] = oColor.g;
		pBgraBuffer[2] = oColor.r;
		pBgraBuffer[3] = 0xff;

		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->HANDLE_META_SETPIXEL(oColor, shY, shX);
			m_pInterpretator->DrawBitmap(shX, shY, 1, 1, pBgraBuffer, 1, 1);
		}
	}

	void CWmfParserBase::HANDLE_META_TEXTOUT(short shStringLength, unsigned char *pString, short shY, short shX)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_TEXTOUT(shStringLength, pString, shY, shX);

		DrawText(pString, shStringLength, shX, shY, NULL);
	}

	void CWmfParserBase::HANDLE_META_CREATEBRUSHINDIRECT(TWmfLogBrush &oBrush)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_CREATEBRUSHINDIRECT(oBrush);

		CWmfBrush* pBrush = new CWmfBrush(oBrush);
		if (!pBrush)
			return SetError();

		if (BS_PATTERN == pBrush->BrushStyle || BS_DIBPATTERNPT == pBrush->BrushStyle)
		{
			pBrush->BrushStyle = BS_SOLID;
			pBrush->Color.Init();
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
			unsigned int unWidth = 0, unHeight = 0;
			if (ReadImage(ushColorUsage, &pBgra, &unWidth, &unHeight))
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
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_EXCLUDECLIPRECT(shBottom, shRight, shTop, shLeft);

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

		TranslatePoint(shLeft, shTop, oClip.dLeft, oClip.dTop);
		TranslatePoint(shRight, shBottom, oClip.dRight, oClip.dBottom);

		TWmfWindow* pWindow = m_pDC->GetWindow();
		TRectD oBB;

		TranslatePoint(pWindow->x, pWindow->y, oBB.dLeft, oBB.dTop);
		TranslatePoint(pWindow->x + pWindow->w, pWindow->y + pWindow->h, oBB.dRight, oBB.dBottom);

		m_pDC->GetClip()->Exclude(oClip, oBB);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_INTERSECTCLIPRECT(short shLeft, short shTop, short shRight, short shBottom)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_INTERSECTCLIPRECT(shLeft, shTop, shRight, shBottom);

		TRectD oClip;

		TranslatePoint(shLeft, shTop, oClip.dLeft, oClip.dTop);
		TranslatePoint(shRight, shBottom, oClip.dRight, oClip.dBottom);

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

		m_pDC->SetViewportScale((double)xNum / (double)xDenom, (double)yNum / (double)xDenom);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SCALEWINDOWEXT(short yDenom, short yNum, short xDenom, short xNum)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SCALEWINDOWEXT(yDenom, yNum, xDenom, xNum);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->SetWindowScale((double)xNum / (double)xDenom, (double)yNum / (double)xDenom);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_SETBKCOLOR(TWmfColor &oColor)
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

	void CWmfParserBase::HANDLE_META_SETTEXTCOLOR(TWmfColor &oColor)
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
	}

	void CWmfParserBase::HANDLE_META_SETWINDOWORG(short shX, short shY)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_META_SETWINDOWORG(shX, shY);

		m_pDC->SetWindowOrg(shX, shY);
		UpdateOutputDC();
	}

	void CWmfParserBase::HANDLE_META_ESCAPE(unsigned short ushEscapeFunction, unsigned short ushByteCount)
	{
		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->HANDLE_META_ESCAPE(ushEscapeFunction, ushByteCount);

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

				m_oStream.ReadBytes(m_oEscapeBuffer.GetCurPtr(), unCurrentRecordSize);

				m_oEscapeBuffer.IncreasePosition(unCurrentRecordSize);

				if (0 == unRemainingBytes)
				{
					CEmfParser oEmfParser;

					oEmfParser.SetFontManager(GetFontManager());
					oEmfParser.SetStream(m_oEscapeBuffer.GetBuffer(), m_oEscapeBuffer.GetSize());
					oEmfParser.Scan();

					if (!oEmfParser.CheckError() && InterpretatorType::Render == m_pInterpretator->GetType())
					{
						CMetaFileRenderer oEmfOut(&oEmfParser, ((CWmfInterpretatorRender*)m_pInterpretator)->GetRenderer());
						oEmfParser.SetInterpretator(&oEmfOut);

						oEmfParser.PlayFile();

						m_bEof = true;
					}
					else if (!oEmfParser.CheckError() && InterpretatorType::Svg == m_pInterpretator->GetType())
					{
						double dWidth, dHeight;

						((CWmfInterpretatorSvg*)m_pInterpretator)->GetSize(dWidth, dHeight);

						((CEmfParserBase*)&oEmfParser)->SetInterpretator(InterpretatorType::Svg, dWidth, dHeight);

						oEmfParser.PlayFile();

						((CWmfInterpretatorSvg*)m_pInterpretator)->SetXmlWriter(((CEmfInterpretatorSvg*)oEmfParser.GetInterpretator())->GetXmlWriter());

						m_bEof = true;
					}
				}
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
