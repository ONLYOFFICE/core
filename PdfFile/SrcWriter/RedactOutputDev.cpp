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

#include "../lib/xpdf/GfxFont.h"
#include "../lib/xpdf/XRef.h"
#include "../lib/pathkit/include/core/SkPath.h"
#include "../lib/pathkit/include/pathops/SkPathOps.h"

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

	m_nRI = 1;
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
	m_sStates.push_back(GfxRedactState());
	updateAll(pGState);
}
void RedactOutputDev::restoreState(GfxState *pGState)
{
	updateAll(pGState);

	if (m_sStates.empty())
		return; // Несбалансированный q/Q - сломанный файл
	m_sStates.pop_back();
}
//----- update graphics state
void RedactOutputDev::updateAll(GfxState *pGState)
{
	updateLineDash(pGState);
	updateFlatness(pGState);
	updateLineJoin(pGState);
	updateLineCap(pGState);
	updateMiterLimit(pGState);
	updateLineWidth(pGState);
	updateStrokeAdjust(pGState);
	updateFillColorSpace(pGState);
	updateFillColor(pGState);
	updateStrokeColorSpace(pGState);
	updateStrokeColor(pGState);
	updateRenderingIntent(pGState);
	// updateBlendMode(pGState);
	// updateFillOpacity(pGState);
	// updateStrokeOpacity(pGState);
	updateFont(pGState);
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
}
void RedactOutputDev::updateFlatness(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetFlatness(pGState->getFlatness());
}
void RedactOutputDev::updateLineJoin(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetJoinStyle(pGState->getLineJoin());
}
void RedactOutputDev::updateLineCap(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetStartCapStyle(pGState->getLineCap());
}
void RedactOutputDev::updateMiterLimit(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetMiter(pGState->getMiterLimit());
}
void RedactOutputDev::updateLineWidth(GfxState *pGState)
{
	m_pRenderer->m_oPen.SetSize(pGState->getLineWidth());
}
void RedactOutputDev::updateFillColorSpace(GfxState *pGState)
{

}
void RedactOutputDev::updateStrokeColorSpace(GfxState *pGState)
{

}
void RedactOutputDev::updateFillColor(GfxState *pGState)
{
	GfxColorSpace* pColorSpace = pGState->getFillColorSpace();
	GfxColorSpaceMode eMode = pColorSpace->getMode();
	GfxColor* pColor = pGState->getFillColor();
	switch (eMode) {
	case csDeviceGray:
	{
		m_pRenderer->m_oBrush.SetDColor2(1, colToDbl(pColor->c[0]));
		break;
	}
	case csDeviceRGB:
	{
		m_pRenderer->m_oBrush.SetDColor2(3, colToDbl(pColor->c[0]), colToDbl(pColor->c[1]), colToDbl(pColor->c[2]));
		break;
	}
	case csDeviceCMYK:
	{
		m_pRenderer->m_oBrush.SetDColor2(4, colToDbl(pColor->c[0]), colToDbl(pColor->c[1]), colToDbl(pColor->c[2]), colToDbl(pColor->c[3]));
		break;
	}
	default:
		break;
	}
}
void RedactOutputDev::updateStrokeColor(GfxState *pGState)
{
	GfxColorSpace* pColorSpace = pGState->getStrokeColorSpace();
	GfxColorSpaceMode eMode = pColorSpace->getMode();
	GfxColor* pColor = pGState->getStrokeColor();
	switch (eMode) {
	case csDeviceGray:
	{
		m_pRenderer->m_oPen.SetDColor2(1, colToDbl(pColor->c[0]));
		break;
	}
	case csDeviceRGB:
	{
		m_pRenderer->m_oPen.SetDColor2(3, colToDbl(pColor->c[0]), colToDbl(pColor->c[1]), colToDbl(pColor->c[2]));
		break;
	}
	case csDeviceCMYK:
	{
		m_pRenderer->m_oPen.SetDColor2(4, colToDbl(pColor->c[0]), colToDbl(pColor->c[1]), colToDbl(pColor->c[2]), colToDbl(pColor->c[3]));
		break;
	}
	default:
		break;
	}
}
void RedactOutputDev::updateRenderingIntent(GfxState *pGState)
{
	GfxRenderingIntent eRI = pGState->getRenderingIntent();
	if (eRI != m_nRI)
	{
		m_nRI = eRI;
		switch (eRI)
		{
		case GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric:
			m_pPage->SetRenderingIntent(ERenderingIntent::RenderingIntent_AbsoluteColorimetric);
			break;
		case GfxRenderingIntent::gfxRenderingIntentRelativeColorimetric:
			m_pPage->SetRenderingIntent(ERenderingIntent::RenderingIntent_RelativeColorimetric);
			break;
		case GfxRenderingIntent::gfxRenderingIntentSaturation:
			m_pPage->SetRenderingIntent(ERenderingIntent::RenderingIntent_Saturation);
			break;
		case GfxRenderingIntent::gfxRenderingIntentPerceptual:
		default:
			m_pPage->SetRenderingIntent(ERenderingIntent::RenderingIntent_Perceptual);
			break;
		}
	}
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
	m_pRenderer->m_oFont.SetHorizontalScaling(pGState->getHorizScaling());
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
	DoPath(pGState, pGState->getPath(), pGState->getCTM(), NULL, true);
	DrawPath(c_nStroke);
	// TODO Нужно пересечь путь с областями Redact, результат записать и сделать stroke
}
void RedactOutputDev::fill(GfxState *pGState)
{
	DoPath(pGState, pGState->getPath(), pGState->getCTM());
	DrawPath(c_nWindingFillMode);
	// TODO Нужно пересечь путь с областями Redact, результат записать и сделать fill
}
void RedactOutputDev::eoFill(GfxState *pGState)
{
	// TODO Нужно пересечь путь с областями Redact, результат записать и сделать eoFill
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
}
void RedactOutputDev::eoClip(GfxState *pGState)
{

}
void RedactOutputDev::clipToStrokePath(GfxState *pGState)
{

}
//----- text drawing
void RedactOutputDev::beginStringOp(GfxState *pGState)
{

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
	double arrMatrix[6];

	arrMatrix[0] = pTm[0] * pCTM[0] + pTm[1] * pCTM[2];
	arrMatrix[1] = pTm[0] * pCTM[1] + pTm[1] * pCTM[3];
	arrMatrix[2] = pTm[2] * pCTM[0] + pTm[3] * pCTM[2];
	arrMatrix[3] = pTm[2] * pCTM[1] + pTm[3] * pCTM[3];
	arrMatrix[4] = pTm[4] * pCTM[0] + pTm[5] * pCTM[2] + pCTM[4];
	arrMatrix[5] = pTm[4] * pCTM[1] + pTm[5] * pCTM[3] + pCTM[5];

	if (arrMatrix[0] != m_arrMatrix[0] || arrMatrix[1] != m_arrMatrix[1] || arrMatrix[2] != m_arrMatrix[2] ||
		arrMatrix[3] != m_arrMatrix[3] || arrMatrix[4] != m_arrMatrix[4] || arrMatrix[5] != m_arrMatrix[5])
	{
		double dShiftX = 0, dShiftY = 0;
		DoTransform(arrMatrix, &dShiftX, &dShiftY, true);
	}

	for (int i = 0; i < m_arrQuadPoints.size(); i += 8)
	{
		// TODO нужно учитывать m_arrMatrix, так ведь?
		double xMin = m_arrQuadPoints[0];
		double yMin = m_arrQuadPoints[1];
		double xMax = m_arrQuadPoints[4];
		double yMax = m_arrQuadPoints[5];

		if (xMin < dX && dX < xMax && yMin < dY && dY < yMax)
			return;
	}

	BYTE* pCodes = new BYTE[2];
	pCodes[0] = (nCode >> 8) & 0xFF;
	pCodes[1] = nCode & 0xFF;

	m_pRenderer->m_oCommandManager.SetTransform(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);

	CRendererTextCommand* pText = m_pRenderer->m_oCommandManager.AddText(pCodes, 2, dX, dY);
	pText->SetName(m_pRenderer->m_oFont.GetName());
	pText->SetSize(m_pRenderer->m_oFont.GetSize());
	int nDColor2Size;
	double* dColor2 = m_pRenderer->m_oBrush.GetDColor2(nDColor2Size);
	pText->SetDColor2(nDColor2Size, dColor2[0], dColor2[1], dColor2[2], dColor2[3]);
	pText->SetAlpha((BYTE)m_pRenderer->m_oBrush.GetAlpha1()); // TODO
	pText->SetCharSpace(m_pRenderer->m_oFont.GetCharSpace());
	pText->SetMode(m_pRenderer->m_oFont.GetRenderMode());
	pText->SetRise(m_pRenderer->m_oFont.GetRise());
	pText->SetWordSpace(m_pRenderer->m_oFont.GetWordSpace());
	pText->SetHorScaling(m_pRenderer->m_oFont.GetHorizontalScaling());
	pText->SetWidth(dDx);
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
GBool RedactOutputDev::useExtGState()
{
	return gTrue;
}
void RedactOutputDev::setExtGState(GfxState *pGState, Object *pDict, const char* name)
{
	if (m_sStates.empty())
		return;
	m_sStates.back().m_arrExtGState.push_back(name);
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

	double dShiftX = 0, dShiftY = 0;
	DoTransform(pGState->getCTM(), &dShiftX, &dShiftY, true);

	// TODO пока что исключается всё изображение
	Object oForm;
	if (!m_pXref->fetch(id.num, id.gen, &oForm)->isStream())
	{
		oForm.free();
		return;
	}

	// TODO нужно учитывать Matrix у формы
	double dXmin = 0, dYmin = 0, dXmax = 0, dYmax = 0;
	Object oBBox;
	if (oForm.streamGetDict()->lookup("BBox", &oBBox)->isArray() && oBBox.arrayGetLength() == 4)
	{
		Object oNum;
		if (oBBox.arrayGet(0, &oNum)->isNum())
			dXmin = oNum.getNum();
		oNum.free();
		if (oBBox.arrayGet(1, &oNum)->isNum())
			dYmin = oNum.getNum();
		oNum.free();
		if (oBBox.arrayGet(2, &oNum)->isNum())
			dXmax = oNum.getNum();
		oNum.free();
		if (oBBox.arrayGet(3, &oNum)->isNum())
			dYmax = oNum.getNum();
		oNum.free();

		Transform(m_arrMatrix, dXmin, dYmin, &dXmin, &dYmin);
		Transform(m_arrMatrix, dXmax, dYmax, &dXmax, &dYmax);
	}
	oBBox.free();

	for (int i = 0; i < m_arrQuadPoints.size(); i += 8)
	{
		double xMin = m_arrQuadPoints[i + 0];
		double yMin = m_arrQuadPoints[i + 1];
		double xMax = m_arrQuadPoints[i + 4];
		double yMax = m_arrQuadPoints[i + 5];

		if (!(dXmax < xMin || dXmin > xMax || dYmax < yMin || dYmin > yMax))
			return;
	}
	oForm.free();

	m_pPage->GrSave();
	UpdateTransform();
	for (int i = 0; i < m_sStates.size(); ++i)
	{
		for (int j = 0; j < m_sStates[i].m_arrExtGState.size(); ++j)
		{
			m_pPage->SetExtGrStateKey(m_sStates[i].m_arrExtGState[j].c_str());
		}
	}
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

// Конвертирует conic в массив квадратичных Безье (минимум 1 сегмент)
void ConvertConicToQuads(const pk::SkPoint& p0, const pk::SkPoint& p1, const pk::SkPoint& p2, pk::SkScalar w, pk::SkPoint pts[], int pow2) {
	const pk::SkScalar k = 0.5f; // Фактор дробления (можно адаптировать)

	pk::SkPoint q0 = p0;
	pk::SkPoint q2 = p2;
	pk::SkPoint q1 = pk::SkPoint::Make(
		(p0.x() + w * p1.x() + p2.x()) / (2 + w),
		(p0.y() + w * p1.y() + p2.y()) / (2 + w)
	);

	if (pow2 == 1) {
		pts[0] = q0;
		pts[1] = q1;
		pts[2] = q2;
		return;
	}

	// Рекурсивное дробление
	pk::SkPoint left[3], right[3];
	pk::SkPoint mid = pk::SkPoint::Make(
		(q0.x() + q2.x() + 2 * w * q1.x()) / (2 + 2 * w),
		(q0.y() + q2.y() + 2 * w * q1.y()) / (2 + 2 * w)
	);

	ConvertConicToQuads(q0, q1, mid, w, left, pow2 - 1);
	ConvertConicToQuads(mid, q1, q2, w, right, pow2 - 1);

	// Слияние результатов
	std::copy(left, left + 3, pts);
	std::copy(right + 1, right + 3, pts + 3);
}
void RedactOutputDev::DoPath(GfxState* pGState, GfxPath* pPath, double* pCTM, GfxClipMatrix* pCTM2, bool bStroke)
{
	double arrMatrix[6];
	if (pCTM2)
	{
		arrMatrix[0] = pCTM2->dA;
		arrMatrix[1] = pCTM2->dB;
		arrMatrix[2] = pCTM2->dC;
		arrMatrix[3] = pCTM2->dD;
		arrMatrix[4] = pCTM2->dE;
		arrMatrix[5] = pCTM2->dF;
	}
	else
	{
		arrMatrix[0] = pCTM[0];
		arrMatrix[1] = pCTM[1];
		arrMatrix[2] = pCTM[2];
		arrMatrix[3] = pCTM[3];
		arrMatrix[4] = pCTM[4];
		arrMatrix[5] = pCTM[5];
	}

	double dShiftX = 0, dShiftY = 0;
	DoTransform(arrMatrix, &dShiftX, &dShiftY, true);

	CMatrix oMatrix(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
	CMatrix oInverse = oMatrix.Inverse();

	/*
	Aggplus::CGraphicsPath oPath, oPathRedact, oPathResult;
	for (int i = 0; i < m_arrQuadPoints.size(); i += 8)
	{
		oPathRedact.StartFigure();
		oPathRedact.MoveTo(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]);
		oPathRedact.LineTo(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]);
		oPathRedact.LineTo(m_arrQuadPoints[i + 4], m_arrQuadPoints[i + 5]);
		oPathRedact.LineTo(m_arrQuadPoints[i + 6], m_arrQuadPoints[i + 7]);
		oPathRedact.CloseFigure();
	}
	size_t length2 = oPathRedact.GetPointCount(), compound2 = oPathRedact.GetCloseCount();
	std::vector<Aggplus::PointD> points2 = oPathRedact.GetPoints(0, length2 + compound2);
	std::cout << "Path2:" <<std::endl;
	for (int i = 0; i < points2.size(); ++i)
	{
		std::cout << "( " << points2[i].X << ", " << points2[i].Y << " ); ";
	}
	std::cout <<std::endl;

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
		if (pSubpath->isClosed())
			oPath.CloseFigure();
	}

	size_t length1 = oPath.GetPointCount(), compound1 = oPath.GetCloseCount();
	std::vector<Aggplus::PointD> points1 = oPath.GetPoints(0, length1 + compound1);
	std::cout << "Path1:" <<std::endl;
	for (int i = 0; i < points1.size(); ++i)
	{
		std::cout << "( " << points1[i].X << ", " << points1[i].Y << " ); ";
	}
	std::cout <<std::endl;

	oPathResult = Aggplus::CalcBooleanOperation(oPath, oPathRedact, Aggplus::BooleanOpType::Subtraction);

	size_t length = oPathResult.GetPointCount(), compound = oPathResult.GetCloseCount();
	std::vector<Aggplus::PointD> points = oPathResult.GetPoints(0, length + compound);
	std::cout << "PathRES:" <<std::endl;
	for (int i = 0; i < points.size(); ++i)
	{
		std::cout << "( " << points[i].X << ", " << points[i].Y << " ); ";
	}
	std::cout <<std::endl;

	m_pRenderer->m_oPath.Clear();

	for (size_t i = 0; i < length + compound; i++)
	{
		if (oPathResult.IsCurvePoint(i))
		{
			double dX = points[i].X;
			double dY = points[i].Y;
			oInverse.Apply(dX, dY);
			double dX2 = points[i + 1].X;
			double dY2 = points[i + 1].Y;
			oInverse.Apply(dX2, dY2);
			double dX3 = points[i + 2].X;
			double dY3 = points[i + 2].Y;
			oInverse.Apply(dX3, dY3);
			m_pRenderer->m_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);

			i += 2;
		}
		else if (oPathResult.IsMovePoint(i))
		{
			double dX = points[i].X, dY = points[i].Y;
			oInverse.Apply(dX, dY);
			m_pRenderer->m_oPath.MoveTo(dX, dY);
		}
		else if (oPathResult.IsLinePoint(i))
		{
			double dX = points[i].X, dY = points[i].Y;
			oInverse.Apply(dX, dY);
			m_pRenderer->m_oPath.LineTo(dX, dY);
		}
		else if (oPathResult.IsClosePoint(i))
			m_pRenderer->m_oPath.Close();
	}

	return;
	*/

	pk::SkPath skPath, skPathRedact, skPathRes;
	for (int i = 0; i < m_arrQuadPoints.size(); i += 8)
	{
		skPathRedact.moveTo(m_arrQuadPoints[i + 0], m_arrQuadPoints[i + 1]);
		skPathRedact.lineTo(m_arrQuadPoints[i + 2], m_arrQuadPoints[i + 3]);
		skPathRedact.lineTo(m_arrQuadPoints[i + 4], m_arrQuadPoints[i + 5]);
		skPathRedact.lineTo(m_arrQuadPoints[i + 6], m_arrQuadPoints[i + 7]);
		skPathRedact.close();
	}

	for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
	{
		GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
		int nPointsCount = pSubpath->getNumPoints();

		double dX = pSubpath->getX(0), dY = pSubpath->getY(0);
		oMatrix.Apply(dX, dY);
		skPath.moveTo(dX, dY);

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
				skPath.cubicTo(dX, dY, dX2, dY2, dX3, dY3);
				nCurPointIndex += 3;
			}
			else
			{
				dX = pSubpath->getX(nCurPointIndex);
				dY = pSubpath->getY(nCurPointIndex);
				oMatrix.Apply(dX, dY);
				skPath.lineTo(dX, dY);
				++nCurPointIndex;
			}
		}
		if (pSubpath->isClosed())
			skPath.close();
	}

	pk::Op(skPath, skPathRedact, pk::SkPathOp::kDifference_SkPathOp, &skPathRes);

	pk::SkPath::Iter iter(skPathRes, false); // false - не сохранять контуры
	pk::SkPoint pts[4];
	pk::SkPath::Verb verb;

	m_pRenderer->m_oPath.Clear();

	while ((verb = iter.next(pts)) != pk::SkPath::kDone_Verb)
	{
		switch (verb)
		{
		case pk::SkPath::kMove_Verb:
		{
			double dX = pts[0].x(), dY = pts[0].y();
			oInverse.Apply(dX, dY);
			m_pRenderer->m_oPath.MoveTo(dX, dY);
			break;
		}
		case pk::SkPath::kLine_Verb:
		{
			double dX = pts[1].x(), dY = pts[1].y();
			oInverse.Apply(dX, dY);
			m_pRenderer->m_oPath.LineTo(dX, dY);
			break;
		}
		case pk::SkPath::kQuad_Verb:
		{
			// Конвертация квадратичной в кубическую кривую
			pk::SkPoint cubic[4];
			cubic[0] = pts[0];  // Начальная точка
			cubic[1] = {
				pts[0].x() + (2.0f/3.0f) * (pts[1].x() - pts[0].x()),
				pts[0].y() + (2.0f/3.0f) * (pts[1].y() - pts[0].y())
			};  // Первая контрольная точка
			cubic[2] = {
				pts[1].x() + (2.0f/3.0f) * (pts[2].x() - pts[1].x()),
				pts[1].y() + (2.0f/3.0f) * (pts[2].y() - pts[1].y())
			};  // Вторая контрольная точка
			cubic[3] = pts[2];  // Конечная точка

			double dX = cubic[1].x();
			double dY = cubic[1].y();
			oInverse.Apply(dX, dY);
			double dX2 = cubic[2].x();
			double dY2 = cubic[2].y();
			oInverse.Apply(dX2, dY2);
			double dX3 = cubic[3].x();
			double dY3 = cubic[3].y();
			oInverse.Apply(dX3, dY3);
			m_pRenderer->m_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
			break;
		}
		case pk::SkPath::kCubic_Verb:
		{
			double dX = pts[1].x();
			double dY = pts[1].y();
			oInverse.Apply(dX, dY);
			double dX2 = pts[2].x();
			double dY2 = pts[2].y();
			oInverse.Apply(dX2, dY2);
			double dX3 = pts[3].x();
			double dY3 = pts[3].y();
			oInverse.Apply(dX3, dY3);
			m_pRenderer->m_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
			break;
		}
		case pk::SkPath::kConic_Verb:
		{
			const int pow2 = 1; // Кол-во сегментов (2^pow2). Для PDF обычно хватает 1.
			pk::SkPoint quadPts[1 + 2 * (1 << pow2)]; // Массив для квадратичных кривых
			ConvertConicToQuads(pts[0], pts[1], pts[2], iter.conicWeight(), quadPts, pow2);

			// Добавляем каждую квадратичную кривую как отдельный сегмент
			for (int i = 0; i < (1 << pow2); ++i)
			{
				// Конвертация квадратичной в кубическую кривую
				pk::SkPoint cubic[4];
				cubic[0] = quadPts[0 + 2*i];  // Начальная точка
				cubic[1] = {
					quadPts[0 + 2*i].x() + (2.0f/3.0f) * (quadPts[1 + 2*i].x() - quadPts[0 + 2*i].x()),
					quadPts[0 + 2*i].y() + (2.0f/3.0f) * (quadPts[1 + 2*i].y() - quadPts[0 + 2*i].y())
				};  // Первая контрольная точка
				cubic[2] = {
					quadPts[1 + 2*i].x() + (2.0f/3.0f) * (quadPts[2 + 2*i].x() - quadPts[1 + 2*i].x()),
					quadPts[1 + 2*i].y() + (2.0f/3.0f) * (quadPts[2 + 2*i].y() - quadPts[1 + 2*i].y())
				};  // Вторая контрольная точка
				cubic[3] = quadPts[2 + 2*i];  // Конечная точка

				double dX = cubic[1].x();
				double dY = cubic[1].y();
				oInverse.Apply(dX, dY);
				double dX2 = cubic[2].x();
				double dY2 = cubic[2].y();
				oInverse.Apply(dX2, dY2);
				double dX3 = cubic[3].x();
				double dY3 = cubic[3].y();
				oInverse.Apply(dX3, dY3);

				m_pRenderer->m_oPath.CurveTo(dX, dY, dX2, dY2, dX3, dY3);
			}
			break;
		}
		case pk::SkPath::kClose_Verb:
		{
			m_pRenderer->m_oPath.Close();
			break;
		}
		case pk::SkPath::kDone_Verb:
			break;
		}
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

	bool bStroke = lType & c_nStroke;
	bool bFill   = lType & c_nWindingFillMode;
	bool bEoFill = lType & c_nEvenOddFillMode;

	m_pPage->GrSave();
	UpdateTransform();

	if (bStroke)
		UpdatePen();

	for (int i = 0; i < m_sStates.size(); ++i)
	{
		for (int j = 0; j < m_sStates[i].m_arrExtGState.size(); ++j)
		{
			m_pPage->SetExtGrStateKey(m_sStates[i].m_arrExtGState[j].c_str());
		}
	}

	std::wstring sTextureOldPath = L"";
	std::wstring sTextureTmpPath = L"";

	if (bFill || bEoFill)
	{
		if (c_BrushTypeTexture == m_pRenderer->m_oBrush.GetType())
		{
			sTextureOldPath = m_pRenderer->m_oBrush.GetTexturePath();
			//sTextureTmpPath = GetDownloadFile(sTextureOldPath, wsTempDirectory);

			if (!sTextureTmpPath.empty())
				m_pRenderer->m_oBrush.SetTexturePath(sTextureTmpPath);
		}

		UpdateBrush(NULL, L"");
	}

	if (!m_pRenderer->m_pShading)
	{
		m_pRenderer->m_oPath.Draw(m_pPage, bStroke, bFill, bEoFill);
	}
	else
	{
		if (bFill || bEoFill)
		{
			m_pPage->GrSave();
			m_pRenderer->m_oPath.Clip(m_pPage, bEoFill);

			if (NULL != m_pRenderer->m_pShadingExtGrState)
				m_pPage->SetExtGrState(m_pRenderer->m_pShadingExtGrState);

			m_pPage->DrawShading(m_pRenderer->m_pShading);
			m_pPage->GrRestore();
		}

		if (bStroke)
			m_pRenderer->m_oPath.Draw(m_pPage, bStroke, false, false);
	}

	m_pPage->GrRestore();

	if (!sTextureTmpPath.empty())
		m_pRenderer->m_oBrush.SetTexturePath(sTextureOldPath);
}
void RedactOutputDev::UpdateTransform()
{
	m_pPage->SetTransform(m_arrMatrix[0], m_arrMatrix[1], m_arrMatrix[2], m_arrMatrix[3], m_arrMatrix[4], m_arrMatrix[5]);
}
void RedactOutputDev::UpdatePen()
{
	int nSize = -1;
	double* dColor = m_pRenderer->m_oPen.GetDColor2(nSize);
	if (nSize == 1)
		m_pPage->SetStrokeG(dColor[0]);
	else if (nSize == 3)
		m_pPage->SetStrokeRGB(dColor[0], dColor[1], dColor[2]);
	else if (nSize == 4)
		m_pPage->SetStrokeCMYK(dColor[0], dColor[1], dColor[2], dColor[3]);

	// TODO ExtGState
	//m_pPage->SetStrokeAlpha((unsigned char)m_pRenderer->m_oPen.GetAlpha());
	m_pPage->SetLineWidth(m_pRenderer->m_oPen.GetSize());

	LONG lDashStyle = m_pRenderer->m_oPen.GetDashStyle();
	if (Aggplus::DashStyleSolid == lDashStyle)
	{
		// Ничего не делаем
	}
	else if (Aggplus::DashStyleCustom == lDashStyle)
	{
		LONG lDashCount = 0;
		double* pDashPattern = m_pRenderer->m_oPen.GetDashPattern(lDashCount);
		m_pPage->SetDash(pDashPattern, lDashCount, m_pRenderer->m_oPen.GetDashOffset());
	}

	BYTE nLineCap = m_pRenderer->m_oPen.GetStartCapStyle();
	if (nLineCap == 0)
		m_pPage->SetLineCap(ELineCapStyle::linecap_Butt);
	else if (nLineCap == 1)
		m_pPage->SetLineCap(ELineCapStyle::linecap_Round);
	else if (nLineCap == 2)
		m_pPage->SetLineCap(ELineCapStyle::linecap_ProjectingSquare);

	BYTE nLineJoin = m_pRenderer->m_oPen.GetJoinStyle();
	if (nLineJoin == 0)
	{
		m_pPage->SetLineJoin(ELineJoinStyle::linejoin_Miter);
		m_pPage->SetMiterLimit(m_pRenderer->m_oPen.GetMiter());
	}
	else if (nLineJoin == 1)
		m_pPage->SetLineJoin(ELineJoinStyle::linejoin_Round);
	else if (nLineJoin == 2)
		m_pPage->SetLineJoin(ELineJoinStyle::linejoin_Bevel);

	double dFlatness = m_pRenderer->m_oPen.GetFlatness();
	if (dFlatness > 0.0001)
		m_pPage->SetFlat(dFlatness);
}
void RedactOutputDev::UpdateBrush(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory)
{
	m_pRenderer->m_pShading = NULL;
	m_pRenderer->m_pShadingExtGrState = NULL;

	LONG lBrushType = m_pRenderer->m_oBrush.GetType();
	/*
	if (c_BrushTypeTexture == lBrushType)
	{
		std::wstring wsTexturePath = m_oBrush.GetTexturePath();
		BYTE nAlpha = m_oBrush.GetTextureAlpha();
		CImageFileFormatChecker oImageFormat(wsTexturePath);

		PdfWriter::CImageDict* pImage = NULL;
		int nImageW = 0;
		int nImageH = 0;
		bool bHasImage = false;
		if (m_pDocument->HasImage(wsTexturePath, nAlpha))
		{
			pImage = m_pDocument->GetImage(wsTexturePath, nAlpha);
			nImageH = pImage->GetHeight();
			nImageW = pImage->GetWidth();
			bHasImage = true;
		}
		else if (_CXIMAGE_FORMAT_JPG == oImageFormat.eFileType || _CXIMAGE_FORMAT_JP2 == oImageFormat.eFileType)
		{
			pImage = m_pDocument->CreateImage();
			CBgraFrame oFrame;
			oFrame.OpenFile(wsTexturePath);
			nImageH = oFrame.get_Height();
			nImageW = oFrame.get_Width();

			if (pImage)
			{
				if (_CXIMAGE_FORMAT_JPG == oImageFormat.eFileType)
					pImage->LoadJpeg(wsTexturePath.c_str(), nImageW, nImageH, oFrame.IsGrayScale());
				else
					pImage->LoadJpx(wsTexturePath.c_str(), nImageW, nImageH);

				m_pDocument->AddImage(wsTexturePath, nAlpha, pImage);
			}
		}
		else if (_CXIMAGE_FORMAT_WMF == oImageFormat.eFileType ||
				 _CXIMAGE_FORMAT_EMF == oImageFormat.eFileType ||
				 _CXIMAGE_FORMAT_SVM == oImageFormat.eFileType ||
				 _CXIMAGE_FORMAT_SVG == oImageFormat.eFileType)
		{
			// TODO: Реализовать отрисовку метафайлов по-нормальному
			MetaFile::IMetaFile* pMeta = MetaFile::Create(pAppFonts);
			pMeta->LoadFromFile(wsTexturePath.c_str());

			double dL, dR, dT, dB;
			m_oPath.GetBounds(dL, dT, dR, dB);

			double dW = 300.0 * (dR - dL) / 72;
			if (dW < 0) dW = -dW;
			double dH = 300.0 * (dB - dT) / 72;
			if (dH < 0) dH = -dH;

			if (dW < 1) dW = 1;
			if (dH < 1) dH = 1;

			double dMax = 2000;
			double dMin = 10;
			if (dW > dMax || dH > dMax)
			{
				double dMaxSrc = (dW > dH) ? dW : dH;
				dW *= (dMax / dMaxSrc);
				dH *= (dMax / dMaxSrc);
			}

			if (dW < dMin) dW = dMin;
			if (dH < dMin) dH = dMin;

			std::wstring wsTempFile = GetTempFile(wsTempDirectory);
			pMeta->ConvertToRaster(wsTempFile.c_str(), _CXIMAGE_FORMAT_PNG, (int)dW, (int)dH);

			RELEASEOBJECT(pMeta);

			Aggplus::CImage oImage(wsTempFile);
			nImageW = abs((int)oImage.GetWidth());
			nImageH = abs((int)oImage.GetHeight());
			pImage = LoadImage(&oImage, 255);
			m_pDocument->AddImage(wsTexturePath, nAlpha, pImage);
		}
		else
		{
			Aggplus::CImage oImage(wsTexturePath);
			nImageW = abs((int)oImage.GetWidth());
			nImageH = abs((int)oImage.GetHeight());
			pImage = LoadImage(&oImage, 255);
			m_pDocument->AddImage(wsTexturePath, nAlpha, pImage);
		}

		if (pImage)
		{
			if (0xFF != nAlpha && !bHasImage)
				pImage->AddTransparency(nAlpha);

			LONG lTextureMode = m_oBrush.GetTextureMode();

			double dW = 10;
			double dH = 10;

			double dL, dR, dT, dB;
			CBrushState::TBrushRect& oRect = m_oBrush.GetBrushRect();
			if (!oRect.bUse)
			{
				m_oPath.GetBounds(dL, dT, dR, dB);
			}
			else
			{
				dL = MM_2_PT(oRect.dLeft);
				dB = MM_2_PT(m_dPageHeight - oRect.dTop);
				dR = MM_2_PT(oRect.dLeft + oRect.dWidth);
				dT = MM_2_PT(m_dPageHeight - oRect.dTop - oRect.dHeight);
			}

			double dXStepSpacing = 0, dYStepSpacing = 0;
			if (c_BrushTextureModeStretch == lTextureMode)
			{
				// Растягиваем картинку по размерам пата
				dW = std::max(10.0, dR - dL);
				dH = std::max(10.0, dB - dT);

				// Чтобы избавиться от погрешностей из-за которых могут возникать полоски или обрезание картинки,
				// удвоим расстрояние между соседними тайлами. Плохого тут нет, т.к. нам нужен всего 1 тайл
				dXStepSpacing = dW;
				dYStepSpacing = dH;
			}
			else
			{
				// Размеры картинки заданы в пикселях. Размеры тайла - это размеры картинки в пунктах.
				dW = (double)nImageW * 72.0 / 96.0;
				dH = (double)nImageH * 72.0 / 96.0;

				dT = dB;
			}

			// Нам нужно, чтобы левый нижний угол границ нашего пата являлся точкой переноса для матрицы преобразования.
			PdfWriter::CMatrix* pMatrix = m_pPage->GetTransform();
			pMatrix->Apply(dL, dT);
			PdfWriter::CMatrix oPatternMatrix = *pMatrix;
			oPatternMatrix.x = dL;
			oPatternMatrix.y = dT;
			m_pPage->SetPatternColorSpace(m_pDocument->CreateImageTilePattern(dW, dH, pImage, &oPatternMatrix, PdfWriter::imagetilepatterntype_Default, dXStepSpacing, dYStepSpacing));
		}
	}
	else if (c_BrushTypeHatch1 == lBrushType)
	{
		std::wstring wsHatchType = m_oBrush.GetTexturePath();

		double dW = 8 * 72 / 96;
		double dH = 8 * 72 / 96;

		TColor oColor1 = m_oBrush.GetTColor1();
		TColor oColor2 = m_oBrush.GetTColor2();
		BYTE nAlpha1 = (BYTE)m_oBrush.GetAlpha1();
		BYTE nAlpha2 = (BYTE)m_oBrush.GetAlpha2();

		m_pPage->SetPatternColorSpace(m_pDocument->CreateHatchPattern(dW, dH, oColor1.r, oColor1.g, oColor1.b, nAlpha1, oColor2.r, oColor2.g, oColor2.b, nAlpha2, wsHatchType));
	}
	else if (c_BrushTypeRadialGradient == lBrushType || c_BrushTypeLinearGradient == lBrushType)
	{
		TColor* pGradientColors;
		double* pPoints;
		LONG lCount;

		m_oBrush.GetGradientColors(pGradientColors, pPoints, lCount);

		if (lCount > 0)
		{
			unsigned char* pColors = new unsigned char[3 * lCount];
			unsigned char* pAlphas = new unsigned char[lCount];
			if (pColors)
			{
				for (LONG lIndex = 0; lIndex < lCount; lIndex++)
				{
					pColors[3 * lIndex + 0] = pGradientColors[lIndex].r;
					pColors[3 * lIndex + 1] = pGradientColors[lIndex].g;
					pColors[3 * lIndex + 2] = pGradientColors[lIndex].b;
					pAlphas[lIndex]         = pGradientColors[lIndex].a;
				}

				if (c_BrushTypeLinearGradient == lBrushType)
				{
					double dX0, dY0, dX1, dY1;
					m_oBrush.GetLinearGradientPattern(dX0, dY0, dX1, dY1);
					m_pShading = m_pDocument->CreateAxialShading(m_pPage, MM_2_PT(dX0), MM_2_PT(m_dPageHeight - dY0), MM_2_PT(dX1), MM_2_PT(m_dPageHeight - dY1), pColors, pAlphas, pPoints, lCount, m_pShadingExtGrState);
				}
				else //if (c_BrushTypeRadialGradient == lBrushType)
				{
					double dX0, dY0, dR0, dX1, dY1, dR1;
					m_oBrush.GetRadialGradientPattern(dX0, dY0, dR0, dX1, dY1, dR1);
					m_pShading = m_pDocument->CreateRadialShading(m_pPage, MM_2_PT(dX0), MM_2_PT(m_dPageHeight - dY0), MM_2_PT(dR0), MM_2_PT(dX1), MM_2_PT(m_dPageHeight - dY1), MM_2_PT(dR1), pColors, pAlphas, pPoints, lCount, m_pShadingExtGrState);
				}
				delete[] pColors;
				delete[] pAlphas;
			}
		}
	}
	*/
	// else// if (c_BrushTypeSolid == lBrushType)
	{
		int nSize = -1;
		double* dColor = m_pRenderer->m_oBrush.GetDColor2(nSize);
		if (nSize == 1)
			m_pPage->SetFillG(dColor[0]);
		else if (nSize == 3)
			m_pPage->SetFillRGB(dColor[0], dColor[1], dColor[2]);
		else if (nSize == 4)
			m_pPage->SetFillCMYK(dColor[0], dColor[1], dColor[2], dColor[3]);

		// TODO ExtGState
		// m_pPage->SetFillAlpha((unsigned char)m_pRenderer->m_oBrush.GetAlpha1());
	}
}
}
