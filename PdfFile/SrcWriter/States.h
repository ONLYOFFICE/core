/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _PDF_WRITER_SRC_STATE_H
#define _PDF_WRITER_SRC_STATE_H

#include "../../DesktopEditor/common/Types.h"
#include "../../DesktopEditor/graphics/AggPlusEnums.h"
#include "Types.h"

#include <string>
#include <vector>
#include <algorithm>

namespace PdfWriter
{
    class CPage;
    class CFontDict;
}

class CPdfWriter;

enum ERendererCommandType
{
    renderercommandtype_Text  = 0x01,
    renderercommandtype_Image = 0x02,
    renderercommandtype_Path  = 0x03,
    renderercommandtype_Clip  = 0x04
};
class CRendererCommandBase
{
public:
    virtual ~CRendererCommandBase(){};
    virtual ERendererCommandType GetType() = 0;
};
class CRendererTextCommand : public CRendererCommandBase
{
public:
    CRendererTextCommand(unsigned char* pCodes, unsigned int nLen, const double& dX, const double& dY)
    {
        m_pCodes      = pCodes;
        m_nLen        = nLen;
        m_dX          = dX;
        m_dY          = dY;
        m_pFont       = NULL;
        m_dSize       = -1;
        m_lColor      = 0;
        m_nAlpha      = 255;
        m_dCharSpace  = 0;
        m_dHorScaling = 100;
        m_nMode       = (int)PdfWriter::textrenderingmode_Fill;

        m_bNeedDoItalic = false;
        m_bNeedDoBold   = false;
    }
    ~CRendererTextCommand()
    {
        if (m_pCodes)
            delete[] m_pCodes;
    }
    ERendererCommandType GetType()
    {
        return renderercommandtype_Text;
    }

    inline double         GetX() const
    {
        return m_dX;
    }
    inline double         GetY() const
    {
        return m_dY;
    }
    inline unsigned char* GetCodes() const
    {
        return m_pCodes;
    }
    inline unsigned int   GetCodesLen() const
    {
        return m_nLen;
    }
    inline void           SetFont(PdfWriter::CFontDict* pFont)
    {
        m_pFont = pFont;
    }
    inline void           SetSize(const double& dSize)
    {
        m_dSize = dSize;
    }
    inline void           SetColor(const LONG& lColor)
    {
        m_lColor = lColor;
    }
    inline void           SetAlpha(const BYTE& nAlpha)
    {
        m_nAlpha = nAlpha;
    }
    inline void           SetCharSpace(const double& dCharSpace)
    {
        m_dCharSpace = dCharSpace;
    }
    inline void           SetHorScaling(const double& dKoef)
    {
        m_dHorScaling = dKoef;
    }
    inline void           SetMode(const int& nMode)
    {
        m_nMode = nMode;
    }
    inline void           SetNeedDoItalic(const bool& bItalic)
    {
        m_bNeedDoItalic = bItalic;
    }
    inline void           SetNeedDoBold(const bool& bBold)
    {
        m_bNeedDoBold = bBold;
    }
    inline PdfWriter::CFontDict*     GetFont() const
    {
        return m_pFont;
    }
    inline double         GetSize() const
    {
        return m_dSize;
    }
    inline LONG           GetColor() const
    {
        return m_lColor;
    }
    inline BYTE           GetAlpha() const
    {
        return m_nAlpha;
    }
    inline double         GetSpace() const
    {
        return m_dCharSpace;
    }
    inline double         GetHorScaling() const
    {
        return m_dHorScaling;
    }
    inline int            GetMode() const
    {
        return m_nMode;
    }
    inline bool           IsNeedDoItalic() const
    {
        return m_bNeedDoItalic;
    }
    inline bool           IsNeedDoBold() const
    {
        return m_bNeedDoBold;
    }

private:

    unsigned char* m_pCodes;
    unsigned int   m_nLen;
    double         m_dX;
    double         m_dY;

    PdfWriter::CFontDict* m_pFont;
    bool           m_bNeedDoItalic;
    bool           m_bNeedDoBold;
    double         m_dSize;
    LONG           m_lColor;
    BYTE           m_nAlpha;
    double         m_dCharSpace;
    int            m_nMode;
    double         m_dHorScaling;
};
struct TFontInfo
{
    TFontInfo(const std::wstring& fontName, const bool& bold, const bool& italic, const std::wstring& fontPath, const LONG& faceIndex)
    {
        wsFontName = fontName;
        bBold      = bold;
        bItalic    = italic;
        wsFontPath = fontPath;
        lFaceIndex = faceIndex;
    }

    std::wstring wsFontName;
    bool         bBold;
    bool         bItalic;
    std::wstring wsFontPath;
    LONG         lFaceIndex;
};
struct TColor
{
    TColor()
    {
        lColor = 0;
        r = 0;
        g = 0;
        b = 0;
        a = 255;
    }
    TColor(const LONG& _lColor)
    {
        Set(_lColor);
    }

    void Set(const LONG& _lColor)
    {
        lColor = _lColor;

        r = (unsigned char)(lColor & 0xFF);
        g = (unsigned char)((lColor >> 8) & 0xFF);
        b = (unsigned char)((lColor >> 16) & 0xFF);
        a = (unsigned char)((lColor >> 24) & 0xFF);
    }
    void operator=(const LONG& _lColor)
    {
        Set(_lColor);
    }
    void Set(BYTE _r, BYTE _g, BYTE _b, BYTE _a = 255)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;

        lColor = (LONG)(((LONG)r) | ((LONG)g << 8) | ((LONG)b << 16) | ((LONG)a << 24));
    }

    LONG lColor;
    BYTE r;
    BYTE g;
    BYTE b;
    BYTE a;
};
class CPenState
{
public:

    CPenState()
    {
        m_pDashPattern = NULL;
        Reset();
    }
    ~CPenState()
    {
        if (m_pDashPattern)
            delete[] m_pDashPattern;
    }
    inline LONG   GetColor()
    {
        return m_oColor.lColor;
    }
    inline void   SetColor(const LONG& lColor)
    {
        m_oColor.Set(lColor);
    }
    inline TColor GetTColor()
    {
        return m_oColor;
    }
    inline LONG   GetAlpha()
    {
        return m_nAlpha;
    }
    inline void   SetAlpha(const LONG& lAlpha)
    {
        m_nAlpha = (BYTE)(std::max)(0, (std::min)(255, (int)lAlpha));
    }
    inline double GetSize()
    {
        return m_dSize;
    }
    inline void   SetSize(const double& dSize)
    {
        m_dSize = dSize;
    }
    inline BYTE   GetDashStyle()
    {
        return m_nDashStyle;
    }
    inline void   SetDashStyle(const BYTE& nDashStyle)
    {
        m_nDashStyle = nDashStyle;
    }
    inline BYTE   GetStartCapStyle()
    {
        return m_nStartCapStyle;
    }
    inline void   SetStartCapStyle(const BYTE& nCapStyle)
    {
        m_nStartCapStyle = nCapStyle;
    }
    inline BYTE   GetEndCapStyle()
    {
        return m_nEndCapStyle;
    }
    inline void   SetEndCapStyle(const BYTE& nCapStyle)
    {
        m_nEndCapStyle = nCapStyle;
    }
    inline BYTE   GetJoinStyle()
    {
        return m_nJoinStyle;
    }
    inline void   SetJoinStyle(const BYTE& nJoinStyle)
    {
        m_nJoinStyle = nJoinStyle;
    }
    inline double GetDashOffset()
    {
        return m_dDashOffset;
    }
    inline void   SetDashOffset(const double& dOffset)
    {
        m_dDashOffset = dOffset;
    }
    inline LONG   GetAlign()
    {
        return m_lAlign;
    }
    inline void   SetAlign(const LONG& lAlign)
    {
        m_lAlign = lAlign;
    }
    inline double GetMiter()
    {
        return m_dMiter;
    }
    inline void   SetMiter(const double& dMiter)
    {
        m_dMiter = dMiter;
    }
    inline void   SetDashPattern(const double* pPattern, const LONG& lSize)
    {
        if (m_pDashPattern)
        {
            delete[] m_pDashPattern;
            m_pDashPattern = NULL;
        }

        m_lDashPatternSize = 0;

        if (!pPattern || !lSize)
        {
            m_lDashPatternSize = 0;
            m_pDashPattern     = NULL;
        }
        else
        {
            // Избавляемся от нулей, потому что все pdf-ридеры плохо их воспринимают
            std::vector<double> vPattern;
            for (LONG lIndex = 0; lIndex < lSize; lIndex++)
            {
                if (lIndex > 1 && fabs(pPattern[lIndex]) < 0.001)
                {
                    if (0 == lIndex % 2)
                    {
                        if (fabs(pPattern[lIndex + 1]) < 0.001)
                        {
                            lIndex++;
                        }
                        else if (lIndex + 1 < lSize)
                        {
                            size_t nPatternSize = vPattern.size();
                            vPattern.at(nPatternSize - 1) = vPattern.at(nPatternSize - 1) + pPattern[lIndex + 1];
                            lIndex++;
                        }
                    }
                    else
                    {
                        size_t nPatternSize = vPattern.size();
                        vPattern.at(nPatternSize - 2) = vPattern.at(nPatternSize - 2) + vPattern.at(nPatternSize - 1);
                        vPattern.pop_back();
                    }
                }
                else
                {
                    vPattern.push_back(pPattern[lIndex]);
                }
            }

            size_t nPatternSize = vPattern.size();
            if (nPatternSize > 0)
            {
                m_pDashPattern = new double[nPatternSize];
                if (m_pDashPattern)
                {
                    for (int nIndex = 0; nIndex < nPatternSize; nIndex++)
                    {
                        m_pDashPattern[nIndex] = vPattern.at(nIndex);
                    }
                    m_lDashPatternSize = nPatternSize;
                }
            }
        }
    }
    inline double*GetDashPattern(LONG& lSize)
    {
        lSize = m_lDashPatternSize;
        return m_pDashPattern;
    }

    void Reset()
    {
        if (m_pDashPattern)
            delete[] m_pDashPattern;

        m_oColor.Set(0);
        m_dSize  = 0;
        m_nAlpha = 255;
        m_nStartCapStyle = Aggplus::LineCapRound;
        m_nEndCapStyle   = Aggplus::LineCapRound;
        m_nJoinStyle     = Aggplus::LineJoinRound;

        m_lAlign = 0;
        m_dMiter = 0;

        m_nDashStyle       = Aggplus::DashStyleSolid;
        m_lDashPatternSize = 0;
        m_pDashPattern     = NULL;

        m_dDashOffset = 0;
    }

private:

    double m_dSize;
    TColor m_oColor;
    BYTE   m_nAlpha;
    BYTE   m_nStartCapStyle;
    BYTE   m_nEndCapStyle;
    BYTE   m_nJoinStyle;

    LONG   m_lAlign;
    double m_dMiter;

    BYTE   m_nDashStyle;
    double m_dDashOffset;
    double*m_pDashPattern;
    LONG   m_lDashPatternSize;

};
class CBrushState
{
public:
    struct TColorAndPoint
    {
        TColorAndPoint()
        {
            lColor = 0;
            dPoint = 0;
            bUse   = false;
        }
        TColorAndPoint(const LONG& color, const double& point)
        {
            lColor = color;
            dPoint = point;
            bUse   = true;
        }

        static bool Compare(const TColorAndPoint& oFirst, const TColorAndPoint& oSecond)
        {
            return (oFirst.dPoint < oSecond.dPoint);
        }
        static LONG GetLinearApprox(const TColorAndPoint& oPoint1, const TColorAndPoint& oPoint2, const double& dDstPoint)
        {
            double dPoint1 = oPoint1.dPoint;
            double dPoint2 = oPoint2.dPoint;
            LONG lColor1 = oPoint1.lColor;
            LONG lColor2 = oPoint2.lColor;

            double dDiff = dPoint2 - dPoint1;
            if (fabs(dDiff) < 0)
                return lColor1;

            TColor oColor1 = lColor1;
            TColor oColor2 = lColor2;

            BYTE r = (BYTE)(std::max)(0, (std::min)(255, (int)(oColor1.r + (oColor2.r - oColor1.r) / dDiff * (dDstPoint - dPoint1))));
            BYTE g = (BYTE)(std::max)(0, (std::min)(255, (int)(oColor1.g + (oColor2.g - oColor1.g) / dDiff * (dDstPoint - dPoint1))));
            BYTE b = (BYTE)(std::max)(0, (std::min)(255, (int)(oColor1.b + (oColor2.b - oColor1.b) / dDiff * (dDstPoint - dPoint1))));
            BYTE a = (BYTE)(std::max)(0, (std::min)(255, (int)(oColor1.a + (oColor2.a - oColor1.a) / dDiff * (dDstPoint - dPoint1))));

            TColor oResColor;
            oResColor.Set(r, g, b, a);
            return oResColor.lColor;
        }

        LONG   lColor;
        double dPoint;
        bool   bUse;
    };
    struct TBrushRect
    {
        TBrushRect()
        {
            Reset();
        }

        void Reset()
        {
            bUse    = false;
            nVal    = 0;
            dLeft   = 0;
            dTop    = 0;
            dWidth  = 0;
            dHeight = 0;
        }

        bool   bUse;
        int    nVal;
        double dLeft;
        double dTop;
        double dWidth;
        double dHeight;
    };

public:
    CBrushState()
    {
        m_pShadingColors      = NULL;
        m_pShadingPoints      = NULL;
        m_lShadingPointsCount = 0;
        Reset();
    }
    ~CBrushState()
    {
        if (m_pShadingColors)
            delete[] m_pShadingColors;
        if (m_pShadingPoints)
            delete[] m_pShadingPoints;
    }
    void                Reset();
    inline LONG         GetType()
    {
        return m_lType;
    }
    inline void         SetType(const LONG& lType)
    {
        m_lType = lType;
    }
    inline LONG         GetColor1()
    {
        return m_oColor1.lColor;
    }
    inline TColor       GetTColor1()
    {
        return m_oColor1;
    }
    inline void         SetColor1(const LONG& lColor)
    {
        m_oColor1.Set(lColor);
    }
    inline LONG         GetColor2()
    {
        return m_oColor2.lColor;
    }
    inline TColor       GetTColor2()
    {
        return m_oColor2;
    }
    inline void         SetColor2(const LONG& lColor)
    {
        m_oColor2.Set(lColor);
    }
    inline LONG         GetAlpha1()
    {
        return m_nAlpha1;
    }
    inline void         SetAlpha1(const LONG& lAlpha)
    {
        m_nAlpha1 = (BYTE)(std::max)(0, (std::min)(255, (int)lAlpha));
    }
    inline LONG         GetAlpha2()
    {
        return m_nAlpha2;
    }
    inline void         SetAlpha2(const LONG& lAlpha)
    {
        m_nAlpha2 = (BYTE)(std::max)(0, (std::min)(255, (int)lAlpha));
    }
    inline std::wstring GetTexturePath()
    {
        return m_wsTexturePath;
    }
    inline void         SetTexturePath(const std::wstring& wsPath)
    {
        m_wsTexturePath = wsPath;
    }
    inline LONG         GetTextureMode()
    {
        return m_lTextureMode;
    }
    inline void         SetTextureMode(const LONG& lMode)
    {
        m_lTextureMode = lMode;
    }
    inline BYTE         GetTextureAlpha()
    {
        return m_nTextureAlpha;
    }
    inline void         SetTextureAlpha(const LONG& lAlpha)
    {
        m_nTextureAlpha = (BYTE)(std::max)(0, (std::min)(255, (int)lAlpha));
    }
    inline double       GetLinearAngle()
    {
        return m_dLinearAngle;
    }
    inline void         SetLinearAngle(const double& dAngle)
    {
        m_dLinearAngle = dAngle;
    }
    inline void         SetGradientColors(LONG* pColors, double* pPoints, const LONG& lCount)
    {
        // Мы создаем упорядоченный по возрастанию массив, причем первая и последняя точки должны быть 0 и 1 соответственно.
        if (m_pShadingColors)
        {
            delete[] m_pShadingColors;
            m_pShadingColors = NULL;
        }

        if (m_pShadingPoints)
        {
            delete[] m_pShadingPoints;
            m_pShadingPoints = NULL;
        }

        if (!pColors || !pPoints || !lCount)
            return;

        // Проверим вырожденный случай, когда задана либо 1 точка, либо несколько точек с одинковым значением
        bool bIrregular = false;
        if (1 == lCount)
        {
            bIrregular = true;
        }
        else
        {
            bIrregular = true;
            for (LONG lIndex = 0; lIndex < lCount; lIndex++)
            {
                double dPoint1 = pPoints[lIndex];
                for (LONG lIndex2 = lIndex + 1; lIndex2 < lCount; lIndex2++)
                {
                    double dPoint2 = pPoints[lIndex2];
                    if (fabs(dPoint2 - dPoint1) > 0.00001)
                    {
                        bIrregular = false;
                        break;
                    }
                }

                if (!bIrregular)
                    break;
            }
        }

        if (bIrregular)
        {
            if (1 == lCount)
            {
                m_pShadingPoints = new double[2];
                m_pShadingColors = new TColor[2];

                if (!m_pShadingColors || !m_pShadingColors)
                    return;

                m_pShadingPoints[0] = 0.0;
                m_pShadingColors[0] = pColors[0];
                m_pShadingPoints[1] = 1.0;
                m_pShadingColors[1] = pColors[0];
                m_lShadingPointsCount = 2;
            }
            else
            {
                if (pPoints[0] < 0)
                {
                    m_pShadingPoints = new double[2];
                    m_pShadingColors = new TColor[2];

                    if (!m_pShadingColors || !m_pShadingColors)
                        return;

                    m_pShadingPoints[0] = 0.0;
                    m_pShadingColors[0] = pColors[lCount - 1];
                    m_pShadingPoints[1] = 1.0;
                    m_pShadingColors[1] = pColors[lCount - 1];
                    m_lShadingPointsCount = 2;
                }
                else if (pPoints[0] > 1)
                {
                    m_pShadingPoints = new double[2];
                    m_pShadingColors = new TColor[2];

                    if (!m_pShadingColors || !m_pShadingColors)
                        return;

                    m_pShadingPoints[0] = 0.0;
                    m_pShadingColors[0] = pColors[0];
                    m_pShadingPoints[1] = 1.0;
                    m_pShadingColors[1] = pColors[0];
                    m_lShadingPointsCount = 2;
                }
                else
                {
                    m_pShadingPoints = new double[4];
                    m_pShadingColors = new TColor[4];

                    if (!m_pShadingColors || !m_pShadingColors)
                        return;

                    m_pShadingPoints[0] = 0.0;
                    m_pShadingColors[0] = pColors[0];
                    m_pShadingPoints[1] = pPoints[0];
                    m_pShadingColors[1] = pColors[0];
                    m_pShadingPoints[2] = pPoints[lCount - 1];
                    m_pShadingColors[2] = pColors[lCount - 1];
                    m_pShadingPoints[3] = 1.0;
                    m_pShadingColors[3] = pColors[lCount - 1];
                    m_lShadingPointsCount = 4;
                }
            }
        }
        else
        {
            std::vector<TColorAndPoint> vPoints;
            for (LONG lIndex = 0; lIndex < lCount; lIndex++)
            {
                vPoints.push_back(TColorAndPoint(pColors[lIndex], pPoints[lIndex]));
            }
            std::sort(vPoints.begin(), vPoints.end(), TColorAndPoint::Compare);

            LONG lMinIn = -1, lMaxIn = -1, lMinOut = -1, lMaxOut = -1;
            for (LONG lIndex = 0; lIndex < lCount; lIndex++)
            {
                double dPoint = vPoints.at(lIndex).dPoint;
                if (0 <= dPoint && dPoint <= 1)
                {
                    if (-1 == lMinIn || dPoint < vPoints.at(lMinIn).dPoint)
                        lMinIn = lIndex;

                    if (-1 == lMaxIn || dPoint > vPoints.at(lMaxIn).dPoint)
                        lMaxIn = lIndex;
                }
                else if (dPoint < 0)
                {
                    if (-1 == lMinOut || dPoint > vPoints.at(lMinOut).dPoint)
                        lMinOut = lIndex;
                }
                else// if (dPoint > 1)
                {
                    if (-1 == lMaxOut || dPoint < vPoints.at(lMaxOut).dPoint)
                        lMaxOut = lIndex;
                }
            }

            LONG lBeginIndex = lMinIn;
            LONG lEndIndex   = lMaxIn;

            bool bNeed0 = true, bNeed1 = true;
            if (-1 != lMinIn && vPoints.at(lMinIn).dPoint < 0.001)
            {
                bNeed0 = false;
                lBeginIndex = lMinIn;
                vPoints.at(lMinIn).dPoint = 0;
            }
            else if (-1 != lMinOut && vPoints.at(lMinOut).dPoint > -0.001)
            {
                bNeed0 = false;
                lBeginIndex = lMinOut;
                vPoints.at(lMinOut).dPoint = 0;
            }

            if (-1 != lMaxIn && vPoints.at(lMaxIn).dPoint > 0.999)
            {
                bNeed1 = false;
                lEndIndex = lMaxIn;
                vPoints.at(lEndIndex).dPoint = 1;
            }
            else if (-1 != lMaxOut && vPoints.at(lMaxOut).dPoint < 1.001)
            {
                bNeed1 = false;
                lEndIndex = lMaxOut;
                vPoints.at(lEndIndex).dPoint = 1;
            }

            std::vector<TColorAndPoint> vResPoints;
            if (bNeed0)
            {
                LONG lIndex0, lIndex1;
                if (-1 != lMinOut)
                {
                    if (-1 != lMinIn)
                    {
                        lIndex0 = lMinOut;
                        lIndex1 = lMinIn;
                    }
                    else if (-1 != lMaxOut)
                    {
                        lIndex0 = lMinOut;
                        lIndex1 = lMaxOut;
                    }
                    else
                    {
                        lIndex0 = lMinIn - 1;
                        lIndex1 = lMinIn;
                    }
                }
                else
                {
                    if (-1 != lMinIn)
                    {
                        lIndex0 = lMinIn;
                        lIndex1 = lMinIn + 1;
                    }
                    else
                    {
                        lIndex0 = lMaxOut;
                        lIndex1 = lMaxOut + 1;
                    }
                }

                LONG lColor0 = TColorAndPoint::GetLinearApprox(vPoints.at(lIndex0), vPoints.at(lIndex1), 0);
                vResPoints.push_back(TColorAndPoint(lColor0, 0));
            }

            if (-1 != lBeginIndex && -1 != lEndIndex)
            {
                for (LONG lIndex = lBeginIndex; lIndex <= lEndIndex; lIndex++)
                {
                    vResPoints.push_back(vPoints.at(lIndex));
                }
            }

            if (bNeed1)
            {
                LONG lIndex0, lIndex1;
                if (-1 != lMaxOut)
                {
                    if (-1 != lMaxIn)
                    {
                        lIndex0 = lMaxIn;
                        lIndex1 = lMaxOut;
                    }
                    else if (-1 != lMinOut)
                    {
                        lIndex0 = lMinOut;
                        lIndex1 = lMaxOut;
                    }
                    else
                    {
                        lIndex0 = lMaxOut;
                        lIndex1 = lMaxOut + 1;
                    }
                }
                else
                {
                    if (-1 != lMaxIn)
                    {
                        lIndex0 = lMaxIn - 1;
                        lIndex1 = lMaxIn;
                    }
                    else
                    {
                        lIndex0 = lMinOut - 1;
                        lIndex1 = lMinOut;
                    }
                }

                LONG lColor1 = TColorAndPoint::GetLinearApprox(vPoints.at(lIndex0), vPoints.at(lIndex1), 1);
                vResPoints.push_back(TColorAndPoint(lColor1, 1));
            }

            size_t lResCount = vResPoints.size();
            if (lResCount == 0)
                return;

            m_pShadingColors      = new TColor[lResCount];
            m_pShadingPoints      = new double[lResCount];
            m_lShadingPointsCount = lResCount;

            if (!m_pShadingColors || !m_pShadingPoints)
                return;

            for (LONG lIndex = 0; lIndex < lResCount; lIndex++)
            {
                m_pShadingColors[lIndex] = vResPoints.at(lIndex).lColor;
                m_pShadingPoints[lIndex] = vResPoints.at(lIndex).dPoint;
            }
        }
    }
    inline void         SetBrushRect(const int& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
    {
        m_oRect.nVal    = nVal;
        m_oRect.dLeft   = dLeft;
        m_oRect.dTop    = dTop;
        m_oRect.dWidth  = dWidth;
        m_oRect.dHeight = dHeight;
    }
    inline void         EnableBrushRect(bool bEnable)
    {
        m_oRect.bUse = bEnable;
    }
    TBrushRect& GetBrushRect()
    {
        return m_oRect;
    }

    inline void         SetLinearGradientPattern(const double& dX0, const double& dY0, const double& dX1, const double& dY1)
    {
        m_pShadingPattern[0] = dX0;
        m_pShadingPattern[1] = dY0;
        m_pShadingPattern[2] = dX1;
        m_pShadingPattern[3] = dY1;
    }
    inline void         SetRadialGradientPattern(const double& dX0, const double& dY0, const double& dR0, const double& dX1, const double& dY1, const double& dR1)
    {
        m_pShadingPattern[0] = dX0;
        m_pShadingPattern[1] = dY0;
        m_pShadingPattern[2] = dR0;
        m_pShadingPattern[3] = dX1;
        m_pShadingPattern[4] = dY1;
        m_pShadingPattern[5] = dR1;
    }
    inline void         GetLinearGradientPattern(double& dX0, double& dY0, double& dX1, double& dY1)
    {
        dX0 = m_pShadingPattern[0];
        dY0 = m_pShadingPattern[1];
        dX1 = m_pShadingPattern[2];
        dY1 = m_pShadingPattern[3];
    }
    inline void         GetRadialGradientPattern(double& dX0, double& dY0, double& dR0, double& dX1, double& dY1, double& dR1)
    {
        dX0 = m_pShadingPattern[0];
        dY0 = m_pShadingPattern[1];
        dR0 = m_pShadingPattern[2];
        dX1 = m_pShadingPattern[3];
        dY1 = m_pShadingPattern[4];
        dR1 = m_pShadingPattern[5];
    }
    inline void         GetGradientColors(TColor*& pColors, double*& pPoints, LONG& lCount)
    {
        pColors = m_pShadingColors;
        pPoints = m_pShadingPoints;
        lCount  = m_lShadingPointsCount;
    }

private:

    LONG         m_lType;
    TColor       m_oColor1;
    TColor       m_oColor2;
    BYTE         m_nAlpha1;
    BYTE         m_nAlpha2;
    std::wstring m_wsTexturePath;
    LONG         m_lTextureMode;
    BYTE         m_nTextureAlpha;
    double       m_dLinearAngle;
    TBrushRect   m_oRect;

    TColor*      m_pShadingColors;
    double*      m_pShadingPoints;
    LONG         m_lShadingPointsCount;
    double       m_pShadingPattern[6]; // У линейного градиента x0, y0, x1, y1 (2 не используются), у радиального x0, y0, r0, x1, y1, r1
};
class CFontState
{
public:

    CFontState() : m_wsName(L"Arial"), m_wsPath(L""), m_dSize(10), m_bGid(false), m_lFaceIndex(0), m_lStyle(0),
        m_bBold(false), m_bItalic(false), m_dCharSpace(0), m_bNeedDoItalic(false), m_bNeedDoBold(false)
    {
    }

    void Reset()
    {
        m_wsName     = L"Arial";
        m_wsPath     = L"";
        m_dSize      = 10;
        m_bGid       = false;
        m_lFaceIndex = 0;
        m_lStyle     = 0;
        m_bBold      = false;
        m_bItalic    = false;
        m_dCharSpace = 0;

        m_bNeedDoItalic = false;
        m_bNeedDoBold   = false;
    }

    inline std::wstring GetName()
    {
        return m_wsName;
    }
    inline void         SetName(const std::wstring& wsName)
    {
        m_wsName = wsName;
    }
    inline std::wstring GetPath()
    {
        return m_wsPath;
    }
    inline void         SetPath(const std::wstring& wsPath)
    {
        m_wsPath = wsPath;
    }
    inline double       GetSize()
    {
        return m_dSize;
    }
    inline void         SetSize(const double& dSize)
    {
        m_dSize = dSize;
    }
    inline LONG         GetFaceIndex()
    {
        return m_lFaceIndex;
    }
    inline void         SetFaceIndex(const LONG& lFaceIndex)
    {
        m_lFaceIndex = lFaceIndex;
    }
    inline LONG         GetStyle()
    {
        return m_lStyle;
    }
    inline void         SetStyle(const LONG& lStyle)
    {
        m_lStyle  = lStyle;
        m_bBold   = (lStyle & 1 ? true : false);
        m_bItalic = (lStyle & 2 ? true : false);
    }
    inline bool         GetGid()
    {
        return m_bGid;
    }
    inline void         SetGid(const bool& bGid)
    {
        m_bGid = bGid;
    }
    inline double       GetCharSpace()
    {
        return m_dCharSpace;
    }
    inline void         SetCharSpace(const double& dCharSpace)
    {
        m_dCharSpace = dCharSpace;
    }
    inline bool         IsBold()
    {
        return m_bBold;
    }
    inline bool         IsItalic()
    {
        return m_bItalic;
    }
    inline void         SetNeedDoItalic(const bool& bNeedDoItalic)
    {
        m_bNeedDoItalic = bNeedDoItalic;
    }
    inline void         SetNeedDoBold(const bool& bNeedDoBold)
    {
        m_bNeedDoBold = bNeedDoBold;
    }
    inline bool         IsNeedDoItalic()
    {
        return m_bNeedDoItalic;
    }
    inline bool         IsNeedDoBold()
    {
        return m_bNeedDoBold;
    }

private:

    std::wstring m_wsName;
    std::wstring m_wsPath;
    double       m_dSize;
    bool         m_bGid;
    LONG         m_lFaceIndex;
    LONG         m_lStyle;
    bool         m_bBold;
    bool         m_bItalic;
    double       m_dCharSpace;
    bool         m_bNeedDoItalic;
    bool         m_bNeedDoBold;
};
class CPath
{
private:

    enum EPathCommandType
    {
        rendererpathcommand_Unknown    = 0x00,
        rendererpathcommand_MoveTo     = 0x01,
        rendererpathcommand_LineTo     = 0x02,
        rendererpathcommand_CurveTo    = 0x03,
        rendererpathcommand_ArcTo      = 0x04,
        rendererpathcommand_Close      = 0x05,
        rendererpathcommand_TextChar   = 0x06,
        rendererpathcommand_Text       = 0x07,
        rendererpathcommand_TextExChar = 0x08,
        rendererpathcommand_TextEx     = 0x09
    };
    class CPathCommandBase
    {
    public:
        CPathCommandBase()
        {
        }
        virtual ~CPathCommandBase()
        {
        }
        virtual void Draw(PdfWriter::CPage* pPage) = 0;
        virtual void UpdateBounds(double& dL, double& dT, double& dR, double& dB) = 0;
        virtual void GetLastPoint(double& dX, double& dY) = 0;
        virtual EPathCommandType GetType() = 0;
    };
    class CPathMoveTo : public CPathCommandBase
    {
    public:
        CPathMoveTo(const double& dX, const double& dY)
        {
            x = dX;
            y = dY;
        }
        void GetLastPoint(double& dX, double& dY)
        {
            dX = x;
            dY = y;
        }
        void Draw(PdfWriter::CPage* pPage);
        void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
        EPathCommandType GetType()
        {
            return rendererpathcommand_MoveTo;
        }

    public:

        double x;
        double y;
    };
    class CPathLineTo : public CPathCommandBase
    {
    public:
        CPathLineTo(const double& dX, const double& dY)
        {
            x = dX;
            y = dY;
        }
        void GetLastPoint(double& dX, double& dY)
        {
            dX = x;
            dY = y;
        }
        void Draw(PdfWriter::CPage* pPage);
        void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
        EPathCommandType GetType()
        {
            return rendererpathcommand_LineTo;
        }

    public:

        double x;
        double y;
    };
    class CPathCurveTo : public CPathCommandBase
    {
    public:
        CPathCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
        {
            x1 = dX1;
            y1 = dY1;
            x2 = dX2;
            y2 = dY2;
            xe = dXe;
            ye = dYe;
        }
        void GetLastPoint(double& dX, double& dY)
        {
            dX = xe;
            dY = ye;
        }
        void Draw(PdfWriter::CPage* pPage);
        void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
        EPathCommandType GetType()
        {
            return rendererpathcommand_CurveTo;
        }

    public:

        double x1;
        double y1;
        double x2;
        double y2;
        double xe;
        double ye;
    };
    class CPathArcTo : public CPathCommandBase
    {
    public:
        CPathArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
        {
            x = dX;
            y = dY;
            w = dW;
            h = dH;
            startAngle = dStartAngle;
            sweepAngle = dSweepAngle;
        }
        void GetLastPoint(double& dX, double& dY)
        {
            // TODO: Надо грамотно пересчитать
            dX = x;
            dY = y;
        }
        void Draw(PdfWriter::CPage* pPage);
        void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
        EPathCommandType GetType()
        {
            return rendererpathcommand_ArcTo;
        }

    public:

        double x;
        double y;
        double w;
        double h;
        double startAngle;
        double sweepAngle;
    };
    class CPathClose : public CPathCommandBase
    {
    public:
        CPathClose()
        {
        }
        void GetLastPoint(double& dX, double& dY)
        {
            // TODO: Надо грамотно пересчитать
            dX = 0;
            dY = 0;
        }
        void Draw(PdfWriter::CPage* pPage);
        void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
        EPathCommandType GetType()
        {
            return rendererpathcommand_Close;
        }
    };
    class CPathText : public CPathCommandBase
    {
    public:
        CPathText(PdfWriter::CFontDict* pFont, unsigned char* pCodes, const unsigned int& unCodesCount, const double& dX, const double& dY, const double& dSize, const double& dCharSpace)
        {
            font       = pFont;
            codes      = pCodes;
            codesCount = unCodesCount;
            x          = dX;
            y          = dY;
            fontSize   = dSize;
            charSpace  = dCharSpace;
        }
        ~CPathText()
        {
            RELEASEARRAYOBJECTS(codes);
        }
        void GetLastPoint(double& dX, double& dY)
        {
            dX = x;
            dY = y;
        }
        void Draw(PdfWriter::CPage* pPage);
        void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
        EPathCommandType GetType()
        {
            return rendererpathcommand_Text;
        }

    public:

        PdfWriter::CFontDict* font;
        unsigned char*        codes;
        unsigned int          codesCount;
        double                x;
        double                y;
        double                fontSize;
        double                charSpace;
    };

public:

    CPath()
    {
        m_bIsMoveTo = false;
    }
    ~CPath()
    {
        Clear();
    }

    bool MoveTo(const double& dX, const double& dY)
    {
        m_bIsMoveTo = true;
        return Add(new CPathMoveTo(dX, dY));
    }
    bool LineTo(const double& dX, const double& dY)
    {
        if (!m_bIsMoveTo)
            MoveTo(dX, dY);

        return Add(new CPathLineTo(dX, dY));
    }
    bool CurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
    {
        if (!m_bIsMoveTo)
            MoveTo(dX1, dY1);

        return Add(new CPathCurveTo(dX1, dY1, dX2, dY2, dXE, dYE));
    }
    bool ArcTo(double dX, double dY, double dW, double dH, double dStart, double dSweep)
    {
        if (!m_bIsMoveTo)
            MoveTo(dX, dY);

        return Add(new CPathArcTo(dX, dY, dW, dH, dStart, dSweep));
    }
    bool AddText(PdfWriter::CFontDict* pFont, unsigned char* pCodes, const unsigned int& unLen, const double& dX, const double& dY, const double& dSize, const double& dCharSpace)
    {
        return Add(new CPathText(pFont, pCodes, unLen, dX, dY, dSize, dCharSpace));
    }
    bool Close()
    {
        return Add(new CPathClose());
    }
    void Clear()
    {
        for (size_t nIndex = 0, nCount = m_vCommands.size(); nIndex < nCount; nIndex++)
        {
            CPathCommandBase* pCommand = m_vCommands.at(nIndex);
            delete pCommand;
        }
        m_vCommands.clear();
        m_bIsMoveTo = false;
    }
    bool IsMoveTo()
    {
        return m_bIsMoveTo;
    }
    void GetLastPoint(double& dX, double& dY);
    void Draw(PdfWriter::CPage* pPage, bool bStroke, bool bFill, bool bEoFill);
    void Clip(PdfWriter::CPage* pPage, bool bEvenOdd = false);
    void GetBounds(double& dL, double& dT, double& dR, double& dB);

private:

    bool Add(CPathCommandBase* pCommand)
    {
        if (pCommand)
        {
            m_vCommands.push_back(pCommand);
            return true;
        }

        return false;
    }

public:

    std::vector<CPathCommandBase*> m_vCommands;
    bool                           m_bIsMoveTo;
};
struct CTransform
{
    CTransform()
    {
        Reset();
    }
    void operator=(const CTransform& oT)
    {
        m11 = oT.m11;
        m12 = oT.m12;
        m21 = oT.m21;
        m22 = oT.m22;
        dx  = oT.dx;
        dy  = oT.dy;
    }
    void Reset()
    {
        m11 = 1.0;
        m12 = 0.0;
        m21 = 0.0;
        m22 = 1.0;
        dx  = 0;
        dy  = 0;
    }
    bool IsIdentity() const
    {
        if (fabs(m11 - 1) < 0.001
            && fabs(m12) < 0.001
            && fabs(m21) < 0.001
            && fabs(m22 - 1) < 0.001
            && fabs(dx) < 0.001
            && fabs(dy) < 0.001)
            return true;

        return false;
    }
    void Set(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
    {
        m11 = dM11;
        m12 = dM12;
        m21 = dM21;
        m22 = dM22;
        dx  = dX;
        dy  = dY;
    }

    double m11;
    double m12;
    double m21;
    double m22;
    double dx;
    double dy;
};
class CCommandManager
{
public:
    CCommandManager(CPdfWriter* pRenderer);
    ~CCommandManager();
    CRendererTextCommand* AddText(unsigned char* pCodes, unsigned int nLen, const double& dX, const double& dY);
    void Flush();
    void SetTransform(const CTransform& oTransform) { m_oTransform = oTransform; }
    void SetTransform(const double& m11, const double& m12, const double& m21, const double& m22, const double& dx, const double& dy) { m_oTransform.Set(m11, m12, m21, m22, dx, dy); }
private:
    void Add(CRendererCommandBase* pCommand);
    void Clear();
private:
    CPdfWriter*                        m_pRenderer;
    std::vector<CRendererCommandBase*> m_vCommands;
    CTransform                         m_oTransform;
};
struct TDestinationInfo
{
    TDestinationInfo(PdfWriter::CPage* page, const double& x, const double& y, const double& w, const double& h, const double& dx, const double& dy, const unsigned int& undpage)
    {
        pPage      = page;
        dX         = x;
        dY         = y;
        dW         = w;
        dH         = h;
        dDestX     = dx;
        dDestY     = dy;
        unDestPage = undpage;
    }

    PdfWriter::CPage* pPage;
    double       dX;
    double       dY;
    double       dW;
    double       dH;
    double       dDestX;
    double       dDestY;
    unsigned int unDestPage;
};
class CMultiLineTextManager
{
public:
    CMultiLineTextManager()
    {
        m_pCodes       = NULL;
        m_pWidths      = NULL;
        m_unLen        = 0;
        m_ushSpaceCode = 0;
        m_unLineHeight = 0;
        m_nAscent      = 0;
        m_nDescent     = 0;
    }
    void Init(unsigned short* pCodes, unsigned int* pWidths, const unsigned int& unLen, const unsigned short& ushSpaceCode, const unsigned int& unLineHeight, const int& nAscent)
    {
        m_pCodes       = pCodes;
        m_pWidths      = pWidths;
        m_unLen        = unLen;
        m_ushSpaceCode = ushSpaceCode;
        m_unLineHeight = unLineHeight;
        m_nAscent      = nAscent;
        m_nDescent     = unLineHeight - nAscent;
    }
    void Clear()
    {
        m_pCodes       = NULL;
        m_pWidths      = NULL;
        m_unLen        = 0;
        m_ushSpaceCode = 0;
        m_unLineHeight = 0;
        m_nAscent      = 0;
        m_nDescent     = 0;
    }
    void CalculateLines(const double& dFontSize, const double& dW)
    {
        m_vBreaks.clear();

        bool bLineStart = true, bWord = false, bFirstItemOnLine = true;

        unsigned int unPos = 0, unWordStartPos = 0;
        double dWordWidth = 0;
        double dX = 0, dKoef = dFontSize / 1000.0;

        while (unPos < m_unLen)
        {
            if (IsSpace(unPos))
            {
                dX += dWordWidth + m_pWidths[unPos] * dKoef;
                bWord             = false;
                dWordWidth        = 0;
                bLineStart        = false;
                bFirstItemOnLine  = false;
            }
            else
            {
                double dLetterWidth = m_pWidths[unPos] * dKoef;
                if (dX + dWordWidth + dLetterWidth > dW)
                {
                    if (bLineStart)
                    {
                        if (bFirstItemOnLine)
                        {
                            if (unPos != m_unLen - 1)
                                m_vBreaks.push_back(unPos + 1);

                            unPos++;
                        }
                        else
                        {
                            m_vBreaks.push_back(unPos);
                        }
                    }
                    else
                    {
                        if (bWord)
                        {
                            m_vBreaks.push_back(unWordStartPos);
                            unPos = unWordStartPos;
                        }
                        else
                        {
                            m_vBreaks.push_back(unPos);
                        }
                    }

                    dX               = 0;
                    bWord            = false;
                    dWordWidth       = 0;
                    bLineStart       = true;
                    bFirstItemOnLine = true;
                    continue;
                }

                if (bWord)
                {
                    dWordWidth += m_pWidths[unPos] * dKoef;
                }
                else
                {
                    unWordStartPos = unPos;
                    bWord          = true;
                    dWordWidth     = m_pWidths[unPos] * dKoef;
                }

                bFirstItemOnLine  = false;
            }

            unPos++;
        }
    }
    double ProcessAutoFit(const double& dW, const double& dH)
    {
        double dGoodFontSize = 0;

        // Параметры подобраны для совместимости с AdobeReader
        double dFontSize     = 4;
        double dFontSizeStep = 0.797 / 3.0;

        while (true)
        {
            CalculateLines(dFontSize,  dW);
            if (CheckHeight(dH, dFontSize))
            {
                dGoodFontSize = dFontSize;
                dFontSize += dFontSizeStep;

                if (dFontSize > 12)
                {
                    dFontSize = 12;
                    break;
                }
            }
            else
            {
                if (dGoodFontSize > 0.001)
                {
                    dFontSize = dGoodFontSize;
                    break;
                }

                dFontSize -= dFontSizeStep;
                if (dFontSize < 4)
                {
                    dFontSize = 4;
                    break;
                }
            }
        }

        return (floor(dFontSize * 1000.0 + 0.5) / 1000.0);
    }
    unsigned int GetLinesCount() const
    {
        return m_vBreaks.size() + 1;
    }
    unsigned int GetLineStartPos(const int& nLineIndex) const
    {
        if (!nLineIndex || nLineIndex > m_vBreaks.size())
            return 0;

        return m_vBreaks[nLineIndex - 1];
    }
    unsigned int GetLineEndPos(const int& nLineIndex) const
    {
        if (nLineIndex >= m_vBreaks.size())
            return m_unLen;

        return m_vBreaks[nLineIndex];
    }
    double GetLineWidth(const int& nLineIndex, const double& dFontSize = 10.0)
    {
        if (nLineIndex < 0 || nLineIndex > m_vBreaks.size())
            return 0;

        unsigned int unStart = GetLineStartPos(nLineIndex);
        unsigned int unEnd   = GetLineEndPos(nLineIndex);

        double dWidth = 0;
        double dKoef  = dFontSize / 1000.0;

        while (unStart < unEnd)
        {
            if (IsSpace(unStart))
                unStart++;
            else
                break;
        }

        while (unEnd > unStart && unEnd > 0)
        {
            if (IsSpace(unEnd - 1))
                unEnd--;
            else
                break;
        }

        for (unsigned int unPos = unStart; unPos < unEnd; ++unPos)
        {
            dWidth += m_pWidths[unPos] * dKoef;
        }

        return dWidth;
    }

private:
    inline bool IsSpace(const unsigned int& unPos) const
    {
        return (m_pCodes[unPos] == m_ushSpaceCode);
    }
    inline bool CheckHeight(const double& dH, const double& dFontSize) const
    {
        double dKoef = dFontSize / 1000.0;
        return (GetLinesCount() * (m_unLineHeight * dKoef) < (dH - (m_nDescent * dKoef)));
    }

private:
    unsigned short* m_pCodes;
    unsigned int*   m_pWidths;
    unsigned int    m_unLen;
    unsigned short  m_ushSpaceCode;
    unsigned int    m_unLineHeight;
    int             m_nAscent;
    int             m_nDescent;

    std::vector<unsigned int> m_vBreaks;
};
#endif // _PDF_WRITER_SRC_STATE_H
