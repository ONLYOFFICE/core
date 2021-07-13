#include "CEmfParserBase.h"

#include "../EmfInterpretator/CEmfInterpretator.h"

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
                CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
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
                CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
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
                CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
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

    void CEmfParserBase::MoveTo(int nX, int nY)
    {
        double dX, dY;
        TranslatePoint(nX, nY, dX, dY);

        if (m_pPath)
        {
            if (!m_pPath->MoveTo(dX, dY))
                return SetError();
        }
        else if (m_pInterpretator)
        {
            m_pInterpretator->MoveTo(dX, dY);
        }

        m_pDC->SetCurPos(nX, nY);
    }


    void CEmfParserBase::MoveTo(TEmfPointL &oPoint)
    {
        MoveTo(oPoint.x, oPoint.y);
    }

    void CEmfParserBase::MoveTo(TEmfPointS &oPoint)
    {
        MoveTo(oPoint.x, oPoint.y);
    }

    void CEmfParserBase::LineTo(int nX, int nY)
    {
        double dX, dY;
        TranslatePoint(nX, nY, dX, dY);

        if (m_pPath)
        {
            if (!m_pPath->LineTo(dX, dY))
                return SetError();
        }
        else if (m_pInterpretator)
        {
            m_pInterpretator->LineTo(dX, dY);
        }

        m_pDC->SetCurPos(nX, nY);
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

    void CEmfParserBase::DrawPath(bool bStroke, bool bFill)
    {
        if (m_pPath && m_pInterpretator)
        {
        }
        else if (m_pInterpretator)
        {
            int lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
            m_pInterpretator->DrawPath(lType);
            m_pInterpretator->EndPath();
        }
    }

    void CEmfParserBase::DrawText(std::wstring &wsString, unsigned int unCharsCount, int _nX, int _nY, int *pnDx, int iGraphicsMode)
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

            m_pInterpretator->DrawString(wsString, unCharsCount, dX, dY, pdDx, iGraphicsMode);

            if (pdDx)
                delete[] pdDx;
            }
    }

    void CEmfParserBase::DrawTextA(TEmfEmrText &oText, int iGraphicsMode)
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

        DrawText(wsText, oText.Chars, oText.Reference.x, oText.Reference.y, pDx, iGraphicsMode);

        if (pDx)
            delete[] pDx;
    }

    void CEmfParserBase::DrawTextW(TEmfEmrText &oText, int iGraphicsMode)
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
            DrawText(wsText, unLen, oText.Reference.x, oText.Reference.y, pDx, iGraphicsMode);

        if (pDx)
            delete[] pDx;
    }

    CEmfDC* CEmfParserBase::GetDC()
    {
        return m_pDC;
    }

    CEmfParserBase::CEmfParserBase() : m_oPlayer(this)
    {
        m_pPath = NULL;
        m_pDC   = m_oPlayer.GetDC();
        m_pInterpretator = NULL;
    }

    CEmfParserBase::~CEmfParserBase()
    {
        if (NULL != m_pInterpretator)
            delete m_pInterpretator;
    }

    void CEmfParserBase::PlayMetaFile()
    {
    }

    void CEmfParserBase::ClearFile()
    {
        RELEASEOBJECT(m_pPath);
        m_oPlayer.Clear();
        m_pDC = m_oPlayer.GetDC();
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
        CEmfLogFont* pFont = m_pDC->GetFont();
        if (!pFont)
            return NULL;

        return (IFont*)pFont;
    }

    IBrush *CEmfParserBase::GetBrush()
    {
        CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
        if (!pBrush)
            return NULL;

        return (IBrush*)pBrush;
    }

    IPen *CEmfParserBase::GetPen()
    {
        CEmfLogPen* pPen = m_pDC->GetPen();
        if (!pPen)
            return NULL;

        return (IPen*)pPen;
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

    void CEmfParserBase::SetInterpretator(IOutputDevice *pOutput)
    {
        if (NULL != m_pInterpretator)
            delete m_pInterpretator;

        m_pInterpretator = new CEmfInterpretatorRender(pOutput);
    }

    void CEmfParserBase::SetInterpretator(const wchar_t *wsFilePath, InterpretatorType oInterpretatorType)
    {
        if (NULL != m_pInterpretator)
            delete m_pInterpretator;

        if(oInterpretatorType == InterpretatorType::XML)
            m_pInterpretator = new CEmfInterpretatorXml(wsFilePath);
        else if (oInterpretatorType == InterpretatorType::Emf)
            m_pInterpretator = new CEmfInterpretator(wsFilePath);
    }

    void CEmfParserBase::SetInterpretator(IOutputDevice *pOutput, const wchar_t *wsFilePath)
    {
        if (NULL != m_pInterpretator)
            delete m_pInterpretator;

        m_pInterpretator = new CEmfInterpretatorAll(pOutput, wsFilePath);
    }

    TEmfRectL* CEmfParserBase::GetBounds()
    {
        return &m_oHeader.oFramePx;
    }
}
