/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
        PdfWriter::CPage* pPage = m_pRenderer->GetPage();
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
			double             dRise = 0;
			double        dWordSpace = 0;
			double        dColor[4] = { 0, 0, 0, 0 };
			int          nColorSize = 1;
			std::wstring sTextName;

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
					if (pTextFont)
						pPage->SetFontAndSize(pTextFont, dTextSize);
                }

				if (sTextName != pText->GetName() || fabs(dTextSize - pText->GetSize()) > 0.001)
				{
					oTextLine.Flush(pPage);
					sTextName = pText->GetName();
					dTextSize = pText->GetSize();
					if (!sTextName.empty())
					{
						std::string sKey = U_TO_UTF8(sTextName);
						pPage->SetFontKeyAndSize(sKey.c_str(), dTextSize);
					}
				}

				if (!pText->GetName().empty())
				{
					int nColorSize2;
					double* dColor2 = pText->GetDColor2(nColorSize2);
					if (nColorSize2 == 1 && (nColorSize != nColorSize2 || dColor[0] != dColor2[0]))
					{
						oTextLine.Flush(pPage);
						nColorSize = nColorSize2;
						dColor[0] = dColor2[0];
						pPage->SetStrokeG(dColor[0]);
					}
					else if (nColorSize2 == 3 && (nColorSize != nColorSize2 || dColor[0] != dColor2[0] || dColor[1] != dColor2[1] || dColor[2] != dColor2[2]))
					{
						oTextLine.Flush(pPage);
						nColorSize = nColorSize2;
						dColor[0] = dColor2[0];
						dColor[1] = dColor2[1];
						dColor[2] = dColor2[2];
						pPage->SetStrokeRGB(dColor[0], dColor[1], dColor[2]);
					}
					else if (nColorSize2 == 4 && (nColorSize != nColorSize2 || dColor[0] != dColor2[0] || dColor[1] != dColor2[1] || dColor[2] != dColor2[2] || dColor[3] != dColor2[3]))
					{
						oTextLine.Flush(pPage);
						nColorSize = nColorSize2;
						dColor[0] = dColor2[0];
						dColor[1] = dColor2[1];
						dColor[2] = dColor2[2];
						dColor[3] = dColor2[3];
						pPage->SetStrokeCMYK(dColor[0], dColor[1], dColor[2], dColor[3]);
					}
				}
				else if (lTextColor != pText->GetColor())
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
					pPage->SetHorizontalScaling(dHorScaling);
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

				if (fabs(dRise - pText->GetRise()) > 0.001)
				{
					oTextLine.Flush(pPage);
					dRise = pText->GetRise();
					pPage->SetTextRise(dRise);
				}

				if (fabs(dWordSpace - pText->GetWordSpace()) > 0.001)
				{
					oTextLine.Flush(pPage);
					dWordSpace = pText->GetWordSpace();
					pPage->SetWordSpace(dWordSpace);
				}

				if (!pText->GetPUA().empty())
				{
					oTextLine.Flush(pPage);
					PdfWriter::CDictObject* pBDC = new PdfWriter::CDictObject();
					pBDC->Add("ActualText", new PdfWriter::CStringObject(pText->GetPUA().c_str(), true));
					pPage->BeginMarkedContentDict("Span", pBDC);
					RELEASEOBJECT(pBDC);
				}

                unsigned char* pCodes    = pText->GetCodes();
                unsigned short ushCode   = (pCodes[0] << 8) + pCodes[1];
                unsigned int   unLen     = pText->GetCodesLen();
                double         dX        = pText->GetX();
                double         dY        = pText->GetY();
                double         dTextSize = pText->GetSize();
				double         dWidth    = pText->GetFont() ? (pText->GetFont()->GetWidth(ushCode) / 1000.0 * dTextSize) : pText->GetWidth();

                if (!oTextLine.Add(pCodes, unLen, dX, dY, dWidth, dTextSize))
                {
                    oTextLine.Flush(pPage);
                    if (!oTextLine.Add(pCodes, unLen, dX, dY, dWidth, dTextSize))
                    {
                        pPage->DrawText(dX, dY, pCodes, unLen);
                    }
                }

				if (!pText->GetPUA().empty())
				{
					oTextLine.Flush(pPage);
					pPage->EndMarkedContent();
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
bool SkipPath(const std::vector<PdfWriter::CSegment>& arrForStroke, const PdfWriter::CPoint& P1, const PdfWriter::CPoint& P2)
{
	for (int i = 0; i < arrForStroke.size(); ++i)
	{
		PdfWriter::CPoint P3 = arrForStroke[i].start;
		PdfWriter::CPoint P4 = arrForStroke[i].end;
		// Вычисляем коэффициенты A, B, C для уравнения прямой P3P4: Ax + By + C = 0
		double A = P4.y - P3.y;
		double B = P3.x - P4.x;
		double C = P4.x * P3.y - P3.x * P4.y;

		// Проверяем, лежит ли точка P1 на прямой P3P4
		double check1 = A * P1.x + B * P1.y + C;

		// Проверяем, лежит ли точка P2 на прямой P3P4
		double check2 = A * P2.x + B * P2.y + C;

		// Если обе проверки близки к нулю (в пределах эпсилон), то лежит
		if ((std::abs(check1) < 0.006) && (std::abs(check2) < 0.006))
			return true;
	}
	return false;
}
void CPath::DrawPathRedact(PdfWriter::CMatrix oMatrix, Aggplus::CGraphicsPath* oPath, bool bStroke, const std::vector<PdfWriter::CSegment>& arrForStroke)
{
	PdfWriter::CMatrix oInverse = oMatrix.Inverse();

	size_t length = oPath->GetPointCount(), compound = oPath->GetCloseCount();
	std::vector<Aggplus::PointD> points = oPath->GetPoints(0, length + compound);

	double dXStart = -1, dYStart = -1, dXCur = -1, dYCur = -1;
	bool bBreak = false;
	for (size_t i = 0; i < length + compound; i++)
	{
		if (oPath->IsCurvePoint(i))
		{
			double dX = points[i].X;
			double dY = points[i].Y;
			oInverse.Apply(dX, dY);
			double dX2 = points[i + 1].X;
			double dY2 = points[i + 1].Y;
			oInverse.Apply(dX2, dY2);
			double dX3 = points[i + 2].X;
			double dY3 = points[i + 2].Y;
			if (bBreak)
			{
				bBreak = false;
				double dXCI = dXCur, dYCI = dYCur;
				oInverse.Apply(dXCI, dYCI);
				MoveTo(dXCI, dYCI);
			}
			dXCur = dX3; dYCur = dY3;
			oInverse.Apply(dX3, dY3);
			CurveTo(dX, dY, dX2, dY2, dX3, dY3);

			i += 2;
		}
		else if (oPath->IsMovePoint(i))
		{
			double dX = points[i].X, dY = points[i].Y;
			dXStart = dX; dYStart = dY; dXCur = dX; dYCur = dY;
			if (bStroke)
				bBreak = true;
			else
			{
				oInverse.Apply(dX, dY);
				MoveTo(dX, dY);
			}
		}
		else if (oPath->IsLinePoint(i))
		{
			double dX = points[i].X, dY = points[i].Y;
			if (bStroke && SkipPath(arrForStroke, PdfWriter::CPoint(dXCur, dYCur), PdfWriter::CPoint(dX, dY)))
			{
				dXCur = dX; dYCur = dY;
				bBreak = true;
				continue;
			}
			if (bBreak)
			{
				bBreak = false;
				double dXCI = dXCur, dYCI = dYCur;
				oInverse.Apply(dXCI, dYCI);
				MoveTo(dXCI, dYCI);
			}
			dXCur = dX; dYCur = dY;
			oInverse.Apply(dX, dY);
			LineTo(dX, dY);
		}
		else if (oPath->IsClosePoint(i))
		{
			if (bStroke && (std::abs(dXCur - dXStart) > PdfWriter::EPS || std::abs(dYCur - dYStart) > PdfWriter::EPS) && SkipPath(arrForStroke, PdfWriter::CPoint(dXCur, dYCur), PdfWriter::CPoint(dXStart, dYStart)))
			{
				dXCur = dXStart; dYCur = dYStart;
				bBreak = true;
				continue;
			}
			if (bStroke || bBreak)
			{
				if (std::abs(dXCur - dXStart) > PdfWriter::EPS || std::abs(dYCur - dYStart) > PdfWriter::EPS)
				{
					bBreak = false;
					double dXCI = dXCur, dYCI = dYCur;
					oInverse.Apply(dXCI, dYCI);
					double dXSI = dXStart, dYSI = dYStart;
					oInverse.Apply(dXSI, dYSI);
					MoveTo(dXCI, dYCI);
					LineTo(dXSI, dYSI);
				}
			}
			else
				Close();
		}
	}
}
void CPath::Redact(const CTransform& oTransform, const std::vector<double>& arrRedact, bool bStroke, bool bEoFill)
{
	PdfWriter::CMatrix oMatrix(oTransform.m11, oTransform.m12, oTransform.m21, oTransform.m22, oTransform.dx, oTransform.dy);
	PdfWriter::CMatrix oInverse = oMatrix.Inverse();

	Aggplus::CGraphicsPath oPath, oPathRedact, oPathResult;
	for (int i = 0; i < arrRedact.size(); i += 4)
	{
		oPathRedact.StartFigure();
		oPathRedact.MoveTo(arrRedact[i + 0], arrRedact[i + 1]);
		oPathRedact.LineTo(arrRedact[i + 0], arrRedact[i + 3]);
		oPathRedact.LineTo(arrRedact[i + 2], arrRedact[i + 3]);
		oPathRedact.LineTo(arrRedact[i + 2], arrRedact[i + 1]);
		oPathRedact.CloseFigure();
	}

	for (int nIndex = 0, nCount = m_vCommands.size(); nIndex < nCount; nIndex++)
	{
		CPathCommandBase* pCommand = m_vCommands.at(nIndex);
		// CPath to CGraphicsPath
		pCommand->ToCGraphicsPath(oTransform, oPath);
	}
	if (bEoFill)
		oPath.SetRuler(true);
	Clear();

	if (bStroke)
	{
		std::vector<PdfWriter::CSegment> arrForStroke;
		std::vector<PdfWriter::TRect> rectangles;
		for (int i = 0; i < arrRedact.size(); i += 4)
		{
			arrForStroke.push_back(PdfWriter::CSegment(PdfWriter::CPoint(arrRedact[i + 0], arrRedact[i + 1]), PdfWriter::CPoint(arrRedact[i + 0], arrRedact[i + 3])));
			arrForStroke.push_back(PdfWriter::CSegment(PdfWriter::CPoint(arrRedact[i + 0], arrRedact[i + 3]), PdfWriter::CPoint(arrRedact[i + 2], arrRedact[i + 3])));
			arrForStroke.push_back(PdfWriter::CSegment(PdfWriter::CPoint(arrRedact[i + 2], arrRedact[i + 3]), PdfWriter::CPoint(arrRedact[i + 2], arrRedact[i + 1])));
			arrForStroke.push_back(PdfWriter::CSegment(PdfWriter::CPoint(arrRedact[i + 2], arrRedact[i + 1]), PdfWriter::CPoint(arrRedact[i + 0], arrRedact[i + 1])));

			rectangles.push_back({arrRedact[i + 0], arrRedact[i + 3], arrRedact[i + 2], arrRedact[i + 1]});
		}

		size_t length = oPath.GetPointCount(), compound = oPath.GetCloseCount();
		std::vector<Aggplus::PointD> points = oPath.GetPoints(0, length + compound);
		double dXStart = -1, dYStart = -1, dXCur = -1, dYCur = -1;;
		for (size_t i = 0; i < length + compound; i++)
		{
			if (oPath.IsCurvePoint(i))
			{
				double dX = points[i].X;
				double dY = points[i].Y;
				double dX2 = points[i + 1].X;
				double dY2 = points[i + 1].Y;
				double dX3 = points[i + 2].X;
				double dY3 = points[i + 2].Y;
				i += 2;

				Aggplus::CGraphicsPath _oPath;
				_oPath.StartFigure();
				_oPath.MoveTo(dXCur, dYCur);
				_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
				_oPath.CloseFigure();

				arrForStroke.push_back(PdfWriter::CSegment(PdfWriter::CPoint(dXCur, dYCur), PdfWriter::CPoint(dX3, dY3)));
				dXCur = dX3, dYCur = dY3;

				oPathResult = Aggplus::CalcBooleanOperation(_oPath, oPathRedact, Aggplus::BooleanOpType::Subtraction);
				DrawPathRedact(oMatrix, &oPathResult, bStroke, arrForStroke);
				oPathResult.Reset();
			}
			else if (oPath.IsMovePoint(i))
			{
				double dX = points[i].X, dY = points[i].Y;
				dXStart = dX; dYStart = dY; dXCur = dX; dYCur = dY;
			}
			else if (oPath.IsLinePoint(i))
			{
				double dX = points[i].X, dY = points[i].Y;

				PdfWriter::CLineClipper clipper(rectangles);
				PdfWriter::CSegment line(PdfWriter::CPoint(dXCur, dYCur), PdfWriter::CPoint(dX, dY));
				dXCur = dX; dYCur = dY;

				auto visibleSegments = clipper.getVisibleSegments(line);
				for (int i = 0; i < visibleSegments.size(); ++i)
				{
					double dX1 = visibleSegments[i].start.x, dY1 = visibleSegments[i].start.y;
					double dX2 = visibleSegments[i].end.x,   dY2 = visibleSegments[i].end.y;
					oInverse.Apply(dX1, dY1);
					oInverse.Apply(dX2, dY2);
					MoveTo(dX1, dY1);
					LineTo(dX2, dY2);
				}
			}
			else if (oPath.IsClosePoint(i))
			{
				PdfWriter::CLineClipper clipper(rectangles);
				PdfWriter::CSegment line(PdfWriter::CPoint(dXCur, dYCur), PdfWriter::CPoint(dXStart, dYStart));
				auto visibleSegments = clipper.getVisibleSegments(line);
				for (int i = 0; i < visibleSegments.size(); ++i)
				{
					double dX1 = visibleSegments[i].start.x, dY1 = visibleSegments[i].start.y;
					double dX2 = visibleSegments[i].end.x,   dY2 = visibleSegments[i].end.y;
					oInverse.Apply(dX1, dY1);
					oInverse.Apply(dX2, dY2);
					MoveTo(dX1, dY1);
					LineTo(dX2, dY2);
				}
			}
		}
	}
	else
	{
		oPathResult = Aggplus::CalcBooleanOperation(oPath, oPathRedact, Aggplus::BooleanOpType::Subtraction);
		DrawPathRedact(oMatrix, &oPathResult, bStroke);
	}
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
void CPath::CPathMoveTo::ToCGraphicsPath(const CTransform& oTransform, Aggplus::CGraphicsPath& oPath)
{
	double dX, dY;
	oTransform.Transform(x, y, &dX, &dY);
	oPath.StartFigure();
	oPath.MoveTo(dX, dY);
}
void CPath::CPathLineTo::Draw(PdfWriter::CPage* pPage)
{
    pPage->LineTo(x, y);
}
void CPath::CPathLineTo::UpdateBounds(double& dL, double& dT, double& dR, double& dB)
{
    UpdateMaxMinPoints(dL, dT, dR, dB, x, y);
}
void CPath::CPathLineTo::ToCGraphicsPath(const CTransform& oTransform, Aggplus::CGraphicsPath& oPath)
{
	double dX, dY;
	oTransform.Transform(x, y, &dX, &dY);
	oPath.LineTo(dX, dY);
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
void CPath::CPathCurveTo::ToCGraphicsPath(const CTransform& oTransform, Aggplus::CGraphicsPath& oPath)
{
	double dX1, dY1, dX2, dY2, dX3, dY3;
	oTransform.Transform(x1, y1, &dX1, &dY1);
	oTransform.Transform(x2, y2, &dX2, &dY2);
	oTransform.Transform(xe, ye, &dX3, &dY3);
	oPath.CurveTo(dX1, dY1, dX2, dY2, dX3, dY3);
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
double AngToEllPrm(double dAngle, double dXRad, double dYRad)
{
	// Функция для перевода реального угла в параметрическое задание эллписа
	// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
	// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
	return atan2(sin(dAngle) / dYRad, cos(dAngle) / dXRad);
}
void WriteEllipseArc(const CTransform& oTransform, Aggplus::CGraphicsPath& oPath, double dX, double dY, double dXRad, double dYRad, double dAngle1, double dAngle2, double& dXCur, double& dYCur, bool bClockDirection = false)
{
	// Рассчитаем начальную, конечную и контрольные точки
	double dX1  = 0.0, dX2  = 0.0, dY1  = 0.0, dY2  = 0.0;
	double dCX1 = 0.0, dCX2 = 0.0, dCY1 = 0.0, dCY2 = 0.0;

	double dAlpha = sin(dAngle2 - dAngle1) * (sqrt(4.0 + 3.0 * tan((dAngle2 - dAngle1) / 2.0) * tan((dAngle2 - dAngle1) / 2.0)) - 1.0) / 3.0;

	dX1 = dX + dXRad * cos(dAngle1);
	dY1 = dY + dYRad * sin(dAngle1);

	dX2 = dX + dXRad * cos(dAngle2);
	dY2 = dY + dYRad * sin(dAngle2);

	dCX1 = dX1 - dAlpha * dXRad * sin(dAngle1);
	dCY1 = dY1 + dAlpha * dYRad * cos(dAngle1);

	dCX2 = dX2 + dAlpha * dXRad * sin(dAngle2);
	dCY2 = dY2 - dAlpha * dYRad * cos(dAngle2);

	if ( !bClockDirection )
	{
		dXCur = dX2;
		dYCur = dY2;

		double _dX1, _dY1, _dX2, _dY2, dX3, dY3;
		oTransform.Transform(dCX1, dCY1, &_dX1, &_dY1);
		oTransform.Transform(dCX2, dCY2, &_dX2, &_dY2);
		oTransform.Transform(dX2, dY2, &dX3, &dY3);
		oPath.CurveTo(_dX1, _dY1, _dX2, _dY2, dX3, dY3);
	}
	else
	{
		dXCur = dX1;
		dYCur = dY1;

		double _dX1, _dY1, _dX2, _dY2, dX3, dY3;
		oTransform.Transform(dCX2, dCY2, &_dX1, &_dY1);
		oTransform.Transform(dCX1, dCY1, &_dX2, &_dY2);
		oTransform.Transform(dX1, dY1, &dX3, &dY3);
		oPath.CurveTo(_dX1, _dY1, _dX2, _dY2, dX3, dY3);
	}
}
void EllipseArc(double dX, double dY, double dXRad, double dYRad, double _dAngle1, double _dAngle2, bool bClockDirection, const CTransform& oTransform, Aggplus::CGraphicsPath& oPath)
{
	// переведем углы в радианы
	double dAngle1 = _dAngle1 * 3.141592f / 180;
	double dAngle2 = _dAngle2 * 3.141592f / 180;

	// Выясним в каких четвертях находятся начальная и конечная точки
	int nFirstPointQuard  = int(_dAngle1) / 90 + 1;
	int nSecondPointQuard = int(_dAngle2) / 90 + 1;

	nSecondPointQuard = std::min(4, std::max(1, nSecondPointQuard));
	nFirstPointQuard  = std::min(4, std::max(1, nFirstPointQuard));

	// Проведем линию в начальную точку дуги
	double dStartX = 0.0, dStartY = 0.0, dEndX = 0.0, dEndY = 0.0;

	dStartX = dX + dXRad * cos(AngToEllPrm(dAngle1, dXRad, dYRad));
	dStartY = dY + dYRad * sin(AngToEllPrm(dAngle1, dXRad, dYRad));

	// Дальше рисуем по четверям
	double dCurX = dStartX, dCurY = dStartY;
	double dStartAngle = dAngle1;
	double dEndAngle = 0;

	if ( !bClockDirection )
	{
		for (unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++)
		{
			if (nIndex == nSecondPointQuard)
				dEndAngle = dAngle2;
			else
				dEndAngle = (90 * (nIndex)) * 3.141592f / 180;
			if (!(nIndex == nFirstPointQuard))
				dStartAngle = (90 * (nIndex - 1)) * 3.141592f / 180;

			WriteEllipseArc(oTransform, oPath, dX, dY, dXRad, dYRad, AngToEllPrm(dStartAngle, dXRad, dYRad), AngToEllPrm(dEndAngle, dXRad, dYRad), dEndX, dEndY, false);
		}
	}
	else
	{
		for( unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex-- )
		{
			if ( nIndex == nFirstPointQuard )
				dStartAngle = dAngle1;
			else
				dStartAngle = (90 * (nIndex ) ) * 3.141592f / 180;
			if ( !( nIndex == nSecondPointQuard ) )
				dEndAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;
			else
				dEndAngle = dAngle2;

			WriteEllipseArc(oTransform, oPath, dX, dY, dXRad, dYRad, AngToEllPrm(dStartAngle, dXRad, dYRad), AngToEllPrm(dEndAngle, dXRad, dYRad), dEndX, dEndY, false);
		}
	}
}
void CPath::CPathArcTo::ToCGraphicsPath(const CTransform& oTransform, Aggplus::CGraphicsPath& oPath)
{
	if (sweepAngle >= 360 - 0.001)
	{
		const double c_dKappa = 0.552;

		double dX = x + w / 2;
		double dY = y + h / 2;
		double dXRay = w / 2;
		double dYRay = h / 2;

		double _dX, _dY;
		oTransform.Transform(dX, dY, &_dX, &_dY);
		oPath.MoveTo(_dX, _dY);

		double dX1, dY1, dX2, dY2, dX3, dY3;
		oTransform.Transform(dX - dXRay, dY + dYRay * c_dKappa, &dX1, &dY1);
		oTransform.Transform(dX - dXRay * c_dKappa, dY + dYRay, &dX2, &dY2);
		oTransform.Transform(dX, dY + dYRay, &dX3, &dY3);
		oPath.CurveTo(dX1, dY1, dX2, dY2, dX3, dY3);

		oTransform.Transform(dX + dXRay * c_dKappa, dY + dYRay, &dX1, &dY1);
		oTransform.Transform(dX + dXRay, dY + dYRay * c_dKappa, &dX2, &dY2);
		oTransform.Transform(dX + dXRay, dY, &dX3, &dY3);
		oPath.CurveTo(dX1, dY1, dX2, dY2, dX3, dY3);

		oTransform.Transform(dX + dXRay, dY - dYRay * c_dKappa, &dX1, &dY1);
		oTransform.Transform(dX + dXRay * c_dKappa, dY - dYRay, &dX2, &dY2);
		oTransform.Transform(dX, dY - dYRay, &dX3, &dY3);
		oPath.CurveTo(dX1, dY1, dX2, dY2, dX3, dY3);

		oTransform.Transform(dX - dXRay * c_dKappa, dY - dYRay, &dX1, &dY1);
		oTransform.Transform(dX - dXRay, dY - dYRay * c_dKappa, &dX2, &dY2);
		oTransform.Transform(dX - dXRay, dY, &dX3, &dY3);
		oPath.CurveTo(dX1, dY1, dX2, dY2, dX3, dY3);
	}
	else
	{
		double dX = x + w / 2, dY = y + h / 2, dXRad = w / 2, dYRad = h / 2, _dAngle1 = 360 - startAngle, _dAngle2 = 360 - (startAngle + sweepAngle);
		bool bClockDirection = sweepAngle > 0;
		// Проверяем эллипс на невырожденность
		if (dXRad < 0.001 || dYRad < 0.001)
		{
			double dAngle1 = _dAngle1 * 3.141592f / 180;
			double dAngle2 = _dAngle2 * 3.141592f / 180;

			double _dX, _dY;
			if (dXRad < 0.001 && dYRad < 0.001)
			{
				oTransform.Transform(dX, dY, &_dX, &_dY);
				oPath.LineTo(_dX, _dY);
			}
			else if (dXRad < 0.001)
			{
				oTransform.Transform(dX, dY + sin(dAngle1) * dYRad, &_dX, &_dY);
				oPath.LineTo(_dX, _dY);
				oTransform.Transform(dX, dY + sin(dAngle2) * dYRad, &_dX, &_dY);
				oPath.LineTo(_dX, _dY);
			}
			else // if (dYRad < 0.001)
			{
				oTransform.Transform(dX + cos(dAngle1) * dXRad, dY, &_dX, &_dY);
				oPath.LineTo(_dX, _dY);
				oTransform.Transform(dX + cos(dAngle2) * dXRad, dY, &_dX, &_dY);
				oPath.LineTo(_dX, _dY);
			}
			return;
		}

		while (_dAngle1 < 0)
			_dAngle1 += 360;

		while (_dAngle1 > 360)
			_dAngle1 -= 360;

		while (_dAngle2 < 0)
			_dAngle2 += 360;

		while (_dAngle2 > 360)
			_dAngle2 -= 360;

		if (!bClockDirection)
		{
			if (_dAngle1 <= _dAngle2)
				EllipseArc(dX, dY, dXRad, dYRad, _dAngle1, _dAngle2, false, oTransform, oPath);
			else
			{
				EllipseArc(dX, dY, dXRad, dYRad, _dAngle1, 360, false, oTransform, oPath);
				EllipseArc(dX, dY, dXRad, dYRad, 0, _dAngle2, false, oTransform, oPath);
			}
		}
		else
		{
			if (_dAngle1 >= _dAngle2)
				EllipseArc(dX, dY, dXRad, dYRad, _dAngle1, _dAngle2, true, oTransform, oPath);
			else
			{
				EllipseArc(dX, dY, dXRad, dYRad, _dAngle1, 0, true, oTransform, oPath);
				EllipseArc(dX, dY, dXRad, dYRad, 360, _dAngle2, true, oTransform, oPath);
			}
		}
	}
}
void CPath::CPathClose::Draw(PdfWriter::CPage* pPage)
{
    pPage->ClosePath();
}
void CPath::CPathClose::UpdateBounds(double& dL, double& dT, double& dR, double& dB)
{
}
void CPath::CPathClose::ToCGraphicsPath(const CTransform& oTransform, Aggplus::CGraphicsPath& oPath)
{
	oPath.CloseFigure();
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
void CPath::CPathText::ToCGraphicsPath(const CTransform& oTransform, Aggplus::CGraphicsPath& oPath)
{
	// Весь текст проверяется в CPdfWriter::PathCommandDrawText
	// Эта функция не должна быть вызвана
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
