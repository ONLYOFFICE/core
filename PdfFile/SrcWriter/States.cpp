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
#include "States.h"
#include "Font.h"
#include "Types.h"
#include "Pages.h"
#include "FontCidTT.h"
#include "../PdfWriter.h"

//----------------------------------------------------------------------------------------
//
// CCommandManager
//
//----------------------------------------------------------------------------------------
CCommandManager::CCommandManager(CPdfWriter* pRenderer) : m_pRenderer(pRenderer)
{
}
CCommandManager::~CCommandManager()
{
    Clear();
}
CRendererTextCommand* CCommandManager::AddText(unsigned char* pCodes, unsigned int nLen, const double& dX, const double& dY)
{
    CRendererCommandBase* pCommand = new CRendererTextCommand(pCodes, nLen, dX, dY);
    Add(pCommand);
    return (CRendererTextCommand*)pCommand;
}
void CCommandManager::Add(CRendererCommandBase* pCommand)
{
    if (pCommand)
    {
        if (m_vCommands.size() > 0 && pCommand->GetType() != m_vCommands.at(0)->GetType())
            Flush();
        m_vCommands.push_back(pCommand);
    }
}
void CCommandManager::Flush()
{
    size_t nCommandsCount = m_vCommands.size();
    if (nCommandsCount > 0)
    {
        PdfWriter::CPage* pPage = m_pRenderer->m_pPage;
        pPage->GrSave();

        pPage->SetTransform(m_oTransform.m11, m_oTransform.m12, m_oTransform.m21, m_oTransform.m22, m_oTransform.dx, m_oTransform.dy);

        ERendererCommandType eType = m_vCommands.at(0)->GetType();
        if (renderercommandtype_Text == eType)
        {
            pPage->BeginText();
            CRendererTextCommand* pText = NULL;

            PdfWriter::CFontDict* pTextFont = NULL;
            double         dTextSize = -1;
            LONG          lTextColor = 0;
            BYTE          nTextAlpha = 255;
            double        dTextSpace = 0;
            double       dHorScaling = 100;
            PdfWriter::ETextRenderingMode eMode = PdfWriter::textrenderingmode_Fill;
            bool        isNeedDoBold = false;
            bool      isNeedDoItalic = false;
            double        dLineWidth = -1;

            PdfWriter::CTextLine oTextLine;
            for (size_t nIndex = 0; nIndex < nCommandsCount; nIndex++)
            {
                pText = (CRendererTextCommand*)m_vCommands.at(nIndex);
                if (!pText)
                    continue;

                if (pTextFont != pText->GetFont() || fabs(dTextSize - pText->GetSize()) > 0.001)
                {
                    oTextLine.Flush(pPage);
                    pTextFont = pText->GetFont();
                    dTextSize = pText->GetSize();
                    pPage->SetFontAndSize(pTextFont, dTextSize);
                }

                if (lTextColor != pText->GetColor())
                {
                    oTextLine.Flush(pPage);
                    lTextColor = pText->GetColor();
                    TColor oColor = lTextColor;
                    pPage->SetFillColor(oColor.r, oColor.g, oColor.b);
                    pPage->SetStrokeColor(oColor.r, oColor.g, oColor.b);
                }

                if (nTextAlpha != pText->GetAlpha())
                {
                    oTextLine.Flush(pPage);
                    nTextAlpha = pText->GetAlpha();
                    pPage->SetFillAlpha(nTextAlpha);
                    pPage->SetStrokeAlpha(nTextAlpha);
                }

                if (fabs(dTextSpace - pText->GetSpace()) > 0.001)
                {
                    oTextLine.Flush(pPage);
                    dTextSpace = pText->GetSpace();
                    pPage->SetCharSpace(dTextSpace);
                }

                if ((int)eMode != pText->GetMode() || isNeedDoBold != pText->IsNeedDoBold())
                {
                    oTextLine.Flush(pPage);
                    eMode = (PdfWriter::ETextRenderingMode)pText->GetMode();
                    isNeedDoBold = pText->IsNeedDoBold();

                    if (isNeedDoBold && eMode == PdfWriter::textrenderingmode_Fill)
                    {
                        double dNewLineWidth = dTextSize / 12 * 0.343;
                        if (fabs(dLineWidth - dNewLineWidth) > 0.001)
                        {
                            dLineWidth = dNewLineWidth;
                            pPage->SetLineWidth(dLineWidth);
                        }

                        pPage->SetTextRenderingMode(PdfWriter::textrenderingmode_FillThenStroke);
                    }
                    else
                    {
                        pPage->SetTextRenderingMode(eMode);
                    }
                }

                if (fabs(dHorScaling - pText->GetHorScaling()) > 0.001)
                {
                    oTextLine.Flush(pPage);
                    dHorScaling = pText->GetHorScaling();
                    pPage->SetHorizontalScalling(dHorScaling);
                }

                if (isNeedDoItalic != pText->IsNeedDoItalic())
                {
                    oTextLine.Flush(pPage);

                    if (pText->IsNeedDoItalic())
                        pPage->SetTextMatrix(1, 0, 0.26, 1, 0, 0);
                    else
                        pPage->SetTextMatrix(1, 0, 0, 1, 0, 0);

                    isNeedDoItalic = pText->IsNeedDoItalic();
                }

                unsigned char* pCodes    = pText->GetCodes();
                unsigned short ushCode   = (pCodes[0] << 8) + pCodes[1];
                unsigned int   unLen     = pText->GetCodesLen();
                double         dX        = pText->GetX();
                double         dY        = pText->GetY();
                double         dTextSize = pText->GetSize();
                double         dWidth    = ((PdfWriter::CFontCidTrueType*)pText->GetFont())->GetWidth(ushCode) / 1000.0 * dTextSize;

                if (!oTextLine.Add(pCodes, unLen, dX, dY, dWidth, dTextSize))
                {
                    oTextLine.Flush(pPage);
                    if (!oTextLine.Add(pCodes, unLen, dX, dY, dWidth, dTextSize))
                    {
                        pPage->DrawText(dX, dY, pCodes, unLen);
                    }
                }
            }

            oTextLine.Flush(pPage);
            pPage->EndText();
        }

        pPage->GrRestore();
    }

    Clear();
}
void CCommandManager::Clear()
{
    for (size_t nIndex = 0, nCount = m_vCommands.size(); nIndex < nCount; nIndex++)
    {
        CRendererCommandBase* pCommand = m_vCommands.at(nIndex);
        delete pCommand;
    }
    m_vCommands.clear();
}

//----------------------------------------------------------------------------------------
// Внутренние функции
//----------------------------------------------------------------------------------------
static inline void UpdateMaxMinPoints(double& dMinX, double& dMinY, double& dMaxX, double& dMaxY, const double& dX, const double& dY)
{
    if (dX < dMinX)
        dMinX = dX;

    if (dX > dMaxX)
        dMaxX = dX;

    if (dY < dMinY)
        dMinY = dY;

    if (dY > dMaxY)
        dMaxY = dY;
}
void CPath::Draw(PdfWriter::CPage* pPage, bool bStroke, bool bFill, bool bEoFill)
{
    for (int nIndex = 0, nCount = m_vCommands.size(); nIndex < nCount; nIndex++)
    {
        CPathCommandBase* pCommand = m_vCommands.at(nIndex);
        pCommand->Draw(pPage);
    }

    if (bStroke && !bFill && !bEoFill)
        pPage->Stroke();
    else if (bStroke && bFill)
        pPage->FillStroke();
    else if (bStroke && bEoFill)
        pPage->EoFillStroke();
    else if (bFill)
        pPage->Fill();
    else if (bEoFill)
        pPage->EoFill();
    else
        pPage->EndPath();
}
void CPath::Clip(PdfWriter::CPage* pPage, bool bEvenOdd)
{
    for (int nIndex = 0, nCount = m_vCommands.size(); nIndex < nCount; nIndex++)
    {
        CPathCommandBase* pCommand = m_vCommands.at(nIndex);
        pCommand->Draw(pPage);
    }

    if (bEvenOdd)
        pPage->Eoclip();
    else
        pPage->Clip();

    pPage->EndPath();
}
void CPath::GetLastPoint(double& dX, double& dY)
{
    dX = 0;
    dY = 0;

    bool bFindMoveTo = false;
    for (int nIndex = m_vCommands.size() - 1; nIndex >= 0; nIndex--)
    {
        CPathCommandBase* pCommand = m_vCommands.at(nIndex);
        if (rendererpathcommand_Close == pCommand->GetType())
        {
            bFindMoveTo = true;
            continue;
        }
        else
        {
            pCommand->GetLastPoint(dX, dY);
            if (!bFindMoveTo || rendererpathcommand_MoveTo == pCommand->GetType())
                break;
        }
    }
}
void CPath::GetBounds(double& dL, double& dT, double& dR, double& dB)
{
    GetLastPoint(dL, dT);
    dR = dL;
    dB = dT;

    for (int nIndex = 0, nCount = m_vCommands.size(); nIndex < nCount; nIndex++)
    {
        CPathCommandBase* pCommand = m_vCommands.at(nIndex);
        pCommand->UpdateBounds(dL, dT, dR, dB);
    }
}
void CPath::CPathMoveTo::Draw(PdfWriter::CPage* pPage)
{
    pPage->MoveTo(x, y);
}
void CPath::CPathMoveTo::UpdateBounds(double& dL, double& dT, double& dR, double& dB)
{
    UpdateMaxMinPoints(dL, dT, dR, dB, x, y);
}
void CPath::CPathLineTo::Draw(PdfWriter::CPage* pPage)
{
    pPage->LineTo(x, y);
}
void CPath::CPathLineTo::UpdateBounds(double& dL, double& dT, double& dR, double& dB)
{
    UpdateMaxMinPoints(dL, dT, dR, dB, x, y);
}
void CPath::CPathCurveTo::Draw(PdfWriter::CPage* pPage)
{
    pPage->CurveTo(x1, y1, x2, y2, xe, ye);
}
void CPath::CPathCurveTo::UpdateBounds(double& dL, double& dT, double& dR, double& dB)
{
    UpdateMaxMinPoints(dL, dT, dR, dB, x1, y1);
    UpdateMaxMinPoints(dL, dT, dR, dB, x2, y2);
    UpdateMaxMinPoints(dL, dT, dR, dB, xe, ye);
}
void CPath::CPathArcTo::Draw(PdfWriter::CPage* pPage)
{
    if (sweepAngle >= 360 - 0.001)
        pPage->Ellipse(x + w / 2, y + h / 2, w / 2, h / 2);
    else
        pPage->EllipseArcTo(x + w / 2, y + h / 2, w / 2, h / 2, 360 - startAngle, 360 - (startAngle + sweepAngle), sweepAngle > 0 ? true : false);
}
void CPath::CPathArcTo::UpdateBounds(double& dL, double& dT, double& dR, double& dB)
{
    UpdateMaxMinPoints(dL, dT, dR, dB, x, y);
    UpdateMaxMinPoints(dL, dT, dR, dB, x + w, y + h);
}
void CPath::CPathClose::Draw(PdfWriter::CPage* pPage)
{
    pPage->ClosePath();
}
void CPath::CPathClose::UpdateBounds(double& dL, double& dT, double& dR, double& dB)
{
}
void CPath::CPathText::Draw(PdfWriter::CPage* pPage)
{
    // TODO: Если данная команда будет часто вызываться, тогда ее нужно будет оптимизировать, точно также как это делается в обычном тексте
    pPage->BeginText();
    pPage->SetFontAndSize(font, fontSize);
    pPage->SetCharSpace(charSpace);
    pPage->SetTextRenderingMode(PdfWriter::textrenderingmode_Stroke);
    pPage->DrawText(x, y, codes, codesCount);
    pPage->EndText();
}
void CPath::CPathText::UpdateBounds(double& dL, double& dT, double& dR, double& dB)
{
    UpdateMaxMinPoints(dL, dT, dR, dB, x, y);
}
void CBrushState::Reset()
{
    m_lType = c_BrushTypeSolid;
    m_oColor1.Set(0);
    m_oColor2.Set(0);
    m_nAlpha1 = 255;
    m_nAlpha2 = 255;
    m_wsTexturePath = L"";
    m_lTextureMode  = c_BrushTextureModeStretch;
    m_nTextureAlpha = 255;
    m_dLinearAngle  = 0;
    m_oRect.Reset();

    if (m_pShadingColors)
        delete[] m_pShadingColors;

    if (m_pShadingPoints)
        delete[] m_pShadingPoints;

    m_pShadingColors      = NULL;
    m_pShadingPoints      = NULL;
    m_lShadingPointsCount = 0;
}
