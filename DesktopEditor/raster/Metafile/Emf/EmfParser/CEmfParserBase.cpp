#include "CEmfParserBase.h"

#include "../EmfInterpretator/CEmfInterpretatorRender.h"
#include "../EmfInterpretator/CEmfInterpretatorArray.h"
#include "../EmfInterpretator/CEmfInterpretatorSvg.h"
#include "../EmfInterpretator/CEmfInterpretator.h"

#ifdef METAFILE_SUPPORT_WMF_EMF_XML
#include "../EmfInterpretator/CEmfInterpretatorXml.h"
#endif

namespace MetaFile
{
	void CEmfParserBase::ImageProcessing(const TEmfAlphaBlend &oTEmfAlphaBlend)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int unWidth, unHeight;

		if (ReadImage(oTEmfAlphaBlend.offBmiSrc, oTEmfAlphaBlend.cbBmiSrc,
					  oTEmfAlphaBlend.offBitsSrc, oTEmfAlphaBlend.cbBitsSrc,
					  c_nTEmfAlphaBlendSize + 8, &pBgraBuffer, &unWidth, &unHeight))
		{
			if (m_pInterpretator)
			{
				if (0x00 == oTEmfAlphaBlend.AlphaFormat)
				{
					for (unsigned int unY = 0; unY < unHeight; unY++)
					{
						for (unsigned int unX = 0; unX < unWidth; unX++)
						{
							unsigned int unIndex = (unX + unY * unWidth) * 4;
							pBgraBuffer[unIndex + 3] = oTEmfAlphaBlend.SrcConstantAlpha;
						}
					}
				}
				else
				{
					double dAlphaKoef = oTEmfAlphaBlend.SrcConstantAlpha / 255.0;
					for (unsigned int unY = 0; unY < unHeight; unY++)
					{
						for (unsigned int unX = 0; unX < unWidth; unX++)
						{
							unsigned int unIndex = (unX + unY * unWidth) * 4;
							pBgraBuffer[unIndex + 3] = (unsigned char)(dAlphaKoef * pBgraBuffer[unIndex + 3]);
						}
					}

				}

				DrawImage(oTEmfAlphaBlend.xDest, oTEmfAlphaBlend.yDest,
						  oTEmfAlphaBlend.cxDest, oTEmfAlphaBlend.cyDest,
						  pBgraBuffer, unWidth, unHeight);
			}
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfStretchDIBITS &oTEmfStretchDIBITS)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		if (ReadImage(oTEmfStretchDIBITS.offBmiSrc, oTEmfStretchDIBITS.cbBmiSrc,
					  oTEmfStretchDIBITS.offBitsSrc, oTEmfStretchDIBITS.cbBitsSrc,
					  sizeof(TEmfStretchDIBITS) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			if (m_pInterpretator)
			{
				ProcessRasterOperation(oTEmfStretchDIBITS.BitBltRasterOperation, &pBgraBuffer, ulWidth, ulHeight);
				DrawImage(oTEmfStretchDIBITS.xDest, oTEmfStretchDIBITS.yDest,
						  oTEmfStretchDIBITS.cxDest, oTEmfStretchDIBITS.cyDest,
						  pBgraBuffer, ulWidth, ulHeight);
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

		if (ReadImage(oTEmfBitBlt.offBmiSrc, oTEmfBitBlt.cbBmiSrc,
					  oTEmfBitBlt.offBitsSrc, oTEmfBitBlt.cbBitsSrc,
					  unEmfBitBltRecordSize, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			DrawImage(oTEmfBitBlt.xDest, oTEmfBitBlt.yDest,
					  oTEmfBitBlt.cxDest, oTEmfBitBlt.cyDest,
					  pBgraBuffer, ulWidth, ulHeight);
		}

		if (m_pInterpretator)
		{
			if (0x00000042 == oTEmfBitBlt.BitBltRasterOperation) // BLACKNESS
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
			if (0x00FF0062 == oTEmfBitBlt.BitBltRasterOperation) // WHITENESS
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
			else if (0x00f00021 == oTEmfBitBlt.BitBltRasterOperation) // PATCOPY
			{
				CEmfLogBrushEx* pBrush = (CEmfLogBrushEx*)m_pDC->GetBrush();
				if (pBrush)
				{
					// Делаем цветом кисти
					pBgraBuffer = new BYTE[4];
					pBgraBuffer[0] = pBrush->Color.b;
					pBgraBuffer[1] = pBrush->Color.g;
					pBgraBuffer[2] = pBrush->Color.r;
					pBgraBuffer[3] = 0xff;

					ulWidth  = 1;
					ulHeight = 1;
				}
			}
			else if (0x005a0049 == oTEmfBitBlt.BitBltRasterOperation) // PATINVERT
			{
				CEmfLogBrushEx* pBrush = (CEmfLogBrushEx*)m_pDC->GetBrush();
				if (pBrush)
				{
					// Делаем цветом кисти
					pBgraBuffer = new BYTE[4];
					pBgraBuffer[0] = pBrush->Color.b;
					pBgraBuffer[1] = pBrush->Color.g;
					pBgraBuffer[2] = pBrush->Color.r;
					pBgraBuffer[3] = 30;

					ulWidth  = 1;
					ulHeight = 1;
				}
			}
			else if (0x00A000C9 == oTEmfBitBlt.BitBltRasterOperation) // PATINVERT
			{
				CEmfLogBrushEx* pBrush = (CEmfLogBrushEx*)m_pDC->GetBrush();
				if (pBrush)
				{
					// Делаем цветом кисти
					pBgraBuffer = new BYTE[4];
					pBgraBuffer[0] = pBrush->Color.b;
					pBgraBuffer[1] = pBrush->Color.g;
					pBgraBuffer[2] = pBrush->Color.r;
					pBgraBuffer[3] = 30;

					ulWidth  = 1;
					ulHeight = 1;
				}
			}

			if (pBgraBuffer)
				DrawImage(oTEmfBitBlt.xDest, oTEmfBitBlt.yDest,
						  oTEmfBitBlt.cxDest, oTEmfBitBlt.cyDest,
						  pBgraBuffer, ulWidth, ulHeight);
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;
		if (ReadImage(oTEmfSetDiBitsToDevice.offBmiSrc, oTEmfSetDiBitsToDevice.cbBmiSrc,
					  oTEmfSetDiBitsToDevice.offBitsSrc, oTEmfSetDiBitsToDevice.cbBitsSrc,
					  sizeof(TEmfSetDiBitsToDevice) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			// TODO: Нужно реализовать обрезку картинки по параметрам oBitmap.iStartScan и oBitmap.cScans
			DrawImage(oTEmfSetDiBitsToDevice.Bounds.lLeft, oTEmfSetDiBitsToDevice.Bounds.lTop,
					  oTEmfSetDiBitsToDevice.Bounds.lRight - oTEmfSetDiBitsToDevice.Bounds.lLeft,
					  oTEmfSetDiBitsToDevice.Bounds.lBottom - oTEmfSetDiBitsToDevice.Bounds.lTop,
					  pBgraBuffer, ulWidth, ulHeight);
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfStretchBLT &oTEmfStretchBLT)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		unsigned int unSkip = 108; // sizeof(TEmfStretchBLT) + 8 неправильно считает, из-з TXForm, там double, а в Emf они по 4 байта
		if (ReadImage(oTEmfStretchBLT.offBmiSrc, oTEmfStretchBLT.cbBmiSrc,
					  oTEmfStretchBLT.offBitsSrc, oTEmfStretchBLT.cbBitsSrc,
					  unSkip, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			if (m_pInterpretator)
			{
				ProcessRasterOperation(oTEmfStretchBLT.BitBltRasterOperation, &pBgraBuffer, ulWidth, ulHeight);
				DrawImage(oTEmfStretchBLT.xDest, oTEmfStretchBLT.yDest,
						  oTEmfStretchBLT.cxDest, oTEmfStretchBLT.cyDest,
						  pBgraBuffer, ulWidth, ulHeight);
			}
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}

	void CEmfParserBase::ImageProcessing(const TEmfDibPatternBrush &oTEmfDibPatternBrush, unsigned int ulBrushIndex)
	{
		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		if (ReadImage(oTEmfDibPatternBrush.offBmi, oTEmfDibPatternBrush.cbBmi,
					  oTEmfDibPatternBrush.offBits, oTEmfDibPatternBrush.cbBits,
					  sizeof(TEmfDibPatternBrush) + 12, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
			if (!pBrush)
				return SetError();

			pBrush->SetDibPattern(pBgraBuffer, ulWidth, ulHeight);
			m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
		}
	}

	void CEmfParserBase::DrawImage(int nX, int nY, int nW, int nH, BYTE *pImageBuffer, unsigned int unImageW, unsigned int unImageH)
	{
		if (NULL != m_pInterpretator)
		{
			double dX, dY, dR, dB;
			TranslatePoint(nX, nY, dX, dY);
			TranslatePoint(nX + nW, nY + nH, dR, dB);
			m_pInterpretator->DrawBitmap(dX, dY, dR - dX, dB - dY, pImageBuffer, unImageW, unImageH);
		}
	}

	void CEmfParserBase::TranslatePoint(TEmfPointL &oPoint, double &dX, double &dY)
	{
		TranslatePoint(oPoint.x, oPoint.y, dX, dY);
	}

	void CEmfParserBase::TranslatePoint(int nX, int nY, double &dX, double &dY)
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

	void CEmfParserBase::MoveTo(TEmfPointD &oPoint)
	{
		MoveTo(oPoint.x, oPoint.y);
	}


	void CEmfParserBase::MoveTo(TEmfPointL &oPoint)
	{
		MoveTo(oPoint.x, oPoint.y);
	}

	void CEmfParserBase::MoveTo(TEmfPointS &oPoint)
	{
		MoveTo(oPoint.x, oPoint.y);
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

	void CEmfParserBase::LineTo(TEmfPointD &oPoint)
	{
		LineTo(oPoint.x, oPoint.y);
	}

	void CEmfParserBase::LineTo(TEmfPointL &oPoint)
	{
		LineTo(oPoint.x, oPoint.y);
	}

	void CEmfParserBase::LineTo(TEmfPointS &oPoint)
	{
		LineTo(oPoint.x, oPoint.y);
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

	void CEmfParserBase::CurveTo(TEmfPointS &oPoint1, TEmfPointS &oPoint2, TEmfPointS &oPointE)
	{
		CurveTo(oPoint1.x, oPoint1.y, oPoint2.x, oPoint2.y, oPointE.x, oPointE.y);
	}

	void CEmfParserBase::CurveTo(TEmfPointL &oPoint1, TEmfPointL &oPoint2, TEmfPointL &oPointE)
	{
		CurveTo(oPoint1.x, oPoint1.y, oPoint2.x, oPoint2.y, oPointE.x, oPointE.y);
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
								  int *pnDx, int iGraphicsMode, TEmfScale oScale)
	{
		int nX = _nX;
		int nY = _nY;

		if (m_pDC->GetTextAlign() & TA_UPDATECP)
		{
			nX = m_pDC->GetCurPos().x;
			nY = m_pDC->GetCurPos().y;
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

			m_pInterpretator->DrawString(wsString, unCharsCount, dX, dY, pdDx, iGraphicsMode, oScale.dX, oScale.dY);

			if (pdDx)
				delete[] pdDx;
		}
	}

	void CEmfParserBase::DrawTextA(TEmfEmrText &oText, int iGraphicsMode, TEmfScale oScale)
	{
		if (!oText.OutputString)
			return SetError();

		IFont* pFont = GetFont();
		NSStringExt::CConverter::ESingleByteEncoding eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT;
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

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)oText.OutputString, oText.Chars, eCharSet);

		int* pDx = NULL;
		if (oText.OutputDx)
		{
			pDx = new int[oText.Chars];
			if (pDx)
			{
				for (unsigned int unIndex = 0; unIndex < oText.Chars; unIndex++)
				{
					pDx[unIndex] = oText.OutputDx[unIndex];

					// Пропускаем сдвиги по Y если они есть
					if (oText.Options & ETO_PDY)
						unIndex++;
				}
			}
		}

		DrawText(wsText, oText.Chars, oText.Reference.x, oText.Reference.y, pDx, iGraphicsMode, oScale);

		if (pDx)
			delete[] pDx;
	}

	void CEmfParserBase::DrawTextW(TEmfEmrText &oText, int iGraphicsMode, TEmfScale oScale)
	{
		if (!oText.OutputString)
			return SetError();

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oText.OutputString, oText.Chars);

		unsigned int unLen = 0;
		int* pDx = NULL;
		if (oText.OutputDx && oText.Chars)
		{
			// Здесь мы эмулируем конвертацию Utf16 в Utf32, чтобы правильно получить массив pDx
			pDx = new int[oText.Chars];
			unLen = 0;

			unsigned short* pUtf16 = (unsigned short*)oText.OutputString;
			wchar_t wLeading, wTrailing;
			unsigned int unCode;
			unsigned int unPos = 0;
			while (unPos < oText.Chars)
			{
				wLeading = pUtf16[unPos++];
				if (wLeading < 0xD800 || wLeading > 0xDFFF)
				{
					pDx[unLen++] = oText.OutputDx[unPos - 1];
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
						pDx[unLen++] = oText.OutputDx[unPos - 2] + oText.OutputDx[unPos - 1];
					}

					// Пропускаем сдвиги по Y если они есть
					if (oText.Options & ETO_PDY)
						unPos++;
				}

				// Пропускаем сдвиги по Y если они есть
				if (oText.Options & ETO_PDY)
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
			DrawText(wsText, unLen, oText.Reference.x, oText.Reference.y, pDx, iGraphicsMode, oScale);

		if (pDx)
			delete[] pDx;
	}

	CEmfDC* CEmfParserBase::GetDC()
	{
		return m_pDC;
	}

	CEmfParserBase::CEmfParserBase()
		: m_oPlayer(this)
	{
		m_pPath = NULL;
		m_pDC   = m_oPlayer.GetDC();
		m_pInterpretator = NULL;
		m_bEof = false;
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

	TRect *CEmfParserBase::GetDCBounds()
	{
		return &m_oHeader.oFrameToBounds;
	}

	double CEmfParserBase::GetPixelHeight()
	{
		return m_pDC->GetPixelHeight();
	}

	double CEmfParserBase::GetPixelWidth()
	{
		return m_pDC->GetPixelWidth();
	}

	int CEmfParserBase::GetTextColor()
	{
		TEmfColor& oColor = m_pDC->GetTextColor();
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
	}

	IFont *CEmfParserBase::GetFont()
	{
		IFont* pFont = m_pDC->GetFont();

		if (!pFont)
			return NULL;

		return pFont;
	}

	IBrush *CEmfParserBase::GetBrush()
	{
		IBrush* pBrush = m_pDC->GetBrush();
		if (!pBrush)
			return NULL;

		return pBrush;
	}

	IPen *CEmfParserBase::GetPen()
	{
		IPen* pPen = m_pDC->GetPen();
		if (!pPen)
			return NULL;

		return pPen;
	}

	unsigned int CEmfParserBase::GetTextAlign()
	{
		return m_pDC->GetTextAlign();
	}

	unsigned int CEmfParserBase::GetTextBgMode()
	{
		return m_pDC->GetBgMode();
	}

	int CEmfParserBase::GetTextBgColor()
	{
		TEmfColor& oColor = m_pDC->GetBgColor();
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
	}

	unsigned int CEmfParserBase::GetFillMode()
	{
		return m_pDC->GetFillMode();
	}

	TPointD CEmfParserBase::GetCurPos()
	{
		TPointL oPoint = m_pDC->GetCurPos();
		TPointD oRes;
		TranslatePoint(oPoint.x, oPoint.y, oRes.x, oRes.y);
		return oRes;
	}

	TXForm *CEmfParserBase::GetInverseTransform()
	{
		return m_pDC->GetInverseTransform();
	}

	TXForm *CEmfParserBase::GetTransform(int iGraphicsMode)
	{
		TRect* pBounds = GetDCBounds();
		double dT = pBounds->nTop;
		double dL = pBounds->nLeft;

		TXForm oShiftXForm(1, 0, 0, 1, -dL, -dT);
		m_oTransform.Copy(m_pDC->GetFinalTransform(iGraphicsMode));
		m_oTransform.Multiply(oShiftXForm, MWT_RIGHTMULTIPLY);
		return &m_oTransform;
	}

	unsigned int CEmfParserBase::GetMiterLimit()
	{
		return m_pDC->GetMiterLimit();
	}

	unsigned int CEmfParserBase::GetRop2Mode()
	{
		return m_pDC->GetRop2Mode();
	}

	IClip *CEmfParserBase::GetClip()
	{
		CEmfClip* pClip = m_pDC->GetClip();
		if (!pClip)
			return NULL;

		return (IClip*)pClip;
	}

	int CEmfParserBase::GetCharSpace()
	{
		return 0;
	}

	bool CEmfParserBase::IsWindowFlippedY()
	{
		TEmfWindow* pWindow = m_pDC->GetWindow();
		return (pWindow->ulH < 0);
	}

	bool CEmfParserBase::IsWindowFlippedX()
	{
		TEmfWindow* pWindow = m_pDC->GetWindow();
		return (pWindow->ulW < 0);
	}

	unsigned int CEmfParserBase::GetMapMode()
	{
		return m_pDC->GetMapMode();
	}

	double CEmfParserBase::GetDpi()
	{
		return 96.;
	}

	IRegion *CEmfParserBase::GetRegion()
	{
		return NULL;
	}

	unsigned int CEmfParserBase::GetArcDirection()
	{
		return m_pDC->GetArcDirection();
	}

	bool CEmfParserBase::IsViewportFlippedY()
	{
		TEmfWindow* pViewport = m_pDC->GetViewport();
		return (pViewport->ulH < 0);
	}

	bool CEmfParserBase::IsViewportFlippedX()
	{
		TEmfWindow* pViewport = m_pDC->GetViewport();
		return (pViewport->ulW < 0);
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
			m_pInterpretator = new CEmfInterpretatorSvg(this, dWidth, dHeight);
	}

	CEmfInterpretatorBase* CEmfParserBase::GetInterpretator()
	{
		return m_pInterpretator;
	}

	TEmfRectL* CEmfParserBase::GetBounds()
	{
		return &m_oHeader.oFramePx;
	}

	CEmfPath *CEmfParserBase::GetPath() const
	{
		return m_pPath;
	}

	TEmfPointL CEmfParserBase::GetStartPointForArc(const TEmfRectL &oBox, double dStartAngle)
	{
		TEmfPointL oStartPoint;

		dStartAngle *= -M_PI / 180;

		double dWidth  = oBox.lRight  - oBox.lLeft;
		double dHeight = oBox.lBottom - oBox.lTop;

		double dTan = atan2( sin( dStartAngle ) / dHeight / 2,  cos( dStartAngle ) / dWidth / 2 );

		oStartPoint.x = oBox.lLeft + dWidth / 2.0 + dWidth / 2 * cos(dTan);
		oStartPoint.y = oBox.lTop + dHeight / 2.0 - dHeight / 2 * sin(dTan);

		return oStartPoint;
	}

	void CEmfParserBase::HANDLE_EMR_HEADER(TEmfHeader &oTEmfHeader)
	{
		if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
			return SetError();

		// Пропускаем остальную часть заголовка, т.к. она нас пока не интересует
		unsigned int ulRemaining = m_ulRecordSize - 80; // sizeof(TEmfHeader)
		m_oStream.Skip(ulRemaining);

		double dL = m_oHeader.oFrame.lLeft / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx;
		double dR = m_oHeader.oFrame.lRight / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx;
		double dT = m_oHeader.oFrame.lTop / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy;
		double dB = m_oHeader.oFrame.lBottom / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy;

		double dW = dR - dL;
		double dH = dB - dT;

		int nL = (int)floor(dL + 0.5);
		int nT = (int)floor(dT + 0.5);
		int nR = (int)floor(dW + 0.5) + nL;
		int nB = (int)floor(dH + 0.5) + nT;

		// По логике мы должны получать рект, точно такой же как и oBounds, но есть файлы, где это не так.
		m_oHeader.oFrameToBounds.nLeft   = nL;
		m_oHeader.oFrameToBounds.nRight  = nR;
		m_oHeader.oFrameToBounds.nTop    = nT;
		m_oHeader.oFrameToBounds.nBottom = nB;

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

		m_oPlayer.SaveDC();
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_RESTOREDC(int &nIndexDC)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_RESTOREDC(nIndexDC);

		if (nIndexDC >= 0)
		{
			SetError();
			return;
		}

		m_oPlayer.RestoreDC(nIndexDC);
		m_pDC = m_oPlayer.GetDC();
		UpdateOutputDC();
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

	void CEmfParserBase::HANDLE_EMR_SETTEXTCOLOR(TEmfColor &oColor)
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

		pPen->Width = unWidthX;

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

		m_pPath = new CEmfPath();
		if (!m_pPath)
			SetError();

		// Иногда MoveTo идет до BeginPath
		TEmfPointL oPoint = m_pDC->GetCurPos();
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

	void CEmfParserBase::HANDLE_EMR_MOVETOEX(TEmfPointL &oPoint)
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

	void CEmfParserBase::HANDLE_EMR_FILLPATH(TEmfRectL &oBounds)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_FILLPATH(oBounds);

		if (m_pPath)
		{
			m_pPath->Draw(m_pInterpretator, false, true);
			RELEASEOBJECT(m_pPath);
		}
	}

	void CEmfParserBase::HANDLE_EMR_SETMAPMODE(unsigned int &unMapMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETMAPMODE(unMapMode);

		m_pDC->SetMapMode(unMapMode);
	}

	void CEmfParserBase::HANDLE_EMR_SETWINDOWORGEX(TEmfPointL &oOrigin)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETWINDOWORGEX(oOrigin);

		m_pDC->SetWindowOrigin(oOrigin);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETWINDOWEXTEX(TEmfSizeL &oExtent)
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

	void CEmfParserBase::HANDLE_EMR_SETVIEWPORTORGEX(TEmfPointL &oOrigin)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETVIEWPORTORGEX(oOrigin);

		m_pDC->SetViewportOrigin(oOrigin);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_SETVIEWPORTEXTEX(TEmfSizeL &oExtent)
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

		if (m_pPath)
		{
			m_pDC->ClipToPath(m_pPath, unRegionMode, GetDC()->GetFinalTransform(GM_ADVANCED));
			RELEASEOBJECT(m_pPath);

			UpdateOutputDC();
		}
	}

	void CEmfParserBase::HANDLE_EMR_SETBKCOLOR(TEmfColor &oColor)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETBKCOLOR(oColor);

		m_pDC->SetBgColor(oColor);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_EXCLUDECLIPRECT(TEmfRectL &oClip)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXCLUDECLIPRECT(oClip);

		TRectD oClipRect, oBB;

		// Поскольку мы реализовываем данный тип клипа с помощью разницы внешнего ректа и заданного, и
		// пересечением с полученной областью, то нам надо вычесть границу заданного ректа.
		if (oClip.lLeft < oClip.lRight)
		{
			oClip.lLeft--;
			oClip.lRight++;
		}
		else
		{
			oClip.lLeft++;
			oClip.lRight--;
		}

		if (oClip.lTop < oClip.lBottom)
		{
			oClip.lTop--;
			oClip.lBottom++;
		}
		else
		{
			oClip.lTop++;
			oClip.lBottom--;
		}

		TranslatePoint(oClip.lLeft, oClip.lTop, oClipRect.dLeft, oClipRect.dTop);
		TranslatePoint(oClip.lRight, oClip.lBottom, oClipRect.dRight, oClipRect.dBottom);

		TRect* pRect = GetDCBounds();
		TranslatePoint(pRect->nLeft, pRect->nTop, oBB.dLeft, oBB.dTop);
		TranslatePoint(pRect->nRight, pRect->nBottom, oBB.dRight, oBB.dBottom);

		m_pDC->GetClip()->Exclude(oClipRect, oBB);
		UpdateOutputDC();
	}

	void CEmfParserBase::HANDLE_EMR_EXTSELECTCLIPRGN(unsigned int &unRgnDataSize, unsigned int &unRegionMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXTSELECTCLIPRGN(unRgnDataSize, unRegionMode, m_oStream);

		m_oStream.Skip(m_ulRecordSize - 8);

		// Тут просто сбрасываем текущий клип. Ничего не добавляем в клип, т.е. реализовать регионы с
		// текущим интерфейсом рендерера невозможно.
		m_pDC->GetClip()->Reset();
	}

	void CEmfParserBase::HANDLE_EMR_SETMETARGN()
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETMETARGN();

		m_pDC->GetClip()->Reset();
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

	void CEmfParserBase::HANDLE_EMR_INTERSECTCLIPRECT(TEmfRectL &oClip)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_INTERSECTCLIPRECT(oClip);

		TRectD oClipRect;
		TranslatePoint(oClip.lLeft, oClip.lTop, oClipRect.dLeft, oClipRect.dTop);
		TranslatePoint(oClip.lRight, oClip.lBottom, oClipRect.dRight, oClipRect.dBottom);
		m_pDC->GetClip()->Intersect(oClipRect);
	}

	void CEmfParserBase::HANDLE_EMR_SETLAYOUT(unsigned int &unLayoutMode)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETLAYOUT(unLayoutMode);
		// TODO: реализовать
	}

	void CEmfParserBase::HANDLE_EMR_SETBRUSHORGEX(TEmfPointL &oOrigin)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETBRUSHORGEX(oOrigin);
		// TODO: реализовать
	}

	void CEmfParserBase::HANDLE_EMR_ANGLEARC(TEmfPointL &oCenter, unsigned int &unRadius, double &dStartAngle, double &dSweepAngle)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_ANGLEARC(oCenter, unRadius, dStartAngle, dSweepAngle);

		ArcTo(oCenter.x - unRadius, oCenter.y - unRadius, oCenter.x + unRadius, oCenter.y + unRadius, dStartAngle, dSweepAngle);
		DrawPath(true, false);
	}

	void CEmfParserBase::HANDLE_EMR_ARC(TEmfRectL &oBox, TEmfPointL &oStart, TEmfPointL &oEnd)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_ARC(oBox, oStart, oEnd);

		double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

		// TODO: Проверить здесь
		//                if (dSweepAngle < 0.001)
		//                        dSweepAngle += 360;

		// TODO: Проверить здесь
		if (AD_COUNTERCLOCKWISE != m_pDC->GetArcDirection())
		{
			dSweepAngle = dSweepAngle - 360;
		}

		oBox.Update(IsViewportFlippedX(),	IsViewportFlippedY()); // Если ширина отрицательная, то не нарисуется

		TEmfPointL oStartDraw = GetStartPointForArc(oBox, dStartAngle);

		MoveTo(oStartDraw);
		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweepAngle);
		DrawPath(true, false);
	}

	void CEmfParserBase::HANDLE_EMR_ARCTO(TEmfRectL &oBox, TEmfPointL &oStart, TEmfPointL &oEnd)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_ARCTO(oBox, oStart, oEnd);

		double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

		oBox.Update(IsViewportFlippedX(),	IsViewportFlippedY()); // Если ширина отрицательная, то не нарисуется

		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweepAngle);
		DrawPath(true, false);
	}

	void CEmfParserBase::HANDLE_EMR_CHORD(TEmfRectL &oBox, TEmfPointL &oStart, TEmfPointL &oEnd)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_CHORD(oBox, oStart, oEnd);

		double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

		TEmfPointL oStartDraw = GetStartPointForArc(oBox, dStartAngle);

		MoveTo(oStartDraw);
		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweepAngle);
		LineTo(oStart);
		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_ELLIPSE(TEmfRectL &oBox)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_ELLIPSE(oBox);

		oBox.Update(IsViewportFlippedX(),	IsViewportFlippedY()); // Если ширина отрицательная, то не нарисуется

		if (m_pDC->GetArcDirection() == AD_COUNTERCLOCKWISE)
			ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, 0, 360);
		else
			ArcTo(oBox.lLeft, oBox.lBottom, oBox.lRight, oBox.lTop, 0, 360);

		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_EXTTEXTOUTA(TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXTTEXTOUTA(oTEmfExtTextoutA);

		if (oTEmfExtTextoutA.aEmrText.Chars > 0)
			DrawTextA(oTEmfExtTextoutA.aEmrText, oTEmfExtTextoutA.iGraphicsMode);
	}

	void CEmfParserBase::HANDLE_EMR_EXTTEXTOUTW(TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_EXTTEXTOUTW(oTEmfExtTextoutW);

		if (oTEmfExtTextoutW.wEmrText.Chars > 0)
		{
			TEmfScale oScale(oTEmfExtTextoutW.exScale, oTEmfExtTextoutW.eyScale);
			DrawTextW(oTEmfExtTextoutW.wEmrText, oTEmfExtTextoutW.iGraphicsMode, oScale);
		}
	}

	void CEmfParserBase::HANDLE_EMR_LINETO(TEmfPointL &oPoint)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_LINETO(oPoint);

		LineTo(oPoint);
	}

	void CEmfParserBase::HANDLE_EMR_PIE(TEmfRectL &oBox, TEmfPointL &oStart, TEmfPointL &oEnd)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_PIE(oBox, oStart, oEnd);

		double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweepAngle);
		LineTo((oBox.lLeft + oBox.lRight) / 2, (oBox.lTop + oBox.lBottom) / 2);
		ClosePath();
		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_RECTANGLE(TEmfRectL &oBox)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_RECTANGLE(oBox);

		if (AD_COUNTERCLOCKWISE == m_pDC->GetArcDirection())
		{
			MoveTo(oBox.lLeft, oBox.lTop);
			LineTo(oBox.lLeft, oBox.lBottom);
			LineTo(oBox.lRight, oBox.lBottom);
			LineTo(oBox.lRight, oBox.lTop);
		}
		else
		{
			MoveTo(oBox.lLeft, oBox.lTop);
			LineTo(oBox.lRight, oBox.lTop);
			LineTo(oBox.lRight, oBox.lBottom);
			LineTo(oBox.lLeft, oBox.lBottom);
		}
		ClosePath();
		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_ROUNDRECT(TEmfRectL &oBox, TEmfSizeL &oCorner)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_ROUNDRECT(oBox, oCorner);

		int lBoxW = oBox.lRight - oBox.lLeft;
		int lBoxH = oBox.lBottom - oBox.lTop;

		int lRoundW = (std::min)((int)oCorner.cx, lBoxW / 2);
		int lRoundH = (std::min)((int)oCorner.cy, lBoxH / 2);

		if (AD_COUNTERCLOCKWISE == m_pDC->GetArcDirection())
		{
			MoveTo(oBox.lLeft + lRoundW, oBox.lTop);
			ArcTo(oBox.lLeft, oBox.lTop, oBox.lLeft + lRoundW, oBox.lTop + lRoundH, 270, -90);
			LineTo(oBox.lLeft, oBox.lBottom - lRoundH);
			ArcTo(oBox.lLeft, oBox.lBottom - lRoundH, oBox.lLeft + lRoundW, oBox.lBottom, 180, -90);
			LineTo(oBox.lRight - lRoundW, oBox.lBottom);
			ArcTo(oBox.lRight - lRoundW, oBox.lBottom - lRoundH, oBox.lRight, oBox.lBottom, 90, -90);
			LineTo(oBox.lRight, oBox.lTop + lRoundH);
			ArcTo(oBox.lRight - lRoundW, oBox.lTop, oBox.lRight, oBox.lTop + lRoundH, 0, -90);
			LineTo(oBox.lLeft + lRoundW, oBox.lTop);
		}
		else
		{
			MoveTo(oBox.lLeft + lRoundW, oBox.lTop);
			LineTo(oBox.lRight - lRoundW, oBox.lTop);
			ArcTo(oBox.lRight - lRoundW, oBox.lTop, oBox.lRight, oBox.lTop + lRoundH, -90, 90);
			LineTo(oBox.lRight, oBox.lBottom - lRoundH);
			ArcTo(oBox.lRight - lRoundW, oBox.lBottom - lRoundH, oBox.lRight, oBox.lBottom, 0, 90);
			LineTo(oBox.lLeft + lRoundW, oBox.lBottom);
			ArcTo(oBox.lLeft, oBox.lBottom - lRoundH, oBox.lLeft + lRoundW, oBox.lBottom, 90, 90);
			LineTo(oBox.lLeft, oBox.lTop + lRoundH);
			ArcTo(oBox.lLeft, oBox.lTop, oBox.lLeft + lRoundW, oBox.lTop + lRoundH, 180, 90);
		}

		ClosePath();
		DrawPath(true, true);
	}

	void CEmfParserBase::HANDLE_EMR_SETPIXELV(TEmfPointL &oPoint, TEmfColor &oColor)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETPIXELV(oPoint, oColor);

		// Делаем цветом кисти
		BYTE pBgraBuffer[4];
		pBgraBuffer[0] = oColor.b;
		pBgraBuffer[1] = oColor.g;
		pBgraBuffer[2] = oColor.r;
		pBgraBuffer[3] = 0xff;

		DrawImage(oPoint.x, oPoint.y, 1, 1, pBgraBuffer, 1, 1);
	}

	void CEmfParserBase::HANDLE_EMR_SMALLTEXTOUT(TEmfSmallTextout &oText)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SMALLTEXTOUT(oText);

		// Переводим oText в TEmfEmrText
		TEmfEmrText oEmrText;
		oEmrText.Chars        = oText.cChars;
		oEmrText.offDx        = 0;
		oEmrText.offString    = 0;
		oEmrText.Options      = oText.fuOptions;
		oEmrText.OutputString = oText.TextString;
		oEmrText.Reference.x  = oText.x;
		oEmrText.Reference.y  = oText.y;
		oEmrText.OutputDx     = NULL;

		// Запись не документированна нормально, остается несколько байт в конце, непонятно почему.
		unsigned int unSize = oText.GetSize();
		if (m_ulRecordSize - unSize > 0)
			m_oStream.Skip(m_ulRecordSize - unSize);
		else if (m_ulRecordSize - unSize < 0)
			m_oStream.SeekBack(unSize - m_ulRecordSize);

		DrawTextW(oEmrText, oText.iGraphicsMode);

		// Поскольку мы просто скопировали ссылку на строку, а не скопировали сами строку обнуляем здесь, потому
		// что на деструкторе структуры освобождается память.
		oEmrText.OutputString = NULL;
	}

	void CEmfParserBase::HANDLE_EMR_STROKEANDFILLPATH(TEmfRectL &oBounds)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_STROKEANDFILLPATH(oBounds);

		if (m_pInterpretator && m_pPath)
		{
			m_pPath->Draw(m_pInterpretator, true, true);
			RELEASEOBJECT(m_pPath);
		}
	}

	void CEmfParserBase::HANDLE_EMR_STROKEPATH(TEmfRectL &oBounds)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_STROKEPATH(oBounds);

		if (m_pInterpretator && m_pPath)
		{
			m_pPath->Draw(m_pInterpretator, true, false);
			RELEASEOBJECT(m_pPath);
		}
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

	void CEmfParserBase::HANDLE_EMR_FILLRGN(const TEmfRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL>& arRects)
	{
		for (const TEmfRectL &oRect : arRects)
		{
			MoveTo(oRect.lLeft,  oRect.lTop);
			LineTo(oRect.lRight, oRect.lTop);
			LineTo(oRect.lRight, oRect.lBottom);
			LineTo(oRect.lLeft,  oRect.lBottom);
		}

		ClosePath();
		m_oPlayer.SelectObject(unIhBrush);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_FILLRGN(oBounds, unIhBrush, oRegionDataHeader, arRects);

		DrawPath(false, true);
	}

	void CEmfParserBase::HANDLE_EMR_PAINTRGN(const TEmfRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_PAINTRGN(oBounds, oRegionDataHeader, arRects);

		for (const TEmfRectL &oRect : arRects)
		{
			MoveTo(oRect.lLeft,  oRect.lTop);
			LineTo(oRect.lRight, oRect.lTop);
			LineTo(oRect.lRight, oRect.lBottom);
			LineTo(oRect.lLeft,  oRect.lBottom);
		}

		ClosePath();
		DrawPath(false, true);
	}

	void CEmfParserBase::HANDLE_EMR_FRAMERGN(const TEmfRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_FRAMERGN(oBounds, unIhBrush, nWidth, nHeight, oRegionDataHeader, arRects);

		//TODO: реализовать
	}

	void CEmfParserBase::HANDLE_EMR_POLYBEZIER(TEmfRectL &oBounds, std::vector<TEmfPointL> &arPoints)
	{
		HANDLE_EMR_POLYBEZIER_BASE<TEmfPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYBEZIER(TEmfRectL &oBounds, std::vector<TEmfPointS> &arPoints)
	{
		HANDLE_EMR_POLYBEZIER_BASE<TEmfPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYBEZIERTO(TEmfRectL &oBounds, std::vector<TEmfPointL> &arPoints)
	{
		HANDLE_EMR_POLYBEZIERTO_BASE<TEmfPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYBEZIERTO(TEmfRectL &oBounds, std::vector<TEmfPointS> &arPoints)
	{
		HANDLE_EMR_POLYBEZIERTO_BASE<TEmfPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYDRAW(TEmfRectL &oBounds, TEmfPointL *arPoints, unsigned int &unCount, unsigned char *pAbTypes)
	{
		HANDLE_EMR_POLYDRAW_BASE<TEmfPointL>(oBounds, arPoints, unCount, pAbTypes);
	}
	void CEmfParserBase::HANDLE_EMR_POLYDRAW(TEmfRectL &oBounds, TEmfPointS *arPoints, unsigned int &unCount, unsigned char *pAbTypes)
	{
		HANDLE_EMR_POLYDRAW_BASE<TEmfPointS>(oBounds, arPoints, unCount, pAbTypes);
	}
	void CEmfParserBase::HANDLE_EMR_POLYGON(TEmfRectL &oBounds, std::vector<TEmfPointL> &arPoints)
	{
		HANDLE_EMR_POLYGON_BASE<TEmfPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYGON(TEmfRectL &oBounds, std::vector<TEmfPointS> &arPoints)
	{
		HANDLE_EMR_POLYGON_BASE<TEmfPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYLINE(TEmfRectL &oBounds, std::vector<TEmfPointL> &arPoints)
	{
		HANDLE_EMR_POLYLINE_BASE<TEmfPointL>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYLINE(TEmfRectL &oBounds, std::vector<TEmfPointS> &arPoints)
	{
		HANDLE_EMR_POLYLINE_BASE<TEmfPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYLINETO(TEmfRectL &oBounds, std::vector<TEmfPointL> &arPoints)
	{
		HANDLE_EMR_POLYLINETO_BASE<TEmfPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYLINETO(TEmfRectL &oBounds, std::vector<TEmfPointS> &arPoints)
	{
		HANDLE_EMR_POLYLINETO_BASE<TEmfPointS>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYPOLYGON(TEmfRectL &oBounds, std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		HANDLE_EMR_POLYPOLYGON_BASE<TEmfPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYPOLYGON(TEmfRectL &oBounds, std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		HANDLE_EMR_POLYPOLYGON_BASE<TEmfPointS>(oBounds, arPoints);
	}
	void CEmfParserBase::HANDLE_EMR_POLYPOLYLINE(TEmfRectL &oBounds, std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		HANDLE_EMR_POLYPOLYLINE_BASE<TEmfPointL>(oBounds, arPoints);
	}

	void CEmfParserBase::HANDLE_EMR_POLYPOLYLINE(TEmfRectL &oBounds, std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		HANDLE_EMR_POLYPOLYLINE_BASE<TEmfPointS>(oBounds, arPoints);
	}
}
