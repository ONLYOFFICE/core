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

namespace PdfWriter
{
//----- constructor/destructor
RedactOutputDev::RedactOutputDev(CPdfWriter* pRenderer)
{
	m_pRenderer = pRenderer;
	m_pDoc = m_pRenderer->GetDocument();
	m_pPage = NULL;
}
RedactOutputDev::~RedactOutputDev()
{
	m_pRenderer = NULL;
	m_pDoc = NULL;
	m_pPage = NULL;
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
	m_pPage = m_pDoc->GetEditPage(nPageIndex);
	m_pRenderer->EditPage(m_pPage);
	m_pDoc->SetCurPage(m_pPage);
	// TODO опасная операция. Удаляет из xref прошлые объекты Contents, но не проверяет всевозможные CProxyObject которые могут ссылаться на удаленные объекты
	// Однако предполагается, что ссылка на стримы страниц есть только в Contents одной конкретной страницы
	// И всё же требуется доработка для безопасного удаления
	m_pDoc->ClearPageFull();
}
void RedactOutputDev::endPage()
{
	m_pPage = NULL;
}
//----- save/restore graphics state
void RedactOutputDev::saveState(GfxState *pGState)
{
	m_pPage->GrSave();
}
void RedactOutputDev::restoreState(GfxState *pGState)
{
	m_pPage->GrRestore();
}
//----- update graphics state
void RedactOutputDev::updateAll(GfxState *pGState)
{
}
void RedactOutputDev::updateCTM(GfxState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32)
{
	m_pPage->Concat(dMatrix11, dMatrix12, dMatrix21, dMatrix22, dMatrix31, dMatrix32);
}
void RedactOutputDev::updateLineDash(GfxState *pGState)
{
	double* pDash  = NULL;
	int     nSize  = 0;
	double  dStart = 0;
	pGState->getLineDash(&pDash, &nSize, &dStart);
	m_pPage->SetDash(pDash, nSize, dStart);
}
void RedactOutputDev::updateFlatness(GfxState *pGState)
{
	m_pPage->SetFlat(pGState->getFlatness());
}
void RedactOutputDev::updateLineJoin(GfxState *pGState)
{
	int nLineJoin = pGState->getLineJoin();
	if (nLineJoin == 0)
		m_pPage->SetLineJoin(ELineJoinStyle::linejoin_Miter);
	else if (nLineJoin == 1)
		m_pPage->SetLineJoin(ELineJoinStyle::linejoin_Round);
	else if (nLineJoin == 2)
		m_pPage->SetLineJoin(ELineJoinStyle::linejoin_Bevel);
}
void RedactOutputDev::updateLineCap(GfxState *pGState)
{
	int nLineCap = pGState->getLineCap();
	if (nLineCap == 0)
		m_pPage->SetLineCap(ELineCapStyle::linecap_Butt);
	else if (nLineCap == 1)
		m_pPage->SetLineCap(ELineCapStyle::linecap_Round);
	else if (nLineCap == 2)
		m_pPage->SetLineCap(ELineCapStyle::linecap_ProjectingSquare);
}
void RedactOutputDev::updateMiterLimit(GfxState *pGState)
{
	m_pPage->SetMiterLimit(pGState->getMiterLimit());
}
void RedactOutputDev::updateLineWidth(GfxState *pGState)
{
	m_pPage->SetLineWidth(pGState->getLineWidth());
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
		m_pPage->SetFillG(colToDbl(pColor->c[0]));
		break;
	}
	case csDeviceRGB:
	{
		m_pPage->SetFillRGB(colToDbl(pColor->c[0]), colToDbl(pColor->c[1]), colToDbl(pColor->c[2]));
		break;
	}
	case csDeviceCMYK:
	{
		m_pPage->SetFillCMYK(colToDbl(pColor->c[0]), colToDbl(pColor->c[1]), colToDbl(pColor->c[2]), colToDbl(pColor->c[3]));
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
		m_pPage->SetStrokeG(colToDbl(pColor->c[0]));
		break;
	}
	case csDeviceRGB:
	{
		m_pPage->SetStrokeRGB(colToDbl(pColor->c[0]), colToDbl(pColor->c[1]), colToDbl(pColor->c[2]));
		break;
	}
	case csDeviceCMYK:
	{
		m_pPage->SetStrokeCMYK(colToDbl(pColor->c[0]), colToDbl(pColor->c[1]), colToDbl(pColor->c[2]), colToDbl(pColor->c[3]));
		break;
	}
	default:
		break;
	}
}
void RedactOutputDev::updateRenderingIntent(GfxState *pGState)
{
	GfxRenderingIntent eRI = pGState->getRenderingIntent();
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
//----- update text state
void RedactOutputDev::updateFont(GfxState *pGState)
{

}
void RedactOutputDev::updateTextMat(GfxState *pGState)
{

}
void RedactOutputDev::updateCharSpace(GfxState *pGState)
{

}
void RedactOutputDev::updateRender(GfxState *pGState)
{

}
void RedactOutputDev::updateRise(GfxState *pGState)
{

}
void RedactOutputDev::updateWordSpace(GfxState *pGState)
{

}
void RedactOutputDev::updateHorizScaling(GfxState *pGState)
{

}
void RedactOutputDev::updateTextPos(GfxState *pGState)
{

}
void RedactOutputDev::updateTextShift(GfxState *pGState, double shift)
{

}
void RedactOutputDev::saveTextPos(GfxState *pGState)
{

}
void RedactOutputDev::restoreTextPos(GfxState *pGState)
{

}
}
