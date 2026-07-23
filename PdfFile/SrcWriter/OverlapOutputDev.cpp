/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "OverlapOutputDev.h"

#include "../lib/xpdf/GfxFont.h"
#include "../lib/xpdf/XRef.h"

namespace PdfWriter
{
namespace
{
void TransformPoint(const double* pMatrix, double dUserX, double dUserY, double* pdDeviceX, double* pdDeviceY)
{
	*pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
	*pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
}
void BuildImageDevicePath(const double* pCTM, Aggplus::CGraphicsPath& oPath)
{
	oPath.Reset();

	double dX, dY;
	oPath.StartFigure();
	TransformPoint(pCTM, 0, 0, &dX, &dY);
	oPath.MoveTo(dX, dY);
	TransformPoint(pCTM, 0, 1, &dX, &dY);
	oPath.LineTo(dX, dY);
	TransformPoint(pCTM, 1, 1, &dX, &dY);
	oPath.LineTo(dX, dY);
	TransformPoint(pCTM, 1, 0, &dX, &dY);
	oPath.LineTo(dX, dY);
	oPath.CloseFigure();
}
void BuildDeviceSpacePath(GfxPath* pPath, const double* pCTM, bool bEoFill, Aggplus::CGraphicsPath& oPath)
{
	oPath.Reset();
	if (bEoFill)
		oPath.SetRuler(true);

	for (int nSubPathIndex = 0, nSubPathCount = pPath->getNumSubpaths(); nSubPathIndex < nSubPathCount; ++nSubPathIndex)
	{
		GfxSubpath* pSubpath = pPath->getSubpath(nSubPathIndex);
		int nPointsCount = pSubpath->getNumPoints();

		oPath.StartFigure();

		double dX = pSubpath->getX(0), dY = pSubpath->getY(0);
		TransformPoint(pCTM, dX, dY, &dX, &dY);
		oPath.MoveTo(dX, dY);

		int nCurPointIndex = 1;
		while (nCurPointIndex < nPointsCount)
		{
			if (pSubpath->getCurve(nCurPointIndex))
			{
				double dX1 = pSubpath->getX(nCurPointIndex);
				double dY1 = pSubpath->getY(nCurPointIndex);
				TransformPoint(pCTM, dX1, dY1, &dX1, &dY1);

				double dX2 = pSubpath->getX(nCurPointIndex + 1);
				double dY2 = pSubpath->getY(nCurPointIndex + 1);
				TransformPoint(pCTM, dX2, dY2, &dX2, &dY2);

				double dX3 = pSubpath->getX(nCurPointIndex + 2);
				double dY3 = pSubpath->getY(nCurPointIndex + 2);
				TransformPoint(pCTM, dX3, dY3, &dX3, &dY3);

				oPath.CurveTo(dX1, dY1, dX2, dY2, dX3, dY3);
				nCurPointIndex += 3;
			}
			else
			{
				dX = pSubpath->getX(nCurPointIndex);
				dY = pSubpath->getY(nCurPointIndex);
				TransformPoint(pCTM, dX, dY, &dX, &dY);
				oPath.LineTo(dX, dY);
				++nCurPointIndex;
			}
		}
		oPath.CloseFigure();
	}
}
bool IsOpaqueGraphicsItem(const SOverlapPaintedItem& oItem)
{
	const double c_dOpaqueEps = 1e-6;
	if (oItem.m_dFillAlpha < 1.0 - c_dOpaqueEps)
		return false;
	if (oItem.m_bHasSoftMask)
		return false;
	if (oItem.m_nBlendMode != gfxBlendNormal)
		return false;
	return true;
}
Aggplus::CGraphicsPath* IntersectWithClip(Aggplus::CGraphicsPath* pRawPath, Aggplus::CGraphicsPath* pCurrentClip)
{
	if (!pCurrentClip)
		return pRawPath;

	Aggplus::CGraphicsPath* pResult = new(std::nothrow) Aggplus::CGraphicsPath();
	if (!pResult)
		return pRawPath;

	*pResult = Aggplus::CalcBooleanOperation(*pRawPath, *pCurrentClip, Aggplus::BooleanOpType::Intersection);
	delete pRawPath;
	return pResult;
}
}
//----- SOverlapPaintedItem -----
SOverlapPaintedItem::SOverlapPaintedItem()
	: m_eKind(EOverlapPaintKind::Text)
	, m_nTextIndex(-1)
	, m_pDevicePath(NULL)
	, m_bEoFill(false)
	, m_dFillAlpha(1)
	, m_bHasSoftMask(gFalse)
	, m_nBlendMode(gfxBlendNormal)
{
}
SOverlapPaintedItem::~SOverlapPaintedItem()
{
	RELEASEOBJECT(m_pDevicePath);
}
//----- SOverlapGraphicsResult -----
SOverlapGraphicsResult::SOverlapGraphicsResult()
	: m_nGraphicsIndex(-1)
	, m_eDecision(EOverlapGraphicsDecision::Unmodified)
	, m_pReplacementPath(NULL)
{
}
SOverlapGraphicsResult::SOverlapGraphicsResult(SOverlapGraphicsResult&& other) noexcept
	: m_nGraphicsIndex(other.m_nGraphicsIndex)
	, m_eDecision(other.m_eDecision)
	, m_pReplacementPath(other.m_pReplacementPath)
{
	other.m_pReplacementPath = NULL;
}
SOverlapGraphicsResult& SOverlapGraphicsResult::operator=(SOverlapGraphicsResult&& other) noexcept
{
	if (this != &other)
	{
		RELEASEOBJECT(m_pReplacementPath);
		m_nGraphicsIndex   = other.m_nGraphicsIndex;
		m_eDecision        = other.m_eDecision;
		m_pReplacementPath = other.m_pReplacementPath;
		other.m_pReplacementPath = NULL;
	}
	return *this;
}
SOverlapGraphicsResult::~SOverlapGraphicsResult()
{
	RELEASEOBJECT(m_pReplacementPath);
}
//----- OverlapOutputDev -----
//----- constructor/destructor -----
OverlapOutputDev::OverlapOutputDev(bool bTextOnly)
	: m_pXref(NULL)
	, m_pCurrentClip(NULL)
	, m_bTextOnly(bTextOnly)
	, m_nTextCounter(0)
	, m_nGraphicsCounter(0)
	, m_nFormDepth(0)
	, m_nFormCounter(0)
{
}
OverlapOutputDev::~OverlapOutputDev()
{
	for (size_t i = 0; i < m_arrPainted.size(); ++i)
		delete m_arrPainted[i];
	m_arrPainted.clear();
	for (size_t i = 0; i < m_arrClipPool.size(); ++i)
		delete m_arrClipPool[i];
	m_arrClipPool.clear();
}
void OverlapOutputDev::NewPDF(XRef* pXref)
{
	m_pXref = pXref;
}
//----- initialization and control -----
void OverlapOutputDev::startPage(int nPageIndex, GfxState* pGState)
{
	for (size_t i = 0; i < m_arrPainted.size(); ++i)
		delete m_arrPainted[i];
	m_arrPainted.clear();
	m_sStates.clear();

	for (size_t i = 0; i < m_arrClipPool.size(); ++i)
		delete m_arrClipPool[i];
	m_arrClipPool.clear();
	m_pCurrentClip = NULL;

	m_nTextCounter     = 0;
	m_nGraphicsCounter = 0;
	m_nFormDepth       = 0;
	m_nFormCounter     = 0;
	m_arrHiddenText.clear();
	m_arrFormTextCounts.clear();
	m_arrFormGraphicsCounts.clear();
}
void OverlapOutputDev::endPage()
{
	ComputeOverlaps();
}
//----- save/restore graphics state -----
void OverlapOutputDev::saveState(GfxState* pGState)
{
	SOverlapState oState;
	oState.m_pSavedClip = m_pCurrentClip;
	m_sStates.push_back(oState);
}
void OverlapOutputDev::restoreState(GfxState* pGState)
{
	if (m_sStates.empty())
		return; // Unbalanced q/Q
	m_pCurrentClip = m_sStates.back().m_pSavedClip;
	m_sStates.pop_back();
}
//----- path painting -----
void OverlapOutputDev::fill(GfxState* pGState)
{
	SOverlapPaintedItem* pItem = new(std::nothrow) SOverlapPaintedItem();
	if (!pItem)
		return;

	pItem->m_eKind          = EOverlapPaintKind::Graphics;
	pItem->m_nGraphicsIndex = m_nGraphicsCounter++;
	pItem->m_bEoFill        = false;

	Aggplus::CGraphicsPath* pRawPath = new(std::nothrow) Aggplus::CGraphicsPath();
	if (pRawPath)
	{
		BuildDeviceSpacePath(pGState->getPath(), pGState->getCTM(), false, *pRawPath);
		pItem->m_pDevicePath = IntersectWithClip(pRawPath, m_pCurrentClip);
	}

	pItem->m_dFillAlpha   = pGState->getFillOpacity();
	pItem->m_bHasSoftMask = gFalse; // pGState->getSoftMask() ? gTrue : gFalse; TODO
	pItem->m_nBlendMode   = pGState->getBlendMode();

	MarkHiddenTextUnderGraphics(pItem);

	m_arrPainted.push_back(pItem);
}
void OverlapOutputDev::eoFill(GfxState* pGState)
{
	SOverlapPaintedItem* pItem = new(std::nothrow) SOverlapPaintedItem();
	if (!pItem)
		return;

	pItem->m_eKind          = EOverlapPaintKind::Graphics;
	pItem->m_nGraphicsIndex = m_nGraphicsCounter++;
	pItem->m_bEoFill        = true;

	Aggplus::CGraphicsPath* pRawPath = new(std::nothrow) Aggplus::CGraphicsPath();
	if (pRawPath)
	{
		BuildDeviceSpacePath(pGState->getPath(), pGState->getCTM(), true, *pRawPath);
		pItem->m_pDevicePath = IntersectWithClip(pRawPath, m_pCurrentClip);
	}

	pItem->m_dFillAlpha   = pGState->getFillOpacity();
	pItem->m_bHasSoftMask = gFalse; // pGState->getSoftMask() ? gTrue : gFalse; TODO
	pItem->m_nBlendMode   = pGState->getBlendMode();

	MarkHiddenTextUnderGraphics(pItem);

	m_arrPainted.push_back(pItem);
}
//----- image drawing -----
void OverlapOutputDev::drawImage(GfxState* pGState, Gfx* gfx, Object* pRef, Stream* pStream, int nWidth, int nHeight, GfxImageColorMap* pColorMap, int* pMaskColors, GBool bInlineImg, GBool interpolate)
{
	SOverlapPaintedItem* pItem = new(std::nothrow) SOverlapPaintedItem();
	if (!pItem)
		return;

	pItem->m_eKind          = EOverlapPaintKind::Graphics;
	pItem->m_nGraphicsIndex = m_nGraphicsCounter++;
	pItem->m_bEoFill        = false;

	Aggplus::CGraphicsPath* pRawPath = new(std::nothrow) Aggplus::CGraphicsPath();
	if (pRawPath)
	{
		BuildImageDevicePath(pGState->getCTM(), *pRawPath);
		pItem->m_pDevicePath = IntersectWithClip(pRawPath, m_pCurrentClip);
	}

	pItem->m_dFillAlpha   = pGState->getFillOpacity();
	pItem->m_bHasSoftMask = pMaskColors ? gTrue : gFalse; // color-key masked image is not fully opaque
	pItem->m_nBlendMode   = pGState->getBlendMode();

	MarkHiddenTextUnderGraphics(pItem);

	m_arrPainted.push_back(pItem);
}
//----- path clipping -----
void OverlapOutputDev::clip(GfxState* pGState)
{
	if (m_sStates.empty())
		return;

	if (!m_sStates.back().m_pClip)
		m_sStates.back().m_pClip = new GfxClip();
	int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeWinding;
	m_sStates.back().m_pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);

	Aggplus::CGraphicsPath* pShape = new(std::nothrow) Aggplus::CGraphicsPath();
	if (!pShape)
		return;
	BuildDeviceSpacePath(pGState->getPath(), pGState->getCTM(), false, *pShape);

	Aggplus::CGraphicsPath* pNewClip = pShape;
	if (m_pCurrentClip)
	{
		Aggplus::CGraphicsPath* pIntersected = new(std::nothrow) Aggplus::CGraphicsPath();
		if (pIntersected)
		{
			*pIntersected = Aggplus::CalcBooleanOperation(*m_pCurrentClip, *pShape, Aggplus::BooleanOpType::Intersection);
			delete pShape;
			pNewClip = pIntersected;
		}
	}

	m_arrClipPool.push_back(pNewClip);
	m_pCurrentClip = pNewClip;
}
void OverlapOutputDev::eoClip(GfxState* pGState)
{
	if (m_sStates.empty())
		return;

	if (!m_sStates.back().m_pClip)
		m_sStates.back().m_pClip = new GfxClip();
	int nClipFlag = c_nClipRegionIntersect | c_nClipRegionTypeEvenOdd;
	m_sStates.back().m_pClip->AddPath(pGState->getPath(), pGState->getCTM(), nClipFlag);

	Aggplus::CGraphicsPath* pShape = new(std::nothrow) Aggplus::CGraphicsPath();
	if (!pShape)
		return;
	BuildDeviceSpacePath(pGState->getPath(), pGState->getCTM(), true, *pShape);

	Aggplus::CGraphicsPath* pNewClip = pShape;
	if (m_pCurrentClip)
	{
		Aggplus::CGraphicsPath* pIntersected = new(std::nothrow) Aggplus::CGraphicsPath();
		if (pIntersected)
		{
			*pIntersected = Aggplus::CalcBooleanOperation(*m_pCurrentClip, *pShape, Aggplus::BooleanOpType::Intersection);
			delete pShape;
			pNewClip = pIntersected;
		}
	}

	m_arrClipPool.push_back(pNewClip);
	m_pCurrentClip = pNewClip;
}
//----- text drawing -----
void OverlapOutputDev::drawChar(GfxState* pGState, double dX, double dY, double dDx, double dDy,
								 double dOriginX, double dOriginY, CharCode nCode, int nBytesCount,
								 Unicode* pUnicode, int nUnicodeLen)
{
	int nCurrentIndex = m_nTextCounter++;

	GfxFont* pFont = pGState->getFont();
	double dAscent  = pFont ? pFont->getAscent()  : 0.718;
	double dDescent = pFont ? pFont->getDescent() : -0.207;
	double dFontSize = pGState->getFontSize();
	double dTop    = dY + dAscent  * dFontSize;
	double dBottom = dY + dDescent * dFontSize;
	double dLeft   = dX;
	double dRight  = dX + dDx;

	SOverlapPaintedItem* pItem = new(std::nothrow) SOverlapPaintedItem();
	if (!pItem)
		return;

	pItem->m_eKind      = EOverlapPaintKind::Text;
	pItem->m_nTextIndex = nCurrentIndex;

	double arrUserX[5] = { dLeft,  dRight, dLeft,   dRight,  (dLeft + dRight) / 2 };
	double arrUserY[5] = { dTop,   dTop,   dBottom, dBottom, (dTop + dBottom) / 2 };

	for (int i = 0; i < 5; ++i)
		pGState->transform(arrUserX[i], arrUserY[i], &pItem->m_arrTextPoints[i][0], &pItem->m_arrTextPoints[i][1]);

	m_arrPainted.push_back(pItem);

	const double c_dOpaqueEps = 1e-6;
	double* pCTM = pGState->getCTM();
	double* pTm  = pGState->getTextMat();
	double det   = pCTM[0] * pCTM[3] - pCTM[1] * pCTM[2];
	double detT  = pTm[0]  * pTm[3]  - pTm[1]  * pTm[2];
	int nRender = pGState->getRender();
	// PDF render mode: 0=fill,1=stroke,2=fill+stroke,3=invisible,4=fill+clip,5=stroke+clip,6=fill+stroke+clip,7=clip only
	bool bUsesFill   = (nRender == 0 || nRender == 2 || nRender == 4 || nRender == 6);
	bool bUsesStroke = (nRender == 1 || nRender == 2 || nRender == 5 || nRender == 6);

	bool bInvisibleRender = (nRender == 3 || nRender == 7);
	bool bZeroFill   = bUsesFill   && fabs(pGState->getFillOpacity())   <= c_dOpaqueEps;
	bool bZeroStroke = bUsesStroke && fabs(pGState->getStrokeOpacity()) <= c_dOpaqueEps;
	bool bZeroPaint  = (bUsesFill && bUsesStroke) ? (bZeroFill && bZeroStroke) : (bZeroFill || bZeroStroke);

	if (bInvisibleRender || bZeroPaint || fabs(dFontSize) <= c_dOpaqueEps || fabs(pGState->getHorizScaling()) <= c_dOpaqueEps || fabs(det) <= c_dOpaqueEps || fabs(detT) <= c_dOpaqueEps)
	{
		m_arrHiddenText.insert(pItem->m_nTextIndex);
		return;
	}

	double dClipXMin, dClipYMin, dClipXMax, dClipYMax;
	pGState->getClipBBox(&dClipXMin, &dClipYMin, &dClipXMax, &dClipYMax);

	double dGlyphXMin = pItem->m_arrTextPoints[0][0], dGlyphXMax = dGlyphXMin;
	double dGlyphYMin = pItem->m_arrTextPoints[0][1], dGlyphYMax = dGlyphYMin;
	for (int p = 1; p < 4; ++p)
	{
		dGlyphXMin = std::min(dGlyphXMin, pItem->m_arrTextPoints[p][0]);
		dGlyphXMax = std::max(dGlyphXMax, pItem->m_arrTextPoints[p][0]);
		dGlyphYMin = std::min(dGlyphYMin, pItem->m_arrTextPoints[p][1]);
		dGlyphYMax = std::max(dGlyphYMax, pItem->m_arrTextPoints[p][1]);
	}
	if (dGlyphXMax < dClipXMin || dGlyphXMin > dClipXMax || dGlyphYMax < dClipYMin || dGlyphYMin > dClipYMax)
		m_arrHiddenText.insert(pItem->m_nTextIndex);
}
//----- additional -----
void OverlapOutputDev::setExtGState(GfxState *pGState, Object* pExtGState, const char* name)
{
	Object obj2;
	double dOpac = 1;
	if (!pExtGState->dictLookup("BM", &obj2)->isNull())
	{
		GfxBlendMode mode;
		if (pGState->parseBlendMode(&obj2, &mode))
			pGState->setBlendMode(mode);
	}
	obj2.free();
	if (pExtGState->dictLookup("ca", &obj2)->isNum())
	{
		dOpac = obj2.getNum();
		pGState->setFillOpacity(dOpac < 0 ? 0 : dOpac > 1 ? 1 : dOpac);
	}
	obj2.free();
	if (pExtGState->dictLookup("CA", &obj2)->isNum())
	{
		dOpac = obj2.getNum();
		pGState->setStrokeOpacity(dOpac < 0 ? 0 : dOpac > 1 ? 1 : dOpac);
	}
	obj2.free();
}
//----- form XObjects -----
void OverlapOutputDev::drawForm(GfxState* pGState, Gfx* gfx, Ref id, const char* name)
{
	static const int c_nMaxFormDepth = 32;
	if (m_nFormDepth >= c_nMaxFormDepth)
		return;

	CObjectGuard oForm, oRef;
	oRef->initRef(id.num, id.gen);
	m_pXref->fetch(id.num, id.gen, oForm.get());

	int nFormIndex = m_nFormCounter++;
	int nStartText = m_nTextCounter;
	int nStartGraphics = m_nGraphicsCounter;

	++m_nFormDepth;
	gfx->doForm(oRef.get(), oForm.get());
	--m_nFormDepth;

	int nDelta = m_nTextCounter - nStartText;
	if ((size_t)nFormIndex >= m_arrFormTextCounts.size())
		m_arrFormTextCounts.resize(nFormIndex + 1, 0);
	m_arrFormTextCounts[nFormIndex] = nDelta;

	int nGraphicsDelta = m_nGraphicsCounter - nStartGraphics;
	if ((size_t)nFormIndex >= m_arrFormGraphicsCounts.size())
		m_arrFormGraphicsCounts.resize(nFormIndex + 1, 0);
	m_arrFormGraphicsCounts[nFormIndex] = nGraphicsDelta;
}
//----- overlay graphics -----
void OverlapOutputDev::MarkHiddenTextUnderGraphics(const SOverlapPaintedItem* pGraphicsItem)
{
	if (!pGraphicsItem->m_pDevicePath || !IsOpaqueGraphicsItem(*pGraphicsItem))
		return;

	for (size_t i = 0; i < m_arrPainted.size(); ++i)
	{
		SOverlapPaintedItem* pItem = m_arrPainted[i];
		if (pItem->m_eKind != EOverlapPaintKind::Text)
			continue;
		if (m_arrHiddenText.find(pItem->m_nTextIndex) != m_arrHiddenText.end())
			continue;

		for (int p = 0; p < 5; ++p)
		{
			if (pGraphicsItem->m_pDevicePath->IsPointInPath(pItem->m_arrTextPoints[p][0], pItem->m_arrTextPoints[p][1]))
			{
				m_arrHiddenText.insert(pItem->m_nTextIndex);
				break;
			}
		}
	}
}
void OverlapOutputDev::ComputeOverlaps()
{
	m_arrHiddenGraphics.clear();

	for (size_t i = 0; i < m_arrPainted.size(); ++i)
	{
		if (m_arrPainted[i]->m_eKind != EOverlapPaintKind::Graphics)
			continue;

		SOverlapGraphicsResult oResult;
		oResult.m_nGraphicsIndex = m_arrPainted[i]->m_nGraphicsIndex;
		oResult.m_eDecision      = EOverlapGraphicsDecision::Unmodified;
		oResult.m_pReplacementPath = NULL;
		m_arrHiddenGraphics.push_back(std::move(oResult));
	}
}
std::vector<SOverlapGraphicsResult> OverlapOutputDev::GetHiddenGraphics()
{
	return std::move(m_arrHiddenGraphics);
}
}
