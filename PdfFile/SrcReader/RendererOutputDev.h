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
#ifndef _PDF_READER_RENDERER_OUTPUTDEV_H
#define _PDF_READER_RENDERER_OUTPUTDEV_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/AlphaMask.h"
#include "../../DesktopEditor/graphics/structures.h"
#include "../lib/xpdf/Gfx.h"
#include "../lib/xpdf/OutputDev.h"

#include "GfxClip.h"
#include <stack>

namespace PdfReader
{
	struct CType3FontMetrics;
	class CPdfFontList;
	//-------------------------------------------------------------------------------------------------------------------------------
	template <typename T>
	inline static double PDFCoordsToMM(T tX)
	{
		return ((double)tX / 72.0) * 25.4;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// RendererOutputDev
	//-------------------------------------------------------------------------------------------------------------------------------

	class RendererOutputDev : public OutputDev
	{
	public:
		RendererOutputDev(IRenderer* pRenderer, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList = NULL);
		virtual ~RendererOutputDev();

		virtual GBool upsideDown() override
		{
			return false;
		}
		virtual GBool useDrawChar() override
		{
			return true;
		}
		virtual GBool useTilingPatternFill() override
        {
			return true;
		}
		virtual GBool useFunctionalShadedFills()
		{
			return true;
		}
		virtual GBool useAxialShadedFills()
		{
			return true;
		}
		virtual GBool useRadialShadedFills()
		{
			return true;
		}
		virtual GBool useGouraundTriangleFills()
		{
			return true;
		}
		virtual GBool usePatchMeshFills()
		{
			return true;
		}
		virtual GBool useClipTo()
		{
			return false;//true;
		}
		virtual GBool interpretType3Chars() override
		{
			return true;
		}
		virtual GBool useFillAndStroke()
		{
			return true;
		}
		virtual GBool useSimpleTransparentGroup()
		{
			return true;
		}
		virtual GBool useSimpleTilingPatternFill()
		{
			if (NULL == m_pRenderer)
				return false;

			// TODO: m_pRenderer->GetAdditionalParam(L"TilingHtmlPattern");

			return false;
		}
		virtual GBool isStopped()
		{
			if (NULL != m_pbBreak)
				return *m_pbBreak;
			else
				return false;
		}
		//---------------------------------------------------------------------------------------------------------------------------
		virtual void startPage(int nPageIndex, GfxState *pGState) override;
		virtual void endPage() override;
		//----- Save/Restore GState
		virtual void saveState(GfxState *pGState) override;
		virtual void restoreState(GfxState *pGState) override;
		//----- GState parameter modification
		virtual void updateCTM(GfxState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32) override;
		virtual void updateLineDash(GfxState *pGState) override;
		virtual void updateFlatness(GfxState *pGState) override;
		virtual void updateLineJoin(GfxState *pGState) override;
		virtual void updateLineCap(GfxState *pGState) override;
		virtual void updateMiterLimit(GfxState *pGState) override;
		virtual void updateLineWidth(GfxState *pGState) override;
		virtual void updateStrokeAdjust(GfxState *pGState) override;
		virtual void updateFillColor(GfxState *pGState) override;
		virtual void updateStrokeColor(GfxState *pGState) override;
		virtual void updateBlendMode(GfxState *pGState) override;
		virtual void updateFillOpacity(GfxState *pGState) override;
		virtual void updateStrokeOpacity(GfxState *pGState) override;
		virtual void updateAll(GfxState *pGState) override;
		virtual void updateRender(GfxState *pGState) override;
		//----- Text parameter modification
		virtual void updateFont(GfxState *pGState) override;
		//----- Path drawing
		virtual void stroke(GfxState *pGState) override;
		virtual void fill(GfxState *pGState) override;
		virtual void eoFill(GfxState *pGState) override;
		virtual void FillStroke(GfxState *pGState);
		virtual void EoFillStroke(GfxState *pGState);
		virtual void tilingPatternFill(GfxState *pGState, Gfx *gfx, Object *pStream, int nPaintType, int nTilingType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep) override;
		virtual void StartTilingFill(GfxState *pGState);
		virtual void EndTilingFill();
		virtual GBool shadedFill(GfxState* pGState, GfxShading* shading) override;
		bool FunctionShadedFill(GfxState* pGState, GfxFunctionShading* pShading);
		bool AxialShadedFill(GfxState* pGState, GfxAxialShading* pShading);
		bool RadialShadedFill(GfxState* pGState, GfxRadialShading* pShading);
		bool GouraundTriangleFill(GfxState* pGState, const std::vector<GfxColor*>& colors, const std::vector<NSStructures::Point>& points);
		bool PatchMeshFill(GfxState* pGState, GfxPatch* pPatch, GfxPatchMeshShading* pShading);
		void StartShadedFill();
		void EndShadedFill();
		void StartTilingFillIteration();
		void EndTilingFillIteration();
		void StartSimpleTilingFill(GfxState* pGState, int  nX0, int nY0, int nX1, int nY1, double dStepX, double dStepY, double dXMin, double dYMin, double dXMax, double dYMax, double* pMatrix);
		void EndSimpleTilingFill();
		//----- Path clipping
		virtual void clip(GfxState *pGState) override;
		virtual void eoClip(GfxState *pGState) override;
		virtual void clipToStrokePath(GfxState *pGState) override;
		virtual void clipToPath(GfxState *pGState, GfxPath *pPath, double *pMatrix, bool bEO);
		//----- Text output
		virtual void endTextObject(GfxState *pGState) override;
		virtual void beginStringOp(GfxState *pGState) override;
		virtual void endStringOp(GfxState *pGState) override;
		virtual void drawString(GfxState *pGState, GString *seString) override;
		virtual void drawChar(GfxState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen) override;
		GBool beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen) override;
		void endType3Char(GfxState *pGState) override;
		//----- Additional functions
		virtual GBool beginMarkedContent(GfxState *state, GString *s) override;
		virtual GBool beginMCOShapes(GfxState *state, GString *s, Object *ref) override;
		virtual void endMarkedContent(GfxState *state) override;
		//----- Image output
		bool ReadImage(Aggplus::CImage* pImageRes, Object *pRef, Stream *pStream);
		virtual void drawImageMask(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		virtual void setSoftMaskFromImageMask(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		virtual void drawImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, GBool bInlineImg, GBool interpolate) override;
		virtual void drawMaskedImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
									 Object* pMaskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GBool bMaskInvert, GBool interpolate) override;
		virtual void drawSoftMaskedImage(GfxState *pGState, Gfx *gfx, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
										 Object *maskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GfxImageColorMap *pMaskColorMap, double *pMatte, GBool interpolate) override;
		//----- Transparency groups and SMasks
		virtual void beginTransparencyGroup(GfxState *pGState, double *pBBox, GfxColorSpace *pBlendingColorSpace, GBool bIsolated, GBool bKnockout, GBool bForSoftMask) override;
		virtual void endTransparencyGroup(GfxState *pGState) override;
		virtual void paintTransparencyGroup(GfxState *pGState, double *pBBox) override;
		virtual void setSoftMask(GfxState *pGState, double *pBBox, GBool bAlpha, Function *pTransferFunc, GfxColor *pBackdropColor) override;
		virtual void clearSoftMask(GfxState *pGState) override;
		//----- Additional functions for this device
		void NewPDF(XRef *pXref);
		void SetBreak(bool* pbBreak)
		{
			m_pbBreak = pbBreak;
		}

	private:
		struct GfxOutputState
		{
			GfxState* pGState;
			Aggplus::CSoftMask* pSoftMask;
			GfxClip* pClip;
			GfxTextClip* pTextClip;

			GfxOutputState() : pGState(NULL), pSoftMask(NULL), pClip(NULL), pTextClip(NULL) {}
			~GfxOutputState()
			{
				RELEASEOBJECT(pClip);
				RELEASEOBJECT(pTextClip);
			}
		};
		struct GfxOutputCS
		{
			bool bKnockout;
			GfxColorSpace* pBlendingCS;

			GfxOutputCS() : bKnockout(false), pBlendingCS(NULL) {}
		};

		void DoPath(GfxState *pGState, GfxPath *pPath, double dPageHeight, double *pCTM, GfxClipMatrix* pCTM2 = NULL);
		void ClipToText(const std::wstring& wsFontName, const std::wstring& wsFontPath, double dFontSize, int nFontStyle, double* pMatrix, const std::wstring& wsText, double dX, double dY, double dWidth = 0, double dHeight = 0, double dBaseLineOffset = 0);
		void AddClip(GfxState* pGState, GfxOutputState* pState, int nIndex);
		void AddTextClip(GfxState* pGState, GfxOutputState* pState = NULL);
		void UpdateAllClip(GfxState *pGState);
		void DoTransform(double *pMatrix, double *pdShiftX, double *pdShiftY, bool bText = false);
		CType3FontMetrics* GetType3FontMetrics(GfxFont* pFont);
	private:

		IRenderer*                    m_pRenderer;
		long                          m_lRendererType;
		double                        m_arrMatrix[6];
        NSFonts::IFontManager*        m_pFontManager;

		XRef*                         m_pXref; // Xref table for this PDF document
		CPdfFontList*                 m_pFontList;
		std::map<int, CType3FontMetrics*> m_mapType3Metrics;

		bool                         *m_pbBreak;         // External renderer stop

		std::deque<GfxOutputCS>       m_sCS;
		std::deque<GfxOutputState>    m_sStates;

		Aggplus::CSoftMask*           m_pSoftMask;
		bool                          m_bTiling;

        bool                          m_bDrawOnlyText; // Special option for html-renderer

	};
}

#endif // _PDF_READER_RENDERER_OUTPUTDEV_H
