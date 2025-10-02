#include "CEmfParserBase.h"

#include "../EmfInterpretator/CEmfInterpretatorRender.h"
#include "../EmfInterpretator/CEmfInterpretatorArray.h"
#include "../EmfInterpretator/CEmfInterpretatorSvg.h"
#include "../EmfInterpretator/CEmfInterpretator.h"

#ifdef METAFILE_SUPPORT_WMF_EMF_XML
#include "../EmfInterpretator/CEmfInterpretatorXml.h"
#endif

#ifndef GRADIENT_FILL_RECT_H
#define GRADIENT_FILL_RECT_H    0x00000000
#define GRADIENT_FILL_RECT_V    0x00000001
#define GRADIENT_FILL_TRIANGLE  0x00000002
#define GRADIENT_FILL_OP_FLAG   0x000000ff
#endif

#ifdef _DEBUG
#ifdef LOG_EMF_RECORDS
	#if 1 == LOG_EMF_RECORDS
		unsigned int unFileLevel = 0;
#endif
#endif
#endif

namespace MetaFile
{
	void CEmfParserBase::ImageProcessing(const TEmfAlphaBlend &oTEmfAlphaBlend)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int unWidth, unHeight;

		if (ReadImage(oTEmfAlphaBlend.unOffBmiSrc, oTEmfAlphaBlend.unCbBmiSrc,
		              oTEmfAlphaBlend.unOffBitsSrc, oTEmfAlphaBlend.unCbBitsSrc,
		              c_nTEmfAlphaBlendSize + 8, &pBgraBuffer, &unWidth, &unHeight))
		{
			if (m_pInterpretator)
			{
				if (0x00 == oTEmfAlphaBlend.uchAlphaFormat)
				{
					for (unsigned int unY = 0; unY < unHeight; unY++)
					{
						for (unsigned int unX = 0; unX < unWidth; unX++)
						{
							unsigned int unIndex = (unX + unY * unWidth) * 4;
							pBgraBuffer[unIndex + 3] = oTEmfAlphaBlend.uchSrcConstantAlpha;
						}
					}
				}
				else
				{
					double dAlphaKoef = oTEmfAlphaBlend.uchSrcConstantAlpha / 255.0;
					for (unsigned int unY = 0; unY < unHeight; unY++)
					{
						for (unsigned int unX = 0; unX < unWidth; unX++)
						{
							unsigned int unIndex = (unX + unY * unWidth) * 4;
							pBgraBuffer[unIndex + 3] = (unsigned char)(dAlphaKoef * pBgraBuffer[unIndex + 3]);
						}
					}

				}

				DrawImage(oTEmfAlphaBlend.nXDest, oTEmfAlphaBlend.nYDest,
						  oTEmfAlphaBlend.nCxDest, oTEmfAlphaBlend.nCyDest,
						  pBgraBuffer, unWidth, unHeight, BLEND_MODE_DEFAULT);
			}
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfStretchDIBITS &oTEmfStretchDIBITS)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		if (ReadImage(oTEmfStretchDIBITS.unOffBmiSrc, oTEmfStretchDIBITS.unCbBmiSrc,
		              oTEmfStretchDIBITS.unOffBitsSrc, oTEmfStretchDIBITS.unCbBitsSrc,
		              sizeof(TEmfStretchDIBITS) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			if (m_pInterpretator)
			{
				ProcessRasterOperation(oTEmfStretchDIBITS.unBitBltRasterOperation, &pBgraBuffer, ulWidth, ulHeight);

				DrawImage(oTEmfStretchDIBITS.nXDest, oTEmfStretchDIBITS.nYDest,
				          oTEmfStretchDIBITS.nCxDest, oTEmfStretchDIBITS.nCyDest,
				          pBgraBuffer, ulWidth, ulHeight, BLEND_MODE_DEFAULT);
			}
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfBitBlt &oTEmfBitBlt)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		// sizeof использовать нельзя, т.к. мы используем double в структуре TEmfBitBlt, а читаем его 4-байтовым
		unsigned int unEmfBitBltRecordSize = 100; // sizeof(TEmfBitBlt) + 8

		if (ReadImage(oTEmfBitBlt.unOffBmiSrc, oTEmfBitBlt.unCbBmiSrc,
					  oTEmfBitBlt.unOffBitsSrc, oTEmfBitBlt.unCbBitsSrc,
					  unEmfBitBltRecordSize, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			DrawImage(oTEmfBitBlt.nXDest, oTEmfBitBlt.nYDest,
					  oTEmfBitBlt.nCxDest, oTEmfBitBlt.nCyDest,
					  pBgraBuffer, ulWidth, ulHeight, BLEND_MODE_DEFAULT);
		}
		else
		{
			if (0x00000042 == oTEmfBitBlt.unBitBltRasterOperation) // BLACKNESS
			{
				// Делаем все черным цветом
				pBgraBuffer = new BYTE[4];
				pBgraBuffer[0] = 0x00;
				pBgraBuffer[1] = 0x00;
				pBgraBuffer[2] = 0x00;
				pBgraBuffer[3] = 0xff;

				ulWidth  = 1;
				ulHeight = 1;
			}
			if (0x00FF0062 == oTEmfBitBlt.unBitBltRasterOperation) // WHITENESS
			{
				// Делаем все черным цветом
				pBgraBuffer = new BYTE[4];
				pBgraBuffer[0] = 0xff;
				pBgraBuffer[1] = 0xff;
				pBgraBuffer[2] = 0xff;
				pBgraBuffer[3] = 0xff;

				ulWidth  = 1;
				ulHeight = 1;
			}
			else if (0x00f00021 == oTEmfBitBlt.unBitBltRasterOperation || // PATCOPY
			         0x005a0049 == oTEmfBitBlt.unBitBltRasterOperation || // PATINVERT
			         0x00A000C9 == oTEmfBitBlt.unBitBltRasterOperation)   // DPA
			{
				CEmfLogBrushEx* pBrush = (CEmfLogBrushEx*)m_pDC->GetBrush();
				if (pBrush)
				{
					// Делаем цветом кисти
					pBgraBuffer = new BYTE[4];
					pBgraBuffer[0] = pBrush->oColor.b;
					pBgraBuffer[1] = pBrush->oColor.g;
					pBgraBuffer[2] = pBrush->oColor.r;
					pBgraBuffer[3] = 0xff;

					ulWidth  = 1;
					ulHeight = 1;
				}
			}

			if (pBgraBuffer)
				DrawImage(oTEmfBitBlt.nXDest, oTEmfBitBlt.nYDest,
						  oTEmfBitBlt.nCxDest, oTEmfBitBlt.nCyDest,
						  pBgraBuffer, ulWidth, ulHeight, BLEND_MODE_DEFAULT);
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;
		if (ReadImage(oTEmfSetDiBitsToDevice.unOffBmiSrc, oTEmfSetDiBitsToDevice.unCbBmiSrc,
					  oTEmfSetDiBitsToDevice.unOffBitsSrc, oTEmfSetDiBitsToDevice.unCbBitsSrc,
					  sizeof(TEmfSetDiBitsToDevice) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			// TODO: Нужно реализовать обрезку картинки по параметрам oBitmap.iStartScan и oBitmap.cScans
			DrawImage(oTEmfSetDiBitsToDevice.oBounds.Left, oTEmfSetDiBitsToDevice.oBounds.Top,
					  oTEmfSetDiBitsToDevice.oBounds.Right - oTEmfSetDiBitsToDevice.oBounds.Left,
					  oTEmfSetDiBitsToDevice.oBounds.Bottom - oTEmfSetDiBitsToDevice.oBounds.Top,
					  pBgraBuffer, ulWidth, ulHeight, BLEND_MODE_DEFAULT);
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfStretchBLT &oTEmfStretchBLT)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		unsigned int unSkip = 108; // sizeof(TEmfStretchBLT) + 8 неправильно считает, из-з TXForm, там double, а в Emf они по 4 байта
		if (ReadImage(oTEmfStretchBLT.unOffBmiSrc, oTEmfStretchBLT.unCbBmiSrc,
					  oTEmfStretchBLT.unOffBitsSrc, oTEmfStretchBLT.unCbBitsSrc,
					  unSkip, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			if (m_pInterpretator)
			{
				ProcessRasterOperation(oTEmfStretchBLT.unBitBltRasterOperation, &pBgraBuffer, ulWidth, ulHeight);

				unsigned int unBlendMode{BLEND_MODE_DEFAULT};

				if (0x00660046 == oTEmfStretchBLT.unBitBltRasterOperation)
					unBlendMode = 0;

				DrawImage(oTEmfStretchBLT.nXDest, oTEmfStretchBLT.nYDest,
						  oTEmfStretchBLT.nCxDest, oTEmfStretchBLT.nCyDest,
						  pBgraBuffer, ulWidth, ulHeight, unBlendMode);
			}
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfDibPatternBrush &oTEmfDibPatternBrush, unsigned int ulBrushIndex)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		if (ReadImage(oTEmfDibPatternBrush.unOffBmi, oTEmfDibPatternBrush.unCbBmi,
					  oTEmfDibPatternBrush.unOffBits, oTEmfDibPatternBrush.unCbBits,
					  sizeof(TEmfDibPatternBrush) + 12, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
			if (!pBrush)
				return SetError();

			pBrush->SetDibPattern(pBgraBuffer, ulWidth, ulHeight);
			m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
		}
	}

	void CEmfParserBase::DrawImage(int nX, int nY, int nW, int nH, BYTE *pImageBuffer, unsigned int unImageW, unsigned int unImageH, unsigned int unBlendMode)
	{
		if (NULL != m_pInterpretator)
		{
			double dX, dY, dR, dB;
			TranslatePoint(nX, nY, dX, dY);
			TranslatePoint(nX + nW, nY + nH, dR, dB);
			m_pInterpretator->DrawBitmap(dX, dY, dR - dX, dB - dY, pImageBuffer, unImageW, unImageH, unBlendMode);
		}
	}

	void CEmfParserBase::TranslatePoint(TPointL &oPoint, double &dX, double &dY) const
	{
		TranslatePoint(oPoint.X, oPoint.Y, dX, dY);
	}

	void CEmfParserBase::TranslatePoint(int nX, int nY, double &dX, double &dY) const
	{
		dX = (double)nX;
		dY = (double)nY;
	}

	void CEmfParserBase::UpdateOutputDC()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->UpdateDC();
	}

	void CEmfParserBase::ClosePath()
	{
		if (m_pPath)
		{
			if (!m_pPath->Close())
				return SetError();
		}
		else if (m_pInterpretator)
			m_pInterpretator->ClosePath();
	}

	void CEmfParserBase::MoveTo(double dX, double dY)
	{
		if (m_pPath)
		{
			if (!m_pPath->MoveTo(dX, dY))
				return SetError();
		}
		else if (m_pInterpretator)
		{
			m_pInterpretator->MoveTo(dX, dY);
		}

		m_pDC->SetCurPos((int)dX, (int)dY);
	}

	void CEmfParserBase::MoveTo(int nX, int nY)
	{
		double dX, dY;
		TranslatePoint(nX, nY, dX, dY);
		MoveTo(dX, dY);
	}

	void CEmfParserBase::MoveTo(TPointD &oPoint)
	{
		MoveTo(oPoint.X, oPoint.Y);
	}


	void CEmfParserBase::MoveTo(TPointL &oPoint)
	{
		MoveTo(oPoint.X, oPoint.Y);
	}

	void CEmfParserBase::MoveTo(TPointS &oPoint)
	{
		MoveTo(oPoint.X, oPoint.Y);
	}

	void CEmfParserBase::LineTo(double dX, double dY)
	{
		if (m_pPath)
		{
			if (!m_pPath->LineTo(dX, dY))
				return SetError();
		}
		else if (m_pInterpretator)
		{
			m_pInterpretator->LineTo(dX, dY);
		}

		m_pDC->SetCurPos((int)dX, (int)dY);
	}

	void CEmfParserBase::LineTo(int nX, int nY)
	{
		double dX, dY;
		TranslatePoint(nX, nY, dX, dY);
		LineTo(dX, dY);
	}

	void CEmfParserBase::LineTo(TPointD &oPoint)
	{
		LineTo(oPoint.X, oPoint.Y);
	}

	void CEmfParserBase::LineTo(TPointL &oPoint)
	{
		LineTo(oPoint.X, oPoint.Y);
	}

	void CEmfParserBase::LineTo(TPointS &oPoint)
	{
		LineTo(oPoint.X, oPoint.Y);
	}

	void CEmfParserBase::CurveTo(int nX1, int nY1, int nX2, int nY2, int nXe, int nYe)
	{
		double dX1, dY1, dX2, dY2, dXe, dYe;
		TranslatePoint(nX1, nY1, dX1, dY1);
		TranslatePoint(nX2, nY2, dX2, dY2);
		TranslatePoint(nXe, nYe, dXe, dYe);

		if (m_pPath)
		{
			if (!m_pPath->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe))
				return SetError();
		}
		else if (m_pInterpretator)
		{
			m_pInterpretator->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
		}

		m_pDC->SetCurPos(nXe, nYe);
	}

	void CEmfParserBase::CurveTo(TPointS &oPoint1, TPointS &oPoint2, TPointS &oPointE)
	{
		CurveTo(oPoint1.X, oPoint1.Y, oPoint2.X, oPoint2.Y, oPointE.X, oPointE.Y);
	}

	void CEmfParserBase::CurveTo(TPointL &oPoint1, TPointL &oPoint2, TPointL &oPointE)
	{
		CurveTo(oPoint1.X, oPoint1.Y, oPoint2.X, oPoint2.Y, oPointE.X, oPointE.Y);
	}

	void CEmfParserBase::ArcTo(int nL, int nT, int nR, int nB, double dStart, double dSweep)
	{
		double dL, dT, dR, dB;
		TranslatePoint(nL, nT, dL, dT);
		TranslatePoint(nR, nB, dR, dB);

		if (m_pPath)
		{
			if (!m_pPath->ArcTo(dL, dT, dR, dB, dStart, dSweep))
				return SetError();
		}
		else if (m_pInterpretator)
		{
			m_pInterpretator->ArcTo(dL, dT, dR, dB, dStart, dSweep);
		}

		// Пересчет текущей позиции делается в каждой функции отдельно после вызова данной
	}

	void CEmfParserBase::DrawPath(bool bStroke, bool bFill, bool bEndPath)
	{
		if (m_pPath && m_pInterpretator)
		{
		}
		else if (m_pInterpretator)
		{
			int lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
			m_pInterpretator->DrawPath(lType);

			if (bEndPath)
				m_pInterpretator->EndPath();
		}
	}

	void CEmfParserBase::DrawText(std::wstring &wsString, unsigned int unCharsCount, int _nX, int _nY,
	                              int *pnDx, int iGraphicsMode, TScale oScale, bool bExChars)
	{
		int nX = _nX;
		int nY = _nY;

		if (m_pDC->GetTextAlign() & TA_UPDATECP && (0 == _nX && 0 == _nY))
		{
			nX = m_pDC->GetCurPos().X;
			nY = m_pDC->GetCurPos().Y;
		}

		if (m_pInterpretator)
		{
			double dX, dY;
			TranslatePoint(nX, nY, dX, dY);

			double* pdDx = NULL;
			if (pnDx)
			{
				pdDx = new double[unCharsCount];
				if (pdDx)
				{
					int nCurX = nX;
					double dCurX = dX;

					for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount; unCharIndex++)
					{
						int nX1 = nCurX + pnDx[unCharIndex];
						double dX1, dY1;
						TranslatePoint(nX1, nY, dX1, dY1);
						pdDx[unCharIndex] = dX1 - dCurX;
						nCurX = nX1;
						dCurX = dX1;
					}
				}
			}

			m_pInterpretator->DrawString(wsString, unCharsCount, dX, dY, pdDx, iGraphicsMode, oScale.X, oScale.Y, bExChars);

			if (pdDx)
				delete[] pdDx;
		}
	}

	void CEmfParserBase::DrawTextA(TEmrTextA &oText, int iGraphicsMode, TScale oScale)
	{
		if (!oText.pOutputString)
			return SetError();

		const IFont* pFont = GetFont();
		std::wstring wsText = ConvertToUnicode(oText.pOutputString, oText.unChars, (NULL != pFont) ? pFont->GetCharSet() : DEFAULT_CHARSET);

		int* pDx = NULL;
		if (oText.pOutputDx && oText.unChars == wsText.length())
		{
			pDx = new int[oText.unChars];
			if (pDx)
			{
				for (unsigned int unIndex = 0; unIndex < oText.unChars; unIndex++)
				{
					pDx[unIndex] = oText.pOutputDx[unIndex];

					// Пропускаем сдвиги по Y если они есть
					if (oText.unOptions & ETO_PDY)
						unIndex++;
				}
			}
		}

		DrawText(wsText, oText.unChars, oText.oReference.X, oText.oReference.Y, pDx, iGraphicsMode, oScale, oText.unOptions & 0x00000010);

		if (pDx)
			delete[] pDx;
	}

	void CEmfParserBase::DrawTextW(TEmrTextW &oText, int iGraphicsMode, TScale oScale)
	{
		if (!oText.pOutputString)
			return SetError();

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oText.pOutputString, oText.unChars);

		unsigned int unLen = 0;
		int* pDx = NULL;
		if (oText.pOutputDx && oText.unChars && oText.unChars == wsText.length())
		{
			// Здесь мы эмулируем конвертацию Utf16 в Utf32, чтобы правильно получить массив pDx
			pDx = new int[oText.unChars];
			unLen = 0;

			unsigned short* pUtf16 = (unsigned short*)oText.pOutputString;
			wchar_t wLeading, wTrailing;
			unsigned int unCode;
			unsigned int unPos = 0;
			while (unPos < oText.unChars)
			{
				wLeading = pUtf16[unPos++];
				if (wLeading < 0xD800 || wLeading > 0xDFFF)
				{
					pDx[unLen++] = oText.pOutputDx[unPos - 1];
				}
				else if (wLeading >= 0xDC00)
				{
					// Такого не должно быть
					continue;
				}
				else
				{
					unCode = (wLeading & 0x3FF) << 10;
					wTrailing = pUtf16[unPos++];
					if (wTrailing < 0xDC00 || wTrailing > 0xDFFF)
					{
						// Такого не должно быть
						continue;
					}
					else
					{
						pDx[unLen++] = oText.pOutputDx[unPos - 2] + oText.pOutputDx[unPos - 1];
					}

					// Пропускаем сдвиги по Y если они есть
					if (oText.unOptions & ETO_PDY)
						unPos++;
				}

				// Пропускаем сдвиги по Y если они есть
				if (oText.unOptions & ETO_PDY)
					unPos++;
			}
		}
		else
		{
			unLen = 0;
			unsigned int* pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsText, unLen);
			if (pUnicodes)
				delete[] pUnicodes;
		}

		if (unLen)
			DrawText(wsText, unLen, oText.oReference.X, oText.oReference.Y, pDx, iGraphicsMode, oScale, oText.unOptions & 0x00000010);

		if (pDx)
			delete[] pDx;
	}

	CEmfDC* CEmfParserBase::GetDC()
	{
		return m_pDC;
	}

	CEmfParserBase::CEmfParserBase()
		: m_oPlayer(this), m_pPath(NULL), m_pInterpretator(NULL), m_bEof(false)
	{
		m_pDC = m_oPlayer.GetDC();
	}

	CEmfParserBase::~CEmfParserBase(){}

	void CEmfParserBase::PlayMetaFile(){}

	void CEmfParserBase::ClearFile()
	{
		RELEASEOBJECT(m_pPath);
		m_oPlayer.Clear();
		m_pDC = m_oPlayer.GetDC();
		m_bEof = false;
	}

	const TRectL& CEmfParserBase::GetDCBounds() const
	{
		return m_oHeader.oFrameToBounds;
	}

	const CClip *CEmfParserBase::GetClip() const
	{
		return m_pDC->GetClip();
	}

	double CEmfParserBase::GetPixelHeight() const
	{
		return m_pDC->GetPixelHeight();
	}

	double CEmfParserBase::GetPixelWidth() const
	{
		return m_pDC->GetPixelWidth();
	}

	int CEmfParserBase::GetTextColor() const
	{
		return m_pDC->GetTextColor().ToInt();
	}

	const IFont *CEmfParserBase::GetFont() const
	{
		const IFont* pFont = m_pDC->GetFont();

		if (!pFont)
			return NULL;

		return pFont;
	}

	const IBrush *CEmfParserBase::GetBrush() const
	{
		return m_pDC->GetBrush();
	}

	const IPen *CEmfParserBase::GetPen() const
	{
		return m_pDC->GetPen();
	}

	unsigned int CEmfParserBase::GetTextAlign() const
	{
		return m_pDC->GetTextAlign();
	}

	unsigned int CEmfParserBase::GetTextBgMode() const
	{
		return m_pDC->GetBgMode();
	}

	int CEmfParserBase::GetTextBgColor() const
	{
		return m_pDC->GetBgColor()->ToInt();
	}

	unsigned int CEmfParserBase::GetFillMode() const
	{
		return m_pDC->GetFillMode();
	}

	TPointD CEmfParserBase::GetCurPos() const
	{
		TPointL oPoint = m_pDC->GetCurPos();
		TPointD oRes;
		TranslatePoint(oPoint.X, oPoint.Y, oRes.X, oRes.Y);
		return oRes;
	}

	const TXForm& CEmfParserBase::GetInverseTransform() const
	{
		return m_pDC->GetInverseTransform();
	}

	const TXForm& CEmfParserBase::GetTransform(int iGraphicsMode)
	{
		double dT = GetDCBounds().Top;
		double dL = GetDCBounds().Left;

		TXForm oShiftXForm(1, 0, 0, 1, -dL, -dT);
		m_oTransform.Copy(&m_pDC->GetFinalTransform(iGraphicsMode));
		m_oTransform.Multiply(oShiftXForm, MWT_RIGHTMULTIPLY);
		return m_oTransform;
	}

	unsigned int CEmfParserBase::GetMiterLimit() const
	{
		return m_pDC->GetMiterLimit();
	}

	unsigned int CEmfParserBase::GetRop2Mode() const
	{
		return m_pDC->GetRop2Mode();
	}

	int CEmfParserBase::GetCharSpace() const
	{
		return 0;
	}

	bool CEmfParserBase::IsWindowFlippedY() const
	{
		return m_pDC->GetWindow().ulH < 0;
	}

	bool CEmfParserBase::IsWindowFlippedX() const
	{
		return m_pDC->GetWindow().ulW < 0;
	}

	unsigned int CEmfParserBase::GetMapMode() const
	{
		return m_pDC->GetMapMode();
	}

	USHORT CEmfParserBase::GetDpi() const
	{
		return 96;
	}

	const IRegion *CEmfParserBase::GetRegion() const
	{
		return NULL;
	}

	unsigned int CEmfParserBase::GetArcDirection() const
	{
		return m_pDC->GetArcDirection();
	}
	
	const CPath *CEmfParserBase::GetPath() const
	{
		return m_pPath;
	}

	bool CEmfParserBase::IsViewportFlippedY() const
	{
		return m_pDC->GetViewport().ulH < 0;
	}

	bool CEmfParserBase::IsViewportFlippedX() const
	{
		return  m_pDC->GetViewport().ulW < 0;
	}

	void CEmfParserBase::SetInterpretator(IOutputDevice *pOutput)
	{
		if (NULL != m_pInterpretator)
			delete m_pInterpretator;

		m_pInterpretator = new CEmfInterpretatorRender(pOutput);
	}

	void CEmfParserBase::SetInterpretator(const wchar_t *wsFilePath, InterpretatorType oInterpretatorType, unsigned int unWidth, unsigned int unHeight)
	{
		if (NULL != m_pInterpretator)
			delete m_pInterpretator;

		if (oInterpretatorType == InterpretatorType::Emf)
			m_pInterpretator = new CEmfInterpretator(wsFilePath);
	#ifdef METAFILE_SUPPORT_WMF_EMF_XML
		else if(oInterpretatorType == InterpretatorType::XML)
			m_pInterpretator = new CEmfInterpretatorXml(wsFilePath);
	#endif
	}

	void CEmfParserBase::SetInterpretator(IOutputDevice *pOutput, const wchar_t *wsFilePath)
	{
		RELEASEOBJECT(m_pInterpretator);

		CEmfInterpretatorArray* pEmfInterpretatorArray = new CEmfInterpretatorArray;
		pEmfInterpretatorArray->AddRenderInterpretator(pOutput);

	#ifdef METAFILE_SUPPORT_WMF_EMF_XML
		pEmfInterpretatorArray->AddXmlInterpretator(wsFilePath);
	#endif

		m_pInterpretator = pEmfInterpretatorArray;
	}

	void CEmfParserBase::SetInterpretator(InterpretatorType oInterpretatorType, double dWidth, double dHeight)
	{
		RELEASEOBJECT(m_pInterpretator);

		if (InterpretatorType::Svg == oInterpretatorType)
		{
			CEmfInterpretatorSvg *pEmfInterpretatorSvg = new CEmfInterpretatorSvg(this, dWidth, dHeight);
			pEmfInterpretatorSvg->SetShapeRendering(EShapeRendering::OptimizeSpeed);
			m_pInterpretator = pEmfInterpretatorSvg;
		}
	}

	CEmfInterpretatorBase* CEmfParserBase::GetInterpretator()
	{
		return m_pInterpretator;
	}

	TXForm CEmfParserBase::CalculateCurrentTransform() const
	{
		if (NULL == m_pParent)
			return m_pDC->GetTransform();

		TXForm oTransform{m_pParent->CalculateCurrentTransform()};
		oTransform.Multiply(m_pDC->GetTransform(), MWT_RIGHTMULTIPLY);

		return oTransform;
	}

	const TRectL& CEmfParserBase::GetBounds() const
	{
		return m_oHeader.oFramePx;
	}

	CPath *CEmfParserBase::GetPath()
	{
		return m_pPath;
	}

	TPointL CEmfParserBase::GetStartPointForArc(const TRectL &oBox, double dStartAngle) const
	{
		TPointL oStartPoint;

		dStartAngle *= -M_PI / 180;

		double dWidth  = oBox.Right  - oBox.Left;
		double dHeight = oBox.Bottom - oBox.Top;

		double dTan = atan2( sin( dStartAngle ) / dHeight / 2,  cos( dStartAngle ) / dWidth / 2 );

		oStartPoint.X = oBox.Left + dWidth / 2.0 + dWidth / 2 * cos(dTan);
		oStartPoint.Y = oBox.Top + dHeight / 2.0 - dHeight / 2 * sin(dTan);

		return oStartPoint;
	}

	void CEmfParserBase::SaveDC()
	{
		m_oPlayer.SaveDC();
		UpdateOutputDC();
	}

	void CEmfParserBase::RestoreDC(int nIndex)
	{
		if (nIndex >= 0)
		{
			SetError();
			return;
		}

		m_oPlayer.RestoreDC(nIndex);
		m_pDC = m_oPlayer.GetDC();
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_HEADER(TEmfHeader &oTEmfHeader)
	{
		if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
			return SetError();

		// Пропускаем остальную часть заголовка, т.к. она нас пока не интересует
		unsigned int ulRemaining = m_ulRecordSize - 80; // sizeof(TEmfHeader)
		m_oStream.Skip(ulRemaining);

		double dL = m_oHeader.oFrame.Left / 100.0 / m_oHeader.oMillimeters.X * m_oHeader.oDevice.X;
		double dR = m_oHeader.oFrame.Right / 100.0 / m_oHeader.oMillimeters.X * m_oHeader.oDevice.X;
		double dT = m_oHeader.oFrame.Top / 100.0 / m_oHeader.oMillimeters.Y * m_oHeader.oDevice.Y;
		double dB = m_oHeader.oFrame.Bottom / 100.0 / m_oHeader.oMillimeters.Y * m_oHeader.oDevice.Y;

		double dW = dR - dL;
		double dH = dB - dT;

		int nL = (int)floor(dL + 0.5);
		int nT = (int)floor(dT + 0.5);
		int nR = (int)floor(dW + 0.5) + nL;
		int nB = (int)floor(dH + 0.5) + nT;

		// По логике мы должны получать рект, точно такой же как и oBounds, но есть файлы, где это не так.
		m_oHeader.oFrameToBounds.Left   = nL;
		m_oHeader.oFrameToBounds.Right  = nR;
		m_oHeader.oFrameToBounds.Top    = nT;
		m_oHeader.oFrameToBounds.Bottom = nB;

		m_oHeader.oFramePx = m_oHeader.oFrameToBounds;

		m_pDC->SetWindowExtents(m_oHeader.oDevice);
		m_pDC->SetViewportExtents(m_oHeader.oDevice);

		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->Begin();
			m_pInterpretator->HANDLE_EMR_HEADER(m_oHeader);
		}
	}

	void CEmfParserBase::HANDLE_EMR_ALPHABLEND(TEmfAlphaBlend &oTEmfAlphaBlend)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_ALPHABLEND(oTEmfAlphaBlend, m_oStream);

		ImageProcessing(oTEmfAlphaBlend);
	}

	void CEmfParserBase::HANDLE_EMR_STRETCHDIBITS(TEmfStretchDIBITS &oTEmfStretchDIBITS)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_STRETCHDIBITS(oTEmfStretchDIBITS, m_oStream);

		ImageProcessing(oTEmfStretchDIBITS);
	}

	void CEmfParserBase::HANDLE_EMR_BITBLT(TEmfBitBlt &oTEmfBitBlt)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_BITBLT(oTEmfBitBlt, m_oStream);

		ImageProcessing(oTEmfBitBlt);
	}

	void CEmfParserBase::HANDLE_EMR_SETDIBITSTODEVICE(TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETDIBITSTODEVICE(oTEmfSetDiBitsToDevice, m_oStream);

		ImageProcessing(oTEmfSetDiBitsToDevice);
	}

	void CEmfParserBase::HANDLE_EMR_STRETCHBLT(TEmfStretchBLT &oTEmfStretchBLT)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_STRETCHBLT(oTEmfStretchBLT, m_oStream);

		ImageProcessing(oTEmfStretchBLT);
	}

	void CEmfParserBase::HANDLE_EMR_EOF()
	{
		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->HANDLE_EMR_EOF();
			m_pInterpretator->End();
		}

		m_bEof = true;
	}

	void CEmfParserBase::HANDLE_EMR_SAVEDC()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SAVEDC();

		SaveDC();
	}

	void CEmfParserBase::HANDLE_EMR_RESTOREDC(int &nIndexDC)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_RESTOREDC(nIndexDC);

		RestoreDC(nIndexDC);
	}

	void CEmfParserBase::HANDLE_EMR_MODIFYWORLDTRANSFORM(TXForm &oXForm, unsigned int &unMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_MODIFYWORLDTRANSFORM(oXForm, unMode);

		m_pDC->MultiplyTransform(oXForm, unMode);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETWORLDTRANSFORM(TXForm &oXForm)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETWORLDTRANSFORM(oXForm);

		m_pDC->MultiplyTransform(oXForm, MWT_SET);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_CREATEBRUSHINDIRECT(unsigned int &unBrushIndex, CEmfLogBrushEx *pBrush)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_CREATEBRUSHINDIRECT(unBrushIndex, pBrush);

		m_oPlayer.RegisterObject(unBrushIndex, (CEmfObjectBase*)pBrush);
	}

	void CEmfParserBase::HANDLE_EMR_SETTEXTCOLOR(TRGBA &oColor)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETTEXTCOLOR(oColor);

		m_pDC->SetTextColor(oColor);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SELECTOBJECT(unsigned int &unObjectIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SELECTOBJECT(unObjectIndex);

		m_oPlayer.SelectObject(unObjectIndex);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_EXTCREATEFONTINDIRECTW(unsigned int &unIndex, CEmfLogFont *oLogFont)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXTCREATEFONTINDIRECTW(unIndex, oLogFont);

		m_oPlayer.RegisterObject(unIndex, (CEmfObjectBase*)oLogFont);
	}

	void CEmfParserBase::HANDLE_EMR_SETTEXTALIGN(unsigned int &unAlign)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETTEXTALIGN(unAlign);

		m_pDC->SetTextAlign(unAlign);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETBKMODE(unsigned int &unBgMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETBKMODE(unBgMode);

		m_pDC->SetBgMode(unBgMode);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_DELETEOBJECT(unsigned int &unObjectIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_DELETEOBJECT(unObjectIndex);

		m_oPlayer.DeleteObject(unObjectIndex);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETMITERLIMIT(unsigned int &unMeterLimit)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETMITERLIMIT(unMeterLimit);

		m_pDC->SetMiterLimit(unMeterLimit);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_EXTCREATEPEN(unsigned int &unPenIndex, CEmfLogPen *pPen, const std::vector<unsigned int>& arUnused)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXTCREATEPEN(unPenIndex, pPen, arUnused);

		m_oPlayer.RegisterObject(unPenIndex, (CEmfObjectBase*)pPen);
	}

	void CEmfParserBase::HANDLE_EMR_CREATEPEN(unsigned int &unPenIndex, unsigned int &unWidthX, CEmfLogPen *pPen)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_CREATEPEN(unPenIndex, unWidthX, pPen);

		pPen->unWidth = unWidthX;

		m_oPlayer.RegisterObject(unPenIndex, (CEmfObjectBase*)pPen);
	}

	void CEmfParserBase::HANDLE_EMR_SETPOLYFILLMODE(unsigned int &unFillMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETPOLYFILLMODE(unFillMode);

		m_pDC->SetFillMode(unFillMode);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_BEGINPATH()
	{
		if (m_pPath)
			delete m_pPath;

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_BEGINPATH();

		m_pPath = new CPath();
		if (!m_pPath)
			SetError();

		// Иногда MoveTo идет до BeginPath
		TPointL oPoint = m_pDC->GetCurPos();
		double dX, dY;
		TranslatePoint(oPoint, dX, dY);
		m_pPath->MoveTo(dX, dY);
	}

	void CEmfParserBase::HANDLE_EMR_ENDPATH()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_ENDPATH();
		// Ничего не делаем
	}

	void CEmfParserBase::HANDLE_EMR_CLOSEFIGURE()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_CLOSEFIGURE();

		if (m_pPath)
		{
			if (!m_pPath->Close())
				return SetError();
		}
	}

	void CEmfParserBase::HANDLE_EMR_FLATTENPATH()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_FLATTENPATH();
		// Ничего не делаем
	}

	void CEmfParserBase::HANDLE_EMR_WIDENPATH()
	{
		// TODO: реализовать
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_WIDENPATH();
	}

	void CEmfParserBase::HANDLE_EMR_ABORTPATH()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_ABORTPATH();

		if (m_pPath)
		{
			delete m_pPath;
			m_pPath = NULL;
		}
	}

	void CEmfParserBase::HANDLE_EMR_MOVETOEX(TPointL &oPoint)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_MOVETOEX(oPoint);

		MoveTo(oPoint);
	}

	void CEmfParserBase::HANDLE_EMR_SETARCDIRECTION(unsigned int &unDirection)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETARCDIRECTION(unDirection);

		m_pDC->SetArcDirection(unDirection);
		// Здесь не обновляем DC у Output, т.к. этот параметр разруливается внутри данного класса.
	}

	void CEmfParserBase::HANDLE_EMR_FILLPATH(TRectL &oBounds)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_FILLPATH(oBounds);

		if (m_pPath)
		{
			m_pPath->DrawOn(m_pInterpretator, false, true);
			m_pPath->Clear();
			RELEASEOBJECT(m_pPath);
		}
	}

	void CEmfParserBase::HANDLE_EMR_SETMAPMODE(unsigned int &unMapMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETMAPMODE(unMapMode);

		m_pDC->SetMapMode(unMapMode);
	}

	void CEmfParserBase::HANDLE_EMR_SETWINDOWORGEX(TPointL &oOrigin)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETWINDOWORGEX(oOrigin);

		m_pDC->SetWindowOrigin(oOrigin);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETWINDOWEXTEX(TSizeL &oExtent)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETWINDOWEXTEX(oExtent);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->SetWindowExtents(oExtent);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SCALEWINDOWEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SCALEWINDOWEXTEX(nXNum, nXDenom, nYNum, nYDenom);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->ScaleWindow((double)nXNum / (double)nXDenom, (double)nYNum / (double)nYDenom);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETVIEWPORTORGEX(TPointL &oOrigin)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETVIEWPORTORGEX(oOrigin);

		m_pDC->SetViewportOrigin(oOrigin);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETVIEWPORTEXTEX(TSizeL &oExtent)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETVIEWPORTEXTEX(oExtent);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->SetViewportExtents(oExtent);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SCALEVIEWPORTEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SCALEVIEWPORTEXTEX(nXNum, nXDenom, nYNum, nYDenom);

		if (MM_ISOTROPIC != m_pDC->GetMapMode() && MM_ANISOTROPIC != m_pDC->GetMapMode())
			return;

		m_pDC->ScaleViewport((double)nXNum / (double)nXDenom, (double)nYNum / (double)nYDenom);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETSTRETCHBLTMODE(unsigned int &unStretchMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETSTRETCHBLTMODE(unStretchMode);

		m_pDC->SetStretchMode(unStretchMode);
	}

	void CEmfParserBase::HANDLE_EMR_SETICMMODE(unsigned int &unICMMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETICMMODE(unICMMode);
	}

	void CEmfParserBase::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(unsigned int &unBrushIndex, TEmfDibPatternBrush &oDibBrush)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(unBrushIndex, oDibBrush, m_oStream);

		ImageProcessing(oDibBrush, unBrushIndex);
	}

	void CEmfParserBase::HANDLE_EMR_CREATEMONOBRUSH(unsigned int &unBrushIndex, TEmfDibPatternBrush &oDibBrush)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_CREATEMONOBRUSH(unBrushIndex, oDibBrush, m_oStream);

		ImageProcessing(oDibBrush, unBrushIndex);
	}

	void CEmfParserBase::HANDLE_EMR_SELECTCLIPPATH(unsigned int &unRegionMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SELECTCLIPPATH(unRegionMode);
		
		if (NULL != m_pPath)
		{
			m_pDC->GetClip()->SetPath(*m_pPath, unRegionMode, m_pDC->GetFinalTransform(GM_ADVANCED));
			RELEASEOBJECT(m_pPath);
			UpdateOutputDC();
		}
	}

	void CEmfParserBase::HANDLE_EMR_SETBKCOLOR(TRGBA &oColor)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETBKCOLOR(oColor);

		m_pDC->SetBgColor(oColor);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_EXCLUDECLIPRECT(TRectL &oClip)
	{
		TRectD oClipRect, oBB;

		// Поскольку мы реализовываем данный тип клипа с помощью разницы внешнего ректа и заданного, и
		// пересечением с полученной областью, то нам надо вычесть границу заданного ректа.
		if (oClip.Left < oClip.Right)
		{
			oClip.Left--;
			oClip.Right++;
		}
		else
		{
			oClip.Left++;
			oClip.Right--;
		}

		if (oClip.Top < oClip.Bottom)
		{
			oClip.Top--;
			oClip.Bottom++;
		}
		else
		{
			oClip.Top++;
			oClip.Bottom--;
		}

		TranslatePoint(oClip.Left, oClip.Top, oClipRect.Left, oClipRect.Top);
		TranslatePoint(oClip.Right, oClip.Bottom, oClipRect.Right, oClipRect.Bottom);

		const TRectL& oRect{GetDCBounds()};
		TranslatePoint(oRect.Left,  oRect.Top,    oBB.Left,  oBB.Top);
		TranslatePoint(oRect.Right, oRect.Bottom, oBB.Right, oBB.Bottom);

		m_pDC->GetClip()->Exclude(oClipRect, oBB);
		UpdateOutputDC();
		
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXCLUDECLIPRECT(oClip);
	}

	void CEmfParserBase::HANDLE_EMR_EXTSELECTCLIPRGN(unsigned int &unRgnDataSize, unsigned int &unRegionMode)
	{
		// Тут просто сбрасываем текущий клип. Ничего не добавляем в клип, т.е. реализовать регионы с
		// текущим интерфейсом рендерера невозможно.
		m_pDC->GetClip()->Reset();
		
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXTSELECTCLIPRGN(unRgnDataSize, unRegionMode, m_oStream);
		
		m_oStream.Skip(m_ulRecordSize - 8);
	}

	void CEmfParserBase::HANDLE_EMR_SETMETARGN()
	{
		m_pDC->GetClip()->Reset();
		
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETMETARGN();
		
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETROP2(unsigned int &unRop2Mode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETROP2(unRop2Mode);

		m_pDC->SetRop2Mode(unRop2Mode);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_CREATEPALETTE(unsigned int &unPaletteIndex, CEmfLogPalette *oEmfLogPalette)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_CREATEPALETTE(unPaletteIndex, oEmfLogPalette);

		m_oPlayer.RegisterObject(unPaletteIndex, (CEmfObjectBase*)oEmfLogPalette);
	}

	void CEmfParserBase::HANDLE_EMR_SELECTPALETTE(unsigned int &unPaletteIndex)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SELECTPALETTE(unPaletteIndex);

		m_oPlayer.SelectPalette(unPaletteIndex);
	}

	void CEmfParserBase::HANDLE_EMR_REALIZEPALETTE()
	{
		// TODO: Реализовать
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_REALIZEPALETTE();
	}

	void CEmfParserBase::HANDLE_EMR_INTERSECTCLIPRECT(TRectL &oClip)
	{
		TRectD oClipRect;
		TranslatePoint(oClip.Left, oClip.Top, oClipRect.Left, oClipRect.Top);
		TranslatePoint(oClip.Right, oClip.Bottom, oClipRect.Right, oClipRect.Bottom);

		m_pDC->GetClip()->Intersect(oClipRect);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_INTERSECTCLIPRECT(oClip);
	}

	void CEmfParserBase::HANDLE_EMR_SETLAYOUT(unsigned int &unLayoutMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETLAYOUT(unLayoutMode);
		// TODO: реализовать
	}

	void CEmfParserBase::HANDLE_EMR_SETBRUSHORGEX(TPointL &oOrigin)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETBRUSHORGEX(oOrigin);
		// TODO: реализовать
	}

	void CEmfParserBase::HANDLE_EMR_ANGLEARC(TPointL &oCenter, unsigned int &unRadius, double &dStartAngle, double &dSweepAngle)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_ANGLEARC(oCenter, unRadius, dStartAngle, dSweepAngle);

		ArcTo(oCenter.X - unRadius, oCenter.Y - unRadius, oCenter.X + unRadius, oCenter.Y + unRadius, dStartAngle, dSweepAngle);
		DrawPath(true, false);
	}

	void CEmfParserBase::HANDLE_EMR_ARC(TRectL &oBox, TPointL &oStart, TPointL &oEnd)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_ARC(oBox, oStart, oEnd);

		double dStartAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oStart.X, oStart.Y);
		double dSweepAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oEnd.X, oEnd.Y) - dStartAngle;

		// TODO: Проверить здесь
		//                if (dSweepAngle < 0.001)
		//                        dSweepAngle += 360;

		// TODO: Проверить здесь
		if (AD_COUNTERCLOCKWISE != m_pDC->GetArcDirection())
		{
			dSweepAngle = dSweepAngle - 360;
		}

		oBox.Update(IsViewportFlippedX(),	IsViewportFlippedY()); // Если ширина отрицательная, то не нарисуется

		TPointL oStartDraw = GetStartPointForArc(oBox, dStartAngle);

		MoveTo(oStartDraw);
		ArcTo(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, dStartAngle, dSweepAngle);
		DrawPath(true, false);
	}

	void CEmfParserBase::HANDLE_EMR_ARCTO(TRectL &oBox, TPointL &oStart, TPointL &oEnd)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_ARCTO(oBox, oStart, oEnd);

		double dStartAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oStart.X, oStart.Y);
		double dSweepAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oEnd.X, oEnd.Y) - dStartAngle;

		oBox.Update(IsViewportFlippedX(),	IsViewportFlippedY()); // Если ширина отрицательная, то не нарисуется

		ArcTo(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, dStartAngle, dSweepAngle);
		DrawPath(true, false);
	}

	void CEmfParserBase::HANDLE_EMR_CHORD(TRectL &oBox, TPointL &oStart, TPointL &oEnd)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_CHORD(oBox, oStart, oEnd);

		double dStartAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oStart.X, oStart.Y);
		double dSweepAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oEnd.X, oEnd.Y) - dStartAngle;

		TPointL oStartDraw = GetStartPointForArc(oBox, dStartAngle);

		MoveTo(oStartDraw);
		ArcTo(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, dStartAngle, dSweepAngle);
		LineTo(oStart);
		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_ELLIPSE(TRectL &oBox)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_ELLIPSE(oBox);

		oBox.Update(IsViewportFlippedX(),	IsViewportFlippedY()); // Если ширина отрицательная, то не нарисуется

		if (m_pDC->GetArcDirection() == AD_COUNTERCLOCKWISE)
			ArcTo(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, 0, 360);
		else
			ArcTo(oBox.Left, oBox.Bottom, oBox.Right, oBox.Top, 0, 360);

		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_EXTTEXTOUTA(TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXTTEXTOUTA(oTEmfExtTextoutA);

		if (oTEmfExtTextoutA.oEmrText.unChars > 0)
			DrawTextA(oTEmfExtTextoutA.oEmrText, oTEmfExtTextoutA.unIGraphicsMode);
	}

	void CEmfParserBase::HANDLE_EMR_EXTTEXTOUTW(TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXTTEXTOUTW(oTEmfExtTextoutW);

		if (oTEmfExtTextoutW.oEmrText.unChars > 0)
		{
			TScale oScale(oTEmfExtTextoutW.dExScale, oTEmfExtTextoutW.dEyScale);
			DrawTextW(oTEmfExtTextoutW.oEmrText, oTEmfExtTextoutW.unIGraphicsMode, oScale);
		}
	}

	void CEmfParserBase::HANDLE_EMR_LINETO(TPointL &oPoint)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_LINETO(oPoint);

		LineTo(oPoint);
	}

	void CEmfParserBase::HANDLE_EMR_PIE(TRectL &oBox, TPointL &oStart, TPointL &oEnd)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_PIE(oBox, oStart, oEnd);

		const int nCenterX = (oBox.Left + oBox.Right) / 2;
		const int nCenterY = (oBox.Top + oBox.Bottom) / 2;

		double dStartAngle = std::atan2(oStart.Y - nCenterY, oStart.X - nCenterX);
		double dEndAngle   = std::atan2(oEnd.Y   - nCenterY, oEnd.X   - nCenterX);

		if (dEndAngle > dStartAngle)
			dEndAngle -= 2 * M_PI;

		MoveTo(nCenterX, nCenterY);
		LineTo(oStart.X, oStart.Y);
		ArcTo(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, dStartAngle * 180. / M_PI, (dEndAngle - dStartAngle) * 180. / M_PI);
		LineTo(nCenterX, nCenterY);
		ClosePath();
		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_RECTANGLE(TRectL &oBox)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_RECTANGLE(oBox);

		if (AD_COUNTERCLOCKWISE == m_pDC->GetArcDirection())
		{
			MoveTo(oBox.Left, oBox.Top);
			LineTo(oBox.Left, oBox.Bottom);
			LineTo(oBox.Right, oBox.Bottom);
			LineTo(oBox.Right, oBox.Top);
		}
		else
		{
			MoveTo(oBox.Left, oBox.Top);
			LineTo(oBox.Right, oBox.Top);
			LineTo(oBox.Right, oBox.Bottom);
			LineTo(oBox.Left, oBox.Bottom);
		}
		ClosePath();
		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_ROUNDRECT(TRectL &oBox, TSizeL &oCorner)
	{
		if (NULL != m_pInterpretator && (NULL == m_pPath || Svg != m_pInterpretator->GetType()))
			m_pInterpretator->HANDLE_EMR_ROUNDRECT(oBox, oCorner);

		int lBoxW = oBox.Right - oBox.Left;
		int lBoxH = oBox.Bottom - oBox.Top;

		int lRoundW = (std::min)((int)oCorner.X, lBoxW / 2);
		int lRoundH = (std::min)((int)oCorner.Y, lBoxH / 2);

		if (AD_COUNTERCLOCKWISE == m_pDC->GetArcDirection())
		{
			MoveTo(oBox.Left + lRoundW, oBox.Top);
			ArcTo(oBox.Left, oBox.Top, oBox.Left + lRoundW, oBox.Top + lRoundH, 270, -90);
			LineTo(oBox.Left, oBox.Bottom - lRoundH);
			ArcTo(oBox.Left, oBox.Bottom - lRoundH, oBox.Left + lRoundW, oBox.Bottom, 180, -90);
			LineTo(oBox.Right - lRoundW, oBox.Bottom);
			ArcTo(oBox.Right - lRoundW, oBox.Bottom - lRoundH, oBox.Right, oBox.Bottom, 90, -90);
			LineTo(oBox.Right, oBox.Top + lRoundH);
			ArcTo(oBox.Right - lRoundW, oBox.Top, oBox.Right, oBox.Top + lRoundH, 0, -90);
			LineTo(oBox.Left + lRoundW, oBox.Top);
		}
		else
		{
			MoveTo(oBox.Left + lRoundW, oBox.Top);
			LineTo(oBox.Right - lRoundW, oBox.Top);
			ArcTo(oBox.Right - lRoundW, oBox.Top, oBox.Right, oBox.Top + lRoundH, -90, 90);
			LineTo(oBox.Right, oBox.Bottom - lRoundH);
			ArcTo(oBox.Right - lRoundW, oBox.Bottom - lRoundH, oBox.Right, oBox.Bottom, 0, 90);
			LineTo(oBox.Left + lRoundW, oBox.Bottom);
			ArcTo(oBox.Left, oBox.Bottom - lRoundH, oBox.Left + lRoundW, oBox.Bottom, 90, 90);
			LineTo(oBox.Left, oBox.Top + lRoundH);
			ArcTo(oBox.Left, oBox.Top, oBox.Left + lRoundW, oBox.Top + lRoundH, 180, 90);
		}

		ClosePath();
		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_SETPIXELV(TPointL &oPoint, TRGBA &oColor)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETPIXELV(oPoint, oColor);

		// Делаем цветом кисти
		BYTE pBgraBuffer[4];
		pBgraBuffer[0] = oColor.b;
		pBgraBuffer[1] = oColor.g;
		pBgraBuffer[2] = oColor.r;
		pBgraBuffer[3] = 0xff;

		DrawImage(oPoint.X, oPoint.Y, 1, 1, pBgraBuffer, 1, 1, BLEND_MODE_DEFAULT);
	}

	void CEmfParserBase::HANDLE_EMR_SMALLTEXTOUT(TEmfSmallTextout &oText)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SMALLTEXTOUT(oText);

		// Переводим oText в TEmfEmrText
		TEmrTextW oEmrText;
		
		oEmrText.unChars       = oText.unCChars;
		oEmrText.unOffDx       = 0;
		oEmrText.unOffString   = 0;
		oEmrText.unOptions     = oText.unFuOptions;
		oEmrText.pOutputString = oText.pTextString;
		oEmrText.oReference.X  = oText.nX;
		oEmrText.oReference.Y  = oText.nY;
		oEmrText.pOutputDx     = NULL;

		// Запись не документированна нормально, остается несколько байт в конце, непонятно почему.
		unsigned int unSize = oText.GetSize();
		if (m_ulRecordSize - unSize > 0)
			m_oStream.Skip(m_ulRecordSize - unSize);
		else if (m_ulRecordSize - unSize < 0)
			m_oStream.SeekBack(unSize - m_ulRecordSize);

		DrawTextW(oEmrText, oText.unIGraphicsMode);

		// Поскольку мы просто скопировали ссылку на строку, а не скопировали сами строку обнуляем здесь, потому
		// что на деструкторе структуры освобождается память.
		oEmrText.pOutputString = NULL;
	}

	void CEmfParserBase::HANDLE_EMR_STROKEANDFILLPATH(TRectL &oBounds)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_STROKEANDFILLPATH(oBounds);

		if (m_pInterpretator && m_pPath)
		{
			m_pPath->DrawOn(m_pInterpretator, true, true);
			m_pPath->Clear();
			RELEASEOBJECT(m_pPath);
		}
	}

	void CEmfParserBase::HANDLE_EMR_STROKEPATH(TRectL &oBounds)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_STROKEPATH(oBounds);

		if (m_pInterpretator && m_pPath)
		{
			m_pPath->DrawOn(m_pInterpretator, true, false);
			m_pPath->Clear();
			RELEASEOBJECT(m_pPath);
		}
	}

	void CEmfParserBase::HANDLE_EMR_GRADIENTFILL(const std::vector<TTriVertex> &arVertex, const std::vector<std::pair<int, int> > &arIndexes, unsigned int unFillMode)
	{
		if (3 > arVertex.size() || arVertex.size() > 4 || INT_MIN == arVertex[0].nX || INT_MIN == arVertex[0].nY || NULL == m_pInterpretator ||
		    0 == (arVertex[1].nX - arVertex[0].nX))
			return;

		CEmfPlusBrush oBrush;
		oBrush.unStyle = BS_LINEARGRADIENT;

		oBrush.oColor.chRed   = arVertex[0].ushRed   / 255.;
		oBrush.oColor.chGreen = arVertex[0].ushGreen / 255.;
		oBrush.oColor.chBlue  = arVertex[0].ushBlue  / 255.;
		oBrush.oColor.chAlpha = 255;

		if (GRADIENT_FILL_RECT_H == unFillMode || GRADIENT_FILL_TRIANGLE == unFillMode)
		{
			if (arVertex[1].nX - arVertex[0].nX != 1)
			{
				oBrush.oColorBack.chRed   = arVertex[1].ushRed   / 255.;
				oBrush.oColorBack.chGreen = arVertex[1].ushGreen / 255.;
				oBrush.oColorBack.chBlue  = arVertex[1].ushBlue  / 255.;
			}
			else
                            oBrush.unStyle = BS_SOLID;
		}
		else if (GRADIENT_FILL_RECT_V == unFillMode)
		{
			if (arVertex[2].nY - arVertex[0].nY != 1)
			{
				oBrush.oColorBack.chRed   = arVertex[2].ushRed   / 255.;
				oBrush.oColorBack.chGreen = arVertex[2].ushGreen / 255.;
				oBrush.oColorBack.chBlue  = arVertex[2].ushBlue  / 255.;
			}
			else
                            oBrush.unStyle = BS_SOLID;
		}
		oBrush.oColorBack.chAlpha = 255;

		const IBrush* pBrush = m_pDC->GetBrush();
		m_pDC->SetBrush(&oBrush);

		MoveTo(arVertex[0].nX, arVertex[0].nY);
		LineTo(arVertex[1].nX, arVertex[1].nY);
		LineTo(arVertex[2].nX, arVertex[2].nY);
		if (4 == arVertex.size())
			LineTo(arVertex[3].nX, arVertex[3].nY);

		ClosePath();
		DrawPath(false, true);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_GRADIENTFILL(arVertex, arIndexes, unFillMode);

		m_pDC->RemoveBrush(&oBrush);
		m_pDC->SetBrush((IBrush*)pBrush);
	}

	void CEmfParserBase::HANDLE_EMR_UNKNOWN(const unsigned int& unRecordSize)
	{
		if (NULL != m_pInterpretator)
		{
			m_oStream.SeekBack(8);
			m_pInterpretator->HANDLE_EMR_UNKNOWN(m_oStream);
		}

		m_oStream.Skip(unRecordSize);
	}

	void CEmfParserBase::HANDLE_EMR_FILLRGN(const TRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL>& arRects)
	{
		for (const TRectL &oRect : arRects)
		{
			MoveTo(oRect.Left,  oRect.Top);
			LineTo(oRect.Right, oRect.Top);
			LineTo(oRect.Right, oRect.Bottom);
			LineTo(oRect.Left,  oRect.Bottom);
		}

		ClosePath();
		m_oPlayer.SelectObject(unIhBrush);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_FILLRGN(oBounds, unIhBrush, oRegionDataHeader, arRects);

		DrawPath(false, true);
	}

	void CEmfParserBase::HANDLE_EMR_PAINTRGN(const TRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_PAINTRGN(oBounds, oRegionDataHeader, arRects);

		for (const TRectL &oRect : arRects)
		{
			MoveTo(oRect.Left,  oRect.Top);
			LineTo(oRect.Right, oRect.Top);
			LineTo(oRect.Right, oRect.Bottom);
			LineTo(oRect.Left,  oRect.Bottom);
		}

		ClosePath();
		DrawPath(false, true);
	}

	void CEmfParserBase::HANDLE_EMR_FRAMERGN(const TRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_FRAMERGN(oBounds, unIhBrush, nWidth, nHeight, oRegionDataHeader, arRects);

		//TODO: реализовать
	}

	void CEmfParserBase::HANDLE_EMR_POLYBEZIER(TRectL &oBounds, std::vector<TPointL> &arPoints)
	{
		HANDLE_EMR_POLYBEZIER_BASE<TPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYBEZIER(TRectL &oBounds, std::vector<TPointS> &arPoints)
	{
		HANDLE_EMR_POLYBEZIER_BASE<TPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYBEZIERTO(TRectL &oBounds, std::vector<TPointL> &arPoints)
	{
		HANDLE_EMR_POLYBEZIERTO_BASE<TPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYBEZIERTO(TRectL &oBounds, std::vector<TPointS> &arPoints)
	{
		HANDLE_EMR_POLYBEZIERTO_BASE<TPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYDRAW(TRectL &oBounds, TPointL *arPoints, unsigned int &unCount, unsigned char *pAbTypes)
	{
		HANDLE_EMR_POLYDRAW_BASE<TPointL>(oBounds, arPoints, unCount, pAbTypes);
	}
	void CEmfParserBase::HANDLE_EMR_POLYDRAW(TRectL &oBounds, TPointS *arPoints, unsigned int &unCount, unsigned char *pAbTypes)
	{
		HANDLE_EMR_POLYDRAW_BASE<TPointS>(oBounds, arPoints, unCount, pAbTypes);
	}
	void CEmfParserBase::HANDLE_EMR_POLYGON(TRectL &oBounds, std::vector<TPointL> &arPoints)
	{
		HANDLE_EMR_POLYGON_BASE<TPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYGON(TRectL &oBounds, std::vector<TPointS> &arPoints)
	{
		HANDLE_EMR_POLYGON_BASE<TPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYLINE(TRectL &oBounds, std::vector<TPointL> &arPoints)
	{
		HANDLE_EMR_POLYLINE_BASE<TPointL>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYLINE(TRectL &oBounds, std::vector<TPointS> &arPoints)
	{
		HANDLE_EMR_POLYLINE_BASE<TPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYLINETO(TRectL &oBounds, std::vector<TPointL> &arPoints)
	{
		HANDLE_EMR_POLYLINETO_BASE<TPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYLINETO(TRectL &oBounds, std::vector<TPointS> &arPoints)
	{
		HANDLE_EMR_POLYLINETO_BASE<TPointS>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYPOLYGON(TRectL &oBounds, std::vector<std::vector<TPointL>> &arPoints)
	{
		HANDLE_EMR_POLYPOLYGON_BASE<TPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYPOLYGON(TRectL &oBounds, std::vector<std::vector<TPointS>> &arPoints)
	{
		HANDLE_EMR_POLYPOLYGON_BASE<TPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYPOLYLINE(TRectL &oBounds, std::vector<std::vector<TPointL>> &arPoints)
	{
		HANDLE_EMR_POLYPOLYLINE_BASE<TPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYPOLYLINE(TRectL &oBounds, std::vector<std::vector<TPointS>> &arPoints)
	{
		HANDLE_EMR_POLYPOLYLINE_BASE<TPointS>(oBounds, arPoints);
	}
}
