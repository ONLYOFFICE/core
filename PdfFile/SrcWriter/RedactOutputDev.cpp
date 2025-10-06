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

#include "RedactOutputDev.h"
#include "Types.h"
#include "Streams.h"
#include "Utils.h"

#include "../lib/xpdf/GfxFont.h"
#include "../lib/xpdf/XRef.h"

#include "../../DesktopEditor/graphics/GraphicsPath.h"

namespace PdfWriter
{
void Transform(double* pMatrix, double dUserX, double dUserY, double* pdDeviceX, double* pdDeviceY)
{
	*pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
	*pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
}

//----- constructor/destructor
RedactOutputDev::RedactOutputDev(CPdfWriter* pRenderer)
{
	m_pXref = NULL;

	m_pRenderer = pRenderer;
	m_pDoc = m_pRenderer->GetDocument();
	m_pPage = NULL;

	m_bUpdateAll = false;
}
RedactOutputDev::~RedactOutputDev()
{
	m_pRenderer = NULL;
	m_pDoc = NULL;
	m_pPage = NULL;
}
void RedactOutputDev::SetRedact(const std::vector<double>& arrQuadPoints)
{
	m_arrQuadPoints = arrQuadPoints;
	for (int i = 0; i < m_arrQuadPoints.size(); i += 4)
	{
		m_oPathRedact.StartFigure();
		m_oPathRedact.MoveTo(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]);
		m_oPathRedact.LineTo(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 3]);
		m_oPathRedact.LineTo(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]);
		m_oPathRedact.LineTo(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 1]);
		m_oPathRedact.CloseFigure();
	}
}
void RedactOutputDev::NewPDF(XRef* pXref)
{
	m_pXref = pXref;
}
//----- initialization and control
void RedactOutputDev::setDefaultCTM(double *ctm)
{
	memcpy(m_arrMatrix, ctm, 6 * sizeof(double));
	if (m_pPage)
		m_pPage->StartTransform(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
}
void RedactOutputDev::startPage(int nPageIndex, GfxState *pGState)
{
	m_pPage = m_pDoc->GetEditPage(nPageIndex - 1);
	m_pRenderer->EditPage(m_pPage);
	m_pDoc->SetCurPage(m_pPage);
	m_pDoc->ClearPageFull();
}
void RedactOutputDev::endPage()
{
	m_pRenderer->m_oCommandManager.Flush();
	m_pPage = NULL;
}
//----- save/restore graphics state
void RedactOutputDev::saveState(GfxState *pGState)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();
	m_pPage->GrSave();
	m_sStates.push_back(GfxRedactState());
	updateAll(pGState);
}
void RedactOutputDev::restoreState(GfxState *pGState)
{
	m_pRenderer->m_oCommandManager.Flush();
	m_pPage->GrRestore();
	updateAll(pGState);

	if (m_sStates.empty())
		return; // Несбалансированный q/Q - сломанный файл
	m_sStates.pop_back();
}
//----- update graphics state
void RedactOutputDev::updateAll(GfxState *pGState)
{
	m_bUpdateAll = true;
	updateLineDash(pGState);
	updateFlatness(pGState);
	updateLineJoin(pGState);
	updateLineCap(pGState);
	updateMiterLimit(pGState);
	updateLineWidth(pGState);
	// updateStrokeAdjust(pGState);
	// updateFillColorSpace(pGState);
	// updateFillColor(pGState);
	// updateStrokeColorSpace(pGState);
	// updateStrokeColor(pGState);
	updateRenderingIntent(pGState);
	// updateBlendMode(pGState);
	// updateFillOpacity(pGState);
	// updateStrokeOpacity(pGState);
	updateFont(pGState);
	m_bUpdateAll = false;
}
void RedactOutputDev::updateCTM(GfxState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32)
{
	// TODO применять только непосредственно при записи
	//m_pPage->Concat(dMatrix11, dMatrix12, dMatrix21, dMatrix22, dMatrix31, dMatrix32);
}
void RedactOutputDev::updateLineDash(GfxState *pGState)
{
	double* pDash  = NULL;
	int     nSize  = 0;
	double  dStart = 0;
	pGState->getLineDash(&pDash, &nSize, &dStart);

	if (0 == nSize) // Solid
	{
		m_pRenderer->put_PenDashOffset(0);
		m_pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
	}
	else
	{
		m_pRenderer->m_oPen.SetDashPattern(pDash, nSize);
		m_pRenderer->m_oPen.SetDashOffset(dStart);
		m_pRenderer->m_oPen.SetDashStyle(Aggplus::DashStyleCustom);
	}

	if (m_bUpdateAll || m_sStates.empty())
		return;
	std::string sOp = "[ ";
	for (int i = 0; i < nSize; ++i)
		sOp += (std::to_string(pDash[i]) + " ");
	sOp += "] " + std::to_string(dStart);
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "d"));
}
void RedactOutputDev::updateFlatness(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetFlatness(pGState->getFlatness());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getFlatness()), "i"));
}
void RedactOutputDev::updateLineJoin(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetJoinStyle(pGState->getLineJoin());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getLineJoin()), "j"));
}
void RedactOutputDev::updateLineCap(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetStartCapStyle(pGState->getLineCap());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getLineCap()), "J"));
}
void RedactOutputDev::updateMiterLimit(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetMiter(pGState->getMiterLimit());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getMiterLimit()), "M"));
}
void RedactOutputDev::updateLineWidth(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetSize(pGState->getLineWidth());
	if (!m_bUpdateAll && !m_sStates.empty())
		m_sStates.back().m_arrOp.push_back(std::make_pair(std::to_string(pGState->getLineWidth()), "w"));
}
void RedactOutputDev::updateFillColorSpace(GfxState *pGState)
{

}
void RedactOutputDev::updateStrokeColorSpace(GfxState *pGState)
{

}
void RedactOutputDev::updateFillColor(GfxState *pGState)
{
	if (m_sStates.empty())
		return;
	GfxColorSpace* pColorSpace = pGState->getFillColorSpace();
	GfxColorSpaceMode eMode = pColorSpace->getMode();
	GfxColor* pColor = pGState->getFillColor();
	std::string sOp;
	std::string sOp2;
	switch (eMode)
	{
	case csDeviceGray:
	{
		sOp = std::to_string(colToDbl(pColor->c[0]));
		sOp2 = "g";
		break;
	}
	case csDeviceRGB:
	{
		sOp = std::to_string(colToDbl(pColor->c[0])) + " " + std::to_string(colToDbl(pColor->c[1])) + " " + std::to_string(colToDbl(pColor->c[2]));
		sOp2 = "rg";
		break;
	}
	case csDeviceCMYK:
	{
		sOp = std::to_string(colToDbl(pColor->c[0])) + " " + std::to_string(colToDbl(pColor->c[1])) + " " + std::to_string(colToDbl(pColor->c[2])) + " " + std::to_string(colToDbl(pColor->c[3]));
		sOp2 = "k";
		break;
	}
	default:
		break;
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, sOp2));
}
void RedactOutputDev::updateStrokeColor(GfxState *pGState)
{
	if (m_sStates.empty())
		return;
	GfxColorSpace* pColorSpace = pGState->getStrokeColorSpace();
	GfxColorSpaceMode eMode = pColorSpace->getMode();
	GfxColor* pColor = pGState->getStrokeColor();
	std::string sOp;
	std::string sOp2;
	switch (eMode)
	{
	case csDeviceGray:
	{
		sOp = std::to_string(colToDbl(pColor->c[0]));
		sOp2 = "G";
		break;
	}
	case csDeviceRGB:
	{
		sOp = std::to_string(colToDbl(pColor->c[0])) + " " + std::to_string(colToDbl(pColor->c[1])) + " " + std::to_string(colToDbl(pColor->c[2]));
		sOp2 = "RG";
		break;
	}
	case csDeviceCMYK:
	{
		sOp = std::to_string(colToDbl(pColor->c[0])) + " " + std::to_string(colToDbl(pColor->c[1])) + " " + std::to_string(colToDbl(pColor->c[2])) + " " + std::to_string(colToDbl(pColor->c[3]));
		sOp2 = "K";
		break;
	}
	default:
		break;
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, sOp2));
}
void RedactOutputDev::updateRenderingIntent(GfxState *pGState)
{
	if (m_bUpdateAll || m_sStates.empty())
		return;
	std::string sOp = "/";

	GfxRenderingIntent eRI = pGState->getRenderingIntent();
	switch (eRI)
	{
	case GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric:
		sOp += "AbsoluteColorimetric";
		break;
	case GfxRenderingIntent::gfxRenderingIntentRelativeColorimetric:
		sOp += "RelativeColorimetric";
		break;
	case GfxRenderingIntent::gfxRenderingIntentSaturation:
		sOp += "Saturation";
		break;
	case GfxRenderingIntent::gfxRenderingIntentPerceptual:
	default:
		sOp += "Perceptual";
		break;
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "ri"));
}
//----- update text state
void RedactOutputDev::updateFont(GfxState *pGState)
{
	GfxFont* pFont = pGState->getFont();
	if (pFont)
	{
		m_pRenderer->m_oFont.SetSize(pGState->getFontSize());
		m_pRenderer->put_FontName(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pFont->getTag()->getCString(), pFont->getTag()->getLength()));
	}
	else
		m_pRenderer->put_FontName(L"");
}
void RedactOutputDev::updateTextMat(GfxState *pGState)
{
	//double* dTM = pGState->getTextMat();
	// TODO
	//m_pPage->SetTextMatrix(dTM[0], dTM[1], dTM[2], dTM[3], dTM[4], dTM[5]);
}
void RedactOutputDev::updateCharSpace(GfxState *pGState)
{
	m_pRenderer->m_oFont.SetCharSpace(pGState->getCharSpace());
}
void RedactOutputDev::updateRender(GfxState *pGState)
{
	int nRender = pGState->getRender();
	m_pRenderer->m_oFont.SetRenderMode(nRender);
}
void RedactOutputDev::updateRise(GfxState *pGState)
{
	m_pRenderer->m_oFont.SetRise(pGState->getRise());
}
void RedactOutputDev::updateWordSpace(GfxState *pGState)
{
	m_pRenderer->m_oFont.SetWordSpace(pGState->getWordSpace());
}
void RedactOutputDev::updateHorizScaling(GfxState *pGState)
{
	m_pRenderer->m_oFont.SetHorizontalScaling(pGState->getHorizScaling() * 100);
}
void RedactOutputDev::updateTextPos(GfxState *pGState)
{
	// TODO Это Td, но опять таки нужно смещать к реальному тексту который не попадает под Redact
}
void RedactOutputDev::updateTextShift(GfxState *pGState, double shift)
{
	// TODO Смещение между строками в TJ, т.е. ~ TL межстрочный интервал
}
//----- path painting
void RedactOutputDev::stroke(GfxState *pGState)
{
	DoPathRedact(pGState, pGState->getPath(), pGState->getCTM(), true);
	DrawPath(c_nStroke);
}
void RedactOutputDev::fill(GfxState *pGState)
{
	DoPathRedact(pGState, pGState->getPath(), pGState->getCTM());
	DrawPath(c_nWindingFillMode);
}
void RedactOutputDev::eoFill(GfxState *pGState)
{
	DoPathRedact(pGState, pGState->getPath(), pGState->getCTM(), false, true);
	DrawPath(c_nEvenOddFillMode);
}
void RedactOutputDev::tilingPatternFill(GfxState *pGState, Gfx *gfx, Object *pStream, int nPaintType, int nTilingType, Dict *pResourcesDict,
										double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep)
{
	// TODO Нужно как-то пересечь области заливки паттерном
}
GBool RedactOutputDev::shadedFill(GfxState* pGState, GfxShading* shading)
{
	// TODO Нужно как-то пересечь области градиентой заливки
	return gFalse;
}
//----- path clipping
void RedactOutputDev::clip(GfxState *pGState)
{
	if (m_sStates.empty())
		return;

	if (!m_sStates.back().m_pClip)
		m_sStates.back().m_pClip = new GfxClip();
	int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeWinding;
	m_sStates.back().m_pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
	AddClip(pGState, &m_sStates.back(), m_sStates.back().m_pClip->GetPathNum() - 1);
}
void RedactOutputDev::eoClip(GfxState *pGState)
{
	if (m_sStates.empty())
		return;

	if (!m_sStates.back().m_pClip)
		m_sStates.back().m_pClip = new GfxClip();
	int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeEvenOdd;
	m_sStates.back().m_pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
	AddClip(pGState, &m_sStates.back(), m_sStates.back().m_pClip->GetPathNum() - 1);
}
void RedactOutputDev::clipToStrokePath(GfxState *pGState)
{
	if (m_sStates.empty())
		return;

	if (!m_sStates.back().m_pClip)
		m_sStates.back().m_pClip = new GfxClip();
	int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeWinding | c_nClipToStrokePath;
	m_sStates.back().m_pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);
	AddClip(pGState, &m_sStates.back(), m_sStates.back().m_pClip->GetPathNum() - 1);
}
//----- text drawing
void RedactOutputDev::beginStringOp(GfxState *pGState)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();
}
void RedactOutputDev::endStringOp(GfxState *pGState)
{

}
void RedactOutputDev::beginString(GfxState *pGState, GString *s)
{

}
void RedactOutputDev::endString(GfxState *pGState)
{

}
void RedactOutputDev::drawChar(GfxState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY,
							   CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen)
{
	double* pCTM = pGState->getCTM();
	double* pTm = pGState->getTextMat();
	double pNewTm[6], arrMatrix[6];

	double dTextScale = std::min(sqrt(pTm[2] * pTm[2] + pTm[3] * pTm[3]), sqrt(pTm[0] * pTm[0] + pTm[1] * pTm[1]));
	double dITextScale = 1 / dTextScale;
	double dOldSize = 10.0;
	m_pRenderer->get_FontSize(&dOldSize);
	if (dOldSize * dTextScale > 0)
	{
		m_pRenderer->put_FontSize(dOldSize * dTextScale);

		pNewTm[0] =  pTm[0] * dITextScale * pGState->getHorizScaling();
		pNewTm[1] =  pTm[1] * dITextScale * pGState->getHorizScaling();
		pNewTm[2] =  pTm[2] * dITextScale;
		pNewTm[3] =  pTm[3] * dITextScale;
		pNewTm[4] =  dX - dOriginX;
		pNewTm[5] =  dY - dOriginY;
	}
	else
	{
		m_pRenderer->put_FontSize(-dOldSize * dTextScale);

		pNewTm[0] = pTm[0] * dITextScale * pGState->getHorizScaling();
		pNewTm[1] = pTm[1] * dITextScale * pGState->getHorizScaling();
		pNewTm[2] = pTm[2] * dITextScale;
		pNewTm[3] = pTm[3] * dITextScale;
		pNewTm[4] = dX;
		pNewTm[5] = dY;
	}

	arrMatrix[0] = pNewTm[0] * pCTM[0] + pNewTm[1] * pCTM[2];
	arrMatrix[1] = pNewTm[0] * pCTM[1] + pNewTm[1] * pCTM[3];
	arrMatrix[2] = pNewTm[2] * pCTM[0] + pNewTm[3] * pCTM[2];
	arrMatrix[3] = pNewTm[2] * pCTM[1] + pNewTm[3] * pCTM[3];
	arrMatrix[4] = pNewTm[4] * pCTM[0] + pNewTm[5] * pCTM[2] + pCTM[4];
	arrMatrix[5] = pNewTm[4] * pCTM[1] + pNewTm[5] * pCTM[3] + pCTM[5];

	double dSize = 1;
	if (true)
	{
		double dNorma = std::min(sqrt(arrMatrix[0] * arrMatrix[0] + arrMatrix[1] * arrMatrix[1]), sqrt(arrMatrix[2] * arrMatrix[2] + arrMatrix[3] * arrMatrix[3]));
		if (dNorma > 0 && dNorma != 1)
		{
			arrMatrix[0] /= dNorma;
			arrMatrix[1] /= dNorma;
			arrMatrix[2] /= dNorma;
			arrMatrix[3] /= dNorma;

			m_pRenderer->get_FontSize(&dSize);
			dSize *= dNorma;
			m_pRenderer->put_FontSize(dSize);
		}
	}

	double dShiftX = 0, dShiftY = 0;
	DoTransform(arrMatrix, &dShiftX, &dShiftY, true);

	double startX, startY, endX, endY;
	pGState->transform(dX, dY, &startX, &startY);
	pGState->transform(dX + dDx, dY + dDy, &endX, &endY);
	double dCenterX = (startX + endX) / 2;
	double dCenterY = (startY + endY) / 2;
	for (int i = 0; i < m_arrQuadPoints.size(); i += 4)
	{
		double xMin = m_arrQuadPoints[i + 0];
		double yMin = m_arrQuadPoints[i + 1];
		double xMax = m_arrQuadPoints[i + 2];
		double yMax = m_arrQuadPoints[i + 3];

		if (xMin < dCenterX && dCenterX < xMax && yMin < dCenterY && dCenterY < yMax)
		{
			m_pRenderer->put_FontSize(dOldSize);
			return;
		}
	}

	BYTE* pCodes = new BYTE[2];
	pCodes[0] = (nCode >> 8) & 0xFF;
	pCodes[1] = nCode & 0xFF;

	m_pRenderer->m_oCommandManager.SetTransform(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);

	CRendererTextCommand* pText = m_pRenderer->m_oCommandManager.AddText(pCodes, 2, dShiftX, dShiftY);
	pText->SetName(m_pRenderer->m_oFont.GetName());
	pText->SetSize(m_pRenderer->m_oFont.GetSize());
	pText->SetType((EFontType)pGState->getFont()->getType());
	pText->SetCharSpace(m_pRenderer->m_oFont.GetCharSpace());
	pText->SetMode(m_pRenderer->m_oFont.GetRenderMode());
	pText->SetRise(m_pRenderer->m_oFont.GetRise());
	pText->SetWordSpace(m_pRenderer->m_oFont.GetWordSpace());
	pText->SetHorScaling(m_pRenderer->m_oFont.GetHorizontalScaling());
	pText->SetWidth(dDx);

	m_pRenderer->put_FontSize(dOldSize);
}
GBool RedactOutputDev::beginType3Char(GfxState *pGState, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen)
{
	return gFalse;
}
void RedactOutputDev::endType3Char(GfxState *pGState)
{

}
void RedactOutputDev::endTextObject(GfxState *pGState)
{

}
void RedactOutputDev::beginActualText(GfxState *state, Unicode *u, int uLen)
{

}
void RedactOutputDev::endActualText(GfxState *state)
{

}
//----- additional
GBool RedactOutputDev::beginMarkedContent(GfxState *pGState, GString *s)
{
	return gFalse;
}
GBool RedactOutputDev::beginMCOShapes(GfxState *pGState, GString *s, Object *ref)
{
	return gFalse;
}
void RedactOutputDev::endMarkedContent(GfxState *pGState)
{

}
GBool RedactOutputDev::useNameOp()
{
	return gTrue;
}
void RedactOutputDev::setExtGState(const char* name)
{
	if (m_sStates.empty())
		return;
	std::string sOp = "/" + std::string(name);
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "gs"));
}
void RedactOutputDev::setFillColorSpace(const char* name)
{
	if (m_sStates.empty())
		return;
	std::string sOp = "/" + std::string(name);
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "cs"));
}
void RedactOutputDev::setFillColor(Object* args, int numArgs)
{
	if (m_sStates.empty())
		return;
	std::string sOp;
	for (int i = 0; i < numArgs; ++i)
	{
		if (args[i].isName())
			sOp += ("/" + std::string(args[i].getName()) + " ");
		else if (args[i].isInt())
			sOp += (std::to_string(args[i].getInt()) + " ");
		else if (args[i].isReal())
			sOp += (std::to_string(args[i].getReal()) + " ");
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "sc"));
}
void RedactOutputDev::setFillColorN(Object* args, int numArgs)
{
	if (m_sStates.empty())
		return;
	std::string sOp;
	for (int i = 0; i < numArgs; ++i)
	{
		if (args[i].isName())
			sOp += ("/" + std::string(args[i].getName()) + " ");
		else if (args[i].isInt())
			sOp += (std::to_string(args[i].getInt()) + " ");
		else if (args[i].isReal())
			sOp += (std::to_string(args[i].getReal()) + " ");
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "scn"));
}
void RedactOutputDev::setStrokeColorSpace(const char* name)
{
	if (m_sStates.empty())
		return;
	std::string sOp = "/" + std::string(name);
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "CS"));
}
void RedactOutputDev::setStrokeColor(Object* args, int numArgs)
{
	if (m_sStates.empty())
		return;
	std::string sOp;
	for (int i = 0; i < numArgs; ++i)
	{
		if (args[i].isName())
			sOp += ("/" + std::string(args[i].getName()) + " ");
		else if (args[i].isInt())
			sOp += (std::to_string(args[i].getInt()) + " ");
		else if (args[i].isReal())
			sOp += (std::to_string(args[i].getReal()) + " ");
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "SC"));
}
void RedactOutputDev::setStrokeColorN(Object* args, int numArgs)
{
	if (m_sStates.empty())
		return;
	std::string sOp;
	for (int i = 0; i < numArgs; ++i)
	{
		if (args[i].isName())
			sOp += ("/" + std::string(args[i].getName()) + " ");
		else if (args[i].isInt())
			sOp += (std::to_string(args[i].getInt()) + " ");
		else if (args[i].isReal())
			sOp += (std::to_string(args[i].getReal()) + " ");
	}
	m_sStates.back().m_arrOp.push_back(std::make_pair(sOp, "SCN"));
}
void RedactOutputDev::setShading(GfxState *pGState, const char* name)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	// TODO Нужно проверять Shading на отсечение?

	m_pPage->GrSave();
	UpdateTransform();
	DoStateOp();
	CStream* pStream = m_pPage->GetStream();
	pStream->WriteEscapeName(name);
	pStream->WriteChar(' ');
	pStream->WriteStr("sh");
	pStream->WriteStr("\012");
	m_pPage->GrRestore();
}
//----- image drawing
void RedactOutputDev::drawImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate)
{

}
void RedactOutputDev::setSoftMaskFromImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate)
{

}
void RedactOutputDev::drawImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, GBool bInlineImg, GBool interpolate)
{

}
void RedactOutputDev::drawMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
							 Object* pMaskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GBool bMaskInvert, GBool interpolate)
{

}
void RedactOutputDev::drawSoftMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
								 Object *maskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GfxImageColorMap *pMaskColorMap, double *pMatte, GBool interpolate)
{

}
//----- Type 3 font operators
void RedactOutputDev::type3D0(GfxState *pGState, double wx, double wy)
{

}
void RedactOutputDev::type3D1(GfxState *pGState, double wx, double wy, double llx, double lly, double urx, double ury)
{

}
//----- form XObjects
void RedactOutputDev::drawForm(GfxState *pGState, Ref id, const char* name)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	// TODO пока что исключается всё изображение
	Object oForm;
	if (!m_pXref->fetch(id.num, id.gen, &oForm)->isStream())
	{
		oForm.free();
		return;
	}

	double dXmin = 0, dYmin = 0, dXmax = 0, dYmax = 0;
	double dX1 = 0, dY1 = 0, dX2 = 0, dY2 = 1, dX3 = 1, dY3 = 1, dX4 = 1, dY4 = 0;
	Object oObj;
	if (oForm.streamGetDict()->lookup("BBox", &oObj)->isArray() && oObj.arrayGetLength() == 4)
	{
		Object oNum;
		if (oObj.arrayGet(0, &oNum)->isNum())
			dXmin = oNum.getNum();
		oNum.free();
		if (oObj.arrayGet(1, &oNum)->isNum())
			dYmin = oNum.getNum();
		oNum.free();
		if (oObj.arrayGet(2, &oNum)->isNum())
			dXmax = oNum.getNum();
		oNum.free();
		if (oObj.arrayGet(3, &oNum)->isNum())
			dYmax = oNum.getNum();
		oNum.free(); oObj.free();

		dX1 = dXmin, dY1 = dYmin;
		dX2 = dXmin, dY2 = dYmax;
		dX3 = dXmax, dY3 = dYmax;
		dX4 = dXmax, dY4 = dYmin;

		double oMatrix[6] = { 1, 0, 0, 1, 0, 0 };
		if (oForm.streamGetDict()->lookup("Matrix", &oObj)->isArray() && oObj.arrayGetLength() == 6)
		{
			Object oObj2;
			for (int i = 0; i < 6; ++i)
			{
				oMatrix[i] = oObj.arrayGet(i, &oObj2)->getNum();
				oObj2.free();
			}
		}

		Transform(oMatrix, dX1, dY1, &dX1, &dY1);
		Transform(oMatrix, dX2, dY2, &dX2, &dY2);
		Transform(oMatrix, dX3, dY3, &dX3, &dY3);
		Transform(oMatrix, dX4, dY4, &dX4, &dY4);

		Transform(m_arrMatrix, dX1, dY1, &dX1, &dY1);
		Transform(m_arrMatrix, dX2, dY2, &dX2, &dY2);
		Transform(m_arrMatrix, dX3, dY3, &dX3, &dY3);
		Transform(m_arrMatrix, dX4, dY4, &dX4, &dY4);
	}
	oObj.free();

	std::vector<CPoint> poly2 =
	{
		CPoint(dX1, dY1),
		CPoint(dX2, dY2),
		CPoint(dX3, dY3),
		CPoint(dX4, dY4)
	};
	for (int i = 0; i < m_arrQuadPoints.size(); i += 4)
	{
		std::vector<CPoint> poly1 =
		{
			CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]),
			CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 1]),
			CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]),
			CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 3])
		};

		if (PdfWriter::SAT(poly1, poly2))
			return;
	}

	m_pPage->GrSave();
	UpdateTransform();
	m_pPage->ExecuteXObject(name);
	m_pPage->GrRestore();
}
void RedactOutputDev::drawImage(GfxState *pGState, Ref id, const char* name)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	// TODO пока что исключается всё изображение

	double dX1 = 0, dY1 = 0, dX2 = 0, dY2 = 1, dX3 = 1, dY3 = 1, dX4 = 1, dY4 = 0;
	Transform(m_arrMatrix, dX1, dY1, &dX1, &dY1);
	Transform(m_arrMatrix, dX2, dY2, &dX2, &dY2);
	Transform(m_arrMatrix, dX3, dY3, &dX3, &dY3);
	Transform(m_arrMatrix, dX4, dY4, &dX4, &dY4);

	std::vector<CPoint> poly2 =
	{
		CPoint(dX1, dY1),
		CPoint(dX2, dY2),
		CPoint(dX3, dY3),
		CPoint(dX4, dY4)
	};
	for (int j = 0; j < m_arrQuadPoints.size(); j += 4)
	{
		std::vector<CPoint> poly1 =
		{
			CPoint(m_arrQuadPoints[j + 0], m_arrQuadPoints[j + 1]),
			CPoint(m_arrQuadPoints[j + 2], m_arrQuadPoints[j + 1]),
			CPoint(m_arrQuadPoints[j + 2], m_arrQuadPoints[j + 3]),
			CPoint(m_arrQuadPoints[j + 0], m_arrQuadPoints[j + 3])
		};

		if (PdfWriter::SAT(poly1, poly2))
			return;
	}

	m_pPage->GrSave();
	UpdateTransform();
	m_pPage->ExecuteXObject(name);
	m_pPage->GrRestore();
}
//----- transparency groups and soft masks
void RedactOutputDev::beginTransparencyGroup(GfxState *pGState, double *pBBox, GfxColorSpace *pBlendingColorSpace, GBool bIsolated, GBool bKnockout, GBool bForSoftMask)
{

}
void RedactOutputDev::endTransparencyGroup(GfxState *pGState)
{

}
void RedactOutputDev::paintTransparencyGroup(GfxState *pGState, double *pBBox)
{

}
void RedactOutputDev::setSoftMask(GfxState *pGState, double *pBBox, GBool bAlpha, Function *pTransferFunc, GfxColor *pBackdropColor)
{

}
void RedactOutputDev::clearSoftMask(GfxState *pGState)
{

}

bool SkipPath(const std::vector<CSegment>& arrForStroke, const CPoint& P1, const CPoint& P2)
{
	for (int i = 0; i < arrForStroke.size(); ++i)
	{
		CPoint P3 = arrForStroke[i].start;
		CPoint P4 = arrForStroke[i].end;
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
void RedactOutputDev::DrawPathRedact(Aggplus::CGraphicsPath* oPath, bool bStroke, const std::vector<CSegment>& arrForStroke)
{
	CMatrix oMatrix(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
	CMatrix oInverse = oMatrix.Inverse();

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
				m_pRenderer->m_oPath.MoveTo(dXCI, dYCI);
			}
			dXCur = dX3; dYCur = dY3;
			oInverse.Apply(dX3, dY3);
			m_pRenderer->m_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);

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
				m_pRenderer->m_oPath.MoveTo(dX, dY);
			}
		}
		else if (oPath->IsLinePoint(i))
		{
			double dX = points[i].X, dY = points[i].Y;
			if (bStroke && SkipPath(arrForStroke, CPoint(dXCur, dYCur), CPoint(dX, dY)))
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
				m_pRenderer->m_oPath.MoveTo(dXCI, dYCI);
			}
			dXCur = dX; dYCur = dY;
			oInverse.Apply(dX, dY);
			m_pRenderer->m_oPath.LineTo(dX, dY);
		}
		else if (oPath->IsClosePoint(i))
		{
			if (bStroke && (std::abs(dXCur - dXStart) > EPS || std::abs(dYCur - dYStart) > EPS) && SkipPath(arrForStroke, CPoint(dXCur, dYCur), CPoint(dXStart, dYStart)))
			{
				dXCur = dXStart; dYCur = dYStart;
				bBreak = true;
				continue;
			}
			if (bStroke || bBreak)
			{
				if (std::abs(dXCur - dXStart) > EPS || std::abs(dYCur - dYStart) > EPS)
				{
					bBreak = false;
					double dXCI = dXCur, dYCI = dYCur;
					oInverse.Apply(dXCI, dYCI);
					double dXSI = dXStart, dYSI = dYStart;
					oInverse.Apply(dXSI, dYSI);
					m_pRenderer->m_oPath.MoveTo(dXCI, dYCI);
					m_pRenderer->m_oPath.LineTo(dXSI, dYSI);
				}
			}
			else
				m_pRenderer->m_oPath.Close();
		}
	}
}
void RedactOutputDev::DoPathRedact(GfxState* pGState, GfxPath* pPath, double* pCTM, bool bStroke, bool bEoFill)
{
	double arrMatrix[6];
	arrMatrix[0] = pCTM[0];
	arrMatrix[1] = pCTM[1];
	arrMatrix[2] = pCTM[2];
	arrMatrix[3] = pCTM[3];
	arrMatrix[4] = pCTM[4];
	arrMatrix[5] = pCTM[5];

	double dShiftX = 0, dShiftY = 0;
	DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
	m_pRenderer->m_oPath.Clear();

	CMatrix oMatrix(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
	CMatrix oInverse = oMatrix.Inverse();

	std::vector<CSegment> arrForStroke;
	Aggplus::CGraphicsPath oPath, oPathResult;
	if (bEoFill)
		oPath.SetRuler(true);

	if (bStroke)
	{
		std::vector<TRect> rectangles;
		for (int i = 0; i < m_arrQuadPoints.size(); i += 4)
		{
			arrForStroke.push_back(CSegment(CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]), CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 3])));
			arrForStroke.push_back(CSegment(CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 3]), CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3])));
			arrForStroke.push_back(CSegment(CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]), CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 1])));
			arrForStroke.push_back(CSegment(CPoint(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 1]), CPoint(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1])));

			rectangles.push_back({m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 3], m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 1]});
		}

		for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
		{
			GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
			int nPointsCount = pSubpath->getNumPoints();

			double dX = pSubpath->getX(0), dY = pSubpath->getY(0);
			oMatrix.Apply(dX, dY);
			double dXStart = dX, dYStart = dY, dXCur = dX, dYCur = dY;

			int nCurPointIndex = 1;
			while (nCurPointIndex < nPointsCount)
			{
				if (pSubpath->getCurve(nCurPointIndex))
				{
					dX = pSubpath->getX(nCurPointIndex);
					dY = pSubpath->getY(nCurPointIndex);
					oMatrix.Apply(dX, dY);
					double dX2 = pSubpath->getX(nCurPointIndex + 1);
					double dY2 = pSubpath->getY(nCurPointIndex + 1);
					oMatrix.Apply(dX2, dY2);
					double dX3 = pSubpath->getX(nCurPointIndex + 2);
					double dY3 = pSubpath->getY(nCurPointIndex + 2);
					oMatrix.Apply(dX3, dY3);
					nCurPointIndex += 3;

					oPath.StartFigure();
					oPath.MoveTo(dXCur, dYCur);
					oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
					oPath.CloseFigure();

					arrForStroke.push_back(CSegment(CPoint(dXCur, dYCur), CPoint(dX3, dY3)));
					dXCur = dX3, dYCur = dY3;

					oPathResult = Aggplus::CalcBooleanOperation(oPath, m_oPathRedact, Aggplus::BooleanOpType::Subtraction);
					DrawPathRedact(&oPathResult, bStroke, arrForStroke);
					oPathResult.Reset(); oPath.Reset();
				}
				else
				{
					dX = pSubpath->getX(nCurPointIndex);
					dY = pSubpath->getY(nCurPointIndex);
					oMatrix.Apply(dX, dY);
					++nCurPointIndex;

					CLineClipper clipper(rectangles);
					CSegment line(CPoint(dXCur, dYCur), CPoint(dX, dY));
					dXCur = dX; dYCur = dY;

					auto visibleSegments = clipper.getVisibleSegments(line);
					for (int i = 0; i < visibleSegments.size(); ++i)
					{
						double dX1 = visibleSegments[i].start.x, dY1 = visibleSegments[i].start.y;
						double dX2 = visibleSegments[i].end.x,   dY2 = visibleSegments[i].end.y;
						oInverse.Apply(dX1, dY1);
						m_pRenderer->m_oPath.MoveTo(dX1, dY1);
						oInverse.Apply(dX2, dY2);
						m_pRenderer->m_oPath.LineTo(dX2, dY2);
					}
				}
			}
			if (pSubpath->isClosed())
			{
				CLineClipper clipper(rectangles);
				CSegment line(CPoint(dXCur, dYCur), CPoint(dXStart, dYStart));
				auto visibleSegments = clipper.getVisibleSegments(line);
				for (int i = 0; i < visibleSegments.size(); ++i)
				{
					double dX1 = visibleSegments[i].start.x, dY1 = visibleSegments[i].start.y;
					double dX2 = visibleSegments[i].end.x,   dY2 = visibleSegments[i].end.y;
					oInverse.Apply(dX1, dY1);
					m_pRenderer->m_oPath.MoveTo(dX1, dY1);
					oInverse.Apply(dX2, dY2);
					m_pRenderer->m_oPath.LineTo(dX2, dY2);
				}
			}
		}
	}
	else
	{
		for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
		{
			GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
			int nPointsCount = pSubpath->getNumPoints();

			oPath.StartFigure();

			double dX = pSubpath->getX(0), dY = pSubpath->getY(0);
			oMatrix.Apply(dX, dY);
			oPath.MoveTo(dX, dY);

			int nCurPointIndex = 1;
			while (nCurPointIndex < nPointsCount)
			{
				if (pSubpath->getCurve(nCurPointIndex))
				{
					dX = pSubpath->getX(nCurPointIndex);
					dY = pSubpath->getY(nCurPointIndex);
					oMatrix.Apply(dX, dY);
					double dX2 = pSubpath->getX(nCurPointIndex + 1);
					double dY2 = pSubpath->getY(nCurPointIndex + 1);
					oMatrix.Apply(dX2, dY2);
					double dX3 = pSubpath->getX(nCurPointIndex + 2);
					double dY3 = pSubpath->getY(nCurPointIndex + 2);
					oMatrix.Apply(dX3, dY3);
					oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
					nCurPointIndex += 3;
				}
				else
				{
					dX = pSubpath->getX(nCurPointIndex);
					dY = pSubpath->getY(nCurPointIndex);
					oMatrix.Apply(dX, dY);
					oPath.LineTo(dX, dY);
					++nCurPointIndex;
				}
			}
			// if (pSubpath->isClosed()) Принудительное замыкание фигур для CGraphicsPath
				oPath.CloseFigure();
		}

		oPathResult = Aggplus::CalcBooleanOperation(oPath, m_oPathRedact, Aggplus::BooleanOpType::Subtraction);
		DrawPathRedact(&oPathResult, bStroke);
	}
}
void RedactOutputDev::DoPath(GfxState* pGState, GfxPath* pPath, double* pCTM)
{
	double arrMatrix[6];
	arrMatrix[0] = pCTM[0];
	arrMatrix[1] = pCTM[1];
	arrMatrix[2] = pCTM[2];
	arrMatrix[3] = pCTM[3];
	arrMatrix[4] = pCTM[4];
	arrMatrix[5] = pCTM[5];

	double dShiftX = 0, dShiftY = 0;
	DoTransform(arrMatrix, &dShiftX, &dShiftY, true);

	m_pRenderer->m_oPath.Clear();

	for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
	{
		GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
		int nPointsCount = pSubpath->getNumPoints();

		double dX = pSubpath->getX(0), dY = pSubpath->getY(0);
		m_pRenderer->m_oPath.MoveTo(dX, dY);

		int nCurPointIndex = 1;
		while (nCurPointIndex < nPointsCount)
		{
			if (pSubpath->getCurve(nCurPointIndex))
			{
				dX = pSubpath->getX(nCurPointIndex);
				dY = pSubpath->getY(nCurPointIndex);
				double dX2 = pSubpath->getX(nCurPointIndex + 1);
				double dY2 = pSubpath->getY(nCurPointIndex + 1);
				double dX3 = pSubpath->getX(nCurPointIndex + 2);
				double dY3 = pSubpath->getY(nCurPointIndex + 2);
				m_pRenderer->m_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
				nCurPointIndex += 3;
			}
			else
			{
				dX = pSubpath->getX(nCurPointIndex);
				dY = pSubpath->getY(nCurPointIndex);
				m_pRenderer->m_oPath.LineTo(dX, dY);
				++nCurPointIndex;
			}
		}
		if (pSubpath->isClosed())
			m_pRenderer->m_oPath.Close();
	}
}
void RedactOutputDev::DoTransform(double* pMatrix, double* pdShiftX, double* pdShiftY, bool bActual)
{
	if (1 == pMatrix[0] && 0 == pMatrix[1] && 0 == pMatrix[2] && 1 == pMatrix[3] && !bActual)
	{
		if (pMatrix[4] || pMatrix[5])
		{
			*pdShiftX = pMatrix[4];
			*pdShiftY = pMatrix[5];
		}
		m_pRenderer->ResetTransform();
		m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
		m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
		m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;
	}
	else if (m_arrMatrix[0] == pMatrix[0] && m_arrMatrix[1] == pMatrix[1] && m_arrMatrix[2] == pMatrix[2] && m_arrMatrix[3] == pMatrix[3]
			 && m_arrMatrix[4] == pMatrix[4] && m_arrMatrix[5] == pMatrix[5] && !bActual)
	{
		double dIDet = 1 / (pMatrix[0] * pMatrix[3] - pMatrix[1] * pMatrix[2]);
		*pdShiftX = ((pMatrix[4] - m_arrMatrix[4]) * m_arrMatrix[3] - (pMatrix[5] - m_arrMatrix[5]) * m_arrMatrix[1]) * dIDet;
		*pdShiftY = ((pMatrix[5] - m_arrMatrix[5]) * m_arrMatrix[0] - (pMatrix[4] - m_arrMatrix[4]) * m_arrMatrix[2]) * dIDet;
	}
	else
	{
		m_pRenderer->SetTransform(pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3], pMatrix[4], pMatrix[5]);
		m_arrMatrix[0] = pMatrix[0]; m_arrMatrix[1] = pMatrix[1];
		m_arrMatrix[2] = pMatrix[2]; m_arrMatrix[3] = pMatrix[3];
		m_arrMatrix[4] = pMatrix[4]; m_arrMatrix[5] = pMatrix[5];
	}
}
void RedactOutputDev::DrawPath(const LONG& lType)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	bool bStroke = lType & c_nStroke;
	bool bFill   = lType & c_nWindingFillMode;
	bool bEoFill = lType & c_nEvenOddFillMode;

	m_pPage->GrSave();
	UpdateTransform();
	m_pRenderer->m_oPath.Draw(m_pPage, bStroke, bFill, bEoFill);
	m_pPage->GrRestore();
}
void RedactOutputDev::UpdateTransform()
{
	m_pPage->SetTransform(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
}
void RedactOutputDev::AddClip(GfxState* pGState, GfxRedactState* pState, int nIndex)
{
	m_pRenderer->m_oCommandManager.Flush();
	DoStateOp();

	GfxClip* pClip = pState->m_pClip;
	GfxPath* pPath = pClip->GetPath(nIndex);
	int  nClipFlag = pClip->GetClipFlag(nIndex);;

	m_pRenderer->m_lClipMode = nClipFlag;
	GfxClipMatrix oMatrix = pClip->m_vMatrix[nIndex];
	double pMatrix[6] = { oMatrix.dA, oMatrix.dB, oMatrix.dC, oMatrix.dD, oMatrix.dE, oMatrix.dF };
	DoPath(pGState, pPath, pMatrix);
	UpdateTransform();
	m_pRenderer->m_oPath.Clip(m_pPage, c_nClipRegionTypeEvenOdd & m_pRenderer->m_lClipMode);
	m_pRenderer->m_oPath.Clear();
}
void RedactOutputDev::DoStateOp()
{
	if (m_sStates.empty() || m_sStates.back().m_arrOp.empty())
		return;

	CStream* pStream = m_pPage->GetStream();
	for (int i = 0; i < m_sStates.back().m_arrOp.size(); ++i)
	{
		auto arrOp = m_sStates.back().m_arrOp;
		pStream->WriteStr(arrOp[i].first.c_str());
		pStream->WriteChar(' ');
		pStream->WriteStr(arrOp[i].second.c_str());
		pStream->WriteStr("\012");
	}
	m_sStates.back().m_arrOp.clear();
}
}
