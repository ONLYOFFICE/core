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
#ifndef _PDF_READER_REDACT_OUTPUTDEV_H
#define _PDF_READER_REDACT_OUTPUTDEV_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/AlphaMask.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../DesktopEditor/graphics/structures.h"

#include "GfxClip.h"
#include <stack>

#include "../lib/xpdf/Gfx.h"
#include "../lib/xpdf/OutputDev.h"

namespace PdfReader
{
	class RedactOutputDev : public OutputDev
	{
	public:
		RedactOutputDev(IRenderer* pRenderer, NSFonts::IFontManager* pFontManager);
		virtual ~RedactOutputDev();

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
		virtual  GBool usePatchMeshFills()
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
		virtual GBool  useSimpleTransparentGroup()
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
		//----- Изменение параметров в GState
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
		//----- Изменение текстовых параметров
		virtual void updateFont(GfxState *pGState) override;
		//----- Рисование Path
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
		//----- Вывод текста
		virtual void endTextObject(GfxState *pGState) override;
		virtual void beginStringOp(GfxState *pGState) override;
		virtual void endStringOp(GfxState *pGState) override;
		virtual void drawString(GfxState *pGState, GString *seString) override;
		virtual void drawChar(GfxState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen) override;
		GBool beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen) override;
		void endType3Char(GfxState *pGState) override;
		//----- Дополнительные функции
		virtual GBool beginMarkedContent(GfxState *state, GString *s) override;
		virtual GBool beginMCOShapes(GfxState *state, GString *s, Object *ref) override;
		virtual void endMarkedContent(GfxState *state) override;
		//----- Вывод картинок
		bool ReadImage(Aggplus::CImage* pImageRes, Object *pRef, Stream *pStream);
		virtual void drawImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		virtual void setSoftMaskFromImageMask(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GBool bInvert, GBool bInlineImage, GBool interpolate) override;
		virtual void drawImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap, int *pMaskColors, GBool bInlineImg, GBool interpolate) override;
		virtual void drawMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
									 Object* pMaskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GBool bMaskInvert, GBool interpolate) override;
		virtual void drawSoftMaskedImage(GfxState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GfxImageColorMap *pColorMap,
										 Object *maskRef, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GfxImageColorMap *pMaskColorMap, double *pMatte, GBool interpolate) override;
		//----- Transparency groups и SMasks
		virtual void beginTransparencyGroup(GfxState *pGState, double *pBBox, GfxColorSpace *pBlendingColorSpace, GBool bIsolated, GBool bKnockout, GBool bForSoftMask) override;
		virtual void endTransparencyGroup(GfxState *pGState) override;
		virtual void paintTransparencyGroup(GfxState *pGState, double *pBBox) override;
		virtual void setSoftMask(GfxState *pGState, double *pBBox, GBool bAlpha, Function *pTransferFunc, GfxColor *pBackdropColor) override;
		virtual void clearSoftMask(GfxState *pGState) override;
		//----- Дополнительные функции для данного устройства

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
	private:

		IRenderer*                    m_pRenderer;
		long                          m_lRendererType;
		double                        m_arrMatrix[6];
		NSFonts::IFontManager*        m_pFontManager;

		XRef*                         m_pXref; // Таблица Xref для данного PDF-документа

		bool                         *m_pbBreak;         // Внешняя остановка рендерера

		std::deque<GfxOutputCS>       m_sCS;
		std::deque<GfxOutputState>    m_sStates;

		Aggplus::CSoftMask*           m_pSoftMask;
		bool                          m_bTiling;

		bool                          m_bDrawOnlyText; // Special option for html-renderer

	};
}

#endif // _PDF_READER_REDACT_OUTPUTDEV_H
